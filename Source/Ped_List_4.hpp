#pragma once

#include "Function.hpp"
#include "fix16.hpp"

class Ped;
class Char_8;
class Car_BC;

class Ped_List_4
{
  public:
    Ped_List_4()
    {
        ClearList_420E90();
    }

    EXPORT Char_8* AddPed_471140(Ped* pPed);
    EXPORT void AddPedToBackIfMissing_471160(Ped* pPed);
    EXPORT void AddPedToFrontIfMissing_4711B0(Ped* pPed);
    EXPORT void RemovePed_4711F0(Ped* pPed);
    // Functionally the same as above? Hmmm
    EXPORT void RemovePed_471240(Ped* pPed);
    // TODO: Rename when we know what that state is/means
    EXPORT char_type RemovePedsInSpecificState_471290();
    EXPORT void ClearPeds_4712F0();
    EXPORT Ped* RemoveFirstPed_471320();
    EXPORT Ped* GetFromListClosestPedToPoint_471340(Fix16 x, Fix16 y);
    EXPORT Ped* FindClosestPedInViewCone_4713C0(Fix16 x, Fix16 y, Ang16 ang1, Ang16 ang2);
    EXPORT void KillAllPedsFromList_4715A0();
    EXPORT void KillAllPedsAndClearCarRef_4715E0();
    EXPORT void ApplyPassengerBusStopBehavior_471630();
    EXPORT void ForceTaxiPassengersToExit_471680();

    // 9.6f 0x445BB0
    EXPORT u16 GetPedsCount_4716B0();
    EXPORT void SyncPassengersWithCarState_4716D0(Car_BC* pCar);
    EXPORT char_type HasPassengerWith_F238_Is_5_471710();

    // inline
    void ClearList_420E90()
    {
        field_0_pFirstPed = 0;
    }

    Char_8* field_0_pFirstPed;
};
GTA2_ASSERT_SIZEOF_ALWAYS(Ped_List_4, 0x4)

EXTERN_GLOBAL(Ped_List_4, gThreateningPedsList_678468);
