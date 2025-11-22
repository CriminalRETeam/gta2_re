#pragma once

#include "Object_3C.hpp"
#include "Pool.hpp"

class Object_3C_Pool
{
  public:
    Object_3C_Pool()
    {

    }
    EXPORT ~Object_3C_Pool();

    PoolBasic<Object_3C, 385> field_0_pool;
};

EXTERN_GLOBAL(Object_3C_Pool*, gObject_3C_Pool_6F8F7C);
