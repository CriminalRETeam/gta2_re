#include "CarPickRule.hpp"
#include "CarIntercept.hpp"
#include <math.h>

namespace
{
const int kMaxDamage = 1500;       // a wreck is no use
const float kStoppedNear = 10.0f;  // a car waiting at a light drives off before we are there unless it is this close
const float kStoppedWalk = 7.0f;   // ... and the walk to it is this short
const float kPedSpeed = 0.06f;     // tiles per frame the walker covers
const float kSpare = 12.0f;        // frames to be there before the car
const float kMaxStand = 30.0f;     // the stand spot is within this of us
} // namespace

int CarPick_Kind(const CarPickPlayer& p, const CarPickCar& c, float max_dist, float* stand_x, float* stand_y)
{
    *stand_x = c.x;
    *stand_y = c.y;
    if (c.locked || c.police || c.mission || c.damage > kMaxDamage || c.dist > max_dist || c.bad_model)
    {
        return kCarPickNo;
    }
    if (!c.has_driver)
    {
        return kCarPickParked;
    }
    if (c.speed > 0.03f)
    {
        if (p.wanted_stars == 0 && p.cops_near)
        {
            return kCarPickNo; // a hijack in front of the police ends in an arrest
        }
        InterceptIn in;
        in.car_x = c.x;
        in.car_y = c.y;
        in.car_vx = c.vx;
        in.car_vy = c.vy;
        in.ped_x = p.x;
        in.ped_y = p.y;
        in.ped_speed = kPedSpeed;
        return CarIntercept_Plan(in, stand_x, stand_y) == kInterceptAhead ? kCarPickIntercept : kCarPickNo;
    }
    if ((p.wanted_stars == 0 && p.cops_near) || c.dist > kStoppedNear)
    {
        return kCarPickNo;
    }
    return kCarPickStopped;
}

bool CarPick_WalkOk(int kind, const CarPickCar& c, float walk_len, float stand_to_car)
{
    switch (kind)
    {
        case kCarPickParked:
            return true;
        case kCarPickStopped:
            return walk_len <= kStoppedWalk;
        case kCarPickIntercept:
        {
            const float speed = c.speed > 0.01f ? c.speed : 0.01f;
            return walk_len / kPedSpeed + kSpare <= stand_to_car / speed && walk_len <= kMaxStand * 1.5f;
        }
        default:
            return false;
    }
}
