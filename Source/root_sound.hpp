#pragma once

#include "Function.hpp"
#include "SoundObject.hpp"
#include "fix16.hpp"
#include <windows.h>

class root_sound
{
  public:
    SoundObject* mpFreeList; // 0x0
    SoundObject mPool[999 + 1]; // 0x4

    // 9.6f 0x40FE60
    SoundObject* DestroySoundObj_40FE60(SoundObject* pSoundObj) // inline
    {
        pSoundObj->Release();
        SoundObject* result = mpFreeList;
        pSoundObj->mpAny.pNextFree = mpFreeList;
        mpFreeList = pSoundObj;
        return result;
    }

    // 9.6f 0x410730
    inline SoundObject* PopFree_410730()
    {
        SoundObject* pCurrent = mpFreeList;
        mpFreeList = pCurrent->mpAny.pNextFree;
        // 9.6f calls a SoundObject reset here (0x4106D0: bStatus true, entry and type 0); as a new inline
        // name it changes ImGuiDebugDraw and the DLLs
        pCurrent->mSoundEntry = 0;
        return pCurrent;
    }

    EXPORT SoundObject* CreateSoundObject(void* pObject, s32 objectType);

    EXPORT void Init();

    EXPORT void DeinitializeAudio();

    EXPORT void Service();

    EXPORT s32 AddSoundObject(SoundObject* pSoundObj);

    EXPORT void FreeSoundEntry(s32 sound_entry);

    EXPORT bool LoadStyle(const char_type* pStyleName);

    EXPORT void InitMusicAndCopRadio();

    EXPORT void DeInitVocals();

    EXPORT void DeclareRadioStation(s32 station_idx, Fix16 xpos, Fix16 ypos);

    EXPORT void RemoveSound(Fix16 xpos, Fix16 ypos);

    EXPORT void CycleRadioStation(char_type direction);

    EXPORT void PlayVoice(s32 voice);

    EXPORT void SetSfxVol(u8 sfxVol);

    EXPORT void SetCDVol(u8 cdVol);

    EXPORT u8 GetCDVol();

    EXPORT void Release();

    EXPORT void Reacquire();

    EXPORT char_type GetAudioDriveLetter();

    EXPORT bool Set3DSound(char_type b3dSound);

    EXPORT char_type Get3DSound();

    EXPORT root_sound();

    EXPORT ~root_sound();
};

EXTERN_GLOBAL(root_sound, gRoot_sound);