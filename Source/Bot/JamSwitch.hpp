#pragma once

// Stuck in stopped traffic: a car close ahead that is faster and less damaged than ours is worth swapping for (the jam is made
// of them, and a car is nearly always better than the walk). A pure function (no game types), tested natively in
// Scripts/bot/sim/jam_switch_sim.cpp.

struct JamSelf
{
    int damage;
    float top_speed; // the model's top speed (game units, only to compare)
    int still_frames; // how long we have stood in the jam
};

struct JamCar
{
    float along; // tiles ahead along our heading (negative: behind)
    float side;  // tiles to the side
    int damage;
    float top_speed;
    bool has_driver;
    bool locked;
    bool mission;
    bool police;
};

// The index of the car to take instead of ours, or -1 to stay with ours
int JamSwitch_Choose(const JamSelf& self, const JamCar* cars, int n);
