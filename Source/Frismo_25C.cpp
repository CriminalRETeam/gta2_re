#include "Frismo_25C.hpp"

MATCH_FUNC(0x4bea80)
Frismo_C::Frismo_C()
{
    mpNext = 0;
    field_0 = 125;
    field_4 = 205;
}

MATCH_FUNC(0x4beaa0)
Frismo_C::~Frismo_C()
{
    mpNext = 0;
}

MATCH_FUNC(0x503110)
void Frismo_C::sub_503110()
{
    field_0 = 0;
    field_4 = 0;
    mpNext = 0;
}

MATCH_FUNC(0x4bc300)
Frismo_C_Pool::~Frismo_C_Pool()
{
    field_0 = 0;
}