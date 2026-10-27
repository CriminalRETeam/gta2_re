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
        s32 car_id; // 0 when not in a car
        s32 car_damage;
        s32 reload;
        s32 shot_fired;
        s32 armour;      // hits the armour power up still absorbs (10 when fresh)
        bool attack_pressed;
        bool ped_attacking;
        s32 ped_state;
        s32 objective;
        s32 internal_objective;
        bool busted;        // the arrest objective (54) is set: the police have the player
        s32 death_cause;    // Ped::field_290_death_cause, meaningful once dead
        s32 killer_occupation; // -1 when nobody is recorded
        bool killer_cop;
        float killer_dist;
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
        float x, y, z;
        float dist;
        bool cop;
        bool in_car;
        bool dead;
        s32 gang_idx;
        s32 remap;
        s32 state1;   // ped_state_1
        s32 state2;
        bool matches_bonus; // counts for the active bonus rule (kill frenzy)
    };

    // The first active bonus rule of the player: what a kill frenzy asks for
    struct BonusInfo
    {
        bool active;
        s32 event_type; // 0 ped killed, 1 car destroyed, 2 car hijacked
        s32 occupation;
        s32 gang_idx;
        s32 remap;
        s32 death_cause;
        s32 car_model;
        bool zone_limited;
        s32 target;
        s32 progress;
        bool cancel_on_mismatch;
    };

    struct CarInfo
    {
        bool exists;
        s32 idx;
        s32 model;
        s32 damage;
        float x, y;
        float dist;
        float speed; // tiles per frame
        float vx, vy; // movement since the last observation, tiles per frame (0 when not seen before)
        float top_speed; // the model's top speed (game units, only to compare models)
        bool has_driver;
        bool locked;
        bool mission;
        bool police;
        bool bulletproof; // car_flag::Bulletproof: guns do nothing to it
        bool rocketproof;
        bool flameproof;
    };

    struct ObjInfo
    {
        s32 id;
        s32 model;
        float x, y, z;
    };

    enum
    {
        kMaxObjs = 64
    };

    WorldModel();

    void Observe();

    // Text map of the blocks around (cx, cy): R road, P pavement, F field, B building / high ground,
    // ' ' nothing; arrows ^v<> where a green road arrow points; c a car, @ the player
    void DumpMap(const char* path, float cx, float cy, int radius) const;

    // Every block of the columns within 2 tiles of (cx, cy): the four faces, lid, arrows and slope, and the world
    // objects there. For finding out what a stuck car is pressing against.
    void DumpTiles(const char* path, int cx, int cy) const;

    // Every world object in the pool: id model x y z behaviour pickup
    void DumpObjects(const char* path) const;

    // Test aid: gives the player a weapon
    static void GiveWeapon(s32 weapon, s32 ammo);

    // True when the tile at (x, y) is a building / high ground
    static bool BuildingAt(float x, float y);

    // 0 air / none, 1 road, 2 pavement, 3 field; building tiles give 4
    static int TileType(int x, int y);

    // Scans the whole object pool
    bool FindObject(s32 object_id, ObjInfo* out) const;

    // A mission timer is running (a pager that counts down, outside a kill frenzy): speed matters more than manners
    bool TimeLimited() const;

    // A live cop on foot or a police car within radius tiles of the player
    bool CopsNear(float radius) const;

    // Scans the whole car pool (not only the nearest few kept in cars[])
    bool FindCar(s32 car_id, CarInfo* out) const;

    // Where every running script thread is: thread id, command offset, command name and its
    // condition result. Shows what a stalled mission is waiting for. Returns chars written.
    static u32 ScriptThreads(char* out, u32 out_size);

    // Every command of the loaded script: index, name, next index and the first raw words
    static void DumpScript(const char* path);

    // Every map zone: type, rectangle, name
    static void DumpZones(const char* path);

    // Every ped within radius tiles of the player: type, occupation, objective (and the internal one), state, car
    static u32 PedTable(char* out, u32 out_size, float radius);

    // The cheap view: under 40 lines, integers and short names
    // Returns the number of chars written
    u32 Text(char* out, u32 out_size, const char* header) const;

    s32 frame;
    s32 rng;
    // Where each car was at the last Observe (hashed by id), for its velocity
    enum
    {
        kPrevCars = 1024
    };
    s32 prev_car_id[kPrevCars];
    float prev_car_x[kPrevCars];
    float prev_car_y[kPrevCars];
    s32 prev_car_frame[kPrevCars];
    PlayerInfo player;
    ArrowInfo arrow;
    bool on_mission;
    s32 missions_passed;
    s32 phones_ringing;
    char brief_key[12];
    char brief_text[400];
    s32 peds_total;
    s32 cops_total;
    s32 cars_total;
    s32 ped_count;
    PedInfo peds[kMaxPeds];
    s32 car_count;
    CarInfo cars[kMaxCars];
    s32 respect[kMaxGangs];
    char gang_names[kMaxGangs][5];
    s32 gang_index[kMaxGangs]; // Gang::field_1_gang_idx of the same slot (PedInfo::gang_idx uses it)
    s32 gang_count;
    // Killing a ped of gang J changes the respect of gang i by kill_reaction[J][i] (Gang::field_122_gang_kill_reaction),
    // and of gang J itself by -kill_respect_change[J]
    s32 kill_reaction[kMaxGangs][kMaxGangs];
    s32 kill_respect_change[kMaxGangs];
    // Gang territory (map zones of the gang type), read once: the map does not change
    struct GangZone
    {
        s32 gang_idx;
        s32 x, y, w, h;
    };
    enum
    {
        kMaxGangZones = 48
    };
    // Train stations (map zones "trakNNplatform" and "trakNNstop"): where to wait for a train
    struct Station
    {
        s32 id;
        s32 px, py, pw, ph;   // platform rectangle
        float stop_x, stop_y; // where the lead car stops (tile centre)
        float board_x, board_y; // a platform spot beside the track
    };
    enum
    {
        kMaxStations = 12
    };
    s32 station_count;
    Station stations[kMaxStations];
    s32 gang_zone_count;
    GangZone gang_zones[kMaxGangZones];
    bool gang_zones_read;
    // Interesting world objects, nearest first within each kind
    s32 token_count;       // secret tokens still in the world
    s32 tokens_listed;     // how many of the nearest are in tokens[]
    ObjInfo tokens[kMaxObjs];
    s32 frenzy_count;
    ObjInfo frenzies[kMaxObjs];
    s32 phone_count;
    ObjInfo phones[kMaxObjs];
    // Pickups, nearest first: health (230), armour (231) and weapons (200..227, weapon type = model - 200)
    s32 pickup_count;
    ObjInfo pickups[kMaxObjs];
    s32 weapon_ammo[28];  // ammo per weapon type the player holds (0 = not held)
    bool kill_frenzy_active;
    s32 track_ped_id;     // set by an action: the ped to follow in tracked
    bool tracked_valid;
    PedInfo tracked;
    BonusInfo bonus;
    s32 secrets_passed;  // kill frenzies passed so far (the script's counter)
    s32 secrets_failed;
    s32 match_count;     // the nearest peds that match the bonus rule
    PedInfo match_peds[kMaxPeds];
    s32 pager_timer[4];   // HUD timers (frames, -1 off)
    s32 pager_value[4];   // HUD counter values, -1 none
    bool exit_pending;
    s32 exit_type;
    s32 finish_score;
};

const char* WorldModel_OccupationName(s32 occupation);
