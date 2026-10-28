#pragma once

// Is a pickup (a weapon, health, armour, a secret token) worth going for. Not by the straight line to it: a pickup on a roof, behind a
// fence or across a river is "near" and miles away. By the walking route to it (the pathfinder the walk itself uses): the route has to
// exist, and be short for what it fetches, unless the pickup is needed, when a long walk is worth it.
// A pure function (no game types), tested natively in Scripts/bot/sim/pickup_reach_sim.cpp.

struct PickupReachIn
{
    float flat;       // straight line distance, tiles
    bool path_found;  // a walking route exists
    bool path_approx; // but only to somewhere near the pickup
    float path_len;   // tiles of that route
    bool needed;      // the pickup is needed (no gun, hurt), not only handy
};

const float kReachDetour = 2.5f;       // the route may be this many times the straight line ...
const float kReachDetourExtra = 6.0f;  // ... plus this many tiles
const float kReachNeededLen = 120.0f;  // a pickup that is needed is worth a walk this long

bool PickupReach_Ok(const PickupReachIn& in);
