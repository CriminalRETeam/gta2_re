#pragma once

#include "Function.hpp"
#include "ang16.hpp"
#include "fix16.hpp"
#include "root_sound.hpp" // for infallible_turing
#include <windows.h>

class Car_BC;
class Object_2C;
class gmp_map_zone;
class Rozza_A;
class infallible_turing;

class serene_brattain
{
  public:
    // Compiler-generated constructor at 0x419DF0
    Fix16 field_0;
    Fix16 field_4;
    Fix16 field_8;
};

struct sound_unknown_0xC
{
    Fix16 field_0;
    u32 field_4;
    //char_type field_4;
    //char_type field_5;
    //char_type field_6;
    //char_type field_7;
    s16 field_8;
    char_type field_A;
    char_type field_B;
};

struct vigilant_maxwell
{
    char_type field_0_bUsed;
    char_type field_1;
    char_type field_2;
    char_type field_3;
    infallible_turing* field_4_pObj;
    sound_unknown_0xC* field_8_pAlloc;
};

struct sound_7
{
    char_type field_0;
    char_type field_1;
    char_type field_2;
    char_type field_3;
    s32 field_4_fp;
    infallible_turing field_8;
    s32 field_18;
};

struct sound_0x68
{
    s32 field_0_EntityIndex;
    char_type field_4_SampleIndex;
    char_type field_5;
    char_type field_6;
    char_type field_7;
    serene_brattain field_8_obj;
    s32 field_14_samp_idx;
    char_type field_18;
    char_type field_19;
    char_type field_1A;
    char_type field_1B;
    u32 field_1C_ReleasingVolumeModificator;
    s32 field_20_rate;
    u8 field_24_nVolume;
    char_type field_25;
    char_type field_26;
    char_type field_27;
    Fix16 field_28_distance;
    char_type field_2C_bIsBeingPlayed;
    char_type field_2D_bIsPlayingFinished;
    char_type field_2E;
    char_type field_2F;
    s32 field_30;
    s32 field_34;
    s32 field_38;
    s32 field_3C;
    char_type field_40_pan;
    char_type field_41;
    char_type field_42;
    char_type field_43;
    s32 field_44;
    u32 field_48_nCalculatedVolume;
    u32 field_4C;
    u8 field_50;
    char_type field_51;
    char_type field_52;
    char_type field_53;
    Fix16 field_54;
    s32 field_58_type;
    sound_0x68* field_5C;
    u8 field_60_nEmittingVolume;
    char_type field_61;
    char_type field_62;
    char_type field_63;
    s32 field_64_max_distance;
};

struct Sound_Params_8
{
    Sprite* field_0_pObj;
    char_type field_4_bDrivenByPlayer;
    char_type field_5_bHasSolidAbove;
    // pad?
    char_type field_6;
    char_type field_7;
};

class sound_obj
{
  public:
    char_type field_0_bSoundInitialized;
    char_type field_1_isPaused;
    char_type field_2_service_enabled;
    char_type field_3;
    s32 field_4;
    u8 field_8;
    char_type field_9;
    char_type field_A;
    char_type field_B;
    s32 field_C;
    u8 field_10_nActiveSamples;
    char_type field_11;
    char_type field_12;
    char_type field_13;
    s32 field_14_sample_rate;
    char_type field_18;
    char_type field_19;
    char_type field_1A_bDoubleVolume;
    char_type field_1B;
    u8 field_1C_samp_count;
    char_type field_1D_b3d_sound;
    char_type field_1E;
    char_type field_1F;
    s32 field_20;
    u8 field_24_sfx_vol;
    u8 field_25_cdVol;
    char_type field_26;
    char_type field_27;
    Fix16 field_28_dist_related;
    char_type field_2C_distCalculated;
    char_type field_2D;
    char_type field_2E;
    char_type field_2F;
    sound_0x68 field_30_sQueueSample;
    u8 field_98_nActiveSampleQueue;
    char_type field_99_pad;
    char_type field_9A_pad;
    char_type field_9B_pad;
    sound_0x68 field_9C_asSamples[2][16];
    u8 field_D9C_abSampleQueueIndexTable[2][16];
    u8 field_DBC_SampleRequestQueuesStatus[2];
    s16 field_DBE;
    sound_0x68 field_DC0_ActiveSamples[16];
    s32 field_1440;
    char_type field_1444;
    char_type field_1445;
    char_type field_1446;
    char_type field_1447;
    s32 field_1448;
    s32 field_144C;
    char_type field_1450;
    char_type field_1451;
    char_type field_1452;
    char_type field_1453;
    s32 field_1454_anRandomTable[5];
    Fix16 field_1468_v1;
    Fix16 field_146C_v2;
    Fix16 field_1470_v3;
    Ang16 field_1474;
    char_type field_1476;
    char_type field_1477;
    s32 field_1478_type5Idx;
    vigilant_maxwell field_147C[1020];
    u32 field_444C_AudioEntityOrderList[1020];
    u32 field_543C_444C_nAudioEntitiesCount;
    s32 field_5440;
    s32 field_5444;
    u32 field_5448_m_FrameCounter;
    sound_7 field_544C[5];
    char_type field_54D8[5];
    char_type field_54DD;
    char_type field_54DE;
    char_type field_54DF;
    char_type field_54E0;
    char_type field_54E1;
    char_type field_54E2;
    char_type field_54E3;
    char_type field_54E4;
    char_type field_54E5;
    char_type field_54E6;
    char_type field_54E7;
    s16 field_54E8[5];
    u8 field_54F2[5];
    char_type field_54F7[5];
    s32 field_54FC;
    s32 field_5500;
    char_type field_5504_radio_station_change_mode;
    char_type field_5505;
    char_type field_5506;
    char_type field_5507;
    s32 field_5508;
    s32 field_550C;
    char_type field_5510;
    char_type field_5511;
    char_type field_5512;
    char_type field_5513;
    s32 field_5514;
    s32 field_5518;
    s32 field_551C;
    u8 field_5520_bCanPlay;
    u8 field_5521_radio_word;
    s16 field_5522_wanted_star_count;
    s32 field_5524_radio_word;
    u8 field_5528_idx15_cur;
    u8 field_5529_idx15;
    char_type field_552A_pad;
    char_type field_552B_pad;
    s32 field_552C_15array[15];
    char_type field_5568_crime_xpos;
    char_type field_5569_crime_ypos;
    s16 field_556A_erv_en_route_timer;
    s16 field_556C_timer;
    s16 field_556E_timer;
    s16 field_5570_timer;
    s16 field_5572; // pad?
    s32 field_5574_car_info_idx;

    EXPORT sound_obj();

    // TODO: Ordering
    EXPORT void sub_41A2A0();
    EXPORT char_type GetAudioDriveLetter_41A2E0();
    EXPORT char_type Get3dSound_41A390();
    EXPORT void Service_419EF0();
    EXPORT void Release_41A290();
    EXPORT u8 GetCDVol_41A280();
    EXPORT void sub_419E10();

    EXPORT void null_412240();
    EXPORT void null_412250();
    EXPORT char_type sub_412260(sound_0x68* a1);
    EXPORT void ProcessEntity_4123A0(s32 idx);
    EXPORT void ProcessType2_412490(s32 idx);
    EXPORT void ProcessType1_Sprite_412740(s32 idx);
    EXPORT void ProcessType8_Crane_412820(s32 idx);
    EXPORT void ProcessType9_Crusher_412A60(s32 idx);
    EXPORT void ProcessCar_412B80(Sound_Params_8* pType2Entity);
    EXPORT char_type Type6_412C90(Rozza_A* pObj, u8 a3);
    EXPORT char_type SelectObjectImpactSound_1_10_412D30(Rozza_A* pObj);
    EXPORT char_type Type6_2_412D40(u8 a2);
    EXPORT void Type6_Play_412D90(s32 model);
    EXPORT char_type Type6_3_413000(Rozza_A* a2);
    EXPORT char_type Type6_4_413040(u8 a2);
    EXPORT char_type Type6_5_413090(u8 a2);
    EXPORT char_type Type6_7_4130E0(Rozza_A* a2);
    EXPORT char_type SelectObjectImpactSound_413120(Rozza_A* pObj, s32 interactionType);
    EXPORT char_type Type6_9_413540(Rozza_A* a2);
    EXPORT char_type Type6_12_4136D0(Rozza_A* a2);
    EXPORT void ProcessType6_Rozza_C88_413760(s32 idx);
    EXPORT char_type Type6_413A10(Rozza_A* a2);
    EXPORT void ProcessTrainCab_413B90(Sound_Params_8* a2, sound_unknown_0xC* a3);
    EXPORT void ProcessTrain_413BE0(Sound_Params_8* a2);
    EXPORT void ProcessTank_413BF0(Sound_Params_8* a2, sound_unknown_0xC* pAlloc);
    EXPORT void ProcessOtherCarTypes_413C50(Sound_Params_8* a2, sound_unknown_0xC* pAlloc);
    EXPORT void HandleAICarHornBeep_413D10(Sound_Params_8* a2);
    EXPORT void HandleTrainEngineSound_4140C0(Sound_Params_8* a2);

    EXPORT char_type Type_16_414320(sound_0x68* pObj);
    EXPORT void HandleTrainCabRollingFrictionSound_4143A0(Sound_Params_8* a2);
    EXPORT bool sub_4145E0(s32 a2, s32* a3);
    EXPORT char_type Type_17_414690(sound_0x68* pObj);
    EXPORT void TrainCab_414710(Sound_Params_8* a2);
    EXPORT char_type Type_19_4149D0(sound_0x68* a1);
    EXPORT void Tank_414A50(Sound_Params_8* a2);
    EXPORT char_type Type_12_414C90(sound_0x68* p68);
    EXPORT void Tank_414D30(Sound_Params_8* a2);
    EXPORT char_type Type_11_414EE0(sound_0x68* p68);
    EXPORT void HandleCarBurningSound_414F90(Sound_Params_8* a2);
    EXPORT char_type Type_15_415100(sound_0x68* a1);
    EXPORT void Tank_415190(Sound_Params_8* a2);
    EXPORT char_type Type_13_4153F0(sound_0x68* a1);
    EXPORT void HandleCarWeaponHitSound_415480(Sound_Params_8* a2);
    EXPORT void HandleCarAlarmSound_415570(Sound_Params_8* a2, sound_unknown_0xC* pAlloc);
    EXPORT char_type Type_5_InitEngineSoundProfile_415730(sound_0x68* a1);
    EXPORT void HandleAICarEngineRevSound_4157C0(Sound_Params_8* a2);
    EXPORT char_type Type_1_6_416260(sound_0x68* a2);
    EXPORT void HandleCarHornSound_417060(Sound_Params_8* a2);
    EXPORT char_type Type_3_HandleCarImpactSound_4174C0(sound_0x68* a2);
    EXPORT void HandleCarDamageSound_4177D0(Sound_Params_8* a2);
    EXPORT void HandleSirenActivationSound_4178C0(Sound_Params_8* a2);
    EXPORT char_type Type_4_417A00(sound_0x68* a2);
    EXPORT s32 GetVehicleAudioClass_417BA0(s32 a1);
    EXPORT s32 ConvertToPlayBackRate_417C60(s32 a1);
    EXPORT void HandleHeavyVehicleStopSound_417E30(Sound_Params_8* a2, sound_unknown_0xC* a3);
    EXPORT s32 Type_7_417EF0(sound_0x68* a2);
    EXPORT char_type IsHeavyTruckOrBus_417F40(s32 a1);
    EXPORT void HandleTruckCorneringAudio_417FD0(Sound_Params_8* a2);
    EXPORT char_type Type_8_418130(sound_0x68* a1);
    EXPORT void HandleAICarEngineSound_418190(Sound_Params_8* a2);
    EXPORT char_type Type_2_4182A0(sound_0x68* a1);
    EXPORT void HandleCarDoorSounds_4182E0(Sound_Params_8* a2);
    EXPORT void UpdateCarSurfaceAudio_418610(Sound_Params_8* a2);
    EXPORT char_type Type_9_4186D0(sound_0x68* a2);
    EXPORT void HandleCarTireScrubSound_418720(Sound_Params_8* a2);
    EXPORT char_type Type_10_HandleCarSkidSound_418940(sound_0x68* a2);
    EXPORT void ProcessType11_HudPager_418B60(s32 a2);
    EXPORT void sub_418C20();
    EXPORT void sub_418C60();
    EXPORT void sub_418C80(s32 a2);
    EXPORT void ProcessType10_Vocals_418CA0();
    EXPORT char_type CalculateDistance_419020(Fix16 new_dist);
    EXPORT bool VolCalc_419070(s32 a2, Fix16 a3, char_type a4);
    EXPORT Fix16 ComputeEmitterDistanceSquared_4190B0();
    EXPORT s32 AddSoundObject_419FA0(infallible_turing* a2);

    EXPORT void FreeSoundEntry_41A090(u32 a2);
    EXPORT char_type LoadStyle_41A1B0(const char_type* pStyleName);
    EXPORT const char_type* GetFileName_41A1E0(const char_type* Str);
    EXPORT void SetSfxVol_41A250(u8 sfxVol);
    EXPORT void SetCDVol_41A270(u8 cdVol);
    EXPORT char_type Set3DSound_41A2F0(char_type a2);
    EXPORT void ServiceSoundEffects_41A3A0();
    EXPORT char_type CalcVolume_41A3F0(u8 a1, Fix16 a2, Fix16 a3);
    EXPORT u8 sub_41A4A0(Fix16 a1, Fix16 a2);
    EXPORT s32 AdjustPlaybackRate_41A580(s32 snd_rate, Fix16 xpos, Fix16 ypos, Fix16 zpos);
    EXPORT s32 RandomDisplacement_41A650(u32 seed);
    EXPORT void ResetEntry_41A6C0(s32 idx);
    EXPORT void sub_41A6F0();
    EXPORT void InterrogateAudioEntities_41A730();
    EXPORT void AddSampleToRequestedQueue_41A850();
    EXPORT void AddDetailsToRequestedOrderList_41A910(u8 a2);
    EXPORT void AddReleasingSounds_41A9D0();
    EXPORT void ProcessActiveQueues_41AB80();

    EXPORT void sub_41B490(sound_0x68* pObj);
    EXPORT void VecDiff_41B4E0(serene_brattain* pVec, serene_brattain* pRet);
    EXPORT void FromFix16_41B520(Fix16 fixVal, f32* s32Val);
    EXPORT void AdjustSamplesVolume_41B540();
    EXPORT char_type ComputeEmittingVolume_41B660(u8 emittingVolume, u32 maxDistance, u32 distance);
    EXPORT void ClearRequestedQueue_41B700();
    EXPORT void ClearActivateSamples_41B7A0();
    EXPORT void GenerateIntegerRandomNumberTable_41BA90();

    EXPORT void ProcessObject_41E820(Sound_Params_8* pEntity);
    EXPORT void ProcessObject_Type12_41E850(Sound_Params_8* a2);
    EXPORT void ProcessOtherObjects_41F520(Sound_Params_8* a2);
    EXPORT void UpdateRadioChatterLoop_41FCA0();

    EXPORT void ProcessPed_422B70(Sound_Params_8* pType3Entity);
    EXPORT void HandlePedVoiceEvent_423080(Sound_Params_8* a2);

    EXPORT void ResetRadioMessageState_426750();
    EXPORT void PoliceRadioMessageGeneration_426790();
    EXPORT void EnqueueRadioLocationPhrase_426E10(u8 xpos, u8 ypos);
    EXPORT void GenerateRadioVehicleDescription_426F20(Car_BC* a2);

    EXPORT void Init_RadioMessageQueue_427180();
    EXPORT void EnqueueRadioWord_4271B0(u32 a2);
    EXPORT void ProcessPoliceRadioWordsPlayback_427220();
    EXPORT u8 GetQueuedRadioWordCount_427310();
    EXPORT void EnqueueRadioCrimeCallout_427340(s32 word_base, u8 xpos, u8 ypos);
    EXPORT void AppendRadioMessageSuffix_4273B0();
    EXPORT u32 GetCopRadioZoneIndex_427400(u8 x, u8 y, gmp_map_zone** ppZone);

    EXPORT void ProcessType7_Weapon_42A500(s32 idx);

    EXPORT void ProcessType3_CopRadioAndMusic_57DD50();
    EXPORT void HandleVocalStreamSwitching_57DF10(char_type a2);
    EXPORT void UpdateCarEngineAudio_57E220();
    EXPORT u32 UpdateVocalStream_57E510();
    EXPORT void Type3_CopRadioReport_57E680();
    EXPORT char_type ChooseRadioEmitterForVehicle_57E6C0();
    EXPORT void DeInitVocals_57EA10();
    EXPORT void UpdateActiveRadioEmitterVolume_57EA90();
    EXPORT char_type ComputeRadioEmitterVolume_57EB90(s32 emitterIndex, s32 bUseFarRadius);
    EXPORT void DeclareRadioStation_57ECB0(s32 station_idx, Fix16 xpos, Fix16 ypos);
    EXPORT void sub_57EDB0(s32 a1, s32 a2);
    EXPORT void RemoveSound_57EE30(s32 a2, s32 a3);
    EXPORT void CycleRadioStation_57EEE0(char_type bPrev);
    EXPORT void SelectBestRadioEmitter_57EF60();
    EXPORT u8 FindEmitterByStatus_57F050(s32 status);
    EXPORT bool IsPoliceOrServiceVehicle_57F090(Car_BC* pCar);
    EXPORT bool IsTrainOrBoxcar_57F120(Car_BC* pCar);
};

EXTERN_GLOBAL(sound_obj, gSound_obj_66F680);
