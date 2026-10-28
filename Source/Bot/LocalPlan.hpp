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
    // the global route ahead of the car (corner points): tiles away from it cost, so the detailed route joins it at once and
    // follows it; with none the line from the start to the goal is used
    const float* route_x;
    const float* route_y;
    int route_n;
    float own_speed; // tiles per frame now (how long until we reach a tile, for moving cars); 0 for a guess
    // may the car go from tile (x, y) to its neighbour (nx, ny)? False for a wall or a step up between them (the whole-map route
    // honours these, so the detailed one has to). May be NULL
    bool (*edge_ok)(void* user, int x, int y, int nx, int ny);
    // 0 for a tile of no consequence, else the axis of the ramp it is on or leads to (1 along x, 2 along y) plus 4 when it is a ramp tile
    // itself and not only one of the two tiles before it. A line over such a tile must run along the axis, in the middle of the tile.
    // May be NULL
    int (*ramp_zone)(void* user, float x, float y);
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
    // the square route as tiles, and the tiles a moving car will be over soon (red on the overlay), so the plan through traffic can be seen
    int tile_count;
    short tile_x[160];
    short tile_y[160];
    int moving_count;
    short moving_x[96];
    short moving_y[96];
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

enum
{
    kLocalRouteMax = 24
};

// The stretch of the global route (corner points wx/wy, the car heading for point `next`) that the detailed route is planned
// along: the car's place projected onto the leg it is on, then `dist` tiles on. The points are written to route_x/route_y
// (*route_n of them, the last is the goal)
void LocalPlan_RouteAhead(const float* wx, const float* wy, int n, int next, float x, float y, float dist, float* route_x, float* route_y, int* route_n);

// The corner the car should now head for: `next` when the car is on the leg towards it, or a later one when the car is already on
// (within max_off tiles of) a later leg, as when the route jogs aside and back and the car went straight on. Looks at most four
// corners on
int LocalPlan_RouteProgress(const float* wx, const float* wy, int n, int next, float x, float y, float max_off);

// A ramp is taken straight along its axis, lined up two tiles before it: the aim point (pure pursuit, `look` tiles along the path) must
// not run on past the place the path comes to the ramp's approach until the car is in it, nor cut the corner at its top while the car
// is on it. Returns the look distance to use: `look`, or less when the path ahead runs over slope tiles
// (is_slope says whether the tile at a point is one) and then leaves them, so the car aims at the end of the ramp, not past it
float LocalPlan_RampLook(const LocalPlanOut& p, float x, float y, float look, int (*ramp_zone)(void* user, float x, float y), void* user);

// Is a moving or standing car going to be in the way at a point of our route? (px, py) is the point, `frames` how long until we are
// there. The car is taken to carry on at its speed. True when its box, grown by our half width, covers the point then
bool LocalPlan_CarInWayAt(const SteerCar& car, float px, float py, float frames, float own_hw);
