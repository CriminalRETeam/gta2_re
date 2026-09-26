#include "Taxi_4.hpp"
#include "Globals.hpp"
#include "error.hpp"
#include "Car_BC.hpp"

DEFINE_GLOBAL(Taxi_4_Pool*, gTaxi_4_Pool_6783F8, 0x6783F8);
DEFINE_GLOBAL(Taxi_4*, gTaxi_4_704130, 0x704130);

MATCH_FUNC(0x457ba0)
void Taxi_4::PushTaxi_457BA0(Car_BC* pCar)
{
    Taxi_8* pFirst = gTaxi_4_Pool_6783F8->Allocate();
    pFirst->field_0 = pCar;
    pFirst->mpNext = this->field_0;
    this->field_0 = pFirst;
}

MATCH_FUNC(0x457bc0)
void Taxi_4::PopAll_457BC0()
{
    Taxi_8* pIter = this->field_0;
    while (pIter)
    {
        Taxi_8* pOldIter = pIter;
        pIter = pIter->mpNext;
        gTaxi_4_Pool_6783F8->DeAllocate(pOldIter);
    }
    this->field_0 = 0;
}

// https://decomp.me/scratch/tPr1q
WIP_FUNC(0x457bf0)
Car_BC* Taxi_4::GetTaxiNear_457BF0(Fix16 xpos, Fix16 ypos)
{
    WIP_IMPLEMENTED;
    Taxi_8* pIter = field_0;
    Fix16 smallest(99999);
    Car_BC* pCarRet;

    for (pCarRet = NULL; pIter; pIter = pIter->mpNext)
    {
        Car_BC* pCurrCar = pIter->field_0;
        Fix16 distance = Fix16::MaxAbsDistance_42A6B0(xpos,
                                                      ypos,
                                                      pIter->field_0->field_50_car_sprite->field_14_xy.x,
                                                      pIter->field_0->field_50_car_sprite->field_14_xy.y);
        if (distance < smallest)
        {
            pCarRet = pCurrCar;
            if (!pCurrCar->IsDespawning_4215B0())
            {
                smallest = distance;
            }
        }
    }
    return pCarRet;
}

MATCH_FUNC(0x5ae060)
Taxi_4::Taxi_4()
{
    sub_4C09B0();

    if (!gTaxi_4_Pool_6783F8)
    {
        gTaxi_4_Pool_6783F8 = new Taxi_4_Pool();
        if (!gTaxi_4_Pool_6783F8)
        {
            FatalError_4A38C0(Gta2Error::OutOfMemoryNewOperator, "C:\\Splitting\\Gta2\\Source\\taxi.cpp", 29);
        }
    }
}

MATCH_FUNC(0x5ae0d0)
Taxi_4::~Taxi_4()
{
    if (gTaxi_4_Pool_6783F8)
    {
        GTA2_DELETE_AND_NULL(gTaxi_4_Pool_6783F8);
    }
}