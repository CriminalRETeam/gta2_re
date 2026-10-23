#pragma once

#include "Object_3C.hpp"
#include "Pool.hpp"


class Object_3C_Pool : public PoolBasic<Object_3C, 385>
{
  public:
    __forceinline Object_3C_Pool()
    {
    }

    EXPORT ~Object_3C_Pool();
};

GTA2_ASSERT_SIZEOF_ALWAYS(Object_3C_Pool, 0x5A40)

typedef Object_3C_Pool TObject_3C_Pool;

EXTERN_GLOBAL(TObject_3C_Pool*, gObject_3C_Pool_6F8F7C);
