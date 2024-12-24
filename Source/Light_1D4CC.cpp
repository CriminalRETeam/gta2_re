#include "Light_1D4CC.hpp"

MATCH_FUNC(0x4bebc0)
Light_1D4CC::~Light_1D4CC()
{
    Light::sub_4D6E30();
    this->field_0 = 0;
    this->field_4 = 0;
}

MATCH_FUNC(0x5c2b70)
nostalgic_ellis_0x28* Light_1D4CC::sub_5C2B70()
{
    nostalgic_ellis_0x28* result = this->field_0;
    this->field_0 = this->field_0->field_1C;
    result->field_1C = 0;
    result->field_0 = 0;
    result->field_14_on_time = 0;
    return result;
}
