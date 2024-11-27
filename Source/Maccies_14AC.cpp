#include "Maccies_14AC.hpp"
#include "rng.hpp"

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
