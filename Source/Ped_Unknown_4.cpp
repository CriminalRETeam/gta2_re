#include "Ped_Unknown_4.hpp"
#include "Car_BC.hpp"
#include "Ped.hpp"

MATCH_FUNC(0x471140)
Char_8* Ped_Unknown_4::AddPassenger_471140(Ped* pPed)
{
    Char_8* pNew = gChar_8_Pool_678b50->field_0_pool.Allocate();

    pNew->field_0_char_ped = pPed;

    pNew->mpNext = this->field_0_pFirstPassenger;
    this->field_0_pFirstPassenger = pNew;

    return pNew;
}

MATCH_FUNC(0x471160)
void Ped_Unknown_4::AddPassengerToBackIfMissing_471160(Ped* pPed)
{
    Char_8* pIter = this->field_0_pFirstPassenger;
    Char_8* pLast = NULL;
    if (pIter)
    {
        while (pIter)
        {
            if (pIter->field_0_char_ped == pPed)
            {
                // Already exists
                return;
            }

            pLast = pIter;
            pIter = pIter->mpNext;
        }
    }

    Char_8* pNew = gChar_8_Pool_678b50->field_0_pool.Allocate();
    pNew->field_0_char_ped = pPed;

    if (pLast)
    {
        pLast->mpNext = pNew;
        pNew->mpNext = 0;
    }
    else
    {
        // List was empty
        this->field_0_pFirstPassenger = pNew;
        pNew->mpNext = 0;
    }
}

MATCH_FUNC(0x4711B0)
void Ped_Unknown_4::AddPassengerToFrontIfMissing_4711B0(Ped* pPed)
{
    Char_8* pIter = this->field_0_pFirstPassenger;
    Char_8* pLast = NULL;
    if (pIter)
    {
        while (pIter)
        {
            if (pIter->field_0_char_ped == pPed)
            {
                // Already exists
                return;
            }

            pLast = pIter;
            pIter = pIter->mpNext;
        }
    }

    Char_8* pNew = gChar_8_Pool_678b50->field_0_pool.Allocate();
    pNew->field_0_char_ped = pPed;

    pNew->mpNext = this->field_0_pFirstPassenger;
    this->field_0_pFirstPassenger = pNew;
}

STUB_FUNC(0x4711f0)
Char_8* Ped_Unknown_4::RemovePassenger_4711F0(Ped* pPed)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x471240)
void Ped_Unknown_4::RemovePassenger_471240(Ped* pPed)
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x471290)
char_type Ped_Unknown_4::RemovePassengersInSpecificState_471290()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x4712F0)
void Ped_Unknown_4::ClearPassengers_4712F0()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x471320)
Ped* Ped_Unknown_4::RemoveFirstPassenger_471320()
{
    NOT_IMPLEMENTED;

    Char_8* pIter = this->field_0_pFirstPassenger;
    if (!this->field_0_pFirstPassenger)
    {
        return 0;
    }

    Ped* pPed = pIter->field_0_char_ped;
    field_0_pFirstPassenger = pIter->mpNext;
    pIter->mpNext = gChar_8_Pool_678b50->field_0_pool.field_0_pHead;
    gChar_8_Pool_678b50->field_0_pool.field_0_pHead = pIter;
    return pPed;
}

STUB_FUNC(0x471340)
Ped* Ped_Unknown_4::GetClosestPassengerToPoint_471340(Fix16 x, Fix16 y)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x4713C0)
Ped* Ped_Unknown_4::FindClosestPassengerInViewCone_4713C0(Fix16 x, Fix16 y, Ang16 ang1, Ang16 ang2)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x4715a0)
Char_8** Ped_Unknown_4::KillAllPassengers_4715A0()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x4715e0)
Char_8** Ped_Unknown_4::KillAllPassengersAndClearCarRef_4715E0()
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x471630)
void Ped_Unknown_4::ApplyPassengerBusStopBehavior_471630()
{
    for (Char_8* pIter = this->field_0_pFirstPassenger; pIter; pIter = pIter->mpNext)
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
void Ped_Unknown_4::ForceTaxiPassengersToExit_471680()
{
    for (Char_8* pIter = field_0_pFirstPassenger; pIter; pIter = pIter->mpNext)
    {
        if (pIter->field_0_char_ped->field_240_occupation == 7)
        {
            pIter->field_0_char_ped->field_21C |= 0x20000000u;
        }
    }
}

MATCH_FUNC(0x4716b0)
u16 Ped_Unknown_4::GetPassengerCount_4716B0()
{
    u16 passengerCount;
    Char_8* pIter = this->field_0_pFirstPassenger;
    for (passengerCount = 0; pIter; ++passengerCount)
    {
        pIter = pIter->mpNext;
    }
    return passengerCount;
}

MATCH_FUNC(0x4716d0)
void Ped_Unknown_4::SyncPassengersWithCarState_4716D0(Car_BC* pCar)
{
    for (Char_8* pIter = this->field_0_pFirstPassenger; pIter; pIter = pIter->mpNext)
    {
        pIter->field_0_char_ped->field_204 = pCar->field_70;
        pIter->field_0_char_ped->field_290 = pCar->field_90;
        pIter->field_0_char_ped->field_264 = 50;
    }
}

MATCH_FUNC(0x471710)
char_type Ped_Unknown_4::HasPassengerWith_F238_Is_5_471710()
{
    Char_8* pIter = this->field_0_pFirstPassenger;
    while (pIter)
    {
        if (pIter->field_0_char_ped->field_238 == 5)
        {
            return 1;
        }
        pIter = pIter->mpNext;
    }
    return 0;
}
