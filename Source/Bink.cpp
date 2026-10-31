#include "Bink.hpp"

DEFINE_GLOBAL(BINK*, gBinkHandleSlot2, 0x6F83B0);
DEFINE_GLOBAL(BINK*, gBinkHandleSlot1, 0x6F8168);
DEFINE_GLOBAL(BINKBUFFER*, gBinkBufferSlot2, 0x6F80C4);
DEFINE_GLOBAL(BINKBUFFER*, gBinkBufferSlot1, 0x6F8170);
DEFINE_GLOBAL(char_type, gBinkDDState, 0x6F83FE);
DEFINE_GLOBAL(char_type, gBinkActiveSlot, 0x6F83FF);
// BinkGetSummary writes a 0x78-byte summary here (the next global the original uses is at 0x6F82C8)
DEFINE_GLOBAL_ARRAY(u32, gBinkSummary, 30, 0x6F8250);
DEFINE_GLOBAL(s32, gBinkPixelFormat, 0x6F81B0);

enum BinkDDState
{
    BinkDD_Closed = 0,
    BinkDD_Opening = 1,
    BinkDD_Open = 2,
};

// 9.6f 0x481DF0
static inline bool IsDirectBufferMode()
{
    return gBufferMode == 2;
}

MATCH_FUNC(0x513210)
void __stdcall Bink::Reset()
{
    gBinkHandleSlot2 = NULL;
    gBinkHandleSlot1 = NULL;
    gBinkBufferSlot2 = NULL;
    gBinkBufferSlot1 = NULL;
    gBinkDDState = BinkDD_Closed;
    gBinkActiveSlot = 0;
}

MATCH_FUNC(0x513240)
bool Bink::TickFrame()
{
    BINK* hbink;
    BINKBUFFER* hbinkbuffer;
    if (gBinkActiveSlot == 1)
    {
        hbink = gBinkHandleSlot1;
        hbinkbuffer = gBinkBufferSlot1;
    }
    else
    {
        hbink = gBinkHandleSlot2;
        hbinkbuffer = gBinkBufferSlot2;
    }

    if (BinkWait(hbink) != 0)
    {
        return false;
    }

    BinkDoFrame(hbink);

    if (IsDirectBufferMode())
    {
        // Hardware-accelerated path: blit decoded frame into the locked surface directly.
        MakeScreenTableAndSetWindow();
        BinkCopyToBuffer(hbink,
                         gVidSys->field_50_surface_pixels_ptr,
                         gVidSys->field_54_surface_pixels_pitch,
                         hbink->height,
                         0,
                         0,
                         gBinkPixelFormat | 0x4000000);
        FreeSurface();
        pVid_FlipBuffers(gVidSys);
        pVid_ClearScreen(gVidSys, 0, 0, 0, 0, 0, gVidSys->field_48_rect_right, gVidSys->field_4C_rect_bottom);
    }
    else
    {
        // Software / DirectDraw path: copy into the locked BinkBuffer, then blit.
        if (BinkBufferLock(hbinkbuffer) != NULL)
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

MATCH_FUNC(0x513340)
void Bink::CloseSlot1()
{
    if (gBinkBufferSlot1 != NULL)
    {
        BinkBufferClose(gBinkBufferSlot1);
        gBinkBufferSlot1 = NULL;
        gBinkDDState = BinkDD_Closed;
        gBinkActiveSlot = 0;
    }

    if (gBinkHandleSlot1 != NULL)
    {
        BinkGetSummary(gBinkHandleSlot1, &gBinkSummary);
        BinkClose(gBinkHandleSlot1);
        gBinkHandleSlot1 = NULL;
    }
}

MATCH_FUNC(0x513390)
void Bink::CloseSlot2()
{
    if (gBinkBufferSlot2 != NULL)
    {
        BinkBufferClose(gBinkBufferSlot2);
        gBinkBufferSlot2 = NULL;
        gBinkDDState = BinkDD_Closed;
        gBinkActiveSlot = 0;
    }

    if (gBinkHandleSlot2)
    {
        BinkGetSummary(gBinkHandleSlot2, &gBinkSummary);
        BinkClose(gBinkHandleSlot2);
        gBinkHandleSlot2 = NULL;
    }
}

// The csv size (0x166) stops after the last FatalError call: the shared slot store and epilogue
// after it are not compared
MATCH_FUNC(0x5133E0)
void __stdcall Bink::OpenSlot2(const char_type* pFileName, HDIGDRIVER hDriver)
{
    BinkSetSoundSystem(reinterpret_cast<void*>(BinkOpenMiles), reinterpret_cast<s32>(hDriver));
    BinkSetIOSize(600000);

    gBinkHandleSlot2 = BinkOpen(pFileName, 0x4000000);

    if (gBinkHandleSlot2 != NULL)
    {
        if (IsDirectBufferMode())
        {
            // Hardware-accelerated path: pick Bink colour format based on surface bit depth.
            if (gVidSys->field_5C == 5)
            {
                if (gVidSys->field_64_r == 5 && gVidSys->field_6C == 5)
                {
                    gBinkPixelFormat = 2;
                    gBinkActiveSlot = 2;
                    return;
                }
                if (gVidSys->field_64_r == 6 && gVidSys->field_6C == 5)
                {
                    gBinkActiveSlot = 2;
                    gBinkPixelFormat = 3;
                    return;
                }
            }
            else if (gVidSys->field_5C == 6)
            {
                if (gVidSys->field_64_r == 5 && gVidSys->field_6C == 5)
                {
                    gBinkActiveSlot = 2;
                    gBinkPixelFormat = 4;
                    return;
                }
                if (gVidSys->field_64_r == 6 && gVidSys->field_6C == 4)
                {
                    gBinkActiveSlot = 2;
                    gBinkPixelFormat = 5;
                    return;
                }
            }

            gBinkActiveSlot = 2;
            gBinkPixelFormat = 3;
            return;
        }

        // Software / DirectDraw path.
        if (gBinkDDState == BinkDD_Closed)
        {
            BinkBufferSetDDPrimary(gVidSys->field_134_SurfacePrimary);
        }

        gBinkDDState = BinkDD_Opening;
        gBinkBufferSlot2 = BinkBufferOpen(gHwnd, gBinkHandleSlot2->width, gBinkHandleSlot2->height, 0);

        if (gBinkBufferSlot2 == NULL)
        {
            gBinkDDState = BinkDD_Closed;
            FatalError(Gta2Error::BinkBufferOpenError, "C:\\Splitting\\Gta2\\Source\\movie2.cpp", 360);
        }
        else
        {
            gBinkDDState = BinkDD_Open;
        }
    }
    else
    {
        FatalError(Gta2Error::BinkOpenError, "C:\\Splitting\\Gta2\\Source\\movie2.cpp", 376);
    }
    gBinkActiveSlot = 2;
}

MATCH_FUNC(0x513560)
void __stdcall Bink::OpenSlot1(const char_type* pFileName, HDIGDRIVER hDriver)
{
    BinkSetSoundSystem(reinterpret_cast<void*>(BinkOpenMiles), reinterpret_cast<s32>(hDriver));
    BinkSetIOSize(600000);

    gBinkHandleSlot1 = BinkOpen(pFileName, 0x4000000);

    if (gBinkHandleSlot1)
    {
        if (IsDirectBufferMode())
        {
            if (gVidSys->field_5C == 5)
            {
                if (gVidSys->field_64_r == 5 && gVidSys->field_6C == 5)
                {
                    gBinkPixelFormat = 2;
                    gBinkActiveSlot = 1;
                    return;
                }
                if (gVidSys->field_64_r == 6 && gVidSys->field_6C == 5)
                {
                    gBinkPixelFormat = 3;
                    gBinkActiveSlot = 1;
                    return;
                }
            }
            else if (gVidSys->field_5C == 6)
            {
                if (gVidSys->field_64_r == 5 && gVidSys->field_6C == 5)
                {
                    gBinkPixelFormat = 4;
                    gBinkActiveSlot = 1;
                    return;
                }
                if (gVidSys->field_64_r == 6 && gVidSys->field_6C == 4)
                {
                    gBinkPixelFormat = 5;
                    gBinkActiveSlot = 1;
                    return;
                }
            }

            gBinkPixelFormat = 3;
            gBinkActiveSlot = 1;
            return;
        }

        BinkBufferSetDDPrimary(gVidSys->field_134_SurfacePrimary);

        gBinkDDState = BinkDD_Opening;
        gBinkBufferSlot1 = BinkBufferOpen(gHwnd, gBinkHandleSlot1->width, gBinkHandleSlot1->height, 0);

        if (gBinkBufferSlot1 == NULL)
        {
            gBinkDDState = BinkDD_Closed;
            FatalError(Gta2Error::BinkBufferOpenError, "C:\\Splitting\\Gta2\\Source\\movie2.cpp", 450);
            gBinkActiveSlot = 1;
            return;
        }

        gBinkDDState = BinkDD_Open;
        gBinkActiveSlot = 1;
    }
    else
    {
        FatalError(Gta2Error::BinkOpenError, "C:\\Splitting\\Gta2\\Source\\movie2.cpp", 460);
    }
}

MATCH_FUNC(0x5136D0)
void __stdcall Bink::CheckWindowPos(s32* pNewWindowX, s32* pNewWindowY)
{
    if (gBinkActiveSlot == 1 && gBinkBufferSlot1 != NULL)
    {
        BinkBufferCheckWinPos(gBinkBufferSlot1, pNewWindowX, pNewWindowY);
        return;
    }
    else if (gBinkActiveSlot == 2 && gBinkBufferSlot2 != NULL)
    {
        BinkBufferCheckWinPos(gBinkBufferSlot2, pNewWindowX, pNewWindowY);
    }
}

MATCH_FUNC(0x513720)
void Bink::ResetBufferOffset()
{
    if (gBinkActiveSlot == 1 && gBinkBufferSlot1 != NULL)
    {
        BinkBufferSetOffset(gBinkBufferSlot1, 0, 0);
    }
    else if (gBinkActiveSlot == 2 && gBinkBufferSlot2 != NULL)
    {
        BinkBufferSetOffset(gBinkBufferSlot2, 0, 0);
    }
}

MATCH_FUNC(0x513760)
bool Bink::IsDDBufferOpen()
{
    return gBinkDDState == BinkDD_Open;
}

MATCH_FUNC(0x513770)
bool Bink::IsUsingDDBuffer()
{
    return gBinkDDState == BinkDD_Open || gBinkDDState == BinkDD_Opening;
}

MATCH_FUNC(0x513790)
char_type Bink::GetActiveSlot()
{
    return gBinkActiveSlot;
}

MATCH_FUNC(0x5137A0)
void __stdcall Bink::SetActiveSlot(char_type slot)
{
    gBinkActiveSlot = slot;
}

MATCH_FUNC(0x5137B0)
void __stdcall Bink::SetDDState(char_type state)
{
    gBinkDDState = state;
}
