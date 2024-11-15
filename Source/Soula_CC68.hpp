#pragma once

#include "Function.hpp"

class gmp_block_info;

class Junction_10
{
public:
    EXPORT char_type sub_588580(s32 a2);
    EXPORT s16 sub_5885C0(s16 a2);
    s16 field_0_n;
    s16 field_2_s;
    s16 field_4_e;
    s16 field_6_w;
    s32 field_8_type;
    u8 field_C_min_x;
    u8 field_D_min_y;
    u8 field_E_max_x;
    u8 field_F_min_y;
};

class Soula_10
{
public:
    EXPORT Soula_10* sub_5892D0();
    u16 field_0_idx;
    s16 field_2;
    s16 field_4;
    s16 field_6;
    Soula_10* field_8;
    Soula_10* field_C;
};

class Soula_CC68
{
public:
    EXPORT void ShowJunctionIds_588620();
    EXPORT u16 sub_588810(u8 a2, u8 a3, u8 a4);
    EXPORT u16 sub_588950(s32 a2, s32 a3, s32 a4);
    EXPORT u16 sub_588AA0(u8 a2, u8 a3, u16 a4, u16 a5); // ret _BOOL2 
    EXPORT void Load_RGEN_588B30();
    EXPORT void Reset_588C60();
    EXPORT char_type sub_588CA0(gmp_block_info* a1, s32 a2, char_type a3);
    EXPORT char_type sub_588DE0(gmp_block_info* a1, s32 a2, s32 a4);
    EXPORT u16 sub_588E60(u8 a2, u8 a3, u8 a4, char_type a5, s32 a6);
    EXPORT u16 sub_588F30(u8 x_coord, u8 y_coord, u8 z_coord, char_type a5, s32 a6);
    EXPORT u16 sub_589000(u8 x_coord, u8 y_coord, u8 z_coord, char_type a5, s32 a6);
    EXPORT u8 sub_5890D0(u16 a2, s32 a3, u8* a4, u8* a5);
    EXPORT s32 sub_589210(char_type a2, char_type a3, s32 a4, char_type a5, s32 a6, u16 a7);
    EXPORT Soula_10* sub_5892F0(Soula_10* a2, u16 a3, s16 a4);
    EXPORT Soula_10* sub_589390(u16 a2);
    EXPORT void sub_589420(Soula_10* a2);
    EXPORT char_type sub_589480(u8 a2, u8 a3, u8 a4, u8 a5, u8 a6, u8 a7, s32 a8);
    EXPORT char_type sub_5895C0(u8 a2, s16 a3, u8 a4, s32 a5, s32 a6);
    EXPORT s16 sub_589930(s16 idx);
    EXPORT s16 sub_589960();
    EXPORT u16 sub_589990(Soula_10* a2, u16 a3, s16 a4);
    EXPORT bool sub_5899C0(Soula_10* a2, s32 a3);
    EXPORT char_type sub_589BB0(Soula_10* a2, s32 a3);
    EXPORT Soula_10* sub_589E00();
    EXPORT char_type sub_589E20(s32 a2);
    EXPORT char_type sub_589E70(s32 a2);
    EXPORT s16 sub_589EB0();
    EXPORT s16 sub_589F70();
    EXPORT void sub_58A020(char_type a2);
    EXPORT Junction_10* sub_58A0B0(u16 jIdx);
    EXPORT s16 sub_58A0D0(u8 a2, u8 a3, u8 a4, u8 a5, u8 a6, u8 a7, s32 a8);
    EXPORT s16 sub_58A130(u8 a2, s16 a3, u8 a4, u8* a5, s32 a6, s32 a7);
    EXPORT s16 sub_58A190(u8 a2, u8 a3, u8 a4, u8 a5, u8 a6, u8 a7, s32 a8);
    EXPORT Soula_CC68* ctor_58A1C0();

    s16 field_0;
    u8 field_2;
    char_type field_3;
    s16 field_4;
    s16 field_6;
    Junction_10 field_8[545];
    char_type field_2218[25600];
    u16 field_8618_idx;
    u16 field_861A;
    Soula_10 field_861C[545];
    Soula_10* field_A82C;
    char_type field_A830[4360];
    char_type field_B938[4360];
    char_type field_CA40[544];
    char_type field_CC60;
    char_type field_CC61;
    s16 field_CC62;
    u16 field_CC64;
    u16 field_CC66_545_count;
};
