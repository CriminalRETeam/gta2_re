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
    bool is_car_weapon_433820(s32 weapon_kind) const
    {
        return weapon_kind >= 15;
    }

    EXPORT Weapon* allocate_5E3C10(s32 weapon_kind, Ped* pPed, u8 ammo);
    EXPORT void deallocate(Weapon* pWeapon);
    EXPORT Weapon* allocate_5E3CE0(s32 weapon_kind, Car* pCar, u8 ammo);
    EXPORT Weapon* find(Car* pCar, s32 weapon_kind);
    EXPORT bool allocate_5E3D50(s32 weapon_kind, u8 ammo, Car* pCar);
    EXPORT void dealloc_car_weapon(Car* pCar);
    EXPORT u8 get_max_ammo_capacity(s32 weapon_kind) const;
    EXPORT u8 get_defalt_ammo(s32 weapon_kind) const;
    EXPORT WeaponManager();
    EXPORT ~WeaponManager();
    SpriteList mHitSprites; // 0x0
    s16 mRefCount; // 0x4
    s16 mUnused6; // 0x6
};
GTA2_ASSERT_SIZEOF_ALWAYS(WeaponManager, 0x8);

class WeaponPool
{
  public:
    WeaponPool() // 4CDA20 inline
    {
    }

    // 9.6f 0x4CC9C0 (which also calls 0x4CC810, init in 10.5)
    Weapon* Allocate()
    {
        Weapon* pWeapon = mPool.mpStart;
        mPool.mpStart = pWeapon->mpNext;
        pWeapon->mpNext = mPool.mpPrev;
        mPool.mpPrev = pWeapon;
        return pWeapon;
    }

    // 9.6f 0x4CC9E0 (which also calls 0x4CC810, init in 10.5)
    inline Weapon* AllocateUnlinked_4CC9E0()
    {
        Weapon* pWeapon = mPool.mpStart;
        mPool.mpStart = pWeapon->mpNext;
        pWeapon->mpNext = NULL;
        return pWeapon;
    }

    // 9.6f 0x4A4F20
    void DeAllocate_4A4F20(Weapon* pWeapon)
    {
        Weapon* pIter = this->mPool.mpPrev;
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
                    this->mPool.mpPrev = pIter->mpNext;
                }
                pIter->mpNext = this->mPool.mpStart;
                this->mPool.mpStart = pIter;
                break;
            }
            else
            {
                pLastIter = pIter;
                pIter = pIter->mpNext;
            }
        }
    }

    // 9.6f 0x4CC9B0
    Weapon* get_next_4CC9B0() const
    {
        return this->mPool.mpPrev;
    }

    EXPORT ~WeaponPool();

    Pool<Weapon, 255> mPool; // 0x0
};

EXTERN_GLOBAL(WeaponPool*, gWeapon_30_Pool);

EXTERN_GLOBAL(WeaponManager*, gWeapon_8);

EXTERN_GLOBAL_ARRAY(u8, max_ammo_capacity, 28);
