#pragma once

#include "Function.hpp"
#include "Particle_4C.hpp"
#include "ang16.hpp"

class Fix16;
class Sprite;

class Particle_8
{
  public:
    EXPORT void ParticlesService_53E320();
    EXPORT void sub_53E880(s32 a2, s32 a3, s32 a4);
    EXPORT void sub_53E970(Fix16* a2);
    EXPORT void sub_53F060(Fix16 xpos, Fix16 ypos, Fix16 zpos, Ang16 rotation, char_type a5);
    EXPORT void sub_5406B0(s32 a2, char_type a3);
    EXPORT Particle_8();
    EXPORT ~Particle_8();
    s32 field_0;
    s32 field_4;
};

EXTERN_GLOBAL(Particle_8*, gParticle_8_6FD5E8);
