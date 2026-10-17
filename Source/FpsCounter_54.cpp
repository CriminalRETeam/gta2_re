// Built with /GX- (see cmake/vc6.cmake) to match, the STL headers pulled in warn about that
#pragma warning(disable : 4530)

#include "FpsCounter_54.hpp"
#include "Function.hpp"
#include "Hud.hpp"
#include <windows.h>

DEFINE_GLOBAL(FpsCounter_54*, gFpsCounter_7055D4, 0x7055D4);

// TODO
EXTERN_GLOBAL_ARRAY(wchar_t, tmpBuff_67BD9C, 640);


MATCH_FUNC(0x5BEC30)
void FpsCounter_54::ShowFps_5BEC30()
{
    swprintf(tmpBuff_67BD9C, L"%d/%d fps", field_8_update_fps, field_14_draw_fps);
    gHud_2B00_706620->field_650_texts.DisplayText_5D1F50(tmpBuff_67BD9C, 0, 0, gDebugFont_706600, 1);
}

MATCH_FUNC(0x5BEC70)
FpsCounter_54::FpsCounter_54()
{
    field_8_update_fps = 0;
    field_14_draw_fps = 0;
    field_0_update_count = 0;
    field_C_draw_count = 0;
    field_4_update_start_time = timeGetTime();
    field_10_draw_start_time = timeGetTime();
}

MATCH_FUNC(0x5BECC0)
FpsCounter_54::~FpsCounter_54()
{

}

MATCH_FUNC(0x5BECF0)
void FpsCounter_54::UpdateFpsCounters_5BECF0(char_type bDrewFrame, char_type bUpdatedLogic)
{
    if (bUpdatedLogic)
    {
        field_0_update_count++;

        if (field_0_update_count == 100)
        {
            u32 elapsed_ms = timeGetTime() - field_4_update_start_time;
            field_8_update_fps = 100000 / elapsed_ms;
            field_18_execute_game_timer.field_0_time_percent = field_18_execute_game_timer.field_8_accum_time * 100 / elapsed_ms;
            field_24_timer_1.field_0_time_percent = field_24_timer_1.field_8_accum_time * 100 / elapsed_ms;
            field_30_timer_2.field_0_time_percent = field_30_timer_2.field_8_accum_time * 100 / elapsed_ms;
            field_3C_timer_3.field_0_time_percent = field_3C_timer_3.field_8_accum_time * 100 / elapsed_ms;
            field_48_timer_4.field_0_time_percent = field_48_timer_4.field_8_accum_time * 100 / elapsed_ms;
            field_4_update_start_time = timeGetTime();
            field_0_update_count = 0;
            field_18_execute_game_timer.field_8_accum_time = 0;
            field_24_timer_1.field_8_accum_time = 0;
            field_30_timer_2.field_8_accum_time = 0;
            field_3C_timer_3.field_8_accum_time = 0;
            field_48_timer_4.field_8_accum_time = 0;
        }
    }

    if (bDrewFrame)
    {
        field_C_draw_count++;
        if (field_C_draw_count == 100)
        {
            field_14_draw_fps = 100000 / (timeGetTime() - field_10_draw_start_time);
            field_10_draw_start_time = timeGetTime();
            field_C_draw_count = 0;
        }
    }
}