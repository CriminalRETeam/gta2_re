#pragma once

#include "Function.hpp"

class gmp_map_zone;
class eager_benz;

class silly_saha_0x2C
{
  public:
    silly_saha_0x2C(); // 0x431D30
    ~silly_saha_0x2C(); // 0x431D40
    void sub_431D50();
    void sub_431DA0();
    void sub_431DB0();

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
    char_type field_25;
    char_type field_26;
    char_type field_27;
    s16 field_28;
    char_type field_2A_bUsed;
    char_type field_2B;
};

class sad_mirzakhani
{
  public:
    sad_mirzakhani(); // 0x431DC0
    ~sad_mirzakhani(); // 0x431DF0
    void sub_431E10(eager_benz* a2);
    void sub_431E30();
    u16 next_free_idx_431E90();
    u16 find_431EC0(u16 idx, s16 f_4, s32 f_8, s32 f_c, s16 f_10, s16 f_12, s32 f_14, s32 f_18, gmp_map_zone* pZone);
    u16 alloc_next_431FE0(s16 f_4,
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
    s16 sub_432080(u16 idx);
    void sub_4320D0(s16 f_4, s32 f_8, s32 f_c, s16 f_10, s16 f_12, s32 f_14, s32 f_18, gmp_map_zone* pZone);
    s8 sub_432170(int a2, int a3);
    s8 sub_432240(int a2, int a3);
    bool sub_432300(int a2, int a3);

    silly_saha_0x2C field_0[10];
    eager_benz* field_1B8;
    s32 field_1BC;
};
