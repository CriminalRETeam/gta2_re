#pragma once
#include "DriveSteer.hpp"

// The fine stage of the detailed route: the tile route (LocalPlan) says which streets and gaps to take, but a tile is 1 x 1 and a car is
// 0.8 x 0.4 and moves, so round a car it can only say "this tile or that". This takes that route and, when something is in the way of it,
// plans again on cells a quarter of a tile wide, in a corridor round the tile route: walls (by the cells), standing cars and pedestrians with
// their boxes, and moving cars with their box swept over the next frames, at the time we get there (so a car crossing is passed behind
// or in front, not run into). The cells' path is pulled straight to a few nodes. A pure function, tested natively in
// Scripts/bot/sim/fine_plan_sim.cpp.

struct FinePlanWorld
{
    bool (*solid)(void* user, float x, float y); // a building, water, a fence
    void* user;
    const SteerCar* cars; // not including our own; the last ped_count of them are pedestrians
    int car_count;
    int ped_count;
    float own_hl, own_hw;
    float own_speed; // tiles per frame now (0 for a guess)
    // 0 for a point of no consequence, else it is on or by a ramp (the tile route lines those up: left alone). May be NULL
    int (*ramp_zone)(void* user, float x, float y);
    // may the car go from tile (x, y) to its neighbour (nx, ny)? False for a wall face or a step between them. May be NULL
    bool (*edge_ok)(void* user, int x, int y, int nx, int ny);
    // the route of the frame before (prev_n points, 0 for none): a new route stays near it, so equal ways round a car do not swap from frame to
    // frame and the wheel does not follow them
    const float* prev_x;
    const float* prev_y;
    int prev_n;
};

// How far ahead of the car the aim point is on the grid path, tiles: short, because the car cuts a corner by about what it looks ahead (the longer
// look of the arcs, 4 + 40 x speed up to 12, took it a tile inside the corner of the path and into a car parked there). The game, the
// planner's own drive of its route and the steering sim all use this
inline float FinePlan_Look(float speed)
{
    const float look = 2.5f + speed * 15.0f;
    return look < 5.0f ? look : 5.0f;
}

// Refines the tile route (px, py: pn points, the first is the car) from (sx, sy). Returns -1 when the route is left as it is (nothing is in its
// way, it is on a ramp, or there is no way round); else the pass that found one: 0 round everything, 1 with pedestrians left out (no way round
// them), 2 with cars only dear, not closed (no way round anything). The new route is written to ox/oy (*on points, at most max_out)
int FinePlan_Refine(const FinePlanWorld& w, const float* px, const float* py, int pn, float sx, float sy, float* ox, float* oy, int* on, int max_out);
