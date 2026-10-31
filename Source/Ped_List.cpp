#include "Ped_List.hpp"
#include "Car.hpp"
#include "Char_Pool.hpp"
#include "Ped.hpp"

DEFINE_GLOBAL(Ped_List, gThreateningPedsList, 0x678468);

DEFINE_GLOBAL_INIT(Ang16, kAng0_678B40, Ang16(0), 0x678B40);

MATCH_FUNC(0x471140)
PedListNode* Ped_List::AddPedToFront(Ped* pPed)
{
    PedListNode* pNew = gChar_8_Pool->Allocate();

    pNew->mCharPed = pPed;

    pNew->mpNext = mpFirstPed;
    mpFirstPed = pNew;

    return pNew;
}

MATCH_FUNC(0x471160)
void Ped_List::AddPedToBackIfMissing(Ped* pPed)
{
    PedListNode* pIter = mpFirstPed;
    PedListNode* pTail = NULL;
    if (pIter)
    {
        while (pIter)
        {
            if (pIter->mCharPed == pPed)
            {
                // Already exists
                return;
            }

            pTail = pIter;
            pIter = pIter->mpNext;
        }
    }

    PedListNode* pNew = gChar_8_Pool->Allocate();
    pNew->mCharPed = pPed;

    if (pTail)
    {
        pTail->mpNext = pNew;
        pNew->mpNext = NULL;
    }
    else
    {
        // List was empty
        mpFirstPed = pNew;
        pNew->mpNext = NULL;
    }
}

MATCH_FUNC(0x4711B0)
void Ped_List::AddPedToFrontIfMissing(Ped* pPed)
{
    PedListNode* pIter = mpFirstPed;
    PedListNode* pTail = NULL;
    if (pIter)
    {
        while (pIter)
        {
            if (pIter->mCharPed == pPed)
            {
                // Already exists
                return;
            }

            pTail = pIter;
            pIter = pIter->mpNext;
        }
    }

    PedListNode* pNew = gChar_8_Pool->Allocate();
    pNew->mCharPed = pPed;

    pNew->mpNext = mpFirstPed;
    mpFirstPed = pNew;
}

MATCH_FUNC(0x4711f0)
void Ped_List::RemovePed_4711F0(Ped* pPed)
{
    PedListNode* pIter = mpFirstPed;
    PedListNode* pPrev = NULL;
    while (pIter)
    {
        if (pIter->mCharPed == pPed)
        {
            if (pPrev)
            {
                pPrev->mpNext = pIter->mpNext;
            }
            else
            {
                mpFirstPed = pIter->mpNext;
            }
            gChar_8_Pool->DeAllocate(pIter);
            break;
        }
        pPrev = pIter;
        pIter = pIter->mpNext;
    }
}

MATCH_FUNC(0x471240)
void Ped_List::RemovePed_471240(Ped* pPed)
{
    PedListNode* pIter = mpFirstPed;
    PedListNode* pPrev = NULL;
    while (pIter)
    {
        if (pIter->mCharPed == pPed)
        {
            if (pPrev)
            {
                pPrev->mpNext = pIter->mpNext;
            }
            else
            {
                mpFirstPed = pIter->mpNext;
            }
            gChar_8_Pool->DeAllocate(pIter);
            break;
        }
        pPrev = pIter;
        pIter = pIter->mpNext;
    }
}

MATCH_FUNC(0x471290)
u8 Ped_List::RemoveNonCorpses()
{
    PedListNode* pPrevKept = NULL;
    PedListNode* pIter = mpFirstPed;
    u8 removedCount = 0;
    while (pIter)
    {
        if (pIter->mCharPed->IsActive() && pIter->mCharPed->mPedState1 == ped_state_1::dead_9)
        {
            pPrevKept = pIter;
            pIter = pIter->mpNext;
        }
        else
        {
            if (pPrevKept)
            {
                pPrevKept->mpNext = pIter->mpNext;
            }
            else
            {
                mpFirstPed = pIter->mpNext;
            }

            PedListNode* pNext = pIter->mpNext;

            gChar_8_Pool->DeAllocate(pIter);
            pIter = pNext;

            ++removedCount;
        }
    }
    return removedCount;
}

MATCH_FUNC(0x4712F0)
void Ped_List::ClearPeds()
{
    PedListNode* pIter = mpFirstPed;
    while (pIter)
    {
        PedListNode* pFreed = pIter;
        pIter = pIter->mpNext;
        gChar_8_Pool->mPool.DeAllocate(pFreed);
    }

    mpFirstPed = NULL;
}

MATCH_FUNC(0x471320)
Ped* Ped_List::RemoveFirstPed()
{
    PedListNode* pIter = mpFirstPed;
    if (!pIter)
    {
        return NULL;
    }

    Ped* pPed = pIter->mCharPed;
    mpFirstPed = pIter->mpNext;
    gChar_8_Pool->DeAllocate(pIter);
    return pPed;
}

// 9.6f 0x445C30
// 9.6f calls MaxAbsDistance here. By value, with the y difference first, VC6 keeps
// both differences and the max in registers as the original does.
MATCH_FUNC(0x471340)
Ped* Ped_List::FindClosestActivePed(Fix16 x, Fix16 y) const
{
    Fix16 closestDistance(99999);
    PedListNode* pIter = this->mpFirstPed;
    Ped* pClosest = NULL;
    while (pIter)
    {
        Ped* pPed = pIter->mCharPed;
        if (pIter->mCharPed->IsActive() == 1)
        {
            Fix16 distance = Fix16::MaxAbsDistance(x, y, pPed->get_cam_x(), pPed->get_cam_y());
            if (distance < closestDistance)
            {
                pClosest = pPed;
                closestDistance = distance;
            }
        }
        pIter = pIter->mpNext;
    }
    return pClosest;
}

// https://decomp.me/scratch/GJ2JR
MATCH_FUNC(0x4713C0)
Ped* Ped_List::FindClosestPedInViewCone(Fix16 x, Fix16 y, Ang16 facing, Ang16 half_cone) const
{
    Fix16 closestDistance = Fix16(99999);
    PedListNode* pIter = mpFirstPed;
    Ped* pClosest = NULL;

    for (; pIter; pIter = pIter->mpNext)
    {
        if (pIter->mCharPed->IsActive() == 1)
        {
            bool withinCone = false;
            Fix16 distance = Fix16::MaxAbsDistance(x, y, pIter->mCharPed->get_cam_x(), pIter->mCharPed->get_cam_y());

            Ang16 angle_diff = Fix16::atan2_fixed(pIter->mCharPed->get_cam_y() - y, pIter->mCharPed->get_cam_x() - x) - facing;

            Ang16 neg_half_cone = kAng0_678B40 - half_cone;

            if (angle_diff < half_cone || angle_diff > neg_half_cone)
            {
                withinCone = true;
            }
            if (distance < closestDistance && withinCone)
            {
                pClosest = pIter->mCharPed;
                closestDistance = distance;
            }
        }
    }
    return pClosest;
}

MATCH_FUNC(0x4715a0)
void Ped_List::KillAndClearAllPeds()
{
    PedListNode* pIter = mpFirstPed;
    while (pIter)
    {
        PedListNode* pFreed = pIter;
        pIter->mCharPed->Kill();
        pIter = pIter->mpNext;
        gChar_8_Pool->DeAllocate(pFreed);
    }

    mpFirstPed = NULL;
}

MATCH_FUNC(0x4715e0)
void Ped_List::KillAndClearAllPedsResetCarRef()
{
    PedListNode* pIter = mpFirstPed;
    while (pIter)
    {
        PedListNode* pFreed = pIter;
        pIter->mCharPed->Kill();
        pIter->mCharPed->mCar = NULL;
        pIter = pIter->mpNext;
        gChar_8_Pool->DeAllocate(pFreed);
    }

    mpFirstPed = NULL;
}

MATCH_FUNC(0x471630)
void Ped_List::ApplyPassengerBusStopBehavior()
{
    for (PedListNode* pIter = mpFirstPed; pIter; pIter = pIter->mpNext)
    {
        if (pIter->mCharPed->get_occupation() == ped_ocupation_enum::bus_customer_8)
        {
            pIter->mCharPed->SetObjective2(objectives_enum::no_obj_0);
            pIter->mCharPed->SetObjective(objectives_enum::leave_car_and_flee_34);
            pIter->mCharPed->SetTargetObjectiveCar(pIter->mCharPed->mCar);
        }
    }
}

MATCH_FUNC(0x471680)
void Ped_List::ForceTaxiPassengersToExit()
{
    for (PedListNode* pIter = mpFirstPed; pIter; pIter = pIter->mpNext)
    {
        if (pIter->mCharPed->mOccupation == ped_ocupation_enum::taxi_customer_7)
        {
            pIter->mCharPed->mBf21C.bForcedOutOfTaxi = true;
        }
    }
}

MATCH_FUNC(0x4716b0)
u16 Ped_List::GetPedsCount() const
{
    u16 count;
    PedListNode* pIter = mpFirstPed;
    for (count = 0; pIter; ++count)
    {
        pIter = pIter->mpNext;
    }
    return count;
}

MATCH_FUNC(0x4716d0)
void Ped_List::SyncPassengersWithCarState(Car* pCar)
{
    for (PedListNode* pIter = mpFirstPed; pIter; pIter = pIter->mpNext)
    {
        pIter->mCharPed->mKillerId = pCar->mExploderPedId;
        pIter->mCharPed->mDeathCause = pCar->mDeathCause;
        pIter->mCharPed->mKillerIdTimer = 50;
    }
}

MATCH_FUNC(0x471710)
bool Ped_List::HasScriptCreatedPed() const
{
    PedListNode* pIter = mpFirstPed;
    while (pIter)
    {
        if (pIter->mCharPed->mPedType == ped_type::script_created_5)
        {
            return true;
        }
        pIter = pIter->mpNext;
    }
    return false;
}
