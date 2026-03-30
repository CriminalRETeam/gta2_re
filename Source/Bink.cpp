#include "Bink.hpp"

DEFINE_GLOBAL(s32, gBink_state_6F83B0, 0x6F83B0);
DEFINE_GLOBAL(s32, gBink_state_6F8168, 0x6F8168);
DEFINE_GLOBAL(s32, gBink_state_6F80C4, 0x6F80C4);
DEFINE_GLOBAL(s32, gBink_state_6F8170, 0x6F8170);
DEFINE_GLOBAL(char_type, gBink_state_6F83FE, 0x6F83FE);
DEFINE_GLOBAL(char_type, gBink_state_6F83FF, 0x6F83FF);
DEFINE_GLOBAL(s32, gBink_state_6f8250, 0x6F8250);
DEFINE_GLOBAL(s32, gBink_state_6F81B0, 0x6F81B0);

MATCH_FUNC(0x513210)
void __stdcall Bink::Reset_513210()
{
    gBink_state_6F83B0 = 0;
    gBink_state_6F8168 = 0;
    gBink_state_6F80C4 = 0;
    gBink_state_6F8170 = 0;
    gBink_state_6F83FE = 0;
    gBink_state_6F83FF = 0;
}

MATCH_FUNC(0x513340)
void Bink::Close1_513340()
{
    if (gBink_state_6F8170 != 0)
    {
        BinkBufferClose(gBink_state_6F8170);
        gBink_state_6F8170 = 0;
        gBink_state_6F83FE = 0;
        gBink_state_6F83FF = 0;
    }

    if (gBink_state_6F8168 != 0)
    {
        BinkGetSummary(gBink_state_6F8168, &gBink_state_6f8250);
        BinkClose(gBink_state_6F8168);
        gBink_state_6F8168 = 0;
    }
}

MATCH_FUNC(0x513390)
void Bink::Close2_513390()
{
    if (gBink_state_6F80C4 != 0)
    {
        BinkBufferClose(gBink_state_6F80C4);
        gBink_state_6F80C4 = 0;
        gBink_state_6F83FE = 0;
        gBink_state_6F83FF = 0;
    }

    if (gBink_state_6F83B0)
    {
        BinkGetSummary(gBink_state_6F83B0, &gBink_state_6f8250);
        BinkClose(gBink_state_6F83B0);
        gBink_state_6F83B0 = 0;
    }
}

MATCH_FUNC(0x5136D0)
void __stdcall Bink::sub_5136D0(s32* pNewWindowX, s32* pNewWindowY)
{
    if (gBink_state_6F83FF == 1 && gBink_state_6F8170 != 0)
    {
        BinkBufferCheckWinPos(gBink_state_6F8170, pNewWindowX, pNewWindowY);
        return;
    }
    else if (gBink_state_6F83FF == 2 && gBink_state_6F80C4 != 0)
    {
        BinkBufferCheckWinPos(gBink_state_6F80C4, pNewWindowX, pNewWindowY);
    }
}

MATCH_FUNC(0x513770)
BOOL Bink::sub_513770()
{
    return gBink_state_6F83FE == 2 || gBink_state_6F83FE == 1;
}

MATCH_FUNC(0x513760)
BOOL Bink::sub_513760()
{
    return gBink_state_6F83FE == 2;
}

MATCH_FUNC(0x513720)
void Bink::sub_513720()
{
    if (gBink_state_6F83FF == 1 && gBink_state_6F8170 != 0)
    {
        BinkBufferSetOffset(gBink_state_6F8170, 0, 0);
    }
    else if (gBink_state_6F83FF == 2 && gBink_state_6F80C4 != 0)
    {
        BinkBufferSetOffset(gBink_state_6F80C4, 0, 0);
    }
}

STUB_FUNC(0x513240)
char_type Bink::sub_513240()
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x513790)
char_type Bink::sub_513790()
{
    return gBink_state_6F83FF;
}

MATCH_FUNC(0x5137A0)
void __stdcall Bink::sub_5137A0(char_type a1)
{
    gBink_state_6F83FF = a1;
}

WIP_FUNC(0x5133E0)
void __stdcall Bink::sub_5133E0(const char_type* a1, HDIGDRIVER a2)
{
    BinkSetSoundSystem((void*)BinkOpenMiles, (s32)a2);
    BinkSetIOSize(600000);

    gBink_state_6F83B0 = BinkOpen(a1, 0x4000000);

    if (gBink_state_6F83B0 == 0)
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
                gBink_state_6F81B0 = 2;
                gBink_state_6F83FF = 2;
                return;
            }
        }
        else if (gVidSys_7071D0->field_5C == 6)
        {
            if (gVidSys_7071D0->field_64_r == 5 && gVidSys_7071D0->field_6C == 5)
            {
                gBink_state_6F81B0 = 4;
                gBink_state_6F83FF = 2;
                return;
            }
            if (gVidSys_7071D0->field_64_r == 6 && gVidSys_7071D0->field_6C == 4)
            {
                gBink_state_6F81B0 = 5;
                gBink_state_6F83FF = 2;
                return;
            }
        }

        gBink_state_6F81B0 = 3;
        gBink_state_6F83FF = 2;
        return;
    }

    // Software / DirectDraw path.
    if (gBink_state_6F83FE == 0)
    {
        BinkBufferSetDDPrimary((s32)gVidSys_7071D0->field_134_SurfacePrimary);
    }
    
    gBink_state_6F83FE = 1;
    gBink_state_6F80C4 = BinkBufferOpen(gHwnd_707F04, *(s32*)gBink_state_6F83B0, ((s32*)gBink_state_6F83B0)[1], 0);

    if (gBink_state_6F80C4 == 0)
    {
        gBink_state_6F83FE = 0;
        FatalError_4A38C0(Gta2Error::BinkBufferOpenError, "C:\\Splitting\\Gta2\\Source\\movie2.", 360);
        gBink_state_6F83FF = 2;
        return;
    }

    gBink_state_6F83FE = 2;
    gBink_state_6F83FF = 2;
}

MATCH_FUNC(0x5137B0)
void __stdcall Bink::sub_5137B0(char_type a1)
{
    gBink_state_6F83FE = a1;
}

STUB_FUNC(0x513560)
void __stdcall Bink::sub_513560(const char_type* pFileName, HDIGDRIVER a2)
{
    NOT_IMPLEMENTED;
    // todo
}