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
    s32 save_at_frame; // -1 = never
    bool has_map_dump;
    float map_dump_x, map_dump_y;
    bool has_goto;
    float goto_x, goto_y;
};

// Returns false (and leaves *out zeroed except defaults) when the command line has no -bot option
bool BotOptions_Parse(const char* cmd_line, BotOptions* out);
