#pragma once

#include "Function.hpp"
#include "Ped_List.hpp"

class Ped;
class EmergencyCrew;

// 25 of these sit in AmbulanceService (the same 25 as the patient queue limit in TryAddPatient_4FA470).
// init_4FA310 zeroes them and nothing else touches them: the queue itself is mPatientQueue.
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
    EXPORT void ClearTask_4FA7D0();
    EXPORT void AddPatient_4FA800(Ped* pPed);
    EXPORT bool SpawnParamedicCrew_4FA820();
    EXPORT void EvaluatePickupState_4FA9D0();
    EXPORT void HandleObjectiveState_4FAAC0();
    EXPORT void UpdateState_4FB330();

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
        init_4FA310();
    }

    EXPORT ~AmbulanceService();
    EXPORT void init_4FA310();
    EXPORT bool HandlePedDeath_4FA330(Ped* pDeadPed);
    EXPORT bool TryAddPatient_4FA470(Ped* pPed);
    EXPORT AmbulanceTask* AllocateTaskSlot_4FA4B0();
    EXPORT void ProcessPatientQueue_4FA500();
    EXPORT void AmbulancesService_4FA790();

    char_type mUnused0; // 0x0: set to 1 by init_4FA310, never read
    u8 mCorpseCount;   // 0x1: bodies waiting in mPatientQueue (an index into the slots below, once)
    Ped_List mPatientQueue; // 0x4
    PatientSlot mUnusedSlots[25]; // 0x8
    AmbulanceTask mTasks[2]; // 0xD0
};
GTA2_ASSERT_SIZEOF_ALWAYS(AmbulanceService, 0x110);

EXTERN_GLOBAL(AmbulanceService*, gAmbulance_110_6F70A8);
