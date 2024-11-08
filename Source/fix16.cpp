#include "Function.hpp"
#include "fix16.hpp"

MATCH_FUNC(0x4369F0)
Fix16 &Fix16::FromInt_4369F0(s32 a2)
{
    mValue = a2 << 14;
    return *this;
}

MATCH_FUNC(0x45C4E0)
Fix16 &Fix16::FromInt_45C4E0(u8 a2)
{
    mValue = a2 << 14;
    return *this;
}

MATCH_FUNC(0x408680)
Fix16 Fix16::operator*(const Fix16 &in)
{
    Fix16 ret;
    ret.mValue = (mValue * (__int64)in.mValue) >> 14;
    return ret;
}

MATCH_FUNC(0x4086A0)
Fix16 Fix16::operator-()
{
    Fix16 ret;
    ret.mValue = -mValue;
    return ret;
}
