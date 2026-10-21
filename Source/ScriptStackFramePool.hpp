#pragma once

#include "Function.hpp"
#include "Pool.hpp"

// A script stack frame (pushed by GOSUB): where to return to and the condition result at the time of the call
class ScriptStackFrame
{
  public:
    EXPORT ScriptStackFrame();
    EXPORT ~ScriptStackFrame();
    EXPORT void PoolAllocate();

    void PoolDeallocate()
    {
      
    }

    s32 field_0_cond_result;
    s16 field_4_return_cmd;
    ScriptStackFrame* mpNext;
};
GTA2_ASSERT_SIZEOF_ALWAYS(ScriptStackFrame, 0xC)

// Pool of the stack frames (GOSUB return info) of all script threads (ScriptGosubStack)
class ScriptStackFramePool
{
  public:
    enum
    {
        k_max_frames = 50
    };

    ScriptStackFramePool()
    {

    }

    EXPORT ~ScriptStackFramePool();

    // 9.6f 0x476780
    inline void DeAllocate_476780(ScriptStackFrame* pItem)
    {
        field_0_pool.DeAllocate(pItem);
    }

    PoolBasic<ScriptStackFrame, k_max_frames> field_0_pool;
};
GTA2_ASSERT_SIZEOF_ALWAYS(ScriptStackFramePool, 0x25C)

EXTERN_GLOBAL(ScriptStackFramePool*, gScriptStackFramePool_6F8068);
