#pragma once

#include "Function.hpp"

class Car_3C;

class Object_3C
{
  public:
    EXPORT void ctor_52AD80();
    EXPORT void dtor_52ADE0();
    EXPORT u32* sub_52ADF0(u32* a2);
    EXPORT u32* sub_5A6CD0(Car_3C* a2);
    EXPORT s32 sub_5A6E10();
    EXPORT s32 sub_5A6E40(s32 a2, s32 a3);
    EXPORT void sub_5A6F70(Car_3C* a2);
    EXPORT s32 sub_5A7010();
    EXPORT void sub_5A7240(Car_3C* a2);
    EXPORT s32* sub_5A72B0(Car_3C* a2, char_type a3);
    EXPORT char_type sub_5A7310();

    s32 field_0;
    s16 field_4;
    s16 field_6;
    Object_3C* field_8;
    s32 field_C;
    s32 field_10;
    s32 field_14;
    s32 field_18;
    s32 field_1C;
    s32 field_20;
    s32 field_24;
    s16 field_28;
    s16 field_2A;
    s16 field_2C;
    char_type field_2E;
    char_type field_2F;
    char_type field_30;
    char_type field_31;
    s16 field_32;
    s32 field_34;
    char_type field_38;
    char_type field_39;
    char_type field_3A;
    char_type field_3B;
};
