#pragma once

#include "Function.hpp"
#include "Particle_4C.hpp"
#include "ang16.hpp"

class Fix16;
class Sprite;
class Sprite;

class Particle_8
{
  public:
    EXPORT void ParticlesService_53E320();
    EXPORT void EmitBloodBurst_53E450(Fix16 x, Fix16 y, Fix16 z, Ang16 ang);
    EXPORT void SpawnBlood_53E880(Fix16 xpos, Fix16 ypos, Fix16 zpos);
    EXPORT void GunMuzzelFlash_53E970(Sprite* pSprite);
    EXPORT void EmitWaterSplash_53F060(Fix16 xpos, Fix16 ypos, Fix16 zpos, Ang16 rotation, char_type a5);
    EXPORT void SpawnParticleSprite_5405D0(Sprite* pSprite);
    EXPORT void EmitElectricArcParticle(Fix16 xpos, Fix16 ypos, Fix16 zpos, Ang16 ang);
    EXPORT void SpawnCigaretteSmokePuff_5406B0(Sprite* pSprite, char_type a3);
    EXPORT Particle_8();
    EXPORT ~Particle_8();

    EXPORT void EmitFireTruckSprayParticle_53FAE0(Sprite* pSprite);
    EXPORT void EmitImpactParticles_53FE40(Fix16 x, Fix16 y, Fix16 z, Fix16 sinv, Fix16 cosv);
    EXPORT void EmitFlameStreamSegment_53F4C0(Sprite* pSprite);

    s32 field_0;
    s32 field_4;
};

EXTERN_GLOBAL(Particle_8*, gParticle_8_6FD5E8);
