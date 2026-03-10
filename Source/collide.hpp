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

class Collide_8_Pool : public PoolBasic<Collide_8, 4096>
{
  public:
    Collide_8_Pool()
    {

    }

    ~Collide_8_Pool()
    {
      this->field_0_pHead = 0;
    }
};

class PurpleDoom_C_Pool : public PoolBasic<PurpleDoom_C, 6000>
{
  public:
    PurpleDoom_C_Pool()
    {

    }

    ~PurpleDoom_C_Pool()
    {
      this->field_0_pHead = 0;
    }
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

EXTERN_GLOBAL(Collide_C*, gCollide_C_6791FC);
