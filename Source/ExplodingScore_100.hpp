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

    s32 field_0_highest_digit_idx;
    s32 field_4_digits[9]; // Number components
    Fix16 field_28_x;
    Fix16 field_2C_y;
    Fix16 field_30_z;
    s16 field_34_palette;
    char_type field_36_alpha; // draw figure param
    char_type field_37_unused;
    s32 field_38_explode_counter;
    s32 field_3C_x_offset_mult;
    s32 field_40_y_offset_mult;
    // Pool<> reaches the next item by this name
    ExplodingScore* mpNext;
    s16 field_48_unused; // set to 0, never read
    s16 field_4A_unused;
    s32 field_4C_score;
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

    s16 field_0_last_palette;
    u16 field_2_free_scores;
    Pool<ExplodingScore, 3> field_4_pool;
};

EXTERN_GLOBAL(ExplodingScorePool*, gExplodingScorePool);
