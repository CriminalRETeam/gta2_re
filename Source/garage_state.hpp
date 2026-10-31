#pragma once

// Garage::mState, stepped by Garage::GaragesService
namespace garage_state
{
enum
{
    idle_0 = 0,
    waiting_for_car_1 = 1,
    pushing_2 = 2,
    park_finished_3 = 3,
};
} // namespace garage_state
