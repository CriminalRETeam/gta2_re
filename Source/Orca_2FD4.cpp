#include "Orca_2FD4.hpp"
#include "Globals.hpp"
#include "map_0x370.hpp"
#include <string.h>

DEFINE_GLOBAL(u8, byte_6FDEEC, 0x6FDEEC);

// FUNCTION: 105 0x554080
bool Orca_2FD4::sub_554080(s32 a2)
{
    if (field_25 > 1u && field_25 < 254u && field_26 > 1u && field_26 < 254u)
    {
        bool result = gMap_0x370_6F6268->sub_4E0130((u8)field_25, (u8)field_26, (u8)field_27, a2, &byte_6FDEEC, 1) == 0;
        return result;
    }
    return false;
}

// FUNCTION: 105 0x5540e0
char_type Orca_2FD4::sub_5540E0(char_type a2, char_type a3, char_type a4, char_type a5, char_type a6)
{
    this->field_25 = a2;
    this->field_26 = a3;
    this->field_27 = a4;
    return sub_554110(a5, a6);
}

STUB_FUNC(0x554110)
char_type Orca_2FD4::sub_554110(char_type a2, char_type a3)
{
    NOT_IMPLEMENTED;
    return 0;
}

// FUNCTION: 105 0x5545c0
void Orca_2FD4::sub_5545C0()
{
    memset(this->field_40, 0, sizeof(this->field_40));
}

// FUNCTION: 105 0x5545e0
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

// FUNCTION: 105 0x554620
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
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x554710)
char_type Orca_2FD4::sub_554710()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x5548c0)
char_type Orca_2FD4::sub_5548C0()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x554920)
char_type Orca_2FD4::sub_554920()
{
    NOT_IMPLEMENTED;
    return 0;
}

// FUNCTION: 105 0x554a90
s32 Orca_2FD4::sub_554A90(Ped* a2)
{
    return field_3C.field_0_pOwner->field_0_char_ped == a2;
}

STUB_FUNC(0x554ab0)
char_type Orca_2FD4::sub_554AB0(s32 a2, s32 a3, u8 a4, u8 a5, u8 a6, u8 a7, u8 a8, u8 a9, s32 a10, u8* a11)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x5552b0)
char_type Orca_2FD4::sub_5552B0(char_type a2, char_type* a3, char_type* a4, char_type* a5, char_type a6)
{
    NOT_IMPLEMENTED;
    return 0;
}