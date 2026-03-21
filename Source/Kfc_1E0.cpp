#include "Kfc_1E0.hpp"
#include "Car_BC.hpp"
#include "Hamburger_500.hpp"
#include "Ped.hpp"
#include "PedGroup.hpp"

DEFINE_GLOBAL(Kfc_1E0*, gKfc_1E0_706280, 0x706280);

MATCH_FUNC(0x4beb00)
Kfc_30::Kfc_30()
{
    sub_5CBC00();
}

MATCH_FUNC(0x4beb10)
Kfc_30::~Kfc_30()
{
}

MATCH_FUNC(0x5cbc00)
void Kfc_30::sub_5CBC00()
{
    field_1A = 150;
    field_1E_is_used = 0;
    field_20_maybe_type = 0;
    field_24 = 0;
    field_0_car = 0;
    field_4_ped = 0;
    field_28 = 0;
    field_8_group = 0;
    field_2C = 0;
    field_1C = 0;
}

MATCH_FUNC(0x5cbc30)
void Kfc_30::sub_5CBC30()
{
    sub_5CBC00();
}

MATCH_FUNC(0x5cbc40)
void Kfc_30::sub_5CBC40(Ped* a2)
{
    field_8_group->RemovePed_4C9970(a2);
    field_4_ped = field_8_group->field_2C_ped_leader;
}

// https://decomp.me/scratch/HmQPr
STUB_FUNC(0x5cbc60)
bool Kfc_30::sub_5CBC60()
{
    WIP_IMPLEMENTED;
    if (field_4_ped && field_4_ped->isDead_403B60())
    {
        return false;
    }
    // TODO: Something strange going on here:
    // 10.5: mov 0x28(%ecx),%ecx
    // 9.6f: mov 0x28(%esi),%esi
    s32 v4 = field_28;
    return true;
}

MATCH_FUNC(0x5cbc90)
char_type Kfc_30::sub_5CBC90()
{
    PedGroup* pGroup = this->field_8_group;
    if (!pGroup)
    {
        return 0;
    }

    u8 idx = 0;
    Ped* pPedAtIdx = pGroup->field_4_ped_list[idx];
    while (pPedAtIdx)
    {
        if (pPedAtIdx->field_278_ped_state_1 != ped_state_1::dead_9 && !pPedAtIdx->field_16C_car)
        {
            Ped* pKfcPed = this->field_4_ped;
            if (pKfcPed->field_16C_car)
            {
                sub_5CBC40(pKfcPed);
            }
            else
            {
                pGroup->PromoteMemberToLeader_4C9680(idx);
                Ped* pLeader = this->field_8_group->field_2C_ped_leader;
                this->field_4_ped = pLeader;
                pLeader->SetObjective(objectives_enum::no_obj_0, 9999);
                const s32 occupation = pPedAtIdx->field_240_occupation;
                pPedAtIdx->field_240_occupation = ped_ocupation_enum::dummy;
                pPedAtIdx->Kill_46F9D0();
                pPedAtIdx->field_240_occupation = occupation;
                pPedAtIdx->SetObjective(objectives_enum::objective_28, 9999);
            }
            return 1;
        }
        idx++;
        pPedAtIdx = pGroup->field_4_ped_list[idx];
    }
    return 0;
}

STUB_FUNC(0x5cbd50)
void Kfc_30::sub_5CBD50()
{
    NOT_IMPLEMENTED;
}

// 9.6f 0x4C5A00
WIP_FUNC(0x5cc1c0)
void Kfc_30::sub_5CC1C0()
{
    WIP_IMPLEMENTED;

    bool bClearRouteAndTryClearOthers = 0;
    bool bClearPedAndGroup = 1;
    bool bClearCharB4F24 = 1;

    if (field_8_group)
    {
        if (field_8_group->field_34_count == 0)
        {
            field_8_group->ClearGroupData_4C8E90();
            this->field_8_group = 0;
        }
    }

    if (this->field_24 == 1)
    {
        if (this->field_0_car && !field_0_car->sub_4215B0() && !field_0_car->IsMaxDamage_40F890())
        {
            if (field_0_car->Get_F76_4A9AD0() > this->field_1A)
            {
                field_0_car->sub_421470();
                bClearRouteAndTryClearOthers = 1;
            }
        }
        else
        {
            bClearRouteAndTryClearOthers = 1;
        }

        if (field_4_ped)
        {
            if (field_4_ped->field_168_game_object)
            {
                if (field_4_ped->Get_F20E_4039F0() < this->field_1A)
                {
                    bClearPedAndGroup = 0;
                }
            }
            else if (field_4_ped->isDead_403B60())
            {
                this->field_4_ped = 0;
            }
        }

        if (field_4_ped)
        {
            if (field_4_ped->field_168_game_object)
            {
                bClearPedAndGroup = 0;
            }
        }

        if (field_8_group)
        {
            u8 i = 0;
            for (Ped* pPedListIter = field_8_group->field_4_ped_list[0]; pPedListIter;)
            {
                if (pPedListIter->field_168_game_object)
                {
                    if (pPedListIter->get_field_20e() < this->field_1A)
                    {
                        bClearPedAndGroup = 0;
                    }
                }
                else
                {
                    bClearCharB4F24 = 0;
                }
                i++;
                pPedListIter = field_8_group->field_4_ped_list[i];
            }
        }

        if (bClearRouteAndTryClearOthers)
        {
            if (field_0_car)
            {
                if (field_0_car->field_60)
                {
                    gHamburger_500_678E30->FreeEntry_474CC0(field_0_car->field_60); // something to do with car route
                    field_0_car->field_60 = 0;
                }
            }
            field_0_car = 0;

            if (bClearPedAndGroup)
            {
                if (field_4_ped)
                {
                    field_4_ped->set_occupation_403970(ped_ocupation_enum::dummy);
                    field_4_ped->ClearGroupAndGroupIdx_403A30();
                    field_4_ped->Deallocate_45EB60();
                }

                if (field_8_group)
                {
                    u8 i = 0;
                    for (Ped* pPedListIter = field_8_group->field_4_ped_list[0]; pPedListIter;)
                    {
                        pPedListIter->set_occupation_403970(ped_ocupation_enum::dummy);
                        pPedListIter->ClearGroupAndGroupIdx_403A30();
                        pPedListIter->Deallocate_45EB60();
                        // TODO: Instruction swap
                        i++;
                        pPedListIter = field_8_group->field_4_ped_list[i];
                    }
                    field_8_group->ClearGroupData_4C8E90();
                }
                this->field_8_group = 0;
                this->field_2C = 1;
            }
            else if (bClearCharB4F24)
            {
                if (this->field_4_ped->field_168_game_object)
                {
                    this->field_24 = 0;
                }
            }
        }
        return;
    }

    if (field_4_ped)
    {
        if (field_4_ped->Get_F20E_4039F0() < this->field_1A)
        {
            bClearPedAndGroup = 0;
        }
    }

    if (field_8_group)
    {
        u8 i = 0;
        for (Ped* pPedListIter = field_8_group->field_4_ped_list[0]; pPedListIter;)
        {
            if (pPedListIter->Get_F20E_4039F0() < this->field_1A)
            {
                bClearPedAndGroup = 0;
            }
            i++;
            pPedListIter = field_8_group->field_4_ped_list[i];
        }
    }

    if (bClearPedAndGroup)
    {
        if (field_4_ped)
        {
            field_4_ped->ClearGroupAndGroupIdx_403A30();
            field_4_ped->Deallocate_45EB60();
        }

        if (field_8_group)
        {
            u8 i = 0;
            Ped* pPedListIter = field_8_group->field_4_ped_list[0];
            while(pPedListIter)
            {
                pPedListIter->ClearGroupAndGroupIdx_403A30();
                pPedListIter->Deallocate_45EB60();
                // TODO: Instruction swap
                i++;
                pPedListIter = field_8_group->field_4_ped_list[i];
            }
            field_8_group->ClearGroupData_4C8E90();
        }
        field_2C = 1;
    }
}

MATCH_FUNC(0x5cc480)
bool Kfc_30::Service_5CC480()
{
    if (field_18 > 0)
    {
        this->field_18--;
        return 0;
    }

    if (field_18 > -80)
    {
        this->field_18--;
    }

    switch (this->field_28)
    {
        case 5:
            sub_5CC1C0();
            return 0;

        case 6:
            sub_5CBD50();
            return 0;

        case 3:
            // fall through to default below
            break;

        default:
            return 0;
    }

    switch (this->field_20_maybe_type)
    {
        case 1:
            if (gCar_6C_677930->CanAllocateOfType_446930(4))
            {
                this->field_0_car = gCar_6C_677930->SpawnCarAtRoadDirection_444CF0(car_model_enum::MEDICAR,
                                                                                   this->field_C_x,
                                                                                   this->field_10_y,
                                                                                   this->field_14_z);
            }
            else
            {
                this->field_0_car = 0;
            }

            if (this->field_0_car)
            {
                this->field_2C = 1;
                field_0_car->IncrementCarStats_443D70(4);
            }
            break;

        case 3:
            if (gCar_6C_677930->CanAllocateOfType_446930(6))
            {
                this->field_0_car = gCar_6C_677930->SpawnCarAtRoadDirection_444CF0(car_model_enum::COPCAR,
                                                                                   this->field_C_x,
                                                                                   this->field_10_y,
                                                                                   this->field_14_z);
            }
            else
            {
                this->field_0_car = 0;
            }
            if (this->field_0_car)
            {
                this->field_2C = 1;
                field_0_car->IncrementCarStats_443D70(6);
            }
            break;

        case 5:
            if (gCar_6C_677930->CanAllocateOfType_446930(6))
            {
                this->field_0_car = gCar_6C_677930->SpawnCarAtRoadDirection_444CF0(car_model_enum::SWATVAN,
                                                                                   this->field_C_x,
                                                                                   this->field_10_y,
                                                                                   this->field_14_z);
            }
            else
            {
                this->field_0_car = 0;
            }

            if (this->field_0_car)
            {
                this->field_2C = 1;
                field_0_car->IncrementCarStats_443D70(6);
            }
            break;

        default:
            this->field_0_car = 0;
            break;
    }

    return 0;
}

MATCH_FUNC(0x4beb20)
Kfc_1E0::~Kfc_1E0()
{
}

MATCH_FUNC(0x5cbb70)
void Kfc_1E0::init_5CBB70()
{
}

MATCH_FUNC(0x5cbb80)
Kfc_30* Kfc_1E0::New_5CBB80()
{
    for (u8 i = 0; i < GTA2_COUNTOF(field_0); i++)
    {
        if (!field_0[i].field_1E_is_used)
        {
            return &field_0[i];
        }
    }
    return 0;
}

MATCH_FUNC(0x5cbbd0)
void Kfc_1E0::sub_5CBBD0()
{
    for (s32 i = 0; i < 10; i++)
    {
        if (field_0[i].field_1E_is_used)
        {
            if (field_0[i].Service_5CC480())
            {
                field_0[i].field_1E_is_used = 0;
            }
        }
    }
}