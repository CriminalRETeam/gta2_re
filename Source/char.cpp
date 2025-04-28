#include "char.hpp"
#include "Car_BC.hpp"
#include "Globals.hpp"
#include "Object_3C.hpp"

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

MATCH_FUNC(0x5451A0)
Char_B4::~Char_B4()
{
    this->field_18 = 0;
    this->field_1C = 0;
    this->field_78_next = 0;
    this->field_7C_pPed = 0;
    this->field_80_sprite_ptr = 0;
    this->field_84 = 0;
}

STUB_FUNC(0x5453d0)
void Char_B4::sub_5453D0()
{
}

MATCH_FUNC(0x5454B0)
void Char_B4::sub_5454B0()
{
    field_b0 = -1;
    ((Object_3C*)&field_88_obj_2c)->sub_5A7080();
}

STUB_FUNC(0x5454d0)
void Char_B4::sub_5454D0()
{
}

STUB_FUNC(0x545530)
void Char_B4::sub_545530(Fix16 xpos, Fix16 ypos, Fix16 zpos)
{
}

MATCH_FUNC(0x545570)
s32 Char_B4::sub_545570()
{
    return field_80_sprite_ptr->sub_59E1D0();
}

STUB_FUNC(0x5455f0)
void Char_B4::sub_5455F0()
{
}

STUB_FUNC(0x545600)
s16 Char_B4::sub_545600()
{
    return 0;
}

STUB_FUNC(0x5456a0)
void Char_B4::sub_5456A0()
{
}

STUB_FUNC(0x545700)
s32 Char_B4::sub_545700()
{
    return 0;
}

STUB_FUNC(0x545720)
char_type Char_B4::sub_545720(s32 a2)
{
    return 0;
}

STUB_FUNC(0x5459c0)
char_type Char_B4::sub_5459C0()
{
    return 0;
}

STUB_FUNC(0x5459e0)
void Char_B4::sub_5459E0()
{
}

STUB_FUNC(0x546360)
void Char_B4::sub_546360()
{
}

STUB_FUNC(0x548590)
void Char_B4::sub_548590()
{
}

STUB_FUNC(0x548670)
void Char_B4::sub_548670(char_type a2)
{
}

STUB_FUNC(0x548840)
void Char_B4::sub_548840(Object_2C* a2)
{
}

STUB_FUNC(0x548bd0)
void Char_B4::sub_548BD0(s32 a2)
{
}

STUB_FUNC(0x54a530)
void Char_B4::sub_54A530(Car_BC* a2, Car_BC* a3, s32 a4)
{
}

STUB_FUNC(0x54b8f0)
char_type Char_B4::sub_54B8F0()
{
    return 0;
}

STUB_FUNC(0x54c090)
s32 Char_B4::sub_54C090()
{
    return 0;
}

STUB_FUNC(0x54c1a0)
char_type Char_B4::sub_54C1A0(s32 a2)
{
    return 0;
}

STUB_FUNC(0x54c3e0)
char_type Char_B4::sub_54C3E0()
{
    return 0;
}

STUB_FUNC(0x54c500)
char_type Char_B4::sub_54C500(char_type a2, char_type a3)
{
    return 0;
}

STUB_FUNC(0x54c580)
void Char_B4::sub_54C580()
{
}

STUB_FUNC(0x54c6c0)
s16 Char_B4::sub_54C6C0(s32 a2)
{
    return 0;
}

STUB_FUNC(0x54c900)
s16 Char_B4::sub_54C900(s32 a2)
{
    return 0;
}

STUB_FUNC(0x54cae0)
s16 Char_B4::sub_54CAE0()
{
    return 0;
}

STUB_FUNC(0x54cc40)
void Char_B4::sub_54CC40()
{
}

STUB_FUNC(0x54dd70)
void Char_B4::sub_54DD70()
{
}

STUB_FUNC(0x54ddf0)
void Char_B4::state_0_54DDF0()
{
}

STUB_FUNC(0x54ecb0)
char_type Char_B4::sub_54ECB0(s32 a2)
{
    return 0;
}

STUB_FUNC(0x54ef60)
char_type Char_B4::sub_54EF60(char_type a2, char_type a3)
{
    return 0;
}

STUB_FUNC(0x54fec0)
char_type Char_B4::sub_54FEC0(s32 a2)
{
    return 0;
}

STUB_FUNC(0x550090)
char_type Char_B4::sub_550090(s32 a2, s32 a3)
{
    return 0;
}

STUB_FUNC(0x5504f0)
void Char_B4::state_1_5504F0()
{
}

STUB_FUNC(0x550f60)
s16* Char_B4::sub_550F60(s16* a2, s16 a3)
{
    return 0;
}

STUB_FUNC(0x551350)
char_type Char_B4::sub_551350(s16 a2)
{
    return 0;
}

STUB_FUNC(0x551400)
char_type Char_B4::sub_551400()
{
    return 0;
}

STUB_FUNC(0x5516f0)
char_type Char_B4::sub_5516F0()
{
    return 0;
}

//STUB_FUNC(0x5519f0)
//void j_Char_B4::state_1_5504F0()
//{
//}

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
Ped* Char_C::SpawnPedAt(Fix16 xpos, Fix16 ypos, Fix16 zpos, u8 remap, Ang16 rotation)
{
    Char_203AC* v6 = gChar_203AC_6787B8;
    Ped* pPed = gChar_203AC_6787B8->field_0;
    v6->field_0 = pPed->field_160_next_ped;

    pPed->field_160_next_ped = v6->field_4;

    v6->field_4 = pPed;

    pPed->sub_45B440();

    if (!pPed->sub_45C830(xpos, ypos, zpos))
    {
        return 0;
    }
    pPed->field_168_game_object->field_40_rotation.field_0_value = rotation.rValue;
    pPed->field_244_remap = remap;

    Char_B4* pB4 = pPed->field_168_game_object;
    pB4->field_5_remap = remap;
    if (remap != 0xFF)
    {
        pB4->field_80_sprite_ptr->SetRemap(remap);
    }
    pPed->field_134 = rotation;
    pPed->field_288_threat_search = 2;
    pPed->field_28C_threat_reaction = 3;
    pPed->field_216_health = 100;
    pPed->field_26C_graphic_type = 0;
    return pPed;
}

STUB_FUNC(0x470b00)
Ped* Char_C::sub_470B00(Car_BC* a2)
{
    return 0;
}

STUB_FUNC(0x470ba0)
Ped* Char_C::sub_470BA0(Car_BC* a1, s32 a2)
{
    return 0;
}

STUB_FUNC(0x470cc0)
Ped* Char_C::sub_470CC0(Car_BC* a2)
{
    return 0;
}

STUB_FUNC(0x470d60)
Ped* Char_C::sub_470D60()
{
    return 0;
}

STUB_FUNC(0x470e30)
Ped* Char_C::sub_470E30()
{
    return 0;
}

STUB_FUNC(0x470f30)
Ped* Char_C::sub_470F30()
{
    return 0;
}

STUB_FUNC(0x470f90)
Ped* Char_C::sub_470F90(Ped* pSrc)
{
    return 0;
}

STUB_FUNC(0x471060)
void Char_C::DoIanTest_471060(s16 a1)
{
}

STUB_FUNC(0x4710c0)
Ped* Char_C::PedById(s32 pedId)
{
    Ped* pPedIter = gChar_203AC_6787B8->field_4;
    if (!pPedIter)
    {
        return 0;
    }

    while (pPedIter->field_200_id != pedId)
    {
        pPedIter = pPedIter->field_160_next_ped;
        if (!pPedIter)
        {
            return 0;
        }
    }
    return pPedIter;
}

STUB_FUNC(0x471110)
Char_203AC::~Char_203AC()
{
}
