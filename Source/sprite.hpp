#pragma once

#include "Car_BC.hpp"
#include "Function.hpp"

class Sprite_8
{
  public:
    EXPORT void sub_5A5860();
    EXPORT void sub_5A5870();
    EXPORT Sprite_8();
    EXPORT ~Sprite_8();
    s32 field_0;
    s16 field_4;
    s16 field_6;
};

class Sprite_18
{
  public:
    EXPORT ~Sprite_18();
    EXPORT Sprite_18();
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

class Sprite_4C
{
  public:
    EXPORT s32 sub_5A57A0();
    EXPORT void ctor_5A57B0();
    EXPORT void dtor_5A5840();
    s32 field_0;
    s32 field_4;
    s32 field_8;
    Car_8 field_C[4];
    Sprite_4C* field_2C_pNext;
    s32 field_30;
    s32 field_34;
    s32 field_38;
    s32 field_3C;
    s32 field_40;
    s32 field_44;
    char_type field_48;
    char_type field_49;
    char_type field_4A;
    char_type field_4B;
};

class Sprite_5D598
{
  public:
    EXPORT void dtor_5A5BE0();
    Sprite_4C* field_0_pFree;
    Sprite_4C field_4[5031];
};

EXPORT_VAR extern Sprite_8* gSprite_8_703820;
EXPORT_VAR extern Sprite_5D598* gSprite_5D598_70381C;
