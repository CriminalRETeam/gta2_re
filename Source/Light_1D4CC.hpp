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

    Pool<Light_28, 3000> field_0_pool;
};

class Light_1D4CC : public LightBase
{
  public:
    Light_1D4CC()
    {
        LightGrid::AllocGrid_4D6E00();
    }

    inline void AddToUpdateList_464C60(Light_28* pLight)
    {
        pLight->mpNext = field_0_pool.field_4_pPrev;
        field_0_pool.field_4_pPrev = pLight;
    }

    inline Light_28* Alloc_464C40()
    {
        Light_28* pFirst = field_0_pool.field_0_pStart;
        field_0_pool.field_0_pStart = field_0_pool.field_0_pStart->mpNext;
        pFirst->mpNext = 0;
        pFirst->Reset_463F50();
        return pFirst;
    }

    inline void SetFlashing_469070(Light_28* pLight, u8 on_time, u8 off_time, u8 shape)
    {
        pLight->field_16_flicker_range = shape;
        pLight->field_14_on_time = on_time;
        pLight->field_15_off_time = off_time;
        pLight->field_17_timer = off_time;
        pLight->SetCurrentIntensity_45B2D0(0);
        AddToUpdateList_464C60(pLight);
    }
    
    inline Light_28* Init_469010(Fix16 xpos, Fix16 ypos, Fix16 zpos, s32 argb, Fix16 radius, u8 intensity)
    {
        Light_28* pLight = Alloc_464C40();
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

    // Same as Init_469010, but TrafficLight_20::Init_5C1D00 calls the out-of-line copies
    // Alloc_5C2B70 and LightIntensityRadius::SetRadius_5C5CD0 in 10.5.
    inline Light_28* InitOutOfLine_469010(Fix16 xpos, Fix16 ypos, Fix16 zpos, s32 argb, Fix16 radius, u8 intensity)
    {
        Light_28* pLight = Alloc_5C2B70();
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

    inline void Free_47F4B0(Light_28* pLight)
    {
        pLight->PoolDeallocate();
        pLight->mpNext = field_0_pool.field_0_pStart;
        field_0_pool.field_0_pStart = pLight;
    }

    // matched https://decomp.me/scratch/cZQwK
    inline void RemoveFromUpdateListAndFree_47F450(Light_28* pLight)
    {
        Light_28* pPrevious = NULL;

        for (Light_28* pCurr = field_0_pool.field_4_pPrev; pCurr; pPrevious = pCurr, pCurr = pCurr->mpNext)
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

    inline void DeallocLight_47F4F0(Light_28* pLight)
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

    EXPORT ~Light_1D4CC();
    EXPORT Light_28* Alloc_5C2B70();
    EXPORT Light_28* CreateLight_52B2A0(Fix16 xpos, Fix16 ypos, Fix16 zpos, s32 argb, Fix16 radius, u8 intensity);
};

EXTERN_GLOBAL(Light_1D4CC*, gLight_1D4CC_6F5520);
