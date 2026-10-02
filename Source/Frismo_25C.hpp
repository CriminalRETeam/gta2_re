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

    s32 field_0_cond_result;
    s16 field_4_return_cmd;
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

    // 9.6f 0x476780
    inline void DeAllocate_476780(Frismo_C* pItem)
    {
        field_0_pool.DeAllocate(pItem);
    }

    PoolBasic<Frismo_C, 50> field_0_pool;
};
GTA2_ASSERT_SIZEOF_ALWAYS(Frismo_C_Pool, 0x25C)

EXTERN_GLOBAL(Frismo_C_Pool*, gFrismo_C_Pool_6F8068);
