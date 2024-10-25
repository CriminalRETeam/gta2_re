#include "Function.hpp"
#include "distracted_einstein_0xC.hpp"
#include <windows.h>

MATCH_FUNC(0x5BEBF0)
void distracted_einstein_0xC::sub_5BEBF0()
{
    field_8 += timeGetTime() - field_4;
}

MATCH_FUNC(0x5BEC10)
distracted_einstein_0xC::distracted_einstein_0xC()
{
    this->field_0 = 0;
    this->field_8 = 0;
    this->field_4 = 0;
}

MATCH_FUNC(0x5BEC20)
distracted_einstein_0xC::~distracted_einstein_0xC()
{

}
