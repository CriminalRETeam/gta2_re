#pragma once

#include "types.hpp"

// Ped::field_21C_bf. Same layout as CompilerBitField32 (the bit field form is needed to match), with the meaning of
// each bit. Bits marked "Flag" are used but their meaning is not understood yet, "Unused" bits are only reset / copied.
struct PedFlags32
{
    u32 bActive : 1; // bit 0
    u32 bUnused1 : 1; // bit 1
    u32 bPanicking : 1; // bit 2
    u32 bDriveAggressively : 1; // bit 3
    u32 bStayInCar : 1; // bit 4
    u32 bBusted : 1; // bit 5
    u32 bSkipCarSearch : 1; // bit 6
    u32 bUseCarWeapon : 1; // bit 7
    u32 bHitByAttacker : 1; // bit 8
    u32 bNoWeapon : 1; // bit 9
    u32 bScheduledForRemoval : 1; // bit 10
    u32 bAttacking : 1; // bit 11
    u32 bUnused12 : 1; // bit 12
    u32 bWeaponChosen : 1; // bit 13
    u32 bInPathList : 1; // bit 14
    u32 bFlag15 : 1; // bit 15
    u32 bFlag16 : 1; // bit 16
    u32 bFlag17 : 1; // bit 17
    u32 bUnused18 : 1; // bit 18
    u32 bFlag19 : 1; // bit 19
    u32 bUnused20 : 1; // bit 20
    u32 bUnused21 : 1; // bit 21
    u32 bFlag22 : 1; // bit 22
    u32 bSpottedPlayer : 1; // bit 23
    u32 bOnFire : 1; // bit 24
    u32 bInvisible : 1; // bit 25
    u32 bElectroFingers : 1; // bit 26
    u32 bLeftVehicle : 1; // bit 27
    u32 bArmedGangMember : 1; // bit 28
    u32 bForcedOutOfTaxi : 1; // bit 29
    u32 bUnused30 : 1; // bit 30
    u32 bUnused31 : 1; // bit 31
};

// Ped::field_224_bf
struct PedFlags8
{
    u8 bUnused0 : 1; // bit 0
    u8 bUnused1 : 1; // bit 1
    u8 bCollidedWithPed : 1; // bit 2
    u8 bUnused3 : 1; // bit 3
    u8 bWaitingOnTarget : 1; // bit 4
    u8 bDropsWeaponOnDeath : 1; // bit 5
    u8 bUnused6 : 1; // bit 6
    u8 bUnused7 : 1; // bit 7
};

// Bit indices of Ped::field_21C for BitSet32::check_bit
namespace ped_bit_index
{
enum
{
    active_0 = 0,
    unused1_1 = 1,
    panicking_2 = 2,
    drive_aggressively_3 = 3,
    stay_in_car_4 = 4,
    busted_5 = 5,
    skip_car_search_6 = 6,
    use_car_weapon_7 = 7,
    hit_by_attacker_8 = 8,
    no_weapon_9 = 9,
    scheduled_for_removal_10 = 10,
    attacking_11 = 11,
    unused12_12 = 12,
    weapon_chosen_13 = 13,
    in_path_list_14 = 14,
    flag15_15 = 15,
    flag16_16 = 16,
    flag17_17 = 17,
    unused18_18 = 18,
    flag19_19 = 19,
    unused20_20 = 20,
    unused21_21 = 21,
    flag22_22 = 22,
    spotted_player_23 = 23,
    on_fire_24 = 24,
    invisible_25 = 25,
    electro_fingers_26 = 26,
    left_vehicle_27 = 27,
    armed_gang_member_28 = 28,
    forced_out_of_taxi_29 = 29,
    unused30_30 = 30,
    unused31_31 = 31,
};
} // namespace ped_bit_index

// Masks of Ped::field_21C
namespace ped_flag_mask
{
enum
{
    k_ped_active = 0x00000001,
    k_ped_unused1 = 0x00000002,
    k_ped_panicking = 0x00000004,
    k_ped_drive_aggressively = 0x00000008,
    k_ped_stay_in_car = 0x00000010,
    k_ped_busted = 0x00000020,
    k_ped_skip_car_search = 0x00000040,
    k_ped_use_car_weapon = 0x00000080,
    k_ped_hit_by_attacker = 0x00000100,
    k_ped_no_weapon = 0x00000200,
    k_ped_scheduled_for_removal = 0x00000400,
    k_ped_attacking = 0x00000800,
    k_ped_unused12 = 0x00001000,
    k_ped_weapon_chosen = 0x00002000,
    k_ped_in_path_list = 0x00004000,
    k_ped_flag15 = 0x00008000,
    k_ped_flag16 = 0x00010000,
    k_ped_flag17 = 0x00020000,
    k_ped_unused18 = 0x00040000,
    k_ped_flag19 = 0x00080000,
    k_ped_unused20 = 0x00100000,
    k_ped_unused21 = 0x00200000,
    k_ped_flag22 = 0x00400000,
    k_ped_spotted_player = 0x00800000,
    k_ped_on_fire = 0x01000000,
    k_ped_invisible = 0x02000000,
    k_ped_electro_fingers = 0x04000000,
    k_ped_left_vehicle = 0x08000000,
    k_ped_armed_gang_member = 0x10000000,
    k_ped_forced_out_of_taxi = 0x20000000,
    k_ped_unused30 = 0x40000000,
    k_ped_unused31 = 0x80000000,
    k_ped_electro_fingers_alias = 0x04000000, // same as k_ped_electro_fingers
};
} // namespace ped_flag_mask

// Masks of Ped::field_224
namespace ped_flag8_mask
{
enum
{
    unused0 = 0x01,
    unused1 = 0x02,
    collided_with_ped = 0x04,
    unused3 = 0x08,
    waiting_on_target = 0x10,
    drops_weapon_on_death = 0x20,
};
} // namespace ped_flag8_mask
