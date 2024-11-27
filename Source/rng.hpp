#pragma once

#include "Function.hpp"

struct rng
{
    rng()
    {
        field_0_rng = 0;
        field_4_rnd = 0;
    }

    s32 field_0_rng;
    s32 field_4_rnd;
};

extern rng* rng_dword_67AB34;
