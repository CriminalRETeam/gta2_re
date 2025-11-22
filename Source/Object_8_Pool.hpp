#pragma once

#include "Object_8.hpp"

class Object_8_Pool
{
  public:
    Object_8_Pool()
    {
        for (s32 j = 0; j < 1100; j++)
        {
            field_4[j].mpNext = &field_4[j+1];
        }
        field_4[1100].mpNext = 0;
        field_0 = &field_4[0];
    }
    EXPORT ~Object_8_Pool();
    Object_8* field_0;
    Object_8 field_4[1101];
};


EXTERN_GLOBAL(Object_8_Pool*, gObject_8_Pool_6F8F78);
