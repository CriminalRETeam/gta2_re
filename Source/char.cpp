#include "char.hpp"
#include "Globals.hpp"

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

STUB_FUNC(0x470a50)
cool_nash_0x294* Char_C::sub_470A50(s32 xpos, s32 ypos, s32 zpos, u8 remap, s16 a6)
{
    return 0;
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
