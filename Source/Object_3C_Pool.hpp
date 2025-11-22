#pragma once

#include "Object_3C.hpp"

class Object_3C_Pool
{
  public:
    Object_3C_Pool()
    {
        Object_3C* pIter = field_4;
        for (s32 k = 0; k < 384; k++)
        {
            pIter->mpNext = pIter + 1;
            pIter++;
        }
        field_4[384].mpNext = NULL;
        field_0 = &field_4[0];
    }
    EXPORT ~Object_3C_Pool();
    Object_3C* field_0;
    Object_3C field_4[385];
};

EXTERN_GLOBAL(Object_3C_Pool*, gObject_3C_Pool_6F8F7C);
