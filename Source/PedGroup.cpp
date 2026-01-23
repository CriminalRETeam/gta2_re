#include "PedGroup.hpp"
#include "Globals.hpp"
#include "Ped.hpp"
#include "enums.hpp"

DEFINE_GLOBAL_ARRAY(PedGroup, pedGroups_67EF20, 20, 0x67EF20);
DEFINE_GLOBAL(Fix16, dword_67F610, 0x67F610);
DEFINE_GLOBAL(Fix16, k_dword_67EEE4, 0x67EEE4);

STUB_FUNC(0x4c8e60)
void PedGroup::sub_4C8E60()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x4c8e80)
void PedGroup::sub_4C8E80()
{
    NOT_IMPLEMENTED;
}

MATCH_FUNC(0x4c8e90)
void PedGroup::sub_4C8E90()
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
void PedGroup::sub_4C8EF0()
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
void PedGroup::sub_4C8F20()
{
    field_2C_ped_leader->SetObjective(0, 9999);
    field_2C_ped_leader->sub_463830(0, 9999);

    u8 bVar1 = 0;

    while (bVar1 < field_34_count)
    {
        field_4_ped_list[bVar1]->SetObjective(0, 9999);
        field_4_ped_list[bVar1]->sub_463830(0, 9999);
        bVar1++;
    }
}

MATCH_FUNC(0x4c8f90)
void PedGroup::add_ped_to_end_of_list_4C8F90(Ped* pPed)
{
    pPed->sub_463830(0, 9999);
    pPed->SetObjective(0, 9999);
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
        field_4_ped_list[i]->sub_463830(0, 9999);
    }
}

MATCH_FUNC(0x4c9040)
bool PedGroup::sub_4C9040()
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
void PedGroup::sub_4C91B0()
{
    for (u8 i = 0; i < field_34_count; i++)
    {
        field_4_ped_list[i]->unset_bitset_0x04();
        field_4_ped_list[i]->sub_463830(9, 9999);
        field_4_ped_list[i]->set_field_14C_403AE0(field_2C_ped_leader);
    }
}

MATCH_FUNC(0x4c9210)
bool PedGroup::sub_4C9210()
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
bool PedGroup::sub_4C9220()
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
void PedGroup::sub_4C9240()
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
void PedGroup::sub_4C92A0()
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
            if ((this_00->get_ped_state1() == ped_state1_enum::ped_wasted) || (this_00->field_280_stored_ped_state_1 == ped_state1_enum::ped_wasted))
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
                    this_00->SetObjective(0, 9999);
                }
                (*pppVar1)->sub_463830(0, 9999);
                (*pppVar1)->reset_ped_group();
                (*pppVar1)->set_ped_type(ped_type_enum::New_Name_2);
            }
            (*pppVar1)->field_21C |= 0x400;
        }
    }
    sub_4C8E90();
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
        ppVar2->SetObjective(0, 9999);
        field_2C_ped_leader->sub_463830(0, 9999);
    }

    field_2C_ped_leader->reset_ped_group();
    if (field_4_ped_list[0] != NULL)
    {
        for (u8 bVar5 = 0; bVar5 < field_34_count; bVar5++)
        {
            ppVar2 = field_4_ped_list[bVar5];
            Ped** pppVar1 = field_4_ped_list + bVar5;
            if ((ppVar2->get_ped_state1() == ped_state1_enum::ped_wasted) || (ppVar2->field_280_stored_ped_state_1 == ped_state1_enum::ped_wasted))
            {
                ppVar2->reset_ped_group();
            }
            else
            {
                if (ppVar2->has_field_16C_car() == true)
                {
                    ppVar2->sub_463830(0, 9999);
                    (*pppVar1)->SetObjective(objectives_enum::objective_34, 9999);
                    (*pppVar1)->set_field_150_target_objective_car((*pppVar1)->field_16C_car);
                    (*pppVar1)->reset_ped_group();
                    (*pppVar1)->set_ped_type(ped_type_enum::New_Name_2);
                }
                else
                {
                    ppVar2->SetObjective(0, 9999);
                    (*pppVar1)->sub_463830(0, 9999);
                    (*pppVar1)->reset_ped_group();
                    (*pppVar1)->set_ped_type(ped_type_enum::New_Name_2);
                }
            }
        }
    }
    sub_4C8E90();
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
            this->field_2C_ped_leader->sub_463830(3, 9999);
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
                this->field_4_ped_list[i]->sub_463830(0, 9999);
                this->field_4_ped_list[i]->SetObjective(objectives_enum::flee_char_always_once_car_stopped_6, 9999);
                this->field_4_ped_list[i]->field_148_objective_target_ped = pAttacker;
                this->field_4_ped_list[i]->field_228 = 0;
                this->field_4_ped_list[i]->ClearGroupAndGroupIdx_403A30();
            }
            else
            {
                this->field_4_ped_list[i]->SetObjective(objectives_enum::flee_char_on_foot_always_3, 9999);
                this->field_4_ped_list[i]->field_148_objective_target_ped = pAttacker;
                this->field_4_ped_list[i]->sub_463830(3, 9999);
                this->field_4_ped_list[i]->field_14C = pAttacker;
                this->field_4_ped_list[i]->field_21C |= 4u;
                this->field_4_ped_list[i]->field_228 = 0;
                this->field_4_ped_list[i]->field_168_game_object->field_3C_run_or_jump_speed = k_dword_67EEE4;
                this->field_4_ped_list[i]->ClearGroupAndGroupIdx_403A30();
                this->field_4_ped_list[i]->field_238 = 3;
            }
        }
        PedGroup::sub_4C8E90();
    }
}

STUB_FUNC(0x4c9680)
void PedGroup::sub_4C9680(u8 a2)
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x4c9970)
void PedGroup::RemovePed_4C9970(Ped* a2)
{
    NOT_IMPLEMENTED;
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

STUB_FUNC(0x4c9b60)
char_type PedGroup::sub_4C9B60(Ped* a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

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

STUB_FUNC(0x4c9f00)
void PedGroup::sub_4C9F00()
{
    NOT_IMPLEMENTED;
}

MATCH_FUNC(0x4ca3e0)
u32 PedGroup::sub_4CA3E0()
{
    return field_2C_ped_leader->field_21C_bf.b2;
}

STUB_FUNC(0x4ca3f0)
Ped* PedGroup::sub_4CA3F0(u32* a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x4ca4b0)
void PedGroup::sub_4CA4B0()
{
    if (!field_2C_ped_leader->field_16C_car && (field_2C_ped_leader->field_21C_bf.b27) == 0)
    {
        for (u8 i = 0; i < field_34_count; i++)
        {
            Ped* pIter = field_4_ped_list[i];
            if (pIter->field_25C_car_state != 9)
            {
                if (pIter->field_168_game_object)
                {
                    pIter->sub_463830(9, 9999);
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

STUB_FUNC(0x4ca5e0)
void PedGroup::sub_4CA5E0(u8 idx)
{
    NOT_IMPLEMENTED;
    /*Ped *this_00 = field_4_ped_list[idx];
    if ((this_00->bit_status & 0x8000000) != 0)
    {
        return;
    }

    if (this_00->ped_get_0x258_00403a80() == 8)
    {
        if (this_00->ped_get_0x25c_00403a90() == 9)
        {
            this_00->sub_436920(0, 9999);
        }
    }
    else
    {
        if (ped_leader_ptr->ped_is_ped_type_004333a0(ped_type_enum::ped_player) == false)
        {
            this_00->field124_0x288 = ped_leader_ptr->field124_0x288;
            this_00->field125_0x28c = ped_leader_ptr->field125_0x28c;
            this_00->field25_0x17c = ped_leader_ptr->field25_0x17c;
        }
        else
        {
            int32_t ped_ocupation = this_00->ped_get_occupation_00403980();
            if (ped_ocupation != ped_ocupation_enum::drone)
            {
                if (ped_ocupation != ped_ocupation_enum::refugees)
                {
                    this_00->field124_0x288 = 1;
                    this_00->field125_0x28c = 2;
                }
                else
                {
                    this_00->field124_0x288 = 2;
                    this_00->field125_0x28c = 0;
                }
            }
        }

        ped *ped_leader = ped_leader_ptr;
        if (ped_leader->ped_has_AutoClass30_s1_00403b70())
        {
            if (this_00->ped_has_AutoClass30_s1_00403b70())
            {
                if ((ped_leader->bit_status & 0x8000000) == 0)
                {
                    ped_leader = FUN_00404ad0(param_1);
                    if (ped_leader != NULL)
                    {
                        this_00->FUN_00436920(0x14, 9999);
                        ped_leader = ped_leader->ped_get_ped_ptr_3_00403af0();
                        this_00->ped_set_ped_ptr_3_00403ae0(ped_leader);
                        this_00->ped_FUN_00403950();
                    }
                    else
                    {
                        if (this_00->ped_get_0x258_00403a80() != 8)
                        {
                            if ((this_00->ped_get_occupation_00403980() != ped_ocupation_enum::unknown_13) ||
                                (this_00->ped_get_0x258_00403a80() != 0x10))
                            {
                                FUN_004045d0();
                            }
                            if (ped_leader_ptr->ped_get_0x25c_00403a90() == 0x3b)
                            {
                                this_00->FUN_00436920(0x3b, 9999);
                                AutoClass21_s1 *pAVar4 = ped_leader_ptr->ped_get_AutoClass21_s1_ptr2_00403b10();
                                this_00->ped_set_AutoClass21_s1_ptr2_00403b00(pAVar4);
                            }
                        }
                        else
                        {
                            if (this_00->ped_get_0x25c_00403a90() == 9)
                            {
                                this_00->FUN_0043bbc0(0, 9999);
                            }
                        }
                    }
                }
            }
            else
            {
                if (this_00->ped_get_0x25c_00403a90() != 0x24)
                {
                    this_00->FUN_00436920(0x24, 9999);
                    this_00->ped_set_AutoClass21_s1_ptr2_00403b00(this_00->AutoClass21_s1_ptr);
                }
            }
        }
        else
        {
            int32_t pVar3;
            if (this_00->ped_get_0x25c_00403a90() == 9 &&
                (pVar3 = this_00->ped_get_occupation_00403980(), pVar3 < 0x18 || (0x1b < pVar3)))
            {
                ped_leader = this_00->ped_get_ped_ptr_3_00403af0();
                if ((ped_leader->ped_has_AutoClass21_s1_00403b80()) && (this_00->ped_get_0x258_00403a80() != 8))
                {
                    this_00->FUN_0043bbc0(8, 9999);
                    this_00->FUN_00436920(0, 9999);
                }
            }
        }
        ped_leader = this_00->ped_get_ped_ptr_3_00403af0();
        if (ped_leader == NULL)
        {
            return;
        }
        if (this_00->ped_get_0x25c_00403a90() != 0xb)
        {
            return;
        }
        if (ped_leader->AutoClass30_s1_ptr == NULL)
        {
            this_00->FUN_00436920(0, 9999);
        }
    }*/
}

STUB_FUNC(0x4ca820)
void PedGroup::sub_4CA820(u8 a2)
{
    NOT_IMPLEMENTED;
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
char_type PedGroup::sub_4CAAE0()
{
    for (u8 i = 0; i < field_34_count; i++)
    {
        if (field_4_ped_list[i]->field_278_ped_state_1 != ped_state_1::in_car_10 
            && field_4_ped_list[i]->field_278_ped_state_1 != ped_state_1::dead_9)
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

// https://decomp.me/scratch/p3Ujn
STUB_FUNC(0x4cac20)
bool PedGroup::sub_4CAC20(s32 idx)
{
    NOT_IMPLEMENTED;
    return 0;
}

// https://decomp.me/scratch/MrO9e
STUB_FUNC(0x4cad40)
bool PedGroup::sub_4CAD40()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x4cae80)
Ped* PedGroup::sub_4CAE80(u8 idx)
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x4cb080)
void PedGroup::sub_4CB080()
{
    sub_4C8E80();

    PedGroup* pIter = pedGroups_67EF20;
    for (s32 i = 0; i < 20; i++)
    {
        pIter->sub_4C8EF0();
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
            pedGroups_67EF20[i].sub_4C8EF0();
            pedGroups_67EF20[i].field_40_in_use = 1;
            return &pedGroups_67EF20[i];
        }
    }
    return NULL;
}

MATCH_FUNC(0x4cb860)
PedGroup::PedGroup()
{
    sub_4C8EF0();
}

MATCH_FUNC(0x4cb870)
PedGroup::~PedGroup()
{
}