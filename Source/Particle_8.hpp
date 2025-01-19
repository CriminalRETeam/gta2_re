#pragma once

#include "Function.hpp"
#include "Particle_4C.hpp"
#include "ang16.hpp"

class Fix16;
class Car_3C;

class Particle_947C
{
  public:
    // inline 0x48F1E0
    Particle_947C()
    {
        Particle_4C* pIter = field_8;
        for (s32 i = 0; i < GTA2_COUNTOF(field_8) - 1; i++)
        {
            pIter->field_3C = pIter + 1;
            pIter++;
        }

        this->field_0 = field_8;
        this->field_8[GTA2_COUNTOF(field_8) - 1].field_3C = 0;
        this->field_4 = 0;
        this->field_9478 = 0;
    }

    // inline 0x48A8E0
    ~Particle_947C()
    {
        field_0 = 0;
        field_4 = 0;
    }

    Particle_4C* field_0;
    Particle_4C* field_4;
    Particle_4C field_8[500];
    s16 field_9478;
    s16 field_947A;
};

class Particle_8
{
  public:
    EXPORT void sub_53E320();
    EXPORT void sub_53E880(s32 a2, s32 a3, s32 a4);
    EXPORT void sub_53E970(Fix16* a2);
    EXPORT void sub_53F060(s32 a1, s32 a2, s32 a3, s16 a4, char_type a5);
    EXPORT void sub_5406B0(s32 a2, char_type a3);
    EXPORT Particle_8();
    EXPORT ~Particle_8();
    s32 field_0;
    s32 field_4;
};
