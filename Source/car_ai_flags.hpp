#pragma once

#include "types.hpp"

// CarAI::field_24_bf. Same layout as CompilerBitField32 (the bit field form is needed to match), with the meaning of
// each bit where it is understood. Bits named bFlagN are used but their meaning is not understood yet.
struct CarAIFlags32
{
    u32 bFlag0 : 1; // bit 0
    u32 bInsideJunction : 1; // bit 1
    u32 bFlag2 : 1; // bit 2
    u32 bFlag3 : 1; // bit 3
    u32 bFlag4 : 1; // bit 4
    u32 bFlag5 : 1; // bit 5
    u32 bReversing : 1; // bit 6
    u32 bTurning : 1; // bit 7
    u32 bChangingLane : 1; // bit 8
    u32 bFlag9 : 1; // bit 9
    u32 bFlag10 : 1; // bit 10
    u32 bTurnPlanned : 1; // bit 11
    u32 bCollidedWithCar : 1; // bit 12
    u32 bArrowSteerAssist : 1; // bit 13
    u32 bTargetSetFromArrows : 1; // bit 14
    u32 bFlag15 : 1; // bit 15
    u32 bFlag16 : 1; // bit 16
    u32 bHasRightOfWay : 1; // bit 17
    u32 bFlag18 : 1; // bit 18
    u32 bFlag19 : 1; // bit 19
    u32 bAIHalted : 1; // bit 20
    u32 bDriveAggressively : 1; // bit 21
    u32 bFlag22 : 1; // bit 22
    u32 bFlag23 : 1; // bit 23
    u32 bFlag24 : 1; // bit 24
    u32 bFlag25 : 1; // bit 25
    u32 bFlag26 : 1; // bit 26
    u32 bFlag27 : 1; // bit 27
    u32 bFlag28 : 1; // bit 28
    u32 bFlag29 : 1; // bit 29
    u32 bFlag30 : 1; // bit 30
    u32 bFlag31 : 1; // bit 31
};
