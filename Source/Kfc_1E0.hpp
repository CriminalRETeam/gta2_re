#pragma once

#include "Function.hpp"
#include "fix16.hpp"

class Ped;
class PedGroup;
class Car_BC;

class Kfc_30
{
  public:
    EXPORT Kfc_30();
    EXPORT ~Kfc_30();
    EXPORT void sub_5CBC00();
    EXPORT void sub_5CBC30();
    EXPORT void sub_5CBC40(Ped* a2);
    EXPORT bool sub_5CBC60();
    EXPORT char_type sub_5CBC90();
    EXPORT void sub_5CBD50();
    EXPORT void sub_5CC1C0();
    EXPORT char_type sub_5CC480();
    Car_BC* field_0;
    Ped* field_4;
    PedGroup* field_8;
    Fix16 field_C_x;
    Fix16 field_10_y;
    Fix16 field_14_z;
    s16 field_18;
    s16 field_1A;
    s16 field_1C;
    char_type field_1E;
    char_type field_1F;
    s32 field_20;
    s32 field_24;
    s32 field_28;
    char_type field_2C;
    char_type field_2D;
    char_type field_2E;
    char_type field_2F;
};

class Kfc_1E0
{
  public:
    Kfc_1E0()
    {
        init_5CBB70();
    }
    EXPORT ~Kfc_1E0();
    EXPORT void init_5CBB70();
    EXPORT Kfc_30* sub_5CBB80();
    EXPORT void sub_5CBBD0();
    Kfc_30 field_0[10];
};
