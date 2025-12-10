#pragma once

#include "Function.hpp"

class thirsty_lamarr
{
  public:
    EXPORT thirsty_lamarr();
    EXPORT void sub_492110(s16 a1, s32 a2, s16 a3);
    EXPORT void sub_492150();
    EXPORT void ChangeStatByAmount_4921B0(s32 amount);
    EXPORT void sub_4921F0(s32 a2, s16 a3);
    EXPORT s32 sub_492260(s32 a2, s32 a3);
    EXPORT s32 sub_492430(s32 a3, s32 a4);
    EXPORT char_type sub_4925C0();
    EXPORT void sub_4925E0();

    inline s32 get_value()
    {
        return field_0;
    }

    s32 field_0;
    s32 field_4;
    char_type field_8;
    s8 field_9[9];
    u8 field_12;
    s8 field_13[9];
    char_type field_1C;
    u8 field_1D_buf[9];
    char_type field_26;
    u8 field_27_sprite_w;
    u8 field_28_sprite_h_calc;
    char_type field_29;
    s16 field_2A;
    s16 field_2C;
    u16 field_2E_idx;
    s32 field_30;
    s16 field_34;
    s16 field_36;
};