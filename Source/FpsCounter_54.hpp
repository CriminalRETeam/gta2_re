#pragma once

#include "Function.hpp"
#include "ProfilerTimer_C.hpp"

class Hud_TextEntry_C4;

class FpsCounter_54
{
  public:
    EXPORT void ShowFps_5BEC30();
    EXPORT FpsCounter_54();
    EXPORT ~FpsCounter_54();
    EXPORT void UpdateFpsCounters_5BECF0(char_type bDrewFrame, char_type bUpdatedLogic);

    // Update (game logic) counter: field_0 counts updates, every 100 the rate is recomputed
    s32 field_0_update_count;
    s32 field_4_update_start_time;
    s32 field_8_update_fps;
    s32 field_C_draw_count;
    s32 field_10_draw_start_time;
    s32 field_14_draw_fps;
    // Only field_18 is ever started/accumulated (ExecuteGame_4DA780); the others only get their percentage computed
    ProfilerTimer_C field_18_execute_game_timer;
    ProfilerTimer_C field_24_timer_1;
    ProfilerTimer_C field_30_timer_2;
    ProfilerTimer_C field_3C_timer_3;
    ProfilerTimer_C field_48_timer_4;
};

EXTERN_GLOBAL(FpsCounter_54*, gFpsCounter_7055D4);
