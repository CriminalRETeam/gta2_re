#pragma once

#include "Function.hpp"
#include "rng.hpp"

class nostalgic_ellis_0x28
{
  public:
    EXPORT nostalgic_ellis_0x28();

    EXPORT ~nostalgic_ellis_0x28();

    EXPORT void sub_4D6D70();

    EXPORT nostalgic_ellis_0x28* sub_4D6DC0();

    // 0x45B330
    s32 PoolUpdate()
    {
        field_17_off_time--;
        if (!field_17_off_time)
        {
            if (field_0 & 0xff)
            {
                field_0 &= ~0xff;
                field_17_off_time = field_15_off_time;
                if (field_16_shape)
                {
                    field_17_off_time += stru_6F6784.get_uint8_4F7B70(&field_16_shape);
                }
            }
            else
            {
                field_0 &= ~0xff;
                u8 t = field_18_intensity;
                field_0 |= t;
                field_17_off_time = field_14_on_time;
                if (field_16_shape)
                {
                    field_17_off_time += stru_6F6784.get_uint8_4F7B70(&field_16_shape);
                }
            }
        }
        return 0;
    }

    void PoolDeallocate()
    {
        field_0 = 0x2A2A2A2A;
    }

    // nostalgic_ellis_0x28* field_0;
    s32 field_0; // todo ??
    s32 field_4_light_x;
    s32 field_8_light_y;
    s32 field_C_light_z;
    s32 field_10_argb;
    char_type field_14_on_time;
    char_type field_15_off_time;
    u8 field_16_shape;
    char_type field_17_off_time;
    char_type field_18_intensity;
    char_type field_19;
    s16 field_1A;
    nostalgic_ellis_0x28* mpNext;
    nostalgic_ellis_0x28* field_20;
    nostalgic_ellis_0x28* field_24;
};

class Light
{
  public:
    EXPORT static void __stdcall sub_4D6E00();

    EXPORT static void sub_4D6E30();

    EXPORT static s32 __stdcall sub_4D6E50(s32 a1, s32 a2, s32 a3, s32 a4);
};