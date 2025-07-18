#pragma once

#include "Function.hpp"

class gmp_map_zone;

class silly_saha_0x2C
{
  public:
    EXPORT silly_saha_0x2C(); // 0x431D30
    EXPORT ~silly_saha_0x2C(); // 0x431D40
    EXPORT void sub_431D50();
    EXPORT void sub_431DA0();
    EXPORT void sub_431DB0();

    gmp_map_zone* field_0_pZone;
    s16 field_4;
    char_type field_6;
    char_type field_7;
    s32 field_8;
    s32 field_C;
    s16 field_10;
    s16 field_12;
    s32 field_14;
    s32 field_18;
    s32 field_1C;
    s32 field_20_counterVal;
    char_type field_24;
    u8 field_25;
    u8 field_26;
    char_type field_27;
    s16 field_28;
    char_type field_2A_bUsed;
    char_type field_2B;
};

class sad_mirzakhani
{
  public:
    EXPORT sad_mirzakhani(); // 0x431DC0
    EXPORT ~sad_mirzakhani(); // 0x431DF0
    EXPORT void sub_431E10(class eager_benz* a2);
    EXPORT void sub_431E30();
    EXPORT u16 next_free_idx_431E90();
    EXPORT u16 find_431EC0(u16 idx, s16 f_4, s32 f_8, s32 f_c, s16 f_10, s16 f_12, s32 f_14, s32 f_18, gmp_map_zone* pZone);
    EXPORT u16 alloc_next_431FE0(s16 f_4,
                                 s32 f_8,
                                 s32 f_c,
                                 s16 f_10,
                                 s16 f_12,
                                 s32 f_14,
                                 s32 f_18,
                                 s32 f_1c,
                                 s8 f_24,
                                 s8 f_25,
                                 u16 f_28,
                                 gmp_map_zone* pZone);
    EXPORT s16 sub_432080(u16 idx);
    EXPORT void sub_4320D0(s16 f_4, s32 f_8, s32 f_c, s16 f_10, s16 f_12, s32 f_14, s32 f_18, gmp_map_zone* pZone);
    EXPORT s8 sub_432170(int a2, int a3);
    EXPORT s8 sub_432240(int a2, int a3);
    EXPORT bool sub_432300(int a2, int a3);

    silly_saha_0x2C field_0[10];
    class eager_benz* field_1B8;
    s32 field_1BC;
};