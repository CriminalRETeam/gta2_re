#include "Mike_A80.hpp"
#include "Globals.hpp"
#include "Draw.hpp"
#include "gtx_0x106C.hpp"
#include <stdio.h>
#include <stdarg.h>
#include <windows.h>
#include <mmsystem.h>

DEFINE_GLOBAL(Mike_A80*, gMike_A80_6F7328, 0x6F7328);

MATCH_FUNC(0x4ff1b0)
void Mike_A80::sub_4FF1B0()
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

STUB_FUNC(0x4ff250)
void Mike_A80::DebugDrawProfiling_4FF250()
{
    NOT_IMPLEMENTED;
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
void Mike_A80::sub_4FF970(u32* a1)
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

STUB_FUNC(0x4ffa90)
s32 Mike_A80::sub_4FFA90()
{
    NOT_IMPLEMENTED;
    return 0;
}

WIP_FUNC(0x4ffd90)
void Mike_A80::sub_4FFD90()
{
    s32 count = field_A7C_count;
    if (count >= 100)
    {
        count = 100;
    }

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