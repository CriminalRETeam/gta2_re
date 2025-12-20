#pragma once

#include "Function.hpp"
#include "Pool.hpp"
#include <cstdio>

struct gmp_tile_animation;

class TileAnim_2
{
  public:
    EXPORT void sub_5BC260(s16 base, s16 f0, s16 length, s16 frame_rate, s16 repeat);
    EXPORT void sub_5BC2C0(const gmp_tile_animation* a1);
    EXPORT void Empty_5BC300();
    EXPORT void UpdateTileAnimations_5BC310();
    EXPORT TileAnim_2();
    EXPORT ~TileAnim_2();
    s16 field_0_count;
};

class TileAnim_18
{
  public:
    EXPORT void PoolAllocate();
    void PoolDeallocate()
    {
      
    }
    EXPORT void sub_5BC150();
    EXPORT void sub_5BC190(const gmp_tile_animation* pStru);
    EXPORT void sub_5BC1D0();
    EXPORT char_type PoolUpdate();
    EXPORT TileAnim_18();
    EXPORT ~TileAnim_18();

    u16 field_0;
    u16 field_2_anim_length;
    s16 field_4_frame_rate;
    u16 field_6_repeat;
    u16 field_8;
    s16 field_A;
    const gmp_tile_animation* field_C_ptr;
    s16 field_10_base;
    s16 field_12_idx;
    TileAnim_18* mpNext;
};

class TileAnimPool
{
  public:
    // Inlined 0x4c34b0
    TileAnimPool()
    {
    }

    EXPORT ~TileAnimPool();

    TileAnim_18* Allocate()
    {
        return field_0_pool.Allocate();
    }

    Pool<TileAnim_18, 50> field_0_pool;
};

EXTERN_GLOBAL(TileAnim_2*, gTileAnim_2_7052C4);
