#pragma once

#include "Function.hpp"
#include "nostalgic_ellis_0x28.hpp"
#include "rng.hpp"
#include "Pool.hpp"

class LightBase
{
  public:
    LightBase()
    {

    }

    void sub_45BF50()
    {
        nostalgic_ellis_0x28* pCurLight = field_0_pool.field_4_pPrev;
        field_0_pool.field_X_count = 0;
        
        nostalgic_ellis_0x28* pNext;
        while (pCurLight)
        {
            ++field_0_pool.field_X_count;
            
            pNext = pCurLight->mpNext;
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
                    u8 t = pCurLight->field_18_intensity;
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

    Pool<nostalgic_ellis_0x28, 3000> field_0_pool;
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
