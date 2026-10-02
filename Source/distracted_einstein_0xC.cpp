#include "distracted_einstein_0xC.hpp"
#include "Function.hpp"
#include <windows.h>

MATCH_FUNC(0x5BEBF0)
void distracted_einstein_0xC::AccumulateElapsed_5BEBF0()
{
    field_8_accum_time += timeGetTime() - field_4_start_time;
}

MATCH_FUNC(0x5BEC10)
distracted_einstein_0xC::distracted_einstein_0xC()
{
    field_0_time_percent = 0;
    field_8_accum_time = 0;
    field_4_start_time = 0;
}

MATCH_FUNC(0x5BEC20)
distracted_einstein_0xC::~distracted_einstein_0xC()
{
}