#pragma once

#include "Function.hpp"

struct gmp_tile_animation;

class TileAnim_2
{
  public:
    EXPORT s32 sub_5BC260(s16 a1, s16 a2, s16 a3, s16 a4, s16 a5);
    EXPORT void sub_5BC2C0(gmp_tile_animation* a1);
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
    EXPORT void sub_5BC190(gmp_tile_animation* pStru);
    EXPORT void sub_5BC1D0();
    EXPORT char_type sub_5BC1F0();
    EXPORT TileAnim_18();
    EXPORT ~TileAnim_18();

    u16 field_0;
    u16 field_2;
    s16 field_4;
    u16 field_6;
    u16 field_8;
    s16 field_A;
    s32 field_C_ptr;
    s16 field_10;
    s16 field_12_idx;
    TileAnim_18* field_14_next_ptr;
};

class TileAnim_4BC
{
  public:
    TileAnim_4BC()
    {
        // TODO
    }
    EXPORT ~TileAnim_4BC();

    TileAnim_18* get_new_TileAnim_18()
    {
        TileAnim_18* tmp = field_0_pStart;
        field_0_pStart = tmp->field_14_next_ptr;
        tmp->field_14_next_ptr = field_4_pPrev;
        field_4_pPrev = tmp;
        tmp->sub_5BC130();
        return tmp;
    }

    TileAnim_18* field_0_pStart;
    TileAnim_18* field_4_pPrev;
    TileAnim_18 field_8[50];
    s16 field_4B8;
    s16 field_4BA;
};

EXPORT_VAR extern TileAnim_2* gTileAnim_2_7052C4;
