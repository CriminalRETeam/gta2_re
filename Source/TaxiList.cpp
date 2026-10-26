#include "TaxiList.hpp"
#include "Globals.hpp"
#include "error.hpp"
#include "Car.hpp"

DEFINE_GLOBAL(TaxiListPool*, gTaxi_4_Pool_6783F8, 0x6783F8);
DEFINE_GLOBAL(TaxiList*, gTaxi_4_704130, 0x704130);

MATCH_FUNC(0x457ba0)
void TaxiList::PushTaxi_457BA0(Car* pCar)
{
    TaxiListNode* pFirst = gTaxi_4_Pool_6783F8->Allocate();
    pFirst->field_0_pCar = pCar;
    pFirst->mpNext = this->field_0_pFirst;
    this->field_0_pFirst = pFirst;
}

MATCH_FUNC(0x457bc0)
void TaxiList::PopAll_457BC0()
{
    TaxiListNode* pIter = this->field_0_pFirst;
    while (pIter)
    {
        TaxiListNode* pOldIter = pIter;
        pIter = pIter->mpNext;
        gTaxi_4_Pool_6783F8->DeAllocate(pOldIter);
    }
    this->field_0_pFirst = NULL;
}

// https://decomp.me/scratch/tPr1q
MATCH_FUNC(0x457bf0)
Car* TaxiList::GetTaxiNear_457BF0(Fix16 xpos, Fix16 ypos)
{
    Car* pCarRet = NULL;
    Fix16 smallest(99999);

    for (TaxiListNode* pIter = field_0_pFirst; pIter; pIter = pIter->mpNext)
    {
        // 9.6f passes references to the sprite position, not get_x/get_y copies
        Fix16 distance = Fix16::MaxAbsDistance_42A6B0(xpos,
                                                             ypos,
                                                             pIter->field_0_pCar->field_50_car_sprite->field_14_xy.x,
                                                             pIter->field_0_pCar->field_50_car_sprite->field_14_xy.y);
        if (distance < smallest)
        {
            pCarRet = pIter->field_0_pCar;
            if (!pCarRet->IsDespawning_4215B0())
            {
                smallest = distance;
            }
        }
    }
    return pCarRet;
}

MATCH_FUNC(0x5ae060)
TaxiList::TaxiList()
{
    Init_4C09B0();

    if (!gTaxi_4_Pool_6783F8)
    {
        gTaxi_4_Pool_6783F8 = new TaxiListPool();
        if (!gTaxi_4_Pool_6783F8)
        {
            FatalError_4A38C0(Gta2Error::OutOfMemoryNewOperator, "C:\\Splitting\\Gta2\\Source\\taxi.cpp", 29);
        }
    }
}

MATCH_FUNC(0x5ae0d0)
TaxiList::~TaxiList()
{
    if (gTaxi_4_Pool_6783F8)
    {
        GTA2_DELETE_AND_NULL(gTaxi_4_Pool_6783F8);
    }
}