#include "sound_obj.hpp"
#include "Camera.hpp"
#include "Car_BC.hpp"
#include "Frontend.hpp"
#include "Function.hpp"
#include "Game_0x40.hpp"
#include "Globals.hpp"
#include "Ped.hpp"
#include "cSampleManager.hpp"
#include "sprite.hpp"
#include <math.h>

DEFINE_GLOBAL(sound_obj, gSound_obj_66F680, 0x66F680);
DEFINE_GLOBAL(Fix16, dword_674CD8, 0x674CD8);
DEFINE_GLOBAL_INIT(Fix16, dword_66F3F0, Fix16(0), 0x66F3F0);
DEFINE_GLOBAL_INIT(Fix16, dword_674DA8, Fix16(0x100000, 0), 0x674DA8);
DEFINE_GLOBAL_ARRAY(u8, byte_61A688, 64, 0x61A688);

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

MATCH_FUNC(0x419DF0)
serene_brattain* serene_brattain::sub_419DF0()
{
    return this;
}

MATCH_FUNC(0x419CD0)
sound_obj::sound_obj()
{
    sound_0x68* pIter = &field_9C_asSamples[0][0];
    for (s32 t = 0; t < 2 * 16; t++)
    {
        pIter->field_8_obj.sub_419DF0();
        pIter++;
    }

    for (s32 j = 0; j < 16; j++)
    {
        field_DC0_ActiveSamples[j].field_8_obj.sub_419DF0();
    }

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
void sound_obj::SetSfxVol_41A250(char_type sfxVol)
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
u8 sound_obj::sub_41A4A0(Fix16 a1, Fix16 a2)
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
s32 sound_obj::sub_41A580(s32 snd_rate, Fix16 xpos, Fix16 ypos, Fix16 zpos)
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x427180)
void sound_obj::Init_15_Array_427180()
{
    for (s32 i = 0; i < 15; i++)
    {
        field_552C_15array[i] = 121;
    }
    field_5528_idx15_cur = 0;
    field_5529_idx15 = 0;
}

MATCH_FUNC(0x4271B0)
void sound_obj::Set15Val_4271B0(u32 val)
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

STUB_FUNC(0x427220)
void sound_obj::sub_427220()
{
    NOT_IMPLEMENTED;
    if (!(field_5448_m_FrameCounter % 10u) && field_5520 == 0 && !gSampManager_6FFF00.SampleNotDone_58E880())
    {
        gSampManager_6FFF00.EndSample_58E960();
        field_5520 = 1;
    }

    if (field_5528_idx15_cur != field_5529_idx15 && !gSampManager_6FFF00.SampleNotDone_58E880() && field_5520 == 1)
    {
        // todo: this load is missing without volatile!
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
void sound_obj::sub_41B540()
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
                sub_41B520(pIter->field_28_distance, &f28_conv);
                pIter->field_60_nEmittingVolume =
                    ComputeEmittingVolume_41B660(pIter->field_60_nEmittingVolume, pIter->field_64_max_distance, (u32)f28_conv);
            }
        }
    }
}

MATCH_FUNC(0x41B520)
void sound_obj::sub_41B520(Fix16 a1, f32* a2)
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
        sub_41B540();
        ProcessActiveQueues_41AB80();
        sub_41A6F0();
    }
}

// https://decomp.me/scratch/i3zcW
STUB_FUNC(0x41A730)
void sound_obj::InterrogateAudioEntities_41A730()
{
    NOT_IMPLEMENTED;
    Camera_0xBC* field_C_pAny;

    if (field_1478_type5Idx != 0 && (field_C_pAny = (Camera_0xBC*)field_147C[field_1478_type5Idx].field_4_pObj->field_C_pAny) != NULL)
    {
        Ped* v4 = field_C_pAny->field_34_ped;

        if (v4 != NULL)
        {
            Car_BC* field_16C_car = v4->field_16C_car;
            if (field_16C_car != NULL)
            {
                field_1468_v1 = field_16C_car->field_50_car_sprite->field_14_xpos.x;
                field_146C_v2 = field_16C_car->field_50_car_sprite->field_14_xpos.y;
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
            field_1468_v1 = field_C_pAny->field_98_cam_pos2.field_0_x;
            field_146C_v2 = field_C_pAny->field_98_cam_pos2.field_4_y;
            field_1470_v3 = field_C_pAny->field_98_cam_pos2.field_8_z;
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
            Sample.field_20_rate = sound_obj::sub_41A580(Sample.field_20_rate,
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
                sound_obj::sub_41B520(position.field_0, &x);
                sound_obj::sub_41B520(position.field_4, &y);
                sound_obj::sub_41B520(position.field_8, &z);
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
                    sound_obj::sub_41B520(position2.field_0, &xpos);
                    sound_obj::sub_41B520(position2.field_4, &ypos);
                    sound_obj::sub_41B520(position2.field_8, &zpos);
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
char_type sound_obj::CalcVolume_41A3F0(u8 a1, s32 a2, Fix16 a3)
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x419070)
bool sound_obj::VolCalc_419070(s32 a2, s32 a3, char_type a4)
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
char_type sound_obj::sub_419020(Fix16 a2)
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
char_type sound_obj::sub_4186D0(sound_0x68* pObj)
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
char_type sound_obj::sub_4182A0(sound_0x68* pObj)
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
char_type sound_obj::sub_418130(sound_0x68* pObj)
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
char_type sound_obj::sub_417F40(s32 a1)
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
s32 sound_obj::sub_417EF0(sound_0x68* pObj)
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
char_type sound_obj::sub_4153F0(sound_0x68* pObj)
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
        field_544C[0].field_8.field_C_pAny = 0;
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

struct brave_archimedes_0x3C
{
    s16 field_0;
    s16 field_2;
    naughty_elion_0x4C* field_4_pUnk;
    inspiring_cori_0xBC* field_8_cori_or_leaky;
    naughty_elion_0x4C* field_C;
    s32 field_10;
    s32 field_14;
    s32 field_18;
    s32 field_1C;
    s16 field_20;
    s16 field_22;
    s16 field_24;
    s16 field_26;
    s32 field_28;
    char_type field_2C;
    char_type field_2D;
    char_type field_2E;
    char_type field_2F;
    s32 field_30;
    s32 field_34;
    char_type field_38;
    char_type field_39;
    char_type field_3A;
    char_type field_3B;
};

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
            if (pTuring->field_0_object_type == 5) // DrawUnk_0xBC ?
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
                case 1: // brave_archimedes_0x3C ?
                {
                    brave_archimedes_0x3C* v7 = (brave_archimedes_0x3C*)field_147C[idx].field_4_pObj->field_C_pAny;
                    if (v7)
                    {
                        switch (v7->field_30)
                        {
                            case 2: // note: sub eax, 2 added via switch case instead of if
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

                case 2:
                {
                    sub_57EA10();
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
        case 1:
            if (pTuring->field_C_pAny)
            {
                brave_archimedes_0x3C* pAny = (brave_archimedes_0x3C*)pTuring->field_C_pAny;
                switch (pAny->field_30)
                {
                    case 2:
                        if (field_147C[idx].field_8_pAlloc)
                        {
                            delete field_147C[idx].field_8_pAlloc;
                        }
                        break;
                }
                break;
            }
            // fall through

        case 2:
            field_3 = 0;
            gSampManager_6FFF00.FadeOut_58E490();
            break;
    }

    field_147C[idx].field_0_bUsed = 0;
    field_147C[idx].field_4_pObj->field_C_pAny = 0;
    field_147C[idx].field_1 = 0;

    if (field_147C[idx].field_4_pObj->field_0_object_type == 5)
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
void sound_obj::sub_57EA10()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x57EA90)
void sound_obj::sub_57EA90()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x57EB90)
char_type sound_obj::sub_57EB90(s32 a2, s32 a3)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x57EE30)
void sound_obj::sub_57EE30(s32 a2, s32 a3)
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x57EEE0)
void sound_obj::sub_57EEE0(char_type a2)
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x57EF60)
void sound_obj::sub_57EF60()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x57F050)
u8 sound_obj::sub_57F050(s32 a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x57F090)
bool sound_obj::sub_57F090(Car_BC* pCar)
{
    NOT_IMPLEMENTED;
    return false;
}

STUB_FUNC(0x57F120)
bool sound_obj::sub_57F120(Car_BC* pCar)
{
    NOT_IMPLEMENTED;
    return false;
}

MATCH_FUNC(0x419EF0)
void sound_obj::Service_419EF0()
{
    static char_type byte_674E24;
    if (gGame_0x40_67E008)
    {
        field_1_isPaused = gGame_0x40_67E008->field_0_game_state == 2;
        if (!field_1_isPaused)
        {
            GenerateIntegerRandomNumberTable_41BA90();
        }
        else
        {
            if (!byte_674E24)
            {
                gSampManager_6FFF00.CloseVocalStream_58E6A0(1);
                gSampManager_6FFF00.PlayVocal_58E510(1, 7, 1);
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
            if (field_147C[id].field_4_pObj->field_0_object_type == 3)
            {
                ProcessType3_57DD50();
            }

            if (!field_1_isPaused)
            {
                switch (field_147C[id].field_4_pObj->field_0_object_type)
                {
                    case 1:
                        ProcessType1_412740(id);
                        break;
                    case 6:
                        ProcessType6_413760(id);
                        break;
                    case 7:
                        ProcessType7_42A500(id);
                        break;
                    case 8:
                        ProcessType8_412820(id);
                        break;
                    case 9:
                        ProcessType9_412A60(id);
                        break;
                    case 10:
                        ProcessType10_418CA0();
                        break;
                    case 11:
                        ProcessType11_418B60(id);
                        break;
                    default:
                        return;
                }
            }
        }

        if (field_147C[id].field_4_pObj->field_0_object_type == 2)
        {
            ProcessType2_412490(id);
        }
    }
}

STUB_FUNC(0x57DD50)
void sound_obj::ProcessType3_57DD50()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x412740)
void sound_obj::ProcessType1_412740(s32 a2)
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x413760)
void sound_obj::ProcessType6_413760(s32 a2)
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x4273B0)
void sound_obj::sub_4273B0()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x42A500)
void sound_obj::ProcessType7_42A500(s32 a2)
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x412820)
void sound_obj::ProcessType8_412820(s32 a2)
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x412A60)
void sound_obj::ProcessType9_412A60(s32 a2)
{
    NOT_IMPLEMENTED;
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
void sound_obj::ProcessType10_418CA0()
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
void sound_obj::ProcessType11_418B60(s32 a2)
{
    infallible_turing* field_C_pObject = (infallible_turing*)field_147C[a2].field_4_pObj->field_C_pAny;
    if (field_C_pObject)
    {
        if (field_C_pObject->field_0_object_type > 0)
        {
            u8 v4;
            if (field_C_pObject->field_0_object_type > 300)
            {
                field_30_sQueueSample.field_20_rate = 22050;
                v4 = 40;
            }
            else if (field_C_pObject->field_0_object_type > 150)
            {
                field_30_sQueueSample.field_20_rate = 26221;
                v4 = 55;
            }
            else
            {
                field_30_sQueueSample.field_20_rate = 33178;
                v4 = 70;
            }

            field_30_sQueueSample.field_14_samp_idx = 57;
            field_30_sQueueSample.field_0_EntityIndex = a2;
            field_30_sQueueSample.field_5C = 0;
            field_30_sQueueSample.field_24_nVolume = v4;
            field_30_sQueueSample.field_60_nEmittingVolume = v4;
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

    infallible_turing* field_C_pObject = (infallible_turing*)field_147C[idx].field_4_pObj->field_C_pAny;
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
    field_30_sQueueSample.field_8_obj.field_0 = dword_66F3F0;
    field_30_sQueueSample.field_8_obj.field_4 = dword_66F3F0;
    field_30_sQueueSample.field_8_obj.field_8 = dword_66F3F0;
    field_30_sQueueSample.field_1C_ReleasingVolumeModificator = 0;
    field_30_sQueueSample.field_24_nVolume = 127;
    field_30_sQueueSample.field_28_distance = dword_66F3F0;
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
            case 1:
            case 6:
                return sub_416260(pObj);
            case 2:
                return sub_4182A0(pObj);
            case 3:
                return sub_4174C0(pObj);
            case 4:
                return sub_417A00(pObj);
            case 5:
                return sub_415730(pObj);
            case 7:
                return sub_417EF0(pObj);
            case 8:
                return sub_418130(pObj);
            case 9:
                return sub_4186D0(pObj);
            case 10:
                return sub_418940(pObj);
            case 11:
                return sub_414EE0(pObj);
            case 12:
                return sub_414C90(pObj);
            case 13:
                return sub_4153F0(pObj);
            case 15:
                return sub_415100(pObj);
            case 16:
                return sub_414320(pObj);
            case 17:
                return sub_414690(pObj);
            case 19:
                return sub_4149D0(pObj);
            case 20:
                return 1;
            default:
                return 0;
        }
    }
    return gFrontend_67DC84 && pObj->field_58_type == 20 ? true : false;
}

STUB_FUNC(0x416260)
char_type sound_obj::sub_416260(sound_0x68* a1)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x4174C0)
char_type sound_obj::sub_4174C0(sound_0x68* a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x417A00)
char_type sound_obj::sub_417A00(sound_0x68* a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x415730)
char_type sound_obj::sub_415730(sound_0x68* a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x418940)
char_type sound_obj::sub_418940(sound_0x68* a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x414EE0)
char_type sound_obj::sub_414EE0(sound_0x68* a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x414C90)
char_type sound_obj::sub_414C90(sound_0x68* a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x415100)
char_type sound_obj::sub_415100(sound_0x68* a1)
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
char_type sound_obj::sub_414320(sound_0x68* pObj)
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
char_type sound_obj::sub_414690(sound_0x68* pObj)
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

STUB_FUNC(0x4149D0)
char_type sound_obj::sub_4149D0(sound_0x68* a2)
{
    NOT_IMPLEMENTED;
    // todo
    return 0;
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
Fix16* sound_obj::sub_4190B0(Fix16* a2)
{
    Fix16 v2 = field_30_sQueueSample.field_8_obj.field_0 - field_1468_v1;
    Fix16 v3 = field_30_sQueueSample.field_8_obj.field_4 - field_146C_v2;
    *a2 = v3 * v3 + v2 * v2;
    return a2;
}

MATCH_FUNC(0x427310)
BYTE sound_obj::sub_427310()
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

MATCH_FUNC(0x427340)
void sound_obj::sub_427340(s32 a4, s32 a5, s32 a6)
{
    if (sub_427310() >= 8u)
    {
        Set15Val_4271B0(120u);
        if ((field_5448_m_FrameCounter & 1) == 0)
        {
            Set15Val_4271B0(70u);
        }
        else
        {
            Set15Val_4271B0(69u);
        }
        Set15Val_4271B0(73u);
        Set15Val_4271B0(a4 + 73);
        Set15Val_4271B0(88u);
        sub_426E10(a5, a6);
    }
}

STUB_FUNC(0x426E10)
void sound_obj::sub_426E10(s32 arg0, u32 a2)
{
    NOT_IMPLEMENTED;
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
