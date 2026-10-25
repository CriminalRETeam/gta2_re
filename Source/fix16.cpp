#include "fix16.hpp"
#include "Function.hpp"
#include "Globals.hpp"
#include "ang16.hpp"
#include <cmath>

DEFINE_GLOBAL_ARRAY(Fix16, gSin_table_667A80, 1440, 0x667A80);
DEFINE_GLOBAL_ARRAY(Fix16, gCos_table_669260, 1440, 0x669260);

DEFINE_GLOBAL_ARRAY(Fix16, gTanTable_6663C8, 1440, 0x6663C8);
DEFINE_GLOBAL_INIT(Fix16, kFPZero_6691B0, Fix16(0), 0x6691B0);
DEFINE_GLOBAL_INIT(Ang16, kAngZero_66A920, Ang16(0), 0x66A920);
DEFINE_GLOBAL_INIT(Ang16, kAng180_669156, Ang16(720), 0x669156);
DEFINE_GLOBAL_INIT(Ang16, kAng90_667A7C, Ang16(360), 0x667A7C);
DEFINE_GLOBAL_INIT(Ang16, kAng270_66916C, Ang16(1080), 0x66916C);

MATCH_FUNC(0x408660)
Fix16 Fix16::Add_408660(const Fix16& rhs) const throw()
{
    s32 value = mValue + rhs.mValue;
    return Fix16(value, 0);
}

MATCH_FUNC(0x408680)
Fix16 Fix16::Multiply_408680(const Fix16& in) const throw()
{
    s32 value = static_cast<s32>((mValue * static_cast<__int64>(in.mValue)) >> 14);
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
    s32 value = static_cast<s32>((static_cast<__int64>(mValue) << 14) / in.mValue);
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
    mValue = static_cast<s32>((static_cast<__int64>(mValue) << 14) / rhs.mValue);
    return *this;
}

// Out-of-line copy of operator/(const s32&) (20 bytes, called by ParticleManager::EmitImpactParticles_53FE40)
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
    mValue = static_cast<s32>((mValue * static_cast<__int64>(rhs.mValue)) >> 14);
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
Ang16 __stdcall Fix16::atan2_fixed_405320(Fix16& y, Fix16& x)
{
    Ang16 table_angle;
    if (x == kFPZero_6691B0)
    {
        if (y >= kFPZero_6691B0)
        {
            return kAngZero_66A920;
        }
        else
        {
            return kAng180_669156;
        }
    }
    else if (y == kFPZero_6691B0)
    {
        if (x > kFPZero_6691B0)
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
        table_angle = ArcTanLookup_405500(Fix16::Abs(y / x));

        if (y > kFPZero_6691B0)
        {
            if (x > kFPZero_6691B0)
            {
                return kAng90_667A7C - table_angle;
            }
            else
            {
                if (table_angle == kAng90_667A7C)
                {
                    return kAngZero_66A920;
                }
                else
                {
                    return kAng270_66916C + table_angle;
                }
            }
        }
        else
        {
            if (x > kFPZero_6691B0)
            {
                return kAng90_667A7C + table_angle;
            }
            else
            {
                return kAng270_66916C - table_angle;
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
EXPORT Fix16 __stdcall TurnAngleToward_405DA0(Fix16 cur, Fix16* pTarget, Fix16* pSpeed)
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

DEFINE_GLOBAL_INIT(Fix16, kAngFix16OneDegree_66A924, Fix16(0x11C, 0), 0x66A924);
DEFINE_GLOBAL_INIT(Fix16, kAngFix16QuarterCircle_669140, Fix16(0x63D8, 0), 0x669140);
DEFINE_GLOBAL_INIT(Fix16, kAngFix16ThreeQuarterCircle_6691FC, Fix16(0x12B88, 0), 0x6691FC);

// Is `*a` within kAngFix16OneDegree_66A924 of `*b`, directly or one turn (kAngFix16FullCircle_66A8E4) either way.
MATCH_FUNC(0x405E20)
EXPORT s32 __stdcall IsAngleWithinTolerance_405E20(Fix16* a, Fix16* b)
{
    if ((*a > *b - kAngFix16OneDegree_66A924 && *a < *b + kAngFix16OneDegree_66A924) ||
        (*a > *b - kAngFix16FullCircle_66A8E4 - kAngFix16OneDegree_66A924 && *a < *b - kAngFix16FullCircle_66A8E4 + kAngFix16OneDegree_66A924) ||
        (*a > *b - kAngFix16OneDegree_66A924 + kAngFix16FullCircle_66A8E4 && *a < *b + kAngFix16FullCircle_66A8E4 + kAngFix16OneDegree_66A924))
    {
        return 1;
    }
    return 0;
}

// Clamps the angle `*pCur` into the window of +-kAngFix16QuarterCircle_669140 around `*pTarget`, allowing for the
// wrap at 0 / 2pi, and returns whether it ends up on either edge. Called by
// Trailer::UpdateTrailerAlignment_407CE0.
MATCH_FUNC(0x405E80)
EXPORT s32 __stdcall ClampAngleAroundTarget_405E80(Fix16* pTarget, Fix16* pCur)
{
    Fix16 lo;
    Fix16 hi;
    if (*pTarget < kAngFix16QuarterCircle_669140)
    {
        lo = kAngFix16ThreeQuarterCircle_6691FC + *pTarget;
        hi = *pTarget + kAngFix16QuarterCircle_669140;
        if (*pCur > lo - kAngFix16QuarterCircle_669140 && *pCur < lo)
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
        lo = *pTarget - kAngFix16QuarterCircle_669140;
        hi = *pTarget + kAngFix16QuarterCircle_669140;
        if (*pCur < lo || *pCur > hi + kAngFix16QuarterCircle_669140)
        {
            *pCur = lo;
        }
        else if (*pCur > hi)
        {
            *pCur = hi;
        }
    }
    else if (*pTarget < kAngFix16ThreeQuarterCircle_6691FC)
    {
        lo = *pTarget - kAngFix16QuarterCircle_669140;
        hi = *pTarget + kAngFix16QuarterCircle_669140;
        if (*pCur < hi - kAngFix16ThreeQuarterCircle_6691FC || *pCur > hi)
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
        lo = *pTarget - kAngFix16QuarterCircle_669140;
        hi = *pTarget - kAngFix16ThreeQuarterCircle_6691FC;
        if (*pCur > hi && *pCur < hi + kAngFix16QuarterCircle_669140)
        {
            *pCur = hi;
        }
        else if (*pCur < lo && *pCur > hi)
        {
            *pCur = lo;
        }
    }

    if (static_cast<u8>(IsAngleWithinTolerance_405E20(pCur, &lo)) || static_cast<u8>(IsAngleWithinTolerance_405E20(pCur, &hi)))
    {
        return 1;
    }
    return 0;
}

// The trigonometry tables are filled by static initialisers. The constants are pi and 1/720:
// 1440 steps of the full circle.
struct SinTableInit
{
    SinTableInit()
    {
        s32 arg = 0;
        Fix16* pSin = gSin_table_667A80;
        for (s32 i = 1440; i != 0; i--)
        {
            f64 radians = arg * 3.141592654;
            *pSin = Fix16(sin(radians * 0.001388888888888889));
            arg++;
            pSin++;
        }
    }
};
static SinTableInit sSinTableInit;

struct CosTableInit
{
    CosTableInit()
    {
        s32 arg = 0;
        Fix16* pCos = gCos_table_669260;
        for (s32 i = 1440; i != 0; i--)
        {
            f64 radians = arg * 3.141592654;
            *pCos = Fix16(cos(radians * 0.001388888888888889));
            arg++;
            pCos++;
        }
    }
};
static CosTableInit sCosTableInit;

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

// In 10.5, arc_tan_table_init_4052D0 is the generated initialiser of this object (its inlined constructor)
struct TanTableInit
{
    TanTableInit()
    {
        arc_tan_table_init_4052D0();
    }
};
static TanTableInit sTanTableInit;

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
EXPORT void __stdcall FindMinMax_5A57E0(Fix16& minOut, Fix16& maxOut, const Fix16& a, const Fix16& b, const Fix16& c, const Fix16& d)
{
    minOut = a;
    maxOut = a;

    // b
    if (b < minOut)
    {
        minOut = b;
    }
    else if (b > maxOut)
    {
        maxOut = b;
    }

    // c
    if (c < minOut)
    {
        minOut = c;
    }
    else if (c > maxOut)
    {
        maxOut = c;
    }

    // d
    if (d < minOut)
    {
        minOut = d;
    }
    else if (d > maxOut)
    {
        maxOut = d;
    }
}
