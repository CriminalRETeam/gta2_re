#pragma once

#include "Function.hpp"
#include "fix16.hpp"
#include "rng.hpp"

struct LightIntensityRadius
{
    inline void SetRadiusByte_463EF0(u8 unknown)
    {
        flag = (flag & ~0x0000FF00) | (unknown << 8);
    }

    inline void SetRadius_463F10(Fix16 radius)
    {
        u8 unknown = (radius * 32).ToInt();
        SetRadiusByte_463EF0(unknown);
    }

    // Out-of-line copy of SetRadius_463F10
    EXPORT void SetRadius_5C5CD0(Fix16 radius);
    s32 flag;
};

class nostalgic_ellis_0x28
{
  public:
    EXPORT nostalgic_ellis_0x28();

    EXPORT ~nostalgic_ellis_0x28();

    EXPORT void AddToGrid_4D6D70();

    EXPORT nostalgic_ellis_0x28* RemoveFromGrid_4D6DC0();

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
                    field_17_off_time += gRng_6F6784.get_uint8_4F7B70(field_16_shape);
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
                    field_17_off_time += gRng_6F6784.get_uint8_4F7B70(field_16_shape);
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

    void Reset_463F50()
    {
        field_0.flag = 0;
        field_14_on_time = 0;
    }

    void SetCurrentIntensity_45B2D0(u8 intensity)
    {
        field_0.flag = intensity | (field_0.flag & ~0xFF);
    }

    inline void SetColourRadiusIntensity_482D60(s32 argb, Fix16 flags, u8 intensity)
    {
        field_10_argb = argb;
        field_0.SetRadius_463F10(flags);
        SetCurrentIntensity_45B2D0(intensity);
        field_18_intensity = intensity;
    }

    inline void SetIntensity_476AE0(u8 intensity)
    {
        SetCurrentIntensity_45B2D0(intensity);
        field_18_intensity = intensity;
    }

    void SetPosition_482D30(Fix16 x, Fix16 y, Fix16 z)
    {
        RemoveFromGrid_4D6DC0();
        field_4_light_x = x;
        field_8_light_y = y;
        field_C_light_z = z;
        AddToGrid_4D6D70();
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
    nostalgic_ellis_0x28* field_20_pGridNext;
    nostalgic_ellis_0x28* field_24_pGridPrev;
};

class Light
{
  public:
    EXPORT static void __stdcall AllocGrid_4D6E00();

    EXPORT static void FreeGrid_4D6E30();

    EXPORT static void __stdcall SubmitLightsInArea_4D6E50(s32 a1, s32 a2, s32 a3, s32 a4);
};