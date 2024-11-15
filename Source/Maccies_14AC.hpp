#pragma once

#include "Function.hpp"

class Maccies_2C
{
public:
    EXPORT void sub_4C1C50();
    EXPORT s32 sub_4C1C70(s32 a2, s32 a3, s32 a4, s16 a5, s32 a6, s16 a7, s16 a8);
    s32 field_0;
    s32 field_4;
    s32 field_8;
    s32 field_C;
    s16 field_10;
    s16 field_12;
    s16 field_14;
    s16 field_16;
    s32 field_18;
    s16 field_1C;
    s16 field_1E;
    char_type field_20;
    char_type field_21;
    char_type field_22;
    char_type field_23;
    s32 field_24;
    s32 field_28;
};

class Maccies_14AC
{
public:
    EXPORT void sub_4C1CD0(Maccies_14AC* a1);
    EXPORT void sub_4C1D70();
    EXPORT Maccies_2C* sub_4C1DC0(s32 a2, s32 a3, s32 a4, s16 a5, s32 a6, s16 a7, s16 a8);
    EXPORT Maccies_14AC* ctor_4C1E20();
    Maccies_2C field_0[120];
    s32 field_14A0;
    s32 field_14A4;
    u8 field_14A8;
    char_type field_14A9;
    char_type field_14AA;
    char_type field_14AB;
};
