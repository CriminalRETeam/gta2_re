#include "TaxiList.hpp"
#include "Globals.hpp"
#include "error.hpp"
#include "Car.hpp"

DEFINE_STATIC_MEMBER(TaxiListPool*, TaxiList, sTaxi4Pool, 0x6783F8);
DEFINE_GLOBAL(TaxiList*, gTaxi_4, 0x704130);

MATCH_FUNC(0x457ba0)
void TaxiList::PushTaxi(Car* pCar)
{
    TaxiListNode* pNode = sTaxi4Pool->Allocate();
    pNode->mpCar = pCar;
    pNode->mpNext = this->mpFirst;
    this->mpFirst = pNode;
}

MATCH_FUNC(0x457bc0)
void TaxiList::PopAll()
{
    TaxiListNode* pIter = this->mpFirst;
    while (pIter)
    {
        TaxiListNode* pOldIter = pIter;
        pIter = pIter->mpNext;
        sTaxi4Pool->DeAllocate(pOldIter);
    }
    this->mpFirst = NULL;
}

// https://decomp.me/scratch/tPr1q
MATCH_FUNC(0x457bf0)
Car* TaxiList::GetTaxiNear(Fix16 xpos, Fix16 ypos)
{
    Car* pCarRet = NULL;
    Fix16 smallest(99999);

    for (TaxiListNode* pIter = mpFirst; pIter; pIter = pIter->mpNext)
    {
        // 9.6f passes references to the sprite position, not get_x/get_y copies
        Fix16 distance = Fix16::MaxAbsDistance_42A6B0(xpos,
                                                             ypos,
                                                             pIter->mpCar->mCarSprite->mXy.x,
                                                             pIter->mpCar->mCarSprite->mXy.y);
        if (distance < smallest)
        {
            pCarRet = pIter->mpCar;
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

    if (!sTaxi4Pool)
    {
        sTaxi4Pool = new TaxiListPool();
        if (!sTaxi4Pool)
        {
            FatalError_4A38C0(Gta2Error::OutOfMemoryNewOperator, "C:\\Splitting\\Gta2\\Source\\taxi.cpp", 29);
        }
    }
}

MATCH_FUNC(0x5ae0d0)
TaxiList::~TaxiList()
{
    if (sTaxi4Pool)
    {
        GTA2_DELETE_AND_NULL(sTaxi4Pool);
    }
}