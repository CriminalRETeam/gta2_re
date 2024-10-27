#pragma once

#include <stdio.h>
#include "types.hpp"

class keybrd_0x204
{
public:
    s16 field_0_keys[256];
    s32 field_200_keyBoardLayout;

    // todo: func order

    EXPORT static void __stdcall create_4D5F50();

    EXPORT static void destroy_4D5FA0();

    EXPORT keybrd_0x204();

    EXPORT ~keybrd_0x204();

    EXPORT void LoadKbCfg_4D5E00();

    EXPORT static s32 GetLayout_4D6000();

    EXPORT void ReadCfg_4D5DA0(FILE *Stream, wchar_t *pOut);

    EXPORT static void RecreateIfLayoutChanged_4D5FD0();
};
