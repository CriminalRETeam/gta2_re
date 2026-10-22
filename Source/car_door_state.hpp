#pragma once

// Car_Door::field_4_state. Close_439EA0 asks for close_requested_5, which waits two Service_439DA0 ticks before closing_3.
namespace car_door_state
{
enum
{
    inactive_0 = 0,
    open_1 = 1,
    opening_2 = 2,
    closing_3 = 3,
    about_to_close_4 = 4,
    close_requested_5 = 5,
    closed_6 = 6,
};
} // namespace car_door_state
