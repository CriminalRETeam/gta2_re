#pragma once

#include "Function.hpp"

EXTERN_GLOBAL(u32, gObj8_count);
EXTERN_GLOBAL(u32, gObj8_dealloc_count);

class Object_8
{
  public:
    EXPORT Object_8();
    EXPORT ~Object_8();

    inline void PoolAllocate()
    {
        gObj8_count++;
        mFrameCounter = 0;
        mFrameIdx = 0;
        mTimer = 0;
    }

    // 9.6f 0x482AF0
    inline void PoolDeallocate()
    {
        --gObj8_count;
        ++gObj8_dealloc_count;
    }

    Object_8* mpNext;
    s16 mTimer; // 0x4
    u8 mFrameCounter; // 0x6
    u8 mFrameIdx; // 0x7
};