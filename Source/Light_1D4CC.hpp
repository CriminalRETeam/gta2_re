#pragma once

#include "Function.hpp"
#include "Light_28.hpp"
#include "rng.hpp"
#include "Pool.hpp"

class LightBase
{
  public:
    LightBase()
    {

    }

    void UpdatePool_45BF50()
    {
        field_0_pool.UpdatePool();
    }

    void Service_45C1E0()
    {
        UpdatePool_45BF50();
    }

    Pool<Light, 3000> field_0_pool;
};

class LightPool : public LightBase
{
  public:
    LightPool()
    {
        LightGrid::AllocGrid_4D6E00();
    }

    inline void AddToUpdateList_464C60(Light* pLight)
    {
        pLight->mpNext = field_0_pool.field_4_pPrev;
        field_0_pool.field_4_pPrev = pLight;
    }

    inline Light* Alloc_464C40()
    {
        Light* pFirst = field_0_pool.field_0_pStart;
        field_0_pool.field_0_pStart = field_0_pool.field_0_pStart->mpNext;
        pFirst->mpNext = 0;
        pFirst->Reset_463F50();
        return pFirst;
    }

    inline void SetFlashing_469070(Light* pLight, u8 on_time, u8 off_time, u8 shape)
    {
        pLight->field_16_flicker_range = shape;
        pLight->field_14_on_time = on_time;
        pLight->field_15_off_time = off_time;
        pLight->field_17_timer = off_time;
        pLight->SetCurrentIntensity_45B2D0(0);
        AddToUpdateList_464C60(pLight);
    }
    
    inline Light* Init_469010(Fix16 xpos, Fix16 ypos, Fix16 zpos, s32 argb, Fix16 radius, u8 intensity)
    {
        Light* pLight = Alloc_464C40();
        pLight->field_4_x = xpos;
        pLight->field_8_y = ypos;
        pLight->field_C_z = zpos;
        pLight->field_10_argb = argb;
        pLight->field_0.flag = 0x10000;
        pLight->field_0.SetRadius_463F10(radius);
        pLight->SetCurrentIntensity_45B2D0(intensity);
        pLight->field_18_intensity = intensity;
        pLight->AddToGrid_4D6D70();
        return pLight;
    }

    // Same as Init_469010, but TrafficLight::Init_5C1D00 calls the out-of-line copies
    // Alloc_5C2B70 and LightIntensityRadius::SetRadius_5C5CD0 in 10.5.
    inline Light* InitOutOfLine_469010(Fix16 xpos, Fix16 ypos, Fix16 zpos, s32 argb, Fix16 radius, u8 intensity)
    {
        Light* pLight = Alloc_5C2B70();
        pLight->field_4_x = xpos;
        pLight->field_8_y = ypos;
        pLight->field_C_z = zpos;
        pLight->field_10_argb = argb;
        pLight->field_0.flag = 0x10000;
        pLight->field_0.SetRadius_5C5CD0(radius);
        pLight->SetCurrentIntensity_45B2D0(intensity);
        pLight->field_18_intensity = intensity;
        pLight->AddToGrid_4D6D70();
        return pLight;
    }

    inline void Free_47F4B0(Light* pLight)
    {
        pLight->PoolDeallocate();
        pLight->mpNext = field_0_pool.field_0_pStart;
        field_0_pool.field_0_pStart = pLight;
    }

    // matched https://decomp.me/scratch/cZQwK
    inline void RemoveFromUpdateListAndFree_47F450(Light* pLight)
    {
        Light* pPrevious = NULL;

        for (Light* pCurr = field_0_pool.field_4_pPrev; pCurr; pPrevious = pCurr, pCurr = pCurr->mpNext)
        {
            if (pCurr == pLight)
            {
                pCurr->PoolDeallocate();
                if (pPrevious != NULL)
                {
                    pPrevious->mpNext = pCurr->mpNext;
                }
                else
                {
                    field_0_pool.field_4_pPrev = pCurr->mpNext;
                }
                pCurr->mpNext = field_0_pool.field_0_pStart;
                field_0_pool.field_0_pStart = pCurr;
                break;
            }
        }
    }

    inline void DeallocLight_47F4F0(Light* pLight)
    {
        pLight->RemoveFromGrid_4D6DC0();
        if (pLight->field_14_on_time)
        {
            //RemoveFromUpdateListAndFree_47F450(pLight);
            field_0_pool.FindAndDeAllocate(pLight);
        }
        else
        {
            field_0_pool.DeAllocate(pLight);
            //Free_47F4B0(pLight);
        }
    }

    EXPORT ~LightPool();
    EXPORT Light* Alloc_5C2B70();
    EXPORT Light* CreateLight_52B2A0(Fix16 xpos, Fix16 ypos, Fix16 zpos, s32 argb, Fix16 radius, u8 intensity);
};
GTA2_ASSERT_SIZEOF_ALWAYS(LightPool, 0x1D4CC);

EXTERN_GLOBAL(LightPool*, gLight_1D4CC_6F5520);
