#include "Zheal_D9C.hpp"
#include "root_sound.hpp"

// TODO: Should match but doesn't
STUB_FUNC(0x47e5b0)
Zheal_15C::~Zheal_15C()
{
    if (field_7C)
    {
        gRoot_sound_66B038.DestroySoundObj_40FE60(field_7C);
        field_7C = 0;
    }
}

MATCH_FUNC(0x47e610)
Zheal_15C::Zheal_15C()
{
    field_28 = 0;
    field_7C = 0;
}

STUB_FUNC(0x47e620)
s32 Zheal_15C::sub_47E620(s32 a2, s32 a3, s32* a4)
{
    return 0;
}

STUB_FUNC(0x47e730)
s32 Zheal_15C::sub_47E730(s32 a2, s32 a3, u32 a4)
{
    return 0;
}

STUB_FUNC(0x47e840)
s32 Zheal_15C::sub_47E840(s32 a2, s32 a3)
{
    return 0;
}

STUB_FUNC(0x47e920)
bool Zheal_15C::sub_47E920()
{
    return 0;
}

STUB_FUNC(0x47eb00)
bool Zheal_15C::sub_47EB00()
{
    return 0;
}

STUB_FUNC(0x47ecc0)
s32 Zheal_15C::sub_47ECC0()
{
    return 0;
}

STUB_FUNC(0x47ed60)
s32 Zheal_15C::sub_47ED60()
{
    return 0;
}

STUB_FUNC(0x47edf0)
s32 Zheal_15C::sub_47EDF0()
{
    return 0;
}

STUB_FUNC(0x47ef80)
s32 Zheal_15C::sub_47EF80()
{
    return 0;
}

STUB_FUNC(0x47f170)
s32 Zheal_15C::sub_47F170()
{
    return 0;
}

STUB_FUNC(0x47f220)
s32 Zheal_15C::sub_47F220(s32 a2, s32 a3, Car_3C* a4, s32 a5)
{
    return 0;
}

STUB_FUNC(0x47f290)
s32 Zheal_15C::sub_47F290(s32 a2, s32 a3, Car_3C* a4)
{
    return 0;
}

STUB_FUNC(0x47f2f0)
s32 Zheal_15C::sub_47F2F0(s32 a2, s32 a3, u32* a4)
{
    return 0;
}

STUB_FUNC(0x47f350)
bool Zheal_15C::sub_47F350()
{
    return 0;
}

STUB_FUNC(0x47f3d0)
bool Zheal_15C::sub_47F3D0()
{
    return 0;
}

STUB_FUNC(0x47f450)
bool Zheal_15C::sub_47F450()
{
    return 0;
}

STUB_FUNC(0x47f4c0)
void Zheal_15C::sub_47F4C0()
{
}

STUB_FUNC(0x47f6c0)
char_type Zheal_15C::sub_47F6C0(u32* a2, s32* a3, u32* a4)
{
    return 0;
}

STUB_FUNC(0x47f7f0)
char_type Zheal_15C::sub_47F7F0(u32* a2)
{
    return 0;
}

STUB_FUNC(0x47f930)
void Zheal_15C::sub_47F930(s32 a2)
{
}

STUB_FUNC(0x47fb40)
s32 Zheal_15C::sub_47FB40()
{
    return 0;
}

STUB_FUNC(0x47fba0)
s32 Zheal_15C::sub_47FBA0()
{
    return 0;
}

STUB_FUNC(0x47fd10)
s32 Zheal_15C::sub_47FD10()
{
    return 0;
}

STUB_FUNC(0x47fd50)
char_type Zheal_15C::sub_47FD50()
{
    return 0;
}

STUB_FUNC(0x47fe10)
void Zheal_15C::sub_47FE10()
{
}

STUB_FUNC(0x480310)
void Zheal_15C::sub_480310()
{
}

STUB_FUNC(0x4803b0)
infallible_turing* Zheal_15C::sub_4803B0(s32 a2, s32 a3, char_type a4)
{
    return 0;
}

STUB_FUNC(0x480900)
s32* Zheal_15C::sub_480900(s32 a2, s32 a3, s32 a4)
{
    return 0;
}

STUB_FUNC(0x480b60)
s32* Zheal_15C::sub_480B60(s32 a2, s32 a3, s32 a4)
{
    return 0;
}

STUB_FUNC(0x480e00)
void Zheal_D9C::sub_480E00(Car_BC* a2, u8 a3)
{
}

MATCH_FUNC(0x480e50)
void Zheal_D9C::sub_480E50()
{
    s32 i = 0;
    Zheal_15C* pIter = field_0;
    while (i < field_D98_count)
    {
        pIter->sub_480310();
        i++;
        pIter++;
    }
}

MATCH_FUNC(0x480ec0)
Zheal_15C* Zheal_D9C::sub_480EC0(s32 a2, s32 a3)
{
    Zheal_15C* v4 = &field_0[field_D98_count];
    v4->sub_4803B0(a2, a3, field_D98_count);
    field_D98_count++;
    return v4;
}

MATCH_FUNC(0x480f50)
Zheal_D9C::Zheal_D9C()
{
    field_D98_count = 0;
}

MATCH_FUNC(0x4bbbf0)
Zheal_D9C::~Zheal_D9C()
{
}
