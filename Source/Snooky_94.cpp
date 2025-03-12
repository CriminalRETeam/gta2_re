#include "Snooky_94.hpp"
#include "Car_BC.hpp"
#include "DrawUnk_0xBC.hpp"
#include "Globals.hpp"
#include "infallible_turing.hpp"
#include "root_sound.hpp"
#include <stdio.h>

EXPORT_VAR Snooky_94* gSnooky_94_67A830;
GLOBAL(gSnooky_94_67A830, 0x67A830);

MATCH_FUNC(0x4882d0)
Crusher_30::Crusher_30()
{
    field_18 = NULL;
}

MATCH_FUNC(0x4882e0)
Crusher_30::~Crusher_30()
{
    if (field_18)
    {
        gRoot_sound_66B038.DestroySoundObj_40FE60(field_18);
        field_18 = NULL;
    }
}

MATCH_FUNC(0x488310)
void Crusher_30::sub_488310(Car_BC* pCar)
{
    if (!field_2C && pCar->field_88 != 5 && !pCar->sub_43DC00() && pCar->field_9C != 7)
    {
        field_14_pObj = pCar;
        field_2C = 1;
        pCar->field_78_flags |= 2u;
    }
}

STUB_FUNC(0x488350)
void Crusher_30::sub_488350()
{
}

STUB_FUNC(0x4885a0)
infallible_turing* Crusher_30::sub_4885A0(Fix16 a2, Fix16 a3, char_type a4)
{
    return 0;
}

MATCH_FUNC(0x4887a0)
bool Snooky_94::sub_4887A0(Car_BC* toFind)
{
    for (s32 i=0; i < field_90_count; i++)
    {
        if (field_0[i].field_14_pObj == toFind)
        {
            return true;
        }
    }
    return false;
}

MATCH_FUNC(0x4887d0)
void Snooky_94::sub_4887D0(Car_BC* a2, u8 idx)
{
    field_0[idx].sub_488310(a2);
}

MATCH_FUNC(0x4887f0)
void Snooky_94::sub_4887F0()
{
    s32 idx = 0;
    Crusher_30* pIter = &field_0[0];
    while (idx < field_90_count)
    {
        pIter->sub_488350();
        idx++;
        pIter++;
    }
}

MATCH_FUNC(0x488820)
Crusher_30* Snooky_94::sub_488820(Fix16 a2, Fix16 a3)
{
    Crusher_30* pNew = &field_0[field_90_count];
    pNew->sub_4885A0(a2, a3, field_90_count);
    field_90_count++;
    return pNew;
}

MATCH_FUNC(0x488860)
Snooky_94::Snooky_94()
{
    field_90_count = 0;
}

MATCH_FUNC(0x4bbc30)
Snooky_94::~Snooky_94()
{
}
