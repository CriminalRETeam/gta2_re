#pragma once

#include "Function.hpp"
#include <memory.h>
#include <wchar.h>

// TODO: Not sure if this type actually exists yet
class Mike_78
{
  public:
    s32 field_0[30];

    void Clear()
    {
        for (s32 i = 0; i < 30; i++)
        {
            field_0[i] = 0;
        }
    }
};

struct Mike_80
{
    Mike_80()
    {
        field_7C = 0;
        field_A0_count = 0;
        field_0.Clear();
    }

    // Keeps a running sum of the last 30 samples
    void AddSample(s32 value)
    {
        field_7C += value - field_0.field_0[field_A0_count];
        field_0.field_0[field_A0_count] = value;
        if (++field_A0_count >= 30)
        {
            field_A0_count = 0;
        }
    }

    Mike_78 field_0;
    s32 field_A0_count;
    s32 field_7C;
};

struct Mike_8
{
    Mike_8()
    {
        init();
    }

    void init()
    {
        field_0 = 0;
        field_4 = 0;
    }
    s32 TakeElapsed()
    {
        s32 elapsed = field_4 - field_0;
        init();
        return elapsed;
    }

    int field_0;
    int field_4;
};

class Mike_A80
{
  public:
    Mike_A80()
    {
        sub_4FF1B0();
    }

    EXPORT void sub_4FF1B0();
    EXPORT s32 sDrawFlatRect_4FF1C0(f32 left, f32 top, f32 right, f32 bottom, s32 colour);
    EXPORT void DebugDrawProfiling_4FF250();
    EXPORT static void sDrawString_4FF910(s32 xpos, s32 ypos, const wchar_t* pFormat, ...);
    EXPORT void sub_4FF970(u32* a1);
    EXPORT void sub_4FF980();
    EXPORT void sub_4FF990(u32 idx);
    EXPORT void sub_4FF9F0(u32 idx);
    EXPORT void sub_4FFA50(const char_type* pFormat, ...);
    EXPORT void sub_4FFA90();
    EXPORT void sub_4FFD90();

    inline void DrawProfileBar(s32 x, s32 value, s32 colour)
    {
        f32 fx = (f32)x;
        f32 left = 630.0f - fx;
        sDrawFlatRect_4FF1C0(left, 478.0f - (value * 4), left + 1.0f, 478.0f, colour);
    }

    Mike_8 m81;
    Mike_8 m82;
    Mike_8 m83;
    Mike_8 m84;
    Mike_8 m85;

    Mike_80 field_28_m80_1;
    Mike_80 field_28_m80_2;
    Mike_80 field_28_m80_3;
    Mike_80 field_28_m80_4;
    Mike_80 field_28_m80_5;

    s32 field_2A8_ary[100];
    s32 field_438_ary[100];
    s32 field_5C8_ary[100];
    s32 field_758_ary[100];
    s32 field_8E8_ary[100];
    s32 field_A78_ary_idx;
    s32 field_A7C_count;
};

EXTERN_GLOBAL(Mike_A80*, gMike_A80_6F7328);
