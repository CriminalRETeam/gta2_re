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

EXTERN_GLOBAL(BINK*, gBinkHandleSlot2);
EXTERN_GLOBAL(BINK*, gBinkHandleSlot1);
EXTERN_GLOBAL(BINKBUFFER*, gBinkBufferSlot2);
EXTERN_GLOBAL(BINKBUFFER*, gBinkBufferSlot1);
EXTERN_GLOBAL(char_type, gBinkDDState);
EXTERN_GLOBAL(char_type, gBinkActiveSlot);
EXTERN_GLOBAL_ARRAY(u32, gBinkSummary, 30);
EXTERN_GLOBAL(s32, gBinkPixelFormat);

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