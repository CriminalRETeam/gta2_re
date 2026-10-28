#include "CarIntercept.hpp"
#include <math.h>

namespace
{
const float kMoving = 0.03f;      // tiles per frame: slower is stopped
const float kMinAhead = 3.0f;     // tiles ahead of the car at least: room to see us and brake
const float kMaxAhead = 16.0f;    // tiles ahead at most
const float kSpareFrames = 12.0f; // be there this long before the car
} // namespace

int CarIntercept_Plan(const InterceptIn& in, float* out_x, float* out_y)
{
    const float speed = sqrtf(in.car_vx * in.car_vx + in.car_vy * in.car_vy);
    if (speed < kMoving)
    {
        return kInterceptStopped;
    }
    const float dx = in.car_vx / speed;
    const float dy = in.car_vy / speed;
    for (float ahead = kMinAhead; ahead <= kMaxAhead; ahead += 1.0f)
    {
        const float sx = in.car_x + dx * ahead;
        const float sy = in.car_y + dy * ahead;
        const float to_spot = sqrtf((sx - in.ped_x) * (sx - in.ped_x) + (sy - in.ped_y) * (sy - in.ped_y));
        const float ped_frames = to_spot / (in.ped_speed > 0.01f ? in.ped_speed : 0.01f);
        const float car_frames = ahead / speed;
        if (ped_frames + kSpareFrames <= car_frames)
        {
            *out_x = sx;
            *out_y = sy;
            return kInterceptAhead;
        }
    }
    return kInterceptNever;
}
