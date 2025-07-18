#include "Maccies_14AC.hpp"
#include "Globals.hpp"
#include "rng.hpp"

DEFINE_GLOBAL(Maccies_14AC*, gMaccies_14AC_67E5D0, 0x67E5D0);

STUB_FUNC(0x4c1c50)
void Generator_2C::sub_4C1C50()
{
    NOT_IMPLEMENTED;
}

MATCH_FUNC(0x4c1c70)
void Generator_2C::sub_4C1C70(Fix16 x, Fix16 y, Fix16 z, Ang16 rot, s32 generator_type, s16 min_delay, s16 max_delay)
{
    field_4_x = x.mValue;
    field_8_y = y.mValue;
    field_C_z = z.mValue;
    field_10 = rot.rValue;
    field_0_gen_type = generator_type;
    field_12_min_delay = min_delay;
    field_14_max_delay = max_delay;
    field_18_cycle = rng_dword_67AB34->field_0_rng + 1;
    field_20 = 1;
    field_1E = 0;
    field_1C_ammo = 0;
    field_24_obj = NULL;
    field_28 = 0;
    field_21 = 1;
}

STUB_FUNC(0x4c1cd0)
void Maccies_14AC::sub_4C1CD0(Maccies_14AC* a1)
{
    NOT_IMPLEMENTED;
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

    Generator_2C* pIter = &field_0[start_idx];
    for (s32 i = start_idx; i < end_idx; i++)
    {
        pIter->sub_4C1C50();
        pIter++;
    }
}

MATCH_FUNC(0x4c1dc0)
Generator_2C* Maccies_14AC::sub_4C1DC0(Fix16 x, Fix16 y, Fix16 z, Ang16 rot, s32 type, s16 min_delay, s16 max_delay)
{
    Generator_2C* pMaccies = &field_0[field_14A0];
    pMaccies->sub_4C1C70(x, y, z, rot, type, min_delay, max_delay);
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