#pragma once

// CarThreadTrigger::field_8_type: what has to touch the trigger area (CarThreadTriggers::CheckThreadTrigger_5C8780).
namespace thread_trigger_type
{
enum
{
    any_car_1 = 1,
    script_car_2 = 2,
    char_any_means_3 = 3, // the script char on foot or driving
    char_on_foot_4 = 4,
    char_driving_5 = 5,
};
} // namespace thread_trigger_type

// CarThreadTrigger::field_14_enable_state
namespace thread_trigger_state
{
enum
{
    triggered_0 = 0,
    enabled_1 = 1,
    disabled_2 = 2,
};
} // namespace thread_trigger_state
