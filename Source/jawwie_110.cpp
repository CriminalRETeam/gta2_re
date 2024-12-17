#include "jawwie_110.hpp"
#include "cool_nash_0x294.hpp"
#include <stdio.h>

MATCH_FUNC(0x4beab0)
jawwie_20::jawwie_20()
{
    field_10.field_0_pOwner = NULL;
    sub_4FA7D0();
}

MATCH_FUNC(0x4bead0)
jawwie_20::~jawwie_20()
{
}

MATCH_FUNC(0x4fa7d0)
void jawwie_20::sub_4FA7D0()
{
    field_10.field_0_pOwner = NULL;
    field_0 = 0;
    field_1 = 0;
    field_2 = 0;
    field_14_count = 0;
    field_16 = 0;
    field_18 = 0;
    field_1C = 0;
    field_4 = NULL;
    field_8 = NULL;
    field_C = NULL;
    field_1D = 0;
}

STUB_FUNC(0x4fa800)
void jawwie_20::sub_4FA800(cool_nash_0x294* pPed)
{
}

STUB_FUNC(0x4fa820)
char_type jawwie_20::sub_4FA820()
{
    return 0;
}

STUB_FUNC(0x4fa9d0)
char_type jawwie_20::sub_4FA9D0()
{
    return 0;
}

STUB_FUNC(0x4faac0)
u32* jawwie_20::sub_4FAAC0()
{
    return 0;
}

STUB_FUNC(0x4fb330)
void jawwie_20::sub_4FB330()
{
}

MATCH_FUNC(0x4beae0)
jawwie_110::~jawwie_110()
{
}

MATCH_FUNC(0x4fa310)
void jawwie_110::init_4FA310()
{
    field_0 = 1;
    field_1_f8_idx = 0;
    field_4.sub_420E90();

    for (s32 i = 0; i < 25; i++)
    {
        field_8[i].field_0 = 0;
        field_8[i].field_4 = 0;
    }
}

STUB_FUNC(0x4fa330)
char_type jawwie_110::sub_4FA330(cool_nash_0x294* a2)
{
    return 0;
}

MATCH_FUNC(0x4fa470)
char_type jawwie_110::sub_4FA470(cool_nash_0x294* a2)
{
    if (a2->sub_45EDE0(2) || field_1_f8_idx >= 25u)
    {
        return 0;
    }

    field_4.sub_471140(a2);
    field_1_f8_idx++;

    return 1;
}

STUB_FUNC(0x4fa4b0)
jawwie_20* jawwie_110::sub_4FA4B0()
{
    return 0;
}

STUB_FUNC(0x4fa500)
void jawwie_110::sub_4FA500()
{
}

STUB_FUNC(0x4fa790)
void jawwie_110::sub_4FA790()
{
}
