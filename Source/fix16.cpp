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
Fix16 Fix16::Abs_436A50(Fix16& a2)
{
    s32 value = a2.mValue;
    if (a2.mValue <= 0)
    {
        value = -a2.mValue;
    }
    return Fix16(value, 0);
}

MATCH_FUNC(0x436A70)
Fix16 Fix16::SquareRoot_436A70(Fix16& a2)
{
    s32 value = sqrt(a2.AsDouble()) * 16384.0f;
    return Fix16(value, 0);
}