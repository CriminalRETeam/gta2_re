#include "Bink.hpp"

DEFINE_GLOBAL(s32, gBink_state_6F83B0, 0x6F83B0);
DEFINE_GLOBAL(s32, gBink_state_6F8168, 0x6F8168);
DEFINE_GLOBAL(s32, gBink_state_6F80C4, 0x6F80C4);
DEFINE_GLOBAL(s32, gBink_state_6F8170, 0x6F8170);
DEFINE_GLOBAL(char_type, gBink_state_6F83FE, 0x6F83FE);
DEFINE_GLOBAL(char_type, gBink_state_6F83FF, 0x6F83FF);

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

WIP_FUNC(0x513340)
void Bink::Close1_513340()
{
    WIP_IMPLEMENTED;

    // To-do: Get bink header stubs.
    if (gBink_state_6F8170 != 0)
    {
        //_BinkBufferClose@4(gBink_state_6F8170);
        gBink_state_6F8170 = 0;
        gBink_state_6F83FE = 0;
        gBink_state_6F83FF = 0;
    }

    if (gBink_state_6F8168 != 0) {
        //_BinkGetSummary@8(gBink_state_6F8168, &DAT_006f8250);
        //_BinkClose@4(gBink_state_6F8168);
        gBink_state_6F8168 = 0;
    }
}

WIP_FUNC(0x513390)
void Bink::Close2_513390()
{
    WIP_IMPLEMENTED;

    // To-do: Get bink header stubs.
    if (gBink_state_6F80C4 != 0)
    {
        //_BinkBufferClose@4(gBink_state_6F80C4);
        gBink_state_6F80C4 = 0;
        gBink_state_6F83FE = 0;
        gBink_state_6F83FF = 0;
    }

    if (gBink_state_6F83B0)
    {
        //_BinkGetSummary@8(gBink_state_6F83B0, &DAT_006f8250);
        //_BinkClose@4(gBink_state_6F83B0);
        gBink_state_6F83B0 = 0;
    }
}

WIP_FUNC(0x5136D0)
void Bink::sub_5136D0(s32* pNewWindowX, s32* pNewWindowY)
{
    WIP_IMPLEMENTED;

    // To-do: Get bink header stubs.
    if (gBink_state_6F83FF == 1 && gBink_state_6F8170 != 0)
    {
        //_BinkBufferCheckWinPos@12(gBink_state_6F8170, pNewWindowX, pNewWindowY);
    }
    else if (gBink_state_6F83FF == 2 && gBink_state_6F80C4 != 0)
    {
        //_BinkBufferCheckWinPos@12(gBink_state_6F8170, pNewWindowX, pNewWindowY);
    }
}

MATCH_FUNC(0x513770)
BOOL Bink::sub_513770()
{
    if (gBink_state_6F83FE == 2 || gBink_state_6F83FE == 1)
    {
        return true;
    }

    return false;
}

MATCH_FUNC(0x513760)
BOOL Bink::sub_513760()
{
    return gBink_state_6F83FE == 2;
}

WIP_FUNC(0x513720)
void Bink::sub_513720()
{
    WIP_IMPLEMENTED;

    if (gBink_state_6F83FF == 1 && gBink_state_6F8170 != 0)
    {
        //_BinkBufferSetOffset@12(gBink_state_6F8170, 0, 0);
    }
    else if (gBink_state_6F83FF == 2 && gBink_state_6F80C4 != 0)
    {
        //_BinkBufferSetOffset@12(gBink_state_6F80C4, 0, 0);
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

STUB_FUNC(0x5137A0)
void __stdcall Bink::sub_5137A0(char_type a1)
{
    NOT_IMPLEMENTED;
    // todo
    //gBink_6F83FF = a1;
}

STUB_FUNC(0x5133E0)
void __stdcall Bink::sub_5133E0(const char_type* a1, HDIGDRIVER a2)
{
    NOT_IMPLEMENTED;
    // todo
}

STUB_FUNC(0x5137B0)
void __stdcall Bink::sub_5137B0(char_type a1)
{
    NOT_IMPLEMENTED;
    // todo
    //gBink_state_6F83FE = a1;
}

STUB_FUNC(0x513560)
void __stdcall Bink::sub_513560(const char_type* pFileName, HDIGDRIVER a2)
{
    NOT_IMPLEMENTED;
    // todo
}