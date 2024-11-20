#pragma once

#include "Function.hpp"

class Car_BC;
class infallible_turing;

class Snooky_30
{
  public:
    EXPORT Snooky_30(); // 4882D0
    EXPORT void dtor_4882E0();
    EXPORT void sub_488310(Car_BC* a2);
    EXPORT void sub_488350();
    EXPORT infallible_turing* sub_4885A0(s32 a2, s32 a3, char_type a4);
    s32 field_0;
    s32 field_4;
    s32 field_8;
    s32 field_C;
    s32 field_10;
    Car_BC* field_14_pObj;
    infallible_turing* field_18;
    s32 field_1C;
    s32 field_20;
    s32 field_24;
    s32 field_28;
    s32 field_2C;
};

class Snooky_94
{
  public:
    EXPORT char_type sub_4887A0(Car_BC* toFind);
    EXPORT void sub_4887D0(Car_BC* a2, u8 idx);
    EXPORT void sub_4887F0();
    EXPORT Snooky_30* sub_488820(s32 a2, s32 a3);
    EXPORT Snooky_94* ctor_488860();
    EXPORT void dtor_4BBC30();
    Snooky_30 field_0[3];
    s32 field_90_count;
};
