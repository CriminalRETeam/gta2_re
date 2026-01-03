#pragma once

#include "Function.hpp"
#include <windows.h>

EXPORT char_type* __stdcall GetString_519A00(const char_type* Key);
EXPORT u16 __stdcall sub_519960(char_type* a1, u16* a2);

enum Network_UI_Control_Ids
{
    COMBO_GAME_TYPE_1036 = 1036u,
    TEXTBOX_FRAG_LIMIT_NUMBER_1059 = 1059u,
    LABEL_FRAG_LIMIT_TEXT_1035 = 1035u,
    LABEL_GAME_SPEED_TEXT_1032 = 1032u,
    TEXTBOX_GAME_TIME_LIMIT_1038 = 1038u,
    CHECKBOX_POLICE_ENABLED_1027 = 1027u,
    TREEVIEW_UNKNOWN_1050 = 1050u,
    TREEVIEW_UNKNOWN_1024 = 1024u,
    LABEL_JOINED_GAME_TIME_LIMIT_1060 = 1060u,
    LABEL_JOINED_GAME_FRAG_LIMIT_1058 = 1058u,
    LABEL_JOINED_GAME_TYPE_1057 = 1057u,
    LABEL_JOINED_GAME_POLICE_ENABLED_1056 = 1056u,
    LISTBOX_UNKNOWN_1000 = 1000u,
    TEXTBOX_PLAYER_NAME_1004 = 1004u,
    LABLE_GAME_SPEED_1055 = 1055u,
    UNKNOWN_PLAYER_COUNT_1054 = 1054u,
};

struct Network_Enumerated_Map
{
    char field_0_map_name[260];
    char field_104_style_name[260];
    char field_208_script_name[260];
    u32 field_30C_player_count;
    char field_310_maybe_description[256];
    char field_410_maybe_display_name[260];
    u32 field_514;
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

class Network_20324
{
  public:
    //EXPORT u16 sub_519960(u8* a1, u16* a2);
    EXPORT static char_type sub_5199B0(wchar_t* a1, char_type* a2);
    //EXPORT char_type* GetString_519A00(const char_type* Key);
    EXPORT static void GetString_519A50(wchar_t* Dest, char_type* Source, size_t MaxCount);
    EXPORT Network_20324();
    EXPORT virtual ~Network_20324();
    EXPORT s32 sub_519BD0(Network_20324* dwInitParam, HINSTANCE hInstance);
    EXPORT INT_PTR DialogFunc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
    EXPORT s32 cb_sub_519D30(Network_20324* a1, s32 a2);
    EXPORT void SetDlgHwnd_519E10(HWND a2);
    EXPORT HWND Get_202E0_HWND_519E20();
    EXPORT LRESULT cb_sub_519E30(Network_20324* a1, wchar_t* Source, s32 a3);
    EXPORT void OnPaint_519FD0(HWND a1);
    EXPORT void OnWmCommand_519FE0(HWND hDlg, s32 a2, s32 a3, s32 a4);
    EXPORT LRESULT OnTimer_51A9D0(HWND hWnd, s32 a2);
    EXPORT char_type sub_51AA90(HWND hWndParent);
    EXPORT s32 sub_51ABF0(s32 a2);
    EXPORT static s32 OnInitDialog_51AC60(HWND hWnd, s32 a2, Network_20324* thisPtr);
    EXPORT s32 Get_202D4_active_control_idx_51ACC0();
    EXPORT LRESULT cb_sub_51ACD0(Network_20324* a1, wchar_t* Source);
    EXPORT s32 cb_sub_51ADE0(Network_20324* a1);
    EXPORT void sub_51AE20(s32 nIDDlgItem);
    EXPORT s32 cb_sub_51AE50(s32 a1, wchar_t* Source);
    EXPORT s32 sub_51AFA0();
    EXPORT LRESULT cb_sub_51B2F0(Network_20324* a1, wchar_t* Source);
    EXPORT s32 sub_51B4F0(s32 a2, const char_type* a3);
    EXPORT void SetPlayerNameText_51B7C0();
    EXPORT void cb_sub_51B7E0(Network_20324* a1, const char_type** a2);
    EXPORT void sub_51B810(const char_type* a2);
    EXPORT s32 sub_51B9C0(s32 a2, char_type* Data);
    EXPORT void sub_51BBC0();
    EXPORT u32 GetCount_51BBD0();
    EXPORT void sub_51BBE0();
    EXPORT void sub_51BBF0();
    EXPORT static void cb_sub_51BC00(Network_20324* pThis);
    EXPORT void sub_51BC70(s32 a2);
    EXPORT s32 sub_51BC80();
    EXPORT void sub_51BC90();
    EXPORT void sub_51BD40(const wchar_t* pPlayerNameW, const char* pPlayerNameA);
    EXPORT LRESULT sub_51BDD0(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
    EXPORT s32 sub_51BEB0(s32 nIDDlgItem, s32 a3);
    EXPORT void sub_51BFA0();
    EXPORT s32 OnWmHScroll_51C630(HWND hWnd, HWND a2, s32 a3, s32 a4);
    EXPORT void sub_51C7F0(s32* a2);
    EXPORT void sub_51C830();
    EXPORT char_type* GetMapName_51CA10();
    EXPORT char_type* GetMapStyName_51CA50();
    EXPORT char_type* GetMapScrName_51CA90();
    EXPORT void sub_51CAD0(const char_type* a1, const char_type* a2);
    EXPORT s32 sub_51CB30(s32 a1, HWND hDlg);
    EXPORT void sub_51CBC0();
    EXPORT void SetPoliceEnabledCheckBox_51CCB0(s32 bPoliceOn, HWND hDlg);
    EXPORT void SetJoinedGamePoliceEnabledText_51CD30(s32 bPoliceOn, HWND hDlg);
    EXPORT void SetFragsNumberAndLabel_51CDC0(s32 gameType, s32 fragLimit, HWND hDlg);
    EXPORT void SetGameSpeedTextLabelAndSlider_51CFC0(LPARAM game_speed, HWND hDlg);
    EXPORT void SetJoinedGameTypeAndFragLimitText_51D0C0(s32 a1, s32 a2, HWND hDlg);
    EXPORT void SetJoinedGameTimeLimitText_51D2F0(s32 a1, HWND hDlg);
    EXPORT void SetGameTimeLimitTextBox_51D3B0(s32 a1, HWND hDlg);

    //s32 field_0_vTable; // added by the compiler
    Network_Enumerated_Map field_4_maps[100];
    s32 field_1FD64_total_map_count;
    s32 field_1FD68;
    s32 field_1FD6C;
    WNDPROC field_1FD70_old_proc;
    WNDPROC field_1FD74_old_proc;
    wchar_t field_1FD78_player_name_2[260];
    char_type field_1FF80_player_name[260];
    s32 field_20084;
    NetworkGameSettings field_20088_game_settings;
    s32 field_201A8[30];
    s32 field_20220[38];
    s32 field_2027C;
    s32 field_20280;
    s32 field_20284;
    s32 field_20288;
    s32 field_2028C;
    s32 field_20290;
    s32 field_20294;
    s32 field_20298;
    s32 field_2029C;
    s32 field_202A0;
    s32 field_202A4;
    s32 field_202A8;
    s32 field_202AC;
    s32 field_202B0;
    s32 field_202B4;
    s32 field_202B8;
    s32 field_202BC;
    s32 field_202C0;
    s32 field_202C4;
    s32 field_202C8;
    s32 field_202CC;
    s32 field_202D0;
    s32 field_202D4;
    s32 field_202D8;
    s32 field_202DC;
    HWND field_202E0_hwnd;
    s32 field_202E4;
    s32 field_202E8;
    s32 field_202EC;
    s32 field_202F0;
    s32 field_202F4;
    s32 field_202F8;
    s32 field_202FC;
    s32 field_20300;
    s32 field_20304;
    s32 field_20308;
    s32 field_2030C;
    s32 field_20310;
    s32 field_20314;
    s16 field_20318;
    s16 field_2031A;
    HWND field_2031C_dlg_hwnd;
    HINSTANCE field_20320_hInstance;
};