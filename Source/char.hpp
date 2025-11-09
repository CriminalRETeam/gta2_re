#pragma once

#include "Function.hpp"
#include "BitSet32.hpp"
#include "Ped.hpp"
#include "angle.hpp"
#include "ang16.hpp"
#include "Object_3C.hpp"
#include "sprite.hpp"

class Sprite_3C;
class Ped;
class Car_BC;
class Sprite;

class Char_B4
{
  public:
    s32 field_0;
    s8 field_4;
    u8 field_5_remap;
    s8 field_6;
    s8 field_7;
    s32 field_8_ped_state_1;
    s32 field_C_ped_state_2;
    s32 field_10;
    angle field_14;
    s8 field_16;
    s8 field_17;
    s32 field_18;
    s32 field_1C;
    s32 field_20;
    s32 field_24;
    angle field_28;
    angle field_2A;
    angle field_2C;
    s8 field_2E;
    s8 field_2F;
    s32 field_30;
    s16 field_34;
    s8 field_36;
    s8 field_37;
    Fix16 field_38;
    Fix16 field_3C;
    Ang16 field_40_rotation;
    angle field_42;
    s8 field_44;
    u8 field_45;
    s16 field_46;
    s8 field_48;
    s8 field_49;
    u16 field_4A;
    Fix16 field_4C;
    Fix16 field_50;
    s8 field_54;
    s8 field_55;
    s8 field_56;
    s8 field_57;
    CompilerBitField32 field_58_flags_bf;
    u8 field_5C;
    s8 field_5D;
    s8 field_5E;
    s8 field_5F;
    s32 field_60;
    s32 field_64;
    u8 field_68;
    s8 field_69;
    u8 field_6A;
    s8 field_6b;
    s32 field_6C;
    s8 field_70;
    s8 field_71;
    s8 field_72;
    s8 field_73;
    angle field_74;
    s8 field_76;
    s8 field_77;
    Char_B4* field_78_next;
    Ped* field_7C_pPed;
    Sprite* field_80_sprite_ptr; // TODO: Or sprite_3c, are they the same type ??
    Car_BC* field_84;
    struct_4 field_88_obj_2c;
    Fix16 field_8C;
    Fix16 field_90;
    Fix16 field_94;
    Fix16_Point_POD field_98;
    //Fix16 field_9C;
    s8 field_A0;
    s8 field_A1;
    s8 field_A2;
    s8 field_A3;
    Fix16 field_A4_xpos;
    Fix16 field_A8_ypos;
    Fix16 field_AC_zpos;
    s32 field_B0;

    inline Ang16 get_rotation_433A40()
    {
        return field_40_rotation;
    }

    inline Fix16 get_sprite_xpos()
    {
        return field_80_sprite_ptr->field_14_xpos.x;
    }

    inline Fix16 get_sprite_ypos()
    {
        return field_80_sprite_ptr->field_14_xpos.y;
    }

    inline Fix16 get_sprite_zpos()
    {
        return field_80_sprite_ptr->field_1C_zpos;
    }

    Char_B4();
    ~Char_B4();

    EXPORT void sub_5453D0();
    EXPORT void sub_5454B0();
    EXPORT void sub_5454D0();
    EXPORT void sub_545530(Fix16 xpos, Fix16 ypos, Fix16 zpos);
    EXPORT s32 sub_545570();
    EXPORT void sub_5455F0();
    EXPORT s16 sub_545600();
    EXPORT void sub_5456A0();
    EXPORT s32 sub_545700();
    EXPORT void sub_545720(Fix16 a2);
    EXPORT char_type sub_5459C0();
    EXPORT void sub_5459E0();
    EXPORT void sub_546360();
    EXPORT void sub_548590();
    EXPORT void sub_548670(char_type a2);
    EXPORT void sub_548840(Object_2C* a2);
    EXPORT void sub_548BD0(s32 a2);
    EXPORT void sub_54A530(Car_BC* a2, Car_BC* a3, s32 a4);
    EXPORT char_type sub_54B8F0();
    EXPORT s32 sub_54C090();
    EXPORT char_type sub_54C1A0(s32 a2);
    EXPORT char_type sub_54C3E0();
    EXPORT char_type sub_54C500(char_type a2, char_type a3);
    EXPORT void sub_54C580();
    EXPORT s16 sub_54C6C0(s32 a2);
    EXPORT s16 sub_54C900(s32 a2);
    EXPORT s16 sub_54CAE0();
    EXPORT void sub_54CC40();
    EXPORT void sub_54DD70();
    EXPORT void state_0_54DDF0();
    EXPORT char_type sub_54ECB0(s32 a2);
    EXPORT char_type sub_54EF60(char_type a2, char_type a3);
    EXPORT char_type sub_54FEC0(s32 a2);
    EXPORT char_type sub_550090(s32 a2, s32 a3);
    EXPORT void state_1_5504F0();
    EXPORT s16* sub_550F60(s16* a2, s16 a3);
    EXPORT char_type sub_551350(s16 a2);
    EXPORT char_type sub_551400();
    EXPORT char_type sub_5516F0();
    //EXPORT void state_1_5504F0();
    EXPORT void state_1_5519F0();
    EXPORT void state_3_551A00();
    EXPORT void state_4_551B30();
    EXPORT void state_5_551BB0();
    EXPORT void state_7_551CB0();
    EXPORT void state_8_5520A0();
    EXPORT void state_9_552E90();
    EXPORT void nullsub_28();
};

class Char_11944
{
  public:
    Char_B4* field_0_next;
    Char_B4 field_4_array[400];

    Char_11944();
    ~Char_11944();

    // inline 0x4355C0
    void sub_4355C0(Char_B4* pB4)
    {
        pB4->sub_5453D0();
        pB4->field_78_next = field_0_next;
        field_0_next = pB4;
    }

    EXPORT void sub_5453D0();
};

class Char_8
{
  public:
    Ped* field_0_char_ped;
    Char_8* field_4_pOwner;
};

class Char_324
{
  public:
    Char_8* field_0_next;
    Char_8 field_4_array[99];
    s8 field_31c;
    s8 field_31d;
    s8 field_31e;
    s8 field_31f;
    s32 field_320_in_use;

    Char_324();
};

class Char_C
{
  public:
    EXPORT void sub_46EB60(u32* a2);
    EXPORT void sub_4703F0();
    EXPORT Char_C();
    EXPORT ~Char_C();
    EXPORT Ped* SpawnPedAt(Fix16 xpos, Fix16 ypos, Fix16 zpos, u8 remap, Ang16 rotation);
    EXPORT Ped* sub_470B00(Car_BC* a2);
    EXPORT Ped* SpawnGangDriver_470BA0(Car_BC* pCar, Gang_144* pGang);
    EXPORT Ped* sub_470CC0(Car_BC* a2);
    EXPORT Ped* sub_470D60();
    EXPORT Ped* sub_470E30();
    EXPORT Ped* sub_470F30();
    EXPORT Ped* sub_470F90(Ped* pSrc);
    EXPORT void DoIanTest_471060(s16 a1);
    EXPORT Ped* PedById(s32 pedId);

    s16 field_0;
    char_type field_2;
    char_type field_3;
    char_type field_4;
    u8 field_5;
    char_type field_6_num_peds_on_screen;
    char_type field_7_make_all_muggers;
    Sprite* field_8;
};

class Char_203AC
{
  public:
    EXPORT ~Char_203AC();
    Ped* field_0;
    Ped* field_4;
    Ped field_8[200];
    s16 field_203A8;
    s16 field_203AA;
};

EXTERN_GLOBAL(Char_C*, gChar_C_6787BC);

EXTERN_GLOBAL(Char_203AC*, gChar_203AC_6787B8);

EXTERN_GLOBAL(Char_11944*, gChar_11944_6FDB44);

EXTERN_GLOBAL(Char_324*, gChar_324_678b50);

EXPORT void __stdcall sub_544F70();
