#include "ScriptStackFramePool.hpp"

MATCH_FUNC(0x4bc300)
ScriptStackFramePool::~ScriptStackFramePool()
{
    field_0_pool.field_0_pHead = 0;
}

MATCH_FUNC(0x4bea80)
ScriptStackFrame::ScriptStackFrame()
{
    mpNext = 0;
    field_0_cond_result = 125;
    field_4_return_cmd = 205;
}

MATCH_FUNC(0x4beaa0)
ScriptStackFrame::~ScriptStackFrame()
{
    mpNext = 0;
}

MATCH_FUNC(0x503110)
void ScriptStackFrame::PoolAllocate()
{
    field_0_cond_result = 0;
    field_4_return_cmd = 0;
    mpNext = 0;
}