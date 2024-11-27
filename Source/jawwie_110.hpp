#pragma once

#include "Function.hpp"
#include "Ped_Unknown_4.hpp"

class cool_nash_0x294;
class Kfc_30;

class jawwie_8
{
  public:
    s32 field_0;
    s32 field_4;
};

class jawwie_20
{
  public:
    EXPORT jawwie_20();
    EXPORT ~jawwie_20();
    EXPORT void sub_4FA7D0();
    EXPORT void sub_4FA800(cool_nash_0x294* pPed);
    EXPORT char_type sub_4FA820();
    EXPORT char_type sub_4FA9D0();
    EXPORT u32* sub_4FAAC0();
    EXPORT void sub_4FB330();

    char_type field_0;
    char_type field_1;
    char_type field_2;
    char_type field_3;
    Kfc_30* field_4;
    cool_nash_0x294* field_8;
    cool_nash_0x294* field_C;
    Ped_Unknown_4 field_10;
    char_type field_14_count;
    char_type field_15;
    s16 field_16;
    s32 field_18;
    char_type field_1C;
    char_type field_1D;
    char_type field_1E;
    char_type field_1F;
};

class jawwie_110
{
  public:
    EXPORT ~jawwie_110();
    EXPORT jawwie_8* ctor_4FA310();
    EXPORT char_type sub_4FA330(cool_nash_0x294* a2);
    EXPORT char_type sub_4FA470(cool_nash_0x294* a2);
    EXPORT jawwie_20* sub_4FA4B0();
    EXPORT void sub_4FA500();
    EXPORT void sub_4FA790();

    char_type field_0;
    char_type field_1_f8_idx;
    char_type field_2;
    char_type field_3;
    Ped_Unknown_4 field_4;
    jawwie_8 field_8[25];
    jawwie_20 field_D0[2];
};
