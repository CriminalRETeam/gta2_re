#pragma once

#include "Function.hpp"

class thirsty_lamarr
{
  public:
    EXPORT thirsty_lamarr();
    EXPORT char_type sub_492110(s16 a2, s32 a3, s16 a4);
    EXPORT void sub_492150();
    EXPORT void sub_4921B0(s32 amount);
    EXPORT void sub_4921F0(s32 a2, s16 a3);
    EXPORT s32 sub_492260(s32 a2, s32 a3);
    EXPORT s32 sub_492430(thirsty_lamarr* a1, s32 a2, s32 a3, s32 a4);
    EXPORT char_type sub_4925C0();
    EXPORT s16 sub_4925E0();

    s32 field_0;
    s32 field_4;
    char_type field_8;
    char_type field_9;
    char_type field_A;
    char_type field_B;
    s32 field_C;
    char_type field_10;
    char_type field_11;
    char_type field_12;
    char_type field_13[9];
    char_type field_1C;
    char_type field_1D_buf[9];
    char_type field_26;
    char_type field_27_sprite_w;
    u8 field_28_sprite_h_calc;
    char_type field_29;
    s16 field_2A;
    char_type field_2C;
    char_type field_2D;
    u16 field_2E_idx;
    s32 field_30;
    s16 field_34;
    s16 field_36;
};
