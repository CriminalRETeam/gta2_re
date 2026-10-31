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
class Light
{
  public:
    EXPORT Light();

    EXPORT ~Light();

    EXPORT void AddToGrid();

    EXPORT Light* RemoveFromGrid();

    // 0x45B330
    s32 PoolUpdate()
    {
        mTimer--;
        if (!mTimer)
        {
            if (mIntensityRadius.flag & 0xff)
            {
                mIntensityRadius.flag &= ~0xff;
                mTimer = mOffTime;
                if (mFlickerRange)
                {
                    mTimer += gRng.get_uint8(mFlickerRange);
                }
            }
            else
            {
                mIntensityRadius.flag &= ~0xff;
                u8 intensity = mIntensity;
                mIntensityRadius.flag |= intensity;
                mTimer = mOnTime;
                if (mFlickerRange)
                {
                    mTimer += gRng.get_uint8(mFlickerRange);
                }
            }
        }
        return 0;
    }

    // 0x45B320
    void PoolDeallocate()
    {
        mIntensityRadius.flag = 0x2A2A2A2A;
    }

    void Reset_463F50()
    {
        mIntensityRadius.flag = 0;
        mOnTime = 0;
    }

    void SetCurrentIntensity_45B2D0(u8 intensity)
    {
        mIntensityRadius.flag = intensity | (mIntensityRadius.flag & ~0xFF);
    }

    inline void SetColourRadiusIntensity_482D60(s32 argb, Fix16 radius, u8 intensity)
    {
        mArgb = argb;
        mIntensityRadius.SetRadius_463F10(radius);
        SetCurrentIntensity_45B2D0(intensity);
        mIntensity = intensity;
    }

    inline void SetIntensity_476AE0(u8 intensity)
    {
        SetCurrentIntensity_45B2D0(intensity);
        mIntensity = intensity;
    }

    void SetPosition_482D30(Fix16 x, Fix16 y, Fix16 z)
    {
        RemoveFromGrid();
        mX = x;
        mY = y;
        mZ = z;
        AddToGrid();
    }

    LightIntensityRadius mIntensityRadius; // 0x0: low byte: current intensity, second byte: radius
    Fix16 mX; // 0x4
    Fix16 mY; // 0x8
    Fix16 mZ; // 0xC
    s32 mArgb; // 0x10
    char_type mOnTime; // 0x14
    char_type mOffTime; // 0x15
    u8 mFlickerRange; // 0x16
    char_type mTimer; // 0x17
    char_type mIntensity; // 0x18
    Light* mpNext;
    Light* mpGridNext; // 0x20
    Light* mpGridPrev; // 0x24
};
GTA2_ASSERT_SIZEOF_ALWAYS(Light, 0x28);

// The 64x64 cell grid (4x4 tiles per cell) that the lights are registered in
class LightGrid
{
  public:
    EXPORT static void __stdcall AllocGrid();

    EXPORT static void FreeGrid();

    EXPORT static void __stdcall SubmitLightsInArea(s32 min_x, s32 min_y, s32 max_x, s32 max_y);
};