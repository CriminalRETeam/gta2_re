#include "FrameProfiler.hpp"
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

DEFINE_GLOBAL(FrameProfiler*, gMike_A80, 0x6F7328);

MATCH_FUNC(0x4ff1b0)
void FrameProfiler::Init()
{
    mHistoryIdx = 0;
    mCount = 0;
}

MATCH_FUNC(0x4ff1c0)
s32 FrameProfiler::sDrawFlatRect(f32 left, f32 top, f32 right, f32 bottom, s32 colour)
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

EXTERN_GLOBAL(u32, gBufferMode);
EXTERN_GLOBAL(s32, gDisplayDraw);
EXTERN_GLOBAL(s32, gDisplayAdd);
EXPORT s32 __stdcall ConvertCycleCount(s32 cycles);

MATCH_FUNC(0x4ff250)
void FrameProfiler::DebugDrawProfiling()
{
    LOCAL_STATIC(s32, sLargeFrameTimer, 0x6F754C);
    LOCAL_STATIC(s32, sMemTotalCounter, 0x6F7550);
    LOCAL_STATIC(u32, sMemTotal, 0x6F7360);
    s32 total_textures_used = 0;
    s32 total_texture_bytes = 0;
    s32 tex_counts[12];
    s32 cache_used[12];

    if (gBufferMode == 0)
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

        swprintf(tmpBuff, L"%d", size);
        DrawStringDefaultPalette(tmpBuff, 0, ypos, gStyleTextFont, 1);
        swprintf(tmpBuff, L"%d", pGlobals[4 + row]);
        DrawStringDefaultPalette(tmpBuff, 50, ypos, gStyleTextFont, 1);
        swprintf(tmpBuff, L"%d", pGlobals[16 + row]);
        DrawStringDefaultPalette(tmpBuff, 100, ypos, gStyleTextFont, 1);
        swprintf(tmpBuff, L"%d", tex_counts[row]);
        DrawStringDefaultPalette(tmpBuff, 150, ypos, gStyleTextFont, 1);
        swprintf(tmpBuff, L"%d", cache_used[row]);
        DrawStringDefaultPalette(tmpBuff, 200, ypos, gStyleTextFont, 1);

        total_textures_used += pGlobals[16 + row];
        total_texture_bytes += pGlobals[16 + row] * size * size * 2;
        row++;
    }

    sDrawString(0, 280, L"Total Num Textures %d - %d bytes", total_textures_used, total_texture_bytes);

    swprintf(tmpBuff, L"Polys Drawn:");
    DrawStringDefaultPalette(tmpBuff, 0, 300, gStyleTextFont, 1);
    swprintf(tmpBuff, L"%d", polys_drawn);
    DrawStringDefaultPalette(tmpBuff, 180, 300, gStyleTextFont, 1);

    swprintf(tmpBuff, L"TextureUsed: ");
    DrawStringDefaultPalette(tmpBuff, 0, 320, gStyleTextFont, 1);
    swprintf(tmpBuff, L"%d", total_cache_used);
    DrawStringDefaultPalette(tmpBuff, 180, 320, gStyleTextFont, 1);

    swprintf(tmpBuff, L"Cache Misses:");
    DrawStringDefaultPalette(tmpBuff, 0, 340, gStyleTextFont, 1);
    swprintf(tmpBuff, L"%d", total_tex_count);
    DrawStringDefaultPalette(tmpBuff, 180, 340, gStyleTextFont, 1);

    swprintf(tmpBuff, L"TextureSwaps:");
    DrawStringDefaultPalette(tmpBuff, 0, 360, gStyleTextFont, 1);
    swprintf(tmpBuff, L"%d", texture_swaps);
    DrawStringDefaultPalette(tmpBuff, 180, 360, gStyleTextFont, 1);

    if (++sMemTotalCounter > 100)
    {
        GetMemTotal(&sMemTotal);
        sMemTotalCounter = 0;
    }
    sDrawString(0, 380, L"Mem Total %5d", sMemTotal);

    s32 total = mProcessAvg.Average() + mDrawAvg.Average() + mRenderAvg.Average() +
        mAudioAvg.Average() + mInputAvg.Average();
    sDrawString(280, 20, L"Process %3d", mProcessAvg.Average());
    sDrawString(280, 40, L"Draw    %3d", mDrawAvg.Average());
    sDrawString(280, 60, L"Render  %3d", mRenderAvg.Average());
    sDrawString(280, 80, L"Audio   %3d", mAudioAvg.Average());
    sDrawString(280, 100, L"Input   %3d", mInputAvg.Average());
    sDrawString(280, 120, L"Total   %3d", total);

    s32 large_timer = total > 30 ? 15 : sLargeFrameTimer;
    if (large_timer)
    {
        sLargeFrameTimer = large_timer - 1;
        sDrawString(280, 140, L"LARGE");
    }

    s32 display_add = ConvertCycleCount(gDisplayAdd);
    s32 display_draw = ConvertCycleCount(gDisplayDraw);
    sDrawString(280, 160, L"DisplayAdd   %3d", display_add);
    sDrawString(280, 180, L"DisplayDraw  %3d", display_draw);
    gDisplayAdd = 0;
    gDisplayDraw = 0;

    DrawProfileHistory();
}

MATCH_FUNC(0x4ff910)
void FrameProfiler::sDrawString(s32 xpos, s32 ypos, const wchar_t* pFormat, ...)
{
    wchar_t buffer[240];
    va_list args;
    va_start(args, pFormat);
    vswprintf(buffer, pFormat, args);
    DrawStringDefaultPalette(buffer, xpos, ypos, gStyleTextFont, 1);
}

MATCH_FUNC(0x4ff970)
void __stdcall FrameProfiler::GetMemTotal(u32* pValue)
{
    *pValue = 0;
}

MATCH_FUNC(0x4ff980)
void FrameProfiler::Service()
{
}

MATCH_FUNC(0x4ff990)
void FrameProfiler::StartProfileTimer(u32 idx)
{
    switch (idx)
    {
        case 0:
            mProcessWatch.mStartTime = GetTickCount();
            break;
        case 1:
            mDrawWatch.mStartTime = GetTickCount();
            break;
        case 2:
            mInputWatch.mStartTime = GetTickCount();
            break;
        case 3:
            mAudioWatch.mStartTime = GetTickCount();
            break;
    }
}

MATCH_FUNC(0x4ff9f0)
void FrameProfiler::StopProfileTimer(u32 idx)
{
    switch (idx)
    {
        case 0:
            mProcessWatch.mEndTime = GetTickCount();
            break;
        case 1:
            mDrawWatch.mEndTime = GetTickCount();
            break;
        case 2:
            mInputWatch.mEndTime = GetTickCount();
            break;
        case 3:
            mAudioWatch.mEndTime = GetTickCount();
            break;
    }
}

MATCH_FUNC(0x4ffa50)
void FrameProfiler::DebugPrintf(const char_type* pFormat, ...)
{
    char_type buffer[240];
    va_list args;
    va_start(args, pFormat);
    vsprintf(buffer, pFormat, args);
    OutputDebugStringA(buffer);
}

MATCH_FUNC(0x4ffa90)
void FrameProfiler::UpdateProfiling()
{
    s32 process_time = mProcessWatch.TakeElapsed();
    s32 draw_time = mDrawWatch.TakeElapsed();
    s32 input_time = mInputWatch.TakeElapsed();
    s32 audio_time = mAudioWatch.TakeElapsed();

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

    mProcessHistory[mHistoryIdx] = process_time;
    mDrawHistory[mHistoryIdx] = draw_time;
    mRenderHistory[mHistoryIdx] = flip_time;
    mInputHistory[mHistoryIdx] = input_time;
    mAudioHistory[mHistoryIdx] = audio_time;

    char_type buffer[240];
    if (process_time)
    {
        mProcessAvg.AddSample(process_time);
    }
    if (process_time > 15)
    {
        wsprintfA(buffer, "LARGE PROCESS %d\n", process_time);
        OutputDebugStringA(buffer);
    }

    if (draw_time)
    {
        mDrawAvg.AddSample(draw_time);
    }
    if (draw_time > 15)
    {
        wsprintfA(buffer, "LARGE DRAW %d\n", draw_time);
        OutputDebugStringA(buffer);
    }

    if (flip_time)
    {
        mRenderAvg.AddSample(flip_time);
    }

    if (input_time)
    {
        mInputAvg.AddSample(input_time);
    }
    if (input_time > 10)
    {
        wsprintfA(buffer, "LARGE INPUT %d\n", input_time);
        OutputDebugStringA(buffer);
    }

    if (audio_time)
    {
        mAudioAvg.AddSample(audio_time);
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

    if (++mHistoryIdx >= 100)
    {
        mHistoryIdx = 0;
    }
    mCount++;
}

MATCH_FUNC(0x4ffd90)
void FrameProfiler::DrawProfileHistory()
{
    s32 count = mCount < 100 ? mCount : 100;

    for (s32 i = 0; i < count; i++)
    {
        s32 idx = mHistoryIdx - i - 1;
        if (idx < 0)
        {
            idx += 100;
        }

        DrawProfileBar(6 * i, mProcessHistory[idx], 0xFF00);
        DrawProfileBar(6 * i + 1, mDrawHistory[idx], 0xFF0000);
        DrawProfileBar(6 * i + 2, mRenderHistory[idx], 0xFF);
        DrawProfileBar(6 * i + 3, mInputHistory[idx], 0);
        // Draws mInputHistory again rather than mAudioHistory
        DrawProfileBar(6 * i + 4, mInputHistory[idx], 0xFFFFFF);
    }
}