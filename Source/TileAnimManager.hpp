#pragma once

#include "Function.hpp"
#include "Pool.hpp"
#include <cstdio>

struct gmp_tile_animation;

class TileAnimManager
{
  public:
    EXPORT void AddAnim_5BC260(s16 base, s16 start_frame, s16 length, s16 frame_rate, s16 repeat);
    EXPORT void AddGmpAnim_5BC2C0(const gmp_tile_animation* pTileAnimation);
    EXPORT void Empty_5BC300();
    EXPORT void UpdateTileAnimations_5BC310();
    EXPORT TileAnimManager();
    EXPORT ~TileAnimManager();
    s16 field_0_count;
};
GTA2_ASSERT_SIZEOF_ALWAYS(TileAnimManager, 0x2);

class TileAnim
{
  public:
    // 9.6f 0x4C33F0
    inline void Init_4C33F0(s16 base, s16 start_frame, s16 length, s16 frame_rate, s16 repeat)
    {
        field_10_base = base;
        field_0_start_frame = start_frame;
        field_2_anim_length = length;
        field_4_frame_rate = frame_rate;
        field_6_repeat = repeat;
        field_C_ptr = NULL;
    }

    EXPORT void PoolAllocate();
    void PoolDeallocate()
    {
      
    }
    EXPORT void ApplyCurrentFrame_5BC150();
    EXPORT void InitFromGmpAnim_5BC190(const gmp_tile_animation* pAnimation);
    EXPORT void Start_5BC1D0();
    EXPORT bool PoolUpdate();
    EXPORT TileAnim();
    EXPORT ~TileAnim();

    u16 field_0_start_frame;
    u16 field_2_anim_length;
    s16 field_4_frame_rate;
    u16 field_6_repeat;
    u16 field_8_current_frame;
    s16 field_A_frame_timer;
    const gmp_tile_animation* field_C_ptr;
    s16 field_10_base;
    s16 field_12_anim_idx;
    TileAnim* mpNext;
};
GTA2_ASSERT_SIZEOF_ALWAYS(TileAnim, 0x18);

class TileAnimPool
{
  public:
    // Inlined 0x4c34b0
    TileAnimPool()
    {
    }

    EXPORT ~TileAnimPool();

    TileAnim* Allocate()
    {
        return field_0_pool.Allocate();
    }

    Pool<TileAnim, 50> field_0_pool;
};

EXTERN_GLOBAL(TileAnimManager*, gTileAnim_2_7052C4);
