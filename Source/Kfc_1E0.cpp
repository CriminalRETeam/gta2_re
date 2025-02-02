#include "Kfc_1E0.hpp"
#include "Mouze_44.hpp"
#include "Ped.hpp"

MATCH_FUNC(0x4beb00)
Kfc_30::Kfc_30()
{
    sub_5CBC00();
}

MATCH_FUNC(0x4beb10)
Kfc_30::~Kfc_30()
{
}

MATCH_FUNC(0x5cbc00)
void Kfc_30::sub_5CBC00()
{
    field_1A = 150;
    field_1E = 0;
    field_20 = 0;
    field_24 = 0;
    field_0 = 0;
    field_4 = 0;
    field_28 = 0;
    field_8 = 0;
    field_2C = 0;
    field_1C = 0;
}

MATCH_FUNC(0x5cbc30)
void Kfc_30::sub_5CBC30()
{
    sub_5CBC00();
}

MATCH_FUNC(0x5cbc40)
void Kfc_30::sub_5CBC40(Ped* a2)
{
    field_8->sub_4C9970(a2);
    field_4 = field_8->field_2C_ped_leader;
}

STUB_FUNC(0x5cbc60)
bool Kfc_30::sub_5CBC60()
{
    return field_4 && field_4->field_278 == 9;
}

STUB_FUNC(0x5cbc90)
char_type Kfc_30::sub_5CBC90()
{
    return 0;
}

STUB_FUNC(0x5cbd50)
void Kfc_30::sub_5CBD50()
{
}

STUB_FUNC(0x5cc1c0)
void Kfc_30::sub_5CC1C0()
{
}

STUB_FUNC(0x5cc480)
char_type Kfc_30::sub_5CC480()
{
    return 0;
}

MATCH_FUNC(0x4beb20)
Kfc_1E0::~Kfc_1E0()
{
}

MATCH_FUNC(0x5cbb70)
void Kfc_1E0::init_5CBB70()
{
}

MATCH_FUNC(0x5cbb80)
Kfc_30* Kfc_1E0::sub_5CBB80()
{
    for (u8 i = 0; i < GTA2_COUNTOF(field_0); i++)
    {
        if (!field_0[i].field_1E)
        {
            return &field_0[i];
        }
    }
    return 0;
}

MATCH_FUNC(0x5cbbd0)
void Kfc_1E0::sub_5CBBD0()
{
    for (s32 i = 0; i < 10; i++)
    {
        if (field_0[i].field_1E)
        {
            if (field_0[i].sub_5CC480())
            {
                field_0[i].field_1E = 0;
            }
        }
    }
}
