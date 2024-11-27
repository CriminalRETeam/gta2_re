#pragma once

#include "Function.hpp"

struct rng
{
    rng()
    {
        field_0_rng = 0;
        field_4_rnd = 0;
    }

    void srand_4F7A40();

    s32 field_0_rng;
    s32 field_4_rnd;
};

extern rng* rng_dword_67AB34;
extern rng stru_6F6784;
