#include "cSampleManager.hpp"
#include "Function.hpp"
#include "Globals.hpp"
#include <stdio.h>

DEFINE_GLOBAL(cSampleManager, gSampManager, 0x6FFF00);

struct voc_name
{
    char_type mStr[30]; // 0x0
};

const voc_name gVocNames_5FEA5C[98] = {{"accuracyb.wav"},
                                       {"back2front.wav"},
                                       {"busted.wav"},
                                       {"carjacker.wav"},
                                       {"copkilla.wav"},
                                       {"elvis.wav"},
                                       {"expiditious.wav"},
                                       {"freeze.wav"},
                                       {"frenzyfail.wav"},
                                       {"frenzypassed.wav"},
                                       {"fryingtonite.wav"},
                                       {"gameover.wav"},
                                       {"genocide.wav"},
                                       {"gta.wav"},
                                       {"insanestunt.wav"},
                                       {"jobcomplete.wav"},
                                       {"jobfail.wav"},
                                       {"killfrenzy.wav"},
                                       {"medicalemer.wav"},
                                       {"respectis.wav"},
                                       {"shocking.wav"},
                                       {"somethingscookin.wav"},
                                       {"timesup.wav"},
                                       {"toasted.wav"},
                                       {"wasted.wav"},
                                       {"wipeout.wav"},
                                       {"bombarmed.wav"},
                                       {"macgun.wav"},
                                       {"dualpist.wav"},
                                       {"electro.wav"},
                                       {"flamet.wav"},
                                       {"grenades.wav"},
                                       {"molotovs.wav"},
                                       {"oilslick.wav"},
                                       {"pistolp.wav"},
                                       {"rocketl.wav"},
                                       {"shotgun.wav"},
                                       {"silentmg.wav"},
                                       {"vehbomb.wav"},
                                       {"vehbomb2.wav"},
                                       {"vehmacgun.wav"},
                                       {"vehmines.wav"},
                                       {"armour.wav"},
                                       {"copbribe.wav"},
                                       {"doubledamage.wav"},
                                       {"electrofingers.wav"},
                                       {"fastreload.wav"},
                                       {"getoutjail.wav"},
                                       {"health.wav"},
                                       {"instantgang.wav"},
                                       {"invisibility.wav"},
                                       {"invulnerability.wav"},
                                       {"life.wav"},
                                       {"multiplyer.wav"},
                                       {"respect.wav"},
                                       {"raceover.wav"},
                                       {"2ndlap.wav"},
                                       {"finallap.wav"},
                                       {"raceon.wav"},
                                       {"30peopledown.wav"},
                                       {"60peopledown.wav"},
                                       {"90peopledown.wav"},
                                       {"120peopledown.wav"},
                                       {"150peopledown.wav"},
                                       {"timeout.wav"},
                                       {"timeextended.wav"},
                                       {"timesup.wav"},
                                       {"sorrydidthathurt.wav"},
                                       {"nicework.wav"},
                                       {"choctastic.wav"},
                                       {"raspberryripple.wav"},
                                       {"youshotyourload.wav"},
                                       {"oohdidthathurt.wav"},
                                       {"deathtoicvans.wav"},
                                       {"crispycritter.wav"},
                                       {"youretoastbuddy.wav"},
                                       {"eatleaddeath.wav"},
                                       {"thatsgottahurt.wav"},
                                       {"sorryaboutthat.wav"},
                                       {"xinloimyman.wav"},
                                       {"damnsundaydrivers.wav"},
                                       {"suckitandsee.wav"},
                                       {"tastemywrath.wav"},
                                       {"hallelujah.wav"},
                                       {"damnation.wav"},
                                       {"laugh1.wav"},
                                       {"laugh2.wav"},
                                       {"laugh3.wav"},
                                       {"laugh4.wav"},
                                       {"laugh5.wav"},
                                       {"laugh6.wav"},
                                       {"laugh7.wav"},
                                       {"laugh8.wav"},
                                       {"laugh9.wav"},
                                       {"laugha.wav"},
                                       {"laughb.wav"},
                                       {"laughc.wav"},
                                       {"laughd.wav"}};

MATCH_FUNC(0x58D430)
cSampleManager::cSampleManager()
{
    mpAudioBuffer1 = NULL;
    mpAudioBuffer2 = NULL;

    for (s32 k = 0; k < 16; k++)
    {
        mhSamples[k] = NULL;
    }

    for (s32 j = 0; j < 2; j++)
    {
        mhStreams[j] = NULL;
    }

    for (s32 m = 0; m < 16; m++)
    {
        m3dSample[m] = NULL;
    }

    mbLoaded = false;

    for (s32 i = 0; i < 320; i++)
    {
        mSdtEntries[i].mOffset = 0;
        mSdtEntries[i].mSampleLength = 0;
        mSdtEntries[i].mPlayBackRate = 11025;
        mSdtEntries[i].mRandomDisplacement = 0;
        mSdtEntries[i].mLoopStart = 0;
        mSdtEntries[i].mLoopEnd = -1;
    }

    mhDriver = NULL;
    mCountSamples = 16;
    mBytesPerSample = 2;
    m3dSampCount = 0;
    m3dProviderIdx = -1;
    mbEaxSupported = 0;
    m3dProvider = 0;
    mFloat2704 = -1.0;
    mFloat2708 = -1.0;
    mFloat270C = -1.0;
    mEaxEnvironment = 0;
    m3dProviderCount = 0;
    mIsStereo = false;

    DisableA3DSplash();

    mGtaAudioDriveLetter = 0;

    mbMusicFound = false;

    u32 driveLetter = 'C';
    while (driveLetter <= 'Z')
    {
        char_type driveStr[2];
        driveStr[0] = driveLetter;
        driveStr[1] = 0;
        strcpy(mStr5, driveStr);
        strcat(mStr5, ":\\GTAUDIO\\");

        char_type fileName[80];
        strcpy(fileName, mStr5);
        strcat(fileName, "1.wav");

        FILE* hFile = fopen(fileName, "rb");
        if (hFile)
        {
            fclose(hFile);
            mbMusicFound = true;
            mGtaAudioDriveLetter = driveLetter;
            break;
        }
        driveLetter++;
    }
}

// The original returns an uninitialised bOk when both values are written
#pragma warning(push)
#pragma warning(disable : 4700)
MATCH_FUNC(0x58D620)
bool cSampleManager::DisableA3DSplash()
{
    bool bOk;
    DWORD data;
    HKEY hKey;
    DWORD disposition;

    if (RegCreateKeyExA(HKEY_LOCAL_MACHINE, "Software\\Aureal\\A3D", 0, NULL, 0, KEY_WRITE, NULL, &hKey, &disposition))
    {
        return false;
    }

    data = 0;
    if (RegSetValueExA(hKey, "SplashScreen", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&data), 4u))
    {
        bOk = false;
    }

    data = 0;
    if (RegSetValueExA(hKey, "SplashAudio", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&data), 4u))
    {
        bOk = false;
    }

    RegCloseKey(hKey);
    return bOk;
}
#pragma warning(pop)

MATCH_FUNC(0x58D6C0)
bool cSampleManager::SoundInit(s32* not_used)
{
    AIL_startup();

    if (!OpenDigitalDriver(true, mIsStereo, 22050))
    {
        AIL_shutdown();
        return false;
    }

    Enum3DProviders();

    mCountSamples = 16;
    AllocSamples(1);

    if (mhDriver)
    {
        AIL_set_digital_master_volume(mhDriver, 127);
    }
    return true;
}

MATCH_FUNC(0x58D720)
bool cSampleManager::OpenDigitalDriver(bool b16Bit, bool bStereo, s32 sampleRate)
{
    PCMWAVEFORMAT format;

    if (b16Bit)
    {
        mBytesPerSample = 2;
    }
    else
    {
        mBytesPerSample = 1;
    }

    format.wf.nChannels = 2;
    format.wf.nSamplesPerSec = sampleRate;
    format.wf.nAvgBytesPerSec = 2 * sampleRate * mBytesPerSample;
    format.wf.wFormatTag = WAVE_FORMAT_PCM;
    format.wf.nBlockAlign = static_cast<u8>(2 * mBytesPerSample);
    format.wBitsPerSample = static_cast<u8>(8 * mBytesPerSample);

    AIL_set_preference(1, 37); // DIG_MIXER_CHANNELS
    AIL_set_preference(15, 0); // DIG_USE_WAVEOUT

    if (bStereo)
    {
        AIL_set_preference(33, 1);
    }
    else
    {
        AIL_set_preference(33, 0);
    }

    AIL_set_preference(31, 1); // DIG_ENABLE_RESAMPLE_FILTER

    if (AIL_waveOutOpen(&mhDriver, NULL, -1, &format.wf))
    {
        return false;
    }

    mIsStereo = bStereo;

    AllocSample();

    mpAudioBuffer1 = AIL_mem_alloc_lock(6100000);
    if (!mpAudioBuffer1)
    {
        ReleaseSample();
        AIL_waveOutClose(mhDriver);
        return false;
    }
    mpAudioBuffer2 = mpAudioBuffer1;
    return true;
}

MATCH_FUNC(0x58D820)
bool cSampleManager::Init3DProvider(u8* pMaxSamples)
{
    Terminate();
    Reset3DSamples();

    if (mIsStereo)
    {
        AIL_waveOutClose(mhDriver);
        if (!OpenDigitalDriver(true, false, 22050))
        {
            if (OpenDigitalDriver(true, false, 22050))
            {
                AllocSamples(1);
            }
            return false;
        }
    }

    u32 i;
    for (i = 0; i < 256; i++)
    {
        if (mStr22B4[i])
        {
            if (strncmp(mStr22B4[i], "Microsoft DirectSound3D hardware support", 0x1Eu) == 0 && Open3DProvider(i))
            {
                break;
            }
        }
    }

    if (i < 256)
    {
        if (m3dProvider)
        {
            *pMaxSamples = 0;

            m3dSampCount = 0;
            AIL_3D_provider_attribute(m3dProvider, "Maximum supported samples", pMaxSamples);

            if (*pMaxSamples > 16u)
            {
                *pMaxSamples = 16;
            }

            if (*pMaxSamples < 8u)
            {
                *pMaxSamples = 0;
                Close3DProvider();
                return false;
            }

            for (u32 sampIdx = 0; sampIdx < *pMaxSamples; sampIdx++)
            {
                m3dSample[sampIdx] = AIL_allocate_3D_sample_handle(m3dProvider);
                if (!m3dSample[sampIdx])
                {
                    *pMaxSamples = 0;
                    Close3DProvider();
                    return false;
                }
            }

            m3dSampCount = *pMaxSamples;
            return true;
        }
    }
    return false;
}

MATCH_FUNC(0x58D960)
void cSampleManager::Reset3DSamples()
{
    for (u32 i = 0; i < m3dSampCount; i++)
    {
        if (m3dSample[i])
        {
            AIL_release_3D_sample_handle(m3dSample[i]);
            m3dSample[i] = NULL;
        }
    }

    Close3DProvider();

    mCountSamples = 16;
    m3dSampCount = 0;
    m3dProviderIdx = -1;
    mbEaxSupported = 0;
    m3dProvider = 0;
    mFloat2704 = -1.0;
    mFloat2708 = -1.0;
    mFloat270C = -1.0;
    mEaxEnvironment = 0;
}

MATCH_FUNC(0x58D9F0)
bool cSampleManager::AllocSamples(s32 not_used)
{
    Terminate();
    Reset3DSamples();

    if (mIsStereo)
    {
        AIL_waveOutClose(mhDriver);

        if (!OpenDigitalDriver(true, false, 22050))
        {
            return false;
        }
    }

    for (u32 i = 0; i < mCountSamples; i++)
    {
        mhSamples[i] = AIL_allocate_sample_handle(mhDriver);
        AIL_init_sample(mhSamples[i]);
        AIL_set_sample_type(mhSamples[i], 1, 1);
    }

    return true;
}

MATCH_FUNC(0x58DA80)
void cSampleManager::AllocSample()
{
    if (!mhSample)
    {
        mhSample = AIL_allocate_sample_handle(mhDriver);
        AIL_init_sample(mhSample);
        AIL_set_sample_type(mhSample, 0, 0);
    }
}

MATCH_FUNC(0x58DAC0)
void cSampleManager::ReleaseSample()
{
    if (mhSample)
    {
        AIL_release_sample_handle(mhSample);
        mhSample = NULL;
    }
}

MATCH_FUNC(0x58DAE0)
void cSampleManager::Terminate()
{
    for (u8 i = 0; i < mCountSamples; i++)
    {
        AIL_release_sample_handle(mhSamples[i]);
        mhSamples[i] = NULL;
    }
}

MATCH_FUNC(0x58DB30)
void cSampleManager::Shutdown()
{
    for (u32 i = 0; i < 2; ++i)
    {
        if (i == 0)
        {
            if (mbMusicFound)
            {
                if (mhStreams[0])
                {
                    const u8 curVol = static_cast<u8>(AIL_stream_volume(mhStreams[0]));
                    if (curVol > 0)
                    {
                        s32 volL = curVol;
                        s32 volR = curVol;
                        do
                        {
                            AIL_set_stream_volume(mhStreams[0], volL);
                            AIL_delay(1);
                            --volL;
                            --volR;
                        } while (volR);
                    }
                }
            }
        }

        if (mhStreams[i])
        {
            AIL_close_stream(mhStreams[i]);
            mhStreams[i] = NULL;
        }
    }

    ReleaseSample();
    Reset3DSamples();
    Terminate();

    AIL_mem_free_lock(mpAudioBuffer1);
    mpAudioBuffer1 = NULL;
    mpAudioBuffer2 = NULL;
    AIL_waveOutClose(mhDriver);
    AIL_shutdown();
}

MATCH_FUNC(0x58DBF0)
s32 cSampleManager::GetPlayBackRate(s32 idx) const
{
    const s32 playBackRate = mSdtEntries[idx].mPlayBackRate;
    if (playBackRate == 0)
    {
        return 1;
    }
    return playBackRate;
}

MATCH_FUNC(0x58DC10)
s32 cSampleManager::GetRandomDisplacement(s32 idx) const
{
    return mSdtEntries[idx].mRandomDisplacement;
}

MATCH_FUNC(0x58DC30)
s32 cSampleManager::GetLoopStart(s32 sample_idx) const
{
    return mSdtEntries[sample_idx].mLoopStart;
}

MATCH_FUNC(0x58DC50)
s32 cSampleManager::GetLoopEnd(s32 sample_idx) const
{
    return mSdtEntries[sample_idx].mLoopEnd;
}

MATCH_FUNC(0x58DC70)
s32 cSampleManager::GetSampleLength(s32 sample_idx) const
{
    return mSdtEntries[sample_idx].mSampleLength;
}

MATCH_FUNC(0x58DC90)
void cSampleManager::InitialiseChannel(s32 channel, s32 sample_idx)
{
    if (mhSamples[channel])
    {
        if (mbLoaded)
        {
            AIL_set_sample_address(mhSamples[channel],
                                   static_cast<BYTE*>(mpAudioBuffer2) + mSdtEntries[sample_idx].mOffset,
                                   mSdtEntries[sample_idx].mSampleLength);
        }
    }
}

MATCH_FUNC(0x58DCE0)
void cSampleManager::SetChannelVolume(s32 channel, s32 volume)
{
    if (mhSamples[channel])
    {
        AIL_set_sample_volume(mhSamples[channel], volume);
    }
}

MATCH_FUNC(0x58DD00)
void cSampleManager::SetChannelPan(s32 channel, s32 pan)
{
    if (mhSamples[channel])
    {
        AIL_set_sample_pan(mhSamples[channel], pan);
    }
}

MATCH_FUNC(0x58DD20)
void cSampleManager::SetChannelFrequency(s32 channel, s32 frequency)
{
    if (mhSamples[channel])
    {
        AIL_set_sample_playback_rate(mhSamples[channel], frequency);
    }
}

MATCH_FUNC(0x58DD40)
void cSampleManager::SetChannelLoopPoints(s32 channel, s32 startOffset, s32 endOffset)
{
    if (mhSamples[channel])
    {
        AIL_set_sample_loop_block(mhSamples[channel], startOffset, endOffset);
    }
}

MATCH_FUNC(0x58DD60)
void cSampleManager::SetChannelLoopCount(s32 channel, s32 loopCount)
{
    if (mhSamples[channel])
    {
        AIL_set_sample_loop_count(mhSamples[channel], loopCount);
    }
}

MATCH_FUNC(0x58DD80)
bool cSampleManager::GetChannelUsedFlag(s32 channel) const
{
    if (mhSamples[channel])
    {
        // note: more setz madness here, ternary, direct return and if/else all change code gen
        return AIL_sample_status(mhSamples[channel]) == SMP_PLAYING;
    }
    return false;
}

MATCH_FUNC(0x58DDB0)
void cSampleManager::StartChannel(s32 channel)
{
    if (mhSamples[channel])
    {
        AIL_start_sample(mhSamples[channel]);
    }
}

MATCH_FUNC(0x58DDD0)
void cSampleManager::StopChannel(s32 channel)
{
    if (mhSamples[channel])
    {
        AIL_end_sample(mhSamples[channel]);
    }
}

MATCH_FUNC(0x58DDF0)
bool cSampleManager::InitialiseChannel3D(s32 channel, s32 nSfx, s32 rate)
{
    if (m3dSample[channel] && mbLoaded)
    {

        AILSOUNDINFO soundInfo;
        soundInfo.format = WAVE_FORMAT_PCM;
        soundInfo.data_ptr = (static_cast<BYTE*>(mpAudioBuffer2) + mSdtEntries[nSfx].mOffset);
        soundInfo.channels = 1;
        soundInfo.data_len = mSdtEntries[nSfx].mSampleLength;
        soundInfo.rate = rate;
        soundInfo.bits = 8 * mBytesPerSample;
        u32 ail_result = AIL_set_3D_sample_info(m3dSample[channel], &soundInfo);
        return ail_result != 0 ? true : false;
    }

    return false;
}

MATCH_FUNC(0x58DE80)
void cSampleManager::SetChannel3DVolume(s32 idx, s32 vol)
{
    if (m3dSample[idx])
    {
        AIL_set_3D_sample_volume(m3dSample[idx], vol);
    }
}

MATCH_FUNC(0x58DEA0)
void cSampleManager::SetChannel3DPosition(s32 channel, f32 x, f32 y, f32 z)
{
    if (m3dSample[channel])
    {
        AIL_set_3D_position(m3dSample[channel], x, y, z);
    }
}

MATCH_FUNC(0x58DED0)
void cSampleManager::SetChannel3DDistances(s32 channel, f32 maxDist, f32 minDist)
{
    if (m3dSample[channel])
    {
        AIL_set_3D_sample_distances(m3dSample[channel], maxDist, minDist, maxDist, minDist);
    }
}

MATCH_FUNC(0x58DF00)
void cSampleManager::SetChannel3DFrequency(s32 channel, s32 freq)
{
    if (m3dSample[channel])
    {
        AIL_set_3D_sample_playback_rate(m3dSample[channel], freq);
    }
}

MATCH_FUNC(0x58DF20)
void cSampleManager::SetChannel3DLoopPoints(s32 channel, s32 loop_start, s32 loop_end)
{
    if (m3dSample[channel])
    {
        AIL_set_3D_sample_loop_block(m3dSample[channel], loop_start, loop_end);
    }
}

MATCH_FUNC(0x58DF50)
void cSampleManager::SetChannel3DLoopCount(s32 channel, s32 loop_count)
{
    if (m3dSample[channel])
    {
        AIL_set_3D_sample_loop_count(m3dSample[channel], loop_count);
    }
}

MATCH_FUNC(0x58DF70)
bool cSampleManager::GetChannel3DUsedFlag(s32 channel) const
{
    if (m3dSample[channel])
    {
        // note: combining statements with && changes codegen here
        return AIL_3D_sample_status(m3dSample[channel]) == SMP_PLAYING;
    }
    return false;
}

MATCH_FUNC(0x58DFA0)
void cSampleManager::StartChannel3D(s32 samp_idx)
{
    if (m3dSample[samp_idx])
    {
        AIL_start_3D_sample(m3dSample[samp_idx]);
    }
}

MATCH_FUNC(0x58DFC0)
void cSampleManager::StopChannel3D(s32 samp_idx)
{
    if (m3dSample[samp_idx])
    {
        if (AIL_3D_sample_status(m3dSample[samp_idx]) == SMP_PLAYING)
        {
            AIL_end_3D_sample(m3dSample[samp_idx]);
        }
    }
}

MATCH_FUNC(0x58E000)
u8 cSampleManager::Get3DSampleCount() const
{
    return m3dSampCount;
}

MATCH_FUNC(0x58E010)
void cSampleManager::SetEaxEnvironment(s32 env_idx)
{
    if (m3dProvider && mbEaxSupported && env_idx < 26)
    {
        mEaxEnvironment = env_idx;
        AIL_set_3D_provider_preference(m3dProvider, "EAX environment selection", &env_idx);
    }
}

MATCH_FUNC(0x58E140)
bool cSampleManager::Open3DProvider(s32 provider_idx)
{
    m3dProviderIdx = provider_idx;
    if (provider_idx != -1)
    {
        M3DRESULT result = AIL_open_3D_provider(mH3dProvider[provider_idx]);
        if (result != M3D_NOERR)
        {
            m3dProviderIdx = -1;
            return false;
        }

        m3dProvider = mH3dProvider[provider_idx];
        // The attribute is read into the same local: a second local doesn't match
        AIL_3D_provider_attribute(m3dProvider, "EAX environment selection", &result);
        if (result != -1)
        {
            mbEaxSupported = 1;
            SetEaxEnvironment(17);
        }
        return true;
    }
    return false;
}

MATCH_FUNC(0x58E1C0)
void cSampleManager::Close3DProvider()
{
    if (m3dProvider)
    {
        AIL_close_3D_provider(m3dProvider);
        m3dProvider = 0;
        Sleep(1500u);
    }
}

MATCH_FUNC(0x58E1F0)
void cSampleManager::Enum3DProviders()
{
    HPROENUM hEnum = 0;
    u32 prov_counter = 0;
    while (prov_counter < 256)
    {
        char* pName;
        if (!AIL_enumerate_3D_providers(&hEnum, &mH3dProvider[prov_counter], &pName))
        {
            break;
        }

        mStr22B4[prov_counter] = static_cast<char_type*>(operator new(0x50u));
        strcpy(mStr22B4[prov_counter], pName);

        prov_counter++;
    }
    m3dProviderCount = prov_counter;
}

MATCH_FUNC(0x58E290)
void cSampleManager::Release()
{
    if (mhDriver)
    {
        AIL_digital_handle_release(mhDriver);
    }
}

MATCH_FUNC(0x58E2A0)
bool cSampleManager::Reacquire()
{
    bool ret = mhDriver && AIL_digital_handle_reacquire(mhDriver) ? true : false;
    return ret;
}

const char_type dma_wav_5FF5D8[3][6] = {"d.wav", "m.wav", "a.wav"};

MATCH_FUNC(0x58E2C0)
bool cSampleManager::StreamStatus() const
{
    if (mbMusicFound)
    {
        if (mhStreams[0])
        {
            if (AIL_stream_status(mhStreams[0]) == SMP_DONE)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        return true;
    }
    return false;
}

MATCH_FUNC(0x58E2F0)
void cSampleManager::StreamSetVolume(u8 vol)
{
    if (mbMusicFound)
    {
        if (mhStreams[0])
        {
            AIL_set_stream_volume(mhStreams[0], vol);
        }
    }
}

MATCH_FUNC(0x58E320)
void cSampleManager::OpenStream(u32 stream_idx)
{
    char_type wavPath[80];
    if (mbMusicFound)
    {
        if (!mhStreams[0] && stream_idx < 3)
        {
            strcpy(wavPath, mStr5);
            strcat(wavPath, dma_wav_5FF5D8[stream_idx]);
            HSTREAM hStream = AIL_open_stream(mhDriver, wavPath, 0);
            mhStreams[0] = hStream;
            if (hStream == NULL)
            {
                if (stream_idx == 1)
                {
                    strcpy(wavPath, mStr5);
                    strcat(wavPath, dma_wav_5FF5D8[0]);
                    hStream = AIL_open_stream(mhDriver, wavPath, 0);
                    mhStreams[0] = hStream;
                }
            }

            if (hStream)
            {
                AIL_set_stream_loop_count(mhStreams[0], 0);
                AIL_start_stream(mhStreams[0]);
            }
        }
    }
}

MATCH_FUNC(0x58E460)
void cSampleManager::CloseStream()
{
    if (mbMusicFound)
    {
        if (mhStreams[0])
        {
            AIL_close_stream(mhStreams[0]);
            mhStreams[0] = NULL;
        }
    }
}

MATCH_FUNC(0x58E490)
void cSampleManager::FadeOut()
{
    if (mbMusicFound && mhStreams[0])
    {
        u8 cur_vol = static_cast<u8>(AIL_stream_volume(mhStreams[0]));
        if (cur_vol > 0)
        {
            u32 volL = cur_vol;
            u32 volR = cur_vol;
            do
            {
                AIL_set_stream_volume(mhStreams[0], volL);
                AIL_delay(1);
                --volL;
                --volR;
            } while (volR);
        }
        AIL_close_stream(mhStreams[0]);
        mhStreams[0] = NULL;
    }
}

MATCH_FUNC(0x58E500)
bool cSampleManager::MusicFileExists() const
{
    return mbMusicFound;
}

MATCH_FUNC(0x58E510)
void cSampleManager::PlayVocal(s32 stream_idx, s32 voc_idx, bool bMainVariant)
{
    if (stream_idx || mbMusicFound)
    {
        if (mhStreams[stream_idx])
        {
            CloseVocalStream(stream_idx);
        }

        char_type fileName[80];
        if (!stream_idx)
        {
            strcpy(fileName, mStr5);

            char_type number[12];
            sprintf(number, "%d", voc_idx);
            strcat(fileName, number);
            if (!bMainVariant)
            {
                strcat(fileName, "A");
            }
            strcat(fileName, ".WAV");
        }
        else
        {
            strcpy(fileName, "data\\audio\\vocals\\");
            strcat(fileName, gVocNames_5FEA5C[voc_idx].mStr);
        }

        mhStreams[stream_idx] = AIL_open_stream(mhDriver, fileName, 0);
        if (mhStreams[stream_idx])
        {
            s32 loops;
            if (stream_idx != 0)
            {
                loops = 1;
            }
            else
            {
                loops = 0;
            }
            AIL_set_stream_loop_count(mhStreams[stream_idx], loops);
            AIL_start_stream(mhStreams[stream_idx]);
        }
    }
}

MATCH_FUNC(0x58E6A0)
void cSampleManager::CloseVocalStream(s32 stream_idx)
{
    if (mhStreams[stream_idx])
    {
        AIL_close_stream(mhStreams[stream_idx]);
        mhStreams[stream_idx] = NULL;
    }
}

MATCH_FUNC(0x58E6D0)
void cSampleManager::SetVocalVolume(s32 stream_idx, u8 vol)
{
    if (mhStreams[stream_idx])
    {
        AIL_set_stream_volume(mhStreams[stream_idx], vol);
    }
}

MATCH_FUNC(0x58E700)
void cSampleManager::SetVocalSpeed(s32 stream_idx, s32 playbackRate)
{
    if (mhStreams[stream_idx])
    {
        AIL_set_stream_playback_rate(mhStreams[stream_idx], playbackRate);
    }
}

MATCH_FUNC(0x58E720)
s32 cSampleManager::GetVocalSpeed(s32 stream_idx) const
{
    if (mhStreams[stream_idx])
    {
        return AIL_stream_playback_rate(mhStreams[stream_idx]);
    }
    else
    {
        return 22050;
    }
}

MATCH_FUNC(0x58E750)
void cSampleManager::SetVocalPosMs(s32 stream_idx, s32 ms_pos)
{
    if (mhStreams[stream_idx])
    {
        AIL_set_stream_ms_position(mhStreams[stream_idx], ms_pos);
    }
}

MATCH_FUNC(0x58E770)
s32 cSampleManager::GetVocalPosMs(long stream_idx) const
{
    if (mhStreams[stream_idx])
    {
        AIL_stream_ms_position(mhStreams[stream_idx], NULL, &stream_idx);
        return stream_idx;
    }
    return 0;
}

MATCH_FUNC(0x58E7A0)
s32 cSampleManager::GetVocalLengthMs(long stream_idx) const
{
    if (mhStreams[stream_idx])
    {
        AIL_stream_ms_position(mhStreams[stream_idx], &stream_idx, NULL);
        return stream_idx;
    }
    return 0;
}

MATCH_FUNC(0x58E7D0)
void cSampleManager::SetSampleVol(u8 vol)
{
    if (mhSample)
    {
        AIL_set_sample_volume(mhSample, vol);
    }
}

MATCH_FUNC(0x58E7F0)
void cSampleManager::PlayAtIdx(s32 idx)
{
    if (mhSample)
    {
        if (mbLoaded)
        {
            AIL_set_sample_address(mhSample,
                                   static_cast<BYTE*>(mpAudioBuffer2) + mSdtEntries[idx].mOffset,
                                   mSdtEntries[idx].mSampleLength);
            AIL_set_sample_playback_rate(mhSample, GetPlayBackRate(idx));
            AIL_set_sample_pan(mhSample, 64);
            AIL_set_sample_loop_count(mhSample, 1);
            AIL_start_sample(mhSample);
        }
    }
}

MATCH_FUNC(0x58E880)
bool cSampleManager::SampleNotDone() const
{
    return AIL_sample_status(mhSample) != SMP_DONE ? true : false;
}

MATCH_FUNC(0x58E8A0)
void cSampleManager::EndSample_58E8A0()
{
    if (mhSample)
    {
        AIL_end_sample(mhSample);
    }
}

MATCH_FUNC(0x58E8C0)
void cSampleManager::PlaySampleRange(u32 idx, u32 end_idx)
{
    if (idx < end_idx && mhSample && !SampleNotDone())
    {
        if (mbLoaded)
        {
            BYTE* start = static_cast<BYTE*>(mpAudioBuffer2) + mSdtEntries[idx].mOffset;
            BYTE* end = static_cast<BYTE*>(mpAudioBuffer2) + mSdtEntries[end_idx].mOffset;
            AIL_set_sample_address(mhSample, start, end - start);

            AIL_set_sample_playback_rate(mhSample, 18050);
            AIL_set_sample_pan(mhSample, 64);
            AIL_set_sample_loop_count(mhSample, 1);
            AIL_start_sample(mhSample);
        }
    }
}

MATCH_FUNC(0x58E960)
void cSampleManager::EndSample_58E960()
{
    if (mhSample)
    {
        AIL_end_sample(mhSample);
    }
}

MATCH_FUNC(0x58E980)
bool cSampleManager::LoadWavSdtData(const char_type* pRawOrSdtName)
{
    char_type rawName[80];
    char_type sdtName[80];

    mbLoaded = false;
    strcpy(rawName, "data\\audio\\");
    strcat(rawName, pRawOrSdtName);
    strcat(rawName, ".RAW");
    strcpy(sdtName, "data\\audio\\");
    strcat(sdtName, pRawOrSdtName);
    strcat(sdtName, ".SDT");
    FILE* hRawFile = fopen(rawName, "rb");
    if (!hRawFile)
    {
        return false;
    }

    fseek(hRawFile, 0, SEEK_END);
    const u32 rawFileSize = ftell(hRawFile);
    if (rawFileSize > 6100000)
    {
        fclose(hRawFile);
        return false;
    }

    rewind(hRawFile);
    fread(mpAudioBuffer1, 1u, rawFileSize, hRawFile);
    fclose(hRawFile);
    FILE* hSdtFile = fopen(sdtName, "rb");
    if (!hSdtFile)
    {
        fclose(hRawFile);
    }

    fread(mSdtEntries, sizeof(SdtEntry), 320u, hSdtFile);
    fclose(hSdtFile);
    mbLoaded = true;
    return true;
}