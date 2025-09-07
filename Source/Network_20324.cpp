#include "Network_20324.hpp"
#include "Globals.hpp"
#include "text_0x14.hpp"

// GLOBAL: 105 0x6F8A18
DEFINE_GLOBAL(UINT_PTR, gTimerId_6F8A18);
// GLOBAL: 105 0x6F88A4
DEFINE_GLOBAL_ARRAY(char_type, Dest_6F88A4, 256);

EXTERN_GLOBAL_ARRAY(char_type, byte_67DC88, 4);

// STUB: 105 0x519960
u16 __stdcall sub_519960(char_type* a1, wchar_t* a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

// STUB: 105 0x5199b0
char_type Network_20324::sub_5199B0(wchar_t* a1, char_type* a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

// FUNCTION: 105 0x519a00
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

// FUNCTION: 105 0x519a50
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
// STUB: 105 0x519a90
Network_20324::Network_20324()
{
    NOT_IMPLEMENTED;
    
    this->field_20320 = 0;
    this->field_202E0_hwnd = 0;
    this->field_20318 = 0;
    this->field_20310 = 0;
    this->field_20314 = 3;
    this->field_20084 = 0;
    this->field_1FD6C = 0;
    this->field_1FD64_f4_idx_count = 0;
    this->field_1FD68 = 0;
    this->field_2019C = GetTickCount();
    this->field_20198_game_type = 1;
    this->field_20194_f_limit = 1;
    this->field_2018C_f4_idx = 0;
    this->field_201A0_police = 0;
    this->field_201A4_game_time_limiot = 0;
    memset(field_4, 0, sizeof(field_4));
    gtext_0x14_6F87F0 = new text_0x14();
    gtext_0x14_6F87F0->Load_5B5E90();
    sub_51BC90();
    sub_51BFA0();
}

// FUNCTION: 105 0x519ba0
Network_20324::~Network_20324()
{
    if (gtext_0x14_6F87F0)
    {
        delete gtext_0x14_6F87F0;
        gtext_0x14_6F87F0 = 0;
    }
}

// STUB: 105 0x519bd0
s32 Network_20324::sub_519BD0(Network_20324* dwInitParam, HINSTANCE hInstance)
{
    NOT_IMPLEMENTED;
    return 0;
}

// STUB: 105 0x519c80
INT_PTR Network_20324::DialogFunc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    NOT_IMPLEMENTED;
    switch (message)
    {
        case WM_TIMER:
            OnTimer_51A9D0(hDlg, wParam);
            return 0;

        case WM_HSCROLL:
            OnWmHScroll_51C630(hDlg, (HWND)lParam, wParam, HIWORD(wParam));
            return 0;

        case WM_COMMAND:
            OnWmCommand_519FE0(hDlg, wParam, lParam, HIWORD(wParam));
            return 0;

        case WM_PAINT:
            OnPaint_519FD0(hDlg);
            return 0;

        case WM_INITDIALOG:
            return OnInitDialog_51AC60(hDlg, wParam, reinterpret_cast<Network_20324*>(lParam));
    }

    return 0;
}

// STUB: 105 0x519d30
s32 Network_20324::cb_sub_519D30(Network_20324* a1, s32 a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

// FUNCTION: 105 0x519e10
void Network_20324::SetDlgHwnd_519E10(HWND a2)
{
    field_202E0_hwnd = a2;
}

// FUNCTION: 105 0x519e20
HWND Network_20324::sub_519E20()
{
    return field_202E0_hwnd;
}

// STUB: 105 0x519e30
LRESULT Network_20324::cb_sub_519E30(Network_20324* a1, wchar_t* Source, s32 a3)
{
    NOT_IMPLEMENTED;
    return 0;
}

// STUB: 105 0x519fd0
void Network_20324::OnPaint_519FD0(HWND a1)
{
    NOT_IMPLEMENTED;
}

// STUB: 105 0x519fe0
void Network_20324::OnWmCommand_519FE0(HWND hDlg, s32 a2, s32 a3, s32 a4)
{
    NOT_IMPLEMENTED;
}

// STUB: 105 0x51a9d0
LRESULT Network_20324::OnTimer_51A9D0(HWND hWnd, s32 a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

// STUB: 105 0x51aa90
char_type Network_20324::sub_51AA90(HWND hWndParent)
{
    NOT_IMPLEMENTED;
    return 0;
}

// STUB: 105 0x51abf0
s32 Network_20324::sub_51ABF0(s32 a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

// FUNCTION: 105 0x51ac60
s32 Network_20324::OnInitDialog_51AC60(HWND hWnd, s32 a2, Network_20324* thisPtr)
{
    SetWindowLongA(hWnd, 8, (LONG)thisPtr);
    gTimerId_6F8A18 = SetTimer(hWnd, 0xAu, 0xAu, 0);
    thisPtr->SetDlgHwnd_519E10(hWnd);
    thisPtr->sub_51AA90(hWnd);
    thisPtr->sub_51AFA0();
    thisPtr->SetPlayerNameText_51B7C0();
    thisPtr->sub_51ABF0(0);
    return 1;
}

// STUB: 105 0x51acc0
s32 Network_20324::sub_51ACC0()
{
    NOT_IMPLEMENTED;
    return 0;
}

// STUB: 105 0x51acd0
LRESULT Network_20324::cb_sub_51ACD0(Network_20324* a1, wchar_t* Source)
{
    NOT_IMPLEMENTED;
    return 0;
}

// STUB: 105 0x51ade0
s32 Network_20324::cb_sub_51ADE0(Network_20324* a1)
{
    NOT_IMPLEMENTED;
    return 0;
}

// STUB: 105 0x51ae20
void Network_20324::sub_51AE20(s32 nIDDlgItem)
{
    NOT_IMPLEMENTED;
}

// STUB: 105 0x51ae50
s32 Network_20324::cb_sub_51AE50(s32 a1, wchar_t* Source)
{
    NOT_IMPLEMENTED;
    return 0;
}

// STUB: 105 0x51afa0
s32 Network_20324::sub_51AFA0()
{
    NOT_IMPLEMENTED;
    return 0;
}

// STUB: 105 0x51b2f0
LRESULT Network_20324::cb_sub_51B2F0(Network_20324* a1, wchar_t* Source)
{
    NOT_IMPLEMENTED;
    return 0;
}

// STUB: 105 0x51b4f0
s32 Network_20324::sub_51B4F0(s32 a2, const char_type* a3)
{
    NOT_IMPLEMENTED;
    return 0;
}

// FUNCTION: 105 0x51b7c0
void Network_20324::SetPlayerNameText_51B7C0()
{
    SetDlgItemTextA(field_202E0_hwnd,
                    1004, // TODO: control constant
                    field_1FF80_player_name);
}

// FUNCTION: 105 0x51b7e0
void Network_20324::cb_sub_51B7E0(Network_20324* a1, const char_type** a2)
{
    const char_type* v2 = *a2;
    if (a1->sub_51ACC0() == 2)
    {
        a1->sub_51B810(v2);
    }
}

// STUB: 105 0x51b810
void Network_20324::sub_51B810(const char_type* a2)
{
    NOT_IMPLEMENTED;
}

// STUB: 105 0x51b9c0
s32 Network_20324::sub_51B9C0(s32 a2, char_type* Data)
{
    NOT_IMPLEMENTED;
    return 0;
}

// FUNCTION: 105 0x51bbc0
void Network_20324::sub_51BBC0()
{
    field_1FD6C++;
}

// FUNCTION: 105 0x51bbd0
s32 Network_20324::sub_51BBD0()
{
    return field_1FD6C;
}

// FUNCTION: 105 0x51bbe0
void Network_20324::sub_51BBE0()
{
    field_1FD6C--;
}

// FUNCTION: 105 0x51bbf0
void Network_20324::sub_51BBF0()
{
    field_1FD6C = 0;
}

// FUNCTION: 105 0x51bc00
void Network_20324::cb_sub_51BC00(Network_20324* pThis)
{
    char String[260];
    wchar_t Dest[260];

    GetDlgItemTextA(pThis->sub_519E20(), 1004, String, GTA2_COUNTOF(String)); // TODO: control constants
    GetString_519A50(Dest, String, GTA2_COUNTOF(String));
    pThis->sub_51BD40(Dest, String);
    pThis->sub_51BC70(1);
}

// FUNCTION: 105 0x51bc70
void Network_20324::sub_51BC70(s32 a2)
{
    field_1FD68 = a2;
}

// FUNCTION: 105 0x51bc80
s32 Network_20324::sub_51BC80()
{
    return field_1FD68;
}

// STRING: 105 0x5fe8ac
#define REGISTRY_KEY_NETWORK_STRING "Software\\DMA Design Ltd\\GTA2\\Network"

// STRING: 105 0x5fe8d4
#define PLAYER_NAME_STRING "PlayerName"

// FUNCTION: 105 0x51bc90
void Network_20324::sub_51BC90()
{
    DWORD Type = REG_SZ;
    BYTE Data[260];
    DWORD cbData = GTA2_COUNTOF(Data);

    HKEY phkResult;

    DWORD dwDisposition;

    if (RegCreateKeyExA(HKEY_LOCAL_MACHINE,
                        REGISTRY_KEY_NETWORK_STRING,
                        0,
                        0,
                        0,
                        KEY_ALL_ACCESS,
                        0,
                        &phkResult,
                        &dwDisposition) == ERROR_SUCCESS)
    {

        if (RegQueryValueExA(phkResult, PLAYER_NAME_STRING, 0, &Type, Data, &cbData) == ERROR_SUCCESS)
        {
            strcpy(field_1FF80_player_name, reinterpret_cast<char*>(Data));
            GetString_519A50(field_1FD78_player_name_2, reinterpret_cast<char*>(Data), GTA2_COUNTOF(field_1FD78_player_name_2));
        }
        RegCloseKey(phkResult);
    }
}

// STUB: 105 0x51bd40
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

// STUB: 105 0x51bdd0
LRESULT Network_20324::sub_51BDD0(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
    NOT_IMPLEMENTED;
    return 0;
}

// STUB: 105 0x51beb0
s32 Network_20324::sub_51BEB0(s32 nIDDlgItem, s32 a3)
{
    NOT_IMPLEMENTED;
    return 0;
}

// STUB: 105 0x51bfa0
void Network_20324::sub_51BFA0()
{
    NOT_IMPLEMENTED;
}

// STUB: 105 0x51c630
s32 Network_20324::OnWmHScroll_51C630(HWND hWnd, HWND a2, s32 a3, s32 a4)
{
    NOT_IMPLEMENTED;
    return 0;
}

// STUB: 105 0x51c7f0
void Network_20324::sub_51C7F0(s32* a2)
{
    NOT_IMPLEMENTED;
}

// STUB: 105 0x51c830
u32 Network_20324::sub_51C830()
{
    NOT_IMPLEMENTED;
    return 0;
}

// FUNCTION: 105 0x51ca10
char_type* Network_20324::sub_51CA10()
{
    return &this->field_4[this->field_2018C_f4_idx][0];
}

// FUNCTION: 105 0x51ca50
char_type* Network_20324::sub_51CA50()
{
    return &this->field_4[this->field_2018C_f4_idx][260];
}

// FUNCTION: 105 0x51ca90
char_type* Network_20324::sub_51CA90()
{
    return &this->field_4[this->field_2018C_f4_idx][520];
}

// STRING: 105 0x623e04
#define UNABLE_TO_OPEN_SUB_MMP_FILE "Unable to open file: %s in mmpfile: %s"

// FUNCTION: 105 0x51cad0
void Network_20324::sub_51CAD0(const char_type* a1, const char_type* a2)
{
    char Text[256];
    sprintf(Text, UNABLE_TO_OPEN_SUB_MMP_FILE, a2, a1);
    MessageBoxA(0, Text, byte_67DC88, MB_ICONEXCLAMATION);
}

// STRING: 105 0x623e34
#define NETUI13_STRING "netui13"

// STRING: 105 0x623e2c
#define STRING_NUMBER_FORMAT_STRING "%s %d"

// FUNCTION: 105 0x51cb30
s32 Network_20324::sub_51CB30(s32 a1, HWND hDlg)
{
    char String[260];
    char* pStr = GetString_519A00(NETUI13_STRING);
    sprintf(String, STRING_NUMBER_FORMAT_STRING, pStr, a1);
    return SetDlgItemTextA(hDlg, 1033, String);
}

// STUB: 105 0x51cbc0
s32 Network_20324::sub_51CBC0()
{
    NOT_IMPLEMENTED;
    return 0;
}

// STUB: 105 0x51ccb0
LRESULT Network_20324::sub_51CCB0(s32 a1, HWND hDlg)
{
    NOT_IMPLEMENTED;
    return 0;
}

// STUB: 105 0x51cd30
s32 Network_20324::sub_51CD30(s32 a1, HWND hDlg)
{
    NOT_IMPLEMENTED;
    return 0;
}

// STUB: 105 0x51cdc0
s32 Network_20324::sub_51CDC0(s32 a2, s32 a3, HWND hDlg)
{
    NOT_IMPLEMENTED;
    return 0;
}

// STUB: 105 0x51cfc0
s32 Network_20324::sub_51CFC0(const char_type* lParam, HWND hDlg)
{
    NOT_IMPLEMENTED;
    return 0;
}

// STUB: 105 0x51d0c0
s32 Network_20324::sub_51D0C0(s32 a1, s32 a2, HWND hDlg)
{
    NOT_IMPLEMENTED;
    return 0;
}

// STUB: 105 0x51d2f0
s32 Network_20324::sub_51D2F0(s32 a1, HWND hDlg)
{
    NOT_IMPLEMENTED;
    return 0;
}

// STUB: 105 0x51d3b0
s32 Network_20324::sub_51D3B0(s32 a1, HWND hDlg)
{
    NOT_IMPLEMENTED;
    return 0;
}