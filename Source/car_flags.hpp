#pragma once

#include "types.hpp"

// Car_BC::field_78_bf (aliases field_78_flags). Bits named bFlagN are used but their meaning is not understood yet.
// The mask helpers (IsFlagSet_411930, add_f78_bits_421890, ...) are 9.6f inlines and must stay: a bit field form of
// them changes the code.
struct CarFlags16
{
    u16 bTrailerDetached : 1; // bit 0
    u16 bNoCollide : 1; // bit 1
    u16 bNoPassengers : 1; // bit 2
    u16 bFlag3 : 1; // bit 3
    u16 bFlag4 : 1; // bit 4
    u16 bFlag5 : 1; // bit 5
    u16 bFlag6 : 1; // bit 6
    u16 bCarAlarm : 1; // bit 7
    u16 bBulletproof : 1; // bit 8
    u16 bRocketproof : 1; // bit 9
    u16 bFlameproof : 1; // bit 10
    u16 bJammed : 1; // bit 11
    u16 bFlag12 : 1; // bit 12
    u16 bFlag13 : 1; // bit 13
    u16 bFlag14 : 1; // bit 14
    u16 bFlag15 : 1; // bit 15
};

// Keeps the number of names MapRenderer.cpp sees at the count that makes Draw3SidedDiagonalDownRight_4EF520 and
// Draw4SidedDiagonalUpLeft_4EF880 match (docs/matching_quirks.md). Do not remove without running the whole build.
enum
{
    kCarFlagsNameCount0,
    kCarFlagsNameCount1,
    kCarFlagsNameCount2,
    kCarFlagsNameCount3,
    kCarFlagsNameCount4
};
