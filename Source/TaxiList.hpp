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
    // 9.6f 0x4C09B0
    void Init()
    {
        ClearList();
    }

    // inlined
    // 9.6f 0x4C0980
    void ClearList()
    {
        mpFirst = NULL;
    }

  public:
    STATIC_MEMBER(TaxiListPool*, sTaxi4Pool);
    // 9.6f 0x434970
    inline bool IsEmpty() const
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

EXTERN_GLOBAL(TaxiList*, gTaxi_4);
