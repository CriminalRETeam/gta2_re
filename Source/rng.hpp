#pragma once

#include "Function.hpp"

class rng
{
  public:
    rng()
    {
        field_0_rng = 0;
        field_4_rnd = 0;
    }

    // FUNCTION: 96f 0x41cfe0
    inline s32 get_cur_rng_41CFE0()
    {
        return field_0_rng;
    }

    // 9.6f 0x45F9E0
    inline s32 get_rnd_45F9E0()
    {
        return field_4_rnd;
    }

    EXPORT void AdvanceCycle_48B900();
    EXPORT void ShowCycle_48B920();

    EXPORT void srand_4F7A40();
    EXPORT s16 get_int_4F7AE0(const s16& max_rnd);
    EXPORT u8 get_uint8_4F7B70(const u8& max_rnd);
    EXPORT s32 rand_4F7C00();

    // Two uses, one per instance: on gpRng_67AB34 the game's tick counter (AdvanceCycle_48B900 adds one per tick, ShowCycle_48B920
    // shows it), which timers are stamped with (CarPhysics::field_8_damaged_until_cycle, Ped::field_220_last_voice_rng); on
    // gRng_6F6784 the last number get_int_4F7AE0 / get_uint8_4F7B70 drew while bLog_random_67D5FC is set
    s32 field_0_rng;
    s32 field_4_rnd;
};

EXTERN_GLOBAL(rng*, gpRng_67AB34);

EXTERN_GLOBAL(rng, gRng_6F6784);
