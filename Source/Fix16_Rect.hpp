#pragma once

#include "Fix16_Point.hpp"
#include "Function.hpp"
#include "fix16.hpp"

class Sprite;

EXTERN_GLOBAL(Fix16, kSmallWidthEpslion_703450);

// 9.6f 0x41E160
// https://decomp.me/scratch/A4s7c
inline bool __stdcall IntervalsOverlap_41E160(const Fix16& ourMin, const Fix16& ourMax, const Fix16& otherMin, const Fix16& otherMax)
{
    if (ourMin < otherMin)
    {
        return ((ourMax < otherMin)) ? false : true;
    }
    else
    {
        return (ourMin <= otherMax) ? true : false;
    }
}

class Fix16_Rect
{
  public:
    // Inlined from 0x41d070
    Fix16_Rect()
    {
    }

    // 9.6f 0x41E350
    // https://decomp.me/scratch/HVOft
    void SetRect_41E350(Fix16 left, Fix16 right, Fix16 top, Fix16 bottom)
    {
        this->field_0_left = left;
        this->field_4_right = right;
        this->field_8_top = top;
        this->field_C_bottom = bottom;
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
        return Fix16::IsBetween(field_0_left, field_4_right, a2->x) && Fix16::IsBetween(field_8_top, field_C_bottom, a2->y);
    }

    // TODO: Ordering
    EXPORT void sub_5A5E30(Fix16 left, Fix16 right, Fix16 top, Fix16 bottom);

    // 9.6f 0x41E390
    // https://decomp.me/scratch/YTMyx
    bool IsZeroWidth_41E390() const
    {
        return (this->field_0_left == this->field_4_right && this->field_0_left <= kSmallWidthEpslion_703450) ? true : false;
    }

    // 9.6f 0x41E2F0
    // https://decomp.me/scratch/TT06X
    bool AABB_Intersects_41E2F0(Fix16_Rect* pOther) const
    {
        return IntervalsOverlap_41E160(this->field_0_left, this->field_4_right, pOther->field_0_left, pOther->field_4_right) &&
                IntervalsOverlap_41E160(this->field_8_top, this->field_C_bottom, pOther->field_8_top, pOther->field_C_bottom) &&
                IntervalsOverlap_41E160(this->field_10, this->field_14, pOther->field_10, pOther->field_14) ?
            true :
            false;
    }

    Fix16 field_0_left;
    Fix16 field_4_right;
    Fix16 field_8_top;
    Fix16 field_C_bottom;

    Fix16 field_10;
    Fix16 field_14;
};
