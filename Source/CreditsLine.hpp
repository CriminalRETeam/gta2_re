#pragma once

#include "Function.hpp"
#include <cwchar>

class CreditsLine
{
  public:
    EXPORT CreditsLine(); // 483E30
    EXPORT ~CreditsLine(); // 483E60

    bool mbLoaded; // 0x0
    s16 mUnused2; // 0x2: set to 300 by LoadCredits, never read
    u16 mYGap; // 0x4: extra space above this line
    u16 mStringCategory; // 0x6: credits_line_category (credits_line_category.hpp)
    wchar_t mText[50]; // 0x8
};
GTA2_ASSERT_SIZEOF_ALWAYS(CreditsLine, 0x6C)