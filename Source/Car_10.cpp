#include "Car_10.hpp"

const u8 byte_61A808[] = {7, 24, 11, 28};

STUB_FUNC(0x439CD0)
Car_10 *Car_10::sub_439CD0(s32 *a2)
{
    return 0;
}

STUB_FUNC(0x439D40)
void Car_10::sub_439D40(Car_10 *a1, s32 a2, s32 *a3)
{
}

STUB_FUNC(0x439DA0)
s32 Car_10::sub_439DA0(s32 edx0, s32 *a3)
{
    return 0;
}

MATCH_FUNC(0x439E40)
void Car_10::sub_439E40(u8 a2)
{
    field_4 = 6;
    field_0 = 0;
    field_2 = byte_61A808[a2];
}

STUB_FUNC(0x439E60)
s32 Car_10::sub_439E60()
{
    return 0;
}

MATCH_FUNC(0x439EA0)
void Car_10::sub_439EA0()
{
    if (field_4 > 0 && field_4 <= 2)
    {
        field_4 = 5;
    }
}

MATCH_FUNC(0x447330)
Car_10::Car_10()
{
    field_8 = 0;
    field_2 = 0;
    field_0 = 0;
    field_1 = 3;
    field_4 = 0;
    field_C = 0;
}

MATCH_FUNC(0x447350)
Car_10::~Car_10()
{
    field_8 = 0;
}
