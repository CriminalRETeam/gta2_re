#pragma once

#include "Function.hpp"

class Object_8
{
  public:
    EXPORT Object_8();
    EXPORT ~Object_8();

    inline void PoolAllocate()
    {
        field_6_frame_counter = 0;
        field_7_anim_speed_counter = 0;
        field_4_timer = 0;
    }

    Object_8* mpNext;
    s16 field_4_timer;
    u8 field_6_frame_counter;
    u8 field_7_anim_speed_counter;
};