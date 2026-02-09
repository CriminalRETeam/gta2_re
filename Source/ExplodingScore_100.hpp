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
    EXPORT void sub_596A70(s32 a2);
    EXPORT void sub_596A90(Fix16 xpos, Fix16 ypos, Fix16 zpos, u32 score);
    EXPORT s32 sub_596C90();
    EXPORT char_type PoolUpdate();
    EXPORT void sub_597100(s32 a2, s32 a3);

    s32 field_0;
    s32 field_4[9];
    Fix16 field_28;
    Fix16 field_2C;
    Fix16 field_30;
    s16 field_34;
    char_type field_36;
    char_type field_37;
    s32 field_38;
    s32 field_3C;
    s32 field_40;
    // NOTE: Used by a template so the name maters
    ExplodingScore_50* mpNext;
    s16 field_48;
    s16 field_4A;
    s32 field_4C;
};

class ExplodingScorePool
{
  public:
    EXPORT ExplodingScorePool();
    EXPORT ~ExplodingScorePool();
    EXPORT s16 sub_596860();
    EXPORT void sub_596880();
    EXPORT void sub_596890(Fix16 a2, Fix16 a3, Fix16 a4, u32 a5);
    EXPORT void sub_596940();
    EXPORT void DrawExplodingScores_5969E0();

    s16 field_0;
    u16 field_2;

    // NOTE: Nearly all other pools start at offset 0, this object is some 
    // suggestion its composition and not inheritance unless this object 
    // has an additional unknown base that been melted by the compiler :']
    Pool<ExplodingScore_50, 3> field_4_pool;
};

EXTERN_GLOBAL(ExplodingScorePool*, gExplodingScorePool);
