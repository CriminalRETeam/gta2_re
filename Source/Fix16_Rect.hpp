#pragma once

#include "Fix16_Point.hpp"
#include "Function.hpp"
#include "fix16.hpp"

class Sprite;

EXTERN_GLOBAL(Fix16, kSmallWidthEpslion);
EXTERN_GLOBAL(Fix16, kCollisionPrismHalfHeight);

// ComputeCollisionPrism_4204D0's half height is a header static each TU of the original has its own copy of
// (9.6f has one, 0x5E4D50). A TU whose copy isn't kCollisionPrismHalfHeight defines
// FIX16_RECT_HALF_HEIGHT to it (and declares it) before its first include.
#ifndef FIX16_RECT_HALF_HEIGHT
    #define FIX16_RECT_HALF_HEIGHT kCollisionPrismHalfHeight
#endif

// 9.6f 0x41E160
// https://decomp.me/scratch/A4s7c
inline bool __stdcall IntervalsOverlap_41E160(const Fix16& ourMin, const Fix16& ourMax, const Fix16& otherMin, const Fix16& otherMax)
{
    if (ourMin < otherMin)
    {
        if (ourMax < otherMin)
        {
            return false;
        }
        return true;
    }
    if (ourMin <= otherMax)
    {
        return true;
    }
    return false;
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
        Fix16 half_size =  offset / 2;

        mLeft = x - half_size;
        mRight = x + half_size;
        mTop = y - half_size;
        mBottom = y + half_size;

        this->mLowZ = z - FIX16_RECT_HALF_HEIGHT;
        this->mHighZ = z + FIX16_RECT_HALF_HEIGHT;
    }

    // Unused since ShockPedsAroundSprite calls ComputeCollisionPrism_4204D0 with FIX16_RECT_HALF_HEIGHT. Kept because removing
    // an unused inline can move code in other TUs (docs/matching_quirks.md).
    void ComputeShockPrism(Fix16 x, Fix16 y, Fix16 offset, Fix16 z, const Fix16& half_z)
    {
        offset = offset / 2;

        mLeft = x.Subtract(offset);
        mRight = x.Add(offset);
        mTop = y.Subtract(offset);
        mBottom = y.Add(offset);

        mLowZ = z.Subtract(half_z);
        mHighZ = z.Add(half_z);
    }

    // 9.6f 0x41E350
    // https://decomp.me/scratch/HVOft
    void SetRect_41E350(Fix16 left, Fix16 right, Fix16 top, Fix16 bottom)
    {
        this->mLeft = left;
        this->mRight = right;
        this->mTop = top;
        this->mBottom = bottom;
    }

    // Out-of-line copy of SetRect_41E350
    EXPORT void SetRect_5A5E30(Fix16 left, Fix16 right, Fix16 top, Fix16 bottom);
    EXPORT void MakeRect(Fix16 x, Fix16 y, Fix16 w, Fix16 h);

    // 9.6f 0x433560
    inline bool OverlapsZ_433560(Fix16_Rect* pOther) const
    {
        return IntervalsOverlap_41E160(mLowZ, mHighZ, pOther->mLowZ, pOther->mHighZ);
    }

    // 9.6f 0x463760
    inline s32 GetMidZ_463760() const
    {
        return (mLowZ.Add(mHighZ) / 2).ToInt();
    }

    // 9.6f 0x463690, defined in map_0x370.cpp
    inline bool EdgesCrossSegment_463690(Fix16_Point& p1, Fix16_Point& p2);

    // 9.6f 0x463710
    inline void SetFromPosSize_463710(Fix16 left, Fix16 top, Fix16 width, Fix16 height)
    {
        mLeft = left;
        mTop = top;
        mRight = left + width;
        mBottom = top + height;
    }

    // 9.6f 0x41E370
    void SetHiLowZ_41E370(Fix16 lowZ, Fix16 highZ)
    {
        this->mLowZ = lowZ;
        this->mHighZ = highZ;
    }

    Fix16_Rect(Fix16 left, Fix16 right, Fix16 top, Fix16 bottom, Fix16 z)
    {
        SetRect_41E350(left, right, top, bottom);
        mLowZ = z - kCollisionPrismHalfHeight;
        mHighZ = z + kCollisionPrismHalfHeight;
    }

    // Unused (SetFromPosSize_463710 does this), but removing it changes code elsewhere: see the shared
    // header names note in docs/matching_quirks.md
    Fix16_Rect(Fix16 x, Fix16 y, Fix16 w, Fix16 h)
    {
        mLeft = x;
        mTop = y;
        mRight = w + x;
        mBottom = h + y;
    }

    EXPORT void DoSetCurrentRect();
    EXPORT bool IntersectsSpriteRenderingRect(Sprite* pSprite);
    EXPORT char_type IsRectBlockedByMap();
    EXPORT void ExpandToIncludePoint(Fix16 x, Fix16 y);
    EXPORT bool RectOverlapsZone(u8 zone_type);
    EXPORT bool EveryTileMatchesArrowType(s32 arrow_type);

    // 9.6f 0x4B9FD0
    inline bool IsPointInRect_4B9FD0(Fix16_Point* pPoint)
    {
        return Fix16::IsBetween_4B9E10(mLeft, mRight, pPoint->x) && Fix16::IsBetween_4B9E10(mTop, mBottom, pPoint->y);
    }

    // 9.6f 0x41E2F0
    // https://decomp.me/scratch/TT06X
    bool AABB_Intersects_41E2F0(Fix16_Rect* pOther) const
    {
        // TODO: HACK - made 0x438FB0 a method of Fix16 to fix crazy compiler inlining behaviour/force it to do what we need
        return mLeft.IntervalIntersectsRange_438FB0_inline(this->mRight, pOther->mLeft, pOther->mRight) &&
                mTop.IntervalIntersectsRange_438FB0_inline(this->mBottom, pOther->mTop, pOther->mBottom) &&
                mLowZ.IntervalIntersectsRange_438FB0_inline( this->mHighZ, pOther->mLowZ, pOther->mHighZ) ?
            true :
            false;
    }

    // 9.6f 0x45ADD0
    Fix16 get_top_45ADD0() const
    {
        return mTop;
    }

    // 9.6f 0x45ADB0
    Fix16 get_left_45ADB0() const
    {
        return mLeft;
    }

    // 9.6f 0x45ADA0
    Fix16 get_right_45ADA0() const
    {
        return mRight;
    }

    // 9.6f 0x45ADC0
    Fix16 get_bottom_45ADC0() const
    {
        return mBottom;
    }

    // 9.6f 0x4637A0
    Fix16 get_low_z_4637A0() const
    {
        return mLowZ;
    }

    Fix16 mLeft; // 0x0
    Fix16 mRight; // 0x4
    Fix16 mTop; // 0x8
    Fix16 mBottom; // 0xC

    Fix16 mLowZ; // 0x10
    Fix16 mHighZ; // 0x14
};
