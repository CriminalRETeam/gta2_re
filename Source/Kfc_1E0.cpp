#include "Kfc_1E0.hpp"

MATCH_FUNC(0x4beb00)
Kfc_30::Kfc_30()
{
    sub_5CBC00();
}

STUB_FUNC(0x4beb10)
Kfc_30::~Kfc_30()
{
}

MATCH_FUNC(0x5cbc00)
void Kfc_30::sub_5CBC00()
{
    this->field_1A = 150;
    this->field_1E = 0;
    this->field_20 = 0;
    this->field_24 = 0;
    this->field_0 = 0;
    this->field_4 = 0;
    this->field_28 = 0;
    this->field_8 = 0;
    this->field_2C = 0;
    this->field_1C = 0;
}

STUB_FUNC(0x5cbc30)
s32 Kfc_30::sub_5CBC30()
{
    return 0;
}

STUB_FUNC(0x5cbc40)
void Kfc_30::sub_5CBC40(cool_nash_0x294* a2)
{
}

STUB_FUNC(0x5cbc60)
bool Kfc_30::sub_5CBC60()
{
    return 0;
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

STUB_FUNC(0x4beb20)
Kfc_1E0::~Kfc_1E0()
{
}

MATCH_FUNC(0x5cbb70)
void Kfc_1E0::init_5CBB70()
{
}

STUB_FUNC(0x5cbb80)
Kfc_30* Kfc_1E0::sub_5CBB80()
{
    return 0;
}

STUB_FUNC(0x5cbbd0)
void Kfc_1E0::sub_5CBBD0()
{
}
