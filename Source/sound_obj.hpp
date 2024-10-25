#pragma once

#include "types.hpp"
#include <windows.h>
#include "root_sound.hpp" // for infallible_turing

class serene_brattain 
{
public:
    EXPORT serene_brattain* sub_419DF0();
    int field_0;
    int field_4;
    int field_8;
};

struct sound_unknown_0xC
{
    int field_0;
    char field_4;
    char field_5;
    char field_6;
    char field_7;
    __int16 field_8;
    char field_A;
    char field_B;
};

struct vigilant_maxwell
{
    char field_0_bUsed;
    char field_1;
    char field_2;
    char field_3;
    infallible_turing* field_4_pObj;
    sound_unknown_0xC* field_8_pAlloc;
};

struct sound_7
{
    char field_0;
    char field_1;
    char field_2;
    char field_3;
    int field_4_fp;
    infallible_turing field_8_fp;
    int field_18;
};

struct sound_0x68
{
    int field_0;
    char field_4;
    char field_5;
    char field_6;
    char field_7;
    serene_brattain field_8_obj;
    int field_14_samp_idx;
    char field_18;
    char field_19;
    char field_1A;
    char field_1B;
    int field_1C_ReleasingVolumeModificator;
    int field_20_rate;
    unsigned __int8 field_24_nVolume;
    char field_25;
    char field_26;
    char field_27;
    int field_28;
    char field_2C;
    char field_2D;
    char field_2E;
    char field_2F;
    int field_30;
    int field_34;
    int field_38;
    int field_3C;
    char field_40_pan;
    char field_41;
    char field_42;
    char field_43;
    int field_44;
    unsigned int field_48_nCalculatedVolume;
    int field_4C;
    char field_50;
    char field_51;
    char field_52;
    char field_53;
    int field_54;
    int field_58_type;
    sound_0x68 *field_5C;
    unsigned __int8 field_60;
    char field_61;
    char field_62;
    char field_63;
    int field_64;
};

struct infallible_turing;

class sound_obj
{
public:
    char field_0;
    char field_1;
    char field_2;
    char field_3;
    int field_4;
    unsigned __int8 field_8;
    char field_9;
    char field_A;
    char field_B;
    int field_C;
    unsigned __int8 field_10_nActiveSamples;
    char field_11;
    char field_12;
    char field_13;
    int field_14_sample_rate;
    char field_18;
    char field_19;
    char field_1A;
    char field_1B;
    BYTE field_1C_samp_count;
    char field_1D_b3d_sound;
    char field_1E;
    char field_1F;
    int field_20;
    unsigned char field_24_sfx_vol;
    unsigned __int8 field_25_cdVol;
    char field_26;
    char field_27;
    int field_28;
    char field_2C;
    char field_2D;
    char field_2E;
    char field_2F;
    sound_0x68 field_30_sQueueSample;
    unsigned __int8 field_98_nActiveSampleQueue;
    char field_99_pad;
    char field_9A_pad;
    char field_9B_pad;
    sound_0x68 field_9C_asSamples[2][16];
    BYTE field_D9C_abSampleQueueIndexTable[2][16];
    BYTE field_DBC_SampleRequestQueuesStatus[2];
    __int16 field_DBE;
    sound_0x68 field_DC0[16];
    int field_1440;
    char field_1444;
    char field_1445;
    char field_1446;
    char field_1447;
    int field_1448;
    int field_144C;
    char field_1450;
    char field_1451;
    char field_1452;
    char field_1453;
    int field_1454_anRandomTable[5];
    int field_1468_v1;
    int field_146C_v2;
    int field_1470_v3;
    __int16 field_1474;
    char field_1476;
    char field_1477;
    int field_1478_type5Idx;
    vigilant_maxwell field_147C[1020];
    unsigned int field_444C_pEntities[1020];
    unsigned int field_543C_444C_max_idx;
    int field_5440;
    int field_5444;
    unsigned int field_5448_m_FrameCounter;
    sound_7 field_544C[5];
    char field_54D8[5];
    char field_54DD;
    char field_54DE;
    char field_54DF;
    char field_54E0;
    char field_54E1;
    char field_54E2;
    char field_54E3;
    char field_54E4;
    char field_54E5;
    char field_54E6;
    char field_54E7;
    __int16 field_54E8[5];
    unsigned __int8 field_54F2[5];
    char field_54F7[5];
    int field_54FC;
    int field_5500;
    char field_5504;
    char field_5505;
    char field_5506;
    char field_5507;
    int field_5508;
    int field_550C;
    char field_5510;
    char field_5511;
    char field_5512;
    char field_5513;
    int field_5514;
    int field_5518;
    int field_551C;
    unsigned char field_5520;
    char field_5521;
    char field_5522;
    char field_5523;
    int field_5524;
    unsigned __int8 field_5528_idx15_cur;
    unsigned __int8 field_5529_idx15;
    char field_552A_pad;
    char field_552B_pad;
    int field_552C_15array[15];
    char field_5568;
    char field_5569;
    __int16 field_556A;
    __int16 field_556C;
    __int16 field_556E;
    __int16 field_5570;
    __int16 field_5572;
    int field_5574;

    EXPORT sound_obj();

    EXPORT void ClearRequestedQueue_41B700();

    EXPORT void ClearActivateSamples_41B7A0();

    EXPORT void sub_41A2A0();

    EXPORT void SetSfxVol_41A250(char sfxVol);

    EXPORT void SetCDVol_41A270(unsigned char cdVol);

    EXPORT char Set3DSound_41A2F0(char b3dSound);

    EXPORT char Get3dSound_41A390();

    EXPORT void Init_15_Array_427180();

    EXPORT void Set15Val_4271B0(unsigned int val);

    EXPORT void sub_427220();

    EXPORT void GenerateIntegerRandomNumberTable_41BA90();

    EXPORT void sub_41B540();

    EXPORT void sub_41B520(int a1, float *a2);

    EXPORT char sub_41B660(unsigned __int8 a1, unsigned int a2, unsigned int a3);

    EXPORT void AddDetailsToRequestedOrderList_41A910(unsigned __int8 a2);

    EXPORT void AddSampleToRequestedQueue_41A850();

    EXPORT int RandomDisplacement_41A650(unsigned int seed);

    EXPORT void ServiceSoundEffects_41A3A0();

    EXPORT void InterrogateAudioEntities_41A730();

    EXPORT void AddReleasingSounds_41A9D0();

    EXPORT void ProcessActiveQueues_41AB80();

    EXPORT void sub_41A6F0();

    EXPORT char CalcVolume_41A3F0(unsigned __int8 a1, int a2, int a3);

    EXPORT  bool VolCalc_419070(int a2, int a3, char a4);

    EXPORT char sub_419020(int a2);

    EXPORT char sub_4186D0(sound_0x68 *pObj);

    EXPORT char sub_4182A0(sound_0x68 *pObj);

    EXPORT char sub_418130(sound_0x68 *pObj);

    EXPORT char sub_417F40(int a1);

    EXPORT int sub_417EF0(sound_0x68 *pObj);

    EXPORT int ConvertToPlayBackRate_417C60(int a1);

    EXPORT char sub_4153F0(sound_0x68 *pObj);

    EXPORT void ResetEntry_41A6C0(int idx);

    EXPORT void null_412240();

    EXPORT void sub_418C20();

    EXPORT void sub_419E10();

    EXPORT char LoadStyle_41A1B0(const char *pStyleName);

    EXPORT const char* GetFileName_41A1E0(const char *pStr);

    EXPORT int AddSoundObject_419FA0(infallible_turing* a2);

    EXPORT void FreeSoundEntry_41A090(unsigned int a2);

    EXPORT void Release_41A290();

    EXPORT unsigned __int8 GetCDVol_41A280();

    EXPORT void sub_57EA10();
    EXPORT void Service_419EF0();

    EXPORT void ProcessEntity_4123A0(int id);

    EXPORT void ProcessType3_57DD50();

    EXPORT void ProcessType1_412740(int a2);

    EXPORT void ProcessType6_413760(int a2);

    EXPORT void ProcessType7_42A500(int a2);

    EXPORT void ProcessType8_412820(int a2);

    EXPORT void ProcessType9_412A60(int a2);

    EXPORT void ProcessType10_418CA0();

    EXPORT void ProcessType11_418B60(int a2);

    EXPORT void ProcessType2_412490(int idx);

    EXPORT char sub_412260(sound_0x68 *pObj);

    EXPORT char sub_416260(sound_0x68 *a1);

    EXPORT char sub_4174C0(sound_0x68 *a2);

    EXPORT char sub_417A00(sound_0x68 *a2);

    EXPORT char sub_415730(sound_0x68 *a2);

    EXPORT char sub_418940(sound_0x68 *a2);

    EXPORT char sub_414EE0(sound_0x68 *a2);
 
    EXPORT char sub_414C90(sound_0x68 *a2);

    EXPORT char sub_415100(sound_0x68 *a2);

    EXPORT char sub_414320(sound_0x68 *a2);

    EXPORT char sub_414690(sound_0x68 *a2);

    EXPORT char sub_4149D0(sound_0x68 *a2);

    EXPORT void VecDiff_41B4E0(serene_brattain *pVec, serene_brattain *pRet);

    EXPORT void sub_41B490(sound_0x68 *pObj);

    EXPORT DWORD* sub_4190B0(DWORD *a2);

    EXPORT BYTE sub_427310();

    EXPORT void sub_427340(int a4, int a5, int a6);

    EXPORT void sub_426E10(int arg0, unsigned int a2);
};

extern sound_obj gSound_obj_66F680;
