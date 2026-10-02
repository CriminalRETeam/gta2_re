#pragma once

#include "Function.hpp"
#include "sleepy_stonebraker_0x6C.hpp"

#pragma pack(push)
#pragma pack(1)
class xenodochial_morse
{
  public:
    EXPORT xenodochial_morse(); // 483E70
    EXPORT ~xenodochial_morse(); // 483EA0

    EXPORT wchar_t ReadNextNonBlankChar_483EC0(const wchar_t *pStr, u16 *pStartPos, bool bStopAtSpace);
    EXPORT void LoadCredits_483F20();

    s16 field_0_line_count;
    sleepy_stonebraker_0x6C field_2_lines[600];
};
#pragma pack(pop)