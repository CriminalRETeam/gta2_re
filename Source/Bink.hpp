#pragma once

#include "3rdParty/bink.h"
#include "3rdParty/mss.h"
#include "Draw.hpp"
#include "Frontend.hpp"
#include "Function.hpp"
#include "dma_video.hpp"
#include "enums.hpp"
#include "error.hpp"
#include "winmain.hpp"
#include <windows.h>

EXTERN_GLOBAL(BINK*, gBinkHandleSlot2_6F83B0);
EXTERN_GLOBAL(BINK*, gBinkHandleSlot1_6F8168);
EXTERN_GLOBAL(BINKBUFFER*, gBinkBufferSlot2_6F80C4);
EXTERN_GLOBAL(BINKBUFFER*, gBinkBufferSlot1_6F8170);
EXTERN_GLOBAL(char_type, gBinkDDState_6F83FE);
EXTERN_GLOBAL(char_type, gBinkActiveSlot_6F83FF);
EXTERN_GLOBAL_ARRAY(u32, gBinkSummary_6F8250, 30);
EXTERN_GLOBAL(s32, gBinkPixelFormat_6F81B0);

class Bink
{
  public:
    EXPORT static void __stdcall Reset();

    EXPORT static void CloseSlot1();

    EXPORT static void CloseSlot2();

    EXPORT static void __stdcall CheckWindowPos(s32* pNewWindowX, s32* pNewWindowY);

    EXPORT static bool IsUsingDDBuffer();

    EXPORT static bool IsDDBufferOpen();

    EXPORT static void ResetBufferOffset();

    EXPORT static bool TickFrame();

    EXPORT static char_type GetActiveSlot();

    EXPORT static void __stdcall SetActiveSlot(char_type slot);

    EXPORT static void __stdcall OpenSlot2(const char_type* pFileName, HDIGDRIVER hDriver);

    EXPORT static void __stdcall SetDDState(char_type state);

    EXPORT static void __stdcall OpenSlot1(const char_type* pFileName, HDIGDRIVER hDriver);
};