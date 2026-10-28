#include "DriveSteer.hpp"
#include <math.h>

namespace
{
const float kTurnDegPerFrame = 1.8f; // measured from the trace: a car turns about this fast at any speed
const float kPi = 3.14159265f;

float WrapDeg(float d)
{
    while (d > 180.0f)
    {
        d -= 360.0f;
    }
    while (d < -180.0f)
    {
        d += 360.0f;
    }
    return d;
}

// How far to turn the wheel for a heading error: nothing inside 8 degrees, half the time up to 20, then full. A wheel that
// is either hard over or straight overshoots and swings from side to side
float Gentle(float err)
{
    const float a = fabsf(err);
    const float m = a > 20.0f ? 1.0f : (a > 8.0f ? 0.5f : 0.0f);
    return err > 0.0f ? m : -m;
}

float HeadingDeg(float dx, float dy)
{
    return atan2f(dx, -dy) * 180.0f / kPi;
}

struct Arc
{
    float cost;
    float wall_d;
};

// Drives the car on for a few tiles: the wheel is held at s for the first stretch (that is the choice being tried),
// then the car steers at the aim point the way the driver does. Cost: where it ends relative to the aim point, and
// what it runs into on the way. A wall the car already touches does not count at the start: an arc beside a wall
// would be blocked at once and the car could not move out of it
Arc Rollout(const SteerWorld& w, float x, float y, float heading, float hdir, float s, float speed, float aim_x, float aim_y, SteerArcDebug* ad)
{
    Arc arc;
    if (ad)
    {
        ad->s = s;
        ad->n = 0;
        ad->hit = 0;
        ad->cost = 0.0f;
        ad->chosen = false;
    }
    float hard_car = 0.0f;
    float h = heading;
    const float v = speed < 0.10f ? 0.10f : speed; // it will pick up speed
    const float reach = 4.0f + speed * 45.0f;
    const int step = 3;
    const float hold = 1.5f; // tiles
    float driven = 0.0f;
    float t = 0.0f;
    bool wall = false;
    bool car_hit = false;
    // the closest the rollout comes to the aim point, and the heading it has there: an aim point nearer than the
    // reach must not make an arc that wiggles past it look better than one that goes straight at it
    float closest = 1e9f;
    float far_closest = 1e9f;
    float closest_err = 0.0f;
    while (driven < reach)
    {
        float turn = s;
        if (driven > hold)
        {
            const float err = WrapDeg(HeadingDeg(aim_x - x, aim_y - y) - h);
            // what hdir means: a right key press adds hdir to the heading, so turning towards err means pressing right when err*hdir > 0
            turn = Gentle(err) * hdir;
        }
        h += hdir * turn * kTurnDegPerFrame * step;
        const float rad = h * kPi / 180.0f;
        const float fx = sinf(rad);
        const float fy = -cosf(rad);
        x += fx * v * step;
        y += fy * v * step;
        driven += v * step;
        t += step;
        {
            const float ddx = aim_x - x;
            const float ddy = aim_y - y;
            const float d = sqrtf(ddx * ddx + ddy * ddy);
            if (d < closest)
            {
                closest = d;
            }
            // the heading towards the aim point only means something while it is still more than a tile away: right on
            // top of it the bearing swings about from step to step and would decide the choice by chance
            if (d > 1.0f && d < far_closest)
            {
                far_closest = d;
                closest_err = fabsf(WrapDeg(HeadingDeg(ddx, ddy) - h));
            }
        }
        if (ad && ad->n < 24)
        {
            ad->px[ad->n] = x;
            ad->py[ad->n] = y;
            ad->n++;
        }
        const float px = -fy * kFootHalfWidth;
        const float py = fx * kFootHalfWidth;
        const float pts[4][2] = {{x + fx * kFootFront + px, y + fy * kFootFront + py}, {x + fx * kFootFront - px, y + fy * kFootFront - py}, {x + px, y + py}, {x - px, y - py}};
        for (int i = 0; i < 4 && !wall; i++)
        {
            wall = w.solid(w.user, pts[i][0], pts[i][1]);
            if (wall && ad)
            {
                ad->hit = 1;
                ad->hit_x = pts[i][0];
                ad->hit_y = pts[i][1];
            }
        }
        if (wall)
        {
            break;
        }
        if (!car_hit && driven > 2.5f)
        {
            for (int k = 0; k < w.car_count; k++)
            {
                const SteerCar& oc = w.cars[k];
                const float cx = oc.x + oc.vx * t;
                const float cy = oc.y + oc.vy * t;
                if ((cx - x) * (cx - x) + (cy - y) * (cy - y) < kCarHitRadius * kCarHitRadius)
                {
                    car_hit = true;
                    if (ad)
                    {
                        ad->hit = 2;
                        ad->hit_x = x;
                        ad->hit_y = y;
                        ad->car_x = cx;
                        ad->car_y = cy;
                    }
                    const bool oncoming = oc.speed > 0.12f && oc.vx * fx + oc.vy * fy < -0.10f;
                    hard_car = (oncoming || oc.police) ? 25.0f : 4.0f;
                    break;
                }
            }
        }
    }
    arc.wall_d = wall ? driven : 1e3f;
    // an arc that goes by the aim point pointing away from it is no better than one that stops a little short of it
    arc.cost = closest + closest_err * (12.0f / 57.3f) + hard_car;
    if (wall)
    {
        arc.cost += 20.0f + (reach - driven) * 3.0f;
    }
    if (ad)
    {
        ad->cost = arc.cost;
    }
    return arc;
}
} // namespace

SteerChoice DriveSteer_Choose(const SteerWorld& w, float x, float y, float heading_deg, float hdir, float speed, float aim_x, float aim_y, float prev_s, SteerDebug* dbg)
{
    if (dbg)
    {
        dbg->arc_count = 0;
    }
    static const float options[5] = {0.0f, -1.0f, 1.0f, -0.5f, 0.5f};
    const float err = WrapDeg(HeadingDeg(aim_x - x, aim_y - y) - heading_deg);
    // the key that turns the car towards the aim point: a right key press adds hdir to the heading
    const float natural = Gentle(err) * hdir;
    SteerChoice best;
    best.s = natural;
    best.wall_d = 1e3f;
    best.changed = false;
    float best_cost = 1e9f;
    for (int o = 0; o < 5; o++)
    {
        SteerArcDebug* ad = (dbg && dbg->arc_count < 5) ? &dbg->arcs[dbg->arc_count++] : 0;
        const Arc arc = Rollout(w, x, y, heading_deg, hdir, options[o], speed, aim_x, aim_y, ad);
        const float cost = arc.cost - (options[o] == natural ? 3.0f : 0.0f) - (options[o] == prev_s ? 1.5f : 0.0f);
        if (cost < best_cost)
        {
            best_cost = cost;
            best.s = options[o];
            best.wall_d = arc.wall_d;
        }
    }
    best.changed = best.s != natural;
    if (dbg)
    {
        for (int i = 0; i < dbg->arc_count; i++)
        {
            dbg->arcs[i].chosen = dbg->arcs[i].s == best.s;
        }
    }
    return best;
}

float DriveSteer_SpeedFor(float wall_d)
{
    float allowed = (wall_d - 1.4f) * 0.06f;
    // never stop dead: beside a wall every way ahead is blocked at once, and standing still is what keeps the car
    // there. A creep costs next to no damage and lets it turn away
    return allowed < 0.04f ? 0.04f : allowed;
}

namespace
{
// Is the strip (half width 0.7) from (x0,y0) to (x1,y1) free of buildings and of cars (where they will be about when we pass)?
// 0 free, 2 a building on the way, 3 a car on the way
int StripFree(const SteerWorld& w, float x0, float y0, float x1, float y1, float speed, const SteerCar* skip)
{
    const float dx = x1 - x0;
    const float dy = y1 - y0;
    const float len = sqrtf(dx * dx + dy * dy);
    if (len < 0.1f)
    {
        return 0;
    }
    const float ux = dx / len;
    const float uy = dy / len;
    const float v = speed < 0.10f ? 0.10f : speed;
    for (float d = 0.6f; d <= len; d += 0.5f)
    {
        const float px = x0 + ux * d;
        const float py = y0 + uy * d;
        // the same four points of the car's footprint as the arcs test, with the car pointing along the strip
        const float sx = -uy * kFootHalfWidth;
        const float sy = ux * kFootHalfWidth;
        if (w.solid(w.user, px + ux * kFootFront + sx, py + uy * kFootFront + sy) || w.solid(w.user, px + ux * kFootFront - sx, py + uy * kFootFront - sy) ||
            w.solid(w.user, px + sx, py + sy) || w.solid(w.user, px - sx, py - sy))
        {
            return 2;
        }
        const float t = d / v;
        for (int k = 0; k < w.car_count; k++)
        {
            if (&w.cars[k] == skip)
            {
                continue;
            }
            const float cx = w.cars[k].x + w.cars[k].vx * t;
            const float cy = w.cars[k].y + w.cars[k].vy * t;
            if ((cx - px) * (cx - px) + (cy - py) * (cy - py) < kCarHitRadius * kCarHitRadius)
            {
                return 3;
            }
        }
    }
    return 0;
}
} // namespace

bool DriveSteer_Overtake(const SteerWorld& w, float x, float y, float speed, float aim_x, float aim_y, float* out_x, float* out_y, OvertakeTrace* trace)
{
    if (trace)
    {
        trace->has_blocker = false;
        trace->count = 0;
    }
    const float ax = aim_x - x;
    const float ay = aim_y - y;
    const float al = sqrtf(ax * ax + ay * ay);
    if (al < 6.0f)
    {
        return false;
    }
    const float ux = ax / al;
    const float uy = ay / al;
    const SteerCar* blocker = 0;
    float blocker_along = 1e9f;
    for (int k = 0; k < w.car_count; k++)
    {
        const SteerCar& oc = w.cars[k];
        const float along = (oc.x - x) * ux + (oc.y - y) * uy;
        const float side = (oc.x - x) * -uy + (oc.y - y) * ux;
        const float along_speed = oc.vx * ux + oc.vy * uy;
        // slower than we are going (or standing), not coming at us, and in our way
        if (along > 1.5f && along < 9.0f && fabsf(side) < 1.5f && along_speed < (speed < 0.10f ? 0.10f : speed) * 0.8f && along_speed > -0.04f && along < blocker_along)
        {
            blocker = &oc;
            blocker_along = along;
        }
    }
    if (!blocker)
    {
        return false;
    }
    if (trace)
    {
        trace->has_blocker = true;
        trace->blocker_x = blocker->x;
        trace->blocker_y = blocker->y;
    }
    // a point beside it and past it: try the near side first, then the far, from close in to wide
    static const float offsets[6] = {2.6f, -2.6f, 3.6f, -3.6f, 4.6f, -4.6f};
    for (int i = 0; i < 6; i++)
    {
        const float cx = blocker->x + ux * 4.5f + -uy * offsets[i];
        const float cy = blocker->y + uy * 4.5f + ux * offsets[i];
        int why = 0;
        if (w.solid(w.user, cx, cy))
        {
            why = 1;
        }
        else
        {
            why = StripFree(w, x, y, cx, cy, speed, blocker);
        }
        if (trace && trace->count < 6)
        {
            trace->cand_x[trace->count] = cx;
            trace->cand_y[trace->count] = cy;
            trace->cand_why[trace->count] = why;
            trace->count++;
        }
        if (why == 0)
        {
            *out_x = cx;
            *out_y = cy;
            return true;
        }
    }
    return false;
}
