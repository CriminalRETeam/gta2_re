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
DEFINE_STATIC_MEMBER(bool, sound_obj, sSoundVocalsInited, 0x6FF538);
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
    if (gGame_0x40_67E008 && mType5Idx && !mIsPaused)
    {
        switch (pObj->mType)
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
    return gFrontend_67DC84 && pObj->mType == 20 ? true : false;
}

MATCH_FUNC(0x4123A0)
void sound_obj::ProcessEntity_4123A0(s32 idx)
{
    if (!mAudioEntities[idx].mpObj->mbStatus)
    {
        if (gGame_0x40_67E008 && mType5Idx)
        {
            if (mAudioEntities[idx].mpObj->mObjectType == SoundObjectTypeEnum::Radio_3)
            {
                ProcessType3_CopRadioAndMusic_57DD50();
            }

            if (!mIsPaused)
            {
                switch (mAudioEntities[idx].mpObj->mObjectType)
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

        if (mAudioEntities[idx].mpObj->mObjectType == SoundObjectTypeEnum::Frontend_2)
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

    SoundObject* pLinkedObj = mAudioEntities[idx].mpObj->mpAny.pNextFree;
    if (!pLinkedObj)
    {
        return;
    }

    if (pLinkedObj->mbStatus)
    {
        mbFirstMenuStream = 0;
        if (gSampManager_6FFF00.StreamStatus_58E2C0() || !gType2StreamStarted_66F540)
        {
            gSampManager_6FFF00.CloseStream_58E460();
            gSampManager_6FFF00.OpenStream_58E320(2u);
            gSampManager_6FFF00.StreamSetVolume_58E2F0((6 * mCdVol) / 10);
            gType2StreamStarted_66F540 = 1;
        }
    }
    else
    {
        if (mbFirstMenuStream)
        {
            if (gSampManager_6FFF00.StreamStatus_58E2C0())
            {
                gSampManager_6FFF00.CloseStream_58E460();
                gSampManager_6FFF00.OpenStream_58E320(0);
                gSampManager_6FFF00.StreamSetVolume_58E2F0((6 * mCdVol) / 10);
            }
        }
        else
        {
            if (gSampManager_6FFF00.StreamStatus_58E2C0())
            {
                gSampManager_6FFF00.CloseStream_58E460();
                gSampManager_6FFF00.OpenStream_58E320(1u);
                gSampManager_6FFF00.StreamSetVolume_58E2F0((6 * mCdVol) / 10);
            }
        }
    }

    s32 sampIdx1;
    s32 sampIdx2;
    switch (pLinkedObj->mObjectType)
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
    msQueueSample.mEntityIndex = idx;
    msQueueSample.mpReleaseSource = NULL;
    msQueueSample.mbIs2D = true;
    msQueueSample.mObj.mX = kFpZero_66F3F0;
    msQueueSample.mObj.mY = kFpZero_66F3F0;
    msQueueSample.mObj.mZ = kFpZero_66F3F0;
    msQueueSample.mReleasingVolumeModificator = 0;
    msQueueSample.mnVolume = 127;
    msQueueSample.mDistance = kFpZero_66F3F0;
    msQueueSample.mLoopCount = 1;
    msQueueSample.mLoopStart = 0;
    msQueueSample.mLoopEnd = -1;
    msQueueSample.mSpeedMultiplier = 0;
    msQueueSample.mReleasingSoundFlag = 1;
    msQueueSample.mSoundIntensity = Fix16(5);
    msQueueSample.mType = 20;
    msQueueSample.mnEmittingVolume = 127;
    msQueueSample.mMaxDistance = 10;
    msQueueSample.mSampIdx = sampIdx1;
    msQueueSample.mPan = 0;
    msQueueSample.mRate = gSampManager_6FFF00.GetPlayBackRate_58DBF0(sampIdx1);
    msQueueSample.mSampleIndex = gType2SampleIndexCounter_66F2D4++;
    if (gType2SampleIndexCounter_66F2D4 >= 0xFF)
    {
        gType2SampleIndexCounter_66F2D4 = 0;
    }
    AddSampleToRequestedQueue_41A850();

    // Add second sample
    msQueueSample.mSampIdx = sampIdx2;
    msQueueSample.mPan = 127;
    msQueueSample.mRate = gSampManager_6FFF00.GetPlayBackRate_58DBF0(sampIdx2);
    msQueueSample.mSampleIndex = gType2SampleIndexCounter_66F2D4++;
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

    entity.mpObj = this->mAudioEntities[idx].mpObj->mpAny.pSprite;
    if (entity.mpObj)
    {

        entity.mpObj->GetPosition_4117B0(&msQueueSample.mObj.mX,
                                           &msQueueSample.mObj.mY,
                                           &msQueueSample.mObj.mZ);

        this->msQueueSample.mEntityIndex = idx;
        this->msQueueSample.mpReleaseSource = NULL;

        entity.mbHasSolidAbove = gMap_0x370_6F6268->CheckColumnHasSolidAbove_4E7FC0(msQueueSample.mObj.mX,
                                                                                           msQueueSample.mObj.mY,
                                                                                           msQueueSample.mObj.mZ);
        this->mDistSquared = ComputeEmitterDistanceSquared_4190B0();
        this->mDistCalculated = false;

        if (entity.mpObj->AsCharB4_40FEA0())
        {
            sound_obj::ProcessPed_422B70(&entity);
        }
        else if (entity.mpObj->AsCar_40FEB0())
        {
            sound_obj::ProcessCar_412B80(&entity);
        }
        else if (entity.mpObj->AsObject2C_40FEC0())
        {
            sound_obj::ProcessObject_41E820(&entity);
        }
    }
}

MATCH_FUNC(0x412820)
void sound_obj::ProcessType8_Crane_412820(s32 idx)
{
    Crane* pCrane = mAudioEntities[idx].mpObj->mpAny.pCrane_15C;
    if (pCrane)
    {
        Sprite* pSprite = pCrane->GetRotorSprite_411A00();
        if (pSprite)
        {
            pSprite->GetPosition_4117B0(&msQueueSample.mObj.mX,
                                   &msQueueSample.mObj.mY,
                                   &msQueueSample.mObj.mZ);

            this->msQueueSample.mEntityIndex = idx;
            this->msQueueSample.mpReleaseSource = NULL;
            bool bSolidAbove = gMap_0x370_6F6268->CheckColumnHasSolidAbove_4E7FC0(msQueueSample.mObj.mX,
                                                                                    msQueueSample.mObj.mY,
                                                                                    msQueueSample.mObj.mZ);
            this->mDistSquared = ComputeEmitterDistanceSquared_4190B0();
            this->mDistCalculated = false;
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
                            if (!pCrane->mIsRotating)
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
                            if (!pCrane->mIsRadiusChanging)
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
                            if (!pCrane->mIsHookDepthChanging)
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
                            if (!pCrane->mHookedCarThisFrame)
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
                        this->msQueueSample.mSampIdx = samp_idx;
                        this->msQueueSample.mSampleIndex = i;
                        this->msQueueSample.mReleasingSoundFlag = bLoop;
                        this->msQueueSample.mRate = rate;
                        if (bLoop)
                        {
                            this->msQueueSample.mLoopCount = 1;
                        }
                        else
                        {
                            this->msQueueSample.mLoopCount = 0;
                            this->msQueueSample.mReleasingVolumeDivider = 3;
                        }
                        this->msQueueSample.mLoopStart = gSampManager_6FFF00.GetLoopStart_58DC30(samp_idx);
                        this->msQueueSample.mLoopEnd = gSampManager_6FFF00.GetLoopEnd_58DC50(samp_idx);
                        this->msQueueSample.mReleasingVolumeModificator = vol_mod;
                        this->msQueueSample.mSoundIntensity = Fix16(25);
                        this->msQueueSample.mnEmittingVolume = emitter_vol;
                        this->msQueueSample.mMaxDistance = 50;
                        this->msQueueSample.mType = 20;
                        this->msQueueSample.mSpeedMultiplier = 400;
                        this->msQueueSample.mbIs2D = false;
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
    Crusher* pCrusher = mAudioEntities[idx].mpObj->mpAny.pCrusher_30;
    if (pCrusher)
    {
        if (pCrusher->get_state_411A10())
        {
            Fix16 x = pCrusher->get_xpos_411A20();
            this->msQueueSample.mObj.mX = x;

            Fix16 y = pCrusher->get_ypos_411A30();
            this->msQueueSample.mObj.mY = y;

            Fix16 z = this->mListenerZ;
            this->msQueueSample.mObj.mZ = z;

            this->msQueueSample.mEntityIndex = idx;
            this->msQueueSample.mpReleaseSource = NULL;
            const bool bSolidAbove = gMap_0x370_6F6268->CheckColumnHasSolidAbove_4E7FC0(x, y, z);
            this->mDistSquared = ComputeEmitterDistanceSquared_4190B0();
            this->mDistCalculated = false;
            if (CalculateDistance_419020(Fix16(121)))
            {
                if (VolCalc_419070(0x6Eu, Fix16(11), bSolidAbove))
                {
                    this->msQueueSample.mSampIdx = 60;
                    this->msQueueSample.mSampleIndex = 0;
                    this->msQueueSample.mReleasingSoundFlag = 0;
                    this->msQueueSample.mRate = 17000;
                    this->msQueueSample.mLoopCount = 0;
                    this->msQueueSample.mReleasingVolumeDivider = 5;
                    this->msQueueSample.mLoopStart = gSampManager_6FFF00.GetLoopStart_58DC30(60);
                    this->msQueueSample.mLoopEnd = gSampManager_6FFF00.GetLoopEnd_58DC50(60);
                    this->msQueueSample.mReleasingVolumeModificator = 6;
                    this->msQueueSample.mSoundIntensity = Fix16(11);
                    this->msQueueSample.mnEmittingVolume = 110;
                    this->msQueueSample.mMaxDistance = 22;
                    this->msQueueSample.mType = 20;
                    this->msQueueSample.mSpeedMultiplier = 400;
                    this->msQueueSample.mbIs2D = false;
                    AddSampleToRequestedQueue_41A850();
                }
            }
        }
    }
}

MATCH_FUNC(0x412B80)
void sound_obj::ProcessCar_412B80(Sound_Params* pParams)
{
    Sprite* pSprite = pParams->mpObj;
    Car* pCar = pParams->mpObj->mCarBcPtr;
    if (pCar)
    {
        pParams->mbDrivenByPlayer = pCar->IsDrivenByPlayer_4118D0();

        SoundPosEntry* pAlloc = mAudioEntities[msQueueSample.mEntityIndex].mpAlloc;
        if (pAlloc)
        {
            CarPhysics* pPhysics = pSprite->mCarBcPtr->mPhysics;
            if (pPhysics)
            {
                pPhysics->SetModelPhysicsGlobal_562EB0();
            }
            else
            {
                pAlloc->mLastSpeed = kFpZero_66F3F0;
            }
            switch (pParams->mpObj->mCarBcPtr->GetCarInfoIdx_411940())
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

    switch (pEvent->mType)
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

    this->msQueueSample.mRate =
        RandomDisplacement_41A650(samp_idx) + gSampManager_6FFF00.GetPlayBackRate_58DBF0(samp_idx);
    this->msQueueSample.mSampIdx = samp_idx;
    this->msQueueSample.mbIs2D = false;
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
    gSampManager_6FFF00.SetVocalVolume_58E6D0(1, 127 * mSfxVol / 127);
}

MATCH_FUNC(0x413000)
bool sound_obj::SelectCarObjectCollisionSound_413000(CollisionEvent* pEvent)
{
    if ((pEvent->mObjectModel < 200 || pEvent->mObjectModel > 244) &&
        (pEvent->mObjectModel < 64 || pEvent->mObjectModel > 108))
    {
        return SelectObjectImpactSound_413120(pEvent, 3);
    }
    PlayPickupVoice_412D90(pEvent->mObjectModel);
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

    this->msQueueSample.mRate =
        RandomDisplacement_41A650(idx_to_use) + gSampManager_6FFF00.GetPlayBackRate_58DBF0(idx_to_use);
    this->msQueueSample.mSampIdx = idx_to_use;
    this->msQueueSample.mbIs2D = false;
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

    this->msQueueSample.mRate =
        RandomDisplacement_41A650(idx_to_use) + gSampManager_6FFF00.GetPlayBackRate_58DBF0(idx_to_use);
    this->msQueueSample.mSampIdx = idx_to_use;

    this->msQueueSample.mbIs2D = false;
    return true;
}

MATCH_FUNC(0x4130E0)
bool sound_obj::SelectPedObjectCollisionSound_4130E0(CollisionEvent* pEvent)
{
    if ((pEvent->mObjectModel < 200 || pEvent->mObjectModel > 244) && (pEvent->mObjectModel < 64 || pEvent->mObjectModel > 108))
    {
        return SelectObjectImpactSound_413120(pEvent, 7);
    }
    PlayPickupVoice_412D90(pEvent->mObjectModel);
    return true;
}

MATCH_FUNC(0x413120)
bool sound_obj::SelectObjectImpactSound_413120(CollisionEvent* pEvent, s32 interactionType)
{
    // Models 2..60 share the samp 37 code of 281/282 (one switch in the original, split at 110 by VC6).
    // The 110 case keeps its own copy after the if, which VC6 tail-merges back into the 281/282 block.
    s32 samp_idx;

    if (pEvent->mObjectModel <= objects::ped_pushed_110)
    {
        if (pEvent->mObjectModel == objects::ped_pushed_110)
        {
            // 110 shares the code of models 182 and 183
            goto case_110;
        }

        switch (pEvent->mObjectModel)
        {
            case objects::bin_lid_1:
            case objects::hydrant_lid_18:
                msQueueSample.mSampIdx = 55;
                msQueueSample.mbIs2D = false;
                msQueueSample.mRate = manRandomTable[0] % 2000 + 23000;
                return true;

            case objects::bin_7:
                msQueueSample.mSampIdx = 55;
                msQueueSample.mbIs2D = false;
                msQueueSample.mRate = manRandomTable[0] % 2000 + 20000;
                return true;

            case objects::oildrum_16:
            case objects::tower_25:
            case 62:
                msQueueSample.mSampIdx = 55;
                msQueueSample.mbIs2D = false;
                msQueueSample.mRate = manRandomTable[0] % 2000 + 15000;
                return true;

            case objects::boxes_4:
            case objects::package_23:
            case 44:
                msQueueSample.mSampIdx = 54;
                // An own copy of the next group's tail is cross-jumped the other way round
                goto rate_17000;

            case objects::rubbish_6:
                msQueueSample.mSampIdx = 52;
                msQueueSample.mbIs2D = false;
                msQueueSample.mRate = manRandomTable[0] % 600 + 16000;
                return true;

            case objects::cone_3:
            case objects::blaster_5:
            case objects::bush_11:
            case objects::footy_13:
            case objects::tyre_17:
            case 53:
            case 54:
            case 55:
                msQueueSample.mSampIdx = 56;
            rate_17000:
                msQueueSample.mbIs2D = false;
                msQueueSample.mRate = manRandomTable[0] % 2000 + 17000;
                return true;

            case objects::roadblock_21:
            case objects::bench_22:
            case 46:
            case 48:
                msQueueSample.mSampIdx = 53;
                msQueueSample.mbIs2D = false;
                msQueueSample.mRate = manRandomTable[0] % 1000 + 15500;
                return true;

            case objects::crate_12:
            case objects::hardbox_14:
            case 58:
                msQueueSample.mSampIdx = 53;
                msQueueSample.mbIs2D = false;
                msQueueSample.mRate = manRandomTable[0] % 2000 + 18000;
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

    switch (pEvent->mObjectModel)
    {
        case objects::diagonal_wall_collision_obj_166:
        case objects::door_collision_obj_169:
        case objects::tunnel_blocker_294:
        {
            msQueueSample.mSampIdx = 37;
            s32 rate = RandomDisplacement_41A650(37) + gSampManager_6FFF00.GetPlayBackRate_58DBF0(37);
            msQueueSample.mbIs2D = false;
            msQueueSample.mRate = rate;
            return true;
        }

        case objects::power_generator_281:
        case objects::destroyed_powergen_282:
        samp_37:
            msQueueSample.mSampIdx = 37;
            msQueueSample.mbIs2D = false;
            msQueueSample.mRate = manRandomTable[0] % 2000 + 20000;
            return true;

        case 182:
        case objects::grenade_obj_183:
        case_110:
            if (interactionType == 1)
            {
                switch (pEvent->mMapBlockSpec)
                {
                    case 1:
                    case 3:
                        msQueueSample.mSampIdx = 198;
                        break;
                    case 2:
                    case 7:
                    case 10:
                        msQueueSample.mSampIdx = 202;
                        break;
                    case 5:
                    case 6:
                    case 8:
                    case 9:
                        msQueueSample.mSampIdx = 194;
                        break;
                    case 4:
                        msQueueSample.mSampIdx = 68;
                        break;
                    default:
                        return false;
                }

                if (pEvent->mMapBlockSpec == 4)
                {
                    msQueueSample.mbIs2D = false;
                    msQueueSample.mRate = manRandomTable[1] % 4000 + 28000;
                    return true;
                }

                msQueueSample.mRate = gSampManager_6FFF00.GetPlayBackRate_58DBF0(msQueueSample.mSampIdx) +
                    RandomDisplacement_41A650(msQueueSample.mSampIdx);
                msQueueSample.mSampIdx += manRandomTable[3] & 3;
                msQueueSample.mbIs2D = false;
                return true;
            }
            msQueueSample.mSampIdx = 37;
            msQueueSample.mbIs2D = false;
            msQueueSample.mRate = manRandomTable[0] % 2000 + 20000;
            return true;

        case objects::shotgun_bullet_192:
        case objects::machine_gun_bullet_254:
        case objects::pistol_bullet_265:
            switch (interactionType)
            {
                case 3:
                    // 9.6f: Car::IsBulletproof_414F60 (mFlags & 0x100 via IsFlagSet_411930, inlined, a bool helper changes the code)
                    if ((pEvent->mCar->mFlags & car_flag::Bulletproof))
                    {
                        samp_idx = manRandomTable[1] % 3 + 46;
                    }
                    else
                    {
                        samp_idx = manRandomTable[1] % 3 + 43;
                    }
                    break;
                case 7:
                    msQueueSample.mSampIdx = 42;
                    msQueueSample.mbIs2D = false;
                    msQueueSample.mRate = manRandomTable[0] % 2000 + 20000;
                    return true;
                default:
                    samp_idx = manRandomTable[1] % 3 + 49;
                    break;
            }
            msQueueSample.mSampIdx = samp_idx;
            break;

        case 295:
            msQueueSample.mSampIdx = 33;
            msQueueSample.mRate = gSampManager_6FFF00.GetPlayBackRate_58DBF0(33);
            msQueueSample.mbIs2D = true;
            return true;

        case objects::secret_token_266:
            msQueueSample.mSampIdx = 34;
            msQueueSample.mRate = gSampManager_6FFF00.GetPlayBackRate_58DBF0(34);
            msQueueSample.mbIs2D = true;
            return true;

        default:
            return false;
    }

    msQueueSample.mbIs2D = false;
    msQueueSample.mRate = manRandomTable[0] % 2000 + 20000;
    return true;
}

MATCH_FUNC(0x413540)
bool sound_obj::SelectObjectObjectCollisionSound_413540(CollisionEvent* pEvent)
{
    switch (pEvent->mOtherObjectModel)
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
    switch (pEvent->mMapBlockSpec)
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
    this->msQueueSample.mRate =
        RandomDisplacement_41A650(samp_idx) + gSampManager_6FFF00.GetPlayBackRate_58DBF0(samp_idx);
    this->msQueueSample.mSampIdx = samp_idx;

    this->msQueueSample.mbIs2D = false;
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

    CollisionSoundQueue* pQueue = mAudioEntities[idx].mpObj->mpAny.pCollisionSoundQueue;
    if (!pQueue)
    {
        return;
    }

    this->msQueueSample.mpReleaseSource = NULL;
    this->msQueueSample.mEntityIndex = idx;

    for (u8 event_idx = 0; event_idx < pQueue->mCount; event_idx++)
    {
        CollisionEvent* pEvent = &pQueue->mEvents[event_idx];
        u8 volume = GetCollisionVolume_413A10(pEvent);
        if (SelectCollisionSound_412C90(pEvent, volume))
        {
            if (this->msQueueSample.mbIs2D)
            {
                this->msQueueSample.mObj.mX = kFpZero_66F3F0;
                this->msQueueSample.mObj.mY = kFpZero_66F3F0;
                this->msQueueSample.mObj.mZ = kFpZero_66F3F0;
                this->msQueueSample.mSampleIndex = sType6SampleIndexCounter++;
                this->msQueueSample.mReleasingVolumeModificator = 0;
                this->msQueueSample.mnVolume = 45;
                this->msQueueSample.mDistance = kFpZero_66F3F0;
                this->msQueueSample.mLoopStart = 0;
                this->msQueueSample.mLoopCount = 1;
                this->msQueueSample.mLoopEnd = -1;
                this->msQueueSample.mSpeedMultiplier = 0;
                this->msQueueSample.mPan = 63;
                this->msQueueSample.mReleasingSoundFlag = 1;
                this->msQueueSample.mSoundIntensity = Fix16(5);
                this->msQueueSample.mType = 20;
                this->msQueueSample.mnEmittingVolume = 45;
                this->msQueueSample.mMaxDistance = 10;
                if (sType6SampleIndexCounter >= 0xFF)
                {
                    sType6SampleIndexCounter = 0;
                }
                AddSampleToRequestedQueue_41A850();
                return;
            }

            if (volume > 0)
            {
                this->msQueueSample.mObj.mX = pQueue->mEvents[event_idx].mX;
                this->msQueueSample.mObj.mY = pQueue->mEvents[event_idx].mY;
                this->msQueueSample.mObj.mZ = pQueue->mEvents[event_idx].mZ;
                this->mDistSquared = ComputeEmitterDistanceSquared_4190B0();
                this->mDistCalculated = false;
                if (CalculateDistance_419020(Fix16(25)))
                {
                    if (VolCalc_419070(volume,
                                       Fix16(5),
                                       gMap_0x370_6F6268->CheckColumnHasSolidAbove_4E7FC0(this->msQueueSample.mObj.mX,
                                                                                          this->msQueueSample.mObj.mY,
                                                                                          this->msQueueSample.mObj.mZ)))
                    {
                        nearest_idx = event_idx;
                        vol = this->msQueueSample.mnVolume;
                        distance = this->msQueueSample.mDistance;
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

    this->msQueueSample.mObj.mX = pQueue->mEvents[nearest_idx].mX;
    this->msQueueSample.mObj.mY = pQueue->mEvents[nearest_idx].mY;
    this->msQueueSample.mObj.mZ = pQueue->mEvents[nearest_idx].mZ;
    this->msQueueSample.mnVolume = vol;
    this->msQueueSample.mDistance = distance;
    this->msQueueSample.mnEmittingVolume = emittingVol;
    this->msQueueSample.mMaxDistance = 10;
    this->msQueueSample.mSoundIntensity = 81920; // F16
    this->msQueueSample.mType = 20;
    this->msQueueSample.mSampleIndex = sType6SampleIndexCounter++;
    this->msQueueSample.mReleasingSoundFlag = 1;
    this->msQueueSample.mReleasingVolumeModificator = 8;
    this->msQueueSample.mbIs2D = false;
    this->msQueueSample.mSpeedMultiplier = 600;
    this->msQueueSample.mLoopCount = 1;
    this->msQueueSample.mLoopStart = 0;
    this->msQueueSample.mLoopEnd = -1;
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
    switch (pEvent->mType)
    {
        case collision_event_type::car_car_2:
            if (pEvent->mImpactStrength < kFpPoint01_66F3B4)
            {
                return 0;
            }
            if (pEvent->mImpactStrength > kFpTwo_66F3F8)
            {
                pEvent->mImpactStrength = kFpTwo_66F3F8;
            }
            ratio = pEvent->mImpactStrength / kFpTwo_66F3F8;
            break;

        case collision_event_type::car_object_3:
            if (pEvent->mObjectModel == objects::shotgun_bullet_192 ||
                pEvent->mObjectModel == objects::machine_gun_bullet_254 ||
                pEvent->mObjectModel == objects::pistol_bullet_265)
            {
                return 37;
            }
            if (!pEvent->mCar)
            {
                // Shares case 5's return block (9.6f jumps to the same block; a plain return doesn't match)
                goto return_zero;
            }
            pEvent->mImpactStrength = pEvent->mCar->GetCarLinearSpeed_43A240();
            if (pEvent->mImpactStrength < kFpPoint02)
            {
                return 0;
            }
            if (pEvent->mImpactStrength > kFpPoint2_66F24C)
            {
                pEvent->mImpactStrength = kFpPoint2_66F24C;
            }
            ratio = pEvent->mImpactStrength / kFpPoint2_66F24C;
            break;

        case collision_event_type::car_ped_4:
            if (pEvent->mImpactStrength < kFpPoint004)
            {
                return 0;
            }
            if (pEvent->mImpactStrength > kFpHalf_66F2FC)
            {
                pEvent->mImpactStrength = kFpHalf_66F2FC;
            }
            ratio = pEvent->mImpactStrength / kFpHalf_66F2FC;
            break;

        case collision_event_type::car_wall_5:
            if (pEvent->mImpactStrength < kFpPoint01_66F3B4)
            {
            return_zero:
                return 0;
            }
            if (pEvent->mImpactStrength > kFpThree_66F3FC)
            {
                pEvent->mImpactStrength = kFpThree_66F3FC;
            }
            ratio = pEvent->mImpactStrength / kFpThree_66F3FC;
            break;

        default:
            return this->manRandomTable[static_cast<u8>(++sCollisionVolumeRngIndex) % 5] % 0xAu + 5;
    }
    return Fix16::Round_To_Int_410BF0(ratio * kFp127);
}

MATCH_FUNC(0x413B90)
void sound_obj::ProcessTrainCab_413B90(Sound_Params* pParams, SoundPosEntry* pAlloc)
{
    if (pParams->mpObj->mCarBcPtr->mPhysics)
    {
        if (!pParams->mbHasSolidAbove)
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
    if (pParams->mpObj->mCarBcPtr->mPhysics)
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
    Car* pCar = pParams->mpObj->mCarBcPtr;
    pAlloc->mCar = pCar;
    if (pCar->mScale != kFpOne_66F3F4)
    {
        if (pCar->mPhysics)
        {
            HandleCarEngineSound_4157C0(pParams);
        }
    }
    else
    {
        if (pParams->mpObj->mCarBcPtr->mPhysics)
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

    Car* pCar = pParams->mpObj->mCarBcPtr;
    const s32 sound_event = pCar->TakeSoundEvent_411950();
    if (!pCar->IsMaxDamage_40F890() && pCar->mDriver && pParams->mbDrivenByPlayer != 1)
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
                        samp_idx = this->manRandomTable[this->msQueueSample.mEntityIndex % 5u] % 7u + 265;
                    }
                    else
                    {
                        --sAICarHornCooldown;
                        return;
                    }
                }
                else
                {
                    samp_idx = (this->manRandomTable[this->msQueueSample.mEntityIndex % 5u] & 7) + 257;
                }

                this->msQueueSample.mSampIdx = samp_idx;

                if (CalculateDistance_419020(Fix16(1806336, 0)))
                {
                    const u8 emit_vol = this->manRandomTable[this->msQueueSample.mEntityIndex % 5u] % 30u + 50;
                    if (VolCalc_419070(emit_vol, Fix16(172032, 0), pParams->mbHasSolidAbove))
                    {
                        this->msQueueSample.mSoundIntensity = Fix16(172032, 0);
                        this->msQueueSample.mnEmittingVolume = emit_vol;
                        this->msQueueSample.mMaxDistance = 21;
                        this->msQueueSample.mRate = gSampManager_6FFF00.GetPlayBackRate_58DBF0(msQueueSample.mSampIdx) +
                            RandomDisplacement_41A650(msQueueSample.mSampIdx);
                        this->msQueueSample.mType = 20;
                        this->msQueueSample.mSpeedMultiplier = 0;
                        this->msQueueSample.mSampleIndex = sSoundSampleIdx;
                        this->msQueueSample.mReleasingSoundFlag = 1;
                        this->msQueueSample.mReleasingVolumeModificator = 7;
                        this->msQueueSample.mbIs2D = false;
                        this->msQueueSample.mLoopStart = 0;
                        this->msQueueSample.mLoopEnd = -1;
                        this->msQueueSample.mLoopCount = 1;
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
    Car* pCar = pParams->mpObj->mCarBcPtr;
    Fix16 car_speed;
    car_speed = pCar->GetCarLinearSpeed_43A240();
    Fix16 max_speed = gCarInfo_48_6FE258->mMaxSpeed;

    if (car_speed > kFpZero_66F3F0 && max_speed > kFpZero_66F3F0)
    {
        if (CalculateDistance_419020(Fix16(4225)))
        {
            u8 vol = (car_speed / max_speed * Fix16(65) + Fix16(0x2000, 0)).ToInt();
            if (vol > 0)
            {
                if (VolCalc_419070(vol, Fix16(65), pParams->mbHasSolidAbove))
                {
                    this->msQueueSample.mSoundIntensity = Fix16(65);
                    this->msQueueSample.mnEmittingVolume = vol;
                    this->msQueueSample.mMaxDistance = 130;
                    this->msQueueSample.mType = 16;
                    this->msQueueSample.mSampleIndex = 0;
                    this->msQueueSample.mReleasingSoundFlag = 0;
                    this->msQueueSample.mReleasingVolumeModificator = 4;
                    this->msQueueSample.mbIs2D = false;
                    AddSampleToRequestedQueue_41A850();
                }
            }
        }
    }
}

MATCH_FUNC(0x414320)
bool sound_obj::Type_16_InitTrainEngineSound_414320(sound_0x68* pObj)
{
    pObj->mSampIdx = 10;
    pObj->mSpeedMultiplier = 400;
    pObj->mLoopCount = 0;
    pObj->mLoopStart = 0;
    pObj->mLoopEnd = -1;
    pObj->mRate = gSampManager_6FFF00.GetPlayBackRate_58DBF0(10);
    pObj->mReleasingVolumeDivider = 30;
    return true;
}

MATCH_FUNC(0x4143A0)
void sound_obj::HandleTrainCabRollingFrictionSound_4143A0(Sound_Params* pParams)
{

    Car* pCar = pParams->mpObj->mCarBcPtr;
    Fix16 car_speed;
    car_speed = pCar->GetCarLinearSpeed_43A240();
    Fix16 max_speed = gCarInfo_48_6FE258->mMaxSpeed;
    if (car_speed > kFpZero_66F3F0 && max_speed > kFpZero_66F3F0)
    {
        if (CalculateDistance_419020(Fix16(225)))
        {
            u8 vol = (car_speed / max_speed * Fix16(127) + Fix16(0x2000, 0)).ToInt();
            if (vol > 0)
            {
                if (VolCalc_419070(vol, Fix16(15), pParams->mbHasSolidAbove))
                {
                    this->msQueueSample.mSoundIntensity = Fix16(15);
                    this->msQueueSample.mnEmittingVolume = vol;
                    this->msQueueSample.mMaxDistance = 30;
                    this->msQueueSample.mType = 17;
                    this->msQueueSample.mSampleIndex = 2;
                    this->msQueueSample.mReleasingSoundFlag = 0;
                    this->msQueueSample.mReleasingVolumeModificator = 5;
                    this->msQueueSample.mbIs2D = false;
                    AddSampleToRequestedQueue_41A850();
                }
            }
        }
    }
}

MATCH_FUNC(0x4145E0)
bool sound_obj::GetCar_4145E0(s32 idx, Car** ppOut) const
{
    if (mAudioEntities[idx].mbUsed)
    {
        Sprite* pSprite = mAudioEntities[idx].mpObj->mpAny.pSprite;
        if (pSprite)
        {
            if (pSprite->mSound)
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
    pObj->mRate = 11025;
    pObj->mSampIdx = 11;
    pObj->mSpeedMultiplier = 500;
    pObj->mLoopCount = 0;
    pObj->mLoopStart = 0;
    pObj->mLoopEnd = -1;
    pObj->mReleasingVolumeDivider = 30;
    return true;
}

// 9.6f 0x412A20
MATCH_FUNC(0x414710)
void sound_obj::HandleTrainStationSound_414710(Sound_Params* pParams)
{
    Ped* pDriver = pParams->mpObj->mCarBcPtr->mDriver;
    if (pDriver)
    {
        TrainStation* pTrainStation = pDriver->PopTrainStation_4117D0();
        if (pTrainStation)
        {
            this->msQueueSample.mSampIdx = 139;
        }
        else
        {
            pTrainStation = pDriver->PopTrainStation_4117D0();
            if (!pTrainStation)
            {
                return;
            }
            this->msQueueSample.mSampIdx = 140;
        }

        if (pTrainStation->mpZone)
        {
            this->msQueueSample.mObj.mX =
                Fix16(pTrainStation->mpZone->mX + (pTrainStation->mpZone->mW >> 1));
            this->msQueueSample.mObj.mY =
                Fix16(pTrainStation->mpZone->mY + (pTrainStation->mpZone->mH >> 1));
        }
        this->mDistSquared = ComputeEmitterDistanceSquared_4190B0();
        this->mDistCalculated = false;
        if (CalculateDistance_419020(Fix16(1225)))
        {
            if (VolCalc_419070(100, Fix16(35), false))
            {
                this->msQueueSample.mSoundIntensity = Fix16(35);
                this->msQueueSample.mnEmittingVolume = 100;
                this->msQueueSample.mMaxDistance = 70;
                this->msQueueSample.mType = 19;
                this->msQueueSample.mSampleIndex = 3;
                this->msQueueSample.mReleasingSoundFlag = 1;
                this->msQueueSample.mReleasingVolumeModificator = 5;
                this->msQueueSample.mbIs2D = false;
                AddSampleToRequestedQueue_41A850();
            }
        }
    }
}

MATCH_FUNC(0x4149D0)
bool sound_obj::Type_19_InitTrainStationSound_4149D0(sound_0x68* pObj)
{
    pObj->mSpeedMultiplier = 400;
    pObj->mLoopCount = 1;
    pObj->mLoopStart = 0;
    pObj->mLoopEnd = -1;
    pObj->mRate = gSampManager_6FFF00.GetPlayBackRate_58DBF0(pObj->mSampIdx);
    return true;
}

MATCH_FUNC(0x414A50)
void sound_obj::HandleTankMovementSound_414A50(Sound_Params* pParams)
{
    Car* pCar = pParams->mpObj->mCarBcPtr;
    if (pCar->mEngineStatus == car_engine_status::on_3)
    {
        if (CalculateDistance_419020((Fix16(20) / Fix16(2)) * (Fix16(20) / Fix16(2))))
        {
            Fix16 vol_mult;
            vol_mult = pCar->GetCarLinearSpeed_43A240();
            Fix16 max_speed = gCarInfo_48_6FE258->mMaxSpeed;
            if (max_speed > kFpZero_66F3F0)
            {
                if (vol_mult > max_speed)
                {
                    vol_mult = max_speed;
                }

                u8 vol = Fix16::Round_To_Int_410BF0((vol_mult / max_speed) * Fix16(90));

                if (VolCalc_419070(vol, Fix16(20) / Fix16(2), pParams->mbHasSolidAbove))
                {
                    this->msQueueSample.mSoundIntensity = Fix16(20) / Fix16(2);
                    this->msQueueSample.mnEmittingVolume = vol;
                    this->msQueueSample.mMaxDistance = 20;
                    this->msQueueSample.mType = 12;
                    this->msQueueSample.mSampleIndex = 5;
                    this->msQueueSample.mReleasingSoundFlag = 0;
                    this->msQueueSample.mReleasingVolumeModificator = 10;
                    this->msQueueSample.mbIs2D = false;
                    AddSampleToRequestedQueue_41A850();
                }
            }
        }
    }
}

MATCH_FUNC(0x414C90)
bool sound_obj::Type_12_InitTankMovementSound_414C90(sound_0x68* pSound)
{
    pSound->mSampIdx = 35;
    pSound->mSpeedMultiplier = 400;
    pSound->mReleasingVolumeDivider = 5;
    pSound->mRate = gSampManager_6FFF00.GetPlayBackRate_58DBF0(35);
    pSound->mLoopCount = 0;
    pSound->mLoopStart = gSampManager_6FFF00.GetLoopStart_58DC30(35);
    pSound->mLoopEnd = gSampManager_6FFF00.GetLoopEnd_58DC50(35);
    return true;
}

MATCH_FUNC(0x414D30)
void sound_obj::HandleTankTurretSound_414D30(Sound_Params* pParams)
{
    if (pParams->mpObj->mCarBcPtr->mTurretRotated)
    {
        if (CalculateDistance_419020(Fix16(25)))
        {
            if (VolCalc_419070(0x32u, Fix16(5), pParams->mbHasSolidAbove))
            {
                this->msQueueSample.mSoundIntensity = Fix16(5);
                this->msQueueSample.mnEmittingVolume = 50;
                this->msQueueSample.mMaxDistance = 10;
                this->msQueueSample.mType = 11;
                this->msQueueSample.mSampleIndex = 4;
                this->msQueueSample.mReleasingSoundFlag = 0;
                this->msQueueSample.mbIs2D = false;
                if (pParams->mbDrivenByPlayer)
                {
                    this->msQueueSample.mReleasingVolumeModificator = 4;
                }
                else
                {
                    this->msQueueSample.mReleasingVolumeModificator = 6;
                }
                AddSampleToRequestedQueue_41A850();
            }
        }
    }
}

MATCH_FUNC(0x414EE0)
bool sound_obj::Type_11_InitTankTurretSound_414EE0(sound_0x68* pSound)
{
    pSound->mSampIdx = 60;
    pSound->mSpeedMultiplier = 400;
    pSound->mReleasingVolumeDivider = 3;
    pSound->mRate = 9000;
    pSound->mLoopCount = 0;
    pSound->mLoopStart = gSampManager_6FFF00.GetLoopStart_58DC30(60);
    pSound->mLoopEnd = gSampManager_6FFF00.GetLoopEnd_58DC50(60);
    return true;
}

MATCH_FUNC(0x414F90)
void sound_obj::HandleCarBombSound_414F90(Sound_Params* pParams)
{
    if (pParams->mpObj->mCarBcPtr->mAttachments.GetSpriteForModel_5A6A50(objects::car_bomb_132))
    {
        if (CalculateDistance_419020(Fix16(25)))
        {
            if (VolCalc_419070(0x28u, Fix16(5), pParams->mbHasSolidAbove))
            {
                this->msQueueSample.mType = 15;
                this->msQueueSample.mSampleIndex = 15;
                this->msQueueSample.mSoundIntensity = Fix16(5);
                this->msQueueSample.mnEmittingVolume = 40;
                this->msQueueSample.mMaxDistance = 10;
                this->msQueueSample.mReleasingSoundFlag = 0;
                this->msQueueSample.mbIs2D = false;
                this->msQueueSample.mReleasingVolumeModificator = 7;
                AddSampleToRequestedQueue_41A850();
            }
        }
    }
}

MATCH_FUNC(0x415100)
bool sound_obj::Type_15_InitCarBombSound_415100(sound_0x68* pSound)
{
    pSound->mSampIdx = 57;
    pSound->mSpeedMultiplier = 400;
    pSound->mReleasingVolumeDivider = 3;
    pSound->mRate = 22050;
    pSound->mLoopCount = 0;
    pSound->mLoopStart = gSampManager_6FFF00.GetLoopStart_58DC30(57);
    pSound->mLoopEnd = gSampManager_6FFF00.GetLoopEnd_58DC50(57);
    return true;
}

MATCH_FUNC(0x415190)
void sound_obj::HandleTankCrushSound_415190(Sound_Params* pParams)
{

    Car* pCar = pParams->mpObj->mCarBcPtr;
    if (pCar->IsEngineOn_4118C0() && pCar->IsCrushing_414F70())
    {
        if (CalculateDistance_419020(Fix16(921600, 0)))
        {
            Fix16 vol_mult;
            vol_mult = pCar->GetCarLinearSpeed_43A240();
            Fix16 max_speed = gCarInfo_48_6FE258->mMaxSpeed;
            if (max_speed > kFpZero_66F3F0)
            {
                if (vol_mult > max_speed)
                {
                    vol_mult = max_speed;
                }

                u8 vol = Fix16::Round_To_Int_410BF0((vol_mult / max_speed) * Fix16(80));

                if (VolCalc_419070(vol, Fix16(122880, 0), pParams->mbHasSolidAbove))
                {
                    this->msQueueSample.mSoundIntensity = Fix16(122880, 0);
                    this->msQueueSample.mnEmittingVolume = vol;
                    this->msQueueSample.mMaxDistance = 15;
                    this->msQueueSample.mType = 13;
                    this->msQueueSample.mSampleIndex = 14;
                    this->msQueueSample.mReleasingSoundFlag = 0;
                    this->msQueueSample.mReleasingVolumeModificator = 16;
                    this->msQueueSample.mbIs2D = false;
                    AddSampleToRequestedQueue_41A850();
                }
            }
        }
    }
}

MATCH_FUNC(0x4153F0)
bool sound_obj::Type_13_InitTankCrushSound_4153F0(sound_0x68* pObj)
{
    pObj->mSampIdx = 36;
    pObj->mSpeedMultiplier = 400;
    pObj->mReleasingVolumeDivider = 3;
    pObj->mRate = gSampManager_6FFF00.GetPlayBackRate_58DBF0(36);
    pObj->mLoopCount = 0;
    pObj->mLoopStart = gSampManager_6FFF00.GetLoopStart_58DC30(36);
    pObj->mLoopEnd = gSampManager_6FFF00.GetLoopEnd_58DC50(36);
    return true;
}

MATCH_FUNC(0x415480)
void sound_obj::PlayCarShopEventSound_415480(Sound_Params* pParams)
{
    Car* pCar = pParams->mpObj->mCarBcPtr;
    const s32 shop_event = pCar->TakeShopEvent_4119D0();
    switch (shop_event)
    {
        case car_shop_event::respray_1:
            this->msQueueSample.mSampIdx = 67;
            break;
        case car_shop_event::change_plates_2:
        case 3:
            this->msQueueSample.mSampIdx = 66;
            break;
        case car_shop_event::car_bomb_4:
            this->msQueueSample.mSampIdx = 65;
            break;
        case car_shop_event::car_mines_5:
            this->msQueueSample.mSampIdx = 62;
            break;
        case car_shop_event::oil_stain_6:
            this->msQueueSample.mSampIdx = 63;
            break;
        case car_shop_event::car_smg_7:
            this->msQueueSample.mSampIdx = 64;
            break;
        default:
            return;
    }

    this->msQueueSample.mnEmittingVolume = 50;
    this->msQueueSample.mMaxDistance = 100;
    this->msQueueSample.mType = 20;
    this->msQueueSample.mSoundIntensity = Fix16(100);
    this->msQueueSample.mSampleIndex = 3;
    this->msQueueSample.mReleasingSoundFlag = 1;
    this->msQueueSample.mbIs2D = true;
    this->msQueueSample.mReleasingVolumeModificator = 0;
    this->msQueueSample.mSpeedMultiplier = 0;
    this->msQueueSample.mRate = gSampManager_6FFF00.GetPlayBackRate_58DBF0(msQueueSample.mSampIdx);
    this->msQueueSample.mLoopCount = 1;
    this->msQueueSample.mLoopStart = 0;
    this->msQueueSample.mLoopEnd = -1;
    this->msQueueSample.mPan = 63;
    AddSampleToRequestedQueue_41A850();
}

MATCH_FUNC(0x415570)
void sound_obj::HandleCarHornSound_415570(Sound_Params* pParams, SoundPosEntry* pAlloc)
{
    Car* pCar = pParams->mpObj->mCarBcPtr;
    if (pCar->IsFireTruck_4118F0() || !pCar->HasEmergencyLights_414F20() || !pCar->AreEmergencyLightsOn_414F80())
    {
        if (!pCar->IsFireTruck_4118F0() && !pCar->IsTank_411900() && !pCar->IsGunJeep_411910() || !pCar->IsTurretRotated_4119F0())
        {
            if (pParams->mpObj->mCarBcPtr->IsEmittingHorn_411970())
            {
                bool tryPlay = false;
                if (pParams->mbDrivenByPlayer || pCar->GetCarInfoIdx_411940() == car_model_enum::ICECREAM)
                {
                    tryPlay = true;
                }
                else if (pCar->IsEmittingHorn_411990())
                {
                    u8 horn_timer = pCar->mHorn;
                    if (horn_timer >= 44u)
                    {
                        pAlloc->mHornPatternIdx = this->manRandomTable[4] & 7;
                        horn_timer = 44;
                    }

                    if (kHornPatterns_5FE434[pAlloc->mHornPatternIdx][static_cast<u8>(44u - horn_timer)])
                    {
                        tryPlay = true;
                    }
                }

                if (tryPlay && CalculateDistance_419020((Fix16(15) / Fix16(2)) * (Fix16(15) / Fix16(2))))
                {
                    const u8 vol = pCar->GetCarInfoIdx_411940() == car_model_enum::TRAINCAB ? 127 : 20;
                    if (VolCalc_419070(vol, Fix16(15) / Fix16(2), pParams->mbHasSolidAbove))
                    {
                        this->msQueueSample.mSoundIntensity = Fix16(15) / Fix16(2);
                        this->msQueueSample.mnEmittingVolume = vol;
                        this->msQueueSample.mMaxDistance = 15;
                        this->msQueueSample.mType = 5;
                        this->msQueueSample.mSampleIndex = 8;
                        this->msQueueSample.mReleasingSoundFlag = 0;
                        this->msQueueSample.mbIs2D = false;
                        if (pParams->mbDrivenByPlayer)
                        {
                            this->msQueueSample.mReleasingVolumeModificator = 2;
                        }
                        else
                        {
                            this->msQueueSample.mReleasingVolumeModificator = 4;
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
    pSound->mRate = 22050;

    Car* pCar;
    if (GetCar_4145E0(pSound->mEntityIndex, &pCar))
    {
        const s32 car_model = pCar->mCarInfoIdx;
        pSound->mSampIdx = GetVehicleAudioClass_417BA0(car_model);
        pSound->mSpeedMultiplier = 600;
        pSound->mReleasingVolumeDivider = 1;
        pSound->mRate = ConvertToPlayBackRate_417C60(car_model);
        pSound->mLoopCount = 0;
        pSound->mLoopStart = gSampManager_6FFF00.GetLoopStart_58DC30(pSound->mSampIdx);
        pSound->mLoopEnd = gSampManager_6FFF00.GetLoopEnd_58DC50(pSound->mSampIdx);
        return true;
    }
    return false;
}

MATCH_FUNC(0x4157C0)
void sound_obj::HandleCarEngineSound_4157C0(Sound_Params* pParams)
{
    Car* pCar = pParams->mpObj->mCarBcPtr;

    if (pCar->mEngineStatus == car_engine_status::on_3 && CalculateDistance_419020(Fix16(36)))
    {
        Fix16 gas_pedal;
        gas_pedal = pCar->GetCarLinearSpeed_43A240();
        Fix16 max_speed = gCarInfo_48_6FE258->mMaxSpeed;
        u8 emitting_vol;
        if (max_speed > kFpZero_66F3F0)
        {
            if (gas_pedal == kFpZero_66F3F0)
            {
                if (pCar->mScale != kFpOne_66F3F4)
                {
                    return;
                }

                max_speed = kFpFive;
                if (!pCar->mPhysics->mIsBackwardGasOn)
                {
                    max_speed = kFpFive_66F1D0;
                }
                gas_pedal = pCar->mPhysics->mGasPedal;
                emitting_vol = Fix16::Round_To_Int_410BF0(Fix16(35) * (gas_pedal / max_speed)) + 25;
            }
            else
            {
                if (gas_pedal > max_speed)
                {
                    gas_pedal = max_speed;
                }

                if (pCar->mCarInfoIdx == car_model_enum::TANK || pCar->mPhysics->mIsBackwardGasOn)
                {
                    emitting_vol = Fix16::Round_To_Int_410BF0(Fix16(35) * (gas_pedal / max_speed)) + 25;
                }
                else if (pCar->mScale != kFpOne_66F3F4)
                {
                    emitting_vol = Fix16::Round_To_Int_410BF0(Fix16(40) * (gas_pedal / max_speed));
                }
                else
                {
                    Fix16 gear2_speed = gCarInfo_48_6FE258->mGear2Speed;
                    Fix16 gear3_speed = gCarInfo_48_6FE258->mGear3Speed;
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

        if (VolCalc_419070(emitting_vol, Fix16(6), pParams->mbHasSolidAbove))
        {
            this->msQueueSample.mSoundIntensity = Fix16(6);
            this->msQueueSample.mnEmittingVolume = emitting_vol;
            this->msQueueSample.mMaxDistance = 12;
            this->msQueueSample.mType = 1;
            this->msQueueSample.mSampleIndex = 0;
            this->msQueueSample.mReleasingSoundFlag = 0;
            if (pParams->mbDrivenByPlayer)
            {
                this->msQueueSample.mReleasingVolumeModificator = 3;
            }
            else
            {
                this->msQueueSample.mReleasingVolumeModificator = 5;
            }
            this->msQueueSample.mbIs2D = false;
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
    if (pSound->mType == 1)
    {
        pSound->mRate = 8000;
    }
    else
    {
        pSound->mRate = 8000;
    }
    Car* pCar;
    if (GetCar_4145E0(pSound->mEntityIndex, &pCar) && pCar->mEngineStatus == car_engine_status::on_3)
    {
        if (pCar->mPhysics)
        {
            pCar->mPhysics->SetModelPhysicsGlobal_562EB0();
            Fix16 max_speed = gCarInfo_48_6FE258->mMaxSpeed;
            Fix16 speed;
            speed = pCar->GetCarLinearSpeed_43A240();

            if (max_speed > kFpZero_66F3F0)
            {
                if (speed == kFpZero_66F3F0)
                {
                    max_speed = kFpFive;
                    if (!pCar->mPhysics->is_backward_gas_on_411810())
                    {
                        max_speed = kFpFive_66F1D0;
                    }

                    if (pSound->mType == 1)
                    {
                        if (pCar->mScale == kFpOne_66F3F4)
                        {
                            speed = pCar->mPhysics->mGasPedal;
                            pSound->mRate = Fix16::Round_To_Int_410BF0(Fix16(22000) * (speed / max_speed)) + 8000;
                        }
                        else
                        {
                            speed = pCar->mPhysics->mGasPedal;
                            pSound->mRate = Fix16::Round_To_Int_410BF0(Fix16(19000) * (speed / max_speed)) + 11000;
                        }
                    }
                    else
                    {
                        speed = pCar->mPhysics->mGasPedal;
                        pSound->mRate = Fix16::Round_To_Int_410BF0(Fix16(33075) * (speed / max_speed)) + 11025;
                    }
                }
                else
                {
                    if (speed > max_speed)
                    {
                        speed = max_speed;
                    }

                    if (pCar->IsTank_411900() || pCar->mPhysics->mIsBackwardGasOn)
                    {
                        pSound->mRate = Fix16::Round_To_Int_410BF0(Fix16(16000) * (speed / max_speed)) + 12000;
                    }
                    else if (pCar->mScale != kFpOne_66F3F4)
                    {
                        pSound->mRate = Fix16::Round_To_Int_410BF0(Fix16(19000) * (speed / max_speed)) + 11000;
                    }
                    else
                    {
                        Fix16 gear2_speed = gCarInfo_48_6FE258->mGear2Speed;
                        Fix16 gear3_speed = gCarInfo_48_6FE258->mGear3Speed;
                        if (speed < gear2_speed)
                        {
                            if (pSound->mType == 1)
                            {
                                pSound->mRate = Fix16::Round_To_Int_410BF0(Fix16(16000) * (speed / gear2_speed)) + 12000;
                            }
                            else
                            {
                                pSound->mRate = Fix16::Round_To_Int_410BF0(Fix16(33075) * (speed / max_speed)) + 11025;
                            }
                        }
                        else if (speed < gear3_speed)
                        {
                            pSound->mRate = pSound->mType == 1
                                ? Fix16::Round_To_Int_410BF0(Fix16(16000) * ((speed - gear2_speed) / (gear3_speed - gear2_speed))) + 14600
                                : Fix16::Round_To_Int_410BF0(Fix16(33075) * (speed / max_speed)) + 11025;
                        }
                        else
                        {
                            if (pSound->mType == 1)
                            {
                                pSound->mRate = Fix16::Round_To_Int_410BF0(
                                                        (speed - gear3_speed) / (max_speed - gear3_speed) * (Fix16(16000) * kFpHalf_66F258)) +
                                    16000;
                            }
                            else
                            {
                                pSound->mRate = Fix16::Round_To_Int_410BF0(Fix16(33075) * (speed / max_speed)) + 11025;
                            }
                        }
                    }
                }
            }

            if (pCar->IsCarInAir_43A3C0())
            {
                pSound->mRate <<= 1;
            }
        }

        if (pSound->mType == 1)
        {
            if (pCar->mScale != kFpOne_66F3F4)
            {
                pSound->mSampIdx = 9;
            }
            else
            {
                pSound->mSampIdx = samp_idx_for_model_417AC0(pCar->mCarInfoIdx);
            }
            pSound->mSpeedMultiplier = 600;
        }
        else
        {
            pSound->mSampIdx = 21;
            pSound->mSpeedMultiplier = 400;
        }

        pSound->mReleasingVolumeDivider = 3;
        pSound->mLoopCount = 0;
        // A local for the first index only: the original loads it into eax before `this` in ecx
        s32 idx = pSound->mSampIdx;
        pSound->mLoopStart = gSampManager_6FFF00.GetLoopStart_58DC30(idx);
        pSound->mLoopEnd = gSampManager_6FFF00.GetLoopEnd_58DC50(pSound->mSampIdx);
        return true;
    }
    return false;
}

MATCH_FUNC(0x417060)
void sound_obj::HandleCarAlarmSound_417060(Sound_Params* pParams)
{
    if (pParams->mpObj->mCarBcPtr->mFlashCount)
    {
        if (CalculateDistance_419020(Fix16(2560000, 0)))
        {
            if (VolCalc_419070(0x32u, Fix16(204800, 0), pParams->mbHasSolidAbove))
            {
                this->msQueueSample.mSoundIntensity = Fix16(204800, 0);
                this->msQueueSample.mnEmittingVolume = 50;
                this->msQueueSample.mMaxDistance = 25;
                this->msQueueSample.mType = 3;
                this->msQueueSample.mSampleIndex = 6;
                this->msQueueSample.mReleasingSoundFlag = 0;
                this->msQueueSample.mbIs2D = false;
                if (pParams->mbDrivenByPlayer)
                {
                    this->msQueueSample.mReleasingVolumeModificator = 3;
                }
                else
                {
                    this->msQueueSample.mReleasingVolumeModificator = 5;
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
    if (GetCar_4145E0(pSound->mEntityIndex, &pCar))
    {
        s32 car_model = pCar->mCarInfoIdx;
        // The model picks one of four alarm tones and shifts its pitch
        switch (car_model & 3)
        {
            default:
                pSound->mSampIdx = 13;
                pSound->mRate = gSampManager_6FFF00.GetPlayBackRate_58DBF0(13) + 30 * car_model;
                break;

            case 2:
                pSound->mSampIdx = 12;
                pSound->mRate = 29 * car_model + gSampManager_6FFF00.GetPlayBackRate_58DBF0(12);
                break;

            case 1:
                pSound->mSampIdx = 13;
                pSound->mRate = 2 * (gSampManager_6FFF00.GetPlayBackRate_58DBF0(13) - 29 * car_model);
                break;

            case 0:
                pSound->mSampIdx = 12;
                pSound->mRate = 2 * (gSampManager_6FFF00.GetPlayBackRate_58DBF0(12) - 25 * car_model);
                break;
        }
        pSound->mSpeedMultiplier = 800;
        pSound->mReleasingVolumeDivider = 3;
        pSound->mLoopCount = 0;
        pSound->mLoopStart = gSampManager_6FFF00.GetLoopStart_58DC30(pSound->mSampIdx);
        pSound->mLoopEnd = gSampManager_6FFF00.GetLoopEnd_58DC50(pSound->mSampIdx);
        return true;
    }
    return false;
}

MATCH_FUNC(0x4177D0)
void sound_obj::HandleCarDamageSound_4177D0(Sound_Params* pParams)
{
    Car* pCar = pParams->mpObj->mCarBcPtr;
    if (pCar->IsEngineOn_4118C0() && pCar->mDamage > 16000)
    {
        if (CalculateDistance_419020(Fix16(49)))
        {
            u8 vol = 60 * (pParams->mpObj->mCarBcPtr->mDamage - 16000) / 16000;
            if (VolCalc_419070(vol,
                               Fix16(7),
                               pParams->mbHasSolidAbove))
            {
                this->msQueueSample.mSoundIntensity = Fix16(7);
                this->msQueueSample.mnEmittingVolume = vol;
                this->msQueueSample.mMaxDistance = 14;
                this->msQueueSample.mType = 6;
                this->msQueueSample.mSampleIndex = 1;
                this->msQueueSample.mReleasingSoundFlag = 0;
                this->msQueueSample.mbIs2D = false;
                this->msQueueSample.mReleasingVolumeModificator = 7;
                AddSampleToRequestedQueue_41A850();
            }
        }
    }
}

MATCH_FUNC(0x4178C0)
void sound_obj::HandleSirenActivationSound_4178C0(Sound_Params* pParams)
{
    Car* pCar = pParams->mpObj->mCarBcPtr;
    if ((pCar->HasEmergencyInfoFlag() || pCar->IsFbiCar_411920()) && (pCar->mBfA4.bEmergencyLights))
    {
        Car* pCar_ = pParams->mpObj->mCarBcPtr;
        if (pCar_->mEngineStatus == car_engine_status::on_3)
        {
            if (pCar_->mDriver)
            {
                if (CalculateDistance_419020(Fix16(400)))
                {
                    if (VolCalc_419070(0x46u, Fix16(20), pParams->mbHasSolidAbove))
                    {
                        this->msQueueSample.mSoundIntensity = Fix16(20);
                        this->msQueueSample.mnEmittingVolume = 70;
                        this->msQueueSample.mMaxDistance = 40;
                        this->msQueueSample.mType = 4;
                        this->msQueueSample.mSampleIndex = 7;
                        this->msQueueSample.mReleasingSoundFlag = 0;
                        this->msQueueSample.mbIs2D = false;
                        if (pParams->mbDrivenByPlayer)
                        {
                            this->msQueueSample.mReleasingVolumeModificator = 2;
                        }
                        else
                        {
                            this->msQueueSample.mReleasingVolumeModificator = 4;
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
    if (GetCar_4145E0(pSound->mEntityIndex, &pCar))
    {
        s32 model = pCar->GetCarInfoIdx_411940();
        pSound->mSampIdx = GetSirenSampleIdx_417B80(pCar->GetCarInfoIdx_411940(), pCar->IsEmittingHorn_411970());
        pSound->mSpeedMultiplier = 800;
        pSound->mReleasingVolumeDivider = 2;

        pSound->mRate = gSampManager_6FFF00.GetPlayBackRate_58DBF0(pSound->mSampIdx);
        if (model == car_model_enum::FIRETRUK || model == car_model_enum::MEDICAR)
        {
            pSound->mRate = 9 * pSound->mRate / 10u;
        }
        pSound->mLoopCount = 0;
        pSound->mLoopStart = gSampManager_6FFF00.GetLoopStart_58DC30(pSound->mSampIdx);
        pSound->mLoopEnd = gSampManager_6FFF00.GetLoopEnd_58DC50(pSound->mSampIdx);
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
    Car* pCar = pParams->mpObj->mCarBcPtr;
    Fix16 car_speed;
    car_speed = pCar->GetCarLinearSpeed_43A240();
    if (pAlloc->mLastSpeed > kFpZero_66F3F0 && car_speed == kFpZero_66F3F0)
    {
        if (IsHeavyTruckOrBus_417F40(pCar->GetCarInfoIdx_411940()))
        {
            if (CalculateDistance_419020(Fix16(25)))
            {
                if (VolCalc_419070(0x28u, Fix16(5), pParams->mbHasSolidAbove))
                {
                    this->msQueueSample.mSoundIntensity = Fix16(5);
                    this->msQueueSample.mnEmittingVolume = 40;
                    this->msQueueSample.mMaxDistance = 10;
                    this->msQueueSample.mType = 7;
                    this->msQueueSample.mSampleIndex = 2;
                    this->msQueueSample.mReleasingSoundFlag = 1;
                    this->msQueueSample.mbIs2D = false;
                    this->msQueueSample.mReleasingVolumeModificator = 7;
                    AddSampleToRequestedQueue_41A850();
                }
            }
        }
    }
    pAlloc->mLastSpeed = car_speed;
}

MATCH_FUNC(0x417EF0)
bool sound_obj::Type_7_InitHeavyVehicleStopSound_417EF0(sound_0x68* pObj)
{
    pObj->mSampIdx = 29;
    pObj->mSpeedMultiplier = 400;
    pObj->mRate = RandomDisplacement_41A650(29) + gSampManager_6FFF00.GetPlayBackRate_58DBF0(29);
    pObj->mLoopCount = 1;
    pObj->mLoopStart = 0;
    pObj->mLoopEnd = -1;
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
    Car* pCar = pParams->mpObj->mCarBcPtr;
    CarPhysics* pPhysics = pCar->mPhysics;

    Ang16 slide_angle = pPhysics->mLinvel1.atan2_40F790().SubtractNormalized_409340(pPhysics->mTheta);
    if (slide_angle > kMinCorneringAngle_66F274 && slide_angle < kMaxCorneringAngle_66F370)
    {
        if (pCar->GetCarLinearSpeed_43A240() > kMinCorneringSpeed)
        {
            switch (pParams->mpObj->mCarBcPtr->mCarInfoIdx)
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
                        if (VolCalc_419070(25, Fix16(5), pParams->mbHasSolidAbove))
                        {
                            this->msQueueSample.mSoundIntensity = Fix16(5);
                            this->msQueueSample.mnEmittingVolume = 25;
                            this->msQueueSample.mMaxDistance = 10;
                            this->msQueueSample.mType = 8;
                            this->msQueueSample.mSampleIndex = 11;
                            this->msQueueSample.mReleasingSoundFlag = 0;
                            this->msQueueSample.mReleasingVolumeModificator = 7;
                            this->msQueueSample.mbIs2D = false;
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
    pObj->mSampIdx = 24;
    pObj->mSpeedMultiplier = 400;
    pObj->mReleasingVolumeDivider = 2;
    pObj->mRate = gSampManager_6FFF00.GetPlayBackRate_58DBF0(24);
    pObj->mLoopCount = 0;
    pObj->mLoopStart = gSampManager_6FFF00.GetLoopStart_58DC30(pObj->mSampIdx);
    pObj->mLoopEnd = gSampManager_6FFF00.GetLoopEnd_58DC50(pObj->mSampIdx);
    return true;
}

MATCH_FUNC(0x418190)
void sound_obj::HandleAICarEngineSound_418190(Sound_Params* pParams)
{
    if (!pParams->mbDrivenByPlayer)
    {
        Fix16 max_speed = gCarInfo_48_6FE258->mMaxSpeed;
        Fix16 speed;
        speed = pParams->mpObj->mCarBcPtr->GetCarLinearSpeed_43A240();
        if (max_speed > kFpZero_66F3F0 && speed > kFpZero_66F3F0 && CalculateDistance_419020(Fix16(81)))
        {
            u8 emitting_vol = Fix16::Round_To_Int_410BF0((speed / max_speed) * Fix16(11));
            if (emitting_vol > 0)
            {
                if (VolCalc_419070(emitting_vol, Fix16(9), pParams->mbHasSolidAbove))
                {
                    this->msQueueSample.mSoundIntensity = Fix16(9);
                    this->msQueueSample.mnEmittingVolume = emitting_vol;
                    this->msQueueSample.mMaxDistance = 18;
                    this->msQueueSample.mType = 2;
                    this->msQueueSample.mSampleIndex = 2;
                    this->msQueueSample.mReleasingSoundFlag = 0;
                    this->msQueueSample.mbIs2D = false;
                    this->msQueueSample.mReleasingVolumeModificator = 17;
                    AddSampleToRequestedQueue_41A850();
                }
            }
        }
    }
}

MATCH_FUNC(0x4182A0)
bool sound_obj::Type_2_InitAICarEngineSound_4182A0(sound_0x68* pObj)
{
    pObj->mSampIdx = 0;
    pObj->mSpeedMultiplier = 700;
    pObj->mLoopCount = 0;
    pObj->mLoopStart = 0;
    pObj->mLoopEnd = -1;
    pObj->mRate = gSampManager_6FFF00.GetPlayBackRate_58DBF0(0);
    pObj->mReleasingVolumeDivider = 5;
    return true;
}

MATCH_FUNC(0x4182E0)
void sound_obj::HandleCarDoorSounds_4182E0(Sound_Params* pParams)
{
    bool bQueued1 = false;
    bool bQueued2 = false;
    Car* pCar = pParams->mpObj->mCarBcPtr;
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

        if (!bQueued1 && pCar->mDoors[i].IsStartingToOpen())
        {
            bQueued1 = true;
            if (CalculateDistance_419020(Fix16(0xE1000, 0)))
            {
                if (VolCalc_419070(0x32u, Fix16(0x1E000, 0), pParams->mbHasSolidAbove))
                {
                    this->msQueueSample.mSoundIntensity = Fix16(0x1E000, 0);
                    this->msQueueSample.mnEmittingVolume = 50;
                    this->msQueueSample.mMaxDistance = 15;
                    this->msQueueSample.mType = 20;
                    this->msQueueSample.mSampleIndex = 9;
                    this->msQueueSample.mReleasingSoundFlag = 1;
                    this->msQueueSample.mbIs2D = false;
                    if (pParams->mbDrivenByPlayer)
                    {
                        this->msQueueSample.mReleasingVolumeModificator = 3;
                    }
                    else
                    {
                        this->msQueueSample.mReleasingVolumeModificator = 5;
                    }
                    this->msQueueSample.mSampIdx = get_samp_idx_for_car_417D70(pCar, true, bLargeDoorOffset);
                    this->msQueueSample.mSpeedMultiplier = 0;
                    this->msQueueSample.mRate =
                        RandomDisplacement_41A650(this->msQueueSample.mSampIdx) +
                        gSampManager_6FFF00.GetPlayBackRate_58DBF0(this->msQueueSample.mSampIdx);
                    this->msQueueSample.mLoopCount = 1;
                    this->msQueueSample.mLoopStart = 0;
                    this->msQueueSample.mLoopEnd = -1;
                    AddSampleToRequestedQueue_41A850();
                }
            }
        }

        if (!bQueued2)
        {
            if (pCar->mDoors[i].IsStartingToClose())
            {
                bQueued2 = true;
                if (CalculateDistance_419020(Fix16(0xE1000, 0)))
                {
                    if (VolCalc_419070(0x32u, Fix16(0x1E000, 0), pParams->mbHasSolidAbove))
                    {
                        this->msQueueSample.mSoundIntensity = Fix16(0x1E000, 0);
                        this->msQueueSample.mnEmittingVolume = 50;
                        this->msQueueSample.mMaxDistance = 15;
                        this->msQueueSample.mType = 20;
                        this->msQueueSample.mSampleIndex = 10;
                        this->msQueueSample.mReleasingSoundFlag = 0;
                        this->msQueueSample.mbIs2D = false;
                        if (pParams->mbDrivenByPlayer)
                        {
                            this->msQueueSample.mReleasingVolumeModificator = 3;
                        }
                        else
                        {
                            this->msQueueSample.mReleasingVolumeModificator = 5;
                        }
                        this->msQueueSample.mSampIdx = get_samp_idx_for_car_417D70(pCar, false, bLargeDoorOffset);
                        this->msQueueSample.mSpeedMultiplier = 0;
                        this->msQueueSample.mRate =
                            RandomDisplacement_41A650(this->msQueueSample.mSampIdx) +
                            gSampManager_6FFF00.GetPlayBackRate_58DBF0(this->msQueueSample.mSampIdx);
                        this->msQueueSample.mLoopCount = 1;
                        this->msQueueSample.mLoopStart = 0;
                        this->msQueueSample.mLoopEnd = -1;
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
    if (pParams->mpObj->mCarBcPtr->mEngineStatus == car_engine_status::starting_4)
    {
        if (CalculateDistance_419020(Fix16(25)))
        {
            if (VolCalc_419070(0x46u, Fix16(5), pParams->mbHasSolidAbove))
            {
                this->msQueueSample.mSoundIntensity = Fix16(5);
                this->msQueueSample.mnEmittingVolume = 70;
                this->msQueueSample.mMaxDistance = 10;
                this->msQueueSample.mType = 9;
                this->msQueueSample.mSampleIndex = 12;
                this->msQueueSample.mReleasingSoundFlag = 1;
                this->msQueueSample.mReleasingVolumeModificator = 2;
                this->msQueueSample.mbIs2D = false;
                if (pParams->mpObj->mCarBcPtr->mCarInfoIdx % 2)
                {
                    this->msQueueSample.mSampIdx = 1;
                }
                else
                {
                    this->msQueueSample.mSampIdx = 2;
                }
                AddSampleToRequestedQueue_41A850();
            }
        }
    }
}

MATCH_FUNC(0x4186D0)
bool sound_obj::Type_9_InitCarEngineStartSound_4186D0(sound_0x68* pObj)
{
    pObj->mSpeedMultiplier = 600;
    pObj->mLoopCount = 1;
    pObj->mLoopStart = 0;
    pObj->mLoopEnd = -1;
    pObj->mRate =
        RandomDisplacement_41A650(pObj->mSampIdx) + gSampManager_6FFF00.GetPlayBackRate_58DBF0(pObj->mSampIdx);
    return true;
}

MATCH_FUNC(0x418720)
void sound_obj::HandleCarTireScrubSound_418720(Sound_Params* pParams)
{
    u8 emitVol = 0;

    Car* pCar = pParams->mpObj->mCarBcPtr;
    CarPhysics* pPhysics = pCar->mPhysics;

    if (pPhysics)
    {
        s32 block_spec = pPhysics->mBlockSpec;
        if (block_spec && block_spec != 4)
        {
            if (pPhysics->mDriveWheelsLockedQ > 0)
            {
                if (gCarInfo_48_6FE258->mMaxSpeed > kFpZero_66F3F0)
                {
                    // The copy through a pointer to the returned temporary is folded into the /= below, so the
                    // speed is read through the returned pointer after the divisor's load, like the original.
                    // speed has to be declared before pSpeed.
                    Fix16 speed;
                    Fix16* pSpeed = &pCar->GetCarLinearSpeed_43A240();
                    speed = *pSpeed;
                    speed /= gCarInfo_48_6FE258->mMaxSpeed;
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
                if (pPhysics->mFrontSkid > gCarInfo_2C_6FE0E4->mSkidThreshhold1)
                {
                    Fix16 front_scrub = (pPhysics->mFrontSkid - gCarInfo_2C_6FE0E4->mSkidThreshhold1) / gCarInfo_2C_6FE0E4->mSkidThreshhold1;
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

                if (pPhysics->mRearSkid > gCarInfo_2C_6FE0E4->mSkidThreshhold2)
                {
                    Fix16 rear_scrub = (pPhysics->mRearSkid - gCarInfo_2C_6FE0E4->mSkidThreshhold2) / gCarInfo_2C_6FE0E4->mSkidThreshhold2;
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
                    if (VolCalc_419070(emitVol, Fix16(5), pParams->mbHasSolidAbove))
                    {
                        this->msQueueSample.mSoundIntensity = Fix16(5);
                        this->msQueueSample.mnEmittingVolume = emitVol;
                        this->msQueueSample.mMaxDistance = 10;
                        this->msQueueSample.mType = 10;
                        this->msQueueSample.mSampleIndex = 13;
                        this->msQueueSample.mReleasingSoundFlag = 0;
                        this->msQueueSample.mReleasingVolumeModificator = 5;
                        this->msQueueSample.mbIs2D = false;
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
    if (GetCar_4145E0(pSound->mEntityIndex, &pCar))
    {
        CarPhysics* pPhysics = pCar->mPhysics;
        if (pPhysics)
        {
            pSound->mSampIdx = 22;
            s32 rate;
            switch (pPhysics->mBlockSpec)
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

            if (pPhysics->mDriveWheelsLockedQ > 0)
            {
                if (gCarInfo_48_6FE258->mMaxSpeed > kFpZero_66F3F0)
                {
                    // Written out: with `GetCarLinearSpeed() / gCarInfo->field_28` VC6 binds the divisor's
                    // reference before the call (`lea 0x28(%ecx)` held across it); the original reloads
                    // gCarInfo_48 after the shift, which only the raw expression gives.
                    skid_amount = Fix16(static_cast<s32>((static_cast<__int64>(pCar->GetCarLinearSpeed_43A240().mValue) << 14) / gCarInfo_48_6FE258->mMaxSpeed.mValue), 0);
                    pSound->mRate = rate + Fix16::Round_To_Int_410BF0(Fix16(6000) * skid_amount);
                }
                else
                {
                    pSound->mRate = rate;
                }
            }
            else
            {
                if (pPhysics->mFrontSkid > gCarInfo_2C_6FE0E4->mSkidThreshhold1)
                {
                    front_skid = (pPhysics->mFrontSkid - gCarInfo_2C_6FE0E4->mSkidThreshhold1) /
                        gCarInfo_2C_6FE0E4->mSkidThreshhold1;
                    if (front_skid > kFpOne_66F3F4)
                    {
                        front_skid = kFpOne_66F3F4;
                    }
                }

                if (pPhysics->mRearSkid > gCarInfo_2C_6FE0E4->mSkidThreshhold2)
                {
                    rear_skid = (pPhysics->mRearSkid - gCarInfo_2C_6FE0E4->mSkidThreshhold2) /
                        gCarInfo_2C_6FE0E4->mSkidThreshhold2;
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

                pSound->mRate = rate + Fix16::Round_To_Int_410BF0(Fix16(6000) * skid_amount);
            }

            pSound->mSpeedMultiplier = 600;
            pSound->mLoopCount = 0;
            pSound->mLoopStart = gSampManager_6FFF00.GetLoopStart_58DC30(pSound->mSampIdx);
            pSound->mLoopEnd = gSampManager_6FFF00.GetLoopEnd_58DC50(pSound->mSampIdx);
            pSound->mReleasingVolumeDivider = 3;
            return true;
        }
    }
    return false;
}

MATCH_FUNC(0x418B60)
void sound_obj::ProcessType11_HudPager_418B60(s32 entity_idx)
{
    Hud_Pager* pPager = mAudioEntities[entity_idx].mpObj->mpAny.pHud_Pager_C;
    if (pPager)
    {
        if (pPager->get_timer_411A40() > 0)
        {
            u8 vol;
            if (pPager->get_timer_411A40() > 300)
            {
                msQueueSample.mRate = 22050;
                vol = 40;
            }
            else if (pPager->get_timer_411A40() > 150)
            {
                msQueueSample.mRate = 26221;
                vol = 55;
            }
            else
            {
                msQueueSample.mRate = 33178;
                vol = 70;
            }

            msQueueSample.mSampIdx = 57;
            msQueueSample.mEntityIndex = entity_idx;
            msQueueSample.mpReleaseSource = NULL;
            msQueueSample.mnVolume = vol;
            msQueueSample.mnEmittingVolume = vol;
            msQueueSample.mMaxDistance = 100;
            msQueueSample.mType = 20;
            msQueueSample.mSoundIntensity = Fix16(100);
            msQueueSample.mSampleIndex = 0;
            msQueueSample.mReleasingSoundFlag = 0;
            msQueueSample.mbIs2D = true;
            msQueueSample.mReleasingVolumeModificator = 0;
            msQueueSample.mSpeedMultiplier = 0;
            msQueueSample.mLoopCount = 0;
            msQueueSample.mLoopStart = 0;
            msQueueSample.mLoopEnd = -1;
            msQueueSample.mPan = 63;
            msQueueSample.mReleasingVolumeDivider = 3;

            AddSampleToRequestedQueue_41A850();
        }
    }
}

MATCH_FUNC(0x418C20)
void sound_obj::AddVocalsEntity_418C20()
{
    if (!mVocals.mEntityIdx)
    {
        SoundObject* pSoundObj = &mVocals.mSoundObj;
        mVocals.mSoundObj.mpAny.pAny = NULL;
        mVocals.mSoundObj.mbStatus = false;
        pSoundObj->mObjectType = SoundObjectTypeEnum::Vocals_10;
        mVocals.mEntityIdx = AddSoundObject_419FA0(pSoundObj);
    }
}

MATCH_FUNC(0x418C60)
void sound_obj::RemoveVocalsEntity_418C60()
{
    if (mVocals.mEntityIdx)
    {
        FreeSoundEntry_41A090(mVocals.mEntityIdx);
        mVocals.mEntityIdx = 0;
    }
}

MATCH_FUNC(0x418C80)
void sound_obj::PlayVocal_418C80(s32 voice)
{
    if (mVocals.mEntityIdx)
    {
        if (voice < 63)
        {
            mVocals.mPendingVoice = voice;
        }
    }
}

MATCH_FUNC(0x418CA0)
void sound_obj::ProcessType10_Vocals_418CA0()
{
    s32 voc_idx;
    if (mVocals.mEntityIdx)
    {
        switch (mVocals.mPendingVoice)
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
                voc_idx = (manRandomTable[0] % 13u) + 85;
                if (voc_idx == 99)
                {
                    mVocals.mPendingVoice = 0;
                    return;
                }
                break;
            default:
                mVocals.mPendingVoice = 0;
                return;
        }

        gSampManager_6FFF00.PlayVocal_58E510(1, voc_idx, true);
        gSampManager_6FFF00.SetVocalVolume_58E6D0(1, 127 * mSfxVol / 127);

        mVocals.mPendingVoice = 0;
    }
}

MATCH_FUNC(0x419020)
bool sound_obj::CalculateDistance_419020(Fix16 max_dist)
{
    if (mDistSquared < max_dist)
    {
        if (!mDistCalculated)
        {
            mDistCalculated = true;
            msQueueSample.mDistance = Fix16::SquareRoot(mDistSquared);
        }
        return true;
    }
    return false;
}

MATCH_FUNC(0x419070)
bool sound_obj::VolCalc_419070(u8 base_volume, Fix16 max_dist, bool bHasSolidAbove)
{
    u8 volume = CalcVolume_41A3F0(base_volume, max_dist, msQueueSample.mDistance);
    msQueueSample.mnVolume = volume;
    if (bHasSolidAbove)
    {
        msQueueSample.mnVolume = volume >> 1;
    }
    return msQueueSample.mnVolume != 0 ? true : false;
}

MATCH_FUNC(0x4190B0)
Fix16 sound_obj::ComputeEmitterDistanceSquared_4190B0() const
{
    Fix16 dx = msQueueSample.mObj.mX - mListenerX;
    Fix16 dy = msQueueSample.mObj.mY - mListenerY;
    return dy * dy + dx * dx;
}

// The three Fix16 fields are set with the Fix16(s32) constructor, not operator=(s32): the
// operator's returned reference chains the four stores, which pushes the mov %esi,%ecx for
// GenerateIntegerRandomNumberTable_41BA90 behind them.
MATCH_FUNC(0x419CD0)
sound_obj::sound_obj()
{
    mRotation = 0;
    mbSoundInitialized = false;
    mSpeedOfSound = 81920;
    mFramesPerSecond = 30;
    mDopplerDivisor = 491520;
    mServiceEnabled = true;
    mSfxVol = 127;
    mCdVol = 127;
    mDistSquared = kFpZero_674CD8;
    mDistCalculated = false;

    mnActiveSampleQueue = 1;
    ClearRequestedQueue_41B700();

    mnActiveSampleQueue = 0;
    ClearRequestedQueue_41B700();

    ClearActivateSamples_41B7A0();

    mUnused1450 = 0;
    mListenerX = Fix16(0);
    mListenerY = Fix16(0);
    mListenerZ = Fix16(0);

    GenerateIntegerRandomNumberTable_41BA90();

    mType5Idx = 0;
    mUnused1B = 0;
    mSampCount = 0;
    mnActiveSamples = 0;
    mUnused5444 = 0;
    mUnused5440 = 0;
    mB3dSound = 0;
    mSampleRate = 22050;
    mUnused18 = 1;
    mUnused19 = 1;
    mIsPaused = false;
    mbDoubleVolume = true;

    for (s32 i = 0; i < 1020; i++)
    {
        mAudioEntities[i].mbUsed = false;
        mAudioEntityOrderList[i] = 0;
    }

    m444CNAudioEntitiesCount = 0;
    mMFrameCounter = 0;
    mVocals.mFifthFrameFlag = false;
    mVocals.mPendingVoice = 0;
    mbFirstMenuStream = 1;
}

MATCH_FUNC(0x419E10)
void sound_obj::Init_419E10()
{
    if (!mbSoundInitialized)
    {
        PreSoundInit_412240();
        mbSoundInitialized = gSampManager_6FFF00.SoundInit_58D6C0(&mUnused5440);
        if (mbSoundInitialized)
        {
            mB3dSound = 0;
            mnActiveSamples = 16;
            AddVocalsEntity_418C20();
        }
    }
}

MATCH_FUNC(0x419E50)
void sound_obj::DeinitializeAudio_419E50()
{
    if (mbSoundInitialized)
    {
        DeInitVocals_57EA10();
        RemoveVocalsEntity_418C60();
        for (u32 i = 0; i < mnActiveSamples; ++i)
        {
            if (mB3dSound)
            {
                gSampManager_6FFF00.StopChannel3D_58DFC0(i);
            }
            else
            {
                gSampManager_6FFF00.StopChannel_58DDD0(i);
            }
        }

        for (u32 j = 0; j < m444CNAudioEntitiesCount; j++)
        {
            mAudioEntities[mAudioEntityOrderList[j]].mbUsed = false;
            mAudioEntityOrderList[j] = 0;
        }
        m444CNAudioEntitiesCount = 0;

        PreSoundShutdown_412250();
        gSampManager_6FFF00.Shutdown_58DB30();
        mbSoundInitialized = false;
    }
}

MATCH_FUNC(0x419EF0)
void sound_obj::Service_419EF0()
{
    static char_type bWasPaused_674E24;
    if (gGame_0x40_67E008)
    {
        mIsPaused = gGame_0x40_67E008->mGameState == GameState::Paused_2;
        if (!mIsPaused)
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
        bWasPaused_674E24 = mIsPaused;
    }

    if (mbSoundInitialized)
    {
        if (!mIsPaused)
        {
            if (!(mMFrameCounter++ % 5))
            {
                mVocals.mFifthFrameFlag = true;
            }
            else
            {
                mVocals.mFifthFrameFlag = false;
            }
        }
        ServiceSoundEffects_41A3A0();
    }
}

MATCH_FUNC(0x419FA0)
s32 sound_obj::AddSoundObject_419FA0(SoundObject* pSoundObj)
{
    u32 idx = 1;
    if (!mbSoundInitialized || !pSoundObj)
    {
        return 0;
    }

    AudioEntity* pEntityIter = &mAudioEntities[1];
    while (idx < 1020)
    {
        if (!pEntityIter->mbUsed)
        {
            if (pSoundObj->mObjectType == SoundObjectTypeEnum::Camera_0xBC_5)
            {
                if (!mType5Idx)
                {
                    mType5Idx = idx;
                }
                else
                {
                    return 0;
                }
            }

            ResetEntry_41A6C0(idx);

            mAudioEntities[idx].mpObj = pSoundObj;
            mAudioEntities[idx].mbUsed = true;
            mAudioEntities[idx].mAge = 1;
            mAudioEntityOrderList[m444CNAudioEntitiesCount++] = idx;

            switch (pSoundObj->mObjectType)
            {
                case SoundObjectTypeEnum::Sprite_1:
                {
                    Sprite* pEntSprite = mAudioEntities[idx].mpObj->mpAny.pSprite;
                    if (pEntSprite)
                    {
                        switch (pEntSprite->GetType_416B40())
                        {
                            case sprite_types_enum::car_2: // note: sub eax, 2 added via switch case instead of if
                                SoundPosEntry* pNewObj = new SoundPosEntry();
                                mAudioEntities[idx].mpAlloc = pNewObj;
                                pNewObj->mLastSpeed = kFpZero_674CD8;
                                pNewObj->mUnused8 = 0;
                                pNewObj->mHornPatternIdx = 0;
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
    if (!mbSoundInitialized || idx >= 1020)
    {
        return;
    }

    if (!mAudioEntities[idx].mbUsed)
    {
        return;
    }

    SoundObject* pSoundObj = mAudioEntities[idx].mpObj;
    switch (pSoundObj->mObjectType)
    {
        case SoundObjectTypeEnum::Sprite_1:
            if (pSoundObj->mpAny.pSprite)
            {
                Sprite* pSprite = pSoundObj->mpAny.pSprite;
                switch (pSprite->GetType_416B40())
                {
                    case sprite_types_enum::car_2:
                        if (mAudioEntities[idx].mpAlloc)
                        {
                            delete mAudioEntities[idx].mpAlloc;
                        }
                        break;
                }
                break;
            }
            // fall through

        case SoundObjectTypeEnum::Frontend_2:
            mbFirstMenuStream = 0;
            gSampManager_6FFF00.FadeOut_58E490();
            break;
    }

    mAudioEntities[idx].mbUsed = false;
    mAudioEntities[idx].mpObj->mpAny.pAny = NULL;
    mAudioEntities[idx].mAge = 0;

    if (mAudioEntities[idx].mpObj->mObjectType == SoundObjectTypeEnum::Camera_0xBC_5)
    {
        mType5Idx = 0;
    }

    u32 idx_iter = 0;
    while (idx_iter < m444CNAudioEntitiesCount)
    {
        if (idx == mAudioEntityOrderList[idx_iter])
        {
            if (idx_iter < 1020 - 1)
            {
                memmove(&mAudioEntityOrderList[idx_iter],
                        &mAudioEntityOrderList[idx_iter + 1],
                        sizeof(u32) * (m444CNAudioEntitiesCount - (idx_iter + 1)));
            }

            m444CNAudioEntitiesCount--;
            mAudioEntityOrderList[m444CNAudioEntitiesCount] = 0;
            break;
        }
        idx_iter++;
    }
}

MATCH_FUNC(0x41A1B0)
bool sound_obj::LoadStyle_41A1B0(const char_type* pStyleName)
{
    if (mbSoundInitialized)
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
    mSfxVol = sfxVol;
}

MATCH_FUNC(0x41A270)
void sound_obj::SetCDVol_41A270(u8 cdVol)
{
    mCdVol = cdVol;
}

MATCH_FUNC(0x41A280)
u8 sound_obj::GetCDVol_41A280() const
{
    return mCdVol;
}

MATCH_FUNC(0x41A290)
void sound_obj::Release_41A290()
{
    gSampManager_6FFF00.Release_58E290();
}

MATCH_FUNC(0x41A2A0)
void sound_obj::Reacquire_41A2A0()
{
    mnActiveSampleQueue = 1;
    ClearRequestedQueue_41B700();
    mnActiveSampleQueue = 0;

    ClearRequestedQueue_41B700();
    ClearActivateSamples_41B7A0();

    mUnused1450 = 0;

    gSampManager_6FFF00.Reacquire_58E2A0();
}

MATCH_FUNC(0x41A2E0)
char_type sound_obj::GetAudioDriveLetter_41A2E0() const
{
    return gSampManager_6FFF00.mGtaAudioDriveLetter;
}

MATCH_FUNC(0x41A2F0)
bool sound_obj::Set3DSound_41A2F0(char_type b3dSound)
{
    if (mbSoundInitialized)
    {
        if (b3dSound != mB3dSound)
        {
            if (b3dSound)
            {
                if (gSampManager_6FFF00.Init3DProvider_58D820(&mSampCount))
                {
                    mB3dSound = 1;
                    mnActiveSamples = mSampCount; // note: regswap from redundant local
                    return true;
                }
                else
                {
                    gSampManager_6FFF00.AllocSamples_58D9F0(1);
                    mB3dSound = 0;
                    mnActiveSamples = 16;
                    return false;
                }
            }

            if (gSampManager_6FFF00.AllocSamples_58D9F0(1))
            {
                mB3dSound = 0;
                mnActiveSamples = 16;
                return true;
            }

            gSampManager_6FFF00.Init3DProvider_58D820(&mSampCount);
            mB3dSound = 1;
            mnActiveSamples = mSampCount;
            return false;
        }
        return true;
    }
    return false;
}

MATCH_FUNC(0x41A390)
char_type sound_obj::Get3dSound_41A390() const
{
    if (mbSoundInitialized)
    {
        return mB3dSound;
    }
    else
    {
        return 0;
    }
}

MATCH_FUNC(0x41A3A0)
void sound_obj::ServiceSoundEffects_41A3A0()
{
    if (mServiceEnabled)
    {
        mnActiveSampleQueue = mnActiveSampleQueue != 1;
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

        // zpos is converted from an integer (shl $0xE) and mDopplerDivisor is already fixed point
        // q and rate are assigned, not initialised: that keeps them as register locals
        // (q moved to ecx before the imul, snd_rate << 14 held in esi across the divide)
        Fix16 q;
        q = Fix16(zpos.mValue) / kFp100;
        s32 relative_speed = (((ypos - xpos) * q) / Fix16(mDopplerDivisor, 0)).mValue;
        s32 a = relative_speed > 0 ? relative_speed : -relative_speed;
        if (a >= mSpeedOfSound)
        {
            return snd_rate;
        }
        Fix16 rate;
        rate = Fix16(snd_rate);
        // The speed of sound is not shifted here (raw ratio speed / (relative_speed + speed))
        return Fix16::Round_To_Int_410BF0(rate * (Fix16(mSpeedOfSound, 0) / Fix16(relative_speed + mSpeedOfSound, 0)));
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

    value = manRandomTable[seed % 5] % static_cast<u32>(value);
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
    mAudioEntities[idx].mbUsed = false;
    mAudioEntities[idx].mAge = 0;
    mAudioEntities[idx].mpObj = NULL;
    mAudioEntities[idx].mpAlloc = NULL;
}

MATCH_FUNC(0x41A6F0)
void sound_obj::IncrementAudioEntitiesAge_41A6F0()
{
    for (u32 idx = 0; idx < m444CNAudioEntitiesCount; idx++)
    {
        if (mAudioEntities[mAudioEntityOrderList[idx]].mAge < 10u)
        {
            mAudioEntities[mAudioEntityOrderList[idx]].mAge++;
        }
    }
}

// https://decomp.me/scratch/i3zcW
MATCH_FUNC(0x41A730)
void sound_obj::InterrogateAudioEntities_41A730()
{
    Camera* pCamera;

    if (mType5Idx != 0 && (pCamera = mAudioEntities[mType5Idx].mpObj->mpAny.pCamera_0xBC) != NULL)
    {
        Ped* pPed = pCamera->GetFollowedPed_416B70();

        if (pPed != NULL)
        {
            Car* pCar = pPed->get_car_416B60();
            if (pCar != NULL)
            {
                pCar->GetXYZ_416B80(&mListenerX, &mListenerY, &mListenerZ);
                mRotation = pCar->GetCarRotation_416BB0();
            }
            else
            {
                mListenerX = pPed->get_cam_x();
                mListenerY = pPed->get_cam_y();
                mListenerZ = pPed->get_cam_z();
                mRotation = pPed->GetRotation();
            }
        }
        else
        {
            mListenerX = pCamera->mCurPos.mX;
            mListenerY = pCamera->mCurPos.mY;
            mListenerZ = pCamera->mCurPos.mZ;
        }
    }
    else
    {
        mListenerX = 0;
        mListenerY = 0;
        mListenerZ = 0;
    }

    for (u32 j = 0; j < m444CNAudioEntitiesCount; j++)
    {
        mDistSquared = kFpZero_674CD8;
        mDistCalculated = false;
        sound_obj::ProcessEntity_4123A0(mAudioEntityOrderList[j]);
    }
}

MATCH_FUNC(0x41A850)
void sound_obj::AddSampleToRequestedQueue_41A850()
{
    u32 newVol = msQueueSample.mReleasingVolumeModificator * (127 - msQueueSample.mnVolume);

    u8 new_requested_count;
    if (mSampleRequestQueuesStatus[mnActiveSampleQueue] < mnActiveSamples)
    {
        new_requested_count = mSampleRequestQueuesStatus[mnActiveSampleQueue];
        mSampleRequestQueuesStatus[mnActiveSampleQueue]++;
    }
    else
    {
        new_requested_count = mabSampleQueueIndexTable[mnActiveSampleQueue][mnActiveSamples - 1];

        if (masSamples[mnActiveSampleQueue][new_requested_count].mnCalculatedVolume <= newVol)
        {
            return;
        }
    }

    msQueueSample.mnCalculatedVolume = newVol;
    msQueueSample.mbIsPlayingFinished = false;

    memcpy(&masSamples[mnActiveSampleQueue][new_requested_count], &msQueueSample, sizeof(sound_0x68));

    AddDetailsToRequestedOrderList_41A910(new_requested_count);
}

MATCH_FUNC(0x41A910)
void sound_obj::AddDetailsToRequestedOrderList_41A910(u8 sample)
{
    u8 i = 0;
    while (i < sample)
    {
        if (masSamples[mnActiveSampleQueue][mabSampleQueueIndexTable[mnActiveSampleQueue][i]]
                .mnCalculatedVolume > masSamples[mnActiveSampleQueue][sample].mnCalculatedVolume)
        {
            memmove(&mabSampleQueueIndexTable[mnActiveSampleQueue][i + 1],
                    &mabSampleQueueIndexTable[mnActiveSampleQueue][i],
                    mnActiveSamples - i - 1);
            break;
        }
        i++;
    }

    mabSampleQueueIndexTable[mnActiveSampleQueue][i] = sample;
}

MATCH_FUNC(0x41A9D0)
void sound_obj::AddReleasingSounds_41A9D0()
{
    bool toProcess[16];

    const u8 queue = mnActiveSampleQueue == 0 ? 1 : 0;

    for (u8 idx = 0; idx < mSampleRequestQueuesStatus[queue]; idx++)
    {
        const u32 sample_idx = mabSampleQueueIndexTable[queue][idx];
        sound_0x68& pSound = masSamples[queue][sample_idx];
        if (!pSound.mbIsPlayingFinished)
        {
            toProcess[idx] = false;
            for (u8 i = 0; i < mSampleRequestQueuesStatus[mnActiveSampleQueue]; i++)
            {
                sound_0x68& t =
                    masSamples[mnActiveSampleQueue][mabSampleQueueIndexTable[mnActiveSampleQueue][i]];

                if (pSound.mEntityIndex == t.mEntityIndex && pSound.mSampleIndex == t.mSampleIndex)
                {
                    toProcess[idx] = true;
                    break;
                }
            }

            if (!toProcess[idx])
            {
                if (pSound.mReleasingVolumeDivider > 0)
                {
                    if (pSound.mLoopCount == 0)
                    {
                        u8* pEmittingVolume = &pSound.mnEmittingVolume;
                        if (!mB3dSound)
                        {
                            pEmittingVolume = &pSound.mnVolume;
                        }

                        if (pSound.mVolumeChange == 0xFF)
                        {
                            pSound.mVolumeChange = *pEmittingVolume / pSound.mReleasingVolumeDivider;
                            if (static_cast<s8>(pSound.mVolumeChange) <= 0)
                            {
                                pSound.mVolumeChange = 1;
                            }
                        }

                        if (*pEmittingVolume > pSound.mVolumeChange)
                        {
                            *pEmittingVolume -= pSound.mVolumeChange;
                        }
                        else
                        {
                            pSound.mReleasingVolumeDivider = 0;
                            continue;
                        }
                    }

                    --pSound.mReleasingVolumeDivider;
                    if (mVocals.mFifthFrameFlag)
                    {
                        if (pSound.mReleasingVolumeModificator < 20)
                        {
                            ++pSound.mReleasingVolumeModificator;
                        }
                    }
                    pSound.mReleasingSoundFlag = 0;
                    if (!pSound.mpReleaseSource)
                    {
                        pSound.mpReleaseSource = &pSound;
                    }
                    memcpy(&msQueueSample, &pSound, sizeof(msQueueSample));
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

    for (u8 idx = 0; idx < mnActiveSamples; idx++)
    {
        masSamples[mnActiveSampleQueue][idx].mbIsBeingPlayed = false;
        mActiveSamples[idx].mbIsBeingPlayed = false;
    }

    for (i = 0; i < mSampleRequestQueuesStatus[mnActiveSampleQueue]; i++)
    {
        sound_0x68& sample =
            masSamples[mnActiveSampleQueue][mabSampleQueueIndexTable[mnActiveSampleQueue][i]];
        for (j = 0; j < mnActiveSamples; j++)
        {
            sound_0x68& active = mActiveSamples[j];

            if (sample.mEntityIndex != active.mEntityIndex || sample.mSampleIndex != active.mSampleIndex ||
                sample.mReleasingSoundFlag == 2 || (active.mpReleaseSource && !sample.mpReleaseSource))
            {
                continue;
            }

            if (!InitSampleForType_412260(&sample) || sample.mSampIdx >= 321 || sample.mSampIdx != active.mSampIdx)
            {
                continue;
            }

            UpdateFramesToPlay_41B490(&sample);

            if (sample.mLoopCount > 0)
            {
                if (active.mFramesToPlay == 0)
                {
                    bool bUsed;
                    if (mB3dSound)
                    {
                        bUsed = gSampManager_6FFF00.GetChannel3DUsedFlag_58DF70(j);
                    }
                    else
                    {
                        bUsed = gSampManager_6FFF00.GetChannelUsedFlag_58DD80(j);
                    }

                    if (!bUsed)
                    {
                        sample.mbIsPlayingFinished = true;
                        active.mbIsPlayingFinished = true;
                        active.mSampIdx = 321;
                        active.mEntityIndex = 0;
                        continue;
                    }
                    active.mFramesToPlay = mFramesPerSecond *
                        gSampManager_6FFF00.GetSampleLength_58DC70(active.mSampIdx) / sample.mRate;
                }
                else
                {
                    active.mFramesToPlay--;
                }
            }

            sample.mbIsBeingPlayed = true;
            active.mbIsBeingPlayed = true;
            sample.mVolumeChange = -1;

            if (sample.mReleasingSoundFlag)
            {
                sample.mbIsBeingPlayed = false;
                active.mbIsBeingPlayed = false;
                continue;
            }

            if (sample.mbIs2D)
            {
                if (mB3dSound)
                {
                    s32 emittingVol = mbDoubleVolume ? 2 * (sample.mnEmittingVolume < 63 ? sample.mnEmittingVolume : 63) : sample.mnEmittingVolume;
                    gSampManager_6FFF00.SetChannel3DFrequency_58DF00(j, sample.mRate);
                    gSampManager_6FFF00.SetChannel3DVolume_58DE80(j, static_cast<u8>((emittingVol * mSfxVol) >> 7));
                }
                else
                {
                    s32 emittingVol = mbDoubleVolume ? 2 * (sample.mnVolume < 63 ? sample.mnVolume : 63) : sample.mnVolume;
                    gSampManager_6FFF00.SetChannelFrequency_58DD20(j, sample.mRate);
                    gSampManager_6FFF00.SetChannelVolume_58DCE0(j, (emittingVol * mSfxVol) >> 7);
                    gSampManager_6FFF00.SetChannelPan_58DD00(j, static_cast<u8>(sample.mPan));
                }
                break;
            }

            // Doppler: the old distance is passed, the new one stored, both truncated to whole units
            s32 old_dist = active.mDistance.ToInt();
            s32 new_dist = sample.mDistance.ToInt();
            active.mDistance = Fix16(new_dist);
            // The speed multiplier is passed as its raw bits (a copy of the s32, no Fix16 constructor)
            sample.mRate =
                AdjustPlaybackRate_41A580(sample.mRate, old_dist, new_dist, reinterpret_cast<Fix16&>(sample.mSpeedMultiplier));
            if (sample.mRate != active.mRate)
            {
                u32 freq = Clamp2(sample.mRate, active.mRate, 6000);
                active.mRate = freq;
                if (mB3dSound)
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
            if (mB3dSound)
            {
                oldVolume = active.mnEmittingVolume;
                newVolume = sample.mnEmittingVolume;
            }
            else
            {
                oldVolume = active.mnVolume;
                newVolume = sample.mnVolume;
            }

            if (newVolume != oldVolume)
            {
                s32 volume = Clamp2(newVolume, oldVolume, 10);
                s32 emittingVol = mbDoubleVolume ? 2 * Min(volume, 63) : volume;
                if (mB3dSound)
                {
                    gSampManager_6FFF00.SetChannel3DVolume_58DE80(j, static_cast<u8>((emittingVol * mSfxVol) >> 7));
                    active.mnEmittingVolume = volume;
                }
                else
                {
                    gSampManager_6FFF00.SetChannelVolume_58DCE0(j, static_cast<u8>((emittingVol * mSfxVol) >> 7));
                    active.mnVolume = volume;
                }
            }

            VecDiff_41B4E0(&sample.mObj, &position);

            if (mB3dSound)
            {
                FromFix16_41B520(position.mX, &x);
                FromFix16_41B520(position.mY, &y);
                FromFix16_41B520(position.mZ, &z);
                // The original passes 0 for y and -y for z (z is computed but unused)
                gSampManager_6FFF00.SetChannel3DPosition_58DEA0(j, x, 0.0f, -y);
                gSampManager_6FFF00.SetChannel3DDistances_58DED0(j,
                                                                 static_cast<f32>(static_cast<u32>(sample.mMaxDistance)),
                                                                 static_cast<f32>(static_cast<u32>(sample.mMaxDistance) >> 2));
                break;
            }

            s32 pan = static_cast<u8>(active.mPan);
            s32 newPan = ComputePan_41A4A0(sample.mSoundIntensity, position.mX);
            if (newPan != pan)
            {
                pan = Clamp2(newPan, pan, 10);
                gSampManager_6FFF00.SetChannelPan_58DD00(j, static_cast<u8>(pan));
                active.mPan = pan;
            }
            break;
        }
    }

    for (j = 0; j < mnActiveSamples; j++)
    {
        sound_0x68& active = mActiveSamples[j];
        if (active.mSampIdx != 321 && !active.mbIsBeingPlayed)
        {
            if (mB3dSound)
            {
                gSampManager_6FFF00.StopChannel3D_58DFC0(j);
            }
            else
            {
                gSampManager_6FFF00.StopChannel_58DDD0(j);
            }
            active.mSampIdx = 321;
            active.mEntityIndex = 0;
        }
    }

    for (i = 0; i < mSampleRequestQueuesStatus[mnActiveSampleQueue]; i++)
    {
        sound_0x68& sample =
            masSamples[mnActiveSampleQueue][mabSampleQueueIndexTable[mnActiveSampleQueue][i]];
        if (sample.mbIsBeingPlayed || sample.mbIsPlayingFinished ||
            !mAudioEntities[sample.mEntityIndex].mbUsed)
        {
            continue;
        }

        for (j = 0; j < mnActiveSamples; j++)
        {
            sound_0x68& active = mActiveSamples[j];
            if (active.mbIsBeingPlayed || !InitSampleForType_412260(&sample) || sample.mSampIdx >= 321)
            {
                continue;
            }

            UpdateFramesToPlay_41B490(&sample);

            if (sample.mLoopCount > 0)
            {
                u32 samplesPerFrame = sample.mRate / mFramesPerSecond;
                u32 samplesToPlay = sample.mLoopCount * gSampManager_6FFF00.GetSampleLength_58DC70(sample.mSampIdx);
                if (samplesPerFrame == 0)
                {
                    continue;
                }
                sample.mReleasingVolumeDivider = samplesToPlay / samplesPerFrame + 1;
            }

            memcpy(&active, &sample, sizeof(sound_0x68));

            if (!active.mbIs2D)
            {
                VecDiff_41B4E0(&active.mObj, &position);
                if (!mB3dSound)
                {
                    active.mPan = ComputePan_41A4A0(active.mSoundIntensity, position.mX);
                }
            }

            if (mB3dSound)
            {
                s32 emittingVol = mbDoubleVolume ? 2 * (active.mnEmittingVolume < 63 ? active.mnEmittingVolume : 63) : active.mnEmittingVolume;
                gSampManager_6FFF00.InitialiseChannel3D_58DDF0(j, active.mSampIdx, active.mRate);
                gSampManager_6FFF00.SetChannel3DFrequency_58DF00(j, active.mRate);
                gSampManager_6FFF00.SetChannel3DVolume_58DE80(j, static_cast<u8>((emittingVol * mSfxVol) >> 7));
                gSampManager_6FFF00.SetChannel3DLoopCount_58DF50(j, active.mLoopCount);
                gSampManager_6FFF00.SetChannel3DLoopPoints_58DF20(j, active.mLoopStart, active.mLoopEnd);

                if (active.mbIs2D)
                {
                    x = 0.0f;
                    y = 0.0f;
                    z = 0.0f;
                    active.mSoundIntensity = 1638400000;
                }
                else
                {
                    FromFix16_41B520(position.mX, &x);
                    FromFix16_41B520(position.mY, &y);
                    FromFix16_41B520(position.mZ, &z);
                }

                gSampManager_6FFF00.SetChannel3DPosition_58DEA0(j, x, 0.0f, -y);
                gSampManager_6FFF00.SetChannel3DDistances_58DED0(j,
                                                                 static_cast<f32>(static_cast<u32>(active.mMaxDistance)),
                                                                 static_cast<f32>(static_cast<u32>(active.mMaxDistance) >> 2));
                gSampManager_6FFF00.StartChannel3D_58DFA0(j);
            }
            else
            {
                s32 emittingVol = mbDoubleVolume ? 2 * (active.mnVolume < 63 ? active.mnVolume : 63) : active.mnVolume;
                gSampManager_6FFF00.InitialiseChannel_58DC90(j, active.mSampIdx);
                gSampManager_6FFF00.SetChannelFrequency_58DD20(j, active.mRate);
                gSampManager_6FFF00.SetChannelVolume_58DCE0(j, static_cast<u8>((emittingVol * mSfxVol) >> 7));
                gSampManager_6FFF00.SetChannelPan_58DD00(j, static_cast<u8>(active.mPan));
                gSampManager_6FFF00.SetChannelLoopCount_58DD60(j, active.mLoopCount);
                gSampManager_6FFF00.SetChannelLoopPoints_58DD40(j, active.mLoopStart, active.mLoopEnd);
                gSampManager_6FFF00.StartChannel_58DDB0(j);
            }

            active.mbIsBeingPlayed = true;
            sample.mbIsBeingPlayed = true;
            sample.mVolumeChange = -1;
            break;
        }
    }
}

MATCH_FUNC(0x41B490)
void sound_obj::UpdateFramesToPlay_41B490(sound_0x68* pObj)
{
    if (pObj->mReleasingSoundFlag)
    {
        if (pObj->mLoopCount)
        {
            pObj->mFramesToPlay = (static_cast<u32>(mFramesPerSecond) * gSampManager_6FFF00.GetSampleLength_58DC70(pObj->mSampIdx)) /
                pObj->mRate;
        }
        else
        {
            pObj->mFramesToPlay = -3;
        }
    }
}

MATCH_FUNC(0x41B4E0)
void sound_obj::VecDiff_41B4E0(const SoundPos* pVec, SoundPos* pRet) const
{
    pRet->mX = pVec->mX - mListenerX;
    pRet->mY = pVec->mY - mListenerY;
    pRet->mZ = pVec->mZ - mListenerZ;
}

MATCH_FUNC(0x41B520)
void sound_obj::FromFix16_41B520(Fix16 value, f32* pOut) const
{
    *pOut = value.ToFloat();
}

MATCH_FUNC(0x41B540)
void sound_obj::AdjustSamplesVolume_41B540()
{
    if (mB3dSound)
    {
        for (u32 idx = 0; idx < mSampleRequestQueuesStatus[mnActiveSampleQueue]; idx++)
        {
            sound_0x68* pIter =
                &masSamples[mnActiveSampleQueue][mabSampleQueueIndexTable[mnActiveSampleQueue][idx]];
            if (!pIter->mbIs2D)
            {
                f32 distance;
                FromFix16_41B520(pIter->mDistance, &distance);
                pIter->mnEmittingVolume =
                    ComputeEmittingVolume_41B660(pIter->mnEmittingVolume, pIter->mMaxDistance, static_cast<u32>(distance));
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
    for (u32 i = 0; i < mnActiveSamples; i++)
    {
        mabSampleQueueIndexTable[mnActiveSampleQueue][i] = mnActiveSamples;
    }

    mSampleRequestQueuesStatus[mnActiveSampleQueue] = 0;
}

MATCH_FUNC(0x41B7A0)
void sound_obj::ClearActivateSamples_41B7A0()
{
    for (u8 i = 0; i < mnActiveSamples; i++)
    {
        mActiveSamples[i].mEntityIndex = 0;
        mActiveSamples[i].mSampleIndex = 0;
        mActiveSamples[i].mSampIdx = 321;
        mActiveSamples[i].mObj.mX = kFpZero_674CD8;
        mActiveSamples[i].mObj.mY = kFpZero_674CD8;
        mActiveSamples[i].mObj.mZ = kFpZero_674CD8;
        mActiveSamples[i].mbIs2D = false;
        mActiveSamples[i].mReleasingVolumeModificator = 4;
        mActiveSamples[i].mRate = 22050;
        mActiveSamples[i].mType = 0;
        mActiveSamples[i].mnVolume = 0;
        mActiveSamples[i].mnEmittingVolume = 0;
        mActiveSamples[i].mMaxDistance = 10;
        mActiveSamples[i].mDistance = 0;
        mActiveSamples[i].mbIsBeingPlayed = false;
        mActiveSamples[i].mbIsPlayingFinished = false;
        mActiveSamples[i].mLoopCount = 1;
        mActiveSamples[i].mLoopStart = 0;
        mActiveSamples[i].mLoopEnd = -1;
        mActiveSamples[i].mSpeedMultiplier = 0;
        mActiveSamples[i].mSoundIntensity = 0x190000;
        mActiveSamples[i].mPan = 63;
        mActiveSamples[i].mReleasingSoundFlag = 0;
        mActiveSamples[i].mFramesToPlay = -3;
        mActiveSamples[i].mnCalculatedVolume = 0;
        mActiveSamples[i].mReleasingVolumeDivider = 0;
        mActiveSamples[i].mVolumeChange = -1;
        mActiveSamples[i].mpReleaseSource = NULL;
    }
}

MATCH_FUNC(0x41BA90)
void sound_obj::GenerateIntegerRandomNumberTable_41BA90()
{
    for (s32 i = 0; i < 5; i++)
    {
        manRandomTable[i] = rand();
    }
}

MATCH_FUNC(0x41E820)
void sound_obj::ProcessObject_41E820(Sound_Params* pParams)
{
    if (pParams->mpObj->mObject2CPtr->mDefinition->mBehaviorType == object_behavior_type::effect_only_12)
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
    u32 kind = pParams->mpObj->mObject2CPtr->mPedRefIdx;
    Fix16 dist;
    u8 vol;
    switch (kind)
    {
        case 3:
            msQueueSample.mSampIdx = 141;
            msQueueSample.mLoopCount = 0;
            msQueueSample.mReleasingVolumeDivider = 3;
            msQueueSample.mSampleIndex = 0;
            msQueueSample.mReleasingSoundFlag = 0;
            msQueueSample.mReleasingVolumeModificator = 1;
            msQueueSample.mSoundIntensity = Fix16(0x32000, 0);
            msQueueSample.mMaxDistance = 25;
            dist = Fix16(0x271000, 0);
            vol = 90;
            break;
        case 4:
            msQueueSample.mSampIdx = 142;
            msQueueSample.mLoopCount = 0;
            msQueueSample.mReleasingVolumeDivider = 3;
            msQueueSample.mSampleIndex = 0;
            msQueueSample.mReleasingSoundFlag = 0;
            msQueueSample.mReleasingVolumeModificator = 1;
            msQueueSample.mSoundIntensity = Fix16(0x32000, 0);
            msQueueSample.mMaxDistance = 25;
            dist = Fix16(0x271000, 0);
            vol = 70;
            break;
        case 5:
            msQueueSample.mSampIdx = 192;
            msQueueSample.mLoopCount = 0;
            msQueueSample.mReleasingVolumeDivider = 3;
            msQueueSample.mSampleIndex = 0;
            msQueueSample.mReleasingSoundFlag = 0;
            msQueueSample.mReleasingVolumeModificator = 4;
            msQueueSample.mSoundIntensity = Fix16(9);
            msQueueSample.mMaxDistance = 18;
            dist = Fix16(81);
            vol = 50;
            break;
        case 8:
            msQueueSample.mSampIdx = 143;
            msQueueSample.mLoopCount = 0;
            msQueueSample.mReleasingVolumeDivider = 3;
            msQueueSample.mSampleIndex = 0;
            msQueueSample.mReleasingSoundFlag = 0;
            msQueueSample.mReleasingVolumeModificator = 3;
            msQueueSample.mSoundIntensity = Fix16(10);
            msQueueSample.mMaxDistance = 20;
            dist = Fix16(100);
            vol = 60;
            break;
        case 9:
            msQueueSample.mSampIdx = 144;
            msQueueSample.mLoopCount = 0;
            msQueueSample.mReleasingVolumeDivider = 3;
            msQueueSample.mSampleIndex = 0;
            msQueueSample.mReleasingSoundFlag = 0;
            msQueueSample.mReleasingVolumeModificator = 1;
            msQueueSample.mSoundIntensity = Fix16(0x32000, 0);
            msQueueSample.mMaxDistance = 25;
            dist = Fix16(0x271000, 0);
            vol = 60;
            break;
        case 13:
            msQueueSample.mSampIdx = 146;
            msQueueSample.mLoopCount = 0;
            msQueueSample.mReleasingVolumeDivider = 3;
            msQueueSample.mSampleIndex = 0;
            msQueueSample.mReleasingSoundFlag = 0;
            msQueueSample.mReleasingVolumeModificator = 3;
            msQueueSample.mSoundIntensity = Fix16(0x2e000, 0);
            msQueueSample.mMaxDistance = 23;
            dist = Fix16(0x211000, 0);
            vol = 50;
            break;
        case 19:
            msQueueSample.mSampIdx = 147;
            msQueueSample.mLoopCount = 0;
            msQueueSample.mReleasingVolumeDivider = 3;
            msQueueSample.mSampleIndex = 0;
            msQueueSample.mReleasingSoundFlag = 0;
            msQueueSample.mReleasingVolumeModificator = 3;
            msQueueSample.mSoundIntensity = Fix16(0x5a000, 0);
            msQueueSample.mMaxDistance = 45;
            dist = Fix16(0x7e9000, 0);
            vol = 70;
            break;
        case 16:
            msQueueSample.mSampIdx = 185;
            msQueueSample.mLoopCount = 0;
            msQueueSample.mReleasingVolumeDivider = 3;
            msQueueSample.mSampleIndex = 0;
            msQueueSample.mReleasingSoundFlag = 0;
            msQueueSample.mReleasingVolumeModificator = 6;
            msQueueSample.mSoundIntensity = Fix16(0x1e000, 0);
            msQueueSample.mMaxDistance = 15;
            dist = Fix16(0xe1000, 0);
            vol = 50;
            break;
        case 10:
        case 14:
            msQueueSample.mSampIdx = 149;
            msQueueSample.mLoopCount = 0;
            msQueueSample.mReleasingVolumeDivider = 3;
            msQueueSample.mSampleIndex = 0;
            msQueueSample.mReleasingSoundFlag = 0;
            msQueueSample.mReleasingVolumeModificator = 5;
            msQueueSample.mSoundIntensity = Fix16(0x32000, 0);
            msQueueSample.mMaxDistance = 25;
            dist = Fix16(0x271000, 0);
            vol = 50;
            break;
        case 33:
            msQueueSample.mSampIdx = 57;
            msQueueSample.mLoopCount = 0;
            msQueueSample.mReleasingVolumeDivider = 3;
            msQueueSample.mSampleIndex = 0;
            msQueueSample.mReleasingSoundFlag = 0;
            msQueueSample.mReleasingVolumeModificator = 7;
            msQueueSample.mSoundIntensity = Fix16(5);
            msQueueSample.mMaxDistance = 10;
            dist = Fix16(25);
            vol = 40;
            break;
        case 23:
            msQueueSample.mSampIdx = 161;
            msQueueSample.mLoopCount = 0;
            msQueueSample.mReleasingVolumeDivider = 3;
            msQueueSample.mSampleIndex = 0;
            msQueueSample.mReleasingSoundFlag = 0;
            msQueueSample.mReleasingVolumeModificator = 0;
            msQueueSample.mSoundIntensity = Fix16(0x46000, 0);
            msQueueSample.mMaxDistance = 35;
            dist = Fix16(0x4c9000, 0);
            vol = 90;
            break;
        case 43:
            msQueueSample.mSampIdx = 162;
            msQueueSample.mLoopCount = 0;
            msQueueSample.mReleasingVolumeDivider = 3;
            msQueueSample.mSampleIndex = 0;
            msQueueSample.mReleasingSoundFlag = 0;
            msQueueSample.mReleasingVolumeModificator = 4;
            msQueueSample.mSoundIntensity = Fix16(0x32000, 0);
            msQueueSample.mMaxDistance = 25;
            dist = Fix16(0x271000, 0);
            vol = 70;
            break;
        case 26:
            msQueueSample.mSampIdx = 163;
            msQueueSample.mLoopCount = 0;
            msQueueSample.mReleasingVolumeDivider = 3;
            msQueueSample.mSampleIndex = 0;
            msQueueSample.mReleasingSoundFlag = 0;
            msQueueSample.mReleasingVolumeModificator = 14;
            msQueueSample.mSoundIntensity = Fix16(5);
            msQueueSample.mMaxDistance = 10;
            dist = Fix16(25);
            vol = 40;
            break;
        case 36:
            msQueueSample.mSampIdx = 169;
            msQueueSample.mLoopCount = 0;
            msQueueSample.mReleasingVolumeDivider = 3;
            msQueueSample.mSampleIndex = 0;
            msQueueSample.mReleasingSoundFlag = 0;
            msQueueSample.mReleasingVolumeModificator = 1;
            msQueueSample.mSoundIntensity = Fix16(5);
            msQueueSample.mMaxDistance = 10;
            dist = Fix16(25);
            vol = 60;
            break;
        case 30:
            msQueueSample.mSampIdx = 166;
            msQueueSample.mLoopCount = 0;
            msQueueSample.mReleasingVolumeDivider = 3;
            msQueueSample.mSampleIndex = 0;
            msQueueSample.mReleasingSoundFlag = 0;
            msQueueSample.mReleasingVolumeModificator = 12;
            msQueueSample.mSoundIntensity = Fix16(0xa000, 0);
            msQueueSample.mMaxDistance = 5;
            dist = Fix16(0x19000, 0);
            vol = 50;
            break;
        case 15:
            msQueueSample.mSampIdx = 174;
            msQueueSample.mLoopCount = 0;
            msQueueSample.mReleasingVolumeDivider = 3;
            msQueueSample.mSampleIndex = 0;
            msQueueSample.mReleasingSoundFlag = 0;
            msQueueSample.mReleasingVolumeModificator = 12;
            msQueueSample.mSoundIntensity = Fix16(0xa000, 0);
            msQueueSample.mMaxDistance = 5;
            dist = Fix16(0x19000, 0);
            vol = 40;
            break;
        case 41:
            msQueueSample.mSampIdx = 175;
            msQueueSample.mLoopCount = 0;
            msQueueSample.mReleasingVolumeDivider = 3;
            msQueueSample.mSampleIndex = 0;
            msQueueSample.mReleasingSoundFlag = 0;
            msQueueSample.mReleasingVolumeModificator = 1;
            msQueueSample.mSoundIntensity = Fix16(10);
            msQueueSample.mMaxDistance = 20;
            dist = Fix16(100);
            vol = 70;
            break;
        case 42:
            msQueueSample.mSampIdx = 176;
            msQueueSample.mLoopCount = 0;
            msQueueSample.mReleasingVolumeDivider = 3;
            msQueueSample.mSampleIndex = 0;
            msQueueSample.mReleasingSoundFlag = 0;
            msQueueSample.mReleasingVolumeModificator = 5;
            msQueueSample.mSoundIntensity = Fix16(5);
            msQueueSample.mMaxDistance = 10;
            dist = Fix16(25);
            vol = 40;
            break;
        case 44:
            msQueueSample.mSampIdx = 177;
            msQueueSample.mLoopCount = 0;
            msQueueSample.mReleasingVolumeDivider = 3;
            msQueueSample.mSampleIndex = 0;
            msQueueSample.mReleasingSoundFlag = 0;
            msQueueSample.mReleasingVolumeModificator = 1;
            msQueueSample.mSoundIntensity = Fix16(0x32000, 0);
            msQueueSample.mMaxDistance = 25;
            dist = Fix16(0x271000, 0);
            vol = 70;
            break;
        case 47:
            msQueueSample.mSampIdx = 178;
            msQueueSample.mLoopCount = 0;
            msQueueSample.mReleasingVolumeDivider = 3;
            msQueueSample.mSampleIndex = 0;
            msQueueSample.mReleasingSoundFlag = 0;
            msQueueSample.mReleasingVolumeModificator = 1;
            msQueueSample.mSoundIntensity = Fix16(0x32000, 0);
            msQueueSample.mMaxDistance = 25;
            dist = Fix16(0x271000, 0);
            vol = 70;
            break;
        case 48:
            msQueueSample.mSampIdx = 179;
            msQueueSample.mLoopCount = 0;
            msQueueSample.mReleasingVolumeDivider = 3;
            msQueueSample.mSampleIndex = 0;
            msQueueSample.mReleasingSoundFlag = 0;
            msQueueSample.mReleasingVolumeModificator = 1;
            msQueueSample.mSoundIntensity = Fix16(0x32000, 0);
            msQueueSample.mMaxDistance = 25;
            dist = Fix16(0x271000, 0);
            vol = 90;
            break;
        case 49:
            msQueueSample.mSampIdx = 180;
            msQueueSample.mLoopCount = 0;
            msQueueSample.mReleasingVolumeDivider = 3;
            msQueueSample.mSampleIndex = 0;
            msQueueSample.mReleasingSoundFlag = 0;
            msQueueSample.mReleasingVolumeModificator = 1;
            msQueueSample.mSoundIntensity = Fix16(0x32000, 0);
            msQueueSample.mMaxDistance = 25;
            dist = Fix16(0x271000, 0);
            vol = 60;
            break;
        case 45:
            msQueueSample.mSampIdx = 192;
            msQueueSample.mLoopCount = 0;
            msQueueSample.mReleasingVolumeDivider = 3;
            msQueueSample.mSampleIndex = 0;
            msQueueSample.mReleasingSoundFlag = 0;
            msQueueSample.mReleasingVolumeModificator = 3;
            msQueueSample.mSoundIntensity = Fix16(0xa000, 0);
            msQueueSample.mMaxDistance = 5;
            dist = Fix16(0x19000, 0);
            vol = 40;
            break;
        case 50:
        case 51:
            msQueueSample.mSampIdx = 173;
            msQueueSample.mLoopCount = 1;
            msQueueSample.mReleasingSoundFlag = 1;
            msQueueSample.mReleasingVolumeModificator = 3;
            msQueueSample.mSoundIntensity = Fix16(10);
            msQueueSample.mMaxDistance = 20;
            dist = Fix16(100);
            vol = 75;
            break;
        case 40:
            msQueueSample.mSampIdx = 172;
            msQueueSample.mLoopCount = 1;
            msQueueSample.mReleasingSoundFlag = 1;
            msQueueSample.mReleasingVolumeModificator = 1;
            msQueueSample.mSoundIntensity = Fix16(10);
            msQueueSample.mMaxDistance = 20;
            dist = Fix16(100);
            vol = 60;
            break;
        case 37:
            msQueueSample.mSampIdx = 170;
            msQueueSample.mLoopCount = 1;
            msQueueSample.mReleasingSoundFlag = 1;
            msQueueSample.mReleasingVolumeModificator = 1;
            msQueueSample.mSoundIntensity = Fix16(10);
            msQueueSample.mMaxDistance = 20;
            dist = Fix16(100);
            vol = 60;
            break;
        case 38:
            msQueueSample.mSampIdx = 171;
            msQueueSample.mLoopCount = 1;
            msQueueSample.mReleasingSoundFlag = 1;
            msQueueSample.mReleasingVolumeModificator = 5;
            msQueueSample.mSoundIntensity = Fix16(10);
            msQueueSample.mMaxDistance = 20;
            dist = Fix16(100);
            vol = 60;
            break;
        case 39:
            msQueueSample.mSampIdx = (manRandomTable[0] & 3) + 181;
            msQueueSample.mLoopCount = 1;
            msQueueSample.mReleasingSoundFlag = 1;
            msQueueSample.mReleasingVolumeModificator = 2;
            msQueueSample.mSoundIntensity = Fix16(10);
            msQueueSample.mMaxDistance = 20;
            dist = Fix16(100);
            vol = 60;
            break;
        case 32:
            msQueueSample.mSampIdx = manRandomTable[static_cast<u32>(msQueueSample.mEntityIndex) % 5] % 6 + 233;
            msQueueSample.mLoopCount = 1;
            msQueueSample.mReleasingSoundFlag = 1;
            msQueueSample.mReleasingVolumeModificator = 3;
            msQueueSample.mSoundIntensity = Fix16(10);
            msQueueSample.mMaxDistance = 20;
            dist = Fix16(100);
            vol = 80;
            break;
        case 27:
            msQueueSample.mSampIdx = 164;
            msQueueSample.mLoopCount = 1;
            msQueueSample.mReleasingSoundFlag = 1;
            msQueueSample.mReleasingVolumeModificator = 2;
            msQueueSample.mSoundIntensity = Fix16(10);
            msQueueSample.mMaxDistance = 20;
            dist = Fix16(100);
            vol = 100;
            break;
        case 34:
            msQueueSample.mSampIdx = 167;
            msQueueSample.mLoopCount = 1;
            msQueueSample.mReleasingSoundFlag = 1;
            msQueueSample.mReleasingVolumeModificator = 1;
            msQueueSample.mSoundIntensity = Fix16(10);
            msQueueSample.mMaxDistance = 20;
            dist = Fix16(100);
            vol = 50;
            break;
        case 35:
            msQueueSample.mSampIdx = 168;
            msQueueSample.mLoopCount = 1;
            msQueueSample.mReleasingSoundFlag = 1;
            msQueueSample.mReleasingVolumeModificator = 1;
            msQueueSample.mSoundIntensity = Fix16(10);
            msQueueSample.mMaxDistance = 20;
            dist = Fix16(100);
            vol = 50;
            break;
        case 17:
            msQueueSample.mSampIdx = 151;
            msQueueSample.mLoopCount = 1;
            msQueueSample.mReleasingSoundFlag = 1;
            msQueueSample.mReleasingVolumeModificator = 7;
            msQueueSample.mSoundIntensity = Fix16(10);
            msQueueSample.mMaxDistance = 20;
            dist = Fix16(100);
            vol = 50;
            break;
        case 7:
            msQueueSample.mSampIdx = 148;
            msQueueSample.mLoopCount = 1;
            msQueueSample.mReleasingSoundFlag = 1;
            msQueueSample.mReleasingVolumeModificator = 20;
            msQueueSample.mSoundIntensity = Fix16(5);
            msQueueSample.mMaxDistance = 10;
            dist = Fix16(25);
            vol = 30;
            break;
        case 28:
            msQueueSample.mSampIdx = 165;
            msQueueSample.mLoopCount = 1;
            msQueueSample.mReleasingSoundFlag = 1;
            msQueueSample.mReleasingVolumeModificator = 10;
            msQueueSample.mSoundIntensity = Fix16(0x32000, 0);
            msQueueSample.mMaxDistance = 25;
            dist = Fix16(0x271000, 0);
            vol = 105;
            break;
        case 11:
            msQueueSample.mLoopCount = 1;
            msQueueSample.mReleasingSoundFlag = 1;
            msQueueSample.mReleasingVolumeModificator = 20;
            msQueueSample.mSoundIntensity = Fix16(10);
            msQueueSample.mMaxDistance = 20;
            dist = Fix16(100);
            vol = 50;
            break;
        case 12:
            msQueueSample.mLoopCount = 1;
            msQueueSample.mReleasingSoundFlag = 1;
            msQueueSample.mReleasingVolumeModificator = 20;
            msQueueSample.mSoundIntensity = Fix16(0x1e000, 0);
            msQueueSample.mMaxDistance = 15;
            dist = Fix16(0xe1000, 0);
            vol = 40;
            break;
        case 20:
        case 24:
        case 52:
            msQueueSample.mLoopCount = 1;
            msQueueSample.mReleasingSoundFlag = 1;
            msQueueSample.mReleasingVolumeModificator = 5;
            msQueueSample.mSoundIntensity = Fix16(0x1e000, 0);
            msQueueSample.mMaxDistance = 15;
            dist = Fix16(0xe1000, 0);
            vol = 90;
            break;
        case 46:
            msQueueSample.mLoopCount = 1;
            msQueueSample.mReleasingSoundFlag = 1;
            msQueueSample.mReleasingVolumeModificator = 5;
            msQueueSample.mSoundIntensity = Fix16(0x32000, 0);
            msQueueSample.mMaxDistance = 25;
            dist = Fix16(0x271000, 0);
            vol = 60;
            break;
        case 21:
            msQueueSample.mLoopCount = 1;
            msQueueSample.mReleasingSoundFlag = 1;
            msQueueSample.mReleasingVolumeModificator = 4;
            msQueueSample.mSoundIntensity = Fix16(10);
            msQueueSample.mMaxDistance = 20;
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
    if (!VolCalc_419070(vol, msQueueSample.mSoundIntensity, false))
    {
        return;
    }

    switch (kind)
    {
        case 17:
            if (!gObjSoundDelayKind17_6751D0)
            {
                gObjSoundDelayKind17_6751D0 = static_cast<u8>(manRandomTable[0] % 20 + 20);
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
                gObjSoundDelayKind7_6751D2 = static_cast<u8>(manRandomTable[1] % 12 + 10);
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
                gObjSoundDelayKind28_6751D4 = static_cast<u8>(manRandomTable[2] % 90 - 46);
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
                msQueueSample.mSampIdx = manRandomTable[3] % 11 + 228;
                gObjSoundDelayKind11_6751D6 = static_cast<u8>(manRandomTable[2] % 10 + 40);
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
                msQueueSample.mSampIdx = (manRandomTable[2] & 3) + 181;
                gObjSoundDelayKind12_6751D8 = static_cast<u8>(manRandomTable[3] % 90 + 30);
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
                msQueueSample.mSampIdx = manRandomTable[1] % 5 + 152;
                gObjSoundDelayKind20_6751DA = static_cast<u8>(manRandomTable[4] % 10 + 4);
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
                msQueueSample.mSampIdx = manRandomTable[1] % 5 + 152;
                gObjSoundDelayKind24_6751DC = static_cast<u8>((manRandomTable[2] & 7) + 5);
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
                msQueueSample.mSampIdx = manRandomTable[1] % 3 + 154;
                gObjSoundDelayKind52_6751E2 = static_cast<u8>(manRandomTable[2] % 30 + 45);
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
                msQueueSample.mSampIdx = manRandomTable[4] % 6 + 303;
                gObjSoundDelayKind46_6751DE = static_cast<u8>(manRandomTable[2] % 90 + 30);
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
                msQueueSample.mSampIdx = (manRandomTable[2] & 3) + 157;
                gObjSoundDelayKind21_6751E0 = static_cast<u8>(manRandomTable[3] % 40 + 90);
            }
            else
            {
                gObjSoundDelayKind21_6751E0--;
                return;
            }
            break;
    }

    if (msQueueSample.mLoopCount > 0)
    {
        if (++gType12SampleIndexCounter_6751E4 == 255)
        {
            gType12SampleIndexCounter_6751E4 = 0;
        }
        msQueueSample.mSampleIndex = gType12SampleIndexCounter_6751E4;
    }

    msQueueSample.mnEmittingVolume = vol;
    // No local for the sample index: with one, VC6 copy-propagates it into the RandomDisplacement
    // argument, where the original reloads field_14 after the first call
    msQueueSample.mRate = gSampManager_6FFF00.GetPlayBackRate_58DBF0(msQueueSample.mSampIdx) +
        RandomDisplacement_41A650(msQueueSample.mSampIdx) + (msQueueSample.mSampIdx % 10) * 40;
    if (kind == 45)
    {
        msQueueSample.mRate += msQueueSample.mRate;
    }
    msQueueSample.mType = 20;
    msQueueSample.mbIs2D = false;
    msQueueSample.mLoopStart = gSampManager_6FFF00.GetLoopStart_58DC30(msQueueSample.mSampIdx);
    msQueueSample.mLoopEnd = gSampManager_6FFF00.GetLoopEnd_58DC50(msQueueSample.mSampIdx);
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

    Object* pObj = pParams->mpObj->mObject2CPtr;
    switch (pObj->mModel)
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
            if (mMFrameCounter % 130)
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
            Explosion* pExplosion = pObj->mpAny.pExplosion;
            if (!pExplosion)
            {
                return;
            }
            switch (pExplosion->mType)
            {
                case explosion_type::small_18:
                case explosion_type::small_33:
                    if (mAudioEntities[msQueueSample.mEntityIndex].mAge == 2)
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
                    else if (mAudioEntities[msQueueSample.mEntityIndex].mAge > 2)
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
                    if (mAudioEntities[msQueueSample.mEntityIndex].mAge == 2)
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
                    else if (mAudioEntities[msQueueSample.mEntityIndex].mAge > 2)
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
                    if (mAudioEntities[msQueueSample.mEntityIndex].mAge == 2)
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
                    else if (mAudioEntities[msQueueSample.mEntityIndex].mAge > 2)
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
            if (VolCalc_419070(volume, emit_distance, pParams->mbHasSolidAbove))
            {
                msQueueSample.mnEmittingVolume = volume;
                msQueueSample.mSampIdx = samp_idx;
                msQueueSample.mSoundIntensity = emit_distance;
                msQueueSample.mMaxDistance = max_distance;
                msQueueSample.mRate = gSampManager_6FFF00.GetPlayBackRate_58DBF0(samp_idx) + rate_displacement;
                msQueueSample.mType = 20;
                msQueueSample.mSampleIndex = sample_index;
                msQueueSample.mReleasingSoundFlag = bLoop;
                msQueueSample.mReleasingVolumeModificator = release_mod;
                msQueueSample.mbIs2D = false;
                msQueueSample.mSpeedMultiplier = 400;
                msQueueSample.mLoopStart = 0;
                msQueueSample.mLoopEnd = -1;
                if (bLoop == 1)
                {
                    msQueueSample.mLoopCount = 1;
                }
                else
                {
                    msQueueSample.mLoopCount = 0;
                    msQueueSample.mReleasingVolumeDivider = 20;
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

    this->mbCanPlay = false;
    if (this->manRandomTable[4] % 20)
    {
        if (!(this->manRandomTable[0] % 3u))
        {
            rnd_3 = this->manRandomTable[1] % 3 + 69;
            rnd_2 = this->manRandomTable[2] % 3 + 87;
        }
        else
        {
            rnd_3 = this->manRandomTable[1] % 5 + 115;
            rnd_2 = this->manRandomTable[2] % 5 + 131;
        }
        Init_RadioMessageQueue_427180();
        gSampManager_6FFF00.SetSampleVol_58E7D0((this->mSfxVol * 50u) >> 7);
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

    Char* pChar = pParams->mpObj->mCharB4Ptr;
    if (pChar->mpPed)
    {
        if (!gGame_0x40_67E008->mLocalPlayer->GetPlayerCar_5698E0() && pChar->get_velocity_41B080() > kFpZero)
        {
            const u32 animation_state = pChar->mAnimationState;
            if (animation_state == 4 || animation_state == 1 || animation_state == 0)
            {
                const char_type animation_frame = pChar->mAnimationFrame;
                if ((animation_frame == 1 || animation_frame == 5) && ((reinterpret_cast<s32>(pChar) + this->mMFrameCounter) & 1) != 0)
                {
                    switch (pChar->mpPed->mBlockSpec)
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
                                if (!pChar->mAnimationState)
                                {
                                    max_dist = 30;
                                    vol = 5;
                                }
                                else
                                {
                                    max_dist = 17;
                                    vol = 20;
                                }
                                if (VolCalc_419070(vol, Fix16(2), pParams->mbHasSolidAbove))
                                {
                                    const u32 samp_offset = this->manRandomTable[this->msQueueSample.mEntityIndex % 5u] %
                                        static_cast<u32>(last_samp - first_samp + 1);
                                    this->msQueueSample.mSampIdx = first_samp + samp_offset;
                                    this->msQueueSample.mSoundIntensity = Fix16(2);
                                    this->msQueueSample.mMaxDistance = 4;
                                    this->msQueueSample.mnEmittingVolume = vol;
                                    const s32 playback_rate = gSampManager_6FFF00.GetPlayBackRate_58DBF0(msQueueSample.mSampIdx);
                                    const s32 base_rate = playback_rate + RandomDisplacement_41A650(msQueueSample.mSampIdx);
                                    this->msQueueSample.mType = 20;
                                    this->msQueueSample.mRate = base_rate;
                                    if (!pChar->mAnimationFrame)
                                    {
                                        this->msQueueSample.mSampleIndex = 0;
                                    }
                                    else
                                    {
                                        this->msQueueSample.mSampleIndex = 5;
                                        this->msQueueSample.mRate = base_rate + 50;
                                    }
                                    this->msQueueSample.mReleasingSoundFlag = 1;
                                    this->msQueueSample.mReleasingVolumeModificator = max_dist;
                                    this->msQueueSample.mbIs2D = false;
                                    this->msQueueSample.mLoopStart = 0;
                                    this->msQueueSample.mLoopEnd = -1;
                                    this->msQueueSample.mLoopCount = 1;
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

        const s32 scream_timer = pChar->mScreamTimer;
        if (scream_timer > -1)
        {
            if (scream_timer > 0)
            {
                pChar->mScreamTimer = scream_timer - 1;
            }
            else
            {
                if (CalculateDistance_419020(Fix16(81)))
                {
                    vol = this->manRandomTable[this->msQueueSample.mEntityIndex % 5u] % 0x17u + 30;
                    if (VolCalc_419070(vol, Fix16(9), pParams->mbHasSolidAbove))
                    {
                        this->msQueueSample.mSampIdx = this->manRandomTable[this->msQueueSample.mEntityIndex % 5u] % 6u + 233;
                        this->msQueueSample.mSoundIntensity = Fix16(9);
                        this->msQueueSample.mnEmittingVolume = vol;
                        this->msQueueSample.mMaxDistance = 18;
                        this->msQueueSample.mRate = gSampManager_6FFF00.GetPlayBackRate_58DBF0(this->msQueueSample.mSampIdx) +
                            sound_obj::RandomDisplacement_41A650(this->msQueueSample.mSampIdx);
                        this->msQueueSample.mType = 20;
                        this->msQueueSample.mSampleIndex = 3;
                        this->msQueueSample.mReleasingSoundFlag = 1;
                        this->msQueueSample.mReleasingVolumeModificator = 10;
                        this->msQueueSample.mbIs2D = false;
                        this->msQueueSample.mLoopStart = 0;
                        this->msQueueSample.mLoopEnd = -1;
                        this->msQueueSample.mLoopCount = 1;
                        sound_obj::AddSampleToRequestedQueue_41A850();
                    }
                }
                pChar->mScreamTimer = this->manRandomTable[3] % 0x1Eu + 20;
            }
        }

        switch (pChar->get_ped_state_2_41B090())
        {
            case ped_state_2::electrocuted_27:
                samp_idx = 58;
                this->msQueueSample.mReleasingSoundFlag = 0;
                this->msQueueSample.mRate = gSampManager_6FFF00.GetPlayBackRate_58DBF0(58);
                vol = 40;
                release_mod = 15;
                sound_intensity = Fix16(286720, 0);
                calc_distance = Fix16(5017600, 0);
                max_dist = 35;
                break;

            case ped_state_2::knocked_fatal_26:
                if (!pChar->mpPed->is_player_41B0A0() && (this->msQueueSample.mEntityIndex & 1) != 0)
                {
                    return;
                }
                zpos = pParams->mpObj->mZpos;
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
                    this->msQueueSample.mReleasingSoundFlag = 0;
                    max_dist = 10;
                    rate = gSampManager_6FFF00.GetPlayBackRate_58DBF0(193);
                    this->msQueueSample.mRate = rate;
                    this->msQueueSample.mRate =
                        rate + Fix16::Round_To_Int_410BF0(pParams->mpObj->mZpos * kFp400);
                    break;
                }
                // fall through: zpos too low, play the ped voice instead

            default:
                HandlePedVoiceEvent_423080(pParams);
                return;
        }

        if (sound_obj::CalculateDistance_419020(calc_distance))
        {
            if (sound_obj::VolCalc_419070(vol, sound_intensity, pParams->mbHasSolidAbove))
            {
                this->msQueueSample.mSampIdx = samp_idx;
                this->msQueueSample.mSoundIntensity = sound_intensity;
                this->msQueueSample.mnEmittingVolume = vol;
                this->msQueueSample.mMaxDistance = max_dist;
                this->msQueueSample.mType = 20;
                this->msQueueSample.mSpeedMultiplier = 700;
                this->msQueueSample.mSampleIndex = 1;
                this->msQueueSample.mReleasingVolumeModificator = release_mod;
                this->msQueueSample.mbIs2D = false;
                this->msQueueSample.mLoopEnd = -1;
                if (this->msQueueSample.mReleasingSoundFlag == 1)
                {
                    this->msQueueSample.mLoopCount = 1;
                    this->msQueueSample.mLoopStart = 0;
                }
                else
                {
                    this->msQueueSample.mLoopStart = gSampManager_6FFF00.GetLoopStart_58DC30(samp_idx);
                    this->msQueueSample.mLoopCount = 0;
                    this->msQueueSample.mReleasingVolumeDivider = 5;
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
    Char* pChar = pParams->mpObj->mCharB4Ptr;
    Ped* pPed = pChar->mpPed;
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
                        samp = manRandomTable[static_cast<u32>(msQueueSample.mEntityIndex) % 5] % 11 + 210;
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
                    samp = manRandomTable[static_cast<u32>(msQueueSample.mEntityIndex) % 5] % 3 + 221;
                }
                break;
            case 3:
                if (!bTank && Cooldown_4236C0())
                {
                    samp = manRandomTable[static_cast<u32>(msQueueSample.mEntityIndex) % 5] % 7 + 221;
                }
                break;
            case 4:
                samp = manRandomTable[static_cast<u32>(msQueueSample.mEntityIndex) % 5] % 5 + 228;
                break;
            case 5:
                if (!bTank)
                {
                    samp = manRandomTable[static_cast<u32>(msQueueSample.mEntityIndex) % 5] % 6 + 239;
                }
                break;
            case 7:
                if (!bTank && !(mMFrameCounter & 1))
                {
                    samp = manRandomTable[static_cast<u32>(msQueueSample.mEntityIndex) % 5] % 5 + 245;
                }
                break;
            case 8:
                if (!bTank && (mMFrameCounter & 1) == 1)
                {
                    samp = static_cast<u32>(msQueueSample.mEntityIndex) % 7 + 250;
                }
                break;
            case 9:
            case 10:
                if (!bTank && Cooldown_4236C0())
                {
                    samp = manRandomTable[static_cast<u32>(msQueueSample.mEntityIndex) % 5] % 8 + 257;
                }
                break;
            case 11:
                if (!bTank && Cooldown_4236C0())
                {
                    samp = manRandomTable[static_cast<u32>(msQueueSample.mEntityIndex) % 5] % 7 + 265;
                }
                break;
            case 12:
                if (!bTank)
                {
                    samp = manRandomTable[static_cast<u32>(msQueueSample.mEntityIndex) % 5] % 5 + 272;
                }
                break;
            case 13:
                if (!bTank)
                {
                    samp = manRandomTable[static_cast<u32>(msQueueSample.mEntityIndex) % 5] % 3 + 277;
                }
                break;
            case 14:
                if (!bTank)
                {
                    samp = manRandomTable[static_cast<u32>(msQueueSample.mEntityIndex) % 5] % 3 + 280;
                }
                break;
            case 15:
                if (!bTank)
                {
                    samp = manRandomTable[static_cast<u32>(msQueueSample.mEntityIndex) % 5] % 2 + 283;
                }
                break;
            case 17:
                if (bTank)
                {
                    if (!sCopVoice17Cooldown)
                    {
                        samp = manRandomTable[static_cast<u32>(msQueueSample.mEntityIndex) % 5] % 14 + 285;
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
                        samp = manRandomTable[static_cast<u32>(msQueueSample.mEntityIndex) % 5] % 4 + 299;
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
                samp = manRandomTable[0] % 6 + 303;
                sElvisVoiceCooldown = 275;
            }
            sElvisVoiceCooldown--;
        }
    }
    else if (voice == 20)
    {
        samp = manRandomTable[0] % 2 + 309;
    }
    else if (voice == 25)
    {
        samp = 59;
    }

    if (voice == 26)
    {
        switch (pChar->mpPed->mBlockSpec)
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
                msQueueSample.mSampIdx = 68;
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

        if (VolCalc_419070(vol, Fix16(9), pParams->mbHasSolidAbove))
        {
            msQueueSample.mSampIdx = samp;
            msQueueSample.mSoundIntensity = Fix16(9);
            msQueueSample.mnEmittingVolume = vol;
            msQueueSample.mMaxDistance = 18;
            msQueueSample.mRate =
                gSampManager_6FFF00.GetPlayBackRate_58DBF0(msQueueSample.mSampIdx) +
                RandomDisplacement_41A650(msQueueSample.mSampIdx);
            msQueueSample.mType = 20;
            msQueueSample.mSpeedMultiplier = 0;
            msQueueSample.mSampleIndex = 2;
            msQueueSample.mReleasingSoundFlag = 1;
            msQueueSample.mReleasingVolumeModificator = 7;
            msQueueSample.mbIs2D = false;
            msQueueSample.mLoopStart = 0;
            msQueueSample.mLoopEnd = -1;
            msQueueSample.mLoopCount = 1;
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
    this->mbCanPlay = true;
    this->mRadioWord5524 = 0;
    this->mErvEnRouteTimer = 0;
    this->mTimer5570 = 0;
    this->mTimer556E = 0;
    this->mTimer556C = 0;
}

// Builds the police radio messages for the player's wanted level and the crimes reported nearby
MATCH_FUNC(0x426790)
void sound_obj::PoliceRadioMessageGeneration_426790()
{
    LOCAL_STATIC(u16, sCrimeCalloutCooldown, 0x6758A8);
    Ped* pPed = gGame_0x40_67E008->mLocalPlayer->GetPlayerPed_41D020();
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
        if (mErvEnRouteTimer)
        {
            if (--mErvEnRouteTimer == 0)
            {
                AppendRadioMessageSuffix_4273B0();
            }
        }
        if (mTimer556E)
        {
            mTimer556E--;
        }
        if (mTimer5570)
        {
            mTimer5570--;
        }
        if (mTimer556C)
        {
            mTimer556C--;
        }

        if (best_crime)
        {
            if (best_crime == crime_stats_type::lawmen_killed_8 && !mErvEnRouteTimer)
            {
                mCrimeYpos = ypos;
                mCrimeXpos = xpos;
                mErvEnRouteTimer = (manRandomTable[0] % 10 + 15) * 100;
            }

            if (stars > mWantedStarCount)
            {
                nullsub_4();
                mTimer5570 = manRandomTable[2] % 1067 + 150;
                mTimer556C = manRandomTable[2] % 1530 + 600;
                sCrimeCalloutCooldown = manRandomTable[3] % 50 + 889;
                switch (stars)
                {
                    case 1:
                        EnqueueRadioWord_4271B0(120);
                        EnqueueRadioWord_4271B0(93);
                        EnqueueRadioWord_4271B0(mRadioWord5521 + 74);
                        EnqueueRadioCrimeCallout_427340(best_crime, xpos, ypos);
                        break;
                    case 2:
                        if (GetFreeRadioWordSlots_427310() >= 9)
                        {
                            EnqueueRadioWord_4271B0(120);
                            EnqueueRadioWord_4271B0(93);
                            EnqueueRadioWord_4271B0(mRadioWord5521 + 74);
                            EnqueueRadioWord_4271B0(72);
                            EnqueueRadioWord_4271B0(73);
                            EnqueueRadioWord_4271B0(mRadioWord5524 + 73);
                            EnqueueRadioLocationPhrase_426E10(xpos, ypos);
                        }
                        break;
                    case 3:
                        if (GetFreeRadioWordSlots_427310() >= 9)
                        {
                            EnqueueRadioWord_4271B0(120);
                            EnqueueRadioWord_4271B0(92);
                            if (!(manRandomTable[2] & 1))
                            {
                                EnqueueRadioWord_4271B0(89);
                                EnqueueRadioLocationPhrase_426E10(xpos, ypos);
                                EnqueueRadioWord_4271B0(71);
                                EnqueueRadioWord_4271B0(73);
                                EnqueueRadioWord_4271B0(mRadioWord5524 + 73);
                            }
                            else
                            {
                                EnqueueRadioWord_4271B0(71);
                                EnqueueRadioWord_4271B0(73);
                                EnqueueRadioWord_4271B0(mRadioWord5524 + 73);
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
                Car* pCar = gGame_0x40_67E008->mLocalPlayer->GetPlayerCar_5698E0();
                nullsub_4();
                switch (manRandomTable[3] % 10)
                {
                    case 0:
                        if (!mTimer556E)
                        {
                            GenerateRadioVehicleDescription_426F20(pCar);
                            mTimer556E = manRandomTable[2] % 1000 + 300;
                        }
                        break;
                    case 1:
                        if (!sCrimeCalloutCooldown)
                        {
                            EnqueueRadioCrimeCallout_427340(best_crime, xpos, ypos);
                            sCrimeCalloutCooldown = manRandomTable[3] % 50 + 889;
                        }
                        break;
                    case 2:
                        if (best_crime == crime_stats_type::weapon_fired_2 && !mTimer556C)
                        {
                            if (GetFreeRadioWordSlots_427310() > 3)
                            {
                                EnqueueRadioWord_4271B0(121);
                                EnqueueRadioWord_4271B0(98);
                                EnqueueRadioWord_4271B0(99);
                                EnqueueRadioWord_4271B0(103);
                            }
                            mTimer556C = manRandomTable[2] % 1530 + 600;
                        }
                        break;
                    case 3:
                        if (!mTimer5570)
                        {
                            if (GetFreeRadioWordSlots_427310() > 6)
                            {
                                EnqueueRadioWord_4271B0(121);
                                EnqueueRadioWord_4271B0(98);
                                EnqueueRadioWord_4271B0(102);
                                EnqueueRadioWord_4271B0(88);
                                EnqueueRadioLocationPhrase_426E10(xpos, ypos);
                            }
                            mTimer5570 = manRandomTable[2] % 1067 + 150;
                        }
                        break;
                    case 7:
                        if (!sCrimeCalloutCooldown)
                        {
                            EnqueueRadioCrimeCallout_427340(best_crime, xpos, ypos);
                            sCrimeCalloutCooldown = manRandomTable[3] % 680 + 628;
                            mTimer5570 = manRandomTable[2] % 1067 + 150;
                        }
                        break;
                }
            }
        }
    }
    else if (mWantedStarCount > stars)
    {
        ResetRadioMessageState_426750();
    }
    else if (best_crime > 0 && !(manRandomTable[4] % 15) && !sCrimeCalloutCooldown)
    {
        EnqueueRadioCrimeCallout_427340(best_crime, xpos, ypos);
        sCrimeCalloutCooldown = manRandomTable[3] % 80 + 228;
    }

    mWantedStarCount = stars;
    if (best_crime > mRadioWord5524)
    {
        if (best_crime != 9)
        {
            mRadioWord5524 = best_crime;
        }
        else if (!mRadioWord5524)
        {
            mRadioWord5524 = 9;
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

    const s16 mid_x = pZone->mX + (pZone->mW >> 1);
    const s16 mid_y = pZone->mY + (pZone->mH >> 1);
    const s16 w_half = pZone->mW >> 2;
    const s16 h_half = pZone->mH >> 2;

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
        if (this->mCarInfoIdx != car_model_enum::none)
        {
            if (sound_obj::GetFreeRadioWordSlots_427310() > 3u)
            {
                sound_obj::EnqueueRadioWord_4271B0(0x79u);
                sound_obj::EnqueueRadioWord_4271B0(0x62u);
                sound_obj::EnqueueRadioWord_4271B0(0x63u);
                sound_obj::EnqueueRadioWord_4271B0(0x6Eu);
            }
            this->mCarInfoIdx = car_model_enum::none;
        }
    }
    else
    {
        const s32 car_info_idx = pCar->GetCarInfoIdx_411940();
        if (this->mCarInfoIdx != car_info_idx || sLastRadioCarRemap != static_cast<u16>(pCar->mCarSprite->get_remap_41C1F0()))
        {
            this->mCarInfoIdx = car_info_idx;
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

            sLastRadioCarRemap = static_cast<u16>(pCar->mCarSprite->get_remap_41C1F0());
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
                if ((this->manRandomTable[2] & 1) == 0)
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
        mRadioWordQueue[i] = 121;
    }
    mRadioWordReadIdx = 0;
    mRadioWordWriteIdx = 0;
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

    const s32 write_idx = mRadioWordWriteIdx;
    if ((write_idx + 1) % 15 != mRadioWordReadIdx)
    {
        mRadioWordQueue[write_idx] = val;
        mRadioWordWriteIdx = static_cast<u8>(mRadioWordWriteIdx + 1) % 15;
    }
}

MATCH_FUNC(0x427220)
void sound_obj::ProcessPoliceRadioWordsPlayback_427220()
{
    if (!(mMFrameCounter % 10u) && !mbCanPlay && !gSampManager_6FFF00.SampleNotDone_58E880())
    {
        gSampManager_6FFF00.EndSample_58E960();
        mbCanPlay = true;
    }

    if (mRadioWordReadIdx != mRadioWordWriteIdx && !gSampManager_6FFF00.SampleNotDone_58E880() && mbCanPlay == 1)
    {
        // The original keeps this dead load + store of `old`, scheduled like a plain (non volatile) store:
        // `cmp $0xF,%al` goes before it. `old` must be address-taken for VC6 to keep the store, and
        // `&old - &old` below does that while folding to 0 only after dead store elimination has run.
        // (`volatile` also keeps the store, but its barrier pins the cmp below it.)
        s32 old = mRadioWordQueue[mRadioWordReadIdx];
        if (mRadioWordReadIdx >= 15)
        {
            mRadioWordReadIdx = 15;
        }

        gSampManager_6FFF00.PlayAtIdx_58E7F0(mRadioWordQueue[mRadioWordReadIdx] + (&old - &old));
        gSampManager_6FFF00.SetSampleVol_58E7D0((110 * static_cast<u32>(mSfxVol)) / 128);
        mRadioWordReadIdx = (mRadioWordReadIdx + 1) % 15;
    }
}

MATCH_FUNC(0x427310)
u8 sound_obj::GetFreeRadioWordSlots_427310() const
{
    // note: return value and'ed with 0xFF if return type is wider
    if (mRadioWordWriteIdx < mRadioWordReadIdx)
    {
        return mRadioWordReadIdx - mRadioWordWriteIdx - 1;
    }
    else
    {
        return mRadioWordReadIdx - mRadioWordWriteIdx + 14;
    }
}

MATCH_FUNC(0x427340)
void sound_obj::EnqueueRadioCrimeCallout_427340(s32 crime_word, u8 loc_a, u8 loc_b)
{
    if (GetFreeRadioWordSlots_427310() >= 8u)
    {
        EnqueueRadioWord_4271B0(120u);
        if ((mMFrameCounter & 1) == 0)
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
    mRadioWord5521 = (mRadioWord5521 + 1) % 13;
}

MATCH_FUNC(0x427400)
u32 sound_obj::GetCopRadioZoneIndex_427400(u8 x, u8 y, gmp_map_zone** ppZone) const
{
    *ppZone = gMap_0x370_6F6268->GetZoneAtPosOfType_4DF4D0(x, y, 15u);
    if (!(*ppZone) || (*ppZone)->mNameLength != 3)
    {
        *ppZone = gMap_0x370_6F6268->GetZoneAtPosOfType_4DF4D0(x, y, 1u);
    }

    if (!*ppZone || (*ppZone)->mNameLength != 3 || (*ppZone)->mName[0] != 'm')
    {
        *ppZone = NULL;
        return 0;
    }

    u32 result = (*ppZone)->mName[2] + 10 * (*ppZone)->mName[1] - 528;
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
    Weapon* pWeapon = mAudioEntities[idx].mpObj->mpAny.pWeapon_30;
    this->msQueueSample.mReleasingSoundFlag = 2;
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
                        this->msQueueSample.mReleasingSoundFlag = 0;
                        break;
                    case weapon_type::fire_truck_gun:
                        samp_idx = 317;
                        this->msQueueSample.mReleasingSoundFlag = 0;
                        break;
                    case weapon_type::shocker:
                        samp_idx = 318;
                        this->msQueueSample.mReleasingSoundFlag = 0;
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

                pWeapon->GetSoundPos_5E3F90(&this->msQueueSample.mObj.mX,
                                            &this->msQueueSample.mObj.mY,
                                            &this->msQueueSample.mObj.mZ);
                this->msQueueSample.mEntityIndex = idx;
                this->msQueueSample.mpReleaseSource = NULL;
                const bool bHasSolidAbove = gMap_0x370_6F6268->CheckColumnHasSolidAbove_4E7FC0(msQueueSample.mObj.mX,
                                                                                    msQueueSample.mObj.mY,
                                                                                    msQueueSample.mObj.mZ);
                this->mDistSquared = ComputeEmitterDistanceSquared_4190B0();
                this->mDistCalculated = false;
                if (CalculateDistance_419020((Fix16(20) / Fix16(2)) * (Fix16(20) / Fix16(2))))
                {
                    if (VolCalc_419070(vol, Fix16(20) / Fix16(2), bHasSolidAbove))
                    {
                        this->msQueueSample.mnEmittingVolume = vol;
                        this->msQueueSample.mSampIdx = samp_idx;
                        this->msQueueSample.mSoundIntensity = Fix16(20) / Fix16(2);
                        this->msQueueSample.mMaxDistance = 20;
                        // One expression: VC6 calls GetPlayBackRateIdx first and adds rate_adjust to it after
                        // the RandomDisplacement call
                        this->msQueueSample.mRate =
                            gSampManager_6FFF00.GetPlayBackRate_58DBF0(this->msQueueSample.mSampIdx) + rate_adjust +
                            RandomDisplacement_41A650(this->msQueueSample.mSampIdx);
                        this->msQueueSample.mType = 20;
                        this->msQueueSample.mSpeedMultiplier = 0;
                        this->msQueueSample.mSampleIndex = 0;
                        this->msQueueSample.mReleasingVolumeModificator = 4;
                        this->msQueueSample.mbIs2D = false;
                        this->msQueueSample.mLoopStart = gSampManager_6FFF00.GetLoopStart_58DC30(this->msQueueSample.mSampIdx);
                        const char_type releasing_flag = this->msQueueSample.mReleasingSoundFlag;
                        this->msQueueSample.mLoopEnd = -1;
                        this->msQueueSample.mReleasingVolumeDivider = 5;
                        if (static_cast<u8>(releasing_flag) > 0)
                        {
                            this->msQueueSample.mLoopCount = 1;
                        }
                        else
                        {
                            this->msQueueSample.mLoopCount = 0;
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

    if (!gGame_0x40_67E008 || !gGame_0x40_67E008->mLocalPlayer)
    {
        return;
    }

    if (!mIsPaused)
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

    mbWasInRadioCar = mbInRadioCar;
    mActiveEmitterIdx[1] = mActiveEmitterIdx[0];
    mPrevMainVariant = mMainVariant;
    TickRadioStationClocks_57E680();

    Car* pCar = gGame_0x40_67E008->mLocalPlayer->GetPlayerCar_5698E0();
    if (pCar)
    {
        // An array keeps VC6 from folding the damage check into the branch: the original computes both
        // flags (sete) before testing either.
        bool flags[2];
        flags[0] = IsTrainOrBoxcar_57F120(pCar);
        flags[1] = pCar->IsMaxDamage_40F890();
        if (!flags[0] && !flags[1])
        {
            mbInRadioCar = 1;
            if (!mbWasInRadioCar)
            {
                ChooseRadioEmitterForVehicle_57E6C0();
                UpdateActiveRadioEmitterVolume_57EA90();
                mMuteTimer = 0;
            }
            else if (mRadioStationChangeMode)
            {
                if (mActiveEmitterIdx[0] < 5)
                {
                    mRadioEmitters[mActiveEmitterIdx[0]].mElapsedFrames = 0;
                }
                SelectBestRadioEmitter_57EF60();
            }
            else
            {
                UpdateActiveRadioEmitterVolume_57EA90();
            }

            if (!mRadioVolume)
            {
                mRadioStationChangeMode = 1;
                SelectBestRadioEmitter_57EF60();
                mMuteTimer = 0;
            }

            HandleVocalStreamSwitching_57DF10(pCar->mScale != kFpOne_6FF3F4);
            sLastPlayerCar = pCar;
            return;
        }
    }

    mbInRadioCar = 0;
    if (mbWasInRadioCar == 1)
    {
        if (sLastPlayerCar)
        {
            sLastPlayerCar->mEmitterStatus = mRadioEmitters[mActiveEmitterIdx[0]].mStationIdx;
        }
        if (mActiveEmitterIdx[1] < 5)
        {
            mRadioEmitters[mActiveEmitterIdx[1]].mElapsedFrames = 0;
        }
    }
    mActiveEmitterIdx[0] = 0x66;
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

    if (mbWasInRadioCar != mbInRadioCar)
    {
        bFast = mMainVariant == 1;
        gSampManager_6FFF00.CloseVocalStream_58E6A0(0);
        if (mActiveEmitterIdx[0] != 101)
        {
            gSampManager_6FFF00.PlayVocal_58E510(0, mRadioEmitters[mActiveEmitterIdx[0]].mStationIdx, bFast);
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
        pos = static_cast<u32>(mRadioEmitters[mActiveEmitterIdx[0]].mElapsedFrames) * 1000 / mFramesPerSecond + mRadioEmitters[mActiveEmitterIdx[0]].mPosMs;
        if (pos > length)
        {
            pos %= length;
        }
        gSampManager_6FFF00.SetVocalPosMs_58E750(0, pos);
        UpdateCarRadioAudio_57E220();
        return;
    }

    if (mActiveEmitterIdx[1] != mActiveEmitterIdx[0])
    {
        bFast = true;
        bStationChanged = true;
    }

    // The do/while (0) and the goto to done (not a return) give the original's block layout and tail merging.
    do
    {
        if (mPrevMainVariant == mMainVariant)
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
                bFast = mMainVariant == 1;
                if (mActiveEmitterIdx[0] != 101)
                {
                    gSampManager_6FFF00.PlayVocal_58E510(0, mRadioEmitters[mActiveEmitterIdx[0]].mStationIdx, bFast);
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
    if (mActiveEmitterIdx[1] < 5)
    {
        pos = gSampManager_6FFF00.GetVocalPosMs_58E770(0);
        mRadioEmitters[mActiveEmitterIdx[1]].mPosMs = pos;
    }
    gSampManager_6FFF00.CloseVocalStream_58E6A0(0);
    bFast = mMainVariant == 1;
    if (mActiveEmitterIdx[0] != 101)
    {
        gSampManager_6FFF00.PlayVocal_58E510(0, mRadioEmitters[mActiveEmitterIdx[0]].mStationIdx, bFast);
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
        pos = static_cast<u32>(mRadioEmitters[mActiveEmitterIdx[0]].mElapsedFrames) * 1000 / mFramesPerSecond + mRadioEmitters[mActiveEmitterIdx[0]].mPosMs;
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
    Car* pCar = gGame_0x40_67E008->mLocalPlayer->GetPlayerCar_5698E0();
    u32 rate;
    if (!pCar)
    {
        return;
    }
    else
    {
        if (!mbWasInRadioCar)
        {
            mMuteTimer = 0;
        }

        if (mMuteTimer)
        {
            mRadioVolume = 0;
            mMuteTimer--;
        }
        else if (gMap_0x370_6F6268->CheckColumnHasSolidAbove_4E7FC0(pCar->mCarSprite->mXy.x,
                                                                     pCar->mCarSprite->mXy.y,
                                                                     pCar->mCarSprite->mZpos))
        {
            mRadioVolume -= mRadioVolume >> 2;
        }

        u32 vocal_volume = (mRadioVolume * mCdVol) / 127;
        u8 static_volume;
        if (mRadioVolume < 115)
        {
            if (!mIsPaused)
            {
                static_volume = 127 - mRadioVolume;
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

        if (mIsPaused == 1)
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
                gCarRadioStaticTimer_6FF542 = manRandomTable[3] % 35;
                rate = gSampManager_6FFF00.GetPlayBackRate_58DBF0(137);
                rate += manRandomTable[2] % (rate >> 2);
            }
            gCarRadioStaticRate_625010 = rate;

            u8 sample_volume = static_cast<u8>((static_volume * mCdVol) / 127) >> 2;
            msQueueSample.mEntityIndex = mRadioEntityIdx;
            msQueueSample.mSampleIndex = 0;
            msQueueSample.mSampIdx = 137;
            msQueueSample.mbIs2D = true;
            msQueueSample.mnVolume = sample_volume;
            msQueueSample.mRate = rate;
            msQueueSample.mLoopStart = 0;
            msQueueSample.mLoopEnd = -1;
            msQueueSample.mReleasingVolumeModificator = 0;
            msQueueSample.mDistance = Fix16(0);
            msQueueSample.mPan = 64;
            msQueueSample.mReleasingSoundFlag = 0;
            msQueueSample.mLoopCount = 0;
            msQueueSample.mReleasingVolumeDivider = 5;
            msQueueSample.mType = 20;
            msQueueSample.mnEmittingVolume = sample_volume;
            msQueueSample.mMaxDistance = 50;
            AddSampleToRequestedQueue_41A850();

            if (mMainVariant == 0)
            {
                u32 tune_rate = gSampManager_6FFF00.GetPlayBackRate_58DBF0(138);
                tune_rate += (tune_rate >> 6) * mRadioEmitters[mActiveEmitterIdx[0]].mStationIdx;
                if (tune_rate > static_cast<u32>(gCarRadioTuneRate_625014 + 90))
                {
                    tune_rate = gCarRadioTuneRate_625014 + 90;
                }
                else if (tune_rate < static_cast<u32>(gCarRadioTuneRate_625014 - 90))
                {
                    tune_rate = gCarRadioTuneRate_625014 - 90;
                }
                gCarRadioTuneRate_625014 = tune_rate;

                u32 rnd = manRandomTable[1] % 140;
                // Same store order as 9.6f (0x4B1E40): rate, volume, then the distance
                // through the Fix16(s32) ctor, pan and type.
                msQueueSample.mSampleIndex = 1;
                msQueueSample.mSampIdx = 138;
                msQueueSample.mbIs2D = true;
                msQueueSample.mRate = tune_rate + rnd;
                msQueueSample.mnVolume =
                    static_cast<u8>(static_cast<u8>((static_volume * mCdVol) / 254) + static_cast<u8>(manRandomTable[2] % 3)) >> 2;
                msQueueSample.mDistance = Fix16(0);
                msQueueSample.mPan = 64;
                msQueueSample.mType = 20;
                AddSampleToRequestedQueue_41A850();
            }
        }

        gSampManager_6FFF00.SetVocalVolume_58E6D0(0, vocal_volume);
    }
}

MATCH_FUNC(0x57E510)
void sound_obj::UpdateVocalStream_57E510()
{
    if (mActiveEmitterIdx[0] == 0x66 && mActiveEmitterIdx[1] != 0x66)
    {
        if (mActiveEmitterIdx[1] < 5)
        {
            mRadioEmitters[mActiveEmitterIdx[1]].mPosMs = gSampManager_6FFF00.GetVocalPosMs_58E770(0);
        }
        gSampManager_6FFF00.CloseVocalStream_58E6A0(0);
        gSampManager_6FFF00.PlayVocal_58E510(0, 12, true);
        gSampManager_6FFF00.SetVocalVolume_58E6D0(0, 0);
        u32 length = gSampManager_6FFF00.GetVocalLengthMs_58E7A0(0);
        if (length > 0)
        {
            gSampManager_6FFF00.SetVocalPosMs_58E750(0, manRandomTable[2] % length);
        }
        mFadeInTimer = 60;
    }
    else if (static_cast<u32>(mFadeInTimer) > 0)
    {
        u32 volume = (3600 - 60 * static_cast<u32>(mFadeInTimer)) / 60;
        if (!mIsPaused)
        {
            mFadeInTimer--;
        }
        else if (volume > 30)
        {
            volume = 30;
        }
        gSampManager_6FFF00.SetVocalVolume_58E6D0(0, (mSfxVol * volume) / 127);
    }
    else if (mIsPaused)
    {
        gSampManager_6FFF00.SetVocalVolume_58E6D0(0, (mSfxVol * 30) / 127);
    }
    else
    {
        gSampManager_6FFF00.SetVocalVolume_58E6D0(0, (mSfxVol * 60) / 127);
    }
}

MATCH_FUNC(0x57E680)
void sound_obj::TickRadioStationClocks_57E680()
{

    if (sSoundVocalsInited)
    {
        u32 ticks = 90000 * this->mFramesPerSecond;
        for (u8 i = 0; i < 5; i++)
        {
            if (static_cast<u32>(mRadioEmitters[i].mElapsedFrames) < ticks)
            {
                ++mRadioEmitters[i].mElapsedFrames;
            }
        }
    }
}

MATCH_FUNC(0x57E6C0)
void sound_obj::ChooseRadioEmitterForVehicle_57E6C0()
{
    Car* pCar = gGame_0x40_67E008->mLocalPlayer->GetPlayerCar_5698E0();
    if (IsPoliceOrServiceVehicle_57F090(pCar) == true)
    {
        mMainVariant = 0;
        mActiveEmitterIdx[0] = 101;
        return;
    }

    if (pCar->mEmitterStatus)
    {
        mActiveEmitterIdx[0] = FindEmitterByStation_57F050(pCar->mEmitterStatus);
        return;
    }

    mMainVariant = 1;

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
        switch (static_cast<u8>(manRandomTable[0] % 5))
        {
            // The search loops return when they run out (jae ret; jmp top). A hit stores and breaks
            // out, and VC6 shares that block between case 0 and case 1.
            case 0:
                i = 0;
                while (1)
                {
                    s32 station_idx = mRadioEmitters[i].mStationIdx;
                    if (station_idx == 6 || (station_idx > 8 && station_idx <= 10))
                    {
                        mActiveEmitterIdx[0] = i;
                        break;
                    }
                    if (i == 4)
                    {
                        mActiveEmitterIdx[0] = FindEmitterByStation_57F050(1);
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
                    s32 station_idx = mRadioEmitters[i].mStationIdx;
                    if (station_idx >= 7 && (station_idx <= 8 || station_idx == 11))
                    {
                        mActiveEmitterIdx[0] = i;
                        break;
                    }
                    if (i == 4)
                    {
                        mActiveEmitterIdx[0] = FindEmitterByStation_57F050(1);
                    }
                    if (++i >= 5)
                    {
                        return;
                    }
                }
                break;
            case 2:
                mActiveEmitterIdx[0] = FindEmitterByStation_57F050(5);
                if (mActiveEmitterIdx[0] == 127)
                {
                    mActiveEmitterIdx[0] = FindEmitterByStation_57F050(1);
                }
                break;
            case 3:
                for (i = 0; i < 5; i++)
                {
                    if (mRadioEmitters[i].mAVariantRadius == 0 && mRadioEmitters[i].mStationIdx != 1)
                    {
                        mActiveEmitterIdx[0] = i;
                        return;
                    }
                }
                break;
            default:
                mActiveEmitterIdx[0] = FindEmitterByStation_57F050(1);
                break;
        }
    }
    else
    {
        mActiveEmitterIdx[0] = emitter;
        mRadioVolume = volume;
    }
}

MATCH_FUNC(0x57E960)
void sound_obj::InitMusicAndCopRadio_57E960()
{
    if (!sSoundVocalsInited)
    {
        gSoundSwitchRadioCoolDown_6FF539 = 0;
        sSoundVocalsInited = true;
        for (u8 i = 0; i < 5; i++)
        {
            mRadioEmitters[i].mbUsed = false;
            mRadioEmitters[i].mAVariantRadius = 0;
            mRadioEmitters[i].mMainRadius = 0;
            mRadioEmitters[i].mElapsedFrames = 0;
            mRadioEmitters[i].mPosMs = 0;
        }
        mbWasInRadioCar = 0;
        mActiveEmitterIdx[1] = 1;
        mActiveEmitterIdx[0] = 1;
        mRadioStationChangeMode = 0;
        mFadeInTimer = 0;
        DeclareRadioStation_57ECB0(1, Fix16(0, 0), Fix16(0, 0));
        if (mRadioEntityIdx == 0)
        {
            mRadioEntity.mpAny.pAny = NULL;
            mRadioEntity.mObjectType = SoundObjectTypeEnum::Radio_3;
            mRadioEntity.mbStatus = false;
            mRadioEntityIdx = AddSoundObject_419FA0(&mRadioEntity);
        }
    }
}

MATCH_FUNC(0x57EA10)
void sound_obj::DeInitVocals_57EA10()
{
    if (sSoundVocalsInited)
    {
        sSoundVocalsInited = false;

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
            mRadioEmitters[j].mbUsed = false;
            mRadioEmitters[j].mAVariantRadius = 0;
            mRadioEmitters[j].mMainRadius = 0;
            mRadioEmitters[j].mElapsedFrames = 0;
            mRadioEmitters[j].mPosMs = 0;
        }

        mFadeInTimer = 0;
        if (mRadioEntityIdx)
        {
            sound_obj::FreeSoundEntry_41A090(mRadioEntityIdx);
            mRadioEntityIdx = 0;
        }
    }
}

MATCH_FUNC(0x57EA90)
void sound_obj::UpdateActiveRadioEmitterVolume_57EA90()
{
    if (mActiveEmitterIdx[0] == 101)
    {
        mRadioVolume = 115;
        return;
    }

    if (mRadioEmitters[mActiveEmitterIdx[0]].mStationIdx && mRadioEmitters[mActiveEmitterIdx[0]].mbUsed == 1)
    {
        if (!mMainVariant)
        {
            u8 volume = ComputeRadioEmitterVolume_57EB90(mActiveEmitterIdx[0], 1);
            if (volume > 110)
            {
                mMainVariant = 1;
                mRadioVolume = volume;
                mMuteTimer = manRandomTable[0] % 10 + 16;
            }
            else
            {
                mRadioVolume = ComputeRadioEmitterVolume_57EB90(mActiveEmitterIdx[0], 0);
            }
        }
        else
        {
            if (ComputeRadioEmitterVolume_57EB90(mActiveEmitterIdx[0], 1) < 90)
            {
                mRadioVolume = ComputeRadioEmitterVolume_57EB90(mActiveEmitterIdx[0], 0);
                mMainVariant = 0;
                mMuteTimer = manRandomTable[0] % 10 + 16;
            }
            else
            {
                mRadioVolume = ComputeRadioEmitterVolume_57EB90(mActiveEmitterIdx[0], 1);
                mMainVariant = 1;
            }
        }
    }
    else
    {
        mRadioVolume = 0;
    }
}

MATCH_FUNC(0x57EB90)
u8 sound_obj::ComputeRadioEmitterVolume_57EB90(u8 emitterIndex, s32 bMainVariant)
{
    if (mRadioEmitters[emitterIndex].mbUsed && mRadioEmitters[emitterIndex].mStationIdx)
    {
        Fix16 xpos;
        Fix16 ypos;
        Fix16 zpos;
        gGame_0x40_67E008->mLocalPlayer->get_pos_569920(&xpos, &ypos, &zpos);

        u16 dist = abs(static_cast<s16>(ypos.ToInt() - mRadioEmitters[emitterIndex].mYpos.ToInt())) +
            abs(static_cast<s16>(xpos.ToInt() - mRadioEmitters[emitterIndex].mXpos.ToInt()));

        u16 radius;
        if (!bMainVariant)
        {
            radius = mRadioEmitters[emitterIndex].mAVariantRadius;
        }
        else
        {
            radius = mRadioEmitters[emitterIndex].mMainRadius;
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
        if (!mRadioEmitters[i].mbUsed && !mRadioEmitters[i].mAVariantRadius && !mRadioEmitters[i].mMainRadius)
        {
            mRadioEmitters[i].mbUsed = true;
            mRadioEmitters[i].mStationIdx = station_idx;
            mRadioEmitters[i].mXpos = Fix16(static_cast<u16>(xpos.ToInt()));
            mRadioEmitters[i].mYpos = Fix16(static_cast<u16>(ypos.ToInt()));
            mRadioEmitters[i].mElapsedFrames = mRadioEmitters[i].mPosMs =
                manRandomTable[(i + 1) % 5] * manRandomTable[i % 5];
            SetRadioEmitterRadii_57EDB0(&mRadioEmitters[i], station_idx);
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
            pEmitter->mAVariantRadius = 0;
            pEmitter->mMainRadius = 600;
            pEmitter->mXpos = 0;
            pEmitter->mYpos = 0;
            break;
        case 2:
            pEmitter->mAVariantRadius = 0;
            pEmitter->mMainRadius = 600;
            break;
        case 3:
            pEmitter->mAVariantRadius = 0;
            pEmitter->mMainRadius = 600;
            break;
        case 4:
            pEmitter->mAVariantRadius = 0;
            pEmitter->mMainRadius = 600;
            break;
        case 5:
            pEmitter->mAVariantRadius = 600;
            pEmitter->mMainRadius = 300;
            break;
        case 6:
            pEmitter->mAVariantRadius = 600;
            pEmitter->mMainRadius = 300;
            break;
        case 7:
            pEmitter->mAVariantRadius = 600;
            pEmitter->mMainRadius = 300;
            break;
        case 8:
            pEmitter->mAVariantRadius = 600;
            pEmitter->mMainRadius = 300;
            break;
        case 9:
            pEmitter->mAVariantRadius = 600;
            pEmitter->mMainRadius = 300;
            break;
        case 10:
            pEmitter->mAVariantRadius = 600;
            pEmitter->mMainRadius = 300;
            break;
        case 11:
            pEmitter->mAVariantRadius = 600;
            pEmitter->mMainRadius = 300;
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
        if (mRadioEmitters[i - 1].mbUsed == 1 && static_cast<s16>(mRadioEmitters[i - 1].mXpos.ToInt()) == x &&
            static_cast<s16>(mRadioEmitters[i - 1].mYpos.ToInt()) == y)
        {
            mRadioEmitters[i - 1].mbUsed = false;
            mRadioEmitters[i - 1].mAVariantRadius = 0;
            mRadioEmitters[i - 1].mMainRadius = 0;
            mRadioEmitters[i - 1].mElapsedFrames = 0;
            mRadioEmitters[i - 1].mPosMs = 0;
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
            Player* pPlayer = gGame_0x40_67E008->mLocalPlayer;
            if (pPlayer)
            {
                if (pPlayer->GetPlayerCar_5698E0())
                {
                    Car* pCar = gGame_0x40_67E008->mLocalPlayer->GetPlayerCar_5698E0();
                    // can't change their sucky radio station ;)
                    if (!IsPoliceOrServiceVehicle_57F090(pCar))
                    {
                        Car* pCarAgain = gGame_0x40_67E008->mLocalPlayer->GetPlayerCar_5698E0();
                        // nor trains, which makes sense
                        if (!IsTrainOrBoxcar_57F120(pCarAgain))
                        {
                            if (bPrev)
                            {
                                mRadioStationChangeMode = 1;
                            }
                            else
                            {
                                mRadioStationChangeMode = 2;
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
    u8 candidate = mActiveEmitterIdx[0];
    for (u8 i = 0; i < 5; i++)
    {
        if (mRadioStationChangeMode == 1)
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
                mActiveEmitterIdx[0] = candidate;
                mRadioVolume = volume;
                mMainVariant = 1 - bNear;
                mRadioStationChangeMode = 0;
                mMuteTimer = manRandomTable[0] % 15 + 15;
                return;
            }
        }
    }
    mRadioStationChangeMode = 0;
}

MATCH_FUNC(0x57F050)
u8 sound_obj::FindEmitterByStation_57F050(s32 station_idx) const
{
    for (u8 i = 0; i < 5; i++)
    {
        if (static_cast<u32>(mRadioEmitters[i].mStationIdx) == static_cast<u32>(station_idx))
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

    if (pCar->mCarInfoIdx == car_model_enum::boxcar ||
        (pCar->mCarInfoIdx > car_model_enum::TOWTRUCK && pCar->mCarInfoIdx <= car_model_enum::TRAINFB))
    {
        return true;
    }
    return false;
}
