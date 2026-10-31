#pragma once

#include "Fix16.hpp"
#include "Function.hpp"
#include "Pool.hpp"

class ExplodingScore
{
  public:
    EXPORT ExplodingScore();
    EXPORT ~ExplodingScore();
    EXPORT void PoolAllocate();
    EXPORT void PoolDeallocate();
    EXPORT void ClearNumbersArrayFrom_596A70(s32 start_idx);
    EXPORT void InitScore_596A90(Fix16 xpos, Fix16 ypos, Fix16 zpos, u32 score);
    EXPORT void DrawNumbers_596C90();
    EXPORT bool PoolUpdate();
    EXPORT void DrawSingleNumber_597100(s32 number_to_draw, s32 xpos_sub);

    s32 mHighestDigitIdx; // 0x0
    s32 mDigits[9]; // 0x4: Number components
    Fix16 mX; // 0x28
    Fix16 mY; // 0x2C
    Fix16 mZ; // 0x30
    s16 mPalette; // 0x34
    char_type mAlpha; // 0x36: draw figure param
    char_type mUnused37; // 0x37
    s32 mExplodeCounter; // 0x38
    s32 mXOffsetMult; // 0x3C
    s32 mYOffsetMult; // 0x40
    // Pool<> reaches the next item by this name
    ExplodingScore* mpNext;
    s16 mUnused48; // 0x48: set to 0, never read
    s16 mUnused4A; // 0x4A
    s32 mScore; // 0x4C
};
GTA2_ASSERT_SIZEOF_ALWAYS(ExplodingScore, 0x50);

class ExplodingScorePool
{
  public:
    EXPORT ExplodingScorePool();
    EXPORT ~ExplodingScorePool();
    EXPORT s16 NextPalette_596860();
    EXPORT void IncrementFreeScores_596880();
    EXPORT void PushScore_596890(Fix16 xpos, Fix16 ypos, Fix16 zpos, u32 score);
    EXPORT void Update_596940();
    EXPORT void DrawExplodingScores_5969E0();

    s16 mLastPalette; // 0x0
    u16 mFreeScores; // 0x2
    Pool<ExplodingScore, 3> mPool; // 0x4
};

EXTERN_GLOBAL(ExplodingScorePool*, gExplodingScorePool);
