#pragma once

#include "Function.hpp"
#include "Particle_4C.hpp"
#include "ang16.hpp"
#include "Fix16_Point.hpp"
#include "Pool.hpp"

class Fix16;
class Sprite;
class Particle_4C;
class Object;

class ParticleManager
{
  public:
    EXPORT Particle_4C* New(Fix16 speed_x, Fix16 speed_y, Fix16 speed_z, Fix16 additional_speed_x, Fix16 additional_speed_y, Fix16 additional_speed_z);

    EXPORT void ParticlesService();
    EXPORT void EmitBloodBurst(Fix16 x, Fix16 y, Fix16 z, Ang16 ang);
    EXPORT void SpawnBlood(Fix16 xpos, Fix16 ypos, Fix16 zpos);
    EXPORT void GunMuzzelFlash(Sprite* pSprite);
    EXPORT void EmitWaterSplash(Fix16 xpos, Fix16 ypos, Fix16 zpos, Ang16 rotation, bool bRandomRot);
    EXPORT void SpawnParticleSprite(Sprite* pSprite);
    EXPORT void EmitElectricArcParticle(Fix16 xpos, Fix16 ypos, Fix16 zpos, Ang16 ang);
    EXPORT void SpawnCigaretteSmokePuff(Sprite* pSprite, char_type bSmokeSource);
    EXPORT ParticleManager();
    EXPORT ~ParticleManager();

    EXPORT void EmitFireTruckSprayParticle(Sprite* pSprite);
    EXPORT void EmitImpactParticles(Fix16 x, Fix16 y, Fix16 z, Fix16_Point dir);
    EXPORT void EmitFlameStreamSegment(Sprite* pSprite);

    Object* mFireHitObj; // 0x0
    Object* mWaterHitObj; // 0x4
};
GTA2_ASSERT_SIZEOF_ALWAYS(ParticleManager, 0x8);

EXTERN_GLOBAL(ParticleManager*, gParticle_8);

typedef Pool<Particle_4C, 500> T_Particle_4C_Pool;

EXTERN_GLOBAL(T_Particle_4C_Pool*, gParticle_4C_Pool);
