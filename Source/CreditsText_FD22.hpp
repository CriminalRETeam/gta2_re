#pragma once

#include "Function.hpp"
#include "CreditsLine_6C.hpp"

#pragma pack(push)
#pragma pack(1)
class CreditsText_FD22
{
  public:
    EXPORT CreditsText_FD22(); // 483E70
    EXPORT ~CreditsText_FD22(); // 483EA0

    EXPORT wchar_t ReadNextNonBlankChar_483EC0(const wchar_t *pStr, u16 *pStartPos, bool bStopAtSpace);
    EXPORT void LoadCredits_483F20();

    s16 field_0_line_count;
    CreditsLine_6C field_2_lines[600];
};
#pragma pack(pop)