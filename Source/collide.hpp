#pragma once

#include "Function.hpp"
#include "Pool.hpp"
#include "PurpleDoom.hpp"
#include "fix16.hpp"

class Sprite;

class Collide_8
{
  public:
    void PoolAllocate()
    {
    }

    void PoolDeallocate()
    {
    }

    Sprite* field_0_sprt;
    Collide_8* mpNext;
};

typedef PoolBasic<Collide_8, 4096> T_Collide_8_Pool;

class Collide_C
{
  public:
    EXPORT void sub_478A20();
    EXPORT Collide_C();
    EXPORT ~Collide_C();
    Fix16 field_0_count;
    s32 field_4_count;
    s32 field_8_bUnknown;
};

class PurpleDoom_C_Pool
{
  public:
    PurpleDoom_C_Pool()
    {
        PurpleDoom_C* pOff = &field_4_pool[0];
        for (s32 i = 0; i < 6000 - 1; i++)
        {
            pOff->mpNext = pOff + 1;
            ++pOff;
        }

        field_4_pool[6000 - 1].mpNext = 0;
        field_0_pHead = field_4_pool;
    }

    // 0x4468B0
    ~PurpleDoom_C_Pool()
    {
        field_0_pHead = 0;
    }

    void Remove(PurpleDoom_C* pToRemove)
    {
        pToRemove->mpNext = this->field_0_pHead;
        this->field_0_pHead = pToRemove;
    }

    PurpleDoom_C* UnlinkAndReturnNext(PurpleDoom_C* pOldItem)
    {
        PurpleDoom_C* pNewItem = pOldItem->mpNext;
        pOldItem->mpNext = this->field_0_pHead;
        this->field_0_pHead = pOldItem;
        return pNewItem;
    }

    PurpleDoom_C* Allocate()
    {
        PurpleDoom_C* pNewItem = this->field_0_pHead;
        this->field_0_pHead = this->field_0_pHead->mpNext;
        return pNewItem;
    }

    PurpleDoom_C* field_0_pHead;
    PurpleDoom_C field_4_pool[6000];
};
