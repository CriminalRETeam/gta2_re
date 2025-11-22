#pragma once

#include "Function.hpp"
#include "Weapon_30.hpp"
#include "Object_3C.hpp"
#include "Pool.hpp"

class Ped;
class Car_BC;
class infallible_turing;

class Weapon_8
{
  public:
    EXPORT Weapon_30* allocate_5E3C10(s32 a2, Ped* a3, u8 a4);
    EXPORT void deallocate_5E3CB0(Weapon_30* pWeapon);
    EXPORT Weapon_30* allocate_5E3CE0(s32 a1, Car_BC* a2, u8 a3);
    EXPORT Weapon_30* find_5E3D20(Car_BC* a2, s32 a3);
    EXPORT char_type allocate_5E3D50(s32 a2, u8 a3, Car_BC* a4);
    EXPORT void alloc_car_weapon_5E3DF0(Car_BC* a1);
    EXPORT char_type get_max_ammo_capacity_5E3E70(s32 a2);
    EXPORT char_type get_ammo_5E3E80(s32 a2);
    EXPORT Weapon_8();
    EXPORT ~Weapon_8();
    struct_4 field_0;
    s16 field_4_ref_count;
    s16 field_6;
};

class Weapon_30_Pool
{
  public:
    Weapon_30_Pool() // 4CDA20 inline
    {

    }

    Weapon_30* Allocate()
    {
        Weapon_30* pWeapon = field_0_pool.field_0_pStart;
        field_0_pool.field_0_pStart = pWeapon->mpNext;
        pWeapon->mpNext = field_0_pool.field_4_pPrev;
        field_0_pool.field_4_pPrev = pWeapon;
        return pWeapon;
    }

    // TODO: This is probably a heavily inlined common iteration func
    // Remove/dealloc?
    void sub_4A4F20(Weapon_30* pW30)
    {
        Weapon_30* pIter = this->field_0_pool.field_4_pPrev;
        Weapon_30* pLastIter = 0;
        while (pIter)
        {
            if (pIter == pW30)
            {
                pIter->clear_5DCDE0();
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
