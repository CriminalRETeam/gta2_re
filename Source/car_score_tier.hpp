#pragma once

// Second argument of PlayerScoreTracker::GetCarScoreValue: multiplier applied to the car model's base value.
// Kept in its own header, like car_despawn_status.hpp, so that the enum does not land in a header many TUs share.
namespace car_score_tier
{
enum
{
    hijacked_0 = 0,  // x1 (AwardCarHijackedScore)
    unused_x2_1 = 1, // x2, no caller
    destroyed_2 = 2, // x5 (AwardCarDestroyedScore)
};
} // namespace car_score_tier
