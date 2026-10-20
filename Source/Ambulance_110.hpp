#pragma once

#include "Function.hpp"
#include "Ped_List_4.hpp"

class Ped;
class EmergencyCrew;

// 25 of these sit in AmbulanceService (the same 25 as the patient queue limit in TryAddPatient_4FA470).
// init_4FA310 zeroes them and nothing else touches them: the queue is a Ped_List now.
class PatientSlot
{
  public:
    s32 field_0;
    s32 field_4_unused;
};
GTA2_ASSERT_SIZEOF_ALWAYS(PatientSlot, 0x8);

class AmbulanceTask
{
  public:
    EXPORT AmbulanceTask();
    EXPORT ~AmbulanceTask();
    EXPORT void ClearTask_4FA7D0();
    EXPORT void AddPassenger_4FA800(Ped* pPed);
    EXPORT bool SpawnParamedicCrew_4FA820();
    EXPORT void EvaluatePickupState_4FA9D0();
    EXPORT void HandleObjectiveState_4FAAC0();
    EXPORT void UpdateState_4FB330();

    u8 field_0_target_x; // pickup tile
    u8 field_1_target_y;
    u8 field_2_target_z;
    char_type field_3_pad;
    EmergencyCrew* field_4_paramedics_crew;
    Ped* field_8_leader_patient; // the patient the leader is heading for or carrying
    Ped* field_C_member_patient; // same for the second paramedic
    Ped_List field_10_patients; // patients still to be picked up
    u8 field_14_patient_count;
    u8 field_15_pad;
    s16 field_16_unused;
    s32 field_18_in_use;
    char_type field_1C_done; // set when no paramedic had anything left to do
    char_type field_1D_revive_ticks; // counts to 50 while the paramedic stands at the patient
    char_type field_1E_pad;
    char_type field_1F_pad;
};
GTA2_ASSERT_SIZEOF_ALWAYS(AmbulanceTask, 0x20);

class AmbulanceService
{
  public:
    AmbulanceService()
    {
        init_4FA310();
    }

    EXPORT ~AmbulanceService();
    EXPORT void init_4FA310();
    EXPORT bool HandlePedDeath_4FA330(Ped* pDeadPed);
    EXPORT bool TryAddPatient_4FA470(Ped* pPed);
    EXPORT AmbulanceTask* AllocateTaskSlot_4FA4B0();
    EXPORT void ProcessPatientQueue_4FA500();
    EXPORT void AmbulancesService_4FA790();

    char_type field_0; // set to 1 by init_4FA310, never read
    u8 field_1_corpse_count;   // bodies waiting in field_4_patient_queue (an index into the slots below, once)
    char_type field_2_pad;
    char_type field_3_pad;
    Ped_List field_4_patient_queue;
    PatientSlot field_8_unused_slots[25];
    AmbulanceTask field_D0_tasks[2];
};
GTA2_ASSERT_SIZEOF_ALWAYS(AmbulanceService, 0x110);

EXTERN_GLOBAL(AmbulanceService*, gAmbulance_110_6F70A8);
