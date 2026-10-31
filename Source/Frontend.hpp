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

    EXPORT void LoadPlySlotSvg(const char_type* FileName);
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

    EXPORT bool SelectNextHorizontalIdx();

    EXPORT bool SelectPrevHorizontalIdx();

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

    EXPORT bool SelectPrevOption();

    EXPORT bool SelectNextOption();

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
    inline void SetField10D(const char_type& value)
    {
        mbInputEnabled = value;
    }

    EXPORT void LoadStringsFromStage(u16 mainBlockIdx, u16 bounusBlockIdx, char* pDebugStr, char* pMapName, char* pStyName);
    EXPORT void StoreStringsForStage(u16 mainBlockIdx, u16 bounusBlockIdx, const char* pDebugStr, const char* pMapName, const char* pStyName);
    EXPORT void DrawSavedStage();

    EXPORT static void __stdcall create();

    EXPORT static void __stdcall destroy();

    EXPORT void ChangeMenuPage(u16 menu_page_idx);

    EXPORT void InitSound();

    EXPORT const char_type* intro_bik();

    EXPORT static bool intro_bik_exists();

    EXPORT void Update();

    EXPORT void read_menu_input();

    EXPORT void GetOptionText(u16 menu_page_idx, u16 option_idx, wchar_t** w_buffer);

    EXPORT void UpdateMenuScreen();

    EXPORT void ManageCredits();

    EXPORT void UpdatePageFromUserInput();

    EXPORT void SetWinMainStateToBootMap();

    EXPORT void HandlePlayerNameTyping();

    EXPORT void HandleDeletePlayerDialog();

    EXPORT void HandlePasswordTyping();

    EXPORT void DeleteCurrentPlayer();

    EXPORT PlySlot* GetCurrPlayerStats();

    EXPORT void UpdateMenuForCurrPlayer();

    EXPORT void SaveAndUpdatePlayerName();

    EXPORT void HandleCheatCode(const wchar_t* String);

    EXPORT void LoadCurrPlayerName();

    EXPORT u8 GetLastUnlockedMainStage(PlySlot* pStats);

    EXPORT u8 GetLastUnlockedBonusStageCode(PlySlot* pStats);

    EXPORT void StripPasswordToCurrLength();

    EXPORT void CheckPassword();

    EXPORT void ContinueToNextStage();

    EXPORT void DrawBonusRating();
    EXPORT s32 DrawKeyHint(u8 str_id_idx, u16 text_xpos, u16 text_ypos, u16 fontType, s32 palette);

    EXPORT bool AreAllStagesUnlocked();

    EXPORT void LoadMapFilenames(u8 mainBlockIdx, u8 bonusBlockIdx);

    EXPORT void DrawLoading();

    EXPORT void DrawDeletePlayerDialog();

    EXPORT void DrawCurrentState();

    EXPORT void Render();

    EXPORT void DrawBackground();

    EXPORT void GetTgaIdxsForMenuScreen(u8 screen, u8* pTgaIdx, u8* pTgaIdx2);

    EXPORT void Load_tga(u16 idx);

    EXPORT Frontend();

    EXPORT ~Frontend();

    EXPORT void AcquireKeyBoard();

    EXPORT void FreeKeyBoardDevice();

    EXPORT void FreeImageTable();

    EXPORT bool KeyBoard_GetKeyStates();

    EXPORT void InitKeyBoardDevice();

    EXPORT void SetFontTypes();

    EXPORT void SetupMenuStringsOptionsElements();

    EXPORT void GetMainAndBonusStagesFromSeqFile();

    EXPORT void GetSeqItem(u8 type, char_type *ppRet, FILE *hSeqFile);

    EXPORT void LoadPlySlotSvgs();

    EXPORT void Load_tgas();

    EXPORT void GetPlySlotSvgName(u8 idx, char_type* pStr);

    EXPORT bool PlySlotSvgExists(u8 idx);

    EXPORT static void __stdcall SetInputEnabled(u8* pEnabled);

    EXPORT static s32 __stdcall GetMaxTextWidth(wchar_t* pStr, u16 font_type);

    EXPORT u16 GetCenteredXPos(wchar_t* pText, u16 fontType, s32 width);

    EXPORT u8 GetPreviousUnlockedMainStage(u8 stage_idx);

    EXPORT u8 GetNextUnlockedMainStage(char_type main_stage_idx);

    EXPORT bool ExistsPreviousMainStage();

    EXPORT bool ExistsNextMainStage();

    EXPORT void UpdateMainStageArrows();

    EXPORT bool ChangeMainStageToPrevious();

    EXPORT void StripPlayerNameToCurrLength();

    EXPORT char_type GetPreviousUnlockedBonusStage(u8 stage);

    EXPORT void UpdateBonusStageArrows();

    EXPORT bool ChangeBonusStageToPrevious();

    EXPORT bool ExistsPreviousBonusStage();

    EXPORT char_type GetNextUnlockedBonusStage(u8 stage);

    EXPORT bool ExistsNextBonusStage();

    EXPORT bool ChangeBonusStageToNext();

    EXPORT bool ChangeMainStageToNext();

    EXPORT void LoadMapFilenamesFromSvg();

    EXPORT s32 Run();

    EXPORT void DrawMenu();

    EXPORT void DrawCredits();

    EXPORT void FreeSound();

    EXPORT static bool pre_intro_bik_exists();

    EXPORT char_type* pre_intro_bik();

    EXPORT void GetElementText(u16 page_idx, u16 element_idx, wchar_t** ppText);

    EXPORT void DrawMultiplayerScores();

    EXPORT void DrawLastAndBestStats(u16 text_xpos, u16 text_ypos);

    EXPORT void DrawTextFixedWidth(wchar_t* pString, u16 text_xpos, u16 text_ypos, u16 font_type, u16 palette, u16 scale, u16 char_width, bool bRightAlign);

    EXPORT void Frontend::DrawScoreTable(ScoreTableLine* pStrings,
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
    EXPORT static s32 __stdcall GetCityInstalled();
    EXPORT static bool __stdcall CheckCityInstalled(u8 city_idx);
    EXPORT static void __cdecl MessageBox_4AE170(UINT uType, LPCSTR lpCaption, LPCSTR lpFormat, ...);
    EXPORT static LPCSTR __stdcall GetRegDword(HKEY hKey, LPCSTR lpValueName, LPCSTR default_value);
};

EXPORT s32 IsFullScreen_5D98D0();
EXPORT s32 __stdcall SetGamma(s32 gamma);

EXTERN_GLOBAL(Frontend*, gFrontend);

EXTERN_GLOBAL_ARRAY(wchar_t, tmpBuff, 640);


EXPORT void __stdcall MakeScreenTableAndSetWindow();
EXPORT void __cdecl FreeSurface();
EXPORT void __stdcall DrawDebugLine(s32 x1, s32 y1, s32 x2, s32 y2, u16 colour);
EXPORT void __stdcall ConvertColourBanks_5D7CB0();

EXTERN_GLOBAL(s16, font_type_703C14);

EXTERN_GLOBAL(s16, gFrontendHeadingFont);

EXTERN_GLOBAL(s16, gFrontendNormalFont);

EXTERN_GLOBAL(s16, gFrontendSelectedFont);

EXTERN_GLOBAL(s16, gFrontendFont6);

EXTERN_GLOBAL(s16, gFrontendPlayMenuScoreFont);

EXTERN_GLOBAL(s16, gFrontendScoreFont);

EXTERN_GLOBAL(s16, gFrontendAreaCompleteFont);

EXTERN_GLOBAL(s16, gFrontendFont12);

EXTERN_GLOBAL(s16, gFrontendTitleFont);

EXTERN_GLOBAL(u32, counter_706C4C);

EXTERN_GLOBAL(bool, gCheatOnlyMuggerPeds);
EXTERN_GLOBAL(bool, gCheatUnlimitedElectroGun);
EXTERN_GLOBAL(bool, gCheatAllGangMaxRespect);
EXTERN_GLOBAL(bool, gCheatOnlyElvisPeds);
EXTERN_GLOBAL(bool, gCheatNakedPeds);
EXTERN_GLOBAL(bool, gCheatGetBasicWeaponsMaxAmmo);
EXTERN_GLOBAL(bool, gCheatGet99Lives);
EXTERN_GLOBAL(bool, gCheatGetPlayerPoints);
EXTERN_GLOBAL(bool, gCheatUnlimitedFlameThrower);
EXTERN_GLOBAL(bool, gCheatUnlimitedJailCard);
EXTERN_GLOBAL(bool, gCheatGet10MillionMoney);
EXTERN_GLOBAL(bool, gCheat10xMultiplier);
EXTERN_GLOBAL(bool, gCheatUnlockThreeLevels);
EXTERN_GLOBAL(bool, gCheatUnlockLevelsOneAndTwo);
EXTERN_GLOBAL(bool, gCheatUnlockAllLevels);
EXTERN_GLOBAL(bool, gCheatUnlimitedDoubleDamage);
EXTERN_GLOBAL(bool, gCheatInvisibility);
EXTERN_GLOBAL(bool, gCheatMiniCars);
