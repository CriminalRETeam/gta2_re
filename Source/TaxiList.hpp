#pragma once

#include "Function.hpp"
#include "Pool.hpp"
#include "fix16.hpp"

class Car;

struct TaxiListNode
{

    void PoolAllocate()
    {
    }

    void PoolDeallocate()
    {
    }

    Car* mpCar; // 0x0
    TaxiListNode* mpNext;
};
GTA2_ASSERT_SIZEOF_ALWAYS(TaxiListNode, 0x8);


class TaxiListPool : public PoolBasic<TaxiListNode, 100>
{
public:
    TaxiListPool()
    {
        
    }
    
    ~TaxiListPool()
    {
        this->mpHead = NULL;
    }
};


class TaxiList
{
  private:
    // inlined
    void Init_4C09B0()
    {
        ClearList_4C0980();
    }

    // inlined
    void ClearList_4C0980()
    {
        mpFirst = NULL;
    }

  public:
    STATIC_MEMBER(TaxiListPool*, sTaxi4Pool);
    // 9.6f 0x434970
    inline bool IsEmpty_434970() const
    {
        return mpFirst == NULL;
    }

    EXPORT void PushTaxi(Car* pCar);
    EXPORT void PopAll();
    EXPORT Car* GetTaxiNear(Fix16 xpos, Fix16 ypos);
    EXPORT TaxiList();
    EXPORT ~TaxiList();

  private:
    TaxiListNode* mpFirst; // 0x0
};
GTA2_ASSERT_SIZEOF_ALWAYS(TaxiList, 0x4);

EXTERN_GLOBAL(TaxiList*, gTaxi_4_704130);
