#include "Network_20324.hpp"
#include "Frontend.hpp"
#include "Globals.hpp"
#include "NetPlay.hpp"
#include "registry.hpp"
#include "text_0x14.hpp"
#include <COMMCTRL.H>

DEFINE_GLOBAL(UINT_PTR, gTimerId_6F8A18, 0x6F8A18);
DEFINE_GLOBAL_ARRAY(char_type, Dest_6F88A4, 256, 0x6F88A4);

Network_UI_Control_Data gUiControlDefinitions_621430[3][30];

STUB_FUNC(0x519960)
u16 __stdcall sub_519960(char_type* a1, wchar_t* a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x5199b0)
char_type Network_20324::sub_5199B0(wchar_t* a1, char_type* a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x519a00)
char_type* __stdcall GetString_519A00(const char_type* pKey)
{
    wchar_t* pText = gtext_0x14_6F87F0->Find_5B5F90(pKey);

    if (gtext_0x14_6F87F0->field_10_lang_code == 'j')
    {
        sub_519960(Dest_6F88A4, pText);
    }
    else
    {
        wcstombs(Dest_6F88A4, pText, 256u);
    }
    return Dest_6F88A4;
}

MATCH_FUNC(0x519a50)
void Network_20324::GetString_519A50(wchar_t* Dest, char_type* Source, size_t MaxCount)
{
    if (gtext_0x14_6F87F0->field_10_lang_code == 'j')
    {
        sub_5199B0(Dest, Source);
    }
    else
    {
        mbstowcs(Dest, Source, MaxCount);
    }
}

// https://decomp.me/scratch/NJ1e0 compiler flag annoyance
STUB_FUNC(0x519a90)
Network_20324::Network_20324()
{
    NOT_IMPLEMENTED;

    this->field_202E4_hInstance = 0;
    this->field_202E0_dlg_hwnd = 0;
    this->field_202DC = 0;
    this->field_202D4_showing_specific_window_idx = 0;
    this->field_202D8_last_showing_specific_window_idx = 3;
    this->field_20084 = 0;
    this->field_1FD6C_count = 0;
    this->field_1FD64_total_map_count = 0;
    this->field_1FD68 = 0;
    this->field_20088_game_settings.field_2019C_tick_count = GetTickCount();
    this->field_20088_game_settings.field_20198_game_type = 1;
    this->field_20088_game_settings.field_20194_frag_limit = 1;
    this->field_20088_game_settings.field_2018C_map_idx = 0;
    this->field_20088_game_settings.field_201A0_police_on = 0;
    this->field_20088_game_settings.field_201A4_game_time_limit = 0;
    memset(field_4_maps, 0, sizeof(field_4_maps));
    gtext_0x14_6F87F0 = new text_0x14();
    gtext_0x14_6F87F0->Load_5B5E90();
    sub_51BC90();
    sub_51BFA0();
}

MATCH_FUNC(0x519ba0)
Network_20324::~Network_20324()
{
    if (gtext_0x14_6F87F0)
    {
        delete gtext_0x14_6F87F0;
        gtext_0x14_6F87F0 = 0;
    }
}

// TODO: Make a scratch
STUB_FUNC(0x519bd0)
s32 Network_20324::ShowNetworkUiBlocking_519BD0(Network_20324* dwInitParam, HINSTANCE hInstance)
{
    NOT_IMPLEMENTED;

    dwInitParam->field_202E4_hInstance = hInstance;
    gNetPlay_7071E8.Set15_51ECD0((int)Network_20324::cb_sub_519D30, dwInitParam);
    gNetPlay_7071E8.Set6_520530((int)Network_20324::cb_sub_519E30, (int)dwInitParam);
    gNetPlay_7071E8.Set9_520E60((int)Network_20324::cb_sub_51ACD0, (int)dwInitParam);
    gNetPlay_7071E8.Set3_Disconnect_520E80((int)Network_20324::cb_Disconnect_51ADE0, (int)dwInitParam);
    gNetPlay_7071E8.Set18_520F50((int)Network_20324::cb_sub_51AE50, (int)dwInitParam);
    gNetPlay_7071E8.Set21_5210D0((int)Network_20324::cb_sub_51B2F0, (int)dwInitParam);
    gNetPlay_7071E8.Set24_521140((int)Network_20324::cb_sub_51B7E0, (int)dwInitParam);
    gNetPlay_7071E8.Set27SavePlayerName_5211F0((int)Network_20324::cb_SavePlayerName_51BC00, (int)dwInitParam);
    return DialogBoxParamA(dwInitParam->field_202E4_hInstance,
                           (LPCSTR)103, // probably IDD_NETWORK_DIALOG
                           0,
                           Network_20324::DialogFunc,
                           (LPARAM)dwInitParam) == 1;
}

MATCH_FUNC(0x519c80)
int __stdcall Network_20324::DialogFunc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        case WM_TIMER:
            Network_20324::OnTimer_51A9D0(hDlg, wParam);
            break;

        case WM_HSCROLL:
        {
            HWND hwndCtl = (HWND)lParam;
            int scrollCode = LOWORD(wParam);
            int thumbPos = (short)HIWORD(wParam);

            Network_20324::OnWmHScroll_51C630(hDlg, hwndCtl, scrollCode, thumbPos);
            return 0;
        }
        break;

        case WM_COMMAND:
        {
            WORD notify = HIWORD(wParam);
            WORD id = LOWORD(wParam);

            Network_20324::OnWmCommand_519FE0(hDlg, id, (HWND)lParam, notify);
        }
        break;

        case WM_PAINT:
            Network_20324::OnPaint_519FD0(hDlg);
            break;
        case WM_INITDIALOG:
            return OnInitDialog_51AC60(hDlg, wParam, (Network_20324*)lParam);

        default:
            break;
    }

    return 0;
}

STUB_FUNC(0x519d30)
s32 Network_20324::cb_sub_519D30(Network_20324* a1, s32 a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x519e10)
void Network_20324::SetDlgHwnd_519E10(HWND a2)
{
    field_202E0_dlg_hwnd = a2;
}

MATCH_FUNC(0x519e20)
HWND Network_20324::Get_202E0_HWND_519E20()
{
    return field_202E0_dlg_hwnd;
}

STUB_FUNC(0x519e30)
LRESULT Network_20324::cb_sub_519E30(Network_20324* a1, wchar_t* Source, s32 a3)
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x519fd0)
void __stdcall Network_20324::OnPaint_519FD0(HWND a1)
{
    // Empty
}

STUB_FUNC(0x519fe0)
void __stdcall Network_20324::OnWmCommand_519FE0(HWND hDlg, s32 a2, HWND a3, s32 a4)
{
    NOT_IMPLEMENTED;
}

MATCH_FUNC(0x51a9d0)
void __stdcall Network_20324::OnTimer_51A9D0(HWND hWnd, s32 a2)
{
    Network_20324* pThis = (Network_20324*)GetWindowLongA(hWnd, 8);
    int hadSelection = 0;
    if (!pThis->sub_51BC80())
    {
        char textBuf[260];

        const LRESULT selIndex = SendDlgItemMessageA(hWnd, LISTBOX_UNKNOWN_1000, LB_GETCURSEL, 0, 0);

        if (selIndex != LB_ERR)
        {
            SendDlgItemMessageA(hWnd, 1000, LB_GETTEXT, selIndex, (LPARAM)textBuf);

            hadSelection = 1;
        }

        gNetPlay_7071E8.NetworkTick_51ED00();

        if (hadSelection)
        {
            SendDlgItemMessageA(hWnd, 1000, LB_SELECTSTRING, (WPARAM)-1, (LPARAM)textBuf);
        }
        else
        {
            SendDlgItemMessageA(hWnd, 1000, LB_SETCURSEL, 0, 0);
        }
    }
    else
    {
        EndDialog(hWnd, 1);
    }
}

// https://decomp.me/scratch/9kLQi TODO: Should match but end je target is diff? or its asm cmp bug
STUB_FUNC(0x51aa90)
void Network_20324::CreateMainUi_51AA90(HWND hWndParent)
{
    NOT_IMPLEMENTED;

    for (u32 i = 0; i < 3; i++)
    {
        for (u32 j = 0; j < 30; j++)
        {
            char* pNewWindowStr;
            if (gUiControlDefinitions_621430[i][j].field_6C_gtx_string_name[0])
            {
                pNewWindowStr = GetString_519A00(gUiControlDefinitions_621430[i][j].field_6C_gtx_string_name);
            }
            else
            {
                pNewWindowStr = ""; // byte_67DC88
            }

            field_201A8_ui_control_hwnds[i][j] = CreateWindowExA(gUiControlDefinitions_621430[i][j].field_0_windowParams.field_18_exStyle,
                                                                 gUiControlDefinitions_621430[i][j].field_1C_window_class_name,
                                                                 pNewWindowStr,
                                                                 gUiControlDefinitions_621430[i][j].field_0_windowParams.field_14_style,
                                                                 gUiControlDefinitions_621430[i][j].field_0_windowParams.field_0_x,
                                                                 gUiControlDefinitions_621430[i][j].field_0_windowParams.field_4_y,
                                                                 gUiControlDefinitions_621430[i][j].field_0_windowParams.field_8_w,
                                                                 gUiControlDefinitions_621430[i][j].field_0_windowParams.field_C_h,
                                                                 hWndParent,
                                                                 gUiControlDefinitions_621430[i][j].field_0_windowParams.field_10_hMenu,
                                                                 this->field_202E4_hInstance,
                                                                 0);
        }
    }

    SetWindowTextA(hWndParent, GetString_519A00("netui08"));

    // Subclass to handle enter key pressing
    this->field_1FD70_old_proc = (WNDPROC)GetWindowLongA(GetDlgItem(hWndParent, 1053), GWL_WNDPROC);
    this->field_1FD74_old_proc = (WNDPROC)GetWindowLongA(GetDlgItem(hWndParent, 1025), GWL_WNDPROC);
    SetWindowLongA(GetDlgItem(hWndParent, 1053), GWL_WNDPROC, (LONG)Network_20324::subclass_proc_51BDD0);
    SetWindowLongA(GetDlgItem(hWndParent, 1025), GWL_WNDPROC, (LONG)Network_20324::subclass_proc_51BDD0);

    SendDlgItemMessageA(hWndParent, 1031, LVM_SETEXTENDEDLISTVIEWSTYLE, LVS_EX_GRIDLINES, LVS_EX_FULLROWSELECT);
    SetDlgItemTextA(hWndParent, 1028, GetString_519A00("netui14"));
    SetDlgItemTextA(hWndParent, 1037, GetString_519A00("netui21"));
    Network_20324::sub_51CB30(0, hWndParent);
    if (gNetPlay_7071E8.field_4)
    {
        EnableWindow(GetDlgItem(hWndParent, 1001), 1);
    }
}

MATCH_FUNC(0x51abf0)
void Network_20324::ShowSpecificWindow_51ABF0(s32 showing_specific_window_idx)
{
    u32 i;

    // Hide old
    for (i = 0; i < 30; ++i)
    {
        ShowWindow(this->field_201A8_ui_control_hwnds[this->field_202D4_showing_specific_window_idx][i], SW_HIDE);
    }

    // Show new (using param, not the mem var)
    for (i = 0; i < 30; ++i)
    {
        ShowWindow(this->field_201A8_ui_control_hwnds[showing_specific_window_idx][i], SW_SHOW);
    }

    this->field_202D8_last_showing_specific_window_idx = this->field_202D4_showing_specific_window_idx;
    this->field_202D4_showing_specific_window_idx = showing_specific_window_idx;
}

MATCH_FUNC(0x51ac60)
s32 Network_20324::OnInitDialog_51AC60(HWND hWnd, s32 a2, Network_20324* thisPtr)
{
    SetWindowLongA(hWnd, 8, (LONG)thisPtr);
    gTimerId_6F8A18 = SetTimer(hWnd, 0xAu, 0xAu, 0);
    thisPtr->SetDlgHwnd_519E10(hWnd);
    thisPtr->CreateMainUi_51AA90(hWnd);
    thisPtr->PopulateMainUI_51AFA0();
    thisPtr->SetPlayerNameText_51B7C0();
    thisPtr->ShowSpecificWindow_51ABF0(0);
    return 1;
}

MATCH_FUNC(0x51acc0)
s32 Network_20324::Get_202D4_active_control_idx_51ACC0()
{
    return field_202D4_showing_specific_window_idx;
}

STUB_FUNC(0x51acd0)
LRESULT Network_20324::cb_sub_51ACD0(Network_20324* a1, wchar_t* Source)
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x51ade0)
void Network_20324::cb_Disconnect_51ADE0(Network_20324* pNetUi)
{
    gNetPlay_7071E8.Disconnect_520D10();
    pNetUi->ClearTreeView_51AE20(TREEVIEW_UNKNOWN_1050);
    pNetUi->ClearTreeView_51AE20(TREEVIEW_UNKNOWN_1024);
    pNetUi->ShowSpecificWindow_51ABF0(0);
}

MATCH_FUNC(0x51ae20)
void Network_20324::ClearTreeView_51AE20(s32 nIDDlgItem)
{
    HWND hwndDlg = this->field_202E0_dlg_hwnd;
    if (hwndDlg)
    {
        SendDlgItemMessageA(hwndDlg, nIDDlgItem, LVM_DELETEALLITEMS, 0, 0);
    }
}

STUB_FUNC(0x51ae50)
s32 Network_20324::cb_sub_51AE50(s32 a1, wchar_t* Source)
{
    NOT_IMPLEMENTED;
    return 0;
}

// This function matches. TODO: disable "/Oi-" compiler flag
STUB_FUNC(0x51afa0)
void Network_20324::PopulateMainUI_51AFA0()
{
    LPARAM columnInfo[8];
    memset(columnInfo, 0, sizeof(columnInfo));

    u16 colWidth = 3 * (gUiControlDefinitions_621430[1][0].field_0_windowParams.field_8_w >> 2);

    columnInfo[0] = 36;
    columnInfo[3] = (LPARAM) "Player Name";
    columnInfo[7] = 0;
    SendDlgItemMessageA(field_202E0_dlg_hwnd, 1024, 0x101B, 0, (LPARAM)columnInfo); // 0x101B = LVM_INSERTCOLUMNA
    SendDlgItemMessageA(field_202E0_dlg_hwnd, 1024, 0x101E, 0, colWidth); // 0x101E = LVM_SETCOLUMNWIDTH
    memset(columnInfo, 0, sizeof(columnInfo));
    columnInfo[0] = 37;
    columnInfo[1] = 0;
    columnInfo[3] = (LPARAM) "Ping";
    columnInfo[7] = 1;
    SendDlgItemMessageA(field_202E0_dlg_hwnd, 1024, 0x101B, 1u, (LPARAM)columnInfo);
    SendDlgItemMessageA(field_202E0_dlg_hwnd, 1024, 0x101E, 1u, 65534);
    memset(columnInfo, 0, sizeof(columnInfo));
    columnInfo[0] = 36;
    columnInfo[3] = (LPARAM) "Player Name";
    u16 v4 = 3 * (gUiControlDefinitions_621430[2][0].field_0_windowParams.field_8_w >> 2);
    columnInfo[7] = 0;
    SendDlgItemMessageA(field_202E0_dlg_hwnd, 1050, 0x101B, 0, (LPARAM)columnInfo);
    SendDlgItemMessageA(field_202E0_dlg_hwnd, 1050, 0x101Eu, 0, v4);
    memset(columnInfo, 0, sizeof(columnInfo));
    columnInfo[0] = 37;
    columnInfo[1] = 0;
    columnInfo[3] = (LPARAM) "Ping";
    columnInfo[7] = 1;
    SendDlgItemMessageA(field_202E0_dlg_hwnd, 1050, 0x101B, 1u, (LPARAM)columnInfo);
    SendDlgItemMessageA(field_202E0_dlg_hwnd, 1050, 0x101E, 1u, 65534);

    for (u32 v5 = 0; v5 < field_1FD64_total_map_count; v5++)
    {
        LRESULT v6 = SendDlgItemMessageA(field_202E0_dlg_hwnd, 1026, CB_ADDSTRING, 0, (LPARAM)&field_4_maps[v5].field_30C_player_count);
        SendDlgItemMessageA(field_202E0_dlg_hwnd, 1026, CB_SETITEMDATA, v6, v5);
    }

    LRESULT v8 = SendDlgItemMessageA(field_202E0_dlg_hwnd, 1036, 323u, 0, (LPARAM)GetString_519A00("netui19"));
    SendDlgItemMessageA(field_202E0_dlg_hwnd, 1036, CB_SETITEMDATA, v8, 0);
    LRESULT v10 = SendDlgItemMessageA(field_202E0_dlg_hwnd, 1036, CB_ADDSTRING, 0, (LPARAM)GetString_519A00("netui20"));
    SendDlgItemMessageA(field_202E0_dlg_hwnd, 1036, CB_SETITEMDATA, v10, 1);
    LRESULT v12 = SendDlgItemMessageA(field_202E0_dlg_hwnd, 1036, 0x143u, 0, (LPARAM)GetString_519A00("netui22"));
    SendDlgItemMessageA(field_202E0_dlg_hwnd, 1036, CB_SETITEMDATA, v12, 2);
    Network_20324::sub_51C830();
    SendDlgItemMessageA(field_202E0_dlg_hwnd, 1026, CB_SETCURSEL, field_20088_game_settings.field_2018C_map_idx, 0);
    SendDlgItemMessageA(field_202E0_dlg_hwnd, COMBO_GAME_TYPE_1036, CB_SETCURSEL, field_20088_game_settings.field_20198_game_type - 1, 0);
    Network_20324::sub_51CB30(field_4_maps[field_20088_game_settings.field_2018C_map_idx].field_514, field_202E0_dlg_hwnd);
    Network_20324::SetPoliceEnabledCheckBox_51CCB0(field_20088_game_settings.field_201A0_police_on, field_202E0_dlg_hwnd);
    Network_20324::SetFragsNumberAndLabel_51CDC0(field_20088_game_settings.field_20198_game_type,
                                                 field_20088_game_settings.field_20194_frag_limit,
                                                 field_202E0_dlg_hwnd);
    Network_20324::SetGameSpeedTextLabelAndSlider_51CFC0(field_20088_game_settings.field_20190_game_speed, field_202E0_dlg_hwnd);
    Network_20324::SetGameTimeLimitTextBox_51D3B0(field_20088_game_settings.field_201A4_game_time_limit, field_202E0_dlg_hwnd);
}

STUB_FUNC(0x51b2f0)
LRESULT Network_20324::cb_sub_51B2F0(Network_20324* a1, wchar_t* Source)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x51b4f0)
s32 Network_20324::AppendChatMessage_51B4F0(s32 a2, const char_type* a3)
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x51b7c0)
void Network_20324::SetPlayerNameText_51B7C0()
{
    SetDlgItemTextA(field_202E0_dlg_hwnd,
                    1004, // TODO: control constant
                    field_1FF80_player_name);
}

MATCH_FUNC(0x51b7e0)
void Network_20324::cb_sub_51B7E0(Network_20324* a1, const char_type** a2)
{
    const char_type* v2 = *a2;
    if (a1->Get_202D4_active_control_idx_51ACC0() == 2)
    {
        a1->sub_51B810(v2);
    }
}

STUB_FUNC(0x51b810)
void Network_20324::sub_51B810(const char_type* a2)
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x51b9c0)
s32 Network_20324::sub_51B9C0(s32 a2, char_type* Data)
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x51bbc0)
void Network_20324::sub_51BBC0()
{
    field_1FD6C_count++;
}

MATCH_FUNC(0x51bbd0)
u32 Network_20324::GetCount_51BBD0()
{
    return field_1FD6C_count;
}

MATCH_FUNC(0x51bbe0)
void Network_20324::sub_51BBE0()
{
    field_1FD6C_count--;
}

MATCH_FUNC(0x51bbf0)
void Network_20324::sub_51BBF0()
{
    field_1FD6C_count = 0;
}

MATCH_FUNC(0x51bc00)
void Network_20324::cb_SavePlayerName_51BC00(Network_20324* pThis)
{
    char String[260];
    wchar_t Dest[260];

    GetDlgItemTextA(pThis->Get_202E0_HWND_519E20(), 1004, String, GTA2_COUNTOF(String)); // TODO: control constants
    GetString_519A50(Dest, String, GTA2_COUNTOF(String));
    pThis->sub_51BD40(Dest, String);
    pThis->sub_51BC70(1);
}

MATCH_FUNC(0x51bc70)
void Network_20324::sub_51BC70(s32 a2)
{
    field_1FD68 = a2;
}

MATCH_FUNC(0x51bc80)
s32 Network_20324::sub_51BC80()
{
    return field_1FD68;
}

MATCH_FUNC(0x51bc90)
void Network_20324::sub_51BC90()
{
    DWORD Type = REG_SZ;
    BYTE Data[260];
    DWORD cbData = GTA2_COUNTOF(Data);

    HKEY phkResult;

    DWORD dwDisposition;

    if (RegCreateKeyExA(HKEY_LOCAL_MACHINE,
                        "Software\\DMA Design Ltd\\GTA2\\Network",
                        0,
                        0,
                        0,
                        KEY_ALL_ACCESS,
                        0,
                        &phkResult,
                        &dwDisposition) == ERROR_SUCCESS)
    {

        if (RegQueryValueExA(phkResult, "PlayerName", 0, &Type, Data, &cbData) == ERROR_SUCCESS)
        {
            strcpy(field_1FF80_player_name, reinterpret_cast<char*>(Data));
            GetString_519A50(field_1FD78_player_name_2, reinterpret_cast<char*>(Data), GTA2_COUNTOF(field_1FD78_player_name_2));
        }
        RegCloseKey(phkResult);
    }
}

STUB_FUNC(0x51bd40)
void Network_20324::sub_51BD40(const wchar_t* pPlayerNameW, const char* pPlayerNameA)
{
    NOT_IMPLEMENTED;
    strcpy(field_1FF80_player_name, pPlayerNameA);
    wcscpy(field_1FD78_player_name_2, pPlayerNameW);

    HKEY hKey;
    DWORD dwDisposition;
    if (RegCreateKeyExA(HKEY_LOCAL_MACHINE, "Software\\DMA Design Ltd\\GTA2\\Network", 0, 0, 0, KEY_ALL_ACCESS, 0, &hKey, &dwDisposition) ==
        ERROR_SUCCESS)
    {
        RegSetValueExA(hKey,
                       "PlayerName",
                       0,
                       1u, // TODO: constant
                       reinterpret_cast<const u8*>(pPlayerNameA),
                       strlen(pPlayerNameA) + 1); // +1 so the NULL is also written
        RegCloseKey(hKey);
    }
}

MATCH_FUNC(0x51bdd0)
LRESULT __stdcall Network_20324::subclass_proc_51BDD0(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
    HWND parentHwnd = GetParent(hWnd);
    if (parentHwnd)
    {
        Network_20324* pThis = (Network_20324*)GetWindowLongA(parentHwnd, 8);
        if (Msg == WM_KEYUP && wParam == VK_RETURN && (lParam & (KF_ALTDOWN << 16)) == 0)
        {
            switch (pThis->Get_202D4_active_control_idx_51ACC0())
            {
                case 2:
                    pThis->OnEnterPressed_51BEB0(1053, 1051);
                    break;

                case 1:
                    pThis->OnEnterPressed_51BEB0(1025, 1022);
                    break;
            }
        }

        switch (pThis->Get_202D4_active_control_idx_51ACC0())
        {
            case 2:
                return CallWindowProcA(pThis->field_1FD70_old_proc, hWnd, Msg, wParam, lParam);
            case 1:
                return CallWindowProcA(pThis->field_1FD74_old_proc, hWnd, Msg, wParam, lParam);
        }
    }
    return 0;
}

MATCH_FUNC(0x51beb0)
void Network_20324::OnEnterPressed_51BEB0(s32 nIDDlgItem, s32 a3)
{
    char_type String[128];
    wchar_t Dest[260];
    GetDlgItemTextA(field_202E0_dlg_hwnd, nIDDlgItem, String, 127);
    Network_20324::GetString_519A50(Dest, String, 260);
    gNetPlay_7071E8.SendChatMessage_521060(Dest, -1);
    SetDlgItemTextA(field_202E0_dlg_hwnd, nIDDlgItem, "");
    Network_20324::AppendChatMessage_51B4F0(a3, String);
}

STUB_FUNC(0x51bfa0)
void Network_20324::sub_51BFA0()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x51c630)
s32 __stdcall Network_20324::OnWmHScroll_51C630(HWND hWnd, HWND a2, s32 a3, s32 a4)
{
    NOT_IMPLEMENTED;
    return 0;
}

// This function matches. TODO: disable "/Oi-" compiler flag
STUB_FUNC(0x51c7f0)
void Network_20324::CopyGameSettings_51C7F0(NetworkGameSettings* pSettings)
{
    if (pSettings)
    {
        memcpy(pSettings, &field_20088_game_settings, sizeof(NetworkGameSettings));
    }
}

// This function matches. TODO: disable "/Oi-" compiler flag
STUB_FUNC(0x51c830)
void Network_20324::sub_51C830()
{
    field_20088_game_settings.field_2018C_map_idx = gRegistry_6FF968.Set_Network_Setting_587690("map_index", 0);
    if (field_20088_game_settings.field_2018C_map_idx >= field_1FD64_total_map_count)
    {
        field_20088_game_settings.field_2018C_map_idx = 0;
    }
    field_20088_game_settings.field_201A4_game_time_limit = gRegistry_6FF968.Set_Network_Setting_587690("game_time_limit", 1);
    field_20088_game_settings.field_20190_game_speed = gRegistry_6FF968.Set_Network_Setting_587690("game_speed", 1);
    field_20088_game_settings.field_20198_game_type = gRegistry_6FF968.Set_Network_Setting_587690("game_type", 1);
    field_20088_game_settings.field_201A0_police_on = gRegistry_6FF968.Set_Network_Setting_587690("police", 0);

    if (field_20088_game_settings.field_20198_game_type == 1)
    {
        field_20088_game_settings.field_20194_frag_limit = gRegistry_6FF968.Set_Network_Setting_587690("f_limit", 3);
    }
    else
    {
        field_20088_game_settings.field_20194_frag_limit = gRegistry_6FF968.Set_Network_Setting_587690("s_limit", 10000);
    }

    strcpy(field_20088_game_settings.field_20088_default_map,
           field_4_maps[field_20088_game_settings.field_2018C_map_idx].field_410_maybe_display_name);
}

MATCH_FUNC(0x51ca10)
char_type* Network_20324::GetMapName_51CA10()
{
    return (char_type*)&field_4_maps[field_20088_game_settings.field_2018C_map_idx].field_0_map_name;
}

MATCH_FUNC(0x51ca50)
char_type* Network_20324::GetMapStyName_51CA50()
{
    return (char_type*)&field_4_maps[field_20088_game_settings.field_2018C_map_idx].field_104_style_name;
}

MATCH_FUNC(0x51ca90)
char_type* Network_20324::GetMapScrName_51CA90()
{
    return (char_type*)&field_4_maps[field_20088_game_settings.field_2018C_map_idx].field_208_script_name;
}

MATCH_FUNC(0x51cad0)
void Network_20324::sub_51CAD0(const char_type* a1, const char_type* a2)
{
    char Text[256];
    sprintf(Text, "Unable to open file: %s in mmpfile: %s", a2, a1);
    MessageBoxA(0, Text, "", MB_ICONEXCLAMATION); // TODO: blank str is byte_67DC88 ?
}

MATCH_FUNC(0x51cb30)
void Network_20324::sub_51CB30(s32 a1, HWND hDlg)
{
    char String[260];
    const char* pStr = GetString_519A00("netui13");
    sprintf(String, "%s %d", pStr, a1);
    SetDlgItemTextA(hDlg, 1033, String);
}

MATCH_FUNC(0x51cbc0)
void Network_20324::sub_51CBC0()
{
    if (Network_20324::GetCount_51BBD0() == field_4_maps[field_20088_game_settings.field_2018C_map_idx].field_514)
    {
        EnableWindow(GetDlgItem(Network_20324::Get_202E0_HWND_519E20(), 1021), true);
    }
    else
    {
        EnableWindow(GetDlgItem(Network_20324::Get_202E0_HWND_519E20(), 1021), false);
    }

    if (Network_20324::GetCount_51BBD0() > 1)
    {
        EnableWindow(GetDlgItem(Network_20324::Get_202E0_HWND_519E20(), 1020), true);
    }
    else
    {
        EnableWindow(GetDlgItem(Network_20324::Get_202E0_HWND_519E20(), 1020), false);
    }
}

MATCH_FUNC(0x51ccb0)
void Network_20324::SetPoliceEnabledCheckBox_51CCB0(s32 bPoliceOn, HWND hDlg)
{
    if (bPoliceOn)
    {
        SendDlgItemMessageA(hDlg, CHECKBOX_POLICE_ENABLED_1027, BM_SETCHECK, 1, 0);
    }
    else
    {
        SendDlgItemMessageA(hDlg, CHECKBOX_POLICE_ENABLED_1027, BM_SETCHECK, 0, 0);
    }
}

MATCH_FUNC(0x51cd30)
void Network_20324::SetJoinedGamePoliceEnabledText_51CD30(s32 bPoliceOn, HWND hDlg)
{
    if (bPoliceOn == 1)
    {
        SetDlgItemTextA(hDlg, LABEL_JOINED_GAME_POLICE_ENABLED_1056, GetString_519A00("netui15"));
    }
    else
    {
        SetDlgItemTextA(hDlg, LABEL_JOINED_GAME_POLICE_ENABLED_1056, GetString_519A00("netui16"));
    }
}

// This function matches. TODO: disable "/Oi-" compiler flag
STUB_FUNC(0x51cdc0)
void Network_20324::SetFragsNumberAndLabel_51CDC0(s32 gameType, s32 fragLimit, HWND hDlg)
{
    char_type* fragLimitString;
    char_type tmpBuffer[260];

    bool bHasFragLimit = false;

    if (gameType == FRAG_GAME_1)
    {
        fragLimitString = GetString_519A00("netui17");
        bHasFragLimit = true;
    }
    else if (gameType == POINTS_GAME_2)
    {
        fragLimitString = GetString_519A00("netui18");
        bHasFragLimit = true;
    }

    if (bHasFragLimit)
    {
        strcpy(tmpBuffer, fragLimitString);
        SetDlgItemTextA(hDlg, LABEL_FRAG_LIMIT_TEXT_1035, tmpBuffer);
        sprintf(tmpBuffer, "%d", fragLimit);
        SetDlgItemTextA(hDlg, TEXTBOX_FRAG_LIMIT_NUMBER_1059, tmpBuffer);
        if (Network_20324::Get_202D4_active_control_idx_51ACC0() == 1)
        {
            ShowWindow(GetDlgItem(hDlg, TEXTBOX_FRAG_LIMIT_NUMBER_1059), SW_SHOW);
            ShowWindow(GetDlgItem(hDlg, LABEL_FRAG_LIMIT_TEXT_1035), SW_SHOW);
        }
    }
    else
    {
        if (Network_20324::Get_202D4_active_control_idx_51ACC0() == 1) // hide if its not a frag game?
        {
            ShowWindow(GetDlgItem(hDlg, TEXTBOX_FRAG_LIMIT_NUMBER_1059), SW_HIDE);
            ShowWindow(GetDlgItem(hDlg, LABEL_FRAG_LIMIT_TEXT_1035), SW_HIDE);
        }
    }
}

// https://decomp.me/scratch/gzeUC
STUB_FUNC(0x51cfc0)
void Network_20324::SetGameSpeedTextLabelAndSlider_51CFC0(LPARAM game_speed, HWND hDlg)
{
    SendDlgItemMessageA(hDlg, 1031, 0x405, 1u, game_speed); // 0x405 = TBM_SETPOS

    switch (game_speed)
    {
        case 2:
            SetDlgItemTextA(hDlg, LABEL_GAME_SPEED_TEXT_1032, GetString_519A00("netui12"));
            break;
        case 1:
            SetDlgItemTextA(hDlg, LABEL_GAME_SPEED_TEXT_1032, GetString_519A00("netui11"));
            break;
        case 0:
            SetDlgItemTextA(hDlg, LABEL_GAME_SPEED_TEXT_1032, GetString_519A00("netui10"));
            break;
        default:
            SetDlgItemTextA(hDlg, LABEL_GAME_SPEED_TEXT_1032, (LPCSTR)game_speed);
            break;
    }
}

// This function matches. TODO: disable "/Oi-" compiler flag
STUB_FUNC(0x51d0c0)
void Network_20324::SetJoinedGameTypeAndFragLimitText_51D0C0(s32 game_type, s32 frag_limit, HWND hDlg)
{
    char_type String[260];
    char_type Buffer[260];

    bool bIsFragOrPoints = false;

    if (game_type == FRAG_GAME_1)
    {
        strcpy(String, GetString_519A00("netui19"));
        strcpy(Buffer, GetString_519A00("netui17"));
        bIsFragOrPoints = true;
    }
    else if (game_type == POINTS_GAME_2)
    {
        strcpy(String, GetString_519A00("netui20"));
        strcpy(Buffer, GetString_519A00("netui18"));
        bIsFragOrPoints = true;
    }

    if (bIsFragOrPoints)
    {
        SetDlgItemTextA(hDlg, LABEL_JOINED_GAME_TYPE_1057, String);
        sprintf(Buffer, "%d %s", frag_limit, String);
        SetDlgItemTextA(hDlg, LABEL_JOINED_GAME_FRAG_LIMIT_1058, Buffer);
    }
    else
    {
        strcpy(String, GetString_519A00("netui22"));
        SetDlgItemTextA(hDlg, LABEL_JOINED_GAME_TYPE_1057, String);
        SetDlgItemTextA(hDlg, LABEL_JOINED_GAME_FRAG_LIMIT_1058, "");
    }
}

// This function matches. TODO: disable "/Oi-" compiler flag
STUB_FUNC(0x51d2f0)
void Network_20324::SetJoinedGameTimeLimitText_51D2F0(s32 gameTimeLimit, HWND hDlg)
{
    char_type Buffer[260];
    char_type String[260];
    strcpy(Buffer, GetString_519A00("netui21"));
    sprintf(String, "%s: %d", Buffer, gameTimeLimit);
    SetDlgItemTextA(hDlg, LABEL_JOINED_GAME_TIME_LIMIT_1060, String);
}

MATCH_FUNC(0x51d3b0)
void Network_20324::SetGameTimeLimitTextBox_51D3B0(s32 timeLimitMins, HWND hDlg)
{
    char_type String[260];
    sprintf(String, "%d", timeLimitMins);
    SetDlgItemTextA(hDlg, TEXTBOX_GAME_TIME_LIMIT_1038, String);
}