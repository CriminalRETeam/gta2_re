#pragma once

#include "Function.hpp"

class RollingDigitCounter_38
{
  public:
    enum
    {
        kNumDigits = 9,
        kDigitAfterNine = '9' + 1, // the glyph rows are 0-9 then a blank row, see DrawDigits
    };

    EXPORT RollingDigitCounter_38();
    EXPORT void SetupDigitsParams_492110(s16 digit_transition_speed, s32 max_value, s16 palette);
    EXPORT void InitDigitSprites_492150();
    EXPORT void ChangeStatByAmount_4921B0(s32 amount);
    EXPORT void ColorDigits_4921F0(s32 palette_type, s16 palette);
    EXPORT s32 DrawDigitsRightAligned_492260(s32 base_xpos, s32 base_ypos);
    EXPORT s32 DrawDigitsLeftAligned_492430(s32 base_xpos, s32 base_ypos);
    EXPORT bool IsAnyDigitRolling_4925C0();
    EXPORT void UpdateRollingDigits_4925E0();

    // 9.6f 0x4A50B0
    inline void SetValueClamped_4A50B0(s32 value)
    {
        if (value < -field_30_max_value)
        {
            field_0_value = -field_30_max_value;
        }
        else if (value > field_30_max_value)
        {
            field_0_value = field_30_max_value;
        }
        else
        {
            field_0_value = value;
        }
    }

    // 9.6f 0x41DC30
    inline s32 get_value()
    {
        return field_0_value;
    }

    s32 field_0_value;
    s32 field_4_target_value;
    bool field_8_bRollingUp;
    char_type field_9_shown_digits[10];
    s8 field_13_scroll_offsets[10];
    u8 field_1D_target_digits[10];
    u8 field_27_sprite_w;
    u8 field_28_sprite_h_calc;
    s16 field_2A_max_num_of_digits;
    s16 field_2C_digit_transition_speed;
    u16 field_2E_non_used_digits;
    s32 field_30_max_value;
    s16 field_34_first_digit_texture_idx;
    s16 field_36_sprite_idx;
};