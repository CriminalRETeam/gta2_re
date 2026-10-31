#include "TileAnimManager.hpp"

#include "Globals.hpp"
#include "error.hpp"
#include "gtx_0x106C.hpp"
#include "map_0x370.hpp"
#include <stdio.h>
#include "enums.hpp"

DEFINE_GLOBAL(TileAnimManager*, gTileAnim_2_7052C4, 0x7052C4);
DEFINE_GLOBAL(TileAnimPool*, gTileAnimPool_7052C8, 0x7052C8);

MATCH_FUNC(0x5bc130)
void TileAnim::PoolAllocate()
{
    mAnimIdx = gTileAnim_2_7052C4->mCount++;
}

MATCH_FUNC(0x5bc150)
void TileAnim::ApplyCurrentFrame()
{
    if (mPtr != NULL)
    {
        gGtx_0x106C_703DD4->SetTileRemap(mBase, mPtr->mTiles[mCurrentFrame]);
        return;
    }
    gGtx_0x106C_703DD4->SetTileRemap(mBase, mCurrentFrame);
}

MATCH_FUNC(0x5bc190)
void TileAnim::InitFromGmpAnim(const gmp_tile_animation* pAnimation)
{
    mPtr = pAnimation;
    mFrameRate = pAnimation->mFrameRate;
    mRepeat = pAnimation->mRepeat;
    mBase = pAnimation->mBase;
    mAnimLength = pAnimation->mAnimLength - 1;
    mStartFrame = 0;
}

MATCH_FUNC(0x5bc1d0)
void TileAnim::Start()
{
    mFrameTimer = mFrameRate;
    mCurrentFrame = mStartFrame;
    ApplyCurrentFrame();
}

MATCH_FUNC(0x5bc1f0)
bool TileAnim::PoolUpdate()
{
    bool bAtEnd;

    mFrameTimer--;
    if (mFrameTimer == 0)
    {
        if (mAnimLength < mStartFrame)
        {
            mCurrentFrame--;
            bAtEnd = mCurrentFrame < mAnimLength;
        }
        else
        {
            mCurrentFrame++;
            bAtEnd = mCurrentFrame > mAnimLength;
        }
        if (bAtEnd)
        {
            if (mRepeat > 0 && --mRepeat == 0)
            {
                return true;
            }
            mCurrentFrame = mStartFrame;
        }
        ApplyCurrentFrame();
        mFrameTimer = mFrameRate;
    }
    return false;
}

MATCH_FUNC(0x5bc260)
void TileAnimManager::AddAnim(s16 base, s16 start_frame, s16 length, s16 frame_rate, s16 repeat)
{
    TileAnim* pAnim = gTileAnimPool_7052C8->Allocate();
    pAnim->Init_4C33F0(base, start_frame, length, frame_rate, repeat);
    pAnim->Start();
}

MATCH_FUNC(0x5bc2c0)
void TileAnimManager::AddGmpAnim(const gmp_tile_animation* pTileAnimation)
{
    TileAnim* pAnim = gTileAnimPool_7052C8->Allocate();
    pAnim->InitFromGmpAnim(pTileAnimation);
    pAnim->Start();
}

MATCH_FUNC(0x5bc300)
void TileAnimManager::Empty()
{
}

MATCH_FUNC(0x5bc310)
void TileAnimManager::UpdateTileAnimations()
{
    gTileAnimPool_7052C8->mPool.UpdatePool();
}

MATCH_FUNC(0x5bc3a0)
TileAnimManager::TileAnimManager()
{
    if (gTileAnimPool_7052C8 == NULL)
    {
        gTileAnimPool_7052C8 = new TileAnimPool();
        if (gTileAnimPool_7052C8 == NULL)
        {
            FatalError_4A38C0(Gta2Error::OutOfMemoryNewOperator, "C:\\Splitting\\Gta2\\Source\\tileanim.cpp", 220);
        }
    }
    mCount = 1;
}

MATCH_FUNC(0x5bc470)
TileAnimManager::~TileAnimManager()
{
    if (gTileAnimPool_7052C8)
    {
        GTA2_DELETE_AND_NULL(gTileAnimPool_7052C8);
    }
}

MATCH_FUNC(0x5bc4a0)
TileAnimPool::~TileAnimPool()
{

}

MATCH_FUNC(0x5beba0)
TileAnim::TileAnim()
{
    mStartFrame = 0;
    mAnimLength = 0;
    mFrameRate = 0;
    mRepeat = 0;
    mCurrentFrame = 0;
    mFrameTimer = 0;
    mPtr = NULL;
    mBase = 0;
    mAnimIdx = 0;
    mpNext = NULL;
}

MATCH_FUNC(0x5bebd0)
TileAnim::~TileAnim()
{
    mPtr = NULL;
    mpNext = NULL;
}