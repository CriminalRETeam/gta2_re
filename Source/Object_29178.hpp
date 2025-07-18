#pragma once

#include "Object_5C.hpp" // for Object_2C

class Object_29178
{
  public:
    EXPORT ~Object_29178();

    // TODO: 9.6f addr/check
    void Remove(Object_2C* p2C)
    {
        p2C->sub_522180();
        p2C->field_0 = field_0;
        field_0 = p2C;
    }

    Object_2C* field_0;
    Object_2C* field_4;
    Object_2C field_8[3825];
    s16 field_29174;
    s16 field_29176;
};

EXTERN_GLOBAL(Object_29178*, gObject_29178_6F8F80);
