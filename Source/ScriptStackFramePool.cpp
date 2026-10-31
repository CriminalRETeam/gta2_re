#include "ScriptStackFramePool.hpp"

MATCH_FUNC(0x4bc300)
ScriptStackFramePool::~ScriptStackFramePool()
{
    mPool.mpHead = NULL;
}

MATCH_FUNC(0x4bea80)
ScriptStackFrame::ScriptStackFrame()
{
    mpNext = NULL;
    mCondResult = 125;
    mReturnCmd = 205;
}

MATCH_FUNC(0x4beaa0)
ScriptStackFrame::~ScriptStackFrame()
{
    mpNext = NULL;
}

MATCH_FUNC(0x503110)
void ScriptStackFrame::PoolAllocate()
{
    mCondResult = 0;
    mReturnCmd = 0;
    mpNext = NULL;
}