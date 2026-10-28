#include "JamSwitch.hpp"

namespace
{
const int kJamFrames = 90;       // standing still in the jam this long before a swap is thought of
const float kMaxAlong = 7.0f;    // tiles ahead
const float kMaxSide = 3.5f;     // tiles to the side
const int kLessDamage = 1500;    // damage points less than ours
const float kFaster = 1.03f;     // top speed against ours
} // namespace

int JamSwitch_Choose(const JamSelf& self, const JamCar* cars, int n)
{
    if (self.still_frames < kJamFrames)
    {
        return -1;
    }
    int best = -1;
    float best_score = 1e9f;
    for (int i = 0; i < n; i++)
    {
        const JamCar& c = cars[i];
        if (c.police || c.locked || c.mission)
        {
            continue;
        }
        if (c.along < 0.3f || c.along > kMaxAlong || c.side > kMaxSide || c.side < -kMaxSide)
        {
            continue;
        }
        if (c.damage > self.damage - kLessDamage)
        {
            continue;
        }
        if (c.top_speed < self.top_speed * kFaster)
        {
            continue;
        }
        // the nearest; an empty car is much better than dragging a driver out of one
        const float score = c.along + (c.side < 0 ? -c.side : c.side) + (c.has_driver ? 6.0f : 0.0f);
        if (score < best_score)
        {
            best_score = score;
            best = i;
        }
    }
    return best;
}
