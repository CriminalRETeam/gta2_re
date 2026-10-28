#pragma once

// Staying out of the way of cars that are about to blow up. A car burns from 16000 damage, burns harder from 25000 and
// explodes at 32000 (Car::AddDamage); the explosion reaches 1 tile (Explosion::GetBlastRadius, type 19), measured from
// the car's centre to the centre of what it hits. So a burning car is a hazard with a known radius, and the player can
// keep burning it until it is nearly gone, step out to the stand-off distance, and go on to something else.
// A pure function (no game types), tested natively in Scripts/bot/sim/blast_avoid_sim.cpp.

struct BlastHazard
{
    float x, y;
    int damage;
};

struct BlastWorld
{
    float x, y; // the player
    const BlastHazard* hazards;
    int count;
    bool (*solid)(void* user, float x, float y);
    void* user;
};

struct BlastOut
{
    bool active;
    float aim_x, aim_y; // the nearest safe spot
};

enum
{
    kBlastHotDamage = 25000,  // burning hard: it will go off before long
    kBlastGoneDamage = 32000  // goes off at this damage
};

const float kBlastRadius = 1.0f;   // what an exploding car hurts, centre to centre
const float kBlastStandOff = 2.0f; // where the player stands: the radius plus the player and a car's half length

// Will this car blow up on its own soon
bool BlastAvoid_IsHazard(int damage);

// Is (x, y) inside a hazard's stand-off distance
bool BlastAvoid_Unsafe(const BlastWorld& w, float x, float y);

// Inside a stand-off distance: the nearest spot outside every one, with a clear way there
BlastOut BlastAvoid_Decide(const BlastWorld& w);

// Cars (not hazards themselves) near enough to a hazard to go up with it: they need no shooting
bool BlastAvoid_WillChain(const BlastWorld& w, float x, float y);
