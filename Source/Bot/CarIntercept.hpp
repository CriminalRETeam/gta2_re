#pragma once

// A car with a driver that is moving cannot be caught by walking at it: it drives off. Traffic stops for a pedestrian standing
// in its lane, so the way to take it is to get ahead of it on its line and wait there. This picks the place: on the car's line,
// far enough ahead that we get there before the car does. A pure function (no game types), tested natively in
// Scripts/bot/sim/car_intercept_sim.cpp.

struct InterceptIn
{
    float car_x, car_y;
    float car_vx, car_vy; // tiles per frame
    float ped_x, ped_y;
    float ped_speed; // tiles per frame the walker covers
};

enum
{
    kInterceptStopped = 0, // the car is not moving: walk up to it
    kInterceptAhead = 1,   // stand at (*out_x, *out_y), on its line, and wait for it to stop
    kInterceptNever = 2    // it would be past us before we got there, whatever the spot
};

int CarIntercept_Plan(const InterceptIn& in, float* out_x, float* out_y);
