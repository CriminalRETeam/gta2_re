#include "Kfc_1E0.hpp"
#include "Car_BC.hpp"
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

STUB_FUNC(0x5cbc60)
bool Kfc_30::sub_5CBC60()
{
    NOT_IMPLEMENTED;
    Ped* pPed = this->field_4_ped;
    if (pPed && pPed->field_278_ped_state_1 == ped_state_1::dead_9)
    {
        return false;
    }
    // TODO: Something strange going on here:
    // mov 0x28(%ecx),%ecx
    //this = (Kfc_30*)field_28;
    return true;
}

WIP_FUNC(0x5cbc90)
char_type Kfc_30::sub_5CBC90()
{
    WIP_IMPLEMENTED;

    PedGroup* pGroup; // ecx
    Ped* pPedAtIdx; // esi
    Ped* pKfcPed; // eax
    Ped* pLeader; // ecx
    s32 occupation; // edi
    u8 idx; // [esp+8h] [ebp-4h]

    pGroup = this->field_8_group;
    if (!pGroup)
    {
        return 0;
    }

    pPedAtIdx = pGroup->field_4_ped_list[0];
    idx = 0;
    if (!pPedAtIdx)
    {
        return 0;
    }
    while (pPedAtIdx->field_278_ped_state_1 == ped_state_1::dead_9 || pPedAtIdx->field_16C_car)
    {
        pPedAtIdx = pGroup->field_4_ped_list[++idx];
        if (!pPedAtIdx)
        {
            return 0;
        }
    }
    pKfcPed = this->field_4_ped;
    if (!pKfcPed->field_16C_car)
    {
        pGroup->sub_4C9680(idx);
        pLeader = this->field_8_group->field_2C_ped_leader;
        this->field_4_ped = pLeader;
        pLeader->SetObjective(objectives_enum::no_obj_0, 9999);
        occupation = pPedAtIdx->field_240_occupation;
        pPedAtIdx->field_240_occupation = ped_ocupation_enum::dummy;
        pPedAtIdx->Kill_46F9D0();
        pPedAtIdx->field_240_occupation = occupation;
        pPedAtIdx->SetObjective(objectives_enum::objective_28, 9999);
    }
    else
    {
        sub_5CBC40(pKfcPed);
    }
    return 1;
}

STUB_FUNC(0x5cbd50)
void Kfc_30::sub_5CBD50()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x5cc1c0)
void Kfc_30::sub_5CC1C0()
{
    NOT_IMPLEMENTED;
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
                this->field_0_car =
                    gCar_6C_677930->sub_444CF0(car_model_enum::MEDICAR, this->field_C_x, this->field_10_y, this->field_14_z);
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
                this->field_0_car = gCar_6C_677930->sub_444CF0(car_model_enum::COPCAR, this->field_C_x, this->field_10_y, this->field_14_z);
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
                this->field_0_car =
                    gCar_6C_677930->sub_444CF0(car_model_enum::SWATVAN, this->field_C_x, this->field_10_y, this->field_14_z);
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