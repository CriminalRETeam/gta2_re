#include "char.hpp"
#include "Globals.hpp"
#include "Car_BC.hpp"

EXPORT_VAR Char_C* gChar_C_6787BC;
GLOBAL(gChar_C_6787BC, 0x6787BC);

EXPORT_VAR Char_203AC* gChar_203AC_6787B8;
GLOBAL(gChar_203AC_6787B8, 0x6787B8);

EXPORT_VAR Char_11944* gChar_11944_6FDB44;
GLOBAL(gChar_11944_6FDB44, 0x6FDB44);

EXPORT_VAR Char_324* gChar_324_678b50;
GLOBAL(gChar_324_678b50, 0x678b50);

STUB_FUNC(0x544ff0)
Char_B4::Char_B4()
{
}

Char_B4::~Char_B4()
{
}

STUB_FUNC(0x5453D0)
void Char_B4::sub_5453D0()
{

}

// This constructor doesn't exist.
// It's inlined in Char_C::Char_C
Char_11944::Char_11944()
{
    Char_B4* pIter = &field_4_array[0];
    for (s32 i = 0; i < 399; i++)
    {
        pIter->field_78_next = pIter + 1;
        pIter++;
    }

    field_4_array[399].field_78_next = NULL;
    field_0_next = &field_4_array[0];
}

// This constructor doesn't exist.
// It's inlined in Char_C::Char_C
Char_324::Char_324()
{
    Char_8* pIter = &field_4_array[0];
    for (s32 i = 0; i < 99; i++)
    {
        pIter->field_4_pOwner = pIter + 1;
        pIter++;
    }

    field_4_array[99].field_4_pOwner = NULL;
    field_0_next = &field_4_array[0];
    field_320_in_use = 0;
}

STUB_FUNC(0x46eb60)
void Char_C::sub_46EB60(u32* a2)
{
}

STUB_FUNC(0x4703f0)
void Char_C::sub_4703F0()
{
}

STUB_FUNC(0x470650)
Char_C::Char_C()
{
}

STUB_FUNC(0x4709b0)
Char_C::~Char_C()
{
}

MATCH_FUNC(0x470a50)
cool_nash_0x294* Char_C::sub_470A50(s32 xpos, s32 ypos, s32 zpos, u8 remap, s16 rotation)
{
    Char_203AC* v6 = gChar_203AC_6787B8;
    cool_nash_0x294* pPed = gChar_203AC_6787B8->field_0;
    v6->field_0 = pPed->field_160_next_ped;

    pPed->field_160_next_ped = v6->field_4;

    v6->field_4 = pPed;

    pPed->sub_45B440();

    if (!pPed->sub_45C830(xpos, ypos, zpos))
    {
        return 0;
    }
    pPed->field_168_game_object->field_40_rotation.field_0_value = rotation;
    pPed->field_244_remap = remap;

    Char_B4* pB4 = pPed->field_168_game_object;
    pB4->field_5_remap = remap;
    if (remap != 0xFF)
    {
        // TODO: Is Car_3C and Sprite_3C the same thing ??
        ((Car_3C*)(pB4->field_80_sprite_ptr))->sub_59EA00(remap);
    }
    pPed->field_134 = rotation;
    pPed->field_288_threat_search = 2;
    pPed->field_28C_threat_reaction = 3;
    pPed->field_216_health = 100;
    pPed->field_26C = 0;
    return pPed;
}

STUB_FUNC(0x470b00)
cool_nash_0x294* Char_C::sub_470B00(Car_BC* a2)
{
    return 0;
}

STUB_FUNC(0x470ba0)
cool_nash_0x294* Char_C::sub_470BA0(Car_BC* a1, s32 a2)
{
    return 0;
}

STUB_FUNC(0x470cc0)
cool_nash_0x294* Char_C::sub_470CC0(Car_BC* a2)
{
    return 0;
}

STUB_FUNC(0x470d60)
cool_nash_0x294* Char_C::sub_470D60()
{
    return 0;
}

STUB_FUNC(0x470e30)
cool_nash_0x294* Char_C::sub_470E30()
{
    return 0;
}

STUB_FUNC(0x470f30)
cool_nash_0x294* Char_C::sub_470F30()
{
    return 0;
}

STUB_FUNC(0x471060)
void Char_C::DoIanTest_471060(s16 a1)
{
}

STUB_FUNC(0x4710c0)
cool_nash_0x294* Char_C::sub_4710C0(s32 a2)
{
    return 0;
}

STUB_FUNC(0x471110)
Char_203AC::~Char_203AC()
{
}
