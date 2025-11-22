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
    Collide_8* field_4_pNext;
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

class Collide_8004
{
  public:
    Collide_8004()
    {
        Collide_8* pOff = &field_4[0];
        for (s32 i = 0; i < 4096 - 1; i++)
        {
            pOff->field_4_pNext = pOff + 1;
            ++pOff;
        }

        field_4[4096-1].field_4_pNext = 0;
        field_0 = field_4;
    }

    // 0x4468A0
    ~Collide_8004()
    {
        field_0 = 0;
    }

    void Remove(Collide_8* pToRemove)
    {
        pToRemove->field_4_pNext = this->field_0;
        this->field_0 = pToRemove;
    }

    Collide_8* Allocate()
    {
        Collide_8* pNewCollide = this->field_0;
        this->field_0 = this->field_0->field_4_pNext;
        return pNewCollide;
    }

    Collide_8* field_0;
    Collide_8 field_4[4096];
};

class Collide_11944
{
  public:
    Collide_11944()
    {
        PurpleDoom_C* pOff = &field_4[0];
        for (s32 i = 0; i < 6000 - 1; i++)
        {
            pOff->field_8_pNext = pOff + 1;
            ++pOff;
        }

        field_11940 = 0;
        field_0_pHead = field_4;
    }

    // 0x4468B0
    ~Collide_11944()
    {
        field_0_pHead = 0;
    }

    void Remove(PurpleDoom_C* pToRemove)
    {
        pToRemove->field_8_pNext = this->field_0_pHead;
        this->field_0_pHead = pToRemove;
    }

    PurpleDoom_C* UnlinkAndReturnNext(PurpleDoom_C* pOldItem)
    {
        PurpleDoom_C* pNewItem = pOldItem->field_8_pNext;
        pOldItem->field_8_pNext = this->field_0_pHead;
        this->field_0_pHead = pOldItem;
        return pNewItem;
    }

    PurpleDoom_C* Allocate()
    {
        PurpleDoom_C* pNewItem = this->field_0_pHead;
        this->field_0_pHead = this->field_0_pHead->field_8_pNext;
        return pNewItem;
    }

    PurpleDoom_C* field_0_pHead;
    PurpleDoom_C field_4[5999];
    s32 field_11938;
    s32 field_1193C;
    s32 field_11940;
};
