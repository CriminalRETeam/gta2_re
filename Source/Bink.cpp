#include "Bink.hpp"

DEFINE_GLOBAL(BINK*, gBinkHandle2_6F83B0, 0x6F83B0);
DEFINE_GLOBAL(BINK*, gBinkHandle1_6F8168, 0x6F8168);
DEFINE_GLOBAL(BINKBUFFER*, gBinkBuffer2_6F80C4, 0x6F80C4);
DEFINE_GLOBAL(BINKBUFFER*, gBinkBuffer1_6F8170, 0x6F8170);
DEFINE_GLOBAL(char_type, gBinkDDState_6F83FE, 0x6F83FE);
DEFINE_GLOBAL(char_type, gBinkActiveSlot_6F83FF, 0x6F83FF);
DEFINE_GLOBAL(s32, gBinkSummary_6F8250, 0x6F8250);
DEFINE_GLOBAL(s32, gBinkSurfaceType_6F81B0, 0x6F81B0);

MATCH_FUNC(0x513210)
void __stdcall Bink::Reset_513210()
{
    gBinkHandle2_6F83B0 = 0;
    gBinkHandle1_6F8168 = 0;
    gBinkBuffer2_6F80C4 = 0;
    gBinkBuffer1_6F8170 = 0;
    gBinkDDState_6F83FE = 0;
    gBinkActiveSlot_6F83FF = 0;
}

MATCH_FUNC(0x513340)
void Bink::Close1_513340()
{
    if (gBinkBuffer1_6F8170 != 0)
    {
        BinkBufferClose(gBinkBuffer1_6F8170);
        gBinkBuffer1_6F8170 = 0;
        gBinkDDState_6F83FE = 0;
        gBinkActiveSlot_6F83FF = 0;
    }

    if (gBinkHandle1_6F8168 != 0)
    {
        BinkGetSummary(gBinkHandle1_6F8168, &gBinkSummary_6F8250);
        BinkClose(gBinkHandle1_6F8168);
        gBinkHandle1_6F8168 = 0;
    }
}

MATCH_FUNC(0x513390)
void Bink::Close2_513390()
{
    if (gBinkBuffer2_6F80C4 != 0)
    {
        BinkBufferClose(gBinkBuffer2_6F80C4);
        gBinkBuffer2_6F80C4 = 0;
        gBinkDDState_6F83FE = 0;
        gBinkActiveSlot_6F83FF = 0;
    }

    if (gBinkHandle2_6F83B0)
    {
        BinkGetSummary(gBinkHandle2_6F83B0, &gBinkSummary_6F8250);
        BinkClose(gBinkHandle2_6F83B0);
        gBinkHandle2_6F83B0 = 0;
    }
}

MATCH_FUNC(0x5136D0)
void __stdcall Bink::sub_5136D0(s32* pNewWindowX, s32* pNewWindowY)
{
    if (gBinkActiveSlot_6F83FF == 1 && gBinkBuffer1_6F8170 != 0)
    {
        BinkBufferCheckWinPos(gBinkBuffer1_6F8170, pNewWindowX, pNewWindowY);
        return;
    }
    else if (gBinkActiveSlot_6F83FF == 2 && gBinkBuffer2_6F80C4 != 0)
    {
        BinkBufferCheckWinPos(gBinkBuffer2_6F80C4, pNewWindowX, pNewWindowY);
    }
}

MATCH_FUNC(0x513770)
BOOL Bink::sub_513770()
{
    return gBinkDDState_6F83FE == 2 || gBinkDDState_6F83FE == 1;
}

MATCH_FUNC(0x513760)
BOOL Bink::sub_513760()
{
    return gBinkDDState_6F83FE == 2;
}

MATCH_FUNC(0x513720)
void Bink::sub_513720()
{
    if (gBinkActiveSlot_6F83FF == 1 && gBinkBuffer1_6F8170 != 0)
    {
        BinkBufferSetOffset(gBinkBuffer1_6F8170, 0, 0);
    }
    else if (gBinkActiveSlot_6F83FF == 2 && gBinkBuffer2_6F80C4 != 0)
    {
        BinkBufferSetOffset(gBinkBuffer2_6F80C4, 0, 0);
    }
}

WIP_FUNC(0x513240)
char_type Bink::sub_513240()
{
    WIP_IMPLEMENTED;

    BINK* hbink = (gBinkActiveSlot_6F83FF == 1) ? gBinkHandle1_6F8168 : gBinkHandle2_6F83B0;
    BINKBUFFER* hbinkbuffer = (gBinkActiveSlot_6F83FF == 1) ? gBinkBuffer1_6F8170 : gBinkBuffer2_6F80C4;

    if (BinkWait(hbink) != 0)
    {
        return false;
    }

    BinkDoFrame(hbink);

    if (gBufferMode_706B34 == 2)
    {
        // Hardware-accelerated path: blit decoded frame into the locked surface directly.
        sub_5D7D30();
        BinkCopyToBuffer(hbink,
                         gVidSys_7071D0->field_50_surface_pixels_ptr,
                         gVidSys_7071D0->field_54_surface_pixels_pitch,
                         hbink->height,
                         0,
                         0,
                         gBinkSurfaceType_6F81B0 | 0x4000000);
        FreeSurface_5D7DC0();
        pVid_FlipBuffers(gVidSys_7071D0);
        pVid_ClearScreen(gVidSys_7071D0, 0, 0, 0, 0, 0, gVidSys_7071D0->field_74, gVidSys_7071D0->field_4C_rect_bottom);
    }
    else
    {
        // Software / DirectDraw path: copy into the locked BinkBuffer, then blit.
        if (BinkBufferLock(hbinkbuffer) != 0)
        {
            BinkCopyToBuffer(hbink,
                             hbinkbuffer->pixels,
                             hbinkbuffer->pitch,
                             hbinkbuffer->height,
                             0,
                             0,
                             hbinkbuffer->surface_type | 0x4000000);
            BinkBufferUnlock(hbinkbuffer);
        }
        BinkBufferBlit(hbinkbuffer, &hbink->rects, 1);
    }

    if (hbink->current_frame == hbink->total_frames)
    {
        return true;
    }

    BinkNextFrame(hbink);
    return false;
}

MATCH_FUNC(0x513790)
char_type Bink::sub_513790()
{
    return gBinkActiveSlot_6F83FF;
}

MATCH_FUNC(0x5137A0)
void __stdcall Bink::sub_5137A0(char_type a1)
{
    gBinkActiveSlot_6F83FF = a1;
}

WIP_FUNC(0x5133E0)
void __stdcall Bink::sub_5133E0(const char_type* a1, HDIGDRIVER a2)
{
    WIP_IMPLEMENTED;

    BinkSetSoundSystem((void*)BinkOpenMiles, (s32)a2);
    BinkSetIOSize(600000);

    gBinkHandle2_6F83B0 = BinkOpen(a1, 0x4000000);

    if (gBinkHandle2_6F83B0 == 0)
    {
        FatalError_4A38C0(Gta2Error::BinkOpenError, "C:\\Splitting\\Gta2\\Source\\movie2.", 376);
        return;
    }

    if (gBufferMode_706B34 == 2)
    {
        // Hardware-accelerated path: pick Bink colour format based on surface bit depth.
        if (gVidSys_7071D0->field_5C == 5)
        {
            if (gVidSys_7071D0->field_64_r == 5 && gVidSys_7071D0->field_6C == 5)
            {
                gBinkSurfaceType_6F81B0 = 2;
                gBinkActiveSlot_6F83FF = 2;
                return;
            }
        }
        else if (gVidSys_7071D0->field_5C == 6)
        {
            if (gVidSys_7071D0->field_64_r == 5 && gVidSys_7071D0->field_6C == 5)
            {
                gBinkSurfaceType_6F81B0 = 4;
                gBinkActiveSlot_6F83FF = 2;
                return;
            }
            if (gVidSys_7071D0->field_64_r == 6 && gVidSys_7071D0->field_6C == 4)
            {
                gBinkSurfaceType_6F81B0 = 5;
                gBinkActiveSlot_6F83FF = 2;
                return;
            }
        }

        gBinkSurfaceType_6F81B0 = 3;
        gBinkActiveSlot_6F83FF = 2;
        return;
    }

    // Software / DirectDraw path.
    if (gBinkDDState_6F83FE == 0)
    {
        BinkBufferSetDDPrimary(gVidSys_7071D0->field_134_SurfacePrimary);
    }

    gBinkDDState_6F83FE = 1;
    gBinkBuffer2_6F80C4 = BinkBufferOpen(gHwnd_707F04, gBinkHandle2_6F83B0->width, gBinkHandle2_6F83B0->height, 0);

    if (gBinkBuffer2_6F80C4 == 0)
    {
        gBinkDDState_6F83FE = 0;
        FatalError_4A38C0(Gta2Error::BinkBufferOpenError, "C:\\Splitting\\Gta2\\Source\\movie2.", 360);
        gBinkActiveSlot_6F83FF = 2;
        return;
    }

    gBinkDDState_6F83FE = 2;
    gBinkActiveSlot_6F83FF = 2;
}

MATCH_FUNC(0x5137B0)
void __stdcall Bink::sub_5137B0(char_type a1)
{
    gBinkDDState_6F83FE = a1;
}

STUB_FUNC(0x513560)
void __stdcall Bink::sub_513560(const char_type* pFileName, HDIGDRIVER a2)
{
    NOT_IMPLEMENTED;
    // todo
}