#pragma once

#include "Car_BC.hpp"
#include "Function.hpp"
#include "fix16.hpp"

class Sprite_14
{
  public:
    s32 field_0;
    s16 field_4;
    char_type field_6;
    char_type field_7;
    s32 field_8;
    s32 field_C;
    u16 field_10;
    s16 field_12;
};

class Sprite_3CC
{
  public:
    EXPORT char_type* sub_48F600(u16* a2, u32* a3, u32* a4, u16* a5);
    EXPORT char_type* sub_48F690(u32* a2);
    EXPORT s16* sub_48F6E0(u16* a2);
    EXPORT s32* sub_48F710();
    EXPORT Sprite_3CC();
    EXPORT ~Sprite_3CC();

    Sprite_14 field_0[48];
    s32 field_3C0;
    s32 field_3C4;
    s32 field_3C8;
};

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
    EXPORT Sprite* sub_5A6CA0(s32 a2);

    Sprite* field_0;
    Sprite_18* field_4_next;
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
    EXPORT Sprite_4C();
    EXPORT ~Sprite_4C();
    Fix16 field_0_width;
    Fix16 field_4_height;
    Fix16 field_8;
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
    EXPORT ~Sprite_5D598();
    Sprite_4C* field_0_pFree;
    Sprite_4C field_4[5031];
};

class Sprite_49B28
{
  public:
    EXPORT ~Sprite_49B28();

    Sprite* field_0_first_free;
    Sprite field_4[5031];

  public:
    Sprite_49B28();
};
GTA2_ASSERT_SIZEOF_ALWAYS(Sprite_49B28, 0x49B28)

EXPORT_VAR extern Sprite_8* gSprite_8_703820;
EXPORT_VAR extern Sprite_49B28* gSprite_49B28_703818;
EXPORT_VAR extern Sprite_5D598* gSprite_5D598_70381C;
