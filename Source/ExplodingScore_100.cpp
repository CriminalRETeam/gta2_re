#include "ExplodingScore_100.hpp"
#include "Camera.hpp"
#include "Game.hpp"
#include "Globals.hpp"
#include "Player.hpp"
#include <memory.h>

DEFINE_GLOBAL(ExplodingScorePool*, gExplodingScorePool, 0x702F34);

DEFINE_GLOBAL_INIT(Fix16, kFp160_702C6C, Fix16(160), 0x702C6C); // Fix16(640) / Fix16(4) (the global 0x702DF0)
DEFINE_GLOBAL_INIT(Fix16, kFp255_702BC4, Fix16(255), 0x702BC4);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFp480, Fix16(480), 0x702F10); // 0xA00000 - kFp160_702C6C
DEFINE_GLOBAL_INIT(Fix16, kFp120_702C74, Fix16(120), 0x702C74); // Fix16(480) / Fix16(4) (the global 0x702DF0)
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFp360, Fix16(360), 0x702C08); // 0x780000 - kFp120_702C74

DEFINE_GLOBAL(Fix16, kFpZero_702DE0, 0x702DE0);
DEFINE_GLOBAL(Ang16, kAng0_702F24, 0x702F24);

MATCH_FUNC(0x5967e0)
ExplodingScorePool::ExplodingScorePool()
{
    mLastPalette = 9;
    mFreeScores = 3;
}

MATCH_FUNC(0x596840)
ExplodingScorePool::~ExplodingScorePool()
{
}

MATCH_FUNC(0x596860)
s16 ExplodingScorePool::NextPalette()
{
    mLastPalette++;
    if (mLastPalette > 13u)
    {
        mLastPalette = 9;
    }
    return mLastPalette;
}

MATCH_FUNC(0x596880)
void ExplodingScorePool::IncrementFreeScores()
{
    mFreeScores++;
}

MATCH_FUNC(0x596890)
void ExplodingScorePool::PushScore(Fix16 xpos, Fix16 ypos, Fix16 zpos, u32 score)
{
    if (this->mFreeScores == 0)
    {
        ExplodingScore* pIter = mPool.GetFirstUsed_4B8FD0();
        while (pIter)
        {
            if (static_cast<u32>(pIter->mScore) >= score)
            {
                pIter = pIter->mpNext;
            }
            else
            {
                mPool.FindAndDeAllocate(pIter);
                ++this->mFreeScores;
                break;
            }
        }
    }

    if (this->mFreeScores > 0)
    {
        ExplodingScore* pNew = mPool.Allocate();
        if (pNew)
        {
            --this->mFreeScores;
            pNew->InitScore(xpos, ypos, zpos, score);
        }
    }
}

MATCH_FUNC(0x596940)
void ExplodingScorePool::Update()
{
    mPool.UpdatePool();
}

MATCH_FUNC(0x5969e0)
void ExplodingScorePool::DrawExplodingScores()
{
    ExplodingScore* pIter = mPool.GetFirstUsed_4B8FD0();
    while (pIter)
    {
        pIter->DrawNumbers();
        pIter = pIter->mpNext;
    }
}

MATCH_FUNC(0x596a00)
ExplodingScore::ExplodingScore()
{
    mHighestDigitIdx = 0;
    for (s32 i = 0; i < 9; i++)
    {
        mDigits[i] = 0;
    }
    mAlpha = 0;
    mExplodeCounter = 0;
    mXOffsetMult = 0;
    mYOffsetMult = 0;
    mScore = 0;
    mUnused48 = 0;
    mpNext = NULL;
    mPalette = 9;
}

MATCH_FUNC(0x596a40)
ExplodingScore::~ExplodingScore()
{
    mpNext = NULL;
}

MATCH_FUNC(0x596a50)
void ExplodingScore::PoolAllocate()
{
}

MATCH_FUNC(0x596a60)
void ExplodingScore::PoolDeallocate()
{
}

MATCH_FUNC(0x596a70)
void ExplodingScore::ClearNumbersArrayFrom(s32 start_idx)
{
    for (u32 i = start_idx; i > 0; i--)
    {
        mDigits[i] = 0;
    }
}

MATCH_FUNC(0x596a90)
void ExplodingScore::InitScore(Fix16 xpos, Fix16 ypos, Fix16 zpos, u32 score)
{
    u32 remaining_score = score;

    this->mScore = score;
    this->mHighestDigitIdx = 0;

    if (score >= 100000000)
    {
        this->mHighestDigitIdx = 8;
        this->mDigits[8] = score / 100000000;
        ClearNumbersArrayFrom(7);
        remaining_score = score % 100000000;
    }

    if (remaining_score >= 10000000)
    {
        if (!this->mHighestDigitIdx)
        {
            this->mHighestDigitIdx = 7;
        }
        this->mDigits[7] = remaining_score / 10000000;
        ClearNumbersArrayFrom(6);
        remaining_score %= 10000000u;
    }

    if (remaining_score >= 1000000)
    {
        if (!this->mHighestDigitIdx)
        {
            this->mHighestDigitIdx = 6;
        }
        this->mDigits[6] = remaining_score / 1000000;
        ClearNumbersArrayFrom(5);
        remaining_score %= 1000000u;
    }

    if (remaining_score >= 100000)
    {
        if (!this->mHighestDigitIdx)
        {
            this->mHighestDigitIdx = 5;
        }
        this->mDigits[5] = remaining_score / 100000;
        ClearNumbersArrayFrom(4);
        remaining_score %= 100000u;
    }

    if (remaining_score >= 10000)
    {
        if (!this->mHighestDigitIdx)
        {
            this->mHighestDigitIdx = 4;
        }
        this->mDigits[4] = remaining_score / 10000;
        ClearNumbersArrayFrom(3);
        remaining_score %= 10000u;
    }

    if (remaining_score >= 1000)
    {
        if (!this->mHighestDigitIdx)
        {
            this->mHighestDigitIdx = 3;
        }
        this->mDigits[3] = remaining_score / 1000;
        ClearNumbersArrayFrom(2);
        remaining_score %= 1000u;
    }

    if (remaining_score >= 100)
    {
        if (!this->mHighestDigitIdx)
        {
            this->mHighestDigitIdx = 2;
        }
        this->mDigits[2] = remaining_score / 100;
        ClearNumbersArrayFrom(1);
        remaining_score %= 100u;
    }

    if (remaining_score >= 10)
    {
        if (!this->mHighestDigitIdx)
        {
            this->mHighestDigitIdx = 1;
        }
        this->mDigits[1] = remaining_score / 10;
        this->mDigits[0] = remaining_score % 10;
    }
    else
    {
        this->mDigits[0] = remaining_score;
    }

    this->mX = xpos;
    this->mY = ypos;
    this->mZ = zpos;

    this->mPalette = gExplodingScorePool->NextPalette();

    this->mAlpha = 25;
    this->mExplodeCounter = 2;
    this->mXOffsetMult = 1;
    this->mYOffsetMult = 1;
}

MATCH_FUNC(0x596c90)
void ExplodingScore::DrawNumbers()
{
    Fix16 screen_x;
    Fix16 screen_y;
    Fix16 x_delta;
    Fix16 y_delta;
    Camera* pCamera = &gGame_0x40_67E008->mLocalPlayer->mViewCamera;
    pCamera->ProjectWorldToScreen_OutOfLine_4B90E0(mX, mY, mZ, &screen_x, &screen_y);
    if (screen_x < kFp160_702C6C)
    {
        screen_x = kFp160_702C6C - screen_x;
        x_delta = screen_x / kFp255_702BC4;
        mX += x_delta;
    }
    else if (screen_x > kFp480)
    {
        screen_x = kFp480 - screen_x;
        x_delta = screen_x / kFp255_702BC4;
        mX += x_delta;
    }

    if (screen_y < kFp120_702C74)
    {
        screen_y = kFp120_702C74 - screen_y;
        y_delta = screen_y / kFp255_702BC4;
        mY += y_delta;
    }
    else if (screen_y > kFp360)
    {
        screen_y = kFp360 - screen_y;
        y_delta = screen_y / kFp255_702BC4;
        mY += y_delta;
    }

    switch (this->mHighestDigitIdx)
    {
        case 0:
            DrawSingleNumber(this->mDigits[0], 10);
            break;

        case 1:
            DrawSingleNumber(this->mDigits[1], 21);
            DrawSingleNumber(this->mDigits[0], 0);
            break;

        case 2:
            DrawSingleNumber(this->mDigits[2], 31);
            DrawSingleNumber(this->mDigits[1], 10);
            DrawSingleNumber(this->mDigits[0], -11);
            break;

        case 3:
            DrawSingleNumber(this->mDigits[3], 42);
            DrawSingleNumber(this->mDigits[2], 21);
            DrawSingleNumber(this->mDigits[1], 0);
            DrawSingleNumber(this->mDigits[0], -21);
            break;

        case 4:
            DrawSingleNumber(this->mDigits[4], 52);
            DrawSingleNumber(this->mDigits[3], 31);
            DrawSingleNumber(this->mDigits[2], 10);
            DrawSingleNumber(this->mDigits[1], -11);
            DrawSingleNumber(this->mDigits[0], -32);
            break;

        case 5:
            DrawSingleNumber(this->mDigits[5], 63);
            DrawSingleNumber(this->mDigits[4], 42);
            DrawSingleNumber(this->mDigits[3], 21);
            DrawSingleNumber(this->mDigits[2], 0);
            DrawSingleNumber(this->mDigits[1], -21);
            DrawSingleNumber(this->mDigits[0], -42);
            break;

        case 6:
            DrawSingleNumber(this->mDigits[6], 73);
            DrawSingleNumber(this->mDigits[5], 52);
            DrawSingleNumber(this->mDigits[4], 31);
            DrawSingleNumber(this->mDigits[3], 10);
            DrawSingleNumber(this->mDigits[2], -11);
            DrawSingleNumber(this->mDigits[1], -32);
            DrawSingleNumber(this->mDigits[0], -53);
            break;

        case 7:
            DrawSingleNumber(this->mDigits[7], 84);
            DrawSingleNumber(this->mDigits[6], 63);
            DrawSingleNumber(this->mDigits[5], 42);
            DrawSingleNumber(this->mDigits[4], 21);
            DrawSingleNumber(this->mDigits[3], 0);
            DrawSingleNumber(this->mDigits[2], -21);
            DrawSingleNumber(this->mDigits[1], -42);
            DrawSingleNumber(this->mDigits[0], -63);
            break;

        case 8:
            DrawSingleNumber(this->mDigits[8], 94);
            DrawSingleNumber(this->mDigits[7], 73);
            DrawSingleNumber(this->mDigits[6], 52);
            DrawSingleNumber(this->mDigits[5], 31);
            DrawSingleNumber(this->mDigits[4], 10);
            DrawSingleNumber(this->mDigits[3], -11);
            DrawSingleNumber(this->mDigits[2], -32);
            DrawSingleNumber(this->mDigits[1], -53);
            DrawSingleNumber(this->mDigits[0], -74);
            break;

        default:
            return;
    }
}

MATCH_FUNC(0x5970c0)
bool ExplodingScore::PoolUpdate()
{
    if (mExplodeCounter < 0)
    {
        mXOffsetMult++;
        mYOffsetMult++;

        if (mAlpha > 2u)
        {
            mAlpha -= 2;
            mExplodeCounter--;
            return false;
        }
        else
        {
            mAlpha = 0;
            gExplodingScorePool->IncrementFreeScores();
            return true;
        }
    }
    else
    {
        mExplodeCounter--;
        return false;
    }
}

// 9.6f 0x4B92B0
MATCH_FUNC(0x597100)
void ExplodingScore::DrawSingleNumber(s32 number_to_draw, s32 xpos_sub)
{

    Camera* pCam = &gGame_0x40_67E008->mLocalPlayer->mViewCamera;

    Fix16 proj_x;
    Fix16 proj_y;

    s32 x_base;
    switch (number_to_draw)
    {
        case 1:
            x_base = 4;
            break;
        default:
            x_base = 0;
            break;
    }

    pCam->ProjectWorldToScreen_4B90E0(mX, mY, mZ, &proj_x, &proj_y);

    Fix16 base_scale;
    s32 x_off;
    s32 y_off;
    if (mExplodeCounter < 0)
    {
        x_off = this->mXOffsetMult * (x_base - xpos_sub);
        y_off = -7 * this->mYOffsetMult;
        base_scale = Fix16(-mExplodeCounter);
    }
    else
    {
        x_off = x_base - xpos_sub;
        y_off = -7;
        base_scale = kFpOne_702DE4;
    }

    Fix16 x_to_use = proj_x + Fix16(x_off);
    Fix16 y_to_use = proj_y + Fix16(y_off);

    if (x_to_use >= kFpZero_702DE0)
    {
        if (x_to_use <= Fix16(640))
        {
            if (y_to_use >= kFpZero_702DE0)
            {
                if (y_to_use <= Fix16(480))
                {
                    s32 palette_type = palette_types_enum::user_remaps_7;
                    Fix16 finalScale = pCam->mUiScale * base_scale;
                    DrawFigure(sprite_types_enum::user_6,
                                      number_to_draw + 163,
                                      x_to_use,
                                      y_to_use,
                                      kAng0_702F24,
                                      finalScale,
                                      palette_type,
                                      this->mPalette,
                                      1,
                                      this->mAlpha,
                                      true);
                }
            }
        }
    }
}
