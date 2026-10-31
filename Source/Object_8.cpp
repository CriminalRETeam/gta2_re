#include "Object_8.hpp"

MATCH_FUNC(0x52ad50)
Object_8::Object_8()
{
    this->mpNext = NULL;
    this->mFrameIdx = 99;
    this->mFrameCounter = 99;
    this->mTimer = 99;
}

MATCH_FUNC(0x52ad70)
Object_8::~Object_8()
{
    mpNext = NULL;
}