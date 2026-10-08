#pragma once

#include "Draw.hpp"
#include "Function.hpp"
#include "ang16.hpp"
#include "fix16.hpp"
#include "gtx_0x106C.hpp"
#include "map_0x370.hpp"
#include <wchar.h>

class Ped;
class Player;
class Gang_144;
class infallible_turing;
class Gang_144;
class Object_2C;
class Car_BC;

class Hud_PlayerNames_4
{
  public:
    EXPORT void DrawPlayerNames_5CFE40();
    s32 field_0_unused;
};

class Hud_MpMessage_D0
{
  public:
    EXPORT void CalcTextWidth_5D56B0();
    EXPORT void Draw_5D56D0();
    EXPORT void ShowText_5D5730(const wchar_t* pStr);
    EXPORT void DecrementTimer_5D5760();
    EXPORT void AnnounceKill_5D5770(Player* killer, Player* victim);
    EXPORT Hud_MpMessage_D0();
    char_type field_0_timer;
    wchar_t field_2_str[101];
    s32 field_CC_text_width;
};

class Hud_ChatInput_1
{
  public:
    EXPORT bool IsTypingOnChat_5D15E0(s32 action, Player* pPlayer);
    EXPORT void DrawChatMessages_5D16B0();
    EXPORT bool IsChatInputKey_5D17D0(s32 key_idx);
    EXPORT void StartChatting_5D1830(Player* pPlayer);
};

class Hud_QuitMessage_1
{
  public:
    EXPORT bool IsOnQuitMessage_5D13C0(s32 action, Player* pPlayer);
    EXPORT void DrawQuitMessage_5D1430();
    EXPORT bool IsQuitMessageKey_5D15A0(s32 action);
    EXPORT void ShowQuitMessage_5D15D0(Player* pPlayer);
    char_type field_12EC_quit_message;
};

class Hud_BriefSelector_4
{
  public:
    // inline 0x4C6AC0
    Hud_BriefSelector_4()
    {
        field_0_value = 0; // TODO: byte ?
    }
    EXPORT void ShowNextNumberedBrief_5CF620();
    EXPORT void ShowPrevNumberedBrief_5CF6B0();
    s32 field_0_value;
};

namespace HudPauseSection
{
enum
{
    target_score_0 = 0,
    gang_1_missions_done_1 = 1,
    gang_2_missions_done_2 = 2,
    gang_3_missions_done_3 = 3,
    all_missions_done_4 = 4,
    kill_frenzies_completed_5 = 5,
    tokens_collected_6 = 6,
};
} // namespace HudPauseSection

class Hud_PauseScreen_2
{
  public:
    // inline 0x4C71A0
    Hud_PauseScreen_2()
    {
        field_0_current_pause_section = 0;
        field_1_timer = 45;
    }
    EXPORT void DrawPause_5D63B0();
    EXPORT void UpdatePauseSection_5D6300();
    u8 field_0_current_pause_section;
    char_type field_1_timer;
};

class Hud_Message_1C8
{
  public:
    EXPORT void ClearTimeToShow_5D1850();
    EXPORT void FormatMessage_5D1860();
    EXPORT void DrawMessage_5D1940();
    EXPORT void ShowMessage_5D1A00(wchar_t* pStr, s32 priority);
    EXPORT void DecrementTimeToShow_5D1AB0();
    EXPORT Hud_Message_1C8();
    u8 field_0_time_to_show;
    wchar_t field_2_str[221];
    s32 field_1BC_str_width;
    s32 field_1C0_num_lines;
    s32 field_1C4_priority; // hud_message_priority (hud_message_priority.hpp)
};

class Hud_PlayerStats_4
{
  public:
    EXPORT void DrawPlayerStats_5D5C80();
    EXPORT void UpdateRollingDigits_5D6290();
    s32 field_0_unused;
};

class Hud_UnderRoofArrowMarker_C
{
  public:
    // inline 0x4C6E50
    Hud_UnderRoofArrowMarker_C()
    {
        field_8_rotation = 0;
        field_A_ped_under_solid = 0;
    }
    EXPORT void Update_5CF730();
    EXPORT void Draw_5CF910();
    Fix16 field_0_screen_x;
    Fix16 field_4_screen_y;
    Ang16 field_8_rotation;
    char_type field_A_ped_under_solid;
    char_type field_B_unused;
};

class Hud_Health_4
{
  public:
    EXPORT void DrawHealth_5D0260();
    s32 field_0_unused;
};

class Hud_PickupText_88
{
  public:
    EXPORT void CalcTextWidth_5D53E0();
    EXPORT void Draw_5D5420();
    EXPORT void ShowPickupText_5D5600(u8 pickup_idx);
    EXPORT void DecrementTimer_5D5690();
    EXPORT Hud_PickupText_88();
    char_type field_0_timer;
    wchar_t field_2_str[65];
    s32 field_84_text_width;
};

class Hud_ShowCoords_1
{
  public:
    // inline 0x4C6E70
    Hud_ShowCoords_1()
    {
        field_0_show_coords = false;
    }
    EXPORT void ShowPlayerCoords_5CF970();

    // 9.6f 0x4A4760
    inline void ToggleShowCoords_4A4760()
    {
        field_0_show_coords = field_0_show_coords == 0;
    }

    char_type field_0_show_coords;
};

class Hud_GangRespectBars_1
{
  public:
    EXPORT void DrawGangRespectBars_5CFA70();
    EXPORT void Empty_5CFE20();
    EXPORT void Empty_5CFE30();
    char_type field_107C_gang_respect_bars;
};

class Hud_CopHead_C
{
  public:
    EXPORT void UpdateHead_5D0050(bool bShakeHead);
    EXPORT Hud_CopHead_C();
    u8 field_0_frame;
    char_type field_1_frame_timer;
    char_type field_2_frame_delay;
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
    EXPORT void Init_5D0210();
    Hud_CopHead_C field_1028_cop_heads[6];
    s32 field_48_cop_level;
    Fix16 field_4C_w_fp;
    Fix16 field_50_h_fp;
};

class Hud_TextEntry_C4
{
  public:
    EXPORT void FormatAndSetupText_5D1B10(const wchar_t* pStr, s16 xpos, s16 ypos, s16 fontType, s32 displayTime);
    EXPORT void Draw_5D1D00();
    EXPORT bool DecrementDisplayTime_5D1DB0();
    EXPORT bool operator_equals_5D1E10(Hud_TextEntry_C4* pOther);

    // 9.6f 0x4C70F0
    inline void Expire_4C70F0()
    {
        field_A4_display_time = 0;
    }

    // 9.6f 0x4C70E0
    inline void ClearAlpha_4C70E0()
    {
        field_B8_alpha = 0;
        *(u8*)&field_BC_alpha_flag = 0; // byte store in both 9.6f and 10.5, the field is read as s32 elsewhere
    }

    // 9.6f 0x45AFD0
    void SetDrawKind8_45AFD0(s16 palette)
    {
        field_B0_drawKind = 8;
        field_B4_palette = palette;
    }
    wchar_t field_0_str_buf[82];
    s32 field_A4_display_time;
    s16 field_A8_x;
    s16 field_AA_y;
    s16 field_AC_fontType;
    s32 field_B0_drawKind;
    s16 field_B4_palette;
    s32 field_B8_alpha;
    s32 field_BC_alpha_flag;
    Hud_TextEntry_C4* field_C0_pNext;
};

class Hud_TextList_968
{
  public:
    EXPORT void ExpireDuplicates_5D1EB0(Hud_TextEntry_C4* String2);
    EXPORT Hud_TextEntry_C4* DisplayText_5D1F50(const wchar_t* pStr, s16 xpos, s16 ypos, s16 font_type, s32 display_time);
    EXPORT void Service_5D2010();
    EXPORT void RemoveExpired_5D2050();
    EXPORT Hud_TextList_968();

    // TODO: Seems like a pool and the ctor would suggest it is, yet somehow the fields are in the wrong order?
    Hud_TextEntry_C4 field_0_29_ary[30];
    Hud_TextEntry_C4* field_960_pFirst;
    Hud_TextEntry_C4* field_964_pFreeList;
};

class Hud_Pager_C
{
  public:
    // 9.6f 0x4C7160
    inline bool no_ptr_counter_4C7160()
    {
        return field_4_ptr_counter == NULL;
    }

    // 9.6f 0x4C7170
    inline bool IsTimerOff_4C7170()
    {
        return field_0_timer < 0;
    }

    // 9.6f 0x4C7120
    inline void SetTimer_4C7120(s32 timer)
    {
        field_0_timer = timer;
    }

    // 9.6f 0x4C7130, defined in Hud.cpp
    inline void SetCounter_4C7130(s32* pCounter);

    // 9.6f 0x411A40
    inline s32 get_timer_411A40()
    {
        return field_0_timer;
    }

    EXPORT ~Hud_Pager_C();
    EXPORT void Service_5D2320();
    EXPORT void DrawCounterDigits_5D2380(s32 xpos, s32 ypos);
    EXPORT void DrawDigits_5D2680(s32 xpos, s32 ypos);
    EXPORT void DrawPager_5D2AB0(u32 xpos, s32 ypos);

    EXPORT Hud_Pager_C();
    s32 field_0_timer;
    s32* field_4_ptr_counter; //  counter?
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
    EXPORT s32 AddOnScreenCounter_5D3220(s32* pCounter);
    EXPORT void ClearPager_5D3280(s32 idx);

    EXPORT void DrawPagers_5D3040();
    EXPORT void UpdatePagers_5D31B0();
    EXPORT s32 CreateTimer_5D31F0(s32 seconds);
    EXPORT void ClearClockOnly_5D32D0(s32 pager_idx);
    EXPORT void AddTime_5D32F0(s32 pager_idx, s32 time_to_add);
    EXPORT void ClearCounterOnly_5D3310(s32 pager_idx);

    Hud_Pager_C field_0_pagers_array[4];
};

inline u8 __stdcall get_sprite_width_4C7220(s16 user_sprite_idx)
{
    s16 sprite_idx = gGtx_0x106C_703DD4->GetSpriteTrueIndex_5AA460(sprite_types_enum::user_6, user_sprite_idx);
    return gGtx_0x106C_703DD4->get_sprite_width_420220(sprite_idx);
}

inline u8 __stdcall get_sprite_height_4C7250(s16 user_sprite_idx)
{
    s16 sprite_idx = gGtx_0x106C_703DD4->GetSpriteTrueIndex_5AA460(sprite_types_enum::user_6, user_sprite_idx);
    return gGtx_0x106C_703DD4->get_sprite_height_4C6C90(sprite_idx);
}

class Hud_BriefEntry_18
{
  public:
    char_type field_0_brief_id_str[8]; // gxt key, e.g. "1_3"
    s32 field_8_brief_priority;
    Hud_BriefEntry_18* field_C_pNext;
    u8 field_10_was_displayed;
    s32 field_14_cost_param;
};

namespace ArrowTargetType
{
enum
{
    Nothing_0 = 0,
    XYZ_Coord_1 = 1,
    Ped_2 = 2,
    Car_3 = 3,
    Object_4 = 4,
    InfoPhone_5 = 5,
    Player_6 = 6 // Multiplayer arrows
};
} // namespace ArrowTargetType

class Hud_ArrowGangInfo_8
{
  public:
    Gang_144* field_30_gang;
    char_type field_34_min_respect;
    char_type field_5_is_visible; // not sure
    char_type field_6_in_use;
};

class ArrowTrace_24
{
  public:
    EXPORT void PointToInfoPhone_5D03C0(Gang_144* pZone);
    EXPORT void UpdateAimCoordinates_5D03F0();

    void SetTargetCar(Car_BC* pCar)
    {
        field_4_car = pCar;
        field_10_target_type = ArrowTargetType::Car_3;
    }

    // 9.6f 0x4767E0
    inline void SetTargetPed_4767E0(Ped* pPed)
    {
        field_0_ped = pPed;
        field_10_target_type = ArrowTargetType::Ped_2;
    }

    // 9.6f 0x476810
    inline void SetTargetObject_476810(Object_2C* pObj)
    {
        field_8_obj = pObj;
        field_10_target_type = ArrowTargetType::Object_4;
    }

    // inline 0x4C6F00
    void init()
    {
        field_0_ped = NULL;
        field_4_car = NULL;
        field_8_obj = NULL;
        field_C_player = NULL;
        field_10_target_type = ArrowTargetType::Nothing_0;
        field_20_bIsTargetVisible = false;
    }

    inline void set_arrow_aim_from_pos_4767C0(Fix16 x, Fix16 y, Fix16 z)
    {
        field_14_aim_x = x;
        field_18_aim_y = y;
        field_1C_aim_z = z;
    }

    // 9.6f inline 0x4C6F20
    inline bool HasNoTarget_4C6F20()
    {
        return field_10_target_type == ArrowTargetType::Nothing_0;
    }
    
    // 9.6f 0x4C6F30
    inline Player* GetTargetPlayer_4C6F30()
    {
        return field_C_player;
    }

    // 9.6f inline 0x4820A0. The player is stored first (VC7 gives 9.6f's body either way): with the
    // type store first, SetNewFugitive_516590 schedules and allocates registers differently
    inline void SetTargetPlayer_4820A0(Player* player)
    {
        field_C_player = player;
        field_10_target_type = ArrowTargetType::Player_6;
    }

    Ped* field_0_ped;
    Car_BC* field_4_car;
    Object_2C* field_8_obj;
    Player* field_C_player;
    s32 field_10_target_type;
    Fix16 field_14_aim_x;
    Fix16 field_18_aim_y;
    Fix16 field_1C_aim_z;
    char_type field_20_bIsTargetVisible;
};

class Hud_ArrowTargets_64
{
  public:

    inline bool HasBothTargets_4C6FB0()
    {
        return !field_18_primary_target.HasNoTarget_4C6F20() && !field_3C_secondary_target.HasNoTarget_4C6F20();
    }

    s32 field_20;
    s16 field_24;
    s32 field_28_arrow_colour;
    s16 field_2C_arrow_sprt_idx;
    u8 field_2E_target_swap_timer;
    Hud_ArrowGangInfo_8 field_10;
    ArrowTrace_24 field_18_primary_target;
    ArrowTrace_24 field_3C_secondary_target;
    ArrowTrace_24* field_60_curr_target;
};

class Hud_Arrow_7C
{
  public:
    // 9.6f 0x4CA610, defined in Hud.cpp
    inline void Reset_4CA610();

    // 9.6f 0x4C6FF0, defined in Hud.cpp
    inline void SetMinRadiusPos_4C6FF0(s32 steps);

    // 9.6f 0x4C6F20
    inline bool Is_radius_pos_0_4C6F20()
    {
        return field_10_radius_pos == 0;
    }

    EXPORT void SetArrowColour_5D0510(s32 arrow_colour);
    EXPORT bool CheckVisibility_5D0530();
    EXPORT bool UpdateTargets_5D0620();
    EXPORT void UpdateScreenPos_5D0850();
    EXPORT void Service_5D0C60();
    EXPORT void DrawArrow_5D0C90();
    EXPORT void SetPlayerArrowColour_5D0DC0(Ped* pPed);

    // 9.6f inline 0x4C6F80
    inline bool IsType0_4C6F80()
    {
        if (field_18.field_18_primary_target.HasNoTarget_4C6F20() && field_18.field_3C_secondary_target.HasNoTarget_4C6F20())
        {
            return true;
        }
        return false;
    }

    // 9.6f inline 0x4C7050
    inline bool IsVisible_4C7050()
    {
        if (field_18.field_10.field_5_is_visible)
        {
            return true;
        }
        return false;
    }

    inline void swap_arrows_4C7060()
    {
        if (field_18.field_60_curr_target == &field_18.field_18_primary_target)
        {
            field_18.field_60_curr_target = &field_18.field_3C_secondary_target;
        }
        else
        {
            field_18.field_60_curr_target = &field_18.field_18_primary_target;
        }
    }

    // 9.6f 0x476850
    inline void SetArrowTargetPed_476850(Ped* pPed)
    {
        field_18.field_18_primary_target.SetTargetPed_4767E0(pPed);
    }

    // 9.6f 0x476860
    inline void SetArrowTargetCar_476860(Car_BC* pCar)
    {
        field_18.field_18_primary_target.SetTargetCar(pCar);
    }

    // 9.6f 0x476870
    inline void SetArrowTargetObject_476870(Object_2C* pObj)
    {
        field_18.field_18_primary_target.SetTargetObject_476810(pObj);
    }

    inline void SetArrowAim_476840(Fix16 xpos, Fix16 ypos, Fix16 zpos)
    {
        ArrowTrace_24* pTarget = &field_18.field_18_primary_target;
        pTarget->field_14_aim_x = xpos;
        pTarget->field_18_aim_y = ypos;
        pTarget->field_1C_aim_z = zpos;
        pTarget->field_10_target_type = ArrowTargetType::XYZ_Coord_1;
    }

    EXPORT Hud_Arrow_7C();
    Fix16 field_0_screen_pos_x; // x and y are not independent from field_10_radius_pos
    Fix16 field_4_screen_pos_y;
    Ang16 field_8_rotation;
    Fix16 field_C_min_radius_pos; // minimum radial distance from the player
    Fix16 field_10_radius_pos; // radial distance from the player
    Fix16 field_14_reposition_speed; // how slower/faster the arrow goes to the aim target, or "get back" to the player
    Hud_ArrowTargets_64 field_18;
};

class Hud_Arrow_7C_Array
{
  public:
    // 9.6f 0x4C70B0
    inline char_type ShowGangArrows_4C70B0()
    {
        return field_83C_show_gang_arrows;
    }

    // inline 0x4C7080
    Hud_Arrow_7C_Array()
    {
        field_83C_show_gang_arrows = 1;
        field_840_visible_gang_arrow = NULL;
        field_844_check_info_phones = 0;
    }

    EXPORT void CreatePlayerArrows_5D1350();
    EXPORT bool IsThereAnyOtherArrowsInSameGang_5D0E40(Hud_Arrow_7C* pArgArrow);
    EXPORT void DrawArrows_5D0E90();
    EXPORT void FindVisibleGangArrow_5D0EF0();
    EXPORT bool IsThereAnyMissionPhoneArrowForGang_5D0F40(Gang_144* pArgGang);
    EXPORT void ClearOrphanInfoPhoneArrows_5D0F80();
    EXPORT void UpdateArrows_5D0FD0();
    EXPORT Hud_Arrow_7C* FindFreeArrow_5D1020(s32* pOutIdx);
    EXPORT Hud_Arrow_7C* AllocArrow_5D1050();
    EXPORT void ReleaseAllArrows_5D10B0();
    EXPORT Hud_Arrow_7C* FindGangPhoneArrow_5D10D0(Gang_144* pZone, s32 phone_type);
    EXPORT void place_gang_phone_5D1110(Object_2C* pPhoneInfo);
    EXPORT void SetNewGangArrow_5D1310(Gang_144* pZone);
    Hud_Arrow_7C field_0_array[17];
    char_type field_83C_show_gang_arrows;
    Hud_Arrow_7C* field_840_visible_gang_arrow;
    char_type field_844_check_info_phones;
};

EXPORT char_type __stdcall GetBriefFaceIdx_5D3680(u16 face_char);

class Hud_Brief_704 // size 0x704
{
  public:
    EXPORT void MovePrevBriefToCurrent_5D3330();
    EXPORT void FreeCurrentBrief_5D3350();
    EXPORT void MoveCurrentBriefToPrev_5D3370();
    EXPORT void AppendCurrentBriefToPrev_5D33A0();
    EXPORT Hud_BriefEntry_18* AllocBrief_5D33F0();
    EXPORT size_t FormatCurrentBrief_5D3470();
    EXPORT void StartCurrentBrief_5D39D0();
    EXPORT void DrawBrief_5D3B80();
    EXPORT void SetHudBrief_5D3F10(s32 priority, const char_type* pText, s32 cost_param);
    EXPORT void SetHudBrief_5D4400(s32 priority, const char_type* pTextIdStr);
    EXPORT void UpdateBrief_5D44D0();
    EXPORT void ShowBrief_5D4850();
    EXPORT void ClearAllBriefsWithPriority_5D4890(s32 priority);
    EXPORT Hud_Brief_704();

    wchar_t field_0_str[640];
    s16 field_500;
    u8 field_502_face_idx;
    u16 field_504_tick_timer;
    s32 field_508_num_lines;
    s32 field_50C_face_variant;
    s32 field_510_time_to_show;
    s32 field_514_upward_timer;
    // Pool of 20 entries linked through field_C_pNext as the free list (field_6FC_free_briefs is its head)
    Hud_BriefEntry_18 field_518_briefs[20];
    // Linked lists through field_C_pNext: the shown brief and those queued behind it by priority, the briefs pushed back
    // by a more urgent one, and the free pool entries
    Hud_BriefEntry_18* field_6F8_curr_briefs;
    Hud_BriefEntry_18* field_6FC_free_briefs;
    Hud_BriefEntry_18* field_700_prev_briefs;
};

class gmp_map_zone;

class Hud_MapZone_98
{
  public:
    // 9.6f 0x4A4770
    void clear_zones()
    {
        field_88_nav_zone = NULL;
        field_8C_local_nav_zone = NULL;
    }

    EXPORT void DrawZoneName_5D5900();
    EXPORT void GetXPosOffset_5D5AD0();
    EXPORT void ShowZoneName_5D5AF0(gmp_map_zone* pZone1, gmp_map_zone* pZone2);
    EXPORT void UpdateZoneName_5D5B60();
    EXPORT void ResetTransparency_5D5C50();
    EXPORT Hud_MapZone_98();
    u8 field_0_timer;
    wchar_t field_2_wstr[65];
    s32 field_84_xpos_offset;
    gmp_map_zone* field_88_nav_zone;
    gmp_map_zone* field_8C_local_nav_zone;
    s32 field_90_alpha_flag;
    u8 field_94_transparency; // range from 0 to 31
};

class Hud_CarName_4C
{
  public:
    EXPORT Hud_CarName_4C();
    EXPORT void DrawCarName_5D4A10();
    char_type field_0_display_time;
    wchar_t field_2_car_name[33];
    s32 field_44_xpos_offset;
    s32 field_48_ypos;
};

class Hud_2B00
{
  public:
    EXPORT ~Hud_2B00();
    EXPORT void CalcCarNameXPosOffset_5D5190();
    EXPORT void ShowCarName_5D5240(wchar_t* Source);
    EXPORT void UpdateCarName_5D5350();
    EXPORT void DrawGui_5D6860();
    EXPORT void UpdatePauseSection_5D69C0();
    EXPORT void UpdateHUD_5D69D0();
    EXPORT void SetFullAmbient_5D6A70();
    EXPORT void GetTextSpeed_5D6A90();
    EXPORT void RecalcTextLayout_5D6AB0();
    EXPORT void SetFontTypes_5D6B00();
    EXPORT void Init_5D6BE0();
    EXPORT bool IsBusy_5D6C20(s32 action, Player* pPlayer);
    EXPORT bool IsInputKeyConsumed_5D6C70(s32 action);
    EXPORT bool IsQuitMessageInputKey_5D6CB0(s32 action);
    EXPORT Hud_2B00();

    Hud_CarName_4C field_0_car_name;
    Hud_MapZone_98 field_4C_zone_name;
    Hud_Brief_704 field_DC_brief;
    Hud_Pager_C_Array field_620_pagers;
    Hud_TextList_968 field_650_texts;
    Hud_Arrow_7C_Array field_1F18_arrows;
    Hud_CopHead_C_Array field_1028_wanted_level;
    Hud_GangRespectBars_1 field_107C_gang_respect_bars;
    Hud_ShowCoords_1 field_27B5_show_coords;
    Hud_PickupText_88 field_1080_pickup_text;
    Hud_Health_4 field_1108_health;
    Hud_UnderRoofArrowMarker_C field_110C_under_roof_arrow_marker;
    Hud_PlayerStats_4 field_1118_player_stats;
    Hud_Message_1C8 field_111C_message;
    Hud_PauseScreen_2 field_12E4_pause_screen;
    Hud_BriefSelector_4 field_12E8_brief_selector;
    Hud_QuitMessage_1 field_12EC_quit_message;
    Hud_ChatInput_1 field_2A25_chat_input;
    Hud_MpMessage_D0 field_12F0_mp_message;
    Hud_PlayerNames_4 field_13C0_player_names;
    u32 field_13C4_text_speed;
};

EXTERN_GLOBAL(Hud_2B00*, gHud_2B00_706620);

EXTERN_GLOBAL(s16, gDebugFont_706600);

EXTERN_GLOBAL_ARRAY(char, gTmpGxtKey_67CE50, 264);

EXPORT s32 __stdcall GetPhoneTypeFromObjModel_5D1260(s32 phone_model_idx);
EXPORT char_type* __stdcall get_phone_colour_5D12B0(s32 phone_type);
EXPORT u8 __stdcall GetMinRespectForPhoneType_5D12E0(s32 phone_type);

EXPORT void __stdcall DrawAmmo_5D6060(s16 ammo_idx, u8 ammo_count);
EXPORT s32 __stdcall DrawPlayerStatsHelper_5D61A0(s32 powerup_idx, s32 base_xpos, u16 optional_number);
