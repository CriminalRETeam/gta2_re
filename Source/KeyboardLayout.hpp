#pragma once

#include "Function.hpp"
#include <stdio.h>

class KeyboardLayout
{
  public:
    EXPORT KeyboardLayout();

    EXPORT ~KeyboardLayout();

    EXPORT void ReadCfg(FILE* Stream, wchar_t* pOut);

    EXPORT void LoadKbCfg();

    EXPORT wchar_t GetKey(u16 key_idx);

    EXPORT static void __stdcall create();

    EXPORT static void destroy();

    EXPORT static void RecreateIfLayoutChanged();

    EXPORT static s32 __stdcall GetLayout();

    s16 mKeys[256]; // 0x0
    s32 mKeyBoardLayout; // 0x200
};
GTA2_ASSERT_SIZEOF_ALWAYS(KeyboardLayout, 0x204);

EXTERN_GLOBAL(KeyboardLayout*, gKeybrd_0x204_6F52F4);