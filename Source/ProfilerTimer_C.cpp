#include "ProfilerTimer_C.hpp"
#include "Function.hpp"
#include <windows.h>

MATCH_FUNC(0x5BEBF0)
void ProfilerTimer::AccumulateElapsed_5BEBF0()
{
    field_8_accum_time += timeGetTime() - field_4_start_time;
}

MATCH_FUNC(0x5BEC10)
ProfilerTimer::ProfilerTimer()
{
    field_0_time_percent = 0;
    field_8_accum_time = 0;
    field_4_start_time = 0;
}

MATCH_FUNC(0x5BEC20)
ProfilerTimer::~ProfilerTimer()
{
}