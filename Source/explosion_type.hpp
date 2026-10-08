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
    trail_3 = 3, // 3, 4, 5, 12, 13, 14 emit a moving particle trail from the object's speed/angle (IsTrailType_5435D0)
    trail_4 = 4,
    trail_5 = 5,
    trail_12 = 12,
    trail_13 = 13,
    trail_14 = 14,
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
