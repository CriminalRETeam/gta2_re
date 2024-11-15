#pragma once

#include "Function.hpp"
#include "types.hpp"

class Fix16;

class Particle_8
{
public:
    EXPORT void sub_53E320();
    EXPORT void sub_53E880(s32 a2, s32 a3, s32 a4);
    EXPORT void sub_53E970(Fix16* a2);
    EXPORT void sub_53F060(s32 a1, s32 a2, s32 a3, s16 a4, char_type a5);
    EXPORT void sub_5406B0(s32 a2, char_type a3);
    EXPORT Particle_8* ctor_5439D0();
    EXPORT void dtor_543A60();
    s32 field_0;
    s32 field_4;
};
