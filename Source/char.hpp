#pragma once

#include "BitSet32.hpp"
#include "Function.hpp"
#include "Object_3C.hpp"
#include "Ped.hpp"
#include "Pool.hpp"
#include "ang16.hpp"
#include "sprite.hpp"

class Sprite_3C;
class Ped;
class Car_BC;
class Sprite;

class Char_B4
{
  public:
    s32 field_0_id;
    s8 field_4;
    u8 field_5_remap;
    s8 field_6;
    s8 field_7;
    s32 field_8_ped_state_1;
    s32 field_C_ped_state_2;
    s32 field_10;
    Ang16 field_14;
    s8 field_16;
    s8 field_17;
    s32 field_18;
    s32 field_1C;
    s32 field_20;
    s32 field_24;
    Ang16 field_28;
    Ang16 field_2A;
    Ang16 field_2C_ang;
    s8 field_2E;
    s8 field_2F;
    s32 field_30;
    s16 field_34;
    s8 field_36;
    s8 field_37;
    Fix16 field_38_velocity;
    Fix16 field_3C_run_or_jump_speed;
    Ang16 field_40_rotation;
    Ang16 field_42;
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
    Ang16 field_74;
    s8 field_76;
    s8 field_77;
    Char_B4* mpNext;
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

    void set_rotation_433A30(Ang16 rotation)
    {
        field_40_rotation = rotation;
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

    EXPORT void PoolAllocate();
    EXPORT void PoolDeallocate();

    // Function chunk
    EXPORT void sub_545430();
    EXPORT bool sub_5451C0();

    EXPORT void sub_5454B0();
    EXPORT void sub_5454D0();
    EXPORT void sub_545530(Fix16 xpos, Fix16 ypos, Fix16 zpos);
    EXPORT s32 IsOnWater_545570();
    EXPORT void sub_5455F0();
    EXPORT void sub_545600();
    EXPORT void sub_5456A0();
    EXPORT s32 IsOnScreen_545700();
    EXPORT void sub_545720(Fix16 a2);
    EXPORT char_type sub_5459C0();
    EXPORT void DrownPed_5459E0();
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

    EXPORT bool sub_5532C0();
    EXPORT bool sub_553340(Sprite* pSprite);
    EXPORT char_type sub_5535B0(Object_2C* p2c);
    EXPORT bool sub_553640(Object_2C* p2c);
    EXPORT char_type sub_5537F0(Object_2C* p2c);
    EXPORT void sub_5538A0(Car_BC* pCar, s32 a3, s32 a4, s32 a5);
    EXPORT void sub_553E00(Ang16 ang, Fix16 a3, Fix16 a4, char_type a5);

    EXPORT void nullsub_28();
};

class Char_B4_Pool
{
  public:
    Char_B4_Pool()
    {
    }

    ~Char_B4_Pool()
    {
    }

    // inline 0x4355C0
    void DeAllocate(Char_B4* pB4)
    {
        field_0_pool.DeAllocate(pB4);
    }

    PoolBasic<Char_B4, 400> field_0_pool;
};

class Char_8
{
  public:
    void PoolAllocate()
    {
    }

    void PoolDeallocate()
    {
    }

    Ped* field_0_char_ped;
    Char_8* mpNext;
};

class Char_8_Pool
{
  public:
    PoolBasic<Char_8, 99> field_0_pool;

    s8 field_31c;
    s8 field_31d;
    s8 field_31e;
    s8 field_31f;
    s32 field_320_in_use;

    Char_8_Pool()
    {
        field_320_in_use = 0;
    }
};

class PedManager
{
  public:
    EXPORT void sub_46EB60(u32* a2);
    EXPORT void PedsService_4703F0();
    EXPORT PedManager();
    EXPORT ~PedManager();
    EXPORT Ped* SpawnPedAt(Fix16 xpos, Fix16 ypos, Fix16 zpos, u8 remap, Ang16 rotation);
    EXPORT Ped* SpawnDriver_470B00(Car_BC* pCar);
    EXPORT Ped* SpawnGangDriver_470BA0(Car_BC* pCar, Gang_144* pGang);
    EXPORT Ped* sub_470CC0(Car_BC* pCar);
    EXPORT Ped* sub_470D60();
    EXPORT Ped* sub_470E30();
    EXPORT Ped* sub_470F30();
    EXPORT Ped* sub_470F90(Ped* pSrc);
    EXPORT void DoIanTest_471060(s16 a1);
    EXPORT Ped* PedById(s32 pedId);

    EXPORT void Dummies_470330();

    s16 field_0;
    char_type field_2;
    char_type field_3;
    char_type field_4;
    u8 field_5_fbi_army_count;
    char_type field_6_num_peds_on_screen;
    char_type field_7_make_all_muggers;
    Sprite* field_8;
};

class PedPool
{
  public:
    EXPORT ~PedPool();

    Ped* Allocate()
    {
        return field_0_pool.Allocate();
    }

    Pool<Ped, 200> field_0_pool;
};

EXTERN_GLOBAL(PedManager*, gPedManager_6787BC);

EXTERN_GLOBAL(PedPool*, gPedPool_6787B8);

EXTERN_GLOBAL(Char_B4_Pool*, gChar_B4_Pool_6FDB44);

EXTERN_GLOBAL(Char_8_Pool*, gChar_8_Pool_678b50);

EXPORT void __stdcall sub_544F70();
