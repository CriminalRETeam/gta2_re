#pragma once

#include "types.hpp"
#include <windows.h>

#ifdef BUILD_BINK
    #define BINKDEC __declspec(dllexport)
#else
    #define BINKDEC __declspec(dllimport)
#endif

struct BINK
{
    s32 width; // 0x00
    s32 height; // 0x04
    s32 _unk08[2]; // 0x08
    s32 total_frames; // 0x10
    s32 current_frame; // 0x14
    s32 _unk18[9]; // 0x18
    void* rects; // 0x3C
};

struct BINKBUFFER
{
    s32 _unk00; // 0x00
    s32 height; // 0x04
    s32 _unk08[2]; // 0x08
    u32 surface_type; // 0x10
    void* pixels; // 0x14
    s32 pitch; // 0x18
};

#ifdef __cplusplus
extern "C"
{
#endif

    BINKDEC BINK* __stdcall BinkOpen(const char* name, s32 flags);
    BINKDEC void __stdcall BinkClose(BINK* hbink);
    BINKDEC void __stdcall BinkGetSummary(BINK* hbink, void* summary);
    BINKDEC s32 __stdcall BinkDoFrame(BINK* hbink);
    BINKDEC void __stdcall BinkNextFrame(BINK* hbink);
    BINKDEC s32 __stdcall BinkWait(BINK* hbink);
    BINKDEC s32 __stdcall BinkCopyToBuffer(BINK* hbink, void* dest, s32 destpitch, s32 destheight, s32 destx, s32 desty, s32 flags);
    BINKDEC void __stdcall BinkSetSoundSystem(void* open, s32 param);
    BINKDEC void __stdcall BinkSetIOSize(s32 size);
    BINKDEC void* __stdcall BinkOpenMiles(s32 driver);
    BINKDEC BINKBUFFER* __stdcall BinkBufferOpen(HWND hwnd, s32 width, s32 height, s32 flags);
    BINKDEC void __stdcall BinkBufferClose(BINKBUFFER* hbinkbuffer);
    BINKDEC void* __stdcall BinkBufferLock(BINKBUFFER* hbinkbuffer);
    BINKDEC void __stdcall BinkBufferUnlock(BINKBUFFER* hbinkbuffer);
    BINKDEC void __stdcall BinkBufferBlit(BINKBUFFER* hbinkbuffer, void* rects, s32 numrects);
    BINKDEC void __stdcall BinkBufferSetDDPrimary(void* surface);
    BINKDEC void __stdcall BinkBufferCheckWinPos(BINKBUFFER* hbinkbuffer, s32* x, s32* y);
    BINKDEC void __stdcall BinkBufferSetOffset(BINKBUFFER* hbinkbuffer, s32 x, s32 y);

#ifdef __cplusplus
}
#endif
