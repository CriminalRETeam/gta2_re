#pragma once

#include "Object_3C.hpp"

class Object_5A40
{
  public:
    Object_5A40()
    {
        Object_3C* pIter = field_4;
        for (s32 k = 0; k < 384; k++)
        {
            pIter->field_8 = pIter + 1;
            pIter++;
        }
        field_4[384].field_8 = NULL;
        field_0 = &field_4[0];
    }
    EXPORT ~Object_5A40();
    Object_3C* field_0;
    Object_3C field_4[385];
};

EXTERN_GLOBAL(Object_5A40*, gObject_5A40_6F8F7C);
