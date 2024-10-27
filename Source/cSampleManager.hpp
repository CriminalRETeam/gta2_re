#pragma once

#include "3rdParty/mss.h"
#include "types.hpp"

struct sdt_entry_0x18
{
    s32 field_0_offset;
    s32 field_4_sample_length;
    s32 field_8_playBackRate;
    s32 field_C; // enable random displacement ?
    s32 field_10; // loop start?
    s32 field_14; // loop end? -1 if no loop?
};

class cSampleManager
{
public:
    EXPORT cSampleManager();

    EXPORT char_type sub_58D620();

    EXPORT char_type SoundInit_58D6C0(s32 *a2);

    EXPORT char_type sub_58D720(char_type a2, char_type a3, s32 sampleRate);

    // todo: order

    EXPORT char_type AllocSamples_58D9F0(s32 a2);

    EXPORT void sub_58E8C0(u32 idx, u32 a3);

    EXPORT char_type sub_58D820(BYTE *pMaxSamples);

    EXPORT void Reset3DSamples_58D960();

    EXPORT void AllocSample_58DA80();

    EXPORT void ReleaseSample_58DAC0();

    EXPORT void Terminate_58DAE0();

    EXPORT void Shutdown_58DB30();

    EXPORT s32 GetPlayBackRateIdx_58DBF0(s32 idx);

    EXPORT s32 sub_58DC10(s32 idx);

    EXPORT s32 sub_58DC30(s32 a2);

    EXPORT s32 sub_58DC50(s32 a2);

    EXPORT s32 GetSampleLength_maybe_58DC70(s32 a2);

    EXPORT void sub_58DC90(s32 channel, s32 a3);

    EXPORT void SetChannelVolume_58DCE0(s32 channel, s32 volume);

    EXPORT void SetChannelPan_58DD00(s32 channel, s32 pan);

    EXPORT void SetChannelFrequency_58DD20(s32 channel, s32 frequency);

    EXPORT void SetChannelLoopPoints_58DD40(s32 channel, s32 startOffset, s32 endOffset);

    EXPORT void SetChannelLoopCount_58DD60(s32 channel, s32 loopCount);

    EXPORT bool GetChannelUsedFlag_58DD80(s32 channel);

    EXPORT void StartChannel_58DDB0(s32 channel);

    EXPORT void StopChannel_58DDD0(s32 channel);

    EXPORT bool InitialiseChannel3D_58DDF0(s32 channel, s32 nSfx, s32 rate);

    EXPORT void SetChannel3DVolume_58DE80(s32 idx, s32 vol);

    EXPORT void SetChannel3DPosition_58DEA0(s32 channel, f32 x, f32 y, f32 z);

    EXPORT void SetChannel3DDistances_58DED0(s32 a2, f32 a3, f32 a4);

    EXPORT void SetChannel3DFrequency_58DF00(s32 a2, s32 a3);

    EXPORT void SetChannel3DLoopPoints_58DF20(s32 a2, s32 a3, s32 a4);

    EXPORT void SetChannel3DLoopCount_58DF50(s32 channel, s32 a3);

    EXPORT bool GetChannel3DUsedFlag_58DF70(s32 a2);

    EXPORT void StartChannel3D_58DFA0(s32 samp_idx);

    EXPORT void StopChannel3D_58DFC0(s32 samp_idx);

    EXPORT void sub_58E010(s32 a2);

    EXPORT char_type sub_58E140(s32 a2);

    // todo order
    EXPORT void Close3DProvider_58E1C0();

    // todo order
    EXPORT void Enum3DProviders_58E1F0();

    // order func

    EXPORT bool StreamStatus_58E2C0();

    EXPORT void StreamSetVolume_58E2F0(u8 vol);

    EXPORT void OpenStream_58E320(u32 a2);

    EXPORT void CloseStream_58E460();

    EXPORT void FadeOut_58E490();

    EXPORT char_type MusicFileExists_58E500();

    EXPORT void PlayVocal_58E510(s32 stream_idx, s32 voc_idx, char_type bAppendA);

    EXPORT void CloseVocalStream_58E6A0(s32 stream_idx);

    EXPORT void SetVocalVolume_58E6D0(s32 stream_idx, u8 vol);

    EXPORT void SetVocalSpeed_58E700(s32 stream_idx, s32 playbackRate);

    EXPORT s32 GetVocalSpeed_58E720(s32 stream_idx);

    EXPORT void SetVocalPosMs_58E750(s32 stream_idx, s32 ms_pos);

    EXPORT s32 GetVocalPosMs_58E770(long stream_idx);

    EXPORT s32 GetVocalLengthMs_58E7A0(long stream_idx);

    EXPORT void SetSampleVol_58E7D0(u8 vol);

    EXPORT void PlayAtIdx_58E7F0(s32 idx);

    EXPORT bool SampleNotDone_58E880();

    EXPORT void EndSample_58E960();

    EXPORT char_type LoadWavSdtData_58E980(const char_type *pRawOrSdtName);

    EXPORT BYTE sub_58E2A0();

    EXPORT void Release_58E290();

    HDIGDRIVER field_0_hDriver;
    char_type field_4_gtaAudioDriveLetter;
    char_type field_5_str[80];
    bool field_55_bMusicLoaded;
    short field_56_pad;
    HSAMPLE field_58_hSamples[16];
    HSAMPLE field_98_hSample;
    HSTREAM field_9C_hStreams[2];
    char_type field_A4_bLoaded;
    char_type field_A5_pad;
    char_type field_A6_pad;
    char_type field_A7_pad;
    sdt_entry_0x18 field_A8_sdt_entries[320];
    void* field_1EA8_pAudioBuffer1;
    void* field_1EAC_pAudioBuffer2;
    u8 field_1EB0_count_samples;
    u8 field_1EB1_unknown;
    u8 field_1EB2_3d_samp_count;
    char_type field_1EB3_pad;
    HPROVIDER field_1EB4_h3dProvider[256];
    char_type *field_22B4_str[256];
    HPROVIDER field_26B4_env_idx;
    s32 field_26B8_bEaxSupported;
    s32 field_26BC_k17;
    s32 field_26C0_3d_provider;
    H3DSAMPLE field_26C4_3d_sample[16];
    f32 field_2704_float;
    f32 field_2708_float;
    f32 field_270C_float;
    s32 field_2710_3d_provider_count;
    char_type field_2714_bUnknown;
    char_type field_2715_pad;
    char_type field_2716_pad;
    char_type field_2717_pad;

};

extern cSampleManager gSampManager_6FFF00;
