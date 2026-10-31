#pragma once

#include "Function.hpp"
#include "multiplayer_game_type.hpp"
#include "CreditsLine.hpp"
#include "CreditsText.hpp"
#include "TagGame.hpp"
#include <windows.h>

#ifndef DIRECTINPUT_VERSION
    #define DIRECTINPUT_VERSION 0x0500
#endif

#include <Dinput.h>

class Player;
class Ang16;
struct ScoreTableLine;

#pragma pack(push)
#pragma pack(1)
struct SaveSlotInfo
{
    char_type field_0_map_name[25]; // 0x0
    char_type field_19_style_name[25]; // 0x19
    char_type field_32_script_name[25]; // 0x32
    char_type field_4B_last_saved_stage; // 0x4B
    char_type field_4C_last_saved_bonus_stage_code; // 0x4C
    char_type field_4D_last_saved_is_bonus; // 0x4D
    s16 field_4E_unused; // 0x4E
    s16 field_50_unused; // 0x50
};
GTA2_ASSERT_SIZEOF_ALWAYS(SaveSlotInfo, 0x52);
#pragma pack(pop)

struct SaveSlotSummary
{
    char_type field_0_save_exists; // 0x0
    char_type field_1_last_saved_stage; // 0x1
    char_type field_2_last_saved_bonus_stage_code; // 0x2
    char_type field_3_last_saved_is_bonus; // 0x3

    EXPORT SaveSlotSummary();

    EXPORT ~SaveSlotSummary();

    EXPORT void LoadPlySlotSvg_4B6480(const char_type* FileName);
};
GTA2_ASSERT_SIZEOF_ALWAYS(SaveSlotSummary, 0x4);

struct MenuElement
{
    EXPORT MenuElement();

    EXPORT ~MenuElement();

    u8 field_0_element_type; // 0x0
    char_type field_1_is_it_displayed; // 0x1
    s16 field_2_xpos; // 0x2
    s16 field_4_ypos; // 0x4
    union 
    {
        struct  // if field_0_element_type == STRING_TEXT_1 or STRING_TEXT_2
        {
            wchar_t field_6_element_name_str[50]; // 0x6
            u16 field_6A_font_type; // 0x6A
            u16 field_6C_font_palette; // 0x6C
        };
        u16 field_6_geometric_shape_type; // 0x6: if field_0_element_type == GEOMETRIC_SHAPE_3
    };
};
GTA2_ASSERT_SIZEOF_ALWAYS(MenuElement, 0x6E);

#pragma pack(push)
#pragma pack(1)
struct MenuItem
{
  public:
    EXPORT MenuItem();

    EXPORT ~MenuItem();

    EXPORT bool SelectNextHorizontalIdx_4B6330();

    EXPORT bool SelectPrevHorizontalIdx_4B6390();

    char_type field_0_option_type; // 0x0
    char_type field_1_is_unlocked; // 0x1
    s16 field_2_x_pos; // 0x2
    s16 field_4_y_pos; // 0x4
    wchar_t field_6_option_name_str[50]; // 0x6
    u16 field_6A_font_type; // 0x6A
    u16 field_6C_palette; // 0x6C
    u16 field_6E_horizontal_selected_idx; // 0x6E
    s16 field_70_unused; // 0x70
    char_type field_72_horizontal_idx_enabled[12]; // 0x72
    u16 field_7E_horizontal_max_idx; // 0x7E
    s16 field_80_menu_page_target; // 0x80
};
GTA2_ASSERT_SIZEOF_ALWAYS(MenuItem, 0x82);
#pragma pack(pop)

struct MenuOption
{
  public:
    EXPORT MenuOption();

    EXPORT ~MenuOption();

    s16 field_0_unused; // 0x0
    s16 field_2_unused; // 0x2
    char_type field_4_is_option_unlocked;   // 0x4: this control if the menu selection will skip a option or not
    char_type field_5_unused; // 0x5
};
GTA2_ASSERT_SIZEOF_ALWAYS(MenuOption, 0x6);

struct MenuPage
{
    EXPORT MenuPage();

    EXPORT ~MenuPage();

    EXPORT bool SelectPrevOption_4B61B0();

    EXPORT bool SelectNextOption_4B6200();

    u16 field_0_number_of_options; // 0x0
    u16 field_2_number_of_elements; // 0x2
    MenuItem field_4_options_array[10]; // 0x4
    MenuElement field_518_elements_array[15]; // 0x518
    MenuOption field_B8A_option_states[10]; // 0xB8A
    u16 field_BC6_current_option_idx; // 0xBC6
    s16 field_BC8_default_option_idx; // 0xBC8
};
GTA2_ASSERT_SIZEOF_ALWAYS(MenuPage, 0xBCA);

struct PlySlot;

enum WinMainState
{
    Quit_1 = 1,
    Run_Frontend_2 = 2,
    Start_Game_3 = 3,
    Start_Replay_4 = 4,
};

namespace FrontendState
{
enum FrontendState
{
    Menu_Navigation_1 = 1,
    Booting_Map_2 = 2,
    User_Typing_New_Player_Name_3 = 3,
    Delete_Player_Dialog_4 = 4,
    User_Typing_Password_5 = 5,
};
} // namespace FrontendState

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

enum MenuPages // enum for field_132_current_page_idx
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

enum MenuOptionType
{
    NO_TYPE_0 = 0,
    ENTERABLE_1 = 1,  // Press "enter" to get into a new frontend page
    HORIZONTAL_SELECTION_2 = 2, // The left/right arrows step through its values (e.g. the player slot)
};

struct MainBlockStrings
{
    char field_0_gmp_name[256]; // 0x0
    char field_100_sty_name[256]; // 0x100
    char field_200_scr_name[256]; // 0x200
};

struct Frontend
{
    STATIC_MEMBER_ARRAY(char_type, sBikDataDir, 8);
    LPDIRECTINPUTA field_0_pDInput; // 0x0
    LPDIRECTINPUTDEVICEA field_4_pKeyboardDevice; // 0x4
    char_type field_8_keys[256]; // 0x8
    s32 field_108_winmain_next_state; // 0x108
    bool field_10C_bKeyboardAcquireFailed; // 0x10C
    char_type field_10D_bInputEnabled; // 0x10D
    s32 field_110_state; // 0x110
    s32 field_114_cursor_blink; // 0x114
    s32 field_118_cursor_blink_timer; // 0x118
    u16 field_11C_normal_font; // 0x11C
    s16 field_11E_text_font; // 0x11E
    s16 field_120_selected_font; // 0x120
    s16 field_122_unused; // 0x122
    u16 field_124_font_type; // 0x124
    s16 field_126_heading_font; // 0x126
    s16 field_128_play_menu_score_font; // 0x128
    s16 field_12A_score_font; // 0x12A
    s16 field_12C_area_complete_font; // 0x12C
    s16 field_12E_unused; // 0x12E
    s16 field_130_title_font; // 0x130
    u16 field_132_current_page_idx; // 0x132
    s16 field_134_unused; // 0x134
    MenuPage field_136_menu_pages_array[17]; // 0x136
    wchar_t field_C9A0_curr_plyr_name[9]; // 0xC9A0
    u8 field_C9B2_curr_plyr_name_length; // 0xC9B2
    bool field_C9B3_key_held; // 0xC9B3
    s16 field_C9B4_last_key; // 0xC9B4
    s16 field_C9B6_key_repeat_timer; // 0xC9B6
    wchar_t field_C9B8_password[9]; // 0xC9B8
    u8 field_C9CA_password_length; // 0xC9CA
    bool field_C9CB_wrong_password_shown; // 0xC9CB
    bool field_C9CC_left_pressed; // 0xC9CC
    bool field_C9CD_right_pressed; // 0xC9CD
    bool field_C9CE_up_pressed; // 0xC9CE
    bool field_C9CF_down_pressed; // 0xC9CF
    bool field_C9D0_return_pressed; // 0xC9D0
    bool field_C9D1_escape_pressed; // 0xC9D1
    bool field_C9D2_delete_pressed; // 0xC9D2
    char_type field_C9D3_left_key_down; // 0xC9D3
    char_type field_C9D4_right_key_down; // 0xC9D4
    char_type field_C9D5_up_key_down; // 0xC9D5
    char_type field_C9D6_down_key_down; // 0xC9D6
    char_type field_C9D7_return_key_down; // 0xC9D7
    char_type field_C9D8_escape_key_down; // 0xC9D8
    char_type field_C9D9_delete_key_down; // 0xC9D9
    char_type field_C9DA_unused; // 0xC9DA
    char_type field_C9DB_unused; // 0xC9DB
    s32 field_C9DC_next_frame_time; // 0xC9DC
    char_type field_C9E0_updates_since_render; // 0xC9E0
    bool field_C9E1_bCheatsEnabled; // 0xC9E1
    char_type field_C9E2_unused; // 0xC9E2
    char_type field_C9E3_unused; // 0xC9E3
    s32 field_C9E4_last_input_time; // 0xC9E4
    MainBlockStrings field_C9E8_blocks[3][4]; // 0xC9E8
    SaveSlotSummary field_EDE8_plySlots[8]; // 0xEDE8
    char_type field_EE08_menu_screen; // 0xEE08
    char_type field_EE09_unused; // 0xEE09
    u16 field_EE0A_dialog_cursor_ypos; // 0xEE0A
    u8 field_EE0C_dialog_type; // 0xEE0C
    u8 field_EE0D_hiscore_table_idx; // 0xEE0D
    CreditsText field_EE0E_credits; // 0xEE0E
    u16 field_1EB30_credits_scroll_timer; // 0x1EB30
    s16 field_1EB32_unused; // 0x1EB32
    Fix16 field_1EB34_credits_ypos; // 0x1EB34
    u16 field_1EB38_credits_line_idx; // 0x1EB38
    u8 field_1EB3A_selected_main_stage[8]; // 0x1EB3A
    u8 field_1EB42_selected_bonus_stage[8]; // 0x1EB42
    bool field_1EB4A_has_prev_player_slot; // 0x1EB4A
    bool field_1EB4B_has_next_player_slot; // 0x1EB4B
    bool field_1EB4C_has_prev_main_stage; // 0x1EB4C
    bool field_1EB4D_has_next_main_stage; // 0x1EB4D
    bool field_1EB4E_has_prev_bonus_stage; // 0x1EB4E
    bool field_1EB4F_has_next_bonus_stage; // 0x1EB4F
    u8 field_1EB50_num_main_stages; // 0x1EB50
    u8 field_1EB51_num_bonus_stages[3]; // 0x1EB51

    // 9.6f 0x453A30
    inline void SetField10D_453A30(const char_type& value)
    {
        field_10D_bInputEnabled = value;
    }

    EXPORT void LoadStringsFromStage_4B4C60(u16 mainBlockIdx, u16 bounusBlockIdx, char* pDebugStr, char* pMapName, char* pStyName);
    EXPORT void StoreStringsForStage_4B4BC0(u16 mainBlockIdx, u16 bounusBlockIdx, const char* pDebugStr, const char* pMapName, const char* pStyName);
    EXPORT void DrawSavedStage_4B5270();

    EXPORT static void __stdcall create_4ACFA0();

    EXPORT static void __stdcall destroy_4AD070();

    EXPORT void ChangeMenuPage_4B3170(u16 menu_page_idx);

    EXPORT void InitSound_4B8680();

    EXPORT const char_type* intro_bik_4B5E50();

    EXPORT static bool intro_bik_exists_4B5FF0();

    EXPORT void Update_4AEC00();

    EXPORT void read_menu_input_4AFEB0();

    EXPORT void GetOptionText_4B3AF0(u16 menu_page_idx, u16 option_idx, wchar_t** w_buffer);

    EXPORT void UpdateMenuScreen_4B6780();

    EXPORT void ManageCredits_4B7A10();

    EXPORT void UpdatePageFromUserInput_4AE2D0();

    EXPORT void SetWinMainStateToBootMap_4AE990();

    EXPORT void HandlePlayerNameTyping_4B2F60();

    EXPORT void HandleDeletePlayerDialog_4AE9A0();

    EXPORT void HandlePasswordTyping_4B8280();

    EXPORT void DeleteCurrentPlayer_4B4410();

    EXPORT PlySlot* GetCurrPlayerStats_4B43E0();

    EXPORT void UpdateMenuForCurrPlayer_4B42E0();

    EXPORT void SaveAndUpdatePlayerName_4B4230();

    EXPORT void HandleCheatCode_4B3DD0(const wchar_t* String);

    EXPORT void LoadCurrPlayerName_4B4280();

    EXPORT u8 GetLastUnlockedMainStage_4B77B0(PlySlot* pStats);

    EXPORT u8 GetLastUnlockedBonusStageCode_4B7800(PlySlot* pStats);

    EXPORT void StripPasswordToCurrLength_4B8530();

    EXPORT void CheckPassword_4B8560();

    EXPORT void ContinueToNextStage_4B8020();

    EXPORT void DrawBonusRating_4B7D60();
    EXPORT s32 DrawKeyHint_4B7E10(u8 str_id_idx, u16 text_xpos, u16 text_ypos, u16 fontType, s32 palette);

    EXPORT bool AreAllStagesUnlocked_4B7FB0();

    EXPORT void LoadMapFilenames_4B4D00(u8 mainBlockIdx, u8 bonusBlockIdx);

    EXPORT void DrawLoading_4AD0D0();

    EXPORT void DrawDeletePlayerDialog_4ADDE0();

    EXPORT void DrawCurrentState_4ADF50();

    EXPORT void Render_4ADFB0();

    EXPORT void DrawBackground_4B6E10();

    EXPORT void GetTgaIdxsForMenuScreen_4B6B00(u8 screen, u8* pTgaIdx, u8* pTgaIdx2);

    EXPORT void Load_tga_4B6520(u16 idx);

    EXPORT Frontend();

    EXPORT ~Frontend();

    EXPORT void AcquireKeyBoard_4AFD70();

    EXPORT void FreeKeyBoardDevice_4AFD00();

    EXPORT void FreeImageTable_4B6750();

    EXPORT bool KeyBoard_GetKeyStates_4AFDD0();

    EXPORT void InitKeyBoardDevice_4AFBE0();

    EXPORT void SetFontTypes_4AF0E0();

    EXPORT void SetupMenuStringsOptionsElements_4B0220();

    EXPORT void GetMainAndBonusStagesFromSeqFile_4B4440();

    EXPORT void GetSeqItem_4B48D0(u8 type, char_type *ppRet, FILE *hSeqFile);

    EXPORT void LoadPlySlotSvgs_4B53C0();

    EXPORT void Load_tgas_4B66B0();

    EXPORT void GetPlySlotSvgName_4B51D0(u8 idx, char_type* pStr);

    EXPORT bool PlySlotSvgExists_4B5370(u8 idx);

    EXPORT static void __stdcall SetInputEnabled_5E53C0(u8* pEnabled);

    EXPORT static s32 __stdcall GetMaxTextWidth_5D8990(wchar_t* pStr, u16 font_type);

    EXPORT u16 GetCenteredXPos_4B0190(wchar_t* pText, u16 fontType, s32 width);

    EXPORT u8 GetPreviousUnlockedMainStage_4B7060(u8 stage_idx);

    EXPORT u8 GetNextUnlockedMainStage_4B7270(char_type main_stage_idx);

    EXPORT bool ExistsPreviousMainStage_4B7490();

    EXPORT bool ExistsNextMainStage_4B74C0();

    EXPORT void UpdateMainStageArrows_4B7550();

    EXPORT bool ChangeMainStageToPrevious_4B6FF0();

    EXPORT void StripPlayerNameToCurrLength_4B42B0();

    EXPORT char_type GetPreviousUnlockedBonusStage_4B7120(u8 stage);

    EXPORT void UpdateBonusStageArrows_4B7610();

    EXPORT bool ChangeBonusStageToPrevious_4B70B0();

    EXPORT bool ExistsPreviousBonusStage_4B74F0();

    EXPORT char_type GetNextUnlockedBonusStage_4B7360(u8 stage);

    EXPORT bool ExistsNextBonusStage_4B7520();

    EXPORT bool ChangeBonusStageToNext_4B72F0();

    EXPORT bool ChangeMainStageToNext_4B7200();

    EXPORT void LoadMapFilenamesFromSvg_4B4EC0();

    EXPORT s32 Run_4AEDB0();

    EXPORT void DrawMenu_4AD140();

    EXPORT void DrawCredits_4B7AE0();

    EXPORT void FreeSound_4B8650();

    EXPORT static bool pre_intro_bik_exists_4B6030();

    EXPORT char_type* pre_intro_bik_4B5F20();

    EXPORT void GetElementText_4B3CC0(u16 page_idx, u16 element_idx, wchar_t** ppText);

    EXPORT void DrawMultiplayerScores_4B55F0();

    EXPORT void DrawLastAndBestStats_4B57B0(u16 text_xpos, u16 text_ypos);

    EXPORT void DrawTextFixedWidth_4B78B0(wchar_t* pString, u16 text_xpos, u16 text_ypos, u16 font_type, u16 palette, u16 scale, u16 char_width, bool bRightAlign);

    EXPORT void Frontend::DrawScoreTable_4B5430(ScoreTableLine* pStrings,
                           u16 text_xpos,
                           u16 text_ypos,
                           u16 num_entries,
                           u16 arg_fontType,
                           u16 palette,
                           u8 spacing_type);
};

class FreeLoader
{
  public:
    EXPORT static s32 __stdcall GetCityInstalled_4AE0F0();
    EXPORT static bool __stdcall CheckCityInstalled_4AE1F0(u8 city_idx);
    EXPORT static void __cdecl MessageBox_4AE170(UINT uType, LPCSTR lpCaption, LPCSTR lpFormat, ...);
    EXPORT static LPCSTR __stdcall GetRegDword_4AE010(HKEY hKey, LPCSTR lpValueName, LPCSTR default_value);
};

EXPORT s32 IsFullScreen_5D98D0();
EXPORT s32 __stdcall SetGamma_5D9910(s32 gamma);

EXTERN_GLOBAL(Frontend*, gFrontend_67DC84);

EXTERN_GLOBAL_ARRAY(wchar_t, tmpBuff_67BD9C, 640);


EXPORT void __stdcall MakeScreenTableAndSetWindow_5D7D30();
EXPORT void __cdecl FreeSurface_5D7DC0();
EXPORT void __stdcall DrawDebugLine_5D7DD0(s32 x1, s32 y1, s32 x2, s32 y2, u16 colour);
EXPORT void __stdcall ConvertColourBanks_5D7CB0();

EXTERN_GLOBAL(s16, font_type_703C14);

EXTERN_GLOBAL(s16, gFrontendHeadingFont_703C3C);

EXTERN_GLOBAL(s16, gFrontendNormalFont_703D0C);

EXTERN_GLOBAL(s16, gFrontendSelectedFont_703C16);

EXTERN_GLOBAL(s16, gFrontendFont6_703C8C);

EXTERN_GLOBAL(s16, gFrontendPlayMenuScoreFont_703C8A);

EXTERN_GLOBAL(s16, gFrontendScoreFont_703BE2);

EXTERN_GLOBAL(s16, gFrontendAreaCompleteFont_703B88);

EXTERN_GLOBAL(s16, gFrontendFont12_703DAC);

EXTERN_GLOBAL(s16, gFrontendTitleFont_703B9C);

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
EXTERN_GLOBAL(bool, gCheatUnlimitedJailCard_67D4F6);
EXTERN_GLOBAL(bool, gCheatGet10MillionMoney_67D6CE);
EXTERN_GLOBAL(bool, gCheat10xMultiplier_67D589);
EXTERN_GLOBAL(bool, gCheatUnlockThreeLevels_67D6CB);
EXTERN_GLOBAL(bool, gCheatUnlockLevelsOneAndTwo_67D584);
EXTERN_GLOBAL(bool, gCheatUnlockAllLevels_67D538);
EXTERN_GLOBAL(bool, gCheatUnlimitedDoubleDamage_67D57C);
EXTERN_GLOBAL(bool, gCheatInvisibility_67D539);
EXTERN_GLOBAL(bool, gCheatMiniCars_67D6C8);
