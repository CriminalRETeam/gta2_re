#pragma once

#include "Object_8.hpp"
#include "Pool.hpp"


class Object_8_Pool : public PoolBasic<Object_8, 1101>
{
  public:
    __forceinline Object_8_Pool()
    {
    }

    EXPORT ~Object_8_Pool();
};

GTA2_ASSERT_SIZEOF_ALWAYS(Object_8_Pool, 0x226C)

typedef Object_8_Pool TObject_8_Pool;

EXTERN_GLOBAL(TObject_8_Pool*, gObject_8_Pool_6F8F78);
