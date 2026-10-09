#pragma once

#include "Function.hpp"
#include <cwchar>

class CreditsLine_6C
{
  public:
    EXPORT CreditsLine_6C(); // 483E30
    EXPORT ~CreditsLine_6C(); // 483E60

    bool field_0_bLoaded;
    s16 field_2_unused; // set to 300 by LoadCredits_483F20, never read
    u16 field_4_y_gap; // extra space above this line
    u16 field_6_string_category; // credits_line_category (credits_line_category.hpp)
    wchar_t field_8_text[50];
};
GTA2_ASSERT_SIZEOF_ALWAYS(CreditsLine_6C, 0x6C)