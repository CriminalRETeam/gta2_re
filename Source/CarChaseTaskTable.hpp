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
    EXPORT void ResetEntry_4747B0();
    EXPORT CarChaseTask();
    EXPORT ~CarChaseTask();

    char_type field_0_in_use;
    Ped* field_4_pDriver;
    s32 field_8_task_type; // car_task_type
    s32 field_C_chase_state; // car_chase_state
    char_type field_10_ram_target; // set when overtaking: touching the target car then damages it (CarAI)
    Fix16 field_14_target_x;
    Fix16 field_18_target_y;
    Fix16 field_1C_target_z;
    char_type field_20_can_snap_to_target; // chase tasks only: may be teleported to the target when off screen (see field_34_snap_timer)
    char_type field_21_unused;
    char_type field_22_following_route; // driving along a route (junction shortcuts), cleared when the route is cancelled
    char_type field_23_unused;
    char_type field_24_unused;
    char_type field_25_unused;
    char_type field_26_route_finished;  // the route ended, Ped completes the goto_position objective
    char_type field_27_pad[3];
    s16 field_2A_settle_counter;
    u16 field_2C_side_counter;
    s16 field_2E_wait_counter; // ticks spent waiting next to the target before pulling ahead
    Ped* field_30_pTargetPed;
    s16 field_34_snap_timer;
    s32 field_38_unused;
    char_type field_3C_block_counter;
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

    EXPORT CarChaseTask* AllocateEntry_474810();
    EXPORT bool AreAllies_474850(Ped* pPed1, Ped* pPed2);
    EXPORT Ped* FindDriverInState_4748A0(s32 chase_state, Ped* pTargetPed);
    EXPORT u8 CountPursuers_474920(Ped* pTargetPed, Ped* pPed);
    EXPORT bool HasAnyPursuer_474970(Ped* pTargetPed);
    EXPORT bool HasPursuerInFront_4749B0(Ped* pPed);
    EXPORT bool HasPursuerAlongsideLeft_474A20(Ped* pPed);
    EXPORT bool HasPursuerOnLeft_474A80(Ped* pPed);
    EXPORT bool HasPursuerAlongsideRight_474AF0(Ped* pPed);
    EXPORT bool HasPursuerOnRight_474B50(Ped* pPed);
    EXPORT bool HasPursuerAlongside_474BC0(Ped* pPed);
    EXPORT bool HasPursuerChoosingSide_474C30(Ped* pPed);
    EXPORT void FreeEntry_474CC0(CarChaseTask* pEntry);
    EXPORT CarChaseTaskTable();
    EXPORT ~CarChaseTaskTable();

    CarChaseTask field_0_entries[k_max_entries];
};
GTA2_ASSERT_SIZEOF_ALWAYS(CarChaseTaskTable, 0x500);

EXTERN_GLOBAL(CarChaseTaskTable*, gCarChaseTaskTable_678E30);
