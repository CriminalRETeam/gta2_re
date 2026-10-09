#pragma once

// Second parameter of Ped::UpdateMovementTowardsTarget_4672E0: which target position the ped moves towards.
// Kept in its own header so that the enum does not land in a header many TUs share.
namespace ped_move_target_type
{
enum
{
    internal_target_ped_0 = 0,
    internal_target_pos_1 = 1,
    move_target_pos_2 = 2, // x / y of the move target, z of the car to enter
    objective_target_ped_3 = 3,
    objective_target_pos_4 = 4,
    objective_target_car_5 = 5,
    internal_target_object_6 = 6,
    objective_target_object_7 = 7,
};
} // namespace ped_move_target_type
