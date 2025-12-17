#include "nostalgic_ellis_0x28.hpp"
#include "Globals.hpp"
#include "crt_stubs.hpp"
#include "gbh_graphics.hpp"
#include <stdlib.h>

DEFINE_GLOBAL(nostalgic_ellis_0x28**, g4096_alloc_6F5400, 0x6F5400) ;

MATCH_FUNC(0x4D6D20)
nostalgic_ellis_0x28::nostalgic_ellis_0x28()
{
    field_0.flag = 0x2A2A2A2A;
    field_4_light_x = 0;
    field_8_light_y = 0;
    field_C_light_z = 0;
    field_10_argb = 0xFFFFFF;
    mpNext = 0;
    field_24 = 0;
    field_20 = 0;
    field_14_on_time = 0;
    field_15_off_time = 0;
    field_16_shape = 0;
    field_17_off_time = 0;
    field_18_intensity = 0;
}

MATCH_FUNC(0x4D6D60)
nostalgic_ellis_0x28::~nostalgic_ellis_0x28()
{
    mpNext = 0;
    field_24 = 0;
    field_20 = 0;
}

MATCH_FUNC(0x4D6D70)
void nostalgic_ellis_0x28::sub_4D6D70()
{
    u32 v1 = (field_4_light_x.ToInt() >> 2) + ((field_8_light_y.ToInt() >> 2) * 64);
    nostalgic_ellis_0x28* v2 = g4096_alloc_6F5400[v1];
    
    if (!v2)
    {
        g4096_alloc_6F5400[v1] = this;
        field_20 = 0;
        field_24 = 0;
    }
    else
    {
        g4096_alloc_6F5400[v1] = this;
        field_24 = 0;
        field_20 = v2;
        v2->field_24 = this;
    }
}

STUB_FUNC(0x4D6DC0)
nostalgic_ellis_0x28* nostalgic_ellis_0x28::sub_4D6DC0()
{
    NOT_IMPLEMENTED;
    nostalgic_ellis_0x28* v1; // eax
    nostalgic_ellis_0x28* result; // eax

    v1 = field_24;
    if (v1)
    {
        v1->field_20 = field_20;
    }
    else
    {
        *(&g4096_alloc_6F5400[64 * (field_8_light_y.ToInt())] + (field_4_light_x.ToInt())) = field_20;
    }

    result = field_20;
    if (result)
    {
        result->field_24 = field_24;
    }
    return result;
}

MATCH_FUNC(0x4D6E00)
void __stdcall Light::sub_4D6E00()
{
    g4096_alloc_6F5400 = (nostalgic_ellis_0x28**)crt::malloc(0x4000u);
    for (s32 i = 0; i < 4096; i++)
    {
        g4096_alloc_6F5400[i] = 0;
    }
}

MATCH_FUNC(0x4D6E30)
void Light::sub_4D6E30()
{
    if (g4096_alloc_6F5400)
    {
        crt::free(g4096_alloc_6F5400);
        g4096_alloc_6F5400 = 0;
    }
}

// https://decomp.me/scratch/eOl7e
STUB_FUNC(0x4D6E50)
void __stdcall Light::sub_4D6E50(s32 min_x, s32 min_y, s32 max_x, s32 max_y)
{
    NOT_IMPLEMENTED;
    
    s32 min_x_offset = (min_x >> 2) - 2;
    if (min_x_offset < 0)
    {
        min_x_offset = 0;
    }

    s32 min_y_offset = (min_y >> 2) - 2;
    if (min_y_offset < 0)
    {
        min_y_offset = 0;
    }

    s32 max_x_offset = (max_x >> 2) + 2;
    if (max_x_offset > 63)
    {
        max_x_offset = 63;
    }

    s32 max_y_offset = (max_y >> 2) + 2;
    if (max_y_offset > 63)
    {
        max_y_offset = 63;
    }

    for (s32 ypos = min_y_offset; ypos <= max_y_offset; ypos++)
    {
        for (s32 xpos = min_x_offset; xpos <= max_x_offset; xpos++)
        {
            nostalgic_ellis_0x28* pLight = g4096_alloc_6F5400[(ypos * 64) + xpos];
            for (; pLight != NULL; pLight = pLight->field_20)
            {
                SLight pSLight;
                pSLight.field_0 = pLight->field_0.flag;
                pSLight.field_4_x = pLight->field_4_light_x.ToFloat();
                pSLight.field_8_y = pLight->field_8_light_y.ToFloat();
                pSLight.field_C_z = pLight->field_C_light_z.ToFloat();
                pSLight.field_10_colour = pLight->field_10_argb;
                pgbh_AddLight(&pSLight);
            }
        }
    }
}