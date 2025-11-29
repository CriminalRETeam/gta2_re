#pragma once

#include "Function.hpp"

class infallible_turing;
class Sprite;
class Car_BC;

class Rozza_A
{
  public:
    s32 field_0;
    s32 field_4_x;
    s32 field_8_y;
    s32 field_C_z;
    Car_BC* field_10;
    s32 field_14;
    s32 field_18;
    s32 field_1C;
    s32 field_20;
    s32 field_24;
};

class Rozza_C88
{
  public:
    EXPORT void Reset_40BB90();
    EXPORT void sub_40BBA0(Sprite* a2, s32 a3);
    EXPORT void sub_40BC40(Sprite* a2);
    EXPORT void sub_40BD10(Sprite* a2);
    EXPORT void sub_40BDD0(Sprite* a2, Sprite* a3);
    EXPORT Rozza_C88();
    EXPORT ~Rozza_C88();

    infallible_turing* field_0_pSoundObj;
    Rozza_A field_4[80];
    s32 field_C84_count;
};

EXTERN_GLOBAL(Rozza_C88*, gRozza_C88_66AFE0);
