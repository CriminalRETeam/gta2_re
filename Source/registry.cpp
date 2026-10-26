#include "registry.hpp"
#include "Function.hpp"
#include "Globals.hpp"
#include "error.hpp"
#include "stdafx.h"
#include <stdio.h>
#include <windows.h>
#include "enums.hpp"

HKEY kRegistryRootKey_625018 = HKEY_LOCAL_MACHINE; // Note: should be const, but can't be

DEFINE_GLOBAL(Registry, gRegistry_6FF968, 0x6FF968);

MATCH_FUNC(0x586A00)
bool Registry::Open_Sound_Root_586A00(PHKEY phkResult)
{
    if (::RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SOFTWARE\\DMA Design Ltd\\GTA2\\Sound", 0, KEY_ALL_ACCESS, phkResult) != ERROR_SUCCESS)
    {
        DWORD dwDisposition;

        if (::RegCreateKeyExA(HKEY_LOCAL_MACHINE,
                              "SOFTWARE\\DMA Design Ltd\\GTA2\\Sound",
                              0,
                              "", //byte_67DC88,
                              0,
                              KEY_ALL_ACCESS,
                              NULL,
                              phkResult,
                              &dwDisposition) != ERROR_SUCCESS)
        {
            FatalError_4A38C0(Gta2Error::CreateRegistryKeyFail, "C:\\Splitting\\Gta2\\Source\\registry.cpp", 58);
        }
        return false;
    }
    return true;
}

MATCH_FUNC(0x586A70)
bool Registry::Sound_Setting_Exists_586A70(const char_type* lpValueName)
{
    HKEY hKey;
    DWORD cbData;
    u8 data[4];

    bool settingReadOK = false;
    if (Open_Sound_Root_586A00(&hKey))
    {
        cbData = sizeof(DWORD);
        if (::RegQueryValueExA(hKey, lpValueName, 0, 0, data, &cbData) == ERROR_SUCCESS)
        {
            settingReadOK = true;
        }
    }

    if (::RegCloseKey(hKey) != ERROR_SUCCESS)
    {
        FatalError_4A38C0(Gta2Error::CloseRegistryKeyFail, "C:\\Splitting\\Gta2\\Source\\registry.cpp", 109);
    }
    return settingReadOK;
}

MATCH_FUNC(0x586AE0)
s32 Registry::Get_Sound_Setting_586AE0(const char_type* lpValueName, s32 value)
{
    HKEY hKey;
    Open_Sound_Root_586A00(&hKey);

    DWORD cbData = sizeof(DWORD);
    DWORD data;
    if (::RegQueryValueExA(hKey, lpValueName, 0, 0, reinterpret_cast<BYTE*>(&data), &cbData) != ERROR_SUCCESS)
    {
        if (::RegSetValueExA(hKey, lpValueName, 0, REG_DWORD, reinterpret_cast<const BYTE*>(&value), sizeof(DWORD)) != ERROR_SUCCESS)
        {
            FatalError_4A38C0(Gta2Error::SetRegistryValueFail, "C:\\Splitting\\Gta2\\Source\\registry.cpp", 138);
        }

        data = value;
    }

    if (::RegCloseKey(hKey) != ERROR_SUCCESS)
    {
        FatalError_4A38C0(Gta2Error::CloseRegistryKeyFail, "C:\\Splitting\\Gta2\\Source\\registry.cpp", 146);
    }

    return data;
}

MATCH_FUNC(0x586B80)
void Registry::Set_Sound_Setting_586B80(const char_type* lpValueName, s32 Data)
{
    HKEY hKey;
    if (Open_Sound_Root_586A00(&hKey))
    {
        if (RegSetValueExA(hKey, lpValueName, 0, REG_DWORD, reinterpret_cast<const BYTE*>(&Data), sizeof(DWORD)) != ERROR_SUCCESS)
        {
            FatalError_4A38C0(Gta2Error::SetRegistryValueFail, "C:\\Splitting\\Gta2\\Source\\registry.cpp", 169);
        }
    }

    if (RegCloseKey(hKey) != ERROR_SUCCESS)
    {
        FatalError_4A38C0(Gta2Error::CloseRegistryKeyFail, "C:\\Splitting\\Gta2\\Source\\registry.cpp", 175);
    }
}

MATCH_FUNC(0x586BF0)
void Registry::Clear_Or_Delete_Sound_Setting_586BF0(const char_type* lpValueName, char_type bClear)
{
    HKEY hKey;
    u32 Data = 0;
    if (Open_Sound_Root_586A00(&hKey))
    {
        if (bClear)
        {
            RegSetValueExA(hKey, lpValueName, 0, REG_DWORD, reinterpret_cast<const BYTE*>(&Data), sizeof(DWORD));
            // note: wrong register fixed by adding else instead of early return + dup'ed close call
        }
        else
        {
            RegDeleteValueA(hKey, lpValueName);
        }
    }
    RegCloseKey(hKey); // bug: closes even if open fails
}

MATCH_FUNC(0x586C60)
bool Registry::Open_Debug_Root_586C60(PHKEY phkResult)
{
    DWORD dwDisposition;
    if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SOFTWARE\\DMA Design Ltd\\GTA2\\Debug", 0, KEY_ALL_ACCESS, phkResult) != ERROR_SUCCESS)
    {
        if (RegCreateKeyExA(HKEY_LOCAL_MACHINE,
                            "SOFTWARE\\DMA Design Ltd\\GTA2\\Debug",
                            0,
                            "", //byte_67DC88,
                            0,
                            KEY_ALL_ACCESS,
                            0,
                            phkResult,
                            &dwDisposition) != ERROR_SUCCESS)
        {
            FatalError_4A38C0(Gta2Error::CreateRegistryKeyFail, "C:\\Splitting\\Gta2\\Source\\registry.cpp", 232);
        }
        return false;
    }
    return true;
}

MATCH_FUNC(0x586CD0)
bool Registry::Open_Options_Root_586CD0(PHKEY phkResult)
{
    DWORD dwDisposition;
    if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SOFTWARE\\DMA Design Ltd\\GTA2\\Option", 0, KEY_ALL_ACCESS, phkResult) != ERROR_SUCCESS)
    {
        if (RegCreateKeyExA(HKEY_LOCAL_MACHINE,
                            "SOFTWARE\\DMA Design Ltd\\GTA2\\Option",
                            0,
                            "", //byte_67DC88,
                            0,
                            KEY_ALL_ACCESS,
                            0,
                            phkResult,
                            &dwDisposition) != ERROR_SUCCESS)
        {
            FatalError_4A38C0(Gta2Error::CreateRegistryKeyFail, "C:\\Splitting\\Gta2\\Source\\registry.cpp", 265);
        }
        return false;
    }
    return true;
}

MATCH_FUNC(0x586D40)
bool Registry::Open_Control_Root_586D40(PHKEY phkResult)
{
    DWORD dwDisposition;
    if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SOFTWARE\\DMA Design Ltd\\GTA2\\Control", 0, KEY_ALL_ACCESS, phkResult) != ERROR_SUCCESS)
    {
        if (RegCreateKeyExA(HKEY_LOCAL_MACHINE,
                            "SOFTWARE\\DMA Design Ltd\\GTA2\\Control",
                            0,
                            "", //byte_67DC88,
                            0,
                            KEY_ALL_ACCESS,
                            0,
                            phkResult,
                            &dwDisposition) != ERROR_SUCCESS)
        {
            FatalError_4A38C0(Gta2Error::CreateRegistryKeyFail, "C:\\Splitting\\Gta2\\Source\\registry.cpp", 298);
        }
        return false;
    }
    return true;
}

MATCH_FUNC(0x586DB0)
bool Registry::Open_Screen_Root_586DB0(PHKEY phkResult)
{
    DWORD dwDisposition;
    if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SOFTWARE\\DMA Design Ltd\\GTA2\\Screen", 0, KEY_ALL_ACCESS, phkResult) != ERROR_SUCCESS)
    {
        if (RegCreateKeyExA(HKEY_LOCAL_MACHINE,
                            "SOFTWARE\\DMA Design Ltd\\GTA2\\Screen",
                            0,
                            "", //byte_67DC88,
                            0,
                            KEY_ALL_ACCESS,
                            0,
                            phkResult,
                            &dwDisposition))
        {
            FatalError_4A38C0(Gta2Error::CreateRegistryKeyFail, "C:\\Splitting\\Gta2\\Source\\registry.cpp", 331);
        }
        return false;
    }
    return true;
}

MATCH_FUNC(0x586E20)
bool Registry::Debug_Setting_Exists_586E20(const char_type* lpValueName)
{
    bool bRead = false;
    HKEY hKey;
    DWORD Data;
    if (Open_Debug_Root_586C60(&hKey))
    {
        DWORD cbData = sizeof(DWORD);
        if (RegQueryValueExA(hKey, lpValueName, 0, 0, reinterpret_cast<BYTE*>(&Data), &cbData) == ERROR_SUCCESS)
        {
            bRead = true;
        }
    }

    if (RegCloseKey(hKey) != ERROR_SUCCESS)
    {
        FatalError_4A38C0(Gta2Error::CloseRegistryKeyFail, "C:\\Splitting\\Gta2\\Source\\registry.cpp", 424);
    }

    return bRead;
}

MATCH_FUNC(0x586E90)
void Registry::Get_Debug_Setting_586E90(const char_type* lpValueName, LPBYTE lpData, s32 data_size)
{
    HKEY hKey;
    DWORD cbData;
    if (Open_Debug_Root_586C60(&hKey))
    {
        cbData = static_cast<u16>(data_size);
        RegQueryValueExA(hKey, lpValueName, 0, 0, lpData, &cbData);
    }

    if (RegCloseKey(hKey) != ERROR_SUCCESS)
    {
        FatalError_4A38C0(Gta2Error::CloseRegistryKeyFail, "C:\\Splitting\\Gta2\\Source\\registry.cpp", 531);
    }
}

MATCH_FUNC(0x586F00)
void Registry::Get_Option_586F00(const char_type* lpValueName, BYTE* pBuffer, s32 bufferSize)
{
    DWORD cbData;
    HKEY hKey;
    *pBuffer = 0;

    if (Open_Options_Root_586CD0(&hKey))
    {
        cbData = static_cast<u16>(bufferSize);
        RegQueryValueExA(hKey, lpValueName, 0, 0, pBuffer, &cbData);
    }

    // bug: closing key even if not opened
    if (RegCloseKey(hKey) != ERROR_SUCCESS)
    {
        FatalError_4A38C0(Gta2Error::CloseRegistryKeyFail, "C:\\Splitting\\Gta2\\Source\\registry.cpp", 586);
    }
}

MATCH_FUNC(0x586F70)
s32 Registry::Get_Option_Int_586F70(const char_type* lpValueName, s32 value)
{
    HKEY hKey;
    DWORD Data;

    Open_Options_Root_586CD0(&hKey);

    DWORD cbData = sizeof(DWORD);

    if (RegQueryValueExA(hKey, lpValueName, 0, 0, reinterpret_cast<BYTE*>(&Data), &cbData) != ERROR_SUCCESS)
    {
        if (RegSetValueExA(hKey, lpValueName, 0, REG_DWORD, reinterpret_cast<const BYTE*>(&value), sizeof(DWORD)) != ERROR_SUCCESS)
        {
            FatalError_4A38C0(Gta2Error::SetRegistryValueFail, "C:\\Splitting\\Gta2\\Source\\registry.cpp", 622);
        }
        Data = value;
    }

    if (RegCloseKey(hKey) != ERROR_SUCCESS)
    {
        FatalError_4A38C0(Gta2Error::CloseRegistryKeyFail, "C:\\Splitting\\Gta2\\Source\\registry.cpp", 629);
    }

    return Data;
}

MATCH_FUNC(0x587010)
s32 Registry::Get_Control_Setting_587010(s32 control_idx, s32 value)
{
    HKEY hKey;
    DWORD Data;
    Open_Control_Root_586D40(&hKey);

    DWORD cbData = sizeof(DWORD);
    char_type ValueName[4];
    sprintf(ValueName, "%d", static_cast<BYTE>(control_idx));

    if (RegQueryValueExA(hKey, ValueName, 0, 0, reinterpret_cast<BYTE*>(&Data), &cbData) != ERROR_SUCCESS)
    {
        if (RegSetValueExA(hKey, ValueName, 0, REG_DWORD, reinterpret_cast<const BYTE*>(&value), sizeof(DWORD)) != ERROR_SUCCESS)
        {
            FatalError_4A38C0(Gta2Error::SetRegistryValueFail, "C:\\Splitting\\Gta2\\Source\\registry.cpp", 667);
        }
        Data = value;
    }

    if (RegCloseKey(hKey) != ERROR_SUCCESS)
    {
        FatalError_4A38C0(Gta2Error::CloseRegistryKeyFail, "C:\\Splitting\\Gta2\\Source\\registry.cpp", 674);
    }

    return Data;
}

MATCH_FUNC(0x5870D0)
s32 Registry::Get_Screen_Setting_5870D0(const char_type* lpValueName, s32 default_value)
{
    HKEY hKey;
    Open_Screen_Root_586DB0(&hKey);

    DWORD Data;
    DWORD cbData = sizeof(DWORD);
    if (RegQueryValueExA(hKey, lpValueName, 0, 0, reinterpret_cast<BYTE*>(&Data), &cbData) != ERROR_SUCCESS)
    {
        if (RegSetValueExA(hKey, lpValueName, 0, REG_DWORD, reinterpret_cast<const BYTE*>(&default_value), sizeof(DWORD)) != ERROR_SUCCESS)
        {
            FatalError_4A38C0(Gta2Error::SetRegistryValueFail, "C:\\Splitting\\Gta2\\Source\\registry.cpp", 709);
        }
        Data = default_value;
    }

    if (RegCloseKey(hKey) != ERROR_SUCCESS)
    {
        FatalError_4A38C0(Gta2Error::CloseRegistryKeyFail, "C:\\Splitting\\Gta2\\Source\\registry.cpp", 716);
    }

    return Data;
}

MATCH_FUNC(0x587170)
void Registry::Set_Screen_Setting_587170(const char_type* lpValueName, s32 Data)
{
    HKEY hKey;
    Open_Screen_Root_586DB0(&hKey);

    if (RegSetValueExA(hKey, lpValueName, 0, REG_DWORD, reinterpret_cast<BYTE*>(&Data), sizeof(DWORD)) != ERROR_SUCCESS)
    {
        FatalError_4A38C0(Gta2Error::SetRegistryValueFail, "C:\\Splitting\\Gta2\\Source\\registry.cpp", 743);
    }

    if (RegCloseKey(hKey) != ERROR_SUCCESS)
    {
        FatalError_4A38C0(Gta2Error::CloseRegistryKeyFail, "C:\\Splitting\\Gta2\\Source\\registry.cpp", 745);
    }
}

MATCH_FUNC(0x5871E0)
void Registry::Get_Screen_String_5871E0(const char_type* lpValueName, BYTE* lpData, u16 maxLen)
{
    HKEY hKey;
    DWORD cbData;

    Open_Screen_Root_586DB0(&hKey);

    cbData = maxLen;
    if (RegQueryValueExA(hKey, lpValueName, 0, 0, lpData, &cbData) &&
        RegSetValueExA(hKey, lpValueName, 0, 1u, lpData, strlen(reinterpret_cast<const char_type*>(lpData)) + 1))
    {
        FatalError_4A38C0(Gta2Error::SetRegistryValueFail, "C:\\Splitting\\Gta2\\Source\\registry.cpp", 781);
    }

    if (RegCloseKey(hKey))
    {
        FatalError_4A38C0(Gta2Error::CloseRegistryKeyFail, "C:\\Splitting\\Gta2\\Source\\registry.cpp", 787);
    }
}

MATCH_FUNC(0x587290)
void Registry::null_587290()
{
}

MATCH_FUNC(0x5872A0)
bool Registry::Set_Binary_5872A0(HKEY hKey, const char_type* base_name, BYTE* lpData, u32 Data)
{
    char_type Buffer[260];
    bool ret = false;

    sprintf(Buffer, "%ss", base_name);
    if (RegSetValueExA(hKey, Buffer, 0, REG_DWORD, reinterpret_cast<const BYTE*>(&Data), sizeof(DWORD)) == ERROR_SUCCESS)
    {
        sprintf(Buffer, "%sd", base_name);
        if (RegSetValueExA(hKey, Buffer, 0, REG_BINARY, lpData, Data) == ERROR_SUCCESS)
        {
            ret = true;
        }
    }
    return ret;
}

MATCH_FUNC(0x587340)
bool Registry::Get_Binary_587340(HKEY hKey, const char_type* keyPath, s32 value, LPBYTE lpData)
{
    char_type ValueName[260];
    bool ret = false;

    sprintf(ValueName, "%ss", keyPath);
    DWORD v = Get_Int_Setting_5874E0(hKey, ValueName);
    if (v == static_cast<DWORD>(value))
    {
        sprintf(ValueName, "%sd", keyPath);
        ret = RegQueryValueExA(hKey, ValueName, 0, 0, lpData, &v) == ERROR_SUCCESS;
    }
    return ret;
}

MATCH_FUNC(0x5873E0)
s32 Registry::Get_Binary_Size_5873E0(HKEY hKey, const char_type* subKey)
{
    char_type keyPath[260];
    sprintf(keyPath, "%ss", subKey);
    return Get_Int_Setting_5874E0(hKey, keyPath);
}

MATCH_FUNC(0x587420)
bool Registry::CreateNetworkRoot_587420(PHKEY phkResult)
{
    DWORD dwDisposition;
    char_type SubKey[260];

    strcpy(SubKey, "SOFTWARE\\DMA Design Ltd\\GTA2");
    strcat(SubKey, "\\Network");

    if (RegCreateKeyExA(kRegistryRootKey_625018, SubKey, 0, "", 0, KEY_ALL_ACCESS, 0, phkResult, &dwDisposition) != ERROR_SUCCESS)
    {
        FatalError_4A38C0(Gta2Error::CreateRegistryKeyFail, "C:\\Splitting\\Gta2\\Source\\registry.cpp", 910);
    }

    return true;
}

MATCH_FUNC(0x5874E0)
DWORD Registry::Get_Int_Setting_5874E0(HKEY hKey, const char_type* lpValueName)
{
    DWORD cbData = 4;

    if (RegQueryValueExA(hKey, lpValueName, 0, reinterpret_cast<LPDWORD>(&hKey), reinterpret_cast<LPBYTE>(&lpValueName), &cbData) != ERROR_SUCCESS)
    {
        // Zeroing the value instead of returning 0 keeps VC6 from going branchless
        lpValueName = 0;
    }

    return reinterpret_cast<DWORD>(lpValueName);
}

MATCH_FUNC(0x5875A0)
s32 __stdcall Registry::Get_Binary_Value_5875A0(HKEY hKey, const char_type* keyPath, LPBYTE lpData)
{
    s32 ret = 0;
    s32 size = gRegistry_6FF968.Get_Binary_Size_5873E0(hKey, keyPath);
    if (size)
    {
        if (gRegistry_6FF968.Get_Binary_587340(hKey, keyPath, size, lpData))
        {
            ret = 1;
        }
    }
    return ret;
}

MATCH_FUNC(0x5875E0)
s32 __stdcall Registry::Set_WString_Value_5875E0(HKEY hKey, const char_type* keyPath, const wchar_t* pStr)
{
    s32 ret = 0;
    u32 size = 2 * wcslen(pStr) + 2;
    if (gRegistry_6FF968.Set_Binary_5872A0(hKey, keyPath, (BYTE*)pStr, size))
    {
        ret = 1;
    }
    return ret;
}

MATCH_FUNC(0x587620)
bool Registry::Network_Setting_Exists_587620(const char_type* lpValueName)
{
    HKEY hKey;
    u32 Data;
    DWORD cbData;
    bool ret = false;
    if (CreateNetworkRoot_587420(&hKey))
    {
        cbData = sizeof(DWORD);
        if (RegQueryValueExA(hKey, lpValueName, 0, 0, reinterpret_cast<BYTE*>(&Data), &cbData) == ERROR_SUCCESS)
        {
            ret = true;
        }
    }

    if (RegCloseKey(hKey) != ERROR_SUCCESS)
    {
        FatalError_4A38C0(Gta2Error::CloseRegistryKeyFail, "C:\\Splitting\\Gta2\\Source\\registry.cpp", 1020);
    }
    return ret;
}

MATCH_FUNC(0x587690)
s32 Registry::Get_Network_Setting_587690(const char_type* lpValueName, s32 value)
{
    HKEY hKey;
    CreateNetworkRoot_587420(&hKey);

    u32 Data;
    DWORD cbData = sizeof(DWORD);
    if (RegQueryValueExA(hKey, lpValueName, 0, 0, reinterpret_cast<BYTE*>(&Data), &cbData) != ERROR_SUCCESS)
    {
        if (RegSetValueExA(hKey, lpValueName, 0, REG_DWORD, reinterpret_cast<const BYTE*>(&value), sizeof(DWORD)) != ERROR_SUCCESS)
        {
            FatalError_4A38C0(Gta2Error::SetRegistryValueFail, "C:\\Splitting\\Gta2\\Source\\registry.cpp", 1051);
        }
        Data = value;
    }

    if (RegCloseKey(hKey) != ERROR_SUCCESS)
    {
        FatalError_4A38C0(Gta2Error::CloseRegistryKeyFail, "C:\\Splitting\\Gta2\\Source\\registry.cpp", 1058);
    }

    return Data;
}

MATCH_FUNC(0x587730)
void Registry::Set_Network_Setting_587730(const char_type* lpValueName, BYTE Data)
{
    HKEY hKey;
    if (CreateNetworkRoot_587420(&hKey))
    {
        if (RegSetValueExA(hKey, lpValueName, 0, REG_DWORD, &Data, sizeof(DWORD)))
        {
            FatalError_4A38C0(Gta2Error::SetRegistryValueFail, "C:\\Splitting\\Gta2\\Source\\registry.cpp", 1080);
        }
    }

    if (RegCloseKey(hKey) != ERROR_SUCCESS)
    {
        FatalError_4A38C0(Gta2Error::CloseRegistryKeyFail, "C:\\Splitting\\Gta2\\Source\\registry.cpp", 1086);
    }
}

MATCH_FUNC(0x5877A0)
bool Registry::Open_Player_Root_5877A0(PHKEY phkResult)
{
    if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SOFTWARE\\DMA Design Ltd\\GTA2\\Player", 0, KEY_ALL_ACCESS, phkResult) != ERROR_SUCCESS)
    {
        DWORD dwDisposition;
        if (RegCreateKeyExA(HKEY_LOCAL_MACHINE,
                            "SOFTWARE\\DMA Design Ltd\\GTA2\\Player",
                            0,
                            "",
                            0,
                            KEY_ALL_ACCESS,
                            0,
                            phkResult,
                            &dwDisposition) != ERROR_SUCCESS)
        {
            FatalError_4A38C0(Gta2Error::CreateRegistryKeyFail, "C:\\Splitting\\Gta2\\Source\\registry.cpp", 1110);
        }
        return false;
    }
    return true;
}

MATCH_FUNC(0x587810)
DWORD Registry::Get_Player_Setting_587810(const char_type* lpValueName)
{
    HKEY hKey;
    DWORD Data;
    if (Open_Player_Root_5877A0(&hKey))
    {
        DWORD cbData = sizeof(DWORD);
        if (RegQueryValueExA(hKey, lpValueName, 0, 0, reinterpret_cast<BYTE*>(&Data), &cbData) != ERROR_SUCCESS)
        {
            Data = 0;
        }
    }
    else
    {
        Data = 0;
        if (RegSetValueExA(hKey, lpValueName, 0, REG_DWORD, reinterpret_cast<BYTE*>(&Data), sizeof(DWORD)) != ERROR_SUCCESS)
        {
            FatalError_4A38C0(Gta2Error::SetRegistryValueFail, "C:\\Splitting\\Gta2\\Source\\registry.cpp", 1143);
        }
    }

    if (RegCloseKey(hKey) != ERROR_SUCCESS)
    {
        FatalError_4A38C0(Gta2Error::CloseRegistryKeyFail, "C:\\Splitting\\Gta2\\Source\\registry.cpp", 1149);
    }

    return Data;
}

MATCH_FUNC(0x5878C0)
void Registry::Set_Player_Setting_5878C0(const char_type* lpValueName, DWORD Data)
{
    HKEY hKey;
    if (Open_Player_Root_5877A0(&hKey))
    {
        if (RegSetValueExA(hKey, lpValueName, 0, REG_DWORD, reinterpret_cast<BYTE*>(&Data), sizeof(DWORD)) != ERROR_SUCCESS)
        {
            FatalError_4A38C0(Gta2Error::SetRegistryValueFail, "C:\\Splitting\\Gta2\\Source\\registry.cpp", 1171);
        }
    }

    if (RegCloseKey(hKey) != ERROR_SUCCESS)
    {
        FatalError_4A38C0(Gta2Error::CloseRegistryKeyFail, "C:\\Splitting\\Gta2\\Source\\registry.cpp", 1177);
    }
}