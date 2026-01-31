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

    inline s32 get_cur_rng_41CFE0()
    {
        return field_0_rng;
    }

    EXPORT void sub_48B900();
    EXPORT void ShowCycle_48B920();

    EXPORT void srand_4F7A40();
    EXPORT s16 get_int_4F7AE0(const s16& max_rnd);
    EXPORT u8 get_uint8_4F7B70(u8* max_rnd);
    EXPORT int rand_4F7C00();

    s32 field_0_rng;
    s32 field_4_rnd;
};

EXTERN_GLOBAL(rng*, rng_dword_67AB34);

EXTERN_GLOBAL(rng, stru_6F6784);
