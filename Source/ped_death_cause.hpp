#pragma once

// Kept out of enums.hpp on purpose: a third enum there changes MapRenderer::Draw4SidedDiagonalUpLeft_4EF880
// (see docs/matching_quirks.md)
// Ped::field_290_death_cause: how the ped was last killed. Values 9..21 come from sub_48E780 (projectile model)
// or Car_BC::field_90 and are not named yet.
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
    projectile_default_9 = 9, // sub_48E780 fallback
    punched_10 = 10,
};
} // namespace ped_death_cause
