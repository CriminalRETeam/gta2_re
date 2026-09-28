#include "PedGroup.hpp"
#include "Car_BC.hpp"
#include "Char_Pool.hpp"
#include "Globals.hpp"
#include "Ped.hpp"
#include "enums.hpp"
#include "rng.hpp"

DEFINE_GLOBAL_ARRAY(PedGroup, pedGroups_67EF20, 20, 0x67EF20);
DEFINE_GLOBAL(Fix16, dword_67F60C, 0x67F60C);
DEFINE_GLOBAL(Fix16, dword_67F608, 0x67F608);
DEFINE_GLOBAL(Fix16, dword_67F610, 0x67F610);
DEFINE_GLOBAL(Fix16, dword_67F670, 0x67F670);
DEFINE_GLOBAL_INIT(Fix16, k_dword_67EEE4, Fix16(0x500, 0), 0x67EEE4);
DEFINE_GLOBAL_INIT(char_type, byte_620838, 1, 0x620838);
DEFINE_GLOBAL_INIT(Fix16, dword_67F630, Fix16(4), 0x67F630);

STUB_FUNC(0x4c8e60)
void PedGroup::sub_4C8E60()
{
    NOT_IMPLEMENTED;
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
        field_2C_ped_leader->reset_ped_group();
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
            field_4_ped_list[i]->reset_ped_group();
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
char_type PedGroup::sub_4C9150()
{
    if (field_2C_ped_leader->field_168_game_object == NULL || field_2C_ped_leader->get_field_20e() < 0x28)
    {
        return false;
    }

    for (u8 i = 0; i < field_34_count; i++)
    {
        if (field_4_ped_list[i]->field_168_game_object == NULL || field_4_ped_list[i]->get_field_20e() < 0x28)
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
        field_4_ped_list[i]->unset_bitset_0x04();
        field_4_ped_list[i]->SetObjective2_463830(objectives_enum::objective_9, 9999);
        field_4_ped_list[i]->set_field_14C_403AE0(field_2C_ped_leader);
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

WIP_FUNC(0x4c9240)
void PedGroup::KillEntireGroup_4C9240()
{
    WIP_IMPLEMENTED;

    Ped* p2CPed = this->field_2C_ped_leader;
    p2CPed->field_164_ped_group = 0;
    p2CPed->field_23C = 0;
    p2CPed->Kill_46F9D0();
    char_type i = 0;
    if (this->field_34_count)
    {
        s32 last_i = 0;
        do
        {
            Ped* pPed = this->field_4_ped_list[last_i];
            pPed->field_164_ped_group = 0;
            pPed->field_23C = 0;
            pPed->Kill_46F9D0();
            last_i = ++i;
        } while (i < (s32)this->field_34_count);
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

    field_2C_ped_leader->reset_ped_group();
    if (field_4_ped_list[0] != NULL)
    {
        for (u8 bVar4 = 0; bVar4 < field_34_count; bVar4++)
        {
            Ped* this_00 = field_4_ped_list[bVar4];
            Ped** pppVar1 = field_4_ped_list + bVar4;
            if ((this_00->get_ped_state1() == ped_state1_enum::ped_wasted) ||
                (this_00->field_280_stored_ped_state_1 == ped_state1_enum::ped_wasted))
            {
                this_00->reset_ped_group();
            }
            else
            {
                if (this_00->has_field_16C_car())
                {
                    this_00->SetObjective(objectives_enum::objective_34, 9999);
                    (*pppVar1)->set_field_150_target_objective_car((*pppVar1)->field_16C_car);
                }
                else
                {
                    this_00->SetObjective(objectives_enum::no_obj_0, 9999);
                }
                (*pppVar1)->SetObjective2_463830(objectives_enum::no_obj_0, 9999);
                (*pppVar1)->reset_ped_group();
                (*pppVar1)->set_ped_type(ped_type_enum::New_Name_2);
            }
            (*pppVar1)->field_21C |= 0x400;
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
    if ((ppVar2->get_ped_state1() != ped_state1_enum::ped_wasted) && (ppVar2->field_280_stored_ped_state_1 != ped_state1_enum::ped_wasted))
    {
        ppVar2->SetObjective(objectives_enum::no_obj_0, 9999);
        field_2C_ped_leader->SetObjective2_463830(objectives_enum::no_obj_0, 9999);
    }

    field_2C_ped_leader->reset_ped_group();
    if (field_4_ped_list[0] != NULL)
    {
        for (u8 bVar5 = 0; bVar5 < field_34_count; bVar5++)
        {
            ppVar2 = field_4_ped_list[bVar5];
            Ped** pppVar1 = field_4_ped_list + bVar5;
            if ((ppVar2->get_ped_state1() == ped_state1_enum::ped_wasted) ||
                (ppVar2->field_280_stored_ped_state_1 == ped_state1_enum::ped_wasted))
            {
                ppVar2->reset_ped_group();
            }
            else
            {
                if (ppVar2->has_field_16C_car() == true)
                {
                    ppVar2->SetObjective2_463830(objectives_enum::no_obj_0, 9999);
                    (*pppVar1)->SetObjective(objectives_enum::objective_34, 9999);
                    (*pppVar1)->set_field_150_target_objective_car((*pppVar1)->field_16C_car);
                    (*pppVar1)->reset_ped_group();
                    (*pppVar1)->set_ped_type(ped_type_enum::New_Name_2);
                }
                else
                {
                    ppVar2->SetObjective(objectives_enum::no_obj_0, 9999);
                    (*pppVar1)->SetObjective2_463830(objectives_enum::no_obj_0, 9999);
                    (*pppVar1)->reset_ped_group();
                    (*pppVar1)->set_ped_type(ped_type_enum::New_Name_2);
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
    // TODO: Bunch of missing getter/setter inlines here
    if (!pAttacker)
    {
        PedGroup::DestroyGroup_4C93A0();
    }
    else
    {
        if (!field_2C_ped_leader->IsField238_45EDE0(2))
        {
            this->field_2C_ped_leader->SetObjective(objectives_enum::flee_char_on_foot_always_3, 9999);
            this->field_2C_ped_leader->field_148_objective_target_ped = pAttacker;
            this->field_2C_ped_leader->SetObjective2_463830(3, 9999);
            this->field_2C_ped_leader->field_14C = pAttacker;
            this->field_2C_ped_leader->field_21C |= 4u;
            this->field_2C_ped_leader->field_228 = 0;
            this->field_2C_ped_leader->field_168_game_object->field_3C_run_or_jump_speed = k_dword_67EEE4;
        }

        this->field_2C_ped_leader->ClearGroupAndGroupIdx_403A30();

        for (char_type i_ = 0; i_ < (s32)this->field_34_count; i_++)
        {
            s32 i = i_;
            if (field_4_ped_list[i]->field_16C_car)
            {
                this->field_4_ped_list[i]->SetObjective2_463830(objectives_enum::no_obj_0, 9999);
                this->field_4_ped_list[i]->SetObjective(objectives_enum::flee_char_always_once_car_stopped_6, 9999);
                this->field_4_ped_list[i]->field_148_objective_target_ped = pAttacker;
                this->field_4_ped_list[i]->field_228 = 0;
                this->field_4_ped_list[i]->ClearGroupAndGroupIdx_403A30();
            }
            else
            {
                this->field_4_ped_list[i]->SetObjective(objectives_enum::flee_char_on_foot_always_3, 9999);
                this->field_4_ped_list[i]->field_148_objective_target_ped = pAttacker;
                this->field_4_ped_list[i]->SetObjective2_463830(3, 9999);
                this->field_4_ped_list[i]->field_14C = pAttacker;
                this->field_4_ped_list[i]->field_21C |= 4u;
                this->field_4_ped_list[i]->field_228 = 0;
                this->field_4_ped_list[i]->field_168_game_object->field_3C_run_or_jump_speed = k_dword_67EEE4;
                this->field_4_ped_list[i]->ClearGroupAndGroupIdx_403A30();
                this->field_4_ped_list[i]->field_238_ped_type = ped_type::dummy_3;
            }
        }
        PedGroup::ClearGroupData_4C8E90();
    }
}

MATCH_FUNC(0x4c9680)
void PedGroup::PromoteMemberToLeader_4C9680(u8 idx)
{
    Ped* pTmp = gPedPool_6787B8->field_0_pool.Allocate();

    Weapon_30* leaderWeapon = field_2C_ped_leader->field_170_selected_weapon;
    Weapon_30* memberWeapon = field_4_ped_list[idx]->field_170_selected_weapon;
    Weapon_30* leaderWeapon2 = field_2C_ped_leader->field_174_pWeapon;
    Weapon_30* memberWeapon2 = field_4_ped_list[idx]->field_174_pWeapon;

    pTmp->CopyStatsFromPed_45B5B0(field_2C_ped_leader);
    field_2C_ped_leader->CopyStatsFromPed_45B5B0(field_4_ped_list[idx]);
    field_2C_ped_leader->field_170_selected_weapon = leaderWeapon;
    field_2C_ped_leader->field_174_pWeapon = leaderWeapon2;

    field_2C_ped_leader->SetObjective(pTmp->field_258_objective, pTmp->field_218_objective_timer);
    field_2C_ped_leader->SetObjective2_463830(pTmp->field_25C_internal_objective, pTmp->field_21A_car_state_timer);
    field_2C_ped_leader->field_225_objective_status = pTmp->field_225_objective_status;
    field_2C_ped_leader->field_226 = pTmp->field_226;
    field_2C_ped_leader->field_148_objective_target_ped = pTmp->field_148_objective_target_ped;
    field_2C_ped_leader->field_150_target_objective_car = pTmp->field_150_target_objective_car;
    field_2C_ped_leader->field_1A0_objective_target_object = pTmp->field_1A0_objective_target_object;
    field_2C_ped_leader->field_1A4 = pTmp->field_1A4;
    field_2C_ped_leader->field_1DC_objective_target_x = pTmp->field_1DC_objective_target_x;
    field_2C_ped_leader->field_1E0_objective_target_y = pTmp->field_1E0_objective_target_y;
    field_2C_ped_leader->field_1E4_objective_target_z = pTmp->field_1E4_objective_target_z;
    field_2C_ped_leader->field_14C = pTmp->field_14C;
    field_2C_ped_leader->field_154_target_to_enter = pTmp->field_154_target_to_enter;
    field_2C_ped_leader->field_1D0 = pTmp->field_1D0;
    field_2C_ped_leader->field_1D4 = pTmp->field_1D4;
    field_2C_ped_leader->field_1D8 = pTmp->field_1D8;
    field_2C_ped_leader->field_23C = 99;
    field_2C_ped_leader->field_248_enter_car_as_passenger = pTmp->field_248_enter_car_as_passenger;
    field_2C_ped_leader->field_24C_target_car_door = pTmp->field_24C_target_car_door;

    Ped* pLeader = field_2C_ped_leader;
    if (pLeader->field_168_game_object)
    {
        pLeader->field_168_game_object->field_7C_pPed = pLeader;
    }
    else if (!pLeader->field_248_enter_car_as_passenger && pLeader->field_16C_car->field_54_driver != pLeader)
    {
        pLeader->field_248_enter_car_as_passenger = 1;
    }

    field_4_ped_list[idx]->CopyStatsFromPed_45B5B0(pTmp);
    field_4_ped_list[idx]->field_23C = idx;
    field_4_ped_list[idx]->field_170_selected_weapon = memberWeapon;
    field_4_ped_list[idx]->field_174_pWeapon = memberWeapon2;

    Ped* pMember = field_4_ped_list[idx];
    if (pMember->field_168_game_object && pMember->field_240_occupation == 0x17)
    {
        pMember->field_168_game_object->field_7C_pPed = pMember;
        field_4_ped_list[idx]->field_248_enter_car_as_passenger = 1;
    }
    else
    {
        if (pMember->field_168_game_object)
        {
            pMember->field_168_game_object->field_7C_pPed = pMember;
        }

        if (idx < field_34_count - 1)
        {
            field_4_ped_list[idx]->reset_ped_group();
            field_4_ped_list[idx] = field_4_ped_list[field_34_count - 1];
            field_4_ped_list[idx]->field_23C = idx;
        }
        else
        {
            field_4_ped_list[idx]->field_164_ped_group = 0;
            field_4_ped_list[idx] = 0;
        }
        field_34_count--;
        field_2C_ped_leader->field_23C = 99;
    }

    if (pTmp->field_238_ped_type == 5)
    {
        pTmp->PoolAllocate();
        pTmp->field_21C |= 0x400;
    }
    else
    {
        pTmp->PoolAllocate();
    }
    pTmp->field_216_health = 100;
    pTmp->field_240_occupation = 2;
    pTmp->Kill_46F9D0();
}

MATCH_FUNC(0x4c9970)
void PedGroup::RemovePed_4C9970(Ped* pPed)
{
    if (pPed == field_2C_ped_leader)
    {
        if (!field_2C_ped_leader->IsField238_45EDE0(2))
        {
            if (field_2C_ped_leader->field_278_ped_state_1 == ped_state_1::dead_9 && field_2C_ped_leader->field_238_ped_type == 5)
            {
                field_2C_ped_leader->field_21C |= 0x400;
            }

            Ped* pNewLeader = field_4_ped_list[0];
            if (pNewLeader && pNewLeader->field_278_ped_state_1 != ped_state_1::dead_9)
            {
                PromoteMemberToLeader_4C9680(0);
                if (field_2C_ped_leader->field_25C_internal_objective == 0)
                {
                    field_2C_ped_leader->sub_4633E0(1);
                }
                else
                {
                    field_2C_ped_leader->sub_4633E0(0);
                }
                if (pPed->field_240_occupation == 0x17)
                {
                    if (field_34_count > 0)
                    {
                        field_4_ped_list[field_34_count] = pNewLeader;
                        field_4_ped_list[field_34_count]->field_23C = field_36_count - 1;
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
                field_4_ped_list[i]->field_23C = i;
                field_4_ped_list[field_34_count - 1] = pPed;
                pPed->field_23C = field_34_count - 1;
                if (pPed->field_240_occupation != 0x17)
                {
                    field_4_ped_list[field_34_count - 1]->reset_ped_group();
                    field_4_ped_list[field_34_count - 1] = 0;
                    if (pPed->field_238_ped_type == 5)
                    {
                        pPed->field_21C |= 0x400;
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
    field_30 = 1;
    if (!pPed->field_164_ped_group)
    {
        for (i = field_34_count - 1; i >= 0; i--)
        {
            Ped* pMember = field_4_ped_list[i];
            if (!pMember->GetBit2() && !pMember->field_21C_bf.b27 && pMember->field_168_game_object)
            {
                if (IsMemberTooFarFromLeader_4CAC20(i))
                {
                    pMember->SetObjective2_463830(7, 9999);
                    pMember->set_field_14C_403AE0(field_2C_ped_leader);
                }
                else
                {
                    if (field_2C_ped_leader->field_25C_internal_objective == 0x23 && !IsLeaderCloseToTargetCar_4CAD40())
                    {
                        continue;
                    }
                    pMember->SetObjective2_463830(20, 9999);
                    pMember->set_field_14C_403AE0(pPed);
                }
                pMember->SetBit2_403950();
            }
        }

        if (!field_2C_ped_leader->IsField238_45EDE0(2) && !field_2C_ped_leader->GetBit2() && !field_2C_ped_leader->field_16C_car &&
            !field_2C_ped_leader->field_21C_bf.b27)
        {
            field_2C_ped_leader->SetObjective2_463830(20, 9999);
            field_2C_ped_leader->set_field_14C_403AE0(pPed);
            field_2C_ped_leader->SetBit2_403950();
        }
    }
    else
    {
        PedGroup* pOther = pPed->field_164_ped_group;
        for (i = field_34_count - 1; i >= 0; i--)
        {
            Ped* pMember = field_4_ped_list[i];
            if (!pMember->GetBit2() && !pMember->field_21C_bf.b27 && pMember->field_168_game_object)
            {
                Ped* pTarget = pOther->sub_4C9ED0();
                if (pTarget)
                {
                    pMember->SetObjective2_463830(20, 9999);
                    pMember->set_field_14C_403AE0(pTarget);
                    pMember->SetBit2_403950();
                    if (!pTarget->field_16C_car)
                    {
                        pTarget->SetObjective2_463830(20, 9999);
                        pTarget->set_field_14C_403AE0(pMember);
                        pTarget->SetBit2_403950();
                    }
                }
                else
                {
                    s16 max = pOther->field_34_count + 1;
                    s16 rnd = stru_6F6784.get_int_4F7AE0(max);
                    if (rnd == pOther->field_34_count)
                    {
                        if (!pOther->field_2C_ped_leader || pOther->field_2C_ped_leader->field_278_ped_state_1 == ped_state_1::dead_9)
                        {
                            continue;
                        }
                        pMember->SetObjective2_463830(20, 9999);
                        pMember->set_field_14C_403AE0(pOther->field_2C_ped_leader);
                    }
                    else
                    {
                        if (!pOther->field_4_ped_list[rnd] || pOther->field_4_ped_list[rnd]->field_278_ped_state_1 == ped_state_1::dead_9)
                        {
                            continue;
                        }
                        pMember->SetObjective2_463830(20, 9999);
                        pMember->set_field_14C_403AE0(pOther->field_4_ped_list[rnd]);
                    }
                    pMember->SetBit2_403950();
                }
            }
        }

        if (!field_2C_ped_leader->GetBit2() && !pOther->field_2C_ped_leader->GetBit2())
        {
            if (!field_2C_ped_leader->IsField238_45EDE0(2) && !field_2C_ped_leader->field_21C_bf.b27 &&
                field_2C_ped_leader->field_168_game_object)
            {
                field_2C_ped_leader->SetObjective2_463830(20, 9999);
                field_2C_ped_leader->set_field_14C_403AE0(pOther->field_2C_ped_leader);
                field_2C_ped_leader->SetBit2_403950();
            }

            if (!pOther->field_2C_ped_leader->IsField238_45EDE0(2) && !pOther->field_2C_ped_leader->field_21C_bf.b27 &&
                pOther->field_2C_ped_leader->field_168_game_object)
            {
                pOther->field_2C_ped_leader->SetObjective2_463830(20, 9999);
                pOther->field_2C_ped_leader->set_field_14C_403AE0(field_2C_ped_leader);
                pOther->field_2C_ped_leader->SetBit2_403950();
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
        if (!pPed->get_bitset_0x04())
        {
            return field_4_ped_list[i];
        }
    }
    return 0;
}

WIP_FUNC(0x4c9f00)
void PedGroup::CoordinateGroupCarEntry_4C9F00()
{
    s8 i;
    Fix16 distance;
    Ped* pLeader = field_2C_ped_leader;
    s32 state = pLeader->field_278_ped_state_1;
    if (state == 3 || state == 10 || state == 5)
    {
        field_30 = 1;
        if (pLeader->field_278_ped_state_1 == 10 || pLeader->FindUsableCarDoor_467090())
        {
            Car_BC* pCar;
            if (field_2C_ped_leader->field_168_game_object)
            {
                pCar = field_2C_ped_leader->field_168_game_object->field_84;
            }
            else
            {
                pCar = field_2C_ped_leader->field_16C_car;
            }
            u8 passengers = pCar->GetPassengersCount_440570();

            for (i = 0; i < passengers && i < field_34_count; i++)
            {
                Ped* pMember = field_4_ped_list[i];
                if (pMember->field_16C_car || pMember->field_278_ped_state_1 == ped_state_1::dead_9 ||
                    (pMember->field_240_occupation == 0x17 && pMember->field_258_objective != 0) || pMember->field_258_objective == 8)
                {
                    continue;
                }

                if (field_2C_ped_leader->field_16C_car && field_2C_ped_leader->field_16C_car->IsTrainModel_403BA0())
                {
                    pMember->SetObjective2_463830(0x25, 9999);
                    pMember->field_154_target_to_enter = field_2C_ped_leader->field_16C_car;
                    continue;
                }

                if (pMember->field_25C_internal_objective != 0x23)
                {
                    pMember->field_21C_bf.b2 = 0;
                    if (field_2C_ped_leader->field_168_game_object)
                    {
                        pMember->SetObjective2_463830(0x12, 9999);
                        pMember->field_248_enter_car_as_passenger = 1;
                        pMember->field_154_target_to_enter = field_2C_ped_leader->field_168_game_object->field_84;
                    }
                    else
                    {
                        pMember->SetObjective(0, 9999);
                        pMember->SetObjective2_463830(0x23, 9999);
                        pMember->field_248_enter_car_as_passenger = 1;
                        pMember->field_154_target_to_enter = field_2C_ped_leader->field_16C_car;
                    }
                    pMember->field_168_game_object->sub_545600();
                }

                Car_BC* pTargetCar = pMember->field_154_target_to_enter;
                u8 tries = 0;
                byte_620838 = i + 1;
                u8 maxDoor = pTargetCar->GetRemap() - 1;
                char_type searching;
                if (pTargetCar->field_84_car_info_idx != car_model_enum::SWATVAN && pTargetCar->field_84_car_info_idx != car_model_enum::bank_van)
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

                else
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
                for (s8 j = 0; j < field_34_count; j++)
                {
                    Ped* pOther = field_4_ped_list[j];
                    if (pOther != pMember && pOther->field_25C_internal_objective == 0x12 &&
                        pMember->field_24C_target_car_door == pOther->field_24C_target_car_door)
                    {
                        pMember->SetObjective2_463830(9, 9999);
                        pMember->field_14C = pOther;
                    }
                }
                pMember->field_24C_target_car_door = byte_620838;
            }

            for (; i < field_34_count; i++)
            {
                Ped* pMember = field_4_ped_list[i];
                if (pMember->field_258_objective != 8)
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
            if (distance > dword_67F608 && !field_2C_ped_leader->IsField238_45EDE0(2))
            {
                if (field_2C_ped_leader->field_258_objective != 0xD && field_2C_ped_leader->field_25C_internal_objective != 0x24)
                {
                    field_2C_ped_leader->SetObjective2_463830(9, 9999);
                    field_2C_ped_leader->field_14C = pFarthest;
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
            if (field_4_ped_list[i]->GetBit2() == 1)
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
    return field_2C_ped_leader->field_21C_bf.b2;
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
    if (!field_2C_ped_leader->field_16C_car && (field_2C_ped_leader->field_21C_bf.b27) == 0)
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
                    pIter->field_14C = field_2C_ped_leader;
                }
                else
                {
                    pIter->field_14C = field_4_ped_list[i - 1];
                }
            }
            else if (pIter->field_278_ped_state_1 != 9)
            {
                if (field_2C_ped_leader->GetPedVelocity_45C920() != dword_67F610)
                {
                    switch (i)
                    {
                        case 3u:
                            pIter->field_14C = this->field_4_ped_list[0];
                            break;
                        case 4u:
                            pIter->field_14C = this->field_4_ped_list[1];
                            break;
                        case 5u:
                            pIter->field_14C = this->field_4_ped_list[2];
                            break;
                        case 6u:
                            pIter->field_14C = this->field_4_ped_list[3];
                            break;
                        case 7u:
                            pIter->field_14C = this->field_4_ped_list[4];
                            break;
                        default:
                            pIter->field_14C = field_2C_ped_leader;
                            break;
                    }
                }
                else
                {
                    pIter->field_14C = field_2C_ped_leader;
                }
            }
            pIter->inline_clear_bit();
        }
    }
}

MATCH_FUNC(0x4ca5e0)
void PedGroup::UpdateMemberAIState_4CA5E0(u8 idx)
{
    Ped* pMember = field_4_ped_list[idx];
    if (pMember->field_21C & 0x8000000)
    {
        return;
    }

    if (pMember->field_258_objective == 8)
    {
        if (pMember->field_25C_internal_objective == 9)
        {
            pMember->SetObjective2_463830(0, 9999);
        }
        return;
    }

    if (!field_2C_ped_leader->IsField238_45EDE0(2))
    {
        pMember->field_288_threat_search = field_2C_ped_leader->field_288_threat_search;
        pMember->field_28C_threat_reaction = field_2C_ped_leader->field_28C_threat_reaction;
        pMember->field_17C_pGang = field_2C_ped_leader->field_17C_pGang;
    }
    else
    {
        s32 occupation = pMember->field_240_occupation;
        if (occupation != 0x29)
        {
            if (occupation != 0x2D)
            {
                pMember->field_288_threat_search = 1;
                pMember->field_28C_threat_reaction = 2;
            }
            else
            {
                pMember->field_288_threat_search = 2;
                pMember->field_28C_threat_reaction = 0;
            }
        }
    }

    Ped* pLeader = field_2C_ped_leader;
    if (pLeader->field_168_game_object)
    {
        if (pMember->field_168_game_object)
        {
            if (!(pLeader->field_21C & 0x8000000))
            {
                Ped* pNearest = FindNearestOtherMember_4CAE80(idx);
                if (pNearest)
                {
                    pMember->SetObjective2_463830(20, 9999);
                    pMember->set_field_14C_403AE0(pNearest->field_14C);
                    pMember->SetBit2_403950();
                }
                else if (pMember->field_258_objective != 8)
                {
                    if (pMember->field_240_occupation != 0x17 || pMember->field_258_objective != 0x10)
                    {
                        UpdateFormation_4CA4B0();
                    }
                    if (field_2C_ped_leader->field_25C_internal_objective == 0x3B)
                    {
                        pMember->SetObjective2_463830(0x3B, 9999);
                        pMember->field_154_target_to_enter = field_2C_ped_leader->field_154_target_to_enter;
                    }
                }
                else if (pMember->field_25C_internal_objective == 9)
                {
                    pMember->SetObjective(0, 9999);
                }
            }
        }
        else if (pMember->field_25C_internal_objective != 0x24)
        {
            pMember->SetObjective2_463830(0x24, 9999);
            pMember->field_154_target_to_enter = pMember->field_16C_car;
        }
    }
    else if (pMember->field_25C_internal_objective == 9)
    {
        s32 occupation = pMember->field_240_occupation;
        if ((occupation < 0x18 || occupation > 0x1B) && pMember->field_14C->field_16C_car && pMember->field_258_objective != 8)
        {
            pMember->SetObjective(8, 9999);
            pMember->SetObjective2_463830(0, 9999);
        }
    }

    Ped* pFollow = pMember->field_14C;
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
            pMember->set_field_14C_403AE0(field_2C_ped_leader);
            pMember->inline_clear_bit();
            pMember->SetObjective(0, 9999);
        }
        else if (pMember->field_14C == field_2C_ped_leader && pMember->field_25C_internal_objective == 9)
        {
            pMember->SetObjective2_463830(0, 9999);
        }
    }
    else if (pMember->field_14C == field_2C_ped_leader && pMember->field_25C_internal_objective == 9)
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
        if (field_4_ped_list[i]->field_278_ped_state_1 != ped_state_1::in_car_10)
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

// TODO: close. The original loads pMember's x before pOther's, and an empty group
// skips the final nearest_distance check.
WIP_FUNC(0x4cae80)
Ped* PedGroup::FindNearestOtherMember_4CAE80(u8 idx)
{
    WIP_IMPLEMENTED;

    Fix16 x_abs;
    Fix16 y_abs;
    Fix16 nearest_distance = dword_67F60C;
    u8 nearest_idx = 0;
    Ped* pMember = field_4_ped_list[idx];
    for (u8 i = 0; i < field_34_count; i++)
    {
        Ped* pOther = field_4_ped_list[i];
        if (i != idx)
        {
            Fix16 x_diff = pMember->get_cam_x() - pOther->get_cam_x();
            Fix16 y_diff = pMember->get_cam_y() - pOther->get_cam_y();

            x_abs = Fix16::Abs(x_diff);
            y_abs = Fix16::Abs(y_diff);

            Fix16 distance = (x_abs > y_abs) ? x_abs : y_abs;
            if (distance < nearest_distance && pOther->sub_465CD0())
            {
                nearest_distance = distance;
                nearest_idx = i;
            }
        }
    }

    if (nearest_distance != dword_67F60C)
    {
        return field_4_ped_list[nearest_idx];
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
        if (!pedGroups_67EF20[i].field_40_in_use)
        {
            pedGroups_67EF20[i].Reset_4C8EF0();
            pedGroups_67EF20[i].field_40_in_use = 1;
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