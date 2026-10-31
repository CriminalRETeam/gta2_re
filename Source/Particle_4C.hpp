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
    EXPORT bool UpdateFloatingParticle_state_6_15_16_17_538060();
    EXPORT bool UpdateDirectedProjectile_state_3_12_5384C0();
    EXPORT bool UpdateBeamSegment_state_43_538A40();
    EXPORT bool UpdateObjectBeamLink_state_38_538AC0();
    EXPORT bool UpdateDirectedBurstSweep_state_4_539040();
    EXPORT bool UpdateDirectedBurst_state_13_14_36_539480();
    EXPORT bool UpdateCircularBurst_state_5_539890();
    EXPORT bool UpdateStaticAnim_state_39_53A180();
    EXPORT bool UpdateSkidOrScrapeSpark_state_40_41_53A280();
    EXPORT bool Empty_state_42_53AB70();
    EXPORT bool UpdateSimpleBallisticMotion_state_1_53ABA0();
    EXPORT bool UpdateLargeBallisticDebris_state_35_53AE60();
    EXPORT bool Empty_state_44_53B170();
    EXPORT bool UpdateDebrisArc_state_7_53B1A0();
    EXPORT bool UpdateShortAnim_state_37_53B580();
    EXPORT bool UpdateAttachedEmitter_state_9_10_53B670();
    EXPORT bool UpdateBurstAnimation_state_29_30_53B9F0();
    EXPORT bool UpdateCollisionBurst_state_31_34_53BAC0();
    EXPORT bool PoolUpdate();
    EXPORT void PoolAllocate();
    EXPORT void PoolDeallocate();
    
    s32 field_0_id; // 0x0
    union
    {
        s32 field_4_flags; // 0x4
        ParticleFlags32 field_4_bf; // 0x4
    };
    Fix16_Vec field_8_speed; // 0x8
    Fix16_Vec field_14_additional_speed; // 0x14
    Fix16 field_20_speed; // 0x20
    Ang16 field_24_angle; // 0x24
    s16 field_26_unused; // 0x26
    Sprite* field_28_pSprite; // 0x28
    u16 field_2C_counter; // 0x2C
    u16 field_2E_initial_counter; // 0x2E
    Sprite* field_30_pOwnSprite; // 0x30
    s32 field_34_unused; // 0x34
    s32 field_38_state; // 0x38
    Particle_4C* mpNext; // 0x3C
    Explosion* field_40_pExplosion; // 0x40
    s16 field_44_explosion_id; // 0x44
    u8 field_46_sub_state; // 0x46
    char_type field_47_unused; // 0x47
    u8 field_48_timer; // 0x48
    char_type field_49_unused; // 0x49
    char_type field_4A_unused; // 0x4A
    char_type field_4B_unused; // 0x4B
};