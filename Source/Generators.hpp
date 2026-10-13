#pragma once

#include "Function.hpp"
#include "ang16.hpp"
#include "fix16.hpp"

class Object_2C;

class Generator_2C
{
  public:
    EXPORT void Activate_4C1A70();
    EXPORT s32 next_cycle_4C1AB0();
    EXPORT void SpawnObject_4C1B10();
    EXPORT void Service_4C1C50();
    EXPORT void Init_4C1C70(Fix16 x, Fix16 y, Fix16 z, Ang16 rot, s32 generator_type, s16 min_delay, s16 max_delay);

    s32 field_0_gen_type;
    Fix16 field_4_x;
    Fix16 field_8_y;
    Fix16 field_C_z;
    Ang16 field_10_rot;
    u16 field_12_min_delay;
    u16 field_14_max_delay;
    s16 field_16_unused;
    u32 field_18_cycle;
    u16 field_1C_ammo;
    u16 field_1E_kill_timer;
    char_type field_20;
    char_type field_21;
    char_type field_22_unused;
    char_type field_23_unused;
    Object_2C* field_24_obj;
    s32 field_28_id;
};

class GeneratorPool_14AC
{
  public:
    EXPORT void sub_4C1CD0();
    EXPORT void GeneratorsService_4C1D70();
    EXPORT Generator_2C* CreateGenerator_4C1DC0(Fix16 x, Fix16 y, Fix16 z, Ang16 rot, s32 type, s16 min_delay, s16 max_delay);
    EXPORT GeneratorPool_14AC();
    Generator_2C field_0_pool[120];
    s32 field_14A0_count;
    s32 field_14A4_kill_count;
    u8 field_14A8_count_d3;
    char_type field_14A9_unused;
    char_type field_14AA_unused;
    char_type field_14AB_unused;
};

EXTERN_GLOBAL(GeneratorPool_14AC*, gGeneratorPool_14AC_67E5D0);
