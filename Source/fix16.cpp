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
DEFINE_GLOBAL_INIT(Ang16, word_669156, Ang16(720), 0x669156);
DEFINE_GLOBAL_INIT(Ang16, word_667A7C, Ang16(360), 0x667A7C);
DEFINE_GLOBAL_INIT(Ang16, word_66916C, Ang16(1080), 0x66916C);

MATCH_FUNC(0x408660)
Fix16 Fix16::operator+(const Fix16& rhs) const
{
    s32 value = mValue + rhs.mValue;
    return Fix16(value, 0);
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

// 10.5 https://decomp.me/scratch/7a41K
// 9.6f https://decomp.me/scratch/ZkUbq
WIP_FUNC(0x405320)
Ang16 __stdcall Fix16::atan2_fixed_405320(Fix16& pMaybeX_FP16, Fix16& pMaybeY_FP16)
{
    Ang16 v9;
    if (pMaybeY_FP16 == kFPZero_6691B0)
    {
        if (pMaybeX_FP16 >= kFPZero_6691B0)
        {
            return kAngZero_66A920;
        }
        else
        {
            return word_669156;
        }
    }
    else if (pMaybeX_FP16 == kFPZero_6691B0)
    {
        if (pMaybeY_FP16 > kFPZero_6691B0)
        {
            return word_667A7C;
        }
        else
        {
            return word_66916C;
        }
    }
    else
    {
        v9 = ArcTanLookup_405500(Fix16::Abs(pMaybeX_FP16 / pMaybeY_FP16));

        if (pMaybeX_FP16 > kFPZero_6691B0)
        {
            if (pMaybeY_FP16 > kFPZero_6691B0)
            {
                return word_667A7C - v9;
            }
            else
            {
                if (v9 == word_667A7C)
                {
                    return kAngZero_66A920;
                }
                else
                {
                    return word_66916C + v9;
                }
            }
        }
        else
        {
            if (pMaybeY_FP16 > kFPZero_6691B0)
            {
                return word_667A7C + v9;
            }
            else
            {
                return word_66916C - v9;
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

// TODO: A crt init func, needs adding to the CRT init table
STUB_FUNC(0x4052D0)
EXPORT void __stdcall arc_tan_table_init_4052D0()
{
    for (s32 i = 0; i < 1440; i++)
    {
        // TODO: Probably construct Ang16(i, 0) and then had ToRadians() or something
        gTanTable_6663C8[i] = Fix16(tan( ((f64)i / 1440.0) * 2 * 3.141592654));
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
