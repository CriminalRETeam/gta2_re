#pragma once

#include "Function.hpp"
#include <memory.h>
#include <wchar.h>

struct ProfileAverage
{
    // 9.6f 0x45B0F0
    ProfileAverage()
    {
        field_7C_sum = 0;
        field_78_sample_idx = 0;
        for (s32 i = 0; i < 30; i++)
        {
            field_0_samples[i] = 0;
        }
    }

    s32 Average() const
    {
        return field_7C_sum / 30;
    }

    // 9.6f 0x474450. Keeps a running sum of the last 30 samples
    void AddSample(s32 value)
    {
        field_7C_sum += value - field_0_samples[field_78_sample_idx];
        field_0_samples[field_78_sample_idx] = value;
        if (++field_78_sample_idx >= 30)
        {
            field_78_sample_idx = 0;
        }
    }

    s32 field_0_samples[30]; // 0x0
    s32 field_78_sample_idx; // 0x78
    s32 field_7C_sum; // 0x7C
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
        field_0_start_time = 0;
        field_4_end_time = 0;
    }
    // 9.6f 0x474430
    s32 TakeElapsed()
    {
        s32 elapsed = field_4_end_time - field_0_start_time;
        init();
        return elapsed;
    }

    s32 field_0_start_time; // 0x0
    s32 field_4_end_time; // 0x4
};
GTA2_ASSERT_SIZEOF_ALWAYS(ProfileStopwatch, 0x8);

class FrameProfiler
{
  public:
    FrameProfiler()
    {
        Init_4FF1B0();
    }

    EXPORT void Init_4FF1B0();
    EXPORT s32 sDrawFlatRect_4FF1C0(f32 left, f32 top, f32 right, f32 bottom, s32 colour);
    EXPORT void DebugDrawProfiling_4FF250();
    EXPORT static void sDrawString_4FF910(s32 xpos, s32 ypos, const wchar_t* pFormat, ...);
    EXPORT static void __stdcall GetMemTotal_4FF970(u32* pValue);
    EXPORT void Service_4FF980();
    EXPORT void StartProfileTimer_4FF990(u32 idx);
    EXPORT void StopProfileTimer_4FF9F0(u32 idx);
    EXPORT void DebugPrintf_4FFA50(const char_type* pFormat, ...);
    EXPORT void UpdateProfiling_4FFA90();
    EXPORT void DrawProfileHistory_4FFD90();

    inline void DrawProfileBar(s32 x, s32 value, s32 colour)
    {
        // right is 630 - fx + 1 rather than left + 1: VC6 keeps fx on the x87 stack for the second
        // use, then CSEs the whole (630 - fx) and pops the dead fx
        f32 fx = static_cast<f32>(x);
        f32 left = 630.0f - fx;
        f32 right = 630.0f - fx + 1.0f;
        sDrawFlatRect_4FF1C0(left, 478.0f - (value * 4), right, 478.0f, colour);
    }

    ProfileStopwatch field_0_process_watch; // 0x0
    ProfileStopwatch field_8_draw_watch; // 0x8
    ProfileStopwatch field_10_unused_watch; // 0x10
    ProfileStopwatch field_18_input_watch; // 0x18
    ProfileStopwatch field_20_audio_watch; // 0x20

    ProfileAverage field_28_process_avg; // 0x28
    ProfileAverage field_A8_draw_avg; // 0xA8
    ProfileAverage field_128_render_avg; // 0x128
    ProfileAverage field_1A8_input_avg; // 0x1A8
    ProfileAverage field_228_audio_avg; // 0x228

    s32 field_2A8_process_history[100]; // 0x2A8
    s32 field_438_draw_history[100]; // 0x438
    s32 field_5C8_render_history[100]; // 0x5C8
    s32 field_758_input_history[100]; // 0x758
    s32 field_8E8_audio_history[100]; // 0x8E8
    s32 field_A78_history_idx; // 0xA78
    s32 field_A7C_count; // 0xA7C
};
GTA2_ASSERT_SIZEOF_ALWAYS(FrameProfiler, 0xA80);

EXTERN_GLOBAL(FrameProfiler*, gMike_A80_6F7328);
