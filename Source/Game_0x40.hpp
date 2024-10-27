#pragma once

#include "types.hpp"

struct angry_lewin_0x85C;

struct Game_0x40
{
    s32 field_0;
    angry_lewin_0x85C *field_4[6];
    angry_lewin_0x85C *field_1C_unk;
    u8 field_20;
    u8 field_21;
    char_type field_22;
    u8 field_23_max_idx;
    u8 field_24_cur_idx;
    char_type field_25;
    char_type field_26;
    char_type field_27;
    s32 field_28;
    s32 field_2C_main_state;
    char_type field_30;
    char_type field_31;
    char_type field_32;
    char_type field_33;
    s32 field_34;
    angry_lewin_0x85C *field_38_orf1;
    char_type field_3C_bSkipPolice;
    char_type field_3D;
    char_type field_3E;
    char_type field_3F;

    // todo: ordering

    EXPORT void sub_4B9720();
};

extern Game_0x40* gGame_0x40_67E008;
