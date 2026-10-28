#pragma once

// The driver's choice of wheel position for the next moments, as a pure function of the world (a callback that says
// where the solid ground is, and the cars about), so that it runs without the game: Scripts/bot/sim/ tests it on
// fake and real maps natively.

struct SteerCar
{
    float x, y;
    float vx, vy; // tiles per frame
    float speed;
    bool police;
};

struct SteerWorld
{
    bool (*solid)(void* user, float x, float y); // a building, water or the edge of the map
    void* user;
    const SteerCar* cars; // not including our own
    int car_count;
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
SteerChoice DriveSteer_Choose(const SteerWorld& w, float x, float y, float heading_deg, float hdir, float speed, float aim_x, float aim_y, float prev_s);

// The speed (tiles per frame) that still stops the car before a wall that far ahead
float DriveSteer_SpeedFor(float wall_d);

// A slower car in the way along the line to the aim point (a queue, a car pottering along the lane): an aim point
// beside it and a little past it, on the pavement or in the next lane, whichever has room, so that the car goes round
// instead of following. false when nothing blocks the way or no side has room
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
bool DriveSteer_Overtake(const SteerWorld& w, float x, float y, float speed, float aim_x, float aim_y, float* out_x, float* out_y, OvertakeTrace* trace);
