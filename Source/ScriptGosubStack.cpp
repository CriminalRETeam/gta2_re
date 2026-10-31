#include "ScriptGosubStack.hpp"
#include "ScriptStackFramePool.hpp"
#include "Globals.hpp"

DEFINE_GLOBAL(ScriptStackFramePool*, gScriptStackFramePool, 0x6F8068);

MATCH_FUNC(0x503120)
ScriptGosubStack::ScriptGosubStack() throw() // 503120
{
    mCurrent = NULL;
    mCount = 0;
}

MATCH_FUNC(0x503130)
ScriptGosubStack::~ScriptGosubStack() // 503130
{
    for (ScriptStackFrame* pOld = mCurrent; mCurrent; pOld = mCurrent)
    {
        mCurrent = pOld->mpNext;
        gScriptStackFramePool->DeAllocate_476780(pOld);
        mCount--;
    }
}

MATCH_FUNC(0x503160)
void ScriptGosubStack::add_503160(ScriptStackFrame* pFrame)
{
    pFrame->mpNext = mCurrent;
    mCurrent = pFrame;
    mCount++;
}

MATCH_FUNC(0x503180)
ScriptStackFrame* ScriptGosubStack::remove_503180()
{
    ScriptStackFrame* pOld = mCurrent;
    if (pOld)
    {
        mCurrent = pOld->mpNext;
        pOld->mpNext = NULL;
        mCount--;
    }

    return pOld;
}

MATCH_FUNC(0x5031A0)
ScriptStackFrame* ScriptGosubStack::AllocFrame()
{
    return gScriptStackFramePool->mPool.Allocate();
}

MATCH_FUNC(0x5031C0)
void ScriptGosubStack::FreeFrame(ScriptStackFrame* pFrame)
{
    gScriptStackFramePool->mPool.DeAllocate(pFrame);
}

MATCH_FUNC(0x5031E0)
void ScriptGosubStack::remove_5031E0(u8 count)
{
    while (mCount > count)
    {
        remove_503180();
    }
}