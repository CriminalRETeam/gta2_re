#include "miss2_8.hpp"
#include "GosubFramePool_25C.hpp"
#include "Globals.hpp"

// TODO: move
DEFINE_GLOBAL(GosubFramePool_25C*, gGosubFramePool_6F8068, 0x6F8068);

MATCH_FUNC(0x503120)
miss2_8::miss2_8() throw() // 503120
{
    field_0_current = 0;
    field_4_count = 0;
}

MATCH_FUNC(0x503130)
miss2_8::~miss2_8() // 503130
{
    for (GosubFrame_C* pOld = field_0_current; field_0_current; pOld = field_0_current)
    {
        field_0_current = pOld->mpNext;
        gGosubFramePool_6F8068->DeAllocate_476780(pOld);
        field_4_count--;
    }
}

MATCH_FUNC(0x503160)
void miss2_8::add_503160(GosubFrame_C* pFrame)
{
    pFrame->mpNext = field_0_current;
    field_0_current = pFrame;
    field_4_count++;
}

MATCH_FUNC(0x503180)
GosubFrame_C* miss2_8::remove_503180()
{
    GosubFrame_C* pOld = field_0_current;
    if (pOld)
    {
        field_0_current = pOld->mpNext;
        pOld->mpNext = 0;
        field_4_count--;
    }

    return pOld;
}

MATCH_FUNC(0x5031A0)
GosubFrame_C* miss2_8::AllocFrame_5031A0()
{
    GosubFrame_C* v1 = gGosubFramePool_6F8068->field_0_pool.field_0_pHead;
    gGosubFramePool_6F8068->field_0_pool.field_0_pHead = gGosubFramePool_6F8068->field_0_pool.field_0_pHead->mpNext;
    v1->PoolAllocate();
    return v1;

    // TOOD: Pools - this should match but doesn't ??
//    return gGosubFramePool_6F8068->field_0_pool.Allocate();
}

MATCH_FUNC(0x5031C0)
void miss2_8::FreeFrame_5031C0(GosubFrame_C* pFrame)
{
    gGosubFramePool_6F8068->field_0_pool.DeAllocate(pFrame);
}

MATCH_FUNC(0x5031E0)
void miss2_8::remove_5031E0(u8 count)
{
    while (field_4_count > count)
    {
        remove_503180();
    }
}