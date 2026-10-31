#pragma once

#include "Function.hpp"
#include "ang16.hpp"
#include "fix16.hpp"
#include "root_sound.hpp" // for SoundObject
#include <windows.h>

class Car;
class Object;
class gmp_map_zone;
class CollisionEvent;
class SoundObject;

class SoundPos
{
  public:
    // Compiler-generated constructor at 0x419DF0
    Fix16 mX; // 0x0
    Fix16 mY; // 0x4
    Fix16 mZ; // 0x8
};
GTA2_ASSERT_SIZEOF_ALWAYS(SoundPos, 0xC);

struct SoundPosEntry
{
    // 9.6f 0x416C00
    SoundPosEntry()
    {
    }

    Fix16 mLastSpeed; // 0x0
    Car* mCar; // 0x4
    s16 mUnused8; // 0x8
    u8 mHornPatternIdx; // 0xA
    char_type mUnusedB; // 0xB
};
GTA2_ASSERT_SIZEOF_ALWAYS(SoundPosEntry, 0xC);

struct AudioEntity
{
    bool mbUsed; // 0x0
    u8 mAge; // 0x1
    char_type mUnused2; // 0x2
    char_type mUnused3; // 0x3
    SoundObject* mpObj; // 0x4
    SoundPosEntry* mpAlloc; // 0x8
};
GTA2_ASSERT_SIZEOF_ALWAYS(AudioEntity, 0xC);

struct SoundEntry
{
    bool mFifthFrameFlag; // 0x0
    char_type mUnused1[3]; // 0x1
    s32 mEntityIdx; // 0x4
    SoundObject mSoundObj; // 0x8
    s32 mPendingVoice; // 0x18
};
GTA2_ASSERT_SIZEOF_ALWAYS(SoundEntry, 0x1C);

struct RadioStationEmitter
{
    bool mbUsed; // 0x0
    u8 mUnused1; // 0x1
    u8 mUnused2; // 0x2
    u8 mUnused3; // 0x3
    
    Fix16 mXpos; // 0x4
    Fix16 mYpos; // 0x8

    s32 mStationIdx; // 0xC
    s16 mAVariantRadius; // 0x10
    s16 mMainRadius; // 0x12
    s32 mElapsedFrames; // 0x14
    s32 mPosMs; // 0x18
};
GTA2_ASSERT_SIZEOF_ALWAYS(RadioStationEmitter, 0x1C);

struct sound_0x68
{
    s32 mEntityIndex; // 0x0
    char_type mSampleIndex; // 0x4
    char_type mUnused5; // 0x5
    char_type mUnused6; // 0x6
    char_type mUnused7; // 0x7
    SoundPos mObj; // 0x8
    u32 mSampIdx; // 0x14
    bool mbIs2D; // 0x18
    char_type mUnused19; // 0x19
    char_type mUnused1A; // 0x1A
    char_type mUnused1B; // 0x1B
    u32 mReleasingVolumeModificator; // 0x1C
    u32 mRate; // 0x20
    u8 mnVolume; // 0x24
    char_type mUnused25; // 0x25
    char_type mUnused26; // 0x26
    char_type mUnused27; // 0x27
    Fix16 mDistance; // 0x28
    bool mbIsBeingPlayed; // 0x2C
    bool mbIsPlayingFinished; // 0x2D
    char_type mUnused2E; // 0x2E
    char_type mUnused2F; // 0x2F
    u32 mLoopCount; // 0x30
    s32 mLoopStart; // 0x34
    s32 mLoopEnd; // 0x38
    s32 mSpeedMultiplier; // 0x3C
    char_type mPan; // 0x40
    char_type mReleasingSoundFlag; // 0x41
    char_type mUnused42; // 0x42
    char_type mUnused43; // 0x43
    s32 mFramesToPlay; // 0x44
    u32 mnCalculatedVolume; // 0x48
    u32 mReleasingVolumeDivider; // 0x4C
    u8 mVolumeChange; // 0x50
    char_type mUnused51; // 0x51
    char_type mUnused52; // 0x52
    char_type mUnused53; // 0x53
    Fix16 mSoundIntensity; // 0x54
    s32 mType; // 0x58
    sound_0x68* mpReleaseSource; // 0x5C
    u8 mnEmittingVolume; // 0x60
    char_type mUnused61; // 0x61
    char_type mUnused62; // 0x62
    char_type mUnused63; // 0x63
    s32 mMaxDistance; // 0x64
};

struct Sound_Params
{
    Sprite* mpObj; // 0x0
    char_type mbDrivenByPlayer; // 0x4
    bool mbHasSolidAbove; // 0x5
    // pad?
    char_type mUnused6; // 0x6
    char_type mUnused7; // 0x7
};
GTA2_ASSERT_SIZEOF_ALWAYS(Sound_Params, 0x8);

class sound_obj
{
  private:
    bool mbSoundInitialized; // 0x0
    bool mIsPaused; // 0x1
    bool mServiceEnabled; // 0x2
    char_type mbFirstMenuStream; // 0x3: frontend music restarts d.wav (stream 0) while set, m.wav (stream 1) once cleared
    s32 mSpeedOfSound; // 0x4
    u8 mFramesPerSecond; // 0x8
    char_type mUnused9; // 0x9
    char_type mUnusedA; // 0xA
    char_type mUnusedB; // 0xB
    s32 mDopplerDivisor; // 0xC
    u8 mnActiveSamples; // 0x10
    char_type mUnused11; // 0x11
    char_type mUnused12; // 0x12
    char_type mUnused13; // 0x13
    s32 mSampleRate; // 0x14
    char_type mUnused18; // 0x18
    char_type mUnused19; // 0x19
    bool mbDoubleVolume; // 0x1A
    char_type mUnused1B; // 0x1B
    u8 mSampCount; // 0x1C
    char_type mB3dSound; // 0x1D
    char_type mUnused1E; // 0x1E
    char_type mUnused1F; // 0x1F
    s32 mUnused20; // 0x20
    u8 mSfxVol; // 0x24
    u8 mCdVol; // 0x25
    char_type mUnused26; // 0x26
    char_type mUnused27; // 0x27
    Fix16 mDistSquared; // 0x28
    bool mDistCalculated; // 0x2C
    char_type mUnused2D; // 0x2D
    char_type mUnused2E; // 0x2E
    char_type mUnused2F; // 0x2F
    sound_0x68 msQueueSample; // 0x30
    u8 mnActiveSampleQueue; // 0x98
    sound_0x68 masSamples[2][16]; // 0x9C
    u8 mabSampleQueueIndexTable[2][16]; // 0xD9C
    u8 mSampleRequestQueuesStatus[2]; // 0xDBC
    s16 mUnusedDBE; // 0xDBE
    sound_0x68 mActiveSamples[16]; // 0xDC0
    s32 mUnused1440; // 0x1440
    char_type mUnused1444; // 0x1444
    char_type mUnused1445; // 0x1445
    char_type mUnused1446; // 0x1446
    char_type mUnused1447; // 0x1447
    s32 mUnused1448; // 0x1448
    s32 mUnused144C; // 0x144C
    char_type mUnused1450; // 0x1450
    char_type mUnused1451; // 0x1451
    char_type mUnused1452; // 0x1452
    char_type mUnused1453; // 0x1453
    u32 manRandomTable[5]; // 0x1454
    Fix16 mListenerX; // 0x1468
    Fix16 mListenerY; // 0x146C
    Fix16 mListenerZ; // 0x1470
    Ang16 mRotation; // 0x1474
    char_type mUnused1476; // 0x1476
    char_type mUnused1477; // 0x1477
    s32 mType5Idx; // 0x1478
    AudioEntity mAudioEntities[1020]; // 0x147C
    u32 mAudioEntityOrderList[1020]; // 0x444C
    u32 m444CNAudioEntitiesCount; // 0x543C
    s32 mUnused5440; // 0x5440
    s32 mUnused5444; // 0x5444
    u32 mMFrameCounter; // 0x5448
    SoundEntry mVocals; // 0x544C
    RadioStationEmitter mRadioEmitters[5]; // 0x5468
    u8 mbWasInRadioCar; // 0x54F4
    u8 mbInRadioCar; // 0x54F5
    u8 mRadioVolume; // 0x54F6
    u8 mActiveEmitterIdx[5]; // 0x54F7
    s32 mMainVariant; // 0x54FC
    s32 mPrevMainVariant; // 0x5500
    char_type mRadioStationChangeMode; // 0x5504
    char_type mUnused5505; // 0x5505
    char_type mMuteTimer; // 0x5506
    char_type mUnused5507; // 0x5507
    s32 mRadioEntityIdx; // 0x5508
    SoundObject mRadioEntity; // 0x550C
    s32 mFadeInTimer; // 0x551C
    bool mbCanPlay; // 0x5520
    u8 mRadioWord5521; // 0x5521
    s16 mWantedStarCount; // 0x5522
    s32 mRadioWord5524; // 0x5524
    u8 mRadioWordReadIdx; // 0x5528
    u8 mRadioWordWriteIdx; // 0x5529
    s32 mRadioWordQueue[15]; // 0x552C
    char_type mCrimeXpos; // 0x5568
    char_type mCrimeYpos; // 0x5569
    s16 mErvEnRouteTimer; // 0x556A
    s16 mTimer556C; // 0x556C
    s16 mTimer556E; // 0x556E
    s16 mTimer5570; // 0x5570
    s16 mUnused5572; // 0x5572: pad?
    s32 mCarInfoIdx; // 0x5574

  public:
    STATIC_MEMBER(bool, sSoundVocalsInited);
    EXPORT sound_obj();

    EXPORT void Reacquire_41A2A0();
    EXPORT char_type GetAudioDriveLetter_41A2E0() const;
    EXPORT char_type Get3dSound_41A390() const;
    EXPORT void Service_419EF0();
    EXPORT void Release_41A290();
    EXPORT u8 GetCDVol_41A280() const;
    EXPORT void Init_419E10();
    EXPORT void DeinitializeAudio_419E50();
    EXPORT s32 get_samp_idx_for_car_417D70(Car* pCar, bool bDoorOpening, bool bLargeDoorOffset) const;
    EXPORT s32 samp_idx_for_model_417AC0(s32 car_model) const;
    EXPORT s32 GetSirenSampleIdx_417B80(s32 car_model, bool bHornOn) const;

    EXPORT void PreSoundInit_412240();
    EXPORT void PreSoundShutdown_412250();
    EXPORT bool InitSampleForType_412260(sound_0x68* pObj);
    EXPORT void ProcessEntity_4123A0(s32 idx);
    EXPORT void ProcessType2_Frontend_412490(s32 idx);
    EXPORT void ProcessType1_Sprite_412740(s32 idx);
    EXPORT void ProcessType8_Crane_412820(s32 idx);
    EXPORT void ProcessType9_Crusher_412A60(s32 idx);
    EXPORT void ProcessCar_412B80(Sound_Params* pParams);
    EXPORT bool SelectCollisionSound_412C90(CollisionEvent* pEvent, u8 volume);
    EXPORT bool SelectObjectImpactSound_1_10_412D30(CollisionEvent* pEvent);
    EXPORT bool SelectCarCarCollisionSound_412D40(u8 volume);
    EXPORT void PlayPickupVoice_412D90(s32 model);
    EXPORT bool SelectCarObjectCollisionSound_413000(CollisionEvent* pEvent);
    EXPORT bool SelectCarPedCollisionSound_413040(u8 volume);
    EXPORT bool SelectCarWallCollisionSound_413090(u8 volume);
    EXPORT bool SelectPedObjectCollisionSound_4130E0(CollisionEvent* pEvent);
    EXPORT bool SelectObjectImpactSound_413120(CollisionEvent* pEvent, s32 interactionType);
    EXPORT bool SelectObjectObjectCollisionSound_413540(CollisionEvent* pEvent);
    EXPORT bool SelectCarFloorCollisionSound_4136D0(CollisionEvent* pEvent);
    EXPORT void ProcessType6_CollisionSoundQueue_413760(s32 idx);
    EXPORT u8 GetCollisionVolume_413A10(CollisionEvent* pEvent);
    EXPORT void ProcessTrainCab_413B90(Sound_Params* pParams, SoundPosEntry* pAlloc);
    EXPORT void ProcessTrain_413BE0(Sound_Params* pParams);
    EXPORT void ProcessTank_413BF0(Sound_Params* pParams, SoundPosEntry* pAlloc);
    EXPORT void ProcessOtherCarTypes_413C50(Sound_Params* pParams, SoundPosEntry* pAlloc);
    EXPORT void HandleAICarHornBeep_413D10(Sound_Params* pParams);
    EXPORT void HandleTrainEngineSound_4140C0(Sound_Params* pParams);

    EXPORT bool Type_16_InitTrainEngineSound_414320(sound_0x68* pObj);
    EXPORT void HandleTrainCabRollingFrictionSound_4143A0(Sound_Params* pParams);
    EXPORT bool GetCar_4145E0(s32 idx, Car** ppOut) const;
    EXPORT bool Type_17_InitTrainRollingFrictionSound_414690(sound_0x68* pObj);
    EXPORT void HandleTrainStationSound_414710(Sound_Params* pParams);
    EXPORT bool Type_19_InitTrainStationSound_4149D0(sound_0x68* pObj);
    EXPORT void HandleTankMovementSound_414A50(Sound_Params* pParams);
    EXPORT bool Type_12_InitTankMovementSound_414C90(sound_0x68* pSound);
    EXPORT void HandleTankTurretSound_414D30(Sound_Params* pParams);
    EXPORT bool Type_11_InitTankTurretSound_414EE0(sound_0x68* pSound);
    EXPORT void HandleCarBombSound_414F90(Sound_Params* pParams);
    EXPORT bool Type_15_InitCarBombSound_415100(sound_0x68* pSound);
    EXPORT void HandleTankCrushSound_415190(Sound_Params* pParams);
    EXPORT bool Type_13_InitTankCrushSound_4153F0(sound_0x68* pObj);
    EXPORT void PlayCarShopEventSound_415480(Sound_Params* pParams);
    EXPORT void HandleCarHornSound_415570(Sound_Params* pParams, SoundPosEntry* pAlloc);
    EXPORT bool Type_5_InitHornSound_415730(sound_0x68* pSound);
    EXPORT void HandleCarEngineSound_4157C0(Sound_Params* pParams);
    EXPORT bool Type_1_6_InitCarEngineOrDamageSound_416260(sound_0x68* pSound);
    EXPORT void HandleCarAlarmSound_417060(Sound_Params* pParams);
    EXPORT bool Type_3_InitCarAlarmSound_4174C0(sound_0x68* pSound);
    EXPORT void HandleCarDamageSound_4177D0(Sound_Params* pParams);
    EXPORT void HandleSirenActivationSound_4178C0(Sound_Params* pParams);
    EXPORT bool Type_4_InitSirenSound_417A00(sound_0x68* pSound);
    EXPORT s32 GetVehicleAudioClass_417BA0(s32 car_model) const;
    EXPORT s32 ConvertToPlayBackRate_417C60(s32 car_model) const;
    EXPORT void HandleHeavyVehicleStopSound_417E30(Sound_Params* pParams, SoundPosEntry* pAlloc);
    EXPORT bool Type_7_InitHeavyVehicleStopSound_417EF0(sound_0x68* pObj);
    EXPORT bool IsHeavyTruckOrBus_417F40(s32 car_model) const;
    EXPORT void HandleTruckCorneringAudio_417FD0(Sound_Params* pParams);
    EXPORT bool Type_8_InitTruckCorneringSound_418130(sound_0x68* pObj);
    EXPORT void HandleAICarEngineSound_418190(Sound_Params* pParams);
    EXPORT bool Type_2_InitAICarEngineSound_4182A0(sound_0x68* pObj);
    EXPORT void HandleCarDoorSounds_4182E0(Sound_Params* pParams);
    EXPORT void HandleCarEngineStartSound_418610(Sound_Params* pParams);
    EXPORT bool Type_9_InitCarEngineStartSound_4186D0(sound_0x68* pObj);
    EXPORT void HandleCarTireScrubSound_418720(Sound_Params* pParams);
    EXPORT bool Type_10_HandleCarSkidSound_418940(sound_0x68* pSound);
    EXPORT void ProcessType11_HudPager_418B60(s32 entity_idx);
    EXPORT void AddVocalsEntity_418C20();
    EXPORT void RemoveVocalsEntity_418C60();
    EXPORT void PlayVocal_418C80(s32 voice);
    EXPORT void ProcessType10_Vocals_418CA0();
    EXPORT bool CalculateDistance_419020(Fix16 max_dist);
    EXPORT bool VolCalc_419070(u8 base_volume, Fix16 max_dist, bool bHasSolidAbove);
    EXPORT Fix16 ComputeEmitterDistanceSquared_4190B0() const;
    EXPORT s32 AddSoundObject_419FA0(SoundObject* pSoundObj);

    EXPORT void FreeSoundEntry_41A090(u32 idx);
    EXPORT bool LoadStyle_41A1B0(const char_type* pStyleName);
    EXPORT const char_type* GetFileName_41A1E0(const char_type* pStr);
    EXPORT void SetSfxVol_41A250(u8 sfxVol);
    EXPORT void SetCDVol_41A270(u8 cdVol);
    EXPORT bool Set3DSound_41A2F0(char_type b3dSound);
    EXPORT void ServiceSoundEffects_41A3A0();
    EXPORT u8 CalcVolume_41A3F0(u8 base_volume, Fix16 max_dist, Fix16 dist) const;
    EXPORT u8 ComputePan_41A4A0(Fix16 intensity, Fix16 x_offset) const;
    EXPORT s32 AdjustPlaybackRate_41A580(s32 snd_rate, Fix16 xpos, Fix16 ypos, Fix16 zpos) const;
    EXPORT s32 RandomDisplacement_41A650(u32 seed);
    EXPORT void ResetEntry_41A6C0(s32 idx);
    EXPORT void IncrementAudioEntitiesAge_41A6F0();
    EXPORT void InterrogateAudioEntities_41A730();
    EXPORT void AddSampleToRequestedQueue_41A850();
    EXPORT void AddDetailsToRequestedOrderList_41A910(u8 sample);
    EXPORT void AddReleasingSounds_41A9D0();
    EXPORT void ProcessActiveQueues_41AB80();

    EXPORT void UpdateFramesToPlay_41B490(sound_0x68* pObj);
    EXPORT void VecDiff_41B4E0(const SoundPos* pVec, SoundPos* pRet) const;
    EXPORT void FromFix16_41B520(Fix16 value, f32* pOut) const;
    EXPORT void AdjustSamplesVolume_41B540();
    EXPORT u8 ComputeEmittingVolume_41B660(u8 emittingVolume, u32 maxDistance, u32 distance) const;
    EXPORT void ClearRequestedQueue_41B700();
    EXPORT void ClearActivateSamples_41B7A0();
    EXPORT void GenerateIntegerRandomNumberTable_41BA90();

    EXPORT void ProcessObject_41E820(Sound_Params* pParams);
    EXPORT void ProcessEffectOnlyObject_41E850(Sound_Params* pParams);
    EXPORT void ProcessOtherObjects_41F520(Sound_Params* pParams);
    EXPORT void UpdateRadioChatterLoop_41FCA0();

    EXPORT void ProcessPed_422B70(Sound_Params* pParams);
    EXPORT void HandlePedVoiceEvent_423080(Sound_Params* pParams);

    EXPORT void ResetRadioMessageState_426750();
    EXPORT void PoliceRadioMessageGeneration_426790();
    EXPORT void EnqueueRadioLocationPhrase_426E10(u8 xpos, u8 ypos);
    EXPORT void GenerateRadioVehicleDescription_426F20(Car* pCar);

    EXPORT void Init_RadioMessageQueue_427180();
    EXPORT void EnqueueRadioWord_4271B0(u32 val);
    EXPORT void ProcessPoliceRadioWordsPlayback_427220();
    EXPORT u8 GetFreeRadioWordSlots_427310() const;
    EXPORT void EnqueueRadioCrimeCallout_427340(s32 crime_word, u8 loc_a, u8 loc_b);
    EXPORT void AppendRadioMessageSuffix_4273B0();
    EXPORT void nullsub_4();
    EXPORT u32 GetCopRadioZoneIndex_427400(u8 x, u8 y, gmp_map_zone** ppZone) const;

    EXPORT void ProcessType7_Weapon_42A500(s32 idx);

    EXPORT void ProcessType3_CopRadioAndMusic_57DD50();
    EXPORT void HandleVocalStreamSwitching_57DF10(char_type bFastForward);
    EXPORT void UpdateCarRadioAudio_57E220();
    EXPORT void UpdateVocalStream_57E510();
    EXPORT void TickRadioStationClocks_57E680();
    EXPORT void ChooseRadioEmitterForVehicle_57E6C0();
    EXPORT void InitMusicAndCopRadio_57E960();
    EXPORT void DeInitVocals_57EA10();
    EXPORT void UpdateActiveRadioEmitterVolume_57EA90();
    EXPORT u8 ComputeRadioEmitterVolume_57EB90(u8 emitterIndex, s32 bMainVariant);
    EXPORT void DeclareRadioStation_57ECB0(s32 station_idx, Fix16 xpos, Fix16 ypos);
    EXPORT void SetRadioEmitterRadii_57EDB0(RadioStationEmitter* pEmitter, s32 type);
    EXPORT void RemoveSound_57EE30(Fix16 xpos, Fix16 ypos);
    EXPORT void CycleRadioStation_57EEE0(char_type bPrev);
    EXPORT void SelectBestRadioEmitter_57EF60();
    EXPORT u8 FindEmitterByStation_57F050(s32 station_idx) const;
    EXPORT bool IsPoliceOrServiceVehicle_57F090(Car* pCar) const;
    EXPORT bool IsTrainOrBoxcar_57F120(Car* pCar) const;
};

EXTERN_GLOBAL(sound_obj, gSound_obj_66F680);
