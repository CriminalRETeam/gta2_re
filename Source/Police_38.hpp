#pragma once

#include "Function.hpp"
#include "fix16.hpp"

class EmergencyCrew;
class Ped;
class Car;
class Object;
class PolicePursuitTarget;
class Fix16_Rect;

namespace police_crew_state
{
enum
{
    none_0 = 0,
    patrol_1 = 1,
    despawning_2 = 2, // PoliceCrew::TryDespawnOffscreenCrew_571350, back to shutdown_6 next update
    alerted_search_3 = 3,
    pursue_or_chase_5 = 5,
    shutdown_6 = 6,
};
} // namespace police_crew_state

// PolicePursuitTarget::field_8_state, see Police::UpdatePursuitTargets_56FBD0
namespace pursuit_state
{
enum
{
    idle_0 = 0,       // criminal registered, no crew sent yet
    responding_1 = 1, // first crew sent, waiting until the criminal is spotted
    active_3 = 3,     // crews engaged, more are sent or sent away as the wanted level changes
    ended_4 = 4,      // wanted level gone or criminal dead: release the crews and clear the target
    searching_5 = 5,  // criminal not seen for a while: crews search, a new one is sent if none is
};
} // namespace pursuit_state

class PoliceCrew
{
  public:
    STATIC_MEMBER(Ped*, sCurrentCrewPed);
    EXPORT PoliceCrew();
    EXPORT ~PoliceCrew();
    EXPORT void Init_5709C0();
    EXPORT void AddToPursuit_570A10();
    EXPORT void RemoveFromPursuit_570AB0();
    EXPORT void SpawnPoliceInCar_570BF0();
    EXPORT void SpawnSWAT_570E30();
    EXPORT void SpawnFBI_nonused_571150();
    EXPORT void TryDespawnOffscreenCrew_571350();
    EXPORT void TryDespawnFinishedCrew_571540();
    EXPORT void TryDespawnInCarCrew_571A30();
    EXPORT void HandleShutdownAndDespawn_5720C0();
    EXPORT bool CanTrackCriminal_572210() const;
    EXPORT void State3_AlertedSearch_572340();
    EXPORT void State5_PursueOrChase_572920();
    EXPORT void State6_ShutDown_574720();
    EXPORT void State1_Patrol_574F10();
    EXPORT void State2_SwitchToShutDown_575200();
    EXPORT void HandleEnterCarObjective_575210();
    EXPORT void HandleLeaveCarObjective_575270();
    EXPORT void HandleGotoAreaInCarObjective_5752C0();
    EXPORT void HandleChaseInCarObjective_575310();
    EXPORT void Service_575590();
    EXPORT void UnlinkFromPursuitTarget_575650();

    u16 field_0_id; // 0x0
    u8 field_2_targ_x; // 0x2
    u8 field_3_targ_y; // 0x3
    u8 field_4_targ_z; // 0x4
    char_type field_5_unused; // 0x5
    char_type field_6_unused; // 0x6
    char_type field_7_unused; // 0x7
    Fix16 field_8_criminal_distance; // 0x8
    Fix16 field_C_unused; // 0xC
    EmergencyCrew* field_10_subObj; // 0x10
    PolicePursuitTarget* field_14_pPursuitTarget; // 0x14: Pursuit of one wanted criminal by police crews
    s16 field_18_unused; // 0x18
    char_type field_1A_unused; // 0x1A
    char_type field_1B_unused; // 0x1B
    s32 field_1C_used; // 0x1C
    s32 field_20_crew_kind; // 0x20: crew_kind
    s32 field_24_state; // 0x24
    bool field_28_use_car; // 0x28
    bool field_29_bCountedInPoliceCount; // 0x29
    s16 field_2A_unused; // 0x2A
    s16 field_2C_unused; // 0x2C
    char_type field_2E_unused; // 0x2E
    char_type field_2F_unused; // 0x2F
    Ped* field_30_prev_target_ped; // 0x30
    bool field_34_is_dismissed; // 0x34
    u8 field_35_fast_target_counter; // 0x35
    char_type field_36_unused; // 0x36
    char_type field_37_unused; // 0x37
};
GTA2_ASSERT_SIZEOF_ALWAYS(PoliceCrew, 0x38);

class PolicePursuitTarget  // Pursuit of one wanted criminal by police crews
{
  public:
    Ped* field_0_criminal_ped; // 0x0
    s32 field_4_wanted_level; // 0x4
    s32 field_8_state; // 0x8: pursuit_state
    u16 field_C_timer; // 0xC
    char_type field_E_block_counter; // 0xE
    char_type field_F_unused; // 0xF
    Fix16 field_10_x; // 0x10
    Fix16 field_14_y; // 0x14
    Fix16 field_18_z; // 0x18
    bool field_1C_crew_gave_up; // 0x1C
    char_type field_1D_unused; // 0x1D
    char_type field_1E_unused; // 0x1E
    char_type field_1F_unused; // 0x1F
    PoliceCrew* field_20_crews[6]; // 0x20
    s32 field_38_unused; // 0x38
    s32 field_3C_unused; // 0x3C
    s32 field_40_unused; // 0x40
    s32 field_44_unused; // 0x44
    s32 field_48_unused; // 0x48
    s32 field_4C_unused; // 0x4C
    s32 field_50_unused; // 0x50
    s32 field_54_unused; // 0x54
    s32 field_58_barrier_1_id; // 0x58
    s32 field_5C_barrier_2_id; // 0x5C
    s32 field_60_barrier_3_id; // 0x60
    s32 field_64_barrier_4_id; // 0x64
    s32 field_68_barrier_5_id; // 0x68
    s32 field_6C_barrier_6_id; // 0x6C
    u8 field_70_num_police_crews; // 0x70
    u8 field_71_target_police_crews; // 0x71
    u8 field_72_num_swat_crews; // 0x72
    u8 field_73_num_fbi_crews; // 0x73
    u8 field_74_num_army_crews; // 0x74
    u8 field_75_num_crews; // 0x75
    s16 field_76_unused; // 0x76
    bool field_78_is_actively_chased; // 0x78
    char_type field_79_unused; // 0x79
    s16 field_7A_wanted_timer; // 0x7A
};
GTA2_ASSERT_SIZEOF_ALWAYS(PolicePursuitTarget, 0x7C);

class PoliceRoadblock
{
  public:
    EXPORT void Reset_575710();
    EXPORT void Update_5757B0();
    EXPORT void RemoveRoadblock_575CA0();
    EXPORT bool CreateRoadblock_575FF0(u8 x, u8 y, u8 z, s32 orientation);

    EXPORT PoliceRoadblock();
    EXPORT ~PoliceRoadblock();
    bool field_0_bActive; // 0x0
    char_type field_1_unused; // 0x1
    char_type field_2_unused; // 0x2
    char_type field_3_unused; // 0x3
    s32 field_4_unused; // 0x4
    u8 field_8_x; // 0x8
    u8 field_9_y; // 0x9
    char_type field_A_unused; // 0xA
    char_type field_B_unused; // 0xB
    u16 field_C_startup_timer; // 0xC
    s16 field_E_unseen_timer; // 0xE
    Car* field_10_car_1; // 0x10
    Car* field_14_car_2; // 0x14
    Car* field_18_car_3; // 0x18
    Car* field_1C_car_4; // 0x1C
    Car* field_20_car_5; // 0x20
    Car* field_24_car_6; // 0x24
    Object* field_28_barrier_1; // 0x28
    Object* field_2C_barrier_2; // 0x2C
    Object* field_30_barrier_3; // 0x30
    Object* field_34_barrier_4; // 0x34
    Object* field_38_barrier_5; // 0x38
    Object* field_3C_barrier_6; // 0x3C
    Object* field_40_barrier_7; // 0x40
    Object* field_44_barrier_8; // 0x44
    Object* field_48_barrier_9; // 0x48
    Object* field_4C_barrier_10; // 0x4C
    Object* field_50_barrier_11; // 0x50
    Object* field_54_barrier_12; // 0x54
    s32 field_58_barrier_1_id; // 0x58
    s32 field_5C_barrier_2_id; // 0x5C
    s32 field_60_barrier_3_id; // 0x60
    s32 field_64_barrier_4_id; // 0x64
    s32 field_68_barrier_5_id; // 0x68
    s32 field_6C_barrier_6_id; // 0x6C
    s32 field_70_barrier_7_id; // 0x70
    s32 field_74_barrier_8_id; // 0x74
    s32 field_78_barrier_9_id; // 0x78
    s32 field_7C_barrier_10_id; // 0x7C
    s32 field_80_barrier_11_id; // 0x80
    s32 field_84_barrier_12_id; // 0x84
    Ped* field_88_guard_1; // 0x88
    Ped* field_8C_guard_2; // 0x8C
    Ped* field_90_guard_3; // 0x90
    Ped* field_94_guard_4; // 0x94
    Ped* field_98_guard_5; // 0x98
    Ped* field_9C_guard_6; // 0x9C
    Fix16_Rect* field_A0_rect; // 0xA0
};
GTA2_ASSERT_SIZEOF_ALWAYS(PoliceRoadblock, 0xA4);