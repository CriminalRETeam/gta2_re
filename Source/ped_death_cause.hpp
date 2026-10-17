#pragma once

// Kept out of enums.hpp on purpose: a third enum there changes MapRenderer::Draw4SidedDiagonalUpLeft_4EF880
// (see docs/matching_quirks.md)
// Ped::field_290_death_cause: how the ped was last killed. Values 9..21 come from GetDeathCauseForObjectModel_48E780 (projectile model)
// or Car_BC::field_90, which uses the same values.
namespace ped_death_cause
{
enum
{
    none_0 = 0,
    run_over_1 = 1,
    electrocuted_2 = 2,
    run_over_by_stolen_car_3 = 3, // the victim's own stolen car (scores 10000 in a network game)
    unknown_4 = 4,
    unknown_5 = 5,
    projectile_default_9 = 9, // GetDeathCauseForObjectModel_48E780 fallback
    punched_10 = 10,
    bullet_11 = 11,       // machine gun / pistol bullet
    bomb_12 = 12,         // car bomb (moving_collect_36_132)
    fire_13 = 13,         // fire_197
    fire_hit_14 = 14,     // fire_hitting_194
    grenade_15 = 15,      // objects 182, 183
    molotov_16 = 16,
    rocket_bullet_17 = 17, // rocket_bullet_128
    rocket_18 = 18,       // rocket_160
    shotgun_19 = 19,
    burning_20 = 20,      // maybe_bullet_on_fire_198, shop_car_oil_stain_251
    unknown_21 = 21,      // model 10
    any_weapon_22 = 22,   // only in a bonus rule: matches 4 and 10..19 (see BonusTracker_1C0::IsDeathCauseInGroup_432170)
    any_23 = 23,          // wildcard in a bonus definition (BonusTracker_1C0)
};
} // namespace ped_death_cause
