#include "Weapon_30.hpp"
#include "root_sound.hpp"
#include "debug.hpp"

MATCH_FUNC(0x5DCD10)
Weapon_30::Weapon_30()
{
    field_0 = 0;
    field_24_pObj = 0;
    field_14 = 0;
    field_2 = 0;
    field_4 = 0;
    field_18 = 0;
    field_1C_idx = 0;
    field_10 = 0;
    field_8 = 0;
    field_C = -1;
    field_20 = 0;
    field_21 = 0;
    field_2C = 0;
    field_28 = 0;
}

MATCH_FUNC(0x5DCD50)
Weapon_30::~Weapon_30()
{
    field_24_pObj = 0;
    field_18 = 0;
    field_14 = 0;
    field_8 = 0;
    if (field_28)
    {
        gRoot_sound_66B038.DestroySoundObj_40FE60(field_28);
        field_28 = 0;
    }
}

MATCH_FUNC(0x5DCD90)
void Weapon_30::sub_5DCD90()
{
    field_24_pObj = 0;
    field_14 = 0;
    field_1C_idx = 0;
    field_0 = 0;
    field_2 = 0;
    field_4 = 0;
    field_21 = 0;
    field_8 = 0;
    field_C = -1;
    field_20 = 0;
    field_2C = 0;
    if (!field_28 && !bSkip_audio_67D6BE)
    {
        field_28 = gRoot_sound_66B038.CreateSoundObject_40EF40(this, 7);
    }
}

MATCH_FUNC(0x5DCDE0)
void Weapon_30::sub_5DCDE0()
{
    sub_5DCD90();

    field_8 = 0;

    if (field_28)
    {
        gRoot_sound_66B038.DestroySoundObj_40FE60(field_28);
        field_28 = 0;
    }
}

MATCH_FUNC(0x5dce20)
void Weapon_30::sub_5DCE20(u8 a2)
{
    field_0 = a2 * 10;
}

STUB_FUNC(0x5dce40)
char_type Weapon_30::sub_5DCE40(u8 a2)
{
    return 0;
}

STUB_FUNC(0x5dcea0)
bool Weapon_30::sub_5DCEA0()
{
    return 0;
}

STUB_FUNC(0x5dcef0)
bool Weapon_30::sub_5DCEF0()
{
    return 0;
}

STUB_FUNC(0x5dcf40)
Player* Weapon_30::sub_5DCF40()
{
    return 0;
}

STUB_FUNC(0x5dcf60)
Object_2C* Weapon_30::sub_5DCF60(s32 a2, s32 a3, s32 a4, s32 a5, s16 a6, s32* a7)
{
    return 0;
}

STUB_FUNC(0x5dd0f0)
void Weapon_30::sub_5DD0F0()
{
}

STUB_FUNC(0x5dd290)
u8 Weapon_30::sub_5DD290()
{
    return 0;
}

STUB_FUNC(0x5dd860)
char_type Weapon_30::sub_5DD860()
{
    return 0;
}

STUB_FUNC(0x5dda70)
u8 Weapon_30::sub_5DDA70()
{
    return 0;
}

STUB_FUNC(0x5ddd20)
u8 Weapon_30::sub_5DDD20()
{
    return 0;
}

STUB_FUNC(0x5ddfc0)
void Weapon_30::sub_5DDFC0(Fix16* a2, char_type* a3, s32 a4)
{
}

STUB_FUNC(0x5de4f0)
s32 Weapon_30::sub_5DE4F0()
{
    return 0;
}

STUB_FUNC(0x5dfb60)
char_type Weapon_30::sub_5DFB60(char_type a2, Sprite* a3, s16 a4)
{
    return 0;
}

STUB_FUNC(0x5e06b0)
void Weapon_30::sub_5E06B0()
{
}

STUB_FUNC(0x5e0740)
char_type Weapon_30::sub_5E0740()
{
    return 0;
}

STUB_FUNC(0x5e0ab0)
void Weapon_30::sub_5E0AB0(char_type a2)
{
}

STUB_FUNC(0x5e0b10)
char_type Weapon_30::sub_5E0B10()
{
    return 0;
}

STUB_FUNC(0x5e0e70)
s32* Weapon_30::sub_5E0E70()
{
    return 0;
}

STUB_FUNC(0x5e10e0)
u8 Weapon_30::sub_5E10E0()
{
    return 0;
}

STUB_FUNC(0x5e13e0)
u8 Weapon_30::sub_5E13E0()
{
    return 0;
}

STUB_FUNC(0x5e1dc0)
char_type Weapon_30::sub_5E1DC0()
{
    return 0;
}

STUB_FUNC(0x5e2550)
char_type Weapon_30::sub_5E2550()
{
    return 0;
}

STUB_FUNC(0x5e2940)
u8 Weapon_30::sub_5E2940()
{
    return 0;
}

STUB_FUNC(0x5e33c0)
char_type Weapon_30::sub_5E33C0()
{
    return 0;
}

STUB_FUNC(0x5e34b0)
char_type Weapon_30::sub_5E34B0()
{
    return 0;
}

STUB_FUNC(0x5e3670)
void Weapon_30::sub_5E3670()
{
}

STUB_FUNC(0x5e3850)
void Weapon_30::sub_5E3850()
{
}

STUB_FUNC(0x5e3bd0)
char_type Weapon_30::sub_5E3BD0()
{
    return 0;
}
