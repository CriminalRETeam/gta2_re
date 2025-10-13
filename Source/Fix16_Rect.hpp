#pragma once

#include "Function.hpp"
#include "fix16.hpp"

class Sprite;

class Fix16_Rect
{
  public:
    // Inlined from 0x41d070
    Fix16_Rect()
    {
    }

    // TODO: Get inline addr
    Fix16_Rect(Fix16 x, Fix16 y, Fix16 w, Fix16 h)
    {
        field_0_left = x;
        field_8_top = y;
        field_4_right = w + x;
        field_C_bottom = h + y;
    }

    EXPORT void DoSetCurrentRect_59DD60();
    EXPORT u32 sub_59DDF0(Sprite* a2);
    EXPORT char_type sub_59DE80();
    EXPORT void sub_59DEE0(Fix16 a2, Fix16 a3);
    EXPORT bool sub_59DF20(u8 a2);
    EXPORT bool sub_59DFB0(s32 a2);

    inline bool inline_sub_4B9FD0(Fix16_Point* a2)
    {
        return Fix16::inline_is_between(field_0_left, field_4_right, a2->x) &&
            Fix16::inline_is_between(field_8_top, field_C_bottom, a2->y);
    }

    // TODO: Ordering
    EXPORT void sub_5A5E30(Fix16 left, Fix16 right, Fix16 top, Fix16 bottom);

    Fix16 field_0_left;
    Fix16 field_4_right;
    Fix16 field_8_top;
    Fix16 field_C_bottom;

    Fix16 field_10;
    Fix16 field_14;
};