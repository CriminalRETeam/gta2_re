#pragma once

#include "Function.hpp"

class ProfilerTimer_C
{
  public:
    EXPORT void AccumulateElapsed_5BEBF0();

    EXPORT ProfilerTimer_C();
    EXPORT ~ProfilerTimer_C();

    s32 field_0_time_percent;
    s32 field_4_start_time;
    s32 field_8_accum_time;
};