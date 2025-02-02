#pragma once

#include "Function.hpp"

class Ped;

class Car_10
{
  public:
    EXPORT void sub_439CD0(s32* a2);
    EXPORT void sub_439D40(s32* a3);
    EXPORT void sub_439DA0(s32* a3);
    EXPORT void sub_439E40(u8 a2);
    EXPORT void sub_439E60();
    EXPORT void sub_439EA0();
    EXPORT Car_10(); // 447330
    EXPORT ~Car_10(); // 447350

    s8 field_0;
    s8 field_1;
    s8 field_2;
    s8 field_3;
    s32 field_4_state;
    Ped* field_8_pObj;
    s8 field_C;
    s8 field_D;
    s8 field_E;
    s8 field_F;
};
GTA2_ASSERT_SIZEOF_ALWAYS(Car_10, 0x10)
