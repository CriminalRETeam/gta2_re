#pragma once

#include "Function.hpp"

struct gmp_block_info;

class Link_2
{
  public:
    EXPORT u16 FUN_0040ce90()
    {
        return field_0 & 0x1ff;
    }

    s16 field_0;
};

class Junction_10
{
  public:
    EXPORT char_type sub_588580(s32 a2);
    EXPORT u16 sub_5885C0(u16 a2);
    Link_2 field_0_n;
    Link_2 field_2_s;
    Link_2 field_4_e;
    Link_2 field_6_w;
    s32 field_8_type;
    u8 field_C_min_x;
    u8 field_D_min_y;
    u8 field_E_max_x;
    u8 field_F_min_y;
};

struct JunctionSegment_0x8
{
    inline bool sub_40CF20(s16 x, s16 y)
    {
        return x >= field_4_min_x 
               && x <= field_6_max_x 
               && y >= field_5_min_y 
               && y <= field_7_min_y;
    }
    u16 field_0_junction_num1;
    u16 field_2_junction_num2;
    u8 field_4_min_x;
    u8 field_5_min_y;
    u8 field_6_max_x;
    u8 field_7_min_y;
};

class RouteFinder_10
{
  public:
    EXPORT RouteFinder_10();
    u16 field_0_idx;
    s16 field_2;
    s16 field_4;
    s16 field_6;
    RouteFinder_10* field_8;
    RouteFinder_10* field_C;
};

class RouteFinder_200
{
  public:
    u16 field_0[0x100];
};

class RouteFinder
{
  public:
    EXPORT void ShowJunctionIds_588620();
    EXPORT u16 sub_588810(u8 a2, u8 a3, u8 a4);
    EXPORT u16 sub_588950(s32 a2, s32 a3, s32 a4);
    EXPORT u16 sub_588AA0(u8 a2, u8 a3, u16 a4, u16 a5); // ret _BOOL2
    EXPORT void Load_RGEN_588B30();
    EXPORT void Reset_588C60();
    EXPORT bool sub_588CA0(gmp_block_info* block, s32 a2, u8 a3);
    EXPORT char_type sub_588DE0(gmp_block_info* a1, s32 a2, s32 a4);
    EXPORT u16 sub_588E60(u8 x, u8 y, u8 z, char_type a5, s32 a6);
    EXPORT u16 sub_588F30(u8 x_coord, u8 y_coord, u8 z_coord, char_type a5, s32 a6);
    EXPORT u16 sub_589000(u8 x_coord, u8 y_coord, u8 z_coord, char_type a5, s32 a6);
    EXPORT u8 sub_5890D0(u16 a2, s32 a3, u8* a4, u8* a5);
    EXPORT s32 sub_589210(char_type a2, char_type a3, s32 a4, char_type a5, s32 a6, u16 a7);
    EXPORT RouteFinder_10* sub_5892F0(RouteFinder_10* a2, u16 a3, s16 a4);
    EXPORT RouteFinder_10* sub_589390(u16 a2);
    EXPORT void sub_589420(RouteFinder_10* a2);
    EXPORT char_type sub_589480(u8 a2, u8 a3, u8 a4, u8 a5, u8 a6, u8 a7, s32 a8);
    EXPORT char_type sub_5895C0(u8 a2, s16 a3, u8 a4, s32 a5, s32 a6);
    EXPORT s16 sub_589930(s16 idx);
    EXPORT s16 sub_589960();
    EXPORT u16 sub_589990(RouteFinder_10* a2, u16 a3, s16 a4);
    EXPORT bool sub_5899C0(RouteFinder_10* a2, s32 a3);
    EXPORT char_type sub_589BB0(RouteFinder_10* a2, s32 a3);
    EXPORT RouteFinder_10* sub_589E00();
    EXPORT char_type sub_589E20(s32 a2);
    EXPORT char_type sub_589E70(s32 a2);
    EXPORT s16 sub_589EB0();
    EXPORT s16 sub_589F70();
    EXPORT void sub_58A020(char_type a2);
    EXPORT Junction_10* sub_58A0B0(u16 jIdx);
    EXPORT s16 sub_58A0D0(u8 a2, u8 a3, u8 a4, u8 a5, u8 a6, u8 a7, s32 a8);
    EXPORT s16 sub_58A130(u8 a1, s16 a2, u8 a3, u8* a4, s32 a5, s32 a6);
    EXPORT void sub_58A190(u8 a2, u8 a3, u8 a4, u8 a5, u8 a6, u8 a7, s32 a8);
    EXPORT RouteFinder();

    u16 field_0;
    u8 field_2;
    char_type field_3;
    s16 field_4;
    s16 field_6;
    Junction_10 field_8[545];
    RouteFinder_200 field_2218[50];
    u16 field_8618_idx;
    u16 field_861A;
    RouteFinder_10 field_861C[545];
    RouteFinder_10* field_A82C;
    JunctionSegment_0x8 field_A830[545];
    JunctionSegment_0x8 field_B938[545];
    char_type field_CA40[545];
    u16 field_CC62;
    u16 field_CC64;
    u16 field_CC66_545_count;
};

EXTERN_GLOBAL(RouteFinder*, gRouteFinder_6FFDC8);
