#pragma once

#include "types.hpp"
#include "Function.hpp"

class Car_10
{
public:
    Car_10 *sub_439CD0(s32 *a2);
    void sub_439D40(Car_10 *a1, s32 a2, s32 *a3);
    s32 sub_439DA0(s32 edx0, s32 *a3);
    s32 sub_439E40(u8 a2);
    s32 sub_439E60();
    s32 sub_439EA0();
    Car_10();  // 447330
    ~Car_10(); // 447350

    s8 field_0;
    s8 field_1;
    s8 field_2;
    s8 field_3;
    u32 field_4;
    u32 field_8;
    s8 field_C;
    s8 field_D;
    s8 field_E;
    s8 field_F;
};
GTA2_ASSERT_SIZEOF_ALWAYS(Car_10, 0x10)
