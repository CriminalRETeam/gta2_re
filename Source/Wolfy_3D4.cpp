#include "Wolfy_3D4.hpp"
#include "Globals.hpp"

EXPORT_VAR s16 gParticleInstCount_6FD5F4;
GLOBAL(gParticleInstCount_6FD5F4, 0x6FD5F4);

STUB_FUNC(0x543690)
s32 Wolfy_7A8::sub_543690()
{
    return 0;
}

STUB_FUNC(0x543800)
Wolfy_30* Wolfy_7A8::sub_543800()
{
    return 0;
}

STUB_FUNC(0x5438b0)
Wolfy_7A8::Wolfy_7A8()
{
}

MATCH_FUNC(0x5438f0)
Wolfy_7A8::~Wolfy_7A8()
{
}

STUB_FUNC(0x5408f0)
Wolfy_30::Wolfy_30()
{
}

STUB_FUNC(0x540a10)
Wolfy_30::~Wolfy_30()
{
}

STUB_FUNC(0x543650)
s32 Wolfy_30::sub_543650()
{
    return 0;
}

// https://decomp.me/scratch/TAlwS
STUB_FUNC(0x543980)
Wolfy_3D4::Wolfy_3D4()
{
    for (s32 i = 0; i < GTA2_COUNTOF(field_3C0); i++)
    {
        field_0[i].field_4_idx = i;
        field_3C0[i] = 0;
    }
    gParticleInstCount_6FD5F4 = 0;
}

STUB_FUNC(0x5439c0)
Wolfy_3D4::~Wolfy_3D4()
{
}
