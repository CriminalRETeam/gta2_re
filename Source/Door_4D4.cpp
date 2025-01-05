#include "Door_4D4.hpp"
#include "cool_nash_0x294.hpp"

STUB_FUNC(0x49c640)
Door_38::Door_38()
{
}

MATCH_FUNC(0x49c690)
Door_38::~Door_38()
{
    this->field_0 = 0;
    this->field_4 = 0;
    this->field_8 = 0;
    this->field_C = 0;
}

STUB_FUNC(0x49c6a0)
s32 Door_38::sub_49C6A0(s32 a1)
{
    return 0;
}

STUB_FUNC(0x49c6d0)
char_type Door_38::sub_49C6D0(u32* a2)
{
    return 0;
}

// https://decomp.me/scratch/XlGeq asm differ bugged
STUB_FUNC(0x49c7f0)
bool Door_38::sub_49C7F0(cool_nash_0x294* a2)
{
    if (this->field_20_state == 1)
    {
        return a2->field_15C_player_weapons != 0;
    }

    if (this->field_20_state == 5)
    {
        if (field_10)
        {
            if (a2 == field_10)
            {
                return a2->field_200 == this->field_14;
            }
        }
    }
    return false;
}

STUB_FUNC(0x49c840)
s32 Door_38::sub_49C840()
{
    return 0;
}

MATCH_FUNC(0x49c870)
void Door_38::sub_49C870(u32* a2)
{
    if (field_29)
    {
        if (sub_49C6D0(a2))
        {
            if (field_2C)
            {
                sub_49C840();
            }
        }
    }
}

MATCH_FUNC(0x49c8a0)
void Door_38::sub_49C8A0(cool_nash_0x294* a2)
{
    if (field_29)
    {
        if (sub_49C7F0(a2))
        {
            if (field_2C)
            {
                sub_49C840();
            }
        }
    }
}

STUB_FUNC(0x49c8d0)
Object_2C* Door_38::sub_49C8D0(s32 arg0, u8 a1, u8 a2, u8 a3, char_type a4, s32 a5)
{
    return 0;
}

STUB_FUNC(0x49ca50)
void Door_38::sub_49CA50(u8 a1, char_type a2, char_type a3, char_type a4, s32 a5)
{
}

STUB_FUNC(0x49cac0)
Object_2C* Door_38::sub_49CAC0(s32 a2, char_type a3, u8 a4, s32 a5, s32 a6, s32 a7, s32 a8, Sprite_4C* a9)
{
    return 0;
}

STUB_FUNC(0x49cc00)
Object_2C* Door_38::sub_49CC00(s32 a2, char_type a3, u8 a4, s32 a5, s32 a6, s32 a7, s32 a8, Sprite_4C* a9)
{
    return 0;
}

STUB_FUNC(0x49cd90)
void Door_38::sub_49CD90()
{
}

STUB_FUNC(0x49ce90)
char_type Door_38::sub_49CE90()
{
    return 0;
}

STUB_FUNC(0x49cf10)
Door_10* Door_4D4::sub_49CF10(u8 a1, char_type a2, char_type a3, char_type a4, s32 a5, char_type a6)
{
    return 0;
}

STUB_FUNC(0x49cf50)
Door_38* Door_4D4::sub_49CF50(u8 a2, char_type a3, s32 a4, char_type a5, s32 a6, char_type a7, char_type a8)
{
    return 0;
}

STUB_FUNC(0x49cfa0)
Door_38* Door_4D4::sub_49CFA0(u8 a1, u8 a2, u8 a3, u8 a4, s32 a5, s32 a7, char_type a8)
{
    return 0;
}

STUB_FUNC(0x49d170)
Door_38* Door_4D4::sub_49D170(u8 a1,
                              char_type a2,
                              char_type a3,
                              char_type a4,
                              s32 a5,
                              s32 a7,
                              s32 a8,
                              s32 a9,
                              s32 a10,
                              s32 a11,
                              char_type a12,
                              char_type a13)
{
    return 0;
}

STUB_FUNC(0x49d1f0)
Door_38* Door_4D4::sub_49D1F0(u8 a1,
                              char_type a2,
                              char_type a3,
                              char_type a4,
                              s32 a5,
                              s32 a7,
                              s32 a8,
                              s32 a9,
                              s32 a10,
                              s32 a11,
                              char_type a12,
                              char_type a13)
{
    return 0;
}

STUB_FUNC(0x49d2d0)
s16 Door_4D4::sub_49D2D0(s16 a1, s16 a2, char_type a3)
{
    return 0;
}

MATCH_FUNC(0x49d340)
void Door_4D4::sub_49D340(u32* a2, u8 a3)
{
    field_0[a3].sub_49C870(a2);
}

MATCH_FUNC(0x49d370)
void Door_4D4::sub_49D370(cool_nash_0x294* a2, u8 idx)
{
    field_0[idx].sub_49C8A0(a2);
}

MATCH_FUNC(0x49d3a0)
Door_38* Door_4D4::sub_49D3A0()
{
    return &field_0[field_4D0_count];
}

STUB_FUNC(0x49d3c0)
char_type Door_4D4::sub_49D3C0(s32 a2, u8 a3)
{
    return 0;
}

MATCH_FUNC(0x49d460)
void Door_4D4::sub_49D460()
{
    for (u16 i = 0; i < field_4D0_count; ++i)
    {
        field_0[i].sub_49CE90();
    }
}

STUB_FUNC(0x49d4a0)
Door_4D4::Door_4D4()
{
}

STUB_FUNC(0x49d570)
Door_4D4::~Door_4D4()
{
}
