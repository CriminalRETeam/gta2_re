#pragma once

// BonusTracker::GetBonusResult_432080. The script's bonus counter keeps passed/failed in place of the rule index.
namespace bonus_result
{
enum
{
    unused_rule = -1,
    in_progress = -2,
    passed = -3, // target count reached
    failed = -4, // time limit, a non matching event in check mode 1, or cancelled by the script
};
} // namespace bonus_result
