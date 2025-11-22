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
typedef PoolBasic<PurpleDoom_C, 6000> T_PurpleDoom_C_Pool;

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
