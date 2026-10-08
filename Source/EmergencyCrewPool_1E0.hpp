#pragma once

#include "Function.hpp"
#include "fix16.hpp"

class Ped;
class PedGroup;
class Car_BC;

// EmergencyCrew_30::field_28_state (stored as s32)
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

// EmergencyCrew_30::field_24_phase (stored as s32)
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
// plus the spawn position and the state machine that drives them. Pooled in EmergencyCrewPool_1E0.
class EmergencyCrew_30
{
  public:
    EXPORT EmergencyCrew_30();
    EXPORT ~EmergencyCrew_30();
    EXPORT void Init_5CBC00();
    EXPORT void ReInit_5CBC30();
    EXPORT void RemovePed_5CBC40(Ped* a2);
    EXPORT bool IsLeaderAlive_5CBC60();
    EXPORT char_type ReplaceLeaderIfNeeded_5CBC90();
    EXPORT void UpdateStateMachine_5CBD50();
    EXPORT void CleanupExpiredEntities_5CC1C0();
    EXPORT bool Service_5CC480();
    Car_BC* field_0_car;
    Ped* field_4_ped;
    PedGroup* field_8_group;
    Fix16 field_C_spawn_x;
    Fix16 field_10_spawn_y;
    Fix16 field_14_spawn_z;
    s16 field_18_spawn_delay; // counts down to 0 before the crew is serviced, then to -80
    s16 field_1A_idle_limit; // car unseen / ped idle limit before the crew is cleaned up
    s16 field_1C_unused;
    char_type field_1E_is_used;
    s32 field_20_crew_type; // crew_type
    s32 field_24_phase; // crew_phase
    s32 field_28_state; // crew_state
    char_type field_2C_ready; // set once the car is spawned or the crew was cleaned up
};

class EmergencyCrewPool_1E0
{
  public:
    EmergencyCrewPool_1E0()
    {
        init_5CBB70();
    }
    EXPORT ~EmergencyCrewPool_1E0();
    EXPORT void init_5CBB70();
    EXPORT EmergencyCrew_30* AllocateSlot_5CBB80();
    EXPORT void ServiceAll_5CBBD0();
    EmergencyCrew_30 field_0_slots[10];
};

EXTERN_GLOBAL(EmergencyCrewPool_1E0*, gEmergencyCrewPool_706280);
