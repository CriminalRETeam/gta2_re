#include "Light.hpp"
#include "Globals.hpp"
#include "crt_stubs.hpp"
#include "gbh_graphics.hpp"
#include <stdlib.h>

DEFINE_GLOBAL(Light**, gLightGrid_6F5400, 0x6F5400) ;

MATCH_FUNC(0x4D6D20)
Light::Light()
{
    mIntensityRadius.flag = 0x2A2A2A2A;
    mX = 0;
    mY = 0;
    mZ = 0;
    mArgb = 0xFFFFFF;
    mpNext = NULL;
    mpGridPrev = NULL;
    mpGridNext = NULL;
    mOnTime = 0;
    mOffTime = 0;
    mFlickerRange = 0;
    mTimer = 0;
    mIntensity = 0;
}

MATCH_FUNC(0x4D6D60)
Light::~Light()
{
    mpNext = NULL;
    mpGridPrev = NULL;
    mpGridNext = NULL;
}

MATCH_FUNC(0x4D6D70)
void Light::AddToGrid()
{
    u32 cell_idx = (mX.ToInt() >> 2) + ((mY.ToInt() >> 2) * 64);
    Light* pFirst = gLightGrid_6F5400[cell_idx];
    
    if (!pFirst)
    {
        gLightGrid_6F5400[cell_idx] = this;
        mpGridNext = NULL;
        mpGridPrev = NULL;
    }
    else
    {
        gLightGrid_6F5400[cell_idx] = this;
        mpGridPrev = NULL;
        mpGridNext = pFirst;
        pFirst->mpGridPrev = this;
    }
}

MATCH_FUNC(0x4D6DC0)
Light* Light::RemoveFromGrid()
{
    Light* pPrev = mpGridPrev;
    if (pPrev)
    {
        pPrev->mpGridNext = mpGridNext;
    }
    else
    {
        *(&gLightGrid_6F5400[64 * (mY.ToInt() >> 2)] + (mX.ToInt() >> 2)) = mpGridNext;
    }

    Light* pNext = mpGridNext;
    if (pNext)
    {
        pNext->mpGridPrev = mpGridPrev;
    }
    return pNext;
}

MATCH_FUNC(0x4D6E00)
void __stdcall LightGrid::AllocGrid()
{
    gLightGrid_6F5400 = static_cast<Light**>(crt::malloc(0x4000u));
    for (s32 i = 0; i < 4096; i++)
    {
        gLightGrid_6F5400[i] = NULL;
    }
}

MATCH_FUNC(0x4D6E30)
void LightGrid::FreeGrid()
{
    if (gLightGrid_6F5400)
    {
        crt::free(gLightGrid_6F5400);
        gLightGrid_6F5400 = NULL;
    }
}

MATCH_FUNC(0x4D6E50)
void __stdcall LightGrid::SubmitLightsInArea(s32 min_x, s32 min_y, s32 max_x, s32 max_y)
{
    min_x = (min_x >> 2) - 2;
    if (min_x < 0)
    {
        min_x = 0;
    }

    min_y = (min_y >> 2) - 2;
    if (min_y < 0)
    {
        min_y = 0;
    }

    max_x = (max_x >> 2) + 2;
    if (max_x > 63)
    {
        max_x = 63;
    }

    max_y = (max_y >> 2) + 2;
    if (max_y > 63)
    {
        max_y = 63;
    }

    for (s32 ypos = min_y; ypos <= max_y; ypos++)
    {
        for (s32 xpos = min_x; xpos <= max_x; xpos++)
        {
            Light* pLight = gLightGrid_6F5400[(ypos * 64) + xpos];
            while (pLight)
            {
                SLight sLight;
                sLight.field_0 = pLight->mIntensityRadius.flag;
                sLight.field_4_x = pLight->mX.ToFloat();
                sLight.field_8_y = pLight->mY.ToFloat();
                sLight.field_C_z = pLight->mZ.ToFloat();
                sLight.field_10_colour = pLight->mArgb;
                pgbh_AddLight(&sLight);
                pLight = pLight->mpGridNext;
            }
        }
    }
}