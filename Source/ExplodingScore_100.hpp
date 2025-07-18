#pragma once

#include "Fix16.hpp"
#include "Function.hpp"

class ExplodingScore_50
{
  public:
    EXPORT ExplodingScore_50();
    EXPORT ~ExplodingScore_50();
    EXPORT void Empty_596A50();
    EXPORT void Empty_596A60();
    EXPORT void sub_596A70(s32 a2);
    EXPORT void sub_596A90(s32 xpos, s32 ypos, s32 zpos, u32 score);
    EXPORT s32 sub_596C90();
    EXPORT char_type sub_5970C0();
    EXPORT void sub_597100(s32 a2, s32 a3);

    s32 field_0;
    s32 field_4[9];
    s32 field_28;
    s32 field_2C;
    s32 field_30;
    s16 field_34;
    char_type field_36;
    char_type field_37;
    s32 field_38;
    s32 field_3C;
    s32 field_40;
    ExplodingScore_50* field_44;
    s16 field_48;
    s16 field_4A;
    s32 field_4C;
};

class ExplodingScore_100
{
  public:
    EXPORT ExplodingScore_100();
    EXPORT ~ExplodingScore_100();
    EXPORT s16 sub_596860();
    EXPORT void sub_596880();
    EXPORT void sub_596890(Fix16 a2, Fix16 a3, Fix16 a4, u32 a5);
    EXPORT void sub_596940();
    EXPORT void DrawExploding_5969E0();

    s16 field_0;
    s16 field_2;
    ExplodingScore_50* field_4;
    ExplodingScore_50* field_8;
    ExplodingScore_50 field_C[3];
    s16 field_FC_count;
    s16 field_FE;
};

EXTERN_GLOBAL(ExplodingScore_100*, gExplodingScore_100_702F34);
