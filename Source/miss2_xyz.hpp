#pragma once

#include "Function.hpp"
#include "fix16.hpp"
#include "Fix16_Vec.hpp"

struct SCR_XY_f
{
    Fix16 field_0_x; // 0x0
    Fix16 field_4_y; // 0x4
};

struct SCR_Rect_f
{
    Fix16_Vec field_0_pos; // 0x0
    SCR_XY_f field_C_size; // 0xC
};

struct SCR_XY_uc
{
    u8 field_0_x; // 0x0
    u8 field_1_y; // 0x1
};

struct SCR_XYZ_uc
{
    u8 field_0_x; // 0x0
    u8 field_1_y; // 0x1
    u8 field_2_z; // 0x2
};
