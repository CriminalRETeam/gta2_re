#pragma once

// How close a weapon has to be to its target for a kill frenzy, and when it may fire. A flamethrower's flame is short: shooting at a
// pedestrian further off than its tail just warns him, and he runs. A pure function (no game types), tested natively in
// Scripts/bot/sim/frenzy_range_sim.cpp.

struct FrenzyRange
{
    float min_d;      // closer than this the bot backs off
    float max_d;      // further than this it walks up
    float fire_slack; // fire up to max_d + this
    float fire_angle; // degrees off the target the weapon may be and still fire
};

// weapon: the game's weapon type; thrown/explosive weapons need room, flames and shocks closeness
FrenzyRange FrenzyRange_Of(int weapon, bool car_mode, int retries);

bool FrenzyRange_FireOk(const FrenzyRange& r, float err_deg, float target_dist, bool clear_shot);
