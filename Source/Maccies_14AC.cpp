#include "Maccies_14AC.hpp"
#include "Globals.hpp"
#include "rng.hpp"

EXPORT_VAR Maccies_14AC* gMaccies_14AC_67E5D0;
GLOBAL(gMaccies_14AC_67E5D0, 0x67E5D0);

STUB_FUNC(0x4c1c50)
void Maccies_2C::sub_4C1C50()
{
}

MATCH_FUNC(0x4c1c70)
void Maccies_2C::sub_4C1C70(s32 a2, s32 a3, s32 a4, s16 a5, s32 a6, s16 a7, s16 a8)
{
    field_4 = a2;
    field_8 = a3;
    field_C = a4;
    field_10 = a5;
    field_0 = a6;
    field_12 = a7;
    field_14 = a8;
    field_18 = rng_dword_67AB34->field_0_rng + 1;
    field_20 = 1;
    field_1E = 0;
    field_1C = 0;
    field_24 = 0;
    field_28 = 0;
    field_21 = 1;
}

STUB_FUNC(0x4c1cd0)
void Maccies_14AC::sub_4C1CD0(Maccies_14AC* a1)
{
}

MATCH_FUNC(0x4c1d70)
void Maccies_14AC::sub_4C1D70()
{
    s32 start_idx = field_14A8 * (rng_dword_67AB34->field_0_rng & 3);
    s32 end_idx = start_idx + field_14A8;
    if (end_idx > field_14A0)
    {
        end_idx = field_14A0;
    }

    Maccies_2C* pIter = &field_0[start_idx];
    for (s32 i = start_idx; i < end_idx; i++)
    {
        pIter->sub_4C1C50();
        pIter++;
    }
}

MATCH_FUNC(0x4c1dc0)
Maccies_2C* Maccies_14AC::sub_4C1DC0(s32 a2, s32 a3, s32 a4, s16 a5, s32 a6, s16 a7, s16 a8)
{
    Maccies_2C* pMaccies = &field_0[field_14A0];
    pMaccies->sub_4C1C70(a2, a3, a4, a5, a6, a7, a8);
    field_14A0++;
    return pMaccies;
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
