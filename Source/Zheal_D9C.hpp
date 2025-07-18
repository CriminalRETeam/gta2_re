#pragma once

#include "Car_BC.hpp"
#include "Function.hpp"
#include "Object_5C.hpp"
#include "ang16.hpp"
#include "fix16.hpp"

class infallible_turing;
class Car_BC;

class Crane_15C
{
  public:
    EXPORT ~Crane_15C();
    EXPORT Crane_15C();
    EXPORT s32 sub_47E620(s32 a2, s32 a3, s32* a4);
    EXPORT s32 sub_47E730(s32 a2, s32 a3, u32 a4);
    EXPORT s32 sub_47E840(s32 a2, s32 a3);
    EXPORT bool sub_47E920();
    EXPORT bool sub_47EB00();
    EXPORT s32 sub_47ECC0();
    EXPORT s32 sub_47ED60();
    EXPORT s32 sub_47EDF0();
    EXPORT s32 sub_47EF80();
    EXPORT s32 sub_47F170();
    EXPORT s32 sub_47F220(s32 a2, s32 a3, Sprite* a4, Sprite* a5);
    EXPORT void sub_47F290(Fix16 a2, Fix16 a3, Sprite* a4);
    EXPORT void sub_47F2F0(s32 a2, s32 a3, Sprite* a4);
    EXPORT bool sub_47F350();
    EXPORT bool sub_47F3D0();
    EXPORT bool sub_47F450();
    EXPORT void sub_47F4C0();
    EXPORT char_type sub_47F6C0(u32* a2, s32* a3, u32* a4);
    EXPORT char_type sub_47F7F0(u32* a2);
    EXPORT void sub_47F930(Car_BC* a2);
    EXPORT s32 sub_47FB40();
    EXPORT s32 sub_47FBA0();
    EXPORT s32 sub_47FD10();
    EXPORT char_type sub_47FD50();
    EXPORT void sub_47FE10();
    EXPORT void sub_480310();
    EXPORT infallible_turing* sub_4803B0(Fix16 a2, Fix16 a3, char_type a4);
    EXPORT s32* sub_480900(s32 a2, s32 a3, s32 a4);
    EXPORT s32* sub_480B60(s32 a2, s32 a3, s32 a4);
    EXPORT Car_BC* GetCarFromCrane_480DA0();

    s32 field_0;
    s32 field_4;
    s32 field_8;
    s32 field_C;
    s32 field_10;
    s32 field_14;
    s32 field_18;
    s32 field_1C;
    s32 field_20;
    s32 field_24;
    s32 field_28;
    Object_2C* field_2C;
    Object_2C* field_30;
    Object_2C* field_34;
    Object_2C* field_38;
    Object_2C* field_3C;
    Object_2C* field_40;
    Object_2C* field_44;
    Object_2C* field_48;
    Object_2C* field_4C;
    Object_2C* field_50;
    Object_2C* field_54;
    Object_2C* field_58;
    Object_2C* field_5C;
    Sprite* field_60;
    Sprite* field_64;
    Sprite* field_68;
    Sprite* field_6C;
    Sprite* field_70;
    Sprite* field_74;
    s32 field_78;
    infallible_turing* field_7C;
    Fix16 field_80;
    Fix16 field_84_hook_depth;
    Fix16 field_88;
    Fix16 field_8C_crane_angle; //  It's not Ang16, maybe Ang32
    Fix16 field_90_hook_radius; //  Radial distance
    Fix16 field_94;
    Fix16 field_98;
    Fix16 field_9C;
    Fix16 field_A0_hook_axial_angle;  // The angle in its own rotation axis, e.g. the angle of the lifted car
    Fix16 field_A4;
    Fix16 field_A8;
    Fix16 field_AC_crane_angle_target;  // the crane will rotate until match this angle
    Fix16 field_B0_hook_radius_target;
    Fix16 field_B4_hook_angle_target;
    Fix16 field_B8_hook_depth_target;
    Fix16 field_BC;
    Fix16 field_C0;
    Fix16 field_C4;
    Fix16 field_C8;
    Fix16 field_CC;
    Fix16 field_D0;
    Fix16 field_D4;
    s32 field_D8;
    s32 field_DC;
    Fix16 field_E0;
    Fix16 field_E4;
    Fix16 field_E8;
    Fix16 field_EC;
    Fix16 field_F0;
    s32 field_F4;
    s32 field_F8;
    Fix16 field_FC;
    Fix16 field_100;
    Fix16 field_104;
    Fix16 field_108;
    Fix16 field_10C;
    Fix16 field_110;  // something to do with the crane angle
    Fix16 field_114;  // something to do with the hook radius
    Fix16 field_118;
    Fix16 field_11C;
    Fix16 field_120;
    Fix16 field_124;
    Fix16 field_128;
    Fix16 field_12C;
    Fix16 field_130;
    Fix16 field_134;
    Fix16 field_138;
    Fix16 field_13C;
    s16 field_140;
    s16 field_142;
    s32 field_144;
    s32 field_148;
    char_type field_14C;
    char_type field_14D_is_busy;
    char_type field_14E;
    char_type field_14F;
    s32 field_150;
    char_type field_154;
    char_type field_155;
    char_type field_156;
    char_type field_157;
    char_type field_158;
    char_type field_159;
    char_type field_15A;
    char_type field_15B;
};

class Zheal_D9C
{
  public:
    EXPORT void sub_480E00(Car_BC* a2, u8 a3);
    EXPORT void sub_480E50();
    EXPORT Crane_15C* sub_480EC0(Fix16 a2, Fix16 a3);
    EXPORT Zheal_D9C();
    EXPORT ~Zheal_D9C();

    Crane_15C field_0[10];
    s32 field_D98_count;
};

EXTERN_GLOBAL(Zheal_D9C*, gZheal_D9C_679FD4);
