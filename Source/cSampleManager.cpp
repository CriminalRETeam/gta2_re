#include "Function.hpp"
#include "cSampleManager.hpp"
#include <stdio.h>

cSampleManager gSampManager_6FFF00;

struct voc_name
{
    char field_0_str[30];
};

const voc_name gVocNames_5FEA5C[98] =
{
    { "accuracyb.wav" },
    { "back2front.wav" },
    { "busted.wav" },
    { "carjacker.wav" },
    { "copkilla.wav" },
    { "elvis.wav" },
    { "expiditious.wav" },
    { "freeze.wav" },
    { "frenzyfail.wav" },
    { "frenzypassed.wav" },
    { "fryingtonite.wav" },
    { "gameover.wav" },
    { "genocide.wav" },
    { "gta.wav" },
    { "insanestunt.wav" },
    { "jobcomplete.wav" },
    { "jobfail.wav" },
    { "killfrenzy.wav" },
    { "medicalemer.wav" },
    { "respectis.wav" },
    { "shocking.wav" },
    { "somethingscookin.wav" },
    { "timesup.wav" },
    { "toasted.wav" },
    { "wasted.wav" },
    { "wipeout.wav" },
    { "bombarmed.wav" },
    { "macgun.wav" },
    { "dualpist.wav" },
    { "electro.wav" },
    { "flamet.wav" },
    { "grenades.wav" },
    { "molotovs.wav" },
    { "oilslick.wav" },
    { "pistolp.wav" },
    { "rocketl.wav" },
    { "shotgun.wav" },
    { "silentmg.wav" },
    { "vehbomb.wav" },
    { "vehbomb2.wav" },
    { "vehmacgun.wav" },
    { "vehmines.wav" },
    { "armour.wav" },
    { "copbribe.wav" },
    { "doubledamage.wav" },
    { "electrofingers.wav" },
    { "fastreload.wav" },
    { "getoutjail.wav" },
    { "health.wav" },
    { "instantgang.wav" },
    { "invisibility.wav" },
    { "invulnerability.wav" },
    { "life.wav" },
    { "multiplyer.wav" },
    { "respect.wav" },
    { "raceover.wav" },
    { "2ndlap.wav" },
    { "finallap.wav" },
    { "raceon.wav" },
    { "30peopledown.wav" },
    { "60peopledown.wav" },
    { "90peopledown.wav" },
    { "120peopledown.wav" },
    { "150peopledown.wav" },
    { "timeout.wav" },
    { "timeextended.wav" },
    { "timesup.wav" },
    { "sorrydidthathurt.wav" },
    { "nicework.wav" },
    { "choctastic.wav" },
    { "raspberryripple.wav" },
    { "youshotyourload.wav" },
    { "oohdidthathurt.wav" },
    { "deathtoicvans.wav" },
    { "crispycritter.wav" },
    { "youretoastbuddy.wav" },
    { "eatleaddeath.wav" },
    { "thatsgottahurt.wav" },
    { "sorryaboutthat.wav" },
    { "xinloimyman.wav" },
    { "damnsundaydrivers.wav" },
    { "suckitandsee.wav" },
    { "tastemywrath.wav" },
    { "hallelujah.wav" },
    { "damnation.wav" },
    { "laugh1.wav" },
    { "laugh2.wav" },
    { "laugh3.wav" },
    { "laugh4.wav" },
    { "laugh5.wav" },
    { "laugh6.wav" },
    { "laugh7.wav" },
    { "laugh8.wav" },
    { "laugh9.wav" },
    { "laugha.wav" },
    { "laughb.wav" },
    { "laughc.wav" },
    { "laughd.wav" }
};



// nomatch 0x58D400
cSampleManager::cSampleManager()
{
    field_1EA8_pAudioBuffer1 = 0;
    field_1EAC_pAudioBuffer2 = 0;

    memset(field_58_hSamples, 0, sizeof(field_58_hSamples));
 
    field_9C_hStreams[0] = 0;
    field_9C_hStreams[1] = 0;

    memset(field_26C4_3d_sample, 0, sizeof(field_26C4_3d_sample));

    field_A4_bLoaded = 0;

    for (int i = 0; i < 320; i++)
    {
        field_A8_sdt_entries[i].field_0_offset = 0;
        field_A8_sdt_entries[i].field_4_sample_length = 0;
        field_A8_sdt_entries[i].field_8_playBackRate = 11025;
        field_A8_sdt_entries[i].field_C = 0;
        field_A8_sdt_entries[i].field_10 = 0;
        field_A8_sdt_entries[i].field_14 = -1;

    }

    field_0_hDriver = 0;
    field_1EB0_count_samples = 16;
    field_1EB1_unknown = 2;
    field_1EB2_3d_samp_count = 0;
    field_26B4_env_idx = -1;
    field_26B8_bEaxSupported = 0;
    field_26C0_3d_provider = 0;
    field_2704_float = -1.0;
    field_2708_float = -1.0;
    field_270C_float = -1.0;
    field_26BC_k17 = 0;
    field_2710_3d_provider_count = 0;
    field_2714_bUnknown = 0;
    
    sub_58D620();

    field_4_gtaAudioDriveLetter = 0;

    field_55_bMusicLoaded = false;

    unsigned int driveLetter = 'C';
    while (driveLetter <= 'Z')
    {
        char driveStr[2];
        driveStr[0] = driveLetter;
        driveStr[1] = 0;
        strcpy(field_5_str, driveStr);
        strcat(field_5_str, ":\\GTAUDIO\\");

        char FileName[80];
        strcpy(FileName, field_5_str);
        strcat(FileName, "1.wav");

        FILE* hFile = fopen(FileName, "rb");
        if (hFile)
        {
            fclose(hFile);
            field_55_bMusicLoaded = true;
            field_4_gtaAudioDriveLetter = driveLetter;
            break;
        }
        driveLetter++;
    }

}

// nomatch
char cSampleManager::sub_58D620()
{
    // todo
    return 0;
    /*
    char v1; // bl
    char v2; // [esp+3h] [ebp-Dh]
    int Data; // [esp+4h] [ebp-Ch] BYREF
    HKEY phkResult; // [esp+8h] [ebp-8h] BYREF
    DWORD dwDisposition; // [esp+Ch] [ebp-4h] BYREF

    if (RegCreateKeyExA(HKEY_LOCAL_MACHINE, "Software\\Aureal\\A3D", 0, 0, 0, 0x20006u, 0, &phkResult, &dwDisposition))
        return 0;
    Data = 0;
    if (RegSetValueExA(phkResult, "SplashScreen", 0, 4u, (const BYTE *)&Data, 4u))
        v1 = 0;
    else
        v1 = v2;
    Data = 0;
    if (RegSetValueExA(phkResult, "SplashAudio", 0, 4u, (const BYTE *)&Data, 4u))
        v1 = 0;
    RegCloseKey(phkResult);
    return v1;
    */
}

MATCH_FUNC(0x58D6C0)
char cSampleManager::SoundInit_58D6C0(int *a2)
{
    AIL_startup();

    if (!sub_58D720(1, this->field_2714_bUnknown, 22050))
    {
        AIL_shutdown();
        return 0;
    }

    Enum3DProviders_58E1F0();
    
    field_1EB0_count_samples = 16;
    AllocSamples_58D9F0(1);

    if (field_0_hDriver)
    {
        AIL_set_digital_master_volume(field_0_hDriver, 127);
    }
    return 1;
}

MATCH_FUNC(0x58D720)
char cSampleManager::sub_58D720(char a2, char a3, int sampleRate)
{
    struct lol
    {
        WAVEFORMAT waveFormat; // [esp+Ch] [ebp-10h] BYREF
        __int16 t;
    };
    lol l;

    if (a2)
    {
        this->field_1EB1_unknown = 2;
    }
    else
    {
        this->field_1EB1_unknown = 1;
    }

    l.waveFormat.nChannels = 2;
    l.waveFormat.nSamplesPerSec = sampleRate;
    l.waveFormat.nAvgBytesPerSec = 2 * sampleRate * field_1EB1_unknown;
    l.waveFormat.wFormatTag = 1;
    l.waveFormat.nBlockAlign = (unsigned __int8)(2 * field_1EB1_unknown);

    // todo: not sure how/why this is here ??
    l.t = (unsigned __int8)(8 * field_1EB1_unknown);

    AIL_set_preference(1, 37);                    // DIG_MIXER_CHANNELS
    AIL_set_preference(15, 0);                    // DIG_USE_WAVEOUT

    if (a3 != 0)
    {
        AIL_set_preference(33, 1);
    }
    else
    {
        AIL_set_preference(33, 0);
    }

    AIL_set_preference(31, 1);                    // DIG_ENABLE_RESAMPLE_FILTER 

    if (AIL_waveOutOpen(&this->field_0_hDriver, 0, -1, &l.waveFormat))
    {
        return 0;
    }

    this->field_2714_bUnknown = a3;
    
    AllocSample_58DA80();

    this->field_1EA8_pAudioBuffer1 = AIL_mem_alloc_lock(6100000);
    if (!field_1EA8_pAudioBuffer1)
    {
        ReleaseSample_58DAC0();
        AIL_waveOutClose(this->field_0_hDriver);
        return 0;
        
    }
    this->field_1EAC_pAudioBuffer2 = field_1EA8_pAudioBuffer1;
    return 1;
}

MATCH_FUNC(0x58E1F0)
void cSampleManager::Enum3DProviders_58E1F0()
{
    HPROENUM hEnum = 0;
    unsigned int prov_counter = 0;
    while (prov_counter < 256)
    {
        char* pName;
        if (!AIL_enumerate_3D_providers(&hEnum, &field_1EB4_h3dProvider[prov_counter], &pName))
        {
            break;
        }

        field_22B4_str[prov_counter] = (char *)operator new(0x50u);
        strcpy(field_22B4_str[prov_counter], pName);

        prov_counter++;
    }
    field_2710_3d_provider_count = prov_counter;
}

MATCH_FUNC(0x58E2C0)
bool cSampleManager::StreamStatus_58E2C0()
{
    if (field_55_bMusicLoaded)
    {
        if (field_9C_hStreams[0])
        {
            if (AIL_stream_status(field_9C_hStreams[0]) == SMP_DONE)
            {
                return 1;
            }
            else
            {
                return 0;
            }
        }
        return 1;
    }
    return 0;
}

MATCH_FUNC(0x58D9F0)
char cSampleManager::AllocSamples_58D9F0(int a2)
{
    Terminate_58DAE0();
    Reset3DSamples_58D960();

    if (this->field_2714_bUnknown)
    {
        AIL_waveOutClose(this->field_0_hDriver);

        if (sub_58D720(1, 0, 22050) == 0)
        {
            return 0;
        }
    }

    for (unsigned int i = 0; i < field_1EB0_count_samples; i++)
    {
        field_58_hSamples[i] = AIL_allocate_sample_handle(field_0_hDriver);
        AIL_init_sample(field_58_hSamples[i]);
        AIL_set_sample_type(field_58_hSamples[i], 1, 1);
    }

    return 1;
}

MATCH_FUNC(0x58DAE0)
void cSampleManager::Terminate_58DAE0()
{
    for (unsigned __int8 i = 0; i < field_1EB0_count_samples; i++)
    {
        AIL_release_sample_handle(field_58_hSamples[i]);
        field_58_hSamples[i] = 0;
    }
}

MATCH_FUNC(0x58DB30)
void cSampleManager::Shutdown_58DB30()
{
    for (unsigned int i = 0; i < 2; ++i)
    {
        if (i == 0)
        {
            if (field_55_bMusicLoaded)
            {
                if (field_9C_hStreams[0])
                {
                    const unsigned __int8 curVol = AIL_stream_volume(field_9C_hStreams[0]);
                    if (curVol > 0)
                    {
                        int volL = curVol;
                        int volR = curVol;
                        do
                        {
                            AIL_set_stream_volume(field_9C_hStreams[0], volL);
                            AIL_delay(1);
                            --volL;
                            --volR;
                        } while (volR);
                    }
                }
            }
        }

        if (field_9C_hStreams[i])
        {
            AIL_close_stream(field_9C_hStreams[i]);
            field_9C_hStreams[i] = 0;
        }
    }

    ReleaseSample_58DAC0();
    Reset3DSamples_58D960();
    Terminate_58DAE0();

    AIL_mem_free_lock(field_1EA8_pAudioBuffer1);
    field_1EA8_pAudioBuffer1 = 0;
    field_1EAC_pAudioBuffer2 = 0;
    AIL_waveOutClose(field_0_hDriver);
    AIL_shutdown();
}

MATCH_FUNC(0x58DBF0)
int cSampleManager::GetPlayBackRateIdx_58DBF0(int idx)
{
    const int playBackRate = this->field_A8_sdt_entries[idx].field_8_playBackRate;
    if (playBackRate == 0)
    {
        return 1;
    }
    return playBackRate;
}

MATCH_FUNC(0x58DC10)
int cSampleManager::sub_58DC10(int idx)
{
    return field_A8_sdt_entries[idx].field_C;
}

MATCH_FUNC(0x58D960)
void cSampleManager::Reset3DSamples_58D960()
{
    for (unsigned int i = 0; i < field_1EB2_3d_samp_count; i++)
    {
        if (field_26C4_3d_sample[i])
        {
            AIL_release_3D_sample_handle(field_26C4_3d_sample[i]);
            field_26C4_3d_sample[i] = 0;
        }
    }

    Close3DProvider_58E1C0();

    field_1EB0_count_samples = 16;
    field_1EB2_3d_samp_count = 0;
    field_26B4_env_idx = -1;
    field_26B8_bEaxSupported = 0;
    field_26C0_3d_provider = 0;
    field_2704_float = -1.0;
    field_2708_float = -1.0;
    field_270C_float = -1.0;
    field_26BC_k17 = 0;
}

MATCH_FUNC(0x58DC30)
int cSampleManager::sub_58DC30(int a2)
{
    return field_A8_sdt_entries[a2].field_10;
}

MATCH_FUNC(0x58DC50)
int cSampleManager::sub_58DC50(int a2)
{
    return field_A8_sdt_entries[a2].field_14;
}

MATCH_FUNC(0x58DC70)
int cSampleManager::GetSampleLength_maybe_58DC70(int a2)
{
    return field_A8_sdt_entries[a2].field_4_sample_length;
}

MATCH_FUNC(0x58DC90)
void cSampleManager::sub_58DC90(int channel, int a3)
{
    if (field_58_hSamples[channel])
    {
        if (field_A4_bLoaded)
        {
            AIL_set_sample_address(
                field_58_hSamples[channel],
                (BYTE*)field_1EAC_pAudioBuffer2 + field_A8_sdt_entries[a3].field_0_offset,
                field_A8_sdt_entries[a3].field_4_sample_length);
        }
    }
}

MATCH_FUNC(0x58DCE0)
void cSampleManager::SetChannelVolume_58DCE0(int channel , int volume)
{
    if (field_58_hSamples[channel])
    {
        AIL_set_sample_volume(field_58_hSamples[channel], volume);
    }
}

MATCH_FUNC(0x58DD00)
void cSampleManager::SetChannelPan_58DD00(int channel, int pan)
{
    if (field_58_hSamples[channel])
    {
        AIL_set_sample_pan(field_58_hSamples[channel], pan);
    }
}

MATCH_FUNC(0x58DD20)
void cSampleManager::SetChannelFrequency_58DD20(int channel, int frequency)
{
    if (field_58_hSamples[channel])
    {
        AIL_set_sample_playback_rate(field_58_hSamples[channel], frequency);
    }
}

MATCH_FUNC(0x58DD40)
void cSampleManager::SetChannelLoopPoints_58DD40(int channel, int startOffset, int endOffset)
{
    if (field_58_hSamples[channel])
    {
        AIL_set_sample_loop_block(field_58_hSamples[channel], startOffset, endOffset);
    }
}

MATCH_FUNC(0x58DD60)
void cSampleManager::SetChannelLoopCount_58DD60(int channel, int loopCount)
{
    if (field_58_hSamples[channel])
    {
        AIL_set_sample_loop_count(field_58_hSamples[channel], loopCount);
    }
}

MATCH_FUNC(0x58DD80)
bool cSampleManager::GetChannelUsedFlag_58DD80(int channel)
{
    if (field_58_hSamples[channel])
    {
        // note: more setz madness here, ternary, direct return and if/else all change code gen
        return AIL_sample_status(field_58_hSamples[channel]) == SMP_PLAYING;
    }
    return false;
}

MATCH_FUNC(0x58DDB0)
void cSampleManager::StartChannel_58DDB0(int channel)
{
    if (field_58_hSamples[channel])
    {
        AIL_start_sample(field_58_hSamples[channel]);
    }
}

MATCH_FUNC(0x58DDD0)
void cSampleManager::StopChannel_58DDD0(int channel)
{
    if (field_58_hSamples[channel])
    {
        AIL_end_sample(field_58_hSamples[channel]);
    }
}

MATCH_FUNC(0x58DDF0)
bool cSampleManager::InitialiseChannel3D_58DDF0(int channel, int nSfx, int rate)
{
    if (field_26C4_3d_sample[channel] && this->field_A4_bLoaded)
    {


        AILSOUNDINFO soundInfo; // [esp+0h] [ebp-24h] BYREF
        soundInfo.format = WAVE_FORMAT_PCM;
        soundInfo.data_ptr = ((BYTE *)field_1EAC_pAudioBuffer2 + field_A8_sdt_entries[nSfx].field_0_offset);
        soundInfo.channels = 1;
        soundInfo.data_len = this->field_A8_sdt_entries[nSfx].field_4_sample_length;
        soundInfo.rate = rate;
        soundInfo.bits = 8 * this->field_1EB1_unknown;
        unsigned int tmp = AIL_set_3D_sample_info(field_26C4_3d_sample[channel], &soundInfo);
        return tmp != 0 ? true : false;
    }

    return 0;
}

MATCH_FUNC(0x58DE80)
void cSampleManager::SetChannel3DVolume_58DE80(int idx, int vol)
{
    if (field_26C4_3d_sample[idx])
    {
        AIL_set_3D_sample_volume(field_26C4_3d_sample[idx], vol);
    }
}

MATCH_FUNC(0x58DEA0)
void cSampleManager::SetChannel3DPosition_58DEA0(int channel, float x, float y, float z)
{
    if (field_26C4_3d_sample[channel])
    {
        AIL_set_3D_position(field_26C4_3d_sample[channel], x, y, z);
    }
}

MATCH_FUNC(0x58DED0)
void cSampleManager::SetChannel3DDistances_58DED0(int channel, float maxDist, float minDist)
{
    if (field_26C4_3d_sample[channel])
    {
        AIL_set_3D_sample_distances(field_26C4_3d_sample[channel], maxDist, minDist, maxDist, minDist);
    }
}

MATCH_FUNC(0x58DF00)
void cSampleManager::SetChannel3DFrequency_58DF00(int channel, int freq)
{
    if (field_26C4_3d_sample[channel])
    {
        AIL_set_3D_sample_playback_rate(field_26C4_3d_sample[channel], freq);
    }
}

MATCH_FUNC(0x58DF20)
void cSampleManager::SetChannel3DLoopPoints_58DF20(int channel, int a3, int a4)
{
    if (field_26C4_3d_sample[channel])
    {
        AIL_set_3D_sample_loop_block(field_26C4_3d_sample[channel], a3, a4);
    }
}

MATCH_FUNC(0x58DF50)
void cSampleManager::SetChannel3DLoopCount_58DF50(int channel, int a3)
{
    if (field_26C4_3d_sample[channel])
    {
        AIL_set_3D_sample_loop_count(field_26C4_3d_sample[channel], a3);
    }
}

MATCH_FUNC(0x58DF70)
bool cSampleManager::GetChannel3DUsedFlag_58DF70(int a2)
{
    if (field_26C4_3d_sample[a2])
    {
        // note: combining statements with && changes codegen here
        return AIL_3D_sample_status(field_26C4_3d_sample[a2]) == SMP_PLAYING;
    }
    return false;
}

MATCH_FUNC(0x58DFA0)
void cSampleManager::StartChannel3D_58DFA0(int samp_idx)
{
    if (field_26C4_3d_sample[samp_idx])
    {
        AIL_start_3D_sample(field_26C4_3d_sample[samp_idx]);
    }
}

MATCH_FUNC(0x58DFC0)
void cSampleManager::StopChannel3D_58DFC0(int samp_idx)
{
    if (field_26C4_3d_sample[samp_idx])
    {
        if (AIL_3D_sample_status(field_26C4_3d_sample[samp_idx]) == 4)
        {
            AIL_end_3D_sample(field_26C4_3d_sample[samp_idx]);
        }
    }
}

MATCH_FUNC(0x58E010)
void cSampleManager::sub_58E010(int a2)
{
    if (field_26C0_3d_provider && field_26B8_bEaxSupported && a2 < 26)
    {
        field_26BC_k17 = a2;
        AIL_set_3D_provider_preference(field_26C0_3d_provider, "EAX environment selection", &a2);
    }
}

MATCH_FUNC(0x58E140)
char cSampleManager::sub_58E140(int envIdx)
{
    field_26B4_env_idx = envIdx;
    if (envIdx != -1)
    {
        M3DRESULT envIdx2 = AIL_open_3D_provider(field_1EB4_h3dProvider[envIdx]);
        if (envIdx2 != M3D_NOERR)
        {
            field_26B4_env_idx = -1;
            return 0;
        }

        field_26C0_3d_provider = field_1EB4_h3dProvider[envIdx];
        // using another local for envIdx2 doesn't match here, meh
        AIL_3D_provider_attribute(field_26C0_3d_provider, "EAX environment selection", &envIdx2);
        if (envIdx2 != -1)
        {
            field_26B8_bEaxSupported = 1;
            sub_58E010(17);
        }
        return 1;
    }
    return 0;
}

MATCH_FUNC(0x58E1C0)
void cSampleManager::Close3DProvider_58E1C0()
{
    if (field_26C0_3d_provider)
    {
        AIL_close_3D_provider(field_26C0_3d_provider);
        field_26C0_3d_provider = 0;
        Sleep(1500u);
    }
}

MATCH_FUNC(0x58DA80)
void cSampleManager::AllocSample_58DA80()
{
    if (!field_98_hSample)
    {
        field_98_hSample = AIL_allocate_sample_handle(field_0_hDriver);
        AIL_init_sample(field_98_hSample);
        AIL_set_sample_type(field_98_hSample, 0, 0);
    }
}

MATCH_FUNC(0x58DAC0)
void cSampleManager::ReleaseSample_58DAC0()
{
    if (field_98_hSample)
    {
        AIL_release_sample_handle(field_98_hSample);
        field_98_hSample = 0;
    }
}

// nomatch
void cSampleManager::sub_58E8C0(unsigned int idx, unsigned int a3)
{
    if (idx < a3 && this->field_98_hSample && !SampleNotDone_58E880())
    {
        if (this->field_A4_bLoaded)
        {
            BYTE* pBuffer = (BYTE*)field_1EAC_pAudioBuffer2;
            int off = field_A8_sdt_entries[idx].field_0_offset;
            int off2= field_A8_sdt_entries[a3].field_0_offset;
            BYTE* start = pBuffer + off;
            int len = off2 - off;
            AIL_set_sample_address(
                field_98_hSample,
                start,
                len
            );
                
            AIL_set_sample_playback_rate(field_98_hSample, 18050);
            AIL_set_sample_pan(field_98_hSample, 64);
            AIL_set_sample_loop_count(field_98_hSample, 1);
            AIL_start_sample(field_98_hSample);
        }
    }
}

MATCH_FUNC(0x58E2F0)
void cSampleManager::StreamSetVolume_58E2F0(unsigned __int8 vol)
{
    if (field_55_bMusicLoaded)
    {
        if (field_9C_hStreams[0])
        {
            AIL_set_stream_volume(field_9C_hStreams[0], vol);
        }
    }
}

const char dma_wav_5FF5D8[3][6] = { "d.wav", "m.wav", "a.wav" };

MATCH_FUNC(0x58E320)
void cSampleManager::OpenStream_58E320(unsigned int a2)
{
    char wavPath[80];
    if (field_55_bMusicLoaded)
    {
        if (!this->field_9C_hStreams[0] && a2 < 3)
        {
            strcpy(wavPath, this->field_5_str);
            strcat(wavPath, dma_wav_5FF5D8[a2]);
            HSTREAM tmp = AIL_open_stream(this->field_0_hDriver, wavPath, 0);
            field_9C_hStreams[0] = tmp;
            if (tmp == 0)
            {
                if (a2 == 1)
                {
                    strcpy(wavPath, this->field_5_str);
                    strcat(wavPath, dma_wav_5FF5D8[0]);
                    tmp = AIL_open_stream(this->field_0_hDriver, wavPath, 0);
                    field_9C_hStreams[0] = tmp;
                }

            }

            if (tmp)
            {
                AIL_set_stream_loop_count(field_9C_hStreams[0], 0);
                AIL_start_stream(this->field_9C_hStreams[0]);
            }

        }
    }
}

MATCH_FUNC(0x58E460)
void cSampleManager::CloseStream_58E460()
{
    if (field_55_bMusicLoaded)
    {
        if (field_9C_hStreams[0])
        {
            AIL_close_stream(field_9C_hStreams[0]);
            field_9C_hStreams[0] = 0;
        }
    }
}

MATCH_FUNC(0x58E490)
void cSampleManager::FadeOut_58E490()
{
    if (field_55_bMusicLoaded && field_9C_hStreams[0])
    {
        unsigned __int8 cur_vol = AIL_stream_volume(field_9C_hStreams[0]);
        if (cur_vol > 0)
        {
            unsigned int volL = cur_vol;
            unsigned int volR = cur_vol;
            do
            {
                AIL_set_stream_volume(field_9C_hStreams[0], volL);
                AIL_delay(1);
                --volL;
                --volR;
            } while (volR);
        }
        AIL_close_stream(field_9C_hStreams[0]);
        field_9C_hStreams[0] = 0;
    }
}

MATCH_FUNC(0x58E500)
char cSampleManager::MusicFileExists_58E500()
{
    return field_55_bMusicLoaded;
}

MATCH_FUNC(0x58E510)
void cSampleManager::PlayVocal_58E510(int stream_idx, int voc_idx, char bAppendA)
{
    if (stream_idx || field_55_bMusicLoaded)
    {
        if (field_9C_hStreams[stream_idx])
        {
            CloseVocalStream_58E6A0(stream_idx);
        }

        char fileName[80];
        if (!stream_idx)
        {
            strcpy(fileName, field_5_str);

            char Buffer[12];
            sprintf(Buffer, "%d", voc_idx);
            strcat(fileName, Buffer);
            if (!bAppendA)
            {
                strcat(fileName, "A");
            }
            strcat(fileName, ".WAV");
        }
        else
        {
            strcpy(fileName, "data\\audio\\vocals\\");
            strcat(fileName, gVocNames_5FEA5C[voc_idx].field_0_str);
        }
        
        field_9C_hStreams[stream_idx] = AIL_open_stream(field_0_hDriver, fileName, 0);
        if (field_9C_hStreams[stream_idx])
        {
            int loops;
            if (stream_idx != 0)
            {
                loops = 1;
            }
            else
            {
                loops = 0;
            }
            AIL_set_stream_loop_count(field_9C_hStreams[stream_idx], loops);
            AIL_start_stream(field_9C_hStreams[stream_idx]);
        }
    }
}

MATCH_FUNC(0x58E6A0)
void cSampleManager::CloseVocalStream_58E6A0(int stream_idx)
{
    if (field_9C_hStreams[stream_idx])
    {
        AIL_close_stream(field_9C_hStreams[stream_idx]);
        field_9C_hStreams[stream_idx] = 0;
    }
}

MATCH_FUNC(0x58E6D0)
void cSampleManager::SetVocalVolume_58E6D0(int stream_idx, unsigned __int8 vol)
{
    if (field_9C_hStreams[stream_idx])
    {
        AIL_set_stream_volume(field_9C_hStreams[stream_idx], vol);
    }
}

MATCH_FUNC(0x58E700)
void cSampleManager::SetVocalSpeed_58E700(int stream_idx, int playbackRate)
{
    if (field_9C_hStreams[stream_idx])
    {
        AIL_set_stream_playback_rate(field_9C_hStreams[stream_idx], playbackRate);
    }
}

MATCH_FUNC(0x58E720)
int cSampleManager::GetVocalSpeed_58E720(int stream_idx)
{
    if (field_9C_hStreams[stream_idx])
    {
        return AIL_stream_playback_rate(field_9C_hStreams[stream_idx]);
    }
    else
    {
        return 22050;
    }
}

MATCH_FUNC(0x58E750)
void cSampleManager::SetVocalPosMs_58E750(int stream_idx, int ms_pos)
{
    if (field_9C_hStreams[stream_idx])
    {
        AIL_set_stream_ms_position(field_9C_hStreams[stream_idx], ms_pos);
    }
}

MATCH_FUNC(0x58E770)
int cSampleManager::GetVocalPosMs_58E770(long stream_idx)
{
    if (field_9C_hStreams[stream_idx])
    {
        AIL_stream_ms_position(field_9C_hStreams[stream_idx], 0, &stream_idx);
        return stream_idx;
    }
    return 0;
}

MATCH_FUNC(0x58E7A0)
int cSampleManager::GetVocalLengthMs_58E7A0(long stream_idx)
{
    if (field_9C_hStreams[stream_idx])
    {
        AIL_stream_ms_position(field_9C_hStreams[stream_idx], &stream_idx, 0);
        return stream_idx;
    }
    return 0;
}

MATCH_FUNC(0x58E7D0)
void cSampleManager::SetSampleVol_58E7D0(unsigned __int8 vol)
{
    if (field_98_hSample)
    {
        AIL_set_sample_volume(field_98_hSample, vol);
    }
}

MATCH_FUNC(0x58E7F0)
void cSampleManager::PlayAtIdx_58E7F0(int idx)
{
    if (field_98_hSample)
    {
        if (field_A4_bLoaded)
        {
            AIL_set_sample_address(
                field_98_hSample,
                (BYTE*)field_1EAC_pAudioBuffer2 + field_A8_sdt_entries[idx].field_0_offset,
                field_A8_sdt_entries[idx].field_4_sample_length);
            AIL_set_sample_playback_rate(field_98_hSample, GetPlayBackRateIdx_58DBF0(idx));
            AIL_set_sample_pan(field_98_hSample, 64);
            AIL_set_sample_loop_count(field_98_hSample, 1);
            AIL_start_sample(field_98_hSample);
        }
    }
}

MATCH_FUNC(0x58E880)
bool cSampleManager::SampleNotDone_58E880()
{
    return AIL_sample_status(field_98_hSample) != SMP_DONE ? true : false;
}

MATCH_FUNC(0x58E960)
void cSampleManager::EndSample_58E960()
{
    if (field_98_hSample)
    {
        AIL_end_sample(field_98_hSample);
    }
}

MATCH_FUNC(0x58E980)
char cSampleManager::LoadWavSdtData_58E980(const char *pRawOrSdtName)
{
    FILE *hRawFile; // eax
    FILE *hRawFile_; // esi
    unsigned int rawFileSize; // edi
    FILE *hSdtFile; // edi
    char rawName[80]; // [esp+10h] [ebp-A0h] BYREF
    char sdtName[80]; // [esp+60h] [ebp-50h] BYREF

    this->field_A4_bLoaded = 0;
    strcpy(rawName, "data\\audio\\");
    strcat(rawName, pRawOrSdtName);
    strcat(rawName, ".RAW");
    strcpy(sdtName, "data\\audio\\");
    strcat(sdtName, pRawOrSdtName);
    strcat(sdtName, ".SDT");
    hRawFile = fopen(rawName, "rb");
    hRawFile_ = hRawFile;
    if (!hRawFile)
    {
        return 0;
    }

    fseek(hRawFile, 0, 2);
    rawFileSize = ftell(hRawFile_);
    if (rawFileSize > 6100000)
    {
        fclose(hRawFile_);
        return 0;
       
    }

    rewind(hRawFile_);
    fread(this->field_1EA8_pAudioBuffer1, 1u, rawFileSize, hRawFile_);
    fclose(hRawFile_);
    hSdtFile = fopen(sdtName, "rb");
    if (!hSdtFile)
    {
        fclose(hRawFile_);
    }

    fread(this->field_A8_sdt_entries, sizeof(sdt_entry_0x18), 320u, hSdtFile);
    fclose(hSdtFile);
    this->field_A4_bLoaded = 1;
    return 1;
}

MATCH_FUNC(0x58E2A0)
BYTE cSampleManager::sub_58E2A0()
{
    BYTE ret = field_0_hDriver && AIL_digital_handle_reacquire(field_0_hDriver) ? 1 : 0;
    return ret;
}

MATCH_FUNC(0x58E290)
void cSampleManager::Release_58E290()
{
    if (field_0_hDriver)
    {
        AIL_digital_handle_release(field_0_hDriver);
    }
}

MATCH_FUNC(0x58D820)
char cSampleManager::sub_58D820(BYTE *pMaxSamples)
{
    Terminate_58DAE0();
    Reset3DSamples_58D960();

    if (field_2714_bUnknown)
    {
        AIL_waveOutClose(field_0_hDriver);
        if (!sub_58D720(1, 0, 22050))
        {
            if (sub_58D720(1, 0, 22050))
            {
                AllocSamples_58D9F0(1);
            }
            return 0;
        }
    }

    unsigned int i;
    for (i = 0; i < 256; i++)
    {
        if (field_22B4_str[i])
        {
            if (strncmp(field_22B4_str[i], "Microsoft DirectSound3D hardware support", 0x1Eu) == 0 && sub_58E140(i))
            {
                break;
            }
        }
    }

    if (i < 256)
    {
        if (field_26C0_3d_provider)
        {
            *pMaxSamples = 0;
            
            field_1EB2_3d_samp_count = 0;
            AIL_3D_provider_attribute(field_26C0_3d_provider, "Maximum supported samples", pMaxSamples);

            if (*pMaxSamples > 16u)
            {
                *pMaxSamples = 16;
            }

            if (*pMaxSamples < 8u)
            {
                *pMaxSamples = 0;
                Close3DProvider_58E1C0();
                return 0;
            }

            for (unsigned int sampIdx = 0; sampIdx < *pMaxSamples; sampIdx++)
            {
                field_26C4_3d_sample[sampIdx] = AIL_allocate_3D_sample_handle(field_26C0_3d_provider);
                if (!field_26C4_3d_sample[sampIdx])
                {
                    *pMaxSamples = 0;
                    Close3DProvider_58E1C0();
                    return 0;
                }
            }

            field_1EB2_3d_samp_count = *pMaxSamples;
            return 1;
        }
    }
    return 0;
}
