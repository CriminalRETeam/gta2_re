#pragma once

#include "particle_flags.hpp"
#include "Function.hpp"
#include "ang16.hpp"
#include "Fix16_Vec.hpp"

class Sprite;
class Explosion;

class Particle_4C
{
  public:
    EXPORT bool UpdateFloatingParticle_state_6_15_16_17();
    EXPORT bool UpdateDirectedProjectile_state_3_12();
    EXPORT bool UpdateBeamSegment_state_43();
    EXPORT bool UpdateObjectBeamLink_state_38();
    EXPORT bool UpdateDirectedBurstSweep_state_4();
    EXPORT bool UpdateDirectedBurst_state_13_14_36();
    EXPORT bool UpdateCircularBurst_state_5();
    EXPORT bool UpdateStaticAnim_state_39();
    EXPORT bool UpdateSkidOrScrapeSpark_state_40_41();
    EXPORT bool Empty_state_42();
    EXPORT bool UpdateSimpleBallisticMotion_state_1();
    EXPORT bool UpdateLargeBallisticDebris_state_35();
    EXPORT bool Empty_state_44();
    EXPORT bool UpdateDebrisArc_state_7();
    EXPORT bool UpdateShortAnim_state_37();
    EXPORT bool UpdateAttachedEmitter_state_9_10();
    EXPORT bool UpdateBurstAnimation_state_29_30();
    EXPORT bool UpdateCollisionBurst_state_31_34();
    EXPORT bool PoolUpdate();
    EXPORT void PoolAllocate();
    EXPORT void PoolDeallocate();
    
    s32 mId; // 0x0
    union
    {
        s32 mFlags; // 0x4
        ParticleFlags32 mBf4; // 0x4
    };
    Fix16_Vec mSpeed8; // 0x8
    Fix16_Vec mAdditionalSpeed; // 0x14
    Fix16 mSpeed20; // 0x20
    Ang16 mAngle; // 0x24
    s16 mUnused26; // 0x26
    Sprite* mpSprite; // 0x28
    u16 mCounter; // 0x2C
    u16 mInitialCounter; // 0x2E
    Sprite* mpOwnSprite; // 0x30
    s32 mUnused34; // 0x34
    s32 mState; // 0x38
    Particle_4C* mpNext; // 0x3C
    Explosion* mpExplosion; // 0x40
    s16 mExplosionId; // 0x44
    u8 mSubState; // 0x46
    char_type mUnused47; // 0x47
    u8 mTimer; // 0x48
    char_type mUnused49; // 0x49
    char_type mUnused4A; // 0x4A
    char_type mUnused4B; // 0x4B
};