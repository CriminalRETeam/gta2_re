#include "AmbulanceService.hpp"
#include "ped_jump_over_mode.hpp"
#include "Char_Pool.hpp"
#include "Ped.hpp"
#include "PedGroup.hpp"
#include "EmergencyCrewPool.hpp"
#include "Car.hpp"
#include "Globals.hpp"
#include "error.hpp"
#include "PathFinder.hpp"
#include "CarAI.hpp"
#include "CarChaseTaskTable.hpp"
#include "RouteFinder.hpp"
#include <stdio.h>
#include "ped_graphic_type.hpp"

DEFINE_GLOBAL(AmbulanceService*, gAmbulance_110_6F70A8, 0x6F70A8);

DEFINE_GLOBAL(class Ped*, gParamedicCrewPed_6F6D60, 0x6F6D60);
DEFINE_GLOBAL_INIT(Fix16, gParamedicRunSpeed_6F6DD4, Fix16(0x1999, 0), 0x6F6DD4);

MATCH_FUNC(0x4beab0)
AmbulanceTask::AmbulanceTask()
{
    field_10_patients.ClearList_420E90();
    ClearTask_4FA7D0();
}

MATCH_FUNC(0x4bead0)
AmbulanceTask::~AmbulanceTask()
{
}

MATCH_FUNC(0x4beae0)
AmbulanceService::~AmbulanceService()
{
}

MATCH_FUNC(0x4fa310)
void AmbulanceService::init_4FA310()
{
    field_0_unused = 1;
    field_1_corpse_count = 0;
    field_4_patient_queue.ClearList_420E90();

    for (s32 i = 0; i < 25; i++)
    {
        field_8_unused_slots[i].field_0_unused = 0;
        field_8_unused_slots[i].field_4_unused = 0;
    }
}

MATCH_FUNC(0x4fa330)
bool AmbulanceService::HandlePedDeath_4FA330(Ped* pDeadPed)
{
    for (u8 i = 0; i < 2; i++)
    {
        AmbulanceTask* pTask = &field_D0_tasks[i];
        if (pTask->field_18_in_use)
        {
            if (pTask->field_4_paramedics_crew->field_4_ped == pDeadPed) // the crew leader died
            {
                bool bLeaderReplaced = pTask->field_4_paramedics_crew->ReplaceLeaderIfNeeded_5CBC90();
                if (pTask->field_4_paramedics_crew->field_8_group)
                {
                    pTask->field_4_paramedics_crew->field_4_ped = pTask->field_4_paramedics_crew->field_8_group->field_2C_ped_leader;
                }

                if (pTask->field_8_leader_patient && pTask->field_8_leader_patient->GetPedState_403990() == ped_state_1::dead_9)
                {
                    TryAddPatient_4FA470(pTask->field_8_leader_patient);
                    pTask->field_8_leader_patient = NULL;
                }

                if (pTask->field_C_member_patient && pTask->field_C_member_patient->GetPedState_403990() == ped_state_1::dead_9)
                {
                    TryAddPatient_4FA470(pTask->field_C_member_patient);
                    pTask->field_C_member_patient = NULL;
                }

                if (bLeaderReplaced)
                {
                    return true;
                }

                if (pDeadPed->field_168_game_object)
                {
                    pDeadPed->SetObjective(objectives_enum::objective_28);
                    return false;
                }
                return false;
            }

            if (pTask->field_4_paramedics_crew->field_8_group && pTask->field_4_paramedics_crew->field_8_group->field_4_ped_list[0] == pDeadPed)
            {
                if (pDeadPed->field_16C_car)
                {
                    pTask->field_4_paramedics_crew->RemovePed_5CBC40(pDeadPed);
                }

                if (pTask->field_C_member_patient && pTask->field_C_member_patient->GetPedState_403990() == ped_state_1::dead_9)
                {
                    TryAddPatient_4FA470(pTask->field_C_member_patient);
                }

                if (pDeadPed->field_168_game_object)
                {
                    pDeadPed->SetObjective(objectives_enum::objective_28);
                }

                return false;
            }
        }
    }
    return false;
}

MATCH_FUNC(0x4fa470)
bool AmbulanceService::TryAddPatient_4FA470(Ped* pPed)
{
    if (pPed->PedTypeIs_45EDE0(ped_type::player_2) || field_1_corpse_count >= 25)
    {
        return false;
    }

    field_4_patient_queue.AddPedToFront_471140(pPed);
    field_1_corpse_count++;

    return true;
}

EXTERN_GLOBAL(Fix16, kFpEight_6F6FC0);

MATCH_FUNC(0x4fa4b0)
AmbulanceTask* AmbulanceService::AllocateTaskSlot_4FA4B0()
{
    for (u8 i = 0; i < 2; i++)
    {
        if (!field_D0_tasks[i].field_18_in_use)
        {
            return &field_D0_tasks[i];
        }
    }
    return NULL;
}

MATCH_FUNC(0x4fa500)
void AmbulanceService::ProcessPatientQueue_4FA500()
{
    u8 x, y, z;
    field_1_corpse_count -= field_4_patient_queue.RemoveNonCorpses_471290();
    if (field_1_corpse_count == 0)
    {
        return;
    }

    Ped* pPed = field_4_patient_queue.RemoveFirstPed_471320();
    if (pPed->IsNearestSpriteACar_4701D0())
    {
        field_1_corpse_count--;
        gAmbulance_110_6F70A8->TryAddPatient_4FA470(pPed);
        return;
    }

    {
        x = pPed->field_1AC_cam.x.ToInt();
        y = pPed->field_1AC_cam.y.ToInt();
        z = pPed->field_1AC_cam.z.ToInt();
        if (!gPathFinder_6FDEF0->FindNearbyTileMatchingSlopeType_5552B0(1, &x, &y, &z, 0))
        {
            field_1_corpse_count--;
            pPed->SetObjective(objectives_enum::objective_50);
            return;
        }

        for (u8 i = 0; i < 2; i++)
        {
            AmbulanceTask* pAmbulance = &field_D0_tasks[i];
            if (1 == pAmbulance->field_18_in_use && pAmbulance->field_4_paramedics_crew->IsLeaderAlive_5CBC60())
            {
                if (Fix16::MaxAbsDistance_42A6B0(pPed->get_cam_x(),
                                                 pPed->get_cam_y(),
                                                 Fix16(pAmbulance->field_0_target_x),
                                                 Fix16(pAmbulance->field_1_target_y)) < kFpEight_6F6FC0 &&
                    pAmbulance->field_14_patient_count < 10)
                {
                    pAmbulance->AddPatient_4FA800(pPed);
                    field_1_corpse_count--;
                    EmergencyCrew* pCrew = pAmbulance->field_4_paramedics_crew;
                    if (pCrew->field_28_state != crew_state::update_6)
                    {
                        if (pCrew->field_28_state == crew_state::clean_up_5)
                        {
                            pAmbulance->field_0_target_x = x;
                            pAmbulance->field_1_target_y = y;
                            pAmbulance->field_2_target_z = z;
                            pCrew->field_28_state = crew_state::update_6;
                        }
                    }
                    return;
                }
            }
        }

        AmbulanceTask* pNew = AllocateTaskSlot_4FA4B0();
        if (pNew)
        {
            pNew->field_18_in_use = 1;
            pNew->field_0_target_x = x;
            pNew->field_1_target_y = y;
            pNew->field_2_target_z = z;
            pNew->field_4_paramedics_crew = gEmergencyCrewPool_706280->AllocateSlot_5CBB80();
            if (!pNew->field_4_paramedics_crew)
            {
                field_1_corpse_count--;
                pPed->SetObjective(objectives_enum::objective_50);
                pNew->ClearTask_4FA7D0();
                return;
            }

            EmergencyCrew* pCrew = pNew->field_4_paramedics_crew;
            pCrew->field_1E_is_used = true;
            pCrew->field_20_crew_type = crew_type::paramedic_1;
            pCrew->field_24_phase = crew_phase::in_car_1;
            pCrew->field_28_state = crew_state::spawn_car_3;
            pCrew->field_18_spawn_delay = 300;
            pCrew->field_1C_spawn_wait_ticks = 0;
            pCrew->field_C_spawn_x = Fix16(x);
            pCrew->field_10_spawn_y = Fix16(y);
            pCrew->field_14_spawn_z = Fix16(z);
            pNew->AddPatient_4FA800(pPed);
        }
    }
    field_1_corpse_count--;
}

MATCH_FUNC(0x4fa790)
void AmbulanceService::AmbulancesService_4FA790()
{
    if (field_1_corpse_count > 0)
    {
        ProcessPatientQueue_4FA500();
    }

    for (s32 i = 0; i < 2; i++)
    {
        if (field_D0_tasks[i].field_18_in_use == 1)
        {
            field_D0_tasks[i].UpdateState_4FB330();
        }
    }
}

MATCH_FUNC(0x4fa7d0)
void AmbulanceTask::ClearTask_4FA7D0()
{
    field_10_patients.ClearList_420E90();
    field_0_target_x = 0;
    field_1_target_y = 0;
    field_2_target_z = 0;
    field_14_patient_count = 0;
    field_16_unused = 0;
    field_18_in_use = 0;
    field_1C_done = false;
    field_4_paramedics_crew = NULL;
    field_8_leader_patient = NULL;
    field_C_member_patient = NULL;
    field_1D_revive_ticks = 0;
}

MATCH_FUNC(0x4fa800)
void AmbulanceTask::AddPatient_4FA800(Ped* pPed)
{
    field_10_patients.AddPedToFront_471140(pPed);
    field_14_patient_count++;
}

MATCH_FUNC(0x4fa820)
bool AmbulanceTask::SpawnParamedicCrew_4FA820()
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

    Ped* pPed1 = gPedManager_6787BC->AllocatePed_470F30();
    if (!pPed1)
    {
        return false;
    }
    pPed1->SetPedType_403920(ped_type::special_ped_4);
    pPed1->set_occupation_403970(ped_ocupation_enum::paramedic_23);
    pPed1->SetJumpOverMode_433BB0(ped_jump_over_mode::can_jump_2);
    pPed1->SpawnPedInCar_45C730(field_4_paramedics_crew->field_0_car);
    pPed1->SetObjective(objectives_enum::goto_area_in_car_14, 0);
    pPed1->field_1DC_objective_target_x = this->field_0_target_x << 14;
    pPed1->field_1E0_objective_target_y = this->field_1_target_y << 14;
    pPed1->field_1E4_objective_target_z = this->field_2_target_z << 14;
    pPed1->field_28C_threat_reaction = threat_reaction_enum::react_as_emergency_1;
    pPed1->field_288_threat_search = threat_search_enum::no_threats_0;
    pPed1->set_remap_433B90(ped_remap_enum::ped_remap_medic);
    pPed1->field_26C_graphic_type = ped_graphic_type::civilian_0;
    pPed1->field_1F8_run_speed = gParamedicRunSpeed_6F6DD4;

    Ped* pPed2 = gPedManager_6787BC->AllocatePed_470F30();
    if (!pPed2)
    {
        return false;
    }

    pPed2->EnterCarAsPassenger_45C7F0(field_4_paramedics_crew->field_0_car);
    pPed2->SetPedType_403920(ped_type::special_ped_4);
    pPed2->set_occupation_403970(ped_ocupation_enum::paramedic_23);
    pPed2->SetJumpOverMode_433BB0(ped_jump_over_mode::can_jump_2);
    pPed2->SetObjective(objectives_enum::no_obj_0);
    pPed2->set_remap_433B90(ped_remap_enum::ped_remap_medic);
    pPed2->field_26C_graphic_type = ped_graphic_type::civilian_0;
    pPed2->field_28C_threat_reaction = threat_reaction_enum::react_as_emergency_1;
    pPed2->field_288_threat_search = threat_search_enum::no_threats_0;
    pGroup->add_ped_leader_4C9B10(pPed1);
    pGroup->SetCounts_433360(1);
    pGroup->add_ped_to_list_4C9B30(pPed2, 0);
    pGroup->field_0_remove_dead_members = 0;

    field_4_paramedics_crew->field_4_ped = pPed1;
    field_4_paramedics_crew->field_28_state = crew_state::update_6;
    field_4_paramedics_crew->field_0_car->SetDriverPedType_421560(ped_type::special_ped_4);
    field_4_paramedics_crew->field_0_car->SetupCarPhysicsAndSpriteBinding_43BCA0();
    field_4_paramedics_crew->field_0_car->InitCarAIControl_440590();
    field_4_paramedics_crew->field_0_car->ResumeAIDriving_43AF40();
    field_4_paramedics_crew->field_8_group = pGroup;
    return true;
}

MATCH_FUNC(0x4fa9d0)
void AmbulanceTask::EvaluatePickupState_4FA9D0()
{
    bool bPhaseFinished = false;
    if (field_4_paramedics_crew->field_24_phase == crew_phase::finished_2)
    {
        bPhaseFinished = true;
        if (!field_4_paramedics_crew->field_4_ped || field_4_paramedics_crew->field_4_ped->isDead_403B60())
        {
            field_4_paramedics_crew->field_28_state = crew_state::clean_up_5;
            field_4_paramedics_crew->field_2C_ready = false;
            gParamedicCrewPed_6F6D60 = NULL;
            return;
        }
    }
    if (bPhaseFinished)
    {
        if (field_4_paramedics_crew->field_0_car)
        {
            if (field_4_paramedics_crew->field_0_car->IsMaxDamage_40F890())
            {
                field_4_paramedics_crew->field_0_car = NULL;
                field_4_paramedics_crew->field_24_phase = crew_phase::on_foot_0;
            }
        }
        else
        {
            field_4_paramedics_crew->field_24_phase = crew_phase::on_foot_0;
        }
    }

    gParamedicCrewPed_6F6D60 = field_4_paramedics_crew->field_4_ped;
    if (!gParamedicCrewPed_6F6D60)
    {
        field_4_paramedics_crew->field_24_phase = crew_phase::finished_2;
    }
    else if (field_1C_done)
    {
        if (!field_4_paramedics_crew->field_24_phase)
        {
            field_4_paramedics_crew->field_28_state = crew_state::clean_up_5;
            field_4_paramedics_crew->field_2C_ready = false;
        }
        else if (gParamedicCrewPed_6F6D60->field_16C_car && gParamedicCrewPed_6F6D60 == gParamedicCrewPed_6F6D60->field_16C_car->field_54_driver)
        {
            if (field_4_paramedics_crew->field_8_group)
            {
                if (field_4_paramedics_crew->field_8_group->IsAllMembersInSomeCar_4CAA20())
                {
                    field_4_paramedics_crew->field_0_car->ResumeAIDriving_43AF40();
                    field_4_paramedics_crew->field_28_state = crew_state::clean_up_5;
                    field_4_paramedics_crew->field_2C_ready = false;
                    gParamedicCrewPed_6F6D60 = NULL;
                }
                else
                {
                    field_4_paramedics_crew->field_0_car->HaltAIDriving_43AF60();
                }
            }
        }
    }
}

DEFINE_GLOBAL_INIT(Fix16, kFpEight_6F6FC0, Fix16(8), 0x6F6FC0);

// Runs the paramedic crew (the leader, then each group member, in gParamedicCrewPed_6F6D60): pick up the
// patients in field_10_patients one by one, revive them, then drive off. Sets field_1C_done when nobody had
// anything left to do.
MATCH_FUNC(0x4faac0)
void AmbulanceTask::HandleObjectiveState_4FAAC0()
{
    bool bBusy = false;
    bool bParamedicVictim = false;
    u8 i = 0;
    gParamedicCrewPed_6F6D60 = field_4_paramedics_crew->field_4_ped;
    bool bNoCar = field_4_paramedics_crew->field_24_phase == crew_phase::on_foot_0;
    Ped* pTarget = field_8_leader_patient;

    while (gParamedicCrewPed_6F6D60)
    {
        if (pTarget)
        {
            if (!pTarget->field_21C_bf.bActive)
            {
                if (gParamedicCrewPed_6F6D60->GetPedState_403990() != ped_state_1::dead_9)
                {
                    gParamedicCrewPed_6F6D60->SetObjective(objectives_enum::no_obj_0);
                }
                if (pTarget == field_8_leader_patient)
                {
                    field_8_leader_patient = NULL;
                }
                else
                {
                    field_C_member_patient = NULL;
                }
            }
            else if (gParamedicCrewPed_6F6D60->field_168_game_object && pTarget->IsNearestSpriteACar_4701D0())
            {
                gAmbulance_110_6F70A8->TryAddPatient_4FA470(pTarget);
                if (gParamedicCrewPed_6F6D60->GetPedState_403990() != ped_state_1::dead_9)
                {
                    gParamedicCrewPed_6F6D60->SetObjective(objectives_enum::no_obj_0);
                }
                if (pTarget == field_8_leader_patient)
                {
                    field_8_leader_patient = NULL;
                }
                else
                {
                    field_C_member_patient = NULL;
                }
            }
        }

        Ped* pPatient;
        switch (gParamedicCrewPed_6F6D60->get_objective_403A80())
        {
            case objectives_enum::goto_area_in_car_14:
                bBusy = true;
                if (gParamedicCrewPed_6F6D60->field_16C_car)
                {
                    Car* pCar = field_4_paramedics_crew->field_0_car;
                    if (pCar->field_76_last_seen_timer > 1000)
                    {
                        bBusy = false;
                    }
                    if (gParamedicCrewPed_6F6D60->GetObjectiveStatus_450CB0() == objective_status::passed_1)
                    {
                        pCar->HaltAIDriving_43AF60();
                        field_4_paramedics_crew->field_28_state = crew_state::update_6;
                        gParamedicCrewPed_6F6D60->SetObjective(objectives_enum::no_obj_0);
                    }
                    else
                    {
                        // 9.6f calls MaxAbsDistance_42A6B0(get_cam_x(), get_cam_y(), ...)
                        if (Fix16::MaxAbsDistance_42A6B0(gParamedicCrewPed_6F6D60->get_cam_x(),
                                                            gParamedicCrewPed_6F6D60->get_cam_y(),
                                                            Fix16(field_0_target_x),
                                                            Fix16(field_1_target_y)) >= kFpEight_6F6FC0)
                        {
                            break;
                        }
                        if (!field_10_patients.GetPedsCount_4716B0())
                        {
                            bBusy = false;
                            break;
                        }
                        if (gParamedicCrewPed_6F6D60->get_objective_timer_403B30() <= 50)
                        {
                            break;
                        }
                        field_4_paramedics_crew->field_0_car->HaltAIDriving_43AF60();
                        field_4_paramedics_crew->field_28_state = crew_state::update_6;
                        gParamedicCrewPed_6F6D60->SetObjective(objectives_enum::no_obj_0);
                    }
                }
                else
                {
                    field_4_paramedics_crew->field_28_state = crew_state::update_6;
                    gParamedicCrewPed_6F6D60->SetObjective(objectives_enum::no_obj_0);
                }
                break;

            case objectives_enum::no_obj_0:
                if (gParamedicCrewPed_6F6D60->field_23C_group_idx == 99)
                {
                    pPatient = field_10_patients.RemoveFirstPed_471320();
                    if (!pPatient)
                    {
                        if (gParamedicCrewPed_6F6D60->field_16C_car)
                        {
                            gParamedicCrewPed_6F6D60->SetObjective(objectives_enum::no_obj_0);
                            gParamedicCrewPed_6F6D60->ChangeNextPedState1_45C500(ped_state_1::in_car_10);
                            gParamedicCrewPed_6F6D60->ChangeNextPedState2_45C540(ped_state_2::ped2_driving_10);
                            if (field_4_paramedics_crew->field_0_car)
                            {
                                CarAI* pAI = field_4_paramedics_crew->field_0_car->field_5C_AI;
                                if (pAI && pAI->field_28_route_idx > 0)
                                {
                                    gRouteFinder_6FFDC8->CancelRoute_589930(pAI->field_28_route_idx);
                                    field_4_paramedics_crew->field_0_car->field_5C_AI->field_28_route_idx = -1;
                                }
                                if (field_4_paramedics_crew->field_0_car->field_60_pChaseTask)
                                {
                                    gCarChaseTaskTable_678E30->FreeEntry_474CC0(field_4_paramedics_crew->field_0_car->field_60_pChaseTask);
                                    field_4_paramedics_crew->field_0_car->field_60_pChaseTask = NULL;
                                }
                                if (field_4_paramedics_crew->field_8_group->IsAllMembersInSomeCar_4CAA20())
                                {
                                    field_4_paramedics_crew->field_0_car->ResumeAIDriving_43AF40();
                                    bBusy = false;
                                }
                                else
                                {
                                    field_4_paramedics_crew->field_0_car->HaltAIDriving_43AF60();
                                }
                            }
                        }
                        else if (bNoCar || (gParamedicCrewPed_6F6D60->field_21C_bf.bLeftVehicle))
                        {
                            bBusy = false;
                        }
                        else
                        {
                            gParamedicCrewPed_6F6D60->SetObjective2_463830(objectives_enum::no_obj_0);
                            gParamedicCrewPed_6F6D60->SetObjective(objectives_enum::enter_car_as_driver_35);
                            gParamedicCrewPed_6F6D60->SetTargetObjectiveCar(field_4_paramedics_crew->field_0_car);
                            gParamedicCrewPed_6F6D60->set_enter_car_as_passenger_4039B0(0);
                            gParamedicCrewPed_6F6D60->set_target_car_door_403A70(0);
                        }
                    }
                    else if (pPatient->IsNearestSpriteACar_4701D0())
                    {
                        if (gParamedicCrewPed_6F6D60->field_16C_car)
                        {
                            gParamedicCrewPed_6F6D60->SetObjective(objectives_enum::no_obj_0);
                            gParamedicCrewPed_6F6D60->ChangeNextPedState1_45C500(ped_state_1::in_car_10);
                            gParamedicCrewPed_6F6D60->ChangeNextPedState2_45C540(ped_state_2::ped2_driving_10);
                        }
                        else if (!bNoCar && !gParamedicCrewPed_6F6D60->field_21C_bf.bLeftVehicle)
                        {
                            gParamedicCrewPed_6F6D60->SetObjective2_463830(objectives_enum::no_obj_0);
                            gParamedicCrewPed_6F6D60->SetObjective(objectives_enum::enter_car_as_driver_35);
                            gParamedicCrewPed_6F6D60->SetTargetObjectiveCar(field_4_paramedics_crew->field_0_car);
                            gParamedicCrewPed_6F6D60->set_enter_car_as_passenger_4039B0(0);
                            gParamedicCrewPed_6F6D60->set_target_car_door_403A70(0);
                        }
                        bBusy = true;
                        field_14_patient_count--;
                    }
                    else if (gParamedicCrewPed_6F6D60->field_16C_car && !gParamedicCrewPed_6F6D60->field_21C_bf.bLeftVehicle)
                    {
                        gParamedicCrewPed_6F6D60->SetObjective(objectives_enum::leave_car_36);
                        bBusy = true;
                        gParamedicCrewPed_6F6D60->SetTargetObjectiveCar(field_4_paramedics_crew->field_0_car);
                        gParamedicCrewPed_6F6D60->SetPedType_403920(ped_type::special_ped_4);
                    }
                    else
                    {
                        gParamedicCrewPed_6F6D60->SetObjective2_463830(objectives_enum::no_obj_0);
                        gParamedicCrewPed_6F6D60->SetObjective(objectives_enum::goto_char_on_foot_16);
                        bBusy = true;
                        gParamedicCrewPed_6F6D60->set_objective_target_ped_403AC0(pPatient);
                    }
                    field_8_leader_patient = pPatient;
                }
                else if (gParamedicCrewPed_6F6D60->GetInternalObjective_403A90() == objectives_enum::objective_9)
                {
                    pPatient = field_10_patients.RemoveFirstPed_471320();
                    if (pPatient)
                    {
                        gParamedicCrewPed_6F6D60->SetObjective2_463830(objectives_enum::no_obj_0);
                        gParamedicCrewPed_6F6D60->SetObjective(objectives_enum::goto_char_on_foot_16);
                        gParamedicCrewPed_6F6D60->set_objective_target_ped_403AC0(pPatient);
                        field_14_patient_count--;
                        field_4_paramedics_crew->field_8_group->field_30_panicking = 1;
                    }
                    field_C_member_patient = pPatient;
                }
                break;

            case objectives_enum::objective_28:
                if (gParamedicCrewPed_6F6D60->GetObjectiveStatus_450CB0())
                {
                    gParamedicCrewPed_6F6D60->field_278_ped_state_1 = ped_state_1::walking_0;
                    gParamedicCrewPed_6F6D60->field_27C_ped_state_2 = ped_state_2::ped2_walking_0;
                    gParamedicCrewPed_6F6D60->SetObjective(objectives_enum::no_obj_0);
                    gParamedicCrewPed_6F6D60->SetObjective2_463830(objectives_enum::no_obj_0);
                    Ped* pLeader = field_4_paramedics_crew->field_4_ped;
                    if (gParamedicCrewPed_6F6D60 != pLeader && pLeader->isDead_403B60())
                    {
                        field_4_paramedics_crew->field_8_group->PromoteMemberToLeader_4C9680(0);
                        field_4_paramedics_crew->field_4_ped = field_4_paramedics_crew->field_8_group->field_2C_ped_leader;
                        field_4_paramedics_crew->field_4_ped->SetObjective(objectives_enum::no_obj_0);
                        field_4_paramedics_crew->field_4_ped->SetObjective2_463830(objectives_enum::no_obj_0);
                    }
                    field_4_paramedics_crew->field_28_state = crew_state::update_6;
                }
                break;

            case objectives_enum::leave_car_36:
                if (gParamedicCrewPed_6F6D60->GetObjectiveStatus_450CB0())
                {
                    gParamedicCrewPed_6F6D60->SetObjective2_463830(objectives_enum::no_obj_0);
                    gParamedicCrewPed_6F6D60->SetObjective(objectives_enum::goto_char_on_foot_16);
                    gParamedicCrewPed_6F6D60->set_objective_target_ped_403AC0(field_8_leader_patient);
                }
                bBusy = true;
                break;

            case objectives_enum::goto_char_on_foot_16:
            {
                Ped* pVictim = gParamedicCrewPed_6F6D60->get_objective_target_ped_403AD0();
                if (gParamedicCrewPed_6F6D60->GetObjectiveStatus_450CB0())
                {
                    if (++field_1D_revive_ticks == 50)
                    {
                        if (pVictim->field_28C_threat_reaction == threat_reaction_enum::react_as_emergency_1)
                        {
                            bParamedicVictim = pVictim->get_occupation_403980() == ped_ocupation_enum::paramedic_23;
                        }
                        pVictim->SetObjective2_463830(objectives_enum::no_obj_0);
                        if (!bParamedicVictim)
                        {
                            pVictim->SetObjective2_463830(objectives_enum::no_obj_0);
                            pVictim->field_278_ped_state_1 = ped_state_1::walking_0;
                            pVictim->field_27C_ped_state_2 = ped_state_2::ped2_walking_0;
                            pVictim->SetObjective(objectives_enum::flee_on_foot_till_safe_1);
                            pVictim->field_1DC_objective_target_x = pVictim->get_cam_x();
                            pVictim->field_1E0_objective_target_y = pVictim->get_cam_y();
                            pVictim->field_1E4_objective_target_z = pVictim->get_cam_z();
                            pVictim->SetPedType_403920(ped_type::dummy_3);
                            pVictim->set_occupation_403970(ped_ocupation_enum::dummy);
                            pVictim->field_28C_threat_reaction = threat_reaction_enum::run_away_3;
                        }
                        else
                        {
                            pVictim->set_objective_status_403B40(1);
                        }
                        pVictim->set_health_4039A0(100);
                        pVictim->field_21C_bf.bPanicking = false;
                        if (pVictim == field_8_leader_patient)
                        {
                            field_8_leader_patient = NULL;
                        }
                        else
                        {
                            field_C_member_patient = NULL;
                        }
                        gParamedicCrewPed_6F6D60->SetObjective(objectives_enum::no_obj_0);
                        field_1D_revive_ticks = 0;
                    }
                }
                bBusy = true;
                if (field_4_paramedics_crew->field_8_group)
                {
                    field_4_paramedics_crew->field_8_group->field_1_wait_for_members = false;
                }
                break;
            }

            case objectives_enum::enter_car_as_driver_35:
                if (gParamedicCrewPed_6F6D60->field_23C_group_idx != 99)
                {
                    gParamedicCrewPed_6F6D60->SetObjective(objectives_enum::no_obj_0);
                    gParamedicCrewPed_6F6D60->SetObjective2_463830(objectives_enum::no_obj_0);
                    break;
                }
                if (gParamedicCrewPed_6F6D60->field_16C_car)
                {
                    gParamedicCrewPed_6F6D60->SetObjective(objectives_enum::no_obj_0);
                    gParamedicCrewPed_6F6D60->SetObjective2_463830(objectives_enum::no_obj_0);
                    gParamedicCrewPed_6F6D60->ChangeNextPedState1_45C500(ped_state_1::in_car_10);
                    gParamedicCrewPed_6F6D60->ChangeNextPedState2_45C540(ped_state_2::ped2_driving_10);
                }
                else
                {
                    pPatient = field_10_patients.RemoveFirstPed_471320();
                    if (!pPatient)
                    {
                        if (field_4_paramedics_crew->field_24_phase == crew_phase::in_car_1)
                        {
                            if (field_4_paramedics_crew->field_8_group)
                            {
                                field_4_paramedics_crew->field_8_group->field_1_wait_for_members = true;
                            }
                        }
                        else
                        {
                            gParamedicCrewPed_6F6D60->SetObjective(objectives_enum::no_obj_0);
                            gParamedicCrewPed_6F6D60->SetObjective2_463830(objectives_enum::no_obj_0);
                        }
                    }
                    else if (pPatient->IsNearestSpriteACar_4701D0())
                    {
                        gAmbulance_110_6F70A8->TryAddPatient_4FA470(pTarget);
                        if (field_4_paramedics_crew->field_24_phase == crew_phase::in_car_1)
                        {
                            if (field_4_paramedics_crew->field_8_group)
                            {
                                field_4_paramedics_crew->field_8_group->field_1_wait_for_members = true;
                            }
                        }
                        else
                        {
                            gParamedicCrewPed_6F6D60->SetObjective(objectives_enum::no_obj_0);
                            gParamedicCrewPed_6F6D60->SetObjective2_463830(objectives_enum::no_obj_0);
                        }
                    }
                    else
                    {
                        gParamedicCrewPed_6F6D60->SetObjective2_463830(objectives_enum::no_obj_0);
                        gParamedicCrewPed_6F6D60->SetObjective(objectives_enum::goto_char_on_foot_16);
                        gParamedicCrewPed_6F6D60->set_objective_target_ped_403AC0(pPatient);
                        field_14_patient_count--;
                        if (gParamedicCrewPed_6F6D60->field_23C_group_idx == 99)
                        {
                            field_8_leader_patient = pPatient;
                        }
                        else
                        {
                            field_C_member_patient = pPatient;
                        }
                    }
                }
                bBusy = true;
                break;
        }

        if (field_4_paramedics_crew->field_8_group)
        {
            gParamedicCrewPed_6F6D60 = field_4_paramedics_crew->field_8_group->field_4_ped_list[i];
            pTarget = field_C_member_patient;
        }
        else
        {
            gParamedicCrewPed_6F6D60 = NULL;
        }
        i++;
    }

    if (!bBusy)
    {
        field_1C_done = true;
    }
}

// https://decomp.me/scratch/cxgie
MATCH_FUNC(0x4fb330)
void AmbulanceTask::UpdateState_4FB330()
{
    bool bHandle = true;
    field_10_patients.RemoveNonCorpses_471290();
    switch (field_4_paramedics_crew->field_28_state)
    {
        case crew_state::spawn_car_3:
        {
            if (field_4_paramedics_crew->field_2C_ready)
            {
                if (!SpawnParamedicCrew_4FA820())
                {
                    if (field_4_paramedics_crew->field_0_car)
                    {
                        field_4_paramedics_crew->field_0_car->MarkForDespawn_421470();
                    }
                    field_4_paramedics_crew->ReInit_5CBC30();
                    ClearTask_4FA7D0();
                    bHandle = false;
                }
                else
                {
                    gParamedicCrewPed_6F6D60 = field_4_paramedics_crew->field_4_ped;
                    this->field_1C_done = false;
                    if (field_4_paramedics_crew->field_0_car)
                    {
                        field_4_paramedics_crew->field_0_car->ActivateEmergencyLights_43C920();
                    }
                }
            }
            else
            {
                ++field_4_paramedics_crew->field_1C_spawn_wait_ticks;
                if (field_4_paramedics_crew->field_1C_spawn_wait_ticks > 500)
                {
                    field_4_paramedics_crew->field_28_state = crew_state::clean_up_5;
                }
                bHandle = false;
            }
            break;
        }
        case crew_state::clean_up_5:
        {
            while (!field_10_patients.IsEmpty_420EA0())
            {
                Ped* pPed = field_10_patients.RemoveFirstPed_471320();
                gAmbulance_110_6F70A8->TryAddPatient_4FA470(pPed);
            }

            if (field_4_paramedics_crew->field_2C_ready)
            {
                field_4_paramedics_crew->field_28_state = crew_state::idle_0;
                field_4_paramedics_crew->ReInit_5CBC30();
                ClearTask_4FA7D0();
                bHandle = false;
            }
            break;
        }
        case crew_state::update_6:
        {
            EvaluatePickupState_4FA9D0();
            break;
        }
        default:
            FatalError_4A38C0(Gta2Error::InvalidCase, "C:\\Splitting\\Gta2\\Source\\medical.cpp", 1087);
            break;
    }
    if (bHandle)
    {
        HandleObjectiveState_4FAAC0();
    }
}