#include "Function.hpp"
#include "root_sound.hpp"
#include "sound_obj.hpp"
#include "cSampleManager.hpp"
#include "debug.hpp"

root_sound gRoot_sound_66B038;

MATCH_FUNC(0x40EF80)
void root_sound::sub_40EF80()
{
    gSound_obj_66F680.sub_419E10();
}

MATCH_FUNC(0x40EFA0)
void root_sound::Service_40EFA0()
{
    gSound_obj_66F680.Service_419EF0();
}

MATCH_FUNC(0x40EFB0)
s32 root_sound::AddSoundObject_40EFB0(infallible_turing* a2)
{
    return gSound_obj_66F680.AddSoundObject_419FA0(a2);
}

MATCH_FUNC(0x40EFD0)
void root_sound::FreeSoundEntry_40EFD0(s32 a2)
{
    gSound_obj_66F680.FreeSoundEntry_41A090(a2);
}

MATCH_FUNC(0x40EFF0)
char_type root_sound::LoadStyle_40EFF0(const char_type *pStyleName)
{
    return gSound_obj_66F680.LoadStyle_41A1B0(pStyleName);
}

// stub
void root_sound::sub_40F010()
{
    // todo
    //gSound_obj_66F680.sub_57E960(); // stub
}

// stub
void root_sound::sub_40F020()
{
    //gSound_obj_66F680.sub_57EA10();
    // todo
}

// stub
char* root_sound::sub_40F030(s32 a1, s32 a2, s32 a3)
{
    //return gSound_obj_66F680.sub_57ECB0(a1, a2, a3);
    // todo
    return 0;
}

// stub
char_type root_sound::sub_40F050(s32 a1, s32 a2)
{
    //return gSound_obj_66F680.sub_57EE30(a1, a2);
    // todo
    return 0;
}

// stub
void root_sound::sub_40F070(char_type a1)
{
    //gSound_obj_66F680.sub_57EEE0(a1);
    // todo
}

// stub
s32 root_sound::sub_40F090(s32 state)
{
    // todo
    //return gSound_obj_66F680.sub_418C80(state);
    return 0;
}

MATCH_FUNC(0x40F0B0)
void root_sound::SetSfxVol_40F0B0(u8 sfxVol)
{
    gSound_obj_66F680.SetSfxVol_41A250(sfxVol > 127 ? 127 : sfxVol);
}

// nomatch
void root_sound::SetCDVol_40F0F0(u32 cdVol)
{
    BYTE v = cdVol;
    if (v > 127)
    {
        v = 127;
        gSound_obj_66F680.SetCDVol_41A270(v);
    }
    else
    {
        gSound_obj_66F680.SetCDVol_41A270(v & 0xFF);
    }
}

MATCH_FUNC(0x40F120)
u8 root_sound::GetCDVol_40F120()
{
    return gSound_obj_66F680.GetCDVol_41A280();
}

MATCH_FUNC(0x40F130)
void root_sound::Release_40F130()
{
    gSound_obj_66F680.Release_41A290();
}

MATCH_FUNC(0x40F140)
void root_sound::sub_40F140()
{
    gSound_obj_66F680.sub_41A2A0();
}

// stub
char_type root_sound::GetAudioDriveLetter_40F150()
{
    // gSound_obj_66F680.GetAudioDriveLetter_41A2E0()
    return gSampManager_6FFF00.field_4_gtaAudioDriveLetter;
}

MATCH_FUNC(0x40EF40)
infallible_turing* root_sound::CreateSoundObject_40EF40(infallible_turing *pObject, s32 objectType)
{
    infallible_turing* pCurrent = field_0;
    field_0 = (infallible_turing*)field_0->field_C_pAny;
    pCurrent->field_C_pAny = pObject;
    pCurrent->field_8 = 0;
    pCurrent->field_4 = 0;
    pCurrent->field_0_object_type = objectType;

    if (!bSkip_audio_67D6BE)
    {
        pCurrent->field_8 = gRoot_sound_66B038.AddSoundObject_40EFB0(pCurrent);
    }

    return pCurrent;
}

MATCH_FUNC(0x40F160)
char_type root_sound::Set3DSound_40F160(char_type b3dSound)
{
    return gSound_obj_66F680.Set3DSound_41A2F0(b3dSound);
}

MATCH_FUNC(0x40F180)
char_type root_sound::Get3DSound_40F180()
{
    return gSound_obj_66F680.Get3dSound_41A390();
}

MATCH_FUNC(0x411E30)
root_sound::root_sound()
{
    infallible_turing* pIter = field_4;
    for (s32 i = 0; i < 999; i++)
    {
        pIter->field_C_pAny = pIter+1;
        pIter++;
    }

    field_0 = field_4;
    field_4[999].field_C_pAny = 0;
}

// nomatch 0x411E60
root_sound::~root_sound()
{
    field_0 = 0;
}
