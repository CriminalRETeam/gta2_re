#pragma once

#include "Function.hpp"

class Object_2C;

class Wolfy_30
{
  public:
    EXPORT Wolfy_30();
    EXPORT ~Wolfy_30();
    EXPORT void sub_540A40();
    EXPORT void sub_540D30(s32 a2, s32 a3);
    EXPORT void state_4_540F90(s32 a2, s32 a3);
    EXPORT void sub_5411E0(s32 a2, s32 a3);
    EXPORT void sub_541430(s32 a2, s32 a3);
    EXPORT u32* sub_541680(u32* a2);
    EXPORT u32* sub_541710(u32* a2);
    EXPORT char_type sub_541760();
    EXPORT void sub_541850(u16 a2);
    EXPORT char_type sub_541D60();
    EXPORT char_type sub_542060();
    EXPORT char_type sub_542340();
    EXPORT s16 sub_542790();
    EXPORT void sub_542E30(char_type a2);
    EXPORT char_type sub_5434A0(s32 a2, s32 a3);
    EXPORT char_type sub_5435D0();
    EXPORT void sub_543610();
    EXPORT void sub_543650();
    EXPORT void sub_543680(Object_2C* a2);

    s32 field_0;
    u16 field_4_idx;
    s16 field_6;
    s32 field_8;
    s16 field_C;
    s16 field_E;
    s32 field_10;
    Object_2C* field_14;
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
