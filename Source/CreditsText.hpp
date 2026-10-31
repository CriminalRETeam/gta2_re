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

    EXPORT wchar_t ReadNextNonBlankChar(const wchar_t *pStr, u16 *pStartPos, bool bStopAtSpace) const;
    EXPORT void LoadCredits();

    s16 mLineCount; // 0x0
    CreditsLine mLines[600]; // 0x2
};
GTA2_ASSERT_SIZEOF_ALWAYS(CreditsText, 0xFD22);
#pragma pack(pop)