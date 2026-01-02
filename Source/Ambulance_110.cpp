#include "Ambulance_110.hpp"
#include "Ped.hpp"
#include "PedGroup.hpp"
#include "Kfc_1E0.hpp"
#include "Car_BC.hpp"
#include "Globals.hpp"
#include <stdio.h>

DEFINE_GLOBAL(Ambulance_110*, gAmbulance_110_6F70A8, 0x6F70A8);

DEFINE_GLOBAL(s32, dword_6F6DD4, 0x6F6DD4);

MATCH_FUNC(0x4beab0)
Ambulance_20::Ambulance_20()
{
    field_10.ClearList_420E90();
    sub_4FA7D0();
}

MATCH_FUNC(0x4bead0)
Ambulance_20::~Ambulance_20()
{
}

MATCH_FUNC(0x4fa7d0)
void Ambulance_20::sub_4FA7D0()
{
    field_10.ClearList_420E90();
    field_0 = 0;
    field_1 = 0;
    field_2 = 0;
    field_14_count = 0;
    field_16 = 0;
    field_18 = 0;
    field_1C = 0;
    field_4 = NULL;
    field_8 = NULL;
    field_C = NULL;
    field_1D = 0;
}

MATCH_FUNC(0x4fa800)
void Ambulance_20::AddPassenger_4FA800(Ped* pPed)
{
    field_10.AddPed_471140(pPed);
    field_14_count++;
}

MATCH_FUNC(0x4fa820)
bool Ambulance_20::sub_4FA820()
{
    PedGroup* pGroup = PedGroup::New_4CB0D0();
    if (!pGroup)
    {
        return false;
    }

    if (gPedManager_6787BC->field_5_fbi_army_count >= 30u)
    {
        return false;
    }

    Ped* pPed1 = gPedManager_6787BC->sub_470F30();
    if (!pPed1)
    {
        return false;
    }
    pPed1->field_238 = 4;
    pPed1->field_240_occupation = ped_ocupation_enum::unknown_13;
    pPed1->field_230 = 2;
    pPed1->SpawnPedInCar_45C730(field_4->field_0_car);
    pPed1->SetObjective(objectives_enum::goto_area_in_car_14, 0);
    pPed1->field_1DC_objective_target_x = (unsigned __int8)this->field_0 << 14;
    pPed1->field_1E0_objective_target_y = (unsigned __int8)this->field_1 << 14;
    pPed1->field_1E4_objective_target_z = (unsigned __int8)this->field_2 << 14;
    pPed1->field_28C_threat_reaction = threat_reaction_enum::react_as_emergency_1;
    pPed1->field_288_threat_search = threat_search_enum::no_threats_0;
    pPed1->field_244_remap = 16;
    pPed1->field_26C_graphic_type = 0;
    pPed1->field_1F8 = dword_6F6DD4;

    Ped* pPed2 = gPedManager_6787BC->sub_470F30();
    if (!pPed2)
    {
        return false;
    }

    pPed2->sub_45C7F0(field_4->field_0_car);
    pPed2->field_238 = 4;
    pPed2->field_240_occupation = ped_ocupation_enum::unknown_13;
    pPed2->field_230 = 2;
    pPed2->SetObjective(objectives_enum::no_obj_0, 9999);
    pPed2->field_244_remap = 16;
    pPed2->field_26C_graphic_type = 0;
    pPed2->field_28C_threat_reaction = threat_reaction_enum::react_as_emergency_1;
    pPed2->field_288_threat_search = threat_search_enum::no_threats_0;
    pGroup->add_ped_leader_4C9B10(pPed1);
    pGroup->field_36_count = 1;
    pGroup->field_34_count = 1;
    pGroup->add_ped_to_list_4C9B30(pPed2, 0);
    pGroup->field_0 = 0;
    field_4->field_4_ped = pPed1;
    field_4->field_28 = 6;

    Car_BC* pCar = field_4->field_0_car;

    pCar->field_7C_uni_num = 4;
    pCar->field_76 = 0;

    field_4->field_0_car->SetupCarPhysicsAndSpriteBinding_43BCA0();
    field_4->field_0_car->InitCarAIControl_440590();
    field_4->field_0_car->sub_43AF40();
    field_4->field_8_group = pGroup;
    return true;
}

STUB_FUNC(0x4fa9d0)
char_type Ambulance_20::sub_4FA9D0()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x4faac0)
u32* Ambulance_20::sub_4FAAC0()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x4fb330)
void Ambulance_20::sub_4FB330()
{
    NOT_IMPLEMENTED;
}

MATCH_FUNC(0x4beae0)
Ambulance_110::~Ambulance_110()
{
}

MATCH_FUNC(0x4fa310)
void Ambulance_110::init_4FA310()
{
    field_0 = 1;
    field_1_f8_idx = 0;
    field_4.ClearList_420E90();

    for (s32 i = 0; i < 25; i++)
    {
        field_8[i].field_0 = 0;
        field_8[i].field_4 = 0;
    }
}

STUB_FUNC(0x4fa330)
char_type Ambulance_110::sub_4FA330(Ped* pPed)
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x4fa470)
char_type Ambulance_110::TryAddPassenger_4FA470(Ped* pPed)
{
    if (pPed->sub_45EDE0(2) || field_1_f8_idx >= 25u)
    {
        return 0;
    }

    field_4.AddPed_471140(pPed);
    field_1_f8_idx++;

    return 1;
}

MATCH_FUNC(0x4fa4b0)
Ambulance_20* Ambulance_110::sub_4FA4B0()
{
    for (u8 i = 0; i < 2; i++)
    {
        if (!field_D0[i].field_18)
        {
            return &field_D0[i];
        }
    }
    return 0;
}

STUB_FUNC(0x4fa500)
void Ambulance_110::sub_4FA500()
{
    NOT_IMPLEMENTED;
}

MATCH_FUNC(0x4fa790)
void Ambulance_110::AmbulancesService_4FA790()
{
    if (field_1_f8_idx > 0)
    {
        sub_4FA500();
    }

    for (s32 i = 0; i < 2; i++)
    {
        if (field_D0[i].field_18 == 1)
        {
            field_D0[i].sub_4FB330();
        }
    }
}