#include "sad_mirzakhani.hpp"
#include "Function.hpp"

// TODO: Move
struct rng
{
    s32 field_0_rng;
    s32 field_4_rnd;
};

// TODO: Move
rng* rng_dword_67AB34;

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

MATCH_FUNC(0x4431DA0);
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

STUB_FUNC(0x431DC0);
sad_mirzakhani::sad_mirzakhani()
{
    field_1B8 = 0;
    field_1BC = 0;
}

STUB_FUNC(0x431DF0);
sad_mirzakhani::~sad_mirzakhani()
{
    field_1B8 = 0;
}

MATCH_FUNC(0x431E10);
void sad_mirzakhani::sub_431E10(eager_benz *a2)
{
    field_1BC = 0;
    field_1B8 = a2;
}

MATCH_FUNC(0x431E30);
void sad_mirzakhani::sub_431E30()
{
    field_1BC = rng_dword_67AB34->field_0_rng;

    silly_saha_0x2C* pIter = &field_0[0];
    for (s32 i = GTA2_COUNTOF(field_0)-1; i >= 0; i--)
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
