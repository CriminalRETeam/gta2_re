#include "thirsty_lamarr.hpp"
#include "gtx_0x106C.hpp"
#include "sharp_pare_0x15D8.hpp"

MATCH_FUNC(0x4920b0)
thirsty_lamarr::thirsty_lamarr()
{
    for (s32 iVar2 = 0; iVar2 < 9; iVar2++)
    {
        field_13[iVar2] = 0;
        field_9[iVar2] = 0x30;
        field_1D_buf[iVar2] = 0x30;
    }

    field_12 = 0;
    field_0 = 0;
    field_4 = -1;
    field_8 = 0;
    field_27_sprite_w = -1;
    field_28_sprite_h_calc = -1;
    field_2A = 0;
    field_2C = 0;
    field_2E_idx = 0;
    field_30 = 0;
    field_34 = 0;
    field_36 = 0;
}

STUB_FUNC(0x492110)
char_type thirsty_lamarr::sub_492110(s16 a2, s32 a3, s16 a4)
{
    return 0;
}

MATCH_FUNC(0x492150)
void thirsty_lamarr::sub_492150()
{
    u16 sprite_pal = gGtx_0x106C_703DD4->convert_sprite_pal_5AA460(6, field_36);
    sprite_index* sprite_index = gGtx_0x106C_703DD4->get_sprite_index_5AA440(sprite_pal);
    field_27_sprite_w = sprite_index->field_4_width;
    field_28_sprite_h_calc = sprite_index->field_5_height / 11;
    field_34 = gSharp_pare_0x15D8_705064->sub_5B9220(field_2A, field_36);
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

MATCH_FUNC(0x4925c0)
char_type thirsty_lamarr::sub_4925C0()
{
    s32 uVar1 = field_2E_idx;

    while (uVar1 < 9)
    {
        if (this->field_13[uVar1] != 0)
        {
            return 1;
        }
        uVar1++;
    }
    return 0;
}

STUB_FUNC(0x4925e0)
s16 thirsty_lamarr::sub_4925E0()
{
    return 0;
}
