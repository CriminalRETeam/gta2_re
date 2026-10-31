#pragma once

#include "Function.hpp"
#include "Light.hpp"
#include "rng.hpp"
#include "Pool.hpp"

class LightBase
{
  public:
    LightBase()
    {

    }

    // 9.6f 0x45BF50
    void UpdatePool()
    {
        mPool.UpdatePool();
    }

    // 9.6f 0x45C1E0
    void Service()
    {
        UpdatePool();
    }

    Pool<Light, 3000> mPool; // 0x0
};

class LightPool : public LightBase
{
  public:
    LightPool()
    {
        LightGrid::AllocGrid();
    }

    // 9.6f 0x464C60
    inline void AddToUpdateList(Light* pLight)
    {
        pLight->mpNext = mPool.mpPrev;
        mPool.mpPrev = pLight;
    }

    // 9.6f 0x464C40
    inline Light* Alloc()
    {
        Light* pFirst = mPool.mpStart;
        mPool.mpStart = mPool.mpStart->mpNext;
        pFirst->mpNext = NULL;
        pFirst->Reset();
        return pFirst;
    }

    // 9.6f 0x469070
    inline void SetFlashing(Light* pLight, u8 on_time, u8 off_time, u8 shape)
    {
        pLight->mFlickerRange = shape;
        pLight->mOnTime = on_time;
        pLight->mOffTime = off_time;
        pLight->mTimer = off_time;
        pLight->SetCurrentIntensity(0);
        AddToUpdateList(pLight);
    }
    
    inline Light* Init_469010(Fix16 xpos, Fix16 ypos, Fix16 zpos, s32 argb, Fix16 radius, u8 intensity)
    {
        Light* pLight = Alloc();
        pLight->mX = xpos;
        pLight->mY = ypos;
        pLight->mZ = zpos;
        pLight->mArgb = argb;
        pLight->mIntensityRadius.flag = 0x10000;
        pLight->mIntensityRadius.SetRadius(radius);
        pLight->SetCurrentIntensity(intensity);
        pLight->mIntensity = intensity;
        pLight->AddToGrid();
        return pLight;
    }

    // Same as Init_469010, but TrafficLight::Init calls the out-of-line copies
    // Alloc_5C2B70 and LightIntensityRadius::SetRadius_5C5CD0 in 10.5.
    inline Light* InitOutOfLine_469010(Fix16 xpos, Fix16 ypos, Fix16 zpos, s32 argb, Fix16 radius, u8 intensity)
    {
        Light* pLight = Alloc_5C2B70();
        pLight->mX = xpos;
        pLight->mY = ypos;
        pLight->mZ = zpos;
        pLight->mArgb = argb;
        pLight->mIntensityRadius.flag = 0x10000;
        pLight->mIntensityRadius.SetRadius_5C5CD0(radius);
        pLight->SetCurrentIntensity(intensity);
        pLight->mIntensity = intensity;
        pLight->AddToGrid();
        return pLight;
    }

    // 9.6f 0x47F4B0
    inline void Free(Light* pLight)
    {
        pLight->PoolDeallocate();
        pLight->mpNext = mPool.mpStart;
        mPool.mpStart = pLight;
    }

    // matched https://decomp.me/scratch/cZQwK
    inline void RemoveFromUpdateListAndFree(Light* pLight)
    {
        Light* pPrevious = NULL;

        for (Light* pCurr = mPool.mpPrev; pCurr; pPrevious = pCurr, pCurr = pCurr->mpNext)
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
                    mPool.mpPrev = pCurr->mpNext;
                }
                pCurr->mpNext = mPool.mpStart;
                mPool.mpStart = pCurr;
                break;
            }
        }
    }

    // 9.6f 0x47F4F0
    inline void DeallocLight(Light* pLight)
    {
        pLight->RemoveFromGrid();
        if (pLight->mOnTime)
        {
            mPool.FindAndDeAllocate(pLight);
        }
        else
        {
            mPool.DeAllocate(pLight);
        }
    }

    EXPORT ~LightPool();
    EXPORT Light* Alloc_5C2B70();
    EXPORT Light* CreateLight(Fix16 xpos, Fix16 ypos, Fix16 zpos, s32 argb, Fix16 radius, u8 intensity);
};
GTA2_ASSERT_SIZEOF_ALWAYS(LightPool, 0x1D4CC);

EXTERN_GLOBAL(LightPool*, gLight_1D4CC);
