#include "registry.hpp"
#include "Function.hpp"
#include "Globals.hpp"
#include "error.hpp"
#include "stdafx.h"
#include <stdio.h>
#include <windows.h>

HKEY dword_625018 = HKEY_LOCAL_MACHINE; // Note: should be const, but can't be

// GLOBAL: 105 0x6FF968
DEFINE_GLOBAL(Registry, gRegistry_6FF968);

// FUNCTION: 105 0x586A00
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
            FatalError_4A38C0(43, "C:\\Splitting\\Gta2\\Source\\registry.cpp", 58);
        }
        return false;
    }
    return true;
}

// FUNCTION: 105 0x586A70
bool Registry::Get_Sound_Settting_586A70(const char_type* lpValueName)
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
        FatalError_4A38C0(42, "C:\\Splitting\\Gta2\\Source\\registry.cpp", 109);
    }
    return settingReadOK;
}

// FUNCTION: 105 0x586AE0
s32 Registry::Set_Sound_Setting_586AE0(const char_type* lpValueName, s32 value)
{
    HKEY hKey;
    Open_Sound_Root_586A00(&hKey);

    DWORD cbData = sizeof(DWORD);
    DWORD data;
    if (::RegQueryValueExA(hKey, lpValueName, 0, 0, reinterpret_cast<BYTE*>(&data), &cbData) != ERROR_SUCCESS)
    {
        if (::RegSetValueExA(hKey, lpValueName, 0, REG_DWORD, reinterpret_cast<const BYTE*>(&value), sizeof(DWORD)) != ERROR_SUCCESS)
        {
            FatalError_4A38C0(46, "C:\\Splitting\\Gta2\\Source\\registry.cpp", 138);
        }

        data = value;
    }

    if (::RegCloseKey(hKey) != ERROR_SUCCESS)
    {
        FatalError_4A38C0(42, "C:\\Splitting\\Gta2\\Source\\registry.cpp", 146);
    }

    return data;
}

// FUNCTION: 105 0x586B80
void Registry::Set_Sound_Setting_586B80(const char_type* lpValueName, s32 Data)
{
    HKEY hKey;
    if (Open_Sound_Root_586A00(&hKey))
    {
        if (RegSetValueExA(hKey, lpValueName, 0, REG_DWORD, reinterpret_cast<const BYTE*>(&Data), sizeof(DWORD)) != ERROR_SUCCESS)
        {
            FatalError_4A38C0(46, "C:\\Splitting\\Gta2\\Source\\registry.cpp", 169);
        }
    }

    if (RegCloseKey(hKey) != ERROR_SUCCESS)
    {
        FatalError_4A38C0(42, "C:\\Splitting\\Gta2\\Source\\registry.cpp", 175);
    }
}

// FUNCTION: 105 0x586BF0
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

// FUNCTION: 105 0x586C60
char_type Registry::Open_Debug_Root_586C60(PHKEY phkResult)
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
            FatalError_4A38C0(43, "C:\\Splitting\\Gta2\\Source\\registry.cpp", 232);
        }
        return 0;
    }
    return 1;
}

// FUNCTION: 105 0x586CD0
char_type Registry::Open_Options_Root_586CD0(PHKEY phkResult)
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
            FatalError_4A38C0(43, "C:\\Splitting\\Gta2\\Source\\registry.cpp", 265);
        }
        return 0;
    }
    return 1;
}

// FUNCTION: 105 0x586D40
char_type Registry::Open_Control_Root_586D40(PHKEY phkResult)
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
            FatalError_4A38C0(43, "C:\\Splitting\\Gta2\\Source\\registry.cpp", 298);
        }
        return 0;
    }
    return 1;
}

// FUNCTION: 105 0x586DB0
char_type Registry::Open_Screen_Root_586DB0(PHKEY phkResult)
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
            FatalError_4A38C0(43, "C:\\Splitting\\Gta2\\Source\\registry.cpp", 331);
        }
        return 0;
    }
    return 1;
}

// FUNCTION: 105 0x586E20
bool Registry::Get_Debug_Setting_586E20(const char_type* lpValueName)
{
    bool bRead = false;
    HKEY hKey;
    DWORD Data;
    if (Open_Debug_Root_586C60(&hKey))
    {
        DWORD cbData = sizeof(DWORD);
        if (RegQueryValueExA(hKey, lpValueName, 0, 0, reinterpret_cast<BYTE*>(&Data), &cbData) == ERROR_SUCCESS)
        {
            bRead = 1;
        }
    }

    if (RegCloseKey(hKey) != ERROR_SUCCESS)
    {
        FatalError_4A38C0(42, "C:\\Splitting\\Gta2\\Source\\registry.cpp", 424);
    }

    return bRead;
}

// FUNCTION: 105 0x586E90
void Registry::Get_Debug_Setting_586E90(const char_type* lpValueName, LPBYTE lpData, s32 a3)
{
    HKEY hKey;
    DWORD cbData;
    if (Open_Debug_Root_586C60(&hKey))
    {
        cbData = static_cast<u16>(a3);
        RegQueryValueExA(hKey, lpValueName, 0, 0, lpData, &cbData);
    }

    if (RegCloseKey(hKey) != ERROR_SUCCESS)
    {
        FatalError_4A38C0(42, "C:\\Splitting\\Gta2\\Source\\registry.cpp", 531);
    }
}

// FUNCTION: 105 0x586F00
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
        FatalError_4A38C0(42, "C:\\Splitting\\Gta2\\Source\\registry.cpp", 586);
    }
}

// FUNCTION: 105 0x586F70
s32 Registry::Set_Option_586F70(const char_type* lpValueName, s32 a3)
{
    HKEY hKey;
    DWORD Data;

    Open_Options_Root_586CD0(&hKey);

    DWORD cbData = sizeof(DWORD);

    if (RegQueryValueExA(hKey, lpValueName, 0, 0, reinterpret_cast<BYTE*>(&Data), &cbData) != ERROR_SUCCESS)
    {
        if (RegSetValueExA(hKey, lpValueName, 0, REG_DWORD, reinterpret_cast<const BYTE*>(&a3), sizeof(DWORD)) != ERROR_SUCCESS)
        {
            FatalError_4A38C0(46, "C:\\Splitting\\Gta2\\Source\\registry.cpp", 622);
        }
        Data = a3;
    }

    if (RegCloseKey(hKey) != ERROR_SUCCESS)
    {
        FatalError_4A38C0(42, "C:\\Splitting\\Gta2\\Source\\registry.cpp", 629);
    }

    return Data;
}

// FUNCTION: 105 0x587010
s32 Registry::Set_Control_Setting_587010(s32 a1, s32 a2)
{
    HKEY hKey;
    DWORD Data;
    Open_Control_Root_586D40(&hKey);

    DWORD cbData = sizeof(DWORD);
    char_type ValueName[4];
    sprintf(ValueName, "%d", static_cast<BYTE>(a1));

    if (RegQueryValueExA(hKey, ValueName, 0, 0, reinterpret_cast<BYTE*>(&Data), &cbData) != ERROR_SUCCESS)
    {
        if (RegSetValueExA(hKey, ValueName, 0, REG_DWORD, reinterpret_cast<const BYTE*>(&a2), sizeof(DWORD)) != ERROR_SUCCESS)
        {
            FatalError_4A38C0(46, "C:\\Splitting\\Gta2\\Source\\registry.cpp", 667);
        }
        Data = a2;
    }

    if (RegCloseKey(hKey) != ERROR_SUCCESS)
    {
        FatalError_4A38C0(42, "C:\\Splitting\\Gta2\\Source\\registry.cpp", 674);
    }

    return Data;
}

// FUNCTION: 105 0x5870D0
s32 Registry::Get_Screen_Setting_5870D0(const char_type* lpValueName, s32 a2)
{
    HKEY hKey;
    Open_Screen_Root_586DB0(&hKey);

    DWORD Data;
    DWORD cbData = sizeof(DWORD);
    if (RegQueryValueExA(hKey, lpValueName, 0, 0, reinterpret_cast<BYTE*>(&Data), &cbData) != ERROR_SUCCESS)
    {
        if (RegSetValueExA(hKey, lpValueName, 0, REG_DWORD, reinterpret_cast<const BYTE*>(&a2), sizeof(DWORD)) != ERROR_SUCCESS)
        {
            FatalError_4A38C0(46, "C:\\Splitting\\Gta2\\Source\\registry.cpp", 709);
        }
        Data = a2;
    }

    if (RegCloseKey(hKey) != ERROR_SUCCESS)
    {
        FatalError_4A38C0(42, "C:\\Splitting\\Gta2\\Source\\registry.cpp", 716);
    }

    return Data;
}

// FUNCTION: 105 0x587290
void Registry::sub_587290()
{
}

// FUNCTION: 105 0x5872A0
char_type Registry::sub_5872A0(HKEY hKey, const char_type* a2, BYTE* lpData, u32 Data)
{
    char_type Buffer[260];
    char_type ret = 0;

    sprintf(Buffer, "%ss", a2);
    if (RegSetValueExA(hKey, Buffer, 0, REG_DWORD, reinterpret_cast<const BYTE*>(&Data), sizeof(DWORD)) == ERROR_SUCCESS)
    {
        sprintf(Buffer, "%sd", a2);
        if (RegSetValueExA(hKey, Buffer, 0, REG_BINARY, lpData, Data) == ERROR_SUCCESS)
        {
            ret = 1;
        }
    }
    return ret;
}

// FUNCTION: 105 0x587340
bool Registry::sub_587340(HKEY hKey, const char_type* keyPath, s32 value, LPBYTE lpData)
{
    char_type ValueName[260]; // [esp+14h] [ebp-104h] BYREF
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

// FUNCTION: 105 0x5873E0
s32 Registry::Get_Int_5873E0(HKEY hKey, const char_type* subKey)
{
    char_type keyPath[260];
    sprintf(keyPath, "%ss", subKey);
    return Get_Int_Setting_5874E0(hKey, keyPath);
}

// FUNCTION: 105 0x587420
char_type Registry::CreateNetworkRoot_587420(PHKEY phkResult)
{
    DWORD dwDisposition;
    char_type SubKey[260];

    strcpy(SubKey, "SOFTWARE\\DMA Design Ltd\\GTA2");
    strcat(SubKey, "\\Network");

    if (RegCreateKeyExA(dword_625018, SubKey, 0, "", 0, KEY_ALL_ACCESS, 0, phkResult, &dwDisposition) != ERROR_SUCCESS)
    {
        FatalError_4A38C0(43, "C:\\Splitting\\Gta2\\Source\\registry.cpp", 910);
    }

    return 1;
}

// STUB: 105 0x5874E0
DWORD Registry::Get_Int_Setting_5874E0(HKEY hKey, const char_type* lpValueName)
{
    NOT_IMPLEMENTED;
    u32 valueData;

    DWORD cbData = sizeof(DWORD);
    DWORD dataType;
    u32 ret;
    u32 err = RegQueryValueExA(hKey, lpValueName, 0, &dataType, reinterpret_cast<BYTE*>(&valueData), &cbData);
    if (err == ERROR_SUCCESS)
    {
        // the f64 return is removed without the printf, ran out of ideas to try
        printf("meh");
        return (int)valueData;
    }
    else
    {
        ret = 0;
        err = 0;
        valueData = 99;
        cbData = 0;
        return ret;
    }
}

// FUNCTION: 105 0x587690
s32 Registry::Set_Network_Setting_587690(const char_type* lpValueName, s32 a2)
{
    HKEY hKey;
    CreateNetworkRoot_587420(&hKey);

    u32 Data;
    DWORD cbData = sizeof(DWORD);
    if (RegQueryValueExA(hKey, lpValueName, 0, 0, reinterpret_cast<BYTE*>(&Data), &cbData) != ERROR_SUCCESS)
    {
        if (RegSetValueExA(hKey, lpValueName, 0, REG_DWORD, reinterpret_cast<const BYTE*>(&a2), sizeof(DWORD)) != ERROR_SUCCESS)
        {
            FatalError_4A38C0(46, "C:\\Splitting\\Gta2\\Source\\registry.cpp", 1051);
        }
        Data = a2;
    }

    if (RegCloseKey(hKey) != ERROR_SUCCESS)
    {
        FatalError_4A38C0(42, "C:\\Splitting\\Gta2\\Source\\registry.cpp", 1058);
    }

    return Data;
}

// FUNCTION: 105 0x587730
void Registry::Set_Network_Setting_587730(const char_type* lpValueName, BYTE Data)
{
    HKEY hKey;
    if (CreateNetworkRoot_587420(&hKey))
    {
        if (RegSetValueExA(hKey, lpValueName, 0, REG_DWORD, &Data, sizeof(DWORD)))
        {
            FatalError_4A38C0(46, "C:\\Splitting\\Gta2\\Source\\registry.cpp", 1080);
        }
    }

    if (RegCloseKey(hKey) != ERROR_SUCCESS)
    {
        FatalError_4A38C0(42, "C:\\Splitting\\Gta2\\Source\\registry.cpp", 1086);
    }
}

// FUNCTION: 105 0x5877A0
char_type Registry::Open_Player_Root_5877A0(PHKEY phkResult)
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
            FatalError_4A38C0(43, "C:\\Splitting\\Gta2\\Source\\registry.cpp", 1110);
        }
        return 0;
    }
    return 1;
}

// FUNCTION: 105 0x587810
DWORD Registry::Create_Player_Setting_587810(const char_type* lpValueName)
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
            FatalError_4A38C0(46, "C:\\Splitting\\Gta2\\Source\\registry.cpp", 1143);
        }
    }

    if (RegCloseKey(hKey) != ERROR_SUCCESS)
    {
        FatalError_4A38C0(42, "C:\\Splitting\\Gta2\\Source\\registry.cpp", 1149);
    }

    return Data;
}

// FUNCTION: 105 0x5878C0
void Registry::Set_Player_Setting_5878C0(const char_type* lpValueName, BYTE Data)
{
    HKEY hKey;
    if (Open_Player_Root_5877A0(&hKey))
    {
        if (RegSetValueExA(hKey, lpValueName, 0, REG_DWORD, &Data, sizeof(DWORD)) != ERROR_SUCCESS)
        {
            FatalError_4A38C0(46, "C:\\Splitting\\Gta2\\Source\\registry.cpp", 1171);
        }
    }

    if (RegCloseKey(hKey) != ERROR_SUCCESS)
    {
        FatalError_4A38C0(42, "C:\\Splitting\\Gta2\\Source\\registry.cpp", 1177);
    }
}

// FUNCTION: 105 0x587170
void Registry::Set_Screen_Setting_587170(const char_type* lpValueName, s32 Data)
{
    HKEY hKey;
    Open_Screen_Root_586DB0(&hKey);

    if (RegSetValueExA(hKey, lpValueName, 0, REG_DWORD, reinterpret_cast<BYTE*>(&Data), sizeof(DWORD)) != ERROR_SUCCESS)
    {
        FatalError_4A38C0(46, "C:\\Splitting\\Gta2\\Source\\registry.cpp", 743);
    }

    if (RegCloseKey(hKey) != ERROR_SUCCESS)
    {
        FatalError_4A38C0(42, "C:\\Splitting\\Gta2\\Source\\registry.cpp", 745);
    }
}

// FUNCTION: 105 0x5871E0
void Registry::Set_Screen_Setting_5871E0(const char_type* lpValueName, BYTE* lpData, u16 maxLen)
{
    HKEY hKey; // [esp+8h] [ebp-8h] BYREF
    DWORD cbData; // [esp+Ch] [ebp-4h] BYREF

    Open_Screen_Root_586DB0(&hKey);

    cbData = maxLen;
    if (RegQueryValueExA(hKey, lpValueName, 0, 0, lpData, &cbData) &&
        RegSetValueExA(hKey, lpValueName, 0, 1u, lpData, strlen((const char_type*)lpData) + 1))
    {
        FatalError_4A38C0(46, "C:\\Splitting\\Gta2\\Source\\registry.cpp", 781);
    }

    if (RegCloseKey(hKey))
    {
        FatalError_4A38C0(42, "C:\\Splitting\\Gta2\\Source\\registry.cpp", 787);
    }
}