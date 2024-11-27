#pragma once

#include "Function.hpp"
#include "nostalgic_ellis_0x28.hpp"

class Light_1D4CC
{
  public:
    Light_1D4CC()
    {
        nostalgic_ellis_0x28* p = &field_8[0];
        for (int i = 0; i < 2999; i++)
        {
            p[i].field_1C = &p[i + 1];
        }

        field_8[2999].field_1C = 0;

        field_0 = &field_8[0];

        field_4 = 0;
        field_1D4C8 = 0;
        Light::sub_4D6E00();
    }

    EXPORT ~Light_1D4CC();
    EXPORT nostalgic_ellis_0x28* sub_5C2B70();

    nostalgic_ellis_0x28* field_0;
    nostalgic_ellis_0x28* field_4;
    nostalgic_ellis_0x28 field_8[3000];
    u16 field_1D4C8;
    // s8 t;
    s16 field_1D4CA;
};
