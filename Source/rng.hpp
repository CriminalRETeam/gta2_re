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

    EXPORT void srand_4F7A40();

    s32 field_0_rng;
    s32 field_4_rnd;
};

EXPORT_VAR extern rng* rng_dword_67AB34;
EXPORT_VAR extern rng stru_6F6784;
