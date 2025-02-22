#pragma once

#include "Function.hpp"
#include "fix16.hpp"

class Player;
class Sprite;
class Car_BC;
class DrawUnk_0xBC;

// TODO: Prob should be in its own header
struct Fix16_Rect
{
    // TODO: Types
    s32 field_0_left;
    s32 field_4_right;
    s32 field_8_top;
    s32 field_C_bottom;
};

class Game_0x40
{
  public:
    EXPORT s32 sub_4B8BB0();
    EXPORT void sub_4B8BD0(s32 arg0, s32 main_state, s8 a2);
    EXPORT void sub_4B8C00(s32 a1, s32 a2);
    EXPORT s8 sub_4B8C20();
    EXPORT void sub_4B8C40();
    EXPORT void sub_4B8E00(u32 a1, u32 a2);
    EXPORT void sub_4B8E50();
    EXPORT void sub_4B8EB0();
    EXPORT void ShowCounters_4B8FF0();
    EXPORT void sub_4B9270();
    EXPORT void Draw_4B92D0();
    EXPORT void sub_4B9380();
    EXPORT void sub_4B93C0();
    EXPORT void sub_4B9410();
    EXPORT s8 sub_4B9640();
    EXPORT void sub_4B9700();
    EXPORT void sub_4B9710();
    EXPORT void sub_4B9720();
    EXPORT Player* sub_4B9750();
    EXPORT void sub_4B9790(s32 a2, s32 a3, s32 a4);
    EXPORT s8 sub_4B97E0(Sprite* a2, s32 a3);
    EXPORT s8 sub_4B9830(Sprite* pCarSprite, s32 a3);
    EXPORT s8 sub_4B9890(s16* a2, s32 a3);
    EXPORT bool sub_4B98E0(s16* a2, u8 playerIdx, s32 a4);
    EXPORT s8 sub_4B9950(Sprite* pCarSprite, u8 idx, s32 a4);
    EXPORT bool sub_4B9A10(Sprite* a2, u8 playerIdx);
    EXPORT bool is_point_on_screen_4B9A80(s32 a2_fp, s32 a3_fp);
    EXPORT s8 sub_4B9B10(Fix16_Rect* pBounds);
    EXPORT DrawUnk_0xBC* IteratePlayerCamera_4B9BC0();
    EXPORT s8 sub_4B9C10(Car_BC* a2);
    EXPORT DrawUnk_0xBC* sub_4B9C50();
    EXPORT Player* sub_4B9CD0();
    EXPORT Player* IterateNextPlayer_4B9D10();
    EXPORT void sub_4B9D60(Sprite* a2, Player* pExclude);
    EXPORT Game_0x40(u8 max_players, s8 player_idx); // 4B9DE0
    EXPORT ~Game_0x40(); // 4BAE30

    s32 field_0_game_state;
    Player* field_4_players[6];
    Player* field_1C_unk;
    u8 field_20_idx;
    u8 field_21_player_camera_idx;
    char_type field_22;
    u8 field_23_max_idx;
    u8 field_24_cur_idx;
    char_type field_25;
    char_type field_26;
    char_type field_27;
    s32 field_28_timer;
    s32 field_2C_main_state;
    char_type field_30;
    char_type field_31;
    char_type field_32;
    char_type field_33;
    s32 field_34;
    Player* field_38_orf1;
    bool field_3C_bSkipPolice;
    char_type field_3D;
    char_type field_3E;
    char_type field_3F;
};


EXPORT_VAR extern Game_0x40* gGame_0x40_67E008;

EXPORT_VAR extern Fix16 dword_67DCCC;

EXPORT_VAR extern class Hamburger_500* gHamburger_500_678E30;
