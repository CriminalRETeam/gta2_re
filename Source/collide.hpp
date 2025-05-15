#pragma once

#include "Function.hpp"
#include "PurpleDoom.hpp"

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
    s32 field_0_count;
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

        field_8000 = 0;
        field_0 = field_4;
    }

    void Remove(Collide_8* pToRemove)
    {
        pToRemove->field_4_pNext = this->field_0;
        this->field_0 = pToRemove;
    }

    Collide_8* field_0;
    Collide_8 field_4[4095];
    s32 field_7FFC;
    s32 field_8000;
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
        field_0 = field_4;
    }

    void Remove(PurpleDoom_C* pToRemove)
    {
        pToRemove->field_8_pNext = this->field_0;
        this->field_0 = pToRemove;
    }    

    PurpleDoom_C* field_0;
    PurpleDoom_C field_4[5999];
    s32 field_11938;
    s32 field_1193C;
    s32 field_11940;
};
