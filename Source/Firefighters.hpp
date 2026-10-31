#pragma once

#include "Function.hpp"
#include "fix16.hpp"

class Car;
class Ped;

// Firefighter::mState (stored as s32)
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
        mTargetCar = NULL;
        mState = firefighter_state::idle_0;
        mbActive = 0;
        mCar = NULL;
        mPed = NULL;
        mNextStateTimer = 0;
    }

    EXPORT bool CheckAbortConditions_4A7FC0();
    EXPORT void deinit_4A81A0();
    EXPORT void Update_4A81F0();
    EXPORT void init_4A85C0();
    EXPORT void Reset_4A85E0();

    s16 mId; // 0x0
    s16 mUnused2; // 0x2
    s32 mbActive; // 0x4
    s32 mState; // 0x8: firefighter_state
    Car* mTargetCar; // 0xC
    Fix16 mXpos; // 0x10
    Fix16 mYpos; // 0x14
    Fix16 mZpos; // 0x18
    Car* mCar; // 0x1C
    Ped* mPed; // 0x20
    u16 mNextStateTimer; // 0x24
    s16 mUnused26; // 0x26
};
GTA2_ASSERT_SIZEOF_ALWAYS(Firefighter, 0x28);

class FirefighterPool
{
  public:
    FirefighterPool()
    {
        for (s32 i = 0; i < 2; i++)
        {
            mFirefighters[i].init_4A85C0();
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

    Firefighter mFirefighters[2]; // 0x0
    s16 mCount; // 0x50
    s16 mUnused52; // 0x52
};
GTA2_ASSERT_SIZEOF_ALWAYS(FirefighterPool, 0x54);

EXTERN_GLOBAL(FirefighterPool*, gFirefighterPool_54_67D4C0);
