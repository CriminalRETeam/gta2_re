#pragma once

#include "Function.hpp"
#include <windows.h>

class Registry
{
  public:
    EXPORT bool Open_Sound_Root(PHKEY phkResult);

    EXPORT bool Sound_Setting_Exists(const char_type* lpValueName);

    EXPORT s32 Get_Sound_Setting(const char_type* lpValueName, s32 value);

    EXPORT void Set_Sound_Setting(const char_type* lpValueName, s32 Data);

    EXPORT void Clear_Or_Delete_Sound_Setting(const char_type* lpValueName, char_type bClear);

    EXPORT bool Open_Debug_Root(PHKEY phkResult);

    EXPORT bool Open_Options_Root(PHKEY phkResult);

    EXPORT bool Open_Control_Root(PHKEY phkResult);

    EXPORT bool Open_Screen_Root(PHKEY phkResult);

    EXPORT bool Debug_Setting_Exists(const char_type* lpValueName);

    EXPORT void Get_Debug_Setting(const char_type* lpValueName, LPBYTE lpData, s32 data_size);

    EXPORT void Get_Option(const char_type* lpValueName, BYTE* pBuffer, s32 bufferSize);

    EXPORT s32 Get_Option_Int(const char_type* lpValueName, s32 value);

    EXPORT s32 Get_Control_Setting(s32 control_idx, s32 value);

    EXPORT s32 Get_Screen_Setting(const char_type* lpValueName, s32 default_value);

    EXPORT void dtor();

    EXPORT bool Set_Binary(HKEY hKey, const char_type* base_name, const BYTE* lpData, u32 size);

    EXPORT bool Get_Binary(HKEY hKey, const char_type* keyPath, s32 expectedSize, LPBYTE lpData);

    EXPORT s32 Get_Binary_Size(HKEY hKey, const char_type* subKey);

    EXPORT bool CreateNetworkRoot(PHKEY phkResult);

    EXPORT DWORD Get_Int_Setting(HKEY hKey, const char_type* lpValueName);

    EXPORT static s32 __stdcall Get_Binary_Value(HKEY hKey, const char_type* keyPath, LPBYTE lpData);

    EXPORT static s32 __stdcall Set_WString_Value(HKEY hKey, const char_type* keyPath, const wchar_t* pStr);

    EXPORT bool Network_Setting_Exists(const char_type* lpValueName);

    EXPORT s32 Get_Network_Setting(const char_type* lpValueName, s32 value);

    EXPORT void Set_Network_Setting(const char_type* lpValueName, BYTE Data);

    EXPORT bool Open_Player_Root(PHKEY phkResult);

    EXPORT DWORD Get_Player_Setting(const char_type* lpValueName);

    EXPORT void Set_Player_Setting(const char_type* lpValueName, DWORD Data);

    EXPORT void Set_Screen_Setting(const char_type* lpValueName, s32 Data);

    EXPORT void Get_Screen_String(const char_type* lpValueName, BYTE* lpData, u16 maxLen);
};

EXTERN_GLOBAL(Registry, gRegistry);
