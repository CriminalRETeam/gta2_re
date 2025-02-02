#include "Police_7B8.hpp"
#include "Car_BC.hpp"
#include "Globals.hpp"
#include "Kfc_1E0.hpp"
#include "Ped.hpp"

EXPORT_VAR s32 dword_6FECE8;
GLOBAL(dword_6FECE8, 0x6FECE8);

EXPORT_VAR Ped* pPed_6FEDDC;
GLOBAL(pPed_6FEDDC, 0x6FEDDC);

MATCH_FUNC(0x4beb30)
Police_38::Police_38()
{
    sub_5709C0();
}

MATCH_FUNC(0x4beb40)
Police_38::~Police_38()
{
}

STUB_FUNC(0x570790)
char_type Police_38::sub_570790(Police_38* a1, s32 a2)
{
    return 0;
}

MATCH_FUNC(0x5709c0)
void Police_38::sub_5709C0()
{
    field_2_targ_x = 0;
    field_3_targ_y = 0;
    field_4_targ_z = 0;
    field_18 = 0;
    field_1C = 0;
    field_1A = 0;
    field_10_subObj = 0;
    field_24_state = 0;
    field_28 = 0;
    field_8 = dword_6FECE8;
    field_C = dword_6FECE8;
    field_14_pObj = 0;
    field_1A = 0;
    field_29 = 0;
    field_2A = 0;
    field_2C = 0;
    field_30 = 0;
    field_34 = 0;
}

STUB_FUNC(0x570a10)
void Police_38::sub_570A10()
{
}

STUB_FUNC(0x570ab0)
void Police_38::sub_570AB0()
{
}

STUB_FUNC(0x570bf0)
Kfc_30* Police_38::sub_570BF0()
{
    return 0;
}

STUB_FUNC(0x570e30)
s32 Police_38::sub_570E30()
{
    return 0;
}

STUB_FUNC(0x571150)
s32 Police_38::sub_571150()
{
    return 0;
}

STUB_FUNC(0x571350)
u32* Police_38::sub_571350()
{
    return 0;
}

STUB_FUNC(0x571540)
char_type Police_38::sub_571540()
{
    return 0;
}

STUB_FUNC(0x571a30)
char_type Police_38::sub_571A30()
{
    return 0;
}

STUB_FUNC(0x5720c0)
char_type Police_38::sub_5720C0()
{
    return 0;
}

STUB_FUNC(0x572210)
bool Police_38::sub_572210()
{
    return 0;
}

STUB_FUNC(0x572340)
char_type Police_38::sub_572340()
{
    return 0;
}

STUB_FUNC(0x572920)
void Police_38::sub_572920()
{
}

STUB_FUNC(0x574720)
void Police_38::sub_574720()
{
}

STUB_FUNC(0x574f10)
char_type Police_38::sub_574F10()
{
    return 0;
}

STUB_FUNC(0x575200)
void Police_38::sub_575200()
{
}

STUB_FUNC(0x575210)
void Police_38::sub_575210()
{
    Kfc_30* pKfc30 = this->field_10_subObj;
    if (!pKfc30->field_24 || pPed_6FEDDC->field_225)
    {
        pPed_6FEDDC->sub_463830(0, 9999);
        pPed_6FEDDC->sub_463570(0, 9999);
    }
    else
    {
        pKfc30->field_0->field_A6 |= 0x20u;
    }
}

STUB_FUNC(0x575270)
void Police_38::sub_575270()
{
    if (!this->field_10_subObj->field_24 || pPed_6FEDDC->field_225)
    {
        pPed_6FEDDC->sub_463830(0, 9999);
        pPed_6FEDDC->sub_463570(0, 9999);
    }
}

STUB_FUNC(0x5752c0)
void Police_38::sub_5752C0()
{
}

STUB_FUNC(0x575310)
void Police_38::sub_575310()
{
}

STUB_FUNC(0x575590)
void Police_38::sub_575590()
{
}

STUB_FUNC(0x575650)
s32 Police_38::sub_575650()
{
    return 0;
}

MATCH_FUNC(0x575710)
void Police_A4::sub_575710()
{
    field_0 = 0;
    field_4 = 0;
    field_8 = 0;
    field_9 = 0;
    field_A = 0;
    field_C = 0;
    field_10 = 0;
    field_14 = 0;
    field_18 = 0;
    field_1C = 0;
    field_20 = 0;
    field_24 = 0;
    field_28 = 0;
    field_2C = 0;
    field_30 = 0;
    field_34 = 0;
    field_38 = 0;
    field_3C = 0;
    field_40 = 0;
    field_44 = 0;
    field_48 = 0;
    field_4C = 0;
    field_50 = 0;
    field_54 = 0;
    field_58 = 0;
    field_5C = 0;
    field_60 = 0;
    field_64 = 0;
    field_68 = 0;
    field_6C = 0;
    field_70 = 0;
    field_74 = 0;
    field_78 = 0;
    field_7C = 0;
    field_80 = 0;
    field_84 = 0;
    field_88 = 0;
    field_8C = 0;
    field_90 = 0;
    field_94 = 0;
    field_98 = 0;
    field_9C = 0;
}

STUB_FUNC(0x5757b0)
void Police_A4::sub_5757B0()
{
}

STUB_FUNC(0x575ca0)
void Police_A4::sub_575CA0()
{
}

STUB_FUNC(0x575ff0)
char_type Police_A4::sub_575FF0(u8 a2, s32 a3, u8 a4, s32 a5)
{
    return 0;
}

MATCH_FUNC(0x577480)
Police_A4::Police_A4()
{
    sub_575710();
    field_A0_pMem = new u8[0x18u]; // TODO: A class/struct?
}

MATCH_FUNC(0x5774a0)
Police_A4::~Police_A4()
{
    delete[] field_A0_pMem;
}

// https://decomp.me/scratch/McaDT
// TODO: Missing SEH for unknown reasons, SEH present on decomp.me
STUB_FUNC(0x4BEB50)
Police_7B8::~Police_7B8()
{
}

STUB_FUNC(0x56f400)
s32 Police_7B8::sub_56F400()
{
    return 0;
}

STUB_FUNC(0x56f4d0)
char_type Police_7B8::sub_56F4D0(Ped* a2)
{
    return 0;
}

STUB_FUNC(0x56f560)
Police_38* Police_7B8::sub_56F560()
{
    return 0;
}

STUB_FUNC(0x56f5c0)
Ped* Police_7B8::sub_56F5C0(s32 a2, s32 a3, s16 a4)
{
    return 0;
}

STUB_FUNC(0x56f6d0)
void Police_7B8::sub_56F6D0(Car_BC* a2)
{
}

STUB_FUNC(0x56f800)
bool Police_7B8::sub_56F800(Ped* a2)
{
    return 0;
}

STUB_FUNC(0x56f880)
bool Police_7B8::sub_56F880(Ped* a2)
{
    return 0;
}

STUB_FUNC(0x56f8e0)
s32 Police_7B8::sub_56F8E0(Ped* a2, Ped* a3)
{
    return 0;
}

STUB_FUNC(0x56f940)
s32 Police_7B8::sub_56F940(u32* a2)
{
    return 0;
}

STUB_FUNC(0x56fa40)
s16 Police_7B8::sub_56FA40()
{
    return 0;
}

STUB_FUNC(0x56faa0)
char_type Police_7B8::sub_56FAA0(Police_7C* a2)
{
    return 0;
}

STUB_FUNC(0x56fbd0)
void Police_7B8::sub_56FBD0()
{
}

STUB_FUNC(0x570270)
void Police_7B8::sub_570270()
{
}

STUB_FUNC(0x570320)
s32 Police_7B8::sub_570320(Ped* a2, s32 a3, s32 a4, s32 a5, s16 a6)
{
    return 0;
}

STUB_FUNC(0x5703e0)
char_type Police_7B8::sub_5703E0(Car_BC* a2)
{
    return 0;
}

STUB_FUNC(0x5707b0)
char_type Police_7B8::sub_5707B0(s32 a2, s32 a3)
{
    return 0;
}

STUB_FUNC(0x5708c0)
s32 Police_7B8::sub_5708C0(Police_7B8* a1, s32 a2, u32* a3)
{
    return 0;
}

STUB_FUNC(0x570940)
void Police_7B8::sub_570940(Police_7B8* a1, s32 a2, Ped* a3)
{
}

STUB_FUNC(0x577320)
char_type Police_7B8::sub_577320()
{
    return 0;
}

STUB_FUNC(0x577370)
void Police_7B8::sub_577370(u8 a2, s32 a3, s32 a4)
{
}
