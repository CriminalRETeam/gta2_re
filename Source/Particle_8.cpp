#include "Particle_8.hpp"
#include "Globals.hpp"
#include "Pool.hpp"
#include "enums.hpp"
#include "error.hpp"

typedef Pool<Particle_4C, 500> T_Particle_4C_Pool;

DEFINE_GLOBAL(T_Particle_4C_Pool*, gParticle_4C_Pool_6FD5E4, 0x6FD5E4);
DEFINE_GLOBAL(Particle_8*, gParticle_8_6FD5E8, 0x6FD5E8);

STUB_FUNC(0x53e320)
void Particle_8::ParticlesService_53E320()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x53e880)
void Particle_8::SpawnBlood_53E880(Fix16 a2, Fix16 a3, Fix16 a4)
{
    NOT_IMPLEMENTED;
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