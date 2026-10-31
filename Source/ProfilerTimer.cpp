#include "ProfilerTimer.hpp"
#include "Function.hpp"
#include <windows.h>

MATCH_FUNC(0x5BEBF0)
void ProfilerTimer::AccumulateElapsed_5BEBF0()
{
    mAccumTime += timeGetTime() - mStartTime;
}

MATCH_FUNC(0x5BEC10)
ProfilerTimer::ProfilerTimer()
{
    mTimePercent = 0;
    mAccumTime = 0;
    mStartTime = 0;
}

MATCH_FUNC(0x5BEC20)
ProfilerTimer::~ProfilerTimer()
{
}