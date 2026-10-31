#pragma once

#include "Function.hpp"
#include <memory.h>
#include <wchar.h>

struct ProfileAverage
{
    // 9.6f 0x45B0F0
    ProfileAverage()
    {
        mSum = 0;
        mSampleIdx = 0;
        for (s32 i = 0; i < 30; i++)
        {
            mSamples[i] = 0;
        }
    }

    s32 Average() const
    {
        return mSum / 30;
    }

    // 9.6f 0x474450. Keeps a running sum of the last 30 samples
    void AddSample(s32 value)
    {
        mSum += value - mSamples[mSampleIdx];
        mSamples[mSampleIdx] = value;
        if (++mSampleIdx >= 30)
        {
            mSampleIdx = 0;
        }
    }

    s32 mSamples[30]; // 0x0
    s32 mSampleIdx; // 0x78
    s32 mSum; // 0x7C
};
GTA2_ASSERT_SIZEOF_ALWAYS(ProfileAverage, 0x80);

struct ProfileStopwatch
{
    ProfileStopwatch()
    {
        init();
    }

    void init()
    {
        mStartTime = 0;
        mEndTime = 0;
    }
    // 9.6f 0x474430
    s32 TakeElapsed()
    {
        s32 elapsed = mEndTime - mStartTime;
        init();
        return elapsed;
    }

    s32 mStartTime; // 0x0
    s32 mEndTime; // 0x4
};
GTA2_ASSERT_SIZEOF_ALWAYS(ProfileStopwatch, 0x8);

class FrameProfiler
{
  public:
    FrameProfiler()
    {
        Init();
    }

    EXPORT void Init();
    EXPORT s32 sDrawFlatRect(f32 left, f32 top, f32 right, f32 bottom, s32 colour);
    EXPORT void DebugDrawProfiling();
    EXPORT static void sDrawString(s32 xpos, s32 ypos, const wchar_t* pFormat, ...);
    EXPORT static void __stdcall GetMemTotal(u32* pValue);
    EXPORT void Service();
    EXPORT void StartProfileTimer(u32 idx);
    EXPORT void StopProfileTimer(u32 idx);
    EXPORT void DebugPrintf(const char_type* pFormat, ...);
    EXPORT void UpdateProfiling();
    EXPORT void DrawProfileHistory();

    inline void DrawProfileBar(s32 x, s32 value, s32 colour)
    {
        // right is 630 - fx + 1 rather than left + 1: VC6 keeps fx on the x87 stack for the second
        // use, then CSEs the whole (630 - fx) and pops the dead fx
        f32 fx = static_cast<f32>(x);
        f32 left = 630.0f - fx;
        f32 right = 630.0f - fx + 1.0f;
        sDrawFlatRect(left, 478.0f - (value * 4), right, 478.0f, colour);
    }

    ProfileStopwatch mProcessWatch; // 0x0
    ProfileStopwatch mDrawWatch; // 0x8
    ProfileStopwatch mUnusedWatch; // 0x10
    ProfileStopwatch mInputWatch; // 0x18
    ProfileStopwatch mAudioWatch; // 0x20

    ProfileAverage mProcessAvg; // 0x28
    ProfileAverage mDrawAvg; // 0xA8
    ProfileAverage mRenderAvg; // 0x128
    ProfileAverage mInputAvg; // 0x1A8
    ProfileAverage mAudioAvg; // 0x228

    s32 mProcessHistory[100]; // 0x2A8
    s32 mDrawHistory[100]; // 0x438
    s32 mRenderHistory[100]; // 0x5C8
    s32 mInputHistory[100]; // 0x758
    s32 mAudioHistory[100]; // 0x8E8
    s32 mHistoryIdx; // 0xA78
    s32 mCount; // 0xA7C
};
GTA2_ASSERT_SIZEOF_ALWAYS(FrameProfiler, 0xA80);

EXTERN_GLOBAL(FrameProfiler*, gMike_A80_6F7328);
