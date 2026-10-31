#pragma once

#include "Function.hpp"
#include "fix16.hpp"
#include "Fix16_Vec.hpp"

struct SCR_XY_f
{
    Fix16 mX; // 0x0
    Fix16 mY; // 0x4
};

struct SCR_Rect_f
{
    Fix16_Vec mPos; // 0x0
    SCR_XY_f mSize; // 0xC
};

struct SCR_XY_uc
{
    u8 mX; // 0x0
    u8 mY; // 0x1
};

struct SCR_XYZ_uc
{
    u8 mX; // 0x0
    u8 mY; // 0x1
    u8 mZ; // 0x2
};
