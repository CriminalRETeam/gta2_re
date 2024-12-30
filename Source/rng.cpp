#include "rng.hpp"
#include "Globals.hpp"
#include <stdlib.h>

EXPORT_VAR rng* rng_dword_67AB34;
GLOBAL(rng_dword_67AB34, 0x67AB34);

EXPORT_VAR rng stru_6F6784;
GLOBAL(stru_6F6784, 0x6F6784);

STUB_FUNC(0x4F7A40)
void rng::srand_4F7A40()
{
}

MATCH_FUNC(0x48B900)
void rng::sub_48B900()
{
    ++this->field_0_rng;
    this->field_4_rnd = stru_6F6784.rand_4F7C00();
}

MATCH_FUNC(0x4F7C00)
int rng::rand_4F7C00()
{
    return rand();
}
