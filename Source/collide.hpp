#pragma once

#include "Function.hpp"
#include "PurpleDoom.hpp"
#include "fix16.hpp"
#include "Pool.hpp"

class Sprite;

class Collide_8
{
  public:
    Sprite* field_0_sprt;
    Collide_8* mpNext;
};

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

class Collide_8_Pool
{
  public:
    Collide_8_Pool()
    {
        Collide_8* pOff = &field_4_pool[0];
        for (s32 i = 0; i < 4096 - 1; i++)
        {
            pOff->mpNext = pOff + 1;
            ++pOff;
        }

        field_4_pool[4096-1].mpNext = 0;
        field_0_pHead = field_4_pool;
    }

    // 0x4468A0
    ~Collide_8_Pool()
    {
        field_0_pHead = 0;
    }

    void Remove(Collide_8* pToRemove)
    {
        pToRemove->mpNext = this->field_0_pHead;
        this->field_0_pHead = pToRemove;
    }

    Collide_8* Allocate()
    {
        Collide_8* pNewCollide = this->field_0_pHead;
        this->field_0_pHead = this->field_0_pHead->mpNext;
        return pNewCollide;
    }

    Collide_8* field_0_pHead;
    Collide_8 field_4_pool[4096];
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

        field_4_pool[6000-1].mpNext = 0;
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
