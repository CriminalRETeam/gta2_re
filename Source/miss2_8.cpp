#include "miss2_8.hpp"
#include "Frismo_25C.hpp"
#include "Globals.hpp"

// TODO: move
DEFINE_GLOBAL(Frismo_C_Pool*, gFrismo_C_Pool_6F8068, 0x6F8068);

MATCH_FUNC(0x503120)
miss2_8::miss2_8() // 503120
{
    field_0_current = 0;
    field_4_count = 0;
}

MATCH_FUNC(0x503130)
miss2_8::~miss2_8() // 503130
{
    Frismo_C_Pool* pGlobal;

    for (Frismo_C* pOld = field_0_current; field_0_current; pOld = field_0_current)
    {
        field_0_current = pOld->mpNext;
        pGlobal = gFrismo_C_Pool_6F8068;
        pOld->mpNext = pGlobal->field_0;
        pGlobal->field_0 = pOld;
        field_4_count--;
    }
}

MATCH_FUNC(0x503160)
void miss2_8::add_503160(Frismo_C* a2)
{
    a2->mpNext = field_0_current;
    field_0_current = a2;
    field_4_count++;
}

MATCH_FUNC(0x503180)
Frismo_C* miss2_8::remove_503180()
{
    Frismo_C* pOld = field_0_current;
    if (pOld)
    {
        field_0_current = pOld->mpNext;
        pOld->mpNext = 0;
        field_4_count--;
    }

    return pOld;
}

MATCH_FUNC(0x5031A0)
Frismo_C* miss2_8::sub_5031A0()
{
    Frismo_C* v1 = gFrismo_C_Pool_6F8068->field_0;
    gFrismo_C_Pool_6F8068->field_0 = gFrismo_C_Pool_6F8068->field_0->mpNext;
    v1->sub_503110();
    return v1;
}

MATCH_FUNC(0x5031C0)
void miss2_8::sub_5031C0(Frismo_C* a2)
{
    Frismo_C_Pool* pFrismo = gFrismo_C_Pool_6F8068;
    a2->mpNext = gFrismo_C_Pool_6F8068->field_0;
    pFrismo->field_0 = a2;
}

MATCH_FUNC(0x5031E0)
void miss2_8::remove_5031E0(u8 count)
{
    while (field_4_count > count)
    {
        remove_503180();
    }
}