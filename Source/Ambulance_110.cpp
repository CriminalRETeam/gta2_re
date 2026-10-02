#include "Ambulance_110.hpp"
#include "Char_Pool.hpp"
#include "Ped.hpp"
#include "PedGroup.hpp"
#include "Kfc_1E0.hpp"
#include "Car_BC.hpp"
#include "Globals.hpp"
#include "error.hpp"
#include "Orca_2FD4.hpp"
#include "CarAI_78.hpp"
#include "Hamburger_500.hpp"
#include "RouteFinder.hpp"
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
    pPed1->SetField238_403920(ped_type::special_ped_4);
    pPed1->set_occupation_403970(ped_ocupation_enum::paramedic_23);
    pPed1->sub_433BB0(2);
    pPed1->SpawnPedInCar_45C730(field_4_paramedics_crew->field_0_car);
    pPed1->SetObjective(objectives_enum::goto_area_in_car_14, 0);
    pPed1->field_1DC_objective_target_x = (unsigned __int8)this->field_0 << 14;
    pPed1->field_1E0_objective_target_y = (unsigned __int8)this->field_1 << 14;
    pPed1->field_1E4_objective_target_z = (unsigned __int8)this->field_2 << 14;
    pPed1->field_28C_threat_reaction = threat_reaction_enum::react_as_emergency_1;
    pPed1->field_288_threat_search = threat_search_enum::no_threats_0;
    pPed1->set_remap_433B90(16);
    pPed1->field_26C_graphic_type = 0;
    pPed1->field_1F8_run_speed = dword_6F6DD4;

    Ped* pPed2 = gPedManager_6787BC->sub_470F30();
    if (!pPed2)
    {
        return false;
    }

    pPed2->EnterCarAsPassenger_45C7F0(field_4_paramedics_crew->field_0_car);
    pPed2->SetField238_403920(ped_type::special_ped_4);
    pPed2->set_occupation_403970(ped_ocupation_enum::paramedic_23);
    pPed2->sub_433BB0(2);
    pPed2->SetObjective(objectives_enum::no_obj_0, 9999);
    pPed2->set_remap_433B90(16);
    pPed2->field_26C_graphic_type = 0;
    pPed2->field_28C_threat_reaction = threat_reaction_enum::react_as_emergency_1;
    pPed2->field_288_threat_search = threat_search_enum::no_threats_0;
    pGroup->add_ped_leader_4C9B10(pPed1);
    pGroup->SetCounts_433360(1);
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
        if (!field_4_paramedics_crew->field_4_ped || field_4_paramedics_crew->field_4_ped->isDead_403B60())
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

EXTERN_GLOBAL(Fix16, dword_6F6FC0);

// Runs the paramedic crew (the leader, then each group member, in dword_6F6D60): pick up the
// patients in field_10 one by one, revive them, then drive off. Sets field_1C when nobody had
// anything left to do.
WIP_FUNC(0x4faac0)
void Ambulance_20::HandleObjectiveState_4FAAC0()
{
    u8 bBusy = 0;
    u8 bCop = 0;
    u8 i = 0;
    dword_6F6D60 = field_4_paramedics_crew->field_4_ped;
    u8 bNoCar = field_4_paramedics_crew->field_24 == 0;
    Ped* pTarget = field_8;

    while (dword_6F6D60)
    {
        if (pTarget)
        {
            if (!(pTarget->field_21C & 1))
            {
                if (dword_6F6D60->GetPedState_403990() != 9)
                {
                    dword_6F6D60->SetObjective(0, 9999);
                }
                if (pTarget == field_8)
                {
                    field_8 = 0;
                }
                else
                {
                    field_C = 0;
                }
            }
            else if (dword_6F6D60->field_168_game_object && pTarget->sub_4701D0())
            {
                gAmbulance_110_6F70A8->TryAddPatient_4FA470(pTarget);
                if (dword_6F6D60->GetPedState_403990() != 9)
                {
                    dword_6F6D60->SetObjective(0, 9999);
                }
                if (pTarget == field_8)
                {
                    field_8 = 0;
                }
                else
                {
                    field_C = 0;
                }
            }
        }

        Ped* pPatient;
        switch (dword_6F6D60->get_objective_403A80())
        {
            case 14:
                bBusy = 1;
                if (dword_6F6D60->field_16C_car)
                {
                    Car_BC* pCar = field_4_paramedics_crew->field_0_car;
                    if (pCar->field_76_last_seen_timer > 1000)
                    {
                        bBusy = 0;
                    }
                    if (dword_6F6D60->sub_450CB0() == 1)
                    {
                        pCar->sub_43AF60();
                        field_4_paramedics_crew->field_28 = 6;
                        dword_6F6D60->SetObjective(0, 9999);
                    }
                    else
                    {
                        Fix16 dx = Fix16((u8)field_0) - dword_6F6D60->field_1AC_cam.x;
                        Fix16 dy = Fix16((u8)field_1) - dword_6F6D60->field_1AC_cam.y;
                        if (Fix16::Max_44E540(Fix16::Abs_436A50(dx), Fix16::Abs_436A50(dy)) >= dword_6F6FC0)
                        {
                            break;
                        }
                        if (!field_10.GetPedsCount_4716B0())
                        {
                            bBusy = 0;
                            break;
                        }
                        if (dword_6F6D60->get_objective_timer_403B30() <= 50)
                        {
                            break;
                        }
                        field_4_paramedics_crew->field_0_car->sub_43AF60();
                        field_4_paramedics_crew->field_28 = 6;
                        dword_6F6D60->SetObjective(0, 9999);
                    }
                }
                else
                {
                    field_4_paramedics_crew->field_28 = 6;
                    dword_6F6D60->SetObjective(0, 9999);
                }
                break;

            case 0:
                if (dword_6F6D60->field_23C == 99)
                {
                    pPatient = field_10.RemoveFirstPed_471320();
                    if (!pPatient)
                    {
                        if (dword_6F6D60->field_16C_car)
                        {
                            dword_6F6D60->SetObjective(0, 9999);
                            dword_6F6D60->ChangeNextPedState1_45C500(10);
                            dword_6F6D60->ChangeNextPedState2_45C540(10);
                            if (field_4_paramedics_crew->field_0_car)
                            {
                                CarAI_78* pAI = field_4_paramedics_crew->field_0_car->field_5C_AI;
                                if (pAI && pAI->field_28_junc_idx > 0)
                                {
                                    gRouteFinder_6FFDC8->CancelRoute_589930(pAI->field_28_junc_idx);
                                    field_4_paramedics_crew->field_0_car->field_5C_AI->field_28_junc_idx = -1;
                                }
                                if (field_4_paramedics_crew->field_0_car->field_60)
                                {
                                    gHamburger_500_678E30->FreeEntry_474CC0(field_4_paramedics_crew->field_0_car->field_60);
                                    field_4_paramedics_crew->field_0_car->field_60 = 0;
                                }
                                if (field_4_paramedics_crew->field_8_group->IsAllMembersInSomeCar_4CAA20())
                                {
                                    field_4_paramedics_crew->field_0_car->sub_43AF40();
                                    bBusy = 0;
                                }
                                else
                                {
                                    field_4_paramedics_crew->field_0_car->sub_43AF60();
                                }
                            }
                        }
                        else if (bNoCar || (dword_6F6D60->field_21C & 0x8000000))
                        {
                            bBusy = 0;
                        }
                        else
                        {
                            dword_6F6D60->SetObjective2_463830(0, 9999);
                            dword_6F6D60->SetObjective(35, 9999);
                            dword_6F6D60->set_field_150_target_objective_car(field_4_paramedics_crew->field_0_car);
                            dword_6F6D60->set_enter_car_as_passenger_4039B0(0);
                            dword_6F6D60->set_target_car_door_403A70(0);
                        }
                    }
                    else if (pPatient->sub_4701D0())
                    {
                        if (dword_6F6D60->field_16C_car)
                        {
                            dword_6F6D60->SetObjective(0, 9999);
                            dword_6F6D60->ChangeNextPedState1_45C500(10);
                            dword_6F6D60->ChangeNextPedState2_45C540(10);
                        }
                        else if (!bNoCar && !(dword_6F6D60->field_21C & 0x8000000))
                        {
                            dword_6F6D60->SetObjective2_463830(0, 9999);
                            dword_6F6D60->SetObjective(35, 9999);
                            dword_6F6D60->set_field_150_target_objective_car(field_4_paramedics_crew->field_0_car);
                            dword_6F6D60->set_enter_car_as_passenger_4039B0(0);
                            dword_6F6D60->set_target_car_door_403A70(0);
                        }
                        bBusy = 1;
                        field_14_count--;
                    }
                    else if (dword_6F6D60->field_16C_car && !(dword_6F6D60->field_21C & 0x8000000))
                    {
                        dword_6F6D60->SetObjective(36, 9999);
                        bBusy = 1;
                        dword_6F6D60->set_field_150_target_objective_car(field_4_paramedics_crew->field_0_car);
                        dword_6F6D60->SetField238_403920(4);
                    }
                    else
                    {
                        dword_6F6D60->SetObjective2_463830(0, 9999);
                        dword_6F6D60->SetObjective(16, 9999);
                        bBusy = 1;
                        dword_6F6D60->set_objective_target_ped_403AC0(pPatient);
                    }
                    field_8 = pPatient;
                }
                else if (dword_6F6D60->GetInternalObjective_403A90() == 9)
                {
                    pPatient = field_10.RemoveFirstPed_471320();
                    if (pPatient)
                    {
                        dword_6F6D60->SetObjective2_463830(0, 9999);
                        dword_6F6D60->SetObjective(16, 9999);
                        dword_6F6D60->set_objective_target_ped_403AC0(pPatient);
                        field_14_count--;
                        field_4_paramedics_crew->field_8_group->field_30 = 1;
                    }
                    field_C = pPatient;
                }
                break;

            case 28:
                if (dword_6F6D60->sub_450CB0())
                {
                    dword_6F6D60->field_278_ped_state_1 = 0;
                    dword_6F6D60->field_27C_ped_state_2 = 0;
                    dword_6F6D60->SetObjective(0, 9999);
                    dword_6F6D60->SetObjective2_463830(0, 9999);
                    Ped* pLeader = field_4_paramedics_crew->field_4_ped;
                    if (dword_6F6D60 != pLeader && pLeader->isDead_403B60())
                    {
                        field_4_paramedics_crew->field_8_group->PromoteMemberToLeader_4C9680(0);
                        field_4_paramedics_crew->field_4_ped = field_4_paramedics_crew->field_8_group->field_2C_ped_leader;
                        field_4_paramedics_crew->field_4_ped->SetObjective(0, 9999);
                        field_4_paramedics_crew->field_4_ped->SetObjective2_463830(0, 9999);
                    }
                    field_4_paramedics_crew->field_28 = 6;
                }
                break;

            case 36:
                if (dword_6F6D60->sub_450CB0())
                {
                    dword_6F6D60->SetObjective2_463830(0, 9999);
                    dword_6F6D60->SetObjective(16, 9999);
                    dword_6F6D60->set_objective_target_ped_403AC0(field_8);
                }
                bBusy = 1;
                break;

            case 16:
            {
                Ped* pVictim = dword_6F6D60->get_objective_target_ped_403AD0();
                if (dword_6F6D60->sub_450CB0())
                {
                    if (++field_1D == 50)
                    {
                        if (pVictim->field_28C_threat_reaction == 1)
                        {
                            bCop = pVictim->get_occupation_403980() == 23;
                        }
                        pVictim->SetObjective2_463830(0, 9999);
                        if (!bCop)
                        {
                            pVictim->SetObjective2_463830(0, 9999);
                            pVictim->field_278_ped_state_1 = 0;
                            pVictim->field_27C_ped_state_2 = 0;
                            pVictim->SetObjective(1, 9999);
                            pVictim->field_1DC_objective_target_x = pVictim->get_cam_x();
                            pVictim->field_1E0_objective_target_y = pVictim->get_cam_y();
                            pVictim->field_1E4_objective_target_z = pVictim->get_cam_z();
                            pVictim->SetField238_403920(3);
                            pVictim->set_occupation_403970(3);
                            pVictim->field_28C_threat_reaction = 3;
                        }
                        else
                        {
                            pVictim->set_objective_status_403B40(1);
                        }
                        pVictim->set_health_4039A0(100);
                        pVictim->field_21C &= ~4;
                        if (pVictim == field_8)
                        {
                            field_8 = 0;
                        }
                        else
                        {
                            field_C = 0;
                        }
                        dword_6F6D60->SetObjective(0, 9999);
                        field_1D = 0;
                    }
                }
                bBusy = 1;
                if (field_4_paramedics_crew->field_8_group)
                {
                    field_4_paramedics_crew->field_8_group->field_1 = 0;
                }
                break;
            }

            case 35:
                if (dword_6F6D60->field_23C != 99)
                {
                    dword_6F6D60->SetObjective(0, 9999);
                    dword_6F6D60->SetObjective2_463830(0, 9999);
                    break;
                }
                if (dword_6F6D60->field_16C_car)
                {
                    dword_6F6D60->SetObjective(0, 9999);
                    dword_6F6D60->SetObjective2_463830(0, 9999);
                    dword_6F6D60->ChangeNextPedState1_45C500(10);
                    dword_6F6D60->ChangeNextPedState2_45C540(10);
                }
                else
                {
                    pPatient = field_10.RemoveFirstPed_471320();
                    if (!pPatient)
                    {
                        if (field_4_paramedics_crew->field_24 == 1)
                        {
                            if (field_4_paramedics_crew->field_8_group)
                            {
                                field_4_paramedics_crew->field_8_group->field_1 = 1;
                            }
                        }
                        else
                        {
                            dword_6F6D60->SetObjective(0, 9999);
                            dword_6F6D60->SetObjective2_463830(0, 9999);
                        }
                    }
                    else if (pPatient->sub_4701D0())
                    {
                        gAmbulance_110_6F70A8->TryAddPatient_4FA470(pTarget);
                        if (field_4_paramedics_crew->field_24 == 1)
                        {
                            if (field_4_paramedics_crew->field_8_group)
                            {
                                field_4_paramedics_crew->field_8_group->field_1 = 1;
                            }
                        }
                        else
                        {
                            dword_6F6D60->SetObjective(0, 9999);
                            dword_6F6D60->SetObjective2_463830(0, 9999);
                        }
                    }
                    else
                    {
                        dword_6F6D60->SetObjective2_463830(0, 9999);
                        dword_6F6D60->SetObjective(16, 9999);
                        dword_6F6D60->set_objective_target_ped_403AC0(pPatient);
                        field_14_count--;
                        if (dword_6F6D60->field_23C == 99)
                        {
                            field_8 = pPatient;
                        }
                        else
                        {
                            field_C = pPatient;
                        }
                    }
                }
                bBusy = 1;
                break;
        }

        if (field_4_paramedics_crew->field_8_group)
        {
            dword_6F6D60 = field_4_paramedics_crew->field_8_group->field_4_ped_list[i];
            pTarget = field_C;
        }
        else
        {
            dword_6F6D60 = NULL;
        }
        i++;
    }

    if (!bBusy)
    {
        field_1C = 1;
    }
}

// near match https://decomp.me/scratch/cxgie
WIP_FUNC(0x4fb330)
void Ambulance_20::UpdateState_4FB330()
{
    field_10.RemovePedsInSpecificState_471290();
    switch (field_4_paramedics_crew->field_28)
    {
        case 3:
        {
            if (field_4_paramedics_crew->field_2C)
            {
                if (!SpawnParamedicCrew_4FA820())
                {
                    if (field_4_paramedics_crew->field_0_car)
                    {
                        field_4_paramedics_crew->field_0_car->sub_421470();
                    }
                    field_4_paramedics_crew->ReInit_5CBC30();
                    ClearTask_4FA7D0();
                    break;
                }
                else
                {
                    dword_6F6D60 = field_4_paramedics_crew->field_4_ped;
                    this->field_1C = 0;
                    if (field_4_paramedics_crew->field_0_car)
                    {
                        field_4_paramedics_crew->field_0_car->ActivateEmergencyLights_43C920();
                        HandleObjectiveState_4FAAC0();
                        break;
                    }
                    HandleObjectiveState_4FAAC0();
                    break;
                }
            }
            else
            {
                ++field_4_paramedics_crew->field_1C;
                if (field_4_paramedics_crew->field_1C > 500)
                {
                    field_4_paramedics_crew->field_28 = 5;
                }
            }
            break;
        }
        case 5:
        {
            while (!field_10.IsEmpty_420EA0())
            {
                Ped* pPed = field_10.RemoveFirstPed_471320();
                gAmbulance_110_6F70A8->TryAddPatient_4FA470(pPed);
            }

            if (field_4_paramedics_crew->field_2C)
            {
                field_4_paramedics_crew->field_28 = 0;
                field_4_paramedics_crew->ReInit_5CBC30();
                ClearTask_4FA7D0();
                break;
            }
            HandleObjectiveState_4FAAC0();
            break;
        }
        case 6:
        {
            EvaluatePickupState_4FA9D0();
            HandleObjectiveState_4FAAC0();
            break;
        }
        default:
            FatalError_4A38C0(Gta2Error::InvalidCase, "C:\\Splitting\\Gta2\\Source\\medical.cpp", 1087);
            HandleObjectiveState_4FAAC0();
    }
    return;
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

                if (pIter->field_8 && pIter->field_8->GetPedState_403990() == ped_state_1::dead_9)
                {
                    TryAddPatient_4FA470(pIter->field_8);
                    pIter->field_8 = 0;
                }

                if (pIter->field_C && pIter->field_C->GetPedState_403990() == ped_state_1::dead_9)
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

                if (pIter->field_C && pIter->field_C->GetPedState_403990() == ped_state_1::dead_9)
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

DEFINE_GLOBAL(Fix16, dword_6F6FC0, 0x6F6FC0);

WIP_FUNC(0x4fa500)
void Ambulance_110::ProcessPatientQueue_4FA500()
{
    field_1_f8_idx -= field_4.RemovePedsInSpecificState_471290();
    if (field_1_f8_idx == 0)
    {
        return;
    }

    Ped* pPed = field_4.RemoveFirstPed_471320();
    if (pPed->sub_4701D0())
    {
        field_1_f8_idx--;
        gAmbulance_110_6F70A8->TryAddPatient_4FA470(pPed);
        return;
    }

    {
        u8 x = pPed->field_1AC_cam.x.ToInt();
        u8 y = pPed->field_1AC_cam.y.ToInt();
        u8 z = pPed->field_1AC_cam.z.ToInt();
        if (!gOrca_2FD4_6FDEF0->FindNearbyTileMatchingSlopeType_5552B0(1, &x, &y, &z, 0))
        {
            field_1_f8_idx--;
            pPed->SetObjective(objectives_enum::objective_50, 9999);
            return;
        }

        for (u8 i = 0; i < 2; i++)
        {
            Ambulance_20* pAmbulance = &field_D0[i];
            if (1 == pAmbulance->field_18 && pAmbulance->field_4_paramedics_crew->PedIsValid_5CBC60())
            {
                Fix16 dx = Fix16((u8)pAmbulance->field_0) - pPed->field_1AC_cam.x;
                Fix16 dy = Fix16((u8)pAmbulance->field_1) - pPed->get_cam_y();
                Fix16 abs_dy = Fix16::Abs_negate_out_of_line(dy);
                Fix16 abs_dx = Fix16::Abs_negate_out_of_line(dx);
                if (Fix16::Max_44E540(abs_dx, abs_dy) < dword_6F6FC0 &&
                    (u8)pAmbulance->field_14_count < 10)
                {
                    pAmbulance->AddPassenger_4FA800(pPed);
                    field_1_f8_idx--;
                    Kfc_30* pCrew = pAmbulance->field_4_paramedics_crew;
                    if (pCrew->field_28 != 6)
                    {
                        if (pCrew->field_28 == 5)
                        {
                            pAmbulance->field_0 = x;
                            pAmbulance->field_1 = y;
                            pAmbulance->field_2 = z;
                            pCrew->field_28 = 6;
                        }
                    }
                    return;
                }
            }
        }

        Ambulance_20* pNew = AllocateTaskSlot_4FA4B0();
        if (pNew)
        {
            pNew->field_18 = 1;
            pNew->field_0 = x;
            pNew->field_1 = y;
            pNew->field_2 = z;
            pNew->field_4_paramedics_crew = gKfc_1E0_706280->New_5CBB80();
            if (!pNew->field_4_paramedics_crew)
            {
                field_1_f8_idx--;
                pPed->SetObjective(objectives_enum::objective_50, 9999);
                pNew->ClearTask_4FA7D0();
                return;
            }

            Kfc_30* pCrew = pNew->field_4_paramedics_crew;
            pCrew->field_1E_is_used = 1;
            pCrew->field_20_maybe_type = 1;
            pCrew->field_24 = 1;
            pCrew->field_28 = 3;
            pCrew->field_18 = 300;
            pCrew->field_1C = 0;
            pCrew->field_C_x = Fix16(x);
            pCrew->field_10_y = Fix16(y);
            pCrew->field_14_z = Fix16(z);
            pNew->AddPassenger_4FA800(pPed);
        }
    }
    field_1_f8_idx--;
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