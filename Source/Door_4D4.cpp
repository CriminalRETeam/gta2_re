#include "Door_4D4.hpp"
#include "Garage_48.hpp"
#include "Globals.hpp"
#include "Object_5C.hpp"
#include "Ped.hpp"
#include "TileAnim_2.hpp"
#include "error.hpp"
#include "gtx_0x106C.hpp"
#include "map_0x370.hpp"

DEFINE_GLOBAL(Door_10_Pool*, gDoor_10_Pool_67BD28, 0x67BD28);

MATCH_FUNC(0x49cf10)
Door_10* Door_4D4::sub_49CF10(u8 a1, char_type a2, char_type a3, char_type a4, s32 a5, char_type a6)
{
    Door_10* tmp = gDoor_10_Pool_67BD28->Allocate();
    tmp->sub_49c340(a1, a2, a3, a4, a5, a6);
    return tmp;
}

MATCH_FUNC(0x49cf50)
Door_38* Door_4D4::sub_49CF50(u8 gr_id, u8 x, u8 y, u8 z, u32 face, u8 flip, u8 reversed)
{
    Door_38* pDVar1 = sub_49D3A0();
    field_4D0_count++;
    pDVar1->field_2A = flip;
    pDVar1->field_2B = reversed;
    pDVar1->sub_49C8D0(field_4D0_count + -1, gr_id, x, y, z, face);
    return pDVar1;
}

STUB_FUNC(0x49cfa0)
Door_38* Door_4D4::sub_49CFA0(u8 gr_id, u8 x, u8 y, u8 z, s32 face, u8 flip, u8 reversed)
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x49d170)
Door_38* Door_4D4::sub_49D170(u8 gr_id,
                              u8 x,
                              u8 y,
                              u8 z,
                              s32 face,
                              Fix16 check_x,
                              Fix16 check_y,
                              Fix16 check_z,
                              Fix16 check_width,
                              Fix16 check_height,
                              u8 flip,
                              u8 reversed)
{
    Door_38* this_00 = sub_49D3A0();
    field_4D0_count++;
    this_00->field_2A = flip;
    this_00->field_2B = reversed;
    this_00->sub_49CA50(gr_id, x, y, z, face);
    this_00->sub_49CAC0(this_00->field_0, 1, field_4D0_count + -1, check_x, check_y, check_z, check_width, check_height);

    return this_00;
}

MATCH_FUNC(0x49d1f0)
Door_38* Door_4D4::sub_49D1F0(u8 gr_id,
                              u8 x,
                              u8 y,
                              u8 z,
                              s32 face,
                              Fix16 check_x,
                              Fix16 check_y,
                              Fix16 check_z,
                              Fix16 check_width,
                              Fix16 check_height,
                              u8 flip,
                              u8 reversed)
{
    Door_38* this_00 = sub_49D3A0();
    field_4D0_count++;
    this_00->field_2A = flip;
    this_00->field_2B = reversed;
    reversed = x;
    flip = y;
    switch (face)
    {
        case 2:
            flip = y + 1;
            break;
        case 3:
            reversed = x + 1;
            break;
        case 1:
            flip = y - 1;
            break;
        case 4:
            reversed = x - 1;
            break;
    }
    this_00->sub_49CA50(gr_id, x, y, z, face);
    this_00->sub_49CA50(gr_id, reversed, flip, z, face);
    this_00->sub_49CC00(this_00->field_0, 1, field_4D0_count - 1, check_x, check_y, check_z, check_width, check_height);
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
void Door_4D4::sub_49D340(Car_BC* a2, u8 a3)
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

// https://decomp.me/scratch/dGqMg
STUB_FUNC(0x49d3c0)
char_type Door_4D4::sub_49D3C0(s32 a2, u8 a3)
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x49d460)
void Door_4D4::DoorsService_49D460()
{
    for (u16 i = 0; i < field_4D0_count; ++i)
    {
        field_0[i].Service_49CE90();
    }
}

MATCH_FUNC(0x49d4a0)
Door_4D4::Door_4D4()
{
    if (!gDoor_10_Pool_67BD28)
    {
        gDoor_10_Pool_67BD28 = new Door_10_Pool();
        if (!gDoor_10_Pool_67BD28)
        {
            FatalError_4A38C0(Gta2Error::OutOfMemoryNewOperator, "C:\\Splitting\\Gta2\\Source\\door.cpp", 1194);
        }
    }
    memset(word_67BB38, 0, sizeof(word_67BB38));
    this->field_4D0_count = 0;
    this->field_4D2 = 205;
}

// TODO: Missing SEH even with Door_38 in another TU
STUB_FUNC(0x49d570)
Door_4D4::~Door_4D4()
{
    if (gDoor_10_Pool_67BD28)
    {
        GTA2_DELETE_AND_NULL(gDoor_10_Pool_67BD28);
    }
}

MATCH_FUNC(0x49c320)
void Door_10::PoolAllocate()
{
    field_0 = 0;
}

MATCH_FUNC(0x4DEEB0)
s32 Door_10::sub_4DEEB0(s32 v)
{
    switch (v)
    {
        case 1:
            return 2;
        case 2:
            return 1;
        case 3:
            return 4;
        case 4:
            return 3;
        default:
            return 0;
    }
}

STUB_FUNC(0x49c340)
void Door_10::sub_49c340(u8 a1, u8 a2, u8 a3, u8 a4, u32 a5, u8 a6)
{
    NOT_IMPLEMENTED;
}

MATCH_FUNC(0x49c4e0)
void Door_10::sub_49C4E0(u8 a1)
{
    Door_A* tmp = &word_67BB38[field_7_gr_id];
    if (field_0 != 2)
    {
        field_0 = 2;
        s16 uVar3 = tmp->field_6 | 0x1000;
        if (a1)
        {
            uVar3 |= 0x2000;
        }
        gMap_0x370_6F6268->ChangeBlock_4E8620(field_4_x, field_5_y, field_6_z, field_8_face, uVar3);
        gMap_0x370_6F6268->ChangeBlock_4E8620(field_4_x, field_5_y, field_6_z, sub_4DEEB0(field_8_face), tmp->field_6);
        gTileAnim_2_7052C4->sub_5BC260(tmp->field_6, tmp->field_0_start_frame, tmp->field_2_end_frame, tmp->field_8_speed, 1);
    }
}

MATCH_FUNC(0x49c590)
void Door_10::sub_49C590(u8 a1)
{
    Door_A* tmp = &word_67BB38[field_7_gr_id];
    if (field_0 != 1)
    {
        field_0 = 1;
        s16 uVar3 = tmp->field_4 | 0x1C00;
        if (a1)
        {
            uVar3 |= 0x2000;
        }
        gMap_0x370_6F6268->ChangeBlock_4E8620(field_4_x, field_5_y, field_6_z, field_8_face, uVar3);
        gMap_0x370_6F6268->ChangeBlock_4E8620(field_4_x, field_5_y, field_6_z, sub_4DEEB0(field_8_face), tmp->field_4);
        gTileAnim_2_7052C4->sub_5BC260(tmp->field_4, tmp->field_2_end_frame, tmp->field_0_start_frame, tmp->field_8_speed, 1);
    }
}