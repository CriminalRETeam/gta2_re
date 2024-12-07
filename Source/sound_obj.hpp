#pragma once

#include "Function.hpp"
#include "root_sound.hpp" // for infallible_turing
#include <windows.h>

class infallible_turing;

class serene_brattain
{
  public:
    EXPORT serene_brattain* sub_419DF0();
    s32 field_0;
    s32 field_4;
    s32 field_8;
};

struct sound_unknown_0xC
{
    s32 field_0;
    char_type field_4;
    char_type field_5;
    char_type field_6;
    char_type field_7;
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
    s32 field_0;
    char_type field_4;
    char_type field_5;
    char_type field_6;
    char_type field_7;
    serene_brattain field_8_obj;
    s32 field_14_samp_idx;
    char_type field_18;
    char_type field_19;
    char_type field_1A;
    char_type field_1B;
    s32 field_1C_ReleasingVolumeModificator;
    s32 field_20_rate;
    u8 field_24_nVolume;
    char_type field_25;
    char_type field_26;
    char_type field_27;
    s32 field_28;
    char_type field_2C;
    char_type field_2D;
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
    s32 field_4C;
    char_type field_50;
    char_type field_51;
    char_type field_52;
    char_type field_53;
    s32 field_54;
    s32 field_58_type;
    sound_0x68* field_5C;
    u8 field_60;
    char_type field_61;
    char_type field_62;
    char_type field_63;
    s32 field_64;
};

class sound_obj
{
  public:
    char_type field_0;
    char_type field_1;
    char_type field_2;
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
    char_type field_1A;
    char_type field_1B;
    BYTE field_1C_samp_count;
    char_type field_1D_b3d_sound;
    char_type field_1E;
    char_type field_1F;
    s32 field_20;
    u8 field_24_sfx_vol;
    u8 field_25_cdVol;
    char_type field_26;
    char_type field_27;
    s32 field_28;
    char_type field_2C;
    char_type field_2D;
    char_type field_2E;
    char_type field_2F;
    sound_0x68 field_30_sQueueSample;
    u8 field_98_nActiveSampleQueue;
    char_type field_99_pad;
    char_type field_9A_pad;
    char_type field_9B_pad;
    sound_0x68 field_9C_asSamples[2][16];
    BYTE field_D9C_abSampleQueueIndexTable[2][16];
    BYTE field_DBC_SampleRequestQueuesStatus[2];
    s16 field_DBE;
    sound_0x68 field_DC0[16];
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
    s32 field_1468_v1;
    s32 field_146C_v2;
    s32 field_1470_v3;
    s16 field_1474;
    char_type field_1476;
    char_type field_1477;
    s32 field_1478_type5Idx;
    vigilant_maxwell field_147C[1020];
    u32 field_444C_pEntities[1020];
    u32 field_543C_444C_max_idx;
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
    char_type field_5504;
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
    u8 field_5520;
    char_type field_5521;
    char_type field_5522;
    char_type field_5523;
    s32 field_5524;
    u8 field_5528_idx15_cur;
    u8 field_5529_idx15;
    char_type field_552A_pad;
    char_type field_552B_pad;
    s32 field_552C_15array[15];
    char_type field_5568;
    char_type field_5569;
    s16 field_556A;
    s16 field_556C;
    s16 field_556E;
    s16 field_5570;
    s16 field_5572;
    s32 field_5574;

    EXPORT sound_obj();

    EXPORT void ClearRequestedQueue_41B700();

    EXPORT void ClearActivateSamples_41B7A0();

    EXPORT void sub_41A2A0();

    EXPORT void SetSfxVol_41A250(char_type sfxVol);

    EXPORT void SetCDVol_41A270(u8 cdVol);

    EXPORT char_type GetAudioDriveLetter_41A2E0();

    EXPORT char_type Set3DSound_41A2F0(char_type b3dSound);

    EXPORT char_type Get3dSound_41A390();

    EXPORT void Init_15_Array_427180();

    EXPORT void Set15Val_4271B0(u32 val);

    EXPORT void sub_427220();

    EXPORT void GenerateIntegerRandomNumberTable_41BA90();

    EXPORT void sub_41B540();

    EXPORT void sub_41B520(s32 a1, f32* a2);

    EXPORT char_type ComputeEmittingVolume_41B660(u8 emittingVolume, u32 maxDistance, u32 distance);

    EXPORT void AddDetailsToRequestedOrderList_41A910(u8 a2);

    EXPORT void AddSampleToRequestedQueue_41A850();

    EXPORT s32 RandomDisplacement_41A650(u32 seed);

    EXPORT void ServiceSoundEffects_41A3A0();

    EXPORT void InterrogateAudioEntities_41A730();

    EXPORT void AddReleasingSounds_41A9D0();

    EXPORT void ProcessActiveQueues_41AB80();

    EXPORT void sub_41A6F0();

    EXPORT char_type CalcVolume_41A3F0(u8 a1, s32 a2, s32 a3);

    EXPORT bool VolCalc_419070(s32 a2, s32 a3, char_type a4);

    EXPORT char_type sub_419020(s32 a2);

    EXPORT char_type sub_4186D0(sound_0x68* pObj);

    EXPORT char_type sub_4182A0(sound_0x68* pObj);

    EXPORT char_type sub_418130(sound_0x68* pObj);

    EXPORT char_type sub_417F40(s32 a1);

    EXPORT s32 sub_417EF0(sound_0x68* pObj);

    EXPORT s32 ConvertToPlayBackRate_417C60(s32 a1);

    EXPORT char_type sub_4153F0(sound_0x68* pObj);

    EXPORT void ResetEntry_41A6C0(s32 idx);

    EXPORT void null_412240();

    EXPORT void sub_418C20();

    EXPORT void sub_418C60();

    EXPORT void sub_419E10();

    EXPORT char_type LoadStyle_41A1B0(const char_type* pStyleName);

    EXPORT const char_type* GetFileName_41A1E0(const char_type* pStr);

    EXPORT s32 AddSoundObject_419FA0(infallible_turing* a2);

    EXPORT void FreeSoundEntry_41A090(u32 a2);

    EXPORT void Release_41A290();

    EXPORT u8 GetCDVol_41A280();

    EXPORT void sub_57EA10();
    EXPORT void Service_419EF0();

    EXPORT void ProcessEntity_4123A0(s32 id);

    EXPORT void ProcessType3_57DD50();

    EXPORT void ProcessType1_412740(s32 a2);

    EXPORT void ProcessType6_413760(s32 a2);

    EXPORT void ProcessType7_42A500(s32 a2);

    EXPORT void ProcessType8_412820(s32 a2);

    EXPORT void ProcessType9_412A60(s32 a2);

    EXPORT void sub_418C80(s32 a2);

    EXPORT void ProcessType10_418CA0();

    EXPORT void ProcessType11_418B60(s32 a2);

    EXPORT void ProcessType2_412490(s32 idx);

    EXPORT char_type sub_412260(sound_0x68* pObj);

    EXPORT char_type sub_416260(sound_0x68* a1);

    EXPORT char_type sub_4174C0(sound_0x68* a2);

    EXPORT char_type sub_417A00(sound_0x68* a2);

    EXPORT char_type sub_415730(sound_0x68* a2);

    EXPORT char_type sub_418940(sound_0x68* a2);

    EXPORT char_type sub_414EE0(sound_0x68* a2);

    EXPORT char_type sub_414C90(sound_0x68* a2);

    EXPORT char_type sub_415100(sound_0x68* a2);

    EXPORT char_type sub_414320(sound_0x68* a2);

    EXPORT char_type sub_414690(sound_0x68* a2);

    EXPORT char_type sub_4149D0(sound_0x68* a2);

    EXPORT void VecDiff_41B4E0(serene_brattain* pVec, serene_brattain* pRet);

    EXPORT void sub_41B490(sound_0x68* pObj);

    EXPORT u32* sub_4190B0(u32* a2);

    EXPORT BYTE sub_427310();

    EXPORT void sub_427340(s32 a4, s32 a5, s32 a6);

    EXPORT void sub_426E10(s32 arg0, u32 a2);
};

EXPORT_VAR extern sound_obj gSound_obj_66F680;
