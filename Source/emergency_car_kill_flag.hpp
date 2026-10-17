#pragma once

// PlayerScoreTracker_36C::field_1A4_killed_cars_flags bits: which kinds of emergency vehicle the player destroyed within
// a short time (the "em_dest" bonus is paid once all three are set). Kept in its own header, like
// car_despawn_status.hpp, so that the enum does not land in a header many TUs share.
namespace emergency_car_kill_flag
{
enum
{
    none_0 = 0,
    medicar_1 = 1,
    cop_car_2 = 2, // COPCAR, SWATVAN, EDSELFBI
    fire_truck_4 = 4,
    all_7 = medicar_1 | cop_car_2 | fire_truck_4,
};
} // namespace emergency_car_kill_flag
