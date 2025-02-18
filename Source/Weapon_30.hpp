#pragma once

#include "Function.hpp"

class Player;
class Object_2C;
class Ped;
class Fix16;
class Sprite;
class Car_BC;

class infallible_turing;

class Weapon_30
{
  public:
    EXPORT Weapon_30();
    EXPORT ~Weapon_30();
    EXPORT void sub_5DCD90();
    EXPORT void sub_5DCDE0();
    EXPORT void sub_5DCE20(u8 a2);
    EXPORT char_type sub_5DCE40(u8 a2);
    EXPORT bool sub_5DCEA0();
    EXPORT bool sub_5DCEF0();
    EXPORT Player* sub_5DCF40();
    EXPORT Object_2C* sub_5DCF60(s32 a2, s32 a3, s32 a4, s32 a5, s16 a6, s32* a7);
    EXPORT void sub_5DD0F0();
    EXPORT u8 sub_5DD290();
    EXPORT char_type sub_5DD860();
    EXPORT u8 sub_5DDA70();
    EXPORT u8 sub_5DDD20();
    EXPORT void sub_5DDFC0(Fix16* a2, char_type* a3, s32 a4);
    EXPORT s32 sub_5DE4F0();
    EXPORT char_type sub_5DFB60(char_type a2, Sprite* a3, s16 a4);
    EXPORT void sub_5E06B0();
    EXPORT char_type sub_5E0740();
    EXPORT void sub_5E0AB0(char_type a2);
    EXPORT char_type sub_5E0B10();
    EXPORT s32* sub_5E0E70();
    EXPORT u8 sub_5E10E0();
    EXPORT u8 sub_5E13E0();
    EXPORT char_type sub_5E1DC0();
    EXPORT char_type sub_5E2550();
    EXPORT u8 sub_5E2940();
    EXPORT char_type sub_5E33C0();
    EXPORT char_type sub_5E34B0();
    EXPORT void sub_5E3670();
    EXPORT void sub_5E3850();
    EXPORT char_type sub_5E3BD0();

    u16 field_0;
    s8 field_2;
    s8 field_3;
    s32 field_4;
    s32 field_8;
    s32 field_C;
    s16 field_10;
    s16 field_12;
    Car_BC* field_14;
    Weapon_30* field_18;
    s32 field_1C_idx;
    s8 field_20;
    s8 field_21;
    s8 field_22;
    s8 field_23;
    Ped* field_24_pObj;
    infallible_turing* field_28;
    s8 field_2C;
    s8 field_2D;
    s8 field_2E;
    s8 field_2F;
};
