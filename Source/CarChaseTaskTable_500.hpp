#pragma once

#include "Function.hpp"
#include "fix16.hpp"
#include "car_chase_state.hpp"

class Ped;



// The chase task of a car whose driver is following / chasing another ped (police cars chasing a wanted player).
// The cars of a chase try to box in the target car, the chase state says what each one is doing at the moment.
// Car_BC::field_60 points at the task of the car.
class CarChaseTask_40
{
  public:
    EXPORT void ResetEntry_4747B0();
    EXPORT CarChaseTask_40();
    EXPORT ~CarChaseTask_40();

    char_type field_0_bInUse;
    Ped* field_4_pDriver;
    s32 field_8_task_type; // car_task_type
    s32 field_C_chase_state; // car_chase_state
    char_type field_10;
    Fix16 field_14_target_x;
    Fix16 field_18_target_y;
    Fix16 field_1C_target_z;
    char_type field_20_bCanSnapToTarget; // chase tasks only: may be teleported to the target when off screen (see field_34_snap_timer)
    char_type field_21_unused;
    char_type field_22_bFollowingRoute; // driving along a route (junction shortcuts), cleared when the route is cancelled
    char_type field_23_unused;
    char_type field_24_unused;
    char_type field_25_unused;
    char_type field_26_bRouteFinished;  // the route ended, Ped completes the goto_position objective
    char_type field_27_pad[3];
    s16 field_2A_settle_counter;
    u16 field_2C_side_counter;
    s16 field_2E;
    Ped* field_30_pTargetPed;
    s16 field_34_snap_timer;
    s32 field_38;
    char_type field_3C_block_counter;
};

// Fixed table of the 20 chase tasks
class CarChaseTaskTable_500
{
  public:
    enum
    {
        k_max_entries = 20
    };

    EXPORT CarChaseTask_40* AllocateEntry_474810();
    EXPORT char_type AreAllies_474850(Ped* pPed1, Ped* pPed2);
    EXPORT Ped* FindDriverInState_4748A0(s32 chase_state, Ped* pTargetPed);
    EXPORT char_type CountPursuers_474920(Ped* pTargetPed, Ped* pPed);
    EXPORT char_type HasAnyPursuer_474970(Ped* pTargetPed);
    EXPORT char_type HasPursuerInFront_4749B0(Ped* pPed);
    EXPORT char_type HasPursuerAlongsideLeft_474A20(Ped* pPed);
    EXPORT char_type HasPursuerOnLeft_474A80(Ped* pPed);
    EXPORT char_type HasPursuerAlongsideRight_474AF0(Ped* pPed);
    EXPORT char_type HasPursuerOnRight_474B50(Ped* pPed);
    EXPORT char_type HasPursuerAlongside_474BC0(Ped* pPed);
    EXPORT char_type HasPursuerChoosingSide_474C30(Ped* pPed);
    EXPORT void FreeEntry_474CC0(CarChaseTask_40* pEntry);
    EXPORT CarChaseTaskTable_500();
    EXPORT ~CarChaseTaskTable_500();

    CarChaseTask_40 field_0_entries[k_max_entries];
};

EXTERN_GLOBAL(CarChaseTaskTable_500*, gCarChaseTaskTable_678E30);
