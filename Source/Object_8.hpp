#pragma once

#include "Function.hpp"

EXTERN_GLOBAL(u32, gObj8_count_6F8F18);
EXTERN_GLOBAL(u32, gObj8_dealloc_count_6F8DC0);

class Object_8
{
  public:
    EXPORT Object_8();
    EXPORT ~Object_8();

    inline void PoolAllocate()
    {
        gObj8_count_6F8F18++;
        mFrameCounter = 0;
        mFrameIdx = 0;
        mTimer = 0;
    }

    // 9.6f 0x482AF0
    inline void PoolDeallocate()
    {
        --gObj8_count_6F8F18;
        ++gObj8_dealloc_count_6F8DC0;
    }

    Object_8* mpNext;
    s16 mTimer; // 0x4
    u8 mFrameCounter; // 0x6
    u8 mFrameIdx; // 0x7
};