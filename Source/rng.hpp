#pragma once

#include "Function.hpp"

class rng
{
  public:
    rng()
    {
        mRng = 0;
        mRnd = 0;
    }

    // FUNCTION: 96f 0x41cfe0
    inline s32 get_cur_rng_41CFE0()
    {
        return mRng;
    }

    // 9.6f 0x45F9E0
    inline s32 get_rnd_45F9E0()
    {
        return mRnd;
    }

    EXPORT void AdvanceCycle();
    EXPORT void ShowCycle();

    EXPORT void srand_4F7A40();
    EXPORT s16 get_int(const s16& max_rnd);
    EXPORT u8 get_uint8(const u8& max_rnd);
    EXPORT s32 rand_4F7C00();

    // Two uses, one per instance: on gpRng_67AB34 the game's tick counter (AdvanceCycle adds one per tick, ShowCycle
    // shows it), which timers are stamped with (CarPhysics::mDamagedUntilCycle, Ped::mLastVoiceRng); on
    // gRng_6F6784 the last number get_int / get_uint8 drew while bLog_random_67D5FC is set
    s32 mRng; // 0x0
    s32 mRnd; // 0x4
};

EXTERN_GLOBAL(rng*, gpRng_67AB34);

EXTERN_GLOBAL(rng, gRng_6F6784);
