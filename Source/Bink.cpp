#include "Bink.hpp"

DEFINE_GLOBAL(BINK*, gBinkHandleSlot2_6F83B0, 0x6F83B0);
DEFINE_GLOBAL(BINK*, gBinkHandleSlot1_6F8168, 0x6F8168);
DEFINE_GLOBAL(BINKBUFFER*, gBinkBufferSlot2_6F80C4, 0x6F80C4);
DEFINE_GLOBAL(BINKBUFFER*, gBinkBufferSlot1_6F8170, 0x6F8170);
DEFINE_GLOBAL(char_type, gBinkDDState_6F83FE, 0x6F83FE);
DEFINE_GLOBAL(char_type, gBinkActiveSlot_6F83FF, 0x6F83FF);
DEFINE_GLOBAL(s32, gBinkSummary_6F8250, 0x6F8250);
DEFINE_GLOBAL(s32, gBinkPixelFormat_6F81B0, 0x6F81B0);

MATCH_FUNC(0x513210)
void __stdcall Bink::Reset_513210()
{
    gBinkHandleSlot2_6F83B0 = 0;
    gBinkHandleSlot1_6F8168 = 0;
    gBinkBufferSlot2_6F80C4 = 0;
    gBinkBufferSlot1_6F8170 = 0;
    gBinkDDState_6F83FE = 0;
    gBinkActiveSlot_6F83FF = 0;
}

MATCH_FUNC(0x513340)
void Bink::CloseSlot1_513340()
{
    if (gBinkBufferSlot1_6F8170 != 0)
    {
        BinkBufferClose(gBinkBufferSlot1_6F8170);
        gBinkBufferSlot1_6F8170 = 0;
        gBinkDDState_6F83FE = 0;
        gBinkActiveSlot_6F83FF = 0;
    }

    if (gBinkHandleSlot1_6F8168 != 0)
    {
        BinkGetSummary(gBinkHandleSlot1_6F8168, &gBinkSummary_6F8250);
        BinkClose(gBinkHandleSlot1_6F8168);
        gBinkHandleSlot1_6F8168 = 0;
    }
}

MATCH_FUNC(0x513390)
void Bink::CloseSlot2_513390()
{
    if (gBinkBufferSlot2_6F80C4 != 0)
    {
        BinkBufferClose(gBinkBufferSlot2_6F80C4);
        gBinkBufferSlot2_6F80C4 = 0;
        gBinkDDState_6F83FE = 0;
        gBinkActiveSlot_6F83FF = 0;
    }

    if (gBinkHandleSlot2_6F83B0)
    {
        BinkGetSummary(gBinkHandleSlot2_6F83B0, &gBinkSummary_6F8250);
        BinkClose(gBinkHandleSlot2_6F83B0);
        gBinkHandleSlot2_6F83B0 = 0;
    }
}

MATCH_FUNC(0x5136D0)
void __stdcall Bink::CheckWindowPos_5136D0(s32* pNewWindowX, s32* pNewWindowY)
{
    if (gBinkActiveSlot_6F83FF == 1 && gBinkBufferSlot1_6F8170 != 0)
    {
        BinkBufferCheckWinPos(gBinkBufferSlot1_6F8170, pNewWindowX, pNewWindowY);
        return;
    }
    else if (gBinkActiveSlot_6F83FF == 2 && gBinkBufferSlot2_6F80C4 != 0)
    {
        BinkBufferCheckWinPos(gBinkBufferSlot2_6F80C4, pNewWindowX, pNewWindowY);
    }
}

MATCH_FUNC(0x513770)
BOOL Bink::IsUsingDDBuffer_513770()
{
    return gBinkDDState_6F83FE == 2 || gBinkDDState_6F83FE == 1;
}

MATCH_FUNC(0x513760)
BOOL Bink::IsDDBufferOpen_513760()
{
    return gBinkDDState_6F83FE == 2;
}

MATCH_FUNC(0x513720)
void Bink::ResetBufferOffset_513720()
{
    if (gBinkActiveSlot_6F83FF == 1 && gBinkBufferSlot1_6F8170 != 0)
    {
        BinkBufferSetOffset(gBinkBufferSlot1_6F8170, 0, 0);
    }
    else if (gBinkActiveSlot_6F83FF == 2 && gBinkBufferSlot2_6F80C4 != 0)
    {
        BinkBufferSetOffset(gBinkBufferSlot2_6F80C4, 0, 0);
    }
}

WIP_FUNC(0x513240)
char_type Bink::TickFrame_513240()
{
    WIP_IMPLEMENTED;

    BINK* hbink = (gBinkActiveSlot_6F83FF == 1) ? gBinkHandleSlot1_6F8168 : gBinkHandleSlot2_6F83B0;
    BINKBUFFER* hbinkbuffer = (gBinkActiveSlot_6F83FF == 1) ? gBinkBufferSlot1_6F8170 : gBinkBufferSlot2_6F80C4;

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
                         gBinkPixelFormat_6F81B0 | 0x4000000);
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
char_type Bink::GetActiveSlot_513790()
{
    return gBinkActiveSlot_6F83FF;
}

MATCH_FUNC(0x5137A0)
void __stdcall Bink::SetActiveSlot_5137A0(char_type slot)
{
    gBinkActiveSlot_6F83FF = slot;
}

WIP_FUNC(0x5133E0)
void __stdcall Bink::OpenSlot2_5133E0(const char_type* pFileName, HDIGDRIVER a2)
{
    WIP_IMPLEMENTED;

    BinkSetSoundSystem((void*)BinkOpenMiles, (s32)a2);
    BinkSetIOSize(600000);

    gBinkHandleSlot2_6F83B0 = BinkOpen(pFileName, 0x4000000);

    if (gBinkHandleSlot2_6F83B0 == 0)
    {
        FatalError_4A38C0(Gta2Error::BinkOpenError, "C:\\Splitting\\Gta2\\Source\\movie2.cpp", 376);
        return;
    }

    if (gBufferMode_706B34 == 2)
    {
        // Hardware-accelerated path: pick Bink colour format based on surface bit depth.
        if (gVidSys_7071D0->field_5C == 5)
        {
            if (gVidSys_7071D0->field_64_r == 5 && gVidSys_7071D0->field_6C == 5)
            {
                gBinkPixelFormat_6F81B0 = 2;
                gBinkActiveSlot_6F83FF = 2;
                return;
            }
        }
        else if (gVidSys_7071D0->field_5C == 6)
        {
            if (gVidSys_7071D0->field_64_r == 5 && gVidSys_7071D0->field_6C == 5)
            {
                gBinkPixelFormat_6F81B0 = 4;
                gBinkActiveSlot_6F83FF = 2;
                return;
            }
            if (gVidSys_7071D0->field_64_r == 6 && gVidSys_7071D0->field_6C == 4)
            {
                gBinkPixelFormat_6F81B0 = 5;
                gBinkActiveSlot_6F83FF = 2;
                return;
            }
        }

        gBinkPixelFormat_6F81B0 = 3;
        gBinkActiveSlot_6F83FF = 2;
        return;
    }

    // Software / DirectDraw path.
    if (gBinkDDState_6F83FE == 0)
    {
        BinkBufferSetDDPrimary(gVidSys_7071D0->field_134_SurfacePrimary);
    }

    gBinkDDState_6F83FE = 1;
    gBinkBufferSlot2_6F80C4 = BinkBufferOpen(gHwnd_707F04, gBinkHandleSlot2_6F83B0->width, gBinkHandleSlot2_6F83B0->height, 0);

    if (gBinkBufferSlot2_6F80C4 == 0)
    {
        gBinkDDState_6F83FE = 0;
        FatalError_4A38C0(Gta2Error::BinkBufferOpenError, "C:\\Splitting\\Gta2\\Source\\movie2.cpp", 360);
        gBinkActiveSlot_6F83FF = 2;
        return;
    }

    gBinkDDState_6F83FE = 2;
    gBinkActiveSlot_6F83FF = 2;
}

MATCH_FUNC(0x5137B0)
void __stdcall Bink::SetDDState_5137B0(char_type state)
{
    gBinkDDState_6F83FE = state;
}

WIP_FUNC(0x513560)
void __stdcall Bink::OpenSlot1_513560(const char_type* pFileName, HDIGDRIVER a2)
{
    WIP_IMPLEMENTED;
    
    BinkSetSoundSystem((void*)BinkOpenMiles, (s32)a2);
    BinkSetIOSize(600000);

    gBinkHandleSlot1_6F8168 = BinkOpen(pFileName, 0x4000000);

    if (gBinkHandleSlot1_6F8168 == 0)
    {
        FatalError_4A38C0(Gta2Error::BinkOpenError, "C:\\Splitting\\Gta2\\Source\\movie2.cpp", 460);
        return;
    }

    if (gBufferMode_706B34 == 2)
    {
        if (gVidSys_7071D0->field_5C == 5)
        {
            if (gVidSys_7071D0->field_64_r == 5 && gVidSys_7071D0->field_6C == 5)
            {
                gBinkPixelFormat_6F81B0 = 2;
                gBinkActiveSlot_6F83FF = 1;
                return;
            }
        }
        else if (gVidSys_7071D0->field_5C == 6)
        {
            if (gVidSys_7071D0->field_64_r == 5 && gVidSys_7071D0->field_6C == 5)
            {
                gBinkPixelFormat_6F81B0 = 4;
                gBinkActiveSlot_6F83FF = 1;
                return;
            }
            if (gVidSys_7071D0->field_64_r == 6 && gVidSys_7071D0->field_6C == 4)
            {
                gBinkPixelFormat_6F81B0 = 5;
                gBinkActiveSlot_6F83FF = 1;
                return;
            }
        }

        gBinkPixelFormat_6F81B0 = 3;
        gBinkActiveSlot_6F83FF = 1;
        return;
    }

    BinkBufferSetDDPrimary(gVidSys_7071D0->field_134_SurfacePrimary);

    gBinkDDState_6F83FE = 1;
    gBinkBufferSlot1_6F8170 = BinkBufferOpen(gHwnd_707F04, gBinkHandleSlot1_6F8168->width, gBinkHandleSlot1_6F8168->height, 0);

    if (gBinkBufferSlot1_6F8170 == 0)
    {
        gBinkDDState_6F83FE = 0;
        FatalError_4A38C0(Gta2Error::BinkBufferOpenError, "C:\\Splitting\\Gta2\\Source\\movie2.cpp", 450);
        gBinkActiveSlot_6F83FF = 1;
        return;
    }

    gBinkDDState_6F83FE = 2;
    gBinkActiveSlot_6F83FF = 1;
}