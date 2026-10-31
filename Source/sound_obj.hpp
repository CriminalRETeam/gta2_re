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

    EXPORT void Reacquire();
    EXPORT char_type GetAudioDriveLetter() const;
    EXPORT char_type Get3dSound() const;
    EXPORT void Service();
    EXPORT void Release();
    EXPORT u8 GetCDVol() const;
    EXPORT void Init();
    EXPORT void DeinitializeAudio();
    EXPORT s32 get_samp_idx_for_car(Car* pCar, bool bDoorOpening, bool bLargeDoorOffset) const;
    EXPORT s32 samp_idx_for_model(s32 car_model) const;
    EXPORT s32 GetSirenSampleIdx(s32 car_model, bool bHornOn) const;

    EXPORT void PreSoundInit();
    EXPORT void PreSoundShutdown();
    EXPORT bool InitSampleForType(sound_0x68* pObj);
    EXPORT void ProcessEntity(s32 idx);
    EXPORT void ProcessType2_Frontend_412490(s32 idx);
    EXPORT void ProcessType1_Sprite(s32 idx);
    EXPORT void ProcessType8_Crane(s32 idx);
    EXPORT void ProcessType9_Crusher(s32 idx);
    EXPORT void ProcessCar(Sound_Params* pParams);
    EXPORT bool SelectCollisionSound(CollisionEvent* pEvent, u8 volume);
    EXPORT bool SelectObjectImpactSound_1_10(CollisionEvent* pEvent);
    EXPORT bool SelectCarCarCollisionSound(u8 volume);
    EXPORT void PlayPickupVoice(s32 model);
    EXPORT bool SelectCarObjectCollisionSound(CollisionEvent* pEvent);
    EXPORT bool SelectCarPedCollisionSound(u8 volume);
    EXPORT bool SelectCarWallCollisionSound(u8 volume);
    EXPORT bool SelectPedObjectCollisionSound(CollisionEvent* pEvent);
    EXPORT bool SelectObjectImpactSound(CollisionEvent* pEvent, s32 interactionType);
    EXPORT bool SelectObjectObjectCollisionSound(CollisionEvent* pEvent);
    EXPORT bool SelectCarFloorCollisionSound(CollisionEvent* pEvent);
    EXPORT void ProcessType6_CollisionSoundQueue_413760(s32 idx);
    EXPORT u8 GetCollisionVolume_413A10(CollisionEvent* pEvent);
    EXPORT void ProcessTrainCab(Sound_Params* pParams, SoundPosEntry* pAlloc);
    EXPORT void ProcessTrain(Sound_Params* pParams);
    EXPORT void ProcessTank(Sound_Params* pParams, SoundPosEntry* pAlloc);
    EXPORT void ProcessOtherCarTypes(Sound_Params* pParams, SoundPosEntry* pAlloc);
    EXPORT void HandleAICarHornBeep(Sound_Params* pParams);
    EXPORT void HandleTrainEngineSound(Sound_Params* pParams);

    EXPORT bool Type_16_InitTrainEngineSound(sound_0x68* pObj);
    EXPORT void HandleTrainCabRollingFrictionSound(Sound_Params* pParams);
    EXPORT bool GetCar(s32 idx, Car** ppOut) const;
    EXPORT bool Type_17_InitTrainRollingFrictionSound(sound_0x68* pObj);
    EXPORT void HandleTrainStationSound(Sound_Params* pParams);
    EXPORT bool Type_19_InitTrainStationSound(sound_0x68* pObj);
    EXPORT void HandleTankMovementSound(Sound_Params* pParams);
    EXPORT bool Type_12_InitTankMovementSound(sound_0x68* pSound);
    EXPORT void HandleTankTurretSound(Sound_Params* pParams);
    EXPORT bool Type_11_InitTankTurretSound(sound_0x68* pSound);
    EXPORT void HandleCarBombSound(Sound_Params* pParams);
    EXPORT bool Type_15_InitCarBombSound(sound_0x68* pSound);
    EXPORT void HandleTankCrushSound(Sound_Params* pParams);
    EXPORT bool Type_13_InitTankCrushSound(sound_0x68* pObj);
    EXPORT void PlayCarShopEventSound(Sound_Params* pParams);
    EXPORT void HandleCarHornSound(Sound_Params* pParams, SoundPosEntry* pAlloc);
    EXPORT bool Type_5_InitHornSound(sound_0x68* pSound);
    EXPORT void HandleCarEngineSound(Sound_Params* pParams);
    EXPORT bool Type_1_6_InitCarEngineOrDamageSound(sound_0x68* pSound);
    EXPORT void HandleCarAlarmSound(Sound_Params* pParams);
    EXPORT bool Type_3_InitCarAlarmSound(sound_0x68* pSound);
    EXPORT void HandleCarDamageSound(Sound_Params* pParams);
    EXPORT void HandleSirenActivationSound(Sound_Params* pParams);
    EXPORT bool Type_4_InitSirenSound(sound_0x68* pSound);
    EXPORT s32 GetVehicleAudioClass(s32 car_model) const;
    EXPORT s32 ConvertToPlayBackRate(s32 car_model) const;
    EXPORT void HandleHeavyVehicleStopSound(Sound_Params* pParams, SoundPosEntry* pAlloc);
    EXPORT bool Type_7_InitHeavyVehicleStopSound(sound_0x68* pObj);
    EXPORT bool IsHeavyTruckOrBus(s32 car_model) const;
    EXPORT void HandleTruckCorneringAudio(Sound_Params* pParams);
    EXPORT bool Type_8_InitTruckCorneringSound(sound_0x68* pObj);
    EXPORT void HandleAICarEngineSound(Sound_Params* pParams);
    EXPORT bool Type_2_InitAICarEngineSound(sound_0x68* pObj);
    EXPORT void HandleCarDoorSounds(Sound_Params* pParams);
    EXPORT void HandleCarEngineStartSound(Sound_Params* pParams);
    EXPORT bool Type_9_InitCarEngineStartSound(sound_0x68* pObj);
    EXPORT void HandleCarTireScrubSound(Sound_Params* pParams);
    EXPORT bool Type_10_HandleCarSkidSound(sound_0x68* pSound);
    EXPORT void ProcessType11_HudPager(s32 entity_idx);
    EXPORT void AddVocalsEntity();
    EXPORT void RemoveVocalsEntity();
    EXPORT void PlayVocal(s32 voice);
    EXPORT void ProcessType10_Vocals();
    EXPORT bool CalculateDistance(Fix16 max_dist);
    EXPORT bool VolCalc(u8 base_volume, Fix16 max_dist, bool bHasSolidAbove);
    EXPORT Fix16 ComputeEmitterDistanceSquared() const;
    EXPORT s32 AddSoundObject(SoundObject* pSoundObj);

    EXPORT void FreeSoundEntry(u32 idx);
    EXPORT bool LoadStyle(const char_type* pStyleName);
    EXPORT const char_type* GetFileName(const char_type* pStr);
    EXPORT void SetSfxVol(u8 sfxVol);
    EXPORT void SetCDVol(u8 cdVol);
    EXPORT bool Set3DSound(char_type b3dSound);
    EXPORT void ServiceSoundEffects();
    EXPORT u8 CalcVolume(u8 base_volume, Fix16 max_dist, Fix16 dist) const;
    EXPORT u8 ComputePan(Fix16 intensity, Fix16 x_offset) const;
    EXPORT s32 AdjustPlaybackRate(s32 snd_rate, Fix16 xpos, Fix16 ypos, Fix16 zpos) const;
    EXPORT s32 RandomDisplacement(u32 seed);
    EXPORT void ResetEntry(s32 idx);
    EXPORT void IncrementAudioEntitiesAge();
    EXPORT void InterrogateAudioEntities();
    EXPORT void AddSampleToRequestedQueue();
    EXPORT void AddDetailsToRequestedOrderList(u8 sample);
    EXPORT void AddReleasingSounds();
    EXPORT void ProcessActiveQueues();

    EXPORT void UpdateFramesToPlay(sound_0x68* pObj);
    EXPORT void VecDiff(const SoundPos* pVec, SoundPos* pRet) const;
    EXPORT void FromFix16(Fix16 value, f32* pOut) const;
    EXPORT void AdjustSamplesVolume();
    EXPORT u8 ComputeEmittingVolume(u8 emittingVolume, u32 maxDistance, u32 distance) const;
    EXPORT void ClearRequestedQueue();
    EXPORT void ClearActivateSamples();
    EXPORT void GenerateIntegerRandomNumberTable();

    EXPORT void ProcessObject(Sound_Params* pParams);
    EXPORT void ProcessEffectOnlyObject(Sound_Params* pParams);
    EXPORT void ProcessOtherObjects(Sound_Params* pParams);
    EXPORT void UpdateRadioChatterLoop();

    EXPORT void ProcessPed(Sound_Params* pParams);
    EXPORT void HandlePedVoiceEvent(Sound_Params* pParams);

    EXPORT void ResetRadioMessageState();
    EXPORT void PoliceRadioMessageGeneration_426790();
    EXPORT void EnqueueRadioLocationPhrase(u8 xpos, u8 ypos);
    EXPORT void GenerateRadioVehicleDescription_426F20(Car* pCar);

    EXPORT void Init_RadioMessageQueue();
    EXPORT void EnqueueRadioWord(u32 val);
    EXPORT void ProcessPoliceRadioWordsPlayback();
    EXPORT u8 GetFreeRadioWordSlots() const;
    EXPORT void EnqueueRadioCrimeCallout(s32 crime_word, u8 loc_a, u8 loc_b);
    EXPORT void AppendRadioMessageSuffix();
    EXPORT void nullsub_4();
    EXPORT u32 GetCopRadioZoneIndex(u8 x, u8 y, gmp_map_zone** ppZone) const;

    EXPORT void ProcessType7_Weapon(s32 idx);

    EXPORT void ProcessType3_CopRadioAndMusic();
    EXPORT void HandleVocalStreamSwitching(char_type bFastForward);
    EXPORT void UpdateCarRadioAudio();
    EXPORT void UpdateVocalStream();
    EXPORT void TickRadioStationClocks();
    EXPORT void ChooseRadioEmitterForVehicle();
    EXPORT void InitMusicAndCopRadio();
    EXPORT void DeInitVocals();
    EXPORT void UpdateActiveRadioEmitterVolume();
    EXPORT u8 ComputeRadioEmitterVolume(u8 emitterIndex, s32 bMainVariant);
    EXPORT void DeclareRadioStation(s32 station_idx, Fix16 xpos, Fix16 ypos);
    EXPORT void SetRadioEmitterRadii(RadioStationEmitter* pEmitter, s32 type);
    EXPORT void RemoveSound(Fix16 xpos, Fix16 ypos);
    EXPORT void CycleRadioStation(char_type bPrev);
    EXPORT void SelectBestRadioEmitter();
    EXPORT u8 FindEmitterByStation(s32 station_idx) const;
    EXPORT bool IsPoliceOrServiceVehicle(Car* pCar) const;
    EXPORT bool IsTrainOrBoxcar(Car* pCar) const;
};

EXTERN_GLOBAL(sound_obj, gSound_obj_66F680);
