#include "Particle_4C.hpp"
#include "map_0x370.hpp"
#include "Object_5C.hpp"
#include "Phi_8CA8.hpp"
#include "PurpleDoom.hpp"
#include "rng.hpp"
#include "sprite.hpp"
#include "Wolfy_3D4.hpp"

EXTERN_GLOBAL(Fix16, kFP16Zero_6FD49C);
EXTERN_GLOBAL(Fix16, dword_6FD2F0);
EXTERN_GLOBAL(Fix16, dword_6FD448);
EXTERN_GLOBAL(Fix16, kFP16Eight_6FD4C0);

DEFINE_GLOBAL_INIT(Fix16, dword_6FD46C, Fix16(0x333, 0), 0x6FD46C);
DEFINE_GLOBAL_INIT(Fix16, dword_6FD554, dword_6FD448, 0x6FD554);
DEFINE_GLOBAL_INIT(Fix16, dword_6FD28C, kFP16Eight_6FD4C0 - dword_6FD554, 0x6FD28C);
//DEFINE_GLOBAL(Fix16_Point, stru_6FD388, 0x6FD388);

DEFINE_GLOBAL(Fix16, stru_6FD388, 0x6FD388);
DEFINE_GLOBAL(Fix16, stru_6FD38C, 0x6FD38C);
DEFINE_GLOBAL_INIT(Fix16, dword_6FD2E8, Fix16(0x666, 0), 0x6FD2E8);
DEFINE_GLOBAL_INIT(Fix16, kFP16One_6FD4A0, Fix16(1), 0x6FD4A0);
DEFINE_GLOBAL_INIT(Fix16, kFP16Half_6FD39C, Fix16(0.5f), 0x6FD39C);
DEFINE_GLOBAL_INIT(Fix16, kFP16Two_6FD4A4, Fix16(2), 0x6FD4A4);

DEFINE_GLOBAL_INIT(Fix16, dword_6FD45C, Fix16(0xA3, 0), 0x6FD45C);
DEFINE_GLOBAL_INIT(Fix16, dword_6FD564, Fix16(0x51, 0), 0x6FD564);
DEFINE_GLOBAL_INIT(Fix16, dword_6FD538, Fix16(0x31, 0), 0x6FD538);

DEFINE_GLOBAL_INIT(Fix16, dword_6FD55C, Fix16(0x3000, 0), 0x6FD55C);
DEFINE_GLOBAL_INIT(Fix16, dword_6FD30C, Fix16(0x3999, 0), 0x6FD30C);
DEFINE_GLOBAL_INIT(Fix16, dword_6FD280, Fix16(255), 0x6FD280);

// https://decomp.me/scratch/nKSYL
WIP_FUNC(0x538060)
char_type Particle_4C::UpdateFloatingParticle_state_6_15_16_17_538060()
{
    WIP_IMPLEMENTED;
    Fix16 rng_1;
    Fix16 rng_2;
    Fix16_Point vector(Fix16(0), Fix16(0));
    Fix16 new_z = dword_6FD45C + field_30_pNext->field_1C_zpos;
    gPurpleDoom_3_679210->Remove_477B00(field_30_pNext);
    if (field_2C_counter == 0)
    {
        return true;
    }
    if (new_z > dword_6FD28C)
    {
        return true;
    }

    if (new_z.GetFracValue() > dword_6FD55C &&
        gMap_0x370_6F6268->IsBlockNonAirType_48A350(field_30_pNext->field_14_xy.x.ToInt(),
                                                    field_30_pNext->field_14_xy.y.ToInt(),
                                                    new_z.ToInt()))
    {
        rng_1 = Fix16(gRng_6F6784.get_int_4F7AE0(61) - 30) / 100;
        rng_2 = Fix16(gRng_6F6784.get_int_4F7AE0(10) - 5) / 100;
        ++field_2C_counter;
    }
    else
    {
        rng_1 = Fix16(gRng_6F6784.get_int_4F7AE0(3) - 1) / 100;
        rng_2 = Fix16(gRng_6F6784.get_int_4F7AE0(3) - 1) / 100;
    }

    if (field_40_pUnknown)
    {
        if (field_2C_counter > 60)
        {
            field_40_pUnknown->field_14_pObj2C;
            if (field_40_pUnknown->field_14_pObj2C->field_4)
            {
                field_20 = field_40_pUnknown->field_14_pObj2C->field_4->field_8_object_2C_ptr->sub_5290F0();
                field_24_angle = field_40_pUnknown->field_14_pObj2C->field_4->field_8_object_2C_ptr->field_10_obj_3c->field_4_angle;
            }
            if (field_40_pUnknown->field_1A_timer == 1)
            {
                field_40_pUnknown = NULL;
            }
        }
    }
    else
    {
        field_20 = kFP16Zero_6FD49C;
    }

    if (field_20 == kFP16Zero_6FD49C)
    {
        stru_6FD388 = field_30_pNext->field_14_xy.x + rng_1;
        stru_6FD38C = field_30_pNext->field_14_xy.y + rng_2;
        if (stru_6FD388 > kFP16One_6FD4A0 && stru_6FD388 < dword_6FD280 - kFP16One_6FD4A0 && stru_6FD38C > kFP16One_6FD4A0 &&
            stru_6FD38C < dword_6FD280 - kFP16One_6FD4A0)
        {
            field_30_pNext->set_xyz_lazy_420600(stru_6FD388, stru_6FD38C, new_z);
        }
        else
        {
            field_30_pNext->set_xyz_lazy_420600(field_30_pNext->field_14_xy.x, field_30_pNext->field_14_xy.y, new_z);
        }
    }
    else
    {
        field_20 = field_20 * dword_6FD30C;
        if (field_20 < kFP16Zero_6FD49C)
        {
            field_20 = kFP16Zero_6FD49C;
        }
        vector.x = field_20;
        vector.y = kFP16Zero_6FD49C;
        vector.RotateByAngle_40F6B0(field_24_angle);

        field_14_additional_speed_x = vector.x;
        field_18_additional_speed_y = vector.y;

        field_8_speed_x = field_14_additional_speed_x + rng_1;
        field_C_speed_y = field_18_additional_speed_y + rng_2;

        stru_6FD388 = field_30_pNext->field_14_xy.x + rng_1;
        stru_6FD38C = field_30_pNext->field_14_xy.y + rng_2;
        if (stru_6FD388 > kFP16One_6FD4A0 && stru_6FD388 < dword_6FD280 - kFP16One_6FD4A0 && stru_6FD38C > kFP16One_6FD4A0 &&
            stru_6FD38C < dword_6FD280 - kFP16One_6FD4A0)
        {
            field_30_pNext->set_xyz_lazy_420600(stru_6FD388, stru_6FD38C, new_z);
        }
        else
        {
            field_30_pNext->set_xyz_lazy_420600(field_30_pNext->field_14_xy.x, field_30_pNext->field_14_xy.y, new_z);
        }
    }

    gPurpleDoom_3_679210->AddToSingleBucket_477AE0(field_30_pNext);
    return false;
}

STUB_FUNC(0x5384c0)
char_type Particle_4C::UpdateDirectedProjectile_state_3_12_5384C0()
{
    NOT_IMPLEMENTED;

    // provisional code
    gPurpleDoom_3_679210->Remove_477B00(field_30_pNext);
    if (field_2C_counter == 0)
    {
        return true;
    }
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

    field_30_pNext->set_id_lazy_4206C0(field_46_sub_state + gPhi_8CA8_6FCF00->field_8CA4_def112_sprite_palette + 103);

    this->field_30_pNext->field_2C_flags = 0xA2;
    this->field_30_pNext->field_2C_flags |= 4u;
    gPurpleDoom_3_679210->AddToSingleBucket_477AE0(this->field_30_pNext);
    return 0;
}

STUB_FUNC(0x538ac0)
char_type Particle_4C::UpdateObjectBeamLink_state_38_538AC0()
{
    NOT_IMPLEMENTED;

    // provisional code
    ++field_46_sub_state;
    gPurpleDoom_3_679210->Remove_477B00(field_30_pNext);
    if (field_46_sub_state == 6)
    {
        return true;
    }
    return 0;
}

STUB_FUNC(0x539040)
char_type Particle_4C::UpdateDirectedBurstSweep_state_4_539040()
{
    NOT_IMPLEMENTED;

    // provisional code
    ++field_46_sub_state;
    gPurpleDoom_3_679210->Remove_477B00(field_30_pNext);
    if (field_46_sub_state == 16)
    {
        return true;
    }
    if (field_2C_counter == 0)
    {
        return true;
    }
    return 0;
}

STUB_FUNC(0x539480)
char_type Particle_4C::UpdateDirectedBurst_state_13_14_36_539480()
{
    NOT_IMPLEMENTED;

    // provisional code
    ++field_46_sub_state;
    gPurpleDoom_3_679210->Remove_477B00(field_30_pNext);
    if (field_46_sub_state == 16)
    {
        return true;
    }
    if (field_2C_counter == 0)
    {
        return true;
    }
    return 0;
}

// https://decomp.me/scratch/2UJLM
STUB_FUNC(0x539890)
char_type Particle_4C::UpdateCircularBurst_state_5_539890()
{
    NOT_IMPLEMENTED;

    // provisional code
    gPurpleDoom_3_679210->Remove_477B00(field_30_pNext);
    if (field_2C_counter == 0)
    {
        return true;
    }
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

    field_30_pNext->set_id_lazy_4206C0(gPhi_8CA8_6FCF00->field_8CA4_def112_sprite_palette + this->field_46_sub_state + 191);
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

    Fix16_Vec2 v(kFP16Zero_6FD49C, kFP16Zero_6FD49C, dword_6FD46C);
    Fix16 new_z;

    gPurpleDoom_3_679210->Remove_477B00(this->field_30_pNext);

    if (!field_2C_counter)
    {
        return 1;
    }

    if (field_2C_counter > this->field_2E >> 1)
    {
        this->field_8_speed_x = this->field_14_additional_speed_x + this->field_8_speed_x;
        this->field_C_speed_y = this->field_18_additional_speed_y + field_C_speed_y;
        new_z = field_30_pNext->field_1C_zpos + v.z;
    }
    else
    {
        this->field_8_speed_x = this->field_14_additional_speed_x + this->field_8_speed_x;
        this->field_C_speed_y = this->field_18_additional_speed_y + field_C_speed_y;
        new_z = field_30_pNext->field_1C_zpos - v.z;
    }

    stru_6FD388 = v.x + field_8_speed_x + field_30_pNext->field_14_xy.x;
    stru_6FD38C = v.y + field_C_speed_y + field_30_pNext->field_14_xy.y;
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

    // provisional code
    ++field_46_sub_state;
    gPurpleDoom_3_679210->Remove_477B00(field_30_pNext);
    if (field_46_sub_state == 15)
    {
        return true;
    }
    return 0;
}

MATCH_FUNC(0x53b170)
char_type Particle_4C::Empty_state_44_53B170()
{
    return 0;
}

// https://decomp.me/scratch/fW2BZ
WIP_FUNC(0x53b1a0)
bool Particle_4C::UpdateDebrisArc_state_7_53B1A0()
{
    WIP_IMPLEMENTED;
    Fix16 v1 = dword_6FD46C;
    Fix16_Point point(kFP16Zero_6FD49C, kFP16Zero_6FD49C);
    Fix16_Point point2(Fix16(0), Fix16(0));

    gPurpleDoom_3_679210->Remove_477B00(field_30_pNext);
    if (!field_2C_counter)
    {
        return true;
    }

    Fix16 zpos;
    if (field_2C_counter > (field_2E >> 1))
    {
        field_8_speed_x = field_8_speed_x + field_14_additional_speed_x;
        field_C_speed_y = field_C_speed_y + field_18_additional_speed_y;
        zpos = field_30_pNext->field_1C_zpos + v1;
    }
    else
    {
        field_8_speed_x = field_8_speed_x + field_14_additional_speed_x;
        field_C_speed_y = field_C_speed_y + field_18_additional_speed_y;
        zpos = field_30_pNext->field_1C_zpos - v1;
    }
    stru_6FD388 = point.x + field_8_speed_x + field_30_pNext->field_14_xy.x;
    stru_6FD38C = point.y + field_C_speed_y + field_30_pNext->field_14_xy.y;

    switch (field_2C_counter)
    {
        case 5:
        case 6:
            field_30_pNext->set_id_lazy_4206C0(gPhi_8CA8_6FCF00->field_8CA4_def112_sprite_palette + 127);
            break;
        case 4:
            field_30_pNext->set_id_4206E0(gPhi_8CA8_6FCF00->field_8CA4_def112_sprite_palette + 128);
            break;
        case 2:
        case 3:
            field_30_pNext->set_id_4206E0(gPhi_8CA8_6FCF00->field_8CA4_def112_sprite_palette + 129);
            break;
        case 0:
        case 1:
            field_30_pNext->set_id_4206E0(gPhi_8CA8_6FCF00->field_8CA4_def112_sprite_palette + 130);
            break;
        default:
            break;
    }
    if (zpos > dword_6FD28C)
    {
        zpos = dword_6FD28C;
    }
    field_30_pNext->set_xyz_lazy_420600(stru_6FD388, stru_6FD38C, zpos);
    gPurpleDoom_3_679210->AddToSingleBucket_477AE0(field_30_pNext);
    return false;
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

    // provisional code
    gPurpleDoom_3_679210->Remove_477B00(field_30_pNext);
    if (field_2C_counter == 0)
    {
        return true;
    }
    return 0;
}

MATCH_FUNC(0x53b9f0)
char_type Particle_4C::UpdateBurstAnimation_state_29_30_53B9F0()
{
    Fix16 scale_related;
    u8 idx;

    ++field_46_sub_state;
    gPurpleDoom_3_679210->Remove_477B00(field_30_pNext);
    if (field_46_sub_state > 25)
    {
        return true;
    }

    if (field_46_sub_state < 20)
    {
        if (field_46_sub_state < 17)
        {
            if (field_46_sub_state < 12)
            {
                scale_related = dword_6FD45C;
                idx = 0;
            }
            else
            {
                scale_related = dword_6FD564;
                idx = 1;
            }
        }
        else
        {
            scale_related = dword_6FD564;
            idx = 2;
        }
    }
    else
    {
        scale_related = dword_6FD538;
        idx = 3;
    }

    field_30_pNext->field_22_sprite_id = idx + gPhi_8CA8_6FCF00->field_8CA4_def112_sprite_palette + 37;
    gPurpleDoom_3_679210->AddToSingleBucket_477AE0(field_30_pNext);
    field_30_pNext->field_2C_flags = 0x52;
    field_30_pNext->field_2C_flags |= 4;
    field_30_pNext->ApplyScaleToDimensions_59E4C0(kFP16One_6FD4A0 + scale_related * Fix16(field_46_sub_state), 0);
    return false;
}

STUB_FUNC(0x53bac0)
char_type Particle_4C::UpdateCollisionBurst_state_31_34_53BAC0()
{
    NOT_IMPLEMENTED;

    // provisional code
    gPurpleDoom_3_679210->Remove_477B00(field_30_pNext);
    if (field_2C_counter == 0)
    {
        return true;
    }
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
                    field_30_pNext->set_id_lazy_4206C0(field_46_sub_state + gPhi_8CA8_6FCF00->field_8CA4_def112_sprite_palette + 20);
                    if (field_30_pNext->field_1C_zpos + dword_6FD2F0 < dword_6FD28C)
                    {
                        field_30_pNext->set_xyz_lazy_420600(field_30_pNext->field_14_xy.x,
                                                            field_30_pNext->field_14_xy.y,
                                                            field_30_pNext->field_1C_zpos + dword_6FD2E8);
                    }

                    gPurpleDoom_3_679210->AddToSingleBucket_477AE0(this->field_30_pNext);
                    if (this->field_46_sub_state == 2)
                    {
                        this->field_30_pNext->field_2C_flags = 0xA2; // sub_4337D0
                    }
                    this->field_30_pNext->field_2C_flags |= 4u;
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
                    field_30_pNext->set_id_lazy_4206C0(field_46_sub_state + gPhi_8CA8_6FCF00->field_8CA4_def112_sprite_palette + 20);

                    if (field_30_pNext->field_1C_zpos + dword_6FD2F0 < dword_6FD28C)
                    {
                        field_30_pNext->set_xyz_lazy_420600(field_30_pNext->field_14_xy.x, field_30_pNext->field_14_xy.y, field_30_pNext->field_1C_zpos + dword_6FD2E8);
                    }

                    gPurpleDoom_3_679210->AddToSingleBucket_477AE0(this->field_30_pNext);
                    if (this->field_46_sub_state == 2)
                    {
                        this->field_30_pNext->field_2C_flags = 0xA2;
                    }
                    field_30_pNext->field_2C_flags |= 4u;
                    field_30_pNext->ApplyScaleToDimensions_59E4C0(kFP16One_6FD4A0 + kFP16Half_6FD39C, 0);
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
                    field_30_pNext->set_id_lazy_4206C0(field_46_sub_state + gPhi_8CA8_6FCF00->field_8CA4_def112_sprite_palette + 56);

                    if (field_30_pNext->field_1C_zpos + dword_6FD2E8 + dword_6FD2F0 < dword_6FD28C)
                    {
                        field_30_pNext->set_xyz_lazy_451950(field_30_pNext->field_14_xy.x,
                                                            field_30_pNext->field_14_xy.y,
                                                            field_30_pNext->field_1C_zpos + dword_6FD2E8);
                    }

                    gPurpleDoom_3_679210->AddToSingleBucket_477AE0(this->field_30_pNext);
                    if (this->field_46_sub_state == 2)
                    {
                        this->field_30_pNext->field_2C_flags = -94;
                    }

                    field_30_pNext->field_2C_flags |= 4u;
                    field_30_pNext->ApplyScaleToDimensions_59E4C0(kFP16Two_6FD4A4, 0);
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

            field_30_pNext->set_id_lazy_4206C0(field_46_sub_state + gPhi_8CA8_6FCF00->field_8CA4_def112_sprite_palette + 147);

            stru_6FD388 = (gSin_table_667A80[this->field_24_angle.rValue] * this->field_20);
            stru_6FD38C = (gCos_table_669260[this->field_24_angle.rValue] * this->field_20);

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
            field_30_pNext->field_2C_flags = 82;
            field_30_pNext->field_2C_flags |= 4u;
            return false;

        case 25:
            gPurpleDoom_3_679210->Remove_477B00(this->field_30_pNext);
            if (this->field_2C_counter)
            {
                field_30_pNext->set_id_lazy_4206C0(gPhi_8CA8_6FCF00->field_8CA4_def112_sprite_palette);
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
    field_8_speed_x = 0;
    field_C_speed_y = 0;
    field_10 = 0;
    field_40_pUnknown = 0;
    field_4_flags &= ~1;
}

MATCH_FUNC(0x53e2e0)
void Particle_4C::PoolDeallocate()
{
    if (field_30_pNext)
    {
        field_30_pNext->field_2C_flags &= ~4u;
        field_30_pNext->field_2C_flags = 0;
        gSprite_Pool_703818->remove(field_30_pNext);
        field_30_pNext = 0;
    }
}
