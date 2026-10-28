#include "EvadeRule.hpp"
#include <math.h>

int Evade_Threat(float px, float py, const EvadeCar* cars, int n, float* side, float* along)
{
    int threat = -1;
    float best = 0.0f;
    for (int i = 0; i < n; i++)
    {
        const EvadeCar& c = cars[i];
        const float v = sqrtf(c.vx * c.vx + c.vy * c.vy);
        const float dx = px - c.x;
        const float dy = py - c.y;
        if (v < kEvadeMinSpeed || dx * dx + dy * dy > kEvadeNear * kEvadeNear)
        {
            continue;
        }
        const float ux = c.vx / v;
        const float uy = c.vy / v;
        const float ahead = dx * ux + dy * uy;       // ahead of the car
        const float across = dx * -uy + dy * ux;     // to its left (+) or right (-)
        // it reaches us within about 25 frames, and we stand in its path (a car is about 1.3 tiles wide)
        if (ahead > 0.0f && ahead < 1.5f + v * 10.0f && fabsf(across) < 1.0f && (threat < 0 || ahead < best))
        {
            threat = i;
            best = ahead;
            *side = across >= 0.0f ? 1.0f : -1.0f;
            *along = ahead;
        }
    }
    return threat;
}
