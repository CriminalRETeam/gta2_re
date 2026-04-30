#include "Ambulance_110.hpp"
#include "Char_Pool.hpp"
#include "Ped.hpp"
#include "PedGroup.hpp"
#include "Kfc_1E0.hpp"
#include "Car_BC.hpp"
#include "Globals.hpp"
#include "error.hpp"
#include <stdio.h>

DEFINE_GLOBAL(Ambulance_110*, gAmbulance_110_6F70A8, 0x6F70A8);

DEFINE_GLOBAL(class Ped*, dword_6F6D60, 0x6F6D60);
DEFINE_GLOBAL_INIT(Fix16, dword_6F6DD4, Fix16(0x1999, 0), 0x6F6DD4);

MATCH_FUNC(0x4beab0)
Ambulance_20::Ambulance_20()
{
    field_10.ClearList_420E90();
    ClearTask_4FA7D0();
}

MATCH_FUNC(0x4bead0)
Ambulance_20::~Ambulance_20()
{
}

MATCH_FUNC(0x4fa7d0)
void Ambulance_20::ClearTask_4FA7D0()
{
    field_10.ClearList_420E90();
    field_0 = 0;
    field_1 = 0;
    field_2 = 0;
    field_14_count = 0;
    field_16 = 0;
    field_18 = 0;
    field_1C = 0;
    field_4_paramedics_crew = NULL;
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
bool Ambulance_20::SpawnParamedicCrew_4FA820()
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
    pPed1->SpawnPedInCar_45C730(field_4_paramedics_crew->field_0_car);
    pPed1->SetObjective(objectives_enum::goto_area_in_car_14, 0);
    pPed1->field_1DC_objective_target_x = (unsigned __int8)this->field_0 << 14;
    pPed1->field_1E0_objective_target_y = (unsigned __int8)this->field_1 << 14;
    pPed1->field_1E4_objective_target_z = (unsigned __int8)this->field_2 << 14;
    pPed1->field_28C_threat_reaction = threat_reaction_enum::react_as_emergency_1;
    pPed1->field_288_threat_search = threat_search_enum::no_threats_0;
    pPed1->field_244_remap = 16;
    pPed1->field_26C_graphic_type = 0;
    pPed1->field_1F8_run_speed = dword_6F6DD4;

    Ped* pPed2 = gPedManager_6787BC->sub_470F30();
    if (!pPed2)
    {
        return false;
    }

    pPed2->EnterCarAsPassenger_45C7F0(field_4_paramedics_crew->field_0_car);
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
    
    field_4_paramedics_crew->field_4_ped = pPed1;
    field_4_paramedics_crew->field_28 = 6;
    field_4_paramedics_crew->field_0_car->sub_421560(4);
    field_4_paramedics_crew->field_0_car->SetupCarPhysicsAndSpriteBinding_43BCA0();
    field_4_paramedics_crew->field_0_car->InitCarAIControl_440590();
    field_4_paramedics_crew->field_0_car->sub_43AF40();
    field_4_paramedics_crew->field_8_group = pGroup;
    return true;
}

MATCH_FUNC(0x4fa9d0)
void Ambulance_20::EvaluatePickupState_4FA9D0()
{
    bool bUnk = false;
    if (field_4_paramedics_crew->field_24 == 2)
    {
        bUnk = true;
        if (!field_4_paramedics_crew->field_4_ped || field_4_paramedics_crew->field_4_ped->GetPedState_403990() == ped_state_1::dead_9)
        {
            field_4_paramedics_crew->field_28 = 5;
            field_4_paramedics_crew->field_2C = 0;
            dword_6F6D60 = 0;
            return;
        }
    }
    if (bUnk)
    {
        if (field_4_paramedics_crew->field_0_car)
        {
            if (field_4_paramedics_crew->field_0_car->IsMaxDamage_40F890())
            {
                field_4_paramedics_crew->field_0_car = 0;
                field_4_paramedics_crew->field_24 = 0;
            }
        }
        else
        {
            field_4_paramedics_crew->field_24 = 0;
        }
    }

    dword_6F6D60 = field_4_paramedics_crew->field_4_ped;
    if (!dword_6F6D60)
    {
        field_4_paramedics_crew->field_24 = 2;
    }
    else if (field_1C)
    {
        if (!field_4_paramedics_crew->field_24)
        {
            field_4_paramedics_crew->field_28 = 5;
            field_4_paramedics_crew->field_2C = 0;
        }
        else if (dword_6F6D60->field_16C_car && dword_6F6D60 == dword_6F6D60->field_16C_car->field_54_driver)
        {
            if (field_4_paramedics_crew->field_8_group)
            {
                if (field_4_paramedics_crew->field_8_group->IsAllMembersInSomeCar_4CAA20())
                {
                    field_4_paramedics_crew->field_0_car->sub_43AF40();
                    field_4_paramedics_crew->field_28 = 5;
                    field_4_paramedics_crew->field_2C = 0;
                    dword_6F6D60 = 0;
                }
                else
                {
                    field_4_paramedics_crew->field_0_car->sub_43AF60();
                }
            }
        }
    }
}

STUB_FUNC(0x4faac0)
void Ambulance_20::HandleObjectiveState_4FAAC0()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x4fb330)
void Ambulance_20::UpdateState_4FB330()
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

MATCH_FUNC(0x4fa330)
bool Ambulance_110::HandlePedDeath_4FA330(Ped* pDeadPed)
{
    for (u8 i = 0; i < 2; i++)
    {
        Ambulance_20* pIter = &field_D0[i];
        if (pIter->field_18)
        {
            if (pIter->field_4_paramedics_crew->field_4_ped == pDeadPed) // the dead person is one of the paramedics?
            {
                char_type v9 = pIter->field_4_paramedics_crew->ReplaceLeaderIfNeeded_5CBC90();
                if (pIter->field_4_paramedics_crew->field_8_group)
                {
                    pIter->field_4_paramedics_crew->field_4_ped = pIter->field_4_paramedics_crew->field_8_group->field_2C_ped_leader;
                }

                if (pIter->field_8 && pIter->field_8->field_278_ped_state_1 == ped_state_1::dead_9)
                {
                    TryAddPatient_4FA470(pIter->field_8);
                    pIter->field_8 = 0;
                }

                if (pIter->field_C && pIter->field_C->field_278_ped_state_1 == ped_state_1::dead_9)
                {
                    TryAddPatient_4FA470(pIter->field_C);
                    pIter->field_C = 0;
                }

                if (v9 != 0)
                {
                    return true;
                }

                if (pDeadPed->field_168_game_object)
                {
                    pDeadPed->SetObjective(objectives_enum::objective_28, 9999);
                    return false;
                }
                return false;
            }

            if (pIter->field_4_paramedics_crew->field_8_group && pIter->field_4_paramedics_crew->field_8_group->field_4_ped_list[0] == pDeadPed)
            {
                if (pDeadPed->field_16C_car)
                {
                    pIter->field_4_paramedics_crew->RemovePed_5CBC40(pDeadPed);
                }

                if (pIter->field_C && pIter->field_C->field_278_ped_state_1 == ped_state_1::dead_9)
                {
                    TryAddPatient_4FA470(pIter->field_C);
                }

                if (pDeadPed->field_168_game_object)
                {
                    pDeadPed->SetObjective(objectives_enum::objective_28, 9999);
                }

                return false;
            }
        }
    }
    return false;
}

MATCH_FUNC(0x4fa470)
char_type Ambulance_110::TryAddPatient_4FA470(Ped* pPed)
{
    if (pPed->IsField238_45EDE0(2) || field_1_f8_idx >= 25)
    {
        return 0;
    }

    field_4.AddPed_471140(pPed);
    field_1_f8_idx++;

    return 1;
}

MATCH_FUNC(0x4fa4b0)
Ambulance_20* Ambulance_110::AllocateTaskSlot_4FA4B0()
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
void Ambulance_110::ProcessPatientQueue_4FA500()
{
    NOT_IMPLEMENTED;
}

MATCH_FUNC(0x4fa790)
void Ambulance_110::AmbulancesService_4FA790()
{
    if (field_1_f8_idx > 0)
    {
        ProcessPatientQueue_4FA500();
    }

    for (s32 i = 0; i < 2; i++)
    {
        if (field_D0[i].field_18 == 1)
        {
            field_D0[i].UpdateState_4FB330();
        }
    }
}