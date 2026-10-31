#include "PedGroup.hpp"
#include "ped_group_type.hpp"
#include "Car.hpp"
#include "Char_Pool.hpp"
#include "Globals.hpp"
#include "Ped.hpp"
#include "enums.hpp"
#include "rng.hpp"
#include "crt_stubs.hpp"

DEFINE_GLOBAL_ARRAY(PedGroup, pedGroups, 20, 0x67EF20);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kNoNearestDistance, Fix16(0x270F00, 0), 0x67F60C);
DEFINE_GLOBAL_INIT(Fix16, kFpTwo_67F608, Fix16(2), 0x67F608);
DEFINE_GLOBAL(Fix16, kFpZero_67F610, 0x67F610);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpEight, Fix16(8), 0x67F670);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpFiveSixtyFourths, Fix16(0x500, 0), 0x67EEE4);
DEFINE_STATIC_MEMBER_INIT(char_type, PedGroup, sGroupMemberCarDoor, 1, 0x620838);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpFour, Fix16(4), 0x67F630);

// The original is the compiler generated static destructor of pedGroups: a call to the
// CRT's vector destructor iterator `??_M` (`__ehvec_dtor`, which VC6 maps to that name). The CRT's
// version takes a __thiscall destructor pointer (`P6E`), which VC6 can't declare (no __thiscall
// keyword), and a destructor's address can't be taken, so __ehvec_dtor (crt_stubs.cpp) takes a
// __cdecl pointer to a destructor thunk here.
static void __cdecl PedGroup_dtor_4CB870(void* pGroup)
{
    static_cast<PedGroup*>(pGroup)->~PedGroup();
}

MATCH_FUNC(0x4c8e60)
void PedGroup::DestroyGroupArray()
{
    __ehvec_dtor(pedGroups, sizeof(PedGroup), GTA2_COUNTOF(pedGroups), PedGroup_dtor_4CB870);
}

MATCH_FUNC(0x4c8e80)
void PedGroup::ResetMemberCarDoor()
{
    sGroupMemberCarDoor = 1;
}

MATCH_FUNC(0x4c8e90)
void PedGroup::ClearGroupData()
{
    mInUse = false;
    mGroupType = ped_group_type::normal_2;
    mPanicking = 0;
    mCount36 = 0;
    mCount34 = 0;
    if (mPedLeader != NULL)
    {
        mPedLeader->ClearGroupAndGroupIdx();
    }
    mPedLeader = NULL;
    mRemoveDeadMembers = true;
    mWaitForMembers = false;
    mCount34 = 0;
    mUnused35 = 0;

    for (s32 i = 0; i < 9; ++i)
    {
        if (mPedList[i] != NULL)
        {
            mPedList[i]->ClearGroupAndGroupIdx();
        }
        mPedList[i] = NULL;
    }
}

MATCH_FUNC(0x4c8ef0)
void PedGroup::Reset()
{
    mInUse = false;
    mGroupType = ped_group_type::normal_2;
    mPanicking = 0;
    mCount36 = 0;
    mPedLeader = NULL;
    mRemoveDeadMembers = true;
    mWaitForMembers = false;
    mCount34 = 0;
    mUnused35 = 0;
    for (s32 i = 0; i < 9; i++)
    {
        mPedList[i] = NULL;
    }
}

MATCH_FUNC(0x4c8f20)
void PedGroup::ResetGroupObjectives()
{
    mPedLeader->SetObjective(objectives_enum::no_obj_0);
    mPedLeader->SetObjective2(objectives_enum::no_obj_0);

    u8 ped_idx = 0;

    while (ped_idx < mCount34)
    {
        mPedList[ped_idx]->SetObjective(objectives_enum::no_obj_0);
        mPedList[ped_idx]->SetObjective2(objectives_enum::no_obj_0);
        ped_idx++;
    }
}

MATCH_FUNC(0x4c8f90)
void PedGroup::add_ped_to_end_of_list(Ped* pPed)
{
    pPed->SetObjective2(objectives_enum::no_obj_0);
    pPed->SetObjective(objectives_enum::no_obj_0);
    add_ped_to_list(pPed, mCount34);
    ++mCount34;
    ++mCount36;
}

MATCH_FUNC(0x4c8fe0)
void PedGroup::replace_leader(Ped* pNewLeader)
{
    add_ped_to_end_of_list(mPedLeader);
    mPedLeader = pNewLeader;
    pNewLeader->set_ped_group_id(99);

    for (u8 i = 0; i < mCount34; i++)
    {
        mPedList[i]->SetObjective2(objectives_enum::no_obj_0);
    }
}

MATCH_FUNC(0x4c9040)
bool PedGroup::PurgeMembersInCars()
{
    mCount36 = mCount34;
    if (mPedLeader->mCar != NULL)
    {
        mPedLeader->Kill();
        mPedLeader = NULL;
    }

    u8 ped_idx;
    for (ped_idx = 0; ped_idx < mCount34; ped_idx++)
    {
        if (mPedList[ped_idx]->mCar != NULL)
        {
            mPedList[ped_idx]->Kill();
            mPedList[ped_idx] = NULL;
        }
    }

    // Compacting the ped_list
    for (ped_idx = 0; ped_idx < mCount34; ped_idx++)
    {
        if (mPedList[ped_idx] == NULL)
        {
            for (u8 i = ped_idx; i < mCount34; i++)
            {
                if (mPedList[i] != NULL)
                {
                    mPedList[ped_idx] = mPedList[i];
                    mPedList[i] = NULL;
                    break;
                }
            }
        }
    }

    ped_idx = 0;
    while (mPedList[ped_idx] != NULL)
    {
        ped_idx++;
    }

    if (ped_idx == 0)
    {
        return false;
    }
    return true;
}

MATCH_FUNC(0x4c9150)
bool PedGroup::AreAllMembersOffScreen() const
{
    if (mPedLeader->mGameObject == NULL || mPedLeader->GetOffscreenCounter() < 0x28)
    {
        return false;
    }

    for (u8 i = 0; i < mCount34; i++)
    {
        if (mPedList[i]->mGameObject == NULL || mPedList[i]->GetOffscreenCounter() < 0x28)
        {
            return false;
        }
    }

    return true;
}

MATCH_FUNC(0x4c91b0)
void PedGroup::ResetMembersToFollowLeader()
{
    for (u8 i = 0; i < mCount34; i++)
    {
        mPedList[i]->ClearPanicking();
        mPedList[i]->SetObjective2(objectives_enum::follow_char_9);
        mPedList[i]->SetInternalTargetPed(mPedLeader);
    }
}

MATCH_FUNC(0x4c9210)
bool PedGroup::IsLeaderInCar() const
{
    if (mPedLeader->mCar != NULL)
    {
        return true;
    }
    else
    {
        return false;
    }
}

MATCH_FUNC(0x4c9220)
bool PedGroup::IsLeaderEnteringCarOrTrain() const
{
    if (mPedLeader->GetPedState() != ped_state_1::entering_car_3)
    {
        if (mPedLeader->GetPedState() != ped_state_1::entering_train_5)
        {
            return false;
        }
    }
    return true;
}

MATCH_FUNC(0x4c9240)
void PedGroup::KillEntireGroup()
{
    mPedLeader->ClearGroupAndGroupIdx();
    mPedLeader->Kill();
    for (char_type i = 0; i < mCount34; i++)
    {
        mPedList[i]->ClearGroupAndGroupIdx();
        mPedList[i]->Kill();
    }
    this->mInUse = false;
}


MATCH_FUNC(0x4c92a0)
void PedGroup::DisbandGroup()
{
    if (!mInUse)
    {
        return;
    }

    mPedLeader->ClearGroupAndGroupIdx();
    if (mPedList[0] != NULL)
    {
        for (u8 ped_idx = 0; ped_idx < mCount34; ped_idx++)
        {
            Ped* pPed = mPedList[ped_idx];
            Ped** ppSlot = mPedList + ped_idx;
            if ((pPed->GetPedState() == ped_state_1::dead_9) ||
                (pPed->mStoredPedState1 == ped_state_1::dead_9))
            {
                pPed->ClearGroupAndGroupIdx();
            }
            else
            {
                if (pPed->has_car())
                {
                    pPed->SetObjective(objectives_enum::leave_car_and_flee_34);
                    (*ppSlot)->SetTargetObjectiveCar((*ppSlot)->mCar);
                }
                else
                {
                    pPed->SetObjective(objectives_enum::no_obj_0);
                }
                (*ppSlot)->SetObjective2(objectives_enum::no_obj_0);
                (*ppSlot)->ClearGroupAndGroupIdx();
                (*ppSlot)->SetPedType(ped_type::dummy_3);
            }
            (*ppSlot)->mBf21C.bScheduledForRemoval = true;
        }
    }
    ClearGroupData();
}

MATCH_FUNC(0x4c93a0)
void PedGroup::DestroyGroup()
{
    if (!mInUse)
    {
        return;
    }

    Ped* pPed = mPedLeader;
    if ((pPed->GetPedState() != ped_state_1::dead_9) && (pPed->mStoredPedState1 != ped_state_1::dead_9))
    {
        pPed->SetObjective(objectives_enum::no_obj_0);
        mPedLeader->SetObjective2(objectives_enum::no_obj_0);
    }

    mPedLeader->ClearGroupAndGroupIdx();
    if (mPedList[0] != NULL)
    {
        for (u8 ped_idx = 0; ped_idx < mCount34; ped_idx++)
        {
            pPed = mPedList[ped_idx];
            Ped** ppSlot = mPedList + ped_idx;
            if ((pPed->GetPedState() == ped_state_1::dead_9) ||
                (pPed->mStoredPedState1 == ped_state_1::dead_9))
            {
                pPed->ClearGroupAndGroupIdx();
            }
            else
            {
                if (pPed->has_car())
                {
                    pPed->SetObjective2(objectives_enum::no_obj_0);
                    (*ppSlot)->SetObjective(objectives_enum::leave_car_and_flee_34);
                    (*ppSlot)->SetTargetObjectiveCar((*ppSlot)->mCar);
                    (*ppSlot)->ClearGroupAndGroupIdx();
                    (*ppSlot)->SetPedType(ped_type::dummy_3);
                }
                else
                {
                    pPed->SetObjective(objectives_enum::no_obj_0);
                    (*ppSlot)->SetObjective2(objectives_enum::no_obj_0);
                    (*ppSlot)->ClearGroupAndGroupIdx();
                    (*ppSlot)->SetPedType(ped_type::dummy_3);
                }
            }
        }
    }
    ClearGroupData();
    return;
}

MATCH_FUNC(0x4c94e0)
void PedGroup::DisbandGroupDueToAttack(Ped* pAttacker)
{
    // 9.6f: Ped 0x403A50 sets mGameObject->mRunOrJumpSpeed (inlined, a Ped helper for it
    // changes the code)
    if (!pAttacker)
    {
        PedGroup::DestroyGroup();
    }
    else
    {
        if (!mPedLeader->PedTypeIs(ped_type::player_2))
        {
            this->mPedLeader->SetObjective(objectives_enum::flee_char_on_foot_always_3);
            this->mPedLeader->set_objective_target_ped(pAttacker);
            this->mPedLeader->SetObjective2(objectives_enum::flee_char_on_foot_always_3);
            this->mPedLeader->SetInternalTargetPed(pAttacker);
            this->mPedLeader->SetPanicking();
            this->mPedLeader->ClearHitCount();
            this->mPedLeader->mGameObject->mRunOrJumpSpeed = kFpFiveSixtyFourths;
        }

        this->mPedLeader->ClearGroupAndGroupIdx();

        for (char_type i_ = 0; i_ < static_cast<s32>(this->mCount34); i_++)
        {
            s32 i = i_;
            if (mPedList[i]->has_car())
            {
                this->mPedList[i]->SetObjective2(objectives_enum::no_obj_0);
                this->mPedList[i]->SetObjective(objectives_enum::flee_char_always_once_car_stopped_6);
                this->mPedList[i]->set_objective_target_ped(pAttacker);
                this->mPedList[i]->ClearHitCount();
                this->mPedList[i]->ClearGroupAndGroupIdx();
            }
            else
            {
                this->mPedList[i]->SetObjective(objectives_enum::flee_char_on_foot_always_3);
                this->mPedList[i]->set_objective_target_ped(pAttacker);
                this->mPedList[i]->SetObjective2(objectives_enum::flee_char_on_foot_always_3);
                this->mPedList[i]->SetInternalTargetPed(pAttacker);
                this->mPedList[i]->SetPanicking();
                this->mPedList[i]->ClearHitCount();
                this->mPedList[i]->mGameObject->mRunOrJumpSpeed = kFpFiveSixtyFourths;
                this->mPedList[i]->ClearGroupAndGroupIdx();
                this->mPedList[i]->SetPedType(ped_type::dummy_3);
            }
        }
        PedGroup::ClearGroupData();
    }
}

MATCH_FUNC(0x4c9680)
void PedGroup::PromoteMemberToLeader(u8 idx)
{
    Ped* pSwapPed = gPedPool->Allocate();

    Weapon* leaderWeapon = mPedLeader->mSelectedWeapon;
    Weapon* memberWeapon = mPedList[idx]->mSelectedWeapon;
    Weapon* leaderWeapon2 = mPedLeader->mpWeapon;
    Weapon* memberWeapon2 = mPedList[idx]->mpWeapon;

    pSwapPed->CopyStatsFromPed(mPedLeader);
    mPedLeader->CopyStatsFromPed(mPedList[idx]);
    mPedLeader->mSelectedWeapon = leaderWeapon;
    mPedLeader->mpWeapon = leaderWeapon2;

    mPedLeader->SetObjective(pSwapPed->get_objective(), pSwapPed->get_objective_timer());
    mPedLeader->SetObjective2(pSwapPed->GetInternalObjective(), pSwapPed->get_internal_objective_timer());
    mPedLeader->set_objective_status(pSwapPed->GetObjectiveStatus());
    mPedLeader->SetInternalObjectiveStatus(pSwapPed->GetInternalObjectiveStatus());
    mPedLeader->set_objective_target_ped(pSwapPed->get_objective_target_ped());
    mPedLeader->SetTargetObjectiveCar(pSwapPed->get_target_objective_car());
    mPedLeader->mObjectiveTargetObject = pSwapPed->mObjectiveTargetObject;
    mPedLeader->mInternalTargetObject = pSwapPed->mInternalTargetObject;
    mPedLeader->mObjectiveTargetX = pSwapPed->mObjectiveTargetX;
    mPedLeader->mObjectiveTargetY = pSwapPed->mObjectiveTargetY;
    mPedLeader->mObjectiveTargetZ = pSwapPed->mObjectiveTargetZ;
    mPedLeader->SetInternalTargetPed(pSwapPed->GetInternalTargetPed());
    mPedLeader->set_target_to_enter(pSwapPed->get_target_to_enter());
    mPedLeader->mInternalTargetX = pSwapPed->mInternalTargetX;
    mPedLeader->mInternalTargetY = pSwapPed->mInternalTargetY;
    mPedLeader->mInternalTargetZ = pSwapPed->mInternalTargetZ;
    mPedLeader->mGroupIdx = 99;
    mPedLeader->set_enter_car_as_passenger(pSwapPed->get_enter_car_as_passenger());
    mPedLeader->set_target_car_door(pSwapPed->get_target_car_door());

    Ped* pLeader = mPedLeader;
    if (pLeader->mGameObject)
    {
        pLeader->mGameObject->mpPed = pLeader;
    }
    else if (!pLeader->get_enter_car_as_passenger() && pLeader->mCar->mDriver != pLeader)
    {
        pLeader->set_enter_car_as_passenger(1);
    }

    mPedList[idx]->CopyStatsFromPed(pSwapPed);
    mPedList[idx]->mGroupIdx = idx;
    mPedList[idx]->mSelectedWeapon = memberWeapon;
    mPedList[idx]->mpWeapon = memberWeapon2;

    Ped* pMember = mPedList[idx];
    if (pMember->mGameObject && pMember->get_occupation() == ped_ocupation_enum::paramedic_23)
    {
        pMember->mGameObject->mpPed = pMember;
        mPedList[idx]->set_enter_car_as_passenger(1);
    }
    else
    {
        if (pMember->mGameObject)
        {
            pMember->mGameObject->mpPed = pMember;
        }

        if (idx < mCount34 - 1)
        {
            mPedList[idx]->ClearGroupAndGroupIdx();
            mPedList[idx] = mPedList[mCount34 - 1];
            mPedList[idx]->mGroupIdx = idx;
        }
        else
        {
            mPedList[idx]->mPedGroup = NULL;
            mPedList[idx] = NULL;
        }
        mCount34--;
        mPedLeader->mGroupIdx = 99;
    }

    if (pSwapPed->mPedType == ped_type::script_created_5)
    {
        pSwapPed->PoolAllocate();
        pSwapPed->mBf21C.bScheduledForRemoval = true;
    }
    else
    {
        pSwapPed->PoolAllocate();
    }
    pSwapPed->set_health(100);
    pSwapPed->set_occupation(ped_ocupation_enum::deallocate_on_death_2);
    pSwapPed->Kill();
}

MATCH_FUNC(0x4c9970)
void PedGroup::RemovePed(Ped* pPed)
{
    if (pPed == mPedLeader)
    {
        if (!mPedLeader->PedTypeIs(ped_type::player_2))
        {
            if (mPedLeader->GetPedState() == ped_state_1::dead_9 && mPedLeader->mPedType == ped_type::script_created_5)
            {
                mPedLeader->mBf21C.bScheduledForRemoval = true;
            }

            Ped* pNewLeader = mPedList[0];
            if (pNewLeader && !pNewLeader->isDead())
            {
                PromoteMemberToLeader(0);
                if (mPedLeader->GetInternalObjective() == objectives_enum::no_obj_0)
                {
                    mPedLeader->SetStatesForObjective(true);
                }
                else
                {
                    mPedLeader->SetStatesForObjective(false);
                }
                if (pPed->get_occupation() == ped_ocupation_enum::paramedic_23)
                {
                    if (mCount34 > 0)
                    {
                        mPedList[mCount34] = pNewLeader;
                        mPedList[mCount34]->set_ped_group_id(mCount36 - 1);
                    }
                }
                else if (mCount34 > 0)
                {
                    mPedList[mCount34] = NULL;
                }
                UpdateFormation();
            }
            else
            {
                ClearGroupData();
            }
        }
    }
    else
    {
        for (u8 i = 0; i < mCount34; i++)
        {
            if (mPedList[i] == pPed)
            {
                mPedList[i] = mPedList[mCount34 - 1];
                mPedList[i]->set_ped_group_id(i);
                mPedList[mCount34 - 1] = pPed;
                pPed->set_ped_group_id(mCount34 - 1);
                if (pPed->get_occupation() != ped_ocupation_enum::paramedic_23)
                {
                    mPedList[mCount34 - 1]->ClearGroupAndGroupIdx();
                    mPedList[mCount34 - 1] = NULL;
                    if (pPed->mPedType == ped_type::script_created_5)
                    {
                        pPed->mBf21C.bScheduledForRemoval = true;
                    }
                }
                mCount34--;
                return;
            }
        }
    }
}

MATCH_FUNC(0x4c9b10)
void PedGroup::add_ped_leader(Ped* pLeader)
{
    mPedLeader = pLeader;
    mPedLeader->set_ped_group(this);
    mPedLeader->set_ped_group_id(99);
}

MATCH_FUNC(0x4c9b30)
void PedGroup::add_ped_to_list(Ped* pPed, u8 idx)
{
    mPedList[idx & 0xff] = pPed;
    pPed->set_ped_group(this);
    pPed->set_ped_group_id(idx);
}

MATCH_FUNC(0x4c9b60)
void PedGroup::GroupAttackPed(Ped* pPed)
{
    s8 i;
    mPanicking = 1;
    PedGroup* pOther = pPed->mPedGroup;
    if (!pOther)
    {
        for (i = mCount34 - 1; i >= 0; i--)
        {
            Ped* pMember = mPedList[i];
            if (!pMember->GetPanicking() && !pMember->mBf21C.bLeftVehicle && pMember->mGameObject)
            {
                if (IsMemberTooFarFromLeader(i))
                {
                    pMember->SetObjective2(objectives_enum::run_to_char_7);
                    pMember->SetInternalTargetPed(mPedLeader);
                    pMember->SetPanicking();
                }
                // Two copies of the follow-pPed body (VC6 cross-jumps them back together). The extra
                // pPed reference gives pPed edi over the list cursor.
                else if (objectives_enum::enter_car_as_driver_35 != mPedLeader->GetInternalObjective())
                {
                    pMember->SetObjective2(objectives_enum::kill_char_on_foot_20);
                    pMember->SetInternalTargetPed(pPed);
                    pMember->SetPanicking();
                }
                else if (IsLeaderCloseToTargetCar())
                {
                    pMember->SetObjective2(objectives_enum::kill_char_on_foot_20);
                    pMember->SetInternalTargetPed(pPed);
                    pMember->SetPanicking();
                }
            }
        }

        if (!mPedLeader->PedTypeIs(ped_type::player_2) && !mPedLeader->GetPanicking() && !mPedLeader->has_car() &&
            !mPedLeader->mBf21C.bLeftVehicle && !mPedLeader->mCar)
        {
            mPedLeader->SetObjective2(objectives_enum::kill_char_on_foot_20);
            mPedLeader->SetInternalTargetPed(pPed);
            mPedLeader->SetPanicking();
        }
    }
    else
    {
        for (i = mCount34 - 1; i >= 0; i--)
        {
            Ped* pMember = mPedList[i];
            if (!pMember->GetPanicking() && !pMember->mBf21C.bLeftVehicle && pMember->mGameObject)
            {
                Ped* pTarget = pOther->FindLastNonPanickingMember();
                if (pTarget)
                {
                    pMember->SetObjective2(objectives_enum::kill_char_on_foot_20);
                    pMember->SetInternalTargetPed(pTarget);
                    pMember->SetPanicking();
                    if (!pTarget->has_car())
                    {
                        pTarget->SetObjective2(objectives_enum::kill_char_on_foot_20);
                        pTarget->SetInternalTargetPed(pMember);
                        pTarget->SetPanicking();
                    }
                }
                else
                {
                    s16 max = pOther->mCount34 + 1;
                    s16 rnd = gRng.get_int(max);
                    if (rnd == pOther->mCount34)
                    {
                        if (!pOther->mPedLeader || pOther->mPedLeader->GetPedState() == ped_state_1::dead_9)
                        {
                            continue;
                        }
                        pMember->SetObjective2(objectives_enum::kill_char_on_foot_20);
                        pMember->SetInternalTargetPed(pOther->mPedLeader);
                    }
                    else
                    {
                        if (!pOther->mPedList[rnd] || pOther->mPedList[rnd]->GetPedState() == ped_state_1::dead_9)
                        {
                            continue;
                        }
                        pMember->SetObjective2(objectives_enum::kill_char_on_foot_20);
                        pMember->SetInternalTargetPed(pOther->mPedList[rnd]);
                    }
                    pMember->SetPanicking();
                }
            }
        }

        if (!mPedLeader->GetPanicking() && !pOther->mPedLeader->GetPanicking())
        {
            if (!mPedLeader->PedTypeIs(ped_type::player_2) && !mPedLeader->mBf21C.bLeftVehicle &&
                mPedLeader->mGameObject)
            {
                mPedLeader->SetObjective2(objectives_enum::kill_char_on_foot_20);
                mPedLeader->SetInternalTargetPed(pOther->mPedLeader);
                mPedLeader->SetPanicking();
            }

            if (!pOther->mPedLeader->PedTypeIs(ped_type::player_2) && !pOther->mPedLeader->mBf21C.bLeftVehicle &&
                pOther->mPedLeader->mGameObject)
            {
                pOther->mPedLeader->SetObjective2(objectives_enum::kill_char_on_foot_20);
                pOther->mPedLeader->SetInternalTargetPed(mPedLeader);
                pOther->mPedLeader->SetPanicking();
            }
        }
    }
}

MATCH_FUNC(0x4c9ed0)
Ped* PedGroup::FindLastNonPanickingMember() const
{
    for (s8 i = this->mCount34 - 1; i >= 0; i--)
    {
        Ped* pPed = mPedList[i];
        if (!pPed->IsPanicking())
        {
            return mPedList[i];
        }
    }
    return NULL;
}

MATCH_FUNC(0x4c9f00)
void PedGroup::CoordinateGroupCarEntry()
{
    s8 i;
    Fix16 distance;
    Ped* pLeader = mPedLeader;
    s32 state = pLeader->GetPedState();
    if (state == ped_state_1::entering_car_3 || state == ped_state_1::in_car_10 || state == ped_state_1::entering_train_5)
    {
        mPanicking = 1;
        if (pLeader->GetPedState() == ped_state_1::in_car_10 || pLeader->FindUsableCarDoor())
        {
            Car* pCar;
            if (mPedLeader->mGameObject)
            {
                pCar = mPedLeader->mGameObject->mTargetCar;
            }
            else
            {
                pCar = mPedLeader->mCar;
            }
            u8 passengers = pCar->GetMaxPassengers();

            for (i = 0; i < passengers && i < mCount34; i++)
            {
                Ped* pMember = mPedList[i];
                if (pMember->has_car() || pMember->isDead() ||
                    (pMember->get_occupation() == ped_ocupation_enum::paramedic_23 && pMember->get_objective() != objectives_enum::no_obj_0) || pMember->get_objective() == objectives_enum::left_behind_by_leader_8)
                {
                    continue;
                }

                if (mPedLeader->mCar && mPedLeader->mCar->IsTrainModel())
                {
                    pMember->SetObjective2(objectives_enum::enter_train_37);
                    pMember->set_target_to_enter(mPedLeader->mCar);
                    continue;
                }

                if (pMember->GetInternalObjective() != objectives_enum::enter_car_as_driver_35)
                {
                    pMember->ClearPanicking();
                    if (mPedLeader->mGameObject)
                    {
                        pMember->SetObjective2(objectives_enum::walk_to_car_door_18);
                        pMember->set_enter_car_as_passenger(1);
                        pMember->set_target_to_enter(mPedLeader->mGameObject->Get_F84());
                    }
                    else
                    {
                        pMember->SetObjective(objectives_enum::no_obj_0);
                        pMember->SetObjective2(objectives_enum::enter_car_as_driver_35);
                        pMember->set_enter_car_as_passenger(1);
                        pMember->set_target_to_enter(mPedLeader->mCar);
                    }
                    pMember->mGameObject->ClearCollisionState();
                }

                Car* pTargetCar = pMember->get_target_to_enter();
                u8 tries = 0;
                sGroupMemberCarDoor = i + 1;
                u8 maxDoor = pTargetCar->GetDoorCount() - 1;
                bool searching;
                if (pTargetCar->IsSwatVanOrBankVan())
                {
                    sGroupMemberCarDoor = i + 2;
                    do
                    {
                        searching = true;
                        do
                        {
                            if (sGroupMemberCarDoor > maxDoor)
                            {
                                sGroupMemberCarDoor += 1 - maxDoor;
                            }
                        } while (sGroupMemberCarDoor > maxDoor);

                        if (pTargetCar->IsDoorAccessible(sGroupMemberCarDoor))
                        {
                            searching = false;
                        }
                        else
                        {
                            sGroupMemberCarDoor++;
                        }

                        if (++tries == maxDoor + 1)
                        {
                            sGroupMemberCarDoor = maxDoor;
                            if (pTargetCar->IsDoorAccessible(0))
                            {
                                sGroupMemberCarDoor = 0;
                            }
                            else if (pTargetCar->IsDoorAccessible(1))
                            {
                                sGroupMemberCarDoor = 1;
                            }
                            break;
                        }
                    } while (searching);
                }
                else
                {
                    do
                    {
                        searching = true;
                        do
                        {
                            if (sGroupMemberCarDoor > maxDoor)
                            {
                                sGroupMemberCarDoor -= maxDoor;
                            }
                        } while (sGroupMemberCarDoor > maxDoor);

                        if (pTargetCar->IsDoorAccessible(sGroupMemberCarDoor))
                        {
                            searching = false;
                        }
                        else
                        {
                            sGroupMemberCarDoor++;
                        }

                        if (++tries == maxDoor + 1)
                        {
                            sGroupMemberCarDoor = maxDoor;
                            if (pTargetCar->IsDoorAccessible(0))
                            {
                                sGroupMemberCarDoor = 0;
                            }
                            break;
                        }
                    } while (searching);
                }
                for (s8 j = 0; j < mCount34; j++)
                {
                    Ped* pOther = mPedList[j];
                    if (pOther != pMember && pOther->GetInternalObjective() == objectives_enum::walk_to_car_door_18 &&
                        pMember->get_target_car_door() == pOther->get_target_car_door())
                    {
                        pMember->SetObjective2(objectives_enum::follow_char_9);
                        pMember->SetInternalTargetPed(pOther);
                    }
                }
                pMember->set_target_car_door(sGroupMemberCarDoor);
            }

            for (; i < mCount34; i++)
            {
                Ped* pMember = mPedList[i];
                if (pMember->get_objective() != objectives_enum::left_behind_by_leader_8)
                {
                    pMember->SetObjective(objectives_enum::left_behind_by_leader_8);
                    pMember->SetObjective2(objectives_enum::no_obj_0);
                }
            }
        }
        return;
    }

    if (pLeader->bHasGameObject_403B70())
    {
        bool bWaiting = false;
        if (mWaitForMembers)
        {
            Ped* pFarthest = FindFarthestMember(&distance);
            if (distance > kFpTwo_67F608 && !mPedLeader->PedTypeIs(ped_type::player_2))
            {
                if (mPedLeader->get_objective() != objectives_enum::goto_area_any_means_13 && mPedLeader->GetInternalObjective() != objectives_enum::leave_car_36)
                {
                    mPedLeader->SetObjective2(objectives_enum::follow_char_9);
                    mPedLeader->SetInternalTargetPed(pFarthest);
                    mIsLeaderWaiting = 1;
                }
                bWaiting = true;
            }
            else if (mIsLeaderWaiting == 1)
            {
                mPedLeader->SetObjective2(objectives_enum::no_obj_0);
            }
        }
        if (!bWaiting)
        {
            mIsLeaderWaiting = 0;
        }
    }

    if (!static_cast<u8>(IsLeaderPanicking()))
    {
        for (i = 0; i < mCount34; i++)
        {
            if (mPedList[i]->GetPanicking() == 1)
            {
                mPanicking = 1;
                return;
            }
        }
        mPanicking = 0;
        return;
    }
    mPanicking = 1;
}

MATCH_FUNC(0x4ca3e0)
u32 PedGroup::IsLeaderPanicking() const
{
    return mPedLeader->mBf21C.bPanicking;
}

MATCH_FUNC(0x4ca3f0)
Ped* PedGroup::FindFarthestMember(Fix16* pFoundDistance) const
{
    Fix16 max_distance = Fix16(0);
    u8 found_index = 0;
    Fix16 x_abs;
    Fix16 y_abs;
    for (u8 i = 0; i < mCount34; i++)
    {
        Fix16 x_diff = mPedLeader->get_cam_x() - mPedList[i]->get_cam_x();
        Fix16 y_diff = mPedLeader->get_cam_y() - mPedList[i]->get_cam_y();

        x_abs = Fix16::Abs(x_diff);
        y_abs = Fix16::Abs(y_diff);

        Fix16 curr_distance = (x_abs > y_abs) ? x_abs : y_abs;

        if (curr_distance > max_distance)
        {
            max_distance = curr_distance;
            found_index = i;
        }
    }
    *pFoundDistance = max_distance;
    return mPedList[found_index];
}

MATCH_FUNC(0x4ca4b0)
void PedGroup::UpdateFormation()
{
    if (!mPedLeader->mCar && !mPedLeader->mBf21C.bLeftVehicle)
    {
        for (u8 i = 0; i < mCount34; i++)
        {
            Ped* pIter = mPedList[i];
            if (pIter->GetInternalObjective() != objectives_enum::follow_char_9)
            {
                if (pIter->mGameObject)
                {
                    pIter->SetObjective2(objectives_enum::follow_char_9);
                }
            }
            if (mGroupType == ped_group_type::chain_1)
            {
                if (i == 0)
                {
                    pIter->SetInternalTargetPed(mPedLeader);
                }
                else
                {
                    pIter->SetInternalTargetPed(mPedList[i - 1]);
                }
            }
            else if (!pIter->isDead())
            {
                if (mPedLeader->GetPedVelocity() != kFpZero_67F610)
                {
                    switch (i)
                    {
                        case 3u:
                            pIter->SetInternalTargetPed(this->mPedList[0]);
                            break;
                        case 4u:
                            pIter->SetInternalTargetPed(this->mPedList[1]);
                            break;
                        case 5u:
                            pIter->SetInternalTargetPed(this->mPedList[2]);
                            break;
                        case 6u:
                            pIter->SetInternalTargetPed(this->mPedList[3]);
                            break;
                        case 7u:
                            pIter->SetInternalTargetPed(this->mPedList[4]);
                            break;
                        default:
                            pIter->SetInternalTargetPed(mPedLeader);
                            break;
                    }
                }
                else
                {
                    pIter->SetInternalTargetPed(mPedLeader);
                }
            }
            pIter->ClearAttacking();
        }
    }
}

MATCH_FUNC(0x4ca5e0)
void PedGroup::UpdateMemberAIState(u8 idx)
{
    Ped* pMember = mPedList[idx];
    if (pMember->mBf21C.bLeftVehicle)
    {
        return;
    }

    if (pMember->get_objective() == objectives_enum::left_behind_by_leader_8)
    {
        if (pMember->GetInternalObjective() == objectives_enum::follow_char_9)
        {
            pMember->SetObjective2(objectives_enum::no_obj_0);
        }
        return;
    }

    if (!mPedLeader->PedTypeIs(ped_type::player_2))
    {
        pMember->mThreatSearch = mPedLeader->mThreatSearch;
        pMember->mThreatReaction = mPedLeader->mThreatReaction;
        pMember->mpGang = mPedLeader->mpGang;
    }
    else
    {
        s32 occupation = pMember->get_occupation();
        if (occupation != ped_ocupation_enum::drone)
        {
            if (occupation != ped_ocupation_enum::refugees)
            {
                pMember->mThreatSearch = threat_search_enum::line_of_sight_1;
                pMember->mThreatReaction = threat_reaction_enum::react_as_normal_2;
            }
            else
            {
                pMember->mThreatSearch = threat_search_enum::area_2;
                pMember->mThreatReaction = threat_reaction_enum::no_reaction_0;
            }
        }
    }

    Ped* pLeader = mPedLeader;
    if (pLeader->bHasGameObject_403B70())
    {
        if (pMember->bHasGameObject_403B70())
        {
            if (!pLeader->mBf21C.bLeftVehicle)
            {
                Ped* pNearest = FindNearestOtherMember(idx);
                if (pNearest)
                {
                    pMember->SetObjective2(objectives_enum::kill_char_on_foot_20);
                    pMember->SetInternalTargetPed(pNearest->GetInternalTargetPed());
                    pMember->SetPanicking();
                }
                else if (pMember->get_objective() != objectives_enum::left_behind_by_leader_8)
                {
                    if (pMember->get_occupation() != ped_ocupation_enum::paramedic_23 || pMember->get_objective() != objectives_enum::goto_char_on_foot_16)
                    {
                        UpdateFormation();
                    }
                    if (mPedLeader->GetInternalObjective() == objectives_enum::destroy_car_59)
                    {
                        pMember->SetObjective2(objectives_enum::destroy_car_59);
                        pMember->set_target_to_enter(mPedLeader->get_target_to_enter());
                    }
                }
                else if (pMember->GetInternalObjective() == objectives_enum::follow_char_9)
                {
                    pMember->SetObjective(objectives_enum::no_obj_0);
                }
            }
        }
        else if (pMember->GetInternalObjective() != objectives_enum::leave_car_36)
        {
            pMember->SetObjective2(objectives_enum::leave_car_36);
            pMember->set_target_to_enter(pMember->mCar);
        }
    }
    else if (pMember->GetInternalObjective() == objectives_enum::follow_char_9)
    {
        s32 occupation = pMember->get_occupation();
        if ((occupation < ped_ocupation_enum::police || occupation > ped_ocupation_enum::army_army) && pMember->GetInternalTargetPed()->has_car() && pMember->mObjective != objectives_enum::left_behind_by_leader_8)
        {
            pMember->SetObjective(objectives_enum::left_behind_by_leader_8);
            pMember->SetObjective2(objectives_enum::no_obj_0);
        }
    }

    Ped* pFollow = pMember->mInternalTargetPed;
    if (pFollow && pMember->mInternalObjective == objectives_enum::stay_close_to_char_11 && !pFollow->mGameObject)
    {
        pMember->SetObjective2(objectives_enum::no_obj_0);
    }
}

MATCH_FUNC(0x4ca820)
void PedGroup::UpdateMemberTightFollowState(u8 idx)
{
    Ped* pMember = mPedList[idx];
    if (mPedLeader->mGameObject)
    {
        Fix16 x_diff = mPedLeader->mCam.x - pMember->mCam.x;
        Fix16 y_diff = mPedLeader->mCam.y - pMember->mCam.y;
        Fix16 x_abs = Fix16::Abs(x_diff);
        Fix16 y_abs = Fix16::Abs(y_diff);
        if (Fix16::Max(x_abs, y_abs) < kFpEight)
        {
            pMember->SetObjective2(objectives_enum::follow_char_9);
            pMember->SetInternalTargetPed(mPedLeader);
            pMember->ClearAttacking();
            pMember->SetObjective(objectives_enum::no_obj_0);
        }
        else if (pMember->mInternalTargetPed == mPedLeader && pMember->mInternalObjective == objectives_enum::follow_char_9)
        {
            pMember->SetObjective2(objectives_enum::no_obj_0);
        }
    }
    else if (pMember->mInternalTargetPed == mPedLeader && pMember->mInternalObjective == objectives_enum::follow_char_9)
    {
        pMember->SetObjective2(objectives_enum::no_obj_0);
    }
}

MATCH_FUNC(0x4caa20)
bool PedGroup::IsAllMembersInSomeCar() const
{
    if (mCount34 == 0)
    {
        return mPedLeader->mCar != NULL;
    }

    for (u8 i = 0; i < mCount34; i++)
    {
        if (mPedList[i]->GetPedState() != ped_state_1::in_car_10)
        {
            return false;
        }
    }
    return true;
}

MATCH_FUNC(0x4caae0)
bool PedGroup::HasNoActiveMembers() const
{
    for (u8 i = 0; i < mCount34; i++)
    {
        if (mPedList[i]->mPedState1 != ped_state_1::in_car_10 &&
            mPedList[i]->mPedState1 != ped_state_1::dead_9)
        {
            return false;
        }
    }
    return true;
}

MATCH_FUNC(0x4cab80)
bool PedGroup::AreAllMembersOnFoot() const
{
    for (u8 i = 0; i < mCount34; i++)
    {
        if (mPedList[i]->mCar)
        {
            return false;
        }
    }
    return true;
}

MATCH_FUNC(0x4cac20)
bool PedGroup::IsMemberTooFarFromLeader(u8 idx) const
{
    Ped* pMember = mPedList[idx];
    Fix16 x_diff = pMember->mCam.x - mPedLeader->mCam.x;
    Fix16 y_diff = pMember->mCam.y - mPedLeader->mCam.y;

    Fix16 x_abs = Fix16::Abs(x_diff);
    Fix16 y_abs = Fix16::Abs(y_diff);

    Fix16 max_distance = (x_abs > y_abs) ? x_abs : y_abs;

    if (max_distance > kFpFour)
    {
        return true;
    }
    return false;
}

MATCH_FUNC(0x4cad40)
bool PedGroup::IsLeaderCloseToTargetCar() const
{
    Fix16 leader_xpos = mPedLeader->get_cam_x();
    Car* pTargetCar = mPedLeader->get_target_to_enter();
    if (!pTargetCar)
    {
        pTargetCar = mPedLeader->get_target_objective_car();
    }

    Fix16 x_diff = leader_xpos - pTargetCar->mCarSprite->mXy.x;
    Fix16 y_diff = mPedLeader->get_cam_y() - pTargetCar->mCarSprite->mXy.y;

    Fix16 x_abs = Fix16::Abs(x_diff);
    Fix16 y_abs = Fix16::Abs(y_diff);

    Fix16 max_distance = (x_abs > y_abs) ? x_abs : y_abs;

    if (max_distance <= kFpFour)
    {
        return true;
    }
    return false;
}

// The loop is a do/while under its own entry test: an empty group skips the final
// nearest_distance check. Reading pOther's x directly (not through get_cam_x) loads
// pMember's x first, as in the original.
MATCH_FUNC(0x4cae80)
Ped* PedGroup::FindNearestOtherMember(u8 idx) const
{
    Fix16 x_abs;
    Fix16 y_abs;
    Fix16 nearest_distance = kNoNearestDistance;
    u8 nearest_idx = 0;
    Ped* pMember = mPedList[idx];
    u8 i = 0;
    if (i < mCount34)
    {
        do
        {
            Ped* pOther = mPedList[i];
            if (i != idx)
            {
                Fix16 x_diff = pMember->get_cam_x() - pOther->mCam.x;
                Fix16 y_diff = pMember->get_cam_y() - pOther->get_cam_y();

                x_abs = Fix16::Abs(x_diff);
                y_abs = Fix16::Abs(y_diff);

                Fix16 distance = (x_abs > y_abs) ? x_abs : y_abs;
                if (distance < nearest_distance && pOther->IsAttackingTargetPed())
                {
                    nearest_distance = distance;
                    nearest_idx = i;
                }
            }
            i++;
        } while (i < mCount34);

        if (nearest_distance != kNoNearestDistance)
        {
            return mPedList[nearest_idx];
        }
    }
    return NULL;
}

MATCH_FUNC(0x4cb080)
void PedGroup::ResetAllGroups()
{
    ResetMemberCarDoor();

    PedGroup* pIter = pedGroups;
    for (s32 i = 0; i < 20; i++)
    {
        pIter->Reset();
        pIter++;
    }
}

MATCH_FUNC(0x4cb0d0)
PedGroup* PedGroup::New()
{
    for (s32 i = 0; i < 20; i++)
    {
        if (!pedGroups[i].get_in_use())
        {
            pedGroups[i].Reset();
            pedGroups[i].set_in_use();
            return &pedGroups[i];
        }
    }
    return NULL;
}

MATCH_FUNC(0x4cb860)
PedGroup::PedGroup()
{
    Reset();
}

MATCH_FUNC(0x4cb870)
PedGroup::~PedGroup()
{
}