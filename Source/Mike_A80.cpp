#include "Mike_A80.hpp"
#include "Globals.hpp"

DEFINE_GLOBAL(Mike_A80*, gMike_A80_6F7328, 0x6F7328);

MATCH_FUNC(0x4ff1b0)
void Mike_A80::sub_4FF1B0()
{
    field_A78_ary_idx = 0;
    field_A7C_count = 0;
}

STUB_FUNC(0x4ff1c0)
s32 Mike_A80::sDrawFlatRect_4FF1C0(s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x4ff250)
void Mike_A80::DebugDrawProfiling_4FF250()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x4ff910)
s32 Mike_A80::sDrawString_4FF910(s32 a1, s32 a2, const wchar_t* pStr, wchar_t Format)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x4ff970)
void Mike_A80::sub_4FF970(u32* a1)
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x4ff980)
void Mike_A80::sub_4FF980()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x4ff990)
u32 Mike_A80::sub_4FF990(u32 a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x4ff9f0)
u32 Mike_A80::sub_4FF9F0(u32 a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x4ffa50)
void Mike_A80::sub_4FFA50(s32 a1, char_type* Format)
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x4ffa90)
s32 Mike_A80::sub_4FFA90()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x4ffd90)
s32 Mike_A80::sub_4FFD90()
{
    NOT_IMPLEMENTED;
    return 0;
}