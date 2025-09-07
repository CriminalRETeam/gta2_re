#include "distracted_einstein_0xC.hpp"
#include "Function.hpp"
#include <windows.h>

// FUNCTION: 105 0x5BEBF0
void distracted_einstein_0xC::sub_5BEBF0()
{
    field_8 += timeGetTime() - field_4;
}

// FUNCTION: 105 0x5BEC10
distracted_einstein_0xC::distracted_einstein_0xC()
{
    field_0 = 0;
    field_8 = 0;
    field_4 = 0;
}

// FUNCTION: 105 0x5BEC20
distracted_einstein_0xC::~distracted_einstein_0xC()
{
}