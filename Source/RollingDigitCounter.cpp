#include "RollingDigitCounter.hpp"
#include "Draw.hpp"
#include "gtx_0x106C.hpp"
#include "TextureCache.hpp"

// Forward declarations: the functions below are in address order
s32 __stdcall GetMaxNumOfDigits_4F7660(const s32& max_value);

DEFINE_GLOBAL_INIT(Ang16, kAngZero_67B210, Ang16(0), 0x67B210);

MATCH_FUNC(0x4920b0)
RollingDigitCounter::RollingDigitCounter()
{
    for (s32 digit_idx = 0; digit_idx < kNumDigits; digit_idx++)
    {
        mScrollOffsets[digit_idx] = 0;
        mShownDigits[digit_idx] = '0';
        mTargetDigits[digit_idx] = '0';
    }

    mShownDigits[kNumDigits] = 0;
    mValue = 0;
    mTargetValue = -1;
    mbRollingUp = false;
    mSpriteW = -1;
    mSpriteHCalc = -1;
    mMaxNumOfDigits = 0;
    mDigitTransitionSpeed = 0;
    mNonUsedDigits = 0;
    mMaxValue = 0;
    mFirstDigitTextureIdx = 0;
    mSpriteIdx = 0;
}

MATCH_FUNC(0x492110)
void RollingDigitCounter::SetupDigitsParams_492110(s16 digit_transition_speed, s32 max_value, s16 palette)
{
    mSpriteIdx = palette;
    mDigitTransitionSpeed = digit_transition_speed;
    mMaxValue = max_value;
    mMaxNumOfDigits = GetMaxNumOfDigits_4F7660(mMaxValue);
    s8 non_used_digits = kNumDigits;
    non_used_digits -= static_cast<s8>(mMaxNumOfDigits);
    mNonUsedDigits = non_used_digits;
}

MATCH_FUNC(0x492150)
void RollingDigitCounter::InitDigitSprites_492150()
{
    u16 true_sprite_idx = gGtx_0x106C_703DD4->GetSpriteTrueIndex_5AA460(sprite_types_enum::user_6, mSpriteIdx);
    sprite_index* pSpriteIndex = gGtx_0x106C_703DD4->get_sprite_index_5AA440(true_sprite_idx);
    mSpriteW = pSpriteIndex->mWidth;
    mSpriteHCalc = pSpriteIndex->mHeight / 11;
    mFirstDigitTextureIdx = gTextureCache_15D8_705064->RegisterDigits_5B9220(mMaxNumOfDigits, mSpriteIdx);
}

MATCH_FUNC(0x4921b0)
void RollingDigitCounter::ChangeStatByAmount_4921B0(s32 amount)
{
    if (amount > 0)
    {
        if(amount >= mMaxValue)
        {
            mValue = mMaxValue;
        }
        else
        {
            mValue += amount;
            if (mValue > mMaxValue )
            {
                mValue = mMaxValue;
            }
        }
    }
    else
    {
        if(amount > -mMaxValue)
        {
            mValue += amount;
            if(mValue < 0 )
            {
                mValue = 0;
            }
        }
        else
        {
            mValue = 0;
        }
    }
}

MATCH_FUNC(0x4921f0)
void RollingDigitCounter::ColorDigits_4921F0(s32 palette_type, s16 palette)
{
    u16 true_palette;
    if (palette_type == palette_types_enum::sprites_2)
    {
        u16 sprite_idx = gGtx_0x106C_703DD4->GetSpriteTrueIndex_5AA460(sprite_types_enum::user_6, mSpriteIdx);
        true_palette = gGtx_0x106C_703DD4->GetTruePalette_5AA5F0(palette_types_enum::sprites_2, sprite_idx);
    }
    else
    {
        true_palette = gGtx_0x106C_703DD4->GetTruePalette_5AA5F0(palette_type, palette);
    }

    s32 digit_idx = mNonUsedDigits;

    while (digit_idx < kNumDigits)
    {
        gTextureCache_15D8_705064->SetPal_5B9660(mFirstDigitTextureIdx - mNonUsedDigits + digit_idx, true_palette);
        digit_idx++;
    }
}

// https://decomp.me/scratch/6E5vt
MATCH_FUNC(0x492260)
s32 RollingDigitCounter::DrawDigitsRightAligned_492260(s32 base_xpos, s32 base_ypos) const
{
    s32 curr_xpos = base_xpos;
    bool bFirst = true;
    s32 ypos_default = base_ypos + (mSpriteHCalc >> 1);

    // The x of each digit counts back from the last one
    for (s32 idx = mNonUsedDigits; idx < kNumDigits; idx++)
    {
        s32 offset = mScrollOffsets[idx];
        if (bFirst)
        {
            char_type curr_char = mShownDigits[idx];
            if (curr_char == '0' && idx != kNumDigits - 1 && !mScrollOffsets[idx])
            {
                continue;
            }

            u8 height;
            if (mShownDigits[idx] != '0' || idx == kNumDigits - 1)
            {
                height = mSpriteHCalc;
            }
            else
            {
                height = mScrollOffsets[idx];
            }

            u16 v = mSpriteHCalc * (kDigitAfterNine - curr_char) - offset;
            curr_xpos = (mSpriteW >> 1) - mSpriteW * (kNumDigits - idx) + base_xpos;
            s32 ypos = base_ypos + static_cast<s8>(height) / 2;
            DrawTextureScaled_495470(gTextureCache_15D8_705064->GetDigitTexture_5B95F0(idx + mFirstDigitTextureIdx - mNonUsedDigits, v, height),
                       curr_xpos,
                       ypos,
                       mSpriteW,
                       height,
                       kAngZero_67B210,
                       0,
                       0);
            bFirst = false;
        }
        else
        {
            u16 v = mSpriteHCalc * (kDigitAfterNine - mShownDigits[idx]) - offset;
            s32 xpos = (mSpriteW >> 1) - mSpriteW * (kNumDigits - idx) + base_xpos;
            DrawTextureScaled_495470(gTextureCache_15D8_705064->GetDigitTexture_5B95F0(idx + mFirstDigitTextureIdx - mNonUsedDigits,
                                                             v,
                                                             mSpriteHCalc),
                       xpos,
                       ypos_default,
                       mSpriteW,
                       mSpriteHCalc,
                       kAngZero_67B210,
                       0,
                       0);
        }
    }
    return curr_xpos - (mSpriteW >> 1);
}

// Draws the digits left to right, skipping leading zeros, and returns the x after the last digit
MATCH_FUNC(0x492430)
s32 RollingDigitCounter::DrawDigitsLeftAligned_492430(s32 base_xpos, s32 base_ypos) const
{
    // u32: converts with the Fix16(u32) constructor, whose out-of-line copy is 0x4926F0
    bool bFirst = true;
    u32 curr_xpos = base_xpos + (mSpriteW >> 1);
    u32 ypos_default = base_ypos + (mSpriteHCalc >> 1);

    for (s32 idx = mNonUsedDigits; idx < kNumDigits; idx++)
    {
        char_type offset_byte = mScrollOffsets[idx];
        s32 offset = offset_byte;
        if (bFirst)
        {
            if (mShownDigits[idx] == '0' && idx != kNumDigits - 1 && !offset_byte)
            {
                continue;
            }

            u8 height;
            if (mShownDigits[idx] != '0' || idx == kNumDigits - 1)
            {
                height = mSpriteHCalc;
            }
            else
            {
                height = offset_byte;
            }

            u16 v = mSpriteHCalc * (kDigitAfterNine - mShownDigits[idx]) - offset;
            u32 ypos = base_ypos + static_cast<s8>(height) / 2;
            DrawTextureScaled_495470(gTextureCache_15D8_705064->GetDigitTexture_5B95F0(idx + mFirstDigitTextureIdx - mNonUsedDigits, v, height),
                       curr_xpos,
                       ypos,
                       mSpriteW,
                       height,
                       kAngZero_67B210,
                       0,
                       0);
            bFirst = false;
            curr_xpos += mSpriteW;
        }
        else
        {
            u16 v = mSpriteHCalc * (kDigitAfterNine - mShownDigits[idx]) - offset;
            DrawTextureScaled_495470(gTextureCache_15D8_705064->GetDigitTexture_5B95F0(idx + mFirstDigitTextureIdx - mNonUsedDigits,
                                                             v,
                                                             mSpriteHCalc),
                       curr_xpos,
                       ypos_default,
                       mSpriteW,
                       mSpriteHCalc,
                       kAngZero_67B210,
                       0,
                       0);
            curr_xpos += mSpriteW;
        }
    }
    return curr_xpos;
}

MATCH_FUNC(0x4925c0)
bool RollingDigitCounter::IsAnyDigitRolling_4925C0() const
{
    s32 digit_idx = mNonUsedDigits;

    while (digit_idx < kNumDigits)
    {
        if (this->mScrollOffsets[digit_idx] != 0)
        {
            return true;
        }
        digit_idx++;
    }
    return false;
}

MATCH_FUNC(0x4925e0)
void RollingDigitCounter::UpdateRollingDigits_4925E0()
{
    s32 shown_value;
    sscanf(mShownDigits, "%d", &shown_value);

    if (mTargetValue == -1 || shown_value == mTargetValue && !RollingDigitCounter::IsAnyDigitRolling_4925C0())
    {
        if (mValue == shown_value)
        {
            mTargetValue = -1;
            return;
        }
        mTargetValue = mValue;
        if (mValue > shown_value)
        {
            mbRollingUp = true;
        }
        else
        {
            mbRollingUp = false;
        }
        sprintf(mTargetDigits, "%09d", mValue);
    }

    for (s32 idx = mNonUsedDigits; idx < kNumDigits; idx++)
    {
        // idx + 20 is mTargetDigits[idx]; written as an offset from mShownDigits, as in the original
        if (mShownDigits[idx + 20] != mShownDigits[idx] || mScrollOffsets[idx])
        {
            if (mbRollingUp)
            {
                mScrollOffsets[idx] = mDigitTransitionSpeed + mScrollOffsets[idx];
                if (mScrollOffsets[idx] >= mSpriteHCalc)
                {
                    if (mShownDigits[idx] < '9')
                    {
                        mShownDigits[idx]++;
                    }
                    else
                    {
                        mShownDigits[idx] = '0';
                    }
                    mScrollOffsets[idx] = 0;
                }
            }
            else
            {
                mScrollOffsets[idx] = mScrollOffsets[idx] - mDigitTransitionSpeed;
                if (mScrollOffsets[idx] < 0)
                {
                    if (mShownDigits[idx] > '0')
                    {
                        mShownDigits[idx]--;
                    }
                    else
                    {
                        mShownDigits[idx] = '9';
                    }
                    mScrollOffsets[idx] = mSpriteHCalc - mDigitTransitionSpeed;
                }
            }
        }
    }
}

MATCH_FUNC(0x4f7660)
s32 __stdcall GetMaxNumOfDigits_4F7660(const s32& max_value)
{
    s32 num_digits = 1;
    s32 abs_value = max_value;

    if (abs_value < 0)
    {
        abs_value = -abs_value;
    }
    for (; 10 <= abs_value; abs_value /= 10, num_digits++)
    {}
    return num_digits;
}