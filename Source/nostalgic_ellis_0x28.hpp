#pragma once

#include "Function.hpp"
#include "rng.hpp"
#include "fix16.hpp"

struct LightIntensityRadius
{
    inline void sub_463EF0(u8 unknown)
    {
        flag = (flag & ~0x0000FF00) | (unknown << 8);
    }
    
    inline void SetRadius_463F10(Fix16 radius)
    {
        u8 unknown = (radius * 32).ToInt();
        sub_463EF0(unknown);
    }
    s32 flag;
};

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
            if (field_0.flag & 0xff)
            {
                field_0.flag &= ~0xff;
                field_17_off_time = field_15_off_time;
                if (field_16_shape)
                {
                    field_17_off_time += stru_6F6784.get_uint8_4F7B70(field_16_shape);
                }
            }
            else
            {
                field_0.flag &= ~0xff;
                u8 t = field_18_intensity;
                field_0.flag |= t;
                field_17_off_time = field_14_on_time;
                if (field_16_shape)
                {
                    field_17_off_time += stru_6F6784.get_uint8_4F7B70(field_16_shape);
                }
            }
        }
        return 0;
    }

    // 0x45B320
    void PoolDeallocate()
    {
        field_0.flag = 0x2A2A2A2A;
    }

    void sub_463F50()
    {
        field_0.flag = 0;
        field_14_on_time = 0;
    }

    void sub_45B2D0(u8 intensity)
    {
        field_0.flag = intensity | (field_0.flag & ~0xFF);
    }

    inline void sub_482D60(s32 argb, s32 flags, u8 intensity)
    {
        field_10_argb = argb;
        field_0.SetRadius_463F10(flags);
        sub_45B2D0(intensity);
        field_18_intensity = intensity;
    }

    inline void SetIntensity_476AE0(u8 intensity)
    {
        sub_45B2D0(intensity);
        field_18_intensity = intensity;
    }

    LightIntensityRadius field_0; // todo ??
    Fix16 field_4_light_x;
    Fix16 field_8_light_y;
    Fix16 field_C_light_z;
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

    EXPORT static void __stdcall sub_4D6E50(s32 a1, s32 a2, s32 a3, s32 a4);
};