#pragma once

#include "Function.hpp"
#include "fix16.hpp"

class Kfc_30;
class Ped;
class Car_BC;
class Object_2C;
class Police_7C;

class Police_38
{
  public:
    EXPORT Police_38();
    EXPORT ~Police_38();
    EXPORT char_type sub_570790(Police_38* a1, s32 a2);
    EXPORT void sub_5709C0();
    EXPORT void sub_570A10();
    EXPORT void sub_570AB0();
    EXPORT Kfc_30* sub_570BF0();
    EXPORT s32 sub_570E30();
    EXPORT s32 sub_571150();
    EXPORT u32* sub_571350();
    EXPORT char_type sub_571540();
    EXPORT char_type sub_571A30();
    EXPORT char_type sub_5720C0();
    EXPORT bool sub_572210();
    EXPORT char_type sub_572340();
    EXPORT void sub_572920();
    EXPORT void sub_574720();
    EXPORT char_type sub_574F10();
    EXPORT void sub_575200();
    EXPORT void sub_575210();
    EXPORT void sub_575270();
    EXPORT void sub_5752C0();
    EXPORT void sub_575310();
    EXPORT void sub_575590();
    EXPORT s32 sub_575650();

    char_type field_0;
    char_type field_1;
    char_type field_2_targ_x;
    char_type field_3_targ_y;
    char_type field_4_targ_z;
    char_type field_5;
    char_type field_6;
    char_type field_7;
    s32 field_8;
    s32 field_C;
    Kfc_30* field_10_subObj;
    Police_7C* field_14_pObj;
    s16 field_18;
    char_type field_1A;
    char_type field_1B;
    s32 field_1C;
    s32 field_20;
    s32 field_24_state;
    char_type field_28;
    char_type field_29;
    s16 field_2A;
    s16 field_2C;
    char_type field_2E;
    char_type field_2F;
    s32 field_30;
    char_type field_34;
    char_type field_35;
    char_type field_36;
    char_type field_37;
};

class Police_7C
{
  public:
    Ped* field_0;
    s32 field_4;
    s32 field_8;
    s16 field_C;
    char_type field_E;
    char_type field_F;
    Fix16 field_10_x;
    Fix16 field_14_y;
    Fix16 field_18_z;
    char_type field_1C;
    char_type field_1D;
    char_type field_1E;
    char_type field_1F;
    Police_38* field_20[2];
    s32 field_28;
    s32 field_2C;
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
    s32 field_60;
    s32 field_64;
    s32 field_68;
    s32 field_6C;
    char_type field_70;
    char_type field_71;
    char_type field_72;
    char_type field_73;
    char_type field_74;
    char_type field_75_count;
    s16 field_76;
    char_type field_78;
    char_type field_79;
    s16 field_7A_kMax900;
};

class PoliceRoadblock_A4
{
  public:
    EXPORT void sub_575710();
    EXPORT void sub_5757B0();
    EXPORT void sub_575CA0();
    EXPORT char_type sub_575FF0(u8 a2, s32 a3, u8 a4, s32 a5);
    EXPORT PoliceRoadblock_A4();
    EXPORT ~PoliceRoadblock_A4();
    char_type field_0;
    char_type field_1;
    char_type field_2;
    char_type field_3;
    s32 field_4;
    u8 field_8;
    u8 field_9;
    char_type field_A;
    char_type field_B;
    u16 field_C;
    s16 field_E;
    Car_BC* field_10_car_1;
    Car_BC* field_14_car_2;
    Car_BC* field_18_car_3;
    Car_BC* field_1C_car_4;
    Car_BC* field_20_car_5;
    Car_BC* field_24_car_6;
    Object_2C* field_28_barrier_1;
    Object_2C* field_2C_barrier_2;
    Object_2C* field_30_barrier_3;
    Object_2C* field_34_barrier_4;
    Object_2C* field_38_barrier_5;
    Object_2C* field_3C_barrier_6;
    Object_2C* field_40_barrier_7;
    Object_2C* field_44_barrier_8;
    Object_2C* field_48_barrier_9;
    Object_2C* field_4C_barrier_10;
    Object_2C* field_50_barrier_11;
    Object_2C* field_54_barrier_12;
    s32 field_58;
    s32 field_5C;
    s32 field_60;
    s32 field_64;
    s32 field_68;
    s32 field_6C;
    s32 field_70;
    s32 field_74;
    s32 field_78;
    s32 field_7C;
    s32 field_80;
    s32 field_84;
    Ped* field_88_guard_1;
    Ped* field_8C_guard_2;
    Ped* field_90_guard_3;
    Ped* field_94_guard_4;
    Ped* field_98_guard_5;
    Ped* field_9C_guard_6;
    u8* field_A0_pMem;
};