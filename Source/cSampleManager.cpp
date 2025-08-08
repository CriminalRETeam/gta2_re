#include "cSampleManager.hpp"
#include "Function.hpp"
#include "Globals.hpp"
#include <stdio.h>

// GLOBAL: 105 0x6FFF00
DEFINE_GLOBAL(cSampleManager, gSampManager_6FFF00);

struct voc_name
{
    char_type field_0_str[30];
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

// STUB: 105 0x58D400
cSampleManager::cSampleManager()
{
    NOT_IMPLEMENTED;
    field_1EA8_pAudioBuffer1 = 0;
    field_1EAC_pAudioBuffer2 = 0;

    memset(field_58_hSamples, 0, sizeof(field_58_hSamples));

    field_9C_hStreams[0] = 0;
    field_9C_hStreams[1] = 0;

    memset(field_26C4_3d_sample, 0, sizeof(field_26C4_3d_sample));

    field_A4_bLoaded = 0;

    for (s32 i = 0; i < 320; i++)
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

    u32 driveLetter = 'C';
    while (driveLetter <= 'Z')
    {
        char_type driveStr[2];
        driveStr[0] = driveLetter;
        driveStr[1] = 0;
        strcpy(field_5_str, driveStr);
        strcat(field_5_str, ":\\GTAUDIO\\");

        char_type FileName[80];
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

// STUB: 105 0x58D620
char_type cSampleManager::sub_58D620()
{
    NOT_IMPLEMENTED;
    // todo
    return 0;
    /*
    char_type v1; // bl
    char_type v2; // [esp+3h] [ebp-Dh]
    s32 Data; // [esp+4h] [ebp-Ch] BYREF
    HKEY phkResult; // [esp+8h] [ebp-8h] BYREF
    u32 dwDisposition; // [esp+Ch] [ebp-4h] BYREF

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

// FUNCTION: 105 0x58D6C0
char_type cSampleManager::SoundInit_58D6C0(s32* a2)
{
    AIL_startup();

    if (!sub_58D720(1, field_2714_bUnknown, 22050))
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

// FUNCTION: 105 0x58D720
char_type cSampleManager::sub_58D720(char_type a2, char_type a3, s32 sampleRate)
{
    struct lol
    {
        WAVEFORMAT waveFormat; // [esp+Ch] [ebp-10h] BYREF
        s16 t;
    };
    lol l;

    if (a2)
    {
        field_1EB1_unknown = 2;
    }
    else
    {
        field_1EB1_unknown = 1;
    }

    l.waveFormat.nChannels = 2;
    l.waveFormat.nSamplesPerSec = sampleRate;
    l.waveFormat.nAvgBytesPerSec = 2 * sampleRate * field_1EB1_unknown;
    l.waveFormat.wFormatTag = 1;
    l.waveFormat.nBlockAlign = (unsigned __int8)(2 * field_1EB1_unknown);

    // todo: not sure how/why this is here ??
    l.t = (unsigned __int8)(8 * field_1EB1_unknown);

    AIL_set_preference(1, 37); // DIG_MIXER_CHANNELS
    AIL_set_preference(15, 0); // DIG_USE_WAVEOUT

    if (a3 != 0)
    {
        AIL_set_preference(33, 1);
    }
    else
    {
        AIL_set_preference(33, 0);
    }

    AIL_set_preference(31, 1); // DIG_ENABLE_RESAMPLE_FILTER

    if (AIL_waveOutOpen(&field_0_hDriver, 0, -1, &l.waveFormat))
    {
        return 0;
    }

    field_2714_bUnknown = a3;

    AllocSample_58DA80();

    field_1EA8_pAudioBuffer1 = AIL_mem_alloc_lock(6100000);
    if (!field_1EA8_pAudioBuffer1)
    {
        ReleaseSample_58DAC0();
        AIL_waveOutClose(field_0_hDriver);
        return 0;
    }
    field_1EAC_pAudioBuffer2 = field_1EA8_pAudioBuffer1;
    return 1;
}

// FUNCTION: 105 0x58E1F0
void cSampleManager::Enum3DProviders_58E1F0()
{
    HPROENUM hEnum = 0;
    u32 prov_counter = 0;
    while (prov_counter < 256)
    {
        char* pName;
        if (!AIL_enumerate_3D_providers(&hEnum, &field_1EB4_h3dProvider[prov_counter], &pName))
        {
            break;
        }

        field_22B4_str[prov_counter] = (char_type*)operator new(0x50u);
        strcpy(field_22B4_str[prov_counter], pName);

        prov_counter++;
    }
    field_2710_3d_provider_count = prov_counter;
}

// FUNCTION: 105 0x58E2C0
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

// FUNCTION: 105 0x58D9F0
char_type cSampleManager::AllocSamples_58D9F0(s32 a2)
{
    Terminate_58DAE0();
    Reset3DSamples_58D960();

    if (field_2714_bUnknown)
    {
        AIL_waveOutClose(field_0_hDriver);

        if (sub_58D720(1, 0, 22050) == 0)
        {
            return 0;
        }
    }

    for (u32 i = 0; i < field_1EB0_count_samples; i++)
    {
        field_58_hSamples[i] = AIL_allocate_sample_handle(field_0_hDriver);
        AIL_init_sample(field_58_hSamples[i]);
        AIL_set_sample_type(field_58_hSamples[i], 1, 1);
    }

    return 1;
}

// FUNCTION: 105 0x58DAE0
void cSampleManager::Terminate_58DAE0()
{
    for (u8 i = 0; i < field_1EB0_count_samples; i++)
    {
        AIL_release_sample_handle(field_58_hSamples[i]);
        field_58_hSamples[i] = 0;
    }
}

// FUNCTION: 105 0x58DB30
void cSampleManager::Shutdown_58DB30()
{
    for (u32 i = 0; i < 2; ++i)
    {
        if (i == 0)
        {
            if (field_55_bMusicLoaded)
            {
                if (field_9C_hStreams[0])
                {
                    const u8 curVol = AIL_stream_volume(field_9C_hStreams[0]);
                    if (curVol > 0)
                    {
                        s32 volL = curVol;
                        s32 volR = curVol;
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

// FUNCTION: 105 0x58DBF0
s32 cSampleManager::GetPlayBackRateIdx_58DBF0(s32 idx)
{
    const s32 playBackRate = field_A8_sdt_entries[idx].field_8_playBackRate;
    if (playBackRate == 0)
    {
        return 1;
    }
    return playBackRate;
}

// FUNCTION: 105 0x58DC10
s32 cSampleManager::sub_58DC10(s32 idx)
{
    return field_A8_sdt_entries[idx].field_C;
}

// FUNCTION: 105 0x58D960
void cSampleManager::Reset3DSamples_58D960()
{
    for (u32 i = 0; i < field_1EB2_3d_samp_count; i++)
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

// FUNCTION: 105 0x58DC30
s32 cSampleManager::sub_58DC30(s32 a2)
{
    return field_A8_sdt_entries[a2].field_10;
}

// FUNCTION: 105 0x58DC50
s32 cSampleManager::sub_58DC50(s32 a2)
{
    return field_A8_sdt_entries[a2].field_14;
}

// FUNCTION: 105 0x58DC70
s32 cSampleManager::GetSampleLength_maybe_58DC70(s32 a2)
{
    return field_A8_sdt_entries[a2].field_4_sample_length;
}

// FUNCTION: 105 0x58DC90
void cSampleManager::sub_58DC90(s32 channel, s32 a3)
{
    if (field_58_hSamples[channel])
    {
        if (field_A4_bLoaded)
        {
            AIL_set_sample_address(field_58_hSamples[channel],
                                   (BYTE*)field_1EAC_pAudioBuffer2 + field_A8_sdt_entries[a3].field_0_offset,
                                   field_A8_sdt_entries[a3].field_4_sample_length);
        }
    }
}

// FUNCTION: 105 0x58DCE0
void cSampleManager::SetChannelVolume_58DCE0(s32 channel, s32 volume)
{
    if (field_58_hSamples[channel])
    {
        AIL_set_sample_volume(field_58_hSamples[channel], volume);
    }
}

// FUNCTION: 105 0x58DD00
void cSampleManager::SetChannelPan_58DD00(s32 channel, s32 pan)
{
    if (field_58_hSamples[channel])
    {
        AIL_set_sample_pan(field_58_hSamples[channel], pan);
    }
}

// FUNCTION: 105 0x58DD20
void cSampleManager::SetChannelFrequency_58DD20(s32 channel, s32 frequency)
{
    if (field_58_hSamples[channel])
    {
        AIL_set_sample_playback_rate(field_58_hSamples[channel], frequency);
    }
}

// FUNCTION: 105 0x58DD40
void cSampleManager::SetChannelLoopPoints_58DD40(s32 channel, s32 startOffset, s32 endOffset)
{
    if (field_58_hSamples[channel])
    {
        AIL_set_sample_loop_block(field_58_hSamples[channel], startOffset, endOffset);
    }
}

// FUNCTION: 105 0x58DD60
void cSampleManager::SetChannelLoopCount_58DD60(s32 channel, s32 loopCount)
{
    if (field_58_hSamples[channel])
    {
        AIL_set_sample_loop_count(field_58_hSamples[channel], loopCount);
    }
}

// FUNCTION: 105 0x58DD80
bool cSampleManager::GetChannelUsedFlag_58DD80(s32 channel)
{
    if (field_58_hSamples[channel])
    {
        // note: more setz madness here, ternary, direct return and if/else all change code gen
        return AIL_sample_status(field_58_hSamples[channel]) == SMP_PLAYING;
    }
    return false;
}

// FUNCTION: 105 0x58DDB0
void cSampleManager::StartChannel_58DDB0(s32 channel)
{
    if (field_58_hSamples[channel])
    {
        AIL_start_sample(field_58_hSamples[channel]);
    }
}

// FUNCTION: 105 0x58DDD0
void cSampleManager::StopChannel_58DDD0(s32 channel)
{
    if (field_58_hSamples[channel])
    {
        AIL_end_sample(field_58_hSamples[channel]);
    }
}

// FUNCTION: 105 0x58DDF0
bool cSampleManager::InitialiseChannel3D_58DDF0(s32 channel, s32 nSfx, s32 rate)
{
    if (field_26C4_3d_sample[channel] && field_A4_bLoaded)
    {

        AILSOUNDINFO soundInfo; // [esp+0h] [ebp-24h] BYREF
        soundInfo.format = WAVE_FORMAT_PCM;
        soundInfo.data_ptr = ((BYTE*)field_1EAC_pAudioBuffer2 + field_A8_sdt_entries[nSfx].field_0_offset);
        soundInfo.channels = 1;
        soundInfo.data_len = field_A8_sdt_entries[nSfx].field_4_sample_length;
        soundInfo.rate = rate;
        soundInfo.bits = 8 * field_1EB1_unknown;
        u32 tmp = AIL_set_3D_sample_info(field_26C4_3d_sample[channel], &soundInfo);
        return tmp != 0 ? true : false;
    }

    return 0;
}

// FUNCTION: 105 0x58DE80
void cSampleManager::SetChannel3DVolume_58DE80(s32 idx, s32 vol)
{
    if (field_26C4_3d_sample[idx])
    {
        AIL_set_3D_sample_volume(field_26C4_3d_sample[idx], vol);
    }
}

// FUNCTION: 105 0x58DEA0
void cSampleManager::SetChannel3DPosition_58DEA0(s32 channel, f32 x, f32 y, f32 z)
{
    if (field_26C4_3d_sample[channel])
    {
        AIL_set_3D_position(field_26C4_3d_sample[channel], x, y, z);
    }
}

// FUNCTION: 105 0x58DED0
void cSampleManager::SetChannel3DDistances_58DED0(s32 channel, f32 maxDist, f32 minDist)
{
    if (field_26C4_3d_sample[channel])
    {
        AIL_set_3D_sample_distances(field_26C4_3d_sample[channel], maxDist, minDist, maxDist, minDist);
    }
}

// FUNCTION: 105 0x58DF00
void cSampleManager::SetChannel3DFrequency_58DF00(s32 channel, s32 freq)
{
    if (field_26C4_3d_sample[channel])
    {
        AIL_set_3D_sample_playback_rate(field_26C4_3d_sample[channel], freq);
    }
}

// FUNCTION: 105 0x58DF20
void cSampleManager::SetChannel3DLoopPoints_58DF20(s32 channel, s32 a3, s32 a4)
{
    if (field_26C4_3d_sample[channel])
    {
        AIL_set_3D_sample_loop_block(field_26C4_3d_sample[channel], a3, a4);
    }
}

// FUNCTION: 105 0x58DF50
void cSampleManager::SetChannel3DLoopCount_58DF50(s32 channel, s32 a3)
{
    if (field_26C4_3d_sample[channel])
    {
        AIL_set_3D_sample_loop_count(field_26C4_3d_sample[channel], a3);
    }
}

// FUNCTION: 105 0x58DF70
bool cSampleManager::GetChannel3DUsedFlag_58DF70(s32 a2)
{
    if (field_26C4_3d_sample[a2])
    {
        // note: combining statements with && changes codegen here
        return AIL_3D_sample_status(field_26C4_3d_sample[a2]) == SMP_PLAYING;
    }
    return false;
}

// FUNCTION: 105 0x58DFA0
void cSampleManager::StartChannel3D_58DFA0(s32 samp_idx)
{
    if (field_26C4_3d_sample[samp_idx])
    {
        AIL_start_3D_sample(field_26C4_3d_sample[samp_idx]);
    }
}

// FUNCTION: 105 0x58DFC0
void cSampleManager::StopChannel3D_58DFC0(s32 samp_idx)
{
    if (field_26C4_3d_sample[samp_idx])
    {
        if (AIL_3D_sample_status(field_26C4_3d_sample[samp_idx]) == 4)
        {
            AIL_end_3D_sample(field_26C4_3d_sample[samp_idx]);
        }
    }
}

// FUNCTION: 105 0x58E010
void cSampleManager::sub_58E010(s32 a2)
{
    if (field_26C0_3d_provider && field_26B8_bEaxSupported && a2 < 26)
    {
        field_26BC_k17 = a2;
        AIL_set_3D_provider_preference(field_26C0_3d_provider, "EAX environment selection", &a2);
    }
}

// FUNCTION: 105 0x58E140
char_type cSampleManager::sub_58E140(s32 envIdx)
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

// FUNCTION: 105 0x58E1C0
void cSampleManager::Close3DProvider_58E1C0()
{
    if (field_26C0_3d_provider)
    {
        AIL_close_3D_provider(field_26C0_3d_provider);
        field_26C0_3d_provider = 0;
        Sleep(1500u);
    }
}

// FUNCTION: 105 0x58DA80
void cSampleManager::AllocSample_58DA80()
{
    if (!field_98_hSample)
    {
        field_98_hSample = AIL_allocate_sample_handle(field_0_hDriver);
        AIL_init_sample(field_98_hSample);
        AIL_set_sample_type(field_98_hSample, 0, 0);
    }
}

// FUNCTION: 105 0x58DAC0
void cSampleManager::ReleaseSample_58DAC0()
{
    if (field_98_hSample)
    {
        AIL_release_sample_handle(field_98_hSample);
        field_98_hSample = 0;
    }
}

// STUB: 105 0x58E8C0
void cSampleManager::sub_58E8C0(u32 idx, u32 a3)
{
    NOT_IMPLEMENTED;
    if (idx < a3 && field_98_hSample && !SampleNotDone_58E880())
    {
        if (field_A4_bLoaded)
        {
            BYTE* pBuffer = (BYTE*)field_1EAC_pAudioBuffer2;
            s32 off = field_A8_sdt_entries[idx].field_0_offset;
            s32 off2 = field_A8_sdt_entries[a3].field_0_offset;
            BYTE* start = pBuffer + off;
            s32 len = off2 - off;
            AIL_set_sample_address(field_98_hSample, start, len);

            AIL_set_sample_playback_rate(field_98_hSample, 18050);
            AIL_set_sample_pan(field_98_hSample, 64);
            AIL_set_sample_loop_count(field_98_hSample, 1);
            AIL_start_sample(field_98_hSample);
        }
    }
}

// FUNCTION: 105 0x58E2F0
void cSampleManager::StreamSetVolume_58E2F0(u8 vol)
{
    if (field_55_bMusicLoaded)
    {
        if (field_9C_hStreams[0])
        {
            AIL_set_stream_volume(field_9C_hStreams[0], vol);
        }
    }
}

const char_type dma_wav_5FF5D8[3][6] = {"d.wav", "m.wav", "a.wav"};

// FUNCTION: 105 0x58E320
void cSampleManager::OpenStream_58E320(u32 a2)
{
    char_type wavPath[80];
    if (field_55_bMusicLoaded)
    {
        if (!field_9C_hStreams[0] && a2 < 3)
        {
            strcpy(wavPath, field_5_str);
            strcat(wavPath, dma_wav_5FF5D8[a2]);
            HSTREAM tmp = AIL_open_stream(field_0_hDriver, wavPath, 0);
            field_9C_hStreams[0] = tmp;
            if (tmp == 0)
            {
                if (a2 == 1)
                {
                    strcpy(wavPath, field_5_str);
                    strcat(wavPath, dma_wav_5FF5D8[0]);
                    tmp = AIL_open_stream(field_0_hDriver, wavPath, 0);
                    field_9C_hStreams[0] = tmp;
                }
            }

            if (tmp)
            {
                AIL_set_stream_loop_count(field_9C_hStreams[0], 0);
                AIL_start_stream(field_9C_hStreams[0]);
            }
        }
    }
}

// FUNCTION: 105 0x58E460
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

// FUNCTION: 105 0x58E490
void cSampleManager::FadeOut_58E490()
{
    if (field_55_bMusicLoaded && field_9C_hStreams[0])
    {
        u8 cur_vol = AIL_stream_volume(field_9C_hStreams[0]);
        if (cur_vol > 0)
        {
            u32 volL = cur_vol;
            u32 volR = cur_vol;
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

// FUNCTION: 105 0x58E500
char_type cSampleManager::MusicFileExists_58E500()
{
    return field_55_bMusicLoaded;
}

// STRING: 105 0x61a9b8
#define PERCENT_D_STRING "%d"

// FUNCTION: 105 0x58E510
void cSampleManager::PlayVocal_58E510(s32 stream_idx, s32 voc_idx, char_type bAppendA)
{
    if (stream_idx || field_55_bMusicLoaded)
    {
        if (field_9C_hStreams[stream_idx])
        {
            CloseVocalStream_58E6A0(stream_idx);
        }

        char_type fileName[80];
        if (!stream_idx)
        {
            strcpy(fileName, field_5_str);

            char_type Buffer[12];
            sprintf(Buffer, PERCENT_D_STRING, voc_idx);
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
            s32 loops;
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

// FUNCTION: 105 0x58E6A0
void cSampleManager::CloseVocalStream_58E6A0(s32 stream_idx)
{
    if (field_9C_hStreams[stream_idx])
    {
        AIL_close_stream(field_9C_hStreams[stream_idx]);
        field_9C_hStreams[stream_idx] = 0;
    }
}

// FUNCTION: 105 0x58E6D0
void cSampleManager::SetVocalVolume_58E6D0(s32 stream_idx, u8 vol)
{
    if (field_9C_hStreams[stream_idx])
    {
        AIL_set_stream_volume(field_9C_hStreams[stream_idx], vol);
    }
}

// FUNCTION: 105 0x58E700
void cSampleManager::SetVocalSpeed_58E700(s32 stream_idx, s32 playbackRate)
{
    if (field_9C_hStreams[stream_idx])
    {
        AIL_set_stream_playback_rate(field_9C_hStreams[stream_idx], playbackRate);
    }
}

// FUNCTION: 105 0x58E720
s32 cSampleManager::GetVocalSpeed_58E720(s32 stream_idx)
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

// FUNCTION: 105 0x58E750
void cSampleManager::SetVocalPosMs_58E750(s32 stream_idx, s32 ms_pos)
{
    if (field_9C_hStreams[stream_idx])
    {
        AIL_set_stream_ms_position(field_9C_hStreams[stream_idx], ms_pos);
    }
}

// FUNCTION: 105 0x58E770
s32 cSampleManager::GetVocalPosMs_58E770(long stream_idx)
{
    if (field_9C_hStreams[stream_idx])
    {
        AIL_stream_ms_position(field_9C_hStreams[stream_idx], 0, &stream_idx);
        return stream_idx;
    }
    return 0;
}

// FUNCTION: 105 0x58E7A0
s32 cSampleManager::GetVocalLengthMs_58E7A0(long stream_idx)
{
    if (field_9C_hStreams[stream_idx])
    {
        AIL_stream_ms_position(field_9C_hStreams[stream_idx], &stream_idx, 0);
        return stream_idx;
    }
    return 0;
}

// FUNCTION: 105 0x58E7D0
void cSampleManager::SetSampleVol_58E7D0(u8 vol)
{
    if (field_98_hSample)
    {
        AIL_set_sample_volume(field_98_hSample, vol);
    }
}

// FUNCTION: 105 0x58E7F0
void cSampleManager::PlayAtIdx_58E7F0(s32 idx)
{
    if (field_98_hSample)
    {
        if (field_A4_bLoaded)
        {
            AIL_set_sample_address(field_98_hSample,
                                   (BYTE*)field_1EAC_pAudioBuffer2 + field_A8_sdt_entries[idx].field_0_offset,
                                   field_A8_sdt_entries[idx].field_4_sample_length);
            AIL_set_sample_playback_rate(field_98_hSample, GetPlayBackRateIdx_58DBF0(idx));
            AIL_set_sample_pan(field_98_hSample, 64);
            AIL_set_sample_loop_count(field_98_hSample, 1);
            AIL_start_sample(field_98_hSample);
        }
    }
}

// FUNCTION: 105 0x58E880
bool cSampleManager::SampleNotDone_58E880()
{
    return AIL_sample_status(field_98_hSample) != SMP_DONE ? true : false;
}

// FUNCTION: 105 0x58E8A0
void cSampleManager::sub_58E8A0()
{
    if (field_98_hSample)
    {
        AIL_end_sample(field_98_hSample);
    }
}

// FUNCTION: 105 0x58E960
void cSampleManager::EndSample_58E960()
{
    if (field_98_hSample)
    {
        AIL_end_sample(field_98_hSample);
    }
}

// FUNCTION: 105 0x58E980
char_type cSampleManager::LoadWavSdtData_58E980(const char_type* pRawOrSdtName)
{
    FILE* hRawFile; // eax
    FILE* hRawFile_; // esi
    u32 rawFileSize; // edi
    FILE* hSdtFile; // edi
    char_type rawName[80]; // [esp+10h] [ebp-A0h] BYREF
    char_type sdtName[80]; // [esp+60h] [ebp-50h] BYREF

    field_A4_bLoaded = 0;
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
    fread(field_1EA8_pAudioBuffer1, 1u, rawFileSize, hRawFile_);
    fclose(hRawFile_);
    hSdtFile = fopen(sdtName, "rb");
    if (!hSdtFile)
    {
        fclose(hRawFile_);
    }

    fread(field_A8_sdt_entries, sizeof(sdt_entry_0x18), 320u, hSdtFile);
    fclose(hSdtFile);
    field_A4_bLoaded = 1;
    return 1;
}

// FUNCTION: 105 0x58E2A0
BYTE cSampleManager::sub_58E2A0()
{
    BYTE ret = field_0_hDriver && AIL_digital_handle_reacquire(field_0_hDriver) ? 1 : 0;
    return ret;
}

// FUNCTION: 105 0x58E290
void cSampleManager::Release_58E290()
{
    if (field_0_hDriver)
    {
        AIL_digital_handle_release(field_0_hDriver);
    }
}

// FUNCTION: 105 0x58D820
char_type cSampleManager::sub_58D820(BYTE* pMaxSamples)
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

    u32 i;
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

            for (u32 sampIdx = 0; sampIdx < *pMaxSamples; sampIdx++)
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