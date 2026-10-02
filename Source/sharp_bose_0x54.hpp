#pragma once

#include "Function.hpp"
#include "distracted_einstein_0xC.hpp"

class Garox_C4;

class sharp_bose_0x54
{
  public:
    EXPORT void ShowFps_5BEC30();
    EXPORT sharp_bose_0x54();
    EXPORT ~sharp_bose_0x54();
    EXPORT void UpdateFpsCounters_5BECF0(char_type a2, char_type a3);

    s32 field_0_update_count;
    s32 field_4_update_start_time;
    s32 field_8_update_rate;
    s32 field_C_draw_count;
    s32 field_10_draw_start_time;
    s32 field_14_draw_fps;
    distracted_einstein_0xC field_18;
    distracted_einstein_0xC field_24;
    distracted_einstein_0xC field_30;
    distracted_einstein_0xC field_3C;
    distracted_einstein_0xC field_48;
};

EXTERN_GLOBAL(sharp_bose_0x54*, gsharp_bose_0x54_7055D4);
