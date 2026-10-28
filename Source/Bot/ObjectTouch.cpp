#include "ObjectTouch.hpp"
#include <math.h>

// the part of each half side that counts as the middle
static const float kMiddleFraction = 0.5f;

TouchAim ObjectTouch_Aim(const TouchBox& box, float ox, float oy)
{
    TouchAim a;
    if (!box.valid)
    {
        a.x = ox;
        a.y = oy;
        a.reach = kTouchNoBoxReach;
        return a;
    }
    a.x = box.cx;
    a.y = box.cy;
    const float small = box.hl < box.hw ? box.hl : box.hw;
    float reach = small * kMiddleFraction;
    reach = reach < kTouchMinReach ? kTouchMinReach : reach;
    reach = reach > kTouchMaxReach ? kTouchMaxReach : reach;
    a.reach = reach;
    return a;
}

bool ObjectTouch_Inside(const TouchBox& box, float ox, float oy, float px, float py)
{
    if (!box.valid)
    {
        return (px - ox) * (px - ox) + (py - oy) * (py - oy) <= kTouchNoBoxReach * kTouchNoBoxReach;
    }
    const float dx = px - box.cx;
    const float dy = py - box.cy;
    const float along = dx * box.ax + dy * box.ay;
    const float across = -dx * box.ay + dy * box.ax;
    return fabsf(along) <= box.hl * kMiddleFraction && fabsf(across) <= box.hw * kMiddleFraction;
}
