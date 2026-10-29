#pragma once

// Which cars may be walked to and taken, and how. The rules that were inline in Cars_Pick, as pure functions (no game types) so
// that Scripts/bot/sim/car_pick_sim.cpp pins them down: a rule that quietly refuses every car (moving traffic was "never
// boardable", and on a road full of it the trip went on foot) shows up there.

struct CarPickPlayer
{
    float x, y;
    int wanted_stars;
    bool cops_near;
};

struct CarPickCar
{
    float x, y;
    float vx, vy; // tiles per frame
    float speed;
    float dist; // from the player
    int damage;
    bool has_driver;
    bool locked;
    bool police;
    bool mission;
    bool bad_model; // a train, a tank, a hearse ...: not for getting about
    bool ours;      // the car the player drove last: a mission's car is no use to take, but the one we came in is the way on
};

enum
{
    kCarPickNo = 0,
    kCarPickParked = 1,    // empty: walk up and get in
    kCarPickStopped = 2,   // a driver in it, at rest (a light, a queue): right beside us or not at all
    kCarPickIntercept = 3  // a driver in it, moving: get ahead of it on its line, where it stops for a pedestrian
};

// What kind of pick the car is, before any route is asked for. For an intercept, (*stand_x, *stand_y) is where to stand; otherwise
// it is the car's own place
int CarPick_Kind(const CarPickPlayer& p, const CarPickCar& c, float max_dist, float* stand_x, float* stand_y);

// With the walking route to the car (or the stand spot) known: is the pick worth it. walk_len: tiles of that route; stand_to_car: for an
// intercept, tiles from the car to the spot
bool CarPick_WalkOk(int kind, const CarPickCar& c, float walk_len, float stand_to_car);
