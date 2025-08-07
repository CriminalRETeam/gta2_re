#include "rng.hpp"
#include "Globals.hpp"
#include <stdlib.h>

// GLOBAL: 105 0x67AB34
DEFINE_GLOBAL(rng*, rng_dword_67AB34);
// GLOBAL: 105 0x6F6784
DEFINE_GLOBAL(rng, stru_6F6784);

// FUNCTION: 105 0x48B900
void rng::sub_48B900()
{
    ++field_0_rng;
    field_4_rnd = stru_6F6784.rand_4F7C00();
}

// STUB: 105 0x48B920
void rng::ShowCycle_48B920()
{
    NOT_IMPLEMENTED;
}

// FUNCTION: 105 0x4F7A40
void rng::srand_4F7A40()
{
    srand(1);
}

// STUB: 105 0x4F7AE0
s16 rng::get_int_4F7AE0(s16* max_rnd)
{
    NOT_IMPLEMENTED;
    return 0;
}

// STUB: 105 0x4F7B70
u8 rng::get_uint8_4F7B70(u8* max_rnd)
{
    NOT_IMPLEMENTED;
    return 0;
}

// FUNCTION: 105 0x4F7C00
int rng::rand_4F7C00()
{
    return rand();
}