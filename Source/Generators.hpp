#pragma once

#include "Function.hpp"
#include "ang16.hpp"
#include "fix16.hpp"

class Object;

// Generator::field_21_spawn_mode
namespace generator_spawn_mode
{
enum
{
    every_cycle_1 = 1,
    one_at_a_time_2 = 2,
};
} // namespace generator_spawn_mode

class Generator
{
  public:
    EXPORT void Activate_4C1A70();
    EXPORT s32 next_cycle_4C1AB0() const;
    EXPORT void SpawnObject_4C1B10();
    EXPORT void Service_4C1C50();
    EXPORT void Init_4C1C70(Fix16 x, Fix16 y, Fix16 z, Ang16 rot, s32 generator_type, s16 min_delay, s16 max_delay);

    s32 field_0_gen_type; // 0x0
    Fix16 field_4_x; // 0x4
    Fix16 field_8_y; // 0x8
    Fix16 field_C_z; // 0xC
    Ang16 field_10_rot; // 0x10
    u16 field_12_min_delay; // 0x12
    u16 field_14_max_delay; // 0x14
    s16 field_16_unused; // 0x16
    u32 field_18_cycle; // 0x18
    u16 field_1C_ammo; // 0x1C
    u16 field_1E_spawns_left; // 0x1E
    char_type field_20_unused; // 0x20
    char_type field_21_spawn_mode; // 0x21
    char_type field_22_unused; // 0x22
    char_type field_23_unused; // 0x23
    Object* field_24_obj; // 0x24
    s32 field_28_id; // 0x28
};
GTA2_ASSERT_SIZEOF_ALWAYS(Generator, 0x2C);

class GeneratorPool
{
  public:
    EXPORT void FinalizeAfterScriptLoad_4C1CD0();
    EXPORT void GeneratorsService_4C1D70();
    EXPORT Generator* CreateGenerator_4C1DC0(Fix16 x, Fix16 y, Fix16 z, Ang16 rot, s32 type, s16 min_delay, s16 max_delay);
    EXPORT GeneratorPool();
    Generator field_0_pool[120]; // 0x0
    s32 field_14A0_count; // 0x14A0
    s32 field_14A4_kill_count; // 0x14A4
    u8 field_14A8_count_d3; // 0x14A8
    char_type field_14A9_unused; // 0x14A9
    char_type field_14AA_unused; // 0x14AA
    char_type field_14AB_unused; // 0x14AB
};
GTA2_ASSERT_SIZEOF_ALWAYS(GeneratorPool, 0x14AC);

EXTERN_GLOBAL(GeneratorPool*, gGeneratorPool_14AC_67E5D0);
