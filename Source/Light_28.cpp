#include "Light_28.hpp"
#include "Globals.hpp"
#include "crt_stubs.hpp"
#include "gbh_graphics.hpp"
#include <stdlib.h>

DEFINE_GLOBAL(Light_28**, gLightGrid_6F5400, 0x6F5400) ;

MATCH_FUNC(0x4D6D20)
Light_28::Light_28()
{
    field_0.flag = 0x2A2A2A2A;
    field_4_x = 0;
    field_8_y = 0;
    field_C_z = 0;
    field_10_argb = 0xFFFFFF;
    mpNext = 0;
    field_24_pGridPrev = 0;
    field_20_pGridNext = 0;
    field_14_on_time = 0;
    field_15_off_time = 0;
    field_16_flicker_range = 0;
    field_17_timer = 0;
    field_18_intensity = 0;
}

MATCH_FUNC(0x4D6D60)
Light_28::~Light_28()
{
    mpNext = 0;
    field_24_pGridPrev = 0;
    field_20_pGridNext = 0;
}

MATCH_FUNC(0x4D6D70)
void Light_28::AddToGrid_4D6D70()
{
    u32 cell_idx = (field_4_x.ToInt() >> 2) + ((field_8_y.ToInt() >> 2) * 64);
    Light_28* pFirst = gLightGrid_6F5400[cell_idx];
    
    if (!pFirst)
    {
        gLightGrid_6F5400[cell_idx] = this;
        field_20_pGridNext = 0;
        field_24_pGridPrev = 0;
    }
    else
    {
        gLightGrid_6F5400[cell_idx] = this;
        field_24_pGridPrev = 0;
        field_20_pGridNext = pFirst;
        pFirst->field_24_pGridPrev = this;
    }
}

MATCH_FUNC(0x4D6DC0)
Light_28* Light_28::RemoveFromGrid_4D6DC0()
{
    Light_28* pPrev = field_24_pGridPrev;
    if (pPrev)
    {
        pPrev->field_20_pGridNext = field_20_pGridNext;
    }
    else
    {
        *(&gLightGrid_6F5400[64 * (field_8_y.ToInt() >> 2)] + (field_4_x.ToInt() >> 2)) = field_20_pGridNext;
    }

    Light_28* pNext = field_20_pGridNext;
    if (pNext)
    {
        pNext->field_24_pGridPrev = field_24_pGridPrev;
    }
    return pNext;
}

MATCH_FUNC(0x4D6E00)
void __stdcall LightGrid::AllocGrid_4D6E00()
{
    gLightGrid_6F5400 = (Light_28**)crt::malloc(0x4000u);
    for (s32 i = 0; i < 4096; i++)
    {
        gLightGrid_6F5400[i] = 0;
    }
}

MATCH_FUNC(0x4D6E30)
void LightGrid::FreeGrid_4D6E30()
{
    if (gLightGrid_6F5400)
    {
        crt::free(gLightGrid_6F5400);
        gLightGrid_6F5400 = 0;
    }
}

MATCH_FUNC(0x4D6E50)
void __stdcall LightGrid::SubmitLightsInArea_4D6E50(s32 min_x, s32 min_y, s32 max_x, s32 max_y)
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
            Light_28* pLight = gLightGrid_6F5400[(ypos * 64) + xpos];
            while (pLight)
            {
                SLight pSLight;
                pSLight.field_0 = pLight->field_0.flag;
                pSLight.field_4_x = pLight->field_4_x.ToFloat();
                pSLight.field_8_y = pLight->field_8_y.ToFloat();
                pSLight.field_C_z = pLight->field_C_z.ToFloat();
                pSLight.field_10_colour = pLight->field_10_argb;
                pgbh_AddLight(&pSLight);
                pLight = pLight->field_20_pGridNext;
            }
        }
    }
}