#pragma once

#include "Function.hpp"

class Wolfy_30
{
  public:
    EXPORT Wolfy_30();
    EXPORT ~Wolfy_30();
    EXPORT s32 sub_543650();
    s32 field_0;
    char_type field_4_idx;
    char_type field_5;
    s16 field_6;
    s32 field_8;
    s16 field_C;
    s16 field_E;
    s32 field_10;
    s32 field_14;
    s16 field_18;
    s16 field_1A;
    s32 field_1C;
    s16 field_20;
    s16 field_22;
    s32 field_24;
    s32 field_28;
    s32 field_2C;
};

class Wolfy_7A8
{
  public:
    EXPORT s32 sub_543690();
    EXPORT Wolfy_30* sub_543800();
    EXPORT Wolfy_7A8();
    EXPORT ~Wolfy_7A8();
    Wolfy_30 field_0[40];
    char_type field_780[40];
};

class Wolfy_3D4
{
  public:
    EXPORT Wolfy_3D4();
    EXPORT ~Wolfy_3D4();
    Wolfy_30 field_0[20];
    char_type field_3C0[20];
};
