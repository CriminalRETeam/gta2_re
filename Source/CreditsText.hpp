#pragma once

#include "Function.hpp"
#include "CreditsLine.hpp"

#pragma pack(push)
#pragma pack(1)
class CreditsText
{
  public:
    EXPORT CreditsText(); // 483E70
    EXPORT ~CreditsText(); // 483EA0

    EXPORT wchar_t ReadNextNonBlankChar_483EC0(const wchar_t *pStr, u16 *pStartPos, bool bStopAtSpace);
    EXPORT void LoadCredits_483F20();

    s16 field_0_line_count;
    CreditsLine field_2_lines[600];
};
GTA2_ASSERT_SIZEOF_ALWAYS(CreditsText, 0xFD22);
#pragma pack(pop)