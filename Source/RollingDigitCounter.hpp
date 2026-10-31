#pragma once

#include "Function.hpp"

class RollingDigitCounter
{
  public:
    enum
    {
        kNumDigits = 9,
        kDigitAfterNine = '9' + 1, // the glyph rows are 0-9 then a blank row, see DrawDigits
    };

    EXPORT RollingDigitCounter();
    EXPORT void SetupDigitsParams_492110(s16 digit_transition_speed, s32 max_value, s16 palette);
    EXPORT void InitDigitSprites_492150();
    EXPORT void ChangeStatByAmount_4921B0(s32 amount);
    EXPORT void ColorDigits_4921F0(s32 palette_type, s16 palette);
    EXPORT s32 DrawDigitsRightAligned_492260(s32 base_xpos, s32 base_ypos) const;
    EXPORT s32 DrawDigitsLeftAligned_492430(s32 base_xpos, s32 base_ypos) const;
    EXPORT bool IsAnyDigitRolling_4925C0() const;
    EXPORT void UpdateRollingDigits_4925E0();

    // 9.6f 0x4A50B0
    inline void SetValueClamped_4A50B0(s32 value)
    {
        if (value < -mMaxValue)
        {
            mValue = -mMaxValue;
        }
        else if (value > mMaxValue)
        {
            mValue = mMaxValue;
        }
        else
        {
            mValue = value;
        }
    }

    // 9.6f 0x41DC30
    inline s32 get_value() const
    {
        return mValue;
    }

    s32 mValue; // 0x0
    s32 mTargetValue; // 0x4
    bool mbRollingUp; // 0x8
    char_type mShownDigits[10]; // 0x9
    s8 mScrollOffsets[10]; // 0x13
    char_type mTargetDigits[10]; // 0x1D
    u8 mSpriteW; // 0x27
    u8 mSpriteHCalc; // 0x28
    s16 mMaxNumOfDigits; // 0x2A
    s16 mDigitTransitionSpeed; // 0x2C
    u16 mNonUsedDigits; // 0x2E
    s32 mMaxValue; // 0x30
    s16 mFirstDigitTextureIdx; // 0x34
    s16 mSpriteIdx; // 0x36
};
GTA2_ASSERT_SIZEOF_ALWAYS(RollingDigitCounter, 0x38);