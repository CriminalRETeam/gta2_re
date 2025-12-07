#pragma once

#include "Function.hpp"

class Object_8
{
  public:
    EXPORT Object_8();
    EXPORT ~Object_8();

    inline void PoolAllocate()
    {
        field_6 = 0;
        field_7 = 0;
        field_4 = 0;
    }

    Object_8* mpNext;
    s16 field_4;
    char_type field_6;
    char_type field_7;
};