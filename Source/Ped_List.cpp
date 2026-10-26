#include "Ped_List.hpp"
#include "Car.hpp"
#include "Char_Pool.hpp"
#include "Ped.hpp"

DEFINE_GLOBAL(Ped_List, gThreateningPedsList_678468, 0x678468);

DEFINE_GLOBAL_INIT(Ang16, kAng0_678B40, Ang16(0), 0x678B40);

MATCH_FUNC(0x471140)
PedListNode* Ped_List::AddPedToFront_471140(Ped* pPed)
{
    PedListNode* pNew = gChar_8_Pool_678b50->Allocate_445EF0();

    pNew->field_0_char_ped = pPed;

    pNew->mpNext = field_0_pFirstPed;
    field_0_pFirstPed = pNew;

    return pNew;
}

MATCH_FUNC(0x471160)
void Ped_List::AddPedToBackIfMissing_471160(Ped* pPed)
{
    PedListNode* pIter = field_0_pFirstPed;
    PedListNode* pTail = NULL;
    if (pIter)
    {
        while (pIter)
        {
            if (pIter->field_0_char_ped == pPed)
            {
                // Already exists
                return;
            }

            pTail = pIter;
            pIter = pIter->mpNext;
        }
    }

    PedListNode* pNew = gChar_8_Pool_678b50->Allocate_445EF0();
    pNew->field_0_char_ped = pPed;

    if (pTail)
    {
        pTail->mpNext = pNew;
        pNew->mpNext = NULL;
    }
    else
    {
        // List was empty
        field_0_pFirstPed = pNew;
        pNew->mpNext = NULL;
    }
}

MATCH_FUNC(0x4711B0)
void Ped_List::AddPedToFrontIfMissing_4711B0(Ped* pPed)
{
    PedListNode* pIter = field_0_pFirstPed;
    PedListNode* pTail = NULL;
    if (pIter)
    {
        while (pIter)
        {
            if (pIter->field_0_char_ped == pPed)
            {
                // Already exists
                return;
            }

            pTail = pIter;
            pIter = pIter->mpNext;
        }
    }

    PedListNode* pNew = gChar_8_Pool_678b50->Allocate_445EF0();
    pNew->field_0_char_ped = pPed;

    pNew->mpNext = field_0_pFirstPed;
    field_0_pFirstPed = pNew;
}

MATCH_FUNC(0x4711f0)
void Ped_List::RemovePed_4711F0(Ped* pPed)
{
    PedListNode* pIter = field_0_pFirstPed;
    PedListNode* pPrev = NULL;
    while (pIter)
    {
        if (pIter->field_0_char_ped == pPed)
        {
            if (pPrev)
            {
                pPrev->mpNext = pIter->mpNext;
            }
            else
            {
                field_0_pFirstPed = pIter->mpNext;
            }
            gChar_8_Pool_678b50->DeAllocate_445F00(pIter);
            break;
        }
        pPrev = pIter;
        pIter = pIter->mpNext;
    }
}

MATCH_FUNC(0x471240)
void Ped_List::RemovePed_471240(Ped* pPed)
{
    PedListNode* pIter = field_0_pFirstPed;
    PedListNode* pPrev = NULL;
    while (pIter)
    {
        if (pIter->field_0_char_ped == pPed)
        {
            if (pPrev)
            {
                pPrev->mpNext = pIter->mpNext;
            }
            else
            {
                field_0_pFirstPed = pIter->mpNext;
            }
            gChar_8_Pool_678b50->DeAllocate_445F00(pIter);
            break;
        }
        pPrev = pIter;
        pIter = pIter->mpNext;
    }
}

MATCH_FUNC(0x471290)
u8 Ped_List::RemoveNonCorpses_471290()
{
    PedListNode* pPrevKept = NULL;
    PedListNode* pIter = field_0_pFirstPed;
    u8 removedCount = 0;
    while (pIter)
    {
        if (pIter->field_0_char_ped->IsActive_433B40() && pIter->field_0_char_ped->field_278_ped_state_1 == ped_state_1::dead_9)
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
                field_0_pFirstPed = pIter->mpNext;
            }

            PedListNode* pNext = pIter->mpNext;

            gChar_8_Pool_678b50->DeAllocate_445F00(pIter);
            pIter = pNext;

            ++removedCount;
        }
    }
    return removedCount;
}

MATCH_FUNC(0x4712F0)
void Ped_List::ClearPeds_4712F0()
{
    PedListNode* pIter = field_0_pFirstPed;
    while (pIter)
    {
        PedListNode* pFreed = pIter;
        pIter = pIter->mpNext;
        gChar_8_Pool_678b50->field_0_pool.DeAllocate(pFreed);
    }

    field_0_pFirstPed = NULL;
}

MATCH_FUNC(0x471320)
Ped* Ped_List::RemoveFirstPed_471320()
{
    PedListNode* pIter = field_0_pFirstPed;
    if (!pIter)
    {
        return NULL;
    }

    Ped* pPed = pIter->field_0_char_ped;
    field_0_pFirstPed = pIter->mpNext;
    gChar_8_Pool_678b50->DeAllocate_445F00(pIter);
    return pPed;
}

// 9.6f 0x445C30
// 9.6f calls MaxAbsDistance_42A6B0 here. By value, with the y difference first, VC6 keeps
// both differences and the max in registers as the original does.
MATCH_FUNC(0x471340)
Ped* Ped_List::FindClosestActivePed_471340(Fix16 x, Fix16 y) const
{
    Fix16 closestDistance(99999);
    PedListNode* pIter = this->field_0_pFirstPed;
    Ped* pClosest = NULL;
    while (pIter)
    {
        Ped* pPed = pIter->field_0_char_ped;
        if (pIter->field_0_char_ped->IsActive_433B40() == 1)
        {
            Fix16 distance = Fix16::MaxAbsDistance_42A6B0(x, y, pPed->get_cam_x(), pPed->get_cam_y());
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
Ped* Ped_List::FindClosestPedInViewCone_4713C0(Fix16 x, Fix16 y, Ang16 facing, Ang16 half_cone) const
{
    Fix16 distance;
    Ang16 angle_diff;
    Ang16 neg_half_cone;

    Fix16 closestDistance = Fix16(99999);
    PedListNode* pIter = field_0_pFirstPed;
    Ped* pClosest = NULL;

    for (; pIter; pIter = pIter->mpNext)
    {
        if (pIter->field_0_char_ped->IsActive_433B40() == 1)
        {
            bool withinCone = false;
            distance = Fix16::MaxAbsDistance_42A6B0(x, y, pIter->field_0_char_ped->get_cam_x(), pIter->field_0_char_ped->get_cam_y());

            angle_diff = Fix16::atan2_fixed_405320(pIter->field_0_char_ped->get_cam_y() - y, pIter->field_0_char_ped->get_cam_x() - x) - facing;

            neg_half_cone = kAng0_678B40 - half_cone;

            if (angle_diff < half_cone || angle_diff > neg_half_cone)
            {
                withinCone = true;
            }
            if (distance < closestDistance && withinCone)
            {
                pClosest = pIter->field_0_char_ped;
                closestDistance = distance;
            }
        }
    }
    return pClosest;
}

MATCH_FUNC(0x4715a0)
void Ped_List::KillAndClearAllPeds_4715A0()
{
    PedListNode* pIter = field_0_pFirstPed;
    while (pIter)
    {
        PedListNode* pFreed = pIter;
        pIter->field_0_char_ped->Kill_46F9D0();
        pIter = pIter->mpNext;
        gChar_8_Pool_678b50->DeAllocate_445F00(pFreed);
    }

    field_0_pFirstPed = NULL;
}

MATCH_FUNC(0x4715e0)
void Ped_List::KillAndClearAllPedsResetCarRef_4715E0()
{
    PedListNode* pIter = field_0_pFirstPed;
    while (pIter)
    {
        PedListNode* pFreed = pIter;
        pIter->field_0_char_ped->Kill_46F9D0();
        pIter->field_0_char_ped->field_16C_car = NULL;
        pIter = pIter->mpNext;
        gChar_8_Pool_678b50->DeAllocate_445F00(pFreed);
    }

    field_0_pFirstPed = NULL;
}

MATCH_FUNC(0x471630)
void Ped_List::ApplyPassengerBusStopBehavior_471630()
{
    for (PedListNode* pIter = field_0_pFirstPed; pIter; pIter = pIter->mpNext)
    {
        if (pIter->field_0_char_ped->get_occupation_403980() == ped_ocupation_enum::bus_customer_8)
        {
            pIter->field_0_char_ped->SetObjective2_463830(objectives_enum::no_obj_0);
            pIter->field_0_char_ped->SetObjective(objectives_enum::objective_34);
            pIter->field_0_char_ped->SetTargetObjectiveCar(pIter->field_0_char_ped->field_16C_car);
        }
    }
}

MATCH_FUNC(0x471680)
void Ped_List::ForceTaxiPassengersToExit_471680()
{
    for (PedListNode* pIter = field_0_pFirstPed; pIter; pIter = pIter->mpNext)
    {
        if (pIter->field_0_char_ped->field_240_occupation == ped_ocupation_enum::taxi_customer_7)
        {
            pIter->field_0_char_ped->field_21C_bf.bForcedOutOfTaxi = true;
        }
    }
}

MATCH_FUNC(0x4716b0)
u16 Ped_List::GetPedsCount_4716B0() const
{
    u16 count;
    PedListNode* pIter = field_0_pFirstPed;
    for (count = 0; pIter; ++count)
    {
        pIter = pIter->mpNext;
    }
    return count;
}

MATCH_FUNC(0x4716d0)
void Ped_List::SyncPassengersWithCarState_4716D0(Car* pCar)
{
    for (PedListNode* pIter = field_0_pFirstPed; pIter; pIter = pIter->mpNext)
    {
        pIter->field_0_char_ped->field_204_killer_id = pCar->field_70_exploder_ped_id;
        pIter->field_0_char_ped->field_290_death_cause = pCar->field_90_death_cause;
        pIter->field_0_char_ped->field_264_killer_id_timer = 50;
    }
}

MATCH_FUNC(0x471710)
bool Ped_List::HasScriptCreatedPed_471710() const
{
    PedListNode* pIter = field_0_pFirstPed;
    while (pIter)
    {
        if (pIter->field_0_char_ped->field_238_ped_type == ped_type::script_created_5)
        {
            return true;
        }
        pIter = pIter->mpNext;
    }
    return false;
}
