#include "Function.hpp"
#include "fix16.hpp"

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
