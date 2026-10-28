#pragma once

// The driver's choice of wheel position for the next moments, as a pure function of the world (a callback that says
// where the solid ground is, and the cars about), so that it runs without the game: Scripts/bot/sim/ tests it on
// fake and real maps natively. Cars are the game's own collision boxes (the corners of each sprite's collision
// shape), ours included, and the overlay draws those same boxes.

// An oriented box: the centre, the unit vector along the long side, half the length and half the width (tiles)
struct SteerBox
{
    float cx, cy;
    float ax, ay;
    float hl, hw;
};

void SteerBox_Corners(const SteerBox& b, float* xs, float* ys);    // the 4 corners
void SteerBox_Points(const SteerBox& b, float* xs, float* ys);     // the 4 corners and the 4 middles of the sides, to test against buildings
bool SteerBox_Overlap(const SteerBox& a, const SteerBox& b);
bool SteerBox_Contains(const SteerBox& b, float x, float y, float margin);

struct SteerCar
{
    float x, y;
    float vx, vy; // tiles per frame
    float speed;
    bool police;
    SteerBox box; // where it is now (box.cx, box.cy == x, y); it moves with vx, vy
};

struct SteerWorld
{
    bool (*solid)(void* user, float x, float y); // a building, water or the edge of the map
    void* user;
    const SteerCar* cars; // not including our own
    int car_count;
    float own_hl, own_hw; // our own car's box: half length, half width
};

// What one rollout did, for the overlay: the track of the car's centre and what stopped it
struct SteerArcDebug
{
    float s;     // the wheel held for the first stretch
    float cost;
    bool chosen;
    int n;       // points of the track
    float px[24];
    float py[24];
    int hit;     // 0 nothing in reach, 1 a building (or water) at (hit_x, hit_y), 2 a car: our box and its box at the moment
    float hit_x, hit_y;
    SteerBox own_box;
    SteerBox car_box;
};

struct SteerDebug
{
    int arc_count;
    SteerArcDebug arcs[5];
};

struct SteerChoice
{
    float s;      // wheel: -1 left key, +1 right key, 0.5 = every other frame, 0 = straight
    float wall_d; // tiles the car can go along the chosen way before a building (1e3 if none in reach)
    bool changed; // differs from steering straight at the aim point
};

// heading_deg: 0 is -Y, clockwise (the direction the car faces). hdir: how much a right key press adds to it per
// frame, in degrees' sign (+1 or -1). aim: where the route wants the car to go
// prev_s: what this returned last frame (a small preference for staying with it: choices that flip from frame to frame
// are a weave)
SteerChoice DriveSteer_Choose(const SteerWorld& w, float x, float y, float heading_deg, float hdir, float speed, float aim_x, float aim_y, float prev_s, SteerDebug* dbg);

// The speed (tiles per frame) that still stops the car before a wall that far ahead
float DriveSteer_SpeedFor(float wall_d);

// What the overtake looked at, for the debug draw: the car it found in the way and each place it tried, with why a place was
// turned down
struct OvertakeTrace
{
    bool has_blocker;
    float blocker_x, blocker_y;
    int count;
    float cand_x[6];
    float cand_y[6];
    int cand_why[6]; // 0 taken, 1 not walkable (a building or water on the spot), 2 a building on the way, 3 a car on the way
};

// A slower car in the way along the line to the aim point (a queue, a car pottering along the lane): an aim point
// beside it and a little past it, on the pavement or in the next lane, whichever has room, so that the car goes round
// instead of following. false when nothing blocks the way or no side has room
bool DriveSteer_Overtake(const SteerWorld& w, float x, float y, float speed, float aim_x, float aim_y, float* out_x, float* out_y, OvertakeTrace* trace);
