#pragma once

// Explosion::mType: what kind of explosion/fire effect this is. Created by Object_5C::CreateExplosion
// (a6), the script explosion commands pass small_18 / item_19 / large_20 / no_ring_32 and the building ones 22-25. Kept in
// its own header, like car_despawn_status.hpp, so that the enum does not land in a header many TUs share.
namespace explosion_type
{
enum
{
    none_0 = 0,
    blood_burst_1 = 1, // ParticleManager::EmitBloodBurst. 1, 29, 31, 34, 39: Particle_4C::mState values, same numbering
    unknown_2 = 2,
    fire_3 = 3, // 3, 4, 5, 12, 13, 14 are fires: a moving particle trail from the object's speed/angle (IsFireType)
    car_fire_4 = 4, // Car::SpawnFire, a burning car
    molotov_fire_5 = 5, // the flames of a thrown molotov (Weapon), also left by explosions
    car_fire_level1_12 = 12, // Car::GetFireExplosionType, fire level 1 to 3
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
    flash_29 = 29, // Explosion::SpawnFlashParticle
    unknown_30 = 30,
    flame_stream_31 = 31, // ParticleManager::EmitFlameStreamSegment
    no_ring_32 = 32, // SCRCMD_EXPLODE_NO_RING1
    small_33 = 33,   // same size as small_18
    fire_truck_spray_34 = 34, // ParticleManager::EmitFireTruckSprayParticle
    blood_39 = 39, // ParticleManager::SpawnBlood
};
} // namespace explosion_type

// Explosion::mTimer: counts down every update, the effect ends at 0
namespace explosion_timer
{
enum
{
    damage_phase_50 = 50, // below this the blast no longer hurts (ApplyBlastDamage runs while above it)
    first_tick_99 = 99,   // first update of an explosion: camera shake, impulse on cars
    start_100 = 100,      // timer a new explosion starts with
    forever_9999 = 9999,  // never counts down (fires follow their object until it is removed)
};
} // namespace explosion_timer

// Explosion::EmitBuildingDebris_22_23_24_25 argument: which way the debris flies (the angle is added to a random
// spread). Update maps building_135_24 -> 0, building_315_25 -> 1, building_225_23 -> 2, building_45_22 -> 3.
namespace building_debris_dir
{
enum
{
    towards_135_0 = 0,
    towards_315_1 = 1,
    towards_225_2 = 2,
    towards_45_3 = 3,
};
} // namespace building_debris_dir
