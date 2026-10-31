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
    char_type mMapName[25]; // 0x0
    char_type mStyleName[25]; // 0x19
    char_type mScriptName[25]; // 0x32
    char_type mLastSavedStage; // 0x4B
    char_type mLastSavedBonusStageCode; // 0x4C
    char_type mLastSavedIsBonus; // 0x4D
    s16 mUnused4E; // 0x4E
    s16 mUnused50; // 0x50
};
GTA2_ASSERT_SIZEOF_ALWAYS(SaveSlotInfo, 0x52);
#pragma pack(pop)

struct SaveSlotSummary
{
    char_type mSaveExists; // 0x0
    char_type mLastSavedStage; // 0x1
    char_type mLastSavedBonusStageCode; // 0x2
    char_type mLastSavedIsBonus; // 0x3

    EXPORT SaveSlotSummary();

    EXPORT ~SaveSlotSummary();

    EXPORT void LoadPlySlotSvg_4B6480(const char_type* FileName);
};
GTA2_ASSERT_SIZEOF_ALWAYS(SaveSlotSummary, 0x4);

struct MenuElement
{
    EXPORT MenuElement();

    EXPORT ~MenuElement();

    u8 mElementType; // 0x0
    char_type mIsItDisplayed; // 0x1
    s16 mXpos; // 0x2
    s16 mYpos; // 0x4
    union 
    {
        struct  // if mElementType == STRING_TEXT_1 or STRING_TEXT_2
        {
            wchar_t mElementNameStr[50]; // 0x6
            u16 mFontType; // 0x6A
            u16 mFontPalette; // 0x6C
        };
        u16 mGeometricShapeType; // 0x6: if mElementType == GEOMETRIC_SHAPE_3
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

    char_type mOptionType; // 0x0
    char_type mIsUnlocked; // 0x1
    s16 mXPos; // 0x2
    s16 mYPos; // 0x4
    wchar_t mOptionNameStr[50]; // 0x6
    u16 mFontType; // 0x6A
    u16 mPalette; // 0x6C
    u16 mHorizontalSelectedIdx; // 0x6E
    s16 mUnused70; // 0x70
    char_type mHorizontalIdxEnabled[12]; // 0x72
    u16 mHorizontalMaxIdx; // 0x7E
    s16 mMenuPageTarget; // 0x80
};
GTA2_ASSERT_SIZEOF_ALWAYS(MenuItem, 0x82);
#pragma pack(pop)

struct MenuOption
{
  public:
    EXPORT MenuOption();

    EXPORT ~MenuOption();

    s16 mUnused0; // 0x0
    s16 mUnused2; // 0x2
    char_type mIsOptionUnlocked;   // 0x4: this control if the menu selection will skip a option or not
    char_type mUnused5; // 0x5
};
GTA2_ASSERT_SIZEOF_ALWAYS(MenuOption, 0x6);

struct MenuPage
{
    EXPORT MenuPage();

    EXPORT ~MenuPage();

    EXPORT bool SelectPrevOption_4B61B0();

    EXPORT bool SelectNextOption_4B6200();

    u16 mNumberOfOptions; // 0x0
    u16 mNumberOfElements; // 0x2
    MenuItem mOptionsArray[10]; // 0x4
    MenuElement mElementsArray[15]; // 0x518
    MenuOption mOptionStates[10]; // 0xB8A
    u16 mCurrentOptionIdx; // 0xBC6
    s16 mDefaultOptionIdx; // 0xBC8
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

enum MenuPages // enum for mCurrentPageIdx
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
    char mGmpName[256]; // 0x0
    char mStyName[256]; // 0x100
    char mScrName[256]; // 0x200
};

struct Frontend
{
    STATIC_MEMBER_ARRAY(char_type, sBikDataDir, 8);
    LPDIRECTINPUTA mpDInput; // 0x0
    LPDIRECTINPUTDEVICEA mpKeyboardDevice; // 0x4
    char_type mKeys[256]; // 0x8
    s32 mWinmainNextState; // 0x108
    bool mbKeyboardAcquireFailed; // 0x10C
    char_type mbInputEnabled; // 0x10D
    s32 mState; // 0x110
    s32 mCursorBlink; // 0x114
    s32 mCursorBlinkTimer; // 0x118
    u16 mNormalFont; // 0x11C
    s16 mTextFont; // 0x11E
    s16 mSelectedFont; // 0x120
    s16 mUnused122; // 0x122
    u16 mFontType; // 0x124
    s16 mHeadingFont; // 0x126
    s16 mPlayMenuScoreFont; // 0x128
    s16 mScoreFont; // 0x12A
    s16 mAreaCompleteFont; // 0x12C
    s16 mUnused12E; // 0x12E
    s16 mTitleFont; // 0x130
    u16 mCurrentPageIdx; // 0x132
    s16 mUnused134; // 0x134
    MenuPage mMenuPagesArray[17]; // 0x136
    wchar_t mCurrPlyrName[9]; // 0xC9A0
    u8 mCurrPlyrNameLength; // 0xC9B2
    bool mKeyHeld; // 0xC9B3
    s16 mLastKey; // 0xC9B4
    s16 mKeyRepeatTimer; // 0xC9B6
    wchar_t mPassword[9]; // 0xC9B8
    u8 mPasswordLength; // 0xC9CA
    bool mWrongPasswordShown; // 0xC9CB
    bool mLeftPressed; // 0xC9CC
    bool mRightPressed; // 0xC9CD
    bool mUpPressed; // 0xC9CE
    bool mDownPressed; // 0xC9CF
    bool mReturnPressed; // 0xC9D0
    bool mEscapePressed; // 0xC9D1
    bool mDeletePressed; // 0xC9D2
    char_type mLeftKeyDown; // 0xC9D3
    char_type mRightKeyDown; // 0xC9D4
    char_type mUpKeyDown; // 0xC9D5
    char_type mDownKeyDown; // 0xC9D6
    char_type mReturnKeyDown; // 0xC9D7
    char_type mEscapeKeyDown; // 0xC9D8
    char_type mDeleteKeyDown; // 0xC9D9
    char_type mUnusedC9DA; // 0xC9DA
    char_type mUnusedC9DB; // 0xC9DB
    s32 mNextFrameTime; // 0xC9DC
    char_type mUpdatesSinceRender; // 0xC9E0
    bool mbCheatsEnabled; // 0xC9E1
    char_type mUnusedC9E2; // 0xC9E2
    char_type mUnusedC9E3; // 0xC9E3
    s32 mLastInputTime; // 0xC9E4
    MainBlockStrings mBlocks[3][4]; // 0xC9E8
    SaveSlotSummary mPlySlots[8]; // 0xEDE8
    char_type mMenuScreen; // 0xEE08
    char_type mUnusedEE09; // 0xEE09
    u16 mDialogCursorYpos; // 0xEE0A
    u8 mDialogType; // 0xEE0C
    u8 mHiscoreTableIdx; // 0xEE0D
    CreditsText mCredits; // 0xEE0E
    u16 mCreditsScrollTimer; // 0x1EB30
    s16 mUnused1EB32; // 0x1EB32
    Fix16 mCreditsYpos; // 0x1EB34
    u16 mCreditsLineIdx; // 0x1EB38
    u8 mSelectedMainStage[8]; // 0x1EB3A
    u8 mSelectedBonusStage[8]; // 0x1EB42
    bool mHasPrevPlayerSlot; // 0x1EB4A
    bool mHasNextPlayerSlot; // 0x1EB4B
    bool mHasPrevMainStage; // 0x1EB4C
    bool mHasNextMainStage; // 0x1EB4D
    bool mHasPrevBonusStage; // 0x1EB4E
    bool mHasNextBonusStage; // 0x1EB4F
    u8 mNumMainStages; // 0x1EB50
    u8 mNumBonusStages[3]; // 0x1EB51

    // 9.6f 0x453A30
    inline void SetField10D_453A30(const char_type& value)
    {
        mbInputEnabled = value;
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
