#include "Maccies_14AC.hpp"

STUB_FUNC(0x4c1c50)
void Maccies_2C::sub_4C1C50()
{
}

STUB_FUNC(0x4c1c70)
s32 Maccies_2C::sub_4C1C70(s32 a2, s32 a3, s32 a4, s16 a5, s32 a6, s16 a7, s16 a8)
{
    return 0;
}

STUB_FUNC(0x4c1cd0)
void Maccies_14AC::sub_4C1CD0(Maccies_14AC* a1)
{
}

STUB_FUNC(0x4c1d70)
void Maccies_14AC::sub_4C1D70()
{
}

STUB_FUNC(0x4c1dc0)
Maccies_2C* Maccies_14AC::sub_4C1DC0(s32 a2, s32 a3, s32 a4, s16 a5, s32 a6, s16 a7, s16 a8)
{
    return 0;
}

MATCH_FUNC(0x4c1e20)
Maccies_14AC::Maccies_14AC()
{
    for (u32 i = 0; i < GTA2_COUNTOF(field_0); i++)
    {
        field_0[i].field_10 = 0;
    }

    field_14A0 = 0;
    field_14A4 = 0;
    field_14A8 = 0;
}
