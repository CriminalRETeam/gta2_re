#pragma once

#include "types.hpp"

// Sprite::field_2C_bf (aliases field_2C_flags): blend mode (0..3), "no lighting" and a 5 bit alpha value
// (see Sprite::GetBlendMode_4BA200, GetAlphaBits_4BA210 and GetLightingDrawFlag_4B9BA0).
struct SpriteFlags8
{
    u8 blend_mode : 2; // bits 0-1
    u8 bNoLighting : 1; // bit 2
    u8 alpha : 5; // bits 3-7
};

// Keeps the number of names MapRenderer.cpp sees where Draw3SidedDiagonalDownRight_4EF520 and
// Draw4SidedDiagonalUpLeft_4EF880 match (docs/matching_quirks.md). Do not remove without running the whole build.
enum
{
    kSpriteFlagsNameCount0,
    kSpriteFlagsNameCount1,
    kSpriteFlagsNameCount2,
    kSpriteFlagsNameCount3,
    kSpriteFlagsNameCount4,
    kSpriteFlagsNameCount5,
    kSpriteFlagsNameCount6,
    kSpriteFlagsNameCount7,
    kSpriteFlagsNameCount8,
    kSpriteFlagsNameCount9,
    kSpriteFlagsNameCount10,
    kSpriteFlagsNameCount11,
    kSpriteFlagsNameCount12,
    kSpriteFlagsNameCount13,
    kSpriteFlagsNameCount14,
    kSpriteFlagsNameCount15
};
