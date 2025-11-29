#pragma once

#include "Function.hpp"
#include "nostalgic_ellis_0x28.hpp"
#include "rng.hpp"
#include "Pool.hpp"

class LightBase
{
  public:
    LightBase()
    {

    }

    void sub_45BF50()
    {
        field_0_pool.UpdatePool();
    }

    void sub_45C1E0()
    {
        sub_45BF50();
    }

    Pool<nostalgic_ellis_0x28, 3000> field_0_pool;
};

class Light_1D4CC : public LightBase
{
  public:
    Light_1D4CC()
    {
        Light::sub_4D6E00();
    }

    EXPORT ~Light_1D4CC();
    EXPORT nostalgic_ellis_0x28* sub_5C2B70();
};

EXTERN_GLOBAL(Light_1D4CC*, gLight_1D4CC_6F5520);
