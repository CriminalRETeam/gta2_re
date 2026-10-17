#pragma once

#include "Function.hpp"
#include "fix16.hpp"
#include "rng.hpp"

struct LightIntensityRadius
{
    inline void SetRadiusByte_463EF0(u8 radius_byte)
    {
        flag = (flag & ~0x0000FF00) | (radius_byte << 8);
    }

    inline void SetRadius_463F10(Fix16 radius)
    {
        u8 radius_byte = (radius * 32).ToInt();
        SetRadiusByte_463EF0(radius_byte);
    }

    // Out-of-line copy of SetRadius_463F10
    EXPORT void SetRadius_5C5CD0(Fix16 radius);
    s32 flag;
};

// A point light: position, colour, radius and an on/off flicker cycle. Registered in LightGrid for rendering.
class Light_28
{
  public:
    EXPORT Light_28();

    EXPORT ~Light_28();

    EXPORT void AddToGrid_4D6D70();

    EXPORT Light_28* RemoveFromGrid_4D6DC0();

    // 0x45B330
    s32 PoolUpdate()
    {
        field_17_timer--;
        if (!field_17_timer)
        {
            if (field_0.flag & 0xff)
            {
                field_0.flag &= ~0xff;
                field_17_timer = field_15_off_time;
                if (field_16_flicker_range)
                {
                    field_17_timer += gRng_6F6784.get_uint8_4F7B70(field_16_flicker_range);
                }
            }
            else
            {
                field_0.flag &= ~0xff;
                u8 t = field_18_intensity;
                field_0.flag |= t;
                field_17_timer = field_14_on_time;
                if (field_16_flicker_range)
                {
                    field_17_timer += gRng_6F6784.get_uint8_4F7B70(field_16_flicker_range);
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

    inline void SetColourRadiusIntensity_482D60(s32 argb, Fix16 radius, u8 intensity)
    {
        field_10_argb = argb;
        field_0.SetRadius_463F10(radius);
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
        field_4_x = x;
        field_8_y = y;
        field_C_z = z;
        AddToGrid_4D6D70();
    }

    LightIntensityRadius field_0; // low byte: current intensity, second byte: radius
    Fix16 field_4_x;
    Fix16 field_8_y;
    Fix16 field_C_z;
    s32 field_10_argb;
    char_type field_14_on_time;
    char_type field_15_off_time;
    u8 field_16_flicker_range;
    char_type field_17_timer;
    char_type field_18_intensity;
    Light_28* mpNext;
    Light_28* field_20_pGridNext;
    Light_28* field_24_pGridPrev;
};

// The 64x64 cell grid (4x4 tiles per cell) that the lights are registered in
class LightGrid
{
  public:
    EXPORT static void __stdcall AllocGrid_4D6E00();

    EXPORT static void FreeGrid_4D6E30();

    EXPORT static void __stdcall SubmitLightsInArea_4D6E50(s32 min_x, s32 min_y, s32 max_x, s32 max_y);
};