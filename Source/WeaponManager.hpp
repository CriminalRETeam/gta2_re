#pragma once

#include "Function.hpp"
#include "Object_3C.hpp"
#include "Pool.hpp"
#include "Weapon.hpp"

class Ped;
class Car;
class SoundObject;

class WeaponManager
{
  public:
    bool is_car_weapon_433820(s32 weapon_kind)
    {
        return weapon_kind >= 15;
    }

    EXPORT Weapon* allocate_5E3C10(s32 weapon_kind, Ped* pPed, u8 ammo);
    EXPORT void deallocate_5E3CB0(Weapon* pWeapon);
    EXPORT Weapon* allocate_5E3CE0(s32 weapon_kind, Car* pCar, u8 ammo);
    EXPORT Weapon* find_5E3D20(Car* pCar, s32 weapon_kind);
    EXPORT bool allocate_5E3D50(s32 weapon_kind, u8 ammo, Car* pCar);
    EXPORT void dealloc_car_weapon_5E3DF0(Car* pCar);
    EXPORT u8 get_max_ammo_capacity_5E3E70(s32 weapon_kind);
    EXPORT u8 get_defalt_ammo_5E3E80(s32 weapon_kind);
    EXPORT WeaponManager();
    EXPORT ~WeaponManager();
    SpriteList field_0_hit_sprites;
    s16 field_4_ref_count;
    s16 field_6_unused;
};
GTA2_ASSERT_SIZEOF_ALWAYS(WeaponManager, 0x8);

class WeaponPool
{
  public:
    WeaponPool() // 4CDA20 inline
    {
    }

    // 9.6f 0x4CC9C0 (which also calls 0x4CC810, init_5DCD90 in 10.5)
    Weapon* Allocate()
    {
        Weapon* pWeapon = field_0_pool.field_0_pStart;
        field_0_pool.field_0_pStart = pWeapon->mpNext;
        pWeapon->mpNext = field_0_pool.field_4_pPrev;
        field_0_pool.field_4_pPrev = pWeapon;
        return pWeapon;
    }

    // 9.6f 0x4CC9E0 (which also calls 0x4CC810, init_5DCD90 in 10.5)
    inline Weapon* AllocateUnlinked_4CC9E0()
    {
        Weapon* pWeapon = field_0_pool.field_0_pStart;
        field_0_pool.field_0_pStart = pWeapon->mpNext;
        pWeapon->mpNext = NULL;
        return pWeapon;
    }

    // TODO: This is probably a heavily inlined common iteration func
    // Remove/dealloc?
    void DeAllocate_4A4F20(Weapon* pWeapon)
    {
        Weapon* pIter = this->field_0_pool.field_4_pPrev;
        Weapon* pLastIter = NULL;
        while (pIter)
        {
            if (pIter == pWeapon)
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

    Weapon* get_next_4CC9B0()
    {
        return this->field_0_pool.field_4_pPrev;
    }

    EXPORT ~WeaponPool();

    Pool<Weapon, 255> field_0_pool;
};

EXTERN_GLOBAL(WeaponPool*, gWeapon_30_Pool_707014);

EXTERN_GLOBAL(WeaponManager*, gWeapon_8_707018);

EXTERN_GLOBAL_ARRAY(u8, max_ammo_capacity_5FF75C, 28);
