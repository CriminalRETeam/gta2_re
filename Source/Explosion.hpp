#pragma once

#include "Function.hpp"
#include "fix16.hpp"
#include "ang16.hpp"

class Object;
class Sprite;

class Explosion
{
  public:
    STATIC_MEMBER(Fix16, kFpPoint09);
    STATIC_MEMBER(u8, sExplosionWantsFlash);
    EXPORT Explosion();
    EXPORT ~Explosion();
    EXPORT void EmitRandomDiagonalParticle_540A40();
    EXPORT void EmitFireTrail_3_12_540D30(Ang16 ang, Fix16 speed);
    EXPORT void EmitFireTrail_4_540F90(Ang16 ang, Fix16 speed);
    EXPORT void EmitFireTrail_13_14_5411E0(Ang16 ang, Fix16 speed);
    EXPORT void EmitFireTrail_5_541430(Ang16 ang, Fix16 speed);
    EXPORT Fix16 GetBlastRadius_541680() const;
    EXPORT Fix16 GetBlastHeight_541710() const;
    EXPORT void SpawnFlashParticle_541760();
    EXPORT void ApplyBlastDamage_541850(u16 timerVal);
    EXPORT void EmitExplosion_18_33_541D60();
    EXPORT void EmitExplosion_19_32_542060();
    EXPORT void EmitExplosion_20_542340();
    EXPORT void UpdateExplosion_18_19_20_32_33_542790();
    EXPORT void EmitBuildingDebris_22_23_24_25_542E30(u8 direction_idx);
    EXPORT bool Update_5434A0(Fix16 speed, Ang16 ang);
    EXPORT bool IsFireType_5435D0() const;
    EXPORT void Release_543610();
    EXPORT void Init_543650();
    EXPORT void SetObject_543680(Object* pObj);

    // 9.6f 0x482A90
    inline void SetType_482A90(s32 type_or_state)
    {
        mType = type_or_state;
    }

    s32 mbIn20Pool; // 0x0
    u8 mPoolSlotIdx; // 0x4
    s16 mId; // 0x6
    Fix16 mSpeed8; // 0x8
    Ang16 mAngle; // 0xC
    s32 mType; // 0x10
    Object* mpObj2C; // 0x14
    s16 mParticleCooldown; // 0x18
    u16 mTimer; // 0x1A
    Sprite* mpAttachedSprite; // 0x1C
    Ang16 mUnused20; // 0x20
    Ang16 mSpawnAngle; // 0x22
    Fix16 mParticleSpread; // 0x24
    Fix16 mBlastRadius; // 0x28
    s32 mOwnerPedId; // 0x2C
};
GTA2_ASSERT_SIZEOF_ALWAYS(Explosion, 0x30);

class ExplosionPool
{
  public:
    EXPORT s32 FreeLowestPriority_543690();
    EXPORT Explosion* Allocate_543800();
    EXPORT ExplosionPool();
    EXPORT ~ExplosionPool();
    Explosion mExplosions[40]; // 0x0
    bool mbUsed[40]; // 0x780
};
GTA2_ASSERT_SIZEOF_ALWAYS(ExplosionPool, 0x7A8);

class ExplosionPool_3D4
{
  public:
    EXPORT Explosion* Allocate_543900();
    EXPORT ExplosionPool_3D4();
    EXPORT ~ExplosionPool_3D4();
    Explosion mExplosions[20]; // 0x0
    bool mbUsed[20]; // 0x3C0
};

EXTERN_GLOBAL(ExplosionPool*, gExplosionPool_7A8_6FD5F0);
EXTERN_GLOBAL(ExplosionPool_3D4*, gExplosionPool_3D4_6FD5EC);
