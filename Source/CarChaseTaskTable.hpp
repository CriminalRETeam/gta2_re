#pragma once

#include "Function.hpp"
#include "fix16.hpp"
#include "car_chase_state.hpp"

class Ped;



// The chase task of a car whose driver is following / chasing another ped (police cars chasing a wanted player).
// The cars of a chase try to box in the target car, the chase state says what each one is doing at the moment.
// Car::field_60 points at the task of the car.
class CarChaseTask
{
  public:
    EXPORT void ResetEntry();
    EXPORT CarChaseTask();
    EXPORT ~CarChaseTask();

    char_type mInUse; // 0x0
    Ped* mpDriver; // 0x4
    s32 mTaskType; // 0x8: car_task_type
    s32 mChaseState; // 0xC: car_chase_state
    bool mRamTarget; // 0x10: set when overtaking: touching the target car then damages it (CarAI)
    Fix16 mTargetX; // 0x14
    Fix16 mTargetY; // 0x18
    Fix16 mTargetZ; // 0x1C
    bool mCanSnapToTarget; // 0x20: chase tasks only: may be teleported to the target when off screen (see mSnapTimer)
    char_type mUnused21; // 0x21
    bool mFollowingRoute; // 0x22: driving along a route (junction shortcuts), cleared when the route is cancelled
    char_type mUnused23; // 0x23
    char_type mUnused24; // 0x24
    char_type mUnused25; // 0x25
    bool mRouteFinished;  // 0x26: the route ended, Ped completes the goto_position objective
    char_type mPad27[3]; // 0x27
    s16 mSettleCounter; // 0x2A
    u16 mSideCounter; // 0x2C
    s16 mWaitCounter; // 0x2E: ticks spent waiting next to the target before pulling ahead
    Ped* mpTargetPed; // 0x30
    s16 mSnapTimer; // 0x34
    s32 mUnused38; // 0x38
    char_type mBlockCounter; // 0x3C
};
GTA2_ASSERT_SIZEOF_ALWAYS(CarChaseTask, 0x40);

// Fixed table of the 20 chase tasks
class CarChaseTaskTable
{
  public:
    enum
    {
        k_max_entries = 20
    };

    EXPORT CarChaseTask* AllocateEntry();
    EXPORT bool AreAllies(Ped* pPed1, Ped* pPed2) const;
    EXPORT Ped* FindDriverInState(s32 chase_state, Ped* pTargetPed) const;
    EXPORT u8 CountPursuers(Ped* pTargetPed, Ped* pPed) const;
    EXPORT bool HasAnyPursuer(Ped* pTargetPed) const;
    EXPORT bool HasPursuerInFront(Ped* pPed) const;
    EXPORT bool HasPursuerAlongsideLeft(Ped* pPed) const;
    EXPORT bool HasPursuerOnLeft(Ped* pPed) const;
    EXPORT bool HasPursuerAlongsideRight(Ped* pPed) const;
    EXPORT bool HasPursuerOnRight(Ped* pPed) const;
    EXPORT bool HasPursuerAlongside(Ped* pPed) const;
    EXPORT bool HasPursuerChoosingSide(Ped* pPed) const;
    EXPORT void FreeEntry(CarChaseTask* pEntry);
    EXPORT CarChaseTaskTable();
    EXPORT ~CarChaseTaskTable();

    CarChaseTask mEntries[k_max_entries]; // 0x0
};
GTA2_ASSERT_SIZEOF_ALWAYS(CarChaseTaskTable, 0x500);

EXTERN_GLOBAL(CarChaseTaskTable*, gCarChaseTaskTable);
