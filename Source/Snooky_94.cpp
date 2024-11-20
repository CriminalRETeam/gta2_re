#include "Snooky_94.hpp"
#include "Car_BC.hpp"
#include "DrawUnk_0xBC.hpp"
#include "infallible_turing.hpp"
#include "root_sound.hpp"
#include <stdio.h>

MATCH_FUNC(0x4882d0)
Snooky_30::Snooky_30()
{
    field_18 = NULL;
}

MATCH_FUNC(0x4882e0)
Snooky_30::~Snooky_30()
{
    if (field_18)
    {
        infallible_turing* pOld = field_18;
        pOld->release_40EF20();
        pOld->field_C_pAny = gRoot_sound_66B038.field_0;
        gRoot_sound_66B038.field_0 = pOld;
        field_18 = NULL;
    }
}

MATCH_FUNC(0x488310)
void Snooky_30::sub_488310(Car_BC* pCar)
{
    if (!field_2C && pCar->field_88 != 5 && !pCar->sub_43DC00() && pCar->field_9C != 7)
    {
        field_14_pObj = pCar;
        field_2C = 1;
        pCar->field_78_flags |= 2u;
    }
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
