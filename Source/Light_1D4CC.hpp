#pragma once

#include "Function.hpp"
#include "nostalgic_ellis_0x28.hpp"
#include "rng.hpp"

class LightBase
{
  public:
    LightBase()
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
    }

    void sub_45BF50()
    {
        nostalgic_ellis_0x28* pCurLight = field_4;
        field_1D4C8 = 0;
        
        nostalgic_ellis_0x28* pNext;
        while (pCurLight)
        {
            ++field_1D4C8;
            
            pNext = pCurLight->field_1C;
            pCurLight->field_17_off_time--;
            if (!pCurLight->field_17_off_time )
            {
                if (pCurLight->field_0 & 0xff) 
                {
                    pCurLight->field_0 &= ~0xff;
                    pCurLight->field_17_off_time = pCurLight->field_15_off_time;
                    if (pCurLight->field_16_shape)
                    {
                        pCurLight->field_17_off_time += stru_6F6784.get_uint8_4F7B70(&pCurLight->field_16_shape);
                    }
                }
                else
                {
                    pCurLight->field_0 &= ~0xff;
                    u8  t = pCurLight->field_18_intensity;
                    pCurLight->field_0 |= t;
                    pCurLight->field_17_off_time = pCurLight->field_14_on_time;
                    if (pCurLight->field_16_shape)
                    {
                        pCurLight->field_17_off_time += stru_6F6784.get_uint8_4F7B70(&pCurLight->field_16_shape);
                    }
                }
            }
            pCurLight = pNext;
        }
    }

    void sub_45C1E0()
    {
        sub_45BF50();
    }

    nostalgic_ellis_0x28* field_0;
    nostalgic_ellis_0x28* field_4;
    nostalgic_ellis_0x28 field_8[3000];
    u16 field_1D4C8;
    s16 field_1D4CA;
};

class Light_1D4CC : public LightBase
{
  public:
    Light_1D4CC()
    {
        Light::sub_4D6E00();
    }

    EXPORT ~Light_1D4CC();
    EXPORT nostalgic_ellis_0x28* sub_5C2B70();
};