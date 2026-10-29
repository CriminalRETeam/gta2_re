#include "BotOptions.hpp"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "BotCompat.hpp"

static void CopyStr(char* dst, u32 dst_size, const char* src, u32 len)
{
    if (len >= dst_size)
    {
        len = dst_size - 1;
    }
    memcpy(dst, src, len);
    dst[len] = 0;
}

static void SetDefaults(BotOptions* o)
{
    memset(o, 0, sizeof(*o));
    strcpy(o->profile, "idle");
    o->seed = 1;
    strcpy(o->map, "wil.gmp");
    strcpy(o->style, "wil.sty");
    strcpy(o->script, "wil.scr");
    strcpy(o->out_dir, "bot_out");
    o->budget_frames = 0;
    o->warmup_frames = 100;
    o->check_frames = 300;
    o->walk_frames = 100;
    o->save_at_frame = -1;
    o->wtf_at_frame = -1;
    o->pause_at_frame = -1;
    o->want_tokens = 3;
    o->want_frenzies = 2;
    o->want_missions = 0;
    o->gang = 0;
    o->kills = 10;
}

bool BotOptions_Parse(const char* cmd_line, BotOptions* o)
{
    SetDefaults(o);
    if (!cmd_line)
    {
        return false;
    }

    CopyStr(o->cmdline, sizeof(o->cmdline), cmd_line, static_cast<u32>(strlen(cmd_line)));
    const char* p = cmd_line;
    while (*p)
    {
        while (*p == ' ' || *p == '\t')
        {
            p++;
        }
        if (!*p)
        {
            break;
        }
        const char* tok = p;
        while (*p && *p != ' ' && *p != '\t')
        {
            p++;
        }
        const u32 tok_len = p - tok;

        if (tok_len < 4 || (tok[0] != '-' && tok[0] != '/') || _strnicmp(tok + 1, "bot", 3) != 0)
        {
            continue;
        }

        // key = text after "-bot" up to '=', value = text after '='
        const char* key = tok + 4;
        const char* key_end = key;
        while (key_end < p && *key_end != '=')
        {
            key_end++;
        }
        const char* value = key_end < p ? key_end + 1 : p;
        const u32 value_len = p - value;
        const u32 key_len = key_end - key;

        o->enabled = true;
        if (key_len == 0)
        {
            if (value_len)
            {
                CopyStr(o->profile, sizeof(o->profile), value, value_len);
            }
        }
        else if (_strnicmp(key, "_seed", key_len) == 0 && key_len == 5)
        {
            o->seed = strtoul(value, NULL, 0);
        }
        else if (_strnicmp(key, "_map", key_len) == 0 && key_len == 4)
        {
            if (memchr(value, '.', value_len))
            {
                CopyStr(o->map, sizeof(o->map), value, value_len);
            }
            else
            {
                char base[32];
                CopyStr(base, sizeof(base), value, value_len);
                _snprintf(o->map, sizeof(o->map), "%s.gmp", base);
                _snprintf(o->style, sizeof(o->style), "%s.sty", base);
                _snprintf(o->script, sizeof(o->script), "%s.scr", base);
            }
        }
        else if (_strnicmp(key, "_style", key_len) == 0 && key_len == 6)
        {
            CopyStr(o->style, sizeof(o->style), value, value_len);
        }
        else if (_strnicmp(key, "_script", key_len) == 0 && key_len == 7)
        {
            CopyStr(o->script, sizeof(o->script), value, value_len);
        }
        else if (_strnicmp(key, "_out", key_len) == 0 && key_len == 4)
        {
            CopyStr(o->out_dir, sizeof(o->out_dir), value, value_len);
        }
        else if (_strnicmp(key, "_budget", key_len) == 0 && key_len == 7)
        {
            o->budget_frames = atoi(value);
        }
        else if (_strnicmp(key, "_load", key_len) == 0 && key_len == 5)
        {
            CopyStr(o->load_snap, sizeof(o->load_snap), value, value_len);
        }
        else if (_strnicmp(key, "_hash_log", key_len) == 0 && key_len == 9)
        {
            o->hash_log = true;
        }
        else if (_strnicmp(key, "_nav_dump", key_len) == 0 && key_len == 9)
        {
            o->nav_dump = true;
        }
        else if (_strnicmp(key, "_no_fine_plan", key_len) == 0 && key_len == 13)
        {
            o->no_fine_plan = true;
        }
        else if (_strnicmp(key, "_drive_pure", key_len) == 0 && key_len == 11)
        {
            o->drive_pure = true;
        }
        else if (_strnicmp(key, "_draw", key_len) == 0 && key_len == 5)
        {
            o->draw = true;
        }
        else if (_strnicmp(key, "_audio", key_len) == 0 && key_len == 6)
        {
            o->audio = true;
        }
        else if (_strnicmp(key, "_warmup", key_len) == 0 && key_len == 7)
        {
            o->warmup_frames = atoi(value);
        }
        else if (_strnicmp(key, "_frames", key_len) == 0 && key_len == 7)
        {
            o->check_frames = atoi(value);
        }
        else if (_strnicmp(key, "_walk", key_len) == 0 && key_len == 5)
        {
            o->walk_frames = atoi(value);
        }
        else if (_strnicmp(key, "_perturb", key_len) == 0 && key_len == 8)
        {
            o->perturb = true;
        }
        else if (_strnicmp(key, "_map_dump", key_len) == 0 && key_len == 9)
        {
            o->has_map_dump = sscanf(value, "%f,%f", &o->map_dump_x, &o->map_dump_y) == 2;
        }
        else if (_strnicmp(key, "_frenzy_id", key_len) == 0 && key_len == 10)
        {
            if (o->only_frenzy_count < 4)
            {
                o->only_frenzy_ids[o->only_frenzy_count++] = atoi(value);
            }
        }
        else if (_strnicmp(key, "_skip_phone", key_len) == 0 && key_len == 11)
        {
            o->skip_phone = true;
        }
        else if (_strnicmp(key, "_phone_id", key_len) == 0 && key_len == 9)
        {
            o->phone_id = atoi(value);
        }
        else if (_strnicmp(key, "_fire_mode", key_len) == 0 && key_len == 10)
        {
            o->fire_mode = atoi(value);
        }
        else if (_strnicmp(key, "_tokens", key_len) == 0 && key_len == 7)
        {
            o->want_tokens = atoi(value);
        }
        else if (_strnicmp(key, "_frenzies", key_len) == 0 && key_len == 9)
        {
            o->want_frenzies = atoi(value);
        }
        else if (_strnicmp(key, "_gang", key_len) == 0 && key_len == 5)
        {
            o->gang = atoi(value);
        }
        else if (_strnicmp(key, "_kills", key_len) == 0 && key_len == 6)
        {
            o->kills = atoi(value);
        }
        else if (_strnicmp(key, "_missions", key_len) == 0 && key_len == 9)
        {
            o->want_missions = atoi(value);
        }
        else if (_strnicmp(key, "_script_dump", key_len) == 0 && key_len == 12)
        {
            o->script_dump = true;
        }
        else if (_strnicmp(key, "_trains_log", key_len) == 0 && key_len == 11)
        {
            o->trains_log = true;
        }
        else if (_strnicmp(key, "_fast", key_len) == 0 && key_len == 5)
        {
            o->fast = true;
        }
        else if (_strnicmp(key, "_zones_dump", key_len) == 0 && key_len == 11)
        {
            o->zones_dump = true;
        }
        else if (_strnicmp(key, "_miss_log", key_len) == 0 && key_len == 9)
        {
            o->miss_log = true;
        }
        else if (_strnicmp(key, "_objects_dump", key_len) == 0 && key_len == 13)
        {
            o->objects_dump = true;
        }
        else if (_strnicmp(key, "_goto", key_len) == 0 && key_len == 5)
        {
            o->has_goto = sscanf(value, "%f,%f", &o->goto_x, &o->goto_y) == 2;
        }
        else if (_strnicmp(key, "_pause_route", key_len) == 0 && key_len == 12)
        {
            o->pause_route = true;
        }
        else if (_strnicmp(key, "_pause_at", key_len) == 0 && key_len == 9)
        {
            o->pause_at_frame = atoi(value);
        }
        else if (_strnicmp(key, "_wtf_at", key_len) == 0 && key_len == 7)
        {
            o->wtf_at_frame = atoi(value);
        }
        else if (_strnicmp(key, "_save_at", key_len) == 0 && key_len == 8)
        {
            o->save_at_frame = atoi(value);
        }
    }
    return o->enabled;
}
