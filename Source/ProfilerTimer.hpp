#pragma once

#include "Function.hpp"

class ProfilerTimer
{
  public:
    EXPORT void AccumulateElapsed_5BEBF0();

    EXPORT ProfilerTimer();
    EXPORT ~ProfilerTimer();

    s32 field_0_time_percent;
    s32 field_4_start_time;
    s32 field_8_accum_time;
};
GTA2_ASSERT_SIZEOF_ALWAYS(ProfilerTimer, 0xC);