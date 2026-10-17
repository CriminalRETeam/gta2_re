#include "CreditsLine_6C.hpp"
#include <string.h>

MATCH_FUNC(0x483E30)
CreditsLine_6C::CreditsLine_6C() // 483E30
{
    field_0_bLoaded = false;
    field_2_unused = 0;
    field_4_y_gap = 0;
    field_6_string_category = 0;
    memset(field_8_text, 0, sizeof(field_8_text));
}

MATCH_FUNC(0x483E60)
CreditsLine_6C::~CreditsLine_6C() // 483E60
{
}