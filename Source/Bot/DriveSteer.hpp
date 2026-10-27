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
SteerChoice DriveSteer_Choose(const SteerWorld& w, float x, float y, float heading_deg, float hdir, float speed, float aim_x, float aim_y);

// The speed (tiles per frame) that still stops the car before a wall that far ahead
float DriveSteer_SpeedFor(float wall_d);
