#pragma once

// Against a crowd of police on foot the best defence is to keep facing them, shoot, and walk backwards away from the
// crowd. A pure function (no game types), tested natively in Scripts/bot/sim/kite_sim.cpp.

struct KiteCop
{
    float x, y;
};

struct KiteWorld
{
    float x, y;           // the player
    float face_x, face_y; // unit vector the player faces
    const KiteCop* cops;  // police on foot, alive, near
    int cop_count;
    bool (*solid)(void* user, float x, float y);
    void* user;
};

struct KiteOut
{
    bool active;
    float target_x, target_y; // the cop to face and shoot (the nearest)
    int move;                 // -1 walk backwards, +1 forward, 0 stand
};

// How many police make a crowd
enum
{
    kKiteCrowd = 3
};

KiteOut Kite_Decide(const KiteWorld& w);
