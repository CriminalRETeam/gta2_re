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
    EXPORT bool DisableA3DSplash();

  public:
    EXPORT bool SoundInit(s32* not_used);

  private:
    EXPORT bool OpenDigitalDriver(bool b16Bit, bool bStereo, s32 sampleRate);

  public:
    EXPORT bool AllocSamples(s32 not_used);

    EXPORT void PlaySampleRange(u32 idx, u32 end_idx);

    EXPORT bool Init3DProvider(u8* pMaxSamples);

  private:
    EXPORT void Reset3DSamples();

    EXPORT void AllocSample();

    EXPORT void ReleaseSample();

    EXPORT void Terminate();

  public:
    EXPORT void Shutdown();

    EXPORT s32 GetPlayBackRate(s32 idx) const;

    EXPORT s32 GetRandomDisplacement(s32 idx) const;

    EXPORT s32 GetLoopStart(s32 sample_idx) const;

    EXPORT s32 GetLoopEnd(s32 sample_idx) const;

    EXPORT s32 GetSampleLength(s32 sample_idx) const;

    EXPORT void InitialiseChannel(s32 channel, s32 sample_idx);

    EXPORT void SetChannelVolume(s32 channel, s32 volume);

    EXPORT void SetChannelPan(s32 channel, s32 pan);

    EXPORT void SetChannelFrequency(s32 channel, s32 frequency);

    EXPORT void SetChannelLoopPoints(s32 channel, s32 startOffset, s32 endOffset);

    EXPORT void SetChannelLoopCount(s32 channel, s32 loopCount);

    EXPORT bool GetChannelUsedFlag(s32 channel) const;

    EXPORT void StartChannel(s32 channel);

    EXPORT void StopChannel(s32 channel);

    EXPORT bool InitialiseChannel3D(s32 channel, s32 nSfx, s32 rate);

    EXPORT void SetChannel3DVolume(s32 idx, s32 vol);

    EXPORT void SetChannel3DPosition(s32 channel, f32 x, f32 y, f32 z);

    EXPORT void SetChannel3DDistances(s32 channel, f32 maxDist, f32 minDist);

    EXPORT void SetChannel3DFrequency(s32 channel, s32 freq);

    EXPORT void SetChannel3DLoopPoints(s32 channel, s32 loop_start, s32 loop_end);

    EXPORT void SetChannel3DLoopCount(s32 channel, s32 loop_count);

    EXPORT bool GetChannel3DUsedFlag(s32 channel) const;

    EXPORT void StartChannel3D(s32 samp_idx);

    EXPORT void StopChannel3D(s32 samp_idx);

    EXPORT u8 Get3DSampleCount() const;

  private:
    EXPORT void SetEaxEnvironment(s32 env_idx);

    EXPORT bool Open3DProvider(s32 provider_idx);

    EXPORT void Close3DProvider();

    EXPORT void Enum3DProviders();

  public:
    // order func

    EXPORT bool StreamStatus() const;

    EXPORT void StreamSetVolume(u8 vol);

    EXPORT void OpenStream(u32 stream_idx);

    EXPORT void CloseStream();

    EXPORT void FadeOut();

    EXPORT bool MusicFileExists() const;

    EXPORT void PlayVocal(s32 stream_idx, s32 voc_idx, bool bMainVariant);

    EXPORT void CloseVocalStream(s32 stream_idx);

    EXPORT void SetVocalVolume(s32 stream_idx, u8 vol);

    EXPORT void SetVocalSpeed(s32 stream_idx, s32 playbackRate);

    EXPORT s32 GetVocalSpeed(s32 stream_idx) const;

    EXPORT void SetVocalPosMs(s32 stream_idx, s32 ms_pos);

    EXPORT s32 GetVocalPosMs(long stream_idx) const;

    EXPORT s32 GetVocalLengthMs(long stream_idx) const;

    EXPORT void SetSampleVol(u8 vol);

    EXPORT void PlayAtIdx(s32 idx);

    EXPORT bool SampleNotDone() const;
    
    EXPORT void EndSample_58E8A0();

    EXPORT void EndSample_58E960();

    EXPORT bool LoadWavSdtData(const char_type* pRawOrSdtName);

    EXPORT bool Reacquire();

    EXPORT void Release();

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