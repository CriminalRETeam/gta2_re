#pragma once

#include "Function.hpp"

class ProfilerTimer
{
  public:
    EXPORT void AccumulateElapsed_5BEBF0();

    EXPORT ProfilerTimer();
    EXPORT ~ProfilerTimer();

    s32 mTimePercent; // 0x0
    s32 mStartTime; // 0x4
    s32 mAccumTime; // 0x8
};
GTA2_ASSERT_SIZEOF_ALWAYS(ProfilerTimer, 0xC);