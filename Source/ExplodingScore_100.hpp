#pragma once

#include "Fix16.hpp"
#include "Function.hpp"
#include "Pool.hpp"

class ExplodingScore_50
{
  public:
    EXPORT ExplodingScore_50();
    EXPORT ~ExplodingScore_50();
    EXPORT void PoolAllocate();
    EXPORT void PoolDeallocate();
    EXPORT void ClearNumbersArrayFrom_596A70(s32 start_idx);
    EXPORT void InitScore_596A90(Fix16 xpos, Fix16 ypos, Fix16 zpos, u32 score);
    EXPORT void DrawNumbers_596C90();
    EXPORT char_type PoolUpdate();
    EXPORT void DrawSingleNumber_597100(s32 number_to_draw, s32 xpos_sub);

    s32 field_0_highest_digit_idx;
    s32 field_4_digits[9]; // Number components
    Fix16 field_28_x;
    Fix16 field_2C_y;
    Fix16 field_30_z;
    s16 field_34_palette;
    char_type field_36; // draw figure param
    char_type field_37_unused; // pad ?
    s32 field_38; // scale factor?
    s32 field_3C; // xoff ?
    s32 field_40; // yoff ?
    // NOTE: Used by a template so the name maters
    ExplodingScore_50* mpNext;
    s16 field_48_unused; // set to 0, never used ??
    s16 field_4A_unused; // pad?
    s32 field_4C_score;
};

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

    s16 field_0_last_palette;
    u16 field_2_free_scores;

    // NOTE: Nearly all other pools start at offset 0, this object is some 
    // suggestion its composition and not inheritance unless this object 
    // has an additional unknown base that been melted by the compiler :']
    Pool<ExplodingScore_50, 3> field_4_pool;
};

EXTERN_GLOBAL(ExplodingScorePool*, gExplodingScorePool);
