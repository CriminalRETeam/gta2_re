#include "BotOptions.hpp"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

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
}

bool BotOptions_Parse(const char* cmd_line, BotOptions* o)
{
    SetDefaults(o);
    if (!cmd_line)
    {
        return false;
    }

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
    }
    return o->enabled;
}
