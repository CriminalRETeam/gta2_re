#pragma once

#include "types.hpp"
#include "Function.hpp"


class Car_10
{
public:
    Car_10* Car_10::sub_439CD0(s32* a2);
    void Car_10::sub_439D40(Car_10* a1, s32 a2, s32* a3);
    s32 Car_10::sub_439DA0(s32 edx0, s32* a3);
    s32 Car_10::sub_439E40(u8 a2);
    s32 Car_10::sub_439E60();
    s32 Car_10::sub_439EA0();
    Car_10(); // 447330
    ~Car_10(); // 447350

    char_type field_0;
    char_type field_1;
    char_type field_2;
    char_type field_3;
    s32 field_4;
    char_type field_8;
    char_type field_9;
    u8 field_A;
    char_type field_B;
    s32 field_C;
};
GTA2_ASSERT_SIZEOF_ALWAYS(Car_10, 0x10)
