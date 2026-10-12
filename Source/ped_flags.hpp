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

// Bit indices of PedFlags32. Unused now, but removing it changes MapRenderer::Draw3SidedDiagonalDownRight_4EF520
// and Draw4SidedDiagonalUpLeft_4EF880 (the name count of the TU, see docs/matching_quirks.md)
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
