#include "FrenzyRange.hpp"
#include <math.h>

namespace
{
const int kRocket = 2;
const int kShocker = 3;
const int kMolotov = 4;
const int kGrenade = 5;
const int kBaton = 7;
const int kFlamethrower = 8;
} // namespace

FrenzyRange FrenzyRange_Of(int weapon, bool car_mode, int retries)
{
    FrenzyRange r;
    r.min_d = 1.5f;
    r.max_d = 9.0f;
    r.fire_slack = 1.0f;
    r.fire_angle = 32.0f;
    if (weapon == kRocket || weapon == kMolotov || weapon == kGrenade)
    {
        // the blast is wide; a molotov and a grenade are thrown and land short of the target
        r.min_d = (car_mode ? 7.0f : (weapon == kRocket ? 5.0f : 3.0f)) + 1.0f * static_cast<float>(retries);
        r.max_d = r.min_d + 6.0f;
        r.fire_angle = 12.0f;
    }
    else if (weapon == kFlamethrower)
    {
        // the flame's tail reaches about 2.5 tiles: walk right up to the target and fire only when he is inside it
        r.min_d = 0.8f;
        r.max_d = 2.2f;
        r.fire_slack = 0.4f;
        r.fire_angle = 16.0f;
    }
    else if (weapon == kShocker || weapon == kBaton)
    {
        r.min_d = 1.0f;
        r.max_d = 1.8f;
        r.fire_slack = 0.4f;
    }
    return r;
}

bool FrenzyRange_FireOk(const FrenzyRange& r, float err_deg, float target_dist, bool clear_shot)
{
    return clear_shot && fabsf(err_deg) < r.fire_angle && target_dist >= r.min_d - 0.3f && target_dist <= r.max_d + r.fire_slack;
}
