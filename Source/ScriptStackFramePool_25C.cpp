#include "ScriptStackFramePool_25C.hpp"

MATCH_FUNC(0x4bc300)
ScriptStackFramePool_25C::~ScriptStackFramePool_25C()
{
    field_0_pool.field_0_pHead = 0;
}

MATCH_FUNC(0x4bea80)
ScriptStackFrame_C::ScriptStackFrame_C()
{
    mpNext = 0;
    field_0_cond_result = 125;
    field_4_return_cmd = 205;
}

MATCH_FUNC(0x4beaa0)
ScriptStackFrame_C::~ScriptStackFrame_C()
{
    mpNext = 0;
}

MATCH_FUNC(0x503110)
void ScriptStackFrame_C::PoolAllocate()
{
    field_0_cond_result = 0;
    field_4_return_cmd = 0;
    mpNext = 0;
}