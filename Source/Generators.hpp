#pragma once

#include "Function.hpp"
#include "ang16.hpp"
#include "fix16.hpp"

class Object_2C;

class Generator_2C
{
  public:
    EXPORT void Service_4C1C50();
    EXPORT void sub_4C1C70(Fix16 a2, Fix16 a3, Fix16 a4, Ang16 a5, s32 a6, s16 a7, s16 a8);

    s32 field_0_gen_type;
    s32 field_4_x;
    s32 field_8_y;
    s32 field_C_z;
    s16 field_10;
    u16 field_12_min_delay;
    u16 field_14_max_delay;
    s16 field_16;
    s32 field_18_cycle;
    s16 field_1C_ammo;
    s16 field_1E;
    char_type field_20;
    char_type field_21;
    char_type field_22;
    char_type field_23;
    Object_2C* field_24_obj;
    s32 field_28;
};

class GeneratorPool_14AC
{
  public:
    EXPORT void sub_4C1CD0();
    EXPORT void GeneratorsService_4C1D70();
    EXPORT Generator_2C* CreateGenerator_4C1DC0(Fix16 a2, Fix16 a3, Fix16 a4, Ang16 a5, s32 a6, s16 a7, s16 a8);
    EXPORT GeneratorPool_14AC();
    Generator_2C field_0_pool[120];
    s32 field_14A0_count;
    s32 field_14A4;
    u8 field_14A8_count_d3;
    char_type field_14A9;
    char_type field_14AA;
    char_type field_14AB;
};

EXTERN_GLOBAL(GeneratorPool_14AC*, gGeneratorPool_14AC_67E5D0);
