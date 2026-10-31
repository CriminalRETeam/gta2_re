#pragma once

#include "Function.hpp"
#include "ProfilerTimer.hpp"

class Hud_TextEntry;

class FpsCounter
{
  public:
    EXPORT void ShowFps_5BEC30();
    EXPORT FpsCounter();
    EXPORT ~FpsCounter();
    EXPORT void UpdateFpsCounters_5BECF0(char_type bDrewFrame, char_type bUpdatedLogic);

    // Update (game logic) counter: field_0 counts updates, every 100 the rate is recomputed
    s32 field_0_update_count; // 0x0
    s32 field_4_update_start_time; // 0x4
    s32 field_8_update_fps; // 0x8
    s32 field_C_draw_count; // 0xC
    s32 field_10_draw_start_time; // 0x10
    s32 field_14_draw_fps; // 0x14
    // Only field_18 is ever started/accumulated (ExecuteGame_4DA780); the others only get their percentage computed
    ProfilerTimer field_18_execute_game_timer; // 0x18
    ProfilerTimer field_24_timer_1; // 0x24
    ProfilerTimer field_30_timer_2; // 0x30
    ProfilerTimer field_3C_timer_3; // 0x3C
    ProfilerTimer field_48_timer_4; // 0x48
};
GTA2_ASSERT_SIZEOF_ALWAYS(FpsCounter, 0x54);

EXTERN_GLOBAL(FpsCounter*, gFpsCounter_7055D4);
