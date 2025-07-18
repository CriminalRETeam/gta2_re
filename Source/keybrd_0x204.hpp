#pragma once

#include "Function.hpp"
#include <stdio.h>

class keybrd_0x204
{
  public:
    EXPORT keybrd_0x204();

    EXPORT ~keybrd_0x204();

    EXPORT void ReadCfg_4D5DA0(FILE* Stream, wchar_t* pOut);

    EXPORT void LoadKbCfg_4D5E00();

    EXPORT wchar_t GetKey_4D5F40(u16 a1);

    EXPORT static void __stdcall create_4D5F50();

    EXPORT static void destroy_4D5FA0();

    EXPORT static void RecreateIfLayoutChanged_4D5FD0();

    EXPORT static s32 GetLayout_4D6000();

    s16 field_0_keys[256];
    s32 field_200_keyBoardLayout;
};

EXTERN_GLOBAL(keybrd_0x204*, gKeybrd_0x204_6F52F4);