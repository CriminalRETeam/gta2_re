#pragma once

// Explosion_30::field_10_type: what kind of explosion/fire effect this is. Created by Object_5C::CreateExplosion_52A3D0
// (a6), the script explosion commands pass small_18 / item_19 / large_20 / no_ring_32 and the building ones 22-25. Kept in
// its own header, like car_despawn_status.hpp, so that the enum does not land in a header many TUs share.
namespace explosion_type
{
enum
{
    none_0 = 0,
    unknown_1 = 1,
    unknown_2 = 2,
    fire_3 = 3, // 3, 4, 5, 12, 13, 14 are fires: a moving particle trail from the object's speed/angle (IsFireType_5435D0)
    car_fire_4 = 4, // Car_BC::SpawnFire_43BBC0, a burning car
    molotov_fire_5 = 5, // the flames of a thrown molotov (Weapon_30), also left by explosions
    car_fire_level1_12 = 12, // Car_BC::GetFireExplosionType_43BB90, fire level 1 to 3
    car_fire_level2_13 = 13,
    car_fire_level3_14 = 14,
    unknown_15 = 15,
    unknown_16 = 16,
    unknown_17 = 17,
    small_18 = 18, // SCRCMD_EXPLODE_SMALL1, also grenades, mines and rockets
    item_19 = 19,  // SCRCMD_EXPLODE_ITEM, also most car explosions
    large_20 = 20, // SCRCMD_EXPLODE_LARGE1
    unknown_21 = 21,
    building_45_22 = 22,  // SCRCMD_EXPLODE_BUILDING 2: debris towards 45 degrees
    building_225_23 = 23, // SCRCMD_EXPLODE_BUILDING 1: 225 degrees
    building_135_24 = 24, // SCRCMD_EXPLODE_BUILDING 3: 135 degrees
    building_315_25 = 25, // SCRCMD_EXPLODE_BUILDING 4: 315 degrees
    unknown_28 = 28,
    unknown_29 = 29,
    unknown_30 = 30,
    unknown_31 = 31,
    no_ring_32 = 32, // SCRCMD_EXPLODE_NO_RING1
    small_33 = 33,   // same size as small_18
    unknown_34 = 34,
    unknown_39 = 39,
};
} // namespace explosion_type

// Explosion_30::field_1A_timer: counts down every update, the effect ends at 0
namespace explosion_timer
{
enum
{
    damage_phase_50 = 50, // below this the blast no longer hurts (ApplyBlastDamage_541850 runs while above it)
    first_tick_99 = 99,   // first update of an explosion: camera shake, impulse on cars
    start_100 = 100,      // timer a new explosion starts with
    forever_9999 = 9999,  // never counts down (fires follow their object until it is removed)
};
} // namespace explosion_timer
