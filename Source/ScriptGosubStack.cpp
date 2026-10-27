#include "ScriptGosubStack.hpp"
#include "ScriptStackFramePool.hpp"
#include "Globals.hpp"

// TODO: move
DEFINE_GLOBAL(ScriptStackFramePool*, gScriptStackFramePool_6F8068, 0x6F8068);

MATCH_FUNC(0x503120)
ScriptGosubStack::ScriptGosubStack() throw() // 503120
{
    field_0_current = NULL;
    field_4_count = 0;
}

MATCH_FUNC(0x503130)
ScriptGosubStack::~ScriptGosubStack() // 503130
{
    for (ScriptStackFrame* pOld = field_0_current; field_0_current; pOld = field_0_current)
    {
        field_0_current = pOld->mpNext;
        gScriptStackFramePool_6F8068->DeAllocate_476780(pOld);
        field_4_count--;
    }
}

MATCH_FUNC(0x503160)
void ScriptGosubStack::add_503160(ScriptStackFrame* pFrame)
{
    pFrame->mpNext = field_0_current;
    field_0_current = pFrame;
    field_4_count++;
}

MATCH_FUNC(0x503180)
ScriptStackFrame* ScriptGosubStack::remove_503180()
{
    ScriptStackFrame* pOld = field_0_current;
    if (pOld)
    {
        field_0_current = pOld->mpNext;
        pOld->mpNext = NULL;
        field_4_count--;
    }

    return pOld;
}

MATCH_FUNC(0x5031A0)
ScriptStackFrame* ScriptGosubStack::AllocFrame_5031A0()
{
    ScriptStackFrame* pFrame = gScriptStackFramePool_6F8068->field_0_pool.field_0_pHead;
    gScriptStackFramePool_6F8068->field_0_pool.field_0_pHead = gScriptStackFramePool_6F8068->field_0_pool.field_0_pHead->mpNext;
    pFrame->PoolAllocate();
    return pFrame;

    // TOOD: Pools - this should match but doesn't ??
//    return gScriptStackFramePool_6F8068->field_0_pool.Allocate();
}

MATCH_FUNC(0x5031C0)
void ScriptGosubStack::FreeFrame_5031C0(ScriptStackFrame* pFrame)
{
    gScriptStackFramePool_6F8068->field_0_pool.DeAllocate(pFrame);
}

MATCH_FUNC(0x5031E0)
void ScriptGosubStack::remove_5031E0(u8 count)
{
    while (field_4_count > count)
    {
        remove_503180();
    }
}