#include "Ped_Unknown_4.hpp"
#include "Ped.hpp"
#include "Car_BC.hpp"

STUB_FUNC(0x471140)
Char_8* Ped_Unknown_4::sub_471140(Ped* pPed)
{
    return 0;
}

STUB_FUNC(0x471160)
Char_8* Ped_Unknown_4::sub_471160(Ped* a2)
{
    return 0;
}

STUB_FUNC(0x4711f0)
Char_8* Ped_Unknown_4::sub_4711F0(Ped* a2)
{
    return 0;
}

STUB_FUNC(0x471290)
char_type Ped_Unknown_4::sub_471290()
{
    return 0;
}

STUB_FUNC(0x471320)
Ped* Ped_Unknown_4::sub_471320()
{
    return 0;
}

STUB_FUNC(0x4715a0)
Char_8** Ped_Unknown_4::sub_4715A0()
{
    return 0;
}

STUB_FUNC(0x4715e0)
Char_8** Ped_Unknown_4::sub_4715E0()
{
    return 0;
}

MATCH_FUNC(0x471630)
void Ped_Unknown_4::sub_471630()
{
    for (Char_8* pIter = this->field_0_pOwner; pIter; pIter = pIter->field_4_pOwner)
    {
        if (pIter->field_0_char_ped->field_240_occupation == 8)
        {
            pIter->field_0_char_ped->sub_463830(0, 9999);
            pIter->field_0_char_ped->sub_463570(34, 9999);
            pIter->field_0_char_ped->field_150_target_objective_car = pIter->field_0_char_ped->field_16C_car;
        }
    }
}

MATCH_FUNC(0x471680)
void Ped_Unknown_4::sub_471680()
{
    Char_8* pIter; // eax

    for (pIter = field_0_pOwner; pIter; pIter = pIter->field_4_pOwner)
    {
        if (pIter->field_0_char_ped->field_240_occupation == 7)
        {
            pIter->field_0_char_ped->field_21C |= 0x20000000u;
        }
    }
}

STUB_FUNC(0x4716b0)
s32 Ped_Unknown_4::sub_4716B0()
{
    return 0;
}

MATCH_FUNC(0x4716d0)
void Ped_Unknown_4::sub_4716D0(Car_BC* a2)
{
    for (Char_8* pIter = this->field_0_pOwner; pIter; pIter = pIter->field_4_pOwner)
    {
        pIter->field_0_char_ped->field_204 = a2->field_70;
        pIter->field_0_char_ped->field_290 = a2->field_90;
        pIter->field_0_char_ped->field_264 = 50;
    }
}

STUB_FUNC(0x471710)
char_type Ped_Unknown_4::sub_471710()
{
    return 0;
}
