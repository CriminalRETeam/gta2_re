#pragma once

#include "Function.hpp"

class Sprite_8
{
  public:
    EXPORT void sub_5A5860();
    EXPORT void sub_5A5870();
    EXPORT Sprite_8* ctor_5A58A0();
    EXPORT void dtor_5A5B50();
    s32 field_0;
    s16 field_4;
    s16 field_6;
};

class Sprite_18
{
  public:
    EXPORT ~Sprite_18();
    EXPORT void ctor_5A5C50();
    s32 field_0;
    Sprite_18* field_4;
    s32 field_8;
    s32 field_C;
    s16 field_10;
    s16 field_12;
    s32 field_14;
};

class Sprite_1C24
{
  public:
    EXPORT ~Sprite_1C24();
    Sprite_18* field_0;
    Sprite_18 field_4[300];
};

extern Sprite_8* gSprite_8_703820;
