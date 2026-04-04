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
EXTERN_GLOBAL(s32, gBinkSummary_6F8250);
EXTERN_GLOBAL(s32, gBinkPixelFormat_6F81B0);

class Bink
{
  public:
    EXPORT static void __stdcall Reset_513210();

    EXPORT static void CloseSlot1_513340();

    EXPORT static void CloseSlot2_513390();

    EXPORT static void __stdcall CheckWindowPos_5136D0(s32* pNewWindowX, s32* pNewWindowY);

    EXPORT static BOOL IsUsingDDBuffer_513770();

    EXPORT static BOOL IsDDBufferOpen_513760();

    EXPORT static void ResetBufferOffset_513720();

    EXPORT static char_type TickFrame_513240();

    EXPORT static char_type GetActiveSlot_513790();

    EXPORT static void __stdcall SetActiveSlot_5137A0(char_type slot);

    EXPORT static void __stdcall OpenSlot2_5133E0(const char_type* pFileName, HDIGDRIVER a2);

    EXPORT static void __stdcall SetDDState_5137B0(char_type state);

    EXPORT static void __stdcall OpenSlot1_513560(const char_type* pFileName, HDIGDRIVER a2);
};