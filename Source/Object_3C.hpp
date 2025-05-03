#pragma once

#include "Function.hpp"

class Sprite;
class Sprite_18;
class Object_2C;

class Object_3C
{
  public:
    EXPORT Object_3C();
    EXPORT ~Object_3C();
    EXPORT u32* sub_52ADF0(u32* a2);
    EXPORT Sprite_18* sub_5A6A50(s32 a2);
    EXPORT Object_2C* sub_5A6A90(s32 obj_type);
    EXPORT Sprite_18* sub_5A6AD0();
    EXPORT void sub_5A6B10(Sprite* a2);
    EXPORT void sub_5A6B60(Sprite* a2);
    EXPORT void sub_5A6BB0();
    EXPORT void sub_5A6BD0();
    EXPORT void sub_5A6BF0(Sprite* pSprite);
    EXPORT char_type sub_5A6C10(Sprite* a2);
    EXPORT void sub_5A6C40(s32 a2);
    EXPORT Sprite* FirstSpriteOfType_5A6CA0(s32 sprite_type);
    EXPORT void sub_5A6CD0(Sprite* pSprite);
    EXPORT void sub_5A6D00(Sprite* pSprite1, s32 a3, Sprite* pSprite2, s16 a5);
    EXPORT void PushSprite_5A6D40(Sprite* pToFind);
    EXPORT char_type SpriteExists_5A6D80(Sprite* pToFind);
    EXPORT Sprite* sub_5A6DA0();
    EXPORT Sprite* sub_5A6DC0();
    EXPORT Sprite_18* sub_5A6E10();
    EXPORT Sprite* sub_5A6E40(s32 a2, s32 a3);
    EXPORT s32 sub_5A6EA0(s32 a2, s32 a3);
    EXPORT void sub_5A6F70(Sprite* a2);
    EXPORT void sub_5A7010();
    EXPORT void sub_5A7080();
    EXPORT void sub_5A7110();
    EXPORT s32 sub_5A71A0();
    EXPORT void sub_5A71F0();
    EXPORT void sub_5A7240(Sprite* a2);
    EXPORT void sub_5A72B0(Sprite* pSprite, char_type bUnknown);
    EXPORT char_type sub_5A7310();

    Sprite_18* field_0;
    s16 field_4;
    s16 field_6;
    Object_3C* field_8;
    s32 field_C;
    s32 field_10;
    s32 field_14;
    s32 field_18;
    s32 field_1C;
    s32 field_20;
    s32 field_24;
    s16 field_28;
    s16 field_2A;
    s16 field_2C;
    char_type field_2E;
    char_type field_2F;
    char_type field_30;
    char_type field_31;
    s16 field_32;
    s32 field_34;
    char_type field_38;
    char_type field_39;
    char_type field_3A;
    char_type field_3B;
};
