#include "Ped_Unknown_4.hpp"
#include "Car_BC.hpp"
#include "Ped.hpp"

MATCH_FUNC(0x471140)
Char_8* Ped_Unknown_4::sub_471140(Ped* pPed)
{
    Char_8* pNew = gChar_8_Pool_678b50->field_0_pool.Allocate();
    
    // Maybe PoolAlloc() ?
    pNew->field_0_char_ped = pPed;
    pNew->mpNext = this->field_0_pOwner;
    
    this->field_0_pOwner = pNew;
    return pNew;
}

STUB_FUNC(0x471160)
Char_8* Ped_Unknown_4::sub_471160(Ped* a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x4711f0)
Char_8* Ped_Unknown_4::sub_4711F0(Ped* a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x471240)
void Ped_Unknown_4::sub_471240(Ped* a1)
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x471290)
char_type Ped_Unknown_4::sub_471290()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x471320)
Ped* Ped_Unknown_4::sub_471320()
{
    NOT_IMPLEMENTED;

    Char_8* pOwner = this->field_0_pOwner;
    if (!this->field_0_pOwner)
    {
        return 0;
    }

    Ped* pPed = pOwner->field_0_char_ped;
    field_0_pOwner = pOwner->mpNext;
    pOwner->mpNext = gChar_8_Pool_678b50->field_0_pool.field_0_pHead;
    gChar_8_Pool_678b50->field_0_pool.field_0_pHead = pOwner;
    return pPed;
}

STUB_FUNC(0x4715a0)
Char_8** Ped_Unknown_4::sub_4715A0()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x4715e0)
Char_8** Ped_Unknown_4::sub_4715E0()
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x471630)
void Ped_Unknown_4::sub_471630()
{
    for (Char_8* pIter = this->field_0_pOwner; pIter; pIter = pIter->mpNext)
    {
        if (pIter->field_0_char_ped->field_240_occupation == 8)
        {
            pIter->field_0_char_ped->sub_463830(0, 9999);
            pIter->field_0_char_ped->SetObjective(objectives_enum::objective_34, 9999);
            pIter->field_0_char_ped->field_150_target_objective_car = pIter->field_0_char_ped->field_16C_car;
        }
    }
}

MATCH_FUNC(0x471680)
void Ped_Unknown_4::sub_471680()
{
    Char_8* pIter; // eax

    for (pIter = field_0_pOwner; pIter; pIter = pIter->mpNext)
    {
        if (pIter->field_0_char_ped->field_240_occupation == 7)
        {
            pIter->field_0_char_ped->field_21C |= 0x20000000u;
        }
    }
}

MATCH_FUNC(0x4716b0)
s32 Ped_Unknown_4::GetPassengerNum_4716B0()
{
    s32 pIter;
    Char_8* pOwner = this->field_0_pOwner;
    for (pIter = 0; pOwner; ++pIter)
    {
        pOwner = pOwner->mpNext;
    }
    return pIter;
}

MATCH_FUNC(0x4716d0)
void Ped_Unknown_4::sub_4716D0(Car_BC* a2)
{
    for (Char_8* pIter = this->field_0_pOwner; pIter; pIter = pIter->mpNext)
    {
        pIter->field_0_char_ped->field_204 = a2->field_70;
        pIter->field_0_char_ped->field_290 = a2->field_90;
        pIter->field_0_char_ped->field_264 = 50;
    }
}

MATCH_FUNC(0x471710)
char_type Ped_Unknown_4::sub_471710()
{
    Char_8* pOwner = this->field_0_pOwner;
    while (pOwner)
    {
        if (pOwner->field_0_char_ped->field_238 == 5)
        {
            return 1;
        }
        pOwner = pOwner->mpNext;
    }
    return 0;
}