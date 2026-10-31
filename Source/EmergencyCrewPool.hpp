#pragma once

#include "Function.hpp"
#include "fix16.hpp"

class Ped;
class PedGroup;
class Car;

// EmergencyCrew::mState (stored as s32)
namespace crew_state
{
enum
{
    idle_0 = 0,
    spawn_car_3 = 3,
    clean_up_5 = 5,
    update_6 = 6,
};
} // namespace crew_state

// EmergencyCrew::mPhase (stored as s32)
namespace crew_phase
{
enum
{
    on_foot_0 = 0,
    in_car_1 = 1,
    finished_2 = 2,
};
} // namespace crew_phase

// One emergency crew (paramedics, police, SWAT, FBI or army): the car, its leader ped and the ped group,
// plus the spawn position and the state machine that drives them. Pooled in EmergencyCrewPool.
class EmergencyCrew
{
  public:
    EXPORT EmergencyCrew();
    EXPORT ~EmergencyCrew();
    EXPORT void Init();
    EXPORT void ReInit();
    EXPORT void RemovePed(Ped* pPed);
    EXPORT bool IsLeaderAlive() const;
    EXPORT bool ReplaceLeaderIfNeeded();
    EXPORT void UpdateStateMachine();
    EXPORT void CleanupExpiredEntities();
    EXPORT bool Service();
    Car* mCar; // 0x0
    Ped* mPed; // 0x4
    PedGroup* mGroup; // 0x8
    Fix16 mSpawnX; // 0xC
    Fix16 mSpawnY; // 0x10
    Fix16 mSpawnZ; // 0x14
    s16 mSpawnDelay; // 0x18: counts down to 0 before the crew is serviced, then to -80
    s16 mIdleLimit; // 0x1A: car unseen / ped idle limit before the crew is cleaned up
    s16 mSpawnWaitTicks; // 0x1C: frames spent waiting for the car to spawn (spawn_car_3); the paramedics give up after 500
    bool mIsUsed; // 0x1E
    s32 mCrewType; // 0x20: crew_type
    s32 mPhase; // 0x24: crew_phase
    s32 mState; // 0x28: crew_state
    bool mReady; // 0x2C: set once the car is spawned or the crew was cleaned up
};
GTA2_ASSERT_SIZEOF_ALWAYS(EmergencyCrew, 0x30);

class EmergencyCrewPool
{
  public:
    EmergencyCrewPool()
    {
        init();
    }
    EXPORT ~EmergencyCrewPool();
    EXPORT void init();
    EXPORT EmergencyCrew* AllocateSlot();
    EXPORT void ServiceAll();
    EmergencyCrew mSlots[10]; // 0x0
};
GTA2_ASSERT_SIZEOF_ALWAYS(EmergencyCrewPool, 0x1E0);

EXTERN_GLOBAL(EmergencyCrewPool*, gEmergencyCrewPool_706280);
