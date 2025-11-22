#pragma once

#include "Function.hpp"
#include "Pool.hpp"

class Frismo_C
{
  public:
    EXPORT Frismo_C();
    EXPORT ~Frismo_C();
    EXPORT void PoolAllocate();

    void PoolDeallocate()
    {
      
    }

    s32 field_0;
    s16 field_4;
    s16 field_6;
    Frismo_C* mpNext;
};
GTA2_ASSERT_SIZEOF_ALWAYS(Frismo_C, 0xC)

class Frismo_C_Pool
{
  public:
    Frismo_C_Pool()
    {

    }

    EXPORT ~Frismo_C_Pool();

    PoolBasic<Frismo_C, 50> field_0_pool;
};
GTA2_ASSERT_SIZEOF_ALWAYS(Frismo_C_Pool, 0x25C)

EXTERN_GLOBAL(Frismo_C_Pool*, gFrismo_C_Pool_6F8068);
