#pragma once

#include "Function.hpp"
#include <windows.h>

class Registry
{
  public:
    EXPORT bool Open_Sound_Root_586A00(PHKEY phkResult);

    EXPORT bool Sound_Setting_Exists_586A70(const char_type* lpValueName);

    EXPORT s32 Get_Sound_Setting_586AE0(const char_type* lpValueName, s32 value);

    EXPORT void Set_Sound_Setting_586B80(const char_type* lpValueName, s32 Data);

    EXPORT void Clear_Or_Delete_Sound_Setting_586BF0(const char_type* lpValueName, char_type bClear);

    EXPORT bool Open_Debug_Root_586C60(PHKEY phkResult);

    EXPORT bool Open_Options_Root_586CD0(PHKEY phkResult);

    EXPORT bool Open_Control_Root_586D40(PHKEY phkResult);

    EXPORT bool Open_Screen_Root_586DB0(PHKEY phkResult);

    EXPORT bool Debug_Setting_Exists_586E20(const char_type* lpValueName);

    EXPORT void Get_Debug_Setting_586E90(const char_type* lpValueName, LPBYTE lpData, s32 data_size);

    EXPORT void Get_Option_586F00(const char_type* lpValueName, BYTE* pBuffer, s32 bufferSize);

    EXPORT s32 Get_Option_Int_586F70(const char_type* lpValueName, s32 value);

    EXPORT s32 Get_Control_Setting_587010(s32 control_idx, s32 value);

    EXPORT s32 Get_Screen_Setting_5870D0(const char_type* lpValueName, s32 default_value);

    EXPORT void null_587290();

    EXPORT bool Set_Binary_5872A0(HKEY hKey, const char_type* base_name, const BYTE* lpData, u32 size);

    EXPORT bool Get_Binary_587340(HKEY hKey, const char_type* keyPath, s32 expectedSize, LPBYTE lpData);

    EXPORT s32 Get_Binary_Size_5873E0(HKEY hKey, const char_type* subKey);

    EXPORT bool CreateNetworkRoot_587420(PHKEY phkResult);

    EXPORT DWORD Get_Int_Setting_5874E0(HKEY hKey, const char_type* lpValueName);

    EXPORT static s32 __stdcall Get_Binary_Value_5875A0(HKEY hKey, const char_type* keyPath, LPBYTE lpData);

    EXPORT static s32 __stdcall Set_WString_Value_5875E0(HKEY hKey, const char_type* keyPath, const wchar_t* pStr);

    EXPORT bool Network_Setting_Exists_587620(const char_type* lpValueName);

    EXPORT s32 Get_Network_Setting_587690(const char_type* lpValueName, s32 value);

    EXPORT void Set_Network_Setting_587730(const char_type* lpValueName, BYTE Data);

    EXPORT bool Open_Player_Root_5877A0(PHKEY phkResult);

    EXPORT DWORD Get_Player_Setting_587810(const char_type* lpValueName);

    EXPORT void Set_Player_Setting_5878C0(const char_type* lpValueName, DWORD Data);

    EXPORT void Set_Screen_Setting_587170(const char_type* lpValueName, s32 Data);

    EXPORT void Get_Screen_String_5871E0(const char_type* lpValueName, BYTE* lpData, u16 maxLen);
};

EXTERN_GLOBAL(Registry, gRegistry_6FF968);
