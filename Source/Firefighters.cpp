#include "Firefighters.hpp"
#include "car_despawn_status.hpp"
#include "Car.hpp"
#include "CarAI.hpp"
#include "CarPhysics.hpp"
#include "Char_Pool.hpp"
#include "Game.hpp"
#include "CarChaseTaskTable.hpp"
#include "PathFinder.hpp"
#include "debug.hpp"

// Forward declarations: the functions below are in address order
EXTERN_GLOBAL(Fix16, kFpThree_67D384);

DEFINE_GLOBAL(FirefighterPool*, gFirefighterPool_54, 0x67D4C0);
DEFINE_GLOBAL_INIT(Fix16, kFpHalf_67D1F0, Fix16(0.5f), 0x67D1F0);
DEFINE_GLOBAL_INIT(Fix16, kFpZero_67D378, Fix16(0), 0x67D378);

DEFINE_GLOBAL_INIT(Ang16, kAng0_67D4B0, Ang16(0), 0x67D4B0);
DEFINE_GLOBAL_INIT(Ang16, kAng90_67D208, Ang16(360), 0x67D208);
DEFINE_GLOBAL_INIT(Ang16, kAng180_67D2D6, Ang16(720), 0x67D2D6);
DEFINE_GLOBAL_INIT(Ang16, kAng270_67D2FC, Ang16(1080), 0x67D2FC);

// https://decomp.me/scratch/X8G5z mov 0x4(%esp) vs mov (%eax)
MATCH_FUNC(0x4a7fc0)
bool Firefighter::CheckAbortConditions()
{
    if (!mCar->mPhysics)
    {
        Firefighter::deinit();
        return false;
    }

    if (mCar->mPhysics->get_car_velocity() == kFpZero_67D378)
    {
        if (++mNextStateTimer >= 1000)
        {
            Firefighter::deinit();
            return false;
        }
    }
    else
    {
        mNextStateTimer = 0;
    }

    if (mCar && mCar->mLastSeenTimer > 5000)
    {
        mState = firefighter_state::abort_6;
    }
    if (mCar && mCar->mDamage > 32000)
    {
        mState = firefighter_state::abort_6;
    }

    if (mTargetCar)
    {
        if (mTargetCar->mDespawnStatus == car_despawn_status::despawned_6 || mTargetCar->mDespawnStatus == car_despawn_status::deactivated_7 ||
            mTargetCar->IsDespawning() || mTargetCar->IsMarkedForDespawn())
        {
            if (mPed)
            {
                mPed->SetObjective(objectives_enum::no_obj_0);
            }
            mState = firefighter_state::abort_6;
        }
    }

    if (mPed && mPed->bHasGameObject_403B70())
    {
        mState = firefighter_state::abort_6;
    }

    if (mState != firefighter_state::drive_to_fire_2)
    {
        if (mState > firefighter_state::drive_to_fire_2 && mState <= firefighter_state::put_out_fire_4)
        {
            if (mTargetCar)
            {
                if (!mTargetCar->mAttachments.FindFirstFireExplosion())
                {
                    mState = firefighter_state::finished_5;
                }
            }
        }
    }
    else
    {
        if (mTargetCar)
        {
            if (mTargetCar->mDespawnStatus != car_despawn_status::despawned_6 && !mTargetCar->IsDespawning())
            {
                if (mPed)
                {
                    if (mTargetCar->GetX().ToUInt8() != mPed->mObjectiveTargetX.ToUInt8() ||
                        mTargetCar->GetY().ToUInt8() != mPed->mObjectiveTargetY.ToUInt8())
                    {
                        mPed->SetObjective(objectives_enum::goto_area_in_car_14);
                        mPed->mObjectiveTargetX = Fix16(mTargetCar->GetX().ToUInt8());
                        mPed->mObjectiveTargetY = Fix16(mTargetCar->GetY().ToUInt8());
                        mPed->mObjectiveTargetZ = Fix16(mTargetCar->GetZ().ToUInt8());
                    }
                }
            }
        }
    }

    if (mState == firefighter_state::abort_6)
    {
        Firefighter::deinit();
    }
    return true;
}

MATCH_FUNC(0x4a81a0)
void Firefighter::deinit()
{
    Car* pCar = this->mCar;
    if (pCar)
    {
        CarChaseTask* pRoute = pCar->mpChaseTask;
        if (pRoute)
        {
            gCarChaseTaskTable->FreeEntry(pRoute);
            this->mCar->mpChaseTask = NULL;
        }
    }
    Ped* pPed = this->mPed;
    if (pPed)
    {
        pPed->SetObjective(objectives_enum::no_obj_0);
    }

    Car* pTargetCar = this->mTargetCar;
    this->mState = firefighter_state::finished_5;
    if (pTargetCar)
    {
        pTargetCar->mAttachments.CleanupSpriteList();
    }
}

MATCH_FUNC(0x4a81f0)
void Firefighter::Update()
{
    if (!mbActive)
    {
        return;
    }

    switch (mState)
    {
        case firefighter_state::spawn_truck_1:
            if (mTargetCar && mTargetCar->mDespawnStatus != car_despawn_status::despawned_6 &&
                !mTargetCar->IsDespawning() && gCarManager_6C->CanAllocateCarOfKind(car_kind::firefighter_5))
            {
                mCar = gCarManager_6C->SpawnCarAtRoadDirection(car_model_enum::FIRETRUK, mXpos, mYpos, mZpos);
                if (mCar)
                {
                    mCar->IncrementCarStats(car_kind::firefighter_5);
                    mCar->SetDriverPedType(ped_type::special_ped_4);
                    Car* pCar = mCar;
                    pCar->AllocateAI_421510();
                    mCar->mAI->SetCar(mCar);

                    mPed = gPedManager->AllocatePed();
                    mPed->SetPedType(ped_type::special_ped_4);
                    mPed->set_occupation(ped_ocupation_enum::fireman);
                    mPed->SpawnPedInCar(mCar);
                    mPed->SetObjective(objectives_enum::goto_area_in_car_14);
                    mPed->mObjectiveTargetX = Fix16(mTargetCar->GetX().ToUInt8());
                    mPed->mObjectiveTargetY = Fix16(mTargetCar->GetY().ToUInt8());
                    mPed->mObjectiveTargetZ = Fix16(mTargetCar->GetZ().ToUInt8());
                    mPed->mBf21C.bUseCarWeapon = 1;
                    mCar->ActivateEmergencyLights();
                    mCar->SetupCarPhysicsAndSpriteBindingWithTrailer();
                    mPed = mCar->GetDriver();
                    mNextStateTimer = 0;
                    mState = firefighter_state::drive_to_fire_2;
                    break;
                }

                if (++mNextStateTimer >= 50)
                {
                    mState = firefighter_state::abort_6;
                }
                break;
            }
            mState = firefighter_state::abort_6;
            break;

        case firefighter_state::drive_to_fire_2:
            if (!mCar->mPhysics)
            {
                mState = firefighter_state::abort_6;
            }
            else if (CheckAbortConditions())
            {
                if (Fix16::MaxAbsDistance(mCar->GetX(),
                                          mCar->GetY(),
                                          mTargetCar->GetX(),
                                          mTargetCar->GetY()) < kFpThree_67D384 &&
                    mNextStateTimer > 100)
                {
                    mState = firefighter_state::arrived_3;
                }

                if (mPed)
                {
                    switch (mPed->GetObjectiveStatus())
                    {
                        case objective_status::passed_1:
                            mState = firefighter_state::arrived_3;
                            break;
                        case objective_status::failed_2:
                            mState = firefighter_state::abort_6;
                            break;
                    }
                }
                else
                {
                    mState = firefighter_state::abort_6;
                }
            }
            break;

        case firefighter_state::arrived_3:
            if (CheckAbortConditions() && mPed && mState == firefighter_state::arrived_3)
            {
                mPed->SetObjective(objectives_enum::turret_put_out_car_fire_60);
                mPed->SetTargetObjectiveCar(mTargetCar);
                mState = firefighter_state::put_out_fire_4;
            }
            break;

        case firefighter_state::put_out_fire_4:
            if (CheckAbortConditions())
            {
                switch (mPed->GetObjectiveStatus())
                {
                    case objective_status::passed_1:
                        mPed->SetObjective(objectives_enum::no_obj_0);
                        mState = firefighter_state::finished_5;
                        break;
                    case objective_status::failed_2:
                        mPed->SetObjective(objectives_enum::no_obj_0);
                        mState = firefighter_state::finished_5;
                        break;
                }
            }
            break;

        case firefighter_state::finished_5:
            if (mCar && mCar->mDespawnStatus != car_despawn_status::despawned_6 && !mCar->IsDespawning() &&
                !mCar->IsMarkedForDespawn())
            {
                if (mCar->mDriver)
                {
                    mCar->mDriver->mBf21C.bDriveAggressively = 1;
                }
                mCar->SetDriverPedType(ped_type::dummy_3);
                mCar->InitCarAIControl();
                mCar->ResumeAIDriving();
                mCar->DeactivateEmergencyLights();
                if (mCar->mDriver)
                {
                    mCar->mDriver->SetTargetObjectiveCar(NULL);
                    mCar->mDriver->SetObjective(objectives_enum::no_obj_0);
                    mCar->mDriver->ClearAttacking_403A40();
                }
            }
            Reset();
            break;
    }

    if (mState == firefighter_state::abort_6)
    {
        deinit();
    }
}

MATCH_FUNC(0x4a85c0)
void Firefighter::init()
{
    Clear();
}

MATCH_FUNC(0x4a85e0)
void Firefighter::Reset()
{
    this->mTargetCar = NULL;
    this->mState = firefighter_state::idle_0;
    this->mbActive = 0;
    this->mCar = NULL;
}

MATCH_FUNC(0x4a85f0)
void FirefighterPool::FireEnginesService()
{
    Firefighter* pFirefighter = mFirefighters;
    if (!bSkip_fire_engines)
    {
        for (u32 i = 0; i < 2; i++)
        {
            if (pFirefighter->mbActive)
            {
                pFirefighter->Update();
            }
            pFirefighter++;
        }
    }
}

MATCH_FUNC(0x4a8620)
Firefighter* FirefighterPool::DispatchFirefighters(Car* pCar, Fix16 xpos, Fix16 ypos, Fix16 zpos)
{
    if (bSkip_fire_engines)
    {
        return NULL;
    }
    u8 xpos_int = xpos.ToUInt8();
    u8 ypos_int = ypos.ToUInt8();
    u8 zpos_int = (zpos + kFpHalf_67D1F0).ToUInt8();
    if (gPathFinder->FindNearbyTileMatchingSlopeType(ROAD, &xpos_int, &ypos_int, &zpos_int, false) != 1)
    {
        return NULL;
    }
    s8 arrow_count;
    if (pCar->CountConsecutiveArrowBlocks(kAng0_67D4B0, &arrow_count, xpos_int, ypos_int) < 0 || arrow_count <= 1)
    {
        if (pCar->CountConsecutiveArrowBlocks(kAng90_67D208, &arrow_count, xpos_int, ypos_int) < 0 || arrow_count <= 1)
        {
            if (pCar->CountConsecutiveArrowBlocks(kAng180_67D2D6, &arrow_count, xpos_int, ypos_int) < 0 || arrow_count <= 1)
            {
                if (pCar->CountConsecutiveArrowBlocks(kAng270_67D2FC, &arrow_count, xpos_int, ypos_int) < 0 || arrow_count <= 1)
                {
                    return NULL;
                }
            }
        }
    }
    Firefighter* pNewFireFighter = FirefighterPool::New28();
    if (!pNewFireFighter)
    {
        return NULL;
    }
    pNewFireFighter->mId = mCount;
    ++mCount;
    pNewFireFighter->mXpos = kFpHalf_67D1F0 + Fix16(xpos_int);
    pNewFireFighter->mYpos = kFpHalf_67D1F0 + Fix16(ypos_int);
    pNewFireFighter->mZpos = Fix16(zpos_int);
    pNewFireFighter->mbActive = 1;
    pNewFireFighter->mState = firefighter_state::spawn_truck_1;
    pNewFireFighter->mNextStateTimer = 0;
    pNewFireFighter->mTargetCar = pCar;
    return pNewFireFighter;
}

// https://decomp.me/scratch/ZcdAk
DEFINE_GLOBAL_INIT(Fix16, kFpThree_67D384, Fix16(3), 0x67D384);

MATCH_FUNC(0x4a8800)
Firefighter* FirefighterPool::New28()
{
    Firefighter* pFirefighter = mFirefighters;
    for (s16 i = 0; i < 2; i++)
    {
        if (!pFirefighter->mbActive)
        {
            return pFirefighter;
        }
        pFirefighter++;
    }
    return NULL;
}

MATCH_FUNC(0x4a8820)
bool FirefighterPool::TryDispatchFirefightersToCar(Car* pCar)
{
    if (!pCar)
    {
        return false;
    }
    const s32 despawn_status = pCar->mDespawnStatus;
    if (despawn_status == car_despawn_status::despawned_6 || despawn_status == car_despawn_status::deactivated_7 || despawn_status == car_despawn_status::despawning_5)
    {
        return false;
    }

    if (pCar->IsTrainModel())
    {
        return false;
    }

    Firefighter* pFirefighter =
        DispatchFirefighters(pCar, pCar->mCarSprite->GetXPos(), pCar->mCarSprite->GetYPos(), pCar->mCarSprite->GetZPos());

    if (!pFirefighter)
    {
        return false;
    }
    pFirefighter->mTargetCar = pCar;
    return true;
}

MATCH_FUNC(0x4A8890)
bool FirefighterPool::IsFireEngineSentToCar(Car* pFireEngine, Car* pTargetCar) const
{
    u8 i = 0;
    const Firefighter* pFirefighter = mFirefighters;
    while (i < GTA2_COUNTOF(mFirefighters))
    {
        if (pFirefighter && pFirefighter->mTargetCar == pTargetCar && pFirefighter->mCar == pFireEngine)
        {
            return true;
        }
        pFirefighter++;
        i++;
    }
    return false;
}

MATCH_FUNC(0x4a88d0)
void FirefighterPool::ResetCount()
{
    mCount = 0;
}