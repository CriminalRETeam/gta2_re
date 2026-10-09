#pragma once

// Second argument of PlayerScoreTracker_36C::GetCarScoreValue_5925B0: multiplier applied to the car model's base value.
// Kept in its own header, like car_despawn_status.hpp, so that the enum does not land in a header many TUs share.
namespace car_score_tier
{
enum
{
    hijacked_0 = 0,  // x1 (AwardCarHijackedScore_593240)
    unused_x2_1 = 1, // x2, no caller
    destroyed_2 = 2, // x5 (AwardCarDestroyedScore_592DD0)
};
} // namespace car_score_tier
