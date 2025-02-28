#include "Phi_8CA8.hpp"
#include "Car_BC.hpp"
#include "Globals.hpp"
#include "gtx_0x106C.hpp"
#include "sprite.hpp"
#include <memory.h>

EXPORT_VAR Fix16 DAT_006f8fd8;
GLOBAL(DAT_006f8fd8, 0x6F8FD8);

EXPORT_VAR Fix16 DAT_006fcdd8;
GLOBAL(DAT_006fcdd8, 0x6FCDD8);

EXPORT_VAR Fix16 DAT_006fce10;
GLOBAL(DAT_006fce10, 0x6FCE10);

EXPORT_VAR Fix16 DAT_006F8FF0;
GLOBAL(DAT_006F8FF0, 0x6F8FF0);

EXPORT_VAR Fix16 DAT_006FCED0;
GLOBAL(DAT_006FCED0, 0x6FCED0);

EXPORT_VAR s32 Phi_54_array_lenght_00623EEC;
GLOBAL(Phi_54_array_lenght_00623EEC, 0x623EEC);

// This array is initialized FUN_005323b0, which I think it's a static constructor.
// It also needs to be defined as an array and not a pointer otherwise Phi_8CA8::sub_533360 doesn't match
EXPORT_VAR Phi_54 Phi_54_array_006FC5F8[24];
GLOBAL(Phi_54_array_006FC5F8, 0x66FC5F86FC5F8);

MATCH_FUNC(0x532fb0)
Phi_54::Phi_54(s32 param_1,
               s32& param_2,
               s32 param_3,
               s8 param_4,
               s32& param_5,
               s32& param_6,
               s32& param_7,
               s32 param_8,
               s32 param_9,
               s8 param_10,
               s32& param_11,
               s32& param_12,
               s8 param_13,
               s32& param_14,
               s32 param_15,
               s32 param_16,
               s32 param_17,
               s32 param_18,
               s8 param_19,
               s32 param_20,
               s8 param_21)
{
    field_0 = param_1;
    field_4 = param_2;
    field_8 = param_3;
    field_C = param_4;
    field_10 = param_5;
    field_14 = param_6;
    field_18 = param_7;
    field_1C = param_8;
    field_20 = param_9;
    field_24 = param_10;
    field_28 = param_11;
    field_2C = param_12;
    field_30 = param_13;
    field_34 = param_14;
    field_38 = param_15;
    field_3C = param_16;
    field_40 = param_17;
    field_44 = param_18;
    field_48 = param_19;
    field_4C = param_20;
    field_50 = param_21;
}

MATCH_FUNC(0x4bdf60)
Phi_74::~Phi_74()
{
}

MATCH_FUNC(0x533060)
void Phi_74::sub_533060(Fix16 a2, Fix16 a3, Fix16 a4)
{
    field_0 = a2;
    field_4 = a3;
    field_8 = a4;
    if (field_0 < field_4)
    {
        field_C = a2;
    }
    else
    {
        field_C = a3;
    }
}

MATCH_FUNC(0x533090)
void Phi_74::sub_533090()
{
    if (this->field_28 != sprite_types_enum::unknown_1)
    {
        u16 sprite_id = gGtx_0x106C_703DD4->convert_sprite_pal_5AA460(field_28, field_1E);
        sprite_index * sprite = gGtx_0x106C_703DD4->get_sprite_index_5AA440(sprite_id);
        field_0 = Fix16(sprite->field_4_width) / 0x40;
        field_4 = Fix16(sprite->field_5_height) / 0x40;
        field_8 = DAT_006FCED0;
    }
    else
    {
        field_0 = DAT_006F8FF0;
        field_4 = DAT_006F8FF0;
        field_8 = DAT_006FCED0;
    }

    if (field_0 < field_4)
    {
        field_C = field_0;
    }
    else
    {
        field_C = field_4;
    }
}

MATCH_FUNC(0x533110)
void Phi_74::sub_533110(s16 a2)
{
    switch (field_28)
    {
        case 4:
            field_30 = 5;
            break;
        case 5:
            field_30 = 6;
            break;
    }
    field_1C = a2;
}

MATCH_FUNC(0x533150)
void Phi_74::sub_533150(s16 a2, s16 a3)
{
    field_1E += a2;
    field_6C = a3;
}

MATCH_FUNC(0x533170)
Sprite* Phi_74::sub_533170()
{
    Sprite* pFreeSprite = gSprite_49B28_703818->field_0_first_free;
    gSprite_49B28_703818->field_0_first_free = gSprite_49B28_703818->field_0_first_free->next_ptr;
    pFreeSprite->sub_5A2CF0();
    sub_5331A0(pFreeSprite);
    return pFreeSprite;
}

MATCH_FUNC(0x5331a0)
void Phi_74::sub_5331A0(Sprite* pSprite)
{
    s16 f1E; // ax
    s32 f40; // eax

    pSprite->field_30_sprite_type_enum = this->field_28;
    pSprite->sub_59E960();
    f1E = this->field_1E;

    if (pSprite->field_22_sprite_id != f1E)
    {
        pSprite->field_22_sprite_id = f1E;
        pSprite->sub_59FA40();
    }

    pSprite->field_28_uni = this->field_2C;
    pSprite->field_2C = this->field_20;

    if (this->field_30 != 2)
    {
        pSprite->sub_59EA00(this->field_1C);
    }

    f40 = this->field_40;
    if (f40 > 0 && (f40 <= 3 || f40 == 4))
    {
        pSprite->sub_59F950(this->field_0, this->field_4, this->field_8);
    }

    if (this->field_70)
    {
        pSprite->sub_5A29D0();
    }
}

// fix16?
s32 dword_6F8FA4 = 0x3FC000;
s32 dword_6FCE08 = 0;

MATCH_FUNC(0x533220)
Phi_74::Phi_74()
{
    field_0 = dword_6F8FA4;
    field_4 = dword_6F8FA4;
    field_8 = dword_6F8FA4;
    field_24_idx = 0;
    field_28 = 0;
    field_2C = 0;
    field_30 = 0;
    field_34 = 0;
    field_38 = 0;
    field_3C = 0;
    field_40 = 0;
    field_44 = 0;
    field_48 = 0;
    field_4C = 1;
    field_50 = 1;
    field_58 = 0;
    field_54 = 0;
    field_C = dword_6F8FA4;
    field_10 = dword_6F8FA4;
    field_14 = dword_6F8FA4;
    field_1C = 9999;
    field_61 = 0;
    field_64 = 99;
    field_65 = 99;
    field_1E = 99;
    field_6C = 99;
    s32 v1 = dword_6FCE08;
    field_68 = 0;
    field_18 = v1;
    field_20 = 0;
    field_60 = 0;
    field_5C = 1;
    field_70 = 0;
}

MATCH_FUNC(0x4bdf40)
Phi_8CA8::~Phi_8CA8()
{
}

MATCH_FUNC(0x5332d0)
Phi_74* Phi_8CA8::sub_5332D0(s32 idx, s32 a3, s16 a4, u8 a5)
{
    Phi_74* result = sub_5343C0(idx);
    result->field_28 = a3;
    result->field_1E = a4;
    result->field_6C = a5;
    result->field_30 = 2;
    return result;
}

MATCH_FUNC(0x533300)
void Phi_8CA8::sub_533300()
{
    s16 sprites = 0;
    u16 i = 0;
    object_info* pIter = gGtx_0x106C_703DD4->get_map_object_info_5AA910(i++);
    while (pIter)
    {
        Phi_74* pPhi = sub_5332D0(pIter->field_0_model, 5, sprites, pIter->field_1_sprites);
        sprites += pIter->field_1_sprites;
        pPhi->sub_533090();
        pIter = gGtx_0x106C_703DD4->get_map_object_info_5AA910(i++);
    }
}

MATCH_FUNC(0x533360)
void Phi_8CA8::sub_533360()
{
    int iVar4 = Phi_54_array_lenght_00623EEC;

    if (0 < iVar4)
    {
        Phi_54* puVar3 = Phi_54_array_006FC5F8;
        do
        {
            Phi_74* pPVar2 = sub_534360(puVar3->field_0);
            pPVar2->field_34 = puVar3->field_4;
            pPVar2->field_38 = puVar3->field_8;
            pPVar2->field_3C = puVar3->field_8;
            pPVar2->field_61 = puVar3->field_C;
            pPVar2->field_40 = puVar3->field_18;
            pPVar2->field_44 = puVar3->field_10;
            pPVar2->field_48 = puVar3->field_14;
            pPVar2->field_10 = puVar3->field_1C;
            pPVar2->field_65 = puVar3->field_24;
            pPVar2->field_14 = puVar3->field_20;
            pPVar2->field_4C = puVar3->field_28;
            pPVar2->field_64 = puVar3->field_30;
            pPVar2->field_58 = puVar3->field_34;
            pPVar2->field_18 = puVar3->field_38;
            pPVar2->field_20 = puVar3->field_40;
            pPVar2->field_2C = (Sprite*)puVar3->field_3C;
            pPVar2->field_60 = 1;
            pPVar2->field_5C = puVar3->field_4C;
            pPVar2->field_68 = 0;
            pPVar2->field_70 = puVar3->field_44;
            pPVar2->field_62 = puVar3->field_48;
            pPVar2->field_63 = puVar3->field_50;
            pPVar2->field_54 = 0;
            puVar3++;
        } while (--iVar4 != 0);
    }
}

MATCH_FUNC(0x533420)
void Phi_8CA8::sub_533420()
{
    Phi_74* pAVar1;

    sub_534370(0x97, 6);
    pAVar1 = sub_534360(0x97);
    pAVar1->sub_533150(1, pAVar1->field_6C + -1);
    pAVar1->field_34 = 2;
    pAVar1->field_64 = '\x05';
    pAVar1->field_38 = 0x98;
    pAVar1->field_3C = 0x98;
    pAVar1->field_40 = 0;
    pAVar1->field_44 = 0;
    pAVar1->field_65 = '\x01';
    pAVar1->field_61 = '\x01';

    pAVar1 = sub_534370(0x98, 6);
    pAVar1->sub_533150(pAVar1->field_6C - 1, 1);
    pAVar1->field_34 = 0;
    pAVar1->field_38 = 0;
    pAVar1->field_3C = 0;
    pAVar1->field_40 = 0;
    pAVar1->field_44 = 0;
    pAVar1->field_61 = '\0';

    pAVar1 = sub_534370(0x34, 4);
    pAVar1->sub_533150(pAVar1->field_6C - 1, 1);
    pAVar1->field_34 = 0;
    pAVar1->field_38 = 0;
    pAVar1->field_3C = 0;
    pAVar1->field_40 = 0;
    pAVar1->field_44 = 0;
    pAVar1->field_61 = '\0';

    sub_534370(0x32, 0xc);
    pAVar1 = sub_534360(0x32);
    pAVar1->sub_533150(1, pAVar1->field_6C + -1);
    pAVar1->field_34 = 2;
    pAVar1->field_64 = '\x03';
    pAVar1->field_38 = 0x33;
    pAVar1->field_3C = 0x33;
    pAVar1->field_40 = 0;
    pAVar1->field_44 = 0;
    pAVar1->field_65 = '\x01';
    pAVar1->field_61 = '\x01';

    pAVar1 = sub_534370(0x33, 0xc);
    pAVar1->sub_533150(pAVar1->field_6C - 1, 1);
    pAVar1->field_34 = 0;
    pAVar1->field_38 = 0;
    pAVar1->field_3C = 0;
    pAVar1->field_40 = 0;
    pAVar1->field_44 = 0;
    pAVar1->field_61 = '\0';

    pAVar1 = sub_534370(0x9b, 3);
    pAVar1->sub_533150(1, pAVar1->field_6C + -1);
    pAVar1->field_34 = 4;
    pAVar1->field_64 = 1;
    pAVar1->field_38 = 0x35;
    pAVar1->field_3C = 0x35;
    pAVar1->field_40 = 4;
    pAVar1->field_44 = 0;
    pAVar1->field_10 = DAT_006f8fd8;
    pAVar1->field_14 = -DAT_006fcdd8;
    pAVar1->field_61 = 1;
    pAVar1->field_4C = 3;
    pAVar1->field_50 = 3;
    pAVar1->field_65 = -1;
    pAVar1->field_58 = 1;

    pAVar1 = sub_534370(0x35, 3);
    pAVar1->sub_533150(pAVar1->field_6C - 1, 1);

    pAVar1 = sub_534370(0x7b, 0xb);
    pAVar1->sub_533150(1, pAVar1->field_6C + -1);
    pAVar1->field_34 = 4;
    pAVar1->field_64 = '\x01';
    pAVar1->field_38 = 0x37;
    pAVar1->field_3C = 0x37;
    pAVar1->field_40 = 4;
    pAVar1->field_44 = 0;
    pAVar1->field_10 = DAT_006f8fd8;
    pAVar1->field_14 = -DAT_006fcdd8;
    pAVar1->field_61 = '\x01';
    pAVar1->field_4C = 3;
    pAVar1->field_50 = 3;
    pAVar1->field_65 = -1;
    pAVar1->field_58 = 1;

    pAVar1 = sub_534370(0x37, 0xb);
    pAVar1->sub_533150(pAVar1->field_6C - 1, 1);

    pAVar1 = sub_534370(0x9c, 5);
    pAVar1->sub_533150(1, pAVar1->field_6C + -1);
    pAVar1->field_34 = 4;
    pAVar1->field_64 = '\x01';
    pAVar1->field_38 = 0x36;
    pAVar1->field_3C = 0x36;
    pAVar1->field_40 = 4;
    pAVar1->field_44 = 0;
    pAVar1->field_10 = DAT_006f8fd8;
    pAVar1->field_14 = -DAT_006fcdd8;
    pAVar1->field_61 = '\x01';
    pAVar1->field_4C = 3;
    pAVar1->field_50 = 3;
    pAVar1->field_65 = -1;
    pAVar1->field_58 = 1;

    pAVar1 = sub_534370(0x36, 5);
    pAVar1->sub_533150(pAVar1->field_6C - 1, 1);

    pAVar1 = sub_534370(0x38, 0xd);
    pAVar1->sub_533150(1, pAVar1->field_6C + -1);
    pAVar1->field_34 = 4;
    pAVar1->field_64 = '\x01';
    pAVar1->field_38 = 0xd;
    pAVar1->field_3C = 0xd;
    pAVar1->field_40 = 4;
    pAVar1->field_44 = 0;
    pAVar1->field_10 = DAT_006f8fd8;
    pAVar1->field_14 = -DAT_006fcdd8;
    pAVar1->field_61 = '\x01';
    pAVar1->field_4C = 3;
    pAVar1->field_50 = 3;
    pAVar1->field_65 = -1;
    pAVar1->field_58 = 1;

    pAVar1 = sub_534370(0x39, 0xe);
    pAVar1->sub_533150(1, pAVar1->field_6C + -1);
    pAVar1->field_34 = 4;
    pAVar1->field_64 = '\x01';
    pAVar1->field_38 = 0x3a;
    pAVar1->field_3C = 0x3a;
    pAVar1->field_40 = 4;
    pAVar1->field_44 = 0;
    pAVar1->field_10 = DAT_006f8fd8;
    pAVar1->field_14 = -DAT_006fcdd8;
    pAVar1->field_61 = '\x01';
    pAVar1->field_4C = 3;
    pAVar1->field_50 = 3;
    pAVar1->field_65 = -1;
    pAVar1->field_58 = 1;

    pAVar1 = sub_534370(0x3a, 0xe);
    pAVar1->sub_533150(pAVar1->field_6C - 1, 1);

    pAVar1 = sub_534370(0x3b, 0xf);
    pAVar1->sub_533150(1, pAVar1->field_6C + -1);
    pAVar1->field_34 = 4;
    pAVar1->field_64 = '\x01';
    pAVar1->field_38 = 0x3c;
    pAVar1->field_3C = 0x3c;
    pAVar1->field_40 = 4;
    pAVar1->field_44 = 0;
    pAVar1->field_10 = DAT_006f8fd8;
    pAVar1->field_14 = -DAT_006fcdd8;
    pAVar1->field_61 = '\x01';
    pAVar1->field_4C = 3;
    pAVar1->field_50 = 3;
    pAVar1->field_65 = -1;
    pAVar1->field_58 = 1;

    pAVar1 = sub_534370(0x3c, 0xf);
    pAVar1->sub_533150(pAVar1->field_6C - 1, 1);

    pAVar1 = sub_534370(0x3d, 0x10);
    pAVar1->sub_533150(1, pAVar1->field_6C + -1);
    pAVar1->field_34 = 4;
    pAVar1->field_64 = '\x01';
    pAVar1->field_38 = 0x3e;
    pAVar1->field_3C = 0x3e;
    pAVar1->field_40 = 4;
    pAVar1->field_44 = 0;
    pAVar1->field_10 = DAT_006f8fd8;
    pAVar1->field_14 = -DAT_006fcdd8;
    pAVar1->field_61 = '\x01';
    pAVar1->field_4C = 3;
    pAVar1->field_50 = 3;
    pAVar1->field_65 = -1;
    pAVar1->field_58 = 1;

    pAVar1 = sub_534370(0x3e, 0x10);
    pAVar1->sub_533150(pAVar1->field_6C - 1, 1);

    pAVar1 = sub_534370(0x31, 0x12);
    pAVar1->sub_533150(1, pAVar1->field_6C + -1);
    pAVar1->field_34 = 4;
    pAVar1->field_64 = '\x01';
    pAVar1->field_38 = 0x12;
    pAVar1->field_3C = 0x12;
    pAVar1->field_40 = 4;
    pAVar1->field_44 = 0;
    pAVar1->field_10 = DAT_006f8fd8;
    pAVar1->field_14 = -DAT_006fcdd8;
    pAVar1->field_61 = '\x01';
    pAVar1->field_4C = 3;
    pAVar1->field_50 = 3;
    pAVar1->field_65 = -1;
    pAVar1->field_58 = 1;

    pAVar1 = sub_534370(0x2d, 0x16);
    pAVar1->sub_533150(1, pAVar1->field_6C + -1);
    pAVar1->field_34 = 4;
    pAVar1->field_64 = '\x01';
    pAVar1->field_38 = 0x2e;
    pAVar1->field_3C = 0x2e;
    pAVar1->field_40 = 4;
    pAVar1->field_44 = 0;
    pAVar1->field_10 = DAT_006f8fd8;
    pAVar1->field_14 = -DAT_006fcdd8;
    pAVar1->field_61 = '\x01';
    pAVar1->field_4C = 3;
    pAVar1->field_50 = 3;
    pAVar1->field_65 = -1;
    pAVar1->field_58 = 1;

    pAVar1 = sub_534370(0x2e, 0x16);
    pAVar1->sub_533150(pAVar1->field_6C - 1, 1);

    pAVar1 = sub_534370(0x2f, 0x15);
    pAVar1->sub_533150(1, pAVar1->field_6C + -1);
    pAVar1->field_34 = 4;
    pAVar1->field_64 = '\x01';
    pAVar1->field_38 = 0x30;
    pAVar1->field_3C = 0x30;
    pAVar1->field_40 = 4;
    pAVar1->field_44 = 0;
    pAVar1->field_10 = DAT_006f8fd8;
    pAVar1->field_14 = -DAT_006fcdd8;
    pAVar1->field_61 = '\x01';
    pAVar1->field_4C = 3;
    pAVar1->field_50 = 3;
    pAVar1->field_65 = -1;
    pAVar1->field_58 = 1;

    pAVar1 = sub_534370(0x30, 0x15);
    pAVar1->sub_533150(pAVar1->field_6C - 1, 1);

    pAVar1 = sub_534370(0x3f, 0x11);
    pAVar1->field_38 = 0x11;
    pAVar1->field_34 = 3;
    pAVar1->field_3C = 0x11;
    pAVar1->field_40 = 4;
    pAVar1->field_44 = 0;
    pAVar1->field_10 = DAT_006f8fd8;
    pAVar1->field_14 = -DAT_006fcdd8;
    pAVar1->field_4C = 3;
    pAVar1->field_50 = 3;
    pAVar1->field_61 = '\x01';
    pAVar1->field_58 = 1;

    pAVar1 = sub_534370(0x2b, 0x17);
    pAVar1->sub_533150(1, pAVar1->field_6C + -1);
    pAVar1->field_34 = 4;
    pAVar1->field_64 = '\x01';
    pAVar1->field_38 = 0x2c;
    pAVar1->field_3C = 0x2c;
    pAVar1->field_40 = 4;
    pAVar1->field_44 = 0;
    pAVar1->field_10 = DAT_006f8fd8;
    pAVar1->field_14 = -DAT_006fcdd8;
    pAVar1->field_61 = '\x01';
    pAVar1->field_4C = 3;
    pAVar1->field_50 = 3;
    pAVar1->field_65 = -1;
    pAVar1->field_58 = 1;

    pAVar1 = sub_534370(0x2c, 0x17);
    pAVar1->sub_533150(pAVar1->field_6C - 1, 1);

    pAVar1 = sub_534370(0x9d, 7);
    pAVar1->field_4C = 2;
    pAVar1->field_34 = 3;
    pAVar1->field_14 = -DAT_006fcdd8;
    pAVar1->field_38 = 7;
    pAVar1->field_3C = 7;
    pAVar1->field_40 = 4;
    pAVar1->field_44 = 0;
    pAVar1->field_61 = '\x01';
    pAVar1->field_58 = 1;

    pAVar1 = sub_534370(0x9e, 1);
    pAVar1->field_34 = 3;
    pAVar1->field_4C = 2;
    pAVar1->field_14 = -DAT_006fcdd8;
    pAVar1->field_38 = 1;
    pAVar1->field_3C = 1;
    pAVar1->field_44 = 0;
    pAVar1->field_61 = '\x01';
    pAVar1->field_58 = 1;
    pAVar1 = sub_534360(0x19);
    pAVar1->field_28 = 1;
    pAVar1->field_8 = DAT_006fce10;
    return;
}

STUB_FUNC(0x533b30)
s32 Phi_8CA8::sub_533B30()
{
    return 0;
}

STUB_FUNC(0x533c90)
u8 Phi_8CA8::sub_533C90()
{
    return 0;
}

MATCH_FUNC(0x534270)
void Phi_8CA8::sub_534270()
{
    sub_5342F0(287);
    sub_5342F0(288);
    sub_5342F0(289);
    sub_5342F0(290);
    sub_5342F0(291);
    sub_5342F0(292);
    sub_5342F0(293);
}

MATCH_FUNC(0x5342d0)
void Phi_8CA8::sub_5342D0()
{
    field_8CA4 = sub_534360(112)->field_1E;
}

MATCH_FUNC(0x5342f0)
void Phi_8CA8::sub_5342F0(s32 idx)
{
    Phi_74* v2 = sub_534360(idx);
    u16 v3 = gGtx_0x106C_703DD4->convert_sprite_pal_5AA460(4, v2->field_1E);
    sprite_index* psprite_index = gGtx_0x106C_703DD4->get_sprite_index_5AA440(v3);
    psprite_index->sub_5ABAA0(1);
}

MATCH_FUNC(0x534330)
void Phi_8CA8::sub_534330()
{
    sub_533300();
    sub_533360();
    sub_533420();
    sub_533B30();
    sub_533C90();
    sub_5342D0();
}

MATCH_FUNC(0x534360)
Phi_74* Phi_8CA8::sub_534360(s32 idx)
{
    return field_87F4[idx];
}

MATCH_FUNC(0x534370)
Phi_74* Phi_8CA8::sub_534370(s32 idx1, s32 idx2)
{
    Phi_74* result = &field_4[field_0_next_idx];
    field_87F4[idx1] = result;
    ++field_0_next_idx;
    memcpy(result, field_87F4[idx2], sizeof(Phi_74));
    result->field_24_idx = idx1;
    return result;
}

MATCH_FUNC(0x5343c0)
Phi_74* Phi_8CA8::sub_5343C0(s32 idx)
{
    Phi_74* result = &field_4[field_0_next_idx];
    field_87F4[idx] = result;
    ++field_0_next_idx;
    result->field_24_idx = idx;
    return result;
}

MATCH_FUNC(0x5343f0)
Phi_8CA8::Phi_8CA8()
{
    for (s32 i = 0; i < GTA2_COUNTOF(field_87F4); i++)
    {
        field_87F4[i] = 0;
    }
    field_0_next_idx = 0;
    field_8CA4 = 99;
}
