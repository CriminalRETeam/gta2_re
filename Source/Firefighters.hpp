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
    // 9.6f 0x450C10
    void Clear_450C10()
    {
        mTargetCar = NULL;
        mState = firefighter_state::idle_0;
        mbActive = 0;
        mCar = NULL;
        mPed = NULL;
        mNextStateTimer = 0;
    }

    EXPORT bool CheckAbortConditions();
    EXPORT void deinit();
    EXPORT void Update();
    EXPORT void init();
    EXPORT void Reset();

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
            mFirefighters[i].init();
        }
        ResetCount();
    }

    ~FirefighterPool()
    {
    }

    EXPORT void FireEnginesService();
    EXPORT Firefighter* DispatchFirefighters(Car* pCar, Fix16 x, Fix16 y, Fix16 z);
    EXPORT Firefighter* New28();
    EXPORT bool TryDispatchFirefightersToCar(Car* pCar);
    EXPORT bool IsFireEngineSentToCar(Car* pFireEngine, Car* pTargetCar) const;
    EXPORT void ResetCount();

    Firefighter mFirefighters[2]; // 0x0
    s16 mCount; // 0x50
    s16 mUnused52; // 0x52
};
GTA2_ASSERT_SIZEOF_ALWAYS(FirefighterPool, 0x54);

EXTERN_GLOBAL(FirefighterPool*, gFirefighterPool_54);
