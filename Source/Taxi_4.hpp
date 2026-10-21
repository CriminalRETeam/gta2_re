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

    Car* field_0_pCar;
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
        this->field_0_pHead = 0;
    }
};


class TaxiList
{
  public:
    // inlined
    void Init_4C09B0()
    {
        ClearList_4C0980();
    }

    // inlined
    void ClearList_4C0980()
    {
        field_0_pFirst = 0;
    }

    // 9.6f 0x434970
    inline bool IsEmpty_434970()
    {
        return field_0_pFirst == 0;
    }

    EXPORT void PushTaxi_457BA0(Car* pCar);
    EXPORT void PopAll_457BC0();
    EXPORT Car* GetTaxiNear_457BF0(Fix16 xpos, Fix16 ypos);
    EXPORT TaxiList();
    EXPORT ~TaxiList();
    TaxiListNode* field_0_pFirst;
};
GTA2_ASSERT_SIZEOF_ALWAYS(TaxiList, 0x4);

EXTERN_GLOBAL(TaxiList*, gTaxi_4_704130);
