#pragma once

#include "Function.hpp"
#include "SoundObject_10.hpp"
#include "fix16.hpp"
#include <windows.h>

class root_sound
{
  public:
    SoundObject_10* field_0_pFreeList;
    SoundObject_10 field_4_pool[999 + 1];

    SoundObject_10* DestroySoundObj_40FE60(SoundObject_10* pSoundObj) // inline
    {
        pSoundObj->Release_40EF20();
        SoundObject_10* result = field_0_pFreeList;
        pSoundObj->field_C_pAny.pNextFree = field_0_pFreeList;
        field_0_pFreeList = pSoundObj;
        return result;
    }

    // 9.6f 0x410730
    inline SoundObject_10* PopFree_410730()
    {
        SoundObject_10* pCurrent = field_0_pFreeList;
        field_0_pFreeList = pCurrent->field_C_pAny.pNextFree;
        pCurrent->field_8_sound_entry = 0; // TODO: 9.6f calls SoundObject_10::sub_4106D0 (not dumped)
        return pCurrent;
    }

    EXPORT SoundObject_10* CreateSoundObject_40EF40(void* pObject, s32 objectType);

    EXPORT void Init_40EF80();

    EXPORT void Service_40EFA0();

    EXPORT s32 AddSoundObject_40EFB0(SoundObject_10* pSoundObj);

    EXPORT void FreeSoundEntry_40EFD0(s32 sound_entry);

    EXPORT char_type LoadStyle_40EFF0(const char_type* pStyleName);

    EXPORT void InitMusicAndCopRadio_40F010();

    EXPORT void DeInitVocals_40F020();

    EXPORT void DeclareRadioStation_40F030(s32 station_idx, Fix16 xpos, Fix16 ypos);

    EXPORT void RemoveSound_40F050(Fix16 a1, Fix16 a2);

    EXPORT void CycleRadioStation_40F070(char_type a1);

    EXPORT void PlayVoice_40F090(s32 state);

    EXPORT void SetSfxVol_40F0B0(u8 sfxVol);

    EXPORT void SetCDVol_40F0F0(u8 cdVol);

    EXPORT u8 GetCDVol_40F120();

    EXPORT void Release_40F130();

    EXPORT void Reacquire_40F140();

    EXPORT char_type GetAudioDriveLetter_40F150();

    EXPORT char_type Set3DSound_40F160(char_type b3dSound);

    EXPORT char_type Get3DSound_40F180();

    EXPORT root_sound();

    EXPORT ~root_sound();

    void DeinitializeAudio();
};

EXTERN_GLOBAL(root_sound, gRoot_sound_66B038);