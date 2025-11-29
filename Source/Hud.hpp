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
class Car_BC;

class Garox_13C0_sub
{
  public:
    EXPORT void DrawPlayerNames_5CFE40();
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
    EXPORT char_type IsTypingOnChat_5D15E0(s32 a1, Player *pPlayer);
    EXPORT void sub_5D16B0();
    EXPORT bool sub_5D17D0(s32 a2);
    EXPORT void StartChatting_5D1830(Player* a1);
    char_type field_2A25;
};

class Garox_12EC_sub
{
  public:
    EXPORT char_type IsOnQuitMessage_5D13C0(s32 a2, Player *pPlayer);
    EXPORT void DrawQuitMessage_5D1430();
    EXPORT bool sub_5D15A0(s32 a1);
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
    EXPORT void DrawPause_5D63B0();
    char_type field_12E4;
    char_type field_1;
};

class Hud_Message_1C8
{
  public:
    EXPORT void sub_5D1850();
    EXPORT void sub_5D1860();
    EXPORT void DrawMessage_5D1940();
    EXPORT void ShowMessage_5D1A00(wchar_t* pStr, s32 a3);
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
    EXPORT void DrawPlayerStats_5D5C80();
    EXPORT void sub_5D6290();
    s32 field_1118;
};

class Garox_110C_sub
{
  public:
    // inline 0x4C6E50
    Garox_110C_sub()
    {
        field_1114 = 0;
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
    EXPORT void DrawHealth_5D0260();
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
        field_27B5_sub = false;
    }
    EXPORT void sub_5CF970();
    char_type field_27B5_sub;
};

class Garox_107C_sub
{
  public:
    EXPORT void DrawGangRespectBars_5CFA70();
    EXPORT void sub_5CFE20();
    EXPORT void Empty_5CFE30();
    char_type field_107C_sub;
};

class Hud_CopHead_C
{
  public:
    EXPORT void UpdateHead_5D0050(bool bShakeHead);
    EXPORT Hud_CopHead_C();
    u8 field_0;
    char_type field_1;
    char_type field_2;
    char_type field_3;
    s32 field_4_height;
    s32 field_8_velocity;
};

class Hud_CopHead_C_Array
{
  public:
    // inline 0x4C6EE0
    Hud_CopHead_C_Array()
    {
        field_48_cop_level = 0;
    }
    EXPORT void UpdateWantedLevel_5D00B0();
    EXPORT void DrawWantedLevel_5D0110();
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

    // TODO: Seems like a pool and the ctor would suggest it is, yet somehow the fields are in the wrong order?
    Garox_C4 field_0_29_ary[30];
    Garox_C4* field_960_pFirst;
    Garox_C4* field_964;
};

class Hud_Pager_C
{
  public:
    EXPORT ~Hud_Pager_C();
    EXPORT void Service_5D2320();
    EXPORT void sub_5D2380(s32 a2, s32 a3);
    EXPORT s32 sub_5D2680(s32 a2, s32 a3);
    EXPORT s32 sub_5D2AB0(s32 a2, s32 a3);

    EXPORT Hud_Pager_C();
    s32 field_0_timer;
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

    // TODO: Correct order ?
    EXPORT s32 sub_5D3220(s32& a2);
    EXPORT s32 sub_5D3280(u32 a2);

    EXPORT void DrawPagers_5D3040();
    EXPORT void UpdatePagers_5D31B0();
    EXPORT s32 CreateTimer_5D31F0(s32 a2);
    EXPORT void sub_5D32D0(s32 a2);
    EXPORT void AddTime_5D32F0(s32 a2, s32 a3);
    EXPORT void sub_5D3310(s32 a2);

    Hud_Pager_C field_620[4];
};

class Garox_18
{
  public:
    Garox_18** field_0; // prob wrong type
    s32 field_4;
    s32 field_8_brief_priority;
    Garox_18* field_C;  // prob wrong type
    u8 field_10;
    u8 field_11;
    u8 field_12;
    u8 field_13;
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
    EXPORT void PointToInfoPhone_5D03C0(Gang_144* pZone);
    EXPORT void sub_5D03F0();

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

    inline void set_arrow_aim_from_pos_4767C0(Fix16 x, Fix16 y, Fix16 z)
    {
        field_14_aim_x = x;
        field_18_aim_y = y;
        field_1C_aim_z = z;
    }

    // 9.6f inline 0x4C6F20
    inline bool sub_4C6F20()
    {
        return field_10_type == 0;
    }

    Ped* field_0;
    Car_BC* field_4;
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
    s32 field_28_arrow_colour;
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
    EXPORT void SetArrowColour_5D0510(s32 a2);
    EXPORT bool sub_5D0530();
    EXPORT char_type sub_5D0620();
    EXPORT s32 sub_5D0850();
    EXPORT void Service_5D0C60();
    EXPORT void sub_5D0C90();
    EXPORT void sub_5D0DC0(Ped* a2);

    // 9.6f inline 0x4C6F80
    inline bool sub_4C6F80()
    {
        if (field_18.field_18.sub_4C6F20() && field_18.field_3C.sub_4C6F20())
        {
            return true;
        }
        return false;
    }

    // 9.6f inline 0x4C7050
    inline bool sub_4C7050()
    {
        if (field_18.field_10.field_5)
        {
            return true;
        }
        return false;
    }

    EXPORT Hud_Arrow_7C();
    Fix16 field_0_screen_pos_x; // x and y are not independent from field_10_radius_pos
    Fix16 field_4_screen_pos_y;
    Ang16 field_8_rotation;
    s16 field_A;
    s32 field_C_min_radius_pos; // minimum radial distance from the player
    s32 field_10_radius_pos;  // radial distance from the player
    s32 field_14_reposition_speed;  // how slower/faster the arrow goes to the aim target, or "get back" to the player
    Garox_20_Sub field_18;
};

class Hud_Arrow_7C_Array
{
  public:
    // inline 0x4C7080
    Hud_Arrow_7C_Array()
    {
        field_83C = 1;
        field_840 = NULL;
        field_844 = 0;
    }

    EXPORT void sub_5D1350();
    EXPORT bool sub_5D0E40(Hud_Arrow_7C* a2);
    EXPORT void sub_5D0E90();
    EXPORT Hud_Arrow_7C* sub_5D0EF0();
    EXPORT char_type sub_5D0F40(Gang_144* a2);
    EXPORT void sub_5D0F80();
    EXPORT void UpdateArrows_5D0FD0();
    EXPORT Hud_Arrow_7C* sub_5D1020(s32* a2);
    EXPORT Hud_Arrow_7C* AllocArrow_5D1050();
    EXPORT void sub_5D10B0();
    EXPORT Hud_Arrow_7C* sub_5D10D0(Gang_144* pZone, s32 phone_type);
    EXPORT void place_gang_phone_5D1110(Object_2C* pPhoneInfo);
    EXPORT void SetNewGangArrow_5D1310(Gang_144* pZone);
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

class Garox_1E34_L  // size 0x704
{
  public:
    EXPORT void sub_5D3330();
    EXPORT char_type* sub_5D3350();
    EXPORT s32 sub_5D3370();
    EXPORT void sub_5D33A0();
    EXPORT s32 sub_5D33F0();
    EXPORT size_t sub_5D3470();
    EXPORT char_type sub_5D3680(s16 a1);
    EXPORT void sub_5D39D0();
    EXPORT void DrawBrief_5D3B80();
    EXPORT s32 sub_5D3F10(s32 a2, const char_type* a3, s32 a4);
    EXPORT s32 sub_5D4400(s32 a2, const char_type* a3);
    EXPORT void sub_5D44D0();
    EXPORT void ShowBrief_5D4850();
    EXPORT s32 ClearAllBriefsWithPriority_5D4890(s32 a2);
    EXPORT Garox_1E34_L();

    wchar_t field_0_str[640];
    s16 field_500;
    s16 field_502_face_idx;
    u16 field_504_tick_timer;
    u16 field_506;
    s32 field_508_num_lines;
    s32 field_50C;
    s32 field_510_time_to_show;
    s32 field_514_upward_timer;
    Garox_18* field_518_ary_19_start_q;
    s32 field_51C;
    s32 field_520;
    Garox_18 field_524_ary_19[19];
    s32 field_6EC;
    s32 field_6F0;
    s32 field_6F4;
    Garox_18* field_6F8_prev_brief;
    Garox_18** field_6FC_p_start_q;
    Garox_18* field_700;
};

class gmp_map_zone;

class Hud_MapZone_98
{
  public:
    EXPORT void DrawZoneName_5D5900();
    EXPORT void sub_5D5AD0();
    EXPORT void sub_5D5AF0(gmp_map_zone* pZone1, gmp_map_zone* pZone2);
    EXPORT void sub_5D5B60();
    EXPORT void sub_5D5C50();
    EXPORT Hud_MapZone_98();
    u8 field_0_timer;
    char_type field_1;
    wchar_t field_2_wstr[65];
    s32 field_84;
    gmp_map_zone* field_88_nav_zone;
    gmp_map_zone* field_8C_local_nav_zone;
    s32 field_90;
    u8 field_94_transparency;  // range from 0 to 31
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
    EXPORT void UpdateHUD_5D69D0();
    EXPORT void sub_5D6A70();
    EXPORT void sub_5D6A90();
    EXPORT void sub_5D6AB0();
    EXPORT void sub_5D6B00();
    EXPORT void sub_5D6BE0();
    EXPORT s32 IsBusy_5D6C20(s32 action, Player *pPlayer);
    EXPORT s32 sub_5D6C70(s32 a1);
    EXPORT bool sub_5D6CB0(s32 a1);
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
    u32 field_13C4_text_speed;
};

EXTERN_GLOBAL(Hud_2B00*, gHud_2B00_706620);


EXTERN_GLOBAL(s16, word_706600);


EXTERN_GLOBAL_ARRAY(char, byte_67CE50, 264);

EXPORT s32 __stdcall sub_5D1260(s32 a1);
EXPORT char_type* __stdcall get_phone_colour_5D12B0(s32 phone_type);
EXPORT u8 __stdcall sub_5D12E0(s32 phone_type);

EXPORT void __stdcall sub_5D6060(s16 a1, u8 a2);
EXPORT s32 __stdcall sub_5D61A0(s32 a1, s32 a2, u16 a3);
