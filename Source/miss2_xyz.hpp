#pragma once

#include "Function.hpp"
#include "fix16.hpp"

struct SCR_XYZ_f
{
    Fix16 field_0_x;
    Fix16 field_4_y;
    Fix16 field_8_z;
};

struct SCR_XY_f
{
    Fix16 field_0_x;
    Fix16 field_4_y;
};

struct SCR_Rect_f
{
    SCR_XYZ_f field_0_pos;
    SCR_XY_f field_C_size;
};

struct SCR_XY_uc
{
    u8 field_0_x;
    u8 field_1_y;
};

struct SCR_XYZ_uc
{
    u8 field_0_x;
    u8 field_1_y;
    u8 field_2_z;
};
