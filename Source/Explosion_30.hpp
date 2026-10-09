#pragma once

#include "Function.hpp"
#include "fix16.hpp"
#include "ang16.hpp"

class Object_2C;
class Sprite;

class Explosion_30
{
  public:
    EXPORT Explosion_30();
    EXPORT ~Explosion_30();
    EXPORT void EmitFireTrail_3_12_540D30(Ang16 ang, Fix16 speed);
    EXPORT void EmitFireTrail_4_540F90(Ang16 ang, Fix16 speed);
    EXPORT void EmitFireTrail_13_14_5411E0(Ang16 ang, Fix16 speed);
    EXPORT void EmitFireTrail_5_541430(Ang16 ang, Fix16 speed);
    EXPORT Fix16 GetBlastRadius_541680();
    EXPORT Fix16 GetBlastHeight_541710();
    EXPORT void SpawnFlashParticle_541760();
    EXPORT void ApplyBlastDamage_541850(u16 timerVal);
    EXPORT void EmitExplosion_18_33_541D60();
    EXPORT void EmitExplosion_19_32_542060();
    EXPORT void EmitExplosion_20_542340();
    EXPORT void UpdateExplosion_18_19_20_32_33_542790();
    EXPORT void EmitBuildingDebris_22_23_24_25_542E30(u8 direction_idx);
    EXPORT char_type Update_5434A0(Fix16 speed, Ang16 ang);
    EXPORT bool IsFireType_5435D0();
    EXPORT void Release_543610();
    EXPORT void Init_543650();
    EXPORT void SetObject_543680(Object_2C* pObj);

    // 9.6f 0x482A90
    inline void SetType_482A90(s32 type_or_state)
    {
        field_10_type = type_or_state;
    }

    s32 field_0_bIn20Pool;
    u8 field_4_idx;
    s16 field_6_id;
    Fix16 field_8_speed;
    Ang16 field_C_angle;
    s32 field_10_type;
    Object_2C* field_14_pObj2C;
    s16 field_18_particle_cooldown;
    u16 field_1A_timer;
    Sprite* field_1C_pAttachedSprite;
    Ang16 field_20_unused;
    Ang16 field_22_spawn_angle;
    Fix16 field_24_particle_spread;
    Fix16 field_28_blast_radius;
    s32 field_2C_owner_ped_id;
};

class ExplosionPool_7A8
{
  public:
    EXPORT s32 FreeLowestPriority_543690();
    EXPORT Explosion_30* Allocate_543800();
    EXPORT ExplosionPool_7A8();
    EXPORT ~ExplosionPool_7A8();
    Explosion_30 field_0_explosions[40];
    bool field_780_bUsed[40];
};

class ExplosionPool_3D4
{
  public:
    EXPORT ExplosionPool_3D4();
    EXPORT ~ExplosionPool_3D4();
    Explosion_30 field_0_explosions[20];
    bool field_3C0_bUsed[20];
};

EXTERN_GLOBAL(ExplosionPool_7A8*, gExplosionPool_7A8_6FD5F0);
EXTERN_GLOBAL(ExplosionPool_3D4*, gExplosionPool_3D4_6FD5EC);
