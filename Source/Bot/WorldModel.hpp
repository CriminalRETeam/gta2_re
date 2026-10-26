#pragma once

#include "types.hpp"
#include "BotHeap.hpp"

// What the bot knows about the game right now, read from the game's own pools and counters
// (docs/bot_worldmodel_fields.md). Observe() copies plain numbers into this object, so nothing in
// it points into game memory and it survives a Snapshot restore (it lives in the bot heap).
// Positions are in tiles (Fix16 / 16384 as float).
class WorldModel : public BotObject
{
  public:
    enum
    {
        kMaxPeds = 10,
        kMaxCars = 8,
        kMaxGangs = 10,
        kMaxPlayers = 1
    };

    struct PlayerInfo
    {
        bool valid;
        float x, y, z;
        float rot_deg; // 0 = +Y.. see Bot walk controller, measured not assumed
        s32 hp;
        s32 lives;
        s32 cash;
        s32 wanted_stars;
        bool in_car;
        bool dead; // wasted or busted countdown running
        s32 weapon;
        s32 ammo;
    };

    struct ArrowInfo
    {
        bool valid;
        s32 type; // ArrowTargetType
        s32 colour;
        float x, y, z;
        s32 ped_id;    // -1 if not a ped
        s32 car_idx;   // pool index, -1 if not a car
    };

    struct PedInfo
    {
        s32 id;
        s32 occupation;
        s32 objective;
        s32 hp;
        float x, y;
        float dist;
        bool cop;
        bool in_car;
        bool dead;
    };

    struct CarInfo
    {
        s32 idx;
        s32 model;
        s32 damage;
        float x, y;
        float dist;
        bool has_driver;
        bool locked;
        bool mission;
        bool police;
    };

    WorldModel();

    void Observe();

    // The cheap view: under 40 lines, integers and short names
    // Returns the number of chars written
    u32 Text(char* out, u32 out_size, const char* header) const;

    s32 frame;
    s32 rng;
    PlayerInfo player;
    ArrowInfo arrow;
    bool on_mission;
    s32 missions_passed;
    s32 phones_ringing;
    char brief_key[12];
    s32 peds_total;
    s32 cops_total;
    s32 cars_total;
    s32 ped_count;
    PedInfo peds[kMaxPeds];
    s32 car_count;
    CarInfo cars[kMaxCars];
    s32 respect[kMaxGangs];
    char gang_names[kMaxGangs][5];
    s32 gang_count;
    bool exit_pending;
    s32 exit_type;
    s32 finish_score;
};

const char* WorldModel_OccupationName(s32 occupation);
