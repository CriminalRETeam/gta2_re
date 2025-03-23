#pragma once

#include "Car_BC.hpp"
#include "Function.hpp"
#include "fix16.hpp"
class Sprite;

class Car_14_18
{
  public:
    // Inlined from 0x41d070
    Car_14_18()
    {
    }
    EXPORT void sub_59DD60();
    EXPORT u32 sub_59DDF0(Sprite* a2);
    EXPORT char_type sub_59DE80();
    EXPORT void sub_59DEE0(Fix16 a2, Fix16 a3);
    EXPORT bool sub_59DF20(u8 a2);
    EXPORT bool sub_59DFB0(s32 a2);

    inline bool inline_sub_4B9FD0(Car_8* a2)
    {
        return Fix16::inline_is_between(field_0, field_4, a2->field_0) && Fix16::inline_is_between(field_8, field_C, a2->field_4);
    }

    Fix16 field_0;
    Fix16 field_4;
    Fix16 field_8;
    Fix16 field_C;
    Fix16 field_10;
    Fix16 field_14;
};
