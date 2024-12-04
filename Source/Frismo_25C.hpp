#pragma once

#include "Function.hpp"

class Frismo_C
{
  public:
    EXPORT Frismo_C();
    EXPORT ~Frismo_C();
    EXPORT s32 sub_503110();
    s32 field_0;
    s16 field_4;
    s16 field_6;
    Frismo_C* field_8_next;
};
GTA2_ASSERT_SIZEOF_ALWAYS(Frismo_C, 0xC)

class Frismo_25C
{
  public:
    Frismo_25C()
    {
        for (s32 i = 0; i < 50 - 1; i++)
        {
            field_4[i].field_8_next = &field_4[i + 1];
        }

        field_4[49].field_8_next = 0;
        field_0 = field_4;
    }

    EXPORT ~Frismo_25C();
    Frismo_C* field_0;
    Frismo_C field_4[50];
};
GTA2_ASSERT_SIZEOF_ALWAYS(Frismo_25C, 0x25C)

EXPORT_VAR extern Frismo_25C* gFrismo_25C_6F8068;
