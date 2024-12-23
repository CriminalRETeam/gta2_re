#include "Light_1D4CC.hpp"

MATCH_FUNC(0x4bebc0)
Light_1D4CC::~Light_1D4CC()
{
    Light::sub_4D6E30();
    this->field_0 = 0;
    this->field_4 = 0;
}

STUB_FUNC(0x5c2b70)
nostalgic_ellis_0x28* Light_1D4CC::sub_5C2B70()
{
    return 0;
}
