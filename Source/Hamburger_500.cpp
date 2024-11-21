#include "Hamburger_500.hpp"

s32 dword_678D0C;

MATCH_FUNC(0x4747b0)
void Hamburger_40::sub_4747B0()
{
    this->field_0 = 0;
    this->field_10 = 0;
    this->field_8 = 0;
    this->field_14 = dword_678D0C;
    this->field_18 = dword_678D0C;
    this->field_1C = dword_678D0C;
    this->field_20 = 0;
    this->field_21 = 0;
    this->field_22 = 1;
    this->field_23 = 1;
    this->field_24 = 0;
    this->field_25 = 0;
    this->field_30 = 0;
    this->field_26 = 0;
    this->field_2A = 0;
    this->field_2C = 0;
    this->field_2E = 0;
    this->field_C = 0;
    this->field_34 = 0;
    this->field_4 = 0;
    this->field_38 = 0;
    this->field_3C = 0;
}

MATCH_FUNC(0x474ca0)
Hamburger_40::Hamburger_40()
{
    sub_4747B0();
}

MATCH_FUNC(0x474cb0)
Hamburger_40::~Hamburger_40()
{
}

MATCH_FUNC(0x474810)
Hamburger_40* Hamburger_500::sub_474810()
{
    for (u8 i = 0; i < 20; i++)
    {
        if (!field_0[i].field_0)
        {
            field_0[i].field_0 = 1;
            return &field_0[i];
        }
    }
    return 0;
}

STUB_FUNC(0x474850)
char_type Hamburger_500::sub_474850(cool_nash_0x294* a1, cool_nash_0x294* a2)
{
    return 0;
}

STUB_FUNC(0x4748a0)
s32 Hamburger_500::sub_4748A0(s32 a2, cool_nash_0x294* a3)
{
    return 0;
}

STUB_FUNC(0x474920)
char_type Hamburger_500::sub_474920(s32 a2, cool_nash_0x294* a3)
{
    return 0;
}

STUB_FUNC(0x474970)
char_type Hamburger_500::sub_474970(s32 a2)
{
    return 0;
}

STUB_FUNC(0x4749b0)
char_type Hamburger_500::sub_4749B0(cool_nash_0x294* a1)
{
    return 0;
}

STUB_FUNC(0x474a20)
char_type Hamburger_500::sub_474A20(cool_nash_0x294* a1)
{
    return 0;
}

STUB_FUNC(0x474a80)
char_type Hamburger_500::sub_474A80(cool_nash_0x294* a1)
{
    return 0;
}

STUB_FUNC(0x474af0)
char_type Hamburger_500::sub_474AF0(cool_nash_0x294* a1)
{
    return 0;
}

STUB_FUNC(0x474b50)
char_type Hamburger_500::sub_474B50(cool_nash_0x294* a1)
{
    return 0;
}

STUB_FUNC(0x474bc0)
char_type Hamburger_500::sub_474BC0(cool_nash_0x294* a1)
{
    return 0;
}

MATCH_FUNC(0x474c30)
char_type Hamburger_500::sub_474C30(cool_nash_0x294* pPed)
{
    for (u8 i = 0; i < 20; i++)
    {
        if (field_0[i].field_0 == 1)
        {
            if (sub_474850(pPed, field_0[i].field_4))
            {
                if (field_0[i].field_C < 4)
                {
                    continue;
                }

                if (field_0[i].field_C <= 5)
                {
                    return 1;
                }
            }
        }
    }
    return 0;
}

MATCH_FUNC(0x474cc0)
void Hamburger_500::sub_474CC0(Hamburger_40* toFind)
{
    for (u8 i = 0; i < 20; i++)
    {
        if (&field_0[i] == toFind)
        {
            field_0[i].field_0 = 0;
            field_0[i].sub_4747B0();
            return;
        }
    }
}

MATCH_FUNC(0x474d10)
Hamburger_500::Hamburger_500()
{
}

MATCH_FUNC(0x474d30)
Hamburger_500::~Hamburger_500()
{
}
