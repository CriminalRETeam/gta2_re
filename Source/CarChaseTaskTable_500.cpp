#include "CarChaseTaskTable_500.hpp"
#include "Globals.hpp"
#include "Ped.hpp"

DEFINE_GLOBAL(CarChaseTaskTable_500*, gCarChaseTaskTable_678E30, 0x678E30);

DEFINE_GLOBAL_INIT(Fix16, kFpZero_678D0C, Fix16(0), 0x678D0C);

MATCH_FUNC(0x4747b0)
void CarChaseTask_40::ResetEntry_4747B0()
{
    field_0_bInUse = 0;
    field_10_bRamTarget = 0;
    field_8_task_type = 0;
    field_14_target_x = kFpZero_678D0C;
    field_18_target_y = kFpZero_678D0C;
    field_1C_target_z = kFpZero_678D0C;
    field_20_bCanSnapToTarget = 0;
    field_21_unused = 0;
    field_22_bFollowingRoute = 1;
    field_23_unused = 1;
    field_24_unused = 0;
    field_25_unused = 0;
    field_30_pTargetPed = 0;
    field_26_bRouteFinished = 0;
    field_2A_settle_counter = 0;
    field_2C_side_counter = 0;
    field_2E_wait_counter = 0;
    field_C_chase_state = car_chase_state::follow_behind_0;
    field_34_snap_timer = 0;
    field_4_pDriver = 0;
    field_38_unused = 0;
    field_3C_block_counter = 0;
}

MATCH_FUNC(0x474810)
CarChaseTask_40* CarChaseTaskTable_500::AllocateEntry_474810()
{
    for (u8 i = 0; i < k_max_entries; i++)
    {
        if (!field_0_entries[i].field_0_bInUse)
        {
            field_0_entries[i].field_0_bInUse = 1;
            return &field_0_entries[i];
        }
    }
    return 0;
}

MATCH_FUNC(0x474850)
char_type CarChaseTaskTable_500::AreAllies_474850(Ped* pPed1, Ped* pPed2)
{
    if (pPed1->get_occupation_403980() < ped_ocupation_enum::police || pPed1->get_occupation_403980() > ped_ocupation_enum::army_army) // ped 1 is not police
    {
        if (pPed2->field_17C_pGang == pPed1->field_17C_pGang) // they are from same gang (or both dont have any)
        {
            return 1;
        }
    }
    else
    {
        if (pPed2->get_occupation_403980() >= ped_ocupation_enum::police && pPed2->field_240_occupation <= ped_ocupation_enum::army_army) // both ped 1 and ped 2 are police feds
        {
            return 1;
        }
    }
    return 0;
}

MATCH_FUNC(0x4748a0)
Ped* CarChaseTaskTable_500::FindDriverInState_4748A0(s32 chase_state, Ped* pTargetPed)
{
    for (u8 i = 0; i < k_max_entries; i++)
    {
        if (field_0_entries[i].field_0_bInUse == 1 && field_0_entries[i].field_30_pTargetPed == pTargetPed && AreAllies_474850(pTargetPed, field_0_entries[i].field_4_pDriver) &&
            chase_state == field_0_entries[i].field_C_chase_state)
        {
            return field_0_entries[i].field_4_pDriver;
        }
    }
    return 0;
}

MATCH_FUNC(0x474920)
char_type CarChaseTaskTable_500::CountPursuers_474920(Ped* pTargetPed, Ped* pPed)
{
    u8 total = 0;
    for (u8 i = 0; i < k_max_entries; i++)
    {
        if (field_0_entries[i].field_0_bInUse == 1 && field_0_entries[i].field_30_pTargetPed == pTargetPed)
        {
            if (AreAllies_474850(pPed, field_0_entries[i].field_4_pDriver))
            {
                total++;
            }
        }
    }
    return total;
}

MATCH_FUNC(0x474970)
char_type CarChaseTaskTable_500::HasAnyPursuer_474970(Ped* pTargetPed)
{
    for (u8 i = 0; i < k_max_entries; i++)
    {
        if (field_0_entries[i].field_0_bInUse == 1 && field_0_entries[i].field_30_pTargetPed == pTargetPed)
        {
            return 1;
        }
    }
    return 0;
}

MATCH_FUNC(0x4749b0)
char_type CarChaseTaskTable_500::HasPursuerInFront_4749B0(Ped* pPed)
{
    for (u8 i = 0; i < k_max_entries; i++)
    {
        if (field_0_entries[i].field_0_bInUse == 1)
        {
            if (AreAllies_474850(pPed, field_0_entries[i].field_4_pDriver))
            {
                switch (field_0_entries[i].field_C_chase_state)
                {
                    case car_chase_state::in_front_stopping_13:
                    case car_chase_state::target_stopped_15:
                        return 1;
                }
            }
        }
    }
    return 0;
}

MATCH_FUNC(0x474a20)
char_type CarChaseTaskTable_500::HasPursuerAlongsideLeft_474A20(Ped* pPed)
{
    for (u8 i = 0; i < k_max_entries; i++)
    {
        if (field_0_entries[i].field_0_bInUse == 1)
        {
            if (AreAllies_474850(pPed, field_0_entries[i].field_4_pDriver) && field_0_entries[i].field_C_chase_state == car_chase_state::alongside_left_8)
            {
                return 1;
            }
        }
    }
    return 0;
}

MATCH_FUNC(0x474a80)
char_type CarChaseTaskTable_500::HasPursuerOnLeft_474A80(Ped* pPed)
{
    for (u8 i = 0; i < k_max_entries; i++)
    {
        if (field_0_entries[i].field_0_bInUse == 1)
        {
            if (AreAllies_474850(pPed, field_0_entries[i].field_4_pDriver))
            {
                switch (field_0_entries[i].field_C_chase_state)
                {
                    case car_chase_state::approach_left_6:
                    case car_chase_state::alongside_left_8:
                    case car_chase_state::pull_ahead_left_10:
                        return 1;
                }
            }
        }
    }
    return 0;
}

MATCH_FUNC(0x474af0)
char_type CarChaseTaskTable_500::HasPursuerAlongsideRight_474AF0(Ped* pPed)
{
    for (u8 i = 0; i < k_max_entries; i++)
    {
        if (field_0_entries[i].field_0_bInUse == 1)
        {
            if (AreAllies_474850(pPed, field_0_entries[i].field_4_pDriver) && field_0_entries[i].field_C_chase_state == car_chase_state::alongside_right_9)
            {
                return 1;
            }
        }
    }
    return 0;
}

MATCH_FUNC(0x474b50)
char_type CarChaseTaskTable_500::HasPursuerOnRight_474B50(Ped* pPed)
{
    for (u8 i = 0; i < k_max_entries; i++)
    {
        if (field_0_entries[i].field_0_bInUse == 1)
        {
            if (AreAllies_474850(pPed, field_0_entries[i].field_4_pDriver))
            {
                switch (field_0_entries[i].field_C_chase_state)
                {
                    case car_chase_state::approach_right_7:
                        return 1;
                    case car_chase_state::alongside_right_9:
                    case car_chase_state::pull_ahead_right_11:
                        return 1;
                }
            }
        }
    }
    return 0;
}

MATCH_FUNC(0x474bc0)
char_type CarChaseTaskTable_500::HasPursuerAlongside_474BC0(Ped* pPed)
{
    for (u8 i = 0; i < k_max_entries; i++)
    {
        if (field_0_entries[i].field_0_bInUse == 1)
        {
            if (AreAllies_474850(pPed, field_0_entries[i].field_4_pDriver))
            {
                if (field_0_entries[i].field_C_chase_state >= car_chase_state::approach_left_6 && (field_0_entries[i].field_C_chase_state <= car_chase_state::alongside_right_9 || field_0_entries[i].field_C_chase_state == car_chase_state::in_front_stopping_13))
                {
                    return 1;
                }
            }
        }
    }
    return 0;
}

MATCH_FUNC(0x474c30)
char_type CarChaseTaskTable_500::HasPursuerChoosingSide_474C30(Ped* pPed)
{
    for (u8 i = 0; i < k_max_entries; i++)
    {
        if (field_0_entries[i].field_0_bInUse == 1)
        {
            if (AreAllies_474850(pPed, field_0_entries[i].field_4_pDriver))
            {
                if (field_0_entries[i].field_C_chase_state < car_chase_state::move_right_4)
                {
                    continue;
                }

                if (field_0_entries[i].field_C_chase_state <= car_chase_state::choose_side_5)
                {
                    return 1;
                }
            }
        }
    }
    return 0;
}

MATCH_FUNC(0x474ca0)
CarChaseTask_40::CarChaseTask_40()
{
    ResetEntry_4747B0();
}

MATCH_FUNC(0x474cb0)
CarChaseTask_40::~CarChaseTask_40()
{
}

MATCH_FUNC(0x474cc0)
void CarChaseTaskTable_500::FreeEntry_474CC0(CarChaseTask_40* toFind)
{
    for (u8 i = 0; i < k_max_entries; i++)
    {
        if (&field_0_entries[i] == toFind)
        {
            field_0_entries[i].field_0_bInUse = 0;
            field_0_entries[i].ResetEntry_4747B0();
            return;
        }
    }
}

MATCH_FUNC(0x474d10)
CarChaseTaskTable_500::CarChaseTaskTable_500()
{
}

MATCH_FUNC(0x474d30)
CarChaseTaskTable_500::~CarChaseTaskTable_500()
{
}