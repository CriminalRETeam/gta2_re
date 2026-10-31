#pragma once

#include "Function.hpp"
#include "ang16.hpp"
#include "fix16.hpp"

class Object;

// Generator::mSpawnMode
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
    EXPORT void Activate();
    EXPORT s32 next_cycle() const;
    EXPORT void SpawnObject();
    EXPORT void Service();
    EXPORT void Init(Fix16 x, Fix16 y, Fix16 z, Ang16 rot, s32 generator_type, s16 min_delay, s16 max_delay);

    s32 mGenType; // 0x0
    Fix16 mX; // 0x4
    Fix16 mY; // 0x8
    Fix16 mZ; // 0xC
    Ang16 mRot; // 0x10
    u16 mMinDelay; // 0x12
    u16 mMaxDelay; // 0x14
    s16 mUnused16; // 0x16
    u32 mCycle; // 0x18
    u16 mAmmo; // 0x1C
    u16 mSpawnsLeft; // 0x1E
    char_type mUnused20; // 0x20
    char_type mSpawnMode; // 0x21
    char_type mUnused22; // 0x22
    char_type mUnused23; // 0x23
    Object* mObj; // 0x24
    s32 mId; // 0x28
};
GTA2_ASSERT_SIZEOF_ALWAYS(Generator, 0x2C);

class GeneratorPool
{
  public:
    EXPORT void FinalizeAfterScriptLoad();
    EXPORT void GeneratorsService();
    EXPORT Generator* CreateGenerator(Fix16 x, Fix16 y, Fix16 z, Ang16 rot, s32 type, s16 min_delay, s16 max_delay);
    EXPORT GeneratorPool();
    Generator mPool[120]; // 0x0
    s32 mCount; // 0x14A0
    s32 mKillCount; // 0x14A4
    u8 mCountD3; // 0x14A8
    char_type mUnused14A9; // 0x14A9
    char_type mUnused14AA; // 0x14AA
    char_type mUnused14AB; // 0x14AB
};
GTA2_ASSERT_SIZEOF_ALWAYS(GeneratorPool, 0x14AC);

EXTERN_GLOBAL(GeneratorPool*, gGeneratorPool_14AC);
