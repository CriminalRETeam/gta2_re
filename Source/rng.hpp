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

    EXPORT void sub_48B900();
    EXPORT void ShowCycle_48B920();

    EXPORT void srand_4F7A40();
    EXPORT s16 get_int_4F7AE0(s16* max_rnd);
    EXPORT u8 get_uint8_4F7B70(u8* max_rnd);
    EXPORT int rand_4F7C00();

    s32 field_0_rng;
    s32 field_4_rnd;
};

EXPORT_VAR extern rng* rng_dword_67AB34;
EXPORT_VAR extern rng stru_6F6784;
