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
        this->field_0 = 0;
        this->field_4 = 0;
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
    EXPORT s32 sDrawFlatRect_4FF1C0(s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
    EXPORT void DebugDrawProfiling_4FF250();
    EXPORT s32 sDrawString_4FF910(s32 a1, s32 a2, const wchar_t* pStr, wchar_t Format);
    EXPORT void sub_4FF970(u32* a1);
    EXPORT void sub_4FF980();
    EXPORT u32 sub_4FF990(u32 a2);
    EXPORT u32 sub_4FF9F0(u32 a2);
    EXPORT void sub_4FFA50(s32 a1, char_type* Format);
    EXPORT s32 sub_4FFA90();
    EXPORT s32 sub_4FFD90();

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

EXPORT_VAR extern Mike_A80* gMike_A80_6F7328;
