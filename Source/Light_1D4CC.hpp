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
        field_0_pool.UpdatePool();
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

    inline void sub_464C60(nostalgic_ellis_0x28* pLight)
    {
        pLight->mpNext = field_0_pool.field_4_pPrev;
        field_0_pool.field_4_pPrev = pLight;
    }

    inline nostalgic_ellis_0x28* sub_464C40()
    {
        nostalgic_ellis_0x28* pFirst = field_0_pool.field_0_pStart;
        field_0_pool.field_0_pStart = field_0_pool.field_0_pStart->mpNext;
        pFirst->mpNext = 0;
        pFirst->sub_463F50();
        return pFirst;
    }

    inline void sub_469070(nostalgic_ellis_0x28* pLight, u8 on_time, u8 off_time, u8 shape)
    {
        pLight->field_16_shape = shape;
        pLight->field_14_on_time = on_time;
        pLight->field_15_off_time = off_time;
        pLight->field_17_off_time = off_time;
        pLight->sub_45B2D0(0);
        sub_464C60(pLight);
    }
    
    inline nostalgic_ellis_0x28* Init_469010(Fix16 xpos, Fix16 ypos, Fix16 zpos, s32 argb, Fix16 radius, u8 intensity)
    {
        nostalgic_ellis_0x28* pLight = sub_464C40();
        pLight->field_4_light_x = xpos;
        pLight->field_8_light_y = ypos;
        pLight->field_C_light_z = zpos;
        pLight->field_10_argb = argb;
        pLight->field_0.flag = 0x10000;
        pLight->field_0.sub_463F10(radius);
        pLight->sub_45B2D0(intensity);
        pLight->field_18_intensity = intensity;
        pLight->sub_4D6D70();
        return pLight;
    }

    EXPORT ~Light_1D4CC();
    EXPORT nostalgic_ellis_0x28* sub_5C2B70();
    EXPORT nostalgic_ellis_0x28* sub_52B2A0(Fix16 xpos, Fix16 ypos, Fix16 zpos, s32 argb, Fix16 radius, u8 intensity);
};

EXTERN_GLOBAL(Light_1D4CC*, gLight_1D4CC_6F5520);
