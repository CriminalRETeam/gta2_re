#pragma once

#include "Function.hpp"

EXTERN_GLOBAL(u32, dword_6F8F18);
EXTERN_GLOBAL(u32, dword_6F8DC0);

class Object_8
{
  public:
    EXPORT Object_8();
    EXPORT ~Object_8();

    inline void PoolAllocate()
    {
        dword_6F8F18++;
        field_6_frame_counter = 0;
        field_7_anim_speed_counter = 0;
        field_4_timer = 0;
    }

    // 9.6f 0x482AF0
    inline void PoolDeallocate()
    {
        --dword_6F8F18;
        ++dword_6F8DC0;
    }

    Object_8* mpNext;
    s16 field_4_timer;
    u8 field_6_frame_counter;
    u8 field_7_anim_speed_counter;
};