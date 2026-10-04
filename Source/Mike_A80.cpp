#include "Mike_A80.hpp"
#include "Globals.hpp"
#include "Draw.hpp"
#include "gbh_graphics.hpp"
#include "Frontend.hpp"
#include "fix16.hpp"
#include "gtx_0x106C.hpp"
#include <stdio.h>
#include <stdarg.h>
#include <windows.h>
#include <mmsystem.h>

DEFINE_GLOBAL(Mike_A80*, gMike_A80_6F7328, 0x6F7328);

MATCH_FUNC(0x4ff1b0)
void Mike_A80::Init_4FF1B0()
{
    field_A78_ary_idx = 0;
    field_A7C_count = 0;
}

MATCH_FUNC(0x4ff1c0)
s32 Mike_A80::sDrawFlatRect_4FF1C0(f32 left, f32 top, f32 right, f32 bottom, s32 colour)
{
    Vert verts[4];
    verts[0].x = left;
    verts[0].y = top;
    verts[0].z = 0.0f;
    verts[1].x = right;
    verts[1].y = top;
    verts[1].z = 0.0f;
    verts[2].x = right;
    verts[2].y = bottom;
    verts[2].z = 0.0f;
    verts[3].x = left;
    verts[3].y = bottom;
    verts[3].z = 0.0f;
    return pgbh_DrawFlatRect(verts, colour);
}

EXTERN_GLOBAL(u32, gBufferMode_706B34);
EXTERN_GLOBAL(s32, gDisplayDraw_67B57C);
EXTERN_GLOBAL(s32, gDisplayAdd_67B578);
EXPORT s32 __stdcall sub_5BEED0(s32 cycles);

DEFINE_GLOBAL(u32, gMemTotal_6F7360, 0x6F7360);
DEFINE_GLOBAL(s32, gMemTotalCounter_6F7550, 0x6F7550);
DEFINE_GLOBAL(s32, gLargeFrameTimer_6F754C, 0x6F754C);

MATCH_FUNC(0x4ff250)
void Mike_A80::DebugDrawProfiling_4FF250()
{
    s32 total_textures_used = 0;
    s32 total_texture_bytes = 0;
    s32 tex_counts[12];
    s32 cache_used[12];

    if (gBufferMode_706B34 == 0)
    {
        return;
    }

    u32* pGlobals = pgbh_GetGlobals();
    s32 total_tex_count = 0;
    s32 total_cache_used = 0;
    for (s32 i = 0; i < 12; i++)
    {
        tex_counts[i] = pGlobals[28 + i];
        cache_used[i] = pgbh_GetUsedCache(i);
        total_tex_count += tex_counts[i];
        total_cache_used += cache_used[i];
    }

    const s32& polys_drawn = pGlobals[0];
    s32 texture_swaps = pGlobals[1];

    s32 row = 0;
    for (s32 ypos = 20; ypos < 260; ypos += 20)
    {
        s32 size = 8 << (ypos < 140 ? row : row - 6);

        swprintf(tmpBuff_67BD9C, L"%d", size);
        DrawText_4B87A0(tmpBuff_67BD9C, 0, ypos, word_703BAA, 1);
        swprintf(tmpBuff_67BD9C, L"%d", pGlobals[4 + row]);
        DrawText_4B87A0(tmpBuff_67BD9C, 50, ypos, word_703BAA, 1);
        swprintf(tmpBuff_67BD9C, L"%d", pGlobals[16 + row]);
        DrawText_4B87A0(tmpBuff_67BD9C, 100, ypos, word_703BAA, 1);
        swprintf(tmpBuff_67BD9C, L"%d", tex_counts[row]);
        DrawText_4B87A0(tmpBuff_67BD9C, 150, ypos, word_703BAA, 1);
        swprintf(tmpBuff_67BD9C, L"%d", cache_used[row]);
        DrawText_4B87A0(tmpBuff_67BD9C, 200, ypos, word_703BAA, 1);

        total_textures_used += pGlobals[16 + row];
        total_texture_bytes += pGlobals[16 + row] * size * size * 2;
        row++;
    }

    sDrawString_4FF910(0, 280, L"Total Num Textures %d - %d bytes", total_textures_used, total_texture_bytes);

    swprintf(tmpBuff_67BD9C, L"Polys Drawn:");
    DrawText_4B87A0(tmpBuff_67BD9C, 0, 300, word_703BAA, 1);
    swprintf(tmpBuff_67BD9C, L"%d", polys_drawn);
    DrawText_4B87A0(tmpBuff_67BD9C, 180, 300, word_703BAA, 1);

    swprintf(tmpBuff_67BD9C, L"TextureUsed: ");
    DrawText_4B87A0(tmpBuff_67BD9C, 0, 320, word_703BAA, 1);
    swprintf(tmpBuff_67BD9C, L"%d", total_cache_used);
    DrawText_4B87A0(tmpBuff_67BD9C, 180, 320, word_703BAA, 1);

    swprintf(tmpBuff_67BD9C, L"Cache Misses:");
    DrawText_4B87A0(tmpBuff_67BD9C, 0, 340, word_703BAA, 1);
    swprintf(tmpBuff_67BD9C, L"%d", total_tex_count);
    DrawText_4B87A0(tmpBuff_67BD9C, 180, 340, word_703BAA, 1);

    swprintf(tmpBuff_67BD9C, L"TextureSwaps:");
    DrawText_4B87A0(tmpBuff_67BD9C, 0, 360, word_703BAA, 1);
    swprintf(tmpBuff_67BD9C, L"%d", texture_swaps);
    DrawText_4B87A0(tmpBuff_67BD9C, 180, 360, word_703BAA, 1);

    if (++gMemTotalCounter_6F7550 > 100)
    {
        sub_4FF970(&gMemTotal_6F7360);
        gMemTotalCounter_6F7550 = 0;
    }
    sDrawString_4FF910(0, 380, L"Mem Total %5d", gMemTotal_6F7360);

    s32 total = field_28_m80_1.Average() + field_28_m80_2.Average() + field_28_m80_3.Average() +
        field_28_m80_5.Average() + field_28_m80_4.Average();
    sDrawString_4FF910(280, 20, L"Process %3d", field_28_m80_1.Average());
    sDrawString_4FF910(280, 40, L"Draw    %3d", field_28_m80_2.Average());
    sDrawString_4FF910(280, 60, L"Render  %3d", field_28_m80_3.Average());
    sDrawString_4FF910(280, 80, L"Audio   %3d", field_28_m80_5.Average());
    sDrawString_4FF910(280, 100, L"Input   %3d", field_28_m80_4.Average());
    sDrawString_4FF910(280, 120, L"Total   %3d", total);

    s32 large_timer = total > 30 ? 15 : gLargeFrameTimer_6F754C;
    if (large_timer)
    {
        gLargeFrameTimer_6F754C = large_timer - 1;
        sDrawString_4FF910(280, 140, L"LARGE");
    }

    s32 display_add = sub_5BEED0(gDisplayAdd_67B578);
    s32 display_draw = sub_5BEED0(gDisplayDraw_67B57C);
    // TODO: the format strings at 0x621100 and 0x6210DC are guesses
    sDrawString_4FF910(280, 160, L"DisplayAdd  %d", display_add);
    sDrawString_4FF910(280, 180, L"DisplayDraw %d", display_draw);
    gDisplayAdd_67B578 = 0;
    gDisplayDraw_67B57C = 0;

    sub_4FFD90();
}

MATCH_FUNC(0x4ff910)
void Mike_A80::sDrawString_4FF910(s32 xpos, s32 ypos, const wchar_t* pFormat, ...)
{
    wchar_t buffer[240];
    va_list args;
    va_start(args, pFormat);
    vswprintf(buffer, pFormat, args);
    DrawText_4B87A0(buffer, xpos, ypos, word_703BAA, 1);
}

MATCH_FUNC(0x4ff970)
void __stdcall Mike_A80::sub_4FF970(u32* a1)
{
    *a1 = 0;
}

MATCH_FUNC(0x4ff980)
void Mike_A80::sub_4FF980()
{
}

MATCH_FUNC(0x4ff990)
void Mike_A80::sub_4FF990(u32 idx)
{
    switch (idx)
    {
        case 0:
            m81.field_0 = timeGetTime();
            break;
        case 1:
            m82.field_0 = timeGetTime();
            break;
        case 2:
            m84.field_0 = timeGetTime();
            break;
        case 3:
            m85.field_0 = timeGetTime();
            break;
    }
}

MATCH_FUNC(0x4ff9f0)
void Mike_A80::sub_4FF9F0(u32 idx)
{
    switch (idx)
    {
        case 0:
            m81.field_4 = timeGetTime();
            break;
        case 1:
            m82.field_4 = timeGetTime();
            break;
        case 2:
            m84.field_4 = timeGetTime();
            break;
        case 3:
            m85.field_4 = timeGetTime();
            break;
    }
}

MATCH_FUNC(0x4ffa50)
void Mike_A80::sub_4FFA50(const char_type* pFormat, ...)
{
    char_type buffer[240];
    va_list args;
    va_start(args, pFormat);
    vsprintf(buffer, pFormat, args);
    OutputDebugStringA(buffer);
}

MATCH_FUNC(0x4ffa90)
void Mike_A80::sub_4FFA90()
{
    s32 process_time = m81.TakeElapsed();
    s32 draw_time = m82.TakeElapsed();
    s32 input_time = m84.TakeElapsed();
    s32 audio_time = m85.TakeElapsed();

    if (process_time == 0 && draw_time == 0 && input_time == 0 && audio_time == 0)
    {
        return;
    }

    s32 flip_time = pgbh_GetGlobals()[3];
    process_time -= audio_time;
    if (draw_time > 0)
    {
        draw_time -= flip_time;
    }

    field_2A8_ary[field_A78_ary_idx] = process_time;
    field_438_ary[field_A78_ary_idx] = draw_time;
    field_5C8_ary[field_A78_ary_idx] = flip_time;
    field_758_ary[field_A78_ary_idx] = input_time;
    field_8E8_ary[field_A78_ary_idx] = audio_time;

    char_type buffer[240];
    if (process_time)
    {
        field_28_m80_1.AddSample(process_time);
    }
    if (process_time > 15)
    {
        wsprintfA(buffer, "LARGE PROCESS %d\n", process_time);
        OutputDebugStringA(buffer);
    }

    if (draw_time)
    {
        field_28_m80_2.AddSample(draw_time);
    }
    if (draw_time > 15)
    {
        wsprintfA(buffer, "LARGE DRAW %d\n", draw_time);
        OutputDebugStringA(buffer);
    }

    if (flip_time)
    {
        field_28_m80_3.AddSample(flip_time);
    }

    if (input_time)
    {
        field_28_m80_4.AddSample(input_time);
    }
    if (input_time > 10)
    {
        wsprintfA(buffer, "LARGE INPUT %d\n", input_time);
        OutputDebugStringA(buffer);
    }

    if (audio_time)
    {
        field_28_m80_5.AddSample(audio_time);
    }
    // Checks input_time rather than audio_time
    if (input_time > 10)
    {
        wsprintfA(buffer, "LARGE AUDIO %d\n", audio_time);
        OutputDebugStringA(buffer);
    }

    s32 total = input_time + audio_time + draw_time + process_time;
    if (total > 30)
    {
        wsprintfA(buffer, "LARGE EVERYTHING %d\n", total);
        OutputDebugStringA(buffer);
    }

    if (++field_A78_ary_idx >= 100)
    {
        field_A78_ary_idx = 0;
    }
    field_A7C_count++;
}

MATCH_FUNC(0x4ffd90)
void Mike_A80::sub_4FFD90()
{
    s32 count = field_A7C_count < 100 ? field_A7C_count : 100;

    for (s32 i = 0; i < count; i++)
    {
        s32 idx = field_A78_ary_idx - i - 1;
        if (idx < 0)
        {
            idx += 100;
        }

        DrawProfileBar(6 * i, field_2A8_ary[idx], 0xFF00);
        DrawProfileBar(6 * i + 1, field_438_ary[idx], 0xFF0000);
        DrawProfileBar(6 * i + 2, field_5C8_ary[idx], 0xFF);
        DrawProfileBar(6 * i + 3, field_758_ary[idx], 0);
        // Draws field_758_ary again rather than field_8E8_ary
        DrawProfileBar(6 * i + 4, field_758_ary[idx], 0xFFFFFF);
    }
}