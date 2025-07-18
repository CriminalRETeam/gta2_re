#pragma once

#include "Function.hpp"
#include <cstdio>

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
    EXPORT void sub_5BC150();
    EXPORT void sub_5BC190(gmp_tile_animation* pStru);
    EXPORT void sub_5BC1D0();
    EXPORT char_type sub_5BC1F0();
    EXPORT TileAnim_18();
    EXPORT ~TileAnim_18();

    u16 field_0;
    u16 field_2_anim_length;
    s16 field_4_frame_rate;
    u16 field_6_repeat;
    u16 field_8;
    s16 field_A;
    gmp_tile_animation* field_C_ptr;
    s16 field_10_base;
    s16 field_12_idx;
    TileAnim_18* field_14_next_ptr;
};

class TileAnim_4BC
{
  public:
    // Inlined 0x4c34b0
    EXPORT TileAnim_4BC()
    {
        TileAnim_18* it = field_8;
        for (u32 i = 0; i < 49; i++)
        {
            it->field_14_next_ptr = it + 1;
            it++;
        }
        field_0_pStart = field_8;
        field_8[49].field_14_next_ptr = NULL;
        field_4_pPrev = NULL;
        field_4B8 = 0;
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

EXTERN_GLOBAL(TileAnim_2*, gTileAnim_2_7052C4);
