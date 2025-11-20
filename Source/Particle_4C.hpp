#pragma once

#include "Function.hpp"
#include "ang16.hpp"

class Sprite;

class Particle_4C
{
  public:
    EXPORT char_type sub_538060();
    EXPORT char_type sub_5384C0();
    EXPORT char_type sub_538A40();
    EXPORT char_type sub_538AC0();
    EXPORT char_type sub_539040();
    EXPORT char_type sub_539480();
    EXPORT char_type sub_539890();
    EXPORT char_type sub_53A180();
    EXPORT char_type sub_53A280();
    EXPORT char_type sub_53AB70();
    EXPORT char_type sub_53ABA0();
    EXPORT char_type sub_53AE60();
    EXPORT char_type sub_53B170();
    EXPORT char_type sub_53B1A0();
    EXPORT char_type sub_53B580();
    EXPORT char_type sub_53B670();
    EXPORT char_type sub_53B9F0();
    EXPORT char_type sub_53BAC0();
    EXPORT char_type Service_53D260();
    EXPORT char_type sub_53E2C0();
    EXPORT void sub_53E2E0();

    s32 field_0;
    s32 field_4;
    s32 field_8;
    s32 field_C;
    s32 field_10;
    s32 field_14;
    s32 field_18;
    s32 field_1C;
    s32 field_20;
    Ang16 field_24;
    s16 field_26;
    s32 field_28;
    s16 field_2C;
    s16 field_2E;
    Sprite* field_30;
    s32 field_34;
    s32 field_38;
    Particle_4C* field_3C;
    s32 field_40;
    char_type field_44;
    char_type field_45;
    char_type field_46;
    char_type field_47;
    char_type field_48;
    char_type field_49;
    char_type field_4A;
    char_type field_4B;
};