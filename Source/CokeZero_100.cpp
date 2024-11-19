#include "CokeZero_100.hpp"
#include <memory.h>
CokeZero_100* gCokeZero_100_702F34;

MATCH_FUNC(0x596a00)
CokeZero_50::CokeZero_50()
{
    field_0 = 0;
    for (s32 i = 0; i < 9; i++)
    {
        field_4[i] = 0;
    }
    field_36 = 0;
    field_38 = 0;
    field_3C = 0;
    field_40 = 0;
    field_4C = 0;
    field_48 = 0;
    field_44 = 0;
    field_34 = 9;
}

MATCH_FUNC(0x596a40)
CokeZero_50::~CokeZero_50()
{
    field_44 = 0;
}

MATCH_FUNC(0x596a50)
void CokeZero_50::Empty_596A50()
{
}

MATCH_FUNC(0x596a60)
void CokeZero_50::Empty_596A60()
{
}

MATCH_FUNC(0x596a70)
void CokeZero_50::sub_596A70(s32 a2)
{
    for (u32 i=a2; i > 0; i--)
    {
        field_4[i] = 0;
    }
}

STUB_FUNC(0x596a90)
s16 CokeZero_50::sub_596A90(s32 a2, s32 a3, s32 a4, u32 a5)
{
    return 0;
}

STUB_FUNC(0x596c90)
s32 CokeZero_50::sub_596C90()
{
    return 0;
}

STUB_FUNC(0x5970c0)
char_type CokeZero_50::sub_5970C0()
{
    return 0;
}

STUB_FUNC(0x597100)
void CokeZero_50::sub_597100(s32 a2, s32 a3)
{
}

MATCH_FUNC(0x5967e0)
CokeZero_100::CokeZero_100()
{
    CokeZero_50* pIter = field_C;
    for (s32 i = 0; i < 3 - 1; i++)
    {
        pIter->field_44 = pIter + 1;
        pIter++;
    }

    field_C[3 - 1].field_44 = 0;
    field_4 = field_C;

    field_8 = 0;
    field_FC_count = 0;
    field_0 = 9;
    field_2 = 3;
}

MATCH_FUNC(0x596840)
CokeZero_100::~CokeZero_100()
{
    field_4 = 0;
    field_8 = 0;
}

MATCH_FUNC(0x596860)
s16 CokeZero_100::sub_596860()
{
    field_0++;
    if (field_0 > 13u)
    {
        field_0 = 9;
    }
    return field_0;
}

MATCH_FUNC(0x596880)
void CokeZero_100::sub_596880()
{
    field_2++;
}

STUB_FUNC(0x596890)
void CokeZero_100::sub_596890(s32 a2, s32 a3, s32 a4, u32 a5)
{
}

STUB_FUNC(0x596940)
void CokeZero_100::sub_596940()
{
}

MATCH_FUNC(0x5969e0)
void CokeZero_100::DrawExploding_5969E0()
{
    CokeZero_50* pIter = field_8;
    while (pIter)
    {
        pIter->sub_596C90();
        pIter = pIter->field_44;
    }
}
