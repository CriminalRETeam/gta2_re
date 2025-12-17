#include "Light_1D4CC.hpp"

DEFINE_GLOBAL(Light_1D4CC*, gLight_1D4CC_6F5520, 0x6F5520);

MATCH_FUNC(0x4bebc0)
Light_1D4CC::~Light_1D4CC()
{
    Light::sub_4D6E30();
}

MATCH_FUNC(0x5c2b70)
nostalgic_ellis_0x28* Light_1D4CC::sub_5C2B70()
{
    nostalgic_ellis_0x28* result = field_0_pool.field_0_pStart;
    field_0_pool.field_0_pStart = field_0_pool.field_0_pStart->mpNext;
    result->mpNext = 0;
    result->field_0.flag = 0;
    result->field_14_on_time = 0;
    return result;
}

MATCH_FUNC(0x52B2A0)
nostalgic_ellis_0x28* Light_1D4CC::sub_52B2A0(Fix16 xpos, Fix16 ypos, Fix16 zpos, s32 argb, Fix16 radius, u8 intensity)
{
    return Init_469010(xpos, ypos, zpos, argb, radius, intensity);
}
