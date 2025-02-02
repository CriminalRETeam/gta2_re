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
    EXPORT s32 sub_47F290(s32 a2, s32 a3, Sprite* a4);
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
    s32 field_84;
    s32 field_88;
    s32 field_8C;
    s32 field_90;
    s32 field_94;
    s32 field_98;
    s32 field_9C;
    s32 field_A0;
    s32 field_A4;
    s32 field_A8;
    s32 field_AC;
    s32 field_B0;
    s32 field_B4;
    s32 field_B8;
    s32 field_BC;
    s32 field_C0;
    Fix16 field_C4;
    Fix16 field_C8;
    s32 field_CC;
    s32 field_D0;
    s32 field_D4;
    s32 field_D8;
    s32 field_DC;
    Fix16 field_E0;
    Fix16 field_E4;
    Fix16 field_E8;
    s32 field_EC;
    s32 field_F0;
    s32 field_F4;
    s32 field_F8;
    Fix16 field_FC;
    Fix16 field_100;
    Fix16 field_104;
    s32 field_108;
    s32 field_10C;
    s32 field_110;
    s32 field_114;
    s32 field_118;
    s32 field_11C;
    s32 field_120;
    s32 field_124;
    s32 field_128;
    s32 field_12C;
    s32 field_130;
    s32 field_134;
    s32 field_138;
    s32 field_13C;
    s16 field_140;
    s16 field_142;
    s32 field_144;
    s32 field_148;
    char_type field_14C;
    char_type field_14D;
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
