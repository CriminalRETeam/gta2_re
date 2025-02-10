#pragma once

#include "Function.hpp"
#include "fix16.hpp"
#include "ang16.hpp"
#include "map_0x370.hpp"
#include <wchar.h>

class Ped;
class Player;
class Gang_144;
class infallible_turing;
class Gang_144;
class Object_2C;

class Garox_13C0_sub
{
  public:
    EXPORT void sub_5CFE40();
    s32 field_13C0;
};

class Garox_1_v2
{
  public:
    EXPORT void sub_5D56B0();
    EXPORT void sub_5D56D0();
    EXPORT void sub_5D5730(const wchar_t* pStr);
    EXPORT void sub_5D5760();
    EXPORT void sub_5D5770(u8* a2, u8* a3);
    EXPORT Garox_1_v2();
    char_type field_0_timer;
    char_type field_1;
    wchar_t field_2_str[101];
    s32 field_CC;
};

class Garox_2A25_sub
{
  public:
    EXPORT char_type sub_5D15E0(s32 a1, char_type* a2);
    EXPORT void sub_5D16B0();
    EXPORT s32 sub_5D17D0(s32 a2);
    EXPORT void sub_5D1830(Player* a1);
    char_type field_2A25;
};

class Garox_12EC_sub
{
  public:
    EXPORT char_type sub_5D13C0(s32 a2, char_type* a3);
    EXPORT void sub_5D1430();
    EXPORT s32 sub_5D15A0(s32 a1);
    EXPORT void sub_5D15D0(Player* pPlayer);
    char_type field_12EC_sub;
};

class Garox_4
{
  public:
    // inline 0x4C6AC0
    Garox_4()
    {
        field_0_value = 0; // TODO: byte ?
    }
    EXPORT void sub_5CF620();
    EXPORT void sub_5CF6B0();
    s32 field_0_value;
};

class Garox_12E4_sub
{
  public:
    // inline 0x4C71A0
    Garox_12E4_sub()
    {
        field_12E4 = 0;
        field_1 = 45;
    }
    EXPORT void sub_5D63B0();
    char_type field_12E4;
    char_type field_1;
};

class Hud_Message_1C8
{
  public:
    EXPORT void sub_5D1850();
    EXPORT void sub_5D1860();
    EXPORT void sub_5D1940();
    EXPORT void sub_5D1A00(wchar_t* pStr, s32 a3);
    EXPORT void sub_5D1AB0();
    EXPORT Hud_Message_1C8();
    u8 field_0_time_to_show;
    char_type field_1_unk;
    wchar_t field_2_str[221];
    s32 field_1BC_str_width;
    s32 field_1C0_num_lines;
    s32 field_1C4_type;
};

class Garox_1118_sub
{
  public:
    EXPORT void sub_5D5C80();
    EXPORT void sub_5D6290();
    s32 field_1118;
};

class Garox_110C_sub
{
  public:
    // inline 0x4C6E50
    Garox_110C_sub()
    {
        field_284E = 0;
    }
    EXPORT void sub_5CF730();
    EXPORT void sub_5CF910();
    s32 field_110C;
    s32 field_1110;
    Ang16 field_1114;
    char_type field_284E;
    char_type field_284F;
};

class Garox_1108_sub
{
  public:
    EXPORT void sub_5D0260();
    s32 field_1108;
};

class Garox_1
{
  public:
    EXPORT void sub_5D53E0();
    EXPORT void sub_5D5420();
    EXPORT void sub_5D5600(u8 a2);
    EXPORT void sub_5D5690();
    EXPORT Garox_1();
    char_type field_0_timer;
    char_type field_1;
    wchar_t field_2_str[65];
    s32 field_84;
};

class Garox_27B5_sub
{
  public:
    // inline 0x4C6E70
    Garox_27B5_sub()
    {
        field_27B5_sub = 0;
    }
    EXPORT void sub_5CF970();
    char_type field_27B5_sub;
};

class Garox_107C_sub
{
  public:
    EXPORT void sub_5CFA70();
    EXPORT void sub_5CFE20();
    EXPORT void Empty_5CFE30();
    char_type field_107C_sub;
};

class Hud_CopHead_C
{
  public:
    EXPORT void sub_5D0050(char_type a2);
    EXPORT Hud_CopHead_C();
    u8 field_0;
    char_type field_1;
    char_type field_2;
    char_type field_3;
    s32 field_4;
    s32 field_8;
};

class Hud_CopHead_C_Array
{
  public:
    // inline 0x4C6EE0
    Hud_CopHead_C_Array()
    {
        field_48_cop_level = 0;
    }
    EXPORT void sub_5D00B0();
    EXPORT void sub_5D0110();
    EXPORT void sub_5D0210();
    Hud_CopHead_C field_1028[6];
    s32 field_48_cop_level;
    Fix16 field_4C_w_fp;
    Fix16 field_50_h_fp;
};

class Garox_C4
{
  public:
    EXPORT void sub_5D1B10(const wchar_t* pStr, s16 a3, s16 a4, s16 a5, s32 displayTime);
    EXPORT void sub_5D1D00();
    EXPORT bool sub_5D1DB0();
    EXPORT bool operator_equals_5D1E10(Garox_C4* pOther);
    wchar_t field_0_str_buf[82];
    s32 field_A4_display_time;
    s16 field_A8;
    s16 field_AA;
    s16 field_AC;
    s16 field_AE;
    s32 field_B0;
    s16 field_B4;
    s16 field_B6;
    s32 field_B8;
    char_type field_BC;
    char_type field_BD;
    char_type field_BE;
    char_type field_BF;
    Garox_C4* field_C0_pNext;
};

class Garox_1700_L
{
  public:
    EXPORT void sub_5D1EB0(Garox_C4* String2);
    EXPORT Garox_C4* sub_5D1F50(const wchar_t* pStr, s16 a3, s16 a4, s16 a5, s32 a6);
    EXPORT void Service_5D2010();
    EXPORT void sub_5D2050();
    EXPORT Garox_1700_L();
    Garox_C4 field_0_29_ary[30];
    Garox_C4* field_960_pFirst;
    Garox_C4* field_964;
};

class Hud_Pager_C
{
  public:
    EXPORT ~Hud_Pager_C();
    EXPORT void sub_5D2320();
    EXPORT void sub_5D2380(s32 a2, s32 a3);
    EXPORT s32 sub_5D2680(s32 a2, s32 a3);
    EXPORT s32 sub_5D2AB0(s32 a2, s32 a3);

    // TODO: These might be part of Hud_Pager_C_Array too
    EXPORT s32 sub_5D31F0(s32 a2);
    EXPORT s32 sub_5D3220(s32 a2);
    EXPORT s32 sub_5D3280(s32 a2);

    EXPORT Hud_Pager_C* sub_5D32F0(s32 a2, s32 a3);

    EXPORT Hud_Pager_C();
    s32 field_0;  //  duration? timer?
    s32 field_4;  //  counter?
    infallible_turing* field_8_sound;
};

class Hud_Pager_C_Array
{
  public:
    // inline 0x4CA660
    Hud_Pager_C_Array()
    {
    }

    EXPORT void sub_5D3040();
    EXPORT void sub_5D31B0();
    EXPORT void sub_5D32D0(s32 a2);
    EXPORT void sub_5D3310(s32 a2);

    Hud_Pager_C field_620[4];
};

class Garox_18
{
  public:
    Garox_18* field_0;
    s32 field_4;
    s32 field_8;
    Garox_18* field_C;
    s32 field_10;
    s32 field_14;
};

class Garox_30_Sub
{
  public:
    Gang_144* field_30;
    char_type field_34;
    char_type field_5;
    char_type field_6;
    char_type field_7;
};

class ArrowTrace_24
{
  public:
    EXPORT void sub_5D03C0(Gang_144* pZone);

    // inline 0x4C6F00
    void init()
    {
        field_0 = 0;
        field_4 = 0;
        field_8 = 0;
        field_C = 0;
        field_10_type = 0;
        field_20 = 0;
    }

    s32 field_0;
    Player* field_4;
    Object_2C* field_8;
    Player* field_C;
    s32 field_10_type;
    Fix16 field_14_aim_x;
    Fix16 field_18_aim_y;
    Fix16 field_1C_aim_z;
    char_type field_20;
    char_type field_21_pad;
    char_type field_22_pad;
    char_type field_23_pad;
};

class Garox_20_Sub
{
  public:
    s32 field_20;
    s16 field_24;
    char_type field_26;
    char_type field_27;
    s32 field_28;
    s16 field_2C;
    char_type field_2E;
    char_type field_2F;
    Garox_30_Sub field_10;
    ArrowTrace_24 field_18;
    ArrowTrace_24 field_3C;
    ArrowTrace_24* field_60;
};

class Hud_Arrow_7C
{
  public:
    EXPORT void sub_5D0510(s32 a2);
    EXPORT char_type sub_5D0530();
    EXPORT char_type sub_5D0620();
    EXPORT s32 sub_5D0850();
    EXPORT void sub_5D0C60();
    EXPORT void sub_5D0C90();
    EXPORT void sub_5D0DC0(Ped* a2);

    EXPORT Hud_Arrow_7C();
    Fix16 field_0_pos_x; // maybe screen x
    Fix16 field_0_pos_y; // maybe screen y
    Ang16 field_8_rotation;
    s16 field_A;
    s32 field_C;
    s32 field_10;
    s32 field_14;
    Garox_20_Sub field_18;
};

class Hud_Arrow_7C_Array
{
  public:
    // inline 0x4C7080
    Hud_Arrow_7C_Array()
    {
        field_83C = 1;
        field_840 = 0;
        field_844 = 0;
    }

    EXPORT void sub_5D1350();
    EXPORT char_type sub_5D0E40(s32* a2);
    EXPORT void sub_5D0E90();
    EXPORT Hud_Arrow_7C* sub_5D0EF0();
    EXPORT char_type sub_5D0F40(Gang_144* a2);
    EXPORT void sub_5D0F80();
    EXPORT void sub_5D0FD0();
    EXPORT Hud_Arrow_7C* sub_5D1020(s32* a2);
    EXPORT Hud_Arrow_7C* sub_5D1050();
    EXPORT char_type* sub_5D10B0();
    EXPORT Hud_Arrow_7C* sub_5D10D0(Gang_144* pZone, s32 phone_type);
    EXPORT void place_gang_phone_5D1110(Object_2C* pPhoneInfo);
    EXPORT void sub_5D1310(Gang_144* pZone);
    Hud_Arrow_7C field_0_array[17];
    char_type field_83C;
    char_type field_83D;
    char_type field_83E;
    char_type field_83F;
    Hud_Arrow_7C* field_840;
    char_type field_844;
    char_type field_845;
    char_type field_846;
    char_type field_847;
};

class Hud_Brief_704   // not sure where to put this, maybe it's Garox_1E34_L, but it has size 0x704
{
  public:
    wchar_t field_0_str[640];
    char field_500[36];
    int* field_524;
    char field_528[452];
    char field_6EC[12];
    Hud_Brief_704* field_6F8_prev;
    int field_6FC;
    char* field_700;
  /*
  public:
    void SetHudBrief(int priority, const char* str);
    void Clear(int priority);
  */
};

class Garox_1E34_L  // size 0x620
{
  public:
    EXPORT s32 sub_5D3330();
    EXPORT char_type* sub_5D3350();
    EXPORT s32 sub_5D3370();
    EXPORT s32 sub_5D33A0();
    EXPORT s32 sub_5D33F0();
    EXPORT size_t sub_5D3470();
    EXPORT char_type sub_5D3680(s16 a1);
    EXPORT s32 sub_5D39D0();
    EXPORT void sub_5D3B80();
    EXPORT s32 sub_5D3F10(s32 a2, const char_type* a3, s32 a4);
    EXPORT s32 sub_5D4400(s32 a2, const char_type* a3);
    EXPORT void sub_5D44D0();
    EXPORT s32 sub_5D4850();
    EXPORT s32 sub_5D4890(s32 a2);
    EXPORT Garox_1E34_L();
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
    s32 field_C4;
    s32 field_C8;
    s32 field_CC;
    s32 field_D0;
    s32 field_D4;
    s32 field_D8;
    s32 field_DC;
    s32 field_E0;
    s32 field_E4;
    s32 field_E8;
    s32 field_EC;
    s32 field_F0;
    s32 field_F4;
    s32 field_F8;
    s32 field_FC;
    s32 field_100;
    s32 field_104;
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
    s32 field_140;
    s32 field_144;
    s32 field_148;
    s32 field_14C;
    s32 field_150;
    s32 field_154;
    s32 field_158;
    s32 field_15C;
    s32 field_160;
    s32 field_164;
    s32 field_168;
    s32 field_16C;
    s32 field_170;
    s32 field_174;
    s32 field_178;
    s32 field_17C;
    s32 field_180;
    s32 field_184;
    s32 field_188;
    s32 field_18C;
    s32 field_190;
    s32 field_194;
    s32 field_198;
    s32 field_19C;
    s32 field_1A0;
    s32 field_1A4;
    s32 field_1A8;
    s32 field_1AC;
    s32 field_1B0;
    s32 field_1B4;
    s32 field_1B8;
    s32 field_1BC;
    s32 field_1C0;
    s32 field_1C4;
    s32 field_1C8;
    s32 field_1CC;
    s32 field_1D0;
    s32 field_1D4;
    s32 field_1D8;
    s32 field_1DC;
    s32 field_1E0;
    s32 field_1E4;
    s32 field_1E8;
    s32 field_1EC;
    s32 field_1F0;
    s32 field_1F4;
    s32 field_1F8;
    s32 field_1FC;
    s32 field_200;
    s32 field_204;
    s32 field_208;
    s32 field_20C;
    s32 field_210;
    s32 field_214;
    s32 field_218;
    s32 field_21C;
    s32 field_220;
    s32 field_224;
    s32 field_228;
    s32 field_22C;
    s32 field_230;
    s32 field_234;
    s32 field_238;
    s32 field_23C;
    s32 field_240;
    s32 field_244;
    s32 field_248;
    s32 field_24C;
    s32 field_250;
    s32 field_254;
    s32 field_258;
    s32 field_25C;
    s32 field_260;
    s32 field_264;
    s32 field_268;
    s32 field_26C;
    s32 field_270;
    s32 field_274;
    s32 field_278;
    s32 field_27C;
    s32 field_280;
    s32 field_284;
    s32 field_288;
    s32 field_28C;
    s32 field_290;
    s32 field_294;
    s32 field_298;
    s32 field_29C;
    s32 field_2A0;
    s32 field_2A4;
    s32 field_2A8;
    s32 field_2AC;
    s32 field_2B0;
    s32 field_2B4;
    s32 field_2B8;
    s32 field_2BC;
    s32 field_2C0;
    s32 field_2C4;
    s32 field_2C8;
    s32 field_2CC;
    s32 field_2D0;
    s32 field_2D4;
    s32 field_2D8;
    s32 field_2DC;
    s32 field_2E0;
    s32 field_2E4;
    s32 field_2E8;
    s32 field_2EC;
    s32 field_2F0;
    s32 field_2F4;
    s32 field_2F8;
    s32 field_2FC;
    s32 field_300;
    s32 field_304;
    s32 field_308;
    s32 field_30C;
    s32 field_310;
    s32 field_314;
    s32 field_318;
    s32 field_31C;
    s32 field_320;
    s32 field_324;
    s32 field_328;
    s32 field_32C;
    s32 field_330;
    s32 field_334;
    s32 field_338;
    s32 field_33C;
    s32 field_340;
    s32 field_344;
    s32 field_348;
    s32 field_34C;
    s32 field_350;
    s32 field_354;
    s32 field_358;
    s32 field_35C;
    s32 field_360;
    s32 field_364;
    s32 field_368;
    s32 field_36C;
    s32 field_370;
    s32 field_374;
    s32 field_378;
    s32 field_37C;
    s32 field_380;
    s32 field_384;
    s32 field_388;
    s32 field_38C;
    s32 field_390;
    s32 field_394;
    s32 field_398;
    s32 field_39C;
    s32 field_3A0;
    s32 field_3A4;
    s32 field_3A8;
    s32 field_3AC;
    s32 field_3B0;
    s32 field_3B4;
    s32 field_3B8;
    s32 field_3BC;
    s32 field_3C0;
    s32 field_3C4;
    s32 field_3C8;
    s32 field_3CC;
    s32 field_3D0;
    s32 field_3D4;
    s32 field_3D8;
    s32 field_3DC;
    s32 field_3E0;
    s32 field_3E4;
    s32 field_3E8;
    s32 field_3EC;
    s32 field_3F0;
    s32 field_3F4;
    s32 field_3F8;
    s32 field_3FC;
    s32 field_400;
    s32 field_404;
    s32 field_408;
    s32 field_40C;
    s32 field_410;
    s32 field_414;
    s32 field_418;
    s32 field_41C;
    s32 field_420;
    s32 field_424;
    s32 field_428;
    s32 field_42C;
    s32 field_430;
    s32 field_434;
    s32 field_438;
    s32 field_43C;
    s32 field_440;
    s32 field_444;
    s32 field_448;
    s32 field_44C;
    s32 field_450;
    s32 field_454;
    s32 field_458;
    s32 field_45C;
    s32 field_460;
    s32 field_464;
    s32 field_468;
    s32 field_46C;
    s32 field_470;
    s32 field_474;
    s32 field_478;
    s32 field_47C;
    s32 field_480;
    s32 field_484;
    s32 field_488;
    s32 field_48C;
    s32 field_490;
    s32 field_494;
    s32 field_498;
    s32 field_49C;
    s32 field_4A0;
    s32 field_4A4;
    s32 field_4A8;
    s32 field_4AC;
    s32 field_4B0;
    s32 field_4B4;
    s32 field_4B8;
    s32 field_4BC;
    s32 field_4C0;
    s32 field_4C4;
    s32 field_4C8;
    s32 field_4CC;
    s32 field_4D0;
    s32 field_4D4;
    s32 field_4D8;
    s32 field_4DC;
    s32 field_4E0;
    s32 field_4E4;
    s32 field_4E8;
    s32 field_4EC;
    s32 field_4F0;
    s32 field_4F4;
    s32 field_4F8;
    s32 field_4FC;
    s32 field_500;
    s16 field_504;
    s16 field_506;
    s32 field_508;
    s32 field_50C;
    s32 field_510;
    s32 field_514;
    s32 field_518_ary_19_start_q;
    s32 field_51C;
    s32 field_520;
    Garox_18 field_524_ary_19[19];
    s32 field_608;
    s32 field_60C;
    s32 field_610;
    char_type* field_614;
    s32 field_618_p_start_q;
    s32 field_61C;
};

class Hud_MapZone_98
{
  public:
    EXPORT void sub_5D5900();
    EXPORT void sub_5D5AD0();
    EXPORT void sub_5D5AF0(u8* a2, u8* a3);
    EXPORT void sub_5D5B60();
    EXPORT void sub_5D5C50();
    EXPORT Hud_MapZone_98();
    char_type field_0;
    char_type field_1;
    wchar_t field_2_wstr[65];
    s32 field_84;
    gmp_map_zone* field_88_nav_zone;
    gmp_map_zone* field_8C_local_nav_zone;
    s32 field_90;
    char_type field_94;
    char_type field_95;
    char_type field_96;
    char_type field_97;
};

class Hud_CarName_4C
{
  public:
    EXPORT Hud_CarName_4C();
    char_type field_0_display_time;
    char_type field_1;
    wchar_t field_2_car_name[33];
    s32 field_44;
    s32 field_48;
};

class Hud_2B00
{
  public:
    EXPORT ~Hud_2B00();
    EXPORT void sub_5D4A10();
    EXPORT void sub_5D5190();
    EXPORT void sub_5D5240(wchar_t* Source);
    EXPORT void sub_5D5350();
    EXPORT void DrawGui_5D6860();
    EXPORT void sub_5D69C0();
    EXPORT void sub_5D69D0();
    EXPORT void sub_5D6A70();
    EXPORT void sub_5D6A90();
    EXPORT void sub_5D6AB0();
    EXPORT s16 sub_5D6B00();
    EXPORT void sub_5D6BE0();
    EXPORT s32 sub_5D6C20(s32 action, char_type* a2);
    EXPORT s32 sub_5D6C70(s32 a1);
    EXPORT s32 sub_5D6CB0(s32 a1);
    EXPORT Hud_2B00();

    Hud_CarName_4C field_0; // ok
    Hud_MapZone_98 field_4C; // ok
    Garox_1E34_L field_DC; // ok    TODO: check if it's Hud_Brief_704
    Hud_Pager_C_Array field_620; // ok
    Garox_1700_L field_650; // ok
    Hud_Arrow_7C_Array field_1F18; // ok
    Hud_CopHead_C_Array field_1028; // ok
    Garox_107C_sub field_107C_sub; // nothing
    Garox_27B5_sub field_27B5_sub; // ok
    char_type field_27B6;
    char_type field_27B7;
    Garox_1 field_1080; // ok
    Garox_1108_sub field_1108_sub;
    Garox_110C_sub field_110C_sub; // ok
    Garox_1118_sub field_1118_sub;
    Hud_Message_1C8 field_111C; // ok
    Garox_12E4_sub field_12E4_sub; // ok
    char_type field_2A1E;
    char_type field_2A1F;
    Garox_4 field_12E8_sub; // ok
    Garox_12EC_sub field_12EC_sub;
    Garox_2A25_sub field_2A25_sub;
    char_type field_2A26;
    char_type field_2A27;
    Garox_1_v2 field_12F0; // ok
    Garox_13C0_sub field_13C0_sub;
    s32 field_13C4_text_speed;
};

EXPORT_VAR extern Hud_2B00* gGarox_2B00_706620;

EXPORT_VAR extern s16 word_706600;

EXPORT_VAR extern char byte_67CE50[];
