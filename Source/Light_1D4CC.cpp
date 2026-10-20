#include "Light_1D4CC.hpp"

DEFINE_GLOBAL(LightPool*, gLight_1D4CC_6F5520, 0x6F5520);

MATCH_FUNC(0x4bebc0)
LightPool::~LightPool()
{
    LightGrid::FreeGrid_4D6E30();
}

MATCH_FUNC(0x52B2A0)
Light* LightPool::CreateLight_52B2A0(Fix16 xpos, Fix16 ypos, Fix16 zpos, s32 argb, Fix16 radius, u8 intensity)
{
    return Init_469010(xpos, ypos, zpos, argb, radius, intensity);
}

MATCH_FUNC(0x5c2b70)
Light* LightPool::Alloc_5C2B70()
{
    Light* result = field_0_pool.field_0_pStart;
    field_0_pool.field_0_pStart = field_0_pool.field_0_pStart->mpNext;
    result->mpNext = 0;
    result->field_0.flag = 0;
    result->field_14_on_time = 0;
    return result;
}

// Out-of-line copy of LightIntensityRadius::SetRadius_463F10 (26 bytes).
MATCH_FUNC(0x5C5CD0)
void LightIntensityRadius::SetRadius_5C5CD0(Fix16 radius)
{
    SetRadius_463F10(radius);
}
