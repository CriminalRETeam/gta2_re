#pragma once

#include "Function.hpp"
#include "Pool.hpp"
#include "fix16.hpp"

class Car_BC;

struct Taxi_8
{

    void PoolAllocate()
    {
    }

    void PoolDeallocate()
    {
    }

    Car_BC* field_0;
    Taxi_8* mpNext;
};

typedef PoolBasic<Taxi_8, 100> T_Taxi_4_Pool;

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
