#include "Network_20324.hpp"
#include "text_0x14.hpp"

STUB_FUNC(0x519960)
u16 Network_20324::sub_519960(u8 *a1, u16 *a2)
{
    return 0;
}

STUB_FUNC(0x5199b0)
char_type Network_20324::sub_5199B0(u16 *a1, char_type *a2)
{
    return 0;
}

STUB_FUNC(0x519a00)
char_type *Network_20324::GetString_519A00(const char_type *Key)
{
    return 0;
}

MATCH_FUNC(0x519a50)
void Network_20324::GetString_519A50(wchar_t *Dest, char_type *Source, size_t MaxCount)
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

STUB_FUNC(0x519a90)
Network_20324 *Network_20324::ctor_519A90()
{
    return 0;
}

STUB_FUNC(0x519b80)
Network_20324 *Network_20324::vdtor_519B80(Network_20324 *lpMem, char_type flags)
{
    return 0;
}

STUB_FUNC(0x519ba0)
void Network_20324::dtor_519BA0()
{
}

STUB_FUNC(0x519bd0)
s32 Network_20324::sub_519BD0(Network_20324 *dwInitParam, HINSTANCE hInstance)
{
    return 0;
}

STUB_FUNC(0x519c80)
INT_PTR Network_20324::DialogFunc(HWND hDlg, UINT message, WPARAM wParam, Network_20324 *lParam)
{
    return 0;
}

STUB_FUNC(0x519d30)
s32 Network_20324::cb_sub_519D30(Network_20324 *a1, s32 a2)
{
    return 0;
}

MATCH_FUNC(0x519e10)
void Network_20324::SetDlgHwnd_519E10(HWND a2)
{
    field_2031C_dlg_hwnd = a2;
}

MATCH_FUNC(0x519e20)
HWND Network_20324::sub_519E20()
{
    return field_2031C_dlg_hwnd;
}

STUB_FUNC(0x519e30)
LRESULT Network_20324::cb_sub_519E30(Network_20324 *a1, wchar_t *Source, s32 a3)
{
    return 0;
}

STUB_FUNC(0x519fd0)
void Network_20324::OnPaint_519FD0(s32 a1)
{
}

STUB_FUNC(0x519fe0)
void Network_20324::OnWmCommand_519FE0(HWND hDlg, s32 a2, s32 a3, s32 a4)
{
}

STUB_FUNC(0x51a9d0)
LRESULT Network_20324::OnTimer_51A9D0(HWND hWnd, s32 a2)
{
    return 0;
}

STUB_FUNC(0x51aa90)
char_type Network_20324::sub_51AA90(HWND hWndParent)
{
    return 0;
}

STUB_FUNC(0x51abf0)
s32 Network_20324::sub_51ABF0(s32 a2)
{
    return 0;
}

STUB_FUNC(0x51ac60)
s32 Network_20324::OnInitDialog_51AC60(HWND hWnd, s32 a2, Network_20324 *thisPtr)
{
    return 0;
}

STUB_FUNC(0x51acc0)
s32 Network_20324::sub_51ACC0()
{
    return 0;
}

STUB_FUNC(0x51acd0)
LRESULT Network_20324::cb_sub_51ACD0(Network_20324 *a1, wchar_t *Source)
{
    return 0;
}

STUB_FUNC(0x51ade0)
s32 Network_20324::cb_sub_51ADE0(Network_20324 *a1)
{
    return 0;
}

STUB_FUNC(0x51ae20)
void Network_20324::sub_51AE20(s32 nIDDlgItem)
{
}

STUB_FUNC(0x51ae50)
s32 Network_20324::cb_sub_51AE50(s32 a1, wchar_t *Source)
{
    return 0;
}

STUB_FUNC(0x51afa0)
s32 Network_20324::sub_51AFA0()
{
    return 0;
}

STUB_FUNC(0x51b2f0)
LRESULT Network_20324::cb_sub_51B2F0(Network_20324 *a1, wchar_t *Source)
{
    return 0;
}

STUB_FUNC(0x51b4f0)
s32 Network_20324::sub_51B4F0(s32 a2, const char_type *a3)
{
    return 0;
}

STUB_FUNC(0x51b7c0)
s32 Network_20324::SetPlayerNameText_51B7C0()
{
    return 0;
}

STUB_FUNC(0x51b7e0)
void Network_20324::cb_sub_51B7E0(s32 a1, const char_type **a2)
{
}

STUB_FUNC(0x51b810)
void Network_20324::sub_51B810(const char_type *a2)
{
}

STUB_FUNC(0x51b9c0)
s32 Network_20324::sub_51B9C0(s32 a2, char_type *Data)
{
    return 0;
}

STUB_FUNC(0x51bbc0)
void Network_20324::sub_51BBC0()
{
}

MATCH_FUNC(0x51bbd0)
s32 Network_20324::sub_51BBD0()
{
    return field_1FD6C;
}

MATCH_FUNC(0x51bbe0)
void Network_20324::sub_51BBE0()
{
    field_1FD6C--;
}

MATCH_FUNC(0x51bbf0)
void Network_20324::sub_51BBF0()
{
    field_1FD6C = 0;
}

MATCH_FUNC(0x51bc00)
void Network_20324::cb_sub_51BC00(Network_20324 *pThis)
{
    char String[260];
    wchar_t Dest[260];

    GetDlgItemTextA(pThis->sub_519E20(), 1004, String, GTA2_COUNTOF(String)); // TODO: control constants
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

    if (RegCreateKeyExA(
            HKEY_LOCAL_MACHINE,
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
            strcpy(field_1FF80_player_name, reinterpret_cast<char *>(Data));
            GetString_519A50(field_1FD78_player_name_2, reinterpret_cast<char *>(Data), GTA2_COUNTOF(field_1FD78_player_name_2));
        }
        RegCloseKey(phkResult);
    }
}

STUB_FUNC(0x51bd40)
void Network_20324::sub_51BD40(const wchar_t *pPlayerNameW, const char *pPlayerNameA)
{
    strcpy(field_1FF80_player_name, pPlayerNameA);
    wcscpy(field_1FD78_player_name_2, pPlayerNameW);

    HKEY hKey;
    DWORD dwDisposition;
    if (RegCreateKeyExA(
            HKEY_LOCAL_MACHINE,
            "Software\\DMA Design Ltd\\GTA2\\Network",
            0,
            0,
            0,
            KEY_ALL_ACCESS,
            0,
            &hKey,
            &dwDisposition) == ERROR_SUCCESS)
    {
        RegSetValueExA(
            hKey,
            "PlayerName",
            0,
            1u, // TODO: constant
            reinterpret_cast<const u8 *>(pPlayerNameA),
            strlen(pPlayerNameA) + 1); // +1 so the NULL is also written
        RegCloseKey(hKey);
    }
}

STUB_FUNC(0x51bdd0)
LRESULT Network_20324::sub_51BDD0(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
    return 0;
}

STUB_FUNC(0x51beb0)
s32 Network_20324::sub_51BEB0(s32 nIDDlgItem, s32 a3)
{
    return 0;
}

STUB_FUNC(0x51bfa0)
void Network_20324::sub_51BFA0()
{
}

STUB_FUNC(0x51c630)
s32 Network_20324::OnWmHScroll_51C630(HWND hWnd, HWND a2, s32 a3, s32 a4)
{
    return 0;
}

STUB_FUNC(0x51c7f0)
void Network_20324::sub_51C7F0(s32 *a2)
{
}

STUB_FUNC(0x51c830)
u32 Network_20324::sub_51C830()
{
    return 0;
}

STUB_FUNC(0x51ca10)
char_type *Network_20324::sub_51CA10()
{
    return 0;
}

STUB_FUNC(0x51ca50)
char_type *Network_20324::sub_51CA50()
{
    return 0;
}

STUB_FUNC(0x51ca90)
char_type *Network_20324::sub_51CA90()
{
    return 0;
}

STUB_FUNC(0x51cad0)
s32 Network_20324::sub_51CAD0(const char_type *a1, const char_type *a2)
{
    return 0;
}

STUB_FUNC(0x51cb30)
s32 Network_20324::sub_51CB30(s32 a1, HWND hDlg)
{
    return 0;
}

STUB_FUNC(0x51cbc0)
s32 Network_20324::sub_51CBC0()
{
    return 0;
}

STUB_FUNC(0x51ccb0)
LRESULT Network_20324::sub_51CCB0(s32 a1, HWND hDlg)
{
    return 0;
}

STUB_FUNC(0x51cd30)
s32 Network_20324::sub_51CD30(s32 a1, HWND hDlg)
{
    return 0;
}

STUB_FUNC(0x51cdc0)
s32 Network_20324::sub_51CDC0(s32 a2, s32 a3, HWND hDlg)
{
    return 0;
}

STUB_FUNC(0x51cfc0)
s32 Network_20324::sub_51CFC0(const char_type *lParam, HWND hDlg)
{
    return 0;
}

STUB_FUNC(0x51d0c0)
s32 Network_20324::sub_51D0C0(s32 a1, s32 a2, HWND hDlg)
{
    return 0;
}

STUB_FUNC(0x51d2f0)
s32 Network_20324::sub_51D2F0(s32 a1, HWND hDlg)
{
    return 0;
}

STUB_FUNC(0x51d3b0)
s32 Network_20324::sub_51D3B0(s32 a1, HWND hDlg)
{
    return 0;
}
