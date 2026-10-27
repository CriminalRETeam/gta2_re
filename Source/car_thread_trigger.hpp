#pragma once

// CarThreadTrigger::field_8_type: what has to enter the touch point (checked in CarThreadTriggers::CheckThreadTrigger_5C8780).
namespace car_thread_trigger_type
{
enum
{
    free_0 = 0,
    any_car_1 = 1,
    script_car_2 = 2,
    char_any_means_3 = 3,
    char_on_foot_4 = 4,
    char_in_car_5 = 5,
    char_stopped_any_means_6 = 6,
    char_stopped_on_foot_7 = 7,
    char_stopped_in_car_8 = 8,
};
} // namespace car_thread_trigger_type

// CarThreadTrigger::field_C_action: what happens once the trigger fires.
namespace car_thread_trigger_action
{
enum
{
    none_0 = 0,
    spawn_thread_2 = 2,
    set_generator_car_type_3 = 3,
};
} // namespace car_thread_trigger_action

// CarThreadTrigger::field_14_enable_state
namespace car_thread_trigger_state
{
enum
{
    fired_0 = 0,
    armed_1 = 1,
    disabled_2 = 2,
};
} // namespace car_thread_trigger_state
