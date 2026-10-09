#include "ProfilerTimer_C.hpp"
#include "Function.hpp"
#include <windows.h>

MATCH_FUNC(0x5BEBF0)
void ProfilerTimer_C::AccumulateElapsed_5BEBF0()
{
    field_8_accum_time += timeGetTime() - field_4_start_time;
}

MATCH_FUNC(0x5BEC10)
ProfilerTimer_C::ProfilerTimer_C()
{
    field_0_time_percent = 0;
    field_8_accum_time = 0;
    field_4_start_time = 0;
}

MATCH_FUNC(0x5BEC20)
ProfilerTimer_C::~ProfilerTimer_C()
{
}