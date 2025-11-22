#include "Weapon_8.hpp"
#include "Car_BC.hpp"
#include "Globals.hpp"
#include "Ped.hpp"
#include "Player.hpp"
#include "debug.hpp"
#include "error.hpp"
#include "root_sound.hpp"

DEFINE_GLOBAL(Weapon_30_Pool*, gWeapon_30_Pool_707014, 0x707014);
DEFINE_GLOBAL(Weapon_8*, gWeapon_8_707018, 0x707018);

DEFINE_GLOBAL_ARRAY_INIT(u8, max_ammo_capacity_5FF75C, 28, 0x5FF75C, 99u, 99u, 99u, 99u, 99u, 99u, 99u, 99u, 99u, 99u,
                                                                      99u, 99u, 99u, 99u, 99u, 1u,  99u, 99u, 99u, 99u,
                                                                      99u, 99u, 99u, 99u, 99u, 99u, 99u, 99u);

                                                                      

u8 byte_5FF778[40] = {10u, 10u, 5u,  20u, 5u, 5u, 10u, 20u, 20u, 10u, 10u, 0u, 0u,  0u,  0u,  1u,  5u,   5u,  10u, 50u,
                      50u, 20u, 50u, 1u,  0u, 0u, 0u,  0u,  0u,  0u,  0u,  0u, 80u, 69u, 82u, 84u, 251u, 33u, 9u,  64u};

MATCH_FUNC(0x5e3c10)
Weapon_30* Weapon_8::allocate_5E3C10(s32 weapon_kind, Ped* pPed, u8 ammo)
{
    Weapon_30* pNewWeap = gWeapon_30_Pool_707014->field_0_pool.field_0_pStart;
    gWeapon_30_Pool_707014->field_0_pool.field_0_pStart = gWeapon_30_Pool_707014->field_0_pool.field_0_pStart->mpNext;
    pNewWeap->mpNext = 0;
    pNewWeap->init_5DCD90();
    field_4_ref_count++;
    pNewWeap->field_1C_idx = weapon_kind;
    pNewWeap->field_24_pPed = pPed;
    pNewWeap->add_ammo_5DCE20(ammo);
    return pNewWeap;
}

MATCH_FUNC(0x5e3cb0)
void Weapon_8::deallocate_5E3CB0(Weapon_30* pWeapon)
{
    Weapon_30_Pool* pRoot = gWeapon_30_Pool_707014;
    pWeapon->clear_5DCDE0();
    pWeapon->mpNext = pRoot->field_0_pool.field_0_pStart;
    pRoot->field_0_pool.field_0_pStart = pWeapon;
    field_4_ref_count--;
}

MATCH_FUNC(0x5e3ce0)
Weapon_30* Weapon_8::allocate_5E3CE0(s32 weapon_kind, Car_BC* pCar, u8 ammo)
{
    Weapon_30* pWeapon = gWeapon_30_Pool_707014->Allocate();
    pWeapon->init_5DCD90();
    pWeapon->field_1C_idx = weapon_kind;
    pWeapon->field_14_car = pCar;
    pWeapon->add_ammo_5DCE20(ammo);
    return pWeapon;
}

MATCH_FUNC(0x5e3d20)
Weapon_30* Weapon_8::find_5E3D20(Car_BC* pCar, s32 weapon_kind)
{
    Weapon_30* result = gWeapon_30_Pool_707014->field_0_pool.field_4_pPrev;
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
char_type Weapon_8::allocate_5E3D50(s32 weapon_kind, u8 ammo, Car_BC* pCar)
{
    char_type bAddedAmmo;

    Weapon_30* pWeapon = find_5E3D20(pCar, weapon_kind);
    if (pWeapon)
    {
        bAddedAmmo = pWeapon->add_ammo_capped_5DCE40(ammo);
    }
    else
    {
        pWeapon = allocate_5E3CE0(weapon_kind, pCar, ammo);
        bAddedAmmo = 1;
    }

    if (bAddedAmmo)
    {
        if (pCar->is_driven_by_player())
        {
            pCar->field_54_driver->field_15C_player->sub_564910(pWeapon);
            return bAddedAmmo;
        }
    }

    if (bAddedAmmo)
    {
        if (pCar->field_54_driver)
        {
            pCar->field_54_driver->field_178 = gWeapon_8_707018->allocate_5E3C10(weapon_kind, pCar->field_54_driver, 99u);
            pCar->field_54_driver->field_178->field_14_car = pCar;
        }
    }

    return bAddedAmmo;
}

MATCH_FUNC(0x5e3df0)
void Weapon_8::alloc_car_weapon_5E3DF0(Car_BC* pCar)
{
    Weapon_30* pIter = gWeapon_30_Pool_707014->get_next_4CC9B0();
    while (pIter)
    {
        if ( pIter->field_14_car == pCar )
        {
          Weapon_30* pOldIter = pIter;
          pIter = pIter->mpNext;
          gWeapon_30_Pool_707014->sub_4A4F20(pOldIter);
        }
        else
        {
          pIter = pIter->mpNext;
        }
    }
}

MATCH_FUNC(0x5e3e70)
char_type Weapon_8::get_max_ammo_capacity_5E3E70(s32 weapon_kind)
{
    return max_ammo_capacity_5FF75C[weapon_kind];
}

MATCH_FUNC(0x5e3e80)
char_type Weapon_8::get_ammo_5E3E80(s32 weapon_kind)
{
    return byte_5FF778[weapon_kind];
}

MATCH_FUNC(0x5e3e90)
Weapon_8::Weapon_8()
{
    if (!gWeapon_30_Pool_707014)
    {
        gWeapon_30_Pool_707014 = new Weapon_30_Pool();
        if (!gWeapon_30_Pool_707014)
        {
            FatalError_4A38C0(0x20, "weapon.cpp", 2428);
        }
    }
    field_4_ref_count = 0;
    field_0.sub_4207E0();
}

MATCH_FUNC(0x5e3f60)
Weapon_8::~Weapon_8()
{
    if (gWeapon_30_Pool_707014)
    {
        GTA2_DELETE_AND_NULL(gWeapon_30_Pool_707014);
    }
}

MATCH_FUNC(0x5E4090)
Weapon_30_Pool::~Weapon_30_Pool()
{

}
