#pragma once

#include "3rdParty/mss.h"
#include "Function.hpp"

struct SdtEntry
{
    s32 mOffset; // 0x0
    s32 mSampleLength; // 0x4
    s32 mPlayBackRate; // 0x8
    s32 mRandomDisplacement; // 0xC
    s32 mLoopStart; // 0x10
    s32 mLoopEnd; // 0x14: -1 = end of sample
};
GTA2_ASSERT_SIZEOF_ALWAYS(SdtEntry, 0x18);

class cSampleManager
{
  public:
    EXPORT cSampleManager();

  private:
    EXPORT bool DisableA3DSplash_58D620();

  public:
    EXPORT bool SoundInit_58D6C0(s32* not_used);

  private:
    EXPORT bool OpenDigitalDriver_58D720(bool b16Bit, bool bStereo, s32 sampleRate);

  public:
    EXPORT bool AllocSamples_58D9F0(s32 not_used);

    EXPORT void PlaySampleRange_58E8C0(u32 idx, u32 end_idx);

    EXPORT bool Init3DProvider_58D820(u8* pMaxSamples);

  private:
    EXPORT void Reset3DSamples_58D960();

    EXPORT void AllocSample_58DA80();

    EXPORT void ReleaseSample_58DAC0();

    EXPORT void Terminate_58DAE0();

  public:
    EXPORT void Shutdown_58DB30();

    EXPORT s32 GetPlayBackRate_58DBF0(s32 idx) const;

    EXPORT s32 GetRandomDisplacement_58DC10(s32 idx) const;

    EXPORT s32 GetLoopStart_58DC30(s32 sample_idx) const;

    EXPORT s32 GetLoopEnd_58DC50(s32 sample_idx) const;

    EXPORT s32 GetSampleLength_58DC70(s32 sample_idx) const;

    EXPORT void InitialiseChannel_58DC90(s32 channel, s32 sample_idx);

    EXPORT void SetChannelVolume_58DCE0(s32 channel, s32 volume);

    EXPORT void SetChannelPan_58DD00(s32 channel, s32 pan);

    EXPORT void SetChannelFrequency_58DD20(s32 channel, s32 frequency);

    EXPORT void SetChannelLoopPoints_58DD40(s32 channel, s32 startOffset, s32 endOffset);

    EXPORT void SetChannelLoopCount_58DD60(s32 channel, s32 loopCount);

    EXPORT bool GetChannelUsedFlag_58DD80(s32 channel) const;

    EXPORT void StartChannel_58DDB0(s32 channel);

    EXPORT void StopChannel_58DDD0(s32 channel);

    EXPORT bool InitialiseChannel3D_58DDF0(s32 channel, s32 nSfx, s32 rate);

    EXPORT void SetChannel3DVolume_58DE80(s32 idx, s32 vol);

    EXPORT void SetChannel3DPosition_58DEA0(s32 channel, f32 x, f32 y, f32 z);

    EXPORT void SetChannel3DDistances_58DED0(s32 channel, f32 maxDist, f32 minDist);

    EXPORT void SetChannel3DFrequency_58DF00(s32 channel, s32 freq);

    EXPORT void SetChannel3DLoopPoints_58DF20(s32 channel, s32 loop_start, s32 loop_end);

    EXPORT void SetChannel3DLoopCount_58DF50(s32 channel, s32 loop_count);

    EXPORT bool GetChannel3DUsedFlag_58DF70(s32 channel) const;

    EXPORT void StartChannel3D_58DFA0(s32 samp_idx);

    EXPORT void StopChannel3D_58DFC0(s32 samp_idx);

    EXPORT u8 Get3DSampleCount_58E000() const;

  private:
    EXPORT void SetEaxEnvironment_58E010(s32 env_idx);

    EXPORT bool Open3DProvider_58E140(s32 provider_idx);

    EXPORT void Close3DProvider_58E1C0();

    EXPORT void Enum3DProviders_58E1F0();

  public:
    // order func

    EXPORT bool StreamStatus_58E2C0() const;

    EXPORT void StreamSetVolume_58E2F0(u8 vol);

    EXPORT void OpenStream_58E320(u32 stream_idx);

    EXPORT void CloseStream_58E460();

    EXPORT void FadeOut_58E490();

    EXPORT bool MusicFileExists_58E500() const;

    EXPORT void PlayVocal_58E510(s32 stream_idx, s32 voc_idx, bool bMainVariant);

    EXPORT void CloseVocalStream_58E6A0(s32 stream_idx);

    EXPORT void SetVocalVolume_58E6D0(s32 stream_idx, u8 vol);

    EXPORT void SetVocalSpeed_58E700(s32 stream_idx, s32 playbackRate);

    EXPORT s32 GetVocalSpeed_58E720(s32 stream_idx) const;

    EXPORT void SetVocalPosMs_58E750(s32 stream_idx, s32 ms_pos);

    EXPORT s32 GetVocalPosMs_58E770(long stream_idx) const;

    EXPORT s32 GetVocalLengthMs_58E7A0(long stream_idx) const;

    EXPORT void SetSampleVol_58E7D0(u8 vol);

    EXPORT void PlayAtIdx_58E7F0(s32 idx);

    EXPORT bool SampleNotDone_58E880() const;
    
    EXPORT void EndSample_58E8A0();

    EXPORT void EndSample_58E960();

    EXPORT bool LoadWavSdtData_58E980(const char_type* pRawOrSdtName);

    EXPORT bool Reacquire_58E2A0();

    EXPORT void Release_58E290();

    HDIGDRIVER mhDriver; // 0x0
    char_type mGtaAudioDriveLetter; // 0x4

  private:
    char_type mStr5[80]; // 0x5
    bool mbMusicFound; // 0x55
    HSAMPLE mhSamples[16]; // 0x58
    HSAMPLE mhSample; // 0x98
    HSTREAM mhStreams[2]; // 0x9C
    bool mbLoaded; // 0xA4
    SdtEntry mSdtEntries[320]; // 0xA8
    void* mpAudioBuffer1; // 0x1EA8
    void* mpAudioBuffer2; // 0x1EAC
    u8 mCountSamples; // 0x1EB0
    u8 mBytesPerSample; // 0x1EB1
    u8 m3dSampCount; // 0x1EB2
    HPROVIDER mH3dProvider[256]; // 0x1EB4
    char_type* mStr22B4[256]; // 0x22B4
    s32 m3dProviderIdx; // 0x26B4
    s32 mbEaxSupported; // 0x26B8
    s32 mEaxEnvironment; // 0x26BC
    s32 m3dProvider; // 0x26C0
    H3DSAMPLE m3dSample[16]; // 0x26C4
    f32 mFloat2704; // 0x2704
    f32 mFloat2708; // 0x2708
    f32 mFloat270C; // 0x270C
    s32 m3dProviderCount; // 0x2710
    bool mIsStereo; // 0x2714
};

EXTERN_GLOBAL(cSampleManager, gSampManager_6FFF00);