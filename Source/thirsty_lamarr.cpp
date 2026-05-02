#include "thirsty_lamarr.hpp"
#include "Draw.hpp"
#include "gtx_0x106C.hpp"
#include "sharp_pare_0x15D8.hpp"

DEFINE_GLOBAL_INIT(Ang16, word_67B210, Ang16(0), 0x67B210);

MATCH_FUNC(0x4f7660)
s32 __stdcall GetMaxNumOfDigits_4F7660(s32 &max_value)
{
    s32 iVar1 = 1;
    s32 iVar2 = max_value;

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
    field_0_value = 0;
    field_4 = -1;
    field_8 = 0;
    field_27_sprite_w = -1;
    field_28_sprite_h_calc = -1;
    field_2A_max_num_of_digits = 0;
    field_2C_digit_transition_speed = 0;
    field_2E_non_used_digits = 0;
    field_30_max_value = 0;
    field_34_first_digit_texture_idx = 0;
    field_36_palette = 0;
}

MATCH_FUNC(0x492110)
void thirsty_lamarr::SetupDigitsParams_492110(s16 digit_transition_speed, s32 max_value, s16 palette)
{
    field_36_palette = palette;
    field_2C_digit_transition_speed = digit_transition_speed;
    field_30_max_value = max_value;
    field_2A_max_num_of_digits = GetMaxNumOfDigits_4F7660(field_30_max_value);
    s8 tmp = 9;
    tmp -= static_cast<s8>(field_2A_max_num_of_digits);
    field_2E_non_used_digits = tmp;
}

MATCH_FUNC(0x492150)
void thirsty_lamarr::sub_492150()
{
    u16 sprite_pal = gGtx_0x106C_703DD4->convert_sprite_pal_5AA460(6, field_36_palette);
    sprite_index* sprite_index = gGtx_0x106C_703DD4->get_sprite_index_5AA440(sprite_pal);
    field_27_sprite_w = sprite_index->field_4_width;
    field_28_sprite_h_calc = sprite_index->field_5_height / 11;
    field_34_first_digit_texture_idx = gSharp_pare_0x15D8_705064->RegisterDigits_5B9220(field_2A_max_num_of_digits, field_36_palette);
}

MATCH_FUNC(0x4921b0)
void thirsty_lamarr::ChangeStatByAmount_4921B0(s32 amount)
{
    if (amount > 0)
    {
        if(amount >= field_30_max_value)
        {
            field_0_value = field_30_max_value;
        }
        else
        {
            field_0_value += amount;
            if (field_0_value > field_30_max_value )
            {
                field_0_value = field_30_max_value;
            }
        }
    }
    else
    {
        if(amount > -field_30_max_value)
        {
            field_0_value += amount;
            if(field_0_value < 0 )
            {
                field_0_value = 0;
            }
        }
        else
        {
            field_0_value = 0;
        }
    }
}

MATCH_FUNC(0x4921f0)
void thirsty_lamarr::sub_4921F0(s32 a2, s16 a3)
{
    u16 a;
    if (a2 == 2)
    {
        u16 new_sprite = gGtx_0x106C_703DD4->convert_sprite_pal_5AA460(6, field_36_palette);
        a = gGtx_0x106C_703DD4->convert_pal_type_5AA5F0(2, new_sprite);
    }
    else
    {
        a = gGtx_0x106C_703DD4->convert_pal_type_5AA5F0(a2, a3);
    }

    s32 uVar4 = field_2E_non_used_digits;

    while (uVar4 < 9)
    {
        gSharp_pare_0x15D8_705064->SetPal_5B9660(field_34_first_digit_texture_idx - field_2E_non_used_digits + uVar4, a);
        uVar4++;
    }
}

// https://decomp.me/scratch/6E5vt
WIP_FUNC(0x492260)
s32 thirsty_lamarr::sub_492260(s32 a2, s32 a3)
{
    WIP_IMPLEMENTED;
    u8 height;

    s32 v4 = a3;
    s16 curr_idx = field_2E_non_used_digits;
    s32 v7 = a3 + (field_28_sprite_h_calc >> 1);
    bool bUnk = true;
    s32 v37 = v7;

    s16 v8 = 9 - curr_idx;
    s16 v36 = 9 - curr_idx;
    for (; curr_idx < 9; curr_idx++, v8--, v36--)
    {
        s32 v9 = field_13[curr_idx];
        if (bUnk)
        {
            char v32 = field_9[curr_idx];
            if (v32 == 0x30) // 56
            {

                if (curr_idx == 8 || field_13[curr_idx] != 0)
                {
                    if (curr_idx == 8)
                    {
                        height = field_28_sprite_h_calc;
                    }
                    else
                    {
                        height = field_13[curr_idx];
                    }
                }
                else
                {
                    continue;
                }
            }
            else
            {
                height = field_28_sprite_h_calc;
            }

            u16 unknown = field_28_sprite_h_calc * (58 - v32) - v9;
            s32 xpos = a2 + (field_27_sprite_w >> 1) - field_27_sprite_w * v8;
            sub_495470(gSharp_pare_0x15D8_705064->sub_5B95F0(curr_idx + field_34_first_digit_texture_idx - field_2E_non_used_digits, unknown, height),
                       xpos,
                       a3 + height / 2,
                       field_27_sprite_w,
                       height,
                       word_67B210,
                       0,
                       0);
            bUnk = false;
        }
        else
        {
            s16 v15 = 58 - field_9[curr_idx];
            u16 v16 = field_28_sprite_h_calc * v15 - v9;
            
            sub_495470(gSharp_pare_0x15D8_705064->sub_5B95F0(curr_idx + field_34_first_digit_texture_idx - field_2E_non_used_digits, v16, field_28_sprite_h_calc),
                       a2 + ((field_27_sprite_w >> 1) - (field_27_sprite_w * v8)),
                       v37,
                       field_27_sprite_w,
                       field_28_sprite_h_calc,
                       word_67B210,
                       0,
                       0);
        }
    }
    return a2 - (field_27_sprite_w >> 1);
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
    s32 uVar1 = field_2E_non_used_digits;

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
        if (field_0_value == v11)
        {
            field_4 = -1;
            return;
        }
        field_4 = field_0_value;
        if (field_0_value > v11)
        {
            field_8 = true;
        }
        else
        {
            field_8 = false;
        }
        sprintf((char_type*)&field_1D_buf, "%09d", field_0_value);
    }

    for (s32 idx = field_2E_non_used_digits; idx < 9; idx++)
    {
        if (field_9[idx + 20] != field_9[idx] || field_13[idx])
        {
            if (field_8)
            {
                field_13[idx] = field_2C_digit_transition_speed + field_13[idx];
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
                field_13[idx] = field_13[idx] - field_2C_digit_transition_speed;
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
                    field_13[idx] = field_28_sprite_h_calc - field_2C_digit_transition_speed;
                }
            }
        }
    }
}