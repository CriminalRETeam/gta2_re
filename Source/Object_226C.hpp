#pragma once

#include "Object_8.hpp"

class Object_226C
{
  public:
    Object_226C()
    {
        for (s32 j = 0; j < 1100; j++)
        {
            field_4[j].field_0 = &field_4[j+1];
        }
        field_4[1100].field_0 = 0;
        field_0 = &field_4[0];
    }
    EXPORT ~Object_226C();
    Object_8* field_0;
    Object_8 field_4[1101];
};


EXTERN_GLOBAL(Object_226C*, gObject_226C_6F8F78);
