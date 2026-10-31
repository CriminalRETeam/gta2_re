#pragma once

#include "Function.hpp"
#include "fix16.hpp"

class Car;
class Ped;

// Firefighter::field_8_state (stored as s32)
namespace firefighter_state
{
enum
{
    idle_0 = 0,
    spawn_truck_1 = 1, // waiting for a fire truck to spawn at the dispatch point
    drive_to_fire_2 = 2,
    arrived_3 = 3,
    put_out_fire_4 = 4,
    finished_5 = 5, // truck goes back to normal AI, slot is reset
    abort_6 = 6, // truck lost, despawned or the target is gone: deinit
};
} // namespace firefighter_state

class Firefighter
{
  public:
    // inline
    void Clear_450C10()
    {
        field_C_target_car = NULL;
        field_8_state = firefighter_state::idle_0;
        field_4_bActive = 0;
        field_1C_car = NULL;
        field_20_ped = NULL;
        field_24_next_state_timer = 0;
    }

    EXPORT bool CheckAbortConditions_4A7FC0();
    EXPORT void deinit_4A81A0();
    EXPORT void Update_4A81F0();
    EXPORT void init_4A85C0();
    EXPORT void Reset_4A85E0();

    s16 field_0_id; // 0x0
    s16 field_2_unused; // 0x2
    s32 field_4_bActive; // 0x4
    s32 field_8_state; // 0x8: firefighter_state
    Car* field_C_target_car; // 0xC
    Fix16 field_10_xpos; // 0x10
    Fix16 field_14_ypos; // 0x14
    Fix16 field_18_zpos; // 0x18
    Car* field_1C_car; // 0x1C
    Ped* field_20_ped; // 0x20
    u16 field_24_next_state_timer; // 0x24
    s16 field_26_unused; // 0x26
};
GTA2_ASSERT_SIZEOF_ALWAYS(Firefighter, 0x28);

class FirefighterPool
{
  public:
    FirefighterPool()
    {
        for (s32 i = 0; i < 2; i++)
        {
            field_0_firefighters[i].init_4A85C0();
        }
        ResetCount_4A88D0();
    }

    ~FirefighterPool()
    {
    }

    EXPORT void FireEnginesService_4A85F0();
    EXPORT Firefighter* DispatchFirefighters_4A8620(Car* pCar, Fix16 x, Fix16 y, Fix16 z);
    EXPORT Firefighter* New28_4A8800();
    EXPORT bool TryDispatchFirefightersToCar_4A8820(Car* pCar);
    EXPORT bool IsFireEngineSentToCar_4A8890(Car* pFireEngine, Car* pTargetCar) const;
    EXPORT void ResetCount_4A88D0();

    Firefighter field_0_firefighters[2]; // 0x0
    s16 field_50_count; // 0x50
    s16 field_52_unused; // 0x52
};
GTA2_ASSERT_SIZEOF_ALWAYS(FirefighterPool, 0x54);

EXTERN_GLOBAL(FirefighterPool*, gFirefighterPool_54_67D4C0);
