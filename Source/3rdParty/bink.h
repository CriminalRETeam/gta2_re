#pragma once

#include "types.hpp"
#include <windows.h>

#ifdef BUILD_BINK
    #define BINKDEC __declspec(dllexport)
#else
    #define BINKDEC __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C"
{
#endif

    BINKDEC s32 __stdcall BinkOpen(const char* name, s32 flags);
    BINKDEC void __stdcall BinkClose(s32 hbink);
    BINKDEC void __stdcall BinkGetSummary(s32 hbink, void* summary);
    BINKDEC s32 __stdcall BinkDoFrame(s32 hbink);
    BINKDEC void __stdcall BinkNextFrame(s32 hbink);
    BINKDEC s32 __stdcall BinkWait(s32 hbink);
    BINKDEC s32 __stdcall BinkCopyToBuffer(s32 hbink, void* dest, s32 destpitch, s32 destheight, s32 destx, s32 desty, s32 flags);
    BINKDEC void __stdcall BinkSetSoundSystem(void* open, s32 param);
    BINKDEC void __stdcall BinkSetIOSize(s32 size);
    BINKDEC void* __stdcall BinkOpenMiles(s32 driver);
    BINKDEC s32 __stdcall BinkBufferOpen(HWND hwnd, s32 width, s32 height, s32 flags);
    BINKDEC void __stdcall BinkBufferClose(s32 hbinkbuffer);
    BINKDEC void* __stdcall BinkBufferLock(s32 hbinkbuffer);
    BINKDEC void __stdcall BinkBufferUnlock(s32 hbinkbuffer);
    BINKDEC void __stdcall BinkBufferBlit(s32 hbinkbuffer, void* rects, s32 numrects);
    BINKDEC void __stdcall BinkBufferSetDDPrimary(s32 surface);
    BINKDEC void __stdcall BinkBufferCheckWinPos(s32 hbinkbuffer, s32* x, s32* y);
    BINKDEC void __stdcall BinkBufferSetOffset(s32 hbinkbuffer, s32 x, s32 y);

#ifdef __cplusplus
}
#endif
