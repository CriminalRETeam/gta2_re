#pragma once

#include "Function.hpp"
#include <windows.h>

struct Network_NameList;
struct Network_PlayerPing;
struct Network_ChatMessage;

EXPORT char_type* __stdcall GetNarrowText(const char_type* Key);
EXPORT void __stdcall WideToDbcs(char_type* pDst, wchar_t* pSrc);

enum Network_UI_Window
{
    NETWORK_MENU_0 = 0,
    HOST_LOBBY_1 = 1,
    GUEST_LOBBY_2 = 2,
};

enum Network_UI_Control_Ids
{
    LISTBOX_ALL_NET_GAMES_1000 = 1000,
    BUTTON_JOIN_GAME_1001 = 1001,
    BUTTON_CREATE_GAME_1002 = 1002,
    LABEL_PLAYER_NAME_1003 = 1003,
    BUTTON_REJECT_PLAYER_1020 = 1020,
    BUTTON_START_GAME_1021 = 1021,
    CHATBOX_HOST_1022 = 1022,
    BUTTON_SEND_CHAT_MESSAGE_HOST_1023 = 1023,
    TEXTFIELD_CHAT_HOST_1025 = 1025,
    COMBO_MAP_LIST_1026 = 1026,
    LABEL_COPS_1028 = 1028,
    LABEL_PLAYERS_REQUIRED_1033 = 1033,
    COMBO_GAME_TYPE_1036 = 1036u,
    TEXTBOX_FRAG_LIMIT_NUMBER_1059 = 1059u,
    LABEL_FRAG_LIMIT_TEXT_1035 = 1035u,
    TRACKBAR_GAME_SPEED_1031 = 1031,
    LABEL_GAME_SPEED_TEXT_1032 = 1032u,
    LABEL_TIME_LIMIT_MINUTES_1037 = 1037,
    TEXTBOX_GAME_TIME_LIMIT_1038 = 1038u,
    CHECKBOX_POLICE_ENABLED_1027 = 1027u,
    LISTVIEW_JOINED_PLAYERS_1050 = 1050u,
    CHATBOX_GUEST_1051 = 1051,
    BUTTON_SEND_CHAT_MESSAGE_GUEST_1052 = 1052,
    TEXTFIELD_CHAT_GUEST_1053 = 1053,
    LISTVIEW_HOST_PLAYERS_1024 = 1024u,
    LABEL_JOINED_GAME_TIME_LIMIT_1060 = 1060u,
    LABEL_JOINED_GAME_FRAG_LIMIT_1058 = 1058u,
    LABEL_JOINED_GAME_TYPE_1057 = 1057u,
    LABEL_JOINED_GAME_POLICE_ENABLED_1056 = 1056u,
    LISTBOX_UNKNOWN_1000 = 1000u,
    TEXTBOX_PLAYER_NAME_1004 = 1004u,
    LABEL_GAME_SPEED_1055 = 1055u,
    LABEL_JOINED_GAME_MAP_1054 = 1054u,
};

struct Network_Enumerated_Map
{
    char mMapName[260]; // 0x0
    char mStyleName[260]; // 0x104
    char mScriptName[260]; // 0x208
    char mDescription[260]; // 0x30C
    char mMmpName[260]; // 0x410
    u32 mPlayerCount; // 0x514
};

struct NetworkGameSettings
{
    char mDefaultMap[260]; // 0x20088
    u32 mMapIdx; // 0x2018C
    u32 mGameSpeed; // 0x20190
    u32 mFragLimit; // 0x20194
    u32 mGameType; // 0x20198
    u32 mTickCount; // 0x2019C
    u32 mPoliceOn; // 0x201A0
    u32 mGameTimeLimit; // 0x201A4
};

class NetUi
{
  public:
    STATIC_MEMBER(UINT_PTR, sTimerId);
    EXPORT static void DbcsToWide(wchar_t* pDst, char_type* pSrc);
    EXPORT static void GetString(wchar_t* Dest, char_type* Source, size_t MaxCount);
    EXPORT NetUi();
    EXPORT virtual ~NetUi();
    EXPORT s32 ShowNetworkUiBlocking(HINSTANCE hInstance);
    EXPORT static int __stdcall DialogFunc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
    EXPORT static s32 __stdcall cb_FillSessionList(NetUi* pThis, Network_NameList* pSessions);
    EXPORT void SetDlgHwnd(HWND hDlg);
    EXPORT HWND Get_2031C_HWND();
    EXPORT static void __stdcall cb_AddPlayerToList(NetUi* pNetUi, wchar_t* pName, s32 player_idx);
    EXPORT static void __stdcall OnPaint(HWND hWnd);
    EXPORT static void __stdcall OnWmCommand(HWND hDlg, s32 id, HWND hCtl, s32 notify);
    EXPORT static void __stdcall OnTimer(HWND hWnd, s32 timer_id);
    EXPORT void CreateMainUi(HWND hWndParent);
    EXPORT void ShowSpecificWindow(s32 showing_specific_window_idx);
    EXPORT static s32 OnInitDialog(HWND hWnd, s32 lParam, NetUi* thisPtr);
    EXPORT s32 Get_20310_showing_window_idx();
    EXPORT static void __stdcall cb_RemovePlayerFromList(NetUi* pNetUi, wchar_t* Source);
    EXPORT static void _stdcall cb_Disconnect(NetUi* pNetUi);
    EXPORT void ClearListView(s32 nIDDlgItem);
    EXPORT static s32 __stdcall cb_SetPlayerPing(NetUi* pNetUi, Network_PlayerPing* pPing);
    EXPORT void PopulateMainUI();
    EXPORT static LRESULT __stdcall cb_AddChatMessage(NetUi* pNetUi, Network_ChatMessage* pMsg);
    EXPORT s32 AppendChatMessage(s32 hmenu, const char_type* pMsg);
    EXPORT void SetPlayerNameText();
    EXPORT static void __stdcall cb_GameSettingsChanged(NetUi* pNetUi, const char_type** ppData);
    EXPORT void ApplyHostGameSettings(const char_type* pData);
    EXPORT void SetSetting(s32 setting, s32 value);
    EXPORT void IncCount();
    EXPORT u32 GetCount();
    EXPORT void DecCount();
    EXPORT void ResetCount();
    EXPORT static void __stdcall cb_SavePlayerName(NetUi* pThis);
    EXPORT void SetEndDialog(s32 bEnd);
    EXPORT s32 GetEndDialog();
    EXPORT void LoadPlayerNameFromRegistry();
    EXPORT void SetPlayNameAndSaveToRegistry(const wchar_t* pPlayerNameW, const char* pPlayerNameA);
    EXPORT static LRESULT __stdcall subclass_proc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
    EXPORT void OnEnterPressed(s32 nIDDlgItem, s32 hmenu);
    EXPORT void EnumerateMaps();
    EXPORT static void __stdcall OnWmHScroll(HWND hWnd, HWND hTrackBar, s32 code, s32 pos);
    EXPORT void CopyGameSettings(NetworkGameSettings* pSettings);
    EXPORT void LoadGameSettingsFromRegistry();
    EXPORT char_type* GetMapName();
    EXPORT char_type* GetMapStyName();
    EXPORT char_type* GetMapScrName();
    EXPORT static void __stdcall ShowUnableToOpenFileError(const char_type* pMmpName, const char_type* pFileName);
    EXPORT void SetPlayerCountText(s32 count, HWND hDlg);
    EXPORT void UpdateButtonsEnabledState();
    EXPORT void SetPoliceEnabledCheckBox(s32 bPoliceOn, HWND hDlg);
    EXPORT void SetJoinedGamePoliceEnabledText(s32 bPoliceOn, HWND hDlg);
    EXPORT void SetFragsNumberAndLabel(s32 gameType, s32 fragLimit, HWND hDlg);
    EXPORT void SetGameSpeedTextLabelAndSlider(LPARAM game_speed, HWND hDlg);
    EXPORT void SetJoinedGameTypeAndFragLimitText(s32 game_type, s32 frag_limit, HWND hDlg);
    EXPORT void SetJoinedGameTimeLimitText(s32 gameTimeLimit, HWND hDlg);
    EXPORT void SetGameTimeLimitTextBox(s32 timeLimitMins, HWND hDlg);

    //s32 field_0_vTable; // added by the compiler
    Network_Enumerated_Map mMaps[100]; // 0x4
    s32 mTotalMapCount; // 0x1FD64
    s32 mbEndDialog; // 0x1FD68
    s32 mCount; // 0x1FD6C
    WNDPROC mOldProc1FD70; // 0x1FD70
    WNDPROC mOldProc1FD74; // 0x1FD74
    wchar_t mPlayerName2[260]; // 0x1FD78
    char_type mPlayerName[260]; // 0x1FF80
    s32 mUnused20084; // 0x20084
    NetworkGameSettings mGameSettings; // 0x20088
    HWND mUiControlHwnds[3][30]; // 0x201A8
    s32 mShowingSpecificWindowIdx; // 0x20310
    s32 mLastShowingSpecificWindowIdx; // 0x20314
    s16 mUnused20318; // 0x20318
    s16 mUnused2031A; // 0x2031A
    HWND mDlgHwnd; // 0x2031C
    HINSTANCE mhInstance; // 0x20320
};
GTA2_ASSERT_SIZEOF_ALWAYS(NetUi, 0x20324);

struct Network_WindowCreateParams
{
    s32 mX; // 0x0
    s32 mY; // 0x4
    u32 mW; // 0x8
    s32 mH; // 0xC
    HMENU mhMenu; // 0x10
    s32 mStyle; // 0x14
    s32 mExStyle; // 0x18
};

struct Network_UI_Control_Data
{
    Network_WindowCreateParams mWindowParams; // 0x0
    char mWindowClassName[80]; // 0x1C
    char mGtxStringName[8]; // 0x6C
};
