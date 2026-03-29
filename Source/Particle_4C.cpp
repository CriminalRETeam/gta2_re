#include "Particle_4C.hpp"
#include "sprite.hpp"

STUB_FUNC(0x538060)
char_type Particle_4C::UpdateFloatingParticle_state_6_15_16_17_538060()
{
    return 0;
}

STUB_FUNC(0x5384c0)
char_type Particle_4C::UpdateDirectedProjectile_state_3_12_5384C0()
{
    return 0;
}

STUB_FUNC(0x538a40)
char_type Particle_4C::UpdateBeamSegment_state_43_538A40()
{
    return 0;
}

STUB_FUNC(0x538ac0)
char_type Particle_4C::UpdateObjectBeamLink_state_38_538AC0()
{
    return 0;
}

STUB_FUNC(0x539040)
char_type Particle_4C::UpdateDirectedBurstSweep_state_4_539040()
{
    return 0;
}

STUB_FUNC(0x539480)
char_type Particle_4C::UpdateDirectedBurst_state_13_14_36_539480()
{
    return 0;
}

STUB_FUNC(0x539890)
char_type Particle_4C::UpdateCircularBurst_state_5_539890()
{
    return 0;
}

STUB_FUNC(0x53a180)
char_type Particle_4C::UpdateStaticAnim_state_39_53A180()
{
    return 0;
}

STUB_FUNC(0x53a280)
char_type Particle_4C::UpdateSkidOrScrapeSpark_state_40_41_53A280()
{
    return 0;
}

STUB_FUNC(0x53ab70)
char_type Particle_4C::Empty_state_42_53AB70()
{
    return 0;
}

STUB_FUNC(0x53aba0)
char_type Particle_4C::UpdateSimpleBallisticMotion_state_1_53ABA0()
{
    return 0;
}

STUB_FUNC(0x53ae60)
char_type Particle_4C::UpdateLargeBallisticDebris_state_35_53AE60()
{
    return 0;
}

STUB_FUNC(0x53b170)
char_type Particle_4C::Empty_state_44_53B170()
{
    return 0;
}

STUB_FUNC(0x53b1a0)
void Particle_4C::UpdateDebrisArc_state_7_53B1A0()
{
}

STUB_FUNC(0x53b580)
char_type Particle_4C::UpdateShortAnim_state_37_53B580()
{
    return 0;
}

STUB_FUNC(0x53b670)
char_type Particle_4C::UpdateAttachedEmitter_state_9_10_53B670()
{
    return 0;
}

STUB_FUNC(0x53b9f0)
char_type Particle_4C::UpdateBurstAnimation_state_29_30_53B9F0()
{
    return 0;
}

STUB_FUNC(0x53bac0)
char_type Particle_4C::UpdateCollisionBurst_state_31_34_53BAC0()
{
    return 0;
}

STUB_FUNC(0x53d260)
s32 Particle_4C::Service_53D260()
{
    return 0;
}

MATCH_FUNC(0x53e2c0)
void Particle_4C::PoolAllocate_53E2C0()
{
    field_8_xpos = 0;
    field_C_ypos = 0;
    field_10 = 0;
    field_40_pUnknown = 0;
    field_4_flags &= ~1;
}

MATCH_FUNC(0x53e2e0)
void Particle_4C::PoolDeallocate_53E2E0()
{
    if (field_30_pNext)
    {
        field_30_pNext->field_2C &= ~4u;
        field_30_pNext->field_2C = 0;
        gSprite_Pool_703818->remove(field_30_pNext);
        field_30_pNext = 0;
    }
}
