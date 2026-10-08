#pragma once

// CarChaseTask_40::field_C_chase_state: what a chasing car (police) is doing to box in the car of the target ped.
// Left / right are as seen by the chasing car. Pairs (6/7, 8/9, 10/11) are the left and right version of a state.
// Kept in its own header so that the enum does not land in a header many TUs share.
namespace car_chase_state
{
enum
{
    follow_behind_0 = 0,      // drive behind the target car
    unused_1 = 1,
    follow_behind_2 = 2,      // same as 0, set when no other car is following behind
    move_left_3 = 3,          // turn anticlockwise to get next to the target
    move_right_4 = 4,         // turn clockwise to get next to the target
    choose_side_5 = 5,        // looks for a free side (left or right) to approach from
    approach_left_6 = 6,      // accelerate until level with the target
    approach_right_7 = 7,
    alongside_left_8 = 8,     // drive next to the target, matching its speed
    alongside_right_9 = 9,
    pull_ahead_left_10 = 10,  // drive up to the front of the target
    pull_ahead_right_11 = 11,
    in_front_12 = 12,         // in front of the target
    in_front_stopping_13 = 13,
    unused_14 = 14,
    target_stopped_15 = 15,   // the target has stopped, the crew gets out
};
} // namespace car_chase_state

// CarChaseTask_40::field_8_task_type
namespace car_task_type
{
enum
{
    goto_position_1 = 1,    // drive to the objective target position
    follow_ped_2 = 2,       // follow / chase a ped
    follow_target_ped_4 = 4, // Ped::SetupCarFollowTargetPed_469E50
    kill_ped_5 = 5,         // chase a ped for the kill_char_any_means objective
};
} // namespace car_task_type
