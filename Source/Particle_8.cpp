#include "Particle_8.hpp"
#include "Car_BC.hpp"
#include "CarPhysics_B0.hpp"
#include "Globals.hpp"
#include "Particle_4C.hpp"
#include "Phi_8CA8.hpp"
#include "Pool.hpp"
#include "PurpleDoom.hpp"
#include "debug.hpp"
#include "enums.hpp"
#include "error.hpp"
#include "Object_5C.hpp"
#include "sprite.hpp"
#include "rng.hpp"


EXTERN_GLOBAL(Fix16, dword_6FD46C);
//EXTERN_GLOBAL(Fix16_Point, stru_6FD388);
EXTERN_GLOBAL(Fix16, stru_6FD388);
EXTERN_GLOBAL(Fix16, stru_6FD38C);
EXTERN_GLOBAL(Fix16, dword_6FD548);
EXTERN_GLOBAL(Fix16, dword_6FD330);
EXTERN_GLOBAL(Fix16, dword_6FD448);
EXTERN_GLOBAL(Fix16, dword_6FD2E8);
EXTERN_GLOBAL(Fix16, kFP16Quarter_6FD2EC);
EXTERN_GLOBAL(Fix16, dword_6FD554);
EXTERN_GLOBAL(Fix16, dword_6FD3C0);
EXTERN_GLOBAL(Fix16, dword_6FD5A8);

EXTERN_GLOBAL(Ang16, kAngZero_6FD5D4);
EXTERN_GLOBAL(Ang16, kAng180_6FD3EE);

DEFINE_GLOBAL(T_Particle_4C_Pool*, gParticle_4C_Pool_6FD5E4, 0x6FD5E4);
DEFINE_GLOBAL(Particle_8*, gParticle_8_6FD5E8, 0x6FD5E8);
DEFINE_GLOBAL_INIT(Fix16, dword_6FD474, Fix16(0x47A, 0), 0x6FD474);
DEFINE_GLOBAL_INIT(Fix16, dword_6FD468, Fix16(0x147, 0), 0x6FD468);
DEFINE_GLOBAL_INIT(Ang16, kAng90_6FD314, Ang16(360), 0x6FD314);

DEFINE_GLOBAL_INIT(Fix16, dword_6FD4EC, Fix16(0xB, 0), 0x6FD4EC);
DEFINE_GLOBAL_INIT(Fix16, dword_6FD558, Fix16(50), 0x6FD558);
DEFINE_GLOBAL_INIT(Ang16, word_6FD5CC, Ang16(4), 0x6FD5CC);
DEFINE_GLOBAL_INIT(Fix16, dword_6FD508, Fix16(10), 0x6FD508);

DEFINE_GLOBAL_INIT(Fix16, dword_6FD2D4, Fix16(0xE00, 0), 0x6FD2D4);
DEFINE_GLOBAL_INIT(Fix16, dword_6FD4CC, Fix16(0xFFFFEE00, 0), 0x6FD4CC);
DEFINE_GLOBAL_INIT(Fix16, kFP16Eighth_6FD2D0, Fix16(0x800, 0), 0x6FD2D0);
DEFINE_GLOBAL_INIT(Fix16, dword_6FD48C, Fix16(0x600, 0), 0x6FD48C);
DEFINE_GLOBAL_INIT(Fix16, dword_6FD464, Fix16(0x1EB, 0), 0x6FD464);

// NOTE: Will not match in marked extern !!
DEFINE_GLOBAL(u16, gParticleInstCount_6FD5F4, 0x6FD5F4);

MATCH_FUNC(0x53E3C0)
Particle_4C* Particle_8::New_53E3C0(Fix16 speed_x, Fix16 speed_y, Fix16 a4, Fix16 additional_speed_x, Fix16 additional_speed_y, Fix16 a7)
{
    Particle_4C* pNew4C = 0;
    if (gParticle_4C_Pool_6FD5E4->has_pStart_48A8F0() && gSprite_Pool_703818->has_free_48A8D0())
    {
        pNew4C = gParticle_4C_Pool_6FD5E4->Allocate();
        pNew4C->field_8_speed_x = speed_x;
        pNew4C->field_C_speed_y = speed_y;
        pNew4C->field_10 = a4;
        pNew4C->field_14_additional_speed_x = additional_speed_x;
        pNew4C->field_18_additional_speed_y = additional_speed_y;
        pNew4C->field_1C = a7;
        pNew4C->field_0_id = gParticleInstCount_6FD5F4;
        pNew4C->field_30_pNext = gSprite_Pool_703818->get_new_sprite();
        ++gParticleInstCount_6FD5F4;
    }
    return pNew4C;
}

MATCH_FUNC(0x53e320)
void Particle_8::ParticlesService_53E320()
{
    gParticle_4C_Pool_6FD5E4->UpdatePool();
}

// https://decomp.me/scratch/ohbD0
WIP_FUNC(0x53E450)
void Particle_8::EmitBloodBurst_53E450(Fix16 x, Fix16 y, Fix16 z, Ang16 ang)
{
    WIP_IMPLEMENTED;
    Ang16 angle;
    Fix16_Point vector(Fix16(0), Fix16(0));

    if (!bSkip_particles_67D64D)
    {
        vector.x = Fix16(0);
        vector.y = Fix16(gRng_6F6784.get_int_4F7AE0(50) + 25) * dword_6FD548;
        vector.RotateByAngle_OOL_40F6B0(ang);

        for (u8 i = 0; i < 6; i++)
        {
            vector.x = Fix16(0);
            vector.y = (Fix16(gRng_6F6784.get_int_4F7AE0(100)) + dword_6FD558) * dword_6FD4EC;

            angle = word_6FD5CC.MultiplyByFix16_401CB0(Fix16(gRng_6F6784.get_int_4F7AE0(16)));
            vector.RotateByAngle_NegOOL_40F6B0((angle + ang) - word_6FD5CC.MultiplyByFix16_401CB0(Fix16(8)));

            Particle_4C* pBloodParticle =
                gParticle_8_6FD5E8->New_53E3C0(vector.x, vector.y, dword_6FD330, (vector.x / 15).Negate_4086A0(), (vector.y / 15).Negate_4086A0(), 0);

            if (pBloodParticle)
            {
                pBloodParticle->field_34 = 1;
                pBloodParticle->field_38_state = 1;
                pBloodParticle->field_2C_counter = 15;
                pBloodParticle->field_2E = 15;

                pBloodParticle->field_30_pNext->SetType_4206F0(8);
                pBloodParticle->field_30_pNext->set_id_lazy_4206C0(gPhi_8CA8_6FCF00->field_8CA4_def112_sprite_palette + 16);
                pBloodParticle->field_30_pNext->set_xyz_lazy_420600(x, y, z);
                gPurpleDoom_3_679210->AddToSingleBucket_477AE0(pBloodParticle->field_30_pNext);
            }
        }
    }
}

// 9.6f 0x48CC50
MATCH_FUNC(0x53e880)
void Particle_8::SpawnBlood_53E880(Fix16 xpos, Fix16 ypos, Fix16 zpos)
{
    Particle_4C* pNew4C = gParticle_8_6FD5E8->New_53E3C0(xpos, ypos, 0, 0, 0, 0);
    if (pNew4C)
    {
        pNew4C->field_34 = 1;
        pNew4C->field_38_state = 39;
        pNew4C->field_2C_counter = 800;
        pNew4C->field_46_sub_state = 0;
        pNew4C->field_48_timer = 3;
        pNew4C->field_2E = 800;
        pNew4C->field_30_pNext->SetType_4206F0(8);
        pNew4C->field_30_pNext->set_id_lazy_4206C0(gPhi_8CA8_6FCF00->field_8CA4_def112_sprite_palette + 191);
        pNew4C->field_30_pNext->set_xyz_lazy_420600(xpos, ypos, zpos);
        pNew4C->field_30_pNext->set_num_40F7B0(2);
        gPurpleDoom_3_679210->AddToSingleBucket_477AE0(pNew4C->field_30_pNext);
    }
}

WIP_FUNC(0x53e970)
void Particle_8::GunMuzzelFlash_53E970(Sprite* a2)
{
    Fix16_Point vel(Fix16(0), Fix16(0));
    if (bSkip_particles_67D64D)
    {
        return;
    }

    Particle_4C* pParticle;
    if (a2->get_type_416B40() == sprite_types_enum::car_2)
    {
        Car_BC* pCar = a2->field_8_car_bc_ptr;
        pParticle = gParticle_8_6FD5E8->New_53E3C0(vel.x, vel.y, dword_6FD330, 0, 0, 0);
        if (pParticle)
        {
            pParticle->field_4_flags |= 1;
            pParticle->field_30_pNext->SetType_4206F0(8);
            pParticle->field_30_pNext->set_id_lazy_4206C0(gPhi_8CA8_6FCF00->field_8CA4_def112_sprite_palette + 197);
            pParticle->field_34 = 0;
            pParticle->field_38_state = 40;
            // Results unused (as in the ped branch, which uses them). Past the inline budget, so the
            // two multiplies stay as out-of-line Multiply_408680 calls; written as `x = sin * r`
            // they were inlined and removed.
            Fix16 dx;
            Fix16 dy;
            Ang16::PolarToCartesian_41FC20(a2->field_0, dword_6FD2E8, dx, dy);
            pParticle->field_46_sub_state = 0;
            pParticle->field_48_timer = 0;

            Fix16 w = pCar->get_car_width() / 2 + dword_6FD3C0;
            Fix16 h = pCar->get_car_height() / 2 + dword_6FD5A8;
            Fix16_Point offset;
            offset.SetXY_432860(w, h);
            offset.RotateByAngle_40F6B0(a2->field_0);
            offset += a2->get_x_y_443580();

            pParticle->field_28_pSprite = a2;
            pParticle->field_30_pNext->set_ang_lazy_420690(a2->field_0);
            pParticle->field_30_pNext->set_xyz_lazy_420600(offset.x, offset.y, a2->field_1C_zpos);
            gPurpleDoom_3_679210->AddToSingleBucket_477AE0(pParticle->field_30_pNext);
            pParticle->field_30_pNext->field_2C_flags |= 4;
        }

        pParticle = gParticle_8_6FD5E8->New_53E3C0(vel.x, vel.y, dword_6FD330, 0, 0, 0);
        if (!pParticle)
        {
            return;
        }
        pParticle->field_4_flags |= 1;
        pParticle->field_30_pNext->SetType_4206F0(8);
        pParticle->field_30_pNext->set_id_lazy_4206C0(gPhi_8CA8_6FCF00->field_8CA4_def112_sprite_palette + 197);
        pParticle->field_34 = 0;
        pParticle->field_38_state = 41;
        Fix16 dx;
        Fix16 dy;
        Ang16::PolarToCartesian_41FC20(a2->field_0, dword_6FD2E8, dx, dy);
        pParticle->field_46_sub_state = 0;
        pParticle->field_48_timer = 0;

        Fix16 w = -(pCar->get_car_width() / 2 + dword_6FD3C0);
        Fix16 h = pCar->get_car_height() / 2 + dword_6FD5A8;
        Fix16_Point offset;
        offset.SetXY_432860(w, h);
        offset.RotateByAngle_40F6B0(a2->field_0);
        offset += a2->get_x_y_443580();

        pParticle->field_30_pNext->set_ang_lazy_420690(a2->field_0);
        pParticle->field_30_pNext->set_xyz_lazy_420600(offset.x, offset.y, a2->field_1C_zpos);
        pParticle->field_28_pSprite = a2;
    }
    else
    {
        pParticle = gParticle_8_6FD5E8->New_53E3C0(vel.x, vel.y, dword_6FD330, 0, 0, 0);
        if (!pParticle)
        {
            return;
        }
        pParticle->field_4_flags |= 1;
        pParticle->field_30_pNext->SetType_4206F0(8);
        pParticle->field_30_pNext->set_id_lazy_4206C0(gPhi_8CA8_6FCF00->field_8CA4_def112_sprite_palette + 197);
        pParticle->field_34 = 0;
        pParticle->field_38_state = 40;
        Fix16 dx;
        Fix16 dy;
        Ang16::PolarToCartesian_41FC20(a2->field_0, dword_6FD2E8, dx, dy);
        pParticle->field_46_sub_state = 0;
        pParticle->field_48_timer = 0;
        stru_6FD388 = a2->field_14_xy.x + dx;
        stru_6FD38C = a2->field_14_xy.y + dy;
        Fix16 zpos = a2->field_1C_zpos;

        Char_B4* pB4 = a2->AsCharB4_40FEA0();
        Fix16_Point offset;
        offset.x = -dword_6FD464;
        offset.y = dword_6FD468 + dword_6FD2E8;
        offset.RotateByAngle_40F6B0(a2->field_0);
        offset = offset + pB4->field_98_velocity_vector;

        pParticle->field_30_pNext->set_ang_lazy_420690(a2->field_0);
        pParticle->field_30_pNext->set_xyz_lazy_420600(a2->field_14_xy.x + offset.x, a2->field_14_xy.y + offset.y, zpos);
        pParticle->field_28_pSprite = a2;
    }
    gPurpleDoom_3_679210->AddToSingleBucket_477AE0(pParticle->field_30_pNext);
    // 9.6f: Sprite::Set_2C_0x4_Flag_4337F0 here and above (inlined, using it in both places makes the diff worse)
    pParticle->field_30_pNext->field_2C_flags |= 4;
}

// Something wrong with the velocities https://decomp.me/scratch/2Kz9I
WIP_FUNC(0x53f060)
void Particle_8::EmitWaterSplash_53F060(Fix16 xpos, Fix16 ypos, Fix16 zpos, Ang16 rotation, char_type bRandomRot)
{
    WIP_IMPLEMENTED;
    Ang16 angle_1;
    Ang16 angle_2;
    Fix16_Point velocity(Fix16(0), Fix16(0));

    if (!bSkip_particles_67D64D)
    {
        velocity.x = Fix16(0);
        velocity.y = Fix16(gRng_6F6784.get_int_4F7AE0(50) + 25) * dword_6FD548;
        velocity.RotateByAngle_40F6B0_out_of_line(rotation);

        for (u8 i = 0; i < 6; i++)
        {
            if (bRandomRot)
            {
                angle_2 = Ang16(Fix16(word_6FD5CC.rValue).Multiply_408680(Fix16(gRng_6F6784.get_int_4F7AE0(360))), 0);
            }
            else
            {
                angle_2 = rotation;
            }

            velocity.x = Fix16(0);
            velocity.y = (Fix16(gRng_6F6784.get_int_4F7AE0(100)) + dword_6FD558) * dword_6FD4EC;

            // 9.6f: MultiplyByFix16_401CB0 (inlined). Here the original inlines the multiply and calls
            // the Ang16 constructor out of line (FromFix16_4516B0); the rotation uses angle_2
            {
                Fix16 spread = Fix16(word_6FD5CC.rValue) * Fix16(gRng_6F6784.get_int_4F7AE0(16));
                angle_1 = Ang16(&spread, 0);
                Fix16 half = Fix16(word_6FD5CC.rValue) * Fix16(8);
                velocity.RotateByAngle_NegOOL_40F6B0((angle_1 + angle_2) - Ang16(&half, 0));
            }

            Particle_4C* pWaterSplashParticle = gParticle_8_6FD5E8->New_53E3C0(velocity.x,
                                                                               velocity.y,
                                                                               dword_6FD330,
                                                                               (velocity.x / 15).Negate_4086A0(),
                                                                               (velocity.y / 15).Negate_4086A0(),
                                                                               0);

            if (pWaterSplashParticle)
            {
                pWaterSplashParticle->field_34 = 0;
                pWaterSplashParticle->field_38_state = 35;
                pWaterSplashParticle->field_2C_counter = 15;
                pWaterSplashParticle->field_2E = 15;

                pWaterSplashParticle->field_30_pNext->SetType_4206F0(8);
                pWaterSplashParticle->field_30_pNext->set_id_lazy_4206C0(gPhi_8CA8_6FCF00->field_8CA4_def112_sprite_palette + 132);
                pWaterSplashParticle->field_46_sub_state = 0;
                pWaterSplashParticle->field_48_timer = 6;
                pWaterSplashParticle->field_30_pNext->set_xyz_lazy_420600(xpos, ypos, zpos);
                gPurpleDoom_3_679210->AddToSingleBucket_477AE0(pWaterSplashParticle->field_30_pNext);
            }
        }
    }
}

MATCH_FUNC(0x5405D0)
void Particle_8::SpawnParticleSprite_5405D0(Sprite* pSprite)
{
    if (gParticle_4C_Pool_6FD5E4->has_pStart_48A8F0())
    {
        Particle_4C* pNew4C = gParticle_4C_Pool_6FD5E4->Allocate();
        if (pNew4C)
        {
            pNew4C->field_28_pSprite = pSprite;
            pNew4C->field_34 = 0;
            pNew4C->field_46_sub_state = 0;

            pNew4C->field_30_pNext = gSprite_Pool_703818->get_new_sprite();
            pNew4C->field_30_pNext->SetType_4206F0(8);
            pNew4C->field_38_state = 38;
            pNew4C->field_30_pNext->set_id_lazy_4206C0(gPhi_8CA8_6FCF00->field_8CA4_def112_sprite_palette + 164);
            pNew4C->field_30_pNext->Set_2C_0x4_Flag_4337F0();
            pNew4C->field_30_pNext->set_xyz_lazy_420600(pSprite->field_14_xy.x, pSprite->field_14_xy.y, pSprite->field_1C_zpos);

            gPurpleDoom_3_679210->AddToSingleBucket_477AE0(pNew4C->field_30_pNext);
        }
    }
}

DEFINE_GLOBAL_INIT(Fix16, dword_6FD500, Fix16(0x1, 0), 0x6FD500);

WIP_FUNC(0x540320)
void Particle_8::EmitElectricArcParticle(Fix16 xpos, Fix16 ypos, Fix16 zpos, Ang16 ang)
{
    Ang16 angle;
    Fix16_Point vector(Fix16(0), Fix16(0));

    if (!bSkip_particles_67D64D)
    {
        gRng_6F6784.get_int_4F7AE0(3);
        vector.x = Fix16(0);
        vector.y = (Fix16(gRng_6F6784.get_int_4F7AE0(100)) + dword_6FD558) * dword_6FD500;

        angle = word_6FD5CC.MultiplyByFix16_401CB0(Fix16(gRng_6F6784.get_int_4F7AE0(360)));
        // RotateByAngle_40F6B0, but x * cos, the y line and both sums use the out-of-line Fix16 operators
        {
            Fix16 sin = Ang16::sine_40F500(angle);
            Fix16 cos = Ang16::cosine_40F520(angle);
            Fix16 x_old = vector.x;
            vector.x = (const Fix16&)vector.x.Multiply_408680(cos) + (vector.y * sin);
            vector.y = (const Fix16&)x_old.Negate_4086A0().Multiply_408680(sin) + vector.y.Multiply_408680(cos);
        }

        Particle_4C* pNew4C = gParticle_8_6FD5E8->New_53E3C0(vector.x, vector.y, dword_6FD330, 0, 0, 0);
        if (pNew4C)
        {
            pNew4C->field_34 = 0;
            pNew4C->field_38_state = 37;
            pNew4C->field_46_sub_state = 0;
            pNew4C->field_2E = pNew4C->field_2C_counter;
            pNew4C->field_30_pNext->SetType_4206F0(8);
            pNew4C->field_30_pNext->Set_2C_0x4_Flag_4337F0();
            pNew4C->field_30_pNext->set_xyz_lazy_420600(xpos, ypos, zpos);
            pNew4C->field_30_pNext->set_ang_lazy_420690(ang);
            pNew4C->field_30_pNext->set_id_lazy_4206C0(gRng_6F6784.get_int_4F7AE0(4) + gPhi_8CA8_6FCF00->field_8CA4_def112_sprite_palette + 175);
            pNew4C->field_30_pNext->field_2C_flags = 0xA2;
            pNew4C->field_30_pNext->Set_2C_0x4_Flag_4337F0();
            gPurpleDoom_3_679210->AddToSingleBucket_477AE0(pNew4C->field_30_pNext);
        }
    }
}

// Ang16::PolarToCartesian_41FC20 with the cosine multiply written as the out-of-line Multiply_408680
// the original calls (past the inline budget VC6 would call a COMDAT copy of operator* instead).
static inline void PolarToCartesian_OutOfLineCos(Ang16& angle, Fix16& radius, Fix16& ret1, Fix16& ret2)
{
    ret1 = Ang16::sine_40F500(angle) * radius;
    ret2 = Ang16::cosine_40F520(angle).Multiply_408680(radius);
}

// 9.6f 0x48E060. Fix16_Point vel has a dtor, which gives the EH frame; its fields are zeroed again
// before the call, and the two other zero args go through the out-of-line Fix16(s32) ctor.
MATCH_FUNC(0x5406b0)
void Particle_8::SpawnCigaretteSmokePuff_5406B0(Sprite* pSprite, char_type bUnknown)
{
    Fix16_Point vel(Fix16(0), Fix16(0));
    if (!bSkip_particles_67D64D)
    {
        vel.x = Fix16(0);
        vel.y = Fix16(0);
        Particle_4C* pNew4C = gParticle_8_6FD5E8->New_53E3C0(vel.x, vel.y, dword_6FD330, 0, 0, 0);
        if (pNew4C)
        {
            pNew4C->field_34 = 1;

            if (bUnknown)
            {
                pNew4C->field_38_state = 9;
                pNew4C->field_2C_counter = 80;
            }
            else
            {
                pNew4C->field_38_state = 10;
                pNew4C->field_2C_counter = 70;
            }

            pNew4C->field_2E = pNew4C->field_2C_counter;
            pNew4C->field_30_pNext->SetType_4206F0(8);
            pNew4C->field_30_pNext->set_id_lazy_4206C0(gPhi_8CA8_6FCF00->field_8CA4_def112_sprite_palette + 3);

            Fix16 x;
            Fix16 y;
            if (bUnknown)
            {
                PolarToCartesian_OutOfLineCos(pSprite->field_0, dword_6FD474, x, y);
            }
            else
            {
                PolarToCartesian_OutOfLineCos(pSprite->field_0, dword_6FD46C, x, y);
            }
            stru_6FD388 = x;
            stru_6FD38C = y;

            Fix16 v16;
            Fix16 v17;
            PolarToCartesian_OutOfLineCos(Ang16((s32)(pSprite->field_0.rValue - kAng90_6FD314.rValue)).Normalized_406C20(), dword_6FD468, v16, v17);

            stru_6FD388 += v16 + pSprite->field_14_xy.x;
            stru_6FD38C += v17 + pSprite->field_14_xy.y;

            pNew4C->field_30_pNext->set_xyz_lazy_420600(stru_6FD388, stru_6FD38C, pSprite->field_1C_zpos);
            pNew4C->field_28_pSprite = pSprite;

            gPurpleDoom_3_679210->AddToSingleBucket_477AE0(pNew4C->field_30_pNext);

            if (bUnknown)
            {
                pNew4C->field_30_pNext->Set_2C_0x4_Flag_4337F0();
            }
        }
    }
}

MATCH_FUNC(0x5439d0)
Particle_8::Particle_8()
{
    if (!gParticle_4C_Pool_6FD5E4)
    {
        gParticle_4C_Pool_6FD5E4 = new T_Particle_4C_Pool();
        if (!gParticle_4C_Pool_6FD5E4)
        {
            FatalError_4A38C0(Gta2Error::OutOfMemoryNewOperator, "C:\\Splitting\\Gta2\\Source\\particle.cpp", 4167);
        }
    }
    field_0_fire_hit_obj = 0;
    field_4 = 0;
}

MATCH_FUNC(0x543a60)
Particle_8::~Particle_8()
{
    if (gParticle_4C_Pool_6FD5E4)
    {
        GTA2_DELETE_AND_NULL(gParticle_4C_Pool_6FD5E4);
    }
    field_0_fire_hit_obj = 0;
    field_4 = 0;
}

MATCH_FUNC(0x53FAE0)
void Particle_8::EmitFireTruckSprayParticle_53FAE0(Sprite* pSprite)
{
    Ang16 angle;
    Fix16_Point vector(Fix16(0), Fix16(0));
    if (!bSkip_particles_67D64D)
    {
        if (!field_4)
        {
            field_4 = gObject_5C_6F8F84->NewPhysicsObj_5299B0(objects::maybe_bullet_on_fire_198, 0, 0, 0, kAngZero_6FD5D4);
        }
        vector.x = Fix16(0);
        vector.y = Fix16(0);
        Particle_4C* pParticle = gParticle_8_6FD5E8->New_53E3C0(vector.x, vector.y, dword_6FD330, 0, 0, 0);
        if (pParticle)
        {
            pParticle->field_4_flags |= 1;
            pParticle->field_30_pNext->SetType_4206F0(8);
            pParticle->field_30_pNext->set_id_lazy_4206C0(gPhi_8CA8_6FCF00->field_8CA4_def112_sprite_palette + 111);
            pParticle->field_30_pNext->AllocInternal_59F950(dword_6FD554 * dword_6FD508, dword_6FD554 * dword_6FD508, kFP16Quarter_6FD2EC);
            pParticle->field_34 = 0;
            pParticle->field_38_state = 34;
            Fix16 vec_x;
            Fix16 vec_y;
            Ang16::PolarToCartesian_41FC20(pSprite->field_0, dword_6FD2E8, vec_x, vec_y);
            pParticle->field_2C_counter = 100;
            pParticle->field_46_sub_state = 0;
            pParticle->field_48_timer = 0;
            stru_6FD388 = pSprite->field_14_xy.x + vec_x;
            stru_6FD38C = pSprite->field_14_xy.y + vec_y;
            Fix16 zpos = pSprite->field_1C_zpos;
            if (pSprite->field_30_sprite_type_enum == sprite_types_enum::car_2)
            {
                Car_BC* pCar = pSprite->field_8_car_bc_ptr;
                Sprite_18* pGun = pCar->field_0_qq.GetSpriteForModel_5A6A50(114);
                if (pGun)
                {
                    angle = pGun->field_0->field_0 + kAng180_6FD3EE;
                }
                else
                {
                    angle = pCar->field_0_qq.GetSpriteForModel_5A6A50(248)->field_0->field_0;
                }
                pParticle->field_30_pNext->set_ang_lazy_420690(angle);
            }
            else
            {
                pParticle->field_30_pNext->set_ang_lazy_420690(pSprite->field_0);
            }
            pParticle->field_30_pNext->set_xyz_lazy_420600(stru_6FD388, stru_6FD38C, zpos);
            pParticle->field_28_pSprite = pSprite;
            if (pParticle->field_30_pNext->CheckSpriteMovementRegion_5A2500())
            {
                pParticle->field_2C_counter = 0;
            }
            gPurpleDoom_3_679210->AddToSingleBucket_477AE0(pParticle->field_30_pNext);
            pParticle->field_30_pNext->field_2C_flags |= 4;
        }
    }
}

// https://decomp.me/scratch/wfzEd
WIP_FUNC(0x53FE40)
void Particle_8::EmitImpactParticles_53FE40(Fix16 x, Fix16 y, Fix16 z, Fix16 sinv, Fix16 cosv)
{
    WIP_IMPLEMENTED;

    Fix16_Point t(Fix16(0), Fix16(0));
    Ang16 tanAng = Fix16::atan2_fixed_405320(cosv, sinv);

    for (u8 i = 0; i < 6; ++i)
    {
        t.x = Fix16(0);
        t.y = (dword_6FD4EC * (dword_6FD558 + Fix16(gRng_6F6784.get_int_4F7AE0(100))));
        if (i < 4)
        {
            Ang16 ang1 = word_6FD5CC.MultiplyByFix16_401CB0_out_of_line(Fix16(gRng_6F6784.get_int_4F7AE0(32)));
            Ang16 ang2 = word_6FD5CC.MultiplyByFix16_401CB0_out_of_line(Fix16(16));
            Ang16 sum = ang1 + tanAng;
            Ang16 rot(Ang16(sum.rValue - ang2.rValue), 0);
            t.RotateByAngle_40F6B0_all_out_of_line(rot);
        }
        else
        {
            Ang16 ang1 = word_6FD5CC.MultiplyByFix16_401CB0_out_of_line(Fix16(gRng_6F6784.get_int_4F7AE0(360)));
            Ang16 ang2 = word_6FD5CC.MultiplyByFix16_401CB0_out_of_line(Fix16(180));
            Ang16 sum(Ang16(ang1.rValue + tanAng.rValue), 0);
            Ang16 rot(Ang16(sum.rValue - ang2.rValue), 0);
            t.RotateByAngle_40F6B0_all_out_of_line(rot);
        }

        Particle_4C* pNew4C = gParticle_8_6FD5E8->New_53E3C0(t.x, t.y, dword_6FD330, t.x.DivideInt_53E860(15).Negate_4086A0(), t.y.DivideInt_53E860(15).Negate_4086A0(), Fix16(0));
        if (pNew4C)
        {
            pNew4C->field_34 = 1;
            pNew4C->field_38_state = 7;
            pNew4C->field_2C_counter = 7;
            pNew4C->field_2E = 7;
            pNew4C->field_30_pNext->SetType_4206F0(8);
            pNew4C->field_30_pNext->set_id_lazy_4206C0(gPhi_8CA8_6FCF00->field_8CA4_def112_sprite_palette + 127);
            pNew4C->field_30_pNext->set_xyz_lazy_420600(x, y, z);
            pNew4C->field_30_pNext->Set_2C_0x4_Flag_4337F0();

            gPurpleDoom_3_679210->AddToSingleBucket_477AE0(pNew4C->field_30_pNext);
        }
    }
}

// https://decomp.me/scratch/3Ars7
WIP_FUNC(0x53F4C0)
void Particle_8::EmitFlameStreamSegment_53F4C0(Sprite* pSprt)
{
    WIP_IMPLEMENTED;
    Ang16 angle;
    Fix16_Point vector(Fix16(0), Fix16(0));
    Fix16_Point vector_2;
    Fix16 zero;
    Fix16 unknown;
    if (!bSkip_particles_67D64D)
    {
        if (!field_0_fire_hit_obj)
        {
            field_0_fire_hit_obj = gObject_5C_6F8F84->NewPhysicsObj_5299B0(objects::fire_hitting_194, 0, 0, 0, kAngZero_6FD5D4);
        }
        vector.x = Fix16(0);
        vector.y = Fix16(0);
        Particle_4C* pParticle = gParticle_8_6FD5E8->New_53E3C0(vector.x, vector.y, dword_6FD330, 0, 0, 0);
        if (pParticle)
        {
            pParticle->field_4_flags |= 1;
            pParticle->field_30_pNext->SetType_4206F0(8);
            pParticle->field_30_pNext->set_id_lazy_4206C0(gPhi_8CA8_6FCF00->field_8CA4_def112_sprite_palette + 73);
            pParticle->field_30_pNext->AllocInternal_59F950(dword_6FD554 * dword_6FD508, dword_6FD554 * dword_6FD508, kFP16Quarter_6FD2EC);
            pParticle->field_34 = 0;
            pParticle->field_38_state = 31;
            Fix16 vec_x;
            Fix16 vec_y;
            // Ang16::PolarToCartesian_41FC20 with the second multiply out of line
            vec_x = Ang16::sine_40F500(pSprt->field_0) * dword_6FD2E8;
            vec_y = Ang16::cosine_40F520(pSprt->field_0).Multiply_408680(dword_6FD2E8);
            pParticle->field_2C_counter = 100;
            pParticle->field_46_sub_state = 0;
            pParticle->field_48_timer = 0;
            stru_6FD388 = vec_x + pParticle->field_30_pNext->field_14_xy.x;
            stru_6FD38C = vec_y + pParticle->field_30_pNext->field_14_xy.y;
            Fix16 zpos = pSprt->field_1C_zpos;
            if (pSprt->get_type_416B40() == sprite_types_enum::car_2)
            {
                Sprite_18* pSprt18 = pSprt->field_8_car_bc_ptr->field_0_qq.GetSpriteForModel_5A6A50(114);
                if (pSprt18)
                {
                    angle = pSprt18->field_0->field_0 + kAng180_6FD3EE;
                    vector.SetXY_432860(Fix16(0), dword_6FD2D4);
                    vector.RotateByAngle_MixOOL_40F6B0(angle);
                    zero = Fix16(0);
                    unknown = kFP16Eighth_6FD2D0;
                }
                else
                {
                    Sprite_18* pSprt18_2 = pSprt->field_8_car_bc_ptr->field_0_qq.GetSpriteForModel_5A6A50(248);
                    angle = pSprt18_2->field_0->field_0;
                    vector.SetXY_432860(Fix16(0), dword_6FD48C);
                    vector.RotateByAngle_MixOOL_40F6B0(angle);
                    zero = Fix16(0);
                    unknown = dword_6FD4CC;
                }
                vector_2.SetXY_432860(zero, unknown);
                pParticle->field_30_pNext->set_ang_lazy_420690(angle);
                vector_2.RotateByAngle_MixOOL_40F6B0(pSprt->field_0);
                vector += vector_2.Add_40AC50(pSprt->get_x_y_443580());
                pSprt->field_8_car_bc_ptr->field_58_physics->GetPointVelocity_561350(&vector); // not used?
                pParticle->field_30_pNext->set_xyz_lazy_420600(vector.x, vector.y, zpos);
            }
            else
            {
                vector_2.x = -dword_6FD464;
                vector_2.y = dword_6FD468 + dword_6FD2E8;
                vector_2.RotateByAngle_MixOOL_40F6B0(pSprt->field_0);
                vector = vector_2.Add_40AC50(*(Fix16_Point*)&pSprt->field_8_char_b4_ptr->field_98_velocity_vector);
                pParticle->field_30_pNext->set_ang_lazy_420690(pSprt->field_0);
                pParticle->field_30_pNext->set_xyz_lazy_420600(pSprt->field_14_xy.x + vector.x, pSprt->field_14_xy.y + vector.y, zpos);
            }
            pParticle->field_28_pSprite = pSprt;
            if (pParticle->field_30_pNext->CheckSpriteMovementRegion_5A2500())
            {
                pParticle->field_2C_counter = 0;
            }
            gPurpleDoom_3_679210->AddToSingleBucket_477AE0(pParticle->field_28_pSprite);
            pParticle->field_30_pNext->field_2C_flags |= 4;
        }
    }
}