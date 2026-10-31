#include "SoundObject.hpp"
#include "Function.hpp"
#include "root_sound.hpp"

MATCH_FUNC(0x40EF20)
void SoundObject::Release()
{
    if (mSoundEntry)
    {
        gRoot_sound.FreeSoundEntry(mSoundEntry);
    }
}