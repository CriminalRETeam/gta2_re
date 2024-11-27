#pragma once

#include "Function.hpp"
#include "cool_nash_0x294.hpp"

class cool_nash_0x294;
class Car_BC;
class Car_3C;

class Char_C
{
  public:
    EXPORT void sub_46EB60(u32* a2);
    EXPORT void sub_4703F0(Char_C* a1);
    EXPORT Char_C();
    EXPORT ~Char_C();
    EXPORT cool_nash_0x294* sub_470A50(s32 xpos, s32 ypos, s32 zpos, u8 remap, s16 a6);
    EXPORT cool_nash_0x294* sub_470B00(Car_BC* a2);
    EXPORT cool_nash_0x294* sub_470BA0(Car_BC* a1, s32 a2);
    EXPORT cool_nash_0x294* sub_470CC0(Car_BC* a2);
    EXPORT cool_nash_0x294* sub_470D60();
    EXPORT cool_nash_0x294* sub_470E30();
    EXPORT cool_nash_0x294* sub_470F30();
    EXPORT void DoIanTest_471060(s16 a1);
    EXPORT cool_nash_0x294* sub_4710C0(s32 a2);

    s16 field_0;
    char_type field_2;
    char_type field_3;
    char_type field_4;
    char_type field_5;
    char_type field_6_num_peds_on_screen;
    char_type field_7;
    Car_3C* field_8;
};

class Char_203AC
{
  public:
    EXPORT ~Char_203AC();
    cool_nash_0x294* field_0;
    cool_nash_0x294* field_4;
    cool_nash_0x294 field_8[200];
    s16 field_203A8;
    s16 field_203AA;
};

extern Char_C* gChar_C_6787BC;
extern Char_203AC* gChar_203AC_6787B8;
