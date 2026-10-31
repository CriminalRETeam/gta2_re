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

    EXPORT PedListNode* AddPedToFront(Ped* pPed);
    EXPORT void AddPedToBackIfMissing(Ped* pPed);
    EXPORT void AddPedToFrontIfMissing(Ped* pPed);
    EXPORT void RemovePed_4711F0(Ped* pPed);
    // Same code as RemovePed_4711F0
    EXPORT void RemovePed_471240(Ped* pPed);
    // Keeps the active dead peds (the bodies waiting for an ambulance) and frees every other node.
    // Returns how many were removed.
    EXPORT u8 RemoveNonCorpses();
    EXPORT void ClearPeds();
    EXPORT Ped* RemoveFirstPed();
    EXPORT Ped* FindClosestActivePed(Fix16 x, Fix16 y) const;
    // facing: the view direction, half_cone: how far either side of it counts as in view
    EXPORT Ped* FindClosestPedInViewCone(Fix16 x, Fix16 y, Ang16 facing, Ang16 half_cone) const;
    EXPORT void KillAndClearAllPeds();
    EXPORT void KillAndClearAllPedsResetCarRef();
    EXPORT void ApplyPassengerBusStopBehavior();
    EXPORT void ForceTaxiPassengersToExit();

    // 9.6f 0x445BB0
    EXPORT u16 GetPedsCount() const;
    EXPORT void SyncPassengersWithCarState(Car* pCar);
    EXPORT bool HasScriptCreatedPed() const;

    void ClearList_420E90()
    {
        mpFirstPed = NULL;
    }

    // 9.6f 0x420EA0
    bool IsEmpty_420EA0() const
    {
        return mpFirstPed == NULL;
    }

    PedListNode* mpFirstPed; // 0x0
};
GTA2_ASSERT_SIZEOF_ALWAYS(Ped_List, 0x4)

EXTERN_GLOBAL(Ped_List, gThreateningPedsList);
