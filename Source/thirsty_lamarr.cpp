#include "thirsty_lamarr.hpp"
#include "gtx_0x106C.hpp"
#include "sharp_pare_0x15D8.hpp"

MATCH_FUNC(0x4f7660)
s32 __stdcall sub_4F7660(s32 &a1)
{
    s32 iVar1 = 1;
    s32 iVar2 = a1;

    if (iVar2 < 0)
    {
        iVar2 = -iVar2;
    }
    for (; 10 <= iVar2; iVar2 /= 10, iVar1++)
    {}
    return iVar1;
}

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

MATCH_FUNC(0x492110)
void thirsty_lamarr::sub_492110(s16 a1, s32 a2, s16 a3)
{
    field_36 = a3;
    field_2C = a1;
    field_30 = a2;
    field_2A = sub_4F7660(field_30);
    s8 tmp = 9;
    tmp -= static_cast<s8>(field_2A);
    field_2E_idx = tmp;
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

MATCH_FUNC(0x4921b0)
void thirsty_lamarr::ChangeStatByAmount_4921B0(s32 amount)
{
    if (amount > 0)
    {
        if(amount >= field_30)
        {
            field_0 = field_30;
        }
        else
        {
            field_0 += amount;
            if (field_0 > field_30 )
            {
                field_0 = field_30;
            }
        }
    }
    else
    {
        if(amount > -field_30)
        {
            field_0 += amount;
            if(field_0 < 0 )
            {
                field_0 = 0;
            }
        }
        else
        {
            field_0 = 0;
        }
    }
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
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x492430)
s32 thirsty_lamarr::sub_492430(s32 a3, s32 a4)
{
    NOT_IMPLEMENTED;
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

MATCH_FUNC(0x4925e0)
void thirsty_lamarr::sub_4925E0()
{
    s32 v11;
    sscanf((const char_type*)&field_9, "%d", &v11);

    if (field_4 == -1 || v11 == field_4 && !thirsty_lamarr::sub_4925C0())
    {
        if (field_0 == v11)
        {
            field_4 = -1;
            return;
        }
        field_4 = field_0;
        if (field_0 > v11)
        {
            field_8 = true;
        }
        else
        {
            field_8 = false;
        }
        sprintf((char_type*)&field_1D_buf, "%09d", field_0);
    }

    for (s32 idx = field_2E_idx; idx < 9; idx++)
    {
        if (field_9[idx + 20] != field_9[idx] || field_13[idx])
        {
            if (field_8)
            {
                field_13[idx] = field_2C + field_13[idx];
                if (field_13[idx] >= field_28_sprite_h_calc)
                {
                    if (field_9[idx] < 57)
                    {
                        field_9[idx]++;
                    }
                    else
                    {
                        field_9[idx] = 48;
                    }
                    field_13[idx] = 0;
                }
            }
            else
            {
                field_13[idx] = field_13[idx] - field_2C;
                if (field_13[idx] < 0)
                {
                    if (field_9[idx] > 48)
                    {
                        field_9[idx]--;
                    }
                    else
                    {
                        field_9[idx] = 57;
                    }
                    field_13[idx] = field_28_sprite_h_calc - field_2C;
                }
            }
        }
    }
}