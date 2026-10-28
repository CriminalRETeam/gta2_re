#pragma once
#include "DriveSteer.hpp"

// The detailed route for the next stretch of a drive: an A* with one node per map tile over a window round the car and a point
// 15 to 20 tiles along the global route, in 8 directions with a cost for every turn (so the path keeps to the direction of
// the line and does not zigzag), tiles under or beside standing cars very dear (round them when there is a way, a bump through when there is none), the track of moving cars costly, and the
// tiles of buildings, water and fences closed. The car follows it by pure pursuit. A pure function (no game types), tested
// natively in Scripts/bot/sim/local_plan_sim.cpp.

struct LocalPlanWorld
{
    bool (*solid)(void* user, float x, float y); // a building, water, a fence (as the steering sees them)
    // extra cost of stepping onto the tile at (x, y) going (dx, dy): traffic against the lane, say. May be NULL
    float (*step_cost)(void* user, float x, float y, int dx, int dy);
    void* user;
    const SteerCar* cars; // not including our own
    int car_count;
    float own_hl, own_hw;
};

enum
{
    kLocalPlanMax = 64
};

struct LocalPlanOut
{
    bool found;
    bool partial; // the route ends at the reachable tile nearest the goal, not at the goal
    int count;
    float length; // tiles, along the path
    float x[kLocalPlanMax];
    float y[kLocalPlanMax];
    // for the overlay: the tiles the search made dear because of standing cars
    int car_closed_count;
    short car_closed_x[96];
    short car_closed_y[96];
};

// Plans from (sx, sy), facing heading_deg (0 is -Y, clockwise), to (gx, gy). The goal is moved to the nearest open tile if it
// is closed
bool LocalPlan_Find(const LocalPlanWorld& w, float sx, float sy, float heading_deg, float gx, float gy, LocalPlanOut* out);

// The point `look` tiles along the path from the place nearest to (x, y)
void LocalPlan_AimPoint(const LocalPlanOut& p, float x, float y, float look, float* aim_x, float* aim_y);

// Is the path still good for the car at (x, y)? False when the car has strayed more than two tiles from it, when less than
// nine tiles of it are left, or when a standing car or a closed tile now lies on the next 14 tiles of it. A path that is
// still good is kept: planning afresh every few frames moves the aim point about and the wheel with it
bool LocalPlan_StillGood(const LocalPlanWorld& w, const LocalPlanOut& p, float x, float y);
