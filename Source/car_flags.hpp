#pragma once

#include "types.hpp"

// Car::field_78_flags masks. Flags named FlagN are not used. Car::IsFlagSet_411930
// and AddFlags_421890 (9.6f inlines) take the mask as an argument, so the original treats this as a plain flag word.
namespace car_flag
{
enum
{
    TrailerDetached = 0x1,
    NoCollide = 0x2,
    NoPassengers = 0x4,
    NoCollisionDamage = 0x8,
    MiniCar = 0x10,
    Flag5 = 0x20,
    Train = 0x40,
    CarAlarm = 0x80,
    Bulletproof = 0x100,
    Rocketproof = 0x200,
    Flameproof = 0x400,
    Jammed = 0x800,
    JammedAccelerator = 0x1000,
    Crushing = 0x2000, // set by SetCrushFlag_49EFC0
    Flag14 = 0x4000,
    Flag15 = 0x8000,
};
} // namespace car_flag


// TODO: CarTurnFlags8 and CarLightFlags8 are unions with the raw flag byte, which the original source can't have been
// (it was either a bitfield or a mask word, like field_78 whose IsFlagSet/AddFlags helpers take masks). Decide which one
// field_A4 / field_A6 really are and drop the other form. See docs/matching_quirks.md.

// Car::field_A6_bf (aliases field_A6_turn_flags).
struct CarTurnFlags8
{
    u8 bTurnRight : 1; // bit 0
    u8 bTurnLeft : 1; // bit 1
    u8 bTurnRightReady : 1; // bit 2
    u8 bTurnLeftReady : 1; // bit 3
    u8 bFlag4 : 1; // bit 4
    u8 bAIHalted : 1; // bit 5
    u8 bFlag6 : 1; // bit 6
    u8 bFlag7 : 1; // bit 7
};

// Car::field_A4_bf (aliases field_A4_light_flags).
struct CarLightFlags8
{
    u8 bBrakeLights : 1; // bit 0
    u8 bHeadlights : 1; // bit 1
    u8 bEmergencyLights : 1; // bit 2
    u8 bLightFlashing : 1; // bit 3
    u8 bHeadlightFlash : 1; // bit 4
    u8 bFlag5 : 1; // bit 5
    u8 bFlag6 : 1; // bit 6
    u8 bFlag7 : 1; // bit 7
};

// Keeps the number of names MapRenderer.cpp sees at the count that makes Draw3SidedDiagonalDownRight_4EF520 and
// Draw4SidedDiagonalUpLeft_4EF880 match (docs/matching_quirks.md). Do not remove without running the whole build.
enum
{
    kCarFlagsNameCount0,
    kCarFlagsNameCount1,
    kCarFlagsNameCount2,
    kCarFlagsNameCount3,
    kCarFlagsNameCount4,
    kCarFlagsNameCount5,
    kCarFlagsNameCount6,
    kCarFlagsNameCount7,
    kCarFlagsNameCount8,
    kCarFlagsNameCount9,
    kCarFlagsNameCount10,
    kCarFlagsNameCount11,
    kCarFlagsNameCount12,
    kCarFlagsNameCount13,
    kCarFlagsNameCount14,
    kCarFlagsNameCount15,
    kCarFlagsNameCount16,
    kCarFlagsNameCount17,
    kCarFlagsNameCount18,
    kCarFlagsNameCount19
};
