#include "WeaponManager.hpp"
#include "Car.hpp"
#include "Globals.hpp"
#include "Ped.hpp"
#include "Player.hpp"
#include "debug.hpp"
#include "error.hpp"
#include "root_sound.hpp"

DEFINE_GLOBAL(WeaponPool*, gWeapon_30_Pool, 0x707014);
DEFINE_GLOBAL(WeaponManager*, gWeapon_8, 0x707018);
// Read only in Weapon.cpp, defined here: see the comment there.
DEFINE_GLOBAL_INIT(Ang16, kAng12_706D5E, Ang16(48), 0x706D5E);

DEFINE_GLOBAL_ARRAY_INIT(u8, max_ammo_capacity, 28, 0x5FF75C, 99u COMMA 99u COMMA 99u COMMA 99u COMMA 99u COMMA 99u COMMA 99u COMMA 99u COMMA 99u COMMA 99u COMMA
                                                                      99u COMMA 99u COMMA 99u COMMA 99u COMMA 99u COMMA 1u COMMA  99u COMMA 99u COMMA 99u COMMA 99u COMMA
                                                                      99u COMMA 99u COMMA 99u COMMA 99u COMMA 99u COMMA 99u COMMA 99u COMMA 99u);

                                                                      

u8 default_ammo_5FF778[40] = {10u, 10u, 5u,  20u, 5u, 5u, 10u, 20u, 20u, 10u, 10u, 0u, 0u,  0u,  0u,  1u,  5u,   5u,  10u, 50u,
                      50u, 20u, 50u, 1u,  0u, 0u, 0u,  0u,  0u,  0u,  0u,  0u, 80u, 69u, 82u, 84u, 251u, 33u, 9u,  64u};

MATCH_FUNC(0x5e3c10)
Weapon* WeaponManager::allocate_5E3C10(s32 weapon_kind, Ped* pPed, u8 ammo)
{
    Weapon* pNewWeap = gWeapon_30_Pool->AllocateUnlinked_4CC9E0();
    pNewWeap->init();
    mRefCount++;
    pNewWeap->set_idx_4CCA00(weapon_kind);
    pNewWeap->set_pPed_4CCA10(pPed);
    pNewWeap->set_ammo_clips(ammo);
    return pNewWeap;
}

MATCH_FUNC(0x5e3cb0)
void WeaponManager::deallocate(Weapon* pWeapon)
{
    WeaponPool* pRoot = gWeapon_30_Pool;
    pWeapon->PoolDeallocate();
    pWeapon->mpNext = pRoot->mPool.mpStart;
    pRoot->mPool.mpStart = pWeapon;
    mRefCount--;
}

MATCH_FUNC(0x5e3ce0)
Weapon* WeaponManager::allocate_5E3CE0(s32 weapon_kind, Car* pCar, u8 ammo)
{
    Weapon* pWeapon = gWeapon_30_Pool->Allocate();
    pWeapon->init();
    pWeapon->set_idx_4CCA00(weapon_kind);
    pWeapon->set_car_4CCA20(pCar);
    pWeapon->set_ammo_clips(ammo);
    return pWeapon;
}

MATCH_FUNC(0x5e3d20)
Weapon* WeaponManager::find(Car* pCar, s32 weapon_kind)
{
    Weapon* result = gWeapon_30_Pool->get_next_4CC9B0();
    if (!result)
    {
        return NULL;
    }

    while (result->mCar != pCar || result->mWeaponType != weapon_kind)
    {
        result = result->mpNext;
        if (!result)
        {
            return NULL;
        }
    }
    return result;
}

MATCH_FUNC(0x5e3d50)
bool WeaponManager::allocate_5E3D50(s32 weapon_kind, u8 ammo, Car* pCar)
{
    bool bAddedAmmo;

    Weapon* pWeapon = find(pCar, weapon_kind);
    if (pWeapon)
    {
        bAddedAmmo = pWeapon->add_ammo_capped(ammo);
    }
    else
    {
        pWeapon = allocate_5E3CE0(weapon_kind, pCar, ammo);
        bAddedAmmo = true;
    }

    if (bAddedAmmo)
    {
        if (pCar->IsDrivenByPlayer_4118D0())
        {
            pCar->mDriver->mPlayer->SetWeapon(pWeapon);
            return bAddedAmmo;
        }
    }

    if (bAddedAmmo)
    {
        if (pCar->mDriver)
        {
            pCar->mDriver->mCarWeapon = gWeapon_8->allocate_5E3C10(weapon_kind, pCar->mDriver, 99u);
            pCar->mDriver->mCarWeapon->mCar = pCar;
        }
    }

    return bAddedAmmo;
}

MATCH_FUNC(0x5e3df0)
void WeaponManager::dealloc_car_weapon(Car* pCar)
{
    Weapon* pIter = gWeapon_30_Pool->get_next_4CC9B0();
    while (pIter)
    {
        if ( pIter->mCar == pCar )
        {
          Weapon* pOldIter = pIter;
          pIter = pIter->mpNext;
          gWeapon_30_Pool->DeAllocate_4A4F20(pOldIter);
        }
        else
        {
          pIter = pIter->mpNext;
        }
    }
}

MATCH_FUNC(0x5e3e70)
u8 WeaponManager::get_max_ammo_capacity(s32 weapon_kind) const
{
    return max_ammo_capacity[weapon_kind];
}

MATCH_FUNC(0x5e3e80)
u8 WeaponManager::get_defalt_ammo(s32 weapon_kind) const
{
    return default_ammo_5FF778[weapon_kind];
}

MATCH_FUNC(0x5e3e90)
WeaponManager::WeaponManager()
{
    if (!gWeapon_30_Pool)
    {
        gWeapon_30_Pool = new WeaponPool();
        if (!gWeapon_30_Pool)
        {
            FatalError_4A38C0(Gta2Error::OutOfMemoryNewOperator, "C:\\Splitting\\Gta2\\Source\\weapon.cpp", 2428);
        }
    }
    mRefCount = 0;
    mHitSprites.ResetHead_4207E0();
}

MATCH_FUNC(0x5e3f60)
WeaponManager::~WeaponManager()
{
    if (gWeapon_30_Pool)
    {
        GTA2_DELETE_AND_NULL(gWeapon_30_Pool);
    }
}

MATCH_FUNC(0x5E4090)
WeaponPool::~WeaponPool()
{

}
