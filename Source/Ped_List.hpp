#pragma once

#include "Function.hpp"
#include "fix16.hpp"

class Ped;
class PedListNode;
class Car;

class Ped_List
{
  public:
    Ped_List()
    {
        ClearList_420E90();
    }

    EXPORT PedListNode* AddPedToFront_471140(Ped* pPed);
    EXPORT void AddPedToBackIfMissing_471160(Ped* pPed);
    EXPORT void AddPedToFrontIfMissing_4711B0(Ped* pPed);
    EXPORT void RemovePed_4711F0(Ped* pPed);
    // Same code as RemovePed_4711F0
    EXPORT void RemovePed_471240(Ped* pPed);
    // Keeps the active dead peds (the bodies waiting for an ambulance) and frees every other node.
    // Returns how many were removed.
    EXPORT u8 RemoveNonCorpses_471290();
    EXPORT void ClearPeds_4712F0();
    EXPORT Ped* RemoveFirstPed_471320();
    EXPORT Ped* FindClosestActivePed_471340(Fix16 x, Fix16 y) const;
    // facing: the view direction, half_cone: how far either side of it counts as in view
    EXPORT Ped* FindClosestPedInViewCone_4713C0(Fix16 x, Fix16 y, Ang16 facing, Ang16 half_cone) const;
    EXPORT void KillAndClearAllPeds_4715A0();
    EXPORT void KillAndClearAllPedsResetCarRef_4715E0();
    EXPORT void ApplyPassengerBusStopBehavior_471630();
    EXPORT void ForceTaxiPassengersToExit_471680();

    // 9.6f 0x445BB0
    EXPORT u16 GetPedsCount_4716B0() const;
    EXPORT void SyncPassengersWithCarState_4716D0(Car* pCar);
    EXPORT bool HasScriptCreatedPed_471710() const;

    void ClearList_420E90()
    {
        field_0_pFirstPed = NULL;
    }

    // 9.6f 0x420EA0
    bool IsEmpty_420EA0() const
    {
        return field_0_pFirstPed == NULL;
    }

    PedListNode* field_0_pFirstPed;
};
GTA2_ASSERT_SIZEOF_ALWAYS(Ped_List, 0x4)

EXTERN_GLOBAL(Ped_List, gThreateningPedsList_678468);
