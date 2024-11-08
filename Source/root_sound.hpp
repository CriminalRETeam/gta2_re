#pragma once

#include <windows.h>
#include "types.hpp"
#include "infallible_turing.hpp"

class root_sound
{
public:

    infallible_turing *field_0;
    infallible_turing field_4[999+1];
    /*
    s32 field_3E74;
    s32 field_3E78;
    s32 field_3E7C;
    s32 field_3E80_count;
    */
    // todo: ordering

    EXPORT infallible_turing* CreateSoundObject_40EF40(infallible_turing *pObject, s32 objectType);

    EXPORT void sub_40EF80();

    EXPORT void Service_40EFA0();

    EXPORT s32 AddSoundObject_40EFB0(infallible_turing* a2);

    EXPORT void FreeSoundEntry_40EFD0(s32 a2);

    EXPORT char_type LoadStyle_40EFF0(const char_type *pStyleName);

    EXPORT void sub_40F010();

    EXPORT void sub_40F020();

    EXPORT char* sub_40F030(s32 a1, s32 a2, s32 a3);

    EXPORT char_type sub_40F050(s32 a1, s32 a2);

    EXPORT void sub_40F070(char_type a1);

    EXPORT s32 sub_40F090(s32 state);

    EXPORT void SetSfxVol_40F0B0(u8 cdVol);

    EXPORT void SetCDVol_40F0F0(u32 cdVol);

    EXPORT u8 GetCDVol_40F120();

    EXPORT void Release_40F130();

    EXPORT void sub_40F140();

    EXPORT char_type GetAudioDriveLetter_40F150();

    EXPORT char_type Set3DSound_40F160(char_type b3dSound);

    EXPORT char_type Get3DSound_40F180();

    EXPORT root_sound();

    EXPORT ~root_sound();
};


extern root_sound gRoot_sound_66B038;
