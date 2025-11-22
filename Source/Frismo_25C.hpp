#pragma once

#include "Function.hpp"

class Frismo_C
{
  public:
    EXPORT Frismo_C();
    EXPORT ~Frismo_C();
    EXPORT void sub_503110();
    s32 field_0;
    s16 field_4;
    s16 field_6;
    Frismo_C* mpNext;
};
GTA2_ASSERT_SIZEOF_ALWAYS(Frismo_C, 0xC)

class Frismo_C_Pool
{
  public:
    Frismo_C_Pool()
    {
        Frismo_C* pIter = &field_4[0];
        for (s32 i = 0; i < 50 - 1; i++)
        {
            pIter->mpNext = pIter + 1;
            pIter++;
        }

        field_4[49].mpNext = 0;
        field_0 = field_4;
    }

    EXPORT ~Frismo_C_Pool();
    Frismo_C* field_0;
    Frismo_C field_4[50];
};
GTA2_ASSERT_SIZEOF_ALWAYS(Frismo_C_Pool, 0x25C)

EXTERN_GLOBAL(Frismo_C_Pool*, gFrismo_C_Pool_6F8068);
