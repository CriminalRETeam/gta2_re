#pragma once

#include "types.hpp"

class angry_lewin_0x85C;
class Car_3C;
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
    s32 sub_4B8BB0();
    void sub_4B8BD0(s32 arg0, s32 main_state, s8 a2);
    void sub_4B8C00(s32 a1, s32 a2);
    s8 sub_4B8C20();
    void sub_4B8C40();
    void sub_4B8E00(u32 a1, u32 a2);
    void sub_4B8E50();
    void sub_4B8EB0();
    void ShowCounters_4B8FF0();
    s8 sub_4B9270(u16 *a2); // TODO: user call 2nd param
    void Draw_4B92D0();
    void sub_4B9380();
    s32 sub_4B93C0();
    void sub_4B9410();
    s8 sub_4B9640();
    void sub_4B9700();
    void sub_4B9710();
    void sub_4B9720();
    angry_lewin_0x85C *sub_4B9750();
    void sub_4B9790(s32 a2, s32 a3, s32 a4);
    s8 sub_4B97E0(Car_3C *a2, s32 a3);
    s8 sub_4B9830(Car_3C *pCarSprite, s32 a3);
    s8 sub_4B9890(s16* a2, s32 a3);      
    bool sub_4B98E0(s16 *a2, u8 playerIdx, s32 a4);
    s8 sub_4B9950(Car_3C *pCarSprite, u8 idx, s32 a4);
    bool sub_4B9A10(Car_3C *a2, u8 playerIdx);
    s8 is_point_on_screen_4B9A80(s32 a2_fp, s32 a3_fp);
    s8 sub_4B9B10(Fix16_Rect *pBounds);
    DrawUnk_0xBC *IteratePlayerCamera_4B9BC0();
    s8 sub_4B9C10(Car_BC *a2);
    DrawUnk_0xBC* sub_4B9C50();
    angry_lewin_0x85C *sub_4B9CD0();
    angry_lewin_0x85C *IterateNextPlayer_4B9D10();
    void sub_4B9D60(angry_lewin_0x85C *a2);
    Game_0x40(u8 max_players, s8 player_idx); // 4B9DE0
    ~Game_0x40(); // 4BAE30  

    s32 field_0_game_state;
    angry_lewin_0x85C *field_4_players[6];
    angry_lewin_0x85C *field_1C_unk;
    u8 field_20;
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
    angry_lewin_0x85C *field_38_orf1;
    char_type field_3C_bSkipPolice;
    char_type field_3D;
    char_type field_3E;
    char_type field_3F;       
};

// extern __declspec(dllexport)
extern EXPORT_VAR Game_0x40* gGame_0x40_67E008;
