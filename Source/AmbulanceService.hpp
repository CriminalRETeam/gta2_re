#pragma once

#include "Function.hpp"
#include "Ped_List.hpp"

class Ped;
class EmergencyCrew;

// 25 of these sit in AmbulanceService (the same 25 as the patient queue limit in TryAddPatient).
// init zeroes them and nothing else touches them: the queue itself is mPatientQueue.
class PatientSlot
{
  public:
    s32 mUnused0; // 0x0
    s32 mUnused4; // 0x4
};
GTA2_ASSERT_SIZEOF_ALWAYS(PatientSlot, 0x8);

class AmbulanceTask
{
  public:
    STATIC_MEMBER(class Ped*, sParamedicCrewPed);
    EXPORT AmbulanceTask();
    EXPORT ~AmbulanceTask();
    EXPORT void ClearTask();
    EXPORT void AddPatient(Ped* pPed);
    EXPORT bool SpawnParamedicCrew();
    EXPORT void EvaluatePickupState();
    EXPORT void HandleObjectiveState();
    EXPORT void UpdateState();

    u8 mTargetX; // 0x0: pickup tile
    u8 mTargetY; // 0x1
    u8 mTargetZ; // 0x2
    EmergencyCrew* mParamedicsCrew; // 0x4
    Ped* mLeaderPatient; // 0x8: the patient the leader is heading for or carrying
    Ped* mMemberPatient; // 0xC: same for the second paramedic
    Ped_List mPatients; // 0x10: patients still to be picked up
    u8 mPatientCount; // 0x14
    s16 mUnused16; // 0x16
    s32 mInUse; // 0x18
    bool mDone; // 0x1C: set when no paramedic had anything left to do
    char_type mReviveTicks; // 0x1D: counts to 50 while the paramedic stands at the patient
};
GTA2_ASSERT_SIZEOF_ALWAYS(AmbulanceTask, 0x20);

class AmbulanceService
{
  public:
    AmbulanceService()
    {
        init();
    }

    EXPORT ~AmbulanceService();
    EXPORT void init();
    EXPORT bool HandlePedDeath(Ped* pDeadPed);
    EXPORT bool TryAddPatient(Ped* pPed);
    EXPORT AmbulanceTask* AllocateTaskSlot();
    EXPORT void ProcessPatientQueue();
    EXPORT void AmbulancesService();

    char_type mUnused0; // 0x0: set to 1 by init, never read
    u8 mCorpseCount;   // 0x1: bodies waiting in mPatientQueue (an index into the slots below, once)
    Ped_List mPatientQueue; // 0x4
    PatientSlot mUnusedSlots[25]; // 0x8
    AmbulanceTask mTasks[2]; // 0xD0
};
GTA2_ASSERT_SIZEOF_ALWAYS(AmbulanceService, 0x110);

EXTERN_GLOBAL(AmbulanceService*, gAmbulance_110_6F70A8);
