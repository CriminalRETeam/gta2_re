#pragma once

#include "Function.hpp"
#include <cwchar>

class CreditsLine
{
  public:
    EXPORT CreditsLine(); // 483E30
    EXPORT ~CreditsLine(); // 483E60

    bool field_0_bLoaded; // 0x0
    s16 field_2_unused; // 0x2: set to 300 by LoadCredits_483F20, never read
    u16 field_4_y_gap; // 0x4: extra space above this line
    u16 field_6_string_category; // 0x6: credits_line_category (credits_line_category.hpp)
    wchar_t field_8_text[50]; // 0x8
};
GTA2_ASSERT_SIZEOF_ALWAYS(CreditsLine, 0x6C)