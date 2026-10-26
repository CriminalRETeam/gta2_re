#include "WeaponManager.hpp"
#include "Car.hpp"
#include "Globals.hpp"
#include "Ped.hpp"
#include "Player.hpp"
#include "debug.hpp"
#include "error.hpp"
#include "root_sound.hpp"

DEFINE_GLOBAL(WeaponPool*, gWeapon_30_Pool_707014, 0x707014);
DEFINE_GLOBAL(WeaponManager*, gWeapon_8_707018, 0x707018);
// Read only in Weapon.cpp, defined here: see the comment there.
DEFINE_GLOBAL_INIT(Ang16, kAng12_706D5E, Ang16(48), 0x706D5E);

DEFINE_GLOBAL_ARRAY_INIT(u8, max_ammo_capacity_5FF75C, 28, 0x5FF75C, 99u COMMA 99u COMMA 99u COMMA 99u COMMA 99u COMMA 99u COMMA 99u COMMA 99u COMMA 99u COMMA 99u COMMA
                                                                      99u COMMA 99u COMMA 99u COMMA 99u COMMA 99u COMMA 1u COMMA  99u COMMA 99u COMMA 99u COMMA 99u COMMA
                                                                      99u COMMA 99u COMMA 99u COMMA 99u COMMA 99u COMMA 99u COMMA 99u COMMA 99u);

                                                                      

u8 byte_5FF778[40] = {10u, 10u, 5u,  20u, 5u, 5u, 10u, 20u, 20u, 10u, 10u, 0u, 0u,  0u,  0u,  1u,  5u,   5u,  10u, 50u,
                      50u, 20u, 50u, 1u,  0u, 0u, 0u,  0u,  0u,  0u,  0u,  0u, 80u, 69u, 82u, 84u, 251u, 33u, 9u,  64u};

MATCH_FUNC(0x5e3c10)
Weapon* WeaponManager::allocate_5E3C10(s32 weapon_kind, Ped* pPed, u8 ammo)
{
    Weapon* pNewWeap = gWeapon_30_Pool_707014->AllocateUnlinked_4CC9E0();
    pNewWeap->init_5DCD90();
    field_4_ref_count++;
    pNewWeap->set_idx_4CCA00(weapon_kind);
    pNewWeap->set_pPed_4CCA10(pPed);
    pNewWeap->set_ammo_clips_5DCE20(ammo);
    return pNewWeap;
}

MATCH_FUNC(0x5e3cb0)
void WeaponManager::deallocate_5E3CB0(Weapon* pWeapon)
{
    WeaponPool* pRoot = gWeapon_30_Pool_707014;
    pWeapon->PoolDeallocate();
    pWeapon->mpNext = pRoot->field_0_pool.field_0_pStart;
    pRoot->field_0_pool.field_0_pStart = pWeapon;
    field_4_ref_count--;
}

MATCH_FUNC(0x5e3ce0)
Weapon* WeaponManager::allocate_5E3CE0(s32 weapon_kind, Car* pCar, u8 ammo)
{
    Weapon* pWeapon = gWeapon_30_Pool_707014->Allocate();
    pWeapon->init_5DCD90();
    pWeapon->set_idx_4CCA00(weapon_kind);
    pWeapon->set_car_4CCA20(pCar);
    pWeapon->set_ammo_clips_5DCE20(ammo);
    return pWeapon;
}

MATCH_FUNC(0x5e3d20)
Weapon* WeaponManager::find_5E3D20(Car* pCar, s32 weapon_kind)
{
    Weapon* result = gWeapon_30_Pool_707014->get_next_4CC9B0();
    if (!result)
    {
        return 0;
    }

    // TODO: Can probably invert this
    while (result->field_14_car != pCar || result->field_1C_idx != weapon_kind)
    {
        result = result->mpNext;
        if (!result)
        {
            return 0;
        }
    }
    return result;
}

MATCH_FUNC(0x5e3d50)
bool WeaponManager::allocate_5E3D50(s32 weapon_kind, u8 ammo, Car* pCar)
{
    bool bAddedAmmo;

    Weapon* pWeapon = find_5E3D20(pCar, weapon_kind);
    if (pWeapon)
    {
        bAddedAmmo = pWeapon->add_ammo_capped_5DCE40(ammo);
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
            pCar->field_54_driver->field_15C_player->SetWeapon_564910(pWeapon);
            return bAddedAmmo;
        }
    }

    if (bAddedAmmo)
    {
        if (pCar->field_54_driver)
        {
            pCar->field_54_driver->field_178_car_weapon = gWeapon_8_707018->allocate_5E3C10(weapon_kind, pCar->field_54_driver, 99u);
            pCar->field_54_driver->field_178_car_weapon->field_14_car = pCar;
        }
    }

    return bAddedAmmo;
}

MATCH_FUNC(0x5e3df0)
void WeaponManager::dealloc_car_weapon_5E3DF0(Car* pCar)
{
    Weapon* pIter = gWeapon_30_Pool_707014->get_next_4CC9B0();
    while (pIter)
    {
        if ( pIter->field_14_car == pCar )
        {
          Weapon* pOldIter = pIter;
          pIter = pIter->mpNext;
          gWeapon_30_Pool_707014->DeAllocate_4A4F20(pOldIter);
        }
        else
        {
          pIter = pIter->mpNext;
        }
    }
}

MATCH_FUNC(0x5e3e70)
u8 WeaponManager::get_max_ammo_capacity_5E3E70(s32 weapon_kind)
{
    return max_ammo_capacity_5FF75C[weapon_kind];
}

MATCH_FUNC(0x5e3e80)
u8 WeaponManager::get_defalt_ammo_5E3E80(s32 weapon_kind)
{
    return byte_5FF778[weapon_kind];
}

MATCH_FUNC(0x5e3e90)
WeaponManager::WeaponManager()
{
    if (!gWeapon_30_Pool_707014)
    {
        gWeapon_30_Pool_707014 = new WeaponPool();
        if (!gWeapon_30_Pool_707014)
        {
            FatalError_4A38C0(Gta2Error::OutOfMemoryNewOperator, "C:\\Splitting\\Gta2\\Source\\weapon.cpp", 2428);
        }
    }
    field_4_ref_count = 0;
    field_0_hit_sprites.ResetHead_4207E0();
}

MATCH_FUNC(0x5e3f60)
WeaponManager::~WeaponManager()
{
    if (gWeapon_30_Pool_707014)
    {
        GTA2_DELETE_AND_NULL(gWeapon_30_Pool_707014);
    }
}

MATCH_FUNC(0x5E4090)
WeaponPool::~WeaponPool()
{

}
