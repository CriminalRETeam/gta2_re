#pragma once

#include "Function.hpp"
#include <windows.h>

class Registry
{
  public:
    // todo: fix ordering

    EXPORT bool Open_Sound_Root_586A00(PHKEY phkResult);

    EXPORT bool Get_Sound_Settting_586A70(const char_type* lpValueName);

    EXPORT s32 Set_Sound_Setting_586AE0(const char_type* lpValueName, s32 value);

    EXPORT void Set_Sound_Setting_586B80(const char_type* lpValueName, s32 Data);

    EXPORT void Clear_Or_Delete_Sound_Setting_586BF0(const char_type* lpValueName, char_type bClear);

    EXPORT char_type Open_Debug_Root_586C60(PHKEY phkResult);

    EXPORT char_type Open_Options_Root_586CD0(PHKEY phkResult);

    EXPORT char_type Open_Control_Root_586D40(PHKEY phkResult);

    EXPORT char_type Open_Screen_Root_586DB0(PHKEY phkResult);

    EXPORT bool Get_Debug_Setting_586E20(const char_type* lpValueName);

    EXPORT void Get_Debug_Setting_586E90(const char_type* lpValueName, LPBYTE lpData, s32 a3);

    EXPORT void Get_Option_586F00(const char_type* lpValueName, BYTE* pBuffer, s32 bufferSize);

    EXPORT s32 Set_Option_586F70(const char_type* lpValueName, s32 a3);

    EXPORT s32 Set_Control_Setting_587010(s32 a1, s32 a2);

    EXPORT s32 Get_Screen_Setting_5870D0(const char_type* lpValueName, s32 a2);

    EXPORT void sub_587290();

    EXPORT char_type sub_5872A0(HKEY hKey, const char_type* a2, BYTE* lpData, u32 Data);

    EXPORT bool sub_587340(HKEY hKey, const char_type* keyPath, s32 value, LPBYTE lpData);

    EXPORT s32 Get_Int_5873E0(HKEY hKey, const char_type* subKey);

    EXPORT char_type CreateNetworkRoot_587420(PHKEY phkResult);

    EXPORT DWORD Get_Int_Setting_5874E0(HKEY hKey, const char_type* lpValueName);

    EXPORT s32 Set_Network_Setting_587690(const char_type* lpValueName, s32 a2);

    EXPORT void Set_Network_Setting_587730(const char_type* lpValueName, BYTE Data);

    EXPORT char_type Open_Player_Root_5877A0(PHKEY phkResult);

    EXPORT DWORD Create_Player_Setting_587810(const char_type* lpValueName);

    EXPORT void Set_Player_Setting_5878C0(const char_type* lpValueName, BYTE Data);

    EXPORT void Set_Screen_Setting_587170(const char_type* lpValueName, s32 Data);

    EXPORT void Set_Screen_Setting_5871E0(const char_type* lpValueName, BYTE* lpData, u16 maxLen);
};

EXPORT_VAR extern Registry gRegistry_6FF968;
