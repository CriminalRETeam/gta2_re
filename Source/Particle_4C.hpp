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
    EXPORT char_type PoolUpdate();
    EXPORT void PoolAllocate();
    EXPORT void PoolDeallocate();
    
    s32 field_0_id;
    union
    {
        s32 field_4_flags;
        ParticleFlags32 field_4_bf;
    };
    Fix16_Vec field_8_speed;
    Fix16_Vec field_14_additional_speed;
    Fix16 field_20_speed;
    Ang16 field_24_angle;
    s16 field_26_unused;
    Sprite* field_28_pSprite;
    u16 field_2C_counter;
    u16 field_2E_initial_counter;
    Sprite* field_30_pOwnSprite;
    s32 field_34_unused;
    s32 field_38_state;
    Particle_4C* mpNext; // 0x3C
    Explosion* field_40_pExplosion;
    s16 field_44_explosion_id;
    u8 field_46_sub_state;
    char_type field_47_unused;
    u8 field_48_timer;
    char_type field_49_unused;
    char_type field_4A_unused;
    char_type field_4B_unused;
};