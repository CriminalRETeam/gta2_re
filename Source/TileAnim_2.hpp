#pragma once

#include "Function.hpp"
#include "types.hpp"

class TileAnim_2
{
public:
    EXPORT s32 sub_5BC260(s16 a1, s16 a2, s16 a3, s16 a4, s16 a5);
    EXPORT s32 sub_5BC2C0();
    EXPORT void Empty_5BC300();
    EXPORT void sub_5BC310();
    EXPORT TileAnim_2* ctor_5BC3A0();
    EXPORT void dtor_5BC470();
    s16 field_0_count;
};
