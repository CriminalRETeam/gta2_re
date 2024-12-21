#pragma once

#include "Function.hpp"

class Car_BC;

struct Taxi_8
{
    u32 field_0;
    Taxi_8* field_4_pNext;
};

class Taxi_324
{
  public:
    // inlined 0x4C0950
    Taxi_324()
    {
        Taxi_8* pIter = &field_4_array[0];
        for (s32 i = 0; i < GTA2_COUNTOF(field_4_array); i++)
        {
            pIter->field_4_pNext = pIter + 1;
            pIter++;
        }
        field_0_pFirst = &field_4_array[0];
        field_320_count = 0;
    }
    Taxi_8* field_0_pFirst;
    Taxi_8 field_4_array[99];
    u32 field_31C;
    u32 field_320_count;
};

class Taxi_4
{
  public:
    // inlined
    void sub_4C09B0()
    {
        sub_4C0980();
    }

    // inlined
    void sub_4C0980()
    {
        field_0 = 0;
    }

    EXPORT u32* sub_457BA0(Car_BC* a2);
    EXPORT s32 sub_457BC0();
    EXPORT s32 sub_457BF0(s32 a2, s32 a3);
    EXPORT Taxi_4();
    EXPORT ~Taxi_4();
    s32 field_0;
};