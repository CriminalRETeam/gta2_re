#pragma once

#include "Function.hpp"
#include "ang16.hpp"
#include "fix16.hpp"
#include "Fix16_Point.hpp"

class Sprite;
class Sprite_18;
class Object_2C;

class struct_4
{
  public:
    struct_4() // 424620 inline
    {
        sub_4207E0();
    }

    void sub_4207E0() // inline
    {
        field_0_p18 = 0;
    }

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
    EXPORT void PushImpactEvent_5A6D00(Sprite* pSprite1, Fix16 a3, Fix16 pSprite2, Ang16 angle);
    EXPORT void PushSprite_5A6D40(Sprite* pToFind);
    EXPORT char_type SpriteExists_5A6D80(Sprite* pToFind);
    EXPORT Sprite* sub_5A6DA0();
    EXPORT Sprite* sub_5A6DC0();
    EXPORT Sprite_18* sub_5A6E10();
    EXPORT Sprite* sub_5A6E40(Fix16 xOff, Fix16 yOff);
    EXPORT s32 sub_5A6EA0(s32 a2, s32 a3);
    EXPORT void PoolUpdate_5A6F70(Sprite* a2);
    EXPORT void sub_5A7010();
    EXPORT void sub_5A7080();
    EXPORT void sub_5A7110();
    EXPORT s32 sub_5A71A0();
    EXPORT void sub_5A71F0();
    EXPORT void sub_5A7240(Sprite* a2);
    EXPORT void sub_5A72B0(Sprite* pSprite, char_type bUnknown);
    EXPORT char_type sub_5A7310();

  public:
    Sprite_18* field_0_p18;
};

class Object_3C
{
  public:
    EXPORT Object_3C(); // 0x52ad80
    EXPORT ~Object_3C(); // 0x52ade0

    void PoolAllocate()
    {
      // TODO: This probably should have some code - once we have all call sites 
      // check and move common stuff in here
    }

    // ?? not sure if this is O3C either :skull:
    EXPORT Fix16_Point GetRot_52ADF0();

    struct_4 field_0;
    Ang16 field_4;
    s16 field_6;
    Object_3C* mpNext;
    Fix16 field_C;
    Fix16 field_10;
    s32 field_14;
    Fix16 field_18;
    Fix16 field_1C;
    s32 field_20;
    s32 field_24;
    s16 field_28;
    s16 field_2A;
    s16 field_2C;
    char_type field_2E;
    char_type field_2F;
    u8 field_30_bSkipAnim;
    u8 field_31;
    u16 field_32;
    s32 field_34;
    char_type field_38;
    char_type field_39;
    char_type field_3A;
    char_type field_3B;
};

EXTERN_GLOBAL(struct_4, stru_67727C);
