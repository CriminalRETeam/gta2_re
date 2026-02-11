#pragma once

#include "char.hpp"
#include "Function.hpp"
#include "Ped.hpp"
#include "Pool.hpp"

class Camera_0xBC;

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
    EXPORT void SpawnDummies_46EB60(Camera_0xBC *pCam);
    EXPORT void PedsService_4703F0();
    EXPORT PedManager();
    EXPORT ~PedManager();
    EXPORT Ped* SpawnPedAt(Fix16 xpos, Fix16 ypos, Fix16 zpos, u8 remap, Ang16 rotation);
    EXPORT Ped* SpawnDriver_470B00(Car_BC* pCar);
    EXPORT Ped* SpawnGangDriver_470BA0(Car_BC* pCar, Gang_144* pGang);
    EXPORT Ped* sub_470CC0(Car_BC* pCar);
    EXPORT Ped* SpawnRunAwayGuy_470D60();
    EXPORT Ped* SpawnTrainLeaver_470E30();
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

EXTERN_GLOBAL(u8, unk_6787EE);

EXTERN_GLOBAL(u8, byte_61A8A2);
