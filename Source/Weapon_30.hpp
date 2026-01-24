#pragma once

#include "Function.hpp"
#include "ang16.hpp"

class Player;
class Object_2C;
class Ped;
class Fix16;
class Sprite;
class Car_BC;
class Fix16_Point;

class infallible_turing;

class Weapon_30
{
  public:
    EXPORT Weapon_30();
    EXPORT ~Weapon_30();
    EXPORT void init_5DCD90();
    EXPORT void clear_5DCDE0();
    EXPORT void add_ammo_5DCE20(u8 a2);
    EXPORT char_type add_ammo_capped_5DCE40(u8 a2);
    EXPORT bool is_max_capacity_5DCEA0();
    EXPORT bool sub_5DCEF0();
    EXPORT void TickReloadSpeed_5DCF40();
    EXPORT Object_2C* spawn_bullet_5DCF60(s32 bullet_type, Fix16 x, Fix16 y, Fix16 z, Ang16 rot, const Fix16_Point* pPoint);
    EXPORT void flamethrower_5DD0F0();
    EXPORT u8 shotgun_5DD290();
    EXPORT void pistol_5DD860();
    EXPORT u8 dual_pistol_5DDA70();
    EXPORT u8 smg_5DDD20();
    EXPORT void throwable_5DDFC0(s32 a2, s32 a3, s32 a4);
    EXPORT s32 sub_5DE4F0();
    EXPORT char_type sub_5DFB60(char_type a2, Sprite* a3, Ang16 a4);
    EXPORT void shocker_5E06B0();
    EXPORT char_type electro_batton_5E0740();
    EXPORT void car_bomb_5E0AB0(char_type a2);
    EXPORT char_type fire_truck_flamethrower_5E0B10();
    EXPORT s32* fire_truck_gun_5E0E70();
    EXPORT u8 tank_main_gun_5E10E0();
    EXPORT u8 army_gun_jeep_5E13E0();
    EXPORT char_type oil_stain_5E1DC0();
    EXPORT char_type car_mine_5E2550();
    EXPORT u8 car_smg_5E2940();
    EXPORT char_type sub_5E33C0();
    EXPORT char_type sub_5E34B0();
    EXPORT void pull_trigger_5E3670();
    EXPORT void rocket_5E3850();
    EXPORT char_type IsExplosiveWeapon_5E3BD0();
    EXPORT void GetSoundPos_5E3F90(Fix16* pX, Fix16* pY, Fix16* pZ);

    // 9.6f inline 0x4A4FA0
    bool is_infinite_ammo_4A4FA0()
    {
        return field_0_ammo == 0xFFFF;
    }

    // 9.6f inline 0x4A4FB0
    u8 get_ammo_4A4FB0()
    {
        if (is_infinite_ammo_4A4FA0())
        {
            return -1;
        }
        else
        {
            return (field_0_ammo + 9) / 10;
        }
    }

    void decrement_ammo_4CCA30()
    {
        s32 new_ammo = field_0_ammo - 10;
        if (!is_infinite_ammo_4A4FA0())
        {
            if (new_ammo < 0)
            {
                new_ammo = 0;
            }
            field_0_ammo = new_ammo;
        }
    }

    void set_field_2C_4CCA80(char_type new_value)
    {
        field_2C = new_value;
    }

    inline bool HasAmmo_4A4F80()
    {
        return field_0_ammo != 0;
    }

    u16 field_0_ammo;
    u8 field_2_reload_speed;
    s8 field_3;
    s32 field_4;
    s32 field_8;
    s32 field_C;
    s16 field_10;
    s16 field_12;
    Car_BC* field_14_car;
    Weapon_30* mpNext;
    s32 field_1C_idx; // weapon_type
    s8 field_20;
    s8 field_21;
    s8 field_22;
    s8 field_23;
    Ped* field_24_pPed;
    infallible_turing* field_28_pSound;
    s8 field_2C;
    s8 field_2D;
    s8 field_2E;
    s8 field_2F;
};