#include "bink.h"

BINKDEC BINK* __stdcall BinkOpen(const char* name, s32 flags)
{
    return 0;
}
BINKDEC void __stdcall BinkClose(BINK* hbink)
{
}
BINKDEC void __stdcall BinkGetSummary(BINK* hbink, void* summary)
{
}
BINKDEC s32 __stdcall BinkDoFrame(BINK* hbink)
{
    return 0;
}
BINKDEC void __stdcall BinkNextFrame(BINK* hbink)
{
}
BINKDEC s32 __stdcall BinkWait(BINK* hbink)
{
    return 0;
}
BINKDEC s32 __stdcall BinkCopyToBuffer(BINK* hbink, void* dest, s32 destpitch, s32 destheight, s32 destx, s32 desty, s32 flags)
{
    return 0;
}
BINKDEC void __stdcall BinkSetSoundSystem(void* open, s32 param)
{
}
BINKDEC void __stdcall BinkSetIOSize(s32 size)
{
}
BINKDEC void* __stdcall BinkOpenMiles(s32 driver)
{
    return 0;
}
BINKDEC s32 __stdcall BinkBufferOpen(HWND hwnd, s32 width, s32 height, s32 flags)
{
    return 0;
}
BINKDEC void __stdcall BinkBufferClose(s32 hbinkbuffer)
{
}
BINKDEC void* __stdcall BinkBufferLock(s32 hbinkbuffer)
{
    return 0;
}
BINKDEC void __stdcall BinkBufferUnlock(s32 hbinkbuffer)
{
}
BINKDEC void __stdcall BinkBufferBlit(s32 hbinkbuffer, void* rects, s32 numrects)
{
}
BINKDEC void __stdcall BinkBufferSetDDPrimary(void* surface)
{
}
BINKDEC void __stdcall BinkBufferCheckWinPos(s32 hbinkbuffer, s32* x, s32* y)
{
}
BINKDEC void __stdcall BinkBufferSetOffset(s32 hbinkbuffer, s32 x, s32 y)
{
}
