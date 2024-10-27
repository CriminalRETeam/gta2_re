#pragma once

#include "types.hpp"

class nostalgic_ellis_0x28
{
public:
    EXPORT nostalgic_ellis_0x28();

    EXPORT ~nostalgic_ellis_0x28();

    EXPORT  void sub_4D6D70();

    EXPORT nostalgic_ellis_0x28* sub_4D6DC0();

    nostalgic_ellis_0x28 *field_0;
    s32 field_4_light_x;
    s32 field_8_light_y;
    s32 field_C_light_z;
    s32 field_10_argb;
    char_type field_14_on_time;
    char_type field_15_off_time;
    char_type field_16_shape;
    char_type field_17_off_time;
    char_type field_18_intensity;
    char_type field_19;
    s16 field_1A;
    nostalgic_ellis_0x28 *field_1C;
    nostalgic_ellis_0x28 *field_20;
    nostalgic_ellis_0x28 *field_24;
};

class Light
{
public:
    EXPORT static void __stdcall sub_4D6E00();

    EXPORT static void sub_4D6E30();

    EXPORT static s32 __stdcall sub_4D6E50(s32 a1, s32 a2, s32 a3, s32 a4);
};