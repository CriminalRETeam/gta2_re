#pragma once

#include "Function.hpp"
#include "ProfilerTimer.hpp"

class Hud_TextEntry;

class FpsCounter
{
  public:
    EXPORT void ShowFps();
    EXPORT FpsCounter();
    EXPORT ~FpsCounter();
    EXPORT void UpdateFpsCounters(char_type bDrewFrame, char_type bUpdatedLogic);

    // Update (game logic) counter: field_0 counts updates, every 100 the rate is recomputed
    s32 mUpdateCount; // 0x0
    s32 mUpdateStartTime; // 0x4
    s32 mUpdateFps; // 0x8
    s32 mDrawCount; // 0xC
    s32 mDrawStartTime; // 0x10
    s32 mDrawFps; // 0x14
    // Only field_18 is ever started/accumulated (ExecuteGame_4DA780); the others only get their percentage computed
    ProfilerTimer mExecuteGameTimer; // 0x18
    ProfilerTimer mTimer1; // 0x24
    ProfilerTimer mTimer2; // 0x30
    ProfilerTimer mTimer3; // 0x3C
    ProfilerTimer mTimer4; // 0x48
};
GTA2_ASSERT_SIZEOF_ALWAYS(FpsCounter, 0x54);

EXTERN_GLOBAL(FpsCounter*, gFpsCounter);
