#include "Orca_2FD4.hpp"
#include "Globals.hpp"
#include "map_0x370.hpp"
#include <string.h>

EXPORT_VAR u8 byte_6FDEEC;
GLOBAL(byte_6FDEEC, 0x6FDEEC);

MATCH_FUNC(0x554080)
bool Orca_2FD4::sub_554080(s32 a2)
{
    u8 v2;
    u8 v3;
    bool result;

    v2 = field_25;
    if (v2 > 1u 
        && v2 < 0xFEu 
        && (v3 = field_26, v3 > 1u) 
        && v3 < 0xFEu)
    {
        result = gMap_0x370_6F6268->sub_4E0130(
            (u8)field_25,
            (u8)field_26, 
            (u8)field_27, 
            a2, 
            &byte_6FDEEC, 
            1) == 0;
        return result;
    }
    return false;
}

STUB_FUNC(0x5540e0)
char_type Orca_2FD4::sub_5540E0(char_type a2, char_type a3, char_type a4, char_type a5, char_type a6)
{
    return 0;
}

STUB_FUNC(0x554110)
char_type Orca_2FD4::sub_554110(char_type a2, char_type a3)
{
    return 0;
}

STUB_FUNC(0x5545c0)
s32 Orca_2FD4::sub_5545C0()
{
    return 0;
}

MATCH_FUNC(0x5545e0)
void Orca_2FD4::init_5545E0()
{
    for (s32 i = 0; i < GTA2_COUNTOF(field_40); i++)
    {
        field_40[i] = 0;
    }
    field_38 = 0;
    field_34 = 1;
    field_36 = 0;
    field_3A = 0;
    field_2FD0 = 1;
    field_2FD1 = 0;
}

MATCH_FUNC(0x554620)
void Orca_2FD4::sub_554620(s32 a2)
{
    if (a2 == field_0)
    {
        field_2FD0 = 1;
        field_2FD1 = 0;
    }
}

STUB_FUNC(0x554640)
char_type Orca_2FD4::sub_554640()
{
    return 0;
}

STUB_FUNC(0x554710)
char_type Orca_2FD4::sub_554710()
{
    return 0;
}

STUB_FUNC(0x5548c0)
char_type Orca_2FD4::sub_5548C0()
{
    return 0;
}

STUB_FUNC(0x554920)
char_type Orca_2FD4::sub_554920()
{
    return 0;
}

MATCH_FUNC(0x554a90)
s32 Orca_2FD4::sub_554A90(cool_nash_0x294* a2)
{
    return field_3C.field_0_pOwner->field_0_char_ped == a2;
}

STUB_FUNC(0x554ab0)
char_type Orca_2FD4::sub_554AB0(s32 a2, s32 a3, u8 a4, u8 a5, u8 a6, u8 a7, u8 a8, u8 a9, s32 a10, u8* a11)
{
    return 0;
}

STUB_FUNC(0x5552b0)
char_type Orca_2FD4::sub_5552B0(char_type a2, char_type* a3, char_type* a4, char_type* a5, char_type a6)
{
    return 0;
}
