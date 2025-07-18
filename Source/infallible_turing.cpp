#include "infallible_turing.hpp"
#include "Function.hpp"
#include "root_sound.hpp"

MATCH_FUNC(0x40EF20)
void infallible_turing::release_40EF20()
{
    if (field_8)
    {
        gRoot_sound_66B038.FreeSoundEntry_40EFD0(field_8);
    }
}