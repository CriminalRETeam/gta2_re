#pragma once

#include "Object_8.hpp"
#include "Pool.hpp"

class Object_8_Pool
{
  public:
    Object_8_Pool()
    {

    }
    EXPORT ~Object_8_Pool();

    PoolBasic<Object_8, 1101> field_0_pool;
};


EXTERN_GLOBAL(Object_8_Pool*, gObject_8_Pool_6F8F78);
