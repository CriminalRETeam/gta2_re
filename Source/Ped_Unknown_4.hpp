#pragma once

#include "Function.hpp"
#include "char.hpp"

class Ped;
class Char_8;
class Car_BC;

// List of Peds that are inside of a Car?
class Ped_Unknown_4
{
  public:
    Ped_Unknown_4()
    {
        ClearList_420E90();
    }

    EXPORT Char_8* AddPassenger_471140(Ped* pPed);
    EXPORT void AddPassengerToBackIfMissing_471160(Ped* pPed);
    EXPORT void AddPassengerToFrontIfMissing_4711B0(Ped* pPed);
    EXPORT Char_8* RemovePassenger_4711F0(Ped* pPed);
    // Functionally the same as above? Hmmm
    EXPORT void RemovePassenger_471240(Ped* pPed);
    // TODO: Rename when we know what that state is/means
    EXPORT char_type RemovePassengersInSpecificState_471290();
    EXPORT void ClearPassengers_4712F0();
    EXPORT Ped* RemoveFirstPassenger_471320();
    EXPORT Ped* GetClosestPassengerToPoint_471340(Fix16 x, Fix16 y);
    EXPORT Ped* FindClosestPassengerInViewCone_4713C0(Fix16 x, Fix16 y, Ang16 ang1, Ang16 ang2);
    EXPORT Char_8** KillAllPassengers_4715A0();
    EXPORT Char_8** KillAllPassengersAndClearCarRef_4715E0();
    EXPORT void ApplyPassengerBusStopBehavior_471630();
    EXPORT void ForceTaxiPassengersToExit_471680();

    // 9.6f 0x445BB0
    EXPORT u16 GetPassengerCount_4716B0();
    EXPORT void SyncPassengersWithCarState_4716D0(Car_BC* pCar);
    EXPORT char_type HasPassengerWith_F238_Is_5_471710();

    // inline
    void ClearList_420E90()
    {
        field_0_pFirstPassenger = 0;
    }

    Char_8* field_0_pFirstPassenger;
};
GTA2_ASSERT_SIZEOF_ALWAYS(Ped_Unknown_4, 0x4)