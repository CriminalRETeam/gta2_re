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

EXPORT void __stdcall ShockPedsAroundSprite(Sprite* pSprite, Fix16 search_size, bool bCheckAngle, bool bShockAllPeds, Ped* pAttacker, Sprite* pTargetSprite);

class Weapon
{
  public:
    STATIC_MEMBER(bool, sbAllowFlameSegment);
    // 9.6f 0x4A4FE0
    inline u16 get_ammo_4A4FE0() const
    {
        return mAmmo;
    }

    // 9.6f 0x4A4F90
    inline void set_infinite_ammo_4A4F90()
    {
        mAmmo = 0xFFFF;
    }

    // 9.6f 0x4CCA00
    inline void set_idx_4CCA00(s32 v)
    {
        mWeaponType = v;
    }

    // 9.6f 0x4CCA10
    inline void set_pPed_4CCA10(Ped* v)
    {
        mpPed = v;
    }

    // 9.6f 0x4CCA20
    inline void set_car_4CCA20(Car* v)
    {
        mCar = v;
    }

    EXPORT Weapon();
    EXPORT ~Weapon();
    EXPORT void init();
    EXPORT void PoolDeallocate();
    EXPORT void set_ammo_clips(u8 clips);
    EXPORT bool add_ammo_capped(u8 to_add);
    EXPORT bool is_max_capacity() const;
    EXPORT bool CountsAsArmedAttack() const;
    EXPORT void ApplyFastReload();
    EXPORT Object* spawn_bullet(s32 bullet_type, Fix16 x, Fix16 y, Fix16 z, Ang16 rot, Fix16_Point& speed);
    EXPORT void flamethrower();
    EXPORT void shotgun();
    EXPORT void pistol();
    EXPORT void dual_pistol();
    EXPORT void smg();
    EXPORT void throwable(s32 obj_idx, s32 throw_strength, s32 throw_charge);
    EXPORT void TraceBeamToHitTarget();
    EXPORT void ChainShockTargets(u8 chain_depth, Sprite* pSprite, Ang16 target_angle);
    EXPORT void shocker();
    EXPORT void electro_batton();
    EXPORT void car_bomb(bool instant_bomb);
    EXPORT void fire_truck_flamethrower();
    EXPORT void fire_truck_gun();
    EXPORT void tank_main_gun();
    EXPORT void army_gun_jeep();
    EXPORT void oil_stain();
    EXPORT void car_mine();
    EXPORT void car_smg();
    EXPORT bool CanFireWhileHeld() const;
    EXPORT void ChuckThrowable();
    EXPORT void pull_trigger();
    EXPORT void rocket();
    EXPORT bool IsExplosiveWeapon() const;
    EXPORT void GetSoundPos(Fix16* pX, Fix16* pY, Fix16* pZ) const;

    // 9.6f inline 0x4A4FA0
    bool is_infinite_ammo_4A4FA0() const
    {
        return mAmmo == 0xFFFF;
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
            return (mAmmo + 9) / 10;
        }
    }

    // Maybe decrement ammo for weapons like shotgun, molotov, grenades, rocket launcher, (dual) pistol etc
    // 9.6f 0x4CCA30
    __forceinline void decrement_ammo_4CCA30()
    {
        s32 new_ammo = mAmmo - 10;
        if (!is_infinite_ammo_4A4FA0())
        {
            if (new_ammo < 0)
            {
                new_ammo = 0;
            }
            mAmmo = new_ammo;
        }
    }

    // Maybe decrement ammo for weapons like machine gun, SMG etc
    // 9.6f 0x4CCA60
    void DecreaseAmmo_4CCA60()
    {
        if (!is_infinite_ammo_4A4FA0())
        {
            --mAmmo;
        }
    }

    // 9.6f 0x4CCA80
    void SetShotFired_4CCA80(bool new_value)
    {
        mShotFired = new_value;
    }

    // 9.6f 0x41CC80
    inline bool TakeShotFired_41CC80()
    {
        bool ret = mShotFired;
        mShotFired = false;
        return ret;
    }

    // 9.6f 0x4A4F80
    inline bool HasAmmo_4A4F80() const
    {
        return mAmmo != 0;
    }

    // 9.6f 0x4A4FF0
    void SetAmmo_4A4FF0(u16 ammo)
    {
        this->mAmmo = ammo;
    }

    // 9.6f 0x41CC90
    s32 GetWeaponType_41CC90() const
    {
        return mWeaponType;
    }
    
    // 9.6f 0x433810
    void SetTargetHit_433810(s32 value)
    {
        mbTargetHit = value;
    }

    // 9.6f 0x41CC70
    s32 IsTargetHit_41CC70() const
    {
        return mbTargetHit;
    }

    u16 mAmmo; // 0x0
    u8 mReloadTimer; // 0x2
    s8 mUnused3; // 0x3
    s32 mbTargetHit; // 0x4
    s32 mUnused8; // 0x8
    s32 mUnusedC; // 0xC
    s16 mUnused10; // 0x10
    s16 mUnused12; // 0x12
    Car* mCar; // 0x14
    Weapon* mpNext;
    s32 mWeaponType; // 0x1C
    s8 mbAiTestShotFired; // 0x20: AI fired the invisible_rocket_159 test shot, the next shot is real
    s8 mbThrown; // 0x21
    s8 mUnused22; // 0x22
    s8 mUnused23; // 0x23
    Ped* mpPed; // 0x24
    SoundObject* mpSound; // 0x28
    bool mShotFired; // 0x2C
    s8 mUnused2D; // 0x2D
    s8 mUnused2E; // 0x2E
    s8 mUnused2F; // 0x2F
};
GTA2_ASSERT_SIZEOF_ALWAYS(Weapon, 0x30);