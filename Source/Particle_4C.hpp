#pragma once

#include "Function.hpp"
#include "ang16.hpp"

class Sprite;
class Wolfy_30;

class Particle_4C
{
  public:
    EXPORT char_type UpdateFloatingParticle_state_6_15_16_17_538060();
    EXPORT char_type UpdateDirectedProjectile_state_3_12_5384C0();
    EXPORT char_type UpdateBeamSegment_state_43_538A40();
    EXPORT char_type UpdateObjectBeamLink_state_38_538AC0();
    EXPORT char_type UpdateDirectedBurstSweep_state_4_539040();
    EXPORT char_type UpdateDirectedBurst_state_13_14_36_539480();
    EXPORT char_type UpdateCircularBurst_state_5_539890();
    EXPORT char_type UpdateStaticAnim_state_39_53A180();
    EXPORT char_type UpdateSkidOrScrapeSpark_state_40_41_53A280();
    EXPORT char_type Empty_state_42_53AB70();
    EXPORT char_type UpdateSimpleBallisticMotion_state_1_53ABA0();
    EXPORT char_type UpdateLargeBallisticDebris_state_35_53AE60();
    EXPORT char_type Empty_state_44_53B170();
    EXPORT void UpdateDebrisArc_state_7_53B1A0();
    EXPORT char_type UpdateShortAnim_state_37_53B580();
    EXPORT char_type UpdateAttachedEmitter_state_9_10_53B670();
    EXPORT char_type UpdateBurstAnimation_state_29_30_53B9F0();
    EXPORT char_type UpdateCollisionBurst_state_31_34_53BAC0();
    EXPORT s32 Service_53D260();
    EXPORT void PoolAllocate_53E2C0();
    EXPORT void PoolDeallocate_53E2E0();
    
    s32 field_0_id;
    s32 field_4_flags;
    s32 field_8_xpos;
    s32 field_C_ypos;
    s32 field_10;
    s32 field_14;
    s32 field_18;
    s32 field_1C;
    s32 field_20;
    Ang16 field_24;
    s16 field_26;
    Sprite* field_28_pSprite;
    s16 field_2C_counter;
    s16 field_2E;
    Sprite* field_30_pNext;
    s32 field_34;
    s32 field_38_state;
    Particle_4C* mpNext; // 0x3C
    Wolfy_30* field_40_pUnknown;
    s16 field_44;
    u8 field_46_sub_state;
    char_type field_47;
    char_type field_48_timer;
    char_type field_49;
    char_type field_4A;
    char_type field_4B;
};