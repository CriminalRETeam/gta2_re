#pragma once

#include "Function.hpp"

class TileAnim_2
{
  public:
    EXPORT s32 sub_5BC260(s16 a1, s16 a2, s16 a3, s16 a4, s16 a5);
    EXPORT s32 sub_5BC2C0();
    EXPORT void Empty_5BC300();
    EXPORT void sub_5BC310();
    EXPORT TileAnim_2();
    EXPORT ~TileAnim_2();
    s16 field_0_count;
};

class TileAnim_18
{
  public:
    EXPORT void sub_5BC130();
    EXPORT s32 sub_5BC150();
    EXPORT void sub_5BC190(s32 pStru);
    EXPORT s32 sub_5BC1D0();
    EXPORT char_type sub_5BC1F0();
    EXPORT void ctor_5BEBA0();
    EXPORT ~TileAnim_18();

    s16 field_0;
    s16 field_2;
    s16 field_4;
    s16 field_6;
    s16 field_8;
    s16 field_A;
    s32 field_C_ptr;
    s16 field_10;
    s16 field_12_idx;
    TileAnim_18* field_14_next_ptr;
};

class TileAnim_4BC
{
  public:
    EXPORT ~TileAnim_4BC();
    TileAnim_18* field_0_pStart;
    TileAnim_18* field_4_pPrev;
    TileAnim_18 field_8[50];
    s16 field_4B8;
    s16 field_4BA;
};

extern TileAnim_2* gTileAnim_2_7052C4;