#pragma once

#include "Function.hpp"
#include "fix16.hpp"

class Car_BC;

struct Taxi_8
{
    Car_BC* field_0;
    Taxi_8* mpNext;
};

class Taxi_324
{
  public:
    // inlined 0x4C0950
    Taxi_324()
    {
        Taxi_8* pIter = &field_4_array[0];
        for (s32 i = 0; i < GTA2_COUNTOF(field_4_array); i++)
        {
            pIter->mpNext = pIter + 1;
            pIter++;
        }
        field_0_pFirst = &field_4_array[0];
        field_320_count = 0;
    }

    // inlined 0x4C0940
    ~Taxi_324()
    {
        field_0_pFirst = 0;
    }

    Taxi_8* Alloc()
    {
        Taxi_8* pFirst = field_0_pFirst;
        field_0_pFirst = pFirst->mpNext;
        return pFirst;
    }

    void DeAlloc(Taxi_8* pItem)
    {
        pItem->mpNext = field_0_pFirst;
        field_0_pFirst = pItem;
    }

    Taxi_8* field_0_pFirst;
    Taxi_8 field_4_array[99];
    u32 field_31C;
    u32 field_320_count;
};

class Taxi_4
{
  public:
    // inlined
    void sub_4C09B0()
    {
        sub_4C0980();
    }

    // inlined
    void sub_4C0980()
    {
        field_0 = 0;
    }

    EXPORT void PushTaxi_457BA0(Car_BC* pCar);
    EXPORT void PopAll_457BC0();
    EXPORT Car_BC* GetTaxiNear_457BF0(Fix16 xpos, Fix16 ypos);
    EXPORT Taxi_4();
    EXPORT ~Taxi_4();
    Taxi_8* field_0;
};

EXTERN_GLOBAL(Taxi_4*, gTaxi_4_704130);
