#pragma once

#include "Function.hpp"
#include "fix16.hpp"

struct object_info;
class Sprite;

// This seems to be a temporary type that stores to be converted into Phi_74
class Phi_54
{
  public:
    // Temporary ctor, otherwise it won't be possible to define Phi_54_array_006FC5F8.
    EXPORT Phi_54()
    {}
    EXPORT Phi_54(s32 param_1,
                  s32& param_2,
                  s32 param_3,
                  s8 param_4,
                  s32& param_5,
                  s32& param_6,
                  s32& param_7,
                  s32 param_8,
                  s32 param_9,
                  s8 param_10,
                  s32& param_11,
                  s32& param_12,
                  s8 param_13,
                  s32& param_14,
                  s32 param_15,
                  s32 param_16,
                  s32 param_17,
                  s32 param_18,
                  s8 param_19,
                  s32 param_20,
                  s8 param_21);

    s32 field_0;
    s32 field_4;
    s32 field_8;
    s8 field_C;
    s8 field_D;
    s8 field_E;
    s8 field_F;
    s32 field_10;
    s32 field_14;
    s32 field_18;
    s32 field_1C;
    s32 field_20;
    s8 field_24;
    s8 field_25;
    s8 field_26;
    s8 field_27;
    s32 field_28;
    s32 field_2C;
    s8 field_30;
    s8 field_31;
    s8 field_32;
    s8 field_33;
    s32 field_34;
    s32 field_38;
    s32 field_3C;
    s8 field_40;
    s8 field_41;
    s8 field_42;
    s8 field_43;
    s32 field_44;
    s8 field_48;
    s8 field_49;
    s8 field_4A;
    s8 field_4B;
    s32 field_4C;
    s8 field_50;
    s8 field_51;
    s8 field_52;
    s8 field_53;
};

class Phi_74
{
  public:
    EXPORT ~Phi_74();
    EXPORT void sub_533060(Fix16 a2, Fix16 a3, Fix16 a4);
    EXPORT void sub_533090();
    EXPORT void sub_533110(s16 a2);
    EXPORT void sub_533150(s16 a2, s16 a3);
    EXPORT Sprite* sub_533170();
    EXPORT void sub_5331A0(Sprite* a2);
    EXPORT Phi_74();

    Fix16 field_0;
    Fix16 field_4;
    Fix16 field_8;
    Fix16 field_C;
    Fix16 field_10;
    Fix16 field_14;
    Fix16 field_18;
    s16 field_1C;
    s16 field_1E;
    char_type field_20;
    char_type field_21;
    char_type field_22;
    char_type field_23;
    s32 field_24_idx;
    s32 field_28;
    Sprite* field_2C;
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
    u8 field_6C;
    char_type field_6D;
    char_type field_6E;
    char_type field_6F;
    s32 field_70;
};

class Phi_8CA8
{
  public:
    EXPORT ~Phi_8CA8();
    EXPORT Phi_74* sub_5332D0(s32 idx, s32 a3, s16 a4, u8 a5);
    EXPORT void sub_533300();
    EXPORT void sub_533360();
    EXPORT void sub_533420();
    EXPORT s32 sub_533B30();
    EXPORT void sub_533C90();
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
