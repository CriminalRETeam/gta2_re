#include "BlastAvoid.hpp"
#include <math.h>

bool BlastAvoid_IsHazard(int damage)
{
    return damage >= kBlastHotDamage && damage < kBlastGoneDamage + 1;
}

static bool Within(const BlastWorld& w, float x, float y, float range)
{
    for (int i = 0; i < w.count; i++)
    {
        if (!BlastAvoid_IsHazard(w.hazards[i].damage))
        {
            continue;
        }
        const float dx = w.hazards[i].x - x;
        const float dy = w.hazards[i].y - y;
        if (dx * dx + dy * dy < range * range)
        {
            return true;
        }
    }
    return false;
}

bool BlastAvoid_Unsafe(const BlastWorld& w, float x, float y)
{
    return Within(w, x, y, kBlastStandOff);
}

bool BlastAvoid_WillChain(const BlastWorld& w, float x, float y)
{
    return Within(w, x, y, kBlastRadius);
}

static bool WayClear(const BlastWorld& w, float x0, float y0, float x1, float y1)
{
    const int steps = 6;
    for (int i = 1; i <= steps; i++)
    {
        const float t = static_cast<float>(i) / steps;
        const float x = x0 + (x1 - x0) * t;
        const float y = y0 + (y1 - y0) * t;
        if (w.solid && w.solid(w.user, x, y))
        {
            return false;
        }
        if (i < steps && BlastAvoid_WillChain(w, x, y))
        {
            return false; // do not run through the blast on the way out
        }
    }
    return true;
}

BlastOut BlastAvoid_Decide(const BlastWorld& w)
{
    BlastOut out = {false, w.x, w.y};
    if (!BlastAvoid_Unsafe(w, w.x, w.y))
    {
        return out;
    }
    out.active = true;
    // The nearest ring that has a spot outside every stand-off with a clear way to it
    const int kAngles = 16;
    for (float r = 1.0f; r <= 7.0f; r += 0.5f)
    {
        bool found = false;
        float best_cost = 1e9f;
        for (int a = 0; a < kAngles; a++)
        {
            const float ang = a * 6.2831853f / kAngles;
            const float cx = w.x + sinf(ang) * r;
            const float cy = w.y - cosf(ang) * r;
            if (BlastAvoid_Unsafe(w, cx, cy) || !WayClear(w, w.x, w.y, cx, cy))
            {
                continue;
            }
            // among the spots at this distance, the one furthest from the hazards
            float nearest = 1e9f;
            for (int i = 0; i < w.count; i++)
            {
                if (BlastAvoid_IsHazard(w.hazards[i].damage))
                {
                    const float dx = w.hazards[i].x - cx;
                    const float dy = w.hazards[i].y - cy;
                    const float d = sqrtf(dx * dx + dy * dy);
                    nearest = d < nearest ? d : nearest;
                }
            }
            const float cost = -nearest;
            if (cost < best_cost)
            {
                best_cost = cost;
                out.aim_x = cx;
                out.aim_y = cy;
                found = true;
            }
        }
        if (found)
        {
            return out;
        }
    }
    return out; // boxed in: stay (aim = own spot)
}
