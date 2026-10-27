#pragma once

#include "types.hpp"
#include <windows.h>

// Command line of the bot, parsed by the entry point before WinMain_5E53F0 (the game's own
// ParseCommandLine_4DA320 only knows -R, -Q and -N and upper cases the line in place).
//
//   -bot[=<profile>]      idle (default), walk_test, restore_check, replay
//   -bot_seed=N           BotRng seed
//   -bot_map=wil          data\wil.gmp, data\wil.sty, data\wil.scr (a value with a '.' is a file name)
//   -bot_style=<file> -bot_script=<file>
//   -bot_out=<dir>        logs and reports (default bot_out)
//   -bot_budget=N         exit after N logic frames
//   -bot_load=<snap>      restore this snapshot at the first running frame (same build and map)
//   -bot_save_at=N        save bot_save.snap at run frame N (a -bot_load test)
//   -bot_hash_log         snapshot hash per frame to bot_hash.txt
//   -bot_nav_dump         text dump of the block grid around the player (phase 2)
//   -bot_draw / -bot_audio  keep drawing / sound (default: both skipped)
//   -bot_warmup=N -bot_frames=N  restore_check: frames before the snapshot, frames per run
//   -bot_walk=N           walk_test: frames to hold forward
//   -bot_pad=xxx...       ignored; Scripts/bot pads the command line to a fixed length so that
//                         the CRT's start up allocations, and so the heap layout, are the same
//   -bot_goto=x,y         walk_arrow: target when the HUD arrow shows nothing (tiles)
//   -bot_map_dump=x,y     text map of the blocks around x,y (road, pavement, buildings, arrows, cars) at the first frame
//   -bot_gang=N -bot_kills=K   kill_gang profile: kill K peds of gang N (Gang::field_1_gang_idx)
//   -bot_fast             with -bot_draw: do not wait for the 33 ms frame clock (run.py --show sets it)
//   -bot_trains_log       log every train car's position every 60 frames
//   -bot_script_dump      list every script command (index, name, next, raw words) to bot_script.txt at the first frame
//   -bot_zones_dump       list every map zone (type, rectangle, name) to bot_zones.txt at the first frame
//   -bot_miss_log         let the game write test\MISS_LOG.TXT (script command offsets run per frame, per thread)
//   -bot_objects_dump     list every world object (model, position) to bot_objects.txt at the first frame
//   -bot_tokens=N -bot_frenzies=N -bot_missions=N   collect profile goals (defaults 3, 2, 0)
//   -bot_frenzy_id=N      collect: only this kill frenzy icon (object id from bot_objects.txt), up to 4 times
//   -bot_skip_phone       phone_test: only the mission loop (after -bot_load of a snapshot taken during a mission)
//   -bot_perturb          restore_check: change the player's health after the restore, so the
//                         check must FAIL and name the ped pool block (verifier sanity check)
struct BotOptions
{
    bool enabled;
    char profile[64];
    u32 seed;
    char map[64];
    char style[64];
    char script[64];
    char out_dir[MAX_PATH];
    s32 budget_frames;
    char load_snap[MAX_PATH];
    bool hash_log;
    bool nav_dump;
    bool draw;
    bool audio;
    s32 warmup_frames;
    s32 check_frames;
    s32 walk_frames;
    bool perturb;
    s32 save_at_frame;
    char cmdline[1024]; // the command line as it was given (the game upper cases its own copy later)
    s32 wtf_at_frame; // presses the overlay's WTF button at this frame (for testing) // -1 = never
    bool has_map_dump;
    float map_dump_x, map_dump_y;
    s32 only_frenzy_ids[4]; // collect: use only these kill frenzy icons (object ids), none = any
    s32 only_frenzy_count;
    s32 phone_id;
    bool skip_phone; // phone_test: the mission is already running (resume from a -bot_load snapshot)
    s32 fire_mode;      // 0 pulse 6/2, 1 hold, 2 pulse 2/2, 3 pulse 1/1
    s32 want_tokens;
    s32 want_frenzies;
    s32 want_missions;
    bool objects_dump;
    s32 gang;
    s32 kills;
    bool fast; // with drawing: let the logic run ahead of the clock (8 logic frames per drawn one)
    bool trains_log;
    bool script_dump;
    bool zones_dump;
    bool miss_log;
    bool has_goto;
    float goto_x, goto_y;
};

// Returns false (and leaves *out zeroed except defaults) when the command line has no -bot option
bool BotOptions_Parse(const char* cmd_line, BotOptions* out);
