#include "nostalgic_ellis_0x28.hpp"
#include <stdlib.h>

nostalgic_ellis_0x28 **g4096_alloc_6F5400;

// nomatch 0x4D6D20
nostalgic_ellis_0x28::nostalgic_ellis_0x28()
{
    field_0 = (nostalgic_ellis_0x28 *)0x2A2A2A2A;
    field_4_light_x = 0;
    field_8_light_y = 0;
    field_C_light_z = 0;
    field_10_argb = 0xFFFFFF;
    field_1C = 0;
    field_24 = 0;
    field_20 = 0;
    field_14_on_time = 0;
    field_15_off_time = 0;
    field_16_shape = 0;
    field_17_off_time = 0;
    field_18_intensity = 0;
}

// nomatch 0x4D6D60
nostalgic_ellis_0x28::~nostalgic_ellis_0x28()
{
    field_1C = 0;
    field_24 = 0;
    field_20 = 0;
}

// nomatch
void nostalgic_ellis_0x28::sub_4D6D70()
{
    s32 idx; // eax
    nostalgic_ellis_0x28 *pOld; // edx

    idx = (this->field_4_light_x >> 16) + (this->field_8_light_y >> 16 << 6);
    pOld = g4096_alloc_6F5400[idx];
    g4096_alloc_6F5400[idx] = this;
    if (pOld)
    {
        this->field_24 = 0;
        this->field_20 = pOld;
        pOld->field_24 = this;
    }
    else
    {
        this->field_20 = 0;
        this->field_24 = 0;
    }
}

// nomatch
nostalgic_ellis_0x28* nostalgic_ellis_0x28::sub_4D6DC0()
{
    nostalgic_ellis_0x28 *v1; // eax
    nostalgic_ellis_0x28 *result; // eax

    v1 = this->field_24;
    if (v1)
    {
        v1->field_20 = this->field_20;
    }
    else
    {
        *(&g4096_alloc_6F5400[64 * (this->field_8_light_y >> 16)] + (this->field_4_light_x >> 16)) = this->field_20;
    }

    result = this->field_20;
    if (result)
    {
        result->field_24 = this->field_24;
    }
    return result;
}

// nomatch
void __stdcall Light::sub_4D6E00()
{
    s32 i; // eax

    g4096_alloc_6F5400 = (nostalgic_ellis_0x28 **)malloc(0x4000u);
    for (i = 0; i < 4096; g4096_alloc_6F5400[i - 1] = 0)
    {
        ++i;
    }
}

// nomatch
void Light::sub_4D6E30()
{
    if (g4096_alloc_6F5400)
    {
        free(g4096_alloc_6F5400);
        g4096_alloc_6F5400 = 0;
    }
}

// stub
s32 __stdcall Light::sub_4D6E50(s32 a1, s32 a2, s32 a3, s32 a4)
{
    return 0;
}
