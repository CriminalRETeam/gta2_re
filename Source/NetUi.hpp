#pragma once

#include "Function.hpp"
#include <windows.h>

struct Network_NameList;
struct Network_PlayerPing;
struct Network_ChatMessage;

EXPORT char_type* __stdcall GetString_519A00(const char_type* Key);
EXPORT void __stdcall WideToDbcs_519960(char_type* pDst, wchar_t* pSrc);

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
    char field_0_map_name[260];
    char field_104_style_name[260];
    char field_208_script_name[260];
    char field_30C_description[260];
    char field_410_mmp_name[260];
    u32 field_514_player_count;
};

struct NetworkGameSettings
{
    char field_20088_default_map[260];
    u32 field_2018C_map_idx;
    u32 field_20190_game_speed;
    u32 field_20194_frag_limit;
    u32 field_20198_game_type;
    u32 field_2019C_tick_count;
    u32 field_201A0_police_on;
    u32 field_201A4_game_time_limit;
};

class NetUi
{
  public:
    EXPORT static void DbcsToWide_5199B0(wchar_t* pDst, char_type* pSrc);
    EXPORT static void GetString_519A50(wchar_t* Dest, char_type* Source, size_t MaxCount);
    EXPORT NetUi();
    EXPORT virtual ~NetUi();
    EXPORT s32 ShowNetworkUiBlocking_519BD0(HINSTANCE hInstance);
    EXPORT static int __stdcall DialogFunc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
    EXPORT static s32 __stdcall cb_FillSessionList_519D30(NetUi* pThis, Network_NameList* pSessions);
    EXPORT void SetDlgHwnd_519E10(HWND hDlg);
    EXPORT HWND Get_2031C_HWND_519E20();
    EXPORT static void __stdcall cb_AddPlayerToList_519E30(NetUi* pNetUi, wchar_t* pName, s32 player_idx);
    EXPORT static void __stdcall OnPaint_519FD0(HWND hWnd);
    EXPORT static void __stdcall OnWmCommand_519FE0(HWND hDlg, s32 id, HWND hCtl, s32 notify);
    EXPORT static void __stdcall OnTimer_51A9D0(HWND hWnd, s32 timer_id);
    EXPORT void CreateMainUi_51AA90(HWND hWndParent);
    EXPORT void ShowSpecificWindow_51ABF0(s32 showing_specific_window_idx);
    EXPORT static s32 OnInitDialog_51AC60(HWND hWnd, s32 lParam, NetUi* thisPtr);
    EXPORT s32 Get_20310_showing_window_idx_51ACC0();
    EXPORT static void __stdcall cb_RemovePlayerFromList_51ACD0(NetUi* pNetUi, wchar_t* Source);
    EXPORT static void _stdcall cb_Disconnect_51ADE0(NetUi* pNetUi);
    EXPORT void ClearListView_51AE20(s32 nIDDlgItem);
    EXPORT static s32 __stdcall cb_SetPlayerPing_51AE50(NetUi* pNetUi, Network_PlayerPing* pPing);
    EXPORT void PopulateMainUI_51AFA0();
    EXPORT static LRESULT __stdcall cb_AddChatMessage_51B2F0(NetUi* pNetUi, Network_ChatMessage* pMsg);
    EXPORT s32 AppendChatMessage_51B4F0(s32 hmenu, const char_type* pMsg);
    EXPORT void SetPlayerNameText_51B7C0();
    EXPORT static void __stdcall cb_GameSettingsChanged_51B7E0(NetUi* pNetUi, const char_type** ppData);
    EXPORT void ApplyHostGameSettings_51B810(const char_type* pData);
    EXPORT void SetSetting_51B9C0(s32 setting, s32 value);
    EXPORT void IncCount_51BBC0();
    EXPORT u32 GetCount_51BBD0();
    EXPORT void DecCount_51BBE0();
    EXPORT void ResetCount_51BBF0();
    EXPORT static void __stdcall cb_SavePlayerName_51BC00(NetUi* pThis);
    EXPORT void SetEndDialog_51BC70(s32 bEnd);
    EXPORT s32 GetEndDialog_51BC80();
    EXPORT void LoadPlayerNameFromRegistry_51BC90();
    EXPORT void SetPlayNameAndSaveToRegistry_51BD40(const wchar_t* pPlayerNameW, const char* pPlayerNameA);
    EXPORT static LRESULT __stdcall subclass_proc_51BDD0(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
    EXPORT void OnEnterPressed_51BEB0(s32 nIDDlgItem, s32 hmenu);
    EXPORT void EnumerateMaps_51BFA0();
    EXPORT static void __stdcall OnWmHScroll_51C630(HWND hWnd, HWND hTrackBar, s32 code, s32 pos);
    EXPORT void CopyGameSettings_51C7F0(NetworkGameSettings* pSettings);
    EXPORT void LoadGameSettingsFromRegistry_51C830();
    EXPORT char_type* GetMapName_51CA10();
    EXPORT char_type* GetMapStyName_51CA50();
    EXPORT char_type* GetMapScrName_51CA90();
    EXPORT static void __stdcall ShowUnableToOpenFileError_51CAD0(const char_type* pMmpName, const char_type* pFileName);
    EXPORT void SetPlayerCountText_51CB30(s32 count, HWND hDlg);
    EXPORT void UpdateButtonsEnabledState_51CBC0();
    EXPORT void SetPoliceEnabledCheckBox_51CCB0(s32 bPoliceOn, HWND hDlg);
    EXPORT void SetJoinedGamePoliceEnabledText_51CD30(s32 bPoliceOn, HWND hDlg);
    EXPORT void SetFragsNumberAndLabel_51CDC0(s32 gameType, s32 fragLimit, HWND hDlg);
    EXPORT void SetGameSpeedTextLabelAndSlider_51CFC0(LPARAM game_speed, HWND hDlg);
    EXPORT void SetJoinedGameTypeAndFragLimitText_51D0C0(s32 game_type, s32 frag_limit, HWND hDlg);
    EXPORT void SetJoinedGameTimeLimitText_51D2F0(s32 gameTimeLimit, HWND hDlg);
    EXPORT void SetGameTimeLimitTextBox_51D3B0(s32 timeLimitMins, HWND hDlg);

    //s32 field_0_vTable; // added by the compiler
    Network_Enumerated_Map field_4_maps[100];
    s32 field_1FD64_total_map_count;
    s32 field_1FD68_bEndDialog;
    s32 field_1FD6C_count;
    WNDPROC field_1FD70_old_proc;
    WNDPROC field_1FD74_old_proc;
    wchar_t field_1FD78_player_name_2[260];
    char_type field_1FF80_player_name[260];
    s32 field_20084_unused;
    NetworkGameSettings field_20088_game_settings;
    HWND field_201A8_ui_control_hwnds[3][30];
    s32 field_20310_showing_specific_window_idx;
    s32 field_20314_last_showing_specific_window_idx;
    s16 field_20318_unused;
    s16 field_2031A_unused;
    HWND field_2031C_dlg_hwnd;
    HINSTANCE field_20320_hInstance;
};
GTA2_ASSERT_SIZEOF_ALWAYS(NetUi, 0x20324);

struct Network_WindowCreateParams
{
    int field_0_x;
    int field_4_y;
    unsigned int field_8_w;
    int field_C_h;
    HMENU field_10_hMenu;
    int field_14_style;
    int field_18_exStyle;
};

struct Network_UI_Control_Data
{
    Network_WindowCreateParams field_0_windowParams;
    char field_1C_window_class_name[80];
    char field_6C_gtx_string_name[8];
};
