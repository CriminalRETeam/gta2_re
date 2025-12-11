#include "fix16.hpp"
#include "Function.hpp"
#include "Globals.hpp"
#include "ang16.hpp"
#include <cmath>

DEFINE_GLOBAL_ARRAY(Fix16, gSin_table_667A80, 1440, 0x667A80);
DEFINE_GLOBAL_ARRAY(Fix16, gCos_table_669260, 1440, 0x669260);

// TODO: hmm, shouldn't this be 360 entries ??
DEFINE_GLOBAL_ARRAY(Fix16, gTanTable_6663C8, 1440, 0x6663C8);

MATCH_FUNC(0x4369F0)
Fix16& Fix16::FromInt_4369F0(s32 a2)
{
    mValue = a2 << 14;
    return *this;
}

MATCH_FUNC(0x45C4E0)
Fix16& Fix16::FromInt_45C4E0(u8 a2)
{
    mValue = a2 << 14;
    return *this;
}

MATCH_FUNC(0x4AE970)
Fix16& Fix16::FromU16_4AE970(u16 a2)
{
    mValue = a2 << 14;
    return *this;
}

MATCH_FUNC(0x408660)
Fix16 Fix16::operator+(const Fix16& rhs) const
{
    s32 value = mValue + rhs.mValue;
    return Fix16(value, 0);
}

MATCH_FUNC(0x44E540)
Fix16 Fix16::Max_44E540(Fix16& pLhs, Fix16& pRhs)
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
inline Fix16 __stdcall Fix16::Abs_436A50(Fix16& input)
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
inline Fix16 __stdcall Fix16::SquareRoot_436A70(Fix16& input)
{
    return Fix16(sqrt(input.AsDouble()));
}

STUB_FUNC(0x405320)
Ang16 Fix16::atan2_fixed_405320(Fix16& pMaybeX_FP16, Fix16& pMaybeY_FP16)
{
    NOT_IMPLEMENTED;
    return Ang16(0, 0);
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

// TODO: A crt init func, needs adding to the CRT init table
STUB_FUNC(0x4052D0)
EXPORT void __stdcall arc_tan_table_init_4052D0()
{
    for (s32 i = 0; i < 1440; i++)
    {
        // TODO: Probably construct Ang16(i, 0) and then had ToRadians() or something 
        gTanTable_6663C8[i].mValue = (tan((double)i * 3.141592654 * 0.001388888888888889) * 16384.0);
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
