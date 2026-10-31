#pragma once

#include "Function.hpp"

class ProfilerTimer
{
  public:
    EXPORT void AccumulateElapsed_5BEBF0();

    EXPORT ProfilerTimer();
    EXPORT ~ProfilerTimer();

    s32 field_0_time_percent; // 0x0
    s32 field_4_start_time; // 0x4
    s32 field_8_accum_time; // 0x8
};
GTA2_ASSERT_SIZEOF_ALWAYS(ProfilerTimer, 0xC);