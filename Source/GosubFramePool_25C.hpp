#pragma once

#include "Function.hpp"
#include "Pool.hpp"

// A saved script GOSUB frame: where to return to and the condition result at the time of the call
class GosubFrame_C
{
  public:
    EXPORT GosubFrame_C();
    EXPORT ~GosubFrame_C();
    EXPORT void PoolAllocate();

    void PoolDeallocate()
    {
      
    }

    s32 field_0_cond_result;
    s16 field_4_return_cmd;
    GosubFrame_C* mpNext;
};
GTA2_ASSERT_SIZEOF_ALWAYS(GosubFrame_C, 0xC)

// Pool of the gosub frames of all script stacks (miss2_8)
class GosubFramePool_25C
{
  public:
    enum
    {
        k_max_frames = 50
    };

    GosubFramePool_25C()
    {

    }

    EXPORT ~GosubFramePool_25C();

    // 9.6f 0x476780
    inline void DeAllocate_476780(GosubFrame_C* pItem)
    {
        field_0_pool.DeAllocate(pItem);
    }

    PoolBasic<GosubFrame_C, k_max_frames> field_0_pool;
};
GTA2_ASSERT_SIZEOF_ALWAYS(GosubFramePool_25C, 0x25C)

EXTERN_GLOBAL(GosubFramePool_25C*, gGosubFramePool_6F8068);
