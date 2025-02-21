#pragma once

#include "Function.hpp"
#include "Ped.hpp"

class Miss2_C
{
  public:
    s32 field_0_uni1;
    s8 field_4_type; // determines what the union types are
    s8 field_5;
    s8 field_6;
    s8 field_7;
    s32 field_8_uni2;
};

class Miss2_25C
{
  public:
    Miss2_25C();
    EXPORT s32 sub_502DC0();
    EXPORT Miss2_C* allocate_next_502F60();
    EXPORT void sub_502F80(s32 a2);
    EXPORT char_type sub_502FB0(Ped* a2);
    EXPORT void sub_502FF0(s32 a2, char_type a3);
    EXPORT void sub_503050(s32 a2);

    Miss2_C field_0[50];

    char_type field_258_count;
    char_type field_259;
    char_type field_25A;
    char_type field_25B;
};

EXPORT_VAR extern Miss2_25C* gMiss2_25C_6F805C;
