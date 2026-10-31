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

DEFINE_GLOBAL(FirefighterPool*, gFirefighterPool_54_67D4C0, 0x67D4C0);
DEFINE_GLOBAL_INIT(Fix16, kFpHalf_67D1F0, Fix16(0.5f), 0x67D1F0);
DEFINE_GLOBAL_INIT(Fix16, kFpZero_67D378, Fix16(0), 0x67D378);

DEFINE_GLOBAL_INIT(Ang16, kAng0_67D4B0, Ang16(0), 0x67D4B0);
DEFINE_GLOBAL_INIT(Ang16, kAng90_67D208, Ang16(360), 0x67D208);
DEFINE_GLOBAL_INIT(Ang16, kAng180_67D2D6, Ang16(720), 0x67D2D6);
DEFINE_GLOBAL_INIT(Ang16, kAng270_67D2FC, Ang16(1080), 0x67D2FC);

// https://decomp.me/scratch/X8G5z mov 0x4(%esp) vs mov (%eax)
MATCH_FUNC(0x4a7fc0)
bool Firefighter::CheckAbortConditions_4A7FC0()
{
    if (!mCar->mPhysics)
    {
        Firefighter::deinit_4A81A0();
        return false;
    }

    if (mCar->mPhysics->get_car_velocity_4211C0() == kFpZero_67D378)
    {
        if (++mNextStateTimer >= 1000)
        {
            Firefighter::deinit_4A81A0();
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
            mTargetCar->IsDespawning_4215B0() || mTargetCar->IsMarkedForDespawn_4214B0())
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
                if (!mTargetCar->mAttachments.FindFirstFireExplosion_5A6AD0())
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
            if (mTargetCar->mDespawnStatus != car_despawn_status::despawned_6 && !mTargetCar->IsDespawning_4215B0())
            {
                if (mPed)
                {
                    if (mTargetCar->GetX_41E430().ToUInt8() != mPed->mObjectiveTargetX.ToUInt8() ||
                        mTargetCar->GetY_41E440().ToUInt8() != mPed->mObjectiveTargetY.ToUInt8())
                    {
                        mPed->SetObjective(objectives_enum::goto_area_in_car_14);
                        mPed->mObjectiveTargetX = Fix16(mTargetCar->GetX_41E430().ToUInt8());
                        mPed->mObjectiveTargetY = Fix16(mTargetCar->GetY_41E440().ToUInt8());
                        mPed->mObjectiveTargetZ = Fix16(mTargetCar->GetZ_41E450().ToUInt8());
                    }
                }
            }
        }
    }

    if (mState == firefighter_state::abort_6)
    {
        Firefighter::deinit_4A81A0();
    }
    return true;
}

MATCH_FUNC(0x4a81a0)
void Firefighter::deinit_4A81A0()
{
    Car* pCar = this->mCar;
    if (pCar)
    {
        CarChaseTask* pRoute = pCar->mpChaseTask;
        if (pRoute)
        {
            gCarChaseTaskTable_678E30->FreeEntry_474CC0(pRoute);
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
        pTargetCar->mAttachments.CleanupSpriteList_5A7080();
    }
}

MATCH_FUNC(0x4a81f0)
void Firefighter::Update_4A81F0()
{
    if (!mbActive)
    {
        return;
    }

    switch (mState)
    {
        case firefighter_state::spawn_truck_1:
            if (mTargetCar && mTargetCar->mDespawnStatus != car_despawn_status::despawned_6 &&
                !mTargetCar->IsDespawning_4215B0() && gCarManager_6C_677930->CanAllocateCarOfKind_446930(car_kind::firefighter_5))
            {
                mCar = gCarManager_6C_677930->SpawnCarAtRoadDirection_444CF0(car_model_enum::FIRETRUK, mXpos, mYpos, mZpos);
                if (mCar)
                {
                    mCar->IncrementCarStats_443D70(car_kind::firefighter_5);
                    mCar->SetDriverPedType_421560(ped_type::special_ped_4);
                    Car* pCar = mCar;
                    pCar->AllocateAI_421510();
                    mCar->mAI->SetCar_453BF0(mCar);

                    mPed = gPedManager_6787BC->AllocatePed_470F30();
                    mPed->SetPedType_403920(ped_type::special_ped_4);
                    mPed->set_occupation_403970(ped_ocupation_enum::fireman);
                    mPed->SpawnPedInCar_45C730(mCar);
                    mPed->SetObjective(objectives_enum::goto_area_in_car_14);
                    mPed->mObjectiveTargetX = Fix16(mTargetCar->GetX_41E430().ToUInt8());
                    mPed->mObjectiveTargetY = Fix16(mTargetCar->GetY_41E440().ToUInt8());
                    mPed->mObjectiveTargetZ = Fix16(mTargetCar->GetZ_41E450().ToUInt8());
                    mPed->mBf21C.bUseCarWeapon = 1;
                    mCar->ActivateEmergencyLights_43C920();
                    mCar->SetupCarPhysicsAndSpriteBinding_43BCA0();
                    mPed = mCar->GetDriver_4118B0();
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
            else if (CheckAbortConditions_4A7FC0())
            {
                if (Fix16::MaxAbsDistance_42A6B0(mCar->GetX_41E430(),
                                          mCar->GetY_41E440(),
                                          mTargetCar->GetX_41E430(),
                                          mTargetCar->GetY_41E440()) < kFpThree_67D384 &&
                    mNextStateTimer > 100)
                {
                    mState = firefighter_state::arrived_3;
                }

                if (mPed)
                {
                    switch (mPed->GetObjectiveStatus_450CB0())
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
            if (CheckAbortConditions_4A7FC0() && mPed && mState == firefighter_state::arrived_3)
            {
                mPed->SetObjective(objectives_enum::turret_put_out_car_fire_60);
                mPed->SetTargetObjectiveCar(mTargetCar);
                mState = firefighter_state::put_out_fire_4;
            }
            break;

        case firefighter_state::put_out_fire_4:
            if (CheckAbortConditions_4A7FC0())
            {
                switch (mPed->GetObjectiveStatus_450CB0())
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
            if (mCar && mCar->mDespawnStatus != car_despawn_status::despawned_6 && !mCar->IsDespawning_4215B0() &&
                !mCar->IsMarkedForDespawn_4214B0())
            {
                if (mCar->mDriver)
                {
                    mCar->mDriver->mBf21C.bDriveAggressively = 1;
                }
                mCar->SetDriverPedType_421560(ped_type::dummy_3);
                mCar->InitCarAIControl_440590();
                mCar->ResumeAIDriving_43AF40();
                mCar->DeactivateEmergencyLights_43C9D0();
                if (mCar->mDriver)
                {
                    mCar->mDriver->SetTargetObjectiveCar(NULL);
                    mCar->mDriver->SetObjective(objectives_enum::no_obj_0);
                    mCar->mDriver->ClearAttacking_403A40();
                }
            }
            Reset_4A85E0();
            break;
    }

    if (mState == firefighter_state::abort_6)
    {
        deinit_4A81A0();
    }
}

MATCH_FUNC(0x4a85c0)
void Firefighter::init_4A85C0()
{
    Clear_450C10();
}

MATCH_FUNC(0x4a85e0)
void Firefighter::Reset_4A85E0()
{
    this->mTargetCar = NULL;
    this->mState = firefighter_state::idle_0;
    this->mbActive = 0;
    this->mCar = NULL;
}

MATCH_FUNC(0x4a85f0)
void FirefighterPool::FireEnginesService_4A85F0()
{
    Firefighter* pFirefighter = mFirefighters;
    if (!bSkip_fire_engines_67D53A)
    {
        for (u32 i = 0; i < 2; i++)
        {
            if (pFirefighter->mbActive)
            {
                pFirefighter->Update_4A81F0();
            }
            pFirefighter++;
        }
    }
}

MATCH_FUNC(0x4a8620)
Firefighter* FirefighterPool::DispatchFirefighters_4A8620(Car* pCar, Fix16 xpos, Fix16 ypos, Fix16 zpos)
{
    if (bSkip_fire_engines_67D53A)
    {
        return NULL;
    }
    u8 xpos_int = xpos.ToUInt8();
    u8 ypos_int = ypos.ToUInt8();
    u8 zpos_int = (zpos + kFpHalf_67D1F0).ToUInt8();
    if (gPathFinder_6FDEF0->FindNearbyTileMatchingSlopeType_5552B0(ROAD, &xpos_int, &ypos_int, &zpos_int, false) != 1)
    {
        return NULL;
    }
    s8 arrow_count;
    if (pCar->CountConsecutiveArrowBlocks_4410D0(kAng0_67D4B0, &arrow_count, xpos_int, ypos_int) < 0 || arrow_count <= 1)
    {
        if (pCar->CountConsecutiveArrowBlocks_4410D0(kAng90_67D208, &arrow_count, xpos_int, ypos_int) < 0 || arrow_count <= 1)
        {
            if (pCar->CountConsecutiveArrowBlocks_4410D0(kAng180_67D2D6, &arrow_count, xpos_int, ypos_int) < 0 || arrow_count <= 1)
            {
                if (pCar->CountConsecutiveArrowBlocks_4410D0(kAng270_67D2FC, &arrow_count, xpos_int, ypos_int) < 0 || arrow_count <= 1)
                {
                    return NULL;
                }
            }
        }
    }
    Firefighter* pNewFireFighter = FirefighterPool::New28_4A8800();
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
Firefighter* FirefighterPool::New28_4A8800()
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
bool FirefighterPool::TryDispatchFirefightersToCar_4A8820(Car* pCar)
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

    if (pCar->IsTrainModel_403BA0())
    {
        return false;
    }

    Firefighter* pFirefighter =
        DispatchFirefighters_4A8620(pCar, pCar->mCarSprite->GetXPos(), pCar->mCarSprite->GetYPos(), pCar->mCarSprite->GetZPos());

    if (!pFirefighter)
    {
        return false;
    }
    pFirefighter->mTargetCar = pCar;
    return true;
}

MATCH_FUNC(0x4A8890)
bool FirefighterPool::IsFireEngineSentToCar_4A8890(Car* pFireEngine, Car* pTargetCar) const
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
void FirefighterPool::ResetCount_4A88D0()
{
    mCount = 0;
}