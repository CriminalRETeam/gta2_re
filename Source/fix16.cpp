#include "fix16.hpp"
#include "Function.hpp"
#include "Globals.hpp"
#include <cmath>

DEFINE_GLOBAL_ARRAY(Fix16, gSin_table_667A80, 1440, 0x667A80);
DEFINE_GLOBAL_ARRAY(Fix16, gCos_table_669260, 1440, 0x669260);

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

MATCH_FUNC(0x438FB0)
EXPORT bool __stdcall IntervalIntersectsRange_438FB0(const Fix16& intervalStart, const Fix16& intervalEnd, const Fix16& rangeMin, const Fix16& rangeMax) 
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
