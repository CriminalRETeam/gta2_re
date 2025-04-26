#pragma once

#include "Function.hpp"

class Ped;

class Hamburger_40
{
  public:
    EXPORT void sub_4747B0();
    EXPORT Hamburger_40();
    EXPORT ~Hamburger_40();

    char_type field_0; // bInUse ?
    char_type field_1;
    char_type field_2;
    char_type field_3;
    Ped* field_4; // owner/thing going to the target
    s32 field_8;
    s32 field_C;
    char_type field_10;
    char_type field_11;
    char_type field_12;
    char_type field_13;
    s32 field_14_target_x;
    s32 field_18_target_y;
    s32 field_1C_target_z;
    char_type field_20;
    char_type field_21;
    char_type field_22;
    char_type field_23;
    char_type field_24;
    char_type field_25;
    char_type field_26;
    char_type field_27;
    char_type field_28;
    char_type field_29;
    s16 field_2A;
    u16 field_2C;
    s16 field_2E;
    Ped* field_30;
    s16 field_34;
    char_type field_36;
    char_type field_37;
    s32 field_38;
    char_type field_3C;
    char_type field_3D;
    char_type field_3E;
    char_type field_3F;
};

class Hamburger_500
{
  public:
    EXPORT Hamburger_40* sub_474810();
    EXPORT char_type sub_474850(Ped* a1, Ped* a2);
    EXPORT Ped* sub_4748A0(s32 a2, Ped* a3);
    EXPORT char_type sub_474920(Ped* a2, Ped* a3);
    EXPORT char_type sub_474970(Ped* a2);
    EXPORT char_type sub_4749B0(Ped* a1);
    EXPORT char_type sub_474A20(Ped* a1);
    EXPORT char_type sub_474A80(Ped* a1);
    EXPORT char_type sub_474AF0(Ped* a1);
    EXPORT char_type sub_474B50(Ped* a1);
    EXPORT char_type sub_474BC0(Ped* a1);
    EXPORT char_type sub_474C30(Ped* a1);
    EXPORT void sub_474CC0(Hamburger_40* a2);
    EXPORT Hamburger_500();
    EXPORT ~Hamburger_500();

    Hamburger_40 field_0[20];
};