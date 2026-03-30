#pragma once

#include "3rdParty/bink.h"
#include "3rdParty/mss.h"
#include "Function.hpp"
#include "dma_video.hpp"
#include "enums.hpp"
#include "error.hpp"
#include "winmain.hpp"
#include <windows.h>

EXTERN_GLOBAL(s32, gBinkHandle2_6F83B0);
EXTERN_GLOBAL(s32, gBinkHandle1_6F8168);
EXTERN_GLOBAL(s32, gBinkBuffer2_6F80C4);
EXTERN_GLOBAL(s32, gBinkBuffer1_6F8170);
EXTERN_GLOBAL(char_type, gBinkDDState_6F83FE);
EXTERN_GLOBAL(char_type, gBinkActiveSlot_6F83FF);
EXTERN_GLOBAL(s32, gBinkSummary_6F8250);
EXTERN_GLOBAL(s32, gBinkSurfaceType_6F81B0);

class Bink
{
  public:
    EXPORT static void __stdcall Reset_513210();

    EXPORT static void Close1_513340();

    EXPORT static void Close2_513390();

    EXPORT static void __stdcall sub_5136D0(s32* pNewWindowX, s32* pNewWindowY);

    EXPORT static BOOL sub_513770();

    EXPORT static BOOL sub_513760();

    EXPORT static void sub_513720();

    EXPORT static char_type sub_513240();

    EXPORT static char_type sub_513790();

    EXPORT static void __stdcall sub_5137A0(char_type a1);

    EXPORT static void __stdcall sub_5133E0(const char_type* a1, HDIGDRIVER a2);

    EXPORT static void __stdcall sub_5137B0(char_type a1);

    EXPORT static void __stdcall sub_513560(const char_type* pFileName, HDIGDRIVER a2);
};