#include "Rozza_C88.hpp"
#include "debug.hpp"
#include "root_sound.hpp"

MATCH_FUNC(0x40bb90)
void Rozza_C88::Reset_40BB90()
{
    field_C84_count = 0;
}

STUB_FUNC(0x40bba0)
void Rozza_C88::sub_40BBA0(Car_3C* a2, s32 a3)
{
}

STUB_FUNC(0x40bc40)
void Rozza_C88::sub_40BC40(Car_3C* a2)
{
}

STUB_FUNC(0x40bd10)
void Rozza_C88::sub_40BD10(Car_3C* a2)
{
}

STUB_FUNC(0x40bdd0)
void Rozza_C88::sub_40BDD0(Car_3C* a2, Car_3C* a3)
{
}

STUB_FUNC(0x40be00)
Rozza_C88::Rozza_C88()
{
    if (bSkip_audio_67D6BE)
    {
        field_0_pSoundObj = NULL;
        Reset_40BB90();
    }
    else
    {
        field_0_pSoundObj = gRoot_sound_66B038.CreateSoundObject_40EF40(field_0_pSoundObj, 6);
        Reset_40BB90();
    }
}

STUB_FUNC(0x40be40)
Rozza_C88::~Rozza_C88()
{
}
