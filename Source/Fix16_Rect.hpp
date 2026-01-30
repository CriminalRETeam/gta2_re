#pragma once

#include "Fix16_Point.hpp"
#include "Function.hpp"
#include "fix16.hpp"

class Sprite;

EXTERN_GLOBAL(Fix16, kSmallWidthEpslion_703450);
EXTERN_GLOBAL(Fix16, k_dword_6771E4);

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
    // 9.6f 0x41D070
    Fix16_Rect()
    {
    }

    // 9.6f 0x4204D0
    void ComputeCollisionPrism_4204D0(Fix16 x, Fix16 y, Fix16 offset, Fix16 z)
    {
        Fix16 tmp =  offset / 2;

        field_0_left = x - tmp;
        field_4_right = x + tmp;
        field_8_top = y - tmp;
        field_C_bottom = y + tmp;

        this->field_10_low_z = z - k_dword_6771E4;
        this->field_14_high_z = z + k_dword_6771E4;
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

    // TODO: None inline version of SetRect_41E350 ??
    EXPORT void SetRect_5A5E30(Fix16 left, Fix16 right, Fix16 top, Fix16 bottom);

    void SetHiLowZ_41E370(Fix16 lowZ, Fix16 highZ)
    {
        this->field_10_low_z = lowZ;
        this->field_14_high_z = highZ;
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
    EXPORT u8 IntersectsSpriteRenderingRect_59DDF0(Sprite* a2);
    EXPORT char_type CanRectEnterMovementRegion_59DE80();
    EXPORT void ExpandToIncludePoint_59DEE0(Fix16 a2, Fix16 a3);
    EXPORT bool RectOverlapsZone_59DF20(u8 a2);
    EXPORT bool EveryTileMatchesArrowType_59DFB0(s32 a2);

    // 9.6f 0x4B9FD0
    inline bool IsPointInRect_4B9FD0(Fix16_Point* a2)
    {
        return Fix16::IsBetween_4B9E10(field_0_left, field_4_right, a2->x) && Fix16::IsBetween_4B9E10(field_8_top, field_C_bottom, a2->y);
    }

    // 9.6f 0x41E2F0
    // https://decomp.me/scratch/TT06X
    bool AABB_Intersects_41E2F0(Fix16_Rect* pOther) const
    {
        // TODO: HACK - made 0x438FB0 a method of Fix16 to fix crazy compiler inlining behaviour/force it to do what we need
        return field_0_left.IntervalIntersectsRange_438FB0_inline(this->field_4_right, pOther->field_0_left, pOther->field_4_right) &&
                field_8_top.IntervalIntersectsRange_438FB0_inline(this->field_C_bottom, pOther->field_8_top, pOther->field_C_bottom) &&
                field_10_low_z.IntervalIntersectsRange_438FB0_inline( this->field_14_high_z, pOther->field_10_low_z, pOther->field_14_high_z) ?
            true :
            false;
    }

    Fix16 field_0_left;
    Fix16 field_4_right;
    Fix16 field_8_top;
    Fix16 field_C_bottom;

    Fix16 field_10_low_z;
    Fix16 field_14_high_z;
};
