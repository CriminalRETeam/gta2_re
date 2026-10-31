#pragma once

#include "Function.hpp"
#include "Pool.hpp"
#include <cstdio>

struct gmp_tile_animation;

class TileAnimManager
{
  public:
    EXPORT void AddAnim(s16 base, s16 start_frame, s16 length, s16 frame_rate, s16 repeat);
    EXPORT void AddGmpAnim(const gmp_tile_animation* pTileAnimation);
    EXPORT void Empty();
    EXPORT void UpdateTileAnimations();
    EXPORT TileAnimManager();
    EXPORT ~TileAnimManager();
    s16 mCount; // 0x0
};
GTA2_ASSERT_SIZEOF_ALWAYS(TileAnimManager, 0x2);

class TileAnim
{
  public:
    // 9.6f 0x4C33F0
    inline void Init_4C33F0(s16 base, s16 start_frame, s16 length, s16 frame_rate, s16 repeat)
    {
        mBase = base;
        mStartFrame = start_frame;
        mAnimLength = length;
        mFrameRate = frame_rate;
        mRepeat = repeat;
        mPtr = NULL;
    }

    EXPORT void PoolAllocate();
    void PoolDeallocate()
    {
      
    }
    EXPORT void ApplyCurrentFrame();
    EXPORT void InitFromGmpAnim(const gmp_tile_animation* pAnimation);
    EXPORT void Start();
    EXPORT bool PoolUpdate();
    EXPORT TileAnim();
    EXPORT ~TileAnim();

    u16 mStartFrame; // 0x0
    u16 mAnimLength; // 0x2
    s16 mFrameRate; // 0x4
    u16 mRepeat; // 0x6
    u16 mCurrentFrame; // 0x8
    s16 mFrameTimer; // 0xA
    const gmp_tile_animation* mPtr; // 0xC
    s16 mBase; // 0x10
    s16 mAnimIdx; // 0x12
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
        return mPool.Allocate();
    }

    Pool<TileAnim, 50> mPool; // 0x0
};

EXTERN_GLOBAL(TileAnimManager*, gTileAnim_2_7052C4);
