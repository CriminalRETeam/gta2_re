#include "Snooky_94.hpp"
#include <stdio.h>

MATCH_FUNC(0x4882d0)
Snooky_30::Snooky_30()
{
    field_18 = NULL;
}

STUB_FUNC(0x4882e0)
void Snooky_30::dtor_4882E0()
{
}

STUB_FUNC(0x488310)
void Snooky_30::sub_488310(Car_BC* a2)
{
}

STUB_FUNC(0x488350)
void Snooky_30::sub_488350()
{
}

STUB_FUNC(0x4885a0)
infallible_turing* Snooky_30::sub_4885A0(s32 a2, s32 a3, char_type a4)
{
    return 0;
}

STUB_FUNC(0x4887a0)
char_type Snooky_94::sub_4887A0(Car_BC* toFind)
{
    return 0;
}

STUB_FUNC(0x4887d0)
void Snooky_94::sub_4887D0(Car_BC* a2, u8 idx)
{
}

MATCH_FUNC(0x4887f0)
void Snooky_94::sub_4887F0()
{
    s32 idx = 0;
    Snooky_30* pIter = &field_0[0];
    while (idx < field_90_count)
    {
        pIter->sub_488350();
        idx++;
        pIter++;
    }
}

STUB_FUNC(0x488820)
Snooky_30* Snooky_94::sub_488820(s32 a2, s32 a3)
{
    return 0;
}

STUB_FUNC(0x488860)
Snooky_94* Snooky_94::ctor_488860()
{
    return 0;
}

STUB_FUNC(0x4bbc30)
void Snooky_94::dtor_4BBC30()
{
}
