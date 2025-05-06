#include "Weapon_8.hpp"
#include "Car_BC.hpp"
#include "Globals.hpp"
#include "Ped.hpp"
#include "Player.hpp"
#include "debug.hpp"
#include "error.hpp"
#include "root_sound.hpp"

EXPORT_VAR Weapon_2FDC* gWeapon_2FDC_707014;
GLOBAL(gWeapon_2FDC_707014, 0x707014);

EXPORT_VAR Weapon_8* gWeapon_8_707018;
GLOBAL(gWeapon_8_707018, 0x707018);

u8 byte_5FF778[40] = {10u, 10u, 5u,  20u, 5u, 5u, 10u, 20u, 20u, 10u, 10u, 0u, 0u,  0u,  0u,  1u,  5u,   5u,  10u, 50u,
    50u, 20u, 50u, 1u,  0u, 0u, 0u,  0u,  0u,  0u,  0u,  0u, 80u, 69u, 82u, 84u, 251u, 33u, 9u,  64u};

MATCH_FUNC(0x5e3c10)
Weapon_30* Weapon_8::allocate_5E3C10(s32 weapon_kind, Ped* pPed, u8 ammo)
{
    Weapon_30* pNewWeap = gWeapon_2FDC_707014->field_0;
    gWeapon_2FDC_707014->field_0 = gWeapon_2FDC_707014->field_0->field_18_pNext;
    pNewWeap->field_18_pNext = 0;
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
    Weapon_2FDC* pRoot = gWeapon_2FDC_707014;
    pWeapon->clear_5DCDE0();
    pWeapon->field_18_pNext = pRoot->field_0;
    pRoot->field_0 = pWeapon;
    field_4_ref_count--;
}

STUB_FUNC(0x5e3ce0)
Weapon_30* Weapon_8::allocate_5E3CE0(s32 weapon_kind, Car_BC* pCar, u8 ammo)
{
    Weapon_30* pWeapon; // esi
    Weapon_30* pNext; // edx

    pWeapon = gWeapon_2FDC_707014->field_0;
    pNext = gWeapon_2FDC_707014->field_4;
    gWeapon_2FDC_707014->field_0 = gWeapon_2FDC_707014->field_0->field_18_pNext;
    pWeapon->field_18_pNext = pNext;
    gWeapon_2FDC_707014->field_4 = pWeapon;
    pWeapon->init_5DCD90();
    pWeapon->field_14_car = pCar;
    pWeapon->field_1C_idx = weapon_kind;
    pWeapon->add_ammo_5DCE20(ammo);
    return pWeapon;
}

MATCH_FUNC(0x5e3d20)
Weapon_30* Weapon_8::find_5E3D20(Car_BC* pCar, s32 weapon_kind)
{
    Weapon_30* result = gWeapon_2FDC_707014->field_4;
    if (!result)
    {
        return 0;
    }

    // TODO: Can probably invert this
    while (result->field_14_car != pCar || result->field_1C_idx != weapon_kind)
    {
        result = result->field_18_pNext;
        if (!result)
        {
            return 0;
        }
    }
    return result;
}

STUB_FUNC(0x5e3d50)
char_type Weapon_8::allocate_5E3D50(s32 weapon_kind, u8 ammo, Car_BC* pCar)
{
    char bAddedAmmo;

    Weapon_30* pWeapon = find_5E3D20(pCar, weapon_kind);
    if (pWeapon)
    {
        bAddedAmmo = pWeapon->add_ammo_capped_5DCE40(ammo);
        if (!bAddedAmmo)
        {
            return bAddedAmmo;
        }
    }
    else
    {
        pWeapon = allocate_5E3CE0(weapon_kind, pCar, ammo);
        bAddedAmmo = 1;
    }

    Ped* pDriver = pCar->field_54_driver;

    if (pDriver)
    {
        Player* pPlayer = pDriver->field_15C_player;
        if (pPlayer)
        {
            pPlayer->sub_564910(pWeapon);
            return bAddedAmmo;
        }
    }

    if (pDriver)
    {
        pCar->field_54_driver->field_178 = gWeapon_8_707018->allocate_5E3C10(weapon_kind, pDriver, 99u);
        pCar->field_54_driver->field_178->field_14_car = pCar;
    }

    return bAddedAmmo;
}

STUB_FUNC(0x5e3df0)
void Weapon_8::alloc_car_weapon_5E3DF0(Car_BC* a1)
{
}

STUB_FUNC(0x5e3e70)
char_type Weapon_8::get_max_ammo_capacity_5E3E70(s32 a2)
{
    return 0;
}


MATCH_FUNC(0x5e3e80)
char_type Weapon_8::get_ammo_5E3E80(s32 weapon_kind)
{
    return byte_5FF778[weapon_kind];
}

MATCH_FUNC(0x5e3e90)
Weapon_8::Weapon_8()
{
    if (!gWeapon_2FDC_707014)
    {
        gWeapon_2FDC_707014 = new Weapon_2FDC();
        if (!gWeapon_2FDC_707014)
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
    if (gWeapon_2FDC_707014)
    {
        GTA2_DELETE_AND_NULL(gWeapon_2FDC_707014);
    }
}

MATCH_FUNC(0x5E4090)
Weapon_2FDC::~Weapon_2FDC()
{
    field_0 = 0;
    field_4 = 0;
}