#include "Crushers.hpp"
#include "Camera.hpp"
#include "Car_BC.hpp"
#include "Globals.hpp"
#include "infallible_turing.hpp"
#include "root_sound.hpp"
#include <stdio.h>

DEFINE_GLOBAL(CrusherPool_94*, gCrusherPool_94_67A830, 0x67A830);

MATCH_FUNC(0x4882d0)
Crusher_30::Crusher_30()
{
    field_18_sound = NULL;
}

MATCH_FUNC(0x4882e0)
Crusher_30::~Crusher_30()
{
    if (field_18_sound)
    {
        gRoot_sound_66B038.DestroySoundObj_40FE60(field_18_sound);
        field_18_sound = NULL;
    }
}

MATCH_FUNC(0x488310)
void Crusher_30::CrushCar_488310(Car_BC* pCar)
{
    if (field_2C_state == 0 && pCar->field_88 != 5 && !pCar->sub_43DC00() && pCar->field_9C != 7)
    {
        field_14_pObj = pCar;
        field_2C_state = 1;
        pCar->field_78_flags |= 2u;
    }
}

STUB_FUNC(0x488350)
void Crusher_30::Service_488350()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x4885a0)
void Crusher_30::InitCrusher_4885A0(Fix16 xpos, Fix16 ypos, char_type crusher_idx)
{
    NOT_IMPLEMENTED;
}

MATCH_FUNC(0x4887a0)
bool CrusherPool_94::IsCarBeingCrushed_4887A0(Car_BC* pCar)
{
    for (s32 i = 0; i < field_90_count; i++)
    {
        if (field_0[i].field_14_pObj == pCar)
        {
            return true;
        }
    }
    return false;
}

MATCH_FUNC(0x4887d0)
void CrusherPool_94::CrushCarWithCrusher_4887D0(Car_BC* pCar, u8 crusher_idx)
{
    field_0[crusher_idx].CrushCar_488310(pCar);
}

MATCH_FUNC(0x4887f0)
void CrusherPool_94::CrushersService_4887F0()
{
    s32 idx = 0;
    Crusher_30* pIter = &field_0[0];
    while (idx < field_90_count)
    {
        pIter->Service_488350();
        idx++;
        pIter++;
    }
}

MATCH_FUNC(0x488820)
Crusher_30* CrusherPool_94::CreateCrusher_488820(Fix16 xpos, Fix16 ypos)
{
    Crusher_30* pNew = &field_0[field_90_count];
    pNew->InitCrusher_4885A0(xpos, ypos, field_90_count);
    field_90_count++;
    return pNew;
}

MATCH_FUNC(0x488860)
CrusherPool_94::CrusherPool_94()
{
    field_90_count = 0;
}

MATCH_FUNC(0x4bbc30)
CrusherPool_94::~CrusherPool_94()
{
}
