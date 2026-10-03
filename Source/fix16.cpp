#include "fix16.hpp"
#include "Function.hpp"
#include "Globals.hpp"
#include "ang16.hpp"
#include <cmath>

DEFINE_GLOBAL_ARRAY(Fix16, gSin_table_667A80, 1440, 0x667A80);
DEFINE_GLOBAL_ARRAY(Fix16, gCos_table_669260, 1440, 0x669260);

// TODO: hmm, shouldn't this be 360 entries ??
DEFINE_GLOBAL_ARRAY(Fix16, gTanTable_6663C8, 1440, 0x6663C8);
DEFINE_GLOBAL_INIT(Fix16, kFPZero_6691B0, Fix16(0), 0x6691B0);
DEFINE_GLOBAL_INIT(Ang16, kAngZero_66A920, Ang16(0), 0x66A920);
DEFINE_GLOBAL_INIT(Ang16, kAng180_669156, Ang16(720), 0x669156);
DEFINE_GLOBAL_INIT(Ang16, kAng90_667A7C, Ang16(360), 0x667A7C);
DEFINE_GLOBAL_INIT(Ang16, kAng270_66916C, Ang16(1080), 0x66916C);

MATCH_FUNC(0x408660)
Fix16 Fix16::operator+(const Fix16& rhs) const throw()
{
    s32 value = mValue + rhs.mValue;
    return Fix16(value, 0);
}

MATCH_FUNC(0x408680)
Fix16 Fix16::Multiply_408680(const Fix16& in) const throw()
{
    s32 value = (s32)((mValue * (__int64)in.mValue) >> 14);
    return Fix16(value, 0);
}

MATCH_FUNC(0x436A00)
Fix16 Fix16::Subtract_436A00(const Fix16& in) const
{
    s32 value = mValue - in.mValue;
    return Fix16(value, 0);
}

MATCH_FUNC(0x436A20)
Fix16 Fix16::Divide_436A20(const Fix16& in) const
{
    s32 value = (s32)(((__int64)mValue << 14) / in.mValue);
    return Fix16(value, 0);
}

MATCH_FUNC(0x451670)
s32 Fix16::IsLess_451670(const Fix16& other) const
{
    return mValue < other.mValue;
}

MATCH_FUNC(0x451690)
s32 Fix16::IsGreater_451690(const Fix16& other) const
{
    return mValue > other.mValue;
}

MATCH_FUNC(0x539F90)
Fix16& Fix16::DivideAssign_539F90(const Fix16& rhs)
{
    mValue = (s32)(((__int64)mValue << 14) / rhs.mValue);
    return *this;
}

// Out-of-line copy of operator/(const s32&) (20 bytes, called by Particle_8::EmitImpactParticles_53FE40)
MATCH_FUNC(0x53E860)
Fix16 Fix16::DivideInt_53E860(const s32& in) const
{
    s32 value = mValue / in;
    return Fix16(value, 0);
}

MATCH_FUNC(0x561DB0)
Fix16 Fix16::MultiplyInt_561DB0(const s32& in) const
{
    s32 value = mValue * in;
    return Fix16(value, 0);
}

MATCH_FUNC(0x562430)
Fix16& Fix16::MultiplyAssign_562430(const Fix16& rhs)
{
    mValue = (s32)((mValue * (__int64)rhs.mValue) >> 14);
    return *this;
}

MATCH_FUNC(0x4086A0)
Fix16 Fix16::Negate_4086A0() const throw()
{
    return Fix16(-mValue, 0);
}

MATCH_FUNC(0x44E540)
Fix16 __stdcall Fix16::Max_44E540(Fix16& pLhs, Fix16& pRhs)
{
    Fix16 result;
    if (pLhs.mValue > pRhs.mValue)
    {
        result.mValue = pLhs.mValue;
    }
    else
    {
        result.mValue = pRhs.mValue;
    }
    return result;
}

/*
MATCH_FUNC(0x436A20)
Fix16 Fix16::operator/(const Fix16& in)
{
    s32 value = ((__int64)mValue << 14) / in.mValue;
    return Fix16(value, 0);
}
*/

MATCH_FUNC(0x436A50)
Fix16 __stdcall Fix16::Abs_436A50(Fix16& input)
{
    if (input.mValue > 0)
    {
        return input;
    }
    else
    {
        return -input;
    }
}

MATCH_FUNC(0x436A70)
Fix16 __stdcall Fix16::SquareRoot_436A70(Fix16& input)
{
    return Fix16(sqrt(input.AsDouble()));
}

// 10.5 https://decomp.me/scratch/7a41K
// 9.6f https://decomp.me/scratch/ZkUbq
MATCH_FUNC(0x405320)
Ang16 __stdcall Fix16::atan2_fixed_405320(Fix16& x, Fix16& y)
{
    Ang16 v9;
    if (y == kFPZero_6691B0)
    {
        if (x >= kFPZero_6691B0)
        {
            return kAngZero_66A920;
        }
        else
        {
            return kAng180_669156;
        }
    }
    else if (x == kFPZero_6691B0)
    {
        if (y > kFPZero_6691B0)
        {
            return kAng90_667A7C;
        }
        else
        {
            return kAng270_66916C;
        }
    }
    else
    {
        v9 = ArcTanLookup_405500(Fix16::Abs(x / y));

        if (x > kFPZero_6691B0)
        {
            if (y > kFPZero_6691B0)
            {
                return kAng90_667A7C - v9;
            }
            else
            {
                if (v9 == kAng90_667A7C)
                {
                    return kAngZero_66A920;
                }
                else
                {
                    return kAng270_66916C + v9;
                }
            }
        }
        else
        {
            if (y > kFPZero_6691B0)
            {
                return kAng90_667A7C + v9;
            }
            else
            {
                return kAng270_66916C - v9;
            }
        }
    }
}

MATCH_FUNC(0x438FB0)
EXPORT bool __stdcall IntervalIntersectsRange_438FB0(const Fix16& intervalStart,
                                                     const Fix16& intervalEnd,
                                                     const Fix16& rangeMin,
                                                     const Fix16& rangeMax)
{
    if (intervalStart < rangeMin)
    {
        return intervalEnd >= rangeMin ? true : false;
    }
    else
    {
        return intervalStart <= rangeMax ? true : false;
    }
}

EXTERN_GLOBAL(Fix16, kFPZero_6691B0);
EXTERN_GLOBAL(Fix16, kAngFix16FullCircle_66A8E4);
EXTERN_GLOBAL(Fix16, kAngFix16HalfCircle_6691EC);

// Turns the angle `cur` toward `*pTarget` by at most `*pSpeed`, the short way round, and wraps the
// result into [0, 2pi). Called by Trailer::UpdateTrailerAlignment_407CE0.
MATCH_FUNC(0x405DA0)
EXPORT Fix16 __stdcall sub_405DA0(Fix16 cur, Fix16* pTarget, Fix16* pSpeed)
{
    if (*pTarget - cur > kAngFix16HalfCircle_6691EC)
    {
        cur += kAngFix16FullCircle_66A8E4;
    }
    else if (*pTarget - cur < -kAngFix16HalfCircle_6691EC)
    {
        cur -= kAngFix16FullCircle_66A8E4;
    }

    Fix16 diff = *pTarget - cur;
    if (diff > kFPZero_6691B0)
    {
        if (diff > *pSpeed)
        {
            diff = *pSpeed;
        }
    }
    else if (diff < kFPZero_6691B0)
    {
        if (diff < -*pSpeed)
        {
            diff = -*pSpeed;
        }
    }

    Fix16 result = diff + cur;
    for (; result < kFPZero_6691B0; result += kAngFix16FullCircle_66A8E4)
    {
        ;
    }
    for (; result >= kAngFix16FullCircle_66A8E4; result -= kAngFix16FullCircle_66A8E4)
    {
        ;
    }
    return result;
}

DEFINE_GLOBAL(Fix16, dword_66A924, 0x66A924);
DEFINE_GLOBAL(Fix16, dword_669140, 0x669140);
DEFINE_GLOBAL(Fix16, dword_6691FC, 0x6691FC);

// Is `*a` within dword_66A924 of `*b`, directly or one turn (kAngFix16FullCircle_66A8E4) either way.
MATCH_FUNC(0x405E20)
EXPORT s32 __stdcall sub_405E20(Fix16* a, Fix16* b)
{
    if ((*a > *b - dword_66A924 && *a < *b + dword_66A924) ||
        (*a > *b - kAngFix16FullCircle_66A8E4 - dword_66A924 && *a < *b - kAngFix16FullCircle_66A8E4 + dword_66A924) ||
        (*a > *b - dword_66A924 + kAngFix16FullCircle_66A8E4 && *a < *b + kAngFix16FullCircle_66A8E4 + dword_66A924))
    {
        return 1;
    }
    return 0;
}

// Clamps the angle `*pCur` into the window of +-dword_669140 around `*pTarget`, allowing for the
// wrap at 0 / 2pi, and returns whether it ends up on either edge. Called by
// Trailer::UpdateTrailerAlignment_407CE0.
MATCH_FUNC(0x405E80)
EXPORT s32 __stdcall sub_405E80(Fix16* pTarget, Fix16* pCur)
{
    Fix16 lo;
    Fix16 hi;
    if (*pTarget < dword_669140)
    {
        lo = dword_6691FC + *pTarget;
        hi = *pTarget + dword_669140;
        if (*pCur > lo - dword_669140 && *pCur < lo)
        {
            *pCur = lo;
        }
        else if (*pCur > hi && *pCur < lo)
        {
            *pCur = hi;
        }
    }
    else if (*pTarget < kAngFix16HalfCircle_6691EC)
    {
        lo = *pTarget - dword_669140;
        hi = *pTarget + dword_669140;
        if (*pCur < lo || *pCur > hi + dword_669140)
        {
            *pCur = lo;
        }
        else if (*pCur > hi)
        {
            *pCur = hi;
        }
    }
    else if (*pTarget < dword_6691FC)
    {
        lo = *pTarget - dword_669140;
        hi = *pTarget + dword_669140;
        if (*pCur < hi - dword_6691FC || *pCur > hi)
        {
            *pCur = hi;
        }
        else if (*pCur < lo)
        {
            *pCur = lo;
        }
    }
    else
    {
        lo = *pTarget - dword_669140;
        hi = *pTarget - dword_6691FC;
        if (*pCur > hi && *pCur < hi + dword_669140)
        {
            *pCur = hi;
        }
        else if (*pCur < lo && *pCur > hi)
        {
            *pCur = lo;
        }
    }

    if ((u8)sub_405E20(pCur, &lo) || (u8)sub_405E20(pCur, &hi))
    {
        return 1;
    }
    return 0;
}

// The original is a CRT init func (called from the CRT init table), here Init_trigonometry_tables
// calls it. The constants are pi and 1/720: 1440 steps of the full circle.
MATCH_FUNC(0x4052D0)
EXPORT void __stdcall arc_tan_table_init_4052D0()
{
    s32 arg = 0;
    Fix16* pTan = gTanTable_6663C8;
    for (s32 i = 1440; i != 0; i--)
    {
        f64 radians = arg * 3.141592654;
        *pTan = Fix16(tan(radians * 0.001388888888888889));
        arg++;
        pTan++;
    }
}

// 9.6f: 0x40E810
MATCH_FUNC(0x405500)
EXPORT Ang16 __stdcall ArcTanLookup_405500(const Fix16& targetTan)
{
    s16 low = 0;
    s16 high = 360 - 1;

    while (true)
    {
        s16 mid = (low + high) >> 1;
        Fix16 tanAtMid = gTanTable_6663C8[mid];

        // Binary search: go left if target < tanAtMid,
        // go right if target > tanAtMid,
        // exact match if equal
        if (targetTan < tanAtMid)
        {
            high = mid - 1;
        }
        else if (targetTan > tanAtMid)
        {
            low = mid + 1;
        }
        else
        {
            return Ang16(mid, 0);
        }

        if (low > high)
        {
            // Lower bound case
            if (mid == (0 + 1))
            {
                if (targetTan < gTanTable_6663C8[0 + 1])
                {
                    mid = 0;
                }
            }
            // Upper bound case
            else if (mid == (360 - 1))
            {
                if (targetTan > gTanTable_6663C8[360 - 1])
                {
                    mid = 360;
                }
            }
            return Ang16(mid, 0);
        }
    }
}

MATCH_FUNC(0x5A57E0)
EXPORT void __stdcall FindMinMax_5A57E0(Fix16& minOut, Fix16& maxOut, const Fix16& v1, const Fix16& v2, const Fix16& v3, const Fix16& v4)
{
    minOut = v1;
    maxOut = v1;

    // v2
    if (v2 < minOut)
    {
        minOut = v2;
    }
    else if (v2 > maxOut)
    {
        maxOut = v2;
    }

    // v3
    if (v3 < minOut)
    {
        minOut = v3;
    }
    else if (v3 > maxOut)
    {
        maxOut = v3;
    }

    // v4
    if (v4 < minOut)
    {
        minOut = v4;
    }
    else if (v4 > maxOut)
    {
        maxOut = v4;
    }
}

void Init_trigonometry_tables()
{
    s16 arg = 0; 
    for (u32 idx = 0; idx < GTA2_COUNTOF(gSin_table_667A80); idx++, arg++)
    {
        gSin_table_667A80[idx] = Fix16( sin(((f64)arg / 1440.0) * 2 * 3.141592654) );
    }
    arg = 0;
    for (u32 entry = 0; entry < GTA2_COUNTOF(gCos_table_669260); entry++, arg++)
    {
        gCos_table_669260[entry] = Fix16( cos(((f64)arg / 1440.0) * 2 * 3.141592654) );
    }
    arc_tan_table_init_4052D0();
    printf("Sine, cosine and tangent tables initialized!\n");
}
