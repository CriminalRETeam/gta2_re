#include "Ped_List_4.hpp"
#include "Car_BC.hpp"
#include "Char_Pool.hpp"
#include "Ped.hpp"

DEFINE_GLOBAL(Ped_List_4, gThreateningPedsList_678468, 0x678468);

MATCH_FUNC(0x471140)
Char_8* Ped_List_4::AddPed_471140(Ped* pPed)
{
    Char_8* pNew = gChar_8_Pool_678b50->field_0_pool.Allocate();

    pNew->field_0_char_ped = pPed;

    pNew->mpNext = field_0_pFirstPed;
    field_0_pFirstPed = pNew;

    return pNew;
}

MATCH_FUNC(0x471160)
void Ped_List_4::AddPedToBackIfMissing_471160(Ped* pPed)
{
    Char_8* pIter = field_0_pFirstPed;
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
        field_0_pFirstPed = pNew;
        pNew->mpNext = 0;
    }
}

MATCH_FUNC(0x4711B0)
void Ped_List_4::AddPedToFrontIfMissing_4711B0(Ped* pPed)
{
    Char_8* pIter = field_0_pFirstPed;
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

    pNew->mpNext = field_0_pFirstPed;
    field_0_pFirstPed = pNew;
}

MATCH_FUNC(0x4711f0)
void Ped_List_4::RemovePed_4711F0(Ped* pPed)
{
    Char_8* pIter = field_0_pFirstPed;
    Char_8* pLast = NULL;
    while (pIter)
    {
        if (pIter->field_0_char_ped == pPed)
        {
            if (pLast)
            {
                pLast->mpNext = pIter->mpNext;
            }
            else
            {
                field_0_pFirstPed = pIter->mpNext;
            }
            gChar_8_Pool_678b50->field_0_pool.DeAllocate(pIter);
            break;
        }
        pLast = pIter;
        pIter = pIter->mpNext;
    }
}

MATCH_FUNC(0x471240)
void Ped_List_4::RemovePed_471240(Ped* pPed)
{
    Char_8* pIter = field_0_pFirstPed;
    Char_8* pLast = NULL;
    while (pIter)
    {
        if (pIter->field_0_char_ped == pPed)
        {
            if (pLast)
            {
                pLast->mpNext = pIter->mpNext;
            }
            else
            {
                field_0_pFirstPed = pIter->mpNext;
            }
            gChar_8_Pool_678b50->field_0_pool.DeAllocate(pIter);
            break;
        }
        pLast = pIter;
        pIter = pIter->mpNext;
    }
}

MATCH_FUNC(0x471290)
char_type Ped_List_4::RemovePedsInSpecificState_471290()
{
    Char_8* pLast = 0;
    Char_8* pIter = field_0_pFirstPed;
    char_type removedCount = 0;
    while (pIter)
    {
        if ((pIter->field_0_char_ped->field_21C & 1) != 0 
            && pIter->field_0_char_ped->field_278_ped_state == ped_state_1::dead_9)
        {
            pLast = pIter;
            pIter = pIter->mpNext;
        }
        else
        {
            if (pLast)
            {
                pLast->mpNext = pIter->mpNext;
            }
            else
            {
                field_0_pFirstPed = pIter->mpNext;
            }

            Char_8* pIterOldNext = pIter->mpNext;

            gChar_8_Pool_678b50->field_0_pool.DeAllocate(pIter);
            pIter = pIterOldNext;

            ++removedCount;
        }
    }
    return removedCount;
}

MATCH_FUNC(0x4712F0)
void Ped_List_4::ClearPeds_4712F0()
{
    Char_8* pIter = field_0_pFirstPed;
    while (pIter)
    {
        Char_8* pLast = pIter;
        pIter = pIter->mpNext;
        gChar_8_Pool_678b50->field_0_pool.DeAllocate(pLast);
    }

    field_0_pFirstPed = 0;
}

MATCH_FUNC(0x471320)
Ped* Ped_List_4::RemoveFirstPed_471320()
{
    Char_8* pIter = field_0_pFirstPed;
    if (!pIter)
    {
        return 0;
    }

    Ped* pPed = pIter->field_0_char_ped;
    field_0_pFirstPed = pIter->mpNext;
    gChar_8_Pool_678b50->field_0_pool.DeAllocate(pIter);
    return pPed;
}

STUB_FUNC(0x471340)
Ped* Ped_List_4::GetFromListClosestPedToPoint_471340(Fix16 x, Fix16 y)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x4713C0)
Ped* Ped_List_4::FindClosestPedInViewCone_4713C0(Fix16 x, Fix16 y, Ang16 ang1, Ang16 ang2)
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x4715a0)
void Ped_List_4::KillAllPedsFromList_4715A0()
{
    Char_8* pIter = field_0_pFirstPed;
    while (pIter)
    {
        Char_8* pLast = pIter;
        pIter->field_0_char_ped->Kill_46F9D0();
        pIter = pIter->mpNext;
        gChar_8_Pool_678b50->field_0_pool.DeAllocate(pLast);
    }

    field_0_pFirstPed = 0;
}

MATCH_FUNC(0x4715e0)
void Ped_List_4::KillAllPedsAndClearCarRef_4715E0()
{
    Char_8* pIter = field_0_pFirstPed;
    while (pIter)
    {
        Char_8* pLast = pIter;
        pIter->field_0_char_ped->Kill_46F9D0();
        pIter->field_0_char_ped->field_16C_car = 0;
        pIter = pIter->mpNext;
        gChar_8_Pool_678b50->field_0_pool.DeAllocate(pLast);
    }

    field_0_pFirstPed = 0;
}

MATCH_FUNC(0x471630)
void Ped_List_4::ApplyPassengerBusStopBehavior_471630()
{
    for (Char_8* pIter = field_0_pFirstPed; pIter; pIter = pIter->mpNext)
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
void Ped_List_4::ForceTaxiPassengersToExit_471680()
{
    for (Char_8* pIter = field_0_pFirstPed; pIter; pIter = pIter->mpNext)
    {
        if (pIter->field_0_char_ped->field_240_occupation == 7)
        {
            pIter->field_0_char_ped->field_21C |= 0x20000000u;
        }
    }
}

MATCH_FUNC(0x4716b0)
u16 Ped_List_4::GetPedsCount_4716B0()
{
    u16 passengerCount;
    Char_8* pIter = field_0_pFirstPed;
    for (passengerCount = 0; pIter; ++passengerCount)
    {
        pIter = pIter->mpNext;
    }
    return passengerCount;
}

MATCH_FUNC(0x4716d0)
void Ped_List_4::SyncPassengersWithCarState_4716D0(Car_BC* pCar)
{
    for (Char_8* pIter = field_0_pFirstPed; pIter; pIter = pIter->mpNext)
    {
        pIter->field_0_char_ped->field_204_killer_id = pCar->field_70_exploder_ped_id;
        pIter->field_0_char_ped->field_290 = pCar->field_90;
        pIter->field_0_char_ped->field_264 = 50;
    }
}

MATCH_FUNC(0x471710)
char_type Ped_List_4::HasPassengerWith_F238_Is_5_471710()
{
    Char_8* pIter = field_0_pFirstPed;
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
