#include "RollingDigitCounter_38.hpp"
#include "Draw.hpp"
#include "gtx_0x106C.hpp"
#include "sharp_pare_0x15D8.hpp"

// Forward declarations: the functions below are in address order
s32 __stdcall GetMaxNumOfDigits_4F7660(s32 &max_value);

DEFINE_GLOBAL_INIT(Ang16, kAngZero_67B210, Ang16(0), 0x67B210);

MATCH_FUNC(0x4920b0)
RollingDigitCounter_38::RollingDigitCounter_38()
{
    for (s32 digit_idx = 0; digit_idx < kNumDigits; digit_idx++)
    {
        field_13_scroll_offsets[digit_idx] = 0;
        field_9_shown_digits[digit_idx] = '0';
        field_1D_target_digits[digit_idx] = '0';
    }

    field_9_shown_digits[kNumDigits] = 0;
    field_0_value = 0;
    field_4_target_value = -1;
    field_8_bRollingUp = 0;
    field_27_sprite_w = -1;
    field_28_sprite_h_calc = -1;
    field_2A_max_num_of_digits = 0;
    field_2C_digit_transition_speed = 0;
    field_2E_non_used_digits = 0;
    field_30_max_value = 0;
    field_34_first_digit_texture_idx = 0;
    field_36_sprite_idx = 0;
}

MATCH_FUNC(0x492110)
void RollingDigitCounter_38::SetupDigitsParams_492110(s16 digit_transition_speed, s32 max_value, s16 palette)
{
    field_36_sprite_idx = palette;
    field_2C_digit_transition_speed = digit_transition_speed;
    field_30_max_value = max_value;
    field_2A_max_num_of_digits = GetMaxNumOfDigits_4F7660(field_30_max_value);
    s8 non_used_digits = kNumDigits;
    non_used_digits -= static_cast<s8>(field_2A_max_num_of_digits);
    field_2E_non_used_digits = non_used_digits;
}

MATCH_FUNC(0x492150)
void RollingDigitCounter_38::InitDigitSprites_492150()
{
    u16 true_sprite_idx = gGtx_0x106C_703DD4->GetSpriteTrueIndex_5AA460(sprite_types_enum::user_6, field_36_sprite_idx);
    sprite_index* pSpriteIndex = gGtx_0x106C_703DD4->get_sprite_index_5AA440(true_sprite_idx);
    field_27_sprite_w = pSpriteIndex->field_4_width;
    field_28_sprite_h_calc = pSpriteIndex->field_5_height / 11;
    field_34_first_digit_texture_idx = gSharp_pare_0x15D8_705064->RegisterDigits_5B9220(field_2A_max_num_of_digits, field_36_sprite_idx);
}

MATCH_FUNC(0x4921b0)
void RollingDigitCounter_38::ChangeStatByAmount_4921B0(s32 amount)
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
void RollingDigitCounter_38::ColorDigits_4921F0(s32 palette_type, s16 palette)
{
    u16 true_palette;
    if (palette_type == palette_types_enum::sprites_2)
    {
        u16 sprite_idx = gGtx_0x106C_703DD4->GetSpriteTrueIndex_5AA460(sprite_types_enum::user_6, field_36_sprite_idx);
        true_palette = gGtx_0x106C_703DD4->GetTruePalette_5AA5F0(palette_types_enum::sprites_2, sprite_idx);
    }
    else
    {
        true_palette = gGtx_0x106C_703DD4->GetTruePalette_5AA5F0(palette_type, palette); // default color ?
    }

    s32 digit_idx = field_2E_non_used_digits;

    while (digit_idx < kNumDigits)
    {
        gSharp_pare_0x15D8_705064->SetPal_5B9660(field_34_first_digit_texture_idx - field_2E_non_used_digits + digit_idx, true_palette);
        digit_idx++;
    }
}

// https://decomp.me/scratch/6E5vt
MATCH_FUNC(0x492260)
s32 RollingDigitCounter_38::DrawDigitsRightAligned_492260(s32 base_xpos, s32 base_ypos)
{
    s32 curr_xpos = base_xpos;
    bool bFirst = true;
    s32 ypos_default = base_ypos + (field_28_sprite_h_calc >> 1);

    // The x of each digit counts back from the last one
    for (s32 idx = field_2E_non_used_digits; idx < kNumDigits; idx++)
    {
        s32 offset = field_13_scroll_offsets[idx];
        if (bFirst)
        {
            char_type curr_char = field_9_shown_digits[idx];
            if (curr_char == '0' && idx != kNumDigits - 1 && !field_13_scroll_offsets[idx])
            {
                continue;
            }

            u8 height;
            if (field_9_shown_digits[idx] != '0' || idx == kNumDigits - 1)
            {
                height = field_28_sprite_h_calc;
            }
            else
            {
                height = field_13_scroll_offsets[idx];
            }

            u16 v = field_28_sprite_h_calc * (kDigitAfterNine - curr_char) - offset;
            curr_xpos = (field_27_sprite_w >> 1) - field_27_sprite_w * (kNumDigits - idx) + base_xpos;
            s32 ypos = base_ypos + (s8)height / 2;
            DrawTextureScaled_495470(gSharp_pare_0x15D8_705064->GetDigitTexture_5B95F0(idx + field_34_first_digit_texture_idx - field_2E_non_used_digits, v, height),
                       curr_xpos,
                       ypos,
                       field_27_sprite_w,
                       height,
                       kAngZero_67B210,
                       0,
                       0);
            bFirst = false;
        }
        else
        {
            u16 v = field_28_sprite_h_calc * (kDigitAfterNine - field_9_shown_digits[idx]) - offset;
            s32 xpos = (field_27_sprite_w >> 1) - field_27_sprite_w * (kNumDigits - idx) + base_xpos;
            DrawTextureScaled_495470(gSharp_pare_0x15D8_705064->GetDigitTexture_5B95F0(idx + field_34_first_digit_texture_idx - field_2E_non_used_digits,
                                                             v,
                                                             field_28_sprite_h_calc),
                       xpos,
                       ypos_default,
                       field_27_sprite_w,
                       field_28_sprite_h_calc,
                       kAngZero_67B210,
                       0,
                       0);
        }
    }
    return curr_xpos - (field_27_sprite_w >> 1);
}

// Draws the digits left to right, skipping leading zeros, and returns the x after the last digit
MATCH_FUNC(0x492430)
s32 RollingDigitCounter_38::DrawDigitsLeftAligned_492430(s32 base_xpos, s32 base_ypos)
{
    // u32: converts with the Fix16(u32) constructor, whose out-of-line copy is 0x4926F0
    bool bFirst = true;
    u32 curr_xpos = base_xpos + (field_27_sprite_w >> 1);
    u32 ypos_default = base_ypos + (field_28_sprite_h_calc >> 1);

    for (s32 idx = field_2E_non_used_digits; idx < kNumDigits; idx++)
    {
        char_type offset_byte = field_13_scroll_offsets[idx];
        s32 offset = offset_byte;
        if (bFirst)
        {
            if (field_9_shown_digits[idx] == '0' && idx != kNumDigits - 1 && !offset_byte)
            {
                continue;
            }

            u8 height;
            if (field_9_shown_digits[idx] != '0' || idx == kNumDigits - 1)
            {
                height = field_28_sprite_h_calc;
            }
            else
            {
                height = offset_byte;
            }

            u16 v = field_28_sprite_h_calc * (kDigitAfterNine - field_9_shown_digits[idx]) - offset;
            u32 ypos = base_ypos + (s8)height / 2;
            DrawTextureScaled_495470(gSharp_pare_0x15D8_705064->GetDigitTexture_5B95F0(idx + field_34_first_digit_texture_idx - field_2E_non_used_digits, v, height),
                       curr_xpos,
                       ypos,
                       field_27_sprite_w,
                       height,
                       kAngZero_67B210,
                       0,
                       0);
            bFirst = false;
            curr_xpos += field_27_sprite_w;
        }
        else
        {
            u16 v = field_28_sprite_h_calc * (kDigitAfterNine - field_9_shown_digits[idx]) - offset;
            DrawTextureScaled_495470(gSharp_pare_0x15D8_705064->GetDigitTexture_5B95F0(idx + field_34_first_digit_texture_idx - field_2E_non_used_digits,
                                                             v,
                                                             field_28_sprite_h_calc),
                       curr_xpos,
                       ypos_default,
                       field_27_sprite_w,
                       field_28_sprite_h_calc,
                       kAngZero_67B210,
                       0,
                       0);
            curr_xpos += field_27_sprite_w;
        }
    }
    return curr_xpos;
}

MATCH_FUNC(0x4925c0)
bool RollingDigitCounter_38::IsAnyDigitRolling_4925C0()
{
    s32 digit_idx = field_2E_non_used_digits;

    while (digit_idx < kNumDigits)
    {
        if (this->field_13_scroll_offsets[digit_idx] != 0)
        {
            return true;
        }
        digit_idx++;
    }
    return false;
}

MATCH_FUNC(0x4925e0)
void RollingDigitCounter_38::UpdateRollingDigits_4925E0()
{
    s32 shown_value;
    sscanf((const char_type*)&field_9_shown_digits, "%d", &shown_value);

    if (field_4_target_value == -1 || shown_value == field_4_target_value && !RollingDigitCounter_38::IsAnyDigitRolling_4925C0())
    {
        if (field_0_value == shown_value)
        {
            field_4_target_value = -1;
            return;
        }
        field_4_target_value = field_0_value;
        if (field_0_value > shown_value)
        {
            field_8_bRollingUp = true;
        }
        else
        {
            field_8_bRollingUp = false;
        }
        sprintf((char_type*)&field_1D_target_digits, "%09d", field_0_value);
    }

    for (s32 idx = field_2E_non_used_digits; idx < kNumDigits; idx++)
    {
        // idx + 20 is field_1D_target_digits[idx]; written as an offset from field_9_shown_digits, as in the original
        if (field_9_shown_digits[idx + 20] != field_9_shown_digits[idx] || field_13_scroll_offsets[idx])
        {
            if (field_8_bRollingUp)
            {
                field_13_scroll_offsets[idx] = field_2C_digit_transition_speed + field_13_scroll_offsets[idx];
                if (field_13_scroll_offsets[idx] >= field_28_sprite_h_calc)
                {
                    if (field_9_shown_digits[idx] < '9')
                    {
                        field_9_shown_digits[idx]++;
                    }
                    else
                    {
                        field_9_shown_digits[idx] = '0';
                    }
                    field_13_scroll_offsets[idx] = 0;
                }
            }
            else
            {
                field_13_scroll_offsets[idx] = field_13_scroll_offsets[idx] - field_2C_digit_transition_speed;
                if (field_13_scroll_offsets[idx] < 0)
                {
                    if (field_9_shown_digits[idx] > '0')
                    {
                        field_9_shown_digits[idx]--;
                    }
                    else
                    {
                        field_9_shown_digits[idx] = '9';
                    }
                    field_13_scroll_offsets[idx] = field_28_sprite_h_calc - field_2C_digit_transition_speed;
                }
            }
        }
    }
}

MATCH_FUNC(0x4f7660)
s32 __stdcall GetMaxNumOfDigits_4F7660(s32 &max_value)
{
    s32 num_digits = 1;
    s32 abs_value = max_value;

    if (abs_value < 0)
    {
        abs_value = -abs_value;
    }
    for (; 10 <= abs_value; abs_value /= 10, num_digits++)
    {}
    return num_digits;
}