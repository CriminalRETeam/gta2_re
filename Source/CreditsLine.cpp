#include "CreditsLine.hpp"
#include <string.h>

MATCH_FUNC(0x483E30)
CreditsLine::CreditsLine()
{
    mbLoaded = false;
    mUnused2 = 0;
    mYGap = 0;
    mStringCategory = 0;
    memset(mText, 0, sizeof(mText));
}

MATCH_FUNC(0x483E60)
CreditsLine::~CreditsLine()
{
}