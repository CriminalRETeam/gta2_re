#include "thirsty_lamarr.hpp"
#include "gtx_0x106C.hpp"
#include "sharp_pare_0x15D8.hpp"

STUB_FUNC(0x4920b0)
thirsty_lamarr::thirsty_lamarr()
{

}

STUB_FUNC(0x492110)
char_type thirsty_lamarr::sub_492110(s16 a2, s32 a3, s16 a4)
{
    return 0;
}

STUB_FUNC(0x492150)
void thirsty_lamarr::sub_492150()
{
}

STUB_FUNC(0x4921b0)
void thirsty_lamarr::sub_4921B0(s32 amount)
{
}

MATCH_FUNC(0x4921f0)
void thirsty_lamarr::sub_4921F0(s32 a2, s16 a3)
{
    u16 a;
    if (a2 == 2)
    {
        u16 new_sprite = gGtx_0x106C_703DD4->convert_sprite_pal_5AA460(6, field_36);
        a = gGtx_0x106C_703DD4->convert_pal_type_5AA5F0(2, new_sprite);
    }
    else
    {
        a = gGtx_0x106C_703DD4->convert_pal_type_5AA5F0(a2, a3);
    }

    s32 uVar4 = field_2E_idx;

    while (uVar4 < 9)
    {
        gSharp_pare_0x15D8_705064->sub_5B9660(field_34 - field_2E_idx + uVar4, a);
        uVar4++;
    }
}

STUB_FUNC(0x492260)
s32 thirsty_lamarr::sub_492260(s32 a2, s32 a3)
{
    return 0;
}

STUB_FUNC(0x492430)
s32 thirsty_lamarr::sub_492430(thirsty_lamarr* a1, s32 a2, s32 a3, s32 a4)
{
    return 0;
}

STUB_FUNC(0x4925c0)
char_type thirsty_lamarr::sub_4925C0()
{
    return 0;
}

STUB_FUNC(0x4925e0)
s16 thirsty_lamarr::sub_4925E0()
{
    return 0;
}
