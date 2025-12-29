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
    EXPORT Char_8* sub_471160(Ped* pPed);
    EXPORT void sub_4711B0(Ped* pPed);
    EXPORT Char_8* sub_4711F0(Ped* pPed);
    EXPORT void sub_471240(Ped* pPed);
    EXPORT char_type sub_471290();
    EXPORT void sub_4712F0();
    EXPORT Ped* sub_471320();
    EXPORT Ped* sub_471340(Fix16 x, Fix16 y);
    EXPORT Ped* sub_4713C0(Fix16 x, Fix16 y, Ang16 ang1, Ang16 ang2);
    EXPORT Char_8** KillAllPassengers_4715A0();
    EXPORT Char_8** KillAllPassengersAndClearCarRef_4715E0();
    EXPORT void sub_471630();
    EXPORT void sub_471680();

    // 9.6f 0x445BB0
    EXPORT u16 GetPassengerCount_4716B0();
    EXPORT void sub_4716D0(Car_BC* pCar);
    EXPORT char_type sub_471710();

    // inline
    void ClearList_420E90()
    {
        field_0_pFirstPassenger = 0;
    }

    Char_8* field_0_pFirstPassenger;
};
GTA2_ASSERT_SIZEOF_ALWAYS(Ped_Unknown_4, 0x4)