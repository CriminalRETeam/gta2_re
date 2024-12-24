#include "Frismo_25C.hpp"

MATCH_FUNC(0x4bea80)
Frismo_C::Frismo_C()
{
    this->field_8_next = 0;
    this->field_0 = 125;
    this->field_4 = 205;
}

STUB_FUNC(0x4beaa0)
Frismo_C::~Frismo_C()
{
}

MATCH_FUNC(0x503110)
void Frismo_C::sub_503110()
{
    field_0 = 0;
    field_4 = 0;
    field_8_next = 0;
}

MATCH_FUNC(0x4bc300)
Frismo_25C::~Frismo_25C()
{
    field_0 = 0;
}
