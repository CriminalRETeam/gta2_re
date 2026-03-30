#include "Particle_8.hpp"
#include "Globals.hpp"
#include "Particle_4C.hpp"
#include "Pool.hpp"
#include "enums.hpp"
#include "error.hpp"
#include "sprite.hpp"
#include "Phi_8CA8.hpp"
#include "PurpleDoom.hpp"

typedef Pool<Particle_4C, 500> T_Particle_4C_Pool;

DEFINE_GLOBAL(T_Particle_4C_Pool*, gParticle_4C_Pool_6FD5E4, 0x6FD5E4);
DEFINE_GLOBAL(Particle_8*, gParticle_8_6FD5E8, 0x6FD5E8);

// NOTE: Will not match in marked extern !!
DEFINE_GLOBAL(u16, gParticleInstCount_6FD5F4, 0x6FD5F4);

MATCH_FUNC(0x53E3C0)
Particle_4C* Particle_8::New_53E3C0(Fix16 xpos, Fix16 ypos, Fix16 a4, Fix16 a5, Fix16 a6, Fix16 a7)
{
    Particle_4C* pNew4C = 0;
    if (gParticle_4C_Pool_6FD5E4->field_0_pStart && gSprite_Pool_703818->field_0_pool.field_0_pHead)
    {
        pNew4C = gParticle_4C_Pool_6FD5E4->Allocate();
        pNew4C->field_8_xpos = xpos;
        pNew4C->field_C_ypos = ypos;
        pNew4C->field_10 = a4;
        pNew4C->field_14 = a5;
        pNew4C->field_18 = a6;
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

STUB_FUNC(0x53E450)
void Particle_8::EmitBloodBurst_53E450(Fix16 x, Fix16 y, Fix16 z, Ang16 ang)
{
    NOT_IMPLEMENTED;
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

STUB_FUNC(0x5405D0)
void Particle_8::SpawnParticleSprite_5405D0(Sprite* pSprite)
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x540320)
void Particle_8::EmitElectricArcParticle(Fix16 xpos, Fix16 ypos, Fix16 zpos, Ang16 ang)
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x5406b0)
void Particle_8::SpawnCigaretteSmokePuff_5406B0(Sprite* a2, char_type a3)
{
    NOT_IMPLEMENTED;
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

STUB_FUNC(0x53FE40)
void Particle_8::EmitImpactParticles_53FE40(Fix16 x, Fix16 y, Fix16 z, Fix16 sinv, Fix16 cosv)
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x53F4C0)
void Particle_8::EmitFlameStreamSegment_53F4C0(Sprite* pSprite)
{
    NOT_IMPLEMENTED;
}