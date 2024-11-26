#pragma once

#include "Function.hpp"

class object_info;
class Car_3C;

class Phi_74
{
  public:
    EXPORT ~Phi_74();
    EXPORT s32 sub_533060(s32 a2, s32 a3, s32 a4);
    EXPORT s32 sub_533090();
    EXPORT void sub_533110(s16 a2);
    EXPORT s16 sub_533150(s16 a2, char_type a3);
    EXPORT Car_3C* sub_533170();
    EXPORT s32 sub_5331A0(Car_3C* a2);
    EXPORT Phi_74();

    s32 field_0;
    s32 field_4;
    s32 field_8;
    s32 field_C;
    s32 field_10;
    s32 field_14;
    s32 field_18;
    s16 field_1C;
    s16 field_1E;
    char_type field_20;
    char_type field_21;
    char_type field_22;
    char_type field_23;
    s32 field_24_idx;
    s32 field_28;
    Car_3C* field_2C;
    s32 field_30;
    s32 field_34;
    s32 field_38;
    s32 field_3C;
    s32 field_40;
    s32 field_44;
    s32 field_48;
    s32 field_4C;
    s32 field_50;
    s32 field_54;
    s32 field_58;
    s32 field_5C;
    char_type field_60;
    char_type field_61;
    char_type field_62;
    char_type field_63;
    char_type field_64;
    char_type field_65;
    char_type field_66;
    char_type field_67;
    s32 field_68;
    char_type field_6C;
    char_type field_6D;
    char_type field_6E;
    char_type field_6F;
    s32 field_70;
};

class Phi_8CA8
{
  public:
    EXPORT void dtor_4BDF40();
    EXPORT Phi_74* sub_5332D0(s32 a2, s32 a3, s16 a4, char_type a5);
    EXPORT object_info* sub_533300();
    EXPORT void sub_533360();
    EXPORT Phi_74* sub_533420();
    EXPORT s32 sub_533B30();
    EXPORT u8 sub_533C90();
    EXPORT void sub_534270();
    EXPORT void sub_5342D0();
    EXPORT void sub_5342F0(s32 idx);
    EXPORT void sub_534330();
    EXPORT Phi_74* sub_534360(s32 idx);
    EXPORT Phi_74* sub_534370(s32 idx1, s32 idx2);
    EXPORT Phi_74* sub_5343C0(s32 idx);
    EXPORT Phi_8CA8();
    u16 field_0_next_idx;
    s16 field_2;
    Phi_74 field_4[300];
    Phi_74* field_87F4[300];
    s16 field_8CA4;
    s16 field_8CA6;
};
