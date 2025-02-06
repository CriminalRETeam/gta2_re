#include "Door_4D4.hpp"
#include "Globals.hpp"
#include "Object_5C.hpp"
#include "Ped.hpp"
#include "error.hpp"
#include "gtx_0x106C.hpp"

EXPORT_VAR s32 dword_67BBE0;
GLOBAL(dword_67BBE0, 0x67BBE0);

EXPORT_VAR Door_A word_67BB38[5];
GLOBAL(word_67BB38, 0x67BB38);

EXPORT_VAR Door_2C4* gDoor_2C4_67BD28;
GLOBAL(gDoor_2C4_67BD28, 0x67BD28);

EXPORT_VAR Door_4D4* gDoor_4D4_67BD2C;
GLOBAL(gDoor_4D4_67BD2C, 0x67BD2C);

EXPORT_VAR Fix16 DAT_0067BA20;
GLOBAL(DAT_0067BA20, 0x67BA20);

EXPORT_VAR Fix16 DAT_0067BBE4;
GLOBAL(DAT_0067BBE4, 0x67BBE4);

EXPORT_VAR Fix16 DAT_0067BBE8;
GLOBAL(DAT_0067BBE8, 0x67BBE8);

EXPORT_VAR Ang16 DAT_0067BA38;
GLOBAL(DAT_0067BA38, 0x67BA38);

EXPORT_VAR Ang16 DAT_0067BB2C;
GLOBAL(DAT_0067BB2C, 0x67BB2C);

EXPORT_VAR Ang16 DAT_0067BD18;
GLOBAL(DAT_0067BD18, 0x67BD18);

MATCH_FUNC(0x49c640)
Door_38::Door_38()
{
    field_0 = 0;
    field_4 = 0;
    field_8 = 0;
    field_C = 0;
    field_20_state = 0;
    field_24 = 0;
    field_10 = 0;
    field_18 = 0;
    field_28 = 1;
    field_2C = 1;
    field_1C = 0;
    field_1E = 0;
    field_14 = 0;
    field_29 = 1;
    field_2A = 0;
    field_2B = 0;
    field_30 = dword_67BBE0;
    field_34 = dword_67BBE0;
    field_2D = 0;
}

MATCH_FUNC(0x49c690)
Door_38::~Door_38()
{
    field_0 = 0;
    field_4 = 0;
    field_8 = 0;
    field_C = 0;
}

STUB_FUNC(0x49c6a0)
s32 Door_38::sub_49C6A0(s32 a1)
{
    return 0;
}

STUB_FUNC(0x49c6d0)
char_type Door_38::sub_49C6D0(u32* a2)
{
    return 0;
}

// https://decomp.me/scratch/XlGeq asm differ bugged
STUB_FUNC(0x49c7f0)
bool Door_38::sub_49C7F0(Ped* a2)
{
    if (field_20_state == 1)
    {
        return a2->field_15C_player_weapons != 0;
    }

    if (field_20_state == 5)
    {
        if (field_10)
        {
            if (a2 == field_10)
            {
                return a2->field_200 == field_14;
            }
        }
    }
    return false;
}

STUB_FUNC(0x49c840)
s32 Door_38::sub_49C840()
{
    return 0;
}

MATCH_FUNC(0x49c870)
void Door_38::sub_49C870(u32* a2)
{
    if (field_29)
    {
        if (sub_49C6D0(a2))
        {
            if (field_2C)
            {
                sub_49C840();
            }
        }
    }
}

MATCH_FUNC(0x49c8a0)
void Door_38::sub_49C8A0(Ped* a2)
{
    if (field_29)
    {
        if (sub_49C7F0(a2))
        {
            if (field_2C)
            {
                sub_49C840();
            }
        }
    }
}

MATCH_FUNC(0x49c8d0)
void Door_38::sub_49C8D0(u8 a1, u8 a2, u8 a3, u8 a4, u8 a5, s32 a6)
{
    Fix16 iVar5;
    Fix16 iVar8;

    sub_49CA50(a2, a3, a4, a5, a6);

    Fix16 _param_4 = Fix16(a3) + DAT_0067BA20;
    Fix16 iVar6 = Fix16(a4) + DAT_0067BA20;

    switch (a6)
    {
        case 1:
            iVar5 = Fix16(a3) - DAT_0067BA20;
            iVar8 = Fix16(a4) + DAT_0067BA20;
            if (this->field_2B)
            {
                _param_4 -= DAT_0067BBE4;
            }
            break;
        case 2:
            iVar5 = Fix16(a3) + DAT_0067BBE4 + DAT_0067BA20;
            iVar8 = Fix16(a4) + DAT_0067BA20;
            if (this->field_2B)
            {
                _param_4 += DAT_0067BBE4;
            }
            break;
        case 3:
            iVar5 = Fix16(a3) + DAT_0067BA20;
            iVar8 = Fix16(a4) - DAT_0067BA20;
            if (this->field_2B)
            {
                iVar6 -= DAT_0067BBE4;
            }
            break;
        case 4:
            iVar5 = Fix16(a3) + DAT_0067BA20;
            iVar8 = Fix16(a4) + DAT_0067BBE4 + DAT_0067BA20;
            if (this->field_2B)
            {
                iVar6 += DAT_0067BBE4;
            }
            break;
    }

    field_28 = 1;
    field_2C = 1;

    field_1E = field_1C = (word_67BB38[a2].field_2_end_frame - word_67BB38[a2].field_0_start_frame) * word_67BB38[a2].field_8_speed;

    field_C = gObject_5C_6F8F84->sub_5299B0(0xa7, iVar5, iVar8, a5, DAT_0067BD18);
    field_C->set_field_26(a1);

    field_8 = gObject_5C_6F8F84->sub_5299B0(0xa9, _param_4, iVar6, a5, DAT_0067BD18);
    field_8->set_field_26(a1);
    field_30 = _param_4;
    field_34 = iVar6;
}

MATCH_FUNC(0x49ca50)
void Door_38::sub_49CA50(u8 a1, char_type a2, char_type a3, char_type a4, s32 a5)
{
    if (!field_0)
    {
        field_0 = gDoor_4D4_67BD2C->sub_49CF10(a1, a2, a3, a4, a5, 0);
        return;
    }

    if (!field_4)
    {
        field_4 = gDoor_4D4_67BD2C->sub_49CF10(a1, a2, a3, a4, a5, field_2A);
    }
}

MATCH_FUNC(0x49cac0)
void Door_38::sub_49CAC0(Door_10* a1, char_type a2, u8 a3, Fix16 a4, Fix16 a5, Fix16 a6, Fix16 a7, Fix16 a8)
{
    Fix16 z(a1->field_6);
    Fix16 x = Fix16(a1->field_4) + DAT_0067BA20;
    Fix16 y = Fix16(a1->field_5) + DAT_0067BA20;
    switch (a1->field_8)
    {
        case 1:
            if (this->field_2B)
            {
                x -= DAT_0067BBE4;
            }
            break;
        case 2:
            if (this->field_2B)
            {
                x += DAT_0067BBE4;
            }
            break;
        case 3:
            if (this->field_2B)
            {
                y -= DAT_0067BBE4;
            }
            break;
        case 4:
            if (this->field_2B)
            {
                y += DAT_0067BBE4;
            }
            break;
    }
    field_28 = 1;
    field_2C = 1;
    u16 sVar1 = (word_67BB38[a1->field_7].field_2_end_frame - word_67BB38[a1->field_7].field_0_start_frame);
    field_1E = field_1C = sVar1 * word_67BB38[a1->field_7].field_8_speed + 0x28;
    if (a2)
    {
        field_C = gObject_5C_6F8F84->sub_529950(0xa7, a4, a5, a6, DAT_0067BD18, a7, a8, DAT_0067BBE4);
        field_C->set_field_26(a3);
    }
    field_8 = gObject_5C_6F8F84->sub_5299B0(0xa9, x, y, z, DAT_0067BD18);
    field_8->set_field_26(a3);
    field_30 = x;
    field_34 = y;
}

MATCH_FUNC(0x49cc00)
void Door_38::sub_49CC00(Door_10* a1, char_type a2, u8 a3, Fix16 a4, Fix16 a5, Fix16 a6, Fix16 a7, Fix16 a8)
{
    Ang16 local_c;
    Fix16 z(a1->field_6);
    Fix16 x = Fix16(a1->field_4) + DAT_0067BA20;
    Fix16 y = Fix16(a1->field_5) + DAT_0067BA20;

    switch (a1->field_8)
    {
        case 3:
            x += DAT_0067BA20;
            local_c = DAT_0067BB2C;
            if (this->field_2B)
            {
                y -= DAT_0067BBE4;
            }
            break;
        case 1:
            y -= DAT_0067BA20;
            local_c = DAT_0067BD18;
            if (this->field_2B)
            {
                x -= DAT_0067BBE4;
            }
            break;
        case 2:
            y += DAT_0067BA20;
            local_c = DAT_0067BD18;
            if (this->field_2B)
            {
                x += DAT_0067BBE4;
            }
            break;
        case 4:
            x -= DAT_0067BA20;
            local_c = DAT_0067BA38;
            if (this->field_2B)
            {
                y += DAT_0067BBE4;
            }
            break;
    }
    field_28 = 1;
    field_2C = 1;
    u16 sVar1 = (word_67BB38[a1->field_7].field_2_end_frame - word_67BB38[a1->field_7].field_0_start_frame);
    field_1E = field_1C = sVar1 * word_67BB38[a1->field_7].field_8_speed + 0x28;
    if (a2)
    {
        field_C = gObject_5C_6F8F84->sub_529950(0xa7, a4, a5, a6, DAT_0067BD18, a7, a8, DAT_0067BBE4);
        field_C->set_field_26(a3);
    }
    field_8 = gObject_5C_6F8F84->sub_529950(0xa9, x, y, z, local_c, DAT_0067BBE4, DAT_0067BBE8, DAT_0067BBE4);
    field_8->set_field_26(a3);
    field_30 = x;
    field_34 = y;
}

STUB_FUNC(0x49cd90)
void Door_38::sub_49CD90()
{
}

MATCH_FUNC(0x49ce90)
char_type Door_38::sub_49CE90()
{
    if (field_2D)
    {
        gObject_5C_6F8F84->sub_5299F0(0x117, 0x32, field_30, field_34, field_0->field_6);
        field_2D = 0;
    }

    if (field_29)
    {
        sub_49CD90();
    }
    field_28 = 1;
    return 0;
}

STUB_FUNC(0x49cf10)
Door_10* Door_4D4::sub_49CF10(u8 a1, char_type a2, char_type a3, char_type a4, s32 a5, char_type a6)
{
    return 0;
}

MATCH_FUNC(0x49cf50)
Door_38* Door_4D4::sub_49CF50(u8 a1, char_type a2, s32 a3, char_type a4, s32 a5, char_type a6, char_type a7)
{
    Door_38* pDVar1 = sub_49D3A0();
    field_4D0_count++;
    pDVar1->field_2A = a6;
    pDVar1->field_2B = a7;
    pDVar1->sub_49C8D0(field_4D0_count + -1, a1, a2, a3, a4, a5);
    return pDVar1;
}

STUB_FUNC(0x49cfa0)
Door_38* Door_4D4::sub_49CFA0(u8 a1, u8 a2, u8 a3, u8 a4, s32 a5, s32 a7, char_type a8)
{
    return 0;
}

MATCH_FUNC(0x49d170)
Door_38* Door_4D4::sub_49D170(u8 a1,
                              char_type a2,
                              char_type a3,
                              char_type a4,
                              s32 a5,
                              Fix16 a6,
                              Fix16 a7,
                              Fix16 a8,
                              Fix16 a9,
                              Fix16 a10,
                              char_type a11,
                              char_type a12)
{
    Door_38* this_00 = sub_49D3A0();
    field_4D0_count++;
    this_00->field_2A = a11;
    this_00->field_2B = a12;
    this_00->sub_49CA50(a1, a2, a3, a4, a5);
    this_00->sub_49CAC0(this_00->field_0, 1, field_4D0_count + -1, a6, a7, a8, a9, a10);

    return this_00;
}

MATCH_FUNC(0x49d1f0)
Door_38* Door_4D4::sub_49D1F0(u8 a1,
                              char_type a2,
                              char_type a3,
                              char_type a4,
                              s32 a5,
                              Fix16 a6,
                              Fix16 a7,
                              Fix16 a8,
                              Fix16 a9,
                              Fix16 a10,
                              char_type a11,
                              char_type a12)
{
    Door_38* this_00 = sub_49D3A0();
    field_4D0_count++;
    this_00->field_2A = a11;
    this_00->field_2B = a12;
    a12 = a2;
    a11 = a3;
    switch (a5)
    {
        case 2:
            a11 = a3 + 1;
            break;
        case 3:
            a12 = a2 + 1;
            break;
        case 1:
            a11 = a3 - 1;
            break;
        case 4:
            a12 = a2 - 1;
            break;
    }
    this_00->sub_49CA50(a1, a2, a3, a4, a5);
    this_00->sub_49CA50(a1, a12, a11, a4, a5);
    this_00->sub_49CC00(this_00->field_0, 1, field_4D0_count - 1, a6, a7, a8, a9, a10);
    return this_00;
}

MATCH_FUNC(0x49d2d0)
void Door_4D4::sub_49D2D0(s16 start_frame, s16 end_frame, char_type speed)
{
    Door_A* psVar3 = &word_67BB38[0];
    u8 bVar1 = 0;
    do
    {
        if (psVar3->field_0_start_frame == 0 && psVar3->field_2_end_frame == 0)
        {
            break;
        }
        psVar3++;
        bVar1++;
    } while (bVar1 < 5);

    psVar3->field_0_start_frame = start_frame;
    psVar3->field_2_end_frame = end_frame;
    psVar3->field_8_speed = speed;
    s16 sVar2 = gGtx_0x106C_703DD4->sub_5AA890();
    psVar3->field_4 = sVar2;
    gGtx_0x106C_703DD4->sub_5AA930(sVar2, psVar3->field_0_start_frame);
    sVar2 = gGtx_0x106C_703DD4->sub_5AA890();
    psVar3->field_6 = sVar2;
}

MATCH_FUNC(0x49d340)
void Door_4D4::sub_49D340(u32* a2, u8 a3)
{
    field_0[a3].sub_49C870(a2);
}

MATCH_FUNC(0x49d370)
void Door_4D4::sub_49D370(Ped* a2, u8 idx)
{
    field_0[idx].sub_49C8A0(a2);
}

MATCH_FUNC(0x49d3a0)
Door_38* Door_4D4::sub_49D3A0()
{
    return &field_0[field_4D0_count];
}

STUB_FUNC(0x49d3c0)
char_type Door_4D4::sub_49D3C0(s32 a2, u8 a3)
{
    return 0;
}

MATCH_FUNC(0x49d460)
void Door_4D4::sub_49D460()
{
    for (u16 i = 0; i < field_4D0_count; ++i)
    {
        field_0[i].sub_49CE90();
    }
}

MATCH_FUNC(0x49d4a0)
Door_4D4::Door_4D4()
{
    if (!gDoor_2C4_67BD28)
    {
        gDoor_2C4_67BD28 = new Door_2C4();
        if (!gDoor_2C4_67BD28)
        {
            FatalError_4A38C0(0x20, "C:\\Splitting\\Gta2\\Source\\door.cpp", 1194);
        }
    }
    memset(word_67BB38, 0, sizeof(word_67BB38));
    this->field_4D0_count = 0;
    this->field_4D2 = 205;
}

STUB_FUNC(0x49d570)
Door_4D4::~Door_4D4()
{
    if (gDoor_2C4_67BD28)
    {
        GTA2_DELETE_AND_NULL(gDoor_2C4_67BD28);
    }
}
