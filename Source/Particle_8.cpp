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
EXTERN_GLOBAL(Fix16, dword_6FD2EC);
EXTERN_GLOBAL(Fix16, dword_6FD554);

EXTERN_GLOBAL(Ang16, word_6FD5D4);
EXTERN_GLOBAL(Ang16, word_6FD3EE);

DEFINE_GLOBAL(T_Particle_4C_Pool*, gParticle_4C_Pool_6FD5E4, 0x6FD5E4);
DEFINE_GLOBAL(Particle_8*, gParticle_8_6FD5E8, 0x6FD5E8);
DEFINE_GLOBAL_INIT(Fix16, dword_6FD474, Fix16(0x47A, 0), 0x6FD474);
DEFINE_GLOBAL_INIT(Fix16, dword_6FD468, Fix16(0x147, 0), 0x6FD468);
DEFINE_GLOBAL_INIT(Ang16, dword_6FD314, Ang16(360), 0x6FD314);

DEFINE_GLOBAL_INIT(Fix16, dword_6FD4EC, Fix16(0xB, 0), 0x6FD4EC);
DEFINE_GLOBAL_INIT(Fix16, dword_6FD558, Fix16(50), 0x6FD558);
DEFINE_GLOBAL_INIT(Ang16, word_6FD5CC, Ang16(4), 0x6FD5CC);
DEFINE_GLOBAL_INIT(Fix16, dword_6FD508, Fix16(10), 0x6FD508);

DEFINE_GLOBAL_INIT(Fix16, dword_6FD2D4, dword_6FD554 * 14, 0x6FD2D4);
DEFINE_GLOBAL_INIT(Fix16, dword_6FD4CC, dword_6FD554 * (-14), 0x6FD4CC);
DEFINE_GLOBAL_INIT(Fix16, dword_6FD2D0, dword_6FD554 * 8, 0x6FD2D0);
DEFINE_GLOBAL_INIT(Fix16, dword_6FD48C, dword_6FD554 * 6, 0x6FD48C);
DEFINE_GLOBAL_INIT(Fix16, dword_6FD464, Fix16(0x1EB, 0), 0x6FD464);

// NOTE: Will not match in marked extern !!
DEFINE_GLOBAL(u16, gParticleInstCount_6FD5F4, 0x6FD5F4);

MATCH_FUNC(0x53E3C0)
Particle_4C* Particle_8::New_53E3C0(Fix16 speed_x, Fix16 speed_y, Fix16 a4, Fix16 additional_speed_x, Fix16 additional_speed_y, Fix16 a7)
{
    Particle_4C* pNew4C = 0;
    if (gParticle_4C_Pool_6FD5E4->field_0_pStart && gSprite_Pool_703818->field_0_pool.field_0_pHead)
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
        vector.y = Fix16(stru_6F6784.get_int_4F7AE0(50)) * dword_6FD548;
        vector.RotateByAngle_40F6B0(ang);

        for (u8 i = 0; i < 6; i++)
        {
            vector.x = Fix16(0);
            vector.y = (Fix16(stru_6F6784.get_int_4F7AE0(100)) + dword_6FD558) * dword_6FD4EC;

            vector.RotateByAngle_40F6B0((word_6FD5CC.sub_401CB0(Fix16(stru_6F6784.get_int_4F7AE0(16))) + ang) -
                                        word_6FD5CC.sub_401CB0(Fix16(8)));

            Fix16 x_dir = vector.x / 15;
            Fix16 y_dir = vector.y / 15;

            Particle_4C* pBloodParticle = gParticle_8_6FD5E8->New_53E3C0(vector.x, vector.y, dword_6FD330, -x_dir, -y_dir, 0);

            if (pBloodParticle)
            {
                pBloodParticle->field_34 = 1;
                pBloodParticle->field_38_state = 1;
                pBloodParticle->field_2C_counter = 15;
                pBloodParticle->field_2E = 15;

                pBloodParticle->field_30_pNext->SetType_4206F0(8);
                pBloodParticle->field_30_pNext->set_id_lazy_4206C0(gPhi_8CA8_6FCF00->field_8CA4 + 16);
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
        pNew4C->field_30_pNext->set_id_lazy_4206C0(gPhi_8CA8_6FCF00->field_8CA4 + 191);
        pNew4C->field_30_pNext->set_xyz_lazy_420600(xpos, ypos, zpos);
        pNew4C->field_30_pNext->set_num_40F7B0(2);
        gPurpleDoom_3_679210->AddToSingleBucket_477AE0(pNew4C->field_30_pNext);
    }
}

STUB_FUNC(0x53e970)
void Particle_8::GunMuzzelFlash_53E970(Sprite* a2)
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x53f060)
void Particle_8::EmitWaterSplash_53F060(Fix16 xpos, Fix16 ypos, Fix16 zpos, Ang16 rotation, char_type a5)
{
    NOT_IMPLEMENTED;
}

MATCH_FUNC(0x5405D0)
void Particle_8::SpawnParticleSprite_5405D0(Sprite* pSprite)
{
    if (gParticle_4C_Pool_6FD5E4->field_0_pStart)
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
            pNew4C->field_30_pNext->set_id_lazy_4206C0(gPhi_8CA8_6FCF00->field_8CA4 + 164);
            pNew4C->field_30_pNext->Set_2C_0x4_Flag_4337F0();
            pNew4C->field_30_pNext->set_xyz_lazy_420600(pSprite->field_14_xy.x, pSprite->field_14_xy.y, pSprite->field_1C_zpos);

            gPurpleDoom_3_679210->AddToSingleBucket_477AE0(pNew4C->field_30_pNext);
        }
    }
}

STUB_FUNC(0x540320)
void Particle_8::EmitElectricArcParticle(Fix16 xpos, Fix16 ypos, Fix16 zpos, Ang16 ang)
{
    NOT_IMPLEMENTED;
}

// 9.6f 0x48E060
WIP_FUNC(0x5406b0)
void Particle_8::SpawnCigaretteSmokePuff_5406B0(Sprite* pSprite, char_type bUnknown)
{
    WIP_IMPLEMENTED;

    if (!bSkip_particles_67D64D)
    {
        Fix16 zero(0);
        Particle_4C* pNew4C = gParticle_8_6FD5E8->New_53E3C0(zero, zero, dword_6FD330, 0, 0, 0);
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
            pNew4C->field_30_pNext->set_id_lazy_4206C0(gPhi_8CA8_6FCF00->field_8CA4 + 3);

            //v11 = pSprite->field_0 * 4;

            if (bUnknown)
            {
                Ang16::PolarToCartesian_41FC20(pSprite->field_0, dword_6FD474, stru_6FD388, stru_6FD38C);
            }
            else
            {
                Ang16::PolarToCartesian_41FC20(pSprite->field_0, dword_6FD46C, stru_6FD388, stru_6FD38C);
            }

            Fix16 v16;
            Fix16 v17;
            Ang16::PolarToCartesian_41FC20(pSprite->field_0 - dword_6FD314, dword_6FD468, v16, v17);

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
    field_0 = 0;
    field_4 = 0;
}

MATCH_FUNC(0x543a60)
Particle_8::~Particle_8()
{
    if (gParticle_4C_Pool_6FD5E4)
    {
        GTA2_DELETE_AND_NULL(gParticle_4C_Pool_6FD5E4);
    }
    field_0 = 0;
    field_4 = 0;
}

STUB_FUNC(0x53FAE0)
void Particle_8::EmitFireTruckSprayParticle_53FAE0(Sprite* pSprite)
{
    NOT_IMPLEMENTED;
}

// https://decomp.me/scratch/wfzEd
WIP_FUNC(0x53FE40)
void Particle_8::EmitImpactParticles_53FE40(Fix16 x, Fix16 y, Fix16 z, Fix16 sinv, Fix16 cosv)
{
    WIP_IMPLEMENTED;

    Ang16 ang1;
    Ang16 ang2;
    Fix16_Point t(Fix16(0), Fix16(0));
    Ang16 tanAng = Fix16::atan2_fixed_405320(cosv, sinv);

    //Fix16 k15_1(15, 0);
    //Fix16 k15_2(15, 0);

    for (u32 i = 0; i < 6; ++i)
    {
        t.x = Fix16(0);
        t.y = (dword_6FD4EC * (dword_6FD558 + Fix16(stru_6F6784.get_int_4F7AE0(100))));
        if (i < 4)
        {
            ang1 = word_6FD5CC.sub_401CB0(stru_6F6784.get_int_4F7AE0(32));
            ang2 = word_6FD5CC.sub_401CB0(Fix16(16));
        }
        else
        {
            ang1 = word_6FD5CC.sub_401CB0(stru_6F6784.get_int_4F7AE0(360));
            ang2 = word_6FD5CC.sub_401CB0(Fix16(180));
        }

        ang1 = ang1 + tanAng - ang2;
        t.RotateByAngle_40F6B0(ang1);

        Particle_4C* pNew4C = gParticle_8_6FD5E8->New_53E3C0(t.x, t.y, dword_6FD330, 0, 0, Fix16(0)); // TODO
        if (pNew4C)
        {
            pNew4C->field_34 = 1;
            pNew4C->field_38_state = 7;
            pNew4C->field_2C_counter = 7;
            pNew4C->field_2E = 7;
            pNew4C->field_30_pNext->SetType_4206F0(8);
            pNew4C->field_30_pNext->set_id_lazy_4206C0(gPhi_8CA8_6FCF00->field_8CA4 + 127);
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
        if (!field_0)
        {
            field_0 = gObject_5C_6F8F84->NewPhysicsObj_5299B0(objects::fire_hitting_194, 0, 0, 0, word_6FD5D4);
        }
        Particle_4C* pParticle = gParticle_8_6FD5E8->New_53E3C0(Fix16(0), Fix16(0), dword_6FD330, 0, 0, Fix16(0));
        if (pParticle)
        {
            pParticle->field_4_flags |= 1;
            pParticle->field_30_pNext->SetType_4206F0(8);
            pParticle->field_30_pNext->set_id_lazy_4206C0(gPhi_8CA8_6FCF00->field_8CA4 + 73);
            pParticle->field_30_pNext->AllocInternal_59F950(dword_6FD554 * dword_6FD508, dword_6FD554 * dword_6FD508, dword_6FD2EC);
            pParticle->field_34 = 0;
            pParticle->field_38_state = 31;
            Fix16 vec_x;
            Fix16 vec_y;
            Ang16::PolarToCartesian_41FC20(pSprt->field_0, dword_6FD2E8, vec_x, vec_y);
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
                    angle = pSprt18->field_0->field_0 + word_6FD3EE;
                    vector.SetXY_432860(Fix16(0), dword_6FD2D4);
                    vector.RotateByAngle_40F6B0(angle);
                    zero = Fix16(0);
                    unknown = dword_6FD2D0;
                }
                else
                {
                    Sprite_18* pSprt18_2 = pSprt->field_8_car_bc_ptr->field_0_qq.GetSpriteForModel_5A6A50(248);
                    angle = pSprt18_2->field_0->field_0;
                    vector.SetXY_432860(Fix16(0), dword_6FD48C);
                    vector.RotateByAngle_40F6B0(angle);
                    zero = Fix16(0);
                    unknown = dword_6FD4CC;
                }
                vector_2.SetXY_432860(zero, unknown);
                pParticle->field_30_pNext->set_ang_lazy_420690(angle);
                vector_2.RotateByAngle_40F6B0(pSprt->field_0);
                vector += vector_2 + pSprt->get_x_y_443580();
                pSprt->field_8_car_bc_ptr->field_58_physics->GetPointVelocity_561350(&vector); // not used?
            }
            else
            {
                vector_2.x = -dword_6FD464;
                vector_2.y = dword_6FD468 + dword_6FD2E8;
                vector_2.RotateByAngle_40F6B0(pSprt->field_0);
                vector = vector + *(Fix16_Point*)&pSprt->field_8_char_b4_ptr->field_98;
                pParticle->field_30_pNext->set_ang_lazy_420690(pSprt->field_0);
                vector.x = pSprt->field_14_xy.x + vector.x;
                vector.y = pSprt->field_14_xy.y + vector.y;
            }
            pParticle->field_30_pNext->set_xyz_lazy_420600(vector.x, vector.y, zpos);
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