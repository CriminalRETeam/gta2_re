#include "Particle_4C.hpp"
#include "Phi_8CA8.hpp"
#include "PurpleDoom.hpp"
#include "sprite.hpp"

EXTERN_GLOBAL(Fix16, dword_6FD49C);

DEFINE_GLOBAL(Fix16, dword_6FD46C, 0x6FD46C);
DEFINE_GLOBAL(Fix16, dword_6FD28C, 0x6FD28C);
DEFINE_GLOBAL(Fix16_Point, stru_6FD388, 0x6FD388);

STUB_FUNC(0x538060)
char_type Particle_4C::UpdateFloatingParticle_state_6_15_16_17_538060()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x5384c0)
char_type Particle_4C::UpdateDirectedProjectile_state_3_12_5384C0()
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x538a40)
char_type Particle_4C::UpdateBeamSegment_state_43_538A40()
{
    gPurpleDoom_3_679210->Remove_477B00(this->field_30_pNext);

    this->field_46_sub_state++;
    if (field_46_sub_state > 7u)
    {
        return 1;
    }

    field_30_pNext->set_id_lazy_4206C0(field_46_sub_state + gPhi_8CA8_6FCF00->field_8CA4 + 103);

    this->field_30_pNext->field_2C = 0xA2;
    this->field_30_pNext->field_2C |= 4u;
    gPurpleDoom_3_679210->AddToSingleBucket_477AE0(this->field_30_pNext);
    return 0;
}

STUB_FUNC(0x538ac0)
char_type Particle_4C::UpdateObjectBeamLink_state_38_538AC0()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x539040)
char_type Particle_4C::UpdateDirectedBurstSweep_state_4_539040()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x539480)
char_type Particle_4C::UpdateDirectedBurst_state_13_14_36_539480()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x539890)
char_type Particle_4C::UpdateCircularBurst_state_5_539890()
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x53a180)
char_type Particle_4C::UpdateStaticAnim_state_39_53A180()
{
    gPurpleDoom_3_679210->Remove_477B00(this->field_30_pNext);

    if (this->field_2C_counter == 0)
    {
        return 1;
    }

    if (field_48_timer > 0)
    {
        this->field_48_timer--;
    }
    else
    {
        this->field_48_timer = 3;
        this->field_46_sub_state++;
        if (field_46_sub_state > 5u)
        {
            this->field_46_sub_state = 5;
        }
    }

    field_30_pNext->set_id_lazy_4206C0(gPhi_8CA8_6FCF00->field_8CA4 + this->field_46_sub_state + 191);
    gPurpleDoom_3_679210->AddToSingleBucket_477AE0(this->field_30_pNext);
    return 0;
}

STUB_FUNC(0x53a280)
char_type Particle_4C::UpdateSkidOrScrapeSpark_state_40_41_53A280()
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x53ab70)
char_type Particle_4C::Empty_state_42_53AB70()
{
    return 0;
}

WIP_FUNC(0x53aba0)
char_type Particle_4C::UpdateSimpleBallisticMotion_state_1_53ABA0()
{
    WIP_IMPLEMENTED;

    Fix16 v1 = dword_6FD46C;
    Fix16 v3 = dword_6FD49C;
    Fix16 v16 = dword_6FD49C;

    gPurpleDoom_3_679210->Remove_477B00(this->field_30_pNext);

    field_2C_counter = this->field_2C_counter;
    if (!field_2C_counter)
    {
        return 1;
    }

    Fix16 new_z;
    Fix16 v8;
    if ((u16)field_2C_counter <= (u16)((s16)this->field_2E >> 1))
    {
        field_C_ypos = this->field_C_ypos;
        Fix16 v12 = this->field_14 + this->field_8_xpos;
        this->field_8_xpos = v12;
        v8 = v12;
        this->field_C_ypos = this->field_18 + field_C_ypos;
        field_30_pNext = this->field_30_pNext;
        new_z = field_30_pNext->field_1C_zpos - v1;
    }
    else
    {
        Fix16 v6 = this->field_14 + this->field_8_xpos;
        Fix16 v7 = this->field_C_ypos;
        this->field_8_xpos = v6;
        v8 = v6;
        field_30_pNext = this->field_30_pNext;
        this->field_C_ypos = this->field_18 + v7;
        new_z = v1 + field_30_pNext->field_1C_zpos;
    }

    Fix16 new_x = v3 + v8 + field_30_pNext->field_14_xy.x;
    stru_6FD388.x = new_x;
    Fix16 new_y = v16 + this->field_C_ypos + field_30_pNext->field_14_xy.y;
    stru_6FD388.y = new_y;
    if (new_z > dword_6FD28C)
    {
        new_z = dword_6FD28C;
    }

    this->field_30_pNext->set_xyz_lazy_420600(new_x, new_y, new_z);

    gPurpleDoom_3_679210->AddToSingleBucket_477AE0(this->field_30_pNext);

    return 0;
}

STUB_FUNC(0x53ae60)
char_type Particle_4C::UpdateLargeBallisticDebris_state_35_53AE60()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x53b170)
char_type Particle_4C::Empty_state_44_53B170()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x53b1a0)
void Particle_4C::UpdateDebrisArc_state_7_53B1A0()
{
    NOT_IMPLEMENTED;
}

// 9.6f 0x48C790
MATCH_FUNC(0x53b580)
char_type Particle_4C::UpdateShortAnim_state_37_53B580()
{
    gPurpleDoom_3_679210->Remove_477B00(this->field_30_pNext);

    field_46_sub_state++;

    if (field_46_sub_state == 5)
    {
        return 1;
    }

    switch (field_46_sub_state)
    {
        case 1:
            this->field_30_pNext->set_id_lazy_4206C0(field_30_pNext->field_22_sprite_id);
            break;

        default:
            this->field_30_pNext->set_id_4206E0(field_30_pNext->field_22_sprite_id + 4);
            break;
    }

    this->field_30_pNext->Set_2C_0x4_Flag_4337F0();
    gPurpleDoom_3_679210->AddToSingleBucket_477AE0(this->field_30_pNext);
    return 0;
}

STUB_FUNC(0x53b670)
char_type Particle_4C::UpdateAttachedEmitter_state_9_10_53B670()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x53b9f0)
char_type Particle_4C::UpdateBurstAnimation_state_29_30_53B9F0()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x53bac0)
char_type Particle_4C::UpdateCollisionBurst_state_31_34_53BAC0()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x53d260)
s32 Particle_4C::Service_53D260()
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x53e2c0)
void Particle_4C::PoolAllocate()
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
