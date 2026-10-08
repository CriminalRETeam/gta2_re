#include "PedGroup.hpp"
#include "Car_BC.hpp"
#include "Char_Pool.hpp"
#include "Globals.hpp"
#include "Ped.hpp"
#include "enums.hpp"
#include "rng.hpp"

DEFINE_GLOBAL_ARRAY(PedGroup, pedGroups_67EF20, 20, 0x67EF20);
DEFINE_GLOBAL_INIT(Fix16, dword_67F60C, Fix16(0x270F00, 0), 0x67F60C);
DEFINE_GLOBAL_INIT(Fix16, dword_67F608, Fix16(2), 0x67F608);
DEFINE_GLOBAL(Fix16, dword_67F610, 0x67F610);
DEFINE_GLOBAL_INIT(Fix16, dword_67F670, Fix16(8), 0x67F670);
DEFINE_GLOBAL_INIT(Fix16, k_dword_67EEE4, Fix16(0x500, 0), 0x67EEE4);
DEFINE_GLOBAL_INIT(char_type, byte_620838, 1, 0x620838);
DEFINE_GLOBAL_INIT(Fix16, dword_67F630, Fix16(4), 0x67F630);

// The original is the static destructor of pedGroups_67EF20 (a ??_M vector destructor call).
// A struct wrapping the array gets an implicit destructor with that ??_M call, but VC6 doesn't
// inline it (not even with an explicit __forceinline destructor), see docs/match_attempts.md.
struct PedGroupArray_4C8E60
{
    PedGroup field_0_groups[20];
};

WIP_FUNC(0x4c8e60)
void PedGroup::sub_4C8E60()
{
    WIP_IMPLEMENTED;
    reinterpret_cast<PedGroupArray_4C8E60*>(pedGroups_67EF20)->~PedGroupArray_4C8E60();
}

MATCH_FUNC(0x4c8e80)
void PedGroup::sub_4C8E80()
{
    byte_620838 = 1;
}

MATCH_FUNC(0x4c8e90)
void PedGroup::ClearGroupData_4C8E90()
{
    field_40_in_use = false;
    field_38_group_type = 2;
    field_30 = 0;
    field_36_count = 0;
    field_34_count = 0;
    if (field_2C_ped_leader != NULL)
    {
        field_2C_ped_leader->ClearGroupAndGroupIdx_403A30();
    }
    field_2C_ped_leader = NULL;
    field_0 = 1;
    field_1 = 0;
    field_34_count = 0;
    field_35 = 0;

    for (int i = 0; i < 9; ++i)
    {
        if (field_4_ped_list[i] != NULL)
        {
            field_4_ped_list[i]->ClearGroupAndGroupIdx_403A30();
        }
        field_4_ped_list[i] = NULL;
    }
}

MATCH_FUNC(0x4c8ef0)
void PedGroup::Reset_4C8EF0()
{
    field_40_in_use = 0;
    field_38_group_type = 2;
    field_30 = 0;
    field_36_count = 0;
    field_2C_ped_leader = NULL;
    field_0 = 1;
    field_1 = 0;
    field_34_count = 0;
    field_35 = 0;
    for (s32 i = 0; i < 9; i++)
    {
        field_4_ped_list[i] = 0;
    }
}

MATCH_FUNC(0x4c8f20)
void PedGroup::ResetGroupObjectives_4C8F20()
{
    field_2C_ped_leader->SetObjective(objectives_enum::no_obj_0, 9999);
    field_2C_ped_leader->SetObjective2_463830(objectives_enum::no_obj_0, 9999);

    u8 bVar1 = 0;

    while (bVar1 < field_34_count)
    {
        field_4_ped_list[bVar1]->SetObjective(objectives_enum::no_obj_0, 9999);
        field_4_ped_list[bVar1]->SetObjective2_463830(objectives_enum::no_obj_0, 9999);
        bVar1++;
    }
}

MATCH_FUNC(0x4c8f90)
void PedGroup::add_ped_to_end_of_list_4C8F90(Ped* pPed)
{
    pPed->SetObjective2_463830(objectives_enum::no_obj_0, 9999);
    pPed->SetObjective(objectives_enum::no_obj_0, 9999);
    add_ped_to_list_4C9B30(pPed, field_34_count);
    ++field_34_count;
    ++field_36_count;
}

MATCH_FUNC(0x4c8fe0)
void PedGroup::replace_leader_4C8FE0(Ped* new_leader)
{
    add_ped_to_end_of_list_4C8F90(field_2C_ped_leader);
    field_2C_ped_leader = new_leader;
    new_leader->set_ped_group_id(99);

    for (u8 i = 0; i < field_34_count; i++)
    {
        field_4_ped_list[i]->SetObjective2_463830(objectives_enum::no_obj_0, 9999);
    }
}

MATCH_FUNC(0x4c9040)
bool PedGroup::PurgeMembersInCars_4C9040()
{
    field_36_count = field_34_count;
    if (field_2C_ped_leader->field_16C_car != NULL)
    {
        field_2C_ped_leader->Kill_46F9D0();
        field_2C_ped_leader = NULL;
    }

    u8 bVar2;
    for (bVar2 = 0; bVar2 < field_34_count; bVar2++)
    {
        if (field_4_ped_list[bVar2]->field_16C_car != NULL)
        {
            field_4_ped_list[bVar2]->Kill_46F9D0();
            field_4_ped_list[bVar2] = NULL;
        }
    }

    // Compacting the ped_list
    for (bVar2 = 0; bVar2 < field_34_count; bVar2++)
    {
        if (field_4_ped_list[bVar2] == NULL)
        {
            for (u8 i = bVar2; i < field_34_count; i++)
            {
                if (field_4_ped_list[i] != NULL)
                {
                    field_4_ped_list[bVar2] = field_4_ped_list[i];
                    field_4_ped_list[i] = NULL;
                    break;
                }
            }
        }
    }

    bVar2 = 0;
    while (field_4_ped_list[bVar2] != NULL)
    {
        bVar2++;
    }

    if (bVar2 == 0)
    {
        return 0;
    }
    return 1;
}

MATCH_FUNC(0x4c9150)
char_type PedGroup::AreAllMembersOffScreen_4C9150()
{
    if (field_2C_ped_leader->field_168_game_object == NULL || field_2C_ped_leader->GetOffscreenCounter_4039F0() < 0x28)
    {
        return false;
    }

    for (u8 i = 0; i < field_34_count; i++)
    {
        if (field_4_ped_list[i]->field_168_game_object == NULL || field_4_ped_list[i]->GetOffscreenCounter_4039F0() < 0x28)
        {
            return false;
        }
    }

    return true;
}

MATCH_FUNC(0x4c91b0)
void PedGroup::ResetMembersToFollowLeader_4C91B0()
{
    for (u8 i = 0; i < field_34_count; i++)
    {
        field_4_ped_list[i]->ClearPanicking_403960();
        field_4_ped_list[i]->SetObjective2_463830(objectives_enum::objective_9, 9999);
        field_4_ped_list[i]->SetInternalTargetPed_403AE0(field_2C_ped_leader);
    }
}

MATCH_FUNC(0x4c9210)
bool PedGroup::IsLeaderInCar_4C9210()
{
    if (field_2C_ped_leader->field_16C_car != NULL)
    {
        return true;
    }
    else
    {
        return false;
    }
}

MATCH_FUNC(0x4c9220)
bool PedGroup::IsLeaderEnteringCarOrUnknown5_4C9220()
{
    if (field_2C_ped_leader->get_ped_state1() != ped_state1_enum::ped_entering_a_car)
    {
        if (field_2C_ped_leader->get_ped_state1() != ped_state1_enum::unused2)
        {
            return false;
        }
    }
    return true;
}

MATCH_FUNC(0x4c9240)
void PedGroup::KillEntireGroup_4C9240()
{
    field_2C_ped_leader->ClearGroupAndGroupIdx_403A30();
    field_2C_ped_leader->Kill_46F9D0();
    for (char_type i = 0; i < field_34_count; i++)
    {
        field_4_ped_list[i]->ClearGroupAndGroupIdx_403A30();
        field_4_ped_list[i]->Kill_46F9D0();
    }
    this->field_40_in_use = 0;
}


MATCH_FUNC(0x4c92a0)
void PedGroup::DisbandGroup_4C92A0()
{
    if (field_40_in_use == false)
    {
        return;
    }

    field_2C_ped_leader->ClearGroupAndGroupIdx_403A30();
    if (field_4_ped_list[0] != NULL)
    {
        for (u8 bVar4 = 0; bVar4 < field_34_count; bVar4++)
        {
            Ped* this_00 = field_4_ped_list[bVar4];
            Ped** pppVar1 = field_4_ped_list + bVar4;
            if ((this_00->GetPedState_403990() == ped_state1_enum::ped_wasted) ||
                (this_00->field_280_stored_ped_state_1 == ped_state1_enum::ped_wasted))
            {
                this_00->ClearGroupAndGroupIdx_403A30();
            }
            else
            {
                if (this_00->has_car_403B80())
                {
                    this_00->SetObjective(objectives_enum::objective_34, 9999);
                    (*pppVar1)->SetTargetObjectiveCar((*pppVar1)->field_16C_car);
                }
                else
                {
                    this_00->SetObjective(objectives_enum::no_obj_0, 9999);
                }
                (*pppVar1)->SetObjective2_463830(objectives_enum::no_obj_0, 9999);
                (*pppVar1)->ClearGroupAndGroupIdx_403A30();
                (*pppVar1)->SetField238_403920(ped_type_enum::New_Name_2);
            }
            (*pppVar1)->field_21C |= ped_flag_mask::k_ped_scheduled_for_removal;
        }
    }
    ClearGroupData_4C8E90();
}

MATCH_FUNC(0x4c93a0)
void PedGroup::DestroyGroup_4C93A0()
{
    if (field_40_in_use == false)
    {
        return;
    }

    Ped* ppVar2 = field_2C_ped_leader;
    if ((ppVar2->GetPedState_403990() != ped_state1_enum::ped_wasted) && (ppVar2->field_280_stored_ped_state_1 != ped_state1_enum::ped_wasted))
    {
        ppVar2->SetObjective(objectives_enum::no_obj_0, 9999);
        field_2C_ped_leader->SetObjective2_463830(objectives_enum::no_obj_0, 9999);
    }

    field_2C_ped_leader->ClearGroupAndGroupIdx_403A30();
    if (field_4_ped_list[0] != NULL)
    {
        for (u8 bVar5 = 0; bVar5 < field_34_count; bVar5++)
        {
            ppVar2 = field_4_ped_list[bVar5];
            Ped** pppVar1 = field_4_ped_list + bVar5;
            if ((ppVar2->GetPedState_403990() == ped_state1_enum::ped_wasted) ||
                (ppVar2->field_280_stored_ped_state_1 == ped_state1_enum::ped_wasted))
            {
                ppVar2->ClearGroupAndGroupIdx_403A30();
            }
            else
            {
                if (ppVar2->has_car_403B80() == true)
                {
                    ppVar2->SetObjective2_463830(objectives_enum::no_obj_0, 9999);
                    (*pppVar1)->SetObjective(objectives_enum::objective_34, 9999);
                    (*pppVar1)->SetTargetObjectiveCar((*pppVar1)->field_16C_car);
                    (*pppVar1)->ClearGroupAndGroupIdx_403A30();
                    (*pppVar1)->SetField238_403920(ped_type_enum::New_Name_2);
                }
                else
                {
                    ppVar2->SetObjective(objectives_enum::no_obj_0, 9999);
                    (*pppVar1)->SetObjective2_463830(objectives_enum::no_obj_0, 9999);
                    (*pppVar1)->ClearGroupAndGroupIdx_403A30();
                    (*pppVar1)->SetField238_403920(ped_type_enum::New_Name_2);
                }
            }
        }
    }
    ClearGroupData_4C8E90();
    return;
}

MATCH_FUNC(0x4c94e0)
void PedGroup::DisbandGroupDueToAttack_4C94E0(Ped* pAttacker)
{
    // 9.6f: Ped 0x403A50 sets field_168_game_object->field_3C_run_or_jump_speed (inlined, a Ped helper for it
    // changes the code)
    if (!pAttacker)
    {
        PedGroup::DestroyGroup_4C93A0();
    }
    else
    {
        if (!field_2C_ped_leader->PedTypeIs_45EDE0(ped_type::player_2))
        {
            this->field_2C_ped_leader->SetObjective(objectives_enum::flee_char_on_foot_always_3, 9999);
            this->field_2C_ped_leader->set_objective_target_ped_403AC0(pAttacker);
            this->field_2C_ped_leader->SetObjective2_463830(3, 9999);
            this->field_2C_ped_leader->SetInternalTargetPed_403AE0(pAttacker);
            this->field_2C_ped_leader->SetPanicking_403950();
            this->field_2C_ped_leader->ClearHitCount_403A20();
            this->field_2C_ped_leader->field_168_game_object->field_3C_run_or_jump_speed = k_dword_67EEE4;
        }

        this->field_2C_ped_leader->ClearGroupAndGroupIdx_403A30();

        for (char_type i_ = 0; i_ < (s32)this->field_34_count; i_++)
        {
            s32 i = i_;
            if (field_4_ped_list[i]->has_car_403B80())
            {
                this->field_4_ped_list[i]->SetObjective2_463830(objectives_enum::no_obj_0, 9999);
                this->field_4_ped_list[i]->SetObjective(objectives_enum::flee_char_always_once_car_stopped_6, 9999);
                this->field_4_ped_list[i]->set_objective_target_ped_403AC0(pAttacker);
                this->field_4_ped_list[i]->ClearHitCount_403A20();
                this->field_4_ped_list[i]->ClearGroupAndGroupIdx_403A30();
            }
            else
            {
                this->field_4_ped_list[i]->SetObjective(objectives_enum::flee_char_on_foot_always_3, 9999);
                this->field_4_ped_list[i]->set_objective_target_ped_403AC0(pAttacker);
                this->field_4_ped_list[i]->SetObjective2_463830(3, 9999);
                this->field_4_ped_list[i]->SetInternalTargetPed_403AE0(pAttacker);
                this->field_4_ped_list[i]->SetPanicking_403950();
                this->field_4_ped_list[i]->ClearHitCount_403A20();
                this->field_4_ped_list[i]->field_168_game_object->field_3C_run_or_jump_speed = k_dword_67EEE4;
                this->field_4_ped_list[i]->ClearGroupAndGroupIdx_403A30();
                this->field_4_ped_list[i]->SetField238_403920(ped_type::dummy_3);
            }
        }
        PedGroup::ClearGroupData_4C8E90();
    }
}

MATCH_FUNC(0x4c9680)
void PedGroup::PromoteMemberToLeader_4C9680(u8 idx)
{
    Ped* pTmp = gPedPool_6787B8->Allocate();

    Weapon_30* leaderWeapon = field_2C_ped_leader->field_170_selected_weapon;
    Weapon_30* memberWeapon = field_4_ped_list[idx]->field_170_selected_weapon;
    Weapon_30* leaderWeapon2 = field_2C_ped_leader->field_174_pWeapon;
    Weapon_30* memberWeapon2 = field_4_ped_list[idx]->field_174_pWeapon;

    pTmp->CopyStatsFromPed_45B5B0(field_2C_ped_leader);
    field_2C_ped_leader->CopyStatsFromPed_45B5B0(field_4_ped_list[idx]);
    field_2C_ped_leader->field_170_selected_weapon = leaderWeapon;
    field_2C_ped_leader->field_174_pWeapon = leaderWeapon2;

    field_2C_ped_leader->SetObjective(pTmp->get_objective_403A80(), pTmp->get_objective_timer_403B30());
    field_2C_ped_leader->SetObjective2_463830(pTmp->GetInternalObjective_403A90(), pTmp->get_car_state_timer_403B20());
    field_2C_ped_leader->set_objective_status_403B40(pTmp->GetObjectiveStatus_450CB0());
    field_2C_ped_leader->SetInternalObjectiveStatus_403B50(pTmp->GetInternalObjectiveStatus_4039D0());
    field_2C_ped_leader->set_objective_target_ped_403AC0(pTmp->get_objective_target_ped_403AD0());
    field_2C_ped_leader->SetTargetObjectiveCar(pTmp->get_target_objective_car_403AB0());
    field_2C_ped_leader->field_1A0_objective_target_object = pTmp->field_1A0_objective_target_object;
    field_2C_ped_leader->field_1A4_internal_target_object = pTmp->field_1A4_internal_target_object;
    field_2C_ped_leader->field_1DC_objective_target_x = pTmp->field_1DC_objective_target_x;
    field_2C_ped_leader->field_1E0_objective_target_y = pTmp->field_1E0_objective_target_y;
    field_2C_ped_leader->field_1E4_objective_target_z = pTmp->field_1E4_objective_target_z;
    field_2C_ped_leader->SetInternalTargetPed_403AE0(pTmp->GetInternalTargetPed_403AF0());
    field_2C_ped_leader->set_target_to_enter_403B00(pTmp->get_target_to_enter_403B10());
    field_2C_ped_leader->field_1D0_internal_target_x = pTmp->field_1D0_internal_target_x;
    field_2C_ped_leader->field_1D4_internal_target_y = pTmp->field_1D4_internal_target_y;
    field_2C_ped_leader->field_1D8_internal_target_z = pTmp->field_1D8_internal_target_z;
    field_2C_ped_leader->field_23C_group_idx = 99;
    field_2C_ped_leader->set_enter_car_as_passenger_4039B0(pTmp->get_enter_car_as_passenger_4039C0());
    field_2C_ped_leader->set_target_car_door_403A70(pTmp->get_target_car_door_403A60());

    Ped* pLeader = field_2C_ped_leader;
    if (pLeader->field_168_game_object)
    {
        pLeader->field_168_game_object->field_7C_pPed = pLeader;
    }
    else if (!pLeader->get_enter_car_as_passenger_4039C0() && pLeader->field_16C_car->field_54_driver != pLeader)
    {
        pLeader->set_enter_car_as_passenger_4039B0(1);
    }

    field_4_ped_list[idx]->CopyStatsFromPed_45B5B0(pTmp);
    field_4_ped_list[idx]->field_23C_group_idx = idx;
    field_4_ped_list[idx]->field_170_selected_weapon = memberWeapon;
    field_4_ped_list[idx]->field_174_pWeapon = memberWeapon2;

    Ped* pMember = field_4_ped_list[idx];
    if (pMember->field_168_game_object && pMember->field_240_occupation == 0x17)
    {
        pMember->field_168_game_object->field_7C_pPed = pMember;
        field_4_ped_list[idx]->set_enter_car_as_passenger_4039B0(1);
    }
    else
    {
        if (pMember->field_168_game_object)
        {
            pMember->field_168_game_object->field_7C_pPed = pMember;
        }

        if (idx < field_34_count - 1)
        {
            field_4_ped_list[idx]->ClearGroupAndGroupIdx_403A30();
            field_4_ped_list[idx] = field_4_ped_list[field_34_count - 1];
            field_4_ped_list[idx]->field_23C_group_idx = idx;
        }
        else
        {
            field_4_ped_list[idx]->field_164_ped_group = 0;
            field_4_ped_list[idx] = 0;
        }
        field_34_count--;
        field_2C_ped_leader->field_23C_group_idx = 99;
    }

    if (pTmp->field_238_ped_type == 5)
    {
        pTmp->PoolAllocate();
        pTmp->field_21C |= ped_flag_mask::k_ped_scheduled_for_removal;
    }
    else
    {
        pTmp->PoolAllocate();
    }
    pTmp->set_health_4039A0(100);
    pTmp->set_occupation_403970(2);
    pTmp->Kill_46F9D0();
}

MATCH_FUNC(0x4c9970)
void PedGroup::RemovePed_4C9970(Ped* pPed)
{
    if (pPed == field_2C_ped_leader)
    {
        if (!field_2C_ped_leader->PedTypeIs_45EDE0(ped_type::player_2))
        {
            if (field_2C_ped_leader->GetPedState_403990() == ped_state_1::dead_9 && field_2C_ped_leader->field_238_ped_type == 5)
            {
                field_2C_ped_leader->field_21C |= ped_flag_mask::k_ped_scheduled_for_removal;
            }

            Ped* pNewLeader = field_4_ped_list[0];
            if (pNewLeader && !pNewLeader->isDead_403B60())
            {
                PromoteMemberToLeader_4C9680(0);
                if (field_2C_ped_leader->GetInternalObjective_403A90() == 0)
                {
                    field_2C_ped_leader->SetStatesForObjective_4633E0(1);
                }
                else
                {
                    field_2C_ped_leader->SetStatesForObjective_4633E0(0);
                }
                if (pPed->get_occupation_403980() == 0x17)
                {
                    if (field_34_count > 0)
                    {
                        field_4_ped_list[field_34_count] = pNewLeader;
                        field_4_ped_list[field_34_count]->field_23C_group_idx = field_36_count - 1;
                    }
                }
                else if (field_34_count > 0)
                {
                    field_4_ped_list[field_34_count] = 0;
                }
                UpdateFormation_4CA4B0();
            }
            else
            {
                ClearGroupData_4C8E90();
            }
        }
    }
    else
    {
        for (u8 i = 0; i < field_34_count; i++)
        {
            if (field_4_ped_list[i] == pPed)
            {
                field_4_ped_list[i] = field_4_ped_list[field_34_count - 1];
                field_4_ped_list[i]->field_23C_group_idx = i;
                field_4_ped_list[field_34_count - 1] = pPed;
                pPed->field_23C_group_idx = field_34_count - 1;
                if (pPed->get_occupation_403980() != 0x17)
                {
                    field_4_ped_list[field_34_count - 1]->ClearGroupAndGroupIdx_403A30();
                    field_4_ped_list[field_34_count - 1] = 0;
                    if (pPed->field_238_ped_type == 5)
                    {
                        pPed->field_21C |= ped_flag_mask::k_ped_scheduled_for_removal;
                    }
                }
                field_34_count--;
                return;
            }
        }
    }
}

MATCH_FUNC(0x4c9b10)
void PedGroup::add_ped_leader_4C9B10(Ped* ptr)
{
    field_2C_ped_leader = ptr;
    field_2C_ped_leader->set_ped_group(this);
    field_2C_ped_leader->set_ped_group_id(99);
}

MATCH_FUNC(0x4c9b30)
void PedGroup::add_ped_to_list_4C9B30(Ped* ptr, u8 idx)
{
    field_4_ped_list[idx & 0xff] = ptr;
    ptr->set_ped_group(this);
    ptr->set_ped_group_id(idx);
}

// The original never sets a return value
#pragma warning(push)
#pragma warning(disable : 4716)
WIP_FUNC(0x4c9b60)
char_type PedGroup::MergeWithOtherGroup_4C9B60(Ped* pPed)
{
    s8 i;
    if (!pPed->field_164_ped_group)
    {
        field_30 = 1;
        for (i = field_34_count - 1; i >= 0; i--)
        {
            Ped* pMember = field_4_ped_list[i];
            if (!pMember->GetPanicking() && !pMember->field_21C_bf.bLeftVehicle && pMember->field_168_game_object)
            {
                if (IsMemberTooFarFromLeader_4CAC20(i))
                {
                    pMember->SetObjective2_463830(7, 9999);
                    pMember->SetInternalTargetPed_403AE0(field_2C_ped_leader);
                }
                else
                {
                    if (0x23 == field_2C_ped_leader->GetInternalObjective_403A90() && !IsLeaderCloseToTargetCar_4CAD40())
                    {
                        continue;
                    }
                    pMember->SetObjective2_463830(20, 9999);
                    pMember->SetInternalTargetPed_403AE0(pPed);
                }
                pMember->SetPanicking_403950();
            }
        }

        if (!field_2C_ped_leader->PedTypeIs_45EDE0(ped_type::player_2) && !field_2C_ped_leader->GetPanicking() && !field_2C_ped_leader->has_car_403B80() &&
            !field_2C_ped_leader->field_21C_bf.bLeftVehicle && !field_2C_ped_leader->field_16C_car)
        {
            field_2C_ped_leader->SetObjective2_463830(20, 9999);
            field_2C_ped_leader->SetInternalTargetPed_403AE0(pPed);
            field_2C_ped_leader->SetPanicking_403950();
        }
    }
    else
    {
        field_30 = 1;
        PedGroup* pOther = pPed->field_164_ped_group;
        for (i = field_34_count - 1; i >= 0; i--)
        {
            Ped* pMember = field_4_ped_list[i];
            if (!pMember->GetPanicking() && !pMember->field_21C_bf.bLeftVehicle && pMember->field_168_game_object)
            {
                Ped* pTarget = pOther->sub_4C9ED0();
                if (pTarget)
                {
                    pMember->SetObjective2_463830(20, 9999);
                    pMember->SetInternalTargetPed_403AE0(pTarget);
                    pMember->SetPanicking_403950();
                    if (!pTarget->has_car_403B80())
                    {
                        pTarget->SetObjective2_463830(20, 9999);
                        pTarget->SetInternalTargetPed_403AE0(pMember);
                        pTarget->SetPanicking_403950();
                    }
                }
                else
                {
                    s16 max = pOther->field_34_count + 1;
                    s16 rnd = gRng_6F6784.get_int_4F7AE0(max);
                    if (rnd == pOther->field_34_count)
                    {
                        if (!pOther->field_2C_ped_leader || pOther->field_2C_ped_leader->GetPedState_403990() == ped_state_1::dead_9)
                        {
                            continue;
                        }
                        pMember->SetObjective2_463830(20, 9999);
                        pMember->SetInternalTargetPed_403AE0(pOther->field_2C_ped_leader);
                    }
                    else
                    {
                        if (!pOther->field_4_ped_list[rnd] || pOther->field_4_ped_list[rnd]->GetPedState_403990() == ped_state_1::dead_9)
                        {
                            continue;
                        }
                        pMember->SetObjective2_463830(20, 9999);
                        pMember->SetInternalTargetPed_403AE0(pOther->field_4_ped_list[rnd]);
                    }
                    pMember->SetPanicking_403950();
                }
            }
        }

        if (!field_2C_ped_leader->GetPanicking() && !pOther->field_2C_ped_leader->GetPanicking())
        {
            if (!field_2C_ped_leader->PedTypeIs_45EDE0(ped_type::player_2) && !field_2C_ped_leader->field_21C_bf.bLeftVehicle &&
                field_2C_ped_leader->field_168_game_object)
            {
                field_2C_ped_leader->SetObjective2_463830(20, 9999);
                field_2C_ped_leader->SetInternalTargetPed_403AE0(pOther->field_2C_ped_leader);
                field_2C_ped_leader->SetPanicking_403950();
            }

            if (!pOther->field_2C_ped_leader->PedTypeIs_45EDE0(ped_type::player_2) && !pOther->field_2C_ped_leader->field_21C_bf.bLeftVehicle &&
                pOther->field_2C_ped_leader->field_168_game_object)
            {
                pOther->field_2C_ped_leader->SetObjective2_463830(20, 9999);
                pOther->field_2C_ped_leader->SetInternalTargetPed_403AE0(field_2C_ped_leader);
                pOther->field_2C_ped_leader->SetPanicking_403950();
            }
        }
    }
}
#pragma warning(pop)

MATCH_FUNC(0x4c9ed0)
Ped* PedGroup::sub_4C9ED0()
{
    for (s8 i = this->field_34_count - 1; i >= 0; i--)
    {
        Ped* pPed = field_4_ped_list[i];
        if (!pPed->IsPanicking())
        {
            return field_4_ped_list[i];
        }
    }
    return 0;
}

MATCH_FUNC(0x4c9f00)
void PedGroup::CoordinateGroupCarEntry_4C9F00()
{
    s8 i;
    Fix16 distance;
    Ped* pLeader = field_2C_ped_leader;
    s32 state = pLeader->GetPedState_403990();
    if (state == 3 || state == 10 || state == 5)
    {
        field_30 = 1;
        if (pLeader->GetPedState_403990() == 10 || pLeader->FindUsableCarDoor_467090())
        {
            Car_BC* pCar;
            if (field_2C_ped_leader->field_168_game_object)
            {
                pCar = field_2C_ped_leader->field_168_game_object->field_84_target_car;
            }
            else
            {
                pCar = field_2C_ped_leader->field_16C_car;
            }
            u8 passengers = pCar->GetPassengersCount_440570();

            for (i = 0; i < passengers && i < field_34_count; i++)
            {
                Ped* pMember = field_4_ped_list[i];
                if (pMember->field_16C_car || pMember->isDead_403B60() ||
                    (pMember->get_occupation_403980() == 0x17 && pMember->field_258_objective != 0) || pMember->get_objective_403A80() == 8)
                {
                    continue;
                }

                if (field_2C_ped_leader->field_16C_car && field_2C_ped_leader->field_16C_car->IsTrainModel_403BA0())
                {
                    pMember->SetObjective2_463830(0x25, 9999);
                    pMember->set_target_to_enter_403B00(field_2C_ped_leader->field_16C_car);
                    continue;
                }

                if (pMember->GetInternalObjective_403A90() != 0x23)
                {
                    pMember->field_21C_bf.bPanicking = 0;
                    if (field_2C_ped_leader->field_168_game_object)
                    {
                        pMember->SetObjective2_463830(0x12, 9999);
                        pMember->set_enter_car_as_passenger_4039B0(1);
                        pMember->set_target_to_enter_403B00(field_2C_ped_leader->field_168_game_object->field_84_target_car);
                    }
                    else
                    {
                        pMember->SetObjective(0, 9999);
                        pMember->SetObjective2_463830(0x23, 9999);
                        pMember->set_enter_car_as_passenger_4039B0(1);
                        pMember->set_target_to_enter_403B00(field_2C_ped_leader->field_16C_car);
                    }
                    pMember->field_168_game_object->ClearCollisionState_545600();
                }

                Car_BC* pTargetCar = pMember->get_target_to_enter_403B10();
                u8 tries = 0;
                byte_620838 = i + 1;
                u8 maxDoor = pTargetCar->GetRemap() - 1;
                char_type searching;
                if (pTargetCar->IsSwatVanOrBankVan_403BC0())
                {
                    byte_620838 = i + 2;
                    do
                    {
                        searching = 1;
                        do
                        {
                            if (byte_620838 > maxDoor)
                            {
                                byte_620838 += 1 - maxDoor;
                            }
                        } while (byte_620838 > maxDoor);

                        if (pTargetCar->IsDoorAccessible_43AFE0(byte_620838))
                        {
                            searching = 0;
                        }
                        else
                        {
                            byte_620838++;
                        }

                        if (++tries == maxDoor + 1)
                        {
                            byte_620838 = maxDoor;
                            if (pTargetCar->IsDoorAccessible_43AFE0(0))
                            {
                                byte_620838 = 0;
                            }
                            else if (pTargetCar->IsDoorAccessible_43AFE0(1))
                            {
                                byte_620838 = 1;
                            }
                            break;
                        }
                    } while (searching);
                }
                else
                {
                    do
                    {
                        searching = 1;
                        do
                        {
                            if (byte_620838 > maxDoor)
                            {
                                byte_620838 -= maxDoor;
                            }
                        } while (byte_620838 > maxDoor);

                        if (pTargetCar->IsDoorAccessible_43AFE0(byte_620838))
                        {
                            searching = 0;
                        }
                        else
                        {
                            byte_620838++;
                        }

                        if (++tries == maxDoor + 1)
                        {
                            byte_620838 = maxDoor;
                            if (pTargetCar->IsDoorAccessible_43AFE0(0))
                            {
                                byte_620838 = 0;
                            }
                            break;
                        }
                    } while (searching);
                }
                for (s8 j = 0; j < field_34_count; j++)
                {
                    Ped* pOther = field_4_ped_list[j];
                    if (pOther != pMember && pOther->field_25C_internal_objective == 0x12 &&
                        pMember->field_24C_target_car_door == pOther->get_target_car_door_403A60())
                    {
                        pMember->SetObjective2_463830(9, 9999);
                        pMember->field_14C_internal_target_ped = pOther;
                    }
                }
                pMember->set_target_car_door_403A70(byte_620838);
            }

            for (; i < field_34_count; i++)
            {
                Ped* pMember = field_4_ped_list[i];
                if (pMember->get_objective_403A80() != 8)
                {
                    pMember->SetObjective(8, 9999);
                    pMember->SetObjective2_463830(0, 9999);
                }
            }
        }
        return;
    }

    if (pLeader->field_168_game_object)
    {
        bool bWaiting = false;
        if (field_1)
        {
            Ped* pFarthest = FindFarthestMember_4CA3F0(&distance);
            if (distance > dword_67F608 && !field_2C_ped_leader->PedTypeIs_45EDE0(ped_type::player_2))
            {
                if (field_2C_ped_leader->get_objective_403A80() != 0xD && field_2C_ped_leader->field_25C_internal_objective != 0x24)
                {
                    field_2C_ped_leader->SetObjective2_463830(9, 9999);
                    field_2C_ped_leader->field_14C_internal_target_ped = pFarthest;
                    field_3C = 1;
                }
                bWaiting = true;
            }
            else if (field_3C == 1)
            {
                field_2C_ped_leader->SetObjective2_463830(0, 9999);
            }
        }
        if (!bWaiting)
        {
            field_3C = 0;
        }
    }

    if (!(u8)sub_4CA3E0())
    {
        for (i = 0; i < field_34_count; i++)
        {
            if (field_4_ped_list[i]->GetPanicking() == 1)
            {
                field_30 = 1;
                return;
            }
        }
        field_30 = 0;
        return;
    }
    field_30 = 1;
}

MATCH_FUNC(0x4ca3e0)
u32 PedGroup::sub_4CA3E0()
{
    return field_2C_ped_leader->field_21C_bf.bPanicking;
}

MATCH_FUNC(0x4ca3f0)
Ped* PedGroup::FindFarthestMember_4CA3F0(Fix16* pFoundDistance)
{
    Fix16 max_distance = Fix16(0);
    u8 found_index = 0;
    Fix16 x_abs;
    Fix16 y_abs;
    for (u8 i = 0; i < field_34_count; i++)
    {
        Fix16 x_diff = field_2C_ped_leader->get_cam_x() - field_4_ped_list[i]->get_cam_x();
        Fix16 y_diff = field_2C_ped_leader->get_cam_y() - field_4_ped_list[i]->get_cam_y();

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
    return field_4_ped_list[found_index];
}

MATCH_FUNC(0x4ca4b0)
void PedGroup::UpdateFormation_4CA4B0()
{
    if (!field_2C_ped_leader->field_16C_car && (field_2C_ped_leader->field_21C_bf.bLeftVehicle) == 0)
    {
        for (u8 i = 0; i < field_34_count; i++)
        {
            Ped* pIter = field_4_ped_list[i];
            if (pIter->field_25C_internal_objective != 9)
            {
                if (pIter->field_168_game_object)
                {
                    pIter->SetObjective2_463830(9, 9999);
                }
            }
            if (field_38_group_type == 1)
            {
                if (i == 0)
                {
                    pIter->field_14C_internal_target_ped = field_2C_ped_leader;
                }
                else
                {
                    pIter->field_14C_internal_target_ped = field_4_ped_list[i - 1];
                }
            }
            else if (pIter->field_278_ped_state_1 != 9)
            {
                if (field_2C_ped_leader->GetPedVelocity_45C920() != dword_67F610)
                {
                    switch (i)
                    {
                        case 3u:
                            pIter->field_14C_internal_target_ped = this->field_4_ped_list[0];
                            break;
                        case 4u:
                            pIter->field_14C_internal_target_ped = this->field_4_ped_list[1];
                            break;
                        case 5u:
                            pIter->field_14C_internal_target_ped = this->field_4_ped_list[2];
                            break;
                        case 6u:
                            pIter->field_14C_internal_target_ped = this->field_4_ped_list[3];
                            break;
                        case 7u:
                            pIter->field_14C_internal_target_ped = this->field_4_ped_list[4];
                            break;
                        default:
                            pIter->field_14C_internal_target_ped = field_2C_ped_leader;
                            break;
                    }
                }
                else
                {
                    pIter->field_14C_internal_target_ped = field_2C_ped_leader;
                }
            }
            pIter->ClearAttacking();
        }
    }
}

MATCH_FUNC(0x4ca5e0)
void PedGroup::UpdateMemberAIState_4CA5E0(u8 idx)
{
    Ped* pMember = field_4_ped_list[idx];
    if (pMember->field_21C & ped_flag_mask::k_ped_left_vehicle)
    {
        return;
    }

    if (pMember->get_objective_403A80() == 8)
    {
        if (pMember->GetInternalObjective_403A90() == 9)
        {
            pMember->SetObjective2_463830(0, 9999);
        }
        return;
    }

    if (!field_2C_ped_leader->PedTypeIs_45EDE0(ped_type::player_2))
    {
        pMember->field_288_threat_search = field_2C_ped_leader->field_288_threat_search;
        pMember->field_28C_threat_reaction = field_2C_ped_leader->field_28C_threat_reaction;
        pMember->field_17C_pGang = field_2C_ped_leader->field_17C_pGang;
    }
    else
    {
        s32 occupation = pMember->get_occupation_403980();
        if (occupation != 0x29)
        {
            if (occupation != 0x2D)
            {
                pMember->field_288_threat_search = threat_search_enum::line_of_sight_1;
                pMember->field_28C_threat_reaction = threat_reaction_enum::react_as_normal_2;
            }
            else
            {
                pMember->field_288_threat_search = threat_search_enum::area_2;
                pMember->field_28C_threat_reaction = threat_reaction_enum::no_reaction_0;
            }
        }
    }

    Ped* pLeader = field_2C_ped_leader;
    if (pLeader->bHasGameObject_403B70())
    {
        if (pMember->bHasGameObject_403B70())
        {
            if (!(pLeader->field_21C & ped_flag_mask::k_ped_left_vehicle))
            {
                Ped* pNearest = FindNearestOtherMember_4CAE80(idx);
                if (pNearest)
                {
                    pMember->SetObjective2_463830(20, 9999);
                    pMember->SetInternalTargetPed_403AE0(pNearest->GetInternalTargetPed_403AF0());
                    pMember->SetPanicking_403950();
                }
                else if (pMember->get_objective_403A80() != 8)
                {
                    if (pMember->get_occupation_403980() != 0x17 || pMember->get_objective_403A80() != 0x10)
                    {
                        UpdateFormation_4CA4B0();
                    }
                    if (field_2C_ped_leader->GetInternalObjective_403A90() == 0x3B)
                    {
                        pMember->SetObjective2_463830(0x3B, 9999);
                        pMember->set_target_to_enter_403B00(field_2C_ped_leader->get_target_to_enter_403B10());
                    }
                }
                else if (pMember->GetInternalObjective_403A90() == 9)
                {
                    pMember->SetObjective(0, 9999);
                }
            }
        }
        else if (pMember->GetInternalObjective_403A90() != 0x24)
        {
            pMember->SetObjective2_463830(0x24, 9999);
            pMember->set_target_to_enter_403B00(pMember->field_16C_car);
        }
    }
    else if (pMember->GetInternalObjective_403A90() == 9)
    {
        s32 occupation = pMember->get_occupation_403980();
        if ((occupation < 0x18 || occupation > 0x1B) && pMember->GetInternalTargetPed_403AF0()->has_car_403B80() && pMember->field_258_objective != 8)
        {
            pMember->SetObjective(8, 9999);
            pMember->SetObjective2_463830(0, 9999);
        }
    }

    Ped* pFollow = pMember->field_14C_internal_target_ped;
    if (pFollow && pMember->field_25C_internal_objective == 0xB && !pFollow->field_168_game_object)
    {
        pMember->SetObjective2_463830(0, 9999);
    }
}

MATCH_FUNC(0x4ca820)
void PedGroup::UpdateMemberTightFollowState_4CA820(u8 idx)
{
    Ped* pMember = field_4_ped_list[idx];
    if (field_2C_ped_leader->field_168_game_object)
    {
        Fix16 x_diff = field_2C_ped_leader->field_1AC_cam.x - pMember->field_1AC_cam.x;
        Fix16 y_diff = field_2C_ped_leader->field_1AC_cam.y - pMember->field_1AC_cam.y;
        Fix16 x_abs = Fix16::Abs(x_diff);
        Fix16 y_abs = Fix16::Abs(y_diff);
        if (Fix16::Max(x_abs, y_abs) < dword_67F670)
        {
            pMember->SetObjective2_463830(9, 9999);
            pMember->SetInternalTargetPed_403AE0(field_2C_ped_leader);
            pMember->ClearAttacking();
            pMember->SetObjective(0, 9999);
        }
        else if (pMember->field_14C_internal_target_ped == field_2C_ped_leader && pMember->field_25C_internal_objective == 9)
        {
            pMember->SetObjective2_463830(0, 9999);
        }
    }
    else if (pMember->field_14C_internal_target_ped == field_2C_ped_leader && pMember->field_25C_internal_objective == 9)
    {
        pMember->SetObjective2_463830(0, 9999);
    }
}

MATCH_FUNC(0x4caa20)
bool PedGroup::IsAllMembersInSomeCar_4CAA20()
{
    if (field_34_count == 0)
    {
        return field_2C_ped_leader->field_16C_car != 0;
    }

    for (u8 i = 0; i < field_34_count; i++)
    {
        if (field_4_ped_list[i]->GetPedState_403990() != ped_state_1::in_car_10)
        {
            return false;
        }
    }
    return true;
}

MATCH_FUNC(0x4caae0)
char_type PedGroup::HasNoActiveMembers_4CAAE0()
{
    for (u8 i = 0; i < field_34_count; i++)
    {
        if (field_4_ped_list[i]->field_278_ped_state_1 != ped_state_1::in_car_10 &&
            field_4_ped_list[i]->field_278_ped_state_1 != ped_state_1::dead_9)
        {
            return false;
        }
    }
    return true;
}

MATCH_FUNC(0x4cab80)
char_type PedGroup::AreAllMembersOnFoot_4CAB80()
{
    for (u8 i = 0; i < field_34_count; i++)
    {
        if (field_4_ped_list[i]->field_16C_car)
        {
            return false;
        }
    }
    return true;
}

MATCH_FUNC(0x4cac20)
bool PedGroup::IsMemberTooFarFromLeader_4CAC20(u8 idx)
{
    Ped* pMember = field_4_ped_list[idx];
    Fix16 x_diff = pMember->field_1AC_cam.x - field_2C_ped_leader->field_1AC_cam.x;
    Fix16 y_diff = pMember->field_1AC_cam.y - field_2C_ped_leader->field_1AC_cam.y;

    Fix16 x_abs = Fix16::Abs(x_diff);
    Fix16 y_abs = Fix16::Abs(y_diff);

    Fix16 max_distance = (x_abs > y_abs) ? x_abs : y_abs;

    if (max_distance > dword_67F630)
    {
        return true;
    }
    return false;
}

MATCH_FUNC(0x4cad40)
bool PedGroup::IsLeaderCloseToTargetCar_4CAD40()
{
    Fix16 leader_xpos = field_2C_ped_leader->get_cam_x();
    Car_BC* pTargetCar = field_2C_ped_leader->get_target_to_enter_403B10();
    if (!pTargetCar)
    {
        pTargetCar = field_2C_ped_leader->get_target_objective_car_403AB0();
    }

    Fix16 x_diff = leader_xpos - pTargetCar->field_50_car_sprite->field_14_xy.x;
    Fix16 y_diff = field_2C_ped_leader->get_cam_y() - pTargetCar->field_50_car_sprite->field_14_xy.y;

    Fix16 x_abs = Fix16::Abs(x_diff);
    Fix16 y_abs = Fix16::Abs(y_diff);

    Fix16 max_distance = (x_abs > y_abs) ? x_abs : y_abs;

    if (max_distance <= dword_67F630)
    {
        return true;
    }
    return false;
}

// The loop is a do/while under its own entry test: an empty group skips the final
// nearest_distance check. Reading pOther's x directly (not through get_cam_x) loads
// pMember's x first, as in the original.
MATCH_FUNC(0x4cae80)
Ped* PedGroup::FindNearestOtherMember_4CAE80(u8 idx)
{
    Fix16 x_abs;
    Fix16 y_abs;
    Fix16 nearest_distance = dword_67F60C;
    u8 nearest_idx = 0;
    Ped* pMember = field_4_ped_list[idx];
    u8 i = 0;
    if (i < field_34_count)
    {
        do
        {
            Ped* pOther = field_4_ped_list[i];
            if (i != idx)
            {
                Fix16 x_diff = pMember->get_cam_x() - pOther->field_1AC_cam.x;
                Fix16 y_diff = pMember->get_cam_y() - pOther->get_cam_y();

                x_abs = Fix16::Abs(x_diff);
                y_abs = Fix16::Abs(y_diff);

                Fix16 distance = (x_abs > y_abs) ? x_abs : y_abs;
                if (distance < nearest_distance && pOther->IsAttackingTargetPed_465CD0())
                {
                    nearest_distance = distance;
                    nearest_idx = i;
                }
            }
            i++;
        } while (i < field_34_count);

        if (nearest_distance != dword_67F60C)
        {
            return field_4_ped_list[nearest_idx];
        }
    }
    return 0;
}

MATCH_FUNC(0x4cb080)
void PedGroup::ResetAllGroups_4CB080()
{
    sub_4C8E80();

    PedGroup* pIter = pedGroups_67EF20;
    for (s32 i = 0; i < 20; i++)
    {
        pIter->Reset_4C8EF0();
        pIter++;
    }
}

MATCH_FUNC(0x4cb0d0)
PedGroup* PedGroup::New_4CB0D0()
{
    for (s32 i = 0; i < 20; i++)
    {
        if (!pedGroups_67EF20[i].get_in_use_4038F0())
        {
            pedGroups_67EF20[i].Reset_4C8EF0();
            pedGroups_67EF20[i].set_in_use_4038E0();
            return &pedGroups_67EF20[i];
        }
    }
    return NULL;
}

MATCH_FUNC(0x4cb860)
PedGroup::PedGroup()
{
    Reset_4C8EF0();
}

MATCH_FUNC(0x4cb870)
PedGroup::~PedGroup()
{
}