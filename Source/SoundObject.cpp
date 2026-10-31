#include "SoundObject.hpp"
#include "Function.hpp"
#include "root_sound.hpp"

MATCH_FUNC(0x40EF20)
void SoundObject::Release_40EF20()
{
    if (mSoundEntry)
    {
        gRoot_sound_66B038.FreeSoundEntry_40EFD0(mSoundEntry);
    }
}