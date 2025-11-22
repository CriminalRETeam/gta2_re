#pragma once

#include "Object_5C.hpp" // for Object_2C
#include "Pool.hpp"

class Object_2C_Pool
{
  public:
    Object_2C_Pool()
    {

    }

    EXPORT ~Object_2C_Pool();

    // TODO: 9.6f addr/check
    void Remove(Object_2C* p2C)
    {
        // TODO: Use pool method
        p2C->sub_522180();
        p2C->mpNext = field_0_pool.field_0_pStart;
        field_0_pool.field_0_pStart = p2C;
    }

    Pool<Object_2C, 3825> field_0_pool;
};

EXTERN_GLOBAL(Object_2C_Pool*, gObject_2C_Pool_6F8F80);
