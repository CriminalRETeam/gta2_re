#pragma once

#include "Function.hpp"
#include "fix16.hpp"

class Ped;
class PedGroup;
class Car;

// EmergencyCrew::field_28_state (stored as s32)
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

// EmergencyCrew::field_24_phase (stored as s32)
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
    EXPORT void Init_5CBC00();
    EXPORT void ReInit_5CBC30();
    EXPORT void RemovePed_5CBC40(Ped* pPed);
    EXPORT bool IsLeaderAlive_5CBC60() const;
    EXPORT bool ReplaceLeaderIfNeeded_5CBC90();
    EXPORT void UpdateStateMachine_5CBD50();
    EXPORT void CleanupExpiredEntities_5CC1C0();
    EXPORT bool Service_5CC480();
    Car* field_0_car; // 0x0
    Ped* field_4_ped; // 0x4
    PedGroup* field_8_group; // 0x8
    Fix16 field_C_spawn_x; // 0xC
    Fix16 field_10_spawn_y; // 0x10
    Fix16 field_14_spawn_z; // 0x14
    s16 field_18_spawn_delay; // 0x18: counts down to 0 before the crew is serviced, then to -80
    s16 field_1A_idle_limit; // 0x1A: car unseen / ped idle limit before the crew is cleaned up
    s16 field_1C_spawn_wait_ticks; // 0x1C: frames spent waiting for the car to spawn (spawn_car_3); the paramedics give up after 500
    bool field_1E_is_used; // 0x1E
    s32 field_20_crew_type; // 0x20: crew_type
    s32 field_24_phase; // 0x24: crew_phase
    s32 field_28_state; // 0x28: crew_state
    bool field_2C_ready; // 0x2C: set once the car is spawned or the crew was cleaned up
};
GTA2_ASSERT_SIZEOF_ALWAYS(EmergencyCrew, 0x30);

class EmergencyCrewPool
{
  public:
    EmergencyCrewPool()
    {
        init_5CBB70();
    }
    EXPORT ~EmergencyCrewPool();
    EXPORT void init_5CBB70();
    EXPORT EmergencyCrew* AllocateSlot_5CBB80();
    EXPORT void ServiceAll_5CBBD0();
    EmergencyCrew field_0_slots[10]; // 0x0
};
GTA2_ASSERT_SIZEOF_ALWAYS(EmergencyCrewPool, 0x1E0);

EXTERN_GLOBAL(EmergencyCrewPool*, gEmergencyCrewPool_706280);
