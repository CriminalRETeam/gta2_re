#include "Particle_4C.hpp"
#include "Phi_8CA8.hpp"
#include "PurpleDoom.hpp"
#include "sprite.hpp"

EXTERN_GLOBAL(Fix16, dword_6FD49C);
EXTERN_GLOBAL(Fix16, dword_6FD2F0);

DEFINE_GLOBAL(Fix16, dword_6FD46C, 0x6FD46C);
DEFINE_GLOBAL(Fix16, dword_6FD28C, 0x6FD28C);
//DEFINE_GLOBAL(Fix16_Point, stru_6FD388, 0x6FD388);

DEFINE_GLOBAL(Fix16, stru_6FD388, 0x6FD388);
DEFINE_GLOBAL(Fix16, stru_6FD38C, 0x6FD38C);
DEFINE_GLOBAL(Fix16, dword_6FD2E8, 0x6FD2E8);
DEFINE_GLOBAL(Fix16, dword_6FD4A0, 0x6FD4A0);
DEFINE_GLOBAL(Fix16, dword_6FD39C, 0x6FD39C);
DEFINE_GLOBAL(Fix16, dword_6FD4A4, 0x6FD4A4);

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

struct Fix16_Vec2
{
    Fix16 x, y, z;

    Fix16_Vec2(Fix16 x, Fix16 y, Fix16 z) : x(x), y(y), z(z)
    {
    }

    ~Fix16_Vec2()
    {
    }
};

// 9.6f 0x48C270
// 92%
WIP_FUNC(0x53aba0)
char_type Particle_4C::UpdateSimpleBallisticMotion_state_1_53ABA0()
{
    WIP_IMPLEMENTED;

    Fix16_Vec2 v(dword_6FD49C, dword_6FD49C, dword_6FD46C);
    Fix16 new_z;

    gPurpleDoom_3_679210->Remove_477B00(this->field_30_pNext);

    if (!field_2C_counter)
    {
        return 1;
    }

    if (field_2C_counter > this->field_2E >> 1)
    {
        this->field_8_xpos = this->field_14 + this->field_8_xpos;
        this->field_C_ypos = this->field_18 + field_C_ypos;
        new_z = field_30_pNext->field_1C_zpos + v.z;
    }
    else
    {
        this->field_8_xpos = this->field_14 + this->field_8_xpos;
        this->field_C_ypos = this->field_18 + field_C_ypos;
        new_z = field_30_pNext->field_1C_zpos - v.z;
    }

    stru_6FD388 = v.x + field_8_xpos + field_30_pNext->field_14_xy.x;
    stru_6FD38C = v.y + field_C_ypos + field_30_pNext->field_14_xy.y;
    if (new_z > dword_6FD28C)
    {
        new_z = dword_6FD28C;
    }

    this->field_30_pNext->set_xyz_lazy_420600(stru_6FD388, stru_6FD38C, new_z);

    gPurpleDoom_3_679210->AddToSingleBucket_477AE0(this->field_30_pNext);

    return 0;
}

STUB_FUNC(0x53ae60)
char_type Particle_4C::UpdateLargeBallisticDebris_state_35_53AE60()
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x53b170)
char_type Particle_4C::Empty_state_44_53B170()
{
    return 0;
}

STUB_FUNC(0x53b1a0)
char_type Particle_4C::UpdateDebrisArc_state_7_53B1A0()
{
    NOT_IMPLEMENTED;
    return 0;
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

WIP_FUNC(0x53d260)
bool Particle_4C::PoolUpdate()
{
    WIP_IMPLEMENTED;

    s32 state = this->field_38_state;
    --this->field_2C_counter;
    switch (state - 1)
    {
        case 0:
            return UpdateSimpleBallisticMotion_state_1_53ABA0();
        case 2:
        case 11:
            return UpdateDirectedProjectile_state_3_12_5384C0();

        case 3:
            return UpdateDirectedBurstSweep_state_4_539040();

        case 4:
            return UpdateCircularBurst_state_5_539890();

        case 5:
        case 14:
        case 15:
        case 16:
            return UpdateFloatingParticle_state_6_15_16_17_538060();

        case 6:
            return UpdateDebrisArc_state_7_53B1A0();

        case 7:
            return true;

        case 8:
        case 9:
            return UpdateAttachedEmitter_state_9_10_53B670();

        case 12:
        case 13:
        case 35:
            return UpdateDirectedBurst_state_13_14_36_539480();

        case 17:
        case 32:
            if (field_48_timer)
            {
                this->field_48_timer--;
                return false;
            }
            else
            {
                this->field_46_sub_state++;
                if (field_46_sub_state < 6u || field_46_sub_state > 10u)
                {
                    this->field_48_timer = 1;
                }

                gPurpleDoom_3_679210->Remove_477B00(this->field_30_pNext);
                if (field_46_sub_state <= 15u)
                {
                    field_30_pNext->set_id_lazy_4206C0(field_46_sub_state + gPhi_8CA8_6FCF00->field_8CA4 + 20);
                    if (field_30_pNext->field_1C_zpos + dword_6FD2F0 < dword_6FD28C)
                    {
                        field_30_pNext->set_xyz_lazy_420600(field_30_pNext->field_14_xy.x,
                                                            field_30_pNext->field_14_xy.y,
                                                            field_30_pNext->field_1C_zpos + dword_6FD2E8);
                    }

                    gPurpleDoom_3_679210->AddToSingleBucket_477AE0(this->field_30_pNext);
                    if (this->field_46_sub_state == 2)
                    {
                        this->field_30_pNext->field_2C = 0xA2; // sub_4337D0
                    }
                    this->field_30_pNext->field_2C |= 4u;
                    return false;
                }
                else
                {
                    return true;
                }
            }
            break;

        case 18:
        case 31:
            if (field_48_timer)
            {
                this->field_48_timer--;
                return false;
            }
            else
            {
                this->field_46_sub_state++;
                if (field_46_sub_state < 6u || field_46_sub_state > 10u)
                {
                    this->field_48_timer = 1;
                }

                gPurpleDoom_3_679210->Remove_477B00(this->field_30_pNext);
                if (field_46_sub_state <= 15u)
                {
                    field_30_pNext->set_id_lazy_4206C0(field_46_sub_state + gPhi_8CA8_6FCF00->field_8CA4 + 20);

                    if (field_30_pNext->field_1C_zpos + dword_6FD2F0 < dword_6FD28C)
                    {
                        field_30_pNext->set_xyz_lazy_420600(field_30_pNext->field_14_xy.x, field_30_pNext->field_14_xy.y, field_30_pNext->field_1C_zpos + dword_6FD2E8);
                    }

                    gPurpleDoom_3_679210->AddToSingleBucket_477AE0(this->field_30_pNext);
                    if (this->field_46_sub_state == 2)
                    {
                        this->field_30_pNext->field_2C = 0xA2;
                    }
                    field_30_pNext->field_2C |= 4u;
                    field_30_pNext->ApplyScaleToDimensions_59E4C0(dword_6FD4A0 + dword_6FD39C, 0);
                    return false;
                }
                else
                {
                    return true;
                }
            }
            break;

        case 19:
            if (field_48_timer)
            {
                this->field_48_timer--;
                return false;
            }
            else
            {
                this->field_46_sub_state++;
                if (field_46_sub_state < 6u || field_46_sub_state > 0xAu)
                {
                    this->field_48_timer = 1;
                }

                gPurpleDoom_3_679210->Remove_477B00(this->field_30_pNext);
                if (field_46_sub_state <= 0xFu)
                {
                    field_30_pNext->set_id_lazy_4206C0(field_46_sub_state + gPhi_8CA8_6FCF00->field_8CA4 + 56);

                    if (field_30_pNext->field_1C_zpos + dword_6FD2E8 + dword_6FD2F0 < dword_6FD28C)
                    {
                        field_30_pNext->set_xyz_lazy_451950(field_30_pNext->field_14_xy.x,
                                                            field_30_pNext->field_14_xy.y,
                                                            field_30_pNext->field_1C_zpos + dword_6FD2E8);
                    }

                    gPurpleDoom_3_679210->AddToSingleBucket_477AE0(this->field_30_pNext);
                    if (this->field_46_sub_state == 2)
                    {
                        this->field_30_pNext->field_2C = -94;
                    }

                    field_30_pNext->field_2C |= 4u;
                    field_30_pNext->ApplyScaleToDimensions_59E4C0(dword_6FD4A4, 0);
                    return false;
                }
                else
                {
                    return true;
                }
            }
            break;

        case 21:
        case 22:
        case 23:
        case 24:
            if (field_48_timer)
            {
                this->field_48_timer--;
                return false;
            }

            gPurpleDoom_3_679210->Remove_477B00(this->field_30_pNext);
            this->field_48_timer = 1;
            this->field_46_sub_state++;
            if (field_46_sub_state > 0xCu)
            {
                return true;
            }

            field_30_pNext->set_id_lazy_4206C0(field_46_sub_state + gPhi_8CA8_6FCF00->field_8CA4 + 147);

            stru_6FD388 = (gSin_table_667A80[this->field_24.rValue] * this->field_20);
            stru_6FD38C = (gCos_table_669260[this->field_24.rValue] * this->field_20);

            stru_6FD388 = this->field_30_pNext->field_14_xy.x + stru_6FD388;
            stru_6FD38C = this->field_30_pNext->field_14_xy.y + stru_6FD38C;


            if (this->field_46_sub_state >= 9u)
            {
                field_30_pNext->set_xyz_lazy_420600(stru_6FD388, stru_6FD38C, field_30_pNext->field_1C_zpos + dword_6FD46C);
            }
            else
            {
                field_30_pNext->set_xyz_lazy_420600(stru_6FD388, stru_6FD38C, field_30_pNext->field_1C_zpos + dword_6FD2E8);
            }

            if (field_30_pNext->field_1C_zpos > dword_6FD28C)
            {
                field_30_pNext->field_1C_zpos = dword_6FD28C;
                field_30_pNext->ResetZCollisionAndDebugBoxes_59E7B0();
            }

            gPurpleDoom_3_679210->AddToSingleBucket_477AE0(this->field_30_pNext);
            field_30_pNext->field_2C = 82;
            field_30_pNext->field_2C |= 4u;
            return false;

        case 25:
            gPurpleDoom_3_679210->Remove_477B00(this->field_30_pNext);
            if (this->field_2C_counter)
            {
                field_30_pNext->set_id_lazy_4206C0(gPhi_8CA8_6FCF00->field_8CA4);
                gPurpleDoom_3_679210->AddToSingleBucket_477AE0(this->field_30_pNext);
                return false;
            }
            else
            {
                return true;
            }
            break;

        case 28:
        case 29:
            return UpdateBurstAnimation_state_29_30_53B9F0();

        case 30:
        case 33:
            return UpdateCollisionBurst_state_31_34_53BAC0();

        case 34:
            return UpdateLargeBallisticDebris_state_35_53AE60();

        case 36:
            return UpdateShortAnim_state_37_53B580();

        case 37:
            return UpdateObjectBeamLink_state_38_538AC0();

        case 38:
            return UpdateStaticAnim_state_39_53A180();

        case 39:
        case 40:
            return UpdateSkidOrScrapeSpark_state_40_41_53A280();

        case 41:
            return Empty_state_42_53AB70();

        case 42:
            return UpdateBeamSegment_state_43_538A40();

        case 43:
            return Empty_state_44_53B170();

        default:
            return false;
    }
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
void Particle_4C::PoolDeallocate()
{
    if (field_30_pNext)
    {
        field_30_pNext->field_2C &= ~4u;
        field_30_pNext->field_2C = 0;
        gSprite_Pool_703818->remove(field_30_pNext);
        field_30_pNext = 0;
    }
}
