#include "root_sound.hpp"
#include "Function.hpp"
#include "Globals.hpp"
#include "cSampleManager.hpp"
#include "debug.hpp"
#include "sound_obj.hpp"

DEFINE_GLOBAL(root_sound, gRoot_sound, 0x66B038);

MATCH_FUNC(0x40EF40)
SoundObject* root_sound::CreateSoundObject(void* pObject, s32 objectType)
{
    SoundObject* pCurrent = PopFree_410730();
    pCurrent->mpAny.pAny = pObject;
    pCurrent->mbStatus = false;
    pCurrent->mObjectType = objectType;

    if (!bSkip_audio)
    {
        pCurrent->mSoundEntry = gRoot_sound.AddSoundObject(pCurrent);
    }

    return pCurrent;
}

MATCH_FUNC(0x40EF80)
void root_sound::Init()
{
    gSound_obj.Init();
}

MATCH_FUNC(0x40EF90)
void root_sound::DeinitializeAudio()
{
    gSound_obj.DeinitializeAudio();
}

MATCH_FUNC(0x40EFA0)
void root_sound::Service()
{
    gSound_obj.Service();
}

MATCH_FUNC(0x40EFB0)
s32 root_sound::AddSoundObject(SoundObject* pSoundObj)
{
    return gSound_obj.AddSoundObject(pSoundObj);
}

MATCH_FUNC(0x40EFD0)
void root_sound::FreeSoundEntry(s32 sound_entry)
{
    gSound_obj.FreeSoundEntry(sound_entry);
}

MATCH_FUNC(0x40EFF0)
bool root_sound::LoadStyle(const char_type* pStyleName)
{
    return gSound_obj.LoadStyle(pStyleName);
}

MATCH_FUNC(0x40F010)
void root_sound::InitMusicAndCopRadio()
{
    gSound_obj.InitMusicAndCopRadio();
}

MATCH_FUNC(0x40F020)
void root_sound::DeInitVocals()
{
    gSound_obj.DeInitVocals();
}

MATCH_FUNC(0x40F030)
void root_sound::DeclareRadioStation(s32 station_idx, Fix16 xpos, Fix16 ypos)
{
    gSound_obj.DeclareRadioStation(station_idx, xpos, ypos);
}

MATCH_FUNC(0x40F050)
void root_sound::RemoveSound(Fix16 xpos, Fix16 ypos)
{
    gSound_obj.RemoveSound(xpos, ypos);
}

MATCH_FUNC(0x40F070)
void root_sound::CycleRadioStation(char_type direction)
{
    gSound_obj.CycleRadioStation(direction);
}

MATCH_FUNC(0x40F090)
void root_sound::PlayVoice(s32 voice)
{
    gSound_obj.PlayVocal(voice);
}

MATCH_FUNC(0x40F0B0)
void root_sound::SetSfxVol(u8 sfxVol)
{
    gSound_obj.SetSfxVol(sfxVol > 127 ? 127 : sfxVol);
}

MATCH_FUNC(0x40F0F0)
void root_sound::SetCDVol(u8 cdVol)
{
    gSound_obj.SetCDVol(cdVol > 127 ? 127 : cdVol);
}

MATCH_FUNC(0x40F120)
u8 root_sound::GetCDVol()
{
    return gSound_obj.GetCDVol();
}

MATCH_FUNC(0x40F130)
void root_sound::Release()
{
    gSound_obj.Release();
}

MATCH_FUNC(0x40F140)
void root_sound::Reacquire()
{
    gSound_obj.Reacquire();
}

MATCH_FUNC(0x40F150)
char_type root_sound::GetAudioDriveLetter()
{
    return gSound_obj.GetAudioDriveLetter();
}

MATCH_FUNC(0x40F160)
bool root_sound::Set3DSound(char_type b3dSound)
{
    return gSound_obj.Set3DSound(b3dSound);
}

MATCH_FUNC(0x40F180)
char_type root_sound::Get3DSound()
{
    return gSound_obj.Get3dSound();
}

MATCH_FUNC(0x411E30)
root_sound::root_sound()
{
    SoundObject* pIter = mPool;
    for (s32 i = 0; i < 999; i++)
    {
        pIter->mpAny.pNextFree = pIter + 1;
        pIter++;
    }

    mpFreeList = mPool;
    mPool[999].mpAny.pNextFree = NULL;
}

MATCH_FUNC(0x411E60)
root_sound::~root_sound()
{
    mpFreeList = NULL;
}
