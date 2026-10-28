#pragma once

// The one route finder under the bot's driving: an A* over a square grid of tiles, moving one tile up, right, down or left at
// a time, with a cost for every turn, so a route is long straight legs joined by right angles (block by block, as a driver
// goes) and not a diagonal cut over kerbs and corners. A state is a tile and the way it was entered (turn costs need it).
// Everything about the world comes from one callback, so the same search serves the whole-map car route (static tiles, lanes,
// jams), the window round the car (moving traffic by when we get to each tile) and anything else. A pure function with no
// game types, tested natively in Scripts/bot/sim/grid_route_sim.cpp.

enum
{
    kGridDirUp = 0, // -Y
    kGridDirRight = 1,
    kGridDirDown = 2,
    kGridDirLeft = 3
};

struct GridRouteWorld
{
    int x0, y0; // the tile at the window's corner
    int w, h;   // the window, tiles (up to 256 x 256)
    // The cost of going from tile (x, y) to its neighbour in direction dir (kGridDir*), on top of 1 for the step: the
    // neighbour's terrain, a wall between the two, the lane arrows, standing and moving cars. Negative: no way. `g` is how
    // far the route has gone to reach (x, y), in tiles, which is how long until we are there (moving cars)
    float (*step)(void* user, int x, int y, int dir, float g);
    void* user;
};

struct GridRouteParams
{
    float turn_cost;  // for a right-angle turn
    float uturn_cost; // for turning right round
    int start_dir;    // the way the start faces (kGridDir*), -1 for any
    int max_expand;   // search effort
    bool allow_partial; // when the goal cannot be reached end at the reachable tile nearest it
    float min_partial_gain; // ... if that is at least this many tiles nearer than the start
    float h_weight;   // weight of the straight-distance estimate (1 is exact, more is greedier and faster)
};

enum
{
    kGridRouteMax = 1024
};

struct GridRouteOut
{
    bool found;
    bool partial;
    int count; // tiles, the start first
    float length;
    short x[kGridRouteMax];
    short y[kGridRouteMax];
    int expanded;
};

void GridRoute_DefaultParams(GridRouteParams* p);

// A route from tile (sx, sy) to (gx, gy), both inside the window. False when there is none
bool GridRoute_Find(const GridRouteWorld& w, int sx, int sy, int gx, int gy, const GridRouteParams& p, GridRouteOut* out);

// The corners of a route: the tile centres where it changes direction, with the first and last, as points (tile + 0.5).
// Returns how many were written
int GridRoute_Corners(const GridRouteOut& r, float* xs, float* ys, int max);

// The direction (kGridDir*) closest to a heading in degrees (0 is -Y, clockwise)
int GridRoute_DirOfHeading(float heading_deg);
