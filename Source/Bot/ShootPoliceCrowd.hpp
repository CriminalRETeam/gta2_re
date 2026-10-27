#pragma once

// Against a crowd of police on foot the best defence is to keep facing them, shoot, and walk backwards away from the
// crowd. A pure function (no game types), tested natively in Scripts/bot/sim/shoot_police_crowd_sim.cpp.

struct CrowdCop
{
    float x, y;
};

struct CrowdWorld
{
    float x, y;           // the player
    float face_x, face_y; // unit vector the player faces
    const CrowdCop* cops;  // police on foot, alive, near
    int cop_count;
    bool (*solid)(void* user, float x, float y);
    void* user;
};

struct CrowdOut
{
    bool active;
    float target_x, target_y; // the cop to face and shoot (the nearest)
    int move;                 // -1 walk backwards, +1 forward, 0 stand
};

// How many police make a crowd
enum
{
    kCrowdSize = 3
};

CrowdOut ShootPoliceCrowd_Decide(const CrowdWorld& w);
