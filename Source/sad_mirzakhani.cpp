#include "sad_mirzakhani.hpp"
#include "Function.hpp"
#include "Player.hpp" // eager_benz
#include "rng.hpp"

MATCH_FUNC(0x431D30);
silly_saha_0x2C::silly_saha_0x2C()
{
    sub_431D50();
}

MATCH_FUNC(0x431D40);
silly_saha_0x2C::~silly_saha_0x2C()
{
}

MATCH_FUNC(0x431D50);
void silly_saha_0x2C::sub_431D50()
{
    field_0_pZone = 0;
    field_4 = -1;
    field_8 = 87;
    field_C = 51;
    field_10 = -1;
    field_12 = -2;
    field_14 = 0;
    field_18 = 87;
    field_1C = 0;
    field_20_counterVal = 0;
    field_24 = 0;
    field_25 = 0;
    field_26 = 0;
    field_28 = 0;
    field_2A_bUsed = 0;
    field_2B = 0;
}

MATCH_FUNC(0x431DA0);
void silly_saha_0x2C::sub_431DA0()
{
    sub_431D50();
}

MATCH_FUNC(0x431DB0);
void silly_saha_0x2C::sub_431DB0()
{
    field_2B = 0;
}

// ============

MATCH_FUNC(0x431DC0);
sad_mirzakhani::sad_mirzakhani()
{
    field_1B8 = 0;
    field_1BC = 0;
}

MATCH_FUNC(0x431DF0);
sad_mirzakhani::~sad_mirzakhani()
{
    field_1B8 = 0;
}

MATCH_FUNC(0x431E10);
void sad_mirzakhani::sub_431E10(eager_benz* a2)
{
    field_1BC = 0;
    field_1B8 = a2;
}

MATCH_FUNC(0x431E30);
void sad_mirzakhani::sub_431E30()
{
    field_1BC = rng_dword_67AB34->field_0_rng;

    silly_saha_0x2C* pIter = &field_0[0];
    for (s32 i = GTA2_COUNTOF(field_0) - 1; i >= 0; i--)
    {
        if (pIter->field_2A_bUsed)
        {
            if (pIter->field_2B)
            {
                const u32 f1c = pIter->field_1C;
                if (f1c != -1 && field_1BC - pIter->field_20_counterVal > f1c)
                {
                    pIter->field_26 = 0;
                    pIter->sub_431DB0();
                }
            }
        }
        pIter++;
    }
}

MATCH_FUNC(0x431E90);
u16 sad_mirzakhani::next_free_idx_431E90()
{
    for (u16 i = 0; i < GTA2_COUNTOF(field_0); i++)
    {
        if (!field_0[i].field_2A_bUsed)
        {
            return i;
        }
    }
    return GTA2_COUNTOF(field_0);
}

STUB_FUNC(0x431EC0);
u16 sad_mirzakhani::find_431EC0(u16 idx, s16 f_4, s32 f_8, s32 f_c, s16 f_10, s16 f_12, s32 f_14, s32 f_18, gmp_map_zone* pZone)
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x431FE0);
u16 sad_mirzakhani::alloc_next_431FE0(s16 f_4,
                                      s32 f_8,
                                      s32 f_c,
                                      s16 f_10,
                                      s16 f_12,
                                      s32 f_14,
                                      s32 f_18,
                                      s32 f_1c,
                                      s8 f_24,
                                      s8 f_25,
                                      u16 f_28,
                                      gmp_map_zone* pZone)
{
    const s16 idx = next_free_idx_431E90();
    if (idx == 10)
    {
        return idx;
    }

    field_0[idx].field_4 = f_4;
    field_0[idx].field_8 = f_8;
    field_0[idx].field_C = f_c;
    field_0[idx].field_10 = f_10;
    field_0[idx].field_12 = f_12;
    field_0[idx].field_14 = f_14;
    field_0[idx].field_18 = f_18;
    field_0[idx].field_0_pZone = pZone;
    field_0[idx].field_20_counterVal = rng_dword_67AB34->field_0_rng;
    field_0[idx].field_1C = f_1c;
    field_0[idx].field_24 = f_24;
    field_0[idx].field_25 = f_25;
    field_0[idx].field_26 = 0; 
    field_0[idx].field_28 = f_28;
    field_0[idx].field_2A_bUsed = 1;
    field_0[idx].field_2B = 1;

    return idx;
}

MATCH_FUNC(0x432080);
s16 sad_mirzakhani::sub_432080(u16 idx)
{
    silly_saha_0x2C* pItem = &field_0[idx];
    if (!pItem->field_2A_bUsed)
    {
        return -1;
    }

    if (pItem->field_2B)
    {
        return -2;
    }

    if (pItem->field_26 == pItem->field_25)
    {
        pItem->sub_431DA0();
        return -3;
    }
    else
    {
        pItem->sub_431DA0();
        return -4;
    }
}

STUB_FUNC(0x4320D0);
void sad_mirzakhani::sub_4320D0(s16 f_4, s32 f_8, s32 f_c, s16 f_10, s16 f_12, s32 f_14, s32 f_18, gmp_map_zone* pZone)
{
    NOT_IMPLEMENTED;
    u16 found_idx = 0;
    for (s16 i = 0; i < 10u; i = found_idx + 1)
    {
        found_idx = find_431EC0(i, f_4, f_8, f_c, f_10, f_12, f_14, f_18, pZone);
        // _found_idx = found_idx;
        if (found_idx >= 10u)
        {
            break;
        }
        silly_saha_0x2C* pFound = &field_0[found_idx];
        pFound->field_26++;
        if (pFound->field_26 == pFound->field_25)
        {
            field_1B8->field_368_player->field_2D4_scores.AddCash_592620(field_1B8->field_368_player->field_6BC_multpliers.field_0 *
                                                                    pFound->field_28);
            pFound->sub_431DB0();
        }
    }
}

STUB_FUNC(0x432170);
s8 sad_mirzakhani::sub_432170(int a2, int a3)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x432240);
s8 sad_mirzakhani::sub_432240(int a2, int a3)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x432300);
bool sad_mirzakhani::sub_432300(int a2, int a3)
{
    NOT_IMPLEMENTED;
    return false;
}