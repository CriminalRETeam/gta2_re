#pragma once

#include "Function.hpp"
#include "Object_3C.hpp"
#include "Pool.hpp"
#include "Weapon_30.hpp"

class Ped;
class Car_BC;
class SoundObject_10;

class Weapon_8
{
  public:
    bool is_car_weapon_433820(s32 weapon_kind)
    {
        return weapon_kind >= 15;
    }

    EXPORT Weapon_30* allocate_5E3C10(s32 weapon_kind, Ped* pPed, u8 ammo);
    EXPORT void deallocate_5E3CB0(Weapon_30* pWeapon);
    EXPORT Weapon_30* allocate_5E3CE0(s32 weapon_kind, Car_BC* pCar, u8 ammo);
    EXPORT Weapon_30* find_5E3D20(Car_BC* pCar, s32 weapon_kind);
    EXPORT char_type allocate_5E3D50(s32 weapon_kind, u8 ammo, Car_BC* pCar);
    EXPORT void dealloc_car_weapon_5E3DF0(Car_BC* pCar);
    EXPORT u8 get_max_ammo_capacity_5E3E70(s32 weapon_kind);
    EXPORT u8 get_defalt_ammo_5E3E80(s32 weapon_kind);
    EXPORT Weapon_8();
    EXPORT ~Weapon_8();
    SpriteList_4 field_0;
    s16 field_4_ref_count;
    s16 field_6_unused;
};

class Weapon_30_Pool
{
  public:
    Weapon_30_Pool() // 4CDA20 inline
    {
    }

    // 9.6f 0x4CC9C0 (which also calls 0x4CC810, init_5DCD90 in 10.5)
    Weapon_30* Allocate()
    {
        Weapon_30* pWeapon = field_0_pool.field_0_pStart;
        field_0_pool.field_0_pStart = pWeapon->mpNext;
        pWeapon->mpNext = field_0_pool.field_4_pPrev;
        field_0_pool.field_4_pPrev = pWeapon;
        return pWeapon;
    }

    // 9.6f 0x4CC9E0 (which also calls 0x4CC810, init_5DCD90 in 10.5)
    inline Weapon_30* AllocateUnlinked_4CC9E0()
    {
        Weapon_30* pWeapon = field_0_pool.field_0_pStart;
        field_0_pool.field_0_pStart = pWeapon->mpNext;
        pWeapon->mpNext = 0;
        return pWeapon;
    }

    // TODO: This is probably a heavily inlined common iteration func
    // Remove/dealloc?
    void DeAllocate_4A4F20(Weapon_30* pW30)
    {
        Weapon_30* pIter = this->field_0_pool.field_4_pPrev;
        Weapon_30* pLastIter = 0;
        while (pIter)
        {
            if (pIter == pW30)
            {
                pIter->PoolDeallocate();
                if (pLastIter)
                {
                    pLastIter->mpNext = pIter->mpNext;
                }
                else
                {
                    this->field_0_pool.field_4_pPrev = pIter->mpNext;
                }
                pIter->mpNext = this->field_0_pool.field_0_pStart;
                this->field_0_pool.field_0_pStart = pIter;
                break;
            }
            else
            {
                pLastIter = pIter;
                pIter = pIter->mpNext;
            }
        }
    }

    Weapon_30* get_next_4CC9B0()
    {
        return this->field_0_pool.field_4_pPrev;
    }

    EXPORT ~Weapon_30_Pool();

    Pool<Weapon_30, 255> field_0_pool;
};

EXTERN_GLOBAL(Weapon_30_Pool*, gWeapon_30_Pool_707014);

EXTERN_GLOBAL(Weapon_8*, gWeapon_8_707018);

EXTERN_GLOBAL_ARRAY(u8, max_ammo_capacity_5FF75C, 28);
