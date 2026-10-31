#include "EmergencyCrewPool.hpp"
#include "Car.hpp"
#include "CarChaseTaskTable.hpp"
#include "Ped.hpp"
#include "PedGroup.hpp"

DEFINE_GLOBAL(EmergencyCrewPool*, gEmergencyCrewPool_706280, 0x706280);
DEFINE_GLOBAL_INIT(Fix16, kFpFour_706148, Fix16(4), 0x706148);

MATCH_FUNC(0x4beb00)
EmergencyCrew::EmergencyCrew()
{
    Init();
}

MATCH_FUNC(0x4beb10)
EmergencyCrew::~EmergencyCrew()
{
}

MATCH_FUNC(0x4beb20)
EmergencyCrewPool::~EmergencyCrewPool()
{
}

MATCH_FUNC(0x5cbb70)
void EmergencyCrewPool::init()
{
}

MATCH_FUNC(0x5cbb80)
EmergencyCrew* EmergencyCrewPool::AllocateSlot()
{
    for (u8 i = 0; i < GTA2_COUNTOF(mSlots); i++)
    {
        if (!mSlots[i].mIsUsed)
        {
            return &mSlots[i];
        }
    }
    return NULL;
}

MATCH_FUNC(0x5cbbd0)
void EmergencyCrewPool::ServiceAll()
{
    for (s32 i = 0; i < 10; i++)
    {
        if (mSlots[i].mIsUsed)
        {
            if (mSlots[i].Service())
            {
                mSlots[i].mIsUsed = false;
            }
        }
    }
}

MATCH_FUNC(0x5cbc00)
void EmergencyCrew::Init()
{
    mIdleLimit = 150;
    mIsUsed = false;
    mCrewType = 0;
    mPhase = crew_phase::on_foot_0;
    mCar = NULL;
    mPed = NULL;
    mState = crew_state::idle_0;
    mGroup = NULL;
    mReady = false;
    mSpawnWaitTicks = 0;
}

MATCH_FUNC(0x5cbc30)
void EmergencyCrew::ReInit()
{
    Init();
}

MATCH_FUNC(0x5cbc40)
void EmergencyCrew::RemovePed(Ped* pPed)
{
    mGroup->RemovePed(pPed);
    mPed = mGroup->mPedLeader;
}

// https://decomp.me/scratch/HmQPr
MATCH_FUNC(0x5cbc60)
bool EmergencyCrew::IsLeaderAlive() const
{
    if (mPed && mPed->isDead_403B60())
    {
        return false;
    }
    // A switch whose cases all return true: VC6 drops the compares but keeps the
    // load of mState (the stray mov 0x28(%ecx),%ecx). The case values are a guess.
    switch (mState)
    {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
            return true;
    }
    return true;
}

MATCH_FUNC(0x5cbc90)
bool EmergencyCrew::ReplaceLeaderIfNeeded()
{
    PedGroup* pGroup = this->mGroup;
    if (!pGroup)
    {
        return false;
    }

    u8 idx = 0;
    Ped* pPedAtIdx = pGroup->mPedList[idx];
    while (pPedAtIdx)
    {
        if (pPedAtIdx->GetPedState_403990() != ped_state_1::dead_9 && !pPedAtIdx->mCar)
        {
            Ped* pLeaderPed = this->mPed;
            if (pLeaderPed->mCar)
            {
                RemovePed(pLeaderPed);
            }
            else
            {
                pGroup->PromoteMemberToLeader(idx);
                Ped* pLeader = this->mGroup->mPedLeader;
                this->mPed = pLeader;
                pLeader->SetObjective(objectives_enum::no_obj_0);
                const s32 occupation = pPedAtIdx->get_occupation_403980();
                pPedAtIdx->set_occupation_403970(ped_ocupation_enum::dummy);
                pPedAtIdx->Kill();
                pPedAtIdx->set_occupation_403970(occupation);
                pPedAtIdx->SetObjective(objectives_enum::dead_patient_28);
            }
            return true;
        }
        idx++;
        pPedAtIdx = pGroup->mPedList[idx];
    }
    return false;
}

// https://decomp.me/scratch/sUPg8
MATCH_FUNC(0x5cbd50)
void EmergencyCrew::UpdateStateMachine()
{
    bool bCarGone = false;
    bool bNoneOnFoot = true;
    bool bAllOnFoot = true;
    bool bDriverIsOther = false;
    u8 idx;
    if (mGroup)
    {
        if (!mGroup->mCount34)
        {
            mGroup->ClearGroupData();
            mGroup = NULL;
        }
    }

    if (mPhase == crew_phase::in_car_1)
    {
        if (mCar)
        {
            if (mCar->IsDespawning_4215B0())
            {
                bCarGone = true;
            }
            if (mCar->IsMaxDamage_40F890())
            {
                bCarGone = true;
            }
            if (mCar->HasSpriteZoom())
            {
                bCarGone = true;
            }

            if (mCar->mDriver && mCar->mDriver != mPed)
            {
                bDriverIsOther = true;
                bCarGone = true;
            }
        }
        else
        {
            bCarGone = true;
        }

        if (bCarGone)
        {
            if (mGroup)
            {
                idx = 0;
                for (Ped* pMember = mGroup->mPedList[0]; pMember; pMember = mGroup->mPedList[++idx])
                {
                    if (pMember->mGameObject)
                    {
                        bNoneOnFoot = false;
                    }
                    else
                    {
                        bAllOnFoot = false;
                    }
                }
            }

            if (mPed)
            {
                if (mPed->mGameObject)
                {
                    bNoneOnFoot = false;
                }
            }
            if (bDriverIsOther)
            {
                if (!bAllOnFoot)
                {
                    return;
                }
                if (mPed)
                {
                    if (!mPed->mGameObject)
                    {
                        return;
                    }

                    if (Fix16::MaxAbsDistance_42A6B0(mPed->get_cam_x(),
                                                     mPed->get_cam_y(),
                                                     mCar->mCarSprite->mXy.x,
                                                     mCar->mCarSprite->mXy.y) > kFpFour_706148)
                    {
                        bCarGone = true;
                    }
                }
                else
                {
                    bCarGone = true;
                }
            }
            if (!bCarGone)
            {
                return;
            }
            if (mCar)
            {
                mCar->mLastSeenTimer = 0;
                if (mCar->mPedType != ped_type::player_2)
                {
                    mCar->mPedType = ped_type::dummy_3;
                }
                if (mCar->mpChaseTask)
                {
                    gCarChaseTaskTable_678E30->FreeEntry(mCar->mpChaseTask);
                    mCar->mpChaseTask = NULL;
                }
                mCar = NULL;
            }
            if (bNoneOnFoot)
            {
                if (mPed)
                {
                    mPed->set_occupation_403970(ped_ocupation_enum::dummy);
                    mPed->ClearGroupAndGroupIdx_403A30();
                    mPed->Deallocate();
                }
                idx = 0;
                if (mGroup)
                {
                    for (Ped* pMember = mGroup->mPedList[0]; pMember; pMember = mGroup->mPedList[idx])
                    {
                        pMember->set_occupation_403970(ped_ocupation_enum::dummy);
                        pMember->ClearGroupAndGroupIdx_403A30();
                        pMember->Deallocate();
                        ++idx;
                    }
                    mGroup->ClearGroupData();
                }
                mGroup = NULL;
                mPed = NULL;
                mPhase = crew_phase::finished_2;
                return;
            }
            if (bAllOnFoot)
            {
                if (mPed)
                {
                    if (mPed->mGameObject)
                    {
                        mPhase = crew_phase::on_foot_0;
                        if (mGroup)
                        {
                            mGroup->ResetGroupObjectives();
                        }
                        return;
                    }
                }
            }

            if (mGroup)
            {
                if (mGroup->PurgeMembersInCars())
                {
                    if (mGroup->mPedLeader)
                    {
                        mPhase = crew_phase::on_foot_0;
                        mGroup->ResetGroupObjectives();
                    }
                    else if (mGroup->mCount34 == 1)
                    {
                        mPed = mGroup->mPedList[0];
                        mGroup->ClearGroupData();
                        mGroup = NULL;
                        mPed->SetObjective(objectives_enum::no_obj_0);
                        mPed->SetObjective2(objectives_enum::no_obj_0);
                        mPhase = crew_phase::on_foot_0;
                    }
                    else
                    {
                        mGroup->mPedLeader = mGroup->mPedList[mGroup->mCount34];
                        mGroup->mPedList[mGroup->mCount34] = NULL;
                        --mGroup->mCount36;
                        --mGroup->mCount34;
                        mPed = mGroup->mPedLeader;
                        mGroup->ResetGroupObjectives();
                        mPhase = crew_phase::on_foot_0;
                    }
                }
                else
                {
                    mPed = mGroup->mPedLeader;
                    mGroup->DestroyGroup();
                    mGroup = NULL;
                    mPed->SetObjective(objectives_enum::no_obj_0);
                    mPed->SetObjective2(objectives_enum::no_obj_0);
                    mPhase = crew_phase::on_foot_0;
                }
            }
            else if (mPed)
            {
                if (mPed->mGameObject)
                {
                    mPed->SetObjective(objectives_enum::no_obj_0);
                    mPed->SetObjective2(objectives_enum::no_obj_0);
                    mPhase = crew_phase::on_foot_0;
                }
            }
            else if (!mCar)
            {
                mPhase = crew_phase::finished_2;
            }
        }
        else
        {
            bCarGone = true;

            if (mGroup)
            {
                idx = 0;
                for (Ped* pMember = mGroup->mPedList[0]; pMember; pMember = mGroup->mPedList[++idx])
                {
                    if (!pMember->isDead_403B60())
                    {
                        bCarGone = false;
                    }
                }
            }

            if (bCarGone)
            {
                if (!mPed || mPed->isDead_403B60())
                {
                    mPhase = crew_phase::finished_2;
                }
            }
            if (mPhase == crew_phase::finished_2)
            {
                if (mCar)
                {
                    mCar->SetDriverPedType_421560(ped_type::dummy_3);
                    mCar->mLastSeenTimer = -200;
                }
            }
        }
    }
    else if (mPhase == crew_phase::on_foot_0)
    {
        bCarGone = true;
        if (mGroup)
        {
            idx = 0;
            for (Ped* pMember = mGroup->mPedList[0]; pMember; pMember = mGroup->mPedList[++idx])
            {
                if (!pMember->isDead_403B60())
                {
                    bCarGone = false;
                }
            }
        }

        if (bCarGone)
        {
            if (!mPed || mPed->isDead_403B60())
            {
                mPhase = crew_phase::finished_2;
            }
        }
    }
    else
    {
        if (mCar)
        {
            if (mCar->mpChaseTask)
            {
                gCarChaseTaskTable_678E30->FreeEntry(mCar->mpChaseTask);
                mCar->mpChaseTask = NULL;
            }
        }
        mCar = NULL;
    }
}

// 9.6f 0x4C5A00
MATCH_FUNC(0x5cc1c0)
void EmergencyCrew::CleanupExpiredEntities()
{
    bool bReleaseCar = false;
    bool bClearPedAndGroup = true;
    bool bAllOnFoot = true;

    if (mGroup)
    {
        if (mGroup->mCount34 == 0)
        {
            mGroup->ClearGroupData();
            this->mGroup = NULL;
        }
    }

    if (this->mPhase == crew_phase::in_car_1)
    {
        if (this->mCar && !mCar->IsDespawning_4215B0() && !mCar->IsMaxDamage_40F890())
        {
            if (mCar->GetLastSeenTimer_4A9AD0() > this->mIdleLimit)
            {
                mCar->MarkForDespawn_421470();
                bReleaseCar = true;
            }
        }
        else
        {
            bReleaseCar = true;
        }

        if (mPed)
        {
            if (mPed->mGameObject)
            {
                if (mPed->GetOffscreenCounter_4039F0() < this->mIdleLimit)
                {
                    bClearPedAndGroup = false;
                }
            }
            else if (mPed->isDead_403B60())
            {
                this->mPed = NULL;
            }
        }

        if (mPed)
        {
            if (mPed->mGameObject)
            {
                bClearPedAndGroup = false;
            }
        }

        if (mGroup)
        {
            u8 i = 0;
            for (Ped* pPedListIter = mGroup->mPedList[0]; pPedListIter;)
            {
                if (pPedListIter->mGameObject)
                {
                    if (pPedListIter->GetOffscreenCounter_4039F0() < this->mIdleLimit)
                    {
                        bClearPedAndGroup = false;
                    }
                }
                else
                {
                    bAllOnFoot = false;
                }
                i++;
                pPedListIter = mGroup->mPedList[i];
            }
        }

        if (bReleaseCar)
        {
            if (mCar)
            {
                if (mCar->mpChaseTask)
                {
                    gCarChaseTaskTable_678E30->FreeEntry(mCar->mpChaseTask);
                    mCar->mpChaseTask = NULL;
                }
            }
            mCar = NULL;

            if (bClearPedAndGroup)
            {
                if (mPed)
                {
                    mPed->set_occupation_403970(ped_ocupation_enum::dummy);
                    mPed->ClearGroupAndGroupIdx_403A30();
                    mPed->Deallocate();
                }

                u8 i = 0;
                if (mGroup)
                {
                    for (Ped* pPedListIter = mGroup->mPedList[0]; pPedListIter;)
                    {
                        pPedListIter->set_occupation_403970(ped_ocupation_enum::dummy);
                        pPedListIter->ClearGroupAndGroupIdx_403A30();
                        pPedListIter->Deallocate();
                        i++;
                        pPedListIter = mGroup->mPedList[i];
                    }
                    mGroup->ClearGroupData();
                }
                this->mGroup = NULL;
                this->mReady = true;
            }
            else if (bAllOnFoot)
            {
                if (this->mPed->mGameObject)
                {
                    this->mPhase = crew_phase::on_foot_0;
                }
            }
        }
        return;
    }

    if (mPed)
    {
        if (mPed->GetOffscreenCounter_4039F0() < this->mIdleLimit)
        {
            bClearPedAndGroup = false;
        }
    }

    if (mGroup)
    {
        u8 i = 0;
        for (Ped* pPedListIter = mGroup->mPedList[0]; pPedListIter;)
        {
            if (pPedListIter->GetOffscreenCounter_4039F0() < this->mIdleLimit)
            {
                bClearPedAndGroup = false;
            }
            i++;
            pPedListIter = mGroup->mPedList[i];
        }
    }

    if (bClearPedAndGroup)
    {
        if (mPed)
        {
            mPed->ClearGroupAndGroupIdx_403A30();
            mPed->Deallocate();
        }

        u8 i = 0;
        if (mGroup)
        {
            Ped* pPedListIter = mGroup->mPedList[0];
            while(pPedListIter)
            {
                pPedListIter->ClearGroupAndGroupIdx_403A30();
                pPedListIter->Deallocate();
                i++;
                pPedListIter = mGroup->mPedList[i];
            }
            mGroup->ClearGroupData();
        }
        mReady = true;
    }
}

MATCH_FUNC(0x5cc480)
bool EmergencyCrew::Service()
{
    if (mSpawnDelay > 0)
    {
        this->mSpawnDelay--;
        return false;
    }

    if (mSpawnDelay > -80)
    {
        this->mSpawnDelay--;
    }

    switch (this->mState)
    {
        case crew_state::clean_up_5:
            CleanupExpiredEntities();
            return false;

        case crew_state::update_6:
            UpdateStateMachine();
            return false;

        case crew_state::spawn_car_3:
            break;

        default:
            return false;
    }

    switch (mCrewType)
    {
        case crew_type::paramedic_1:
            if (gCarManager_6C_677930->CanAllocateCarOfKind(car_kind::paramedic_car_4))
            {
                this->mCar = gCarManager_6C_677930->SpawnCarAtRoadDirection(car_model_enum::MEDICAR,
                                                                                   this->mSpawnX,
                                                                                   this->mSpawnY,
                                                                                   this->mSpawnZ);
            }
            else
            {
                this->mCar = NULL;
            }

            if (this->mCar)
            {
                this->mReady = true;
                mCar->IncrementCarStats(car_kind::paramedic_car_4);
            }
            break;

        case crew_type::police_3:
            if (gCarManager_6C_677930->CanAllocateCarOfKind(car_kind::police_6))
            {
                this->mCar = gCarManager_6C_677930->SpawnCarAtRoadDirection(car_model_enum::COPCAR,
                                                                                   this->mSpawnX,
                                                                                   this->mSpawnY,
                                                                                   this->mSpawnZ);
            }
            else
            {
                this->mCar = NULL;
            }
            if (this->mCar)
            {
                this->mReady = true;
                mCar->IncrementCarStats(car_kind::police_6);
            }
            break;

        case crew_type::swat_5:
            if (gCarManager_6C_677930->CanAllocateCarOfKind(car_kind::police_6))
            {
                this->mCar = gCarManager_6C_677930->SpawnCarAtRoadDirection(car_model_enum::SWATVAN,
                                                                                   this->mSpawnX,
                                                                                   this->mSpawnY,
                                                                                   this->mSpawnZ);
            }
            else
            {
                this->mCar = NULL;
            }

            if (this->mCar)
            {
                this->mReady = true;
                mCar->IncrementCarStats(car_kind::police_6);
            }
            break;

        default:
            mCar = NULL;
            break;
    }

    return false;
}