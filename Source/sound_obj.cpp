#include "sound_obj.hpp"
#include "explosion_type.hpp"
// Keep cSampleManager.hpp early: the include order changes the order VC6 emits the
// sampManager and sound_obj calls in e.g. Type_9_InitCarEngineStartSound_4186D0 and SelectCarCarCollisionSound_412D40
#include "PlayerCrimeStats.hpp"
#include "cSampleManager.hpp"
#include "Camera.hpp"
#include "CarInfoTable.hpp"
#include "CarPhysics.hpp"
#include "Car.hpp"
#include "Cranes.hpp"
#include "Crushers.hpp"
#include "Frontend.hpp"
#include "Function.hpp"
#include "Game.hpp"
#include "Globals.hpp"
#include "Hud.hpp"
#include "Object_5C.hpp"
#include "Explosion.hpp"
#include "Ped.hpp"
#include "Player.hpp"
#include "PublicTransport.hpp"
#include "CollisionSoundQueue.hpp"
#include "collision_event_type.hpp"
#include "Weapon.hpp"
#include "map_0x370.hpp"
#include "sprite.hpp"
#include "CrimeReportQueue.hpp"
#include "voice_line.hpp"
#include "car_shop_event.hpp"
#include <math.h>

// Forward declarations: the functions below are in address order
EXTERN_GLOBAL(u16, gObjSoundDelayKind17_6751D0);
EXTERN_GLOBAL(u16, gObjSoundDelayKind7_6751D2);
EXTERN_GLOBAL(u16, gObjSoundDelayKind28_6751D4);
EXTERN_GLOBAL(u16, gObjSoundDelayKind11_6751D6);
EXTERN_GLOBAL(u16, gObjSoundDelayKind12_6751D8);
EXTERN_GLOBAL(u16, gObjSoundDelayKind20_6751DA);
EXTERN_GLOBAL(u16, gObjSoundDelayKind24_6751DC);
EXTERN_GLOBAL(u16, gObjSoundDelayKind52_6751E2);
EXTERN_GLOBAL(u16, gObjSoundDelayKind46_6751DE);
EXTERN_GLOBAL(u16, gObjSoundDelayKind21_6751E0);
EXTERN_GLOBAL(u8, gType12SampleIndexCounter_6751E4);
EXTERN_GLOBAL(s32, gExplosionSound_Unused_61A6CC);
EXTERN_GLOBAL(s32, gExplosionSound_Unused_61A6D0);
EXTERN_GLOBAL(u8, gCarRadioStaticVolume_6FF540);
EXTERN_GLOBAL(u16, gCarRadioStaticTimer_6FF542);
EXTERN_GLOBAL(s32, gCarRadioStaticRate_625010);
EXTERN_GLOBAL(s32, gCarRadioTuneRate_625014);

DEFINE_GLOBAL(sound_obj, gSound_obj_66F680, 0x66F680);
DEFINE_GLOBAL(Fix16, kFpZero_674CD8, 0x674CD8);
DEFINE_GLOBAL_INIT(Fix16, kFpZero_66F3F0, Fix16(0), 0x66F3F0);
DEFINE_GLOBAL_INIT(Ang16, kMinCorneringAngle_66F274, Ang16(360), 0x66F274);
DEFINE_GLOBAL_INIT(Ang16, kMaxCorneringAngle_66F370, Ang16(1080), 0x66F370);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kMinCorneringSpeed, Fix16(0x40, 0), 0x66F378);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFp64, Fix16(64), 0x674DA8);
DEFINE_GLOBAL(u8, gSoundSwitchRadioCoolDown_6FF539, 0x6FF539);
DEFINE_GLOBAL_INIT(Fix16, kFpOne_6FF3F4, Fix16(1), 0x6FF3F4);
DEFINE_GLOBAL(bool, gSoundVocalsInited_6FF538, 0x6FF538);
DEFINE_GLOBAL_INIT(Fix16, kFpOne_66F3F4, Fix16(1), 0x66F3F4);

DEFINE_GLOBAL_INIT(Fix16, kFpFive_675414, Fix16(5), 0x675414);
DEFINE_GLOBAL_INIT(Fix16, kFpZero_675400, Fix16(0), 0x675400);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpZero, kFpZero_675400, 0x675220);
DEFINE_GLOBAL_INIT(Fix16, kFpSix_675418, Fix16(6), 0x675418);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFp400, Fix16(400), 0x6751F4);

DEFINE_GLOBAL_INIT(Fix16, kFpPoint01_66F3B4, Fix16(0xA3, 0), 0x66F3B4);
DEFINE_GLOBAL_INIT(Fix16, kFpTwo_66F3F8, Fix16(2), 0x66F3F8);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpPoint02, Fix16(0x147, 0), 0x66F3C0);
DEFINE_GLOBAL_INIT(Fix16, kFpPoint2_66F24C, Fix16(0xCCC, 0), 0x66F24C);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpPoint004, Fix16(0x41, 0), 0x66F490);
DEFINE_GLOBAL_INIT(Fix16, kFpHalf_66F2FC, Fix16(0x2000, 0), 0x66F2FC);
DEFINE_GLOBAL_INIT(Fix16, kFpThree_66F3FC, Fix16(3), 0x66F3FC);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFp127, Fix16(127), 0x66F1CC);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFp100, Fix16(100), 0x674E18);

DEFINE_GLOBAL_INIT(Fix16, kFpFive_66F404, Fix16(5), 0x66F404);
DEFINE_GLOBAL_INIT(Fix16, kFpFive_66F1D0, kFpFive_66F404, 0x66F1D0);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpFive, kFpFive_66F1D0, 0x66F220);
DEFINE_GLOBAL_INIT(Fix16, kFpHalf_66F258, Fix16(0x2000, 0), 0x66F258);

const char_type kHornPatterns_5FE434[8][44] = {
    {0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0},
    {0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0},
    {0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
    {0, 0, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
    {0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0},
    {0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
};

static inline s32 Min(s32 a, s32 b)
{
    return a < b ? a : b;
}

static inline u8 MinU8(u8 a, u8 b)
{
    return a < b ? a : b;
}

static inline s32 Max(s32 a, s32 b)
{
    return a > b ? a : b;
}

static inline s32 Clamp2(s32 v, s32 center, s32 radius)
{
    return v > center ? Min(center + radius, v) : Max(center - radius, v);
}

MATCH_FUNC(0x412240)
void sound_obj::PreSoundInit_412240()
{
}

MATCH_FUNC(0x412250)
void sound_obj::PreSoundShutdown_412250()
{
}

MATCH_FUNC(0x412260)
bool sound_obj::InitSampleForType_412260(sound_0x68* pObj)
{
    if (gGame_0x40_67E008 && field_1478_type5Idx && !field_1_isPaused)
    {
        switch (pObj->field_58_type)
        {
            case 2:
                return Type_2_InitAICarEngineSound_4182A0(pObj);
            case 1:
            case 6:
                return Type_1_6_InitCarEngineOrDamageSound_416260(pObj);
            case 3:
                return Type_3_InitCarAlarmSound_4174C0(pObj);
            case 4:
                return Type_4_InitSirenSound_417A00(pObj);
            case 5:
                return Type_5_InitHornSound_415730(pObj);
            case 16:
                return Type_16_InitTrainEngineSound_414320(pObj);
            case 17:
                return Type_17_InitTrainRollingFrictionSound_414690(pObj);
            case 19:
                return Type_19_InitTrainStationSound_4149D0(pObj);
            case 8:
                return Type_8_InitTruckCorneringSound_418130(pObj);
            case 9:
                return Type_9_InitCarEngineStartSound_4186D0(pObj);
            case 10:
                return Type_10_HandleCarSkidSound_418940(pObj);
            case 11:
                return Type_11_InitTankTurretSound_414EE0(pObj);
            case 12:
                return Type_12_InitTankMovementSound_414C90(pObj);
            case 13:
                return Type_13_InitTankCrushSound_4153F0(pObj);
            case 7:
                return Type_7_InitHeavyVehicleStopSound_417EF0(pObj);
            case 15:
                return Type_15_InitCarBombSound_415100(pObj);
            case 20:
                return true;
            default:
                return false;
        }
    }
    return gFrontend_67DC84 && pObj->field_58_type == 20 ? true : false;
}

MATCH_FUNC(0x4123A0)
void sound_obj::ProcessEntity_4123A0(s32 idx)
{
    if (!field_147C_audio_entities[idx].field_4_pObj->field_4_bStatus)
    {
        if (gGame_0x40_67E008 && field_1478_type5Idx)
        {
            if (field_147C_audio_entities[idx].field_4_pObj->field_0_object_type == SoundObjectTypeEnum::Radio_3)
            {
                ProcessType3_CopRadioAndMusic_57DD50();
            }

            if (!field_1_isPaused)
            {
                switch (field_147C_audio_entities[idx].field_4_pObj->field_0_object_type)
                {
                    case SoundObjectTypeEnum::Sprite_1:
                        ProcessType1_Sprite_412740(idx);
                        break;
                    case SoundObjectTypeEnum::CollisionSoundQueue_6:
                        ProcessType6_CollisionSoundQueue_413760(idx);
                        break;
                    case SoundObjectTypeEnum::Weapon_30_7:
                        ProcessType7_Weapon_42A500(idx);
                        break;
                    case SoundObjectTypeEnum::Crane_15C_8:
                        ProcessType8_Crane_412820(idx);
                        break;
                    case SoundObjectTypeEnum::Crusher_30_9:
                        ProcessType9_Crusher_412A60(idx);
                        break;
                    case SoundObjectTypeEnum::Vocals_10:
                        ProcessType10_Vocals_418CA0();
                        break;
                    case SoundObjectTypeEnum::Hud_Pager_C_11:
                        ProcessType11_HudPager_418B60(idx);
                        break;
                    default:
                        return;
                }
            }
        }

        if (field_147C_audio_entities[idx].field_4_pObj->field_0_object_type == SoundObjectTypeEnum::Frontend_2)
        {
            ProcessType2_Frontend_412490(idx);
        }
    }
}

MATCH_FUNC(0x412490)
void sound_obj::ProcessType2_Frontend_412490(s32 idx)
{
    static u8 gType2SampleIndexCounter_66F2D4;
    static u8 gType2StreamStarted_66F540;

    SoundObject* pLinkedObj = field_147C_audio_entities[idx].field_4_pObj->field_C_pAny.pNextFree;
    if (!pLinkedObj)
    {
        return;
    }

    if (pLinkedObj->field_4_bStatus)
    {
        field_3_bFirstMenuStream = 0;
        if (gSampManager_6FFF00.StreamStatus_58E2C0() || !gType2StreamStarted_66F540)
        {
            gSampManager_6FFF00.CloseStream_58E460();
            gSampManager_6FFF00.OpenStream_58E320(2u);
            gSampManager_6FFF00.StreamSetVolume_58E2F0((6 * field_25_cdVol) / 10);
            gType2StreamStarted_66F540 = 1;
        }
    }
    else
    {
        if (field_3_bFirstMenuStream)
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
    switch (pLinkedObj->field_0_object_type)
    {
        case 1:
            sampIdx1 = 0;
            sampIdx2 = 1;
            break;
        case 2:
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
    field_30_sQueueSample.field_5C_pReleaseSource = NULL;
    field_30_sQueueSample.field_18_bIs2D = true;
    field_30_sQueueSample.field_8_obj.field_0_x = kFpZero_66F3F0;
    field_30_sQueueSample.field_8_obj.field_4_y = kFpZero_66F3F0;
    field_30_sQueueSample.field_8_obj.field_8_z = kFpZero_66F3F0;
    field_30_sQueueSample.field_1C_ReleasingVolumeModificator = 0;
    field_30_sQueueSample.field_24_nVolume = 127;
    field_30_sQueueSample.field_28_distance = kFpZero_66F3F0;
    field_30_sQueueSample.field_30_loop_count = 1;
    field_30_sQueueSample.field_34_loop_start = 0;
    field_30_sQueueSample.field_38_loop_end = -1;
    field_30_sQueueSample.field_3C_speed_multiplier = 0;
    field_30_sQueueSample.field_41_releasing_sound_flag = 1;
    field_30_sQueueSample.field_54_sound_intensity = Fix16(5);
    field_30_sQueueSample.field_58_type = 20;
    field_30_sQueueSample.field_60_nEmittingVolume = 127;
    field_30_sQueueSample.field_64_max_distance = 10;
    field_30_sQueueSample.field_14_samp_idx = sampIdx1;
    field_30_sQueueSample.field_40_pan = 0;
    field_30_sQueueSample.field_20_rate = gSampManager_6FFF00.GetPlayBackRate_58DBF0(sampIdx1);
    field_30_sQueueSample.field_4_SampleIndex = gType2SampleIndexCounter_66F2D4++;
    if (gType2SampleIndexCounter_66F2D4 >= 0xFF)
    {
        gType2SampleIndexCounter_66F2D4 = 0;
    }
    AddSampleToRequestedQueue_41A850();

    // Add second sample
    field_30_sQueueSample.field_14_samp_idx = sampIdx2;
    field_30_sQueueSample.field_40_pan = 127;
    field_30_sQueueSample.field_20_rate = gSampManager_6FFF00.GetPlayBackRate_58DBF0(sampIdx2);
    field_30_sQueueSample.field_4_SampleIndex = gType2SampleIndexCounter_66F2D4++;
    if (gType2SampleIndexCounter_66F2D4 >= 0xFF)
    {
        gType2SampleIndexCounter_66F2D4 = 0;
    }
    AddSampleToRequestedQueue_41A850();
}

MATCH_FUNC(0x412740)
void sound_obj::ProcessType1_Sprite_412740(s32 idx)
{
    Sound_Params entity;

    entity.field_0_pObj = this->field_147C_audio_entities[idx].field_4_pObj->field_C_pAny.pSprite;
    if (entity.field_0_pObj)
    {

        entity.field_0_pObj->GetPosition_4117B0(&field_30_sQueueSample.field_8_obj.field_0_x,
                                           &field_30_sQueueSample.field_8_obj.field_4_y,
                                           &field_30_sQueueSample.field_8_obj.field_8_z);

        this->field_30_sQueueSample.field_0_EntityIndex = idx;
        this->field_30_sQueueSample.field_5C_pReleaseSource = NULL;

        entity.field_5_bHasSolidAbove = gMap_0x370_6F6268->CheckColumnHasSolidAbove_4E7FC0(field_30_sQueueSample.field_8_obj.field_0_x,
                                                                                           field_30_sQueueSample.field_8_obj.field_4_y,
                                                                                           field_30_sQueueSample.field_8_obj.field_8_z);
        this->field_28_dist_squared = ComputeEmitterDistanceSquared_4190B0();
        this->field_2C_distCalculated = false;

        if (entity.field_0_pObj->AsCharB4_40FEA0())
        {
            sound_obj::ProcessPed_422B70(&entity);
        }
        else if (entity.field_0_pObj->AsCar_40FEB0())
        {
            sound_obj::ProcessCar_412B80(&entity);
        }
        else if (entity.field_0_pObj->AsObject2C_40FEC0())
        {
            sound_obj::ProcessObject_41E820(&entity);
        }
    }
}

MATCH_FUNC(0x412820)
void sound_obj::ProcessType8_Crane_412820(s32 idx)
{
    Crane* pCrane = field_147C_audio_entities[idx].field_4_pObj->field_C_pAny.pCrane_15C;
    if (pCrane)
    {
        Sprite* pSprite = pCrane->GetRotorSprite_411A00();
        if (pSprite)
        {
            pSprite->GetPosition_4117B0(&field_30_sQueueSample.field_8_obj.field_0_x,
                                   &field_30_sQueueSample.field_8_obj.field_4_y,
                                   &field_30_sQueueSample.field_8_obj.field_8_z);

            this->field_30_sQueueSample.field_0_EntityIndex = idx;
            this->field_30_sQueueSample.field_5C_pReleaseSource = NULL;
            bool bSolidAbove = gMap_0x370_6F6268->CheckColumnHasSolidAbove_4E7FC0(field_30_sQueueSample.field_8_obj.field_0_x,
                                                                                    field_30_sQueueSample.field_8_obj.field_4_y,
                                                                                    field_30_sQueueSample.field_8_obj.field_8_z);
            this->field_28_dist_squared = ComputeEmitterDistanceSquared_4190B0();
            this->field_2C_distCalculated = false;
            if (CalculateDistance_419020(Fix16(625)))
            {
                s32 samp_idx;
                s32 rate;
                u8 emitter_vol;
                s32 vol_mod;
                bool bLoop;
                for (u8 i = 0; i < 4; i++)
                {
                    switch (i)
                    {
                        case 0:
                            if (!pCrane->field_156_is_rotating)
                            {
                                continue;
                            }
                            samp_idx = 60;
                            rate = 9000;
                            emitter_vol = 60;
                            vol_mod = 3;
                            bLoop = false;
                            break;
                        case 1:
                            if (!pCrane->field_157_is_radius_changing)
                            {
                                continue;
                            }
                            samp_idx = 60;
                            rate = 13000;
                            emitter_vol = 30;
                            vol_mod = 4;
                            bLoop = false;
                            break;
                        case 2:
                            if (!pCrane->field_158_is_hook_depth_changing)
                            {
                                continue;
                            }
                            samp_idx = 60;
                            rate = 8000;
                            emitter_vol = 45;
                            vol_mod = 4;
                            bLoop = false;
                            break;
                        case 3:
                            if (!pCrane->field_159_hooked_car_this_frame)
                            {
                                continue;
                            }
                            samp_idx = 37;
                            rate = 22050;
                            emitter_vol = 50;
                            vol_mod = 5;
                            bLoop = true;
                            break;
                        default:
                            continue;
                    }

                    if (i < 5u && VolCalc_419070(emitter_vol, Fix16(25), bSolidAbove))
                    {
                        this->field_30_sQueueSample.field_14_samp_idx = samp_idx;
                        this->field_30_sQueueSample.field_4_SampleIndex = i;
                        this->field_30_sQueueSample.field_41_releasing_sound_flag = bLoop;
                        this->field_30_sQueueSample.field_20_rate = rate;
                        if (bLoop)
                        {
                            this->field_30_sQueueSample.field_30_loop_count = 1;
                        }
                        else
                        {
                            this->field_30_sQueueSample.field_30_loop_count = 0;
                            this->field_30_sQueueSample.field_4C_releasing_volume_divider = 3;
                        }
                        this->field_30_sQueueSample.field_34_loop_start = gSampManager_6FFF00.GetLoopStart_58DC30(samp_idx);
                        this->field_30_sQueueSample.field_38_loop_end = gSampManager_6FFF00.GetLoopEnd_58DC50(samp_idx);
                        this->field_30_sQueueSample.field_1C_ReleasingVolumeModificator = vol_mod;
                        this->field_30_sQueueSample.field_54_sound_intensity = Fix16(25);
                        this->field_30_sQueueSample.field_60_nEmittingVolume = emitter_vol;
                        this->field_30_sQueueSample.field_64_max_distance = 50;
                        this->field_30_sQueueSample.field_58_type = 20;
                        this->field_30_sQueueSample.field_3C_speed_multiplier = 400;
                        this->field_30_sQueueSample.field_18_bIs2D = false;
                        AddSampleToRequestedQueue_41A850();
                    }
                }
            }
        }
    }
}

MATCH_FUNC(0x412A60)
void sound_obj::ProcessType9_Crusher_412A60(s32 idx)
{
    Crusher* pCrusher = field_147C_audio_entities[idx].field_4_pObj->field_C_pAny.pCrusher_30;
    if (pCrusher)
    {
        if (pCrusher->get_state_411A10())
        {
            Fix16 x = pCrusher->get_xpos_411A20();
            this->field_30_sQueueSample.field_8_obj.field_0_x = x;

            Fix16 y = pCrusher->get_ypos_411A30();
            this->field_30_sQueueSample.field_8_obj.field_4_y = y;

            Fix16 z = this->field_1470_listener_z;
            this->field_30_sQueueSample.field_8_obj.field_8_z = z;

            this->field_30_sQueueSample.field_0_EntityIndex = idx;
            this->field_30_sQueueSample.field_5C_pReleaseSource = NULL;
            const bool bSolidAbove = gMap_0x370_6F6268->CheckColumnHasSolidAbove_4E7FC0(x, y, z);
            this->field_28_dist_squared = ComputeEmitterDistanceSquared_4190B0();
            this->field_2C_distCalculated = false;
            if (CalculateDistance_419020(Fix16(121)))
            {
                if (VolCalc_419070(0x6Eu, Fix16(11), bSolidAbove))
                {
                    this->field_30_sQueueSample.field_14_samp_idx = 60;
                    this->field_30_sQueueSample.field_4_SampleIndex = 0;
                    this->field_30_sQueueSample.field_41_releasing_sound_flag = 0;
                    this->field_30_sQueueSample.field_20_rate = 17000;
                    this->field_30_sQueueSample.field_30_loop_count = 0;
                    this->field_30_sQueueSample.field_4C_releasing_volume_divider = 5;
                    this->field_30_sQueueSample.field_34_loop_start = gSampManager_6FFF00.GetLoopStart_58DC30(60);
                    this->field_30_sQueueSample.field_38_loop_end = gSampManager_6FFF00.GetLoopEnd_58DC50(60);
                    this->field_30_sQueueSample.field_1C_ReleasingVolumeModificator = 6;
                    this->field_30_sQueueSample.field_54_sound_intensity = Fix16(11);
                    this->field_30_sQueueSample.field_60_nEmittingVolume = 110;
                    this->field_30_sQueueSample.field_64_max_distance = 22;
                    this->field_30_sQueueSample.field_58_type = 20;
                    this->field_30_sQueueSample.field_3C_speed_multiplier = 400;
                    this->field_30_sQueueSample.field_18_bIs2D = false;
                    AddSampleToRequestedQueue_41A850();
                }
            }
        }
    }
}

MATCH_FUNC(0x412B80)
void sound_obj::ProcessCar_412B80(Sound_Params* pParams)
{
    Sprite* pSprite = pParams->field_0_pObj;
    Car* pCar = pParams->field_0_pObj->field_8_car_bc_ptr;
    if (pCar)
    {
        pParams->field_4_bDrivenByPlayer = pCar->IsDrivenByPlayer_4118D0();

        SoundPosEntry* pAlloc = field_147C_audio_entities[field_30_sQueueSample.field_0_EntityIndex].field_8_pAlloc;
        if (pAlloc)
        {
            CarPhysics* pPhysics = pSprite->field_8_car_bc_ptr->field_58_physics;
            if (pPhysics)
            {
                pPhysics->SetModelPhysicsGlobal_562EB0();
            }
            else
            {
                pAlloc->field_0_last_speed = kFpZero_66F3F0;
            }
            switch (pParams->field_0_pObj->field_8_car_bc_ptr->GetCarInfoIdx_411940())
            {
                case car_model_enum::boxcar:
                case car_model_enum::TRAIN:
                case car_model_enum::TRAINFB:
                    sound_obj::ProcessTrain_413BE0(pParams);
                    break;
                case car_model_enum::TRAINCAB:
                    sound_obj::ProcessTrainCab_413B90(pParams, pAlloc);
                    break;
                case car_model_enum::TANK:
                    sound_obj::ProcessTank_413BF0(pParams, pAlloc);
                    break;
                default:
                    sound_obj::ProcessOtherCarTypes_413C50(pParams, pAlloc);
                    break;
            }
        }
    }
}

MATCH_FUNC(0x412C90)
bool sound_obj::SelectCollisionSound_412C90(CollisionEvent* pEvent, u8 volume)
{
    bool result;

    switch (pEvent->field_0_type)
    {
        case collision_event_type::object_floor_1:
        case collision_event_type::object_wall_10:
            result = sound_obj::SelectObjectImpactSound_1_10_412D30(pEvent);
            break;
        case collision_event_type::car_car_2:
            result = sound_obj::SelectCarCarCollisionSound_412D40(volume);
            break;
        case collision_event_type::car_object_3:
            result = sound_obj::SelectCarObjectCollisionSound_413000(pEvent);
            break;
        case collision_event_type::car_ped_4:
            result = sound_obj::SelectCarPedCollisionSound_413040(volume);
            break;
        case collision_event_type::car_wall_5:
            result = sound_obj::SelectCarWallCollisionSound_413090(volume);
            break;
        case collision_event_type::ped_object_7:
            result = sound_obj::SelectPedObjectCollisionSound_4130E0(pEvent);
            break;
        case collision_event_type::object_object_9:
            result = sound_obj::SelectObjectObjectCollisionSound_413540(pEvent);
            break;
        case collision_event_type::car_floor_12:
            result = sound_obj::SelectCarFloorCollisionSound_4136D0(pEvent);
            break;
        default:
            result = false;
            break;
    }
    return result;
}

MATCH_FUNC(0x412D30)
bool sound_obj::SelectObjectImpactSound_1_10_412D30(CollisionEvent* pEvent)
{
    return SelectObjectImpactSound_413120(pEvent, 1);
}

MATCH_FUNC(0x412D40)
bool sound_obj::SelectCarCarCollisionSound_412D40(u8 volume)
{
    s32 samp_idx;

    if (volume < 15u)
    {
        return false;
    }

    if (volume < 40u)
    {
        samp_idx = 37;
    }
    else if (volume < 90u)
    {
        samp_idx = 38;
    }
    else
    {
        samp_idx = 39;
    }

    this->field_30_sQueueSample.field_20_rate =
        RandomDisplacement_41A650(samp_idx) + gSampManager_6FFF00.GetPlayBackRate_58DBF0(samp_idx);
    this->field_30_sQueueSample.field_14_samp_idx = samp_idx;
    this->field_30_sQueueSample.field_18_bIs2D = false;
    return true;
}

MATCH_FUNC(0x412D90)
void sound_obj::PlayPickupVoice_412D90(s32 model)
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
    gSampManager_6FFF00.PlayVocal_58E510(1, sample, true);
    gSampManager_6FFF00.SetVocalVolume_58E6D0(1, 127 * field_24_sfx_vol / 127);
}

MATCH_FUNC(0x413000)
bool sound_obj::SelectCarObjectCollisionSound_413000(CollisionEvent* pEvent)
{
    if ((pEvent->field_18_object_model < 200 || pEvent->field_18_object_model > 244) &&
        (pEvent->field_18_object_model < 64 || pEvent->field_18_object_model > 108))
    {
        return SelectObjectImpactSound_413120(pEvent, 3);
    }
    PlayPickupVoice_412D90(pEvent->field_18_object_model);
    return true;
}

MATCH_FUNC(0x413040)
bool sound_obj::SelectCarPedCollisionSound_413040(u8 volume)
{
    if (volume < 15u)
    {
        return false;
    }

    s32 idx_to_use;
    if (volume < 23)
    {
        idx_to_use = 40;
    }
    else
    {
        idx_to_use = 41;
    }

    this->field_30_sQueueSample.field_20_rate =
        RandomDisplacement_41A650(idx_to_use) + gSampManager_6FFF00.GetPlayBackRate_58DBF0(idx_to_use);
    this->field_30_sQueueSample.field_14_samp_idx = idx_to_use;
    this->field_30_sQueueSample.field_18_bIs2D = false;
    return true;
}

MATCH_FUNC(0x413090)
bool sound_obj::SelectCarWallCollisionSound_413090(u8 volume)
{
    s32 idx_to_use;
    if (volume < 40u)
    {
        idx_to_use = 37;
    }
    else if (volume < 90u)
    {
        idx_to_use = 38;
    }
    else
    {
        idx_to_use = 39;
    }

    this->field_30_sQueueSample.field_20_rate =
        RandomDisplacement_41A650(idx_to_use) + gSampManager_6FFF00.GetPlayBackRate_58DBF0(idx_to_use);
    this->field_30_sQueueSample.field_14_samp_idx = idx_to_use;

    this->field_30_sQueueSample.field_18_bIs2D = false;
    return true;
}

MATCH_FUNC(0x4130E0)
bool sound_obj::SelectPedObjectCollisionSound_4130E0(CollisionEvent* pEvent)
{
    if ((pEvent->field_18_object_model < 200 || pEvent->field_18_object_model > 244) && (pEvent->field_18_object_model < 64 || pEvent->field_18_object_model > 108))
    {
        return SelectObjectImpactSound_413120(pEvent, 7);
    }
    PlayPickupVoice_412D90(pEvent->field_18_object_model);
    return true;
}

MATCH_FUNC(0x413120)
bool sound_obj::SelectObjectImpactSound_413120(CollisionEvent* pEvent, s32 interactionType)
{
    // Models 2..60 share the samp 37 code of 281/282 (one switch in the original, split at 110 by VC6).
    // The 110 case keeps its own copy after the if, which VC6 tail-merges back into the 281/282 block.
    s32 samp_idx;

    if (pEvent->field_18_object_model <= objects::ped_pushed_110)
    {
        if (pEvent->field_18_object_model == objects::ped_pushed_110)
        {
            // 110 shares the code of models 182 and 183
            goto case_110;
        }

        switch (pEvent->field_18_object_model)
        {
            case objects::bin_lid_1:
            case objects::hydrant_lid_18:
                field_30_sQueueSample.field_14_samp_idx = 55;
                field_30_sQueueSample.field_18_bIs2D = false;
                field_30_sQueueSample.field_20_rate = field_1454_anRandomTable[0] % 2000 + 23000;
                return true;

            case objects::bin_7:
                field_30_sQueueSample.field_14_samp_idx = 55;
                field_30_sQueueSample.field_18_bIs2D = false;
                field_30_sQueueSample.field_20_rate = field_1454_anRandomTable[0] % 2000 + 20000;
                return true;

            case objects::oildrum_16:
            case objects::tower_25:
            case 62:
                field_30_sQueueSample.field_14_samp_idx = 55;
                field_30_sQueueSample.field_18_bIs2D = false;
                field_30_sQueueSample.field_20_rate = field_1454_anRandomTable[0] % 2000 + 15000;
                return true;

            case objects::boxes_4:
            case objects::package_23:
            case 44:
                field_30_sQueueSample.field_14_samp_idx = 54;
                // An own copy of the next group's tail is cross-jumped the other way round
                goto rate_17000;

            case objects::rubbish_6:
                field_30_sQueueSample.field_14_samp_idx = 52;
                field_30_sQueueSample.field_18_bIs2D = false;
                field_30_sQueueSample.field_20_rate = field_1454_anRandomTable[0] % 600 + 16000;
                return true;

            case objects::cone_3:
            case objects::blaster_5:
            case objects::bush_11:
            case objects::footy_13:
            case objects::tyre_17:
            case 53:
            case 54:
            case 55:
                field_30_sQueueSample.field_14_samp_idx = 56;
            rate_17000:
                field_30_sQueueSample.field_18_bIs2D = false;
                field_30_sQueueSample.field_20_rate = field_1454_anRandomTable[0] % 2000 + 17000;
                return true;

            case objects::roadblock_21:
            case objects::bench_22:
            case 46:
            case 48:
                field_30_sQueueSample.field_14_samp_idx = 53;
                field_30_sQueueSample.field_18_bIs2D = false;
                field_30_sQueueSample.field_20_rate = field_1454_anRandomTable[0] % 1000 + 15500;
                return true;

            case objects::crate_12:
            case objects::hardbox_14:
            case 58:
                field_30_sQueueSample.field_14_samp_idx = 53;
                field_30_sQueueSample.field_18_bIs2D = false;
                field_30_sQueueSample.field_20_rate = field_1454_anRandomTable[0] % 2000 + 18000;
                return true;

            case objects::bollard_2:
            case objects::newsdis_15:
            case objects::hydrant_19:
            case objects::hydrant_unlid_20:
            case 60:
                // Same code as 281/282
                goto samp_37;

            default:
                return false;
        }
    }

    switch (pEvent->field_18_object_model)
    {
        case objects::diagonal_wall_collision_obj_166:
        case objects::door_collision_obj_169:
        case objects::tunnel_blocker_294:
        {
            field_30_sQueueSample.field_14_samp_idx = 37;
            s32 rate = RandomDisplacement_41A650(37) + gSampManager_6FFF00.GetPlayBackRate_58DBF0(37);
            field_30_sQueueSample.field_18_bIs2D = false;
            field_30_sQueueSample.field_20_rate = rate;
            return true;
        }

        case objects::power_generator_281:
        case objects::destroyed_powergen_282:
        samp_37:
            field_30_sQueueSample.field_14_samp_idx = 37;
            field_30_sQueueSample.field_18_bIs2D = false;
            field_30_sQueueSample.field_20_rate = field_1454_anRandomTable[0] % 2000 + 20000;
            return true;

        case 182:
        case objects::grenade_obj_183:
        case_110:
            if (interactionType == 1)
            {
                switch (pEvent->field_20_map_block_spec)
                {
                    case 1:
                    case 3:
                        field_30_sQueueSample.field_14_samp_idx = 198;
                        break;
                    case 2:
                    case 7:
                    case 10:
                        field_30_sQueueSample.field_14_samp_idx = 202;
                        break;
                    case 5:
                    case 6:
                    case 8:
                    case 9:
                        field_30_sQueueSample.field_14_samp_idx = 194;
                        break;
                    case 4:
                        field_30_sQueueSample.field_14_samp_idx = 68;
                        break;
                    default:
                        return false;
                }

                if (pEvent->field_20_map_block_spec == 4)
                {
                    field_30_sQueueSample.field_18_bIs2D = false;
                    field_30_sQueueSample.field_20_rate = field_1454_anRandomTable[1] % 4000 + 28000;
                    return true;
                }

                field_30_sQueueSample.field_20_rate = gSampManager_6FFF00.GetPlayBackRate_58DBF0(field_30_sQueueSample.field_14_samp_idx) +
                    RandomDisplacement_41A650(field_30_sQueueSample.field_14_samp_idx);
                field_30_sQueueSample.field_14_samp_idx += field_1454_anRandomTable[3] & 3;
                field_30_sQueueSample.field_18_bIs2D = false;
                return true;
            }
            field_30_sQueueSample.field_14_samp_idx = 37;
            field_30_sQueueSample.field_18_bIs2D = false;
            field_30_sQueueSample.field_20_rate = field_1454_anRandomTable[0] % 2000 + 20000;
            return true;

        case objects::shotgun_bullet_192:
        case objects::machine_gun_bullet_254:
        case objects::pistol_bullet_265:
            switch (interactionType)
            {
                case 3:
                    // 9.6f: Car::IsBulletproof_414F60 (field_78_flags & 0x100 via IsFlagSet_411930, inlined, a bool helper changes the code)
                    if ((pEvent->field_10_car->field_78_flags & car_flag::Bulletproof))
                    {
                        samp_idx = field_1454_anRandomTable[1] % 3 + 46;
                    }
                    else
                    {
                        samp_idx = field_1454_anRandomTable[1] % 3 + 43;
                    }
                    break;
                case 7:
                    field_30_sQueueSample.field_14_samp_idx = 42;
                    field_30_sQueueSample.field_18_bIs2D = false;
                    field_30_sQueueSample.field_20_rate = field_1454_anRandomTable[0] % 2000 + 20000;
                    return true;
                default:
                    samp_idx = field_1454_anRandomTable[1] % 3 + 49;
                    break;
            }
            field_30_sQueueSample.field_14_samp_idx = samp_idx;
            break;

        case 295:
            field_30_sQueueSample.field_14_samp_idx = 33;
            field_30_sQueueSample.field_20_rate = gSampManager_6FFF00.GetPlayBackRate_58DBF0(33);
            field_30_sQueueSample.field_18_bIs2D = true;
            return true;

        case objects::secret_token_266:
            field_30_sQueueSample.field_14_samp_idx = 34;
            field_30_sQueueSample.field_20_rate = gSampManager_6FFF00.GetPlayBackRate_58DBF0(34);
            field_30_sQueueSample.field_18_bIs2D = true;
            return true;

        default:
            return false;
    }

    field_30_sQueueSample.field_18_bIs2D = false;
    field_30_sQueueSample.field_20_rate = field_1454_anRandomTable[0] % 2000 + 20000;
    return true;
}

MATCH_FUNC(0x413540)
bool sound_obj::SelectObjectObjectCollisionSound_413540(CollisionEvent* pEvent)
{
    switch (pEvent->field_1C_other_object_model)
    {
        case objects::door_collision_obj_169:
        case 182:
        case objects::grenade_obj_183:
        case objects::shotgun_bullet_192:
        case objects::machine_gun_bullet_254:
        case objects::pistol_bullet_265:
        case objects::power_generator_281:
        case objects::destroyed_powergen_282:
        case objects::kill_frenzy_icon_286:
        case objects::tunnel_blocker_294:
        case 295:
            return SelectObjectImpactSound_413120(pEvent, 9);

        case objects::bin_lid_1:
        case objects::bollard_2:
        case objects::cone_3:
        case objects::boxes_4:
        case objects::blaster_5:
        case objects::rubbish_6:
        case objects::bin_7:
        case objects::bush_11:
        case objects::crate_12:
        case objects::footy_13:
        case objects::hardbox_14:
        case objects::newsdis_15:
        case objects::oildrum_16:
        case objects::tyre_17:
        case objects::hydrant_lid_18:
        case objects::hydrant_19:
        case objects::hydrant_unlid_20:
        case objects::roadblock_21:
        case objects::bench_22:
        case objects::package_23:
        case objects::tower_25:
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
        case objects::ped_pushed_110:
        case objects::moving_bush_123:
        case objects::moving_cone_155:
        case objects::moving_blaster_156:
        case objects::moving_bin_157:
        case objects::moving_bin_lid_158:
        case objects::diagonal_wall_collision_obj_166:
            return SelectObjectImpactSound_413120(pEvent, 9);

        default:
            return false;
    }
}

MATCH_FUNC(0x4136D0)
bool sound_obj::SelectCarFloorCollisionSound_4136D0(CollisionEvent* pEvent)
{
    s32 samp_idx;
    switch (pEvent->field_20_map_block_spec)
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
            return false;
    }
    this->field_30_sQueueSample.field_20_rate =
        RandomDisplacement_41A650(samp_idx) + gSampManager_6FFF00.GetPlayBackRate_58DBF0(samp_idx);
    this->field_30_sQueueSample.field_14_samp_idx = samp_idx;

    this->field_30_sQueueSample.field_18_bIs2D = false;
    return true;
}

MATCH_FUNC(0x413760)
void sound_obj::ProcessType6_CollisionSoundQueue_413760(s32 idx)
{
    LOCAL_STATIC(u8, sType6SampleIndexCounter, 0x66F541);
    u8 vol = 0;
    Fix16 distance = 0;
    char_type nearest_idx = -1;
    u8 emittingVol = 0;

    CollisionSoundQueue* pQueue = field_147C_audio_entities[idx].field_4_pObj->field_C_pAny.pCollisionSoundQueue;
    if (!pQueue)
    {
        return;
    }

    this->field_30_sQueueSample.field_5C_pReleaseSource = NULL;
    this->field_30_sQueueSample.field_0_EntityIndex = idx;

    for (u8 event_idx = 0; event_idx < pQueue->field_C84_count; event_idx++)
    {
        CollisionEvent* pEvent = &pQueue->field_4_events[event_idx];
        u8 volume = GetCollisionVolume_413A10(pEvent);
        if (SelectCollisionSound_412C90(pEvent, volume))
        {
            if (this->field_30_sQueueSample.field_18_bIs2D)
            {
                this->field_30_sQueueSample.field_8_obj.field_0_x = kFpZero_66F3F0;
                this->field_30_sQueueSample.field_8_obj.field_4_y = kFpZero_66F3F0;
                this->field_30_sQueueSample.field_8_obj.field_8_z = kFpZero_66F3F0;
                this->field_30_sQueueSample.field_4_SampleIndex = sType6SampleIndexCounter++;
                this->field_30_sQueueSample.field_1C_ReleasingVolumeModificator = 0;
                this->field_30_sQueueSample.field_24_nVolume = 45;
                this->field_30_sQueueSample.field_28_distance = kFpZero_66F3F0;
                this->field_30_sQueueSample.field_34_loop_start = 0;
                this->field_30_sQueueSample.field_30_loop_count = 1;
                this->field_30_sQueueSample.field_38_loop_end = -1;
                this->field_30_sQueueSample.field_3C_speed_multiplier = 0;
                this->field_30_sQueueSample.field_40_pan = 63;
                this->field_30_sQueueSample.field_41_releasing_sound_flag = 1;
                this->field_30_sQueueSample.field_54_sound_intensity = Fix16(5);
                this->field_30_sQueueSample.field_58_type = 20;
                this->field_30_sQueueSample.field_60_nEmittingVolume = 45;
                this->field_30_sQueueSample.field_64_max_distance = 10;
                if (sType6SampleIndexCounter >= 0xFF)
                {
                    sType6SampleIndexCounter = 0;
                }
                AddSampleToRequestedQueue_41A850();
                return;
            }

            if (volume > 0)
            {
                this->field_30_sQueueSample.field_8_obj.field_0_x = pQueue->field_4_events[event_idx].field_4_x;
                this->field_30_sQueueSample.field_8_obj.field_4_y = pQueue->field_4_events[event_idx].field_8_y;
                this->field_30_sQueueSample.field_8_obj.field_8_z = pQueue->field_4_events[event_idx].field_C_z;
                this->field_28_dist_squared = ComputeEmitterDistanceSquared_4190B0();
                this->field_2C_distCalculated = false;
                if (CalculateDistance_419020(Fix16(25)))
                {
                    if (VolCalc_419070(volume,
                                       Fix16(5),
                                       gMap_0x370_6F6268->CheckColumnHasSolidAbove_4E7FC0(this->field_30_sQueueSample.field_8_obj.field_0_x,
                                                                                          this->field_30_sQueueSample.field_8_obj.field_4_y,
                                                                                          this->field_30_sQueueSample.field_8_obj.field_8_z)))
                    {
                        nearest_idx = event_idx;
                        vol = this->field_30_sQueueSample.field_24_nVolume;
                        distance = this->field_30_sQueueSample.field_28_distance;
                        emittingVol = volume;
                    }
                }
            }
        }
    }

    if (nearest_idx == -1)
    {
        return;
    }

    this->field_30_sQueueSample.field_8_obj.field_0_x = pQueue->field_4_events[nearest_idx].field_4_x;
    this->field_30_sQueueSample.field_8_obj.field_4_y = pQueue->field_4_events[nearest_idx].field_8_y;
    this->field_30_sQueueSample.field_8_obj.field_8_z = pQueue->field_4_events[nearest_idx].field_C_z;
    this->field_30_sQueueSample.field_24_nVolume = vol;
    this->field_30_sQueueSample.field_28_distance = distance;
    this->field_30_sQueueSample.field_60_nEmittingVolume = emittingVol;
    this->field_30_sQueueSample.field_64_max_distance = 10;
    this->field_30_sQueueSample.field_54_sound_intensity = 81920; // F16
    this->field_30_sQueueSample.field_58_type = 20;
    this->field_30_sQueueSample.field_4_SampleIndex = sType6SampleIndexCounter++;
    this->field_30_sQueueSample.field_41_releasing_sound_flag = 1;
    this->field_30_sQueueSample.field_1C_ReleasingVolumeModificator = 8;
    this->field_30_sQueueSample.field_18_bIs2D = false;
    this->field_30_sQueueSample.field_3C_speed_multiplier = 600;
    this->field_30_sQueueSample.field_30_loop_count = 1;
    this->field_30_sQueueSample.field_34_loop_start = 0;
    this->field_30_sQueueSample.field_38_loop_end = -1;
    if (sType6SampleIndexCounter >= 0xFF)
    {
        sType6SampleIndexCounter = 0;
    }
    AddSampleToRequestedQueue_41A850();
}

MATCH_FUNC(0x413A10)
u8 sound_obj::GetCollisionVolume_413A10(CollisionEvent* pEvent)
{
    LOCAL_STATIC(u8, sCollisionVolumeRngIndex, 0x66F542);
    // Each case divides by its own max (9.6f calls div per case); VC6 tail-merges the divisions.
    Fix16 ratio;
    switch (pEvent->field_0_type)
    {
        case collision_event_type::car_car_2:
            if (pEvent->field_24_impact_strength < kFpPoint01_66F3B4)
            {
                return 0;
            }
            if (pEvent->field_24_impact_strength > kFpTwo_66F3F8)
            {
                pEvent->field_24_impact_strength = kFpTwo_66F3F8;
            }
            ratio = pEvent->field_24_impact_strength / kFpTwo_66F3F8;
            break;

        case collision_event_type::car_object_3:
            if (pEvent->field_18_object_model == objects::shotgun_bullet_192 ||
                pEvent->field_18_object_model == objects::machine_gun_bullet_254 ||
                pEvent->field_18_object_model == objects::pistol_bullet_265)
            {
                return 37;
            }
            if (!pEvent->field_10_car)
            {
                // Shares case 5's return block (9.6f jumps to the same block; a plain return doesn't match)
                goto return_zero;
            }
            pEvent->field_24_impact_strength = pEvent->field_10_car->GetCarLinearSpeed_43A240();
            if (pEvent->field_24_impact_strength < kFpPoint02)
            {
                return 0;
            }
            if (pEvent->field_24_impact_strength > kFpPoint2_66F24C)
            {
                pEvent->field_24_impact_strength = kFpPoint2_66F24C;
            }
            ratio = pEvent->field_24_impact_strength / kFpPoint2_66F24C;
            break;

        case collision_event_type::car_ped_4:
            if (pEvent->field_24_impact_strength < kFpPoint004)
            {
                return 0;
            }
            if (pEvent->field_24_impact_strength > kFpHalf_66F2FC)
            {
                pEvent->field_24_impact_strength = kFpHalf_66F2FC;
            }
            ratio = pEvent->field_24_impact_strength / kFpHalf_66F2FC;
            break;

        case collision_event_type::car_wall_5:
            if (pEvent->field_24_impact_strength < kFpPoint01_66F3B4)
            {
            return_zero:
                return 0;
            }
            if (pEvent->field_24_impact_strength > kFpThree_66F3FC)
            {
                pEvent->field_24_impact_strength = kFpThree_66F3FC;
            }
            ratio = pEvent->field_24_impact_strength / kFpThree_66F3FC;
            break;

        default:
            return this->field_1454_anRandomTable[static_cast<u8>(++sCollisionVolumeRngIndex) % 5] % 0xAu + 5;
    }
    return Fix16::Round_To_Int_410BF0(ratio * kFp127);
}

MATCH_FUNC(0x413B90)
void sound_obj::ProcessTrainCab_413B90(Sound_Params* pParams, SoundPosEntry* pAlloc)
{
    if (pParams->field_0_pObj->field_8_car_bc_ptr->field_58_physics)
    {
        if (!pParams->field_5_bHasSolidAbove)
        {
            HandleTrainEngineSound_4140C0(pParams);
            HandleTrainCabRollingFrictionSound_4143A0(pParams);
        }
    }
    HandleCarHornSound_415570(pParams, pAlloc);
    HandleCarDoorSounds_4182E0(pParams);
    HandleTrainStationSound_414710(pParams);
}

MATCH_FUNC(0x413BE0)
void sound_obj::ProcessTrain_413BE0(Sound_Params* pParams)
{
    HandleCarDoorSounds_4182E0(pParams);
}

MATCH_FUNC(0x413BF0)
void sound_obj::ProcessTank_413BF0(Sound_Params* pParams, SoundPosEntry* pAlloc)
{
    if (pParams->field_0_pObj->field_8_car_bc_ptr->field_58_physics)
    {
        HandleTankMovementSound_414A50(pParams);
        HandleCarEngineSound_4157C0(pParams);
        HandleTankCrushSound_415190(pParams);
    }
    HandleTankTurretSound_414D30(pParams);
    HandleCarDoorSounds_4182E0(pParams);
    HandleCarHornSound_415570(pParams, pAlloc);
    PlayCarShopEventSound_415480(pParams);
}

MATCH_FUNC(0x413C50)
void sound_obj::ProcessOtherCarTypes_413C50(Sound_Params* pParams, SoundPosEntry* pAlloc)
{
    Car* pCar = pParams->field_0_pObj->field_8_car_bc_ptr;
    pAlloc->field_4_car = pCar;
    if (pCar->field_68_scale != kFpOne_66F3F4)
    {
        if (pCar->field_58_physics)
        {
            HandleCarEngineSound_4157C0(pParams);
        }
    }
    else
    {
        if (pParams->field_0_pObj->field_8_car_bc_ptr->field_58_physics)
        {
            HandleCarEngineSound_4157C0(pParams);
            HandleTruckCorneringAudio_417FD0(pParams);
            HandleAICarEngineSound_418190(pParams);
            HandleCarTireScrubSound_418720(pParams);
        }
        HandleCarDoorSounds_4182E0(pParams);
        HandleSirenActivationSound_4178C0(pParams);
        HandleCarAlarmSound_417060(pParams);
        HandleCarHornSound_415570(pParams, pAlloc);
        HandleCarEngineStartSound_418610(pParams);
        HandleCarDamageSound_4177D0(pParams);
        HandleHeavyVehicleStopSound_417E30(pParams, pAlloc);
        PlayCarShopEventSound_415480(pParams);
        HandleCarBombSound_414F90(pParams);
        HandleAICarHornBeep_413D10(pParams);
    }
}

MATCH_FUNC(0x413D10)
void sound_obj::HandleAICarHornBeep_413D10(Sound_Params* pParams)
{
    LOCAL_STATIC(char_type, sAICarHornCooldown, 0x66F543);
    LOCAL_STATIC_INIT(u8, sSoundSampleIdx, 0x64, 0x61A684);
    s32 samp_idx;

    Car* pCar = pParams->field_0_pObj->field_8_car_bc_ptr;
    const s32 sound_event = pCar->TakeSoundEvent_411950();
    if (!pCar->IsMaxDamage_40F890() && pCar->field_54_driver && pParams->field_4_bDrivenByPlayer != 1)
    {
        switch (pCar->GetCarInfoIdx_411940())
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
                if (sound_event <= 0)
                {
                    return;
                }

                if (sound_event > 2)
                {
                    if (sound_event != 3)
                    {
                        return;
                    }
                    if (!sAICarHornCooldown)
                    {
                        sAICarHornCooldown = 8;
                        samp_idx = this->field_1454_anRandomTable[this->field_30_sQueueSample.field_0_EntityIndex % 5u] % 7u + 265;
                    }
                    else
                    {
                        --sAICarHornCooldown;
                        return;
                    }
                }
                else
                {
                    samp_idx = (this->field_1454_anRandomTable[this->field_30_sQueueSample.field_0_EntityIndex % 5u] & 7) + 257;
                }

                this->field_30_sQueueSample.field_14_samp_idx = samp_idx;

                if (CalculateDistance_419020(Fix16(1806336, 0)))
                {
                    const u8 emit_vol = this->field_1454_anRandomTable[this->field_30_sQueueSample.field_0_EntityIndex % 5u] % 30u + 50;
                    if (VolCalc_419070(emit_vol, Fix16(172032, 0), pParams->field_5_bHasSolidAbove))
                    {
                        this->field_30_sQueueSample.field_54_sound_intensity = Fix16(172032, 0);
                        this->field_30_sQueueSample.field_60_nEmittingVolume = emit_vol;
                        this->field_30_sQueueSample.field_64_max_distance = 21;
                        this->field_30_sQueueSample.field_20_rate = gSampManager_6FFF00.GetPlayBackRate_58DBF0(field_30_sQueueSample.field_14_samp_idx) +
                            RandomDisplacement_41A650(field_30_sQueueSample.field_14_samp_idx);
                        this->field_30_sQueueSample.field_58_type = 20;
                        this->field_30_sQueueSample.field_3C_speed_multiplier = 0;
                        this->field_30_sQueueSample.field_4_SampleIndex = sSoundSampleIdx;
                        this->field_30_sQueueSample.field_41_releasing_sound_flag = 1;
                        this->field_30_sQueueSample.field_1C_ReleasingVolumeModificator = 7;
                        this->field_30_sQueueSample.field_18_bIs2D = false;
                        this->field_30_sQueueSample.field_34_loop_start = 0;
                        this->field_30_sQueueSample.field_38_loop_end = -1;
                        this->field_30_sQueueSample.field_30_loop_count = 1;
                        AddSampleToRequestedQueue_41A850();
                        if (++sSoundSampleIdx >= 200u)
                        {
                            sSoundSampleIdx = 100;
                        }
                    }
                }
                break;
        }
    }
}

MATCH_FUNC(0x4140C0)
void sound_obj::HandleTrainEngineSound_4140C0(Sound_Params* pParams)
{
    Car* pCar = pParams->field_0_pObj->field_8_car_bc_ptr;
    Fix16 car_speed;
    car_speed = pCar->GetCarLinearSpeed_43A240();
    Fix16 max_speed = gCarInfo_48_6FE258->field_28_max_speed;

    if (car_speed > kFpZero_66F3F0 && max_speed > kFpZero_66F3F0)
    {
        if (CalculateDistance_419020(Fix16(4225)))
        {
            u8 vol = (car_speed / max_speed * Fix16(65) + Fix16(0x2000, 0)).ToInt();
            if (vol > 0)
            {
                if (VolCalc_419070(vol, Fix16(65), pParams->field_5_bHasSolidAbove))
                {
                    this->field_30_sQueueSample.field_54_sound_intensity = Fix16(65);
                    this->field_30_sQueueSample.field_60_nEmittingVolume = vol;
                    this->field_30_sQueueSample.field_64_max_distance = 130;
                    this->field_30_sQueueSample.field_58_type = 16;
                    this->field_30_sQueueSample.field_4_SampleIndex = 0;
                    this->field_30_sQueueSample.field_41_releasing_sound_flag = 0;
                    this->field_30_sQueueSample.field_1C_ReleasingVolumeModificator = 4;
                    this->field_30_sQueueSample.field_18_bIs2D = false;
                    AddSampleToRequestedQueue_41A850();
                }
            }
        }
    }
}

MATCH_FUNC(0x414320)
bool sound_obj::Type_16_InitTrainEngineSound_414320(sound_0x68* pObj)
{
    pObj->field_14_samp_idx = 10;
    pObj->field_3C_speed_multiplier = 400;
    pObj->field_30_loop_count = 0;
    pObj->field_34_loop_start = 0;
    pObj->field_38_loop_end = -1;
    pObj->field_20_rate = gSampManager_6FFF00.GetPlayBackRate_58DBF0(10);
    pObj->field_4C_releasing_volume_divider = 30;
    return true;
}

MATCH_FUNC(0x4143A0)
void sound_obj::HandleTrainCabRollingFrictionSound_4143A0(Sound_Params* pParams)
{

    Car* pCar = pParams->field_0_pObj->field_8_car_bc_ptr;
    Fix16 car_speed;
    car_speed = pCar->GetCarLinearSpeed_43A240();
    Fix16 max_speed = gCarInfo_48_6FE258->field_28_max_speed;
    if (car_speed > kFpZero_66F3F0 && max_speed > kFpZero_66F3F0)
    {
        if (CalculateDistance_419020(Fix16(225)))
        {
            u8 vol = (car_speed / max_speed * Fix16(127) + Fix16(0x2000, 0)).ToInt();
            if (vol > 0)
            {
                if (VolCalc_419070(vol, Fix16(15), pParams->field_5_bHasSolidAbove))
                {
                    this->field_30_sQueueSample.field_54_sound_intensity = Fix16(15);
                    this->field_30_sQueueSample.field_60_nEmittingVolume = vol;
                    this->field_30_sQueueSample.field_64_max_distance = 30;
                    this->field_30_sQueueSample.field_58_type = 17;
                    this->field_30_sQueueSample.field_4_SampleIndex = 2;
                    this->field_30_sQueueSample.field_41_releasing_sound_flag = 0;
                    this->field_30_sQueueSample.field_1C_ReleasingVolumeModificator = 5;
                    this->field_30_sQueueSample.field_18_bIs2D = false;
                    AddSampleToRequestedQueue_41A850();
                }
            }
        }
    }
}

MATCH_FUNC(0x4145E0)
bool sound_obj::GetCar_4145E0(s32 idx, Car** ppOut) const
{
    if (field_147C_audio_entities[idx].field_0_bUsed)
    {
        Sprite* pSprite = field_147C_audio_entities[idx].field_4_pObj->field_C_pAny.pSprite;
        if (pSprite)
        {
            if (pSprite->field_10_sound)
            {
                Car* pCar = pSprite->AsCar_40FEB0();
                *ppOut = pCar;
                if (pCar)
                {
                    return true;
                }
            }
        }
    }
    return false;
}

MATCH_FUNC(0x414690)
bool sound_obj::Type_17_InitTrainRollingFrictionSound_414690(sound_0x68* pObj)
{
    pObj->field_20_rate = 11025;
    pObj->field_14_samp_idx = 11;
    pObj->field_3C_speed_multiplier = 500;
    pObj->field_30_loop_count = 0;
    pObj->field_34_loop_start = 0;
    pObj->field_38_loop_end = -1;
    pObj->field_4C_releasing_volume_divider = 30;
    return true;
}

// 9.6f 0x412A20
MATCH_FUNC(0x414710)
void sound_obj::HandleTrainStationSound_414710(Sound_Params* pParams)
{
    Ped* pDriver = pParams->field_0_pObj->field_8_car_bc_ptr->field_54_driver;
    if (pDriver)
    {
        TrainStation* pTrainStation = pDriver->PopTrainStation_4117D0();
        if (pTrainStation)
        {
            this->field_30_sQueueSample.field_14_samp_idx = 139;
        }
        else
        {
            pTrainStation = pDriver->PopTrainStation_4117D0();
            if (!pTrainStation)
            {
                return;
            }
            this->field_30_sQueueSample.field_14_samp_idx = 140;
        }

        if (pTrainStation->field_10_pZone)
        {
            this->field_30_sQueueSample.field_8_obj.field_0_x =
                Fix16(pTrainStation->field_10_pZone->field_1_x + (pTrainStation->field_10_pZone->field_3_w >> 1));
            this->field_30_sQueueSample.field_8_obj.field_4_y =
                Fix16(pTrainStation->field_10_pZone->field_2_y + (pTrainStation->field_10_pZone->field_4_h >> 1));
        }
        this->field_28_dist_squared = ComputeEmitterDistanceSquared_4190B0();
        this->field_2C_distCalculated = false;
        if (CalculateDistance_419020(Fix16(1225)))
        {
            if (VolCalc_419070(100, Fix16(35), false))
            {
                this->field_30_sQueueSample.field_54_sound_intensity = Fix16(35);
                this->field_30_sQueueSample.field_60_nEmittingVolume = 100;
                this->field_30_sQueueSample.field_64_max_distance = 70;
                this->field_30_sQueueSample.field_58_type = 19;
                this->field_30_sQueueSample.field_4_SampleIndex = 3;
                this->field_30_sQueueSample.field_41_releasing_sound_flag = 1;
                this->field_30_sQueueSample.field_1C_ReleasingVolumeModificator = 5;
                this->field_30_sQueueSample.field_18_bIs2D = false;
                AddSampleToRequestedQueue_41A850();
            }
        }
    }
}

MATCH_FUNC(0x4149D0)
bool sound_obj::Type_19_InitTrainStationSound_4149D0(sound_0x68* pObj)
{
    pObj->field_3C_speed_multiplier = 400;
    pObj->field_30_loop_count = 1;
    pObj->field_34_loop_start = 0;
    pObj->field_38_loop_end = -1;
    pObj->field_20_rate = gSampManager_6FFF00.GetPlayBackRate_58DBF0(pObj->field_14_samp_idx);
    return true;
}

MATCH_FUNC(0x414A50)
void sound_obj::HandleTankMovementSound_414A50(Sound_Params* pParams)
{
    Car* pCar = pParams->field_0_pObj->field_8_car_bc_ptr;
    if (pCar->field_9C_engine_status == car_engine_status::on_3)
    {
        if (CalculateDistance_419020((Fix16(20) / Fix16(2)) * (Fix16(20) / Fix16(2))))
        {
            Fix16 vol_mult;
            vol_mult = pCar->GetCarLinearSpeed_43A240();
            Fix16 max_speed = gCarInfo_48_6FE258->field_28_max_speed;
            if (max_speed > kFpZero_66F3F0)
            {
                if (vol_mult > max_speed)
                {
                    vol_mult = max_speed;
                }

                u8 vol = Fix16::Round_To_Int_410BF0((vol_mult / max_speed) * Fix16(90));

                if (VolCalc_419070(vol, Fix16(20) / Fix16(2), pParams->field_5_bHasSolidAbove))
                {
                    this->field_30_sQueueSample.field_54_sound_intensity = Fix16(20) / Fix16(2);
                    this->field_30_sQueueSample.field_60_nEmittingVolume = vol;
                    this->field_30_sQueueSample.field_64_max_distance = 20;
                    this->field_30_sQueueSample.field_58_type = 12;
                    this->field_30_sQueueSample.field_4_SampleIndex = 5;
                    this->field_30_sQueueSample.field_41_releasing_sound_flag = 0;
                    this->field_30_sQueueSample.field_1C_ReleasingVolumeModificator = 10;
                    this->field_30_sQueueSample.field_18_bIs2D = false;
                    AddSampleToRequestedQueue_41A850();
                }
            }
        }
    }
}

MATCH_FUNC(0x414C90)
bool sound_obj::Type_12_InitTankMovementSound_414C90(sound_0x68* pSound)
{
    pSound->field_14_samp_idx = 35;
    pSound->field_3C_speed_multiplier = 400;
    pSound->field_4C_releasing_volume_divider = 5;
    pSound->field_20_rate = gSampManager_6FFF00.GetPlayBackRate_58DBF0(35);
    pSound->field_30_loop_count = 0;
    pSound->field_34_loop_start = gSampManager_6FFF00.GetLoopStart_58DC30(35);
    pSound->field_38_loop_end = gSampManager_6FFF00.GetLoopEnd_58DC50(35);
    return true;
}

MATCH_FUNC(0x414D30)
void sound_obj::HandleTankTurretSound_414D30(Sound_Params* pParams)
{
    if (pParams->field_0_pObj->field_8_car_bc_ptr->field_B8_turret_rotated)
    {
        if (CalculateDistance_419020(Fix16(25)))
        {
            if (VolCalc_419070(0x32u, Fix16(5), pParams->field_5_bHasSolidAbove))
            {
                this->field_30_sQueueSample.field_54_sound_intensity = Fix16(5);
                this->field_30_sQueueSample.field_60_nEmittingVolume = 50;
                this->field_30_sQueueSample.field_64_max_distance = 10;
                this->field_30_sQueueSample.field_58_type = 11;
                this->field_30_sQueueSample.field_4_SampleIndex = 4;
                this->field_30_sQueueSample.field_41_releasing_sound_flag = 0;
                this->field_30_sQueueSample.field_18_bIs2D = false;
                if (pParams->field_4_bDrivenByPlayer)
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

MATCH_FUNC(0x414EE0)
bool sound_obj::Type_11_InitTankTurretSound_414EE0(sound_0x68* pSound)
{
    pSound->field_14_samp_idx = 60;
    pSound->field_3C_speed_multiplier = 400;
    pSound->field_4C_releasing_volume_divider = 3;
    pSound->field_20_rate = 9000;
    pSound->field_30_loop_count = 0;
    pSound->field_34_loop_start = gSampManager_6FFF00.GetLoopStart_58DC30(60);
    pSound->field_38_loop_end = gSampManager_6FFF00.GetLoopEnd_58DC50(60);
    return true;
}

MATCH_FUNC(0x414F90)
void sound_obj::HandleCarBombSound_414F90(Sound_Params* pParams)
{
    if (pParams->field_0_pObj->field_8_car_bc_ptr->field_0_attachments.GetSpriteForModel_5A6A50(objects::car_bomb_132))
    {
        if (CalculateDistance_419020(Fix16(25)))
        {
            if (VolCalc_419070(0x28u, Fix16(5), pParams->field_5_bHasSolidAbove))
            {
                this->field_30_sQueueSample.field_58_type = 15;
                this->field_30_sQueueSample.field_4_SampleIndex = 15;
                this->field_30_sQueueSample.field_54_sound_intensity = Fix16(5);
                this->field_30_sQueueSample.field_60_nEmittingVolume = 40;
                this->field_30_sQueueSample.field_64_max_distance = 10;
                this->field_30_sQueueSample.field_41_releasing_sound_flag = 0;
                this->field_30_sQueueSample.field_18_bIs2D = false;
                this->field_30_sQueueSample.field_1C_ReleasingVolumeModificator = 7;
                AddSampleToRequestedQueue_41A850();
            }
        }
    }
}

MATCH_FUNC(0x415100)
bool sound_obj::Type_15_InitCarBombSound_415100(sound_0x68* pSound)
{
    pSound->field_14_samp_idx = 57;
    pSound->field_3C_speed_multiplier = 400;
    pSound->field_4C_releasing_volume_divider = 3;
    pSound->field_20_rate = 22050;
    pSound->field_30_loop_count = 0;
    pSound->field_34_loop_start = gSampManager_6FFF00.GetLoopStart_58DC30(57);
    pSound->field_38_loop_end = gSampManager_6FFF00.GetLoopEnd_58DC50(57);
    return true;
}

MATCH_FUNC(0x415190)
void sound_obj::HandleTankCrushSound_415190(Sound_Params* pParams)
{

    Car* pCar = pParams->field_0_pObj->field_8_car_bc_ptr;
    if (pCar->IsEngineOn_4118C0() && pCar->IsCrushing_414F70())
    {
        if (CalculateDistance_419020(Fix16(921600, 0)))
        {
            Fix16 vol_mult;
            vol_mult = pCar->GetCarLinearSpeed_43A240();
            Fix16 max_speed = gCarInfo_48_6FE258->field_28_max_speed;
            if (max_speed > kFpZero_66F3F0)
            {
                if (vol_mult > max_speed)
                {
                    vol_mult = max_speed;
                }

                u8 vol = Fix16::Round_To_Int_410BF0((vol_mult / max_speed) * Fix16(80));

                if (VolCalc_419070(vol, Fix16(122880, 0), pParams->field_5_bHasSolidAbove))
                {
                    this->field_30_sQueueSample.field_54_sound_intensity = Fix16(122880, 0);
                    this->field_30_sQueueSample.field_60_nEmittingVolume = vol;
                    this->field_30_sQueueSample.field_64_max_distance = 15;
                    this->field_30_sQueueSample.field_58_type = 13;
                    this->field_30_sQueueSample.field_4_SampleIndex = 14;
                    this->field_30_sQueueSample.field_41_releasing_sound_flag = 0;
                    this->field_30_sQueueSample.field_1C_ReleasingVolumeModificator = 16;
                    this->field_30_sQueueSample.field_18_bIs2D = false;
                    AddSampleToRequestedQueue_41A850();
                }
            }
        }
    }
}

MATCH_FUNC(0x4153F0)
bool sound_obj::Type_13_InitTankCrushSound_4153F0(sound_0x68* pObj)
{
    pObj->field_14_samp_idx = 36;
    pObj->field_3C_speed_multiplier = 400;
    pObj->field_4C_releasing_volume_divider = 3;
    pObj->field_20_rate = gSampManager_6FFF00.GetPlayBackRate_58DBF0(36);
    pObj->field_30_loop_count = 0;
    pObj->field_34_loop_start = gSampManager_6FFF00.GetLoopStart_58DC30(36);
    pObj->field_38_loop_end = gSampManager_6FFF00.GetLoopEnd_58DC50(36);
    return true;
}

MATCH_FUNC(0x415480)
void sound_obj::PlayCarShopEventSound_415480(Sound_Params* pParams)
{
    Car* pCar = pParams->field_0_pObj->field_8_car_bc_ptr;
    const s32 shop_event = pCar->TakeShopEvent_4119D0();
    switch (shop_event)
    {
        case car_shop_event::respray_1:
            this->field_30_sQueueSample.field_14_samp_idx = 67;
            break;
        case car_shop_event::change_plates_2:
        case 3:
            this->field_30_sQueueSample.field_14_samp_idx = 66;
            break;
        case car_shop_event::car_bomb_4:
            this->field_30_sQueueSample.field_14_samp_idx = 65;
            break;
        case car_shop_event::car_mines_5:
            this->field_30_sQueueSample.field_14_samp_idx = 62;
            break;
        case car_shop_event::oil_stain_6:
            this->field_30_sQueueSample.field_14_samp_idx = 63;
            break;
        case car_shop_event::car_smg_7:
            this->field_30_sQueueSample.field_14_samp_idx = 64;
            break;
        default:
            return;
    }

    this->field_30_sQueueSample.field_60_nEmittingVolume = 50;
    this->field_30_sQueueSample.field_64_max_distance = 100;
    this->field_30_sQueueSample.field_58_type = 20;
    this->field_30_sQueueSample.field_54_sound_intensity = Fix16(100);
    this->field_30_sQueueSample.field_4_SampleIndex = 3;
    this->field_30_sQueueSample.field_41_releasing_sound_flag = 1;
    this->field_30_sQueueSample.field_18_bIs2D = true;
    this->field_30_sQueueSample.field_1C_ReleasingVolumeModificator = 0;
    this->field_30_sQueueSample.field_3C_speed_multiplier = 0;
    this->field_30_sQueueSample.field_20_rate = gSampManager_6FFF00.GetPlayBackRate_58DBF0(field_30_sQueueSample.field_14_samp_idx);
    this->field_30_sQueueSample.field_30_loop_count = 1;
    this->field_30_sQueueSample.field_34_loop_start = 0;
    this->field_30_sQueueSample.field_38_loop_end = -1;
    this->field_30_sQueueSample.field_40_pan = 63;
    AddSampleToRequestedQueue_41A850();
}

MATCH_FUNC(0x415570)
void sound_obj::HandleCarHornSound_415570(Sound_Params* pParams, SoundPosEntry* pAlloc)
{
    Car* pCar = pParams->field_0_pObj->field_8_car_bc_ptr;
    if (pCar->IsFireTruck_4118F0() || !pCar->HasEmergencyLights_414F20() || !pCar->AreEmergencyLightsOn_414F80())
    {
        if (!pCar->IsFireTruck_4118F0() && !pCar->IsTank_411900() && !pCar->IsGunJeep_411910() || !pCar->IsTurretRotated_4119F0())
        {
            if (pParams->field_0_pObj->field_8_car_bc_ptr->IsEmittingHorn_411970())
            {
                bool tryPlay = false;
                if (pParams->field_4_bDrivenByPlayer || pCar->GetCarInfoIdx_411940() == car_model_enum::ICECREAM)
                {
                    tryPlay = true;
                }
                else if (pCar->IsEmittingHorn_411990())
                {
                    u8 horn_timer = pCar->field_A7_horn;
                    if (horn_timer >= 44u)
                    {
                        pAlloc->field_A_horn_pattern_idx = this->field_1454_anRandomTable[4] & 7;
                        horn_timer = 44;
                    }

                    if (kHornPatterns_5FE434[pAlloc->field_A_horn_pattern_idx][static_cast<u8>(44u - horn_timer)])
                    {
                        tryPlay = true;
                    }
                }

                if (tryPlay && CalculateDistance_419020((Fix16(15) / Fix16(2)) * (Fix16(15) / Fix16(2))))
                {
                    const u8 vol = pCar->GetCarInfoIdx_411940() == car_model_enum::TRAINCAB ? 127 : 20;
                    if (VolCalc_419070(vol, Fix16(15) / Fix16(2), pParams->field_5_bHasSolidAbove))
                    {
                        this->field_30_sQueueSample.field_54_sound_intensity = Fix16(15) / Fix16(2);
                        this->field_30_sQueueSample.field_60_nEmittingVolume = vol;
                        this->field_30_sQueueSample.field_64_max_distance = 15;
                        this->field_30_sQueueSample.field_58_type = 5;
                        this->field_30_sQueueSample.field_4_SampleIndex = 8;
                        this->field_30_sQueueSample.field_41_releasing_sound_flag = 0;
                        this->field_30_sQueueSample.field_18_bIs2D = false;
                        if (pParams->field_4_bDrivenByPlayer)
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

MATCH_FUNC(0x415730)
bool sound_obj::Type_5_InitHornSound_415730(sound_0x68* pSound)
{
    pSound->field_20_rate = 22050;

    Car* pCar;
    if (GetCar_4145E0(pSound->field_0_EntityIndex, &pCar))
    {
        const s32 car_model = pCar->field_84_car_info_idx;
        pSound->field_14_samp_idx = GetVehicleAudioClass_417BA0(car_model);
        pSound->field_3C_speed_multiplier = 600;
        pSound->field_4C_releasing_volume_divider = 1;
        pSound->field_20_rate = ConvertToPlayBackRate_417C60(car_model);
        pSound->field_30_loop_count = 0;
        pSound->field_34_loop_start = gSampManager_6FFF00.GetLoopStart_58DC30(pSound->field_14_samp_idx);
        pSound->field_38_loop_end = gSampManager_6FFF00.GetLoopEnd_58DC50(pSound->field_14_samp_idx);
        return true;
    }
    return false;
}

MATCH_FUNC(0x4157C0)
void sound_obj::HandleCarEngineSound_4157C0(Sound_Params* pParams)
{
    Car* pCar = pParams->field_0_pObj->field_8_car_bc_ptr;

    if (pCar->field_9C_engine_status == car_engine_status::on_3 && CalculateDistance_419020(Fix16(36)))
    {
        Fix16 gas_pedal;
        gas_pedal = pCar->GetCarLinearSpeed_43A240();
        Fix16 max_speed = gCarInfo_48_6FE258->field_28_max_speed;
        u8 emitting_vol;
        if (max_speed > kFpZero_66F3F0)
        {
            if (gas_pedal == kFpZero_66F3F0)
            {
                if (pCar->field_68_scale != kFpOne_66F3F4)
                {
                    return;
                }

                max_speed = kFpFive;
                if (!pCar->field_58_physics->field_94_is_backward_gas_on)
                {
                    max_speed = kFpFive_66F1D0;
                }
                gas_pedal = pCar->field_58_physics->field_60_gas_pedal;
                emitting_vol = Fix16::Round_To_Int_410BF0(Fix16(35) * (gas_pedal / max_speed)) + 25;
            }
            else
            {
                if (gas_pedal > max_speed)
                {
                    gas_pedal = max_speed;
                }

                if (pCar->field_84_car_info_idx == car_model_enum::TANK || pCar->field_58_physics->field_94_is_backward_gas_on)
                {
                    emitting_vol = Fix16::Round_To_Int_410BF0(Fix16(35) * (gas_pedal / max_speed)) + 25;
                }
                else if (pCar->field_68_scale != kFpOne_66F3F4)
                {
                    emitting_vol = Fix16::Round_To_Int_410BF0(Fix16(40) * (gas_pedal / max_speed));
                }
                else
                {
                    Fix16 gear2_speed = gCarInfo_48_6FE258->field_40_gear2_speed;
                    Fix16 gear3_speed = gCarInfo_48_6FE258->field_44_gear3_speed;
                    if (gas_pedal < gear2_speed)
                    {
                        emitting_vol = Fix16::Round_To_Int_410BF0(Fix16(35) * (gas_pedal / gear2_speed)) + 25;
                    }
                    else if (gas_pedal < gear3_speed)
                    {
                        emitting_vol = Fix16::Round_To_Int_410BF0(Fix16(35) * ((gas_pedal - gear2_speed) / (gear3_speed - gear2_speed))) + 25;
                    }
                    else
                    {
                        emitting_vol = Fix16::Round_To_Int_410BF0((Fix16(35) * kFpHalf_66F258) * ((gas_pedal - gear3_speed) / (max_speed - gear3_speed))) + 25;
                    }
                }
            }
        }
        else
        {
            emitting_vol = 20;
        }

        if (VolCalc_419070(emitting_vol, Fix16(6), pParams->field_5_bHasSolidAbove))
        {
            this->field_30_sQueueSample.field_54_sound_intensity = Fix16(6);
            this->field_30_sQueueSample.field_60_nEmittingVolume = emitting_vol;
            this->field_30_sQueueSample.field_64_max_distance = 12;
            this->field_30_sQueueSample.field_58_type = 1;
            this->field_30_sQueueSample.field_4_SampleIndex = 0;
            this->field_30_sQueueSample.field_41_releasing_sound_flag = 0;
            if (pParams->field_4_bDrivenByPlayer)
            {
                this->field_30_sQueueSample.field_1C_ReleasingVolumeModificator = 3;
            }
            else
            {
                this->field_30_sQueueSample.field_1C_ReleasingVolumeModificator = 5;
            }
            this->field_30_sQueueSample.field_18_bIs2D = false;
            AddSampleToRequestedQueue_41A850();
        }
    }
}

// Car engine sounds/revs etc of the car we are driving
MATCH_FUNC(0x416260)
bool sound_obj::Type_1_6_InitCarEngineOrDamageSound_416260(sound_0x68* pSound)
{
    // Both branches store the same rate: VC6 merges them but keeps the type compare (an orphan
    // `cmp %ebx,%eax` with ebx = 1 at the top of the original), which also keeps 1 in ebx
    if (pSound->field_58_type == 1)
    {
        pSound->field_20_rate = 8000;
    }
    else
    {
        pSound->field_20_rate = 8000;
    }
    Car* pCar;
    if (GetCar_4145E0(pSound->field_0_EntityIndex, &pCar) && pCar->field_9C_engine_status == car_engine_status::on_3)
    {
        if (pCar->field_58_physics)
        {
            pCar->field_58_physics->SetModelPhysicsGlobal_562EB0();
            Fix16 max_speed = gCarInfo_48_6FE258->field_28_max_speed;
            Fix16 speed;
            speed = pCar->GetCarLinearSpeed_43A240();

            if (max_speed > kFpZero_66F3F0)
            {
                if (speed == kFpZero_66F3F0)
                {
                    max_speed = kFpFive;
                    if (!pCar->field_58_physics->is_backward_gas_on_411810())
                    {
                        max_speed = kFpFive_66F1D0;
                    }

                    if (pSound->field_58_type == 1)
                    {
                        if (pCar->field_68_scale == kFpOne_66F3F4)
                        {
                            speed = pCar->field_58_physics->field_60_gas_pedal;
                            pSound->field_20_rate = Fix16::Round_To_Int_410BF0(Fix16(22000) * (speed / max_speed)) + 8000;
                        }
                        else
                        {
                            speed = pCar->field_58_physics->field_60_gas_pedal;
                            pSound->field_20_rate = Fix16::Round_To_Int_410BF0(Fix16(19000) * (speed / max_speed)) + 11000;
                        }
                    }
                    else
                    {
                        speed = pCar->field_58_physics->field_60_gas_pedal;
                        pSound->field_20_rate = Fix16::Round_To_Int_410BF0(Fix16(33075) * (speed / max_speed)) + 11025;
                    }
                }
                else
                {
                    if (speed > max_speed)
                    {
                        speed = max_speed;
                    }

                    if (pCar->IsTank_411900() || pCar->field_58_physics->field_94_is_backward_gas_on)
                    {
                        pSound->field_20_rate = Fix16::Round_To_Int_410BF0(Fix16(16000) * (speed / max_speed)) + 12000;
                    }
                    else if (pCar->field_68_scale != kFpOne_66F3F4)
                    {
                        pSound->field_20_rate = Fix16::Round_To_Int_410BF0(Fix16(19000) * (speed / max_speed)) + 11000;
                    }
                    else
                    {
                        Fix16 gear2_speed = gCarInfo_48_6FE258->field_40_gear2_speed;
                        Fix16 gear3_speed = gCarInfo_48_6FE258->field_44_gear3_speed;
                        if (speed < gear2_speed)
                        {
                            if (pSound->field_58_type == 1)
                            {
                                pSound->field_20_rate = Fix16::Round_To_Int_410BF0(Fix16(16000) * (speed / gear2_speed)) + 12000;
                            }
                            else
                            {
                                pSound->field_20_rate = Fix16::Round_To_Int_410BF0(Fix16(33075) * (speed / max_speed)) + 11025;
                            }
                        }
                        else if (speed < gear3_speed)
                        {
                            pSound->field_20_rate = pSound->field_58_type == 1
                                ? Fix16::Round_To_Int_410BF0(Fix16(16000) * ((speed - gear2_speed) / (gear3_speed - gear2_speed))) + 14600
                                : Fix16::Round_To_Int_410BF0(Fix16(33075) * (speed / max_speed)) + 11025;
                        }
                        else
                        {
                            if (pSound->field_58_type == 1)
                            {
                                pSound->field_20_rate = Fix16::Round_To_Int_410BF0(
                                                        (speed - gear3_speed) / (max_speed - gear3_speed) * (Fix16(16000) * kFpHalf_66F258)) +
                                    16000;
                            }
                            else
                            {
                                pSound->field_20_rate = Fix16::Round_To_Int_410BF0(Fix16(33075) * (speed / max_speed)) + 11025;
                            }
                        }
                    }
                }
            }

            if (pCar->IsCarInAir_43A3C0())
            {
                pSound->field_20_rate <<= 1;
            }
        }

        if (pSound->field_58_type == 1)
        {
            if (pCar->field_68_scale != kFpOne_66F3F4)
            {
                pSound->field_14_samp_idx = 9;
            }
            else
            {
                pSound->field_14_samp_idx = samp_idx_for_model_417AC0(pCar->field_84_car_info_idx);
            }
            pSound->field_3C_speed_multiplier = 600;
        }
        else
        {
            pSound->field_14_samp_idx = 21;
            pSound->field_3C_speed_multiplier = 400;
        }

        pSound->field_4C_releasing_volume_divider = 3;
        pSound->field_30_loop_count = 0;
        // A local for the first index only: the original loads it into eax before `this` in ecx
        s32 idx = pSound->field_14_samp_idx;
        pSound->field_34_loop_start = gSampManager_6FFF00.GetLoopStart_58DC30(idx);
        pSound->field_38_loop_end = gSampManager_6FFF00.GetLoopEnd_58DC50(pSound->field_14_samp_idx);
        return true;
    }
    return false;
}

MATCH_FUNC(0x417060)
void sound_obj::HandleCarAlarmSound_417060(Sound_Params* pParams)
{
    if (pParams->field_0_pObj->field_8_car_bc_ptr->field_8E_flash_count)
    {
        if (CalculateDistance_419020(Fix16(2560000, 0)))
        {
            if (VolCalc_419070(0x32u, Fix16(204800, 0), pParams->field_5_bHasSolidAbove))
            {
                this->field_30_sQueueSample.field_54_sound_intensity = Fix16(204800, 0);
                this->field_30_sQueueSample.field_60_nEmittingVolume = 50;
                this->field_30_sQueueSample.field_64_max_distance = 25;
                this->field_30_sQueueSample.field_58_type = 3;
                this->field_30_sQueueSample.field_4_SampleIndex = 6;
                this->field_30_sQueueSample.field_41_releasing_sound_flag = 0;
                this->field_30_sQueueSample.field_18_bIs2D = false;
                if (pParams->field_4_bDrivenByPlayer)
                {
                    this->field_30_sQueueSample.field_1C_ReleasingVolumeModificator = 3;
                }
                else
                {
                    this->field_30_sQueueSample.field_1C_ReleasingVolumeModificator = 5;
                }
                AddSampleToRequestedQueue_41A850();
            }
        }
    }
}

MATCH_FUNC(0x4174C0)
bool sound_obj::Type_3_InitCarAlarmSound_4174C0(sound_0x68* pSound)
{
    Car* pCar;
    if (GetCar_4145E0(pSound->field_0_EntityIndex, &pCar))
    {
        s32 car_model = pCar->field_84_car_info_idx;
        // The model picks one of four alarm tones and shifts its pitch
        switch (car_model & 3)
        {
            default:
                pSound->field_14_samp_idx = 13;
                pSound->field_20_rate = gSampManager_6FFF00.GetPlayBackRate_58DBF0(13) + 30 * car_model;
                break;

            case 2:
                pSound->field_14_samp_idx = 12;
                pSound->field_20_rate = 29 * car_model + gSampManager_6FFF00.GetPlayBackRate_58DBF0(12);
                break;

            case 1:
                pSound->field_14_samp_idx = 13;
                pSound->field_20_rate = 2 * (gSampManager_6FFF00.GetPlayBackRate_58DBF0(13) - 29 * car_model);
                break;

            case 0:
                pSound->field_14_samp_idx = 12;
                pSound->field_20_rate = 2 * (gSampManager_6FFF00.GetPlayBackRate_58DBF0(12) - 25 * car_model);
                break;
        }
        pSound->field_3C_speed_multiplier = 800;
        pSound->field_4C_releasing_volume_divider = 3;
        pSound->field_30_loop_count = 0;
        pSound->field_34_loop_start = gSampManager_6FFF00.GetLoopStart_58DC30(pSound->field_14_samp_idx);
        pSound->field_38_loop_end = gSampManager_6FFF00.GetLoopEnd_58DC50(pSound->field_14_samp_idx);
        return true;
    }
    return false;
}

MATCH_FUNC(0x4177D0)
void sound_obj::HandleCarDamageSound_4177D0(Sound_Params* pParams)
{
    Car* pCar = pParams->field_0_pObj->field_8_car_bc_ptr;
    if (pCar->IsEngineOn_4118C0() && pCar->field_74_damage > 16000)
    {
        if (CalculateDistance_419020(Fix16(49)))
        {
            u8 vol = 60 * (pParams->field_0_pObj->field_8_car_bc_ptr->field_74_damage - 16000) / 16000;
            if (VolCalc_419070(vol,
                               Fix16(7),
                               pParams->field_5_bHasSolidAbove))
            {
                this->field_30_sQueueSample.field_54_sound_intensity = Fix16(7);
                this->field_30_sQueueSample.field_60_nEmittingVolume = vol;
                this->field_30_sQueueSample.field_64_max_distance = 14;
                this->field_30_sQueueSample.field_58_type = 6;
                this->field_30_sQueueSample.field_4_SampleIndex = 1;
                this->field_30_sQueueSample.field_41_releasing_sound_flag = 0;
                this->field_30_sQueueSample.field_18_bIs2D = false;
                this->field_30_sQueueSample.field_1C_ReleasingVolumeModificator = 7;
                AddSampleToRequestedQueue_41A850();
            }
        }
    }
}

MATCH_FUNC(0x4178C0)
void sound_obj::HandleSirenActivationSound_4178C0(Sound_Params* pParams)
{
    Car* pCar = pParams->field_0_pObj->field_8_car_bc_ptr;
    if ((pCar->HasEmergencyInfoFlag() || pCar->IsFbiCar_411920()) && (pCar->field_A4_bf.bEmergencyLights))
    {
        Car* pCar_ = pParams->field_0_pObj->field_8_car_bc_ptr;
        if (pCar_->field_9C_engine_status == car_engine_status::on_3)
        {
            if (pCar_->field_54_driver)
            {
                if (CalculateDistance_419020(Fix16(400)))
                {
                    if (VolCalc_419070(0x46u, Fix16(20), pParams->field_5_bHasSolidAbove))
                    {
                        this->field_30_sQueueSample.field_54_sound_intensity = Fix16(20);
                        this->field_30_sQueueSample.field_60_nEmittingVolume = 70;
                        this->field_30_sQueueSample.field_64_max_distance = 40;
                        this->field_30_sQueueSample.field_58_type = 4;
                        this->field_30_sQueueSample.field_4_SampleIndex = 7;
                        this->field_30_sQueueSample.field_41_releasing_sound_flag = 0;
                        this->field_30_sQueueSample.field_18_bIs2D = false;
                        if (pParams->field_4_bDrivenByPlayer)
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

MATCH_FUNC(0x417A00)
bool sound_obj::Type_4_InitSirenSound_417A00(sound_0x68* pSound)
{
    Car* pCar;
    if (GetCar_4145E0(pSound->field_0_EntityIndex, &pCar))
    {
        s32 model = pCar->GetCarInfoIdx_411940();
        pSound->field_14_samp_idx = GetSirenSampleIdx_417B80(pCar->GetCarInfoIdx_411940(), pCar->IsEmittingHorn_411970());
        pSound->field_3C_speed_multiplier = 800;
        pSound->field_4C_releasing_volume_divider = 2;

        pSound->field_20_rate = gSampManager_6FFF00.GetPlayBackRate_58DBF0(pSound->field_14_samp_idx);
        if (model == car_model_enum::FIRETRUK || model == car_model_enum::MEDICAR)
        {
            pSound->field_20_rate = 9 * pSound->field_20_rate / 10u;
        }
        pSound->field_30_loop_count = 0;
        pSound->field_34_loop_start = gSampManager_6FFF00.GetLoopStart_58DC30(pSound->field_14_samp_idx);
        pSound->field_38_loop_end = gSampManager_6FFF00.GetLoopEnd_58DC50(pSound->field_14_samp_idx);
        return true;
    }
    return false;
}

MATCH_FUNC(0x417AC0)
s32 sound_obj::samp_idx_for_model_417AC0(s32 car_model) const
{
    s32 result;

    switch (car_model)
    {
        case car_model_enum::bug:
        case car_model_enum::DART:
        case car_model_enum::FIAT:
        case car_model_enum::ISETTA:
        case car_model_enum::MESSER:
        case car_model_enum::MORRIS:
        case car_model_enum::STYPECAB:
            result = 4;
            break;

        case car_model_enum::amdb4:
        case car_model_enum::GT24640:
        case car_model_enum::GUNJEEP:
        case car_model_enum::JEEP:
        case car_model_enum::MERC:
        case car_model_enum::MIURA:
        case car_model_enum::SPIDER:
        case car_model_enum::STRATOS:
        case car_model_enum::STRATOSB:
        case car_model_enum::TRANCEAM:
        case car_model_enum::VTYPE:
        case car_model_enum::XK120:
            result = 5;
            break;

        case car_model_enum::allard:
        case car_model_enum::bmw:
        case car_model_enum::COPCAR:
        case car_model_enum::EDSEL:
        case car_model_enum::JEFFREY:
        case car_model_enum::STINGRAY:
        case car_model_enum::STRIPETB:
        case car_model_enum::T2000GT:
        case car_model_enum::TBIRD:
        case car_model_enum::WBTWIN:
        case car_model_enum::ZCX5:
        case car_model_enum::EDSELFBI:
            result = 6;
            break;

        case car_model_enum::bank_van:
        case car_model_enum::HOTDOG:
        case car_model_enum::HOTDOG_D1:
        case car_model_enum::HOTDOG_D2:
        case car_model_enum::HOTDOG_D3:
        case car_model_enum::ICECREAM:
        case car_model_enum::PICKUP:
        case car_model_enum::SWATVAN:
        case car_model_enum::TVVAN:
        case car_model_enum::VAN:
        case car_model_enum::VESPA:
        case car_model_enum::HOTDOG_D4:
            result = 7;
            break;

        case car_model_enum::apc:
        case car_model_enum::boxtruck:
        case car_model_enum::BUS:
        case car_model_enum::FIRETRUK:
        case car_model_enum::GTRUCK:
        case car_model_enum::MONSTER:
        case car_model_enum::TANK:
        case car_model_enum::TRUKCAB1:
        case car_model_enum::TRUKCAB2:
        case car_model_enum::KRSNABUS:
            result = 8;
            break;

        default:
            result = 3;
            break;
    }
    return result;
}

MATCH_FUNC(0x417B80)
s32 sound_obj::GetSirenSampleIdx_417B80(s32 car_model, bool bHornOn) const
{
    if (car_model != car_model_enum::FIRETRUK)
    {
        return (bHornOn != 0) + 14;
    }
    return 14;
}

MATCH_FUNC(0x417BA0)
s32 sound_obj::GetVehicleAudioClass_417BA0(s32 car_model) const
{
    s32 result;
    switch (car_model)
    {
        case car_model_enum::bug:
        case car_model_enum::DART:
        case car_model_enum::FIAT:
        case car_model_enum::ISETTA:
        case car_model_enum::MESSER:
        case car_model_enum::MORRIS:
            result = 17;
            break;

        case car_model_enum::allard:
        case car_model_enum::amdb4:
        case car_model_enum::bmw:
        case car_model_enum::GT24640:
        case car_model_enum::GUNJEEP:
        case car_model_enum::JEEP:
        case car_model_enum::JEFFREY:
        case car_model_enum::MONSTER:
        case car_model_enum::SPIDER:
        case car_model_enum::STINGRAY:
        case car_model_enum::STRIPETB:
        case car_model_enum::T2000GT:
        case car_model_enum::TBIRD:
        case car_model_enum::WBTWIN:
        case car_model_enum::ZCX5:
            result = 18;
            break;

        case car_model_enum::apc:
        case car_model_enum::boxtruck:
        case car_model_enum::BUS:
        case car_model_enum::FIRETRUK:
        case car_model_enum::GTRUCK:
        case car_model_enum::TANK:
        case car_model_enum::TRUKCAB1:
        case car_model_enum::TRUKCAB2:
        case car_model_enum::KRSNABUS:
            result = 19;
            break;

        case car_model_enum::TRAINCAB:
            result = 20;
            break;

        case car_model_enum::ICECREAM:
            result = 23;
            break;

        default:
            result = 16;
            break;
    }
    return result;
}

MATCH_FUNC(0x417C60)
s32 sound_obj::ConvertToPlayBackRate_417C60(s32 car_model) const
{
    switch (car_model)
    {
        case car_model_enum::DART:
        case car_model_enum::MESSER:
            return 22000;
        case car_model_enum::amdb4:
        case car_model_enum::bug:
        case car_model_enum::GT24640:
        case car_model_enum::MORRIS:
        case car_model_enum::T2000GT:
        case car_model_enum::WBTWIN:
            return 18000;
        case car_model_enum::BUICK:
        case car_model_enum::FIAT:
        case car_model_enum::HOTDOG:
        case car_model_enum::HOTDOG_D1:
        case car_model_enum::HOTDOG_D2:
        case car_model_enum::HOTDOG_D3:
        case car_model_enum::ISETLIMO:
        case car_model_enum::ISETTA:
        case car_model_enum::MIURA:
        case car_model_enum::RTYPE:
        case car_model_enum::TRANCEAM:
        case car_model_enum::TVVAN:
        case car_model_enum::HOTDOG_D4:
            return 24000;
        case car_model_enum::allard:
        case car_model_enum::GUNJEEP:
        case car_model_enum::JEEP:
        case car_model_enum::STINGRAY:
        case car_model_enum::STRIPETB:
        case car_model_enum::ZCX5:
            return 16000;
        case car_model_enum::bmw:
        case car_model_enum::JEFFREY:
        case car_model_enum::MONSTER:
        case car_model_enum::TBIRD:
            return 14000;
        case car_model_enum::apc:
        case car_model_enum::BUS:
        case car_model_enum::TANK:
            return 11000;
        case car_model_enum::FIRETRUK:
        case car_model_enum::GTRUCK:
        case car_model_enum::TRUKCAB1:
            return 10300;
        case car_model_enum::boxtruck:
        case car_model_enum::TRUKCAB2:
        case car_model_enum::KRSNABUS:
            return 11700;
        case car_model_enum::TRAINCAB:
            return 15000;
        case car_model_enum::ICECREAM:
            return 11025;
        case car_model_enum::bank_van:
        case car_model_enum::LIMO:
        case car_model_enum::MERC:
        case car_model_enum::PICKUP:
        case car_model_enum::SPIDER:
        case car_model_enum::STYPE:
        case car_model_enum::TAXI:
        case car_model_enum::VTYPE:
        case car_model_enum::XK120:
            return 19000;
        default:
            return 22050;
    }
}

MATCH_FUNC(0x417D70)
s32 sound_obj::get_samp_idx_for_car_417D70(Car* pCar, bool bDoorOpening, bool bLargeDoorOffset) const
{
    s32 result;

    if (bLargeDoorOffset)
    {
        return 31;
    }

    switch (pCar->GetCarInfoIdx_411940())
    {
        case car_model_enum::apc:
        case car_model_enum::boxtruck:
        case car_model_enum::FIRETRUK:
        case car_model_enum::GTRUCK:
        case car_model_enum::SWATVAN:
        case car_model_enum::TANK:
        case car_model_enum::TOWTRUCK:
        case car_model_enum::TRUKCAB1:
        case car_model_enum::TRUKCAB2:
        case car_model_enum::KRSNABUS:
            result = bDoorOpening ? 27 : 28;
            break;

        case car_model_enum::boxcar:
        case car_model_enum::BUS:
        case car_model_enum::TRAIN:
        case car_model_enum::TRAINCAB:
        case car_model_enum::TRAINFB:
            result = 29;
            break;

        default:
            result = bDoorOpening ? 25 : 26;
            break;
    }
    return result;
}

MATCH_FUNC(0x417E30)
void sound_obj::HandleHeavyVehicleStopSound_417E30(Sound_Params* pParams, SoundPosEntry* pAlloc)
{
    Car* pCar = pParams->field_0_pObj->field_8_car_bc_ptr;
    Fix16 car_speed;
    car_speed = pCar->GetCarLinearSpeed_43A240();
    if (pAlloc->field_0_last_speed > kFpZero_66F3F0 && car_speed == kFpZero_66F3F0)
    {
        if (IsHeavyTruckOrBus_417F40(pCar->GetCarInfoIdx_411940()))
        {
            if (CalculateDistance_419020(Fix16(25)))
            {
                if (VolCalc_419070(0x28u, Fix16(5), pParams->field_5_bHasSolidAbove))
                {
                    this->field_30_sQueueSample.field_54_sound_intensity = Fix16(5);
                    this->field_30_sQueueSample.field_60_nEmittingVolume = 40;
                    this->field_30_sQueueSample.field_64_max_distance = 10;
                    this->field_30_sQueueSample.field_58_type = 7;
                    this->field_30_sQueueSample.field_4_SampleIndex = 2;
                    this->field_30_sQueueSample.field_41_releasing_sound_flag = 1;
                    this->field_30_sQueueSample.field_18_bIs2D = false;
                    this->field_30_sQueueSample.field_1C_ReleasingVolumeModificator = 7;
                    AddSampleToRequestedQueue_41A850();
                }
            }
        }
    }
    pAlloc->field_0_last_speed = car_speed;
}

MATCH_FUNC(0x417EF0)
bool sound_obj::Type_7_InitHeavyVehicleStopSound_417EF0(sound_0x68* pObj)
{
    pObj->field_14_samp_idx = 29;
    pObj->field_3C_speed_multiplier = 400;
    pObj->field_20_rate = RandomDisplacement_41A650(29) + gSampManager_6FFF00.GetPlayBackRate_58DBF0(29);
    pObj->field_30_loop_count = 1;
    pObj->field_34_loop_start = 0;
    pObj->field_38_loop_end = -1;
    return true;
}

MATCH_FUNC(0x417F40)
bool sound_obj::IsHeavyTruckOrBus_417F40(s32 car_model) const
{
    switch (car_model)
    {
        case car_model_enum::apc:
        case car_model_enum::boxtruck:
        case car_model_enum::BUS:
        case car_model_enum::FIRETRUK:
        case car_model_enum::GTRUCK:
        case car_model_enum::TOWTRUCK:
        case car_model_enum::TRUKCAB1:
        case car_model_enum::TRUKCAB2:
        case car_model_enum::KRSNABUS:
            return true;
        default:
            return false;
    }
}

MATCH_FUNC(0x417FD0)
void sound_obj::HandleTruckCorneringAudio_417FD0(Sound_Params* pParams)
{
    Car* pCar = pParams->field_0_pObj->field_8_car_bc_ptr;
    CarPhysics* pPhysics = pCar->field_58_physics;

    Ang16 slide_angle = pPhysics->field_40_linvel_1.atan2_40F790().SubtractNormalized_409340(pPhysics->field_58_theta);
    if (slide_angle > kMinCorneringAngle_66F274 && slide_angle < kMaxCorneringAngle_66F370)
    {
        if (pCar->GetCarLinearSpeed_43A240() > kMinCorneringSpeed)
        {
            switch (pParams->field_0_pObj->field_8_car_bc_ptr->field_84_car_info_idx)
            {
                case car_model_enum::boxtruck:
                case car_model_enum::BUS:
                case car_model_enum::FIRETRUK:
                case car_model_enum::GTRUCK:
                case car_model_enum::TOWTRUCK:
                case car_model_enum::TRUKCAB1:
                case car_model_enum::TRUKCAB2:
                case car_model_enum::KRSNABUS: // 10.5.exe's index table: the bus too
                    if (CalculateDistance_419020(Fix16(25)))
                    {
                        if (VolCalc_419070(25, Fix16(5), pParams->field_5_bHasSolidAbove))
                        {
                            this->field_30_sQueueSample.field_54_sound_intensity = Fix16(5);
                            this->field_30_sQueueSample.field_60_nEmittingVolume = 25;
                            this->field_30_sQueueSample.field_64_max_distance = 10;
                            this->field_30_sQueueSample.field_58_type = 8;
                            this->field_30_sQueueSample.field_4_SampleIndex = 11;
                            this->field_30_sQueueSample.field_41_releasing_sound_flag = 0;
                            this->field_30_sQueueSample.field_1C_ReleasingVolumeModificator = 7;
                            this->field_30_sQueueSample.field_18_bIs2D = false;
                            AddSampleToRequestedQueue_41A850();
                        }
                    }
                    break;
            }
        }
    }
}

MATCH_FUNC(0x418130)
bool sound_obj::Type_8_InitTruckCorneringSound_418130(sound_0x68* pObj)
{
    pObj->field_14_samp_idx = 24;
    pObj->field_3C_speed_multiplier = 400;
    pObj->field_4C_releasing_volume_divider = 2;
    pObj->field_20_rate = gSampManager_6FFF00.GetPlayBackRate_58DBF0(24);
    pObj->field_30_loop_count = 0;
    pObj->field_34_loop_start = gSampManager_6FFF00.GetLoopStart_58DC30(pObj->field_14_samp_idx);
    pObj->field_38_loop_end = gSampManager_6FFF00.GetLoopEnd_58DC50(pObj->field_14_samp_idx);
    return true;
}

MATCH_FUNC(0x418190)
void sound_obj::HandleAICarEngineSound_418190(Sound_Params* pParams)
{
    if (!pParams->field_4_bDrivenByPlayer)
    {
        Fix16 max_speed = gCarInfo_48_6FE258->field_28_max_speed;
        Fix16 speed;
        speed = pParams->field_0_pObj->field_8_car_bc_ptr->GetCarLinearSpeed_43A240();
        if (max_speed > kFpZero_66F3F0 && speed > kFpZero_66F3F0 && CalculateDistance_419020(Fix16(81)))
        {
            u8 emitting_vol = Fix16::Round_To_Int_410BF0((speed / max_speed) * Fix16(11));
            if (emitting_vol > 0)
            {
                if (VolCalc_419070(emitting_vol, Fix16(9), pParams->field_5_bHasSolidAbove))
                {
                    this->field_30_sQueueSample.field_54_sound_intensity = Fix16(9);
                    this->field_30_sQueueSample.field_60_nEmittingVolume = emitting_vol;
                    this->field_30_sQueueSample.field_64_max_distance = 18;
                    this->field_30_sQueueSample.field_58_type = 2;
                    this->field_30_sQueueSample.field_4_SampleIndex = 2;
                    this->field_30_sQueueSample.field_41_releasing_sound_flag = 0;
                    this->field_30_sQueueSample.field_18_bIs2D = false;
                    this->field_30_sQueueSample.field_1C_ReleasingVolumeModificator = 17;
                    AddSampleToRequestedQueue_41A850();
                }
            }
        }
    }
}

MATCH_FUNC(0x4182A0)
bool sound_obj::Type_2_InitAICarEngineSound_4182A0(sound_0x68* pObj)
{
    pObj->field_14_samp_idx = 0;
    pObj->field_3C_speed_multiplier = 700;
    pObj->field_30_loop_count = 0;
    pObj->field_34_loop_start = 0;
    pObj->field_38_loop_end = -1;
    pObj->field_20_rate = gSampManager_6FFF00.GetPlayBackRate_58DBF0(0);
    pObj->field_4C_releasing_volume_divider = 5;
    return true;
}

MATCH_FUNC(0x4182E0)
void sound_obj::HandleCarDoorSounds_4182E0(Sound_Params* pParams)
{
    bool bQueued1 = false;
    bool bQueued2 = false;
    Car* pCar = pParams->field_0_pObj->field_8_car_bc_ptr;
    s32 model = pCar->GetCarInfoIdx_411940();
    u8* pDoorData = gGtx_0x106C_703DD4->get_car_door_data_5AA3D0(model);
    for (u8 i = 0; i < static_cast<u8>(pCar->GetDoorCount_43B360()); ++i)
    {
        bool bLargeDoorOffset = false;
        s8 door_rx = pDoorData[2 * i + 1];
        if ((door_rx > 64 || door_rx < -64) && model != car_model_enum::TRAIN && model != car_model_enum::TRAINCAB &&
            model != car_model_enum::TRAINFB && model != car_model_enum::boxcar && model != car_model_enum::BUS)
        {
            bLargeDoorOffset = true;
        }

        if (!bQueued1 && pCar->field_C_doors[i].IsStartingToOpen())
        {
            bQueued1 = true;
            if (CalculateDistance_419020(Fix16(0xE1000, 0)))
            {
                if (VolCalc_419070(0x32u, Fix16(0x1E000, 0), pParams->field_5_bHasSolidAbove))
                {
                    this->field_30_sQueueSample.field_54_sound_intensity = Fix16(0x1E000, 0);
                    this->field_30_sQueueSample.field_60_nEmittingVolume = 50;
                    this->field_30_sQueueSample.field_64_max_distance = 15;
                    this->field_30_sQueueSample.field_58_type = 20;
                    this->field_30_sQueueSample.field_4_SampleIndex = 9;
                    this->field_30_sQueueSample.field_41_releasing_sound_flag = 1;
                    this->field_30_sQueueSample.field_18_bIs2D = false;
                    if (pParams->field_4_bDrivenByPlayer)
                    {
                        this->field_30_sQueueSample.field_1C_ReleasingVolumeModificator = 3;
                    }
                    else
                    {
                        this->field_30_sQueueSample.field_1C_ReleasingVolumeModificator = 5;
                    }
                    this->field_30_sQueueSample.field_14_samp_idx = get_samp_idx_for_car_417D70(pCar, true, bLargeDoorOffset);
                    this->field_30_sQueueSample.field_3C_speed_multiplier = 0;
                    this->field_30_sQueueSample.field_20_rate =
                        RandomDisplacement_41A650(this->field_30_sQueueSample.field_14_samp_idx) +
                        gSampManager_6FFF00.GetPlayBackRate_58DBF0(this->field_30_sQueueSample.field_14_samp_idx);
                    this->field_30_sQueueSample.field_30_loop_count = 1;
                    this->field_30_sQueueSample.field_34_loop_start = 0;
                    this->field_30_sQueueSample.field_38_loop_end = -1;
                    AddSampleToRequestedQueue_41A850();
                }
            }
        }

        if (!bQueued2)
        {
            if (pCar->field_C_doors[i].IsStartingToClose())
            {
                bQueued2 = true;
                if (CalculateDistance_419020(Fix16(0xE1000, 0)))
                {
                    if (VolCalc_419070(0x32u, Fix16(0x1E000, 0), pParams->field_5_bHasSolidAbove))
                    {
                        this->field_30_sQueueSample.field_54_sound_intensity = Fix16(0x1E000, 0);
                        this->field_30_sQueueSample.field_60_nEmittingVolume = 50;
                        this->field_30_sQueueSample.field_64_max_distance = 15;
                        this->field_30_sQueueSample.field_58_type = 20;
                        this->field_30_sQueueSample.field_4_SampleIndex = 10;
                        this->field_30_sQueueSample.field_41_releasing_sound_flag = 0;
                        this->field_30_sQueueSample.field_18_bIs2D = false;
                        if (pParams->field_4_bDrivenByPlayer)
                        {
                            this->field_30_sQueueSample.field_1C_ReleasingVolumeModificator = 3;
                        }
                        else
                        {
                            this->field_30_sQueueSample.field_1C_ReleasingVolumeModificator = 5;
                        }
                        this->field_30_sQueueSample.field_14_samp_idx = get_samp_idx_for_car_417D70(pCar, false, bLargeDoorOffset);
                        this->field_30_sQueueSample.field_3C_speed_multiplier = 0;
                        this->field_30_sQueueSample.field_20_rate =
                            RandomDisplacement_41A650(this->field_30_sQueueSample.field_14_samp_idx) +
                            gSampManager_6FFF00.GetPlayBackRate_58DBF0(this->field_30_sQueueSample.field_14_samp_idx);
                        this->field_30_sQueueSample.field_30_loop_count = 1;
                        this->field_30_sQueueSample.field_34_loop_start = 0;
                        this->field_30_sQueueSample.field_38_loop_end = -1;
                        AddSampleToRequestedQueue_41A850();
                    }
                }
            }
        }
    }
}

MATCH_FUNC(0x418610)
void sound_obj::HandleCarEngineStartSound_418610(Sound_Params* pParams)
{
    if (pParams->field_0_pObj->field_8_car_bc_ptr->field_9C_engine_status == car_engine_status::starting_4)
    {
        if (CalculateDistance_419020(Fix16(25)))
        {
            if (VolCalc_419070(0x46u, Fix16(5), pParams->field_5_bHasSolidAbove))
            {
                this->field_30_sQueueSample.field_54_sound_intensity = Fix16(5);
                this->field_30_sQueueSample.field_60_nEmittingVolume = 70;
                this->field_30_sQueueSample.field_64_max_distance = 10;
                this->field_30_sQueueSample.field_58_type = 9;
                this->field_30_sQueueSample.field_4_SampleIndex = 12;
                this->field_30_sQueueSample.field_41_releasing_sound_flag = 1;
                this->field_30_sQueueSample.field_1C_ReleasingVolumeModificator = 2;
                this->field_30_sQueueSample.field_18_bIs2D = false;
                if (pParams->field_0_pObj->field_8_car_bc_ptr->field_84_car_info_idx % 2)
                {
                    this->field_30_sQueueSample.field_14_samp_idx = 1;
                }
                else
                {
                    this->field_30_sQueueSample.field_14_samp_idx = 2;
                }
                AddSampleToRequestedQueue_41A850();
            }
        }
    }
}

MATCH_FUNC(0x4186D0)
bool sound_obj::Type_9_InitCarEngineStartSound_4186D0(sound_0x68* pObj)
{
    pObj->field_3C_speed_multiplier = 600;
    pObj->field_30_loop_count = 1;
    pObj->field_34_loop_start = 0;
    pObj->field_38_loop_end = -1;
    pObj->field_20_rate =
        RandomDisplacement_41A650(pObj->field_14_samp_idx) + gSampManager_6FFF00.GetPlayBackRate_58DBF0(pObj->field_14_samp_idx);
    return true;
}

MATCH_FUNC(0x418720)
void sound_obj::HandleCarTireScrubSound_418720(Sound_Params* pParams)
{
    u8 emitVol = 0;

    Car* pCar = pParams->field_0_pObj->field_8_car_bc_ptr;
    CarPhysics* pPhysics = pCar->field_58_physics;

    if (pPhysics)
    {
        s32 block_spec = pPhysics->field_9C_block_spec;
        if (block_spec && block_spec != 4)
        {
            if (pPhysics->field_AC_drive_wheels_locked_q > 0)
            {
                if (gCarInfo_48_6FE258->field_28_max_speed > kFpZero_66F3F0)
                {
                    // The copy through a pointer to the returned temporary is folded into the /= below, so the
                    // speed is read through the returned pointer after the divisor's load, like the original.
                    // speed has to be declared before pSpeed.
                    Fix16 speed;
                    Fix16* pSpeed = &pCar->GetCarLinearSpeed_43A240();
                    speed = *pSpeed;
                    speed /= gCarInfo_48_6FE258->field_28_max_speed;
                    emitVol = Fix16::Round_To_Int_410BF0(Fix16(60) * speed) + 10;
                }
                else
                {
                    emitVol = 10;
                }
            }
            else
            {
                pPhysics->SetCarInfoGlobal_562ED0();
                if (pPhysics->field_84_front_skid > gCarInfo_2C_6FE0E4->field_24_skid_threshhold_1)
                {
                    Fix16 front_scrub = (pPhysics->field_84_front_skid - gCarInfo_2C_6FE0E4->field_24_skid_threshhold_1) / gCarInfo_2C_6FE0E4->field_24_skid_threshhold_1;
                    if (front_scrub > kFpOne_66F3F4)
                    {
                        front_scrub = kFpOne_66F3F4;
                    }
                    u8 frontVol = Fix16::Round_To_Int_410BF0(Fix16(70) * front_scrub);
                    if (frontVol > emitVol)
                    {
                        emitVol = frontVol;
                    }
                }

                if (pPhysics->field_88_rear_skid > gCarInfo_2C_6FE0E4->field_28_skid_threshhold_2)
                {
                    Fix16 rear_scrub = (pPhysics->field_88_rear_skid - gCarInfo_2C_6FE0E4->field_28_skid_threshhold_2) / gCarInfo_2C_6FE0E4->field_28_skid_threshhold_2;
                    if (rear_scrub > kFpOne_66F3F4)
                    {
                        rear_scrub = kFpOne_66F3F4;
                    }
                    u8 rearVol = Fix16::Round_To_Int_410BF0(Fix16(70) * rear_scrub);
                    if (rearVol > emitVol)
                    {
                        emitVol = rearVol;
                    }
                }
            }

            if (emitVol > 0)
            {
                if (CalculateDistance_419020(Fix16(25)))
                {
                    if (VolCalc_419070(emitVol, Fix16(5), pParams->field_5_bHasSolidAbove))
                    {
                        this->field_30_sQueueSample.field_54_sound_intensity = Fix16(5);
                        this->field_30_sQueueSample.field_60_nEmittingVolume = emitVol;
                        this->field_30_sQueueSample.field_64_max_distance = 10;
                        this->field_30_sQueueSample.field_58_type = 10;
                        this->field_30_sQueueSample.field_4_SampleIndex = 13;
                        this->field_30_sQueueSample.field_41_releasing_sound_flag = 0;
                        this->field_30_sQueueSample.field_1C_ReleasingVolumeModificator = 5;
                        this->field_30_sQueueSample.field_18_bIs2D = false;
                        AddSampleToRequestedQueue_41A850();
                    }
                }
            }
        }
    }
}

MATCH_FUNC(0x418940)
bool sound_obj::Type_10_HandleCarSkidSound_418940(sound_0x68* pSound)
{
    Fix16 front_skid = kFpZero_66F3F0;
    Fix16 rear_skid = kFpZero_66F3F0;

    Car* pCar;
    if (GetCar_4145E0(pSound->field_0_EntityIndex, &pCar))
    {
        CarPhysics* pPhysics = pCar->field_58_physics;
        if (pPhysics)
        {
            pSound->field_14_samp_idx = 22;
            s32 rate;
            switch (pPhysics->field_9C_block_spec)
            {
                case 5:
                case 6:
                case 8:
                case 9:
                    rate = 18000;
                    break;
                case 2:
                    rate = 13000;
                    break;
                case 7:
                    rate = 17000;
                    break;
                case 10:
                    rate = 12600;
                    break;
                default:
                    rate = 16000;
                    break;
            }

            pPhysics->SetCarInfoGlobal_562ED0();
            pPhysics->SetModelPhysicsGlobal_562EB0();

            Fix16 skid_amount;

            if (pPhysics->field_AC_drive_wheels_locked_q > 0)
            {
                if (gCarInfo_48_6FE258->field_28_max_speed > kFpZero_66F3F0)
                {
                    // Written out: with `GetCarLinearSpeed() / gCarInfo->field_28` VC6 binds the divisor's
                    // reference before the call (`lea 0x28(%ecx)` held across it); the original reloads
                    // gCarInfo_48 after the shift, which only the raw expression gives.
                    skid_amount = Fix16(static_cast<s32>((static_cast<__int64>(pCar->GetCarLinearSpeed_43A240().mValue) << 14) / gCarInfo_48_6FE258->field_28_max_speed.mValue), 0);
                    pSound->field_20_rate = rate + Fix16::Round_To_Int_410BF0(Fix16(6000) * skid_amount);
                }
                else
                {
                    pSound->field_20_rate = rate;
                }
            }
            else
            {
                if (pPhysics->field_84_front_skid > gCarInfo_2C_6FE0E4->field_24_skid_threshhold_1)
                {
                    front_skid = (pPhysics->field_84_front_skid - gCarInfo_2C_6FE0E4->field_24_skid_threshhold_1) /
                        gCarInfo_2C_6FE0E4->field_24_skid_threshhold_1;
                    if (front_skid > kFpOne_66F3F4)
                    {
                        front_skid = kFpOne_66F3F4;
                    }
                }

                if (pPhysics->field_88_rear_skid > gCarInfo_2C_6FE0E4->field_28_skid_threshhold_2)
                {
                    rear_skid = (pPhysics->field_88_rear_skid - gCarInfo_2C_6FE0E4->field_28_skid_threshhold_2) /
                        gCarInfo_2C_6FE0E4->field_28_skid_threshhold_2;
                    if (rear_skid > kFpOne_66F3F4)
                    {
                        rear_skid = kFpOne_66F3F4;
                    }
                }

                skid_amount = rear_skid;

                if (front_skid > skid_amount)
                {
                    skid_amount = front_skid;
                }

                pSound->field_20_rate = rate + Fix16::Round_To_Int_410BF0(Fix16(6000) * skid_amount);
            }

            pSound->field_3C_speed_multiplier = 600;
            pSound->field_30_loop_count = 0;
            pSound->field_34_loop_start = gSampManager_6FFF00.GetLoopStart_58DC30(pSound->field_14_samp_idx);
            pSound->field_38_loop_end = gSampManager_6FFF00.GetLoopEnd_58DC50(pSound->field_14_samp_idx);
            pSound->field_4C_releasing_volume_divider = 3;
            return true;
        }
    }
    return false;
}

MATCH_FUNC(0x418B60)
void sound_obj::ProcessType11_HudPager_418B60(s32 entity_idx)
{
    Hud_Pager* pPager = field_147C_audio_entities[entity_idx].field_4_pObj->field_C_pAny.pHud_Pager_C;
    if (pPager)
    {
        if (pPager->get_timer_411A40() > 0)
        {
            u8 vol;
            if (pPager->get_timer_411A40() > 300)
            {
                field_30_sQueueSample.field_20_rate = 22050;
                vol = 40;
            }
            else if (pPager->get_timer_411A40() > 150)
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
            field_30_sQueueSample.field_0_EntityIndex = entity_idx;
            field_30_sQueueSample.field_5C_pReleaseSource = NULL;
            field_30_sQueueSample.field_24_nVolume = vol;
            field_30_sQueueSample.field_60_nEmittingVolume = vol;
            field_30_sQueueSample.field_64_max_distance = 100;
            field_30_sQueueSample.field_58_type = 20;
            field_30_sQueueSample.field_54_sound_intensity = Fix16(100);
            field_30_sQueueSample.field_4_SampleIndex = 0;
            field_30_sQueueSample.field_41_releasing_sound_flag = 0;
            field_30_sQueueSample.field_18_bIs2D = true;
            field_30_sQueueSample.field_1C_ReleasingVolumeModificator = 0;
            field_30_sQueueSample.field_3C_speed_multiplier = 0;
            field_30_sQueueSample.field_30_loop_count = 0;
            field_30_sQueueSample.field_34_loop_start = 0;
            field_30_sQueueSample.field_38_loop_end = -1;
            field_30_sQueueSample.field_40_pan = 63;
            field_30_sQueueSample.field_4C_releasing_volume_divider = 3;

            AddSampleToRequestedQueue_41A850();
        }
    }
}

MATCH_FUNC(0x418C20)
void sound_obj::AddVocalsEntity_418C20()
{
    if (!field_544C_vocals.field_4_entity_idx)
    {
        SoundObject* pSoundObj = &field_544C_vocals.field_8_sound_obj;
        field_544C_vocals.field_8_sound_obj.field_C_pAny.pAny = NULL;
        field_544C_vocals.field_8_sound_obj.field_4_bStatus = false;
        pSoundObj->field_0_object_type = SoundObjectTypeEnum::Vocals_10;
        field_544C_vocals.field_4_entity_idx = AddSoundObject_419FA0(pSoundObj);
    }
}

MATCH_FUNC(0x418C60)
void sound_obj::RemoveVocalsEntity_418C60()
{
    if (field_544C_vocals.field_4_entity_idx)
    {
        FreeSoundEntry_41A090(field_544C_vocals.field_4_entity_idx);
        field_544C_vocals.field_4_entity_idx = 0;
    }
}

MATCH_FUNC(0x418C80)
void sound_obj::PlayVocal_418C80(s32 voice)
{
    if (field_544C_vocals.field_4_entity_idx)
    {
        if (voice < 63)
        {
            field_544C_vocals.field_18_pending_voice = voice;
        }
    }
}

MATCH_FUNC(0x418CA0)
void sound_obj::ProcessType10_Vocals_418CA0()
{
    s32 voc_idx;
    if (field_544C_vocals.field_4_entity_idx)
    {
        switch (field_544C_vocals.field_18_pending_voice)
        {
            case voice_line::accuracyb_9:
                voc_idx = 0;
                break;
            case voice_line::back2front_10:
                voc_idx = 1;
                break;
            case voice_line::busted_17:
                voc_idx = 2;
                break;
            case voice_line::carjacker_7:
                voc_idx = 3;
                break;
            case voice_line::copkilla_6:
                voc_idx = 4;
                break;
            case voice_line::elvis_8:
                voc_idx = 5;
                break;
            case voice_line::expiditious_4:
                voc_idx = 6;
                break;
            case voice_line::frenzyfail_18:
                voc_idx = 8;
                break;
            case voice_line::frenzypassed_19:
                voc_idx = 9;
                break;
            case voice_line::fryingtonite_20:
                voc_idx = 10;
                break;
            case voice_line::genocide_5:
                voc_idx = 12;
                break;
            case voice_line::gta_2:
                voc_idx = 13;
                break;
            case voice_line::insanestunt_1:
                voc_idx = 14;
                break;
            case voice_line::jobcomplete_22:
                voc_idx = 15;
                break;
            case voice_line::jobfail_23:
                voc_idx = 16;
                break;
            case voice_line::killfrenzy_12:
                voc_idx = 17;
                break;
            case voice_line::medicalemer_11:
                voc_idx = 18;
                break;
            case voice_line::respectis_24:
                voc_idx = 19;
                break;
            case voice_line::shocking_25:
                voc_idx = 20;
                break;
            case voice_line::somethingscookin_26:
                voc_idx = 21;
                break;
            case voice_line::timesup_27:
                voc_idx = 22;
                break;
            case voice_line::toasted_28:
                voc_idx = 23;
                break;
            case voice_line::wasted_29:
                voc_idx = 24;
                break;
            case voice_line::wipeout_3:
                voc_idx = 25;
                break;
            case voice_line::bombarmed_30:
                voc_idx = 26;
                break;
            case voice_line::raceover_33:
                voc_idx = 55;
                break;
            case voice_line::second_lap_34:
                voc_idx = 56;
                break;
            case voice_line::finallap_35:
                voc_idx = 57;
                break;
            case voice_line::raceon_36:
                voc_idx = 58;
                break;
            case voice_line::people_down_30_37:
                voc_idx = 59;
                break;
            case voice_line::people_down_60_38:
                voc_idx = 60;
                break;
            case voice_line::people_down_90_39:
                voc_idx = 61;
                break;
            case voice_line::people_down_120_40:
                voc_idx = 62;
                break;
            case voice_line::people_down_150_41:
                voc_idx = 63;
                break;
            case voice_line::timeout_42:
                voc_idx = 64;
                break;
            case voice_line::timeextended_43:
                voc_idx = 65;
                break;
            case voice_line::timesup_44:
                voc_idx = 66;
                break;
            case voice_line::sorrydidthathurt_45:
                voc_idx = 67;
                break;
            case voice_line::nicework_46:
                voc_idx = 68;
                break;
            case voice_line::choctastic_47:
                voc_idx = 69;
                break;
            case voice_line::raspberryripple_48:
                voc_idx = 70;
                break;
            case voice_line::youshotyourload_49:
                voc_idx = 71;
                break;
            case voice_line::oohdidthathurt_50:
                voc_idx = 72;
                break;
            case voice_line::deathtoicvans_51:
                voc_idx = 73;
                break;
            case voice_line::crispycritter_52:
                voc_idx = 74;
                break;
            case voice_line::youretoastbuddy_53:
                voc_idx = 75;
                break;
            case voice_line::eatleaddeath_54:
                voc_idx = 76;
                break;
            case voice_line::thatsgottahurt_55:
                voc_idx = 77;
                break;
            case voice_line::sorryaboutthat_56:
                voc_idx = 78;
                break;
            case voice_line::xinloimyman_57:
                voc_idx = 79;
                break;
            case voice_line::damnsundaydrivers_58:
                voc_idx = 80;
                break;
            case voice_line::suckitandsee_59:
                voc_idx = 81;
                break;
            case voice_line::tastemywrath_60:
                voc_idx = 82;
                break;
            case voice_line::hallelujah_61:
                voc_idx = 83;
                break;
            case voice_line::damnation_62:
                voc_idx = 84;
                break;
            case voice_line::random_laugh_31:
            case voice_line::random_laugh_32:
                voc_idx = (field_1454_anRandomTable[0] % 13u) + 85;
                if (voc_idx == 99)
                {
                    field_544C_vocals.field_18_pending_voice = 0;
                    return;
                }
                break;
            default:
                field_544C_vocals.field_18_pending_voice = 0;
                return;
        }

        gSampManager_6FFF00.PlayVocal_58E510(1, voc_idx, true);
        gSampManager_6FFF00.SetVocalVolume_58E6D0(1, 127 * field_24_sfx_vol / 127);

        field_544C_vocals.field_18_pending_voice = 0;
    }
}

MATCH_FUNC(0x419020)
bool sound_obj::CalculateDistance_419020(Fix16 max_dist)
{
    if (field_28_dist_squared < max_dist)
    {
        if (!field_2C_distCalculated)
        {
            field_2C_distCalculated = true;
            field_30_sQueueSample.field_28_distance = Fix16::SquareRoot(field_28_dist_squared);
        }
        return true;
    }
    return false;
}

MATCH_FUNC(0x419070)
bool sound_obj::VolCalc_419070(u8 base_volume, Fix16 max_dist, bool bHasSolidAbove)
{
    u8 volume = CalcVolume_41A3F0(base_volume, max_dist, field_30_sQueueSample.field_28_distance);
    field_30_sQueueSample.field_24_nVolume = volume;
    if (bHasSolidAbove)
    {
        field_30_sQueueSample.field_24_nVolume = volume >> 1;
    }
    return field_30_sQueueSample.field_24_nVolume != 0 ? true : false;
}

MATCH_FUNC(0x4190B0)
Fix16 sound_obj::ComputeEmitterDistanceSquared_4190B0() const
{
    Fix16 dx = field_30_sQueueSample.field_8_obj.field_0_x - field_1468_listener_x;
    Fix16 dy = field_30_sQueueSample.field_8_obj.field_4_y - field_146C_listener_y;
    return dy * dy + dx * dx;
}

// The three Fix16 fields are set with the Fix16(s32) constructor, not operator=(s32): the
// operator's returned reference chains the four stores, which pushes the mov %esi,%ecx for
// GenerateIntegerRandomNumberTable_41BA90 behind them.
MATCH_FUNC(0x419CD0)
sound_obj::sound_obj()
{
    field_1474_rotation = 0;
    field_0_bSoundInitialized = false;
    field_4_speed_of_sound = 81920;
    field_8_frames_per_second = 30;
    field_C_doppler_divisor = 491520;
    field_2_service_enabled = true;
    field_24_sfx_vol = 127;
    field_25_cdVol = 127;
    field_28_dist_squared = kFpZero_674CD8;
    field_2C_distCalculated = false;

    field_98_nActiveSampleQueue = 1;
    ClearRequestedQueue_41B700();

    field_98_nActiveSampleQueue = 0;
    ClearRequestedQueue_41B700();

    ClearActivateSamples_41B7A0();

    field_1450_unused = 0;
    field_1468_listener_x = Fix16(0);
    field_146C_listener_y = Fix16(0);
    field_1470_listener_z = Fix16(0);

    GenerateIntegerRandomNumberTable_41BA90();

    field_1478_type5Idx = 0;
    field_1B_unused = 0;
    field_1C_samp_count = 0;
    field_10_nActiveSamples = 0;
    field_5444_unused = 0;
    field_5440_unused = 0;
    field_1D_b3d_sound = 0;
    field_14_sample_rate = 22050;
    field_18_unused = 1;
    field_19_unused = 1;
    field_1_isPaused = false;
    field_1A_bDoubleVolume = true;

    for (s32 i = 0; i < 1020; i++)
    {
        field_147C_audio_entities[i].field_0_bUsed = false;
        field_444C_AudioEntityOrderList[i] = 0;
    }

    field_543C_444C_nAudioEntitiesCount = 0;
    field_5448_m_FrameCounter = 0;
    field_544C_vocals.field_0_fifth_frame_flag = false;
    field_544C_vocals.field_18_pending_voice = 0;
    field_3_bFirstMenuStream = 1;
}

MATCH_FUNC(0x419E10)
void sound_obj::Init_419E10()
{
    if (!field_0_bSoundInitialized)
    {
        PreSoundInit_412240();
        field_0_bSoundInitialized = gSampManager_6FFF00.SoundInit_58D6C0(&field_5440_unused);
        if (field_0_bSoundInitialized)
        {
            field_1D_b3d_sound = 0;
            field_10_nActiveSamples = 16;
            AddVocalsEntity_418C20();
        }
    }
}

MATCH_FUNC(0x419E50)
void sound_obj::DeinitializeAudio_419E50()
{
    if (field_0_bSoundInitialized)
    {
        DeInitVocals_57EA10();
        RemoveVocalsEntity_418C60();
        for (u32 i = 0; i < field_10_nActiveSamples; ++i)
        {
            if (field_1D_b3d_sound)
            {
                gSampManager_6FFF00.StopChannel3D_58DFC0(i);
            }
            else
            {
                gSampManager_6FFF00.StopChannel_58DDD0(i);
            }
        }

        for (u32 j = 0; j < field_543C_444C_nAudioEntitiesCount; j++)
        {
            field_147C_audio_entities[field_444C_AudioEntityOrderList[j]].field_0_bUsed = false;
            field_444C_AudioEntityOrderList[j] = 0;
        }
        field_543C_444C_nAudioEntitiesCount = 0;

        PreSoundShutdown_412250();
        gSampManager_6FFF00.Shutdown_58DB30();
        field_0_bSoundInitialized = false;
    }
}

MATCH_FUNC(0x419EF0)
void sound_obj::Service_419EF0()
{
    static char_type bWasPaused_674E24;
    if (gGame_0x40_67E008)
    {
        field_1_isPaused = gGame_0x40_67E008->field_0_game_state == GameState::Paused_2;
        if (!field_1_isPaused)
        {
            GenerateIntegerRandomNumberTable_41BA90();
        }
        else
        {
            if (!bWasPaused_674E24)
            {
                gSampManager_6FFF00.CloseVocalStream_58E6A0(1);
                gSampManager_6FFF00.PlayVocal_58E510(1, 7, true); // FREEZE
                gSampManager_6FFF00.SetVocalVolume_58E6D0(1, 127u);
            }
        }
        bWasPaused_674E24 = field_1_isPaused;
    }

    if (field_0_bSoundInitialized)
    {
        if (!field_1_isPaused)
        {
            if (!(field_5448_m_FrameCounter++ % 5))
            {
                field_544C_vocals.field_0_fifth_frame_flag = true;
            }
            else
            {
                field_544C_vocals.field_0_fifth_frame_flag = false;
            }
        }
        ServiceSoundEffects_41A3A0();
    }
}

MATCH_FUNC(0x419FA0)
s32 sound_obj::AddSoundObject_419FA0(SoundObject* pSoundObj)
{
    u32 idx = 1;
    if (!field_0_bSoundInitialized || !pSoundObj)
    {
        return 0;
    }

    AudioEntity* pEntityIter = &field_147C_audio_entities[1];
    while (idx < 1020)
    {
        if (!pEntityIter->field_0_bUsed)
        {
            if (pSoundObj->field_0_object_type == SoundObjectTypeEnum::Camera_0xBC_5)
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

            field_147C_audio_entities[idx].field_4_pObj = pSoundObj;
            field_147C_audio_entities[idx].field_0_bUsed = true;
            field_147C_audio_entities[idx].field_1_age = 1;
            field_444C_AudioEntityOrderList[field_543C_444C_nAudioEntitiesCount++] = idx;

            switch (pSoundObj->field_0_object_type)
            {
                case SoundObjectTypeEnum::Sprite_1:
                {
                    Sprite* pEntSprite = field_147C_audio_entities[idx].field_4_pObj->field_C_pAny.pSprite;
                    if (pEntSprite)
                    {
                        switch (pEntSprite->GetType_416B40())
                        {
                            case sprite_types_enum::car_2: // note: sub eax, 2 added via switch case instead of if
                                SoundPosEntry* pNewObj = new SoundPosEntry();
                                field_147C_audio_entities[idx].field_8_pAlloc = pNewObj;
                                pNewObj->field_0_last_speed = kFpZero_674CD8;
                                pNewObj->field_8_unused = 0;
                                pNewObj->field_A_horn_pattern_idx = 0;
                                break;
                        }
                    }
                    break;
                }

                case SoundObjectTypeEnum::Frontend_2:
                {
                    DeInitVocals_57EA10();
                    break;
                }
            }

            return idx;
        }
        idx++;

        ++pEntityIter;
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

    if (!field_147C_audio_entities[idx].field_0_bUsed)
    {
        return;
    }

    SoundObject* pSoundObj = field_147C_audio_entities[idx].field_4_pObj;
    switch (pSoundObj->field_0_object_type)
    {
        case SoundObjectTypeEnum::Sprite_1:
            if (pSoundObj->field_C_pAny.pSprite)
            {
                Sprite* pSprite = pSoundObj->field_C_pAny.pSprite;
                switch (pSprite->GetType_416B40())
                {
                    case sprite_types_enum::car_2:
                        if (field_147C_audio_entities[idx].field_8_pAlloc)
                        {
                            delete field_147C_audio_entities[idx].field_8_pAlloc;
                        }
                        break;
                }
                break;
            }
            // fall through

        case SoundObjectTypeEnum::Frontend_2:
            field_3_bFirstMenuStream = 0;
            gSampManager_6FFF00.FadeOut_58E490();
            break;
    }

    field_147C_audio_entities[idx].field_0_bUsed = false;
    field_147C_audio_entities[idx].field_4_pObj->field_C_pAny.pAny = NULL;
    field_147C_audio_entities[idx].field_1_age = 0;

    if (field_147C_audio_entities[idx].field_4_pObj->field_0_object_type == SoundObjectTypeEnum::Camera_0xBC_5)
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
                        sizeof(u32) * (field_543C_444C_nAudioEntitiesCount - (idx_iter + 1)));
            }

            field_543C_444C_nAudioEntitiesCount--;
            field_444C_AudioEntityOrderList[field_543C_444C_nAudioEntitiesCount] = 0;
            break;
        }
        idx_iter++;
    }
}

MATCH_FUNC(0x41A1B0)
bool sound_obj::LoadStyle_41A1B0(const char_type* pStyleName)
{
    if (field_0_bSoundInitialized)
    {
        return gSampManager_6FFF00.LoadWavSdtData_58E980(GetFileName_41A1E0(pStyleName));
    }
    return false;
}

MATCH_FUNC(0x41A1E0)
const char_type* sound_obj::GetFileName_41A1E0(const char_type* pStr)
{
    LOCAL_STATIC_ARRAY(char_type, sSoundFileName, 80, 0x674E28);
    const char_type* pTmp = strrchr(pStr, '\\');
    if (pTmp)
    {
        pTmp = pTmp + 1;
    }
    else
    {
        pTmp = pStr;
    }

    u8 src_idx = 0;
    while (pTmp[src_idx] != '.')
    {
        if (!pTmp[src_idx])
        {
            break;
        }
        sSoundFileName[src_idx] = pTmp[src_idx];
        src_idx++;
    }

    sSoundFileName[src_idx] = 0;
    return sSoundFileName;
}

MATCH_FUNC(0x41A250)
void sound_obj::SetSfxVol_41A250(u8 sfxVol)
{
    field_24_sfx_vol = sfxVol;
}

MATCH_FUNC(0x41A270)
void sound_obj::SetCDVol_41A270(u8 cdVol)
{
    field_25_cdVol = cdVol;
}

MATCH_FUNC(0x41A280)
u8 sound_obj::GetCDVol_41A280() const
{
    return field_25_cdVol;
}

MATCH_FUNC(0x41A290)
void sound_obj::Release_41A290()
{
    gSampManager_6FFF00.Release_58E290();
}

MATCH_FUNC(0x41A2A0)
void sound_obj::Reacquire_41A2A0()
{
    field_98_nActiveSampleQueue = 1;
    ClearRequestedQueue_41B700();
    field_98_nActiveSampleQueue = 0;

    ClearRequestedQueue_41B700();
    ClearActivateSamples_41B7A0();

    field_1450_unused = 0;

    gSampManager_6FFF00.Reacquire_58E2A0();
}

MATCH_FUNC(0x41A2E0)
char_type sound_obj::GetAudioDriveLetter_41A2E0() const
{
    return gSampManager_6FFF00.field_4_gtaAudioDriveLetter;
}

MATCH_FUNC(0x41A2F0)
bool sound_obj::Set3DSound_41A2F0(char_type b3dSound)
{
    if (field_0_bSoundInitialized)
    {
        if (b3dSound != field_1D_b3d_sound)
        {
            if (b3dSound)
            {
                if (gSampManager_6FFF00.Init3DProvider_58D820(&field_1C_samp_count))
                {
                    field_1D_b3d_sound = 1;
                    field_10_nActiveSamples = field_1C_samp_count; // note: regswap from redundant local
                    return true;
                }
                else
                {
                    gSampManager_6FFF00.AllocSamples_58D9F0(1);
                    field_1D_b3d_sound = 0;
                    field_10_nActiveSamples = 16;
                    return false;
                }
            }

            if (gSampManager_6FFF00.AllocSamples_58D9F0(1))
            {
                field_1D_b3d_sound = 0;
                field_10_nActiveSamples = 16;
                return true;
            }

            gSampManager_6FFF00.Init3DProvider_58D820(&field_1C_samp_count);
            field_1D_b3d_sound = 1;
            field_10_nActiveSamples = field_1C_samp_count;
            return false;
        }
        return true;
    }
    return false;
}

MATCH_FUNC(0x41A390)
char_type sound_obj::Get3dSound_41A390() const
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
        IncrementAudioEntitiesAge_41A6F0();
    }
}
MATCH_FUNC(0x41A3F0)
u8 sound_obj::CalcVolume_41A3F0(u8 base_volume, Fix16 max_dist, Fix16 dist) const
{
    if (max_dist == kFpZero_674CD8)
    {
        return 0;
    }

    Fix16 falloff = (max_dist - dist) / max_dist;
    dist = Fix16(base_volume); // 9.6f also reuses the dist slot here
    s32 volume = Fix16::Round_To_Int_410BF0(falloff * falloff * dist);
    if (volume > 127)
    {
        volume = 127;
    }
    else if (volume < 0)
    {
        volume = 0;
    }
    return volume;
}

MATCH_FUNC(0x41A4A0)
u8 sound_obj::ComputePan_41A4A0(Fix16 intensity, Fix16 x_offset) const
{
    LOCAL_STATIC_ARRAY_INIT(u8, sPanTable, 64, 0x61A688, 0 COMMA 3 COMMA 8 COMMA 12 COMMA 16 COMMA 19 COMMA 22 COMMA 24 COMMA 26 COMMA 28 COMMA 30 COMMA 31 COMMA 33 COMMA 34 COMMA 36 COMMA 37 COMMA 39 COMMA 40 COMMA 41 COMMA 42 COMMA 44 COMMA 45 COMMA 46 COMMA 47 COMMA 48 COMMA 49 COMMA 49 COMMA 50 COMMA 51 COMMA 52 COMMA 53 COMMA 53 COMMA 54 COMMA 55 COMMA 55 COMMA 56 COMMA 56 COMMA 57 COMMA 57 COMMA 58 COMMA 58 COMMA 58 COMMA 59 COMMA 59 COMMA 59 COMMA 60 COMMA 60 COMMA 61 COMMA 61 COMMA 61 COMMA 61 COMMA 62 COMMA 62 COMMA 62 COMMA 62 COMMA 62 COMMA 63 COMMA 63 COMMA 63 COMMA 63 COMMA 63 COMMA 63 COMMA 63 COMMA 63);
    Fix16 half_width = intensity / kFp64;
    if (half_width == kFpZero_674CD8)
    {
        return 63;
    }
    else
    {
        Fix16 offset = x_offset;
        if (x_offset <= 0)
        {
            offset = -x_offset;
        }
        s32 table_idx = (offset / half_width + Fix16(0x2000, 0)).ToInt();
        if (table_idx > 63)
        {
            table_idx = 63;
        }
        s32 pan;
        if (x_offset > kFpZero_674CD8)
        {
            pan = sPanTable[table_idx] + 63;
            if (pan >= 117)
            {
                pan = 117;
            }
        }
        else
        {
            pan = 63 - sPanTable[table_idx];
            if (pan <= 10)
            {
                pan = 10;
            }
        }
        return pan;
    }
}

MATCH_FUNC(0x41A580)
s32 sound_obj::AdjustPlaybackRate_41A580(s32 snd_rate, Fix16 xpos, Fix16 ypos, Fix16 zpos) const
{
    if (zpos != 0)
    {
        Fix16 diff = ypos - xpos;
        if (diff == kFpZero_674CD8)
        {
            return snd_rate;
        }

        // zpos is converted from an integer (shl $0xE) and field_C_doppler_divisor is already fixed point
        // q and rate are assigned, not initialised: that keeps them as register locals
        // (q moved to ecx before the imul, snd_rate << 14 held in esi across the divide)
        Fix16 q;
        q = Fix16(zpos.mValue) / kFp100;
        s32 relative_speed = (((ypos - xpos) * q) / Fix16(field_C_doppler_divisor, 0)).mValue;
        s32 a = relative_speed > 0 ? relative_speed : -relative_speed;
        if (a >= field_4_speed_of_sound)
        {
            return snd_rate;
        }
        Fix16 rate;
        rate = Fix16(snd_rate);
        // The speed of sound is not shifted here (raw ratio speed / (relative_speed + speed))
        return Fix16::Round_To_Int_410BF0(rate * (Fix16(field_4_speed_of_sound, 0) / Fix16(relative_speed + field_4_speed_of_sound, 0)));
    }

    return snd_rate;
}

MATCH_FUNC(0x41A650)
s32 sound_obj::RandomDisplacement_41A650(u32 seed)
{
    static bool bPos_61A6C8 = true;

    s32 value = gSampManager_6FFF00.GetRandomDisplacement_58DC10(seed);
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

MATCH_FUNC(0x41A6C0)
void sound_obj::ResetEntry_41A6C0(s32 idx)
{
    field_147C_audio_entities[idx].field_0_bUsed = false;
    field_147C_audio_entities[idx].field_1_age = 0;
    field_147C_audio_entities[idx].field_4_pObj = NULL;
    field_147C_audio_entities[idx].field_8_pAlloc = NULL;
}

MATCH_FUNC(0x41A6F0)
void sound_obj::IncrementAudioEntitiesAge_41A6F0()
{
    for (u32 idx = 0; idx < field_543C_444C_nAudioEntitiesCount; idx++)
    {
        if (field_147C_audio_entities[field_444C_AudioEntityOrderList[idx]].field_1_age < 10u)
        {
            field_147C_audio_entities[field_444C_AudioEntityOrderList[idx]].field_1_age++;
        }
    }
}

// https://decomp.me/scratch/i3zcW
MATCH_FUNC(0x41A730)
void sound_obj::InterrogateAudioEntities_41A730()
{
    Camera* pCamera;

    if (field_1478_type5Idx != 0 && (pCamera = field_147C_audio_entities[field_1478_type5Idx].field_4_pObj->field_C_pAny.pCamera_0xBC) != NULL)
    {
        Ped* pPed = pCamera->GetFollowedPed_416B70();

        if (pPed != NULL)
        {
            Car* pCar = pPed->get_car_416B60();
            if (pCar != NULL)
            {
                pCar->GetXYZ_416B80(&field_1468_listener_x, &field_146C_listener_y, &field_1470_listener_z);
                field_1474_rotation = pCar->GetCarRotation_416BB0();
            }
            else
            {
                field_1468_listener_x = pPed->get_cam_x();
                field_146C_listener_y = pPed->get_cam_y();
                field_1470_listener_z = pPed->get_cam_z();
                field_1474_rotation = pPed->GetRotation();
            }
        }
        else
        {
            field_1468_listener_x = pCamera->field_98_cur_pos.field_0_x;
            field_146C_listener_y = pCamera->field_98_cur_pos.field_4_y;
            field_1470_listener_z = pCamera->field_98_cur_pos.field_8_z;
        }
    }
    else
    {
        field_1468_listener_x = 0;
        field_146C_listener_y = 0;
        field_1470_listener_z = 0;
    }

    for (u32 j = 0; j < field_543C_444C_nAudioEntitiesCount; j++)
    {
        field_28_dist_squared = kFpZero_674CD8;
        field_2C_distCalculated = false;
        sound_obj::ProcessEntity_4123A0(field_444C_AudioEntityOrderList[j]);
    }
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
    field_30_sQueueSample.field_2D_bIsPlayingFinished = false;

    memcpy(&field_9C_asSamples[field_98_nActiveSampleQueue][new_requested_count], &field_30_sQueueSample, sizeof(sound_0x68));

    AddDetailsToRequestedOrderList_41A910(new_requested_count);
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

MATCH_FUNC(0x41A9D0)
void sound_obj::AddReleasingSounds_41A9D0()
{
    bool toProcess[16];

    const u8 queue = field_98_nActiveSampleQueue == 0 ? 1 : 0;

    for (u8 idx = 0; idx < field_DBC_SampleRequestQueuesStatus[queue]; idx++)
    {
        const u32 sample_idx = field_D9C_abSampleQueueIndexTable[queue][idx];
        sound_0x68& pSound = field_9C_asSamples[queue][sample_idx];
        if (!pSound.field_2D_bIsPlayingFinished)
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
                if (pSound.field_4C_releasing_volume_divider > 0)
                {
                    if (pSound.field_30_loop_count == 0)
                    {
                        u8* pEmittingVolume = &pSound.field_60_nEmittingVolume;
                        if (!field_1D_b3d_sound)
                        {
                            pEmittingVolume = &pSound.field_24_nVolume;
                        }

                        if (pSound.field_50_volume_change == 0xFF)
                        {
                            pSound.field_50_volume_change = *pEmittingVolume / pSound.field_4C_releasing_volume_divider;
                            if (static_cast<s8>(pSound.field_50_volume_change) <= 0)
                            {
                                pSound.field_50_volume_change = 1;
                            }
                        }

                        if (*pEmittingVolume > pSound.field_50_volume_change)
                        {
                            *pEmittingVolume -= pSound.field_50_volume_change;
                        }
                        else
                        {
                            pSound.field_4C_releasing_volume_divider = 0;
                            continue;
                        }
                    }

                    --pSound.field_4C_releasing_volume_divider;
                    if (field_544C_vocals.field_0_fifth_frame_flag)
                    {
                        if (pSound.field_1C_ReleasingVolumeModificator < 20)
                        {
                            ++pSound.field_1C_ReleasingVolumeModificator;
                        }
                    }
                    pSound.field_41_releasing_sound_flag = 0;
                    if (!pSound.field_5C_pReleaseSource)
                    {
                        pSound.field_5C_pReleaseSource = &pSound;
                    }
                    memcpy(&field_30_sQueueSample, &pSound, sizeof(field_30_sQueueSample));
                    sound_obj::AddSampleToRequestedQueue_41A850();
                }
            }
        }
    }
}

MATCH_FUNC(0x41AB80)
void sound_obj::ProcessActiveQueues_41AB80()
{
    SoundPos position;
    f32 x;
    f32 y;
    f32 z;
    u8 i;
    u8 j;

    for (u8 idx = 0; idx < field_10_nActiveSamples; idx++)
    {
        field_9C_asSamples[field_98_nActiveSampleQueue][idx].field_2C_bIsBeingPlayed = false;
        field_DC0_ActiveSamples[idx].field_2C_bIsBeingPlayed = false;
    }

    for (i = 0; i < field_DBC_SampleRequestQueuesStatus[field_98_nActiveSampleQueue]; i++)
    {
        sound_0x68& sample =
            field_9C_asSamples[field_98_nActiveSampleQueue][field_D9C_abSampleQueueIndexTable[field_98_nActiveSampleQueue][i]];
        for (j = 0; j < field_10_nActiveSamples; j++)
        {
            sound_0x68& active = field_DC0_ActiveSamples[j];

            if (sample.field_0_EntityIndex != active.field_0_EntityIndex || sample.field_4_SampleIndex != active.field_4_SampleIndex ||
                sample.field_41_releasing_sound_flag == 2 || (active.field_5C_pReleaseSource && !sample.field_5C_pReleaseSource))
            {
                continue;
            }

            if (!InitSampleForType_412260(&sample) || sample.field_14_samp_idx >= 321 || sample.field_14_samp_idx != active.field_14_samp_idx)
            {
                continue;
            }

            UpdateFramesToPlay_41B490(&sample);

            if (sample.field_30_loop_count > 0)
            {
                if (active.field_44_frames_to_play == 0)
                {
                    bool bUsed;
                    if (field_1D_b3d_sound)
                    {
                        bUsed = gSampManager_6FFF00.GetChannel3DUsedFlag_58DF70(j);
                    }
                    else
                    {
                        bUsed = gSampManager_6FFF00.GetChannelUsedFlag_58DD80(j);
                    }

                    if (!bUsed)
                    {
                        sample.field_2D_bIsPlayingFinished = true;
                        active.field_2D_bIsPlayingFinished = true;
                        active.field_14_samp_idx = 321;
                        active.field_0_EntityIndex = 0;
                        continue;
                    }
                    active.field_44_frames_to_play = field_8_frames_per_second *
                        gSampManager_6FFF00.GetSampleLength_58DC70(active.field_14_samp_idx) / sample.field_20_rate;
                }
                else
                {
                    active.field_44_frames_to_play--;
                }
            }

            sample.field_2C_bIsBeingPlayed = true;
            active.field_2C_bIsBeingPlayed = true;
            sample.field_50_volume_change = -1;

            if (sample.field_41_releasing_sound_flag)
            {
                sample.field_2C_bIsBeingPlayed = false;
                active.field_2C_bIsBeingPlayed = false;
                continue;
            }

            if (sample.field_18_bIs2D)
            {
                if (field_1D_b3d_sound)
                {
                    s32 emittingVol = field_1A_bDoubleVolume ? 2 * (sample.field_60_nEmittingVolume < 63 ? sample.field_60_nEmittingVolume : 63) : sample.field_60_nEmittingVolume;
                    gSampManager_6FFF00.SetChannel3DFrequency_58DF00(j, sample.field_20_rate);
                    gSampManager_6FFF00.SetChannel3DVolume_58DE80(j, static_cast<u8>((emittingVol * field_24_sfx_vol) >> 7));
                }
                else
                {
                    s32 emittingVol = field_1A_bDoubleVolume ? 2 * (sample.field_24_nVolume < 63 ? sample.field_24_nVolume : 63) : sample.field_24_nVolume;
                    gSampManager_6FFF00.SetChannelFrequency_58DD20(j, sample.field_20_rate);
                    gSampManager_6FFF00.SetChannelVolume_58DCE0(j, (emittingVol * field_24_sfx_vol) >> 7);
                    gSampManager_6FFF00.SetChannelPan_58DD00(j, static_cast<u8>(sample.field_40_pan));
                }
                break;
            }

            // Doppler: the old distance is passed, the new one stored, both truncated to whole units
            s32 old_dist = active.field_28_distance.ToInt();
            s32 new_dist = sample.field_28_distance.ToInt();
            active.field_28_distance = Fix16(new_dist);
            // The speed multiplier is passed as its raw bits (a copy of the s32, no Fix16 constructor)
            sample.field_20_rate =
                AdjustPlaybackRate_41A580(sample.field_20_rate, old_dist, new_dist, reinterpret_cast<Fix16&>(sample.field_3C_speed_multiplier));
            if (sample.field_20_rate != active.field_20_rate)
            {
                u32 freq = Clamp2(sample.field_20_rate, active.field_20_rate, 6000);
                active.field_20_rate = freq;
                if (field_1D_b3d_sound)
                {
                    gSampManager_6FFF00.SetChannel3DFrequency_58DF00(j, freq);
                }
                else
                {
                    gSampManager_6FFF00.SetChannelFrequency_58DD20(j, freq);
                }
            }

            s32 oldVolume;
            s32 newVolume;
            if (field_1D_b3d_sound)
            {
                oldVolume = active.field_60_nEmittingVolume;
                newVolume = sample.field_60_nEmittingVolume;
            }
            else
            {
                oldVolume = active.field_24_nVolume;
                newVolume = sample.field_24_nVolume;
            }

            if (newVolume != oldVolume)
            {
                s32 volume = Clamp2(newVolume, oldVolume, 10);
                s32 emittingVol = field_1A_bDoubleVolume ? 2 * Min(volume, 63) : volume;
                if (field_1D_b3d_sound)
                {
                    gSampManager_6FFF00.SetChannel3DVolume_58DE80(j, static_cast<u8>((emittingVol * field_24_sfx_vol) >> 7));
                    active.field_60_nEmittingVolume = volume;
                }
                else
                {
                    gSampManager_6FFF00.SetChannelVolume_58DCE0(j, static_cast<u8>((emittingVol * field_24_sfx_vol) >> 7));
                    active.field_24_nVolume = volume;
                }
            }

            VecDiff_41B4E0(&sample.field_8_obj, &position);

            if (field_1D_b3d_sound)
            {
                FromFix16_41B520(position.field_0_x, &x);
                FromFix16_41B520(position.field_4_y, &y);
                FromFix16_41B520(position.field_8_z, &z);
                // The original passes 0 for y and -y for z (z is computed but unused)
                gSampManager_6FFF00.SetChannel3DPosition_58DEA0(j, x, 0.0f, -y);
                gSampManager_6FFF00.SetChannel3DDistances_58DED0(j,
                                                                 static_cast<f32>(static_cast<u32>(sample.field_64_max_distance)),
                                                                 static_cast<f32>(static_cast<u32>(sample.field_64_max_distance) >> 2));
                break;
            }

            s32 pan = static_cast<u8>(active.field_40_pan);
            s32 newPan = ComputePan_41A4A0(sample.field_54_sound_intensity, position.field_0_x);
            if (newPan != pan)
            {
                pan = Clamp2(newPan, pan, 10);
                gSampManager_6FFF00.SetChannelPan_58DD00(j, static_cast<u8>(pan));
                active.field_40_pan = pan;
            }
            break;
        }
    }

    for (j = 0; j < field_10_nActiveSamples; j++)
    {
        sound_0x68& active = field_DC0_ActiveSamples[j];
        if (active.field_14_samp_idx != 321 && !active.field_2C_bIsBeingPlayed)
        {
            if (field_1D_b3d_sound)
            {
                gSampManager_6FFF00.StopChannel3D_58DFC0(j);
            }
            else
            {
                gSampManager_6FFF00.StopChannel_58DDD0(j);
            }
            active.field_14_samp_idx = 321;
            active.field_0_EntityIndex = 0;
        }
    }

    for (i = 0; i < field_DBC_SampleRequestQueuesStatus[field_98_nActiveSampleQueue]; i++)
    {
        sound_0x68& sample =
            field_9C_asSamples[field_98_nActiveSampleQueue][field_D9C_abSampleQueueIndexTable[field_98_nActiveSampleQueue][i]];
        if (sample.field_2C_bIsBeingPlayed || sample.field_2D_bIsPlayingFinished ||
            !field_147C_audio_entities[sample.field_0_EntityIndex].field_0_bUsed)
        {
            continue;
        }

        for (j = 0; j < field_10_nActiveSamples; j++)
        {
            sound_0x68& active = field_DC0_ActiveSamples[j];
            if (active.field_2C_bIsBeingPlayed || !InitSampleForType_412260(&sample) || sample.field_14_samp_idx >= 321)
            {
                continue;
            }

            UpdateFramesToPlay_41B490(&sample);

            if (sample.field_30_loop_count > 0)
            {
                u32 samplesPerFrame = sample.field_20_rate / field_8_frames_per_second;
                u32 samplesToPlay = sample.field_30_loop_count * gSampManager_6FFF00.GetSampleLength_58DC70(sample.field_14_samp_idx);
                if (samplesPerFrame == 0)
                {
                    continue;
                }
                sample.field_4C_releasing_volume_divider = samplesToPlay / samplesPerFrame + 1;
            }

            memcpy(&active, &sample, sizeof(sound_0x68));

            if (!active.field_18_bIs2D)
            {
                VecDiff_41B4E0(&active.field_8_obj, &position);
                if (!field_1D_b3d_sound)
                {
                    active.field_40_pan = ComputePan_41A4A0(active.field_54_sound_intensity, position.field_0_x);
                }
            }

            if (field_1D_b3d_sound)
            {
                s32 emittingVol = field_1A_bDoubleVolume ? 2 * (active.field_60_nEmittingVolume < 63 ? active.field_60_nEmittingVolume : 63) : active.field_60_nEmittingVolume;
                gSampManager_6FFF00.InitialiseChannel3D_58DDF0(j, active.field_14_samp_idx, active.field_20_rate);
                gSampManager_6FFF00.SetChannel3DFrequency_58DF00(j, active.field_20_rate);
                gSampManager_6FFF00.SetChannel3DVolume_58DE80(j, static_cast<u8>((emittingVol * field_24_sfx_vol) >> 7));
                gSampManager_6FFF00.SetChannel3DLoopCount_58DF50(j, active.field_30_loop_count);
                gSampManager_6FFF00.SetChannel3DLoopPoints_58DF20(j, active.field_34_loop_start, active.field_38_loop_end);

                if (active.field_18_bIs2D)
                {
                    x = 0.0f;
                    y = 0.0f;
                    z = 0.0f;
                    active.field_54_sound_intensity = 1638400000;
                }
                else
                {
                    FromFix16_41B520(position.field_0_x, &x);
                    FromFix16_41B520(position.field_4_y, &y);
                    FromFix16_41B520(position.field_8_z, &z);
                }

                gSampManager_6FFF00.SetChannel3DPosition_58DEA0(j, x, 0.0f, -y);
                gSampManager_6FFF00.SetChannel3DDistances_58DED0(j,
                                                                 static_cast<f32>(static_cast<u32>(active.field_64_max_distance)),
                                                                 static_cast<f32>(static_cast<u32>(active.field_64_max_distance) >> 2));
                gSampManager_6FFF00.StartChannel3D_58DFA0(j);
            }
            else
            {
                s32 emittingVol = field_1A_bDoubleVolume ? 2 * (active.field_24_nVolume < 63 ? active.field_24_nVolume : 63) : active.field_24_nVolume;
                gSampManager_6FFF00.InitialiseChannel_58DC90(j, active.field_14_samp_idx);
                gSampManager_6FFF00.SetChannelFrequency_58DD20(j, active.field_20_rate);
                gSampManager_6FFF00.SetChannelVolume_58DCE0(j, static_cast<u8>((emittingVol * field_24_sfx_vol) >> 7));
                gSampManager_6FFF00.SetChannelPan_58DD00(j, static_cast<u8>(active.field_40_pan));
                gSampManager_6FFF00.SetChannelLoopCount_58DD60(j, active.field_30_loop_count);
                gSampManager_6FFF00.SetChannelLoopPoints_58DD40(j, active.field_34_loop_start, active.field_38_loop_end);
                gSampManager_6FFF00.StartChannel_58DDB0(j);
            }

            active.field_2C_bIsBeingPlayed = true;
            sample.field_2C_bIsBeingPlayed = true;
            sample.field_50_volume_change = -1;
            break;
        }
    }
}

MATCH_FUNC(0x41B490)
void sound_obj::UpdateFramesToPlay_41B490(sound_0x68* pObj)
{
    if (pObj->field_41_releasing_sound_flag)
    {
        if (pObj->field_30_loop_count)
        {
            pObj->field_44_frames_to_play = (static_cast<u32>(field_8_frames_per_second) * gSampManager_6FFF00.GetSampleLength_58DC70(pObj->field_14_samp_idx)) /
                pObj->field_20_rate;
        }
        else
        {
            pObj->field_44_frames_to_play = -3;
        }
    }
}

MATCH_FUNC(0x41B4E0)
void sound_obj::VecDiff_41B4E0(const SoundPos* pVec, SoundPos* pRet) const
{
    pRet->field_0_x = pVec->field_0_x - field_1468_listener_x;
    pRet->field_4_y = pVec->field_4_y - field_146C_listener_y;
    pRet->field_8_z = pVec->field_8_z - field_1470_listener_z;
}

MATCH_FUNC(0x41B520)
void sound_obj::FromFix16_41B520(Fix16 value, f32* pOut) const
{
    *pOut = value.ToFloat();
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
            if (!pIter->field_18_bIs2D)
            {
                f32 distance;
                FromFix16_41B520(pIter->field_28_distance, &distance);
                pIter->field_60_nEmittingVolume =
                    ComputeEmittingVolume_41B660(pIter->field_60_nEmittingVolume, pIter->field_64_max_distance, static_cast<u32>(distance));
            }
        }
    }
}

EXPORT bool Cooldown_4236C0();

MATCH_FUNC(0x41B660)
u8 sound_obj::ComputeEmittingVolume_41B660(u8 emittingVolume, u32 maxDistance, u32 distance) const
{
    const s32 minDistance = maxDistance / 4;
    const u32 diffDistance = maxDistance - minDistance;
    if (distance > diffDistance)
    {
        return (minDistance - (distance - diffDistance)) * emittingVolume / minDistance;
    }
    return emittingVolume;
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
        field_DC0_ActiveSamples[i].field_8_obj.field_0_x = kFpZero_674CD8;
        field_DC0_ActiveSamples[i].field_8_obj.field_4_y = kFpZero_674CD8;
        field_DC0_ActiveSamples[i].field_8_obj.field_8_z = kFpZero_674CD8;
        field_DC0_ActiveSamples[i].field_18_bIs2D = false;
        field_DC0_ActiveSamples[i].field_1C_ReleasingVolumeModificator = 4;
        field_DC0_ActiveSamples[i].field_20_rate = 22050;
        field_DC0_ActiveSamples[i].field_58_type = 0;
        field_DC0_ActiveSamples[i].field_24_nVolume = 0;
        field_DC0_ActiveSamples[i].field_60_nEmittingVolume = 0;
        field_DC0_ActiveSamples[i].field_64_max_distance = 10;
        field_DC0_ActiveSamples[i].field_28_distance = 0;
        field_DC0_ActiveSamples[i].field_2C_bIsBeingPlayed = false;
        field_DC0_ActiveSamples[i].field_2D_bIsPlayingFinished = false;
        field_DC0_ActiveSamples[i].field_30_loop_count = 1;
        field_DC0_ActiveSamples[i].field_34_loop_start = 0;
        field_DC0_ActiveSamples[i].field_38_loop_end = -1;
        field_DC0_ActiveSamples[i].field_3C_speed_multiplier = 0;
        field_DC0_ActiveSamples[i].field_54_sound_intensity = 0x190000;
        field_DC0_ActiveSamples[i].field_40_pan = 63;
        field_DC0_ActiveSamples[i].field_41_releasing_sound_flag = 0;
        field_DC0_ActiveSamples[i].field_44_frames_to_play = -3;
        field_DC0_ActiveSamples[i].field_48_nCalculatedVolume = 0;
        field_DC0_ActiveSamples[i].field_4C_releasing_volume_divider = 0;
        field_DC0_ActiveSamples[i].field_50_volume_change = -1;
        field_DC0_ActiveSamples[i].field_5C_pReleaseSource = NULL;
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

MATCH_FUNC(0x41E820)
void sound_obj::ProcessObject_41E820(Sound_Params* pParams)
{
    if (pParams->field_0_pObj->field_8_object_2C_ptr->field_8_definition->field_34_behavior_type == object_behavior_type::effect_only_12)
    {
        sound_obj::ProcessEffectOnlyObject_41E850(pParams);
    }
    else
    {
        sound_obj::ProcessOtherObjects_41F520(pParams);
    }
}

// The looping or one-shot sound of a map object, by its object kind. Some kinds only play now and
// then (the gObjSoundDelayKind* globals count down between plays) and pick a random sample when they do.
MATCH_FUNC(0x41E850)
void sound_obj::ProcessEffectOnlyObject_41E850(Sound_Params* pParams)
{
    u32 kind = pParams->field_0_pObj->field_8_object_2C_ptr->field_26_ped_ref_idx;
    Fix16 dist;
    u8 vol;
    switch (kind)
    {
        case 3:
            field_30_sQueueSample.field_14_samp_idx = 141;
            field_30_sQueueSample.field_30_loop_count = 0;
            field_30_sQueueSample.field_4C_releasing_volume_divider = 3;
            field_30_sQueueSample.field_4_SampleIndex = 0;
            field_30_sQueueSample.field_41_releasing_sound_flag = 0;
            field_30_sQueueSample.field_1C_ReleasingVolumeModificator = 1;
            field_30_sQueueSample.field_54_sound_intensity = Fix16(0x32000, 0);
            field_30_sQueueSample.field_64_max_distance = 25;
            dist = Fix16(0x271000, 0);
            vol = 90;
            break;
        case 4:
            field_30_sQueueSample.field_14_samp_idx = 142;
            field_30_sQueueSample.field_30_loop_count = 0;
            field_30_sQueueSample.field_4C_releasing_volume_divider = 3;
            field_30_sQueueSample.field_4_SampleIndex = 0;
            field_30_sQueueSample.field_41_releasing_sound_flag = 0;
            field_30_sQueueSample.field_1C_ReleasingVolumeModificator = 1;
            field_30_sQueueSample.field_54_sound_intensity = Fix16(0x32000, 0);
            field_30_sQueueSample.field_64_max_distance = 25;
            dist = Fix16(0x271000, 0);
            vol = 70;
            break;
        case 5:
            field_30_sQueueSample.field_14_samp_idx = 192;
            field_30_sQueueSample.field_30_loop_count = 0;
            field_30_sQueueSample.field_4C_releasing_volume_divider = 3;
            field_30_sQueueSample.field_4_SampleIndex = 0;
            field_30_sQueueSample.field_41_releasing_sound_flag = 0;
            field_30_sQueueSample.field_1C_ReleasingVolumeModificator = 4;
            field_30_sQueueSample.field_54_sound_intensity = Fix16(9);
            field_30_sQueueSample.field_64_max_distance = 18;
            dist = Fix16(81);
            vol = 50;
            break;
        case 8:
            field_30_sQueueSample.field_14_samp_idx = 143;
            field_30_sQueueSample.field_30_loop_count = 0;
            field_30_sQueueSample.field_4C_releasing_volume_divider = 3;
            field_30_sQueueSample.field_4_SampleIndex = 0;
            field_30_sQueueSample.field_41_releasing_sound_flag = 0;
            field_30_sQueueSample.field_1C_ReleasingVolumeModificator = 3;
            field_30_sQueueSample.field_54_sound_intensity = Fix16(10);
            field_30_sQueueSample.field_64_max_distance = 20;
            dist = Fix16(100);
            vol = 60;
            break;
        case 9:
            field_30_sQueueSample.field_14_samp_idx = 144;
            field_30_sQueueSample.field_30_loop_count = 0;
            field_30_sQueueSample.field_4C_releasing_volume_divider = 3;
            field_30_sQueueSample.field_4_SampleIndex = 0;
            field_30_sQueueSample.field_41_releasing_sound_flag = 0;
            field_30_sQueueSample.field_1C_ReleasingVolumeModificator = 1;
            field_30_sQueueSample.field_54_sound_intensity = Fix16(0x32000, 0);
            field_30_sQueueSample.field_64_max_distance = 25;
            dist = Fix16(0x271000, 0);
            vol = 60;
            break;
        case 13:
            field_30_sQueueSample.field_14_samp_idx = 146;
            field_30_sQueueSample.field_30_loop_count = 0;
            field_30_sQueueSample.field_4C_releasing_volume_divider = 3;
            field_30_sQueueSample.field_4_SampleIndex = 0;
            field_30_sQueueSample.field_41_releasing_sound_flag = 0;
            field_30_sQueueSample.field_1C_ReleasingVolumeModificator = 3;
            field_30_sQueueSample.field_54_sound_intensity = Fix16(0x2e000, 0);
            field_30_sQueueSample.field_64_max_distance = 23;
            dist = Fix16(0x211000, 0);
            vol = 50;
            break;
        case 19:
            field_30_sQueueSample.field_14_samp_idx = 147;
            field_30_sQueueSample.field_30_loop_count = 0;
            field_30_sQueueSample.field_4C_releasing_volume_divider = 3;
            field_30_sQueueSample.field_4_SampleIndex = 0;
            field_30_sQueueSample.field_41_releasing_sound_flag = 0;
            field_30_sQueueSample.field_1C_ReleasingVolumeModificator = 3;
            field_30_sQueueSample.field_54_sound_intensity = Fix16(0x5a000, 0);
            field_30_sQueueSample.field_64_max_distance = 45;
            dist = Fix16(0x7e9000, 0);
            vol = 70;
            break;
        case 16:
            field_30_sQueueSample.field_14_samp_idx = 185;
            field_30_sQueueSample.field_30_loop_count = 0;
            field_30_sQueueSample.field_4C_releasing_volume_divider = 3;
            field_30_sQueueSample.field_4_SampleIndex = 0;
            field_30_sQueueSample.field_41_releasing_sound_flag = 0;
            field_30_sQueueSample.field_1C_ReleasingVolumeModificator = 6;
            field_30_sQueueSample.field_54_sound_intensity = Fix16(0x1e000, 0);
            field_30_sQueueSample.field_64_max_distance = 15;
            dist = Fix16(0xe1000, 0);
            vol = 50;
            break;
        case 10:
        case 14:
            field_30_sQueueSample.field_14_samp_idx = 149;
            field_30_sQueueSample.field_30_loop_count = 0;
            field_30_sQueueSample.field_4C_releasing_volume_divider = 3;
            field_30_sQueueSample.field_4_SampleIndex = 0;
            field_30_sQueueSample.field_41_releasing_sound_flag = 0;
            field_30_sQueueSample.field_1C_ReleasingVolumeModificator = 5;
            field_30_sQueueSample.field_54_sound_intensity = Fix16(0x32000, 0);
            field_30_sQueueSample.field_64_max_distance = 25;
            dist = Fix16(0x271000, 0);
            vol = 50;
            break;
        case 33:
            field_30_sQueueSample.field_14_samp_idx = 57;
            field_30_sQueueSample.field_30_loop_count = 0;
            field_30_sQueueSample.field_4C_releasing_volume_divider = 3;
            field_30_sQueueSample.field_4_SampleIndex = 0;
            field_30_sQueueSample.field_41_releasing_sound_flag = 0;
            field_30_sQueueSample.field_1C_ReleasingVolumeModificator = 7;
            field_30_sQueueSample.field_54_sound_intensity = Fix16(5);
            field_30_sQueueSample.field_64_max_distance = 10;
            dist = Fix16(25);
            vol = 40;
            break;
        case 23:
            field_30_sQueueSample.field_14_samp_idx = 161;
            field_30_sQueueSample.field_30_loop_count = 0;
            field_30_sQueueSample.field_4C_releasing_volume_divider = 3;
            field_30_sQueueSample.field_4_SampleIndex = 0;
            field_30_sQueueSample.field_41_releasing_sound_flag = 0;
            field_30_sQueueSample.field_1C_ReleasingVolumeModificator = 0;
            field_30_sQueueSample.field_54_sound_intensity = Fix16(0x46000, 0);
            field_30_sQueueSample.field_64_max_distance = 35;
            dist = Fix16(0x4c9000, 0);
            vol = 90;
            break;
        case 43:
            field_30_sQueueSample.field_14_samp_idx = 162;
            field_30_sQueueSample.field_30_loop_count = 0;
            field_30_sQueueSample.field_4C_releasing_volume_divider = 3;
            field_30_sQueueSample.field_4_SampleIndex = 0;
            field_30_sQueueSample.field_41_releasing_sound_flag = 0;
            field_30_sQueueSample.field_1C_ReleasingVolumeModificator = 4;
            field_30_sQueueSample.field_54_sound_intensity = Fix16(0x32000, 0);
            field_30_sQueueSample.field_64_max_distance = 25;
            dist = Fix16(0x271000, 0);
            vol = 70;
            break;
        case 26:
            field_30_sQueueSample.field_14_samp_idx = 163;
            field_30_sQueueSample.field_30_loop_count = 0;
            field_30_sQueueSample.field_4C_releasing_volume_divider = 3;
            field_30_sQueueSample.field_4_SampleIndex = 0;
            field_30_sQueueSample.field_41_releasing_sound_flag = 0;
            field_30_sQueueSample.field_1C_ReleasingVolumeModificator = 14;
            field_30_sQueueSample.field_54_sound_intensity = Fix16(5);
            field_30_sQueueSample.field_64_max_distance = 10;
            dist = Fix16(25);
            vol = 40;
            break;
        case 36:
            field_30_sQueueSample.field_14_samp_idx = 169;
            field_30_sQueueSample.field_30_loop_count = 0;
            field_30_sQueueSample.field_4C_releasing_volume_divider = 3;
            field_30_sQueueSample.field_4_SampleIndex = 0;
            field_30_sQueueSample.field_41_releasing_sound_flag = 0;
            field_30_sQueueSample.field_1C_ReleasingVolumeModificator = 1;
            field_30_sQueueSample.field_54_sound_intensity = Fix16(5);
            field_30_sQueueSample.field_64_max_distance = 10;
            dist = Fix16(25);
            vol = 60;
            break;
        case 30:
            field_30_sQueueSample.field_14_samp_idx = 166;
            field_30_sQueueSample.field_30_loop_count = 0;
            field_30_sQueueSample.field_4C_releasing_volume_divider = 3;
            field_30_sQueueSample.field_4_SampleIndex = 0;
            field_30_sQueueSample.field_41_releasing_sound_flag = 0;
            field_30_sQueueSample.field_1C_ReleasingVolumeModificator = 12;
            field_30_sQueueSample.field_54_sound_intensity = Fix16(0xa000, 0);
            field_30_sQueueSample.field_64_max_distance = 5;
            dist = Fix16(0x19000, 0);
            vol = 50;
            break;
        case 15:
            field_30_sQueueSample.field_14_samp_idx = 174;
            field_30_sQueueSample.field_30_loop_count = 0;
            field_30_sQueueSample.field_4C_releasing_volume_divider = 3;
            field_30_sQueueSample.field_4_SampleIndex = 0;
            field_30_sQueueSample.field_41_releasing_sound_flag = 0;
            field_30_sQueueSample.field_1C_ReleasingVolumeModificator = 12;
            field_30_sQueueSample.field_54_sound_intensity = Fix16(0xa000, 0);
            field_30_sQueueSample.field_64_max_distance = 5;
            dist = Fix16(0x19000, 0);
            vol = 40;
            break;
        case 41:
            field_30_sQueueSample.field_14_samp_idx = 175;
            field_30_sQueueSample.field_30_loop_count = 0;
            field_30_sQueueSample.field_4C_releasing_volume_divider = 3;
            field_30_sQueueSample.field_4_SampleIndex = 0;
            field_30_sQueueSample.field_41_releasing_sound_flag = 0;
            field_30_sQueueSample.field_1C_ReleasingVolumeModificator = 1;
            field_30_sQueueSample.field_54_sound_intensity = Fix16(10);
            field_30_sQueueSample.field_64_max_distance = 20;
            dist = Fix16(100);
            vol = 70;
            break;
        case 42:
            field_30_sQueueSample.field_14_samp_idx = 176;
            field_30_sQueueSample.field_30_loop_count = 0;
            field_30_sQueueSample.field_4C_releasing_volume_divider = 3;
            field_30_sQueueSample.field_4_SampleIndex = 0;
            field_30_sQueueSample.field_41_releasing_sound_flag = 0;
            field_30_sQueueSample.field_1C_ReleasingVolumeModificator = 5;
            field_30_sQueueSample.field_54_sound_intensity = Fix16(5);
            field_30_sQueueSample.field_64_max_distance = 10;
            dist = Fix16(25);
            vol = 40;
            break;
        case 44:
            field_30_sQueueSample.field_14_samp_idx = 177;
            field_30_sQueueSample.field_30_loop_count = 0;
            field_30_sQueueSample.field_4C_releasing_volume_divider = 3;
            field_30_sQueueSample.field_4_SampleIndex = 0;
            field_30_sQueueSample.field_41_releasing_sound_flag = 0;
            field_30_sQueueSample.field_1C_ReleasingVolumeModificator = 1;
            field_30_sQueueSample.field_54_sound_intensity = Fix16(0x32000, 0);
            field_30_sQueueSample.field_64_max_distance = 25;
            dist = Fix16(0x271000, 0);
            vol = 70;
            break;
        case 47:
            field_30_sQueueSample.field_14_samp_idx = 178;
            field_30_sQueueSample.field_30_loop_count = 0;
            field_30_sQueueSample.field_4C_releasing_volume_divider = 3;
            field_30_sQueueSample.field_4_SampleIndex = 0;
            field_30_sQueueSample.field_41_releasing_sound_flag = 0;
            field_30_sQueueSample.field_1C_ReleasingVolumeModificator = 1;
            field_30_sQueueSample.field_54_sound_intensity = Fix16(0x32000, 0);
            field_30_sQueueSample.field_64_max_distance = 25;
            dist = Fix16(0x271000, 0);
            vol = 70;
            break;
        case 48:
            field_30_sQueueSample.field_14_samp_idx = 179;
            field_30_sQueueSample.field_30_loop_count = 0;
            field_30_sQueueSample.field_4C_releasing_volume_divider = 3;
            field_30_sQueueSample.field_4_SampleIndex = 0;
            field_30_sQueueSample.field_41_releasing_sound_flag = 0;
            field_30_sQueueSample.field_1C_ReleasingVolumeModificator = 1;
            field_30_sQueueSample.field_54_sound_intensity = Fix16(0x32000, 0);
            field_30_sQueueSample.field_64_max_distance = 25;
            dist = Fix16(0x271000, 0);
            vol = 90;
            break;
        case 49:
            field_30_sQueueSample.field_14_samp_idx = 180;
            field_30_sQueueSample.field_30_loop_count = 0;
            field_30_sQueueSample.field_4C_releasing_volume_divider = 3;
            field_30_sQueueSample.field_4_SampleIndex = 0;
            field_30_sQueueSample.field_41_releasing_sound_flag = 0;
            field_30_sQueueSample.field_1C_ReleasingVolumeModificator = 1;
            field_30_sQueueSample.field_54_sound_intensity = Fix16(0x32000, 0);
            field_30_sQueueSample.field_64_max_distance = 25;
            dist = Fix16(0x271000, 0);
            vol = 60;
            break;
        case 45:
            field_30_sQueueSample.field_14_samp_idx = 192;
            field_30_sQueueSample.field_30_loop_count = 0;
            field_30_sQueueSample.field_4C_releasing_volume_divider = 3;
            field_30_sQueueSample.field_4_SampleIndex = 0;
            field_30_sQueueSample.field_41_releasing_sound_flag = 0;
            field_30_sQueueSample.field_1C_ReleasingVolumeModificator = 3;
            field_30_sQueueSample.field_54_sound_intensity = Fix16(0xa000, 0);
            field_30_sQueueSample.field_64_max_distance = 5;
            dist = Fix16(0x19000, 0);
            vol = 40;
            break;
        case 50:
        case 51:
            field_30_sQueueSample.field_14_samp_idx = 173;
            field_30_sQueueSample.field_30_loop_count = 1;
            field_30_sQueueSample.field_41_releasing_sound_flag = 1;
            field_30_sQueueSample.field_1C_ReleasingVolumeModificator = 3;
            field_30_sQueueSample.field_54_sound_intensity = Fix16(10);
            field_30_sQueueSample.field_64_max_distance = 20;
            dist = Fix16(100);
            vol = 75;
            break;
        case 40:
            field_30_sQueueSample.field_14_samp_idx = 172;
            field_30_sQueueSample.field_30_loop_count = 1;
            field_30_sQueueSample.field_41_releasing_sound_flag = 1;
            field_30_sQueueSample.field_1C_ReleasingVolumeModificator = 1;
            field_30_sQueueSample.field_54_sound_intensity = Fix16(10);
            field_30_sQueueSample.field_64_max_distance = 20;
            dist = Fix16(100);
            vol = 60;
            break;
        case 37:
            field_30_sQueueSample.field_14_samp_idx = 170;
            field_30_sQueueSample.field_30_loop_count = 1;
            field_30_sQueueSample.field_41_releasing_sound_flag = 1;
            field_30_sQueueSample.field_1C_ReleasingVolumeModificator = 1;
            field_30_sQueueSample.field_54_sound_intensity = Fix16(10);
            field_30_sQueueSample.field_64_max_distance = 20;
            dist = Fix16(100);
            vol = 60;
            break;
        case 38:
            field_30_sQueueSample.field_14_samp_idx = 171;
            field_30_sQueueSample.field_30_loop_count = 1;
            field_30_sQueueSample.field_41_releasing_sound_flag = 1;
            field_30_sQueueSample.field_1C_ReleasingVolumeModificator = 5;
            field_30_sQueueSample.field_54_sound_intensity = Fix16(10);
            field_30_sQueueSample.field_64_max_distance = 20;
            dist = Fix16(100);
            vol = 60;
            break;
        case 39:
            field_30_sQueueSample.field_14_samp_idx = (field_1454_anRandomTable[0] & 3) + 181;
            field_30_sQueueSample.field_30_loop_count = 1;
            field_30_sQueueSample.field_41_releasing_sound_flag = 1;
            field_30_sQueueSample.field_1C_ReleasingVolumeModificator = 2;
            field_30_sQueueSample.field_54_sound_intensity = Fix16(10);
            field_30_sQueueSample.field_64_max_distance = 20;
            dist = Fix16(100);
            vol = 60;
            break;
        case 32:
            field_30_sQueueSample.field_14_samp_idx = field_1454_anRandomTable[static_cast<u32>(field_30_sQueueSample.field_0_EntityIndex) % 5] % 6 + 233;
            field_30_sQueueSample.field_30_loop_count = 1;
            field_30_sQueueSample.field_41_releasing_sound_flag = 1;
            field_30_sQueueSample.field_1C_ReleasingVolumeModificator = 3;
            field_30_sQueueSample.field_54_sound_intensity = Fix16(10);
            field_30_sQueueSample.field_64_max_distance = 20;
            dist = Fix16(100);
            vol = 80;
            break;
        case 27:
            field_30_sQueueSample.field_14_samp_idx = 164;
            field_30_sQueueSample.field_30_loop_count = 1;
            field_30_sQueueSample.field_41_releasing_sound_flag = 1;
            field_30_sQueueSample.field_1C_ReleasingVolumeModificator = 2;
            field_30_sQueueSample.field_54_sound_intensity = Fix16(10);
            field_30_sQueueSample.field_64_max_distance = 20;
            dist = Fix16(100);
            vol = 100;
            break;
        case 34:
            field_30_sQueueSample.field_14_samp_idx = 167;
            field_30_sQueueSample.field_30_loop_count = 1;
            field_30_sQueueSample.field_41_releasing_sound_flag = 1;
            field_30_sQueueSample.field_1C_ReleasingVolumeModificator = 1;
            field_30_sQueueSample.field_54_sound_intensity = Fix16(10);
            field_30_sQueueSample.field_64_max_distance = 20;
            dist = Fix16(100);
            vol = 50;
            break;
        case 35:
            field_30_sQueueSample.field_14_samp_idx = 168;
            field_30_sQueueSample.field_30_loop_count = 1;
            field_30_sQueueSample.field_41_releasing_sound_flag = 1;
            field_30_sQueueSample.field_1C_ReleasingVolumeModificator = 1;
            field_30_sQueueSample.field_54_sound_intensity = Fix16(10);
            field_30_sQueueSample.field_64_max_distance = 20;
            dist = Fix16(100);
            vol = 50;
            break;
        case 17:
            field_30_sQueueSample.field_14_samp_idx = 151;
            field_30_sQueueSample.field_30_loop_count = 1;
            field_30_sQueueSample.field_41_releasing_sound_flag = 1;
            field_30_sQueueSample.field_1C_ReleasingVolumeModificator = 7;
            field_30_sQueueSample.field_54_sound_intensity = Fix16(10);
            field_30_sQueueSample.field_64_max_distance = 20;
            dist = Fix16(100);
            vol = 50;
            break;
        case 7:
            field_30_sQueueSample.field_14_samp_idx = 148;
            field_30_sQueueSample.field_30_loop_count = 1;
            field_30_sQueueSample.field_41_releasing_sound_flag = 1;
            field_30_sQueueSample.field_1C_ReleasingVolumeModificator = 20;
            field_30_sQueueSample.field_54_sound_intensity = Fix16(5);
            field_30_sQueueSample.field_64_max_distance = 10;
            dist = Fix16(25);
            vol = 30;
            break;
        case 28:
            field_30_sQueueSample.field_14_samp_idx = 165;
            field_30_sQueueSample.field_30_loop_count = 1;
            field_30_sQueueSample.field_41_releasing_sound_flag = 1;
            field_30_sQueueSample.field_1C_ReleasingVolumeModificator = 10;
            field_30_sQueueSample.field_54_sound_intensity = Fix16(0x32000, 0);
            field_30_sQueueSample.field_64_max_distance = 25;
            dist = Fix16(0x271000, 0);
            vol = 105;
            break;
        case 11:
            field_30_sQueueSample.field_30_loop_count = 1;
            field_30_sQueueSample.field_41_releasing_sound_flag = 1;
            field_30_sQueueSample.field_1C_ReleasingVolumeModificator = 20;
            field_30_sQueueSample.field_54_sound_intensity = Fix16(10);
            field_30_sQueueSample.field_64_max_distance = 20;
            dist = Fix16(100);
            vol = 50;
            break;
        case 12:
            field_30_sQueueSample.field_30_loop_count = 1;
            field_30_sQueueSample.field_41_releasing_sound_flag = 1;
            field_30_sQueueSample.field_1C_ReleasingVolumeModificator = 20;
            field_30_sQueueSample.field_54_sound_intensity = Fix16(0x1e000, 0);
            field_30_sQueueSample.field_64_max_distance = 15;
            dist = Fix16(0xe1000, 0);
            vol = 40;
            break;
        case 20:
        case 24:
        case 52:
            field_30_sQueueSample.field_30_loop_count = 1;
            field_30_sQueueSample.field_41_releasing_sound_flag = 1;
            field_30_sQueueSample.field_1C_ReleasingVolumeModificator = 5;
            field_30_sQueueSample.field_54_sound_intensity = Fix16(0x1e000, 0);
            field_30_sQueueSample.field_64_max_distance = 15;
            dist = Fix16(0xe1000, 0);
            vol = 90;
            break;
        case 46:
            field_30_sQueueSample.field_30_loop_count = 1;
            field_30_sQueueSample.field_41_releasing_sound_flag = 1;
            field_30_sQueueSample.field_1C_ReleasingVolumeModificator = 5;
            field_30_sQueueSample.field_54_sound_intensity = Fix16(0x32000, 0);
            field_30_sQueueSample.field_64_max_distance = 25;
            dist = Fix16(0x271000, 0);
            vol = 60;
            break;
        case 21:
            field_30_sQueueSample.field_30_loop_count = 1;
            field_30_sQueueSample.field_41_releasing_sound_flag = 1;
            field_30_sQueueSample.field_1C_ReleasingVolumeModificator = 4;
            field_30_sQueueSample.field_54_sound_intensity = Fix16(10);
            field_30_sQueueSample.field_64_max_distance = 20;
            dist = Fix16(100);
            vol = 50;
            break;
        default:
            return;
    }

    if (!CalculateDistance_419020(dist))
    {
        return;
    }
    if (!VolCalc_419070(vol, field_30_sQueueSample.field_54_sound_intensity, false))
    {
        return;
    }

    switch (kind)
    {
        case 17:
            if (!gObjSoundDelayKind17_6751D0)
            {
                gObjSoundDelayKind17_6751D0 = static_cast<u8>(field_1454_anRandomTable[0] % 20 + 20);
            }
            else
            {
                gObjSoundDelayKind17_6751D0--;
                return;
            }
            break;
        case 7:
            if (!gObjSoundDelayKind7_6751D2)
            {
                gObjSoundDelayKind7_6751D2 = static_cast<u8>(field_1454_anRandomTable[1] % 12 + 10);
            }
            else
            {
                gObjSoundDelayKind7_6751D2--;
                return;
            }
            break;
        case 28:
            if (!gObjSoundDelayKind28_6751D4)
            {
                gObjSoundDelayKind28_6751D4 = static_cast<u8>(field_1454_anRandomTable[2] % 90 - 46);
            }
            else
            {
                gObjSoundDelayKind28_6751D4--;
                return;
            }
            break;
        case 11:
            if (!gObjSoundDelayKind11_6751D6)
            {
                field_30_sQueueSample.field_14_samp_idx = field_1454_anRandomTable[3] % 11 + 228;
                gObjSoundDelayKind11_6751D6 = static_cast<u8>(field_1454_anRandomTable[2] % 10 + 40);
            }
            else
            {
                gObjSoundDelayKind11_6751D6--;
                return;
            }
            break;
        case 12:
            if (!gObjSoundDelayKind12_6751D8)
            {
                field_30_sQueueSample.field_14_samp_idx = (field_1454_anRandomTable[2] & 3) + 181;
                gObjSoundDelayKind12_6751D8 = static_cast<u8>(field_1454_anRandomTable[3] % 90 + 30);
            }
            else
            {
                gObjSoundDelayKind12_6751D8--;
                return;
            }
            break;
        case 20:
            if (!gObjSoundDelayKind20_6751DA)
            {
                field_30_sQueueSample.field_14_samp_idx = field_1454_anRandomTable[1] % 5 + 152;
                gObjSoundDelayKind20_6751DA = static_cast<u8>(field_1454_anRandomTable[4] % 10 + 4);
            }
            else
            {
                gObjSoundDelayKind20_6751DA--;
                return;
            }
            break;
        case 24:
            if (!gObjSoundDelayKind24_6751DC)
            {
                field_30_sQueueSample.field_14_samp_idx = field_1454_anRandomTable[1] % 5 + 152;
                gObjSoundDelayKind24_6751DC = static_cast<u8>((field_1454_anRandomTable[2] & 7) + 5);
            }
            else
            {
                gObjSoundDelayKind24_6751DC--;
                return;
            }
            break;
        case 52:
            if (!gObjSoundDelayKind52_6751E2)
            {
                field_30_sQueueSample.field_14_samp_idx = field_1454_anRandomTable[1] % 3 + 154;
                gObjSoundDelayKind52_6751E2 = static_cast<u8>(field_1454_anRandomTable[2] % 30 + 45);
            }
            else
            {
                gObjSoundDelayKind52_6751E2--;
                return;
            }
            break;
        case 46:
            if (!gObjSoundDelayKind46_6751DE)
            {
                field_30_sQueueSample.field_14_samp_idx = field_1454_anRandomTable[4] % 6 + 303;
                gObjSoundDelayKind46_6751DE = static_cast<u8>(field_1454_anRandomTable[2] % 90 + 30);
            }
            else
            {
                gObjSoundDelayKind46_6751DE--;
                return;
            }
            break;
        case 21:
            if (!gObjSoundDelayKind21_6751E0)
            {
                field_30_sQueueSample.field_14_samp_idx = (field_1454_anRandomTable[2] & 3) + 157;
                gObjSoundDelayKind21_6751E0 = static_cast<u8>(field_1454_anRandomTable[3] % 40 + 90);
            }
            else
            {
                gObjSoundDelayKind21_6751E0--;
                return;
            }
            break;
    }

    if (field_30_sQueueSample.field_30_loop_count > 0)
    {
        if (++gType12SampleIndexCounter_6751E4 == 255)
        {
            gType12SampleIndexCounter_6751E4 = 0;
        }
        field_30_sQueueSample.field_4_SampleIndex = gType12SampleIndexCounter_6751E4;
    }

    field_30_sQueueSample.field_60_nEmittingVolume = vol;
    // No local for the sample index: with one, VC6 copy-propagates it into the RandomDisplacement
    // argument, where the original reloads field_14 after the first call
    field_30_sQueueSample.field_20_rate = gSampManager_6FFF00.GetPlayBackRate_58DBF0(field_30_sQueueSample.field_14_samp_idx) +
        RandomDisplacement_41A650(field_30_sQueueSample.field_14_samp_idx) + (field_30_sQueueSample.field_14_samp_idx % 10) * 40;
    if (kind == 45)
    {
        field_30_sQueueSample.field_20_rate += field_30_sQueueSample.field_20_rate;
    }
    field_30_sQueueSample.field_58_type = 20;
    field_30_sQueueSample.field_18_bIs2D = false;
    field_30_sQueueSample.field_34_loop_start = gSampManager_6FFF00.GetLoopStart_58DC30(field_30_sQueueSample.field_14_samp_idx);
    field_30_sQueueSample.field_38_loop_end = gSampManager_6FFF00.GetLoopEnd_58DC50(field_30_sQueueSample.field_14_samp_idx);
    AddSampleToRequestedQueue_41A850();
}

MATCH_FUNC(0x41F520)
void sound_obj::ProcessOtherObjects_41F520(Sound_Params* pParams)
{
    s32 samp_idx = 321;
    u8 max_distance = 10;
    u8 release_mod = 20;
    u8 sample_index = 0;
    s32 rate_displacement = 0;
    u8 volume = 0;
    bool bLoop = true;
    Fix16 emit_distance;
    Fix16 calc_distance;

    Object* pObj = pParams->field_0_pObj->field_8_object_2C_ptr;
    switch (pObj->field_18_model)
    {
        case objects::rocket_bullet_128:
            samp_idx = 191;
            bLoop = false;
            volume = 127;
            sample_index = 1;
            emit_distance = Fix16(14) / Fix16(2);
            max_distance = 14;
            calc_distance = (Fix16(14) / Fix16(2)) * (Fix16(14) / Fix16(2));
            release_mod = 15;
            break;

        case objects::blue_phone_164:
        case objects::red_phone_177:
        case objects::yellow_phone_179:
        case objects::green_phone_181:
            if (field_5448_m_FrameCounter % 130)
            {
                return;
            }
            rate_displacement = (reinterpret_cast<u32>(pParams) * 8) % 760;
            samp_idx = 30;
            volume = 85;
            emit_distance = Fix16(15) / Fix16(2);
            max_distance = 15;
            calc_distance = (Fix16(15) / Fix16(2)) * (Fix16(15) / Fix16(2));
            release_mod = 3;
            bLoop = true;
            break;

        case 185:
        case 187:
        case objects::yellow_phone_answered_189:
        case objects::green_phone_answered_191:
            UpdateRadioChatterLoop_41FCA0();
            return;

        case objects::fire_197:
            samp_idx = 189;
            volume = 50;
            bLoop = false;
            sample_index = 1;
            emit_distance = Fix16(14) / Fix16(2);
            max_distance = 14;
            calc_distance = (Fix16(14) / Fix16(2)) * (Fix16(14) / Fix16(2));
            release_mod = 15;
            break;

        case objects::explosion_113:
        {
            Explosion* pExplosion = pObj->field_C_pAny.pExplosion;
            if (!pExplosion)
            {
                return;
            }
            switch (pExplosion->field_10_type)
            {
                case explosion_type::small_18:
                case explosion_type::small_33:
                    if (field_147C_audio_entities[field_30_sQueueSample.field_0_EntityIndex].field_1_age == 2)
                    {
                        samp_idx = 186;
                        volume = 127;
                        bLoop = true;
                        emit_distance = Fix16(35) / Fix16(2);
                        max_distance = 35;
                        calc_distance = (Fix16(35) / Fix16(2)) * (Fix16(35) / Fix16(2));
                        release_mod = 0;
                        rate_displacement = RandomDisplacement_41A650(samp_idx);
                    }
                    else if (field_147C_audio_entities[field_30_sQueueSample.field_0_EntityIndex].field_1_age > 2)
                    {
                        samp_idx = 190;
                        bLoop = false;
                        volume = 50;
                        sample_index = 1;
                        emit_distance = Fix16(14) / Fix16(2);
                        max_distance = 14;
                        calc_distance = (Fix16(14) / Fix16(2)) * (Fix16(14) / Fix16(2));
                        release_mod = 15;
                    }
                    gExplosionSound_Unused_61A6CC = 50;
                    gExplosionSound_Unused_61A6D0 = 321;
                    break;

                case explosion_type::item_19:
                case explosion_type::building_45_22:
                case explosion_type::building_225_23:
                case explosion_type::building_135_24:
                case explosion_type::building_315_25:
                case explosion_type::no_ring_32:
                    if (field_147C_audio_entities[field_30_sQueueSample.field_0_EntityIndex].field_1_age == 2)
                    {
                        samp_idx = 187;
                        volume = 127;
                        bLoop = true;
                        emit_distance = Fix16(35) / Fix16(2);
                        max_distance = 35;
                        calc_distance = (Fix16(35) / Fix16(2)) * (Fix16(35) / Fix16(2));
                        release_mod = 0;
                        rate_displacement = RandomDisplacement_41A650(samp_idx);
                    }
                    else if (field_147C_audio_entities[field_30_sQueueSample.field_0_EntityIndex].field_1_age > 2)
                    {
                        samp_idx = 190;
                        volume = 127;
                        bLoop = false;
                        sample_index = 1;
                        emit_distance = Fix16(14) / Fix16(2);
                        max_distance = 14;
                        calc_distance = (Fix16(14) / Fix16(2)) * (Fix16(14) / Fix16(2));
                        release_mod = 15;
                    }
                    gExplosionSound_Unused_61A6CC = 50;
                    gExplosionSound_Unused_61A6D0 = 321;
                    break;

                case explosion_type::large_20:
                    if (field_147C_audio_entities[field_30_sQueueSample.field_0_EntityIndex].field_1_age == 2)
                    {
                        samp_idx = 188;
                        volume = 127;
                        bLoop = true;
                        emit_distance = Fix16(35) / Fix16(2);
                        max_distance = 35;
                        calc_distance = (Fix16(35) / Fix16(2)) * (Fix16(35) / Fix16(2));
                        release_mod = 0;
                        rate_displacement = RandomDisplacement_41A650(samp_idx);
                    }
                    else if (field_147C_audio_entities[field_30_sQueueSample.field_0_EntityIndex].field_1_age > 2)
                    {
                        samp_idx = 190;
                        volume = 127;
                        bLoop = false;
                        sample_index = 1;
                        emit_distance = Fix16(14) / Fix16(2);
                        max_distance = 14;
                        calc_distance = (Fix16(14) / Fix16(2)) * (Fix16(14) / Fix16(2));
                        release_mod = 15;
                    }
                    gExplosionSound_Unused_61A6CC = 50;
                    gExplosionSound_Unused_61A6D0 = 321;
                    break;

                case explosion_type::car_fire_4:
                case explosion_type::car_fire_level1_12:
                    samp_idx = 190;
                    volume = 50;
                    bLoop = false;
                    sample_index = 1;
                    emit_distance = Fix16(14) / Fix16(2);
                    max_distance = 14;
                    calc_distance = (Fix16(14) / Fix16(2)) * (Fix16(14) / Fix16(2));
                    release_mod = 15;
                    break;

                case explosion_type::car_fire_level2_13:
                case explosion_type::car_fire_level3_14:
                    samp_idx = 190;
                    volume = 85;
                    bLoop = false;
                    sample_index = 1;
                    emit_distance = Fix16(14) / Fix16(2);
                    max_distance = 14;
                    calc_distance = (Fix16(14) / Fix16(2)) * (Fix16(14) / Fix16(2));
                    release_mod = 15;
                    break;

                default:
                    return;
            }
            break;
        }

        case objects::blaster_5:
            samp_idx = 145;
            volume = 50;
            bLoop = false;
            emit_distance = Fix16(12) / Fix16(2);
            max_distance = 12;
            calc_distance = (Fix16(12) / Fix16(2)) * (Fix16(12) / Fix16(2));
            release_mod = 7;
            break;

        case objects::conveyor_139:
            samp_idx = 150;
            bLoop = false;
            volume = 20;
            emit_distance = Fix16(40) / Fix16(2);
            max_distance = 40;
            calc_distance = (Fix16(40) / Fix16(2)) * (Fix16(40) / Fix16(2));
            release_mod = 7;
            break;

        default:
            return;
    }

    if (samp_idx != 321)
    {
        if (CalculateDistance_419020(calc_distance))
        {
            if (VolCalc_419070(volume, emit_distance, pParams->field_5_bHasSolidAbove))
            {
                field_30_sQueueSample.field_60_nEmittingVolume = volume;
                field_30_sQueueSample.field_14_samp_idx = samp_idx;
                field_30_sQueueSample.field_54_sound_intensity = emit_distance;
                field_30_sQueueSample.field_64_max_distance = max_distance;
                field_30_sQueueSample.field_20_rate = gSampManager_6FFF00.GetPlayBackRate_58DBF0(samp_idx) + rate_displacement;
                field_30_sQueueSample.field_58_type = 20;
                field_30_sQueueSample.field_4_SampleIndex = sample_index;
                field_30_sQueueSample.field_41_releasing_sound_flag = bLoop;
                field_30_sQueueSample.field_1C_ReleasingVolumeModificator = release_mod;
                field_30_sQueueSample.field_18_bIs2D = false;
                field_30_sQueueSample.field_3C_speed_multiplier = 400;
                field_30_sQueueSample.field_34_loop_start = 0;
                field_30_sQueueSample.field_38_loop_end = -1;
                if (bLoop == 1)
                {
                    field_30_sQueueSample.field_30_loop_count = 1;
                }
                else
                {
                    field_30_sQueueSample.field_30_loop_count = 0;
                    field_30_sQueueSample.field_4C_releasing_volume_divider = 20;
                }
                AddSampleToRequestedQueue_41A850();
            }
        }
    }
}

void sound_obj::nullsub_4()
{
    ;
}

MATCH_FUNC(0x41FCA0)
void sound_obj::UpdateRadioChatterLoop_41FCA0()
{
    u32 rnd_3;
    u32 rnd_2;

    this->field_5520_bCanPlay = false;
    if (this->field_1454_anRandomTable[4] % 20)
    {
        if (!(this->field_1454_anRandomTable[0] % 3u))
        {
            rnd_3 = this->field_1454_anRandomTable[1] % 3 + 69;
            rnd_2 = this->field_1454_anRandomTable[2] % 3 + 87;
        }
        else
        {
            rnd_3 = this->field_1454_anRandomTable[1] % 5 + 115;
            rnd_2 = this->field_1454_anRandomTable[2] % 5 + 131;
        }
        Init_RadioMessageQueue_427180();
        gSampManager_6FFF00.SetSampleVol_58E7D0((this->field_24_sfx_vol * 50u) >> 7);
        gSampManager_6FFF00.PlaySampleRange_58E8C0(rnd_3, rnd_2);
    }
    else
    {
        gSampManager_6FFF00.EndSample_58E960();
    }
}

MATCH_FUNC(0x422B70)
void sound_obj::ProcessPed_422B70(Sound_Params* pParams)
{
    s32 first_samp;
    s32 last_samp;
    s32 samp_idx;
    s32 release_mod;
    Fix16 sound_intensity;
    Fix16 calc_distance;
    Fix16 zpos;
    s32 rate;
    u8 vol;
    s32 max_dist;

    Char* pChar = pParams->field_0_pObj->field_8_char_b4_ptr;
    if (pChar->field_7C_pPed)
    {
        if (!gGame_0x40_67E008->field_38_local_player->GetPlayerCar_5698E0() && pChar->get_velocity_41B080() > kFpZero)
        {
            const u32 animation_state = pChar->field_6C_animation_state;
            if (animation_state == 4 || animation_state == 1 || animation_state == 0)
            {
                const char_type animation_frame = pChar->field_68_animation_frame;
                if ((animation_frame == 1 || animation_frame == 5) && ((reinterpret_cast<s32>(pChar) + this->field_5448_m_FrameCounter) & 1) != 0)
                {
                    switch (pChar->field_7C_pPed->field_254_block_spec)
                    {
                        case 2:
                        case 0xA:
                            first_samp = 202;
                            last_samp = 205;
                            goto play_footstep;
                        case 7:
                            first_samp = 206;
                            last_samp = 209;
                            goto play_footstep;
                        case 5:
                        case 6:
                        case 8:
                        case 9:
                            first_samp = 194;
                            last_samp = 197;
                            goto play_footstep;
                        case 1:
                        case 3:
                            first_samp = 198;
                            last_samp = 201;
                        // shared by every surface case: goto-free forms need a flag or a copy of the block
                        play_footstep:
                            if (CalculateDistance_419020(Fix16(4)))
                            {
                                if (!pChar->field_6C_animation_state)
                                {
                                    max_dist = 30;
                                    vol = 5;
                                }
                                else
                                {
                                    max_dist = 17;
                                    vol = 20;
                                }
                                if (VolCalc_419070(vol, Fix16(2), pParams->field_5_bHasSolidAbove))
                                {
                                    const u32 samp_offset = this->field_1454_anRandomTable[this->field_30_sQueueSample.field_0_EntityIndex % 5u] %
                                        static_cast<u32>(last_samp - first_samp + 1);
                                    this->field_30_sQueueSample.field_14_samp_idx = first_samp + samp_offset;
                                    this->field_30_sQueueSample.field_54_sound_intensity = Fix16(2);
                                    this->field_30_sQueueSample.field_64_max_distance = 4;
                                    this->field_30_sQueueSample.field_60_nEmittingVolume = vol;
                                    const s32 playback_rate = gSampManager_6FFF00.GetPlayBackRate_58DBF0(field_30_sQueueSample.field_14_samp_idx);
                                    const s32 base_rate = playback_rate + RandomDisplacement_41A650(field_30_sQueueSample.field_14_samp_idx);
                                    this->field_30_sQueueSample.field_58_type = 20;
                                    this->field_30_sQueueSample.field_20_rate = base_rate;
                                    if (!pChar->field_68_animation_frame)
                                    {
                                        this->field_30_sQueueSample.field_4_SampleIndex = 0;
                                    }
                                    else
                                    {
                                        this->field_30_sQueueSample.field_4_SampleIndex = 5;
                                        this->field_30_sQueueSample.field_20_rate = base_rate + 50;
                                    }
                                    this->field_30_sQueueSample.field_41_releasing_sound_flag = 1;
                                    this->field_30_sQueueSample.field_1C_ReleasingVolumeModificator = max_dist;
                                    this->field_30_sQueueSample.field_18_bIs2D = false;
                                    this->field_30_sQueueSample.field_34_loop_start = 0;
                                    this->field_30_sQueueSample.field_38_loop_end = -1;
                                    this->field_30_sQueueSample.field_30_loop_count = 1;
                                    sound_obj::AddSampleToRequestedQueue_41A850();
                                }
                            }
                            break;
                        default:
                            break;
                    }
                }
            }
        }

        const s32 scream_timer = pChar->field_B0_scream_timer;
        if (scream_timer > -1)
        {
            if (scream_timer > 0)
            {
                pChar->field_B0_scream_timer = scream_timer - 1;
            }
            else
            {
                if (CalculateDistance_419020(Fix16(81)))
                {
                    vol = this->field_1454_anRandomTable[this->field_30_sQueueSample.field_0_EntityIndex % 5u] % 0x17u + 30;
                    if (VolCalc_419070(vol, Fix16(9), pParams->field_5_bHasSolidAbove))
                    {
                        this->field_30_sQueueSample.field_14_samp_idx = this->field_1454_anRandomTable[this->field_30_sQueueSample.field_0_EntityIndex % 5u] % 6u + 233;
                        this->field_30_sQueueSample.field_54_sound_intensity = Fix16(9);
                        this->field_30_sQueueSample.field_60_nEmittingVolume = vol;
                        this->field_30_sQueueSample.field_64_max_distance = 18;
                        this->field_30_sQueueSample.field_20_rate = gSampManager_6FFF00.GetPlayBackRate_58DBF0(this->field_30_sQueueSample.field_14_samp_idx) +
                            sound_obj::RandomDisplacement_41A650(this->field_30_sQueueSample.field_14_samp_idx);
                        this->field_30_sQueueSample.field_58_type = 20;
                        this->field_30_sQueueSample.field_4_SampleIndex = 3;
                        this->field_30_sQueueSample.field_41_releasing_sound_flag = 1;
                        this->field_30_sQueueSample.field_1C_ReleasingVolumeModificator = 10;
                        this->field_30_sQueueSample.field_18_bIs2D = false;
                        this->field_30_sQueueSample.field_34_loop_start = 0;
                        this->field_30_sQueueSample.field_38_loop_end = -1;
                        this->field_30_sQueueSample.field_30_loop_count = 1;
                        sound_obj::AddSampleToRequestedQueue_41A850();
                    }
                }
                pChar->field_B0_scream_timer = this->field_1454_anRandomTable[3] % 0x1Eu + 20;
            }
        }

        switch (pChar->get_ped_state_2_41B090())
        {
            case ped_state_2::electrocuted_27:
                samp_idx = 58;
                this->field_30_sQueueSample.field_41_releasing_sound_flag = 0;
                this->field_30_sQueueSample.field_20_rate = gSampManager_6FFF00.GetPlayBackRate_58DBF0(58);
                vol = 40;
                release_mod = 15;
                sound_intensity = Fix16(286720, 0);
                calc_distance = Fix16(5017600, 0);
                max_dist = 35;
                break;

            case ped_state_2::knocked_fatal_26:
                if (!pChar->field_7C_pPed->is_player_41B0A0() && (this->field_30_sQueueSample.field_0_EntityIndex & 1) != 0)
                {
                    return;
                }
                zpos = pParams->field_0_pObj->field_1C_zpos;
                if (zpos > kFpSix_675418)
                {
                    zpos = kFpSix_675418;
                }
                if (zpos > kFpFive_675414)
                {
                    samp_idx = 193;
                    if (Fix16::Round_To_Int_410BF0(Fix16(40) * (zpos - kFpFive_675414)) > 127)
                    {
                        vol = 127;
                    }
                    else
                    {
                        vol = Fix16::Round_To_Int_410BF0(Fix16(40) * (zpos - kFpFive_675414));
                    }
                    release_mod = 5;
                    sound_intensity = Fix16(5);
                    calc_distance = Fix16(25);
                    this->field_30_sQueueSample.field_41_releasing_sound_flag = 0;
                    max_dist = 10;
                    rate = gSampManager_6FFF00.GetPlayBackRate_58DBF0(193);
                    this->field_30_sQueueSample.field_20_rate = rate;
                    this->field_30_sQueueSample.field_20_rate =
                        rate + Fix16::Round_To_Int_410BF0(pParams->field_0_pObj->field_1C_zpos * kFp400);
                    break;
                }
                // fall through: zpos too low, play the ped voice instead

            default:
                HandlePedVoiceEvent_423080(pParams);
                return;
        }

        if (sound_obj::CalculateDistance_419020(calc_distance))
        {
            if (sound_obj::VolCalc_419070(vol, sound_intensity, pParams->field_5_bHasSolidAbove))
            {
                this->field_30_sQueueSample.field_14_samp_idx = samp_idx;
                this->field_30_sQueueSample.field_54_sound_intensity = sound_intensity;
                this->field_30_sQueueSample.field_60_nEmittingVolume = vol;
                this->field_30_sQueueSample.field_64_max_distance = max_dist;
                this->field_30_sQueueSample.field_58_type = 20;
                this->field_30_sQueueSample.field_3C_speed_multiplier = 700;
                this->field_30_sQueueSample.field_4_SampleIndex = 1;
                this->field_30_sQueueSample.field_1C_ReleasingVolumeModificator = release_mod;
                this->field_30_sQueueSample.field_18_bIs2D = false;
                this->field_30_sQueueSample.field_38_loop_end = -1;
                if (this->field_30_sQueueSample.field_41_releasing_sound_flag == 1)
                {
                    this->field_30_sQueueSample.field_30_loop_count = 1;
                    this->field_30_sQueueSample.field_34_loop_start = 0;
                }
                else
                {
                    this->field_30_sQueueSample.field_34_loop_start = gSampManager_6FFF00.GetLoopStart_58DC30(samp_idx);
                    this->field_30_sQueueSample.field_30_loop_count = 0;
                    this->field_30_sQueueSample.field_4C_releasing_volume_divider = 5;
                }
                sound_obj::AddSampleToRequestedQueue_41A850();
            }
        }
    }
}

DEFINE_GLOBAL(u16, gObjSoundDelayKind17_6751D0, 0x6751D0);
DEFINE_GLOBAL(u16, gObjSoundDelayKind7_6751D2, 0x6751D2);
DEFINE_GLOBAL(u16, gObjSoundDelayKind28_6751D4, 0x6751D4);
DEFINE_GLOBAL(u16, gObjSoundDelayKind11_6751D6, 0x6751D6);
DEFINE_GLOBAL(u16, gObjSoundDelayKind12_6751D8, 0x6751D8);
DEFINE_GLOBAL(u16, gObjSoundDelayKind20_6751DA, 0x6751DA);
DEFINE_GLOBAL(u16, gObjSoundDelayKind24_6751DC, 0x6751DC);
DEFINE_GLOBAL(u16, gObjSoundDelayKind46_6751DE, 0x6751DE);
DEFINE_GLOBAL(u16, gObjSoundDelayKind21_6751E0, 0x6751E0);
DEFINE_GLOBAL(u16, gObjSoundDelayKind52_6751E2, 0x6751E2);
DEFINE_GLOBAL(u8, gType12SampleIndexCounter_6751E4, 0x6751E4);

// Plays what a ped says for the voice event in Ped::field_250 (shouts, screams, ...)
MATCH_FUNC(0x423080)
void sound_obj::HandlePedVoiceEvent_423080(Sound_Params* pParams)
{
    LOCAL_STATIC(u8, sCopVoice17Cooldown, 0x67554A);
    LOCAL_STATIC(s16, sElvisVoiceCooldown, 0x675548);
    LOCAL_STATIC(u8, sPedVoice1Cooldown, 0x67554B);
    LOCAL_STATIC(u8, sPedVoice22Cooldown, 0x67554C);
    Char* pChar = pParams->field_0_pObj->field_8_char_b4_ptr;
    Ped* pPed = pChar->field_7C_pPed;
    s32 voice = pPed->TakeVoiceEvent_41B0B0();
    bool bTank;
    s32 samp = 321;

    if (!pPed->is_player_41B0A0())
    {
        bTank = pPed->IsLawEnforcement_45B4E0() || pPed->get_occupation_403980() == ped_ocupation_enum::tank_driver ||
            pPed->get_occupation_403980() == ped_ocupation_enum::road_block_tank_man;
        switch (voice)
        {
            case 1:
                if (!bTank)
                {
                    if (!sPedVoice1Cooldown)
                    {
                        sPedVoice1Cooldown = 3;
                        samp = field_1454_anRandomTable[static_cast<u32>(field_30_sQueueSample.field_0_EntityIndex) % 5] % 11 + 210;
                    }
                    else
                    {
                        sPedVoice1Cooldown--;
                    }
                }
                break;
            case 2:
                if (!bTank && Cooldown_4236C0())
                {
                    samp = field_1454_anRandomTable[static_cast<u32>(field_30_sQueueSample.field_0_EntityIndex) % 5] % 3 + 221;
                }
                break;
            case 3:
                if (!bTank && Cooldown_4236C0())
                {
                    samp = field_1454_anRandomTable[static_cast<u32>(field_30_sQueueSample.field_0_EntityIndex) % 5] % 7 + 221;
                }
                break;
            case 4:
                samp = field_1454_anRandomTable[static_cast<u32>(field_30_sQueueSample.field_0_EntityIndex) % 5] % 5 + 228;
                break;
            case 5:
                if (!bTank)
                {
                    samp = field_1454_anRandomTable[static_cast<u32>(field_30_sQueueSample.field_0_EntityIndex) % 5] % 6 + 239;
                }
                break;
            case 7:
                if (!bTank && !(field_5448_m_FrameCounter & 1))
                {
                    samp = field_1454_anRandomTable[static_cast<u32>(field_30_sQueueSample.field_0_EntityIndex) % 5] % 5 + 245;
                }
                break;
            case 8:
                if (!bTank && (field_5448_m_FrameCounter & 1) == 1)
                {
                    samp = static_cast<u32>(field_30_sQueueSample.field_0_EntityIndex) % 7 + 250;
                }
                break;
            case 9:
            case 10:
                if (!bTank && Cooldown_4236C0())
                {
                    samp = field_1454_anRandomTable[static_cast<u32>(field_30_sQueueSample.field_0_EntityIndex) % 5] % 8 + 257;
                }
                break;
            case 11:
                if (!bTank && Cooldown_4236C0())
                {
                    samp = field_1454_anRandomTable[static_cast<u32>(field_30_sQueueSample.field_0_EntityIndex) % 5] % 7 + 265;
                }
                break;
            case 12:
                if (!bTank)
                {
                    samp = field_1454_anRandomTable[static_cast<u32>(field_30_sQueueSample.field_0_EntityIndex) % 5] % 5 + 272;
                }
                break;
            case 13:
                if (!bTank)
                {
                    samp = field_1454_anRandomTable[static_cast<u32>(field_30_sQueueSample.field_0_EntityIndex) % 5] % 3 + 277;
                }
                break;
            case 14:
                if (!bTank)
                {
                    samp = field_1454_anRandomTable[static_cast<u32>(field_30_sQueueSample.field_0_EntityIndex) % 5] % 3 + 280;
                }
                break;
            case 15:
                if (!bTank)
                {
                    samp = field_1454_anRandomTable[static_cast<u32>(field_30_sQueueSample.field_0_EntityIndex) % 5] % 2 + 283;
                }
                break;
            case 17:
                if (bTank)
                {
                    if (!sCopVoice17Cooldown)
                    {
                        samp = field_1454_anRandomTable[static_cast<u32>(field_30_sQueueSample.field_0_EntityIndex) % 5] % 14 + 285;
                        sCopVoice17Cooldown = 3;
                    }
                    else
                    {
                        sCopVoice17Cooldown--;
                    }
                }
                break;
            case 22:
                if (!bTank)
                {
                    if (!sPedVoice22Cooldown)
                    {
                        sPedVoice22Cooldown = 3;
                        samp = field_1454_anRandomTable[static_cast<u32>(field_30_sQueueSample.field_0_EntityIndex) % 5] % 4 + 299;
                    }
                    else
                    {
                        sPedVoice22Cooldown--;
                    }
                }
                break;
            case 25:
                samp = 59;
                break;
        }

        s32 occupation = pPed->get_occupation_403980();
        if (occupation == ped_ocupation_enum::elvis || occupation == ped_ocupation_enum::elvis_leader)
        {
            if (!sElvisVoiceCooldown)
            {
                samp = field_1454_anRandomTable[0] % 6 + 303;
                sElvisVoiceCooldown = 275;
            }
            sElvisVoiceCooldown--;
        }
    }
    else if (voice == 20)
    {
        samp = field_1454_anRandomTable[0] % 2 + 309;
    }
    else if (voice == 25)
    {
        samp = 59;
    }

    if (voice == 26)
    {
        switch (pChar->field_7C_pPed->field_254_block_spec)
        {
            case 2:
            case 10:
                samp = 202;
                break;
            case 7:
                samp = 206;
                break;
            case 5:
            case 6:
            case 8:
            case 9:
                samp = 194;
                break;
            case 4:
                field_30_sQueueSample.field_14_samp_idx = 68;
                break;
            case 1:
            case 3:
                samp = 198;
                break;
        }
    }
    else if (voice == 28)
    {
        samp = 68;
    }
    else if (voice == 27)
    {
        samp = 41;
    }

    if (samp != 321 && CalculateDistance_419020(Fix16(81)))
    {
        u8 vol;
        if (samp == 309 || samp == 310)
        {
            vol = 25;
        }
        else if (voice == 27)
        {
            vol = 50;
        }
        else if (voice == 26)
        {
            vol = 50;
        }
        else if (voice == 28)
        {
            vol = 35;
        }
        else
        {
            vol = voice == 4 ? 35 : bTank ? 62 : 40;
        }

        if (VolCalc_419070(vol, Fix16(9), pParams->field_5_bHasSolidAbove))
        {
            field_30_sQueueSample.field_14_samp_idx = samp;
            field_30_sQueueSample.field_54_sound_intensity = Fix16(9);
            field_30_sQueueSample.field_60_nEmittingVolume = vol;
            field_30_sQueueSample.field_64_max_distance = 18;
            field_30_sQueueSample.field_20_rate =
                gSampManager_6FFF00.GetPlayBackRate_58DBF0(field_30_sQueueSample.field_14_samp_idx) +
                RandomDisplacement_41A650(field_30_sQueueSample.field_14_samp_idx);
            field_30_sQueueSample.field_58_type = 20;
            field_30_sQueueSample.field_3C_speed_multiplier = 0;
            field_30_sQueueSample.field_4_SampleIndex = 2;
            field_30_sQueueSample.field_41_releasing_sound_flag = 1;
            field_30_sQueueSample.field_1C_ReleasingVolumeModificator = 7;
            field_30_sQueueSample.field_18_bIs2D = false;
            field_30_sQueueSample.field_34_loop_start = 0;
            field_30_sQueueSample.field_38_loop_end = -1;
            field_30_sQueueSample.field_30_loop_count = 1;
            AddSampleToRequestedQueue_41A850();
        }
    }
}

MATCH_FUNC(0x4236C0)
EXPORT bool Cooldown_4236C0()
{
    LOCAL_STATIC(u8, sCooldownCounter, 0x67554D);
    if (sCooldownCounter > 0)
    {
        --sCooldownCounter;
        return false;
    }
    else
    {
        sCooldownCounter = 6;
        return true;
    }
}

DEFINE_GLOBAL_INIT(s32, gExplosionSound_Unused_61A6CC, 340, 0x61A6CC);
DEFINE_GLOBAL_INIT(s32, gExplosionSound_Unused_61A6D0, 321, 0x61A6D0);

MATCH_FUNC(0x426750)
void sound_obj::ResetRadioMessageState_426750()
{
    Init_RadioMessageQueue_427180();
    this->field_5520_bCanPlay = true;
    this->field_5524_radio_word = 0;
    this->field_556A_erv_en_route_timer = 0;
    this->field_5570_timer = 0;
    this->field_556E_timer = 0;
    this->field_556C_timer = 0;
}

// Builds the police radio messages for the player's wanted level and the crimes reported nearby
MATCH_FUNC(0x426790)
void sound_obj::PoliceRadioMessageGeneration_426790()
{
    LOCAL_STATIC(u16, sCrimeCalloutCooldown, 0x6758A8);
    Ped* pPed = gGame_0x40_67E008->field_38_local_player->GetPlayerPed_41D020();
    s32 crime_type;
    Fix16 crime_x;
    Fix16 crime_y;
    Fix16 crime_z;
    s32 best_crime = 0;
    u8 xpos = 0;
    u8 ypos = 0;

    while (gCrimeReportQueue_67A4B8->TryPopOldestReport(&crime_type, &crime_x, &crime_y, &crime_z) == 1)
    {
        if (crime_type)
        {
            if (crime_type == crime_stats_type::gang_members_killed_9)
            {
                if (best_crime)
                {
                    continue;
                }
                best_crime = crime_type;
            }
            else
            {
                if (best_crime >= crime_type)
                {
                    continue;
                }
                best_crime = crime_type;
            }
            xpos = crime_x.ToInt();
            ypos = crime_y.ToInt();
        }
    }

    s16 stars = pPed->get_wanted_star_count_46EF00();
    if (sCrimeCalloutCooldown > 0)
    {
        sCrimeCalloutCooldown--;
    }

    if (stars > 0)
    {
        if (field_556A_erv_en_route_timer)
        {
            if (--field_556A_erv_en_route_timer == 0)
            {
                AppendRadioMessageSuffix_4273B0();
            }
        }
        if (field_556E_timer)
        {
            field_556E_timer--;
        }
        if (field_5570_timer)
        {
            field_5570_timer--;
        }
        if (field_556C_timer)
        {
            field_556C_timer--;
        }

        if (best_crime)
        {
            if (best_crime == crime_stats_type::lawmen_killed_8 && !field_556A_erv_en_route_timer)
            {
                field_5569_crime_ypos = ypos;
                field_5568_crime_xpos = xpos;
                field_556A_erv_en_route_timer = (field_1454_anRandomTable[0] % 10 + 15) * 100;
            }

            if (stars > field_5522_wanted_star_count)
            {
                nullsub_4();
                field_5570_timer = field_1454_anRandomTable[2] % 1067 + 150;
                field_556C_timer = field_1454_anRandomTable[2] % 1530 + 600;
                sCrimeCalloutCooldown = field_1454_anRandomTable[3] % 50 + 889;
                switch (stars)
                {
                    case 1:
                        EnqueueRadioWord_4271B0(120);
                        EnqueueRadioWord_4271B0(93);
                        EnqueueRadioWord_4271B0(field_5521_radio_word + 74);
                        EnqueueRadioCrimeCallout_427340(best_crime, xpos, ypos);
                        break;
                    case 2:
                        if (GetFreeRadioWordSlots_427310() >= 9)
                        {
                            EnqueueRadioWord_4271B0(120);
                            EnqueueRadioWord_4271B0(93);
                            EnqueueRadioWord_4271B0(field_5521_radio_word + 74);
                            EnqueueRadioWord_4271B0(72);
                            EnqueueRadioWord_4271B0(73);
                            EnqueueRadioWord_4271B0(field_5524_radio_word + 73);
                            EnqueueRadioLocationPhrase_426E10(xpos, ypos);
                        }
                        break;
                    case 3:
                        if (GetFreeRadioWordSlots_427310() >= 9)
                        {
                            EnqueueRadioWord_4271B0(120);
                            EnqueueRadioWord_4271B0(92);
                            if (!(field_1454_anRandomTable[2] & 1))
                            {
                                EnqueueRadioWord_4271B0(89);
                                EnqueueRadioLocationPhrase_426E10(xpos, ypos);
                                EnqueueRadioWord_4271B0(71);
                                EnqueueRadioWord_4271B0(73);
                                EnqueueRadioWord_4271B0(field_5524_radio_word + 73);
                            }
                            else
                            {
                                EnqueueRadioWord_4271B0(71);
                                EnqueueRadioWord_4271B0(73);
                                EnqueueRadioWord_4271B0(field_5524_radio_word + 73);
                                EnqueueRadioWord_4271B0(88);
                                EnqueueRadioLocationPhrase_426E10(xpos, ypos);
                            }
                        }
                        break;
                    case 4:
                        if (GetFreeRadioWordSlots_427310() >= 6)
                        {
                            EnqueueRadioWord_4271B0(120);
                            EnqueueRadioWord_4271B0(92);
                            EnqueueRadioWord_4271B0(90);
                            EnqueueRadioWord_4271B0(120);
                            EnqueueRadioWord_4271B0(97);
                            EnqueueRadioWord_4271B0(91);
                        }
                        break;
                    case 5:
                        if (GetFreeRadioWordSlots_427310() >= 6)
                        {
                            EnqueueRadioWord_4271B0(120);
                            EnqueueRadioWord_4271B0(92);
                            EnqueueRadioWord_4271B0(90);
                            EnqueueRadioWord_4271B0(120);
                            EnqueueRadioWord_4271B0(96);
                            EnqueueRadioWord_4271B0(91);
                        }
                        break;
                    case 6:
                        if (GetFreeRadioWordSlots_427310() >= 6)
                        {
                            EnqueueRadioWord_4271B0(120);
                            EnqueueRadioWord_4271B0(92);
                            EnqueueRadioWord_4271B0(90);
                            EnqueueRadioWord_4271B0(120);
                            EnqueueRadioWord_4271B0(94);
                            EnqueueRadioWord_4271B0(91);
                        }
                        break;
                }
            }
            else
            {
                Car* pCar = gGame_0x40_67E008->field_38_local_player->GetPlayerCar_5698E0();
                nullsub_4();
                switch (field_1454_anRandomTable[3] % 10)
                {
                    case 0:
                        if (!field_556E_timer)
                        {
                            GenerateRadioVehicleDescription_426F20(pCar);
                            field_556E_timer = field_1454_anRandomTable[2] % 1000 + 300;
                        }
                        break;
                    case 1:
                        if (!sCrimeCalloutCooldown)
                        {
                            EnqueueRadioCrimeCallout_427340(best_crime, xpos, ypos);
                            sCrimeCalloutCooldown = field_1454_anRandomTable[3] % 50 + 889;
                        }
                        break;
                    case 2:
                        if (best_crime == crime_stats_type::weapon_fired_2 && !field_556C_timer)
                        {
                            if (GetFreeRadioWordSlots_427310() > 3)
                            {
                                EnqueueRadioWord_4271B0(121);
                                EnqueueRadioWord_4271B0(98);
                                EnqueueRadioWord_4271B0(99);
                                EnqueueRadioWord_4271B0(103);
                            }
                            field_556C_timer = field_1454_anRandomTable[2] % 1530 + 600;
                        }
                        break;
                    case 3:
                        if (!field_5570_timer)
                        {
                            if (GetFreeRadioWordSlots_427310() > 6)
                            {
                                EnqueueRadioWord_4271B0(121);
                                EnqueueRadioWord_4271B0(98);
                                EnqueueRadioWord_4271B0(102);
                                EnqueueRadioWord_4271B0(88);
                                EnqueueRadioLocationPhrase_426E10(xpos, ypos);
                            }
                            field_5570_timer = field_1454_anRandomTable[2] % 1067 + 150;
                        }
                        break;
                    case 7:
                        if (!sCrimeCalloutCooldown)
                        {
                            EnqueueRadioCrimeCallout_427340(best_crime, xpos, ypos);
                            sCrimeCalloutCooldown = field_1454_anRandomTable[3] % 680 + 628;
                            field_5570_timer = field_1454_anRandomTable[2] % 1067 + 150;
                        }
                        break;
                }
            }
        }
    }
    else if (field_5522_wanted_star_count > stars)
    {
        ResetRadioMessageState_426750();
    }
    else if (best_crime > 0 && !(field_1454_anRandomTable[4] % 15) && !sCrimeCalloutCooldown)
    {
        EnqueueRadioCrimeCallout_427340(best_crime, xpos, ypos);
        sCrimeCalloutCooldown = field_1454_anRandomTable[3] % 80 + 228;
    }

    field_5522_wanted_star_count = stars;
    if (best_crime > field_5524_radio_word)
    {
        if (best_crime != 9)
        {
            field_5524_radio_word = best_crime;
        }
        else if (!field_5524_radio_word)
        {
            field_5524_radio_word = 9;
        }
    }
    ProcessPoliceRadioWordsPlayback_427220();
}

MATCH_FUNC(0x426E10)
void sound_obj::EnqueueRadioLocationPhrase_426E10(u8 xpos, u8 ypos)
{
    bool bQueuedYDirection = false;
    gmp_map_zone* pZone = NULL;
    const u32 cop_zone = sound_obj::GetCopRadioZoneIndex_427400(xpos, ypos, &pZone);
    if (!cop_zone && !pZone)
    {
        sound_obj::EnqueueRadioWord_4271B0(0x75u);
        sound_obj::EnqueueRadioWord_4271B0(0x77u);
        return;
    }

    const u32 word_zone_name = cop_zone + 121;

    const s16 mid_x = pZone->field_1_x + (pZone->field_3_w >> 1);
    const s16 mid_y = pZone->field_2_y + (pZone->field_4_h >> 1);
    const s16 w_half = pZone->field_3_w >> 2;
    const s16 h_half = pZone->field_4_h >> 2;

    if (ypos < mid_y - h_half)
    {
        sound_obj::EnqueueRadioWord_4271B0(0x73u);
        bQueuedYDirection = true;
    }
    else if (ypos > h_half + mid_y)
    {
        sound_obj::EnqueueRadioWord_4271B0(0x75u);
        bQueuedYDirection = true;
    }

    if (xpos > mid_x + w_half)
    {
        sound_obj::EnqueueRadioWord_4271B0(0x74u);
    }
    else if (xpos < mid_x - w_half)
    {
        sound_obj::EnqueueRadioWord_4271B0(0x76u);
    }
    else if (!bQueuedYDirection)
    {
        sound_obj::EnqueueRadioWord_4271B0(0x77u);
    }
    sound_obj::EnqueueRadioWord_4271B0(word_zone_name);
}

MATCH_FUNC(0x426F20)
void sound_obj::GenerateRadioVehicleDescription_426F20(Car* pCar)
{
    LOCAL_STATIC(u16, sLastRadioCarRemap, 0x6757FC);
    u32 car_name_word;
    u32 car_colour_word;

    u32 extra_name_word = 321;
    if (!pCar)
    {
        if (this->field_5574_car_info_idx != car_model_enum::none)
        {
            if (sound_obj::GetFreeRadioWordSlots_427310() > 3u)
            {
                sound_obj::EnqueueRadioWord_4271B0(0x79u);
                sound_obj::EnqueueRadioWord_4271B0(0x62u);
                sound_obj::EnqueueRadioWord_4271B0(0x63u);
                sound_obj::EnqueueRadioWord_4271B0(0x6Eu);
            }
            this->field_5574_car_info_idx = car_model_enum::none;
        }
    }
    else
    {
        const s32 car_info_idx = pCar->GetCarInfoIdx_411940();
        if (this->field_5574_car_info_idx != car_info_idx || sLastRadioCarRemap != static_cast<u16>(pCar->field_50_car_sprite->get_remap_41C1F0()))
        {
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
                    extra_name_word = 114;
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
                case car_model_enum::none:
                    // Dead store, but it keeps 'none' as its own entry in the switch table as in the original
                    car_name_word = 111;
                    return;
                default:
                    return;
            }

            sLastRadioCarRemap = static_cast<u16>(pCar->field_50_car_sprite->get_remap_41C1F0());
            switch (sLastRadioCarRemap)
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

            if (sound_obj::GetFreeRadioWordSlots_427310() > 6u)
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
                if (extra_name_word != 321)
                {
                    sound_obj::EnqueueRadioWord_4271B0(extra_name_word);
                }
            }
        }
    }
}

MATCH_FUNC(0x427180)
void sound_obj::Init_RadioMessageQueue_427180()
{
    for (s32 i = 0; i < 15; i++)
    {
        field_552C_radio_word_queue[i] = 121;
    }
    field_5528_radio_word_read_idx = 0;
    field_5529_radio_word_write_idx = 0;
}

MATCH_FUNC(0x4271B0)
void sound_obj::EnqueueRadioWord_4271B0(u32 val)
{
    if (val > 136)
    {
        val = 136;
    }
    else if (val < 69)
    {
        val = 69;
    }

    const s32 write_idx = field_5529_radio_word_write_idx;
    if ((write_idx + 1) % 15 != field_5528_radio_word_read_idx)
    {
        field_552C_radio_word_queue[write_idx] = val;
        field_5529_radio_word_write_idx = static_cast<u8>(field_5529_radio_word_write_idx + 1) % 15;
    }
}

MATCH_FUNC(0x427220)
void sound_obj::ProcessPoliceRadioWordsPlayback_427220()
{
    if (!(field_5448_m_FrameCounter % 10u) && !field_5520_bCanPlay && !gSampManager_6FFF00.SampleNotDone_58E880())
    {
        gSampManager_6FFF00.EndSample_58E960();
        field_5520_bCanPlay = true;
    }

    if (field_5528_radio_word_read_idx != field_5529_radio_word_write_idx && !gSampManager_6FFF00.SampleNotDone_58E880() && field_5520_bCanPlay == 1)
    {
        // The original keeps this dead load + store of `old`, scheduled like a plain (non volatile) store:
        // `cmp $0xF,%al` goes before it. `old` must be address-taken for VC6 to keep the store, and
        // `&old - &old` below does that while folding to 0 only after dead store elimination has run.
        // (`volatile` also keeps the store, but its barrier pins the cmp below it.)
        s32 old = field_552C_radio_word_queue[field_5528_radio_word_read_idx];
        if (field_5528_radio_word_read_idx >= 15)
        {
            field_5528_radio_word_read_idx = 15;
        }

        gSampManager_6FFF00.PlayAtIdx_58E7F0(field_552C_radio_word_queue[field_5528_radio_word_read_idx] + (&old - &old));
        gSampManager_6FFF00.SetSampleVol_58E7D0((110 * static_cast<u32>(field_24_sfx_vol)) / 128);
        field_5528_radio_word_read_idx = (field_5528_radio_word_read_idx + 1) % 15;
    }
}

MATCH_FUNC(0x427310)
u8 sound_obj::GetFreeRadioWordSlots_427310() const
{
    // note: return value and'ed with 0xFF if return type is wider
    if (field_5529_radio_word_write_idx < field_5528_radio_word_read_idx)
    {
        return field_5528_radio_word_read_idx - field_5529_radio_word_write_idx - 1;
    }
    else
    {
        return field_5528_radio_word_read_idx - field_5529_radio_word_write_idx + 14;
    }
}

MATCH_FUNC(0x427340)
void sound_obj::EnqueueRadioCrimeCallout_427340(s32 crime_word, u8 loc_a, u8 loc_b)
{
    if (GetFreeRadioWordSlots_427310() >= 8u)
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
        EnqueueRadioWord_4271B0(crime_word + 73);
        EnqueueRadioWord_4271B0(88u);
        EnqueueRadioLocationPhrase_426E10(loc_a, loc_b);
    }
}

MATCH_FUNC(0x4273B0)
void sound_obj::AppendRadioMessageSuffix_4273B0()
{
    if (GetFreeRadioWordSlots_427310() > 3u)
    {
        EnqueueRadioWord_4271B0(0x79u);
        EnqueueRadioWord_4271B0(0x5Fu);
        EnqueueRadioWord_4271B0(0x5Bu);
        EnqueueRadioWord_4271B0(0x78u);
    }
    field_5521_radio_word = (field_5521_radio_word + 1) % 13;
}

MATCH_FUNC(0x427400)
u32 sound_obj::GetCopRadioZoneIndex_427400(u8 x, u8 y, gmp_map_zone** ppZone) const
{
    *ppZone = gMap_0x370_6F6268->GetZoneAtPosOfType_4DF4D0(x, y, 15u);
    if (!(*ppZone) || (*ppZone)->field_5_name_length != 3)
    {
        *ppZone = gMap_0x370_6F6268->GetZoneAtPosOfType_4DF4D0(x, y, 1u);
    }

    if (!*ppZone || (*ppZone)->field_5_name_length != 3 || (*ppZone)->field_6_name[0] != 'm')
    {
        *ppZone = NULL;
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
    *ppZone = NULL;
    return 0;
}

MATCH_FUNC(0x42A500)
void sound_obj::ProcessType7_Weapon_42A500(s32 idx)
{
    s32 samp_idx;

    u8 vol = 40;
    s32 rate_adjust = 0;
    Weapon* pWeapon = field_147C_audio_entities[idx].field_4_pObj->field_C_pAny.pWeapon_30;
    this->field_30_sQueueSample.field_41_releasing_sound_flag = 2;
    const char_type shot_fired = pWeapon->TakeShotFired_41CC80();
    if (shot_fired)
    {
        if (pWeapon)
        {
            if (!pWeapon->IsTargetHit_41CC70()) // bSuppressSound ?
            {
                switch (pWeapon->GetWeaponType_41CC90())
                {
                    case weapon_type::pistol:
                    case weapon_type::dual_pistol:
                        samp_idx = 311;
                        break;
                    case weapon_type::shotgun:
                        samp_idx = 314;
                        vol = 44;
                        rate_adjust = -8000;
                        break;
                    case weapon_type::smg:
                    case weapon_type::car_smg:
                    case weapon_type::army_gun_jeep:
                        samp_idx = 312;
                        vol = 44;
                        break;
                    case weapon_type::silence_smg:
                        samp_idx = 313;
                        vol = 40;
                        break;
                    case weapon_type::flamethrower:
                    case weapon_type::fire_truck_flamethrower:
                        samp_idx = 316;
                        vol = 120;
                        this->field_30_sQueueSample.field_41_releasing_sound_flag = 0;
                        break;
                    case weapon_type::fire_truck_gun:
                        samp_idx = 317;
                        this->field_30_sQueueSample.field_41_releasing_sound_flag = 0;
                        break;
                    case weapon_type::shocker:
                        samp_idx = 318;
                        this->field_30_sQueueSample.field_41_releasing_sound_flag = 0;
                        break;
                    case weapon_type::rocket:
                    case weapon_type::tank_main_gun:
                        samp_idx = 315;
                        break;
                    case weapon_type::car_bomb:
                    case weapon_type::car_mines:
                    case weapon_type::weapon_0x17:
                        samp_idx = 319;
                        break;
                    case weapon_type::oil_stain:
                        samp_idx = 61;
                        break;
                    default:
                        return;
                }

                pWeapon->GetSoundPos_5E3F90(&this->field_30_sQueueSample.field_8_obj.field_0_x,
                                            &this->field_30_sQueueSample.field_8_obj.field_4_y,
                                            &this->field_30_sQueueSample.field_8_obj.field_8_z);
                this->field_30_sQueueSample.field_0_EntityIndex = idx;
                this->field_30_sQueueSample.field_5C_pReleaseSource = NULL;
                const bool bHasSolidAbove = gMap_0x370_6F6268->CheckColumnHasSolidAbove_4E7FC0(field_30_sQueueSample.field_8_obj.field_0_x,
                                                                                    field_30_sQueueSample.field_8_obj.field_4_y,
                                                                                    field_30_sQueueSample.field_8_obj.field_8_z);
                this->field_28_dist_squared = ComputeEmitterDistanceSquared_4190B0();
                this->field_2C_distCalculated = false;
                if (CalculateDistance_419020((Fix16(20) / Fix16(2)) * (Fix16(20) / Fix16(2))))
                {
                    if (VolCalc_419070(vol, Fix16(20) / Fix16(2), bHasSolidAbove))
                    {
                        this->field_30_sQueueSample.field_60_nEmittingVolume = vol;
                        this->field_30_sQueueSample.field_14_samp_idx = samp_idx;
                        this->field_30_sQueueSample.field_54_sound_intensity = Fix16(20) / Fix16(2);
                        this->field_30_sQueueSample.field_64_max_distance = 20;
                        // One expression: VC6 calls GetPlayBackRateIdx first and adds rate_adjust to it after
                        // the RandomDisplacement call
                        this->field_30_sQueueSample.field_20_rate =
                            gSampManager_6FFF00.GetPlayBackRate_58DBF0(this->field_30_sQueueSample.field_14_samp_idx) + rate_adjust +
                            RandomDisplacement_41A650(this->field_30_sQueueSample.field_14_samp_idx);
                        this->field_30_sQueueSample.field_58_type = 20;
                        this->field_30_sQueueSample.field_3C_speed_multiplier = 0;
                        this->field_30_sQueueSample.field_4_SampleIndex = 0;
                        this->field_30_sQueueSample.field_1C_ReleasingVolumeModificator = 4;
                        this->field_30_sQueueSample.field_18_bIs2D = false;
                        this->field_30_sQueueSample.field_34_loop_start = gSampManager_6FFF00.GetLoopStart_58DC30(this->field_30_sQueueSample.field_14_samp_idx);
                        const char_type releasing_flag = this->field_30_sQueueSample.field_41_releasing_sound_flag;
                        this->field_30_sQueueSample.field_38_loop_end = -1;
                        this->field_30_sQueueSample.field_4C_releasing_volume_divider = 5;
                        if (static_cast<u8>(releasing_flag) > 0)
                        {
                            this->field_30_sQueueSample.field_30_loop_count = 1;
                        }
                        else
                        {
                            this->field_30_sQueueSample.field_30_loop_count = 0;
                        }
                        AddSampleToRequestedQueue_41A850();
                    }
                }
            }
        }
    }
}

MATCH_FUNC(0x57DD50)
void sound_obj::ProcessType3_CopRadioAndMusic_57DD50()
{
    LOCAL_STATIC(Car*, sLastPlayerCar, 0x6FF53C);
    if (gSoundSwitchRadioCoolDown_6FF539 > 0)
    {
        gSoundSwitchRadioCoolDown_6FF539--;
    }

    if (!gGame_0x40_67E008 || !gGame_0x40_67E008->field_38_local_player)
    {
        return;
    }

    if (!field_1_isPaused)
    {
        PoliceRadioMessageGeneration_426790();
    }
    else
    {
        gSampManager_6FFF00.EndSample_58E8A0();
    }

    if (!gSampManager_6FFF00.MusicFileExists_58E500())
    {
        return;
    }

    field_54F4_bWasInRadioCar = field_54F5_bInRadioCar;
    field_54F7_active_emitter_idx[1] = field_54F7_active_emitter_idx[0];
    field_5500_prev_main_variant = field_54FC_main_variant;
    TickRadioStationClocks_57E680();

    Car* pCar = gGame_0x40_67E008->field_38_local_player->GetPlayerCar_5698E0();
    if (pCar)
    {
        // An array keeps VC6 from folding the damage check into the branch: the original computes both
        // flags (sete) before testing either.
        bool flags[2];
        flags[0] = IsTrainOrBoxcar_57F120(pCar);
        flags[1] = pCar->IsMaxDamage_40F890();
        if (!flags[0] && !flags[1])
        {
            field_54F5_bInRadioCar = 1;
            if (!field_54F4_bWasInRadioCar)
            {
                ChooseRadioEmitterForVehicle_57E6C0();
                UpdateActiveRadioEmitterVolume_57EA90();
                field_5506_mute_timer = 0;
            }
            else if (field_5504_radio_station_change_mode)
            {
                if (field_54F7_active_emitter_idx[0] < 5)
                {
                    field_5468_radio_emitters[field_54F7_active_emitter_idx[0]].field_14_elapsed_frames = 0;
                }
                SelectBestRadioEmitter_57EF60();
            }
            else
            {
                UpdateActiveRadioEmitterVolume_57EA90();
            }

            if (!field_54F6_radio_volume)
            {
                field_5504_radio_station_change_mode = 1;
                SelectBestRadioEmitter_57EF60();
                field_5506_mute_timer = 0;
            }

            HandleVocalStreamSwitching_57DF10(pCar->field_68_scale != kFpOne_6FF3F4);
            sLastPlayerCar = pCar;
            return;
        }
    }

    field_54F5_bInRadioCar = 0;
    if (field_54F4_bWasInRadioCar == 1)
    {
        if (sLastPlayerCar)
        {
            sLastPlayerCar->field_B0_emitter_status = field_5468_radio_emitters[field_54F7_active_emitter_idx[0]].field_C_station_idx;
        }
        if (field_54F7_active_emitter_idx[1] < 5)
        {
            field_5468_radio_emitters[field_54F7_active_emitter_idx[1]].field_14_elapsed_frames = 0;
        }
    }
    field_54F7_active_emitter_idx[0] = 0x66;
    UpdateVocalStream_57E510();
}

// bFastForward is the caller's flag; the local bFast is only PlayVocal's append flag.
// 9.6f 0x4B25D0
MATCH_FUNC(0x57DF10)
void sound_obj::HandleVocalStreamSwitching_57DF10(char_type bFastForward)
{
    bool bFast = false;
    bool bStationChanged = false;
    u32 length;
    u32 pos;

    if (field_54F4_bWasInRadioCar != field_54F5_bInRadioCar)
    {
        bFast = field_54FC_main_variant == 1;
        gSampManager_6FFF00.CloseVocalStream_58E6A0(0);
        if (field_54F7_active_emitter_idx[0] != 101)
        {
            gSampManager_6FFF00.PlayVocal_58E510(0, field_5468_radio_emitters[field_54F7_active_emitter_idx[0]].field_C_station_idx, bFast);
        }
        else
        {
            gSampManager_6FFF00.PlayVocal_58E510(0, 101, bFast);
        }
        gSampManager_6FFF00.SetVocalVolume_58E6D0(0, 0);
        if (bFastForward)
        {
            gSampManager_6FFF00.SetVocalSpeed_58E700(0, gSampManager_6FFF00.GetVocalSpeed_58E720(0) * 2);
        }

        length = gSampManager_6FFF00.GetVocalLengthMs_58E7A0(0);
        if (length <= 0)
        {
            gSampManager_6FFF00.CloseVocalStream_58E6A0(0);
            return;
        }
        pos = static_cast<u32>(field_5468_radio_emitters[field_54F7_active_emitter_idx[0]].field_14_elapsed_frames) * 1000 / field_8_frames_per_second + field_5468_radio_emitters[field_54F7_active_emitter_idx[0]].field_18_pos_ms;
        if (pos > length)
        {
            pos %= length;
        }
        gSampManager_6FFF00.SetVocalPosMs_58E750(0, pos);
        UpdateCarRadioAudio_57E220();
        return;
    }

    if (field_54F7_active_emitter_idx[1] != field_54F7_active_emitter_idx[0])
    {
        bFast = true;
        bStationChanged = true;
    }

    // The do/while (0) and the goto to done (not a return) give the original's block layout and tail merging.
    do
    {
        if (field_5500_prev_main_variant == field_54FC_main_variant)
        {
            if (!bStationChanged)
            {
                UpdateCarRadioAudio_57E220();
                return;
            }
        }
        else if (!bFast)
        {
            length = gSampManager_6FFF00.GetVocalLengthMs_58E7A0(0);
            if (length <= 0)
            {
                gSampManager_6FFF00.CloseVocalStream_58E6A0(0);
            }
            else
            {
                pos = gSampManager_6FFF00.GetVocalPosMs_58E770(0);
                gSampManager_6FFF00.CloseVocalStream_58E6A0(0);
                bFast = field_54FC_main_variant == 1;
                if (field_54F7_active_emitter_idx[0] != 101)
                {
                    gSampManager_6FFF00.PlayVocal_58E510(0, field_5468_radio_emitters[field_54F7_active_emitter_idx[0]].field_C_station_idx, bFast);
                }
                else
                {
                    gSampManager_6FFF00.PlayVocal_58E510(0, 101, bFast);
                }
                if (bFastForward)
                {
                    gSampManager_6FFF00.SetVocalSpeed_58E700(0, gSampManager_6FFF00.GetVocalSpeed_58E720(0) * 2);
                }
                if (pos >= static_cast<u32>(gSampManager_6FFF00.GetVocalLengthMs_58E7A0(0)))
                {
                    pos = 0;
                }
                gSampManager_6FFF00.SetVocalPosMs_58E750(0, pos);
                UpdateCarRadioAudio_57E220();
            }
            goto done;
        }
    } while (0);
    if (field_54F7_active_emitter_idx[1] < 5)
    {
        pos = gSampManager_6FFF00.GetVocalPosMs_58E770(0);
        field_5468_radio_emitters[field_54F7_active_emitter_idx[1]].field_18_pos_ms = pos;
    }
    gSampManager_6FFF00.CloseVocalStream_58E6A0(0);
    bFast = field_54FC_main_variant == 1;
    if (field_54F7_active_emitter_idx[0] != 101)
    {
        gSampManager_6FFF00.PlayVocal_58E510(0, field_5468_radio_emitters[field_54F7_active_emitter_idx[0]].field_C_station_idx, bFast);
    }
    else
    {
        gSampManager_6FFF00.PlayVocal_58E510(0, 101, bFast);
    }
    gSampManager_6FFF00.SetVocalVolume_58E6D0(0, 0);
    if (bFastForward)
    {
        gSampManager_6FFF00.SetVocalSpeed_58E700(0, gSampManager_6FFF00.GetVocalSpeed_58E720(0) * 2);
    }

    length = gSampManager_6FFF00.GetVocalLengthMs_58E7A0(0);
    if (length > 0)
    {
        pos = static_cast<u32>(field_5468_radio_emitters[field_54F7_active_emitter_idx[0]].field_14_elapsed_frames) * 1000 / field_8_frames_per_second + field_5468_radio_emitters[field_54F7_active_emitter_idx[0]].field_18_pos_ms;
        if (pos > length)
        {
            pos %= length;
        }
        gSampManager_6FFF00.SetVocalPosMs_58E750(0, pos);
        UpdateCarRadioAudio_57E220();
    }
    else
    {
        gSampManager_6FFF00.CloseVocalStream_58E6A0(0);
    }
done:;
}

MATCH_FUNC(0x57E220)
void sound_obj::UpdateCarRadioAudio_57E220()
{
    Car* pCar = gGame_0x40_67E008->field_38_local_player->GetPlayerCar_5698E0();
    u32 rate;
    if (!pCar)
    {
        return;
    }
    else
    {
        if (!field_54F4_bWasInRadioCar)
        {
            field_5506_mute_timer = 0;
        }

        if (field_5506_mute_timer)
        {
            field_54F6_radio_volume = 0;
            field_5506_mute_timer--;
        }
        else if (gMap_0x370_6F6268->CheckColumnHasSolidAbove_4E7FC0(pCar->field_50_car_sprite->field_14_xy.x,
                                                                     pCar->field_50_car_sprite->field_14_xy.y,
                                                                     pCar->field_50_car_sprite->field_1C_zpos))
        {
            field_54F6_radio_volume -= field_54F6_radio_volume >> 2;
        }

        u32 vocal_volume = (field_54F6_radio_volume * field_25_cdVol) / 127;
        u8 static_volume;
        if (field_54F6_radio_volume < 115)
        {
            if (!field_1_isPaused)
            {
                static_volume = 127 - field_54F6_radio_volume;
            }
            else
            {
                static_volume = 0;
            }
            if (static_volume > 100)
            {
                static_volume = 100;
            }

            if (static_volume > gCarRadioStaticVolume_6FF540 + 5)
            {
                static_volume = gCarRadioStaticVolume_6FF540 + 5;
            }
            else if (static_volume < gCarRadioStaticVolume_6FF540 - 10)
            {
                static_volume = gCarRadioStaticVolume_6FF540 - 10;
            }
        }
        else
        {
            static_volume = 0;
        }

        if (field_1_isPaused == 1)
        {
            static_volume = 0;
            vocal_volume >>= 1;
        }

        gCarRadioStaticVolume_6FF540 = static_volume;
        if (static_volume > 0)
        {
            if (gCarRadioStaticTimer_6FF542 > 0)
            {
                gCarRadioStaticTimer_6FF542--;
                rate = gCarRadioStaticRate_625010;
            }
            else
            {
                gCarRadioStaticTimer_6FF542 = field_1454_anRandomTable[3] % 35;
                rate = gSampManager_6FFF00.GetPlayBackRate_58DBF0(137);
                rate += field_1454_anRandomTable[2] % (rate >> 2);
            }
            gCarRadioStaticRate_625010 = rate;

            u8 sample_volume = static_cast<u8>((static_volume * field_25_cdVol) / 127) >> 2;
            field_30_sQueueSample.field_0_EntityIndex = field_5508_radio_entity_idx;
            field_30_sQueueSample.field_4_SampleIndex = 0;
            field_30_sQueueSample.field_14_samp_idx = 137;
            field_30_sQueueSample.field_18_bIs2D = true;
            field_30_sQueueSample.field_24_nVolume = sample_volume;
            field_30_sQueueSample.field_20_rate = rate;
            field_30_sQueueSample.field_34_loop_start = 0;
            field_30_sQueueSample.field_38_loop_end = -1;
            field_30_sQueueSample.field_1C_ReleasingVolumeModificator = 0;
            field_30_sQueueSample.field_28_distance = Fix16(0);
            field_30_sQueueSample.field_40_pan = 64;
            field_30_sQueueSample.field_41_releasing_sound_flag = 0;
            field_30_sQueueSample.field_30_loop_count = 0;
            field_30_sQueueSample.field_4C_releasing_volume_divider = 5;
            field_30_sQueueSample.field_58_type = 20;
            field_30_sQueueSample.field_60_nEmittingVolume = sample_volume;
            field_30_sQueueSample.field_64_max_distance = 50;
            AddSampleToRequestedQueue_41A850();

            if (field_54FC_main_variant == 0)
            {
                u32 tune_rate = gSampManager_6FFF00.GetPlayBackRate_58DBF0(138);
                tune_rate += (tune_rate >> 6) * field_5468_radio_emitters[field_54F7_active_emitter_idx[0]].field_C_station_idx;
                if (tune_rate > static_cast<u32>(gCarRadioTuneRate_625014 + 90))
                {
                    tune_rate = gCarRadioTuneRate_625014 + 90;
                }
                else if (tune_rate < static_cast<u32>(gCarRadioTuneRate_625014 - 90))
                {
                    tune_rate = gCarRadioTuneRate_625014 - 90;
                }
                gCarRadioTuneRate_625014 = tune_rate;

                u32 rnd = field_1454_anRandomTable[1] % 140;
                // Same store order as 9.6f (0x4B1E40): rate, volume, then the distance
                // through the Fix16(s32) ctor, pan and type.
                field_30_sQueueSample.field_4_SampleIndex = 1;
                field_30_sQueueSample.field_14_samp_idx = 138;
                field_30_sQueueSample.field_18_bIs2D = true;
                field_30_sQueueSample.field_20_rate = tune_rate + rnd;
                field_30_sQueueSample.field_24_nVolume =
                    static_cast<u8>(static_cast<u8>((static_volume * field_25_cdVol) / 254) + static_cast<u8>(field_1454_anRandomTable[2] % 3)) >> 2;
                field_30_sQueueSample.field_28_distance = Fix16(0);
                field_30_sQueueSample.field_40_pan = 64;
                field_30_sQueueSample.field_58_type = 20;
                AddSampleToRequestedQueue_41A850();
            }
        }

        gSampManager_6FFF00.SetVocalVolume_58E6D0(0, vocal_volume);
    }
}

MATCH_FUNC(0x57E510)
void sound_obj::UpdateVocalStream_57E510()
{
    if (field_54F7_active_emitter_idx[0] == 0x66 && field_54F7_active_emitter_idx[1] != 0x66)
    {
        if (field_54F7_active_emitter_idx[1] < 5)
        {
            field_5468_radio_emitters[field_54F7_active_emitter_idx[1]].field_18_pos_ms = gSampManager_6FFF00.GetVocalPosMs_58E770(0);
        }
        gSampManager_6FFF00.CloseVocalStream_58E6A0(0);
        gSampManager_6FFF00.PlayVocal_58E510(0, 12, true);
        gSampManager_6FFF00.SetVocalVolume_58E6D0(0, 0);
        u32 length = gSampManager_6FFF00.GetVocalLengthMs_58E7A0(0);
        if (length > 0)
        {
            gSampManager_6FFF00.SetVocalPosMs_58E750(0, field_1454_anRandomTable[2] % length);
        }
        field_551C_fade_in_timer = 60;
    }
    else if (static_cast<u32>(field_551C_fade_in_timer) > 0)
    {
        u32 volume = (3600 - 60 * static_cast<u32>(field_551C_fade_in_timer)) / 60;
        if (!field_1_isPaused)
        {
            field_551C_fade_in_timer--;
        }
        else if (volume > 30)
        {
            volume = 30;
        }
        gSampManager_6FFF00.SetVocalVolume_58E6D0(0, (field_24_sfx_vol * volume) / 127);
    }
    else if (field_1_isPaused)
    {
        gSampManager_6FFF00.SetVocalVolume_58E6D0(0, (field_24_sfx_vol * 30) / 127);
    }
    else
    {
        gSampManager_6FFF00.SetVocalVolume_58E6D0(0, (field_24_sfx_vol * 60) / 127);
    }
}

MATCH_FUNC(0x57E680)
void sound_obj::TickRadioStationClocks_57E680()
{

    if (gSoundVocalsInited_6FF538)
    {
        u32 ticks = 90000 * this->field_8_frames_per_second;
        for (u8 i = 0; i < 5; i++)
        {
            if (static_cast<u32>(field_5468_radio_emitters[i].field_14_elapsed_frames) < ticks)
            {
                ++field_5468_radio_emitters[i].field_14_elapsed_frames;
            }
        }
    }
}

MATCH_FUNC(0x57E6C0)
void sound_obj::ChooseRadioEmitterForVehicle_57E6C0()
{
    Car* pCar = gGame_0x40_67E008->field_38_local_player->GetPlayerCar_5698E0();
    if (IsPoliceOrServiceVehicle_57F090(pCar) == true)
    {
        field_54FC_main_variant = 0;
        field_54F7_active_emitter_idx[0] = 101;
        return;
    }

    if (pCar->field_B0_emitter_status)
    {
        field_54F7_active_emitter_idx[0] = FindEmitterByStation_57F050(pCar->field_B0_emitter_status);
        return;
    }

    field_54FC_main_variant = 1;

    u8 emitter;
    switch (pCar->GetCarInfoIdx_411940())
    {
        case car_model_enum::VTYPE:
            emitter = FindEmitterByStation_57F050(5);
            break;
        case car_model_enum::ISETTA:
            emitter = FindEmitterByStation_57F050(7);
            break;
        case car_model_enum::MIURA:
            emitter = FindEmitterByStation_57F050(6);
            break;
        case car_model_enum::PICKUP:
            emitter = FindEmitterByStation_57F050(8);
            break;
        case car_model_enum::STRATOSB:
            emitter = FindEmitterByStation_57F050(9);
            break;
        case car_model_enum::BUICK:
            emitter = FindEmitterByStation_57F050(11);
            break;
        case car_model_enum::KRSNABUS:
            emitter = FindEmitterByStation_57F050(10);
            break;
        default:
            emitter = 127;
            break;
    }

    if (emitter == 127)
    {
        emitter = 0;
    }

    u8 volume = ComputeRadioEmitterVolume_57EB90(emitter, 0);
    if (volume < 50)
    {
        u8 i;
        switch (static_cast<u8>(field_1454_anRandomTable[0] % 5))
        {
            // The search loops return when they run out (jae ret; jmp top). A hit stores and breaks
            // out, and VC6 shares that block between case 0 and case 1.
            case 0:
                i = 0;
                while (1)
                {
                    s32 station_idx = field_5468_radio_emitters[i].field_C_station_idx;
                    if (station_idx == 6 || (station_idx > 8 && station_idx <= 10))
                    {
                        field_54F7_active_emitter_idx[0] = i;
                        break;
                    }
                    if (i == 4)
                    {
                        field_54F7_active_emitter_idx[0] = FindEmitterByStation_57F050(1);
                    }
                    if (++i >= 5)
                    {
                        return;
                    }
                }
                break;
            case 1:
                i = 0;
                while (1)
                {
                    s32 station_idx = field_5468_radio_emitters[i].field_C_station_idx;
                    if (station_idx >= 7 && (station_idx <= 8 || station_idx == 11))
                    {
                        field_54F7_active_emitter_idx[0] = i;
                        break;
                    }
                    if (i == 4)
                    {
                        field_54F7_active_emitter_idx[0] = FindEmitterByStation_57F050(1);
                    }
                    if (++i >= 5)
                    {
                        return;
                    }
                }
                break;
            case 2:
                field_54F7_active_emitter_idx[0] = FindEmitterByStation_57F050(5);
                if (field_54F7_active_emitter_idx[0] == 127)
                {
                    field_54F7_active_emitter_idx[0] = FindEmitterByStation_57F050(1);
                }
                break;
            case 3:
                for (i = 0; i < 5; i++)
                {
                    if (field_5468_radio_emitters[i].field_10_a_variant_radius == 0 && field_5468_radio_emitters[i].field_C_station_idx != 1)
                    {
                        field_54F7_active_emitter_idx[0] = i;
                        return;
                    }
                }
                break;
            default:
                field_54F7_active_emitter_idx[0] = FindEmitterByStation_57F050(1);
                break;
        }
    }
    else
    {
        field_54F7_active_emitter_idx[0] = emitter;
        field_54F6_radio_volume = volume;
    }
}

MATCH_FUNC(0x57E960)
void sound_obj::InitMusicAndCopRadio_57E960()
{
    if (!gSoundVocalsInited_6FF538)
    {
        gSoundSwitchRadioCoolDown_6FF539 = 0;
        gSoundVocalsInited_6FF538 = true;
        for (u8 i = 0; i < 5; i++)
        {
            field_5468_radio_emitters[i].field_0_bUsed = false;
            field_5468_radio_emitters[i].field_10_a_variant_radius = 0;
            field_5468_radio_emitters[i].field_12_main_radius = 0;
            field_5468_radio_emitters[i].field_14_elapsed_frames = 0;
            field_5468_radio_emitters[i].field_18_pos_ms = 0;
        }
        field_54F4_bWasInRadioCar = 0;
        field_54F7_active_emitter_idx[1] = 1;
        field_54F7_active_emitter_idx[0] = 1;
        field_5504_radio_station_change_mode = 0;
        field_551C_fade_in_timer = 0;
        DeclareRadioStation_57ECB0(1, Fix16(0, 0), Fix16(0, 0));
        if (field_5508_radio_entity_idx == 0)
        {
            field_550C_radio_entity.field_C_pAny.pAny = NULL;
            field_550C_radio_entity.field_0_object_type = SoundObjectTypeEnum::Radio_3;
            field_550C_radio_entity.field_4_bStatus = false;
            field_5508_radio_entity_idx = AddSoundObject_419FA0(&field_550C_radio_entity);
        }
    }
}

MATCH_FUNC(0x57EA10)
void sound_obj::DeInitVocals_57EA10()
{
    if (gSoundVocalsInited_6FF538)
    {
        gSoundVocalsInited_6FF538 = false;

        s32 idx = 0;
        s32 k2 = 2;
        do
        {
            gSampManager_6FFF00.CloseVocalStream_58E6A0(idx);
            idx++;
            --k2;
        } while (k2);
        
        for (s32 j = 4; j >= 0; j--)
        {
            field_5468_radio_emitters[j].field_0_bUsed = false;
            field_5468_radio_emitters[j].field_10_a_variant_radius = 0;
            field_5468_radio_emitters[j].field_12_main_radius = 0;
            field_5468_radio_emitters[j].field_14_elapsed_frames = 0;
            field_5468_radio_emitters[j].field_18_pos_ms = 0;
        }

        field_551C_fade_in_timer = 0;
        if (field_5508_radio_entity_idx)
        {
            sound_obj::FreeSoundEntry_41A090(field_5508_radio_entity_idx);
            field_5508_radio_entity_idx = 0;
        }
    }
}

MATCH_FUNC(0x57EA90)
void sound_obj::UpdateActiveRadioEmitterVolume_57EA90()
{
    if (field_54F7_active_emitter_idx[0] == 101)
    {
        field_54F6_radio_volume = 115;
        return;
    }

    if (field_5468_radio_emitters[field_54F7_active_emitter_idx[0]].field_C_station_idx && field_5468_radio_emitters[field_54F7_active_emitter_idx[0]].field_0_bUsed == 1)
    {
        if (!field_54FC_main_variant)
        {
            u8 volume = ComputeRadioEmitterVolume_57EB90(field_54F7_active_emitter_idx[0], 1);
            if (volume > 110)
            {
                field_54FC_main_variant = 1;
                field_54F6_radio_volume = volume;
                field_5506_mute_timer = field_1454_anRandomTable[0] % 10 + 16;
            }
            else
            {
                field_54F6_radio_volume = ComputeRadioEmitterVolume_57EB90(field_54F7_active_emitter_idx[0], 0);
            }
        }
        else
        {
            if (ComputeRadioEmitterVolume_57EB90(field_54F7_active_emitter_idx[0], 1) < 90)
            {
                field_54F6_radio_volume = ComputeRadioEmitterVolume_57EB90(field_54F7_active_emitter_idx[0], 0);
                field_54FC_main_variant = 0;
                field_5506_mute_timer = field_1454_anRandomTable[0] % 10 + 16;
            }
            else
            {
                field_54F6_radio_volume = ComputeRadioEmitterVolume_57EB90(field_54F7_active_emitter_idx[0], 1);
                field_54FC_main_variant = 1;
            }
        }
    }
    else
    {
        field_54F6_radio_volume = 0;
    }
}

MATCH_FUNC(0x57EB90)
u8 sound_obj::ComputeRadioEmitterVolume_57EB90(u8 emitterIndex, s32 bMainVariant)
{
    if (field_5468_radio_emitters[emitterIndex].field_0_bUsed && field_5468_radio_emitters[emitterIndex].field_C_station_idx)
    {
        Fix16 xpos;
        Fix16 ypos;
        Fix16 zpos;
        gGame_0x40_67E008->field_38_local_player->get_pos_569920(&xpos, &ypos, &zpos);

        u16 dist = abs(static_cast<s16>(ypos.ToInt() - field_5468_radio_emitters[emitterIndex].field_8_ypos.ToInt())) +
            abs(static_cast<s16>(xpos.ToInt() - field_5468_radio_emitters[emitterIndex].field_4_xpos.ToInt()));

        u16 radius;
        if (!bMainVariant)
        {
            radius = field_5468_radio_emitters[emitterIndex].field_10_a_variant_radius;
        }
        else
        {
            radius = field_5468_radio_emitters[emitterIndex].field_12_main_radius;
        }

        if (dist < radius)
        {
            u16 inner_radius = (radius * 7) / 8;
            if (dist < inner_radius)
            {
                return 127;
            }
            u8 volume = 127;
            volume -= static_cast<u16>(127 * dist - 127 * inner_radius) / (dist / 8);
            return volume;
        }
    }
    return 0;
}

MATCH_FUNC(0x57ECB0)
void sound_obj::DeclareRadioStation_57ECB0(s32 station_idx, Fix16 xpos, Fix16 ypos)
{
    for (u8 i = 0; i < 5; i++)
    {
        if (!field_5468_radio_emitters[i].field_0_bUsed && !field_5468_radio_emitters[i].field_10_a_variant_radius && !field_5468_radio_emitters[i].field_12_main_radius)
        {
            field_5468_radio_emitters[i].field_0_bUsed = true;
            field_5468_radio_emitters[i].field_C_station_idx = station_idx;
            field_5468_radio_emitters[i].field_4_xpos = Fix16(static_cast<u16>(xpos.ToInt()));
            field_5468_radio_emitters[i].field_8_ypos = Fix16(static_cast<u16>(ypos.ToInt()));
            field_5468_radio_emitters[i].field_14_elapsed_frames = field_5468_radio_emitters[i].field_18_pos_ms =
                field_1454_anRandomTable[(i + 1) % 5] * field_1454_anRandomTable[i % 5];
            SetRadioEmitterRadii_57EDB0(&field_5468_radio_emitters[i], station_idx);
            return;
        }
    }
}

// Cases are written out separately so the jump table has one entry per case
MATCH_FUNC(0x57EDB0)
void sound_obj::SetRadioEmitterRadii_57EDB0(RadioStationEmitter* pEmitter, s32 type)
{
    switch (type)
    {
        case 1:
            pEmitter->field_10_a_variant_radius = 0;
            pEmitter->field_12_main_radius = 600;
            pEmitter->field_4_xpos = 0;
            pEmitter->field_8_ypos = 0;
            break;
        case 2:
            pEmitter->field_10_a_variant_radius = 0;
            pEmitter->field_12_main_radius = 600;
            break;
        case 3:
            pEmitter->field_10_a_variant_radius = 0;
            pEmitter->field_12_main_radius = 600;
            break;
        case 4:
            pEmitter->field_10_a_variant_radius = 0;
            pEmitter->field_12_main_radius = 600;
            break;
        case 5:
            pEmitter->field_10_a_variant_radius = 600;
            pEmitter->field_12_main_radius = 300;
            break;
        case 6:
            pEmitter->field_10_a_variant_radius = 600;
            pEmitter->field_12_main_radius = 300;
            break;
        case 7:
            pEmitter->field_10_a_variant_radius = 600;
            pEmitter->field_12_main_radius = 300;
            break;
        case 8:
            pEmitter->field_10_a_variant_radius = 600;
            pEmitter->field_12_main_radius = 300;
            break;
        case 9:
            pEmitter->field_10_a_variant_radius = 600;
            pEmitter->field_12_main_radius = 300;
            break;
        case 10:
            pEmitter->field_10_a_variant_radius = 600;
            pEmitter->field_12_main_radius = 300;
            break;
        case 11:
            pEmitter->field_10_a_variant_radius = 600;
            pEmitter->field_12_main_radius = 300;
            break;
    }
}

DEFINE_GLOBAL(u8, gCarRadioStaticVolume_6FF540, 0x6FF540);
DEFINE_GLOBAL(u16, gCarRadioStaticTimer_6FF542, 0x6FF542);
DEFINE_GLOBAL_INIT(s32, gCarRadioStaticRate_625010, 11025, 0x625010);
DEFINE_GLOBAL_INIT(s32, gCarRadioTuneRate_625014, 11025, 0x625014);

MATCH_FUNC(0x57EE30)
void sound_obj::RemoveSound_57EE30(Fix16 xpos, Fix16 ypos)
{
    s16 x = xpos.ToInt();
    s16 y = ypos.ToInt();
    for (u8 i = 5; i != 0; i--)
    {
        if (field_5468_radio_emitters[i - 1].field_0_bUsed == 1 && static_cast<s16>(field_5468_radio_emitters[i - 1].field_4_xpos.ToInt()) == x &&
            static_cast<s16>(field_5468_radio_emitters[i - 1].field_8_ypos.ToInt()) == y)
        {
            field_5468_radio_emitters[i - 1].field_0_bUsed = false;
            field_5468_radio_emitters[i - 1].field_10_a_variant_radius = 0;
            field_5468_radio_emitters[i - 1].field_12_main_radius = 0;
            field_5468_radio_emitters[i - 1].field_14_elapsed_frames = 0;
            field_5468_radio_emitters[i - 1].field_18_pos_ms = 0;
            return;
        }
    }
}

MATCH_FUNC(0x57EEE0)
void sound_obj::CycleRadioStation_57EEE0(char_type bPrev)
{
    if (gSoundSwitchRadioCoolDown_6FF539 <= 0)
    {
        gSoundSwitchRadioCoolDown_6FF539 = 10;
        if (gGame_0x40_67E008)
        {
            Player* pPlayer = gGame_0x40_67E008->field_38_local_player;
            if (pPlayer)
            {
                if (pPlayer->GetPlayerCar_5698E0())
                {
                    Car* pCar = gGame_0x40_67E008->field_38_local_player->GetPlayerCar_5698E0();
                    // can't change their sucky radio station ;)
                    if (!IsPoliceOrServiceVehicle_57F090(pCar))
                    {
                        Car* pCarAgain = gGame_0x40_67E008->field_38_local_player->GetPlayerCar_5698E0();
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

MATCH_FUNC(0x57EF60)
void sound_obj::SelectBestRadioEmitter_57EF60()
{
    u8 candidate = field_54F7_active_emitter_idx[0];
    for (u8 i = 0; i < 5; i++)
    {
        if (field_5504_radio_station_change_mode == 1)
        {
            candidate = (candidate + 1) % 5;
        }
        else if (candidate == 0)
        {
            candidate = 4;
        }
        else
        {
            candidate--;
        }

        for (u8 bNear = 0; bNear < 2; bNear++)
        {
            u8 volume = ComputeRadioEmitterVolume_57EB90(candidate, 1 - bNear);
            if (volume > 65)
            {
                field_54F7_active_emitter_idx[0] = candidate;
                field_54F6_radio_volume = volume;
                field_54FC_main_variant = 1 - bNear;
                field_5504_radio_station_change_mode = 0;
                field_5506_mute_timer = field_1454_anRandomTable[0] % 15 + 15;
                return;
            }
        }
    }
    field_5504_radio_station_change_mode = 0;
}

MATCH_FUNC(0x57F050)
u8 sound_obj::FindEmitterByStation_57F050(s32 station_idx) const
{
    for (u8 i = 0; i < 5; i++)
    {
        if (static_cast<u32>(field_5468_radio_emitters[i].field_C_station_idx) == static_cast<u32>(station_idx))
        {
            return i;
        }
    }
    return 127;
}

MATCH_FUNC(0x57F090)
bool sound_obj::IsPoliceOrServiceVehicle_57F090(Car* pCar) const
{
    switch (pCar->GetCarInfoIdx_411940())
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

MATCH_FUNC(0x57F120)
bool sound_obj::IsTrainOrBoxcar_57F120(Car* pCar) const
{
    if (!pCar)
    {
        return false;
    }

    if (pCar->field_84_car_info_idx == car_model_enum::boxcar ||
        (pCar->field_84_car_info_idx > car_model_enum::TOWTRUCK && pCar->field_84_car_info_idx <= car_model_enum::TRAINFB))
    {
        return true;
    }
    return false;
}
