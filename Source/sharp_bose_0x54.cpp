#include "sharp_bose_0x54.hpp"
#include "Function.hpp"
#include "Hud.hpp"
#include <windows.h>

DEFINE_GLOBAL(sharp_bose_0x54*, gsharp_bose_0x54_7055D4, 0x7055D4);

// TODO
EXTERN_GLOBAL_ARRAY(wchar_t, tmpBuff_67BD9C, 640);


MATCH_FUNC(0x5BEC30)
void sharp_bose_0x54::ShowFps_5BEC30()
{
    swprintf(tmpBuff_67BD9C, L"%d/%d fps", field_8_update_rate, field_14_draw_fps);
    gHud_2B00_706620->field_650_texts.DisplayText_5D1F50(tmpBuff_67BD9C, 0, 0, gDebugFont_706600, 1);
}

MATCH_FUNC(0x5BEC70)
sharp_bose_0x54::sharp_bose_0x54()
{
    field_8_update_rate = 0;
    field_14_draw_fps = 0;
    field_0_update_count = 0;
    field_C_draw_count = 0;
    field_4_update_start_time = timeGetTime();
    field_10_draw_start_time = timeGetTime();
}

MATCH_FUNC(0x5BECC0)
sharp_bose_0x54::~sharp_bose_0x54()
{

}

MATCH_FUNC(0x5BECF0)
void sharp_bose_0x54::UpdateFpsCounters_5BECF0(char_type a2, char_type a3)
{
    if (a3)
    {
        field_0_update_count++;

        if (field_0_update_count == 100)
        {
            u32 v5 = timeGetTime() - field_4_update_start_time;
            field_8_update_rate = 100000 / v5;
            field_18.field_0_time_percent = 100 * field_18.field_8_accum_time / v5;
            field_24.field_0_time_percent = 100 * field_24.field_8_accum_time / v5;
            field_30.field_0_time_percent = 100 * field_30.field_8_accum_time / v5;
            field_3C.field_0_time_percent = 100 * field_3C.field_8_accum_time / v5;
            field_48.field_0_time_percent = 100 * field_48.field_8_accum_time / v5;
            field_4_update_start_time = timeGetTime();
            field_0_update_count = 0;
            field_18.field_8_accum_time = 0;
            field_24.field_8_accum_time = 0;
            field_30.field_8_accum_time = 0;
            field_3C.field_8_accum_time = 0;
            field_48.field_8_accum_time = 0;
        }
    }

    if (a2)
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