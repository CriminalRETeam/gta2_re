#pragma once

#include "Function.hpp"
#include "ang16.hpp"

class Player;
class Object;
class Ped;
class Fix16;
class Sprite;
class Car;
class Fix16_Point;

class SoundObject;

EXPORT void __stdcall ShockPedsAroundSprite_5DF270(Sprite* pSprite, Fix16 search_size, bool bCheckAngle, bool bShockAllPeds, Ped* pAttacker, Sprite* pTargetSprite);

class Weapon
{
  public:
    // 9.6f 0x4A4FE0
    inline u16 get_ammo_4A4FE0() const
    {
        return field_0_ammo;
    }

    // 9.6f 0x4A4F90
    inline void set_infinite_ammo_4A4F90()
    {
        field_0_ammo = 0xFFFF;
    }

    // 9.6f 0x4CCA00
    inline void set_idx_4CCA00(s32 v)
    {
        field_1C_weapon_type = v;
    }

    // 9.6f 0x4CCA10
    inline void set_pPed_4CCA10(Ped* v)
    {
        field_24_pPed = v;
    }

    // 9.6f 0x4CCA20
    inline void set_car_4CCA20(Car* v)
    {
        field_14_car = v;
    }

    EXPORT Weapon();
    EXPORT ~Weapon();
    EXPORT void init_5DCD90();
    EXPORT void PoolDeallocate();
    EXPORT void set_ammo_clips_5DCE20(u8 clips);
    EXPORT bool add_ammo_capped_5DCE40(u8 to_add);
    EXPORT bool is_max_capacity_5DCEA0() const;
    EXPORT bool CountsAsArmedAttack_5DCEF0() const;
    EXPORT void ApplyFastReload_5DCF40();
    EXPORT Object* spawn_bullet_5DCF60(s32 bullet_type, Fix16 x, Fix16 y, Fix16 z, Ang16 rot, Fix16_Point& speed);
    EXPORT void flamethrower_5DD0F0();
    EXPORT void shotgun_5DD290();
    EXPORT void pistol_5DD860();
    EXPORT void dual_pistol_5DDA70();
    EXPORT void smg_5DDD20();
    EXPORT void throwable_5DDFC0(s32 obj_idx, s32 throw_strength, s32 throw_charge);
    EXPORT void TraceBeamToHitTarget_5DE4F0();
    EXPORT void ChainShockTargets_5DFB60(u8 chain_depth, Sprite* pSprite, Ang16 target_angle);
    EXPORT void shocker_5E06B0();
    EXPORT void electro_batton_5E0740();
    EXPORT void car_bomb_5E0AB0(bool instant_bomb);
    EXPORT void fire_truck_flamethrower_5E0B10();
    EXPORT void fire_truck_gun_5E0E70();
    EXPORT void tank_main_gun_5E10E0();
    EXPORT void army_gun_jeep_5E13E0();
    EXPORT void oil_stain_5E1DC0();
    EXPORT void car_mine_5E2550();
    EXPORT void car_smg_5E2940();
    EXPORT bool CanFireWhileHeld_5E33C0() const;
    EXPORT void ChuckThrowable_5E34B0();
    EXPORT void pull_trigger_5E3670();
    EXPORT void rocket_5E3850();
    EXPORT bool IsExplosiveWeapon_5E3BD0() const;
    EXPORT void GetSoundPos_5E3F90(Fix16* pX, Fix16* pY, Fix16* pZ) const;

    // 9.6f inline 0x4A4FA0
    bool is_infinite_ammo_4A4FA0() const
    {
        return field_0_ammo == 0xFFFF;
    }

    // 9.6f inline 0x4A4FB0
    u8 get_ammo_4A4FB0() const
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

    // Maybe decrement ammo for weapons like shotgun, molotov, grenades, rocket launcher, (dual) pistol etc
    __forceinline void decrement_ammo_4CCA30()
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

    // Maybe decrement ammo for weapons like machine gun, SMG etc
    void DecreaseAmmo_4CCA60()
    {
        if (!is_infinite_ammo_4A4FA0())
        {
            --field_0_ammo;
        }
    }

    void SetShotFired_4CCA80(bool new_value)
    {
        field_2C_shot_fired = new_value;
    }

    // 9.6f 0x41CC80
    inline bool TakeShotFired_41CC80()
    {
        bool ret = field_2C_shot_fired;
        field_2C_shot_fired = false;
        return ret;
    }

    inline bool HasAmmo_4A4F80() const
    {
        return field_0_ammo != 0;
    }

    void SetAmmo_4A4FF0(u16 ammo)
    {
        this->field_0_ammo = ammo;
    }

    s32 GetWeaponType_41CC90() const
    {
        return field_1C_weapon_type;
    }
    
    void SetTargetHit_433810(s32 value)
    {
        field_4_bTargetHit = value;
    }

    s32 IsTargetHit_41CC70() const
    {
        return field_4_bTargetHit;
    }

    u16 field_0_ammo;
    u8 field_2_reload_timer;
    s8 field_3_unused;
    s32 field_4_bTargetHit;
    s32 field_8_unused;
    s32 field_C_unused;
    s16 field_10_unused;
    s16 field_12_unused;
    Car* field_14_car;
    Weapon* mpNext;
    s32 field_1C_weapon_type;
    s8 field_20_bAiTestShotFired; // AI fired the invisible_rocket_159 test shot, the next shot is real
    s8 field_21_bThrown;
    s8 field_22_unused;
    s8 field_23_unused;
    Ped* field_24_pPed;
    SoundObject* field_28_pSound;
    bool field_2C_shot_fired;
    s8 field_2D_unused;
    s8 field_2E_unused;
    s8 field_2F_unused;
};
GTA2_ASSERT_SIZEOF_ALWAYS(Weapon, 0x30);