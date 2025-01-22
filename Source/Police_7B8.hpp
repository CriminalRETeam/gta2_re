#pragma once

#include "Function.hpp"

class cool_nash_0x294;
class Car_BC;
class Kfc_30;

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
    s32 field_14_pObj;
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
    cool_nash_0x294* field_0;
    s32 field_4;
    s32 field_8;
    s16 field_C;
    char_type field_E;
    char_type field_F;
    s32 field_10;
    s32 field_14;
    s32 field_18;
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

class Police_A4
{
  public:
    EXPORT void sub_575710();
    EXPORT void sub_5757B0();
    EXPORT void sub_575CA0();
    EXPORT char_type sub_575FF0(u8 a2, s32 a3, u8 a4, s32 a5);
    EXPORT Police_A4();
    EXPORT ~Police_A4();
    char_type field_0;
    char_type field_1;
    char_type field_2;
    char_type field_3;
    s32 field_4;
    char_type field_8;
    char_type field_9;
    char_type field_A;
    char_type field_B;
    s16 field_C;
    s16 field_E;
    s32 field_10;
    s32 field_14;
    s32 field_18;
    s32 field_1C;
    s32 field_20;
    s32 field_24;
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
    s32 field_70;
    s32 field_74;
    s32 field_78;
    s32 field_7C;
    s32 field_80;
    s32 field_84;
    cool_nash_0x294* field_88;
    cool_nash_0x294* field_8C;
    cool_nash_0x294* field_90;
    cool_nash_0x294* field_94;
    cool_nash_0x294* field_98;
    cool_nash_0x294* field_9C;
    u8* field_A0_pMem;
};

class Police_7B8
{
  public:
    Police_7B8()
    {
        sub_56F400();
    }

    EXPORT ~Police_7B8();
    EXPORT s32 sub_56F400();
    EXPORT char_type sub_56F4D0(cool_nash_0x294* a2);
    EXPORT Police_38* sub_56F560();
    EXPORT cool_nash_0x294* sub_56F5C0(s32 a2, s32 a3, s16 a4);
    EXPORT void sub_56F6D0(Car_BC* a2);
    EXPORT bool sub_56F800(cool_nash_0x294* a2);
    EXPORT bool sub_56F880(cool_nash_0x294* a2);
    EXPORT s32 sub_56F8E0(cool_nash_0x294* a2, cool_nash_0x294* a3);
    EXPORT s32 sub_56F940(u32* a2);
    EXPORT s16 sub_56FA40();
    EXPORT char_type sub_56FAA0(Police_7C* a2);
    EXPORT void sub_56FBD0();
    EXPORT void sub_570270();
    EXPORT s32 sub_570320(cool_nash_0x294* a2, s32 a3, s32 a4, s32 a5, s16 a6);
    EXPORT char_type sub_5703E0(Car_BC* a2);
    EXPORT char_type sub_5707B0(s32 a2, s32 a3);
    EXPORT s32 sub_5708C0(Police_7B8* a1, s32 a2, u32* a3);
    EXPORT void sub_570940(Police_7B8* a1, s32 a2, cool_nash_0x294* a3);
    EXPORT char_type sub_577320();
    EXPORT void sub_577370(u8 a2, s32 a3, s32 a4);

    s32 field_0;
    Police_38 field_4[20];
    Police_7C field_464[4];
    s32 field_654_wanted_level;
    char_type field_658;
    char_type field_659;
    char_type field_65A;
    char_type field_65B;
    s32 field_65C;
    u8 field_660_wanted_star_count;
    char_type field_661;
    char_type field_662;
    char_type field_663;
    Police_A4 field_664_obj;
    Police_A4 field_708_obj;
    char_type field_7AC;
    char_type field_7AD_police_peds_in_range_screen;
    char_type field_7AE;
    char_type field_7AF;
    cool_nash_0x294* field_7B0;
    char_type field_7B4;
    char_type field_7B5;
    char_type field_7B6;
    char_type field_7B7;
};

EXPORT_VAR extern class Police_7B8* gPolice_7B8_6FEE40;
