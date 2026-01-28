#include "sound_obj.hpp"
#include "Camera.hpp"
#include "CarInfo_808.hpp"
#include "CarPhysics_B0.hpp"
#include "Car_BC.hpp"
#include "Cranes.hpp"
#include "Crushers.hpp"
#include "Frontend.hpp"
#include "Function.hpp"
#include "Game_0x40.hpp"
#include "Globals.hpp"
#include "Hud.hpp"
#include "Object_5C.hpp"
#include "Ped.hpp"
#include "Player.hpp"
#include "Rozza_C88.hpp"
#include "Weapon_30.hpp"
#include "cSampleManager.hpp"
#include "map_0x370.hpp"
#include "sprite.hpp"
#include <math.h>

DEFINE_GLOBAL(sound_obj, gSound_obj_66F680, 0x66F680);
DEFINE_GLOBAL(Fix16, dword_674CD8, 0x674CD8);
DEFINE_GLOBAL_INIT(Fix16, k_dword_66F3F0, Fix16(0), 0x66F3F0);
DEFINE_GLOBAL_INIT(Fix16, dword_674DA8, Fix16(0x100000, 0), 0x674DA8);
DEFINE_GLOBAL_ARRAY(u8, byte_61A688, 64, 0x61A688);
DEFINE_GLOBAL(u8, gSoundSwitchRadioCoolDown_6FF539, 0x6FF539);
DEFINE_GLOBAL(bool, gSoundVocalsInited_6FF538, 0x6FF538);
DEFINE_GLOBAL(Fix16, k_dword_66F3F4, 0x66F3F4);
DEFINE_GLOBAL(u16, word_6757FC, 0x6757FC);

DEFINE_GLOBAL(u8, byte_66F541, 0x66F541);
DEFINE_GLOBAL(u8, gSoundSampleIdx_61A684, 0x61A684);
DEFINE_GLOBAL(char_type, byte_66F543, 0x66F543);

// TODO: can't use 2d arrays here :Skull:
//DEFINE_GLOBAL(char_type, byte_5FE434[8][44], 0x5FE434);
char_type byte_5FE434[8][44];

static inline s32 Min(s32 a, s32 b)
{
    return a < b ? a : b;
}

static inline s32 Max(s32 a, s32 b)
{
    return a > b ? a : b;
}

static inline s32 Clamp2(s32 v, s32 center, s32 radius)
{
    return v > center ? Min(v, center + radius) : Max(v, center - radius);
}

MATCH_FUNC(0x419CD0)
sound_obj::sound_obj()
{
    field_1474 = 0;
    field_0_bSoundInitialized = 0;
    field_4 = 81920;
    field_8 = 30;
    field_C = 491520;
    field_2_service_enabled = 1;
    field_24_sfx_vol = 127;
    field_25_cdVol = 127;
    field_28_dist_related = dword_674CD8;
    field_2C_distCalculated = 0;

    field_98_nActiveSampleQueue = 1;
    ClearRequestedQueue_41B700();

    field_98_nActiveSampleQueue = 0;
    ClearRequestedQueue_41B700();

    ClearActivateSamples_41B7A0();

    field_1450 = 0;
    field_1468_v1 = 0;
    field_146C_v2 = 0;
    field_1470_v3 = 0;

    GenerateIntegerRandomNumberTable_41BA90();

    field_1478_type5Idx = 0;
    field_1B = 0;
    field_1C_samp_count = 0;
    field_10_nActiveSamples = 0;
    field_5444 = 0;
    field_5440 = 0;
    field_1D_b3d_sound = 0;
    field_14_sample_rate = 22050;
    field_18 = 1;
    field_19 = 1;
    field_1_isPaused = 0;
    field_1A_bDoubleVolume = 1;

    for (s32 i = 0; i < 1020; i++)
    {
        field_147C[i].field_0_bUsed = 0;
        field_444C_AudioEntityOrderList[i] = 0;
    }

    field_543C_444C_nAudioEntitiesCount = 0;
    field_5448_m_FrameCounter = 0;
    field_544C[0].field_0 = 0; // todo: type check
    field_544C[0].field_18 = 0; // todo: type check
    field_3 = 1;
}

MATCH_FUNC(0x41B700)
void sound_obj::ClearRequestedQueue_41B700()
{
    for (u32 i = 0; i < field_10_nActiveSamples; i++)
    {
        field_D9C_abSampleQueueIndexTable[field_98_nActiveSampleQueue][i] = field_10_nActiveSamples;
    }

    field_DBC_SampleRequestQueuesStatus[field_98_nActiveSampleQueue] = 0;
}

MATCH_FUNC(0x41B7A0)
void sound_obj::ClearActivateSamples_41B7A0()
{
    for (u8 i = 0; i < field_10_nActiveSamples; i++)
    {
        field_DC0_ActiveSamples[i].field_0_EntityIndex = 0;
        field_DC0_ActiveSamples[i].field_4_SampleIndex = 0;
        field_DC0_ActiveSamples[i].field_14_samp_idx = 321;
        field_DC0_ActiveSamples[i].field_8_obj.field_0 = dword_674CD8; // vec3 type ?
        field_DC0_ActiveSamples[i].field_8_obj.field_4 = dword_674CD8;
        field_DC0_ActiveSamples[i].field_8_obj.field_8 = dword_674CD8;
        field_DC0_ActiveSamples[i].field_18 = 0;
        field_DC0_ActiveSamples[i].field_1C_ReleasingVolumeModificator = 4;
        field_DC0_ActiveSamples[i].field_20_rate = 22050;
        field_DC0_ActiveSamples[i].field_58_type = 0;
        field_DC0_ActiveSamples[i].field_24_nVolume = 0;
        field_DC0_ActiveSamples[i].field_60_nEmittingVolume = 0;
        field_DC0_ActiveSamples[i].field_64_max_distance = 10;
        field_DC0_ActiveSamples[i].field_28_distance = 0;
        field_DC0_ActiveSamples[i].field_2C_bIsBeingPlayed = 0;
        field_DC0_ActiveSamples[i].field_2D_bIsPlayingFinished = 0;
        field_DC0_ActiveSamples[i].field_30 = 1;
        field_DC0_ActiveSamples[i].field_34 = 0;
        field_DC0_ActiveSamples[i].field_38 = -1;
        field_DC0_ActiveSamples[i].field_3C = 0;
        field_DC0_ActiveSamples[i].field_54 = 0x190000;
        field_DC0_ActiveSamples[i].field_40_pan = 63;
        field_DC0_ActiveSamples[i].field_41 = 0;
        field_DC0_ActiveSamples[i].field_44 = -3;
        field_DC0_ActiveSamples[i].field_48_nCalculatedVolume = 0;
        field_DC0_ActiveSamples[i].field_4C = 0;
        field_DC0_ActiveSamples[i].field_50 = -1;
        field_DC0_ActiveSamples[i].field_5C = 0;
    }
}

MATCH_FUNC(0x41A2A0)
void sound_obj::sub_41A2A0()
{
    field_98_nActiveSampleQueue = 1;
    ClearRequestedQueue_41B700();
    field_98_nActiveSampleQueue = 0;

    ClearRequestedQueue_41B700();
    ClearActivateSamples_41B7A0();

    field_1450 = 0;

    gSampManager_6FFF00.sub_58E2A0();
}

MATCH_FUNC(0x41A250)
void sound_obj::SetSfxVol_41A250(u8 sfxVol) // TODO: Arg type
{
    field_24_sfx_vol = sfxVol;
}

MATCH_FUNC(0x41A270)
void sound_obj::SetCDVol_41A270(u8 cdVol)
{
    field_25_cdVol = cdVol;
}

MATCH_FUNC(0x41A2E0)
char_type sound_obj::GetAudioDriveLetter_41A2E0()
{
    return gSampManager_6FFF00.field_4_gtaAudioDriveLetter;
}

MATCH_FUNC(0x41A2F0)
char_type sound_obj::Set3DSound_41A2F0(char_type b3dSound)
{
    if (field_0_bSoundInitialized != 0)
    {
        if (b3dSound != field_1D_b3d_sound)
        {
            if (b3dSound)
            {
                if (gSampManager_6FFF00.sub_58D820(&field_1C_samp_count))
                {
                    field_1D_b3d_sound = 1;
                    field_10_nActiveSamples = field_1C_samp_count; // note: regswap from redundant local
                    return 1;
                }
                else
                {
                    gSampManager_6FFF00.AllocSamples_58D9F0(1);
                    field_1D_b3d_sound = 0;
                    field_10_nActiveSamples = 16;
                    return 0;
                }
            }

            if (gSampManager_6FFF00.AllocSamples_58D9F0(1))
            {
                field_1D_b3d_sound = 0;
                field_10_nActiveSamples = 16;
                return 1;
            }

            gSampManager_6FFF00.sub_58D820(&field_1C_samp_count);
            field_1D_b3d_sound = 1;
            field_10_nActiveSamples = field_1C_samp_count;
            return 0;
        }
        return 1;
    }
    return 0;
}

MATCH_FUNC(0x41A390)
char_type sound_obj::Get3dSound_41A390()
{
    if (field_0_bSoundInitialized)
    {
        return field_1D_b3d_sound;
    }
    else
    {
        return 0;
    }
}

MATCH_FUNC(0x41A4A0)
u8 sound_obj::sub_41A4A0(Fix16 a1, Fix16 a2) // TODO: Ret type
{
    Fix16 v2 = a1 / dword_674DA8;
    if (v2 == dword_674CD8)
    {
        return 63;
    }
    else
    {
        Fix16 v4 = a2;
        if (a2 <= 0)
        {
            v4 = -a2;
        }
        s32 v5 = (v4 / v2 + Fix16(0x2000, 0)).ToInt();
        if (v5 > 63)
        {
            v5 = 63;
        }
        s32 v3;
        if (a2 > dword_674CD8)
        {
            v3 = byte_61A688[v5] + 63;
            if (v3 >= 117)
            {
                v3 = 117;
            }
        }
        else
        {
            v3 = 63 - byte_61A688[v5];
            if (v3 <= 10)
            {
                v3 = 10;
            }
        }
        return v3;
    }
}

STUB_FUNC(0x41A580)
s32 sound_obj::AdjustPlaybackRate_41A580(s32 snd_rate, Fix16 xpos, Fix16 ypos, Fix16 zpos)
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x427180)
void sound_obj::Init_RadioMessageQueue_427180()
{
    for (s32 i = 0; i < 15; i++)
    {
        field_552C_15array[i] = 121;
    }
    field_5528_idx15_cur = 0;
    field_5529_idx15 = 0;
}

MATCH_FUNC(0x4271B0)
void sound_obj::EnqueueRadioWord_4271B0(u32 val)
{
    s32 local_field_5529_idx15;

    if (val > 136)
    {
        val = 136;
    }
    else if (val < 69)
    {
        val = 69;
    }

    local_field_5529_idx15 = field_5529_idx15;
    if ((local_field_5529_idx15 + 1) % 15 != field_5528_idx15_cur)
    {
        field_552C_15array[local_field_5529_idx15] = val;
        field_5529_idx15 = (u8)(field_5529_idx15 + 1) % 15;
    }
}

WIP_FUNC(0x427220)
void sound_obj::ProcessPoliceRadioWordsPlayback_427220()
{
    WIP_IMPLEMENTED;
    if (!(field_5448_m_FrameCounter % 10u) && field_5520_bCanPlay == 0 && !gSampManager_6FFF00.SampleNotDone_58E880())
    {
        gSampManager_6FFF00.EndSample_58E960();
        field_5520_bCanPlay = 1;
    }

    if (field_5528_idx15_cur != field_5529_idx15 && !gSampManager_6FFF00.SampleNotDone_58E880() && field_5520_bCanPlay == 1)
    {
        // todo: this load is missing without volatile! Seems to cache the old value but strange because
        // surely it can't know if SampleNotDone_58E880 may have modified it
        volatile s32 old = field_552C_15array[field_5528_idx15_cur];
        if (field_5528_idx15_cur >= 15)
        {
            field_5528_idx15_cur = 15;
        }

        gSampManager_6FFF00.PlayAtIdx_58E7F0(field_552C_15array[field_5528_idx15_cur]);
        gSampManager_6FFF00.SetSampleVol_58E7D0((110 * (u32)field_24_sfx_vol) / 128);
        field_5528_idx15_cur = (field_5528_idx15_cur + 1) % 15;
    }
}

MATCH_FUNC(0x41BA90)
void sound_obj::GenerateIntegerRandomNumberTable_41BA90()
{
    for (s32 i = 0; i < 5; i++)
    {
        field_1454_anRandomTable[i] = rand();
    }
}

MATCH_FUNC(0x41B540)
void sound_obj::AdjustSamplesVolume_41B540()
{
    if (field_1D_b3d_sound)
    {
        for (u32 idx = 0; idx < field_DBC_SampleRequestQueuesStatus[field_98_nActiveSampleQueue]; idx++)
        {
            sound_0x68* pIter =
                &field_9C_asSamples[field_98_nActiveSampleQueue][field_D9C_abSampleQueueIndexTable[field_98_nActiveSampleQueue][idx]];
            if (!pIter->field_18)
            {
                f32 f28_conv;
                FromFix16_41B520(pIter->field_28_distance, &f28_conv);
                pIter->field_60_nEmittingVolume =
                    ComputeEmittingVolume_41B660(pIter->field_60_nEmittingVolume, pIter->field_64_max_distance, (u32)f28_conv);
            }
        }
    }
}

MATCH_FUNC(0x41B520)
void sound_obj::FromFix16_41B520(Fix16 a1, f32* a2)
{
    *a2 = a1.ToFloat();
}

MATCH_FUNC(0x41B660)
char_type sound_obj::ComputeEmittingVolume_41B660(u8 emittingVolume, u32 maxDistance, u32 distance)
{
    const s32 minDistance = maxDistance / 4;
    const s32 diffDistance = maxDistance - minDistance;
    if (distance > diffDistance)
    {
        return (minDistance - (distance - diffDistance)) * emittingVolume / minDistance;
    }
    return emittingVolume;
}

MATCH_FUNC(0x41A910)
void sound_obj::AddDetailsToRequestedOrderList_41A910(u8 sample)
{
    u8 i = 0;
    while (i < sample)
    {
        if (field_9C_asSamples[field_98_nActiveSampleQueue][field_D9C_abSampleQueueIndexTable[field_98_nActiveSampleQueue][i]]
                .field_48_nCalculatedVolume > field_9C_asSamples[field_98_nActiveSampleQueue][sample].field_48_nCalculatedVolume)
        {
            memmove(&field_D9C_abSampleQueueIndexTable[field_98_nActiveSampleQueue][i + 1],
                    &field_D9C_abSampleQueueIndexTable[field_98_nActiveSampleQueue][i],
                    field_10_nActiveSamples - i - 1);
            break;
        }
        i++;
    }

    field_D9C_abSampleQueueIndexTable[field_98_nActiveSampleQueue][i] = sample;
}

MATCH_FUNC(0x41A850)
void sound_obj::AddSampleToRequestedQueue_41A850()
{
    u32 newVol = field_30_sQueueSample.field_1C_ReleasingVolumeModificator * (127 - field_30_sQueueSample.field_24_nVolume);

    u8 new_requested_count;
    if (field_DBC_SampleRequestQueuesStatus[field_98_nActiveSampleQueue] < field_10_nActiveSamples)
    {
        new_requested_count = field_DBC_SampleRequestQueuesStatus[field_98_nActiveSampleQueue];
        field_DBC_SampleRequestQueuesStatus[field_98_nActiveSampleQueue]++;
    }
    else
    {
        new_requested_count = field_D9C_abSampleQueueIndexTable[field_98_nActiveSampleQueue][field_10_nActiveSamples - 1];

        if (field_9C_asSamples[field_98_nActiveSampleQueue][new_requested_count].field_48_nCalculatedVolume <= newVol)
        {
            return;
        }
    }

    field_30_sQueueSample.field_48_nCalculatedVolume = newVol;
    field_30_sQueueSample.field_2D_bIsPlayingFinished = 0;

    memcpy(&field_9C_asSamples[field_98_nActiveSampleQueue][new_requested_count], &field_30_sQueueSample, sizeof(sound_0x68));

    AddDetailsToRequestedOrderList_41A910(new_requested_count);
}

MATCH_FUNC(0x41A650)
s32 sound_obj::RandomDisplacement_41A650(u32 seed)
{
    static bool bPos_61A6C8 = true;

    s32 value = gSampManager_6FFF00.sub_58DC10(seed);
    if (!value)
    {
        return 0;
    }

    value = field_1454_anRandomTable[seed % 5] % static_cast<u32>(value);
    if (value % 2)
    {
        bPos_61A6C8 = bPos_61A6C8 == false;
    }

    if (!bPos_61A6C8)
    {
        return -value;
    }

    return value;
}

MATCH_FUNC(0x41A3A0)
void sound_obj::ServiceSoundEffects_41A3A0()
{
    if (field_2_service_enabled)
    {
        field_98_nActiveSampleQueue = field_98_nActiveSampleQueue != 1;
        ClearRequestedQueue_41B700();
        InterrogateAudioEntities_41A730();
        AddReleasingSounds_41A9D0();
        AdjustSamplesVolume_41B540();
        ProcessActiveQueues_41AB80();
        sub_41A6F0();
    }
}

// https://decomp.me/scratch/i3zcW
STUB_FUNC(0x41A730)
void sound_obj::InterrogateAudioEntities_41A730()
{
    NOT_IMPLEMENTED;
    Camera_0xBC* pTmp;

    if (field_1478_type5Idx != 0 && (pTmp = field_147C[field_1478_type5Idx].field_4_pObj->field_C_pAny.pCamera_0xBC) != NULL)
    {
        Ped* v4 = pTmp->field_34_ped;

        if (v4 != NULL)
        {
            Car_BC* field_16C_car = v4->field_16C_car;
            if (field_16C_car != NULL)
            {
                field_1468_v1 = field_16C_car->field_50_car_sprite->field_14_xy.x;
                field_146C_v2 = field_16C_car->field_50_car_sprite->field_14_xy.y;
                field_1470_v3 = field_16C_car->field_50_car_sprite->field_1C_zpos;
                field_1474 = field_16C_car->field_50_car_sprite->field_0;
            }
            else
            {
                field_1468_v1 = v4->get_cam_x();
                field_146C_v2 = v4->get_cam_y();
                field_1470_v3 = v4->field_1AC_cam.z;
                field_1474 = v4->GetRotation();
            }
        }
        else
        {
            field_1468_v1 = pTmp->field_98_cam_pos2.field_0_x;
            field_146C_v2 = pTmp->field_98_cam_pos2.field_4_y;
            field_1470_v3 = pTmp->field_98_cam_pos2.field_8_z;
        }
    }
    else
    {
        field_1468_v1 = 0;
        field_146C_v2 = 0;
        field_1470_v3 = 0;
    }

    for (u32 j = 0; j < field_543C_444C_nAudioEntitiesCount; j++)
    {
        field_28_dist_related = dword_674CD8;
        field_2C_distCalculated = 0;
        sound_obj::ProcessEntity_4123A0(field_444C_AudioEntityOrderList[j]);
    }
}

MATCH_FUNC(0x41A9D0)
void sound_obj::AddReleasingSounds_41A9D0()
{
    bool toProcess[16];

    const u8 queue = field_98_nActiveSampleQueue == 0 ? 1 : 0;

    for (u8 idx = 0; idx < field_DBC_SampleRequestQueuesStatus[queue]; idx++)
    {
        const u32 v2 = field_D9C_abSampleQueueIndexTable[queue][idx];
        sound_0x68& pSound = field_9C_asSamples[queue][v2];
        if (!pSound.field_2D_bIsPlayingFinished) // m_bIsPlayingFinished ?
        {
            toProcess[idx] = false;
            for (u8 i = 0; i < field_DBC_SampleRequestQueuesStatus[field_98_nActiveSampleQueue]; i++)
            {
                sound_0x68& t =
                    field_9C_asSamples[field_98_nActiveSampleQueue][field_D9C_abSampleQueueIndexTable[field_98_nActiveSampleQueue][i]];

                if (pSound.field_0_EntityIndex == t.field_0_EntityIndex && pSound.field_4_SampleIndex == t.field_4_SampleIndex)
                {
                    toProcess[idx] = true;
                    break;
                }
            }

            if (!toProcess[idx])
            {
                if (pSound.field_4C > 0)
                {
                    if (pSound.field_30 == 0)
                    {
                        u8* pEmittingVolume = &pSound.field_60_nEmittingVolume;
                        if (!field_1D_b3d_sound)
                        {
                            pEmittingVolume = &pSound.field_24_nVolume;
                        }

                        if (pSound.field_50 == 0xFF)
                        {
                            pSound.field_50 = *pEmittingVolume / pSound.field_4C;
                            if ((s8)pSound.field_50 <= 0)
                            {
                                pSound.field_50 = 1;
                            }
                        }

                        if (*pEmittingVolume > pSound.field_50)
                        {
                            *pEmittingVolume -= pSound.field_50;
                        }
                        else
                        {
                            pSound.field_4C = 0;
                            continue;
                        }
                    }

                    --pSound.field_4C;
                    if (field_544C[0].field_0)
                    {
                        if (pSound.field_1C_ReleasingVolumeModificator < 20)
                        {
                            ++pSound.field_1C_ReleasingVolumeModificator;
                        }
                    }
                    pSound.field_41 = 0;
                    if (!pSound.field_5C)
                    {
                        pSound.field_5C = &pSound;
                    }
                    memcpy(&field_30_sQueueSample, &pSound, sizeof(field_30_sQueueSample));
                    sound_obj::AddSampleToRequestedQueue_41A850();
                }
            }
        }
    }
}

// https://decomp.me/scratch/uOtew matching this function will make sound works
STUB_FUNC(0x41AB80)
void sound_obj::ProcessActiveQueues_41AB80()
{
    NOT_IMPLEMENTED;
    serene_brattain position;
    for (u8 idx = 0; idx < field_10_nActiveSamples; idx++)
    {
        field_9C_asSamples[field_98_nActiveSampleQueue][idx].field_2C_bIsBeingPlayed = 0;
        field_DC0_ActiveSamples[idx].field_2C_bIsBeingPlayed = 0;
    }

    for (u8 i = 0; i < field_DBC_SampleRequestQueuesStatus[field_98_nActiveSampleQueue]; i++)
    {
        sound_0x68& Sample =
            field_9C_asSamples[field_98_nActiveSampleQueue][field_D9C_abSampleQueueIndexTable[field_98_nActiveSampleQueue][i]];
        for (u8 j = 0; j < field_10_nActiveSamples; j++)
        {
            sound_0x68& t = field_DC0_ActiveSamples[j];

            if (Sample.field_0_EntityIndex != t.field_0_EntityIndex || Sample.field_4_SampleIndex != t.field_4_SampleIndex ||
                Sample.field_41 == 2 || (t.field_5C != 0 && Sample.field_5C == 0))
            {
                continue;
            }

            // OBS: field_14_samp_idx = u32
            if (!sub_412260(&Sample) || Sample.field_14_samp_idx >= 0x141 || Sample.field_14_samp_idx != t.field_14_samp_idx)
            {
                continue;
            }

            sub_41B490(&Sample);

            if (Sample.field_30 > 0) // OBS: field_30 = u32
            {
                if (t.field_44 == 0)
                {
                    bool Channel3DUsedFlag_58DF70;
                    if (field_1D_b3d_sound)
                    {
                        Channel3DUsedFlag_58DF70 = gSampManager_6FFF00.GetChannel3DUsedFlag_58DF70(j); // SLODWORD(v64)
                    }
                    else
                    {
                        Channel3DUsedFlag_58DF70 = gSampManager_6FFF00.GetChannelUsedFlag_58DD80(j);
                    }

                    if (!Channel3DUsedFlag_58DF70)
                    {
                        Sample.field_2D_bIsPlayingFinished = true;
                        t.field_2D_bIsPlayingFinished = true;
                        t.field_14_samp_idx = 321;
                        t.field_0_EntityIndex = 0;
                        continue;
                    }
                    else
                    {
                        t.field_44 = field_8 *
                            gSampManager_6FFF00.GetSampleLength_maybe_58DC70(field_DC0_ActiveSamples[j].field_14_samp_idx) /
                            Sample.field_20_rate;
                    }
                }
                else
                {
                    t.field_44--;
                }
            }

            Sample.field_2C_bIsBeingPlayed = true;
            t.field_2C_bIsBeingPlayed = true;
            Sample.field_50 = -1; // nVolumeChange

            if (Sample.field_41)
            {
                Sample.field_2C_bIsBeingPlayed = false;
                t.field_2C_bIsBeingPlayed = false;
                continue;
            }

            if (Sample.field_18) // is2D
            {
                if (field_1D_b3d_sound)
                {
                    s32 emittingVol = field_1A_bDoubleVolume ? 2 * Min(Sample.field_60_nEmittingVolume, 63) : Sample.field_24_nVolume;

                    gSampManager_6FFF00.SetChannel3DFrequency_58DF00(j, Sample.field_20_rate);
                    gSampManager_6FFF00.SetChannel3DVolume_58DE80(j,
                                                                  (emittingVol * field_24_sfx_vol) >>
                                                                      7); // (u8)((field_60_nEmittingVolume * this->field_24_sfx_vol) >> 7)
                }
                else
                {
                    s32 emittingVol = field_1A_bDoubleVolume ? 2 * Min(Sample.field_60_nEmittingVolume, 63) : Sample.field_24_nVolume;

                    gSampManager_6FFF00.SetChannelFrequency_58DD20(j, Sample.field_20_rate);
                    gSampManager_6FFF00.SetChannelVolume_58DCE0(j, (emittingVol * field_24_sfx_vol) >> 7);
                    gSampManager_6FFF00.SetChannelPan_58DD00(j, (u8)Sample.field_40_pan);
                }

                break;
            }

            t.field_28_distance = Sample.field_28_distance;
            // v25
            // doppler effect?
            Sample.field_20_rate = sound_obj::AdjustPlaybackRate_41A580(Sample.field_20_rate,
                                                                        t.field_28_distance,
                                                                        Sample.field_28_distance,
                                                                        Sample.field_3C); // v64, field_3C = Fix16 ?
            if (Sample.field_20_rate != t.field_20_rate)
            {
                u32 freq = Clamp2((s32)Sample.field_20_rate, (s32)t.field_20_rate, 6000);
                t.field_20_rate = freq;

                if (field_1D_b3d_sound)
                {
                    gSampManager_6FFF00.SetChannel3DFrequency_58DF00(j, freq);
                }
                else
                {
                    gSampManager_6FFF00.SetChannelFrequency_58DD20(j, freq);
                }
            }

            s32 volume_1;
            s32 volume_2;

            if (field_1D_b3d_sound)
            {
                volume_1 = t.field_60_nEmittingVolume;
                volume_2 = Sample.field_60_nEmittingVolume;
            }
            else
            {
                volume_1 = t.field_24_nVolume;
                volume_2 = Sample.field_24_nVolume;
            }

            // volume_1 = v30
            // volume_2 = v31
            if (volume_2 != volume_1)
            {
                u32 volume = Clamp2(volume_2, volume_1, 10);
                u32 new_volume = field_1A_bDoubleVolume ? 2 * Min(63, volume) : volume;
                if (field_1D_b3d_sound)
                {

                    gSampManager_6FFF00.SetChannel3DVolume_58DE80(j, (u8)((new_volume * field_24_sfx_vol) >> 7));
                    t.field_60_nEmittingVolume = volume;
                }
                else
                {
                    gSampManager_6FFF00.SetChannelVolume_58DCE0(j, (u8)((new_volume * field_24_sfx_vol) >> 7));
                    t.field_24_nVolume = volume;
                }
            }

            // more stuff here....

            sound_obj::VecDiff_41B4E0(&Sample.field_8_obj, &position);

            if (field_1D_b3d_sound)
            {
                f32 x;
                f32 y;
                f32 z;
                sound_obj::FromFix16_41B520(position.field_0, &x);
                sound_obj::FromFix16_41B520(position.field_4, &y);
                sound_obj::FromFix16_41B520(position.field_8, &z);
                gSampManager_6FFF00.SetChannel3DPosition_58DEA0(j, x, y, z);
                gSampManager_6FFF00.SetChannel3DDistances_58DED0(j, Sample.field_64_max_distance, Sample.field_64_max_distance >> 2);
                break;
            }

            u8 pan = sound_obj::sub_41A4A0(Sample.field_54, position.field_0);
            if (pan != t.field_40_pan)
            {
                pan = Clamp2(pan, t.field_40_pan, 10);
                gSampManager_6FFF00.SetChannelPan_58DD00(j, pan);
                t.field_40_pan = pan;
            }

            // SampleManager.SetChannelReverbFlag(j, sample.m_bReverb);
            break;
        } // end FOR j

    } // end FOR i

    for (u8 k = 0; k < field_10_nActiveSamples; k++)
    {
        sound_0x68& t = field_DC0_ActiveSamples[k];
        if (t.field_14_samp_idx == 321 || t.field_2C_bIsBeingPlayed)
        {
            continue;
        }

        if (field_1D_b3d_sound)
        {
            gSampManager_6FFF00.StopChannel3D_58DFC0(k);
        }
        else
        {
            gSampManager_6FFF00.StopChannel_58DDD0(k);
        }
        t.field_14_samp_idx = 321;
        t.field_0_EntityIndex = 0;
    }

    for (u8 l = 0; l < field_DBC_SampleRequestQueuesStatus[field_98_nActiveSampleQueue]; l++)
    {
        sound_0x68& Samp =
            field_9C_asSamples[field_98_nActiveSampleQueue][field_D9C_abSampleQueueIndexTable[field_98_nActiveSampleQueue][l]];
        if (Samp.field_2C_bIsBeingPlayed || Samp.field_2D_bIsPlayingFinished || !field_147C[Samp.field_0_EntityIndex].field_0_bUsed ||
            !field_10_nActiveSamples)
        {
            continue;
        }

        for (u8 m = 0; m < field_10_nActiveSamples; m++)
        {
            sound_0x68& t = field_DC0_ActiveSamples[m];
            if (t.field_2C_bIsBeingPlayed || !sound_obj::sub_412260(&Samp) || Samp.field_14_samp_idx >= 0x141u)
            {
                continue;
            }

            sound_obj::sub_41B490(&Samp);

            if (Samp.field_30 > 0) // LoopCount
            {
                u32 samplesPerFrame = Samp.field_20_rate / (u32)field_8;
                u32 samplesToPlay = Samp.field_30 * gSampManager_6FFF00.GetSampleLength_maybe_58DC70(Samp.field_14_samp_idx);
                if (samplesToPlay == 0)
                {
                    continue;
                }
                Samp.field_4C = samplesToPlay / samplesPerFrame + 1;
            }
            memcpy(&t, &Samp, sizeof(sound_0x68));

            serene_brattain position2;
            if (!t.field_18)
            {

                sound_obj::VecDiff_41B4E0(&Samp.field_8_obj, &position2);
                if (field_1D_b3d_sound)
                {
                    goto AUDIO_3D; // TODO: remove this
                }
                Samp.field_40_pan = sound_obj::sub_41A4A0(Samp.field_54, position2.field_0);
            }

            if (field_1D_b3d_sound)
            {
            AUDIO_3D:
                s32 emittingVol = field_1A_bDoubleVolume ? 2 * Min(63, Samp.field_60_nEmittingVolume) : Samp.field_60_nEmittingVolume;

                gSampManager_6FFF00.InitialiseChannel3D_58DDF0(m, Samp.field_14_samp_idx, Samp.field_20_rate);
                gSampManager_6FFF00.SetChannel3DFrequency_58DF00(m, Samp.field_20_rate);
                gSampManager_6FFF00.SetChannel3DVolume_58DE80(m, (u8)((emittingVol * field_24_sfx_vol) >> 7));
                gSampManager_6FFF00.SetChannel3DLoopCount_58DF50(m, Samp.field_30);
                gSampManager_6FFF00.SetChannel3DLoopPoints_58DF20(m, Samp.field_34, Samp.field_38);

                f32 xpos;
                f32 ypos;
                f32 zpos;
                if (Samp.field_18)
                {
                    xpos = 0;
                    ypos = 0.0;
                    zpos = 0.0;
                    Samp.field_54 = 1638400000;
                }
                else
                {
                    sound_obj::FromFix16_41B520(position2.field_0, &xpos);
                    sound_obj::FromFix16_41B520(position2.field_4, &ypos);
                    sound_obj::FromFix16_41B520(position2.field_8, &zpos);
                }

                gSampManager_6FFF00.SetChannel3DPosition_58DEA0(m, xpos, ypos, zpos);
                gSampManager_6FFF00.SetChannel3DDistances_58DED0(m, Samp.field_64_max_distance, Samp.field_64_max_distance >> 2);
                gSampManager_6FFF00.StartChannel3D_58DFA0(m);
            }
            else
            {
                s32 emittingVol = field_1A_bDoubleVolume ? 2 * Min(63, Samp.field_24_nVolume) : Samp.field_24_nVolume;
                gSampManager_6FFF00.sub_58DC90(m, Samp.field_14_samp_idx);
                gSampManager_6FFF00.SetChannelFrequency_58DD20(m, Samp.field_20_rate);
                gSampManager_6FFF00.SetChannelVolume_58DCE0(m, (u8)((emittingVol * field_24_sfx_vol) >> 7));
                gSampManager_6FFF00.SetChannelPan_58DD00(m, (u8)Samp.field_40_pan);
                gSampManager_6FFF00.SetChannelLoopCount_58DD60(m, Samp.field_30);
                gSampManager_6FFF00.SetChannelLoopPoints_58DD40(m, Samp.field_34, Samp.field_38);
                gSampManager_6FFF00.StartChannel_58DDB0(m);
            }

            t.field_2C_bIsBeingPlayed = true;
            Samp.field_2C_bIsBeingPlayed = true;
            Samp.field_50 = -1;
            break;
        }
    }
}

MATCH_FUNC(0x41A6F0)
void sound_obj::sub_41A6F0()
{
    for (u32 idx = 0; idx < field_543C_444C_nAudioEntitiesCount; idx++)
    {
        if (field_147C[field_444C_AudioEntityOrderList[idx]].field_1 < 10u)
        {
            field_147C[field_444C_AudioEntityOrderList[idx]].field_1++;
        }
    }
}

STUB_FUNC(0x41A3F0)
char_type sound_obj::CalcVolume_41A3F0(u8 a1, Fix16 a2, Fix16 a3)
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x419070)
bool sound_obj::VolCalc_419070(u8 a2, Fix16 a3, char_type a4)
{
    u8 v5 = CalcVolume_41A3F0(a2, a3, field_30_sQueueSample.field_28_distance);
    field_30_sQueueSample.field_24_nVolume = v5;
    if (a4)
    {
        field_30_sQueueSample.field_24_nVolume = v5 >> 1;
    }
    return field_30_sQueueSample.field_24_nVolume != 0 ? true : false;
}

MATCH_FUNC(0x419020)
char_type sound_obj::CalculateDistance_419020(Fix16 a2)
{
    if (field_28_dist_related < a2)
    {
        if (!field_2C_distCalculated)
        {
            field_2C_distCalculated = 1;
            field_30_sQueueSample.field_28_distance = Fix16::SquareRoot(field_28_dist_related);
        }
        return 1;
    }
    return 0;
}

MATCH_FUNC(0x4186D0)
char_type sound_obj::Type_9_4186D0(sound_0x68* pObj)
{
    pObj->field_3C = 600;
    pObj->field_30 = 1;
    pObj->field_34 = 0;
    pObj->field_38 = -1;
    pObj->field_20_rate =
        RandomDisplacement_41A650(pObj->field_14_samp_idx) + gSampManager_6FFF00.GetPlayBackRateIdx_58DBF0(pObj->field_14_samp_idx);
    return 1;
}

MATCH_FUNC(0x4182A0)
char_type sound_obj::Type_2_4182A0(sound_0x68* pObj)
{
    pObj->field_14_samp_idx = 0;
    pObj->field_3C = 700;
    pObj->field_30 = 0;
    pObj->field_34 = 0;
    pObj->field_38 = -1;
    pObj->field_20_rate = gSampManager_6FFF00.GetPlayBackRateIdx_58DBF0(0);
    pObj->field_4C = 5;
    return 1;
}

MATCH_FUNC(0x418130)
char_type sound_obj::Type_8_418130(sound_0x68* pObj)
{
    pObj->field_14_samp_idx = 24;
    pObj->field_3C = 400;
    pObj->field_4C = 2;
    pObj->field_20_rate = gSampManager_6FFF00.GetPlayBackRateIdx_58DBF0(24);
    pObj->field_30 = 0;
    pObj->field_34 = gSampManager_6FFF00.sub_58DC30(pObj->field_14_samp_idx);
    pObj->field_38 = gSampManager_6FFF00.sub_58DC50(pObj->field_14_samp_idx);
    return 1;
}

MATCH_FUNC(0x417F40)
char_type sound_obj::IsHeavyTruckOrBus_417F40(s32 a1)
{
    switch (a1)
    {
        case 3:
        case 7:
        case 11:
        case 17:
        case 21:
        case 58:
        case 63:
        case 64:
        case 86:
            return 1;
        default:
            return 0;
    }
}

MATCH_FUNC(0x417EF0)
s32 sound_obj::Type_7_417EF0(sound_0x68* pObj)
{
    pObj->field_14_samp_idx = 29;
    pObj->field_3C = 400;
    pObj->field_20_rate = RandomDisplacement_41A650(29) + gSampManager_6FFF00.GetPlayBackRateIdx_58DBF0(29);
    pObj->field_30 = 1;
    pObj->field_34 = 0;
    pObj->field_38 = -1;
    return 1;
}

MATCH_FUNC(0x417C60)
s32 sound_obj::ConvertToPlayBackRate_417C60(s32 a1)
{
    switch (a1)
    {
        case 13:
        case 36:
            return 22000;
        case 2:
        case 8:
        case 19:
        case 40:
        case 53:
        case 71:
            return 18000;
        case 10:
        case 16:
        case 23:
        case 24:
        case 25:
        case 26:
        case 28:
        case 29:
        case 37:
        case 42:
        case 62:
        case 67:
        case 85:
            return 24000;
        case 1:
        case 22:
        case 30:
        case 46:
        case 49:
        case 83:
            return 16000;
        case 5:
        case 31:
        case 38:
        case 57:
            return 14000;
        case 3:
        case 11:
        case 54:
            return 11000;
        case 17:
        case 21:
        case 63:
            return 10300;
        case 7:
        case 64:
        case 86:
            return 11700;
        case 60:
            return 15000;
        case 27:
            return 11025;
        case 4:
        case 32:
        case 35:
        case 41:
        case 44:
        case 50:
        case 56:
        case 70:
        case 82:
            return 19000;
        default:
            return 22050;
    }
}

MATCH_FUNC(0x4153F0)
char_type sound_obj::Type_13_4153F0(sound_0x68* pObj)
{
    pObj->field_14_samp_idx = 36;
    pObj->field_3C = 400;
    pObj->field_4C = 3;
    pObj->field_20_rate = gSampManager_6FFF00.GetPlayBackRateIdx_58DBF0(36);
    pObj->field_30 = 0;
    pObj->field_34 = gSampManager_6FFF00.sub_58DC30(36);
    pObj->field_38 = gSampManager_6FFF00.sub_58DC50(36);
    return 1;
}

MATCH_FUNC(0x41A6C0)
void sound_obj::ResetEntry_41A6C0(s32 idx)
{
    field_147C[idx].field_0_bUsed = 0;
    field_147C[idx].field_1 = 0;
    field_147C[idx].field_4_pObj = 0;
    field_147C[idx].field_8_pAlloc = 0;
}

MATCH_FUNC(0x412240)
void sound_obj::null_412240()
{
}

MATCH_FUNC(0x412250)
void sound_obj::null_412250()
{
}

MATCH_FUNC(0x418C20)
void sound_obj::sub_418C20()
{
    if (!field_544C[0].field_4_fp)
    {
        infallible_turing* pSoundObj = &field_544C[0].field_8;
        field_544C[0].field_8.field_C_pAny.pAny = 0;
        field_544C[0].field_8.field_4_bStatus = 0;
        pSoundObj->field_0_object_type = 10;
        field_544C[0].field_4_fp = AddSoundObject_419FA0(pSoundObj);
    }
}

MATCH_FUNC(0x418C60)
void sound_obj::sub_418C60()
{
    if (field_544C[0].field_4_fp)
    {
        FreeSoundEntry_41A090(field_544C[0].field_4_fp);
        field_544C[0].field_4_fp = 0;
    }
}

MATCH_FUNC(0x419E10)
void sound_obj::sub_419E10()
{
    if (!field_0_bSoundInitialized)
    {
        null_412240();
        field_0_bSoundInitialized = gSampManager_6FFF00.SoundInit_58D6C0(&field_5440);
        if (field_0_bSoundInitialized)
        {
            field_1D_b3d_sound = 0;
            field_10_nActiveSamples = 16;
            sub_418C20();
        }
    }
}

MATCH_FUNC(0x41A1B0)
char_type sound_obj::LoadStyle_41A1B0(const char_type* pStyleName)
{
    if (field_0_bSoundInitialized)
    {
        return gSampManager_6FFF00.LoadWavSdtData_58E980(GetFileName_41A1E0(pStyleName));
    }
    return 0;
}

DEFINE_GLOBAL_ARRAY(char_type, byte_674E28, 80, 0x674E28);

STUB_FUNC(0x41A1E0)
const char_type* sound_obj::GetFileName_41A1E0(const char_type* pStr)
{
    NOT_IMPLEMENTED;
    const char_type* pSlashStr; // eax
    const char_type* pAfterSlash; // eax
    char_type after_slash_char; // dl
    u8 total_idx; // si
    s32 dst_idx; // ecx
    const char_type* result; // eax
    u8 src_idx; // [esp+8h] [ebp+4h]

    pSlashStr = strrchr(pStr, '\\');
    if (pSlashStr)
        pAfterSlash = pSlashStr + 1;
    else
        pAfterSlash = pStr;
    after_slash_char = *pAfterSlash;
    src_idx = 0;
    if (*pAfterSlash == '.')
    {
        total_idx = 0;
    }
    else
    {
        total_idx = 0;
        dst_idx = 0;
        do
        {
            if (!after_slash_char)
                break;
            byte_674E28[dst_idx] = after_slash_char;
            total_idx = ++src_idx;
            dst_idx = src_idx;
            after_slash_char = pAfterSlash[src_idx];
        } while (after_slash_char != '.');
    }
    result = byte_674E28;
    byte_674E28[total_idx] = 0;
    return result;
}

struct naughty_elion_0x4C;
struct inspiring_cori_0xBC;
struct naughty_elion_0x4C;

MATCH_FUNC(0x419FA0)
s32 sound_obj::AddSoundObject_419FA0(infallible_turing* pTuring)
{
    u32 idx = 1;
    if (!field_0_bSoundInitialized || !pTuring)
    {
        return 0;
    }

    vigilant_maxwell* pMaxwellIter = &field_147C[1];
    while (idx < 1020)
    {
        if (!pMaxwellIter->field_0_bUsed)
        {
            if (pTuring->field_0_object_type == SoundObjectTypeEnum::Camera_0xBC_5)
            {
                if (!field_1478_type5Idx)
                {
                    field_1478_type5Idx = idx;
                }
                else
                {
                    return 0;
                }
            }

            ResetEntry_41A6C0(idx);

            field_147C[idx].field_4_pObj = pTuring;
            field_147C[idx].field_0_bUsed = 1;
            field_147C[idx].field_1 = 1;
            field_444C_AudioEntityOrderList[field_543C_444C_nAudioEntitiesCount++] = idx;

            switch (pTuring->field_0_object_type)
            {
                case SoundObjectTypeEnum::Sprite_1:
                {
                    Sprite* v7 = field_147C[idx].field_4_pObj->field_C_pAny.pSprite;
                    if (v7)
                    {
                        switch (v7->field_30_sprite_type_enum)
                        {
                            case sprite_types_enum::car: // note: sub eax, 2 added via switch case instead of if
                                sound_unknown_0xC* pNewObj = new sound_unknown_0xC();
                                field_147C[idx].field_8_pAlloc = pNewObj;
                                pNewObj->field_0 = dword_674CD8;
                                pNewObj->field_8 = 0;
                                pNewObj->field_A = 0;
                                break;
                        }
                    }
                    break;
                }

                case SoundObjectTypeEnum::infallible_turing_2:
                {
                    DeInitVocals_57EA10();
                    break;
                }
            }

            return idx;
        }
        idx++;

        ++pMaxwellIter;
    }

    return 0;
}

MATCH_FUNC(0x41A090)
void sound_obj::FreeSoundEntry_41A090(u32 idx)
{
    if (!field_0_bSoundInitialized || idx >= 1020)
    {
        return;
    }

    if (!field_147C[idx].field_0_bUsed)
    {
        return;
    }

    infallible_turing* pTuring = field_147C[idx].field_4_pObj;
    switch (pTuring->field_0_object_type)
    {
        case SoundObjectTypeEnum::Sprite_1:
            if (pTuring->field_C_pAny.pSprite)
            {
                Sprite* pAny = pTuring->field_C_pAny.pSprite;
                switch (pAny->field_30_sprite_type_enum)
                {
                    case sprite_types_enum::car:
                        if (field_147C[idx].field_8_pAlloc)
                        {
                            delete field_147C[idx].field_8_pAlloc;
                        }
                        break;
                }
                break;
            }
            // fall through

        case SoundObjectTypeEnum::infallible_turing_2:
            field_3 = 0;
            gSampManager_6FFF00.FadeOut_58E490();
            break;
    }

    field_147C[idx].field_0_bUsed = 0;
    field_147C[idx].field_4_pObj->field_C_pAny.pAny = 0;
    field_147C[idx].field_1 = 0;

    if (field_147C[idx].field_4_pObj->field_0_object_type == SoundObjectTypeEnum::Camera_0xBC_5)
    {
        field_1478_type5Idx = 0;
    }

    u32 idx_iter = 0;
    while (idx_iter < field_543C_444C_nAudioEntitiesCount)
    {
        if (idx == field_444C_AudioEntityOrderList[idx_iter])
        {
            if (idx_iter < 1020 - 1)
            {
                memmove(&field_444C_AudioEntityOrderList[idx_iter],
                        &field_444C_AudioEntityOrderList[idx_iter + 1],
                        sizeof(int) * (field_543C_444C_nAudioEntitiesCount - (idx_iter + 1)));
            }

            field_543C_444C_nAudioEntitiesCount--;
            field_444C_AudioEntityOrderList[field_543C_444C_nAudioEntitiesCount] = 0;
            break;
        }
        idx_iter++;
    }
}

MATCH_FUNC(0x41A290)
void sound_obj::Release_41A290()
{
    gSampManager_6FFF00.Release_58E290();
}

MATCH_FUNC(0x41A280)
u8 sound_obj::GetCDVol_41A280()
{
    return field_25_cdVol;
}

STUB_FUNC(0x57EA10)
void sound_obj::DeInitVocals_57EA10()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x57EA90)
void sound_obj::UpdateActiveRadioEmitterVolume_57EA90() // sound_obj* a1 ??
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x57EB90)
char_type sound_obj::ComputeRadioEmitterVolume_57EB90(s32 a2, s32 a3)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x57EE30)
void sound_obj::RemoveSound_57EE30(s32 a2, s32 a3)
{
    NOT_IMPLEMENTED;
}

MATCH_FUNC(0x57EEE0)
void sound_obj::CycleRadioStation_57EEE0(char_type bPrev)
{
    if (gSoundSwitchRadioCoolDown_6FF539 <= 0)
    {
        gSoundSwitchRadioCoolDown_6FF539 = 10;
        if (gGame_0x40_67E008)
        {
            Player* pPlayer = gGame_0x40_67E008->field_38_orf1;
            if (pPlayer)
            {
                if (pPlayer->sub_5698E0())
                {
                    Car_BC* pCar = gGame_0x40_67E008->field_38_orf1->sub_5698E0();
                    // can't change their sucky radio station ;)
                    if (!IsPoliceOrServiceVehicle_57F090(pCar))
                    {
                        Car_BC* pCarAgain = gGame_0x40_67E008->field_38_orf1->sub_5698E0();
                        // nor trains, which makes sense
                        if (!IsTrainOrBoxcar_57F120(pCarAgain))
                        {
                            if (bPrev)
                            {
                                field_5504_radio_station_change_mode = 1;
                            }
                            else
                            {
                                field_5504_radio_station_change_mode = 2;
                            }
                        }
                    }
                }
            }
        }
    }
}

STUB_FUNC(0x57EF60)
void sound_obj::SelectBestRadioEmitter_57EF60()
{
    NOT_IMPLEMENTED;
}

MATCH_FUNC(0x57F050)
u8 sound_obj::FindEmitterByStatus_57F050(s32 status)
{
    for (u8 i = 0; i < 5; i++)
    {
        // TODO: Structure is wrong, maybe field_544C starts earlier or later?
        if (*(u32*)&field_544C[i + 1].field_8.field_4_bStatus == status)
        {
            return i;
        }
    }
    return 127;
}

MATCH_FUNC(0x57F090)
bool sound_obj::IsPoliceOrServiceVehicle_57F090(Car_BC* pCar)
{
    switch (pCar->field_84_car_info_idx)
    {
        case car_model_enum::apc:
        case car_model_enum::COPCAR:
        case car_model_enum::FIRETRUK:
        case car_model_enum::GUNJEEP:
        case car_model_enum::JEEP:
        case car_model_enum::MEDICAR:
        case car_model_enum::SWATVAN:
        case car_model_enum::TANK:
        case car_model_enum::EDSELFBI:
            return true;
    }
    return false;
}

WIP_FUNC(0x57F120)
bool sound_obj::IsTrainOrBoxcar_57F120(Car_BC* pCar)
{
    WIP_IMPLEMENTED;

    if (!pCar)
    {
        return 0;
    }

    return pCar->field_84_car_info_idx == car_model_enum::boxcar ||
        pCar->field_84_car_info_idx > car_model_enum::TOWTRUCK && pCar->field_84_car_info_idx <= car_model_enum::TRAINFB;
}

MATCH_FUNC(0x419EF0)
void sound_obj::Service_419EF0()
{
    static char_type byte_674E24;
    if (gGame_0x40_67E008)
    {
        field_1_isPaused = gGame_0x40_67E008->field_0_game_state == GameState::Paused_2;
        if (!field_1_isPaused)
        {
            GenerateIntegerRandomNumberTable_41BA90();
        }
        else
        {
            if (!byte_674E24)
            {
                gSampManager_6FFF00.CloseVocalStream_58E6A0(1);
                gSampManager_6FFF00.PlayVocal_58E510(1, 7, 1); // FREEZE
                gSampManager_6FFF00.SetVocalVolume_58E6D0(1, 127u);
            }
        }
        byte_674E24 = field_1_isPaused;
    }

    if (field_0_bSoundInitialized)
    {
        if (!field_1_isPaused)
        {
            if (!(field_5448_m_FrameCounter++ % 5))
            {
                field_544C[0].field_0 = 1;
            }
            else
            {
                field_544C[0].field_0 = 0;
            }
        }
        ServiceSoundEffects_41A3A0();
    }
}

MATCH_FUNC(0x4123A0)
void sound_obj::ProcessEntity_4123A0(s32 id)
{
    if (!field_147C[id].field_4_pObj->field_4_bStatus)
    {
        if (gGame_0x40_67E008 && field_1478_type5Idx)
        {
            if (field_147C[id].field_4_pObj->field_0_object_type == SoundObjectTypeEnum::Radio_3)
            {
                ProcessType3_CopRadioAndMusic_57DD50();
            }

            if (!field_1_isPaused)
            {
                switch (field_147C[id].field_4_pObj->field_0_object_type)
                {
                    case SoundObjectTypeEnum::Sprite_1:
                        //case SoundObjectTypeEnum::infallible_turing_2:
                        //case SoundObjectTypeEnum::Unknown_3:
                        //case SoundObjectTypeEnum::Unknown_4:
                        ProcessType1_Sprite_412740(id);
                        break;
                    case SoundObjectTypeEnum::Rozza_C88_6:
                        ProcessType6_Rozza_C88_413760(id);
                        break;
                    case SoundObjectTypeEnum::Weapon_30_7:
                        ProcessType7_Weapon_42A500(id);
                        break;
                    case SoundObjectTypeEnum::Crane_15C_8:
                        ProcessType8_Crane_412820(id);
                        break;
                    case SoundObjectTypeEnum::Crusher_30_9:
                        ProcessType9_Crusher_412A60(id);
                        break;
                    case SoundObjectTypeEnum::Vocals_10:
                        ProcessType10_Vocals_418CA0();
                        break;
                    case SoundObjectTypeEnum::Hud_Pager_C_11:
                        ProcessType11_HudPager_418B60(id);
                        break;
                    default:
                        return;
                }
            }
        }

        if (field_147C[id].field_4_pObj->field_0_object_type == SoundObjectTypeEnum::infallible_turing_2)
        {
            ProcessType2_412490(id);
        }
    }
}

STUB_FUNC(0x57DD50)
void sound_obj::ProcessType3_CopRadioAndMusic_57DD50()
{
    NOT_IMPLEMENTED;
}

MATCH_FUNC(0x412740)
void sound_obj::ProcessType1_Sprite_412740(s32 idx)
{
    Sound_Params_8 entity;

    entity.field_0_pObj = this->field_147C[idx].field_4_pObj->field_C_pAny.pSprite;
    if (entity.field_0_pObj)
    {

        entity.field_0_pObj->GetXYZ_4117B0(&field_30_sQueueSample.field_8_obj.field_0,
                                           &field_30_sQueueSample.field_8_obj.field_4,
                                           &field_30_sQueueSample.field_8_obj.field_8);

        this->field_30_sQueueSample.field_0_EntityIndex = idx;
        this->field_30_sQueueSample.field_5C = 0;

        entity.field_5_bHasSolidAbove = gMap_0x370_6F6268->CheckColumnHasSolidAbove_4E7FC0(field_30_sQueueSample.field_8_obj.field_0,
                                                                                           field_30_sQueueSample.field_8_obj.field_4,
                                                                                           field_30_sQueueSample.field_8_obj.field_8);
        this->field_28_dist_related = ComputeEmitterDistanceSquared_4190B0();
        this->field_2C_distCalculated = 0;

        if (entity.field_0_pObj->AsCharB4_40FEA0())
        {
            sound_obj::ProcessPed_422B70(&entity);
        }
        else if (entity.field_0_pObj->AsCar_40FEB0())
        {
            sound_obj::ProcessCar_412B80(&entity);
        }
        else if (entity.field_0_pObj->As2C_40FEC0())
        {
            sound_obj::ProcessObject_41E820(&entity);
        }
    }
}

WIP_FUNC(0x413760)
void sound_obj::ProcessType6_Rozza_C88_413760(s32 idx)
{
    WIP_IMPLEMENTED;

    s32 rozza_idx; // eax
    Rozza_C88* p88; // ebx
    Rozza_A* pRozzAOff; // edi
    char_type bHasSolidAbove; // al
    Rozza_A* pRozzAOff_; // eax
    char_type v9; // [esp+11h] [ebp-Fh]
    u8 vol; // [esp+12h] [ebp-Eh]
    u8 emittingVol; // [esp+13h] [ebp-Dh]
    u8 rozza_idx_; // [esp+14h] [ebp-Ch]
    Fix16 distance; // [esp+18h] [ebp-8h]
    char_type base_sample; // [esp+24h] [ebp+4h]

    rozza_idx = 0;
    vol = 0;
    distance = 0;
    v9 = -1;
    emittingVol = 0;

    p88 = field_147C[idx].field_4_pObj->field_C_pAny.pRozza_C88;
    if (!p88)
    {
        return;
    }

    this->field_30_sQueueSample.field_5C = 0;
    this->field_30_sQueueSample.field_0_EntityIndex = idx;

    rozza_idx_ = 0;
    if (p88->field_C84_count <= 0)
    {
        return;
    }

    while (1)
    {
        pRozzAOff = &p88->field_4_pool[rozza_idx];
        base_sample = Type6_413A10(pRozzAOff);
        if (Type6_412C90(pRozzAOff, base_sample))
        {
            break;
        }

    LABEL_9:
        rozza_idx = ++rozza_idx_;
        if (rozza_idx_ >= p88->field_C84_count)
        {
            if (v9 == -1)
            {
                return;
            }
            pRozzAOff_ = &p88->field_4_pool[v9];
            this->field_30_sQueueSample.field_8_obj = *(serene_brattain*)&pRozzAOff_->field_4_x;
            this->field_30_sQueueSample.field_24_nVolume = vol;
            this->field_30_sQueueSample.field_28_distance = distance;
            this->field_30_sQueueSample.field_60_nEmittingVolume = emittingVol;
            this->field_30_sQueueSample.field_64_max_distance = 10;
            this->field_30_sQueueSample.field_54 = 81920;
            this->field_30_sQueueSample.field_58_type = 20;
            this->field_30_sQueueSample.field_4_SampleIndex = byte_66F541++;
            this->field_30_sQueueSample.field_41 = 1;
            this->field_30_sQueueSample.field_1C_ReleasingVolumeModificator = 8;
            this->field_30_sQueueSample.field_18 = 0;
            this->field_30_sQueueSample.field_3C = 600;
            this->field_30_sQueueSample.field_30 = 1;
            this->field_30_sQueueSample.field_34 = 0;
            this->field_30_sQueueSample.field_38 = -1;
            if (byte_66F541 == -1)
            {
                goto LABEL_12;
            }
            goto LABEL_13;
        }
    } // while(1)

    if (!this->field_30_sQueueSample.field_18)
    {
        if (base_sample)
        {
            this->field_30_sQueueSample.field_8_obj = *(serene_brattain*)&pRozzAOff->field_4_x;
            this->field_28_dist_related = ComputeEmitterDistanceSquared_4190B0();
            this->field_2C_distCalculated = 0;
            if (CalculateDistance_419020(Fix16(409600, 0)))
            {
                bHasSolidAbove = gMap_0x370_6F6268->CheckColumnHasSolidAbove_4E7FC0(this->field_30_sQueueSample.field_8_obj.field_0,
                                                                                    this->field_30_sQueueSample.field_8_obj.field_4,
                                                                                    this->field_30_sQueueSample.field_8_obj.field_8);
                if (VolCalc_419070(base_sample, Fix16(81920, 0), bHasSolidAbove))
                {
                    v9 = rozza_idx_;
                    vol = this->field_30_sQueueSample.field_24_nVolume;
                    distance = this->field_30_sQueueSample.field_28_distance;
                    emittingVol = base_sample;
                }
            }
        }
        goto LABEL_9; // another loop iteration
    }

    this->field_30_sQueueSample.field_8_obj.field_0 = k_dword_66F3F0;
    this->field_30_sQueueSample.field_8_obj.field_4 = k_dword_66F3F0;
    this->field_30_sQueueSample.field_8_obj.field_8 = k_dword_66F3F0;
    this->field_30_sQueueSample.field_4_SampleIndex = byte_66F541++;
    this->field_30_sQueueSample.field_1C_ReleasingVolumeModificator = 0;
    this->field_30_sQueueSample.field_24_nVolume = 45;
    this->field_30_sQueueSample.field_34 = 0;
    this->field_30_sQueueSample.field_28_distance = k_dword_66F3F0;
    this->field_30_sQueueSample.field_30 = 1;
    this->field_30_sQueueSample.field_38 = -1;
    this->field_30_sQueueSample.field_3C = 0;
    this->field_30_sQueueSample.field_40_pan = 63;
    this->field_30_sQueueSample.field_41 = 1;
    this->field_30_sQueueSample.field_54 = Fix16(81920, 0);
    this->field_30_sQueueSample.field_58_type = 20;
    this->field_30_sQueueSample.field_60_nEmittingVolume = 45;
    this->field_30_sQueueSample.field_64_max_distance = 10;

    if (byte_66F541 == -1)
    {
    LABEL_12:
        byte_66F541 = 0;
    }

LABEL_13:
    AddSampleToRequestedQueue_41A850();
}

MATCH_FUNC(0x4273B0)
void sound_obj::AppendRadioMessageSuffix_4273B0()
{
    if (GetQueuedRadioWordCount_427310() > 3u)
    {
        EnqueueRadioWord_4271B0(0x79u);
        EnqueueRadioWord_4271B0(0x5Fu);
        EnqueueRadioWord_4271B0(0x5Bu);
        EnqueueRadioWord_4271B0(0x78u);
    }
    field_5521_radio_word = (field_5521_radio_word + 1) % 13;
}

WIP_FUNC(0x42A500)
void sound_obj::ProcessType7_Weapon_42A500(s32 idx)
{
    WIP_IMPLEMENTED;

    Weapon_30* pWeapon; // ecx
    char weapon_f2C; // al
    s32 samp_idx; // edi
    Fix16 zpos; // eax
    Fix16 ypos; // ecx
    Fix16 xpos; // edx
    int rate; // edi
    int displacement; // eax
    char bSetF30; // al
    int f_14_samp_idx; // [esp-4h] [ebp-1Ch]
    u8 vol; // [esp+Ch] [ebp-Ch]
    int rate_adjust; // [esp+10h] [ebp-8h]
    char bHasSolidAbove; // [esp+1Ch] [ebp+4h]

    vol = 40;
    rate_adjust = 0;
    pWeapon = field_147C[idx].field_4_pObj->field_C_pAny.pWeapon_30;
    this->field_30_sQueueSample.field_41 = 2;
    weapon_f2C = pWeapon->field_2C;
    pWeapon->field_2C = 0;
    if (weapon_f2C)
    {
        if (pWeapon)
        {
            if (!pWeapon->field_4) // bSuppressSound ?
            {
                switch (pWeapon->field_1C_idx)
                {
                    case weapon_type::pistol:
                    case weapon_type::dual_pistol:
                        samp_idx = 311;
                        goto LABEL_15;
                    case weapon_type::smg:
                    case weapon_type::car_smg:
                    case weapon_type::army_gun_jeep:
                        samp_idx = 312;
                        vol = 44;
                        goto LABEL_15;
                    case weapon_type::rocket:
                    case weapon_type::tank_main_gun:
                        samp_idx = 315;
                        goto LABEL_15;
                    case weapon_type::shocker:
                        samp_idx = 318;
                        this->field_30_sQueueSample.field_41 = 0;
                        goto LABEL_15;
                    case weapon_type::shotgun:
                        samp_idx = 314;
                        vol = 44;
                        rate_adjust = -8000;
                        goto LABEL_15;
                    case weapon_type::flamethrower:
                    case weapon_type::fire_truck_flamethrower:
                        samp_idx = 316;
                        vol = 120;
                        this->field_30_sQueueSample.field_41 = 0;
                        goto LABEL_15;
                    case weapon_type::silence_smg:
                        samp_idx = 313;
                        vol = 40;
                        goto LABEL_15;
                    case weapon_type::car_bomb:
                    case weapon_type::car_mines:
                    case weapon_type::weapon_0x17:
                        samp_idx = 319;
                        goto LABEL_15;
                    case weapon_type::oil_stain:
                        samp_idx = 61;
                        goto LABEL_15;
                    case weapon_type::fire_truck_gun:
                        samp_idx = 317;
                        this->field_30_sQueueSample.field_41 = 0;

                        break;
                    default:
                        return;
                }

            LABEL_15:
                pWeapon->GetSoundPos_5E3F90(&this->field_30_sQueueSample.field_8_obj.field_0,
                                            &this->field_30_sQueueSample.field_8_obj.field_4,
                                            &this->field_30_sQueueSample.field_8_obj.field_8);
                zpos = this->field_30_sQueueSample.field_8_obj.field_8;
                ypos = this->field_30_sQueueSample.field_8_obj.field_4;
                xpos = this->field_30_sQueueSample.field_8_obj.field_0;

                this->field_30_sQueueSample.field_0_EntityIndex = idx;
                this->field_30_sQueueSample.field_5C = 0;
                bHasSolidAbove = gMap_0x370_6F6268->CheckColumnHasSolidAbove_4E7FC0(xpos, ypos, zpos);
                this->field_28_dist_related = ComputeEmitterDistanceSquared_4190B0();
                this->field_2C_distCalculated = 0;
                if (CalculateDistance_419020(1638400))
                {
                    if (VolCalc_419070(vol, 163840, bHasSolidAbove))
                    {
                        this->field_30_sQueueSample.field_60_nEmittingVolume = vol;
                        this->field_30_sQueueSample.field_14_samp_idx = samp_idx;
                        this->field_30_sQueueSample.field_54 = 163840;
                        this->field_30_sQueueSample.field_64_max_distance = 20;
                        rate = gSampManager_6FFF00.GetPlayBackRateIdx_58DBF0(samp_idx);
                        displacement = RandomDisplacement_41A650(this->field_30_sQueueSample.field_14_samp_idx);
                        f_14_samp_idx = this->field_30_sQueueSample.field_14_samp_idx;
                        this->field_30_sQueueSample.field_58_type = 20;
                        this->field_30_sQueueSample.field_20_rate = rate + rate_adjust + displacement;
                        this->field_30_sQueueSample.field_3C = 0;
                        this->field_30_sQueueSample.field_4_SampleIndex = 0;
                        this->field_30_sQueueSample.field_1C_ReleasingVolumeModificator = 4;
                        this->field_30_sQueueSample.field_18 = 0;
                        this->field_30_sQueueSample.field_34 = gSampManager_6FFF00.sub_58DC30(f_14_samp_idx);
                        bSetF30 = this->field_30_sQueueSample.field_41;
                        this->field_30_sQueueSample.field_38 = -1;
                        this->field_30_sQueueSample.field_4C = 5;
                        if (bSetF30)
                        {
                            this->field_30_sQueueSample.field_30 = 1;
                        }
                        else
                        {
                            this->field_30_sQueueSample.field_30 = 0;
                        }
                        AddSampleToRequestedQueue_41A850();
                    }
                }
            }
        }
    }
}

WIP_FUNC(0x412820)
void sound_obj::ProcessType8_Crane_412820(s32 idx)
{
    WIP_IMPLEMENTED;

    s32 idx_; // ecx
    Crane_15C* pCrane; // edi
    Sprite* pSprite; // eax
    Fix16 ypos; // edx
    Fix16 f1C_zpos; // eax
    Fix16 zpos; // ecx
    Fix16 xpos; // eax
    u8 new_idx; // cl
    s32 new_counter; // eax
    s32 samp_idx; // edi
    s32 rate; // ebp
    char_type new_f41; // bl
    char_type samp_idx_1; // al
    s32 new_f38; // eax
    bool bContinue; // cf
    u8 emitter_vol; // [esp+8h] [ebp-14h]
    s32 vol_mod; // [esp+Ch] [ebp-10h]
    Crane_15C* pCrane_; // [esp+10h] [ebp-Ch]
    s32 counter; // [esp+14h] [ebp-8h]
    char_type bSolidAbove; // [esp+18h] [ebp-4h]

    idx_ = idx;
    pCrane = field_147C[idx].field_4_pObj->field_C_pAny.pCrane_15C;
    pCrane_ = pCrane;
    if (pCrane)
    {
        pSprite = pCrane->field_2C->field_4;
        if (pSprite)
        {
            this->field_30_sQueueSample.field_8_obj.field_0 = pSprite->field_14_xy.x;

            ypos = pSprite->field_14_xy.y;
            this->field_30_sQueueSample.field_8_obj.field_4 = ypos;

            f1C_zpos = pSprite->field_1C_zpos;

            this->field_30_sQueueSample.field_0_EntityIndex = idx_;

            zpos = f1C_zpos;
            this->field_30_sQueueSample.field_8_obj.field_8 = f1C_zpos;

            xpos = this->field_30_sQueueSample.field_8_obj.field_0;

            this->field_30_sQueueSample.field_5C = 0;
            bSolidAbove = gMap_0x370_6F6268->CheckColumnHasSolidAbove_4E7FC0(xpos, ypos, zpos);
            this->field_28_dist_related = ComputeEmitterDistanceSquared_4190B0();
            this->field_2C_distCalculated = 0;
            if (CalculateDistance_419020(Fix16(0x9C4000, 0)))
            {
                new_idx = 0;
                new_counter = 0;
                idx = 0;
                counter = 0;
                do
                {
                    switch (new_counter)
                    {
                        case 0:
                            if (pCrane->field_156)
                            {
                                samp_idx = 60;
                                rate = 9000;
                                emitter_vol = 60;
                                vol_mod = 3;
                                new_f41 = 0;
                                goto LABEL_14;
                            }
                            break;

                        case 1:
                            if (pCrane->field_157)
                            {
                                samp_idx = 60;
                                rate = 13000;
                                emitter_vol = 30;
                                vol_mod = 4;
                                new_f41 = 0;
                                goto LABEL_14;
                            }
                            break;

                        case 2:
                            if (pCrane->field_158)
                            {
                                samp_idx = 60;
                                rate = 8000;
                                emitter_vol = 45;
                                vol_mod = 4;
                                new_f41 = 0;
                                goto LABEL_14;
                            }
                            break;

                        case 3:
                            if (pCrane->field_159)
                            {
                                samp_idx = 37;
                                rate = 22050;
                                emitter_vol = 50;
                                vol_mod = 5;
                                new_f41 = 1;

                            LABEL_14:
                                if (new_idx < 5u && VolCalc_419070(emitter_vol, Fix16(409600, 0), bSolidAbove))
                                {
                                    samp_idx_1 = idx;
                                    this->field_30_sQueueSample.field_14_samp_idx = samp_idx;
                                    this->field_30_sQueueSample.field_4_SampleIndex = samp_idx_1;
                                    this->field_30_sQueueSample.field_41 = new_f41;
                                    this->field_30_sQueueSample.field_20_rate = rate;
                                    if (new_f41)
                                    {
                                        this->field_30_sQueueSample.field_30 = 1;
                                    }
                                    else
                                    {
                                        this->field_30_sQueueSample.field_30 = 0;
                                        this->field_30_sQueueSample.field_4C = 3;
                                    }
                                    this->field_30_sQueueSample.field_34 = gSampManager_6FFF00.sub_58DC30(samp_idx);
                                    new_f38 = gSampManager_6FFF00.sub_58DC50(samp_idx);
                                    this->field_30_sQueueSample.field_1C_ReleasingVolumeModificator = vol_mod;
                                    this->field_30_sQueueSample.field_38 = new_f38;
                                    this->field_30_sQueueSample.field_54 = Fix16(409600, 0);
                                    this->field_30_sQueueSample.field_60_nEmittingVolume = emitter_vol;
                                    this->field_30_sQueueSample.field_64_max_distance = 50;
                                    this->field_30_sQueueSample.field_58_type = 20;
                                    this->field_30_sQueueSample.field_3C = 400;
                                    this->field_30_sQueueSample.field_18 = 0;
                                    AddSampleToRequestedQueue_41A850();
                                }
                                pCrane = pCrane_;
                            }
                            break;
                        default:
                            break;
                    }
                    new_idx = idx + 1;
                    new_counter = counter + 1;
                    bContinue = (u8)(idx + 1) < 4u;
                    idx = idx + 1; // LOBYTE(idx) =
                    ++counter;
                } while (bContinue);
            }
        }
    }
}

MATCH_FUNC(0x412A60)
void sound_obj::ProcessType9_Crusher_412A60(s32 idx)
{
    Crusher_30* pCrusher = field_147C[idx].field_4_pObj->field_C_pAny.pCrusher_30;
    if (pCrusher)
    {
        if (pCrusher->field_2C_state)
        {
            Fix16 x = pCrusher->field_24_xpos;
            this->field_30_sQueueSample.field_8_obj.field_0 = x;

            Fix16 y = pCrusher->field_28_ypos;
            this->field_30_sQueueSample.field_8_obj.field_4 = y;

            Fix16 z = this->field_1470_v3;
            this->field_30_sQueueSample.field_8_obj.field_8 = z;

            this->field_30_sQueueSample.field_0_EntityIndex = idx;
            this->field_30_sQueueSample.field_5C = 0;
            const char_type bSolidAbove = gMap_0x370_6F6268->CheckColumnHasSolidAbove_4E7FC0(x, y, z);
            this->field_28_dist_related = ComputeEmitterDistanceSquared_4190B0();
            this->field_2C_distCalculated = 0;
            if (CalculateDistance_419020(Fix16(121)))
            {
                if (VolCalc_419070(0x6Eu, Fix16(11), bSolidAbove))
                {
                    this->field_30_sQueueSample.field_14_samp_idx = 60;
                    this->field_30_sQueueSample.field_4_SampleIndex = 0;
                    this->field_30_sQueueSample.field_41 = 0;
                    this->field_30_sQueueSample.field_20_rate = 17000;
                    this->field_30_sQueueSample.field_30 = 0;
                    this->field_30_sQueueSample.field_4C = 5;
                    this->field_30_sQueueSample.field_34 = gSampManager_6FFF00.sub_58DC30(60);
                    this->field_30_sQueueSample.field_38 = gSampManager_6FFF00.sub_58DC50(60);
                    this->field_30_sQueueSample.field_1C_ReleasingVolumeModificator = 6;
                    this->field_30_sQueueSample.field_54 = Fix16(11);
                    this->field_30_sQueueSample.field_60_nEmittingVolume = 110;
                    this->field_30_sQueueSample.field_64_max_distance = 22;
                    this->field_30_sQueueSample.field_58_type = 20;
                    this->field_30_sQueueSample.field_3C = 400;
                    this->field_30_sQueueSample.field_18 = 0;
                    AddSampleToRequestedQueue_41A850();
                }
            }
        }
    }
}

MATCH_FUNC(0x418C80)
void sound_obj::sub_418C80(s32 a2)
{
    if (field_544C[0].field_4_fp)
    {
        if (a2 < 63)
        {
            field_544C[0].field_18 = a2;
        }
    }
}

MATCH_FUNC(0x418CA0)
void sound_obj::ProcessType10_Vocals_418CA0()
{
    s32 voc_idx;
    if (field_544C[0].field_4_fp)
    {
        switch (field_544C[0].field_18)
        {
            case 9:
                voc_idx = 0;
                break;
            case 10:
                voc_idx = 1;
                break;
            case 17:
                voc_idx = 2;
                break;
            case 7:
                voc_idx = 3;
                break;
            case 6:
                voc_idx = 4;
                break;
            case 8:
                voc_idx = 5;
                break;
            case 4:
                voc_idx = 6;
                break;
            case 18:
                voc_idx = 8;
                break;
            case 19:
                voc_idx = 9;
                break;
            case 20:
                voc_idx = 10;
                break;
            case 5:
                voc_idx = 12;
                break;
            case 2:
                voc_idx = 13;
                break;
            case 1:
                voc_idx = 14;
                break;
            case 22:
                voc_idx = 15;
                break;
            case 23:
                voc_idx = 16;
                break;
            case 12:
                voc_idx = 17;
                break;
            case 11:
                voc_idx = 18;
                break;
            case 24:
                voc_idx = 19;
                break;
            case 25:
                voc_idx = 20;
                break;
            case 26:
                voc_idx = 21;
                break;
            case 27:
                voc_idx = 22;
                break;
            case 28:
                voc_idx = 23;
                break;
            case 29:
                voc_idx = 24;
                break;
            case 3:
                voc_idx = 25;
                break;
            case 30:
                voc_idx = 26;
                break;
            case 33:
                voc_idx = 55;
                break;
            case 34:
                voc_idx = 56;
                break;
            case 35:
                voc_idx = 57;
                break;
            case 36:
                voc_idx = 58;
                break;
            case 37:
                voc_idx = 59;
                break;
            case 38:
                voc_idx = 60;
                break;
            case 39:
                voc_idx = 61;
                break;
            case 40:
                voc_idx = 62;
                break;
            case 41:
                voc_idx = 63;
                break;
            case 42:
                voc_idx = 64;
                break;
            case 43:
                voc_idx = 65;
                break;
            case 44:
                voc_idx = 66;
                break;
            case 45:
                voc_idx = 67;
                break;
            case 46:
                voc_idx = 68;
                break;
            case 47:
                voc_idx = 69;
                break;
            case 48:
                voc_idx = 70;
                break;
            case 49:
                voc_idx = 71;
                break;
            case 50:
                voc_idx = 72;
                break;
            case 51:
                voc_idx = 73;
                break;
            case 52:
                voc_idx = 74;
                break;
            case 53:
                voc_idx = 75;
                break;
            case 54:
                voc_idx = 76;
                break;
            case 55:
                voc_idx = 77;
                break;
            case 56:
                voc_idx = 78;
                break;
            case 57:
                voc_idx = 79;
                break;
            case 58:
                voc_idx = 80;
                break;
            case 59:
                voc_idx = 81;
                break;
            case 60:
                voc_idx = 82;
                break;
            case 61:
                voc_idx = 83;
                break;
            case 62:
                voc_idx = 84;
                break;
            case 31:
            case 32:
                voc_idx = (field_1454_anRandomTable[0] % 13u) + 85;
                if (voc_idx == 99)
                {
                    field_544C[0].field_18 = 0;
                    return;
                }
                break;
            default:
                field_544C[0].field_18 = 0;
                return;
        }

        gSampManager_6FFF00.PlayVocal_58E510(1, voc_idx, 1);
        gSampManager_6FFF00.SetVocalVolume_58E6D0(1, 127 * field_24_sfx_vol / 127);

        field_544C[0].field_18 = 0;
    }
}

MATCH_FUNC(0x418B60)
void sound_obj::ProcessType11_HudPager_418B60(s32 a2)
{
    Hud_Pager_C* pPager = field_147C[a2].field_4_pObj->field_C_pAny.pHud_Pager_C;
    if (pPager)
    {
        if (pPager->field_0_timer > 0)
        {
            u8 vol;
            if (pPager->field_0_timer > 300)
            {
                field_30_sQueueSample.field_20_rate = 22050;
                vol = 40;
            }
            else if (pPager->field_0_timer > 150)
            {
                field_30_sQueueSample.field_20_rate = 26221;
                vol = 55;
            }
            else
            {
                field_30_sQueueSample.field_20_rate = 33178;
                vol = 70;
            }

            field_30_sQueueSample.field_14_samp_idx = 57;
            field_30_sQueueSample.field_0_EntityIndex = a2;
            field_30_sQueueSample.field_5C = 0;
            field_30_sQueueSample.field_24_nVolume = vol;
            field_30_sQueueSample.field_60_nEmittingVolume = vol;
            field_30_sQueueSample.field_64_max_distance = 100;
            field_30_sQueueSample.field_58_type = 20;
            field_30_sQueueSample.field_54 = Fix16(100);
            field_30_sQueueSample.field_4_SampleIndex = 0;
            field_30_sQueueSample.field_41 = 0;
            field_30_sQueueSample.field_18 = 1;
            field_30_sQueueSample.field_1C_ReleasingVolumeModificator = 0;
            field_30_sQueueSample.field_3C = 0;
            field_30_sQueueSample.field_30 = 0;
            field_30_sQueueSample.field_34 = 0;
            field_30_sQueueSample.field_38 = -1;
            field_30_sQueueSample.field_40_pan = 63;
            field_30_sQueueSample.field_4C = 3;

            AddSampleToRequestedQueue_41A850();
        }
    }
}

MATCH_FUNC(0x412490)
void sound_obj::ProcessType2_412490(s32 idx)
{
    static BYTE byte_66F2D4;
    static BYTE byte_66F540;

    infallible_turing* field_C_pObject = field_147C[idx].field_4_pObj->field_C_pAny.pInfallible_turing;
    if (!field_C_pObject)
    {
        return;
    }

    if (field_C_pObject->field_4_bStatus)
    {
        field_3 = 0;
        if (gSampManager_6FFF00.StreamStatus_58E2C0() || !byte_66F540)
        {
            gSampManager_6FFF00.CloseStream_58E460();
            gSampManager_6FFF00.OpenStream_58E320(2u);
            gSampManager_6FFF00.StreamSetVolume_58E2F0((6 * field_25_cdVol) / 10);
            byte_66F540 = 1;
        }
    }
    else
    {
        if (field_3)
        {
            if (gSampManager_6FFF00.StreamStatus_58E2C0())
            {
                gSampManager_6FFF00.CloseStream_58E460();
                gSampManager_6FFF00.OpenStream_58E320(0);
                gSampManager_6FFF00.StreamSetVolume_58E2F0((6 * field_25_cdVol) / 10);
            }
        }
        else
        {
            if (gSampManager_6FFF00.StreamStatus_58E2C0())
            {
                gSampManager_6FFF00.CloseStream_58E460();
                gSampManager_6FFF00.OpenStream_58E320(1u);
                gSampManager_6FFF00.StreamSetVolume_58E2F0((6 * field_25_cdVol) / 10);
            }
        }
    }

    s32 sampIdx1;
    s32 sampIdx2;
    switch (field_C_pObject->field_0_object_type)
    {
        case 1:
            sampIdx1 = 0;
            sampIdx2 = 1;
            break;
        case 2: // call this this function is guarded by type 2, so all other cases are dead?
            sampIdx1 = 2;
            sampIdx2 = 3;
            break;
        case 3:
            sampIdx1 = 4;
            sampIdx2 = 5;
            break;
        case 4:
            sampIdx1 = 6;
            sampIdx2 = 7;
            break;
        case 5:
            sampIdx1 = 8;
            sampIdx2 = 9;
            break;
        case 6:
            sampIdx1 = 10;
            sampIdx2 = 11;
            break;
        case 7:
            sampIdx1 = 12;
            sampIdx2 = 13;
            break;
        case 8:
            sampIdx1 = 14;
            sampIdx2 = 15;
            break;
        case 9:
            sampIdx1 = 16;
            sampIdx2 = 17;
            break;

        default:
            return;
    }

    // Add first sample
    field_30_sQueueSample.field_0_EntityIndex = idx;
    field_30_sQueueSample.field_5C = 0;
    field_30_sQueueSample.field_18 = 1;
    field_30_sQueueSample.field_8_obj.field_0 = k_dword_66F3F0;
    field_30_sQueueSample.field_8_obj.field_4 = k_dword_66F3F0;
    field_30_sQueueSample.field_8_obj.field_8 = k_dword_66F3F0;
    field_30_sQueueSample.field_1C_ReleasingVolumeModificator = 0;
    field_30_sQueueSample.field_24_nVolume = 127;
    field_30_sQueueSample.field_28_distance = k_dword_66F3F0;
    field_30_sQueueSample.field_30 = 1;
    field_30_sQueueSample.field_34 = 0;
    field_30_sQueueSample.field_38 = -1;
    field_30_sQueueSample.field_3C = 0;
    field_30_sQueueSample.field_41 = 1;
    field_30_sQueueSample.field_54 = Fix16(5);
    field_30_sQueueSample.field_58_type = 20;
    field_30_sQueueSample.field_60_nEmittingVolume = 127;
    field_30_sQueueSample.field_64_max_distance = 10;
    field_30_sQueueSample.field_14_samp_idx = sampIdx1;
    field_30_sQueueSample.field_40_pan = 0;
    field_30_sQueueSample.field_20_rate = gSampManager_6FFF00.GetPlayBackRateIdx_58DBF0(sampIdx1);
    field_30_sQueueSample.field_4_SampleIndex = byte_66F2D4++;
    if (byte_66F2D4 >= 0xFF)
    {
        byte_66F2D4 = 0;
    }
    AddSampleToRequestedQueue_41A850();

    // Add second sample
    field_30_sQueueSample.field_14_samp_idx = sampIdx2;
    field_30_sQueueSample.field_40_pan = 127;
    field_30_sQueueSample.field_20_rate = gSampManager_6FFF00.GetPlayBackRateIdx_58DBF0(sampIdx2);
    field_30_sQueueSample.field_4_SampleIndex = byte_66F2D4++;
    if (byte_66F2D4 >= 0xFF)
    {
        byte_66F2D4 = 0;
    }
    AddSampleToRequestedQueue_41A850();
}

MATCH_FUNC(0x412260)
char_type sound_obj::sub_412260(sound_0x68* pObj)
{
    if (gGame_0x40_67E008 && field_1478_type5Idx && !field_1_isPaused)
    {
        switch (pObj->field_58_type)
        {
            case 2:
                return Type_2_4182A0(pObj);
            case 1:
            case 6:
                return Type_1_6_416260(pObj);
            case 3:
                return Type_3_HandleCarImpactSound_4174C0(pObj);
            case 4:
                return Type_4_417A00(pObj);
            case 5:
                return Type_5_InitEngineSoundProfile_415730(pObj);
            case 16:
                return Type_16_414320(pObj);
            case 17:
                return Type_17_414690(pObj);
            case 19:
                return Type_19_4149D0(pObj);
            case 8:
                return Type_8_418130(pObj);
            case 9:
                return Type_9_4186D0(pObj);
            case 10:
                return Type_10_HandleCarSkidSound_418940(pObj);
            case 11:
                return Type_11_414EE0(pObj);
            case 12:
                return Type_12_414C90(pObj);
            case 13:
                return Type_13_4153F0(pObj);
            case 7:
                return Type_7_417EF0(pObj);
            case 15:
                return Type_15_415100(pObj);
            case 20:
                return 1;
            default:
                return 0;
        }
    }
    return gFrontend_67DC84 && pObj->field_58_type == 20 ? true : false;
}

STUB_FUNC(0x416260)
char_type sound_obj::Type_1_6_416260(sound_0x68* a1)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x4174C0)
char_type sound_obj::Type_3_HandleCarImpactSound_4174C0(sound_0x68* a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x417A00)
char_type sound_obj::Type_4_417A00(sound_0x68* a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x415730)
char_type sound_obj::Type_5_InitEngineSoundProfile_415730(sound_0x68* a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x418940)
char_type sound_obj::Type_10_HandleCarSkidSound_418940(sound_0x68* a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x414EE0)
char_type sound_obj::Type_11_414EE0(sound_0x68* p68)
{
    p68->field_14_samp_idx = 60;
    p68->field_3C = 400;
    p68->field_4C = 3;
    p68->field_20_rate = 9000;
    p68->field_30 = 0;
    p68->field_34 = gSampManager_6FFF00.sub_58DC30(60);
    p68->field_38 = gSampManager_6FFF00.sub_58DC50(60);
    return 1;
}

MATCH_FUNC(0x414C90)
char_type sound_obj::Type_12_414C90(sound_0x68* p68)
{
    p68->field_14_samp_idx = 35;
    p68->field_3C = 400;
    p68->field_4C = 5;
    p68->field_20_rate = gSampManager_6FFF00.GetPlayBackRateIdx_58DBF0(35);
    p68->field_30 = 0;
    p68->field_34 = gSampManager_6FFF00.sub_58DC30(35);
    p68->field_38 = gSampManager_6FFF00.sub_58DC50(35);
    return 1;
}

MATCH_FUNC(0x415100)
char_type sound_obj::Type_15_415100(sound_0x68* a1)
{
    a1->field_14_samp_idx = 57;
    a1->field_3C = 400;
    a1->field_4C = 3;
    a1->field_20_rate = 22050;
    a1->field_30 = 0;
    a1->field_34 = gSampManager_6FFF00.sub_58DC30(57);
    a1->field_38 = gSampManager_6FFF00.sub_58DC50(57);
    return 1;
}

MATCH_FUNC(0x414320)
char_type sound_obj::Type_16_414320(sound_0x68* pObj)
{
    pObj->field_14_samp_idx = 10;
    pObj->field_3C = 400;
    pObj->field_30 = 0;
    pObj->field_34 = 0;
    pObj->field_38 = -1;
    pObj->field_20_rate = gSampManager_6FFF00.GetPlayBackRateIdx_58DBF0(10);
    pObj->field_4C = 30;
    return 1;
}

MATCH_FUNC(0x414690)
char_type sound_obj::Type_17_414690(sound_0x68* pObj)
{
    pObj->field_20_rate = 11025;
    pObj->field_14_samp_idx = 11;
    pObj->field_3C = 500;
    pObj->field_30 = 0;
    pObj->field_34 = 0;
    pObj->field_38 = -1;
    pObj->field_4C = 30;
    return 1;
}

MATCH_FUNC(0x4149D0)
char_type sound_obj::Type_19_4149D0(sound_0x68* pObj)
{
    pObj->field_3C = 400;
    pObj->field_30 = 1;
    pObj->field_34 = 0;
    pObj->field_38 = -1;
    pObj->field_20_rate = gSampManager_6FFF00.GetPlayBackRateIdx_58DBF0(pObj->field_14_samp_idx);
    return 1;
}

MATCH_FUNC(0x41B4E0)
void sound_obj::VecDiff_41B4E0(serene_brattain* pVec, serene_brattain* pRet)
{
    pRet->field_0 = pVec->field_0 - field_1468_v1;
    pRet->field_4 = pVec->field_4 - field_146C_v2;
    pRet->field_8 = pVec->field_8 - field_1470_v3;
}

MATCH_FUNC(0x41B490)
void sound_obj::sub_41B490(sound_0x68* pObj)
{
    if (pObj->field_41)
    {
        if (pObj->field_30)
        {
            pObj->field_44 = (static_cast<u32>(field_8) * gSampManager_6FFF00.GetSampleLength_maybe_58DC70(pObj->field_14_samp_idx)) /
                pObj->field_20_rate;
        }
        else
        {
            pObj->field_44 = -3;
        }
    }
}

MATCH_FUNC(0x4190B0)
Fix16 sound_obj::ComputeEmitterDistanceSquared_4190B0()
{
    Fix16 v2 = field_30_sQueueSample.field_8_obj.field_0 - field_1468_v1;
    Fix16 v3 = field_30_sQueueSample.field_8_obj.field_4 - field_146C_v2;
    return v3 * v3 + v2 * v2;
}

MATCH_FUNC(0x427310)
BYTE sound_obj::GetQueuedRadioWordCount_427310()
{
    // note: return value and'ed with 0xFF if return type is wider
    if (field_5529_idx15 < field_5528_idx15_cur)
    {
        return field_5528_idx15_cur - field_5529_idx15 - 1;
    }
    else
    {
        return field_5528_idx15_cur - field_5529_idx15 + 14;
    }
}

// TODO: Arg types ??
MATCH_FUNC(0x427340)
void sound_obj::EnqueueRadioCrimeCallout_427340(s32 a4, u8 a5, u8 a6)
{
    if (GetQueuedRadioWordCount_427310() >= 8u)
    {
        EnqueueRadioWord_4271B0(120u);
        if ((field_5448_m_FrameCounter & 1) == 0)
        {
            EnqueueRadioWord_4271B0(70u);
        }
        else
        {
            EnqueueRadioWord_4271B0(69u);
        }
        EnqueueRadioWord_4271B0(73u);
        EnqueueRadioWord_4271B0(a4 + 73);
        EnqueueRadioWord_4271B0(88u);
        EnqueueRadioLocationPhrase_426E10(a5, a6); // TODO: Arg types ??
    }
}

WIP_FUNC(0x426E10)
void sound_obj::EnqueueRadioLocationPhrase_426E10(u8 xpos, u8 ypos)
{
    WIP_IMPLEMENTED;

    char_type bUnknown = 0;
    gmp_map_zone* pZone = 0;
    const u32 cop_zone = sound_obj::GetCopRadioZoneIndex_427400(xpos, ypos, &pZone);
    if (!cop_zone && !pZone)
    {
        sound_obj::EnqueueRadioWord_4271B0(0x75u);
        sound_obj::EnqueueRadioWord_4271B0(0x77u);
        return;
    }

    const u32 word_zone_name = cop_zone + 121;

    const s16 mid_x = pZone->field_1_x + (pZone->field_3_w >> 1);
    const s16 w_half = pZone->field_3_w >> 2;
    const s16 mid_y = pZone->field_2_y + (pZone->field_4_h >> 1);
    const s16 h_half = pZone->field_4_h >> 2;

    if (ypos < mid_y - h_half)
    {
        sound_obj::EnqueueRadioWord_4271B0(0x73u);
    LABEL_8:
        bUnknown = 1;
        goto LABEL_9;
    }
    if (ypos > h_half + mid_y)
    {
        sound_obj::EnqueueRadioWord_4271B0(0x75u);
        bUnknown = 1;
        goto LABEL_9;
    }
LABEL_9:
    if (xpos <= mid_x + w_half)
    {
        if (xpos >= mid_x - w_half)
        {
            if (!bUnknown)
            {
                sound_obj::EnqueueRadioWord_4271B0(0x77u);
            }
        }
        else
        {
            sound_obj::EnqueueRadioWord_4271B0(0x76u);
        }
    }
    else
    {
        sound_obj::EnqueueRadioWord_4271B0(0x74u);
    }
    sound_obj::EnqueueRadioWord_4271B0(word_zone_name);
}

STUB_FUNC(0x57ECB0)
void sound_obj::DeclareRadioStation_57ECB0(s32 station_idx, Fix16 xpos, Fix16 ypos)
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x57EDB0)
void sound_obj::sub_57EDB0(s32 a1, s32 a2)
{
    NOT_IMPLEMENTED;
}
STUB_FUNC(0x57E6C0)
char_type sound_obj::ChooseRadioEmitterForVehicle_57E6C0()
{
    NOT_IMPLEMENTED;
    return 0;
}

WIP_FUNC(0x426F20)
void sound_obj::GenerateRadioVehicleDescription_426F20(Car_BC* pCar)
{
    WIP_IMPLEMENTED;

    u32 car_name_word;
    u32 car_colour_word;

    u32 unknown = 321;
    if (!pCar)
    {
        if (this->field_5574_car_info_idx != 87)
        {
            if (sound_obj::GetQueuedRadioWordCount_427310() > 3u)
            {
                sound_obj::EnqueueRadioWord_4271B0(0x79u);
                sound_obj::EnqueueRadioWord_4271B0(0x62u);
                sound_obj::EnqueueRadioWord_4271B0(0x63u);
                sound_obj::EnqueueRadioWord_4271B0(0x6Eu);
            }
            this->field_5574_car_info_idx = 87;
        }
    }
    else
    {
        const s32 car_info_idx = pCar->field_84_car_info_idx;
        if (this->field_5574_car_info_idx != car_info_idx || word_6757FC != (u16)pCar->field_50_car_sprite->field_24_remap)
        {
            // TODO: Switch base is wrong, fix that and then this func matches
            this->field_5574_car_info_idx = car_info_idx;
            switch (car_info_idx)
            {
                case car_model_enum::bug:
                case car_model_enum::FIAT:
                case car_model_enum::ISETTA:
                case car_model_enum::MESSER:
                case car_model_enum::SPRITE:
                    car_name_word = 111;
                    break;

                case car_model_enum::VAN:
                case car_model_enum::VESPA:
                    car_name_word = 111;
                    unknown = 114;
                    break;

                case car_model_enum::boxtruck:
                case car_model_enum::TOWTRUCK:
                case car_model_enum::TRUKCAB1:
                case car_model_enum::TRUKCAB2:
                    car_name_word = 114;
                    break;

                case car_model_enum::alfa:
                case car_model_enum::amdb4:
                case car_model_enum::DART:
                case car_model_enum::MERC:
                case car_model_enum::MORGAN:
                case car_model_enum::MORRIS:
                case car_model_enum::SPIDER:
                    car_name_word = 112;
                    break;

                case car_model_enum::allard:
                case car_model_enum::bmw:
                case car_model_enum::GT24640:
                case car_model_enum::JEFFREY:
                case car_model_enum::MIURA:
                case car_model_enum::STINGRAY:
                case car_model_enum::STRATOS:
                case car_model_enum::STRATOSB:
                case car_model_enum::STRIPETB:
                case car_model_enum::T2000GT:
                case car_model_enum::TBIRD:
                case car_model_enum::TRANCEAM:
                case car_model_enum::WBTWIN:
                case car_model_enum::ZCX5:
                    car_name_word = 113;
                    break;
                default:
                    return;
            }

            word_6757FC = (u16)pCar->field_50_car_sprite->field_24_remap;
            switch (word_6757FC)
            {
                case 1:
                case 2:
                case 27:
                    car_colour_word = 108;
                    break;
                case 11:
                case 33:
                case 34:
                    car_colour_word = 109;
                    break;
                case 6:
                case 28:
                    car_colour_word = 104;
                    break;

                case 5:
                    car_colour_word = 105;
                    break;
                case 0:
                case 3:
                case 4:
                    car_colour_word = 106;
                    break;

                case 10:
                case 19:
                case 22:
                case 35:
                    car_colour_word = 107;
                    break;

                default:
                    car_colour_word = 321;
                    break;
            }

            if (sound_obj::GetQueuedRadioWordCount_427310() > 6u)
            {
                sound_obj::EnqueueRadioWord_4271B0(0x79u);
                sound_obj::EnqueueRadioWord_4271B0(0x62u);
                if ((this->field_1454_anRandomTable[2] & 1) == 0)
                {
                    sound_obj::EnqueueRadioWord_4271B0(0x63u);
                }
                else
                {
                    sound_obj::EnqueueRadioWord_4271B0(0x66u);
                }
                sound_obj::EnqueueRadioWord_4271B0(0x64u);
                if (car_colour_word != 321)
                {
                    sound_obj::EnqueueRadioWord_4271B0(car_colour_word);
                }
                sound_obj::EnqueueRadioWord_4271B0(car_name_word);
                if (unknown != 321)
                {
                    sound_obj::EnqueueRadioWord_4271B0(unknown);
                }
            }
        }
    }
}

MATCH_FUNC(0x427400)
u32 sound_obj::GetCopRadioZoneIndex_427400(u8 x, u8 y, gmp_map_zone** ppZone)
{
    *ppZone = gMap_0x370_6F6268->zone_by_pos_and_type_4DF4D0(x, y, 15u);
    if (!(*ppZone) || (*ppZone)->field_5_name_length != 3)
    {
        *ppZone = gMap_0x370_6F6268->zone_by_pos_and_type_4DF4D0(x, y, 1u);
    }

    if (!*ppZone || (*ppZone)->field_5_name_length != 3 || (*ppZone)->field_6_name[0] != 'm')
    {
        *ppZone = 0;
        return 0;
    }

    u32 result = (*ppZone)->field_6_name[2] + 10 * (*ppZone)->field_6_name[1] - 528;
    if (result < 45)
    {
        if (result > 30)
        {
            result -= 30;
        }
        else if (result > 15)
        {
            result -= 15;
        }

        if (result <= 14)
        {
            return result;
        }
    }
    *ppZone = 0;
    return 0;
}

STUB_FUNC(0x417BA0)
s32 sound_obj::GetVehicleAudioClass_417BA0(s32 a1)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x4157C0)
void sound_obj::HandleAICarEngineRevSound_4157C0(Sound_Params_8* a2)
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x418190)
void sound_obj::HandleAICarEngineSound_418190(Sound_Params_8* a2)
{
    NOT_IMPLEMENTED;
}

WIP_FUNC(0x413D10)
void sound_obj::HandleAICarHornBeep_413D10(Sound_Params_8* a2)
{
    WIP_IMPLEMENTED;

    Car_BC* pCar; // eax
    s32 fAC; // ecx
    bool bMaxDmg; // zf
    s32 samp_idx; // edx
    u8 emit_vol; // bl
    s32 f14_samp_idx; // ecx
    s32 rate; // edi
    s32 displacement; // eax

    pCar = a2->field_0_pObj->field_8_car_bc_ptr;
    fAC = pCar->field_AC;
    bMaxDmg = pCar->field_74_damage == 32001;
    pCar->field_AC = 0;
    if (!bMaxDmg && pCar->field_54_driver && a2->field_4_bDrivenByPlayer != 1)
    {
        switch (pCar->field_84_car_info_idx)
        {
            case car_model_enum::apc:
            case car_model_enum::COPCAR:
            case car_model_enum::FIRETRUK:
            case car_model_enum::GUNJEEP:
            case car_model_enum::JEEP:
            case car_model_enum::MEDICAR:
            case car_model_enum::SWATVAN:
            case car_model_enum::TANK:
            case car_model_enum::EDSELFBI:
                return;

            default:
                if (fAC <= 0)
                {
                    return;
                }

                // TODO: Prob a switch or something here
                if (fAC <= 2)
                {
                    samp_idx = (this->field_1454_anRandomTable[this->field_30_sQueueSample.field_0_EntityIndex % 5u] & 7) + 257;
                }
                else
                {
                    if (fAC != 3)
                    {
                        return;
                    }
                    if (byte_66F543)
                    {
                        --byte_66F543;
                        return;
                    }
                    byte_66F543 = 8;
                    samp_idx = this->field_1454_anRandomTable[this->field_30_sQueueSample.field_0_EntityIndex % 5u] % 7u + 265;
                }

                this->field_30_sQueueSample.field_14_samp_idx = samp_idx;

                if (CalculateDistance_419020(Fix16(1806336, 0)))
                {
                    emit_vol = this->field_1454_anRandomTable[this->field_30_sQueueSample.field_0_EntityIndex % 5u] % 30u + 50;
                    if (VolCalc_419070(emit_vol, Fix16(172032, 0), a2->field_5_bHasSolidAbove))
                    {
                        f14_samp_idx = this->field_30_sQueueSample.field_14_samp_idx;
                        this->field_30_sQueueSample.field_54 = Fix16(172032, 0);
                        this->field_30_sQueueSample.field_60_nEmittingVolume = emit_vol;
                        this->field_30_sQueueSample.field_64_max_distance = 21;
                        rate = gSampManager_6FFF00.GetPlayBackRateIdx_58DBF0(f14_samp_idx);
                        displacement = RandomDisplacement_41A650(field_30_sQueueSample.field_14_samp_idx);
                        this->field_30_sQueueSample.field_58_type = 20;
                        this->field_30_sQueueSample.field_20_rate = displacement + rate;
                        this->field_30_sQueueSample.field_3C = 0;
                        this->field_30_sQueueSample.field_4_SampleIndex = gSoundSampleIdx_61A684;
                        this->field_30_sQueueSample.field_41 = 1;
                        this->field_30_sQueueSample.field_1C_ReleasingVolumeModificator = 7;
                        this->field_30_sQueueSample.field_18 = 0;
                        this->field_30_sQueueSample.field_34 = 0;
                        this->field_30_sQueueSample.field_38 = -1;
                        this->field_30_sQueueSample.field_30 = 1;
                        AddSampleToRequestedQueue_41A850();
                        if (++gSoundSampleIdx_61A684 >= 200u)
                        {
                            gSoundSampleIdx_61A684 = 100;
                        }
                    }
                }
                break;
        }
    }
}

WIP_FUNC(0x415570)
void sound_obj::HandleCarAlarmSound_415570(Sound_Params_8* a2, sound_unknown_0xC* pAlloc)
{
    WIP_IMPLEMENTED;

    u8 f_A7;

    Car_BC* pCar = a2->field_0_pObj->field_8_car_bc_ptr;
    if (pCar->IsFireTruck_4118F0() || !pCar->sub_414F20() || !pCar->sub_414F80())
    {
        if (!pCar->IsFireTruck_4118F0() && !pCar->IsTank_411900() && !pCar->IsGunJeep_411910() ||
            !pCar->field_B8)
        {
            if (a2->field_0_pObj->field_8_car_bc_ptr->IsEmittingHorn_411970())
            {
                if (a2->field_4_bDrivenByPlayer)
                {
                    goto LABEL_28;
                }
                if (pCar->GetCarInfoIdx_411940() == car_model_enum::ICECREAM)
                {
                    goto LABEL_28;
                }

                if (pCar->IsEmittingHorn_411990())
                {
                    f_A7 = pCar->field_A7_horn;
                    if ((u8)f_A7 >= 44u)
                    {
                        pAlloc->field_A = this->field_1454_anRandomTable[4] & 7;
                        f_A7 = 44;
                    }

                    if (byte_5FE434[pAlloc->field_A][(u8)(44 - f_A7)])
                    {
                    LABEL_28:
                        if (CalculateDistance_419020((Fix16(15) / Fix16(2)) * (Fix16(15) / Fix16(2))))
                        {
                            const u8 vol = pCar->field_84_car_info_idx == car_model_enum::TRAINCAB ? 127 : 20;
                            if (VolCalc_419070(vol, Fix16(15) / Fix16(2), a2->field_5_bHasSolidAbove))
                            {
                                this->field_30_sQueueSample.field_54 = Fix16(15) / Fix16(2);
                                this->field_30_sQueueSample.field_60_nEmittingVolume = vol;
                                this->field_30_sQueueSample.field_64_max_distance = 15;
                                this->field_30_sQueueSample.field_58_type = 5;
                                this->field_30_sQueueSample.field_4_SampleIndex = 8;
                                this->field_30_sQueueSample.field_41 = 0;
                                this->field_30_sQueueSample.field_18 = 0;
                                if (a2->field_4_bDrivenByPlayer)
                                {
                                    this->field_30_sQueueSample.field_1C_ReleasingVolumeModificator = 2;
                                }
                                else
                                {
                                    this->field_30_sQueueSample.field_1C_ReleasingVolumeModificator = 4;
                                }
                                AddSampleToRequestedQueue_41A850();
                            }
                        }
                    }
                }
            }
        }
    }
}

MATCH_FUNC(0x414F90)
void sound_obj::HandleCarBurningSound_414F90(Sound_Params_8* a2)
{
    if (a2->field_0_pObj->field_8_car_bc_ptr->field_0_qq.GetSpriteForModel_5A6A50(132))
    {
        if (CalculateDistance_419020(Fix16(25)))
        {
            if (VolCalc_419070(0x28u, Fix16(5), a2->field_5_bHasSolidAbove))
            {
                this->field_30_sQueueSample.field_58_type = 15;
                this->field_30_sQueueSample.field_4_SampleIndex = 15;
                this->field_30_sQueueSample.field_54 = Fix16(5);
                this->field_30_sQueueSample.field_60_nEmittingVolume = 40;
                this->field_30_sQueueSample.field_64_max_distance = 10;
                this->field_30_sQueueSample.field_41 = 0;
                this->field_30_sQueueSample.field_18 = 0;
                this->field_30_sQueueSample.field_1C_ReleasingVolumeModificator = 7;
                AddSampleToRequestedQueue_41A850();
            }
        }
    }
}

WIP_FUNC(0x4177D0)
void sound_obj::HandleCarDamageSound_4177D0(Sound_Params_8* a2)
{
    WIP_IMPLEMENTED;

    Car_BC* pCar = a2->field_0_pObj->field_8_car_bc_ptr;
    if (pCar->field_9C == 3 && pCar->field_74_damage > 16000)
    {
        if (CalculateDistance_419020(Fix16(802816, 0)))
        {
            s32 vol = 60 * (a2->field_0_pObj->field_8_car_bc_ptr->field_74_damage - 16000) / 16000;
            if (VolCalc_419070((char)(60 * (a2->field_0_pObj->field_8_car_bc_ptr->field_74_damage + 128)) / -128,
                               Fix16(0x1C000, 0),
                               a2->field_5_bHasSolidAbove))
            {
                this->field_30_sQueueSample.field_54 = Fix16(0x1C000, 0);
                this->field_30_sQueueSample.field_60_nEmittingVolume = vol;
                this->field_30_sQueueSample.field_64_max_distance = 14;
                this->field_30_sQueueSample.field_58_type = 6;
                this->field_30_sQueueSample.field_4_SampleIndex = 1;
                this->field_30_sQueueSample.field_41 = 0;
                this->field_30_sQueueSample.field_18 = 0;
                this->field_30_sQueueSample.field_1C_ReleasingVolumeModificator = 7;
                AddSampleToRequestedQueue_41A850();
            }
        }
    }
}

STUB_FUNC(0x4182E0)
void sound_obj::HandleCarDoorSounds_4182E0(Sound_Params_8* a2)
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x417060)
void sound_obj::HandleCarHornSound_417060(Sound_Params_8* a2)
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x418720)
void sound_obj::HandleCarTireScrubSound_418720(Sound_Params_8* a2)
{
    NOT_IMPLEMENTED;
}

MATCH_FUNC(0x415480)
void sound_obj::HandleCarWeaponHitSound_415480(Sound_Params_8* a2)
{
    Car_BC* pCar = a2->field_0_pObj->field_8_car_bc_ptr;
    const s32 old_weapon_kind = pCar->field_B4_weapon_kind;
    pCar->field_B4_weapon_kind = 0;
    switch (old_weapon_kind)
    {
        case weapon_type::smg:
            this->field_30_sQueueSample.field_14_samp_idx = 67;
            break;
        case weapon_type::rocket:
        case weapon_type::shocker:
            this->field_30_sQueueSample.field_14_samp_idx = 66;
            break;
        case weapon_type::molotov:
            this->field_30_sQueueSample.field_14_samp_idx = 65;
            break;
        case weapon_type::grenade:
            this->field_30_sQueueSample.field_14_samp_idx = 62;
            break;
        case weapon_type::shotgun:
            this->field_30_sQueueSample.field_14_samp_idx = 63;
            break;
        case weapon_type::electro_batton:
            this->field_30_sQueueSample.field_14_samp_idx = 64;
            break;
        default:
            return;
    }

    this->field_30_sQueueSample.field_60_nEmittingVolume = 50;
    this->field_30_sQueueSample.field_64_max_distance = 100;
    this->field_30_sQueueSample.field_58_type = 20;
    this->field_30_sQueueSample.field_54 = Fix16(1638400, 0);
    this->field_30_sQueueSample.field_4_SampleIndex = 3;
    this->field_30_sQueueSample.field_41 = 1;
    this->field_30_sQueueSample.field_18 = 1;
    this->field_30_sQueueSample.field_1C_ReleasingVolumeModificator = 0;
    this->field_30_sQueueSample.field_3C = 0;
    this->field_30_sQueueSample.field_20_rate = gSampManager_6FFF00.GetPlayBackRateIdx_58DBF0(field_30_sQueueSample.field_14_samp_idx);
    this->field_30_sQueueSample.field_30 = 1;
    this->field_30_sQueueSample.field_34 = 0;
    this->field_30_sQueueSample.field_38 = -1;
    this->field_30_sQueueSample.field_40_pan = 63;
    AddSampleToRequestedQueue_41A850();
}

STUB_FUNC(0x417E30)
void sound_obj::HandleHeavyVehicleStopSound_417E30(Sound_Params_8* a2, sound_unknown_0xC* a3)
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x423080)
void sound_obj::HandlePedVoiceEvent_423080(Sound_Params_8* a2)
{
    NOT_IMPLEMENTED;
}

MATCH_FUNC(0x4178C0)
void sound_obj::HandleSirenActivationSound_4178C0(Sound_Params_8* a2)
{
    Car_BC* pCar = a2->field_0_pObj->field_8_car_bc_ptr;
    if ((pCar->inline_info_flags_bit2() || pCar->is_FBI_car_411920()) && (pCar->field_A4 & 4))
    {
        Car_BC* pCar_ = a2->field_0_pObj->field_8_car_bc_ptr;
        if (pCar_->field_9C == 3)
        {
            if (pCar_->field_54_driver)
            {
                if (CalculateDistance_419020(Fix16(400)))
                {
                    if (VolCalc_419070(0x46u, Fix16(20), a2->field_5_bHasSolidAbove))
                    {
                        this->field_30_sQueueSample.field_54 = Fix16(20);
                        this->field_30_sQueueSample.field_60_nEmittingVolume = 70;
                        this->field_30_sQueueSample.field_64_max_distance = 40;
                        this->field_30_sQueueSample.field_58_type = 4;
                        this->field_30_sQueueSample.field_4_SampleIndex = 7;
                        this->field_30_sQueueSample.field_41 = 0;
                        this->field_30_sQueueSample.field_18 = 0;
                        if (a2->field_4_bDrivenByPlayer)
                        {
                            this->field_30_sQueueSample.field_1C_ReleasingVolumeModificator = 2;
                        }
                        else
                        {
                            this->field_30_sQueueSample.field_1C_ReleasingVolumeModificator = 4;
                        }
                        AddSampleToRequestedQueue_41A850();
                    }
                }
            }
        }
    }
}

STUB_FUNC(0x4143A0)
void sound_obj::HandleTrainCabRollingFrictionSound_4143A0(Sound_Params_8* a2)
{
    NOT_IMPLEMENTED;
}

WIP_FUNC(0x4140C0)
void sound_obj::HandleTrainEngineSound_4140C0(Sound_Params_8* a2)
{
    WIP_IMPLEMENTED;

    Fix16 v4 = a2->field_0_pObj->field_8_car_bc_ptr->sub_43A240();
    Fix16 max_speed = dword_6FE258->field_28_max_speed;

    if (v4 > k_dword_66F3F0 && max_speed > k_dword_66F3F0)
    {
        if (CalculateDistance_419020(Fix16(0x4204000, 0)))
        {
            //s32 vol = (int)(((0x104000LL * (int)(((__int64)v4 << 14) / max_speed)) >> 14) + 0x2000) >> 14;
            s32 vol = (v4 / max_speed * Fix16(0x104000, 0) + Fix16(0x2000, 0)).ToInt();
            if ((u8)vol)
            {
                if (VolCalc_419070((u8)vol, 1064960, a2->field_5_bHasSolidAbove))
                {
                    this->field_30_sQueueSample.field_54 = Fix16(1064960, 0);
                    this->field_30_sQueueSample.field_60_nEmittingVolume = vol;
                    this->field_30_sQueueSample.field_64_max_distance = 130;
                    this->field_30_sQueueSample.field_58_type = 16;
                    this->field_30_sQueueSample.field_4_SampleIndex = 0;
                    this->field_30_sQueueSample.field_41 = 0;
                    this->field_30_sQueueSample.field_1C_ReleasingVolumeModificator = 4;
                    this->field_30_sQueueSample.field_18 = 0;
                    AddSampleToRequestedQueue_41A850();
                }
            }
        }
    }
}

STUB_FUNC(0x417FD0)
void sound_obj::HandleTruckCorneringAudio_417FD0(Sound_Params_8* a2)
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x57DF10)
void sound_obj::HandleVocalStreamSwitching_57DF10(char_type a2)
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x426790)
void sound_obj::PoliceRadioMessageGeneration_426790()
{
    NOT_IMPLEMENTED;
}

MATCH_FUNC(0x412B80)
void sound_obj::ProcessCar_412B80(Sound_Params_8* pParams)
{
    Sprite* pSprite = pParams->field_0_pObj;
    Car_BC* pCar = pParams->field_0_pObj->field_8_car_bc_ptr;
    if (pCar)
    {
        pParams->field_4_bDrivenByPlayer = pCar->is_driven_by_player();

        sound_unknown_0xC* pAlloc = field_147C[field_30_sQueueSample.field_0_EntityIndex].field_8_pAlloc;
        if (pAlloc)
        {
            CarPhysics_B0* pPhysics = pSprite->field_8_car_bc_ptr->field_58_physics; // +8 = Car_BC*
            if (pPhysics)
            {
                pPhysics->sub_562EB0();
            }
            else
            {
                pAlloc->field_0 = k_dword_66F3F0;
            }
            switch (pParams->field_0_pObj->field_8_car_bc_ptr->field_84_car_info_idx)
            {
                case car_model_enum::boxcar:
                case car_model_enum::TRAIN:
                case car_model_enum::TRAINFB:
                    sound_obj::ProcessTrain_413BE0(pParams);
                    break;
                case car_model_enum::TANK:
                    sound_obj::ProcessTank_413BF0(pParams, pAlloc);
                    break;
                case car_model_enum::TRAINCAB:
                    sound_obj::ProcessTrainCab_413B90(pParams, pAlloc);
                    break;
                default:
                    sound_obj::ProcessOtherCarTypes_413C50(pParams, pAlloc);
                    break;
            }
        }
    }
}

MATCH_FUNC(0x41E820)
void sound_obj::ProcessObject_41E820(Sound_Params_8* pEntity)
{
    if (pEntity->field_0_pObj->field_8_object_2C_ptr->field_8->field_34_behavior_type == 12)
    {
        sound_obj::ProcessObject_Type12_41E850(pEntity);
    }
    else
    {
        sound_obj::ProcessOtherObjects_41F520(pEntity);
    }
}

STUB_FUNC(0x41E850)
void sound_obj::ProcessObject_Type12_41E850(Sound_Params_8* a2)
{
    NOT_IMPLEMENTED;
}

MATCH_FUNC(0x413C50)
void sound_obj::ProcessOtherCarTypes_413C50(Sound_Params_8* a2, sound_unknown_0xC* pAlloc)
{
    Car_BC* cBC = a2->field_0_pObj->field_8_car_bc_ptr;
    pAlloc->field_4 = (u32)cBC; // TODO: Likely another union??
    if (cBC->field_68 != k_dword_66F3F4)
    {
        if (cBC->field_58_physics)
        {
            HandleAICarEngineRevSound_4157C0(a2);
        }
    }
    else
    {
        if (a2->field_0_pObj->field_8_car_bc_ptr->field_58_physics)
        {
            HandleAICarEngineRevSound_4157C0(a2);
            HandleTruckCorneringAudio_417FD0(a2);
            HandleAICarEngineSound_418190(a2);
            HandleCarTireScrubSound_418720(a2);
        }
        HandleCarDoorSounds_4182E0(a2);
        HandleSirenActivationSound_4178C0(a2);
        HandleCarHornSound_417060(a2);
        HandleCarAlarmSound_415570(a2, pAlloc);
        UpdateCarSurfaceAudio_418610(a2);
        HandleCarDamageSound_4177D0(a2);
        HandleHeavyVehicleStopSound_417E30(a2, pAlloc);
        HandleCarWeaponHitSound_415480(a2);
        HandleCarBurningSound_414F90(a2);
        HandleAICarHornBeep_413D10(a2);
    }
}

STUB_FUNC(0x41F520)
void sound_obj::ProcessOtherObjects_41F520(Sound_Params_8* a2)
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x422B70)
void sound_obj::ProcessPed_422B70(Sound_Params_8* pType3Entity)
{
    NOT_IMPLEMENTED;
}

MATCH_FUNC(0x413BF0)
void sound_obj::ProcessTank_413BF0(Sound_Params_8* a2, sound_unknown_0xC* pAlloc)
{
    if (a2->field_0_pObj->field_8_car_bc_ptr->field_58_physics)
    {
        Tank_414A50(a2);
        HandleAICarEngineRevSound_4157C0(a2);
        Tank_415190(a2);
    }
    Tank_414D30(a2);
    HandleCarDoorSounds_4182E0(a2);
    HandleCarAlarmSound_415570(a2, pAlloc);
    HandleCarWeaponHitSound_415480(a2);
}

MATCH_FUNC(0x413B90)
void sound_obj::ProcessTrainCab_413B90(Sound_Params_8* a2, sound_unknown_0xC* a3)
{
    if (a2->field_0_pObj->field_8_car_bc_ptr->field_58_physics)
    {
        if (!a2->field_5_bHasSolidAbove)
        {
            HandleTrainEngineSound_4140C0(a2);
            HandleTrainCabRollingFrictionSound_4143A0(a2);
        }
    }
    HandleCarAlarmSound_415570(a2, a3);
    HandleCarDoorSounds_4182E0(a2);
    TrainCab_414710(a2);
}

MATCH_FUNC(0x413BE0)
void sound_obj::ProcessTrain_413BE0(Sound_Params_8* a2)
{
    HandleCarDoorSounds_4182E0(a2);
}

MATCH_FUNC(0x426750)
void sound_obj::ResetRadioMessageState_426750()
{
    Init_RadioMessageQueue_427180();
    this->field_5520_bCanPlay = 1;
    this->field_5524_radio_word = 0;
    this->field_556A_erv_en_route_timer = 0;
    this->field_5570_timer = 0;
    this->field_556E_timer = 0;
    this->field_556C_timer = 0;
}

MATCH_FUNC(0x412D30)
char_type sound_obj::SelectObjectImpactSound_1_10_412D30(Rozza_A* pObj)
{
    return SelectObjectImpactSound_413120(pObj, 1);
}

WIP_FUNC(0x413120)
char_type sound_obj::SelectObjectImpactSound_413120(Rozza_A* pObj, s32 interactionType)
{
    WIP_IMPLEMENTED;

    s32 model_copy; // eax
    char_type result; // al
    u32 rnd1_mod; // et2
    s32 rate__; // edi MAPDST
    s32 rate_plus_displacement; // edi
    s32 rnd_samp_idx; // eax
    s32 samp_idx; // edx
    u32 rnd_1; // eax MAPDST
    u32 rnd_0; // eax MAPDST
    u32 rnd_0_mod; // et2 MAPDST
    s32 rate; // eax

    model_copy = pObj->field_18_model_copy;
    if (model_copy > 110)
    {
        switch (model_copy)
        {
            case 166:
            case 169:
            case 294:
                this->field_30_sQueueSample.field_14_samp_idx = 37;
                this->field_30_sQueueSample.field_18 = 0;
                this->field_30_sQueueSample.field_20_rate =
                    RandomDisplacement_41A650(37u) + gSampManager_6FFF00.GetPlayBackRateIdx_58DBF0(37);
                return 1;

            case 182:
            case 183:
                goto skip_switch_2;

            case 192:
            case 254:
            case 265:
                if (interactionType == 3)
                {
                    rnd_1 = this->field_1454_anRandomTable[1];
                    if ((pObj->field_10->field_78_flags & 0x100) != 0)
                    {
                        samp_idx = rnd_1 % 3 + 46;
                    }
                    else
                    {
                        samp_idx = rnd_1 % 3 + 43;
                    }
                }
                else
                {
                    if (interactionType == 7)
                    {
                        this->field_30_sQueueSample.field_14_samp_idx = 42;
                        goto exit_switch_1;
                    }
                    samp_idx = this->field_1454_anRandomTable[1] % 3u + 49;
                }
                this->field_30_sQueueSample.field_14_samp_idx = samp_idx;
                break;

            case 266:
                this->field_30_sQueueSample.field_14_samp_idx = 34;
                rate = gSampManager_6FFF00.GetPlayBackRateIdx_58DBF0(34);
                goto set_rate;

            case 281:
            case 282:
                goto set_samp_idx_rnd_rate;

            case 295:
                this->field_30_sQueueSample.field_14_samp_idx = 33;
                rate = gSampManager_6FFF00.GetPlayBackRateIdx_58DBF0(33);
            set_rate:
                this->field_30_sQueueSample.field_20_rate = rate;
                this->field_30_sQueueSample.field_18 = 1;
                return 1;

            default:
                return 0;
        }
        goto exit_switch_1;
    }

    if (model_copy != 110)
    {
        switch (model_copy)
        {
            case 1:
            case 18:
                rnd_0 = this->field_1454_anRandomTable[0];
                this->field_30_sQueueSample.field_14_samp_idx = 55;
                rnd_0_mod = rnd_0 % 2000;
                this->field_30_sQueueSample.field_18 = 0;
                result = 1;
                this->field_30_sQueueSample.field_20_rate = rnd_0_mod + 23000;
                return result;
            case 2:
            case 15:
            case 19:
            case 20:
            case 60:
                goto set_samp_idx_rnd_rate;
            case 3:
            case 5:
            case 11:
            case 13:
            case 17:
            case 53:
            case 54:
            case 55:
                this->field_30_sQueueSample.field_14_samp_idx = 56;
                goto LABEL_10;
            case 4:
            case 23:
            case 44:
                this->field_30_sQueueSample.field_14_samp_idx = 54;
            LABEL_10:
                rnd_0 = this->field_1454_anRandomTable[0];
                this->field_30_sQueueSample.field_18 = 0;
                rnd_0_mod = rnd_0 % 2000;
                result = 1;
                this->field_30_sQueueSample.field_20_rate = rnd_0_mod + 17000;
                break;
            case 6:
                rnd_0 = this->field_1454_anRandomTable[0];
                this->field_30_sQueueSample.field_14_samp_idx = 52;
                rnd_0_mod = rnd_0 % 600;
                this->field_30_sQueueSample.field_18 = 0;
                result = 1;
                this->field_30_sQueueSample.field_20_rate = rnd_0_mod + 16000;
                break;
            case 7:
                this->field_30_sQueueSample.field_14_samp_idx = 55;
                goto exit_switch_1;
            case 12:
            case 14:
            case 58:
                rnd_0 = this->field_1454_anRandomTable[0];
                this->field_30_sQueueSample.field_14_samp_idx = 53;
                rnd_0_mod = rnd_0 % 2000;
                this->field_30_sQueueSample.field_18 = 0;
                result = 1;
                this->field_30_sQueueSample.field_20_rate = rnd_0_mod + 18000;
                break;
            case 16:
            case 25:
            case 62:
                rnd_0 = this->field_1454_anRandomTable[0];
                this->field_30_sQueueSample.field_14_samp_idx = 55;
                rnd_0_mod = rnd_0 % 2000;
                this->field_30_sQueueSample.field_18 = 0;
                result = 1;
                this->field_30_sQueueSample.field_20_rate = rnd_0_mod + 15000;
                break;
            case 21:
            case 22:
            case 46:
            case 48:
                rnd_0 = this->field_1454_anRandomTable[0];
                this->field_30_sQueueSample.field_14_samp_idx = 53;
                rnd_0_mod = rnd_0 % 1000;
                this->field_30_sQueueSample.field_18 = 0;
                result = 1;
                this->field_30_sQueueSample.field_20_rate = rnd_0_mod + 15500;
                break;
            default:
                return 0;
        }
        return result;
    }

    // model is 110 here, or via goto

skip_switch_2:
    if (interactionType != 1)
    {
    set_samp_idx_rnd_rate:
        this->field_30_sQueueSample.field_14_samp_idx = 37;
    exit_switch_1:
        rnd_0 = this->field_1454_anRandomTable[0];
        this->field_30_sQueueSample.field_18 = 0;
        rnd_0_mod = rnd_0 % 2000;
        result = 1;
        this->field_30_sQueueSample.field_20_rate = rnd_0_mod + 20000;
        return result;
    }

    switch (pObj->field_20_map_block_spec)
    {
        case 1:
        case 3:
            this->field_30_sQueueSample.field_14_samp_idx = 198;
            break;
        case 2:
        case 7:
        case 10:
            this->field_30_sQueueSample.field_14_samp_idx = 202;
            break;
        case 4:
            this->field_30_sQueueSample.field_14_samp_idx = 68;
            break;
        case 5:
        case 6:
        case 8:
        case 9:
            this->field_30_sQueueSample.field_14_samp_idx = 194;
            break;
        default:
            return 0;
    }

    if (pObj->field_20_map_block_spec == 4)
    {
        rnd_1 = this->field_1454_anRandomTable[1];
        this->field_30_sQueueSample.field_18 = 0;
        rnd1_mod = rnd_1 % 4000;
        result = 1;
        this->field_30_sQueueSample.field_20_rate = rnd1_mod + 28000;
    }
    else
    {
        rate__ = gSampManager_6FFF00.GetPlayBackRateIdx_58DBF0(field_30_sQueueSample.field_14_samp_idx);
        rate_plus_displacement = RandomDisplacement_41A650(field_30_sQueueSample.field_14_samp_idx) + rate__;
        rnd_samp_idx = (this->field_1454_anRandomTable[3] & 3) + field_30_sQueueSample.field_14_samp_idx;
        this->field_30_sQueueSample.field_20_rate = rate_plus_displacement;
        this->field_30_sQueueSample.field_14_samp_idx = rnd_samp_idx;
        this->field_30_sQueueSample.field_18 = 0;
        return 1;
    }
    return result;
}

STUB_FUNC(0x414A50)
void sound_obj::Tank_414A50(Sound_Params_8* a2)
{
    NOT_IMPLEMENTED;
}

MATCH_FUNC(0x414D30)
void sound_obj::Tank_414D30(Sound_Params_8* a2)
{
    if (a2->field_0_pObj->field_8_car_bc_ptr->field_B8)
    {
        if (CalculateDistance_419020(Fix16(409600, 0)))
        {
            if (VolCalc_419070(0x32u, Fix16(81920, 0), a2->field_5_bHasSolidAbove))
            {
                this->field_30_sQueueSample.field_54 = Fix16(81920, 0);
                this->field_30_sQueueSample.field_60_nEmittingVolume = 50;
                this->field_30_sQueueSample.field_64_max_distance = 10;
                this->field_30_sQueueSample.field_58_type = 11;
                this->field_30_sQueueSample.field_4_SampleIndex = 4;
                this->field_30_sQueueSample.field_41 = 0;
                this->field_30_sQueueSample.field_18 = 0;
                if (a2->field_4_bDrivenByPlayer)
                {
                    this->field_30_sQueueSample.field_1C_ReleasingVolumeModificator = 4;
                }
                else
                {
                    this->field_30_sQueueSample.field_1C_ReleasingVolumeModificator = 6;
                }
                AddSampleToRequestedQueue_41A850();
            }
        }
    }
}

STUB_FUNC(0x415190)
void sound_obj::Tank_415190(Sound_Params_8* a2)
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x414710)
void sound_obj::TrainCab_414710(Sound_Params_8* a2)
{
    NOT_IMPLEMENTED;
}

WIP_FUNC(0x57E680)
void sound_obj::Type3_CopRadioReport_57E680()
{
    WIP_IMPLEMENTED;

    if (gSoundVocalsInited_6FF538)
    {
        for (u8 i = 0; i < 5; i++)
        {
            // Increment some counter on each station, probably wrong offset etc here
            u32 v2 = 90000 * this->field_8;
            if (*(u32*)&field_544C[i + 1].field_8.field_C_pAny < v2)
            {
                ++*(u32*)&field_544C[i + 1].field_8.field_C_pAny;
            }
        }
    }
}

MATCH_FUNC(0x4136D0)
char_type sound_obj::Type6_12_4136D0(Rozza_A* a2)
{
    s32 samp_idx;
    switch (a2->field_20_map_block_spec)
    {
        case 4:
            samp_idx = 68;
            break;

        case 1:
        case 3:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
            samp_idx = 39;
            break;
        case 2:
        case 0xA:
            samp_idx = 38;
            break;

        default:
            return 0;
    }
    this->field_30_sQueueSample.field_20_rate =
        RandomDisplacement_41A650(samp_idx) + gSampManager_6FFF00.GetPlayBackRateIdx_58DBF0(samp_idx);
    this->field_30_sQueueSample.field_14_samp_idx = samp_idx;

    this->field_30_sQueueSample.field_18 = 0;
    return 1;
}

MATCH_FUNC(0x412D40)
char_type sound_obj::Type6_2_412D40(u8 a2)
{
    s32 samp_idx;

    if (a2 < 15u)
    {
        return 0;
    }

    if (a2 < 40u)
    {
        samp_idx = 37;
    }
    else if (a2 < 90u)
    {
        samp_idx = 38;
    }
    else
    {
        samp_idx = 39;
    }

    this->field_30_sQueueSample.field_20_rate =
        RandomDisplacement_41A650(samp_idx) + gSampManager_6FFF00.GetPlayBackRateIdx_58DBF0(samp_idx);
    this->field_30_sQueueSample.field_14_samp_idx = samp_idx;
    this->field_30_sQueueSample.field_18 = 0;
    return 1;
}

MATCH_FUNC(0x413000)
char_type sound_obj::Type6_3_413000(Rozza_A* pObj)
{
    if ((pObj->field_18_model_copy < 200 || pObj->field_18_model_copy > 244) &&
        (pObj->field_18_model_copy < 64 || pObj->field_18_model_copy > 108))
    {
        return SelectObjectImpactSound_413120(pObj, 3);
    }
    Type6_Play_412D90(pObj->field_18_model_copy);
    return 1;
}

MATCH_FUNC(0x412C90)
char_type sound_obj::Type6_412C90(Rozza_A* pObj, u8 a3)
{
    char_type result;

    switch (pObj->field_0)
    {
        case 1u:
        case 10u:
            result = sound_obj::SelectObjectImpactSound_1_10_412D30(pObj);
            break;
        case 2u:
            result = sound_obj::Type6_2_412D40(a3);
            break;
        case 3u:
            result = sound_obj::Type6_3_413000(pObj);
            break;
        case 4u:
            result = sound_obj::Type6_4_413040(a3);
            break;
        case 5u:
            result = sound_obj::Type6_5_413090(a3);
            break;
        case 7u:
            result = sound_obj::Type6_7_4130E0(pObj);
            break;
        case 9u:
            result = sound_obj::Type6_9_413540(pObj);
            break;
        case 12u:
            result = sound_obj::Type6_12_4136D0(pObj);
            break;
        default:
            result = 0;
            break;
    }
    return result;
}

STUB_FUNC(0x413A10)
char_type sound_obj::Type6_413A10(Rozza_A* a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x413040)
char_type sound_obj::Type6_4_413040(u8 a2)
{
    if (a2 < 15u)
    {
        return 0;
    }

    s32 idx_to_use;
    if (a2 < 23)
    {
        idx_to_use = 40;
    }
    else
    {
        idx_to_use = 41;
    }

    this->field_30_sQueueSample.field_20_rate =
        RandomDisplacement_41A650(idx_to_use) + gSampManager_6FFF00.GetPlayBackRateIdx_58DBF0(idx_to_use);
    this->field_30_sQueueSample.field_14_samp_idx = idx_to_use;
    this->field_30_sQueueSample.field_18 = 0;
    return 1;
}

MATCH_FUNC(0x413090)
char_type sound_obj::Type6_5_413090(u8 a2)
{
    s32 idx_to_use;
    if (a2 < 40u)
    {
        idx_to_use = 37;
    }
    else if (a2 < 90u)
    {
        idx_to_use = 38;
    }
    else
    {
        idx_to_use = 39;
    }

    this->field_30_sQueueSample.field_20_rate =
        RandomDisplacement_41A650(idx_to_use) + gSampManager_6FFF00.GetPlayBackRateIdx_58DBF0(idx_to_use);
    this->field_30_sQueueSample.field_14_samp_idx = idx_to_use;

    this->field_30_sQueueSample.field_18 = 0;
    return 1;
}

MATCH_FUNC(0x4130E0)
char_type sound_obj::Type6_7_4130E0(Rozza_A* a2)
{
    if ((a2->field_18_model_copy < 200 || a2->field_18_model_copy > 244) && (a2->field_18_model_copy < 64 || a2->field_18_model_copy > 108))
    {
        return SelectObjectImpactSound_413120(a2, 7);
    }
    Type6_Play_412D90(a2->field_18_model_copy);
    return 1;
}

MATCH_FUNC(0x413540)
char_type sound_obj::Type6_9_413540(Rozza_A* a2)
{
    switch (a2->field_1C)
    {
        case 169:
        case 182:
        case 183:
        case 192:
        case 254:
        case 265:
        case 281:
        case 282:
        case 286:
        case 294:
        case 295:
            return SelectObjectImpactSound_413120(a2, 9);

        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
        case 16:
        case 17:
        case 18:
        case 19:
        case 20:
        case 21:
        case 22:
        case 23:
        case 25:
        case 43:
        case 44:
        case 45:
        case 46:
        case 47:
        case 48:
        case 49:
        case 53:
        case 54:
        case 55:
        case 56:
        case 57:
        case 58:
        case 59:
        case 60:
        case 61:
        case 62:
        case 63:
        case 110:
        case 123:
        case 155:
        case 156:
        case 157:
        case 158:
        case 166:
            return SelectObjectImpactSound_413120(a2, 9);

        default:
            return 0;
    }
}

MATCH_FUNC(0x412D90)
void sound_obj::Type6_Play_412D90(s32 model)
{
    s32 sample;
    switch (model)
    {
        case 64:
        case 200:
            sample = 34;
            break;
        case 65:
        case 201:
            sample = 27;
            break;
        case 66:
        case 202:
            sample = 35;
            break;
        case 67:
        case 203:
            sample = 29;
            break;
        case 68:
        case 204:
            sample = 32;
            break;
        case 69:
        case 205:
            sample = 31;
            break;
        case 70:
        case 206:
            sample = 36;
            break;
        case 72:
        case 208:
            sample = 30;
            break;
        case 73:
        case 209:
            sample = 37;
            break;
        case 74:
        case 210:
            sample = 28;
            break;
        case 79:
        case 215:
            sample = 39;
            break;
        case 80:
        case 216:
            sample = 33;
            break;
        case 81:
        case 217:
            sample = 41;
            break;
        case 82:
        case 218:
            sample = 40;
            break;
        case 87:
        case 223:
            sample = 38;
            break;
        case 92:
        case 228:
            sample = 53;
            break;
        case 93:
        case 229:
            sample = 52;
            break;
        case 94:
        case 230:
            sample = 48;
            break;
        case 95:
        case 231:
            sample = 42;
            break;
        case 96:
        case 232:
            sample = 47;
            break;
        case 97:
        case 233:
            sample = 43;
            break;
        case 98:
        case 234:
            sample = 51;
            break;
        case 99:
        case 235:
            sample = 44;
            break;
        case 100:
        case 236:
            sample = 46;
            break;
        case 101:
        case 237:
            sample = 45;
            break;
        case 102:
        case 238:
            sample = 54;
            break;
        case 103:
        case 239:
            sample = 50;
            break;
        case 104:
        case 240:
            sample = 49;
            break;
        default:
            return;
    }
    gSampManager_6FFF00.PlayVocal_58E510(1, sample, 1);
    gSampManager_6FFF00.SetVocalVolume_58E6D0(1, 127 * field_24_sfx_vol / 127);
}

STUB_FUNC(0x57E220)
void sound_obj::UpdateCarEngineAudio_57E220()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x418610)
void sound_obj::UpdateCarSurfaceAudio_418610(Sound_Params_8* a2)
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x41FCA0)
void sound_obj::UpdateRadioChatterLoop_41FCA0()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x57E510)
u32 sound_obj::UpdateVocalStream_57E510()
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x4145E0)
bool sound_obj::GetCar_4145E0(s32 idx, Car_BC** ppOut)
{
    if (field_147C[idx].field_0_bUsed)
    {
        Sprite* pSprite = field_147C[idx].field_4_pObj->field_C_pAny.pSprite;
        if (pSprite)
        {
            if (pSprite->field_10_sound)
            {
                Car_BC* pCar = pSprite->AsCar_40FEB0();
                *ppOut = pCar;
                if (pCar)
                {
                    return 1;
                }
            }
        }
    }
    return 0;
}
