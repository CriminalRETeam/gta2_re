#pragma once

#include "Function.hpp"
#include "sleepy_stonebraker_0x6C.hpp"
#include "xenodochial_morse.hpp"
#include "youthful_einstein.hpp"
#include <windows.h>

#ifndef DIRECTINPUT_VERSION
    #define DIRECTINPUT_VERSION 0x0500
#endif

#include <Dinput.h>

class Player;
class Ang16;
struct score_table_line;

#pragma pack(push)
#pragma pack(1)
struct svg_stru
{
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
    char_type field_48;
    char_type field_49;
    char_type field_4A;
    char_type field_4B;
    char_type field_4C;
    char_type field_4D;
    short field_4E;
    s16 field_50;
};
#pragma pack(pop)

// todo: move
struct admiring_euler_4
{
    char_type field_0_save_exists;
    char_type field_1_last_saved_stage;
    char_type field_2;
    char_type field_3;

    EXPORT admiring_euler_4();

    EXPORT ~admiring_euler_4();

    EXPORT void LoadPlySlotSvg_4B6480(const char_type* FileName);
};

// todo: move
struct menu_element_0x6E
{
    EXPORT menu_element_0x6E();

    EXPORT ~menu_element_0x6E();

    char_type field_0_element_type;
    char_type field_1_is_it_displayed;
    s16 field_2_xpos;
    s16 field_4_ypos;
    union 
    {
        struct  // if field_0_element_type == STRING_TEXT_1 or STRING_TEXT_2
        {
            wchar_t field_6_element_name_str[50];
            u16 field_6A_font_type;
            u16 field_6C_font_variant;  // actaully it's the font palette
        };
        u16 field_6_geometric_shape_type; // if field_0_element_type == GEOMETRIC_SHAPE_3
    };
};

// todo: move
#pragma pack(push)
#pragma pack(1)
struct menu_option_0x82
{
  public:
    EXPORT menu_option_0x82();

    EXPORT ~menu_option_0x82();

    EXPORT bool sub_4B6330();

    EXPORT bool sub_4B6390();

    char_type field_0_option_type;
    char_type field_1_is_unlocked;
    s16 field_2_x_pos;
    s16 field_4_y_pos;
    wchar_t field_6_option_name_str[50];
    u16 field_6A;
    u16 field_6C;
    u16 field_6E_horizontal_selected_idx;
    s16 field_70;
    char_type field_72[12];
    u16 field_7E_horizontal_max_idx;
    s16 field_80_menu_page_target;
};
#pragma pack(pop)

// todo: move
struct kind_beaver_6
{
  public:
    EXPORT kind_beaver_6();

    EXPORT ~kind_beaver_6();

    s16 field_0;
    s16 field_2;
    char_type field_4_is_option_unlocked;   // this control if the menu selection will skip a option or not
    char_type field_5;
};

// todo: move
struct MenuPage_0xBCA
{
    EXPORT MenuPage_0xBCA();

    EXPORT ~MenuPage_0xBCA();

    EXPORT bool sub_4B61B0();

    EXPORT bool sub_4B6200();

    u16 field_0_number_of_options;
    s16 field_2;
    menu_option_0x82 field_4_options_array[10];
    menu_element_0x6E field_518_elements_array[15];
    kind_beaver_6 field_B8A[10];
    u16 field_BC6_current_option_idx;
    s16 field_BC8;
};

struct player_stats_0xA4;

enum WinMainState
{
    Quit_1 = 1,
    Run_Frontend_2 = 2,
    Start_Game_3 = 3,
    Start_Replay_4 = 4,
};

enum FrontendState
{
    Unknown_1 = 1,
    Booting_Map_2 = 2,
    User_Typing_New_Player_Name_3 = 3,
    Unknown_4 = 4,
    Unknown_5 = 5,
};

enum MenuScreen
{
    Options_0 = 0,
    Play_1 = 1,
    Quit_2 = 2,
    BonusAC_3 = 3,
    BonusDF_4 = 4,
    BonusGI_5 = 5,
    ViewHiScore_6 = 6,
    PlayArea1_7 = 7,
    PlayArea2_8 = 8,
    PlayArea3_9 = 9,
    EnterPlayerName_10 = 10,
    ResumeLoadSave_11 = 11,
    HiScoresDisplay_12 = 12,
    GameOver_13 = 13,
    BlueBar_14 = 14, // for game over?
    Loading_15 = 15, // seems to have some progress bar? prob never seen due to fast loading
    RedBar_16 = 16, // for player quit ?
    Credits_17 = 17,
};

enum MenuPages // enum for field_132_f136_idx
{
    MENUPAGE_NONE = -1,
    MENUPAGE_START_MENU = 0,
    MENUPAGE_PLAY = 1,
    MENUPAGE_DEAD = 2,
    MENUPAGE_AREA_COMPLETE = 3,
    MENUPAGE_GAME_COMPLETE = 4,
    MENUPAGE_VIEW_HIGH_SCORE = 5,
    MENUPAGE_BONUS_AREA = 6,
    MENUPAGE_MULTIPLAYER_RESULTS = 7,
    MENUPAGE_PLAY_INTRO = 8,
    MENUPAGE_CREDITS = 9,
    MENUPAGE_NICE_TRY = 10,
    MENUPAGE_RESULTS_PLAYER_QUIT = 11,
    MENUPAGE_12 = 12,
    MENUPAGE_13 = 13,
    MENUPAGE_PARENTAL_CONTROL = 14,
    MENUPAGE_15 = 15,
    NUM_MENUPAGES,

    // Not really pages
    MENUPAGE_GTA2MANAGER = 257,
    MENUPAGE_QUIT = 258,
    MENUPAGE_REPLAY_PREVIOUS_AREA = 259,
    MENUPAGE_LOADING_SAVE = 260,
    MENUPAGE_PLAY_NEXT_AREA = 261,
    MENUPAGE_GET_READY_TO_PLAY = 264,
    MENUPAGE_GET_READY_TO_PLAY_BONUS = 265,
    MENUPAGE_CONTINUE_NEXT_STAGE = 266,
};

enum ElementType
{
    NULL_TYPE_0 = 0,    //  i.e. don't exists
    STRING_TEXT_1 = 1,
    STRING_TEXT_2 = 2,
    GEOMETRIC_SHAPE_3 = 3,  //  triangles, circles
};

enum MultiplayerGameType
{
    FRAG_GAME_1 = 1,
    POINTS_GAME_2 = 2,
    TAG_GAME_3 = 3,
};

struct MainBlockStrings
{
    char field_0[256]; // debugstr
    char field_100[256]; // map name
    char field_200[256]; // sty name
};

struct Frontend
{
    LPDIRECTINPUTA field_0_pDInput;
    LPDIRECTINPUTDEVICEA field_4_pKeyboardDevice;
    char_type field_8_keys[256];
    s32 field_108_winmain_next_state;
    char_type field_10C_bKeyboardAcquired;
    char_type field_10D;
    char_type field_10E;
    char_type field_10F;
    s32 field_110_state;
    s32 field_114;
    s32 field_118;
    s16 field_11C;
    s16 field_11E;
    s16 field_120;
    s16 field_122;
    u16 field_124_font_type;
    s16 field_126;
    s16 field_128;
    s16 field_12A;
    s16 field_12C;
    s16 field_12E;
    s16 field_130;
    u16 field_132_f136_idx;
    s16 field_134;
    MenuPage_0xBCA field_136_menu_pages_array[17];
    wchar_t field_C9A0_curr_plyr_name[9];
    u8 field_C9B2_curr_plyr_name_length;
    char_type field_C9B3;
    s16 field_C9B4;
    s16 field_C9B6;
    wchar_t field_C9B8[9];
    //char_type field_C9C8;
    //char_type field_C9C9;
    u8 field_C9CA;
    char_type field_C9CB;
    char_type field_C9CC_left_pressed;
    char_type field_C9CD_right_pressed;
    char_type field_C9CE_up_pressed;
    char_type field_C9CF_down_pressed;
    char_type field_C9D0_return_pressed;
    char_type field_C9D1_escape_pressed;
    char_type field_C9D2_delete_pressed;
    char_type field_C9D3_left_key_down;
    char_type field_C9D4_right_key_down;
    char_type field_C9D5_up_key_down;
    char_type field_C9D6_down_key_down;
    char_type field_C9D7_return_key_down;
    char_type field_C9D8_escape_key_down;
    char_type field_C9D9_delete_key_down;
    char_type field_C9DA;
    char_type field_C9DB;
    s32 field_C9DC;
    char_type field_C9E0;
    char_type field_C9E1_bCheatsEnabled;
    char_type field_C9E2;
    char_type field_C9E3;
    s32 field_C9E4;
    MainBlockStrings field_C9E8_blocks[3][4];
    admiring_euler_4 field_EDE8_plySlots[8];
    char_type field_EE08;
    char_type field_EE09;
    u16 field_EE0A;
    u8 field_EE0C;
    u8 field_EE0D;
    xenodochial_morse field_EE0E_unk;
    s16 field_1EB30;
    s16 field_1EB32;
    Fix16 field_1EB34;
    s16 field_1EB38; // -11 bytes ?
    u8 field_1EB3A[8];
    u8 field_1EB42[8];
    char_type field_1EB4A;
    char_type field_1EB4B;
    char_type field_1EB4C;
    char_type field_1EB4D;
    char_type field_1EB4E;
    char_type field_1EB4F;
    u8 field_1EB50_idx;
    u8 field_1EB51_blocks[3];

    // todo: ordering
    EXPORT void LoadStringsFromStage_4B4C60(u16 mainBlockIdx, u16 bounusBlockIdx, char* pDebugStr, char* pMapName, char* pStyName);
    EXPORT void sub_4B4BC0(u16 mainBlockIdx, u16 bounusBlockIdx, const char* pDebugStr, const char* pMapName, const char* pStyName);
    EXPORT void sub_4B5270();

    // todo move to another file for ordering
    EXPORT static void __stdcall create_4ACFA0();

    EXPORT static void __stdcall destroy_4AD070();

    EXPORT void sub_4B3170(u16 arg0);

    EXPORT void sub_4B8680();

    EXPORT const char_type* intro_bik_4B5E50();

    EXPORT static bool intro_bik_exists_4B5FF0();

    EXPORT void sub_4AEC00();

    EXPORT void read_menu_input_4AFEB0();

    EXPORT void sub_4B3AF0(u16 menu_page_idx, u16 option_idx, wchar_t** w_buffer);

    EXPORT void sub_4B6780();

    EXPORT void sub_4B7A10();

    EXPORT void UpdatePageFromUserInput_4AE2D0();

    EXPORT void SetWinMainStateToBootMap_4AE990();

    EXPORT void sub_4B2F60();

    EXPORT void sub_4AE9A0();

    EXPORT void sub_4B8280();

    EXPORT void sub_4B4410();

    EXPORT player_stats_0xA4* sub_4B43E0();

    EXPORT void sub_4B42E0();

    EXPORT void sub_4B4230();

    EXPORT void HandleCheatCode_4B3DD0(const wchar_t* String);

    EXPORT void sub_4B4280();

    EXPORT u8 sub_4B77B0(player_stats_0xA4* a2);

    EXPORT u8 sub_4B7800(player_stats_0xA4* a2);

    EXPORT void sub_4B8530();

    EXPORT void sub_4B8560();

    EXPORT void sub_4B8020();

    EXPORT int __stdcall sub_4B7E10(s32 str_id_idx, u16 text_xpos, u16 text_ypos, s32 fontType, s32 draw_kind);

    EXPORT char_type sub_4B7FB0();

    EXPORT void LoadMapFilenames_4B4D00(u8 mainBlockIdx, u8 bonusBlockIdx);

    EXPORT void sub_4ADF50();

    EXPORT void sub_4ADFB0();

    EXPORT void DrawBackground_4B6E10();

    EXPORT void sub_4B6B00(u8 a1, BYTE* pTgaIdx, BYTE* a3);

    EXPORT void Load_tga_4B6520(u16 idx);

    EXPORT Frontend();

    EXPORT ~Frontend();

    EXPORT void AcquireKeyBoard_4AFD70();

    EXPORT void FreeKeyBoardDevice_4AFD00();

    EXPORT void FreeImageTable_4B6750();

    EXPORT char_type KeyBoard_GetKeyStates_4AFDD0();

    EXPORT void InitKeyBoardDevice_4AFBE0();

    EXPORT void sub_4AF0E0();

    EXPORT void sub_4B0220();

    EXPORT void sub_4B4440();

    EXPORT void GetSeqItem_4B48D0(s32 type, char_type *ppRet, FILE *hSeqFile);

    EXPORT void LoadPlySlotSvgs_4B53C0();

    EXPORT void Load_tgas_4B66B0();

    EXPORT void GetPlySlotSvgName_4B51D0(u8 idx, char_type* pStr);

    EXPORT char_type PlySlotSvgExists_4B5370(u8 idx);

    EXPORT static void __stdcall sub_5E53C0(BYTE* a1);

    EXPORT static s32 __stdcall sub_5D8990(wchar_t* pStr, u16 a2);

    EXPORT u16 sub_4B0190(wchar_t* a2, s16 a3, s32 a4);

    EXPORT u8 sub_4B7060(u8 a2);

    EXPORT u8 sub_4B7270(char_type a2);

    EXPORT bool sub_4B7490();

    EXPORT bool sub_4B74C0();

    EXPORT void sub_4B7550();

    EXPORT bool sub_4B6FF0();

    EXPORT void sub_4B42B0();

    EXPORT char_type sub_4B7120(char_type a2);

    EXPORT void sub_4B7610();

    EXPORT bool sub_4B70B0();

    EXPORT bool sub_4B74F0();

    EXPORT char_type sub_4B7360(char_type a2);

    EXPORT bool sub_4B7520();

    EXPORT bool sub_4B72F0();

    EXPORT bool sub_4B7200();

    EXPORT void sub_4B4EC0();

    EXPORT s32 sub_4AEDB0();

    EXPORT void sub_4AD140();

    EXPORT void sub_4B7AE0();

    EXPORT void sub_4B8650();

    EXPORT bool pre_intro_bik_exists_4B6030();

    EXPORT char_type* pre_intro_bik_4B5F20();

    EXPORT void sub_4B3CC0(u16 a2, u16 a3, wchar_t** a4);

    EXPORT void sub_4B55F0();

    EXPORT void sub_4B57B0(u16 a3, u16 a5);

    EXPORT void sub_4B78B0(wchar_t* pString, u16 text_xpos, u16 text_ypos, u16 arg_C, s32 a2, u16 a6, u16 a7, u8 pStr);

    EXPORT void Frontend::sub_4B5430(score_table_line* pStrings,
                           u16 text_xpos,
                           u16 text_ypos,
                           u16 num_entries,
                           u16 arg_fontType,
                           u16 draw_kind,
                           u8 spacing_type);
};

EXPORT s32 __stdcall SetGamma_5D9910(s32 gamma);

EXTERN_GLOBAL(Frontend*, gFrontend_67DC84);

EXTERN_GLOBAL_ARRAY(wchar_t, tmpBuff_67BD9C, 640);


EXPORT void __stdcall sub_5D7D30();
EXPORT void __cdecl FreeSurface_5D7DC0();
EXPORT void __stdcall sub_5D7CB0();

EXTERN_GLOBAL(short, font_type_703C14);

EXTERN_GLOBAL(s16, word_703C3C);

EXTERN_GLOBAL(s16, word_703D0C);

EXTERN_GLOBAL(s16, word_703C16);

EXTERN_GLOBAL(s16, word_703C8C);

EXTERN_GLOBAL(s16, word_703C8A);

EXTERN_GLOBAL(s16, word_703BE2);

EXTERN_GLOBAL(s16, word_703B88);

EXTERN_GLOBAL(s16, word_703DAC);

EXTERN_GLOBAL(s16, word_703B9C);

EXTERN_GLOBAL(u32, counter_706C4C);

EXTERN_GLOBAL(bool, gCheatOnlyMuggerPeds_67D5A4);
EXTERN_GLOBAL(bool, gCheatUnlimitedElectroGun_67D4F7);
EXTERN_GLOBAL(bool, gCheatAllGangMaxRespect_67D587);
EXTERN_GLOBAL(bool, gCheatOnlyElvisPeds_67D4ED);
EXTERN_GLOBAL(bool, gCheatNakedPeds_67D5E8);
EXTERN_GLOBAL(bool, gCheatGetBasicWeaponsMaxAmmo_67D545);
EXTERN_GLOBAL(bool, gCheatGet99Lives_67D4F1);
EXTERN_GLOBAL(bool, gCheatGetPlayerPoints_67D4C8);
EXTERN_GLOBAL(bool, gCheatUnlimitedFlameThrower_67D6CC);
EXTERN_GLOBAL(bool, gCheatUnknown_67D4F6);
EXTERN_GLOBAL(bool, gCheatGet10MillionMoney_67D6CE);
EXTERN_GLOBAL(bool, gCheat10xMultiplier_67D589);
EXTERN_GLOBAL(bool, gCheatUnlockThreeLevels_67D6CB);
EXTERN_GLOBAL(bool, gCheatUnlockLevelsOneAndTwo_67D584);
EXTERN_GLOBAL(bool, gCheatUnlockAllLevels_67D538);
EXTERN_GLOBAL(bool, gCheatUnlimitedDoubleDamage_67D57C);
EXTERN_GLOBAL(bool, gCheatInvisibility_67D539);
EXTERN_GLOBAL(bool, gCheatMiniCars_67D6C8);
