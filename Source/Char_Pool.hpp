#pragma once

#include "Function.hpp"
#include "Ped.hpp"
#include "Pool.hpp"
#include "char.hpp"

class Camera_0xBC;

class Char_B4_Pool
{
  public:
    __forceinline Char_B4_Pool()
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
    // 9.6f 0x420EA0
    inline bool no_char_ped_420EA0()
    {
        return field_0_char_ped == NULL;
    }

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
    PoolBasic<Char_8, 100> field_0_pool;

    ~Char_8_Pool()
    {
        field_0_pool.field_0_pHead = 0;
    }

    // 9.6f 0x445EF0
    inline Char_8* Allocate_445EF0()
    {
        return field_0_pool.Allocate();
    }

    // 9.6f 0x445F00
    inline void DeAllocate_445F00(Char_8* pItem)
    {
        field_0_pool.DeAllocate(pItem);
    }

    __forceinline Char_8_Pool()
    {
    }
};

class PedManager
{
  public:
    EXPORT void SpawnDummies_46EB60(Camera_0xBC* pCam);
    EXPORT void PedsService_4703F0();
    EXPORT PedManager();
    EXPORT ~PedManager();
    EXPORT Ped* SpawnPedAt(Fix16 xpos, Fix16 ypos, Fix16 zpos, u8 remap, Ang16 rotation);
    EXPORT Ped* SpawnDriver_470B00(Car_BC* pCar);
    EXPORT Ped* SpawnGangDriver_470BA0(Car_BC* pCar, Gang_144* pGang);
    EXPORT Ped* CreateDummyDriver_470CC0(Car_BC* pCar);
    EXPORT Ped* SpawnRunAwayGuy_470D60();
    EXPORT Ped* SpawnTrainLeaver_470E30();
    EXPORT Ped* AllocatePed_470F30();
    EXPORT Ped* ClonePed_470F90(Ped* pSrc);
    EXPORT void DoIanTest_471060(u16 a1);
    EXPORT Ped* PedById(s32 pedId);

    EXPORT void Dummies_470330();

    s16 field_0_max_dummy_chars;
    char_type field_2_num_dummy_chars;
    char_type field_3_num_peds_updated;
    char_type field_4_num_script_created_peds;
    u8 field_5_fbi_army_count;
    char_type field_6_num_peds_on_screen;
    char_type field_7_make_all_muggers;
    Sprite* field_8;
};

class PedPool
{
  public:
    // __forceinline on the pool ctors: PedManager::PedManager (0x470650) runs out of inline
    // expansions on the Fix16 argument conversions but still expands these
    __forceinline PedPool()
    {
    }

    EXPORT ~PedPool();

    // 9.6f 0x403890
    Ped* Allocate()
    {
        return field_0_pool.Allocate();
    }

    // 9.6f 0x435530
    inline Ped* GetFirstPed_435530()
    {
        return field_0_pool.field_4_pPrev;
    }

    Pool<Ped, 200> field_0_pool;
};

EXTERN_GLOBAL(PedManager*, gPedManager_6787BC);

EXTERN_GLOBAL(PedPool*, gPedPool_6787B8);

EXTERN_GLOBAL(Char_B4_Pool*, gChar_B4_Pool_6FDB44);

EXTERN_GLOBAL(Char_8_Pool*, gChar_8_Pool_678b50);

EXTERN_GLOBAL(u8, gNumPolicePedsInRangeScreen_6787EE);

EXTERN_GLOBAL(u8, byte_61A8A2);
