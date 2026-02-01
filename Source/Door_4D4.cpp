#include "Door_4D4.hpp"
#include "Garage_48.hpp"
#include "Globals.hpp"
#include "Object_5C.hpp"
#include "Ped.hpp"
#include "TileAnim_2.hpp"
#include "error.hpp"
#include "gtx_0x106C.hpp"
#include "map_0x370.hpp"

DEFINE_GLOBAL(DoorData_10_Pool*, gDoor_10_Pool_67BD28, 0x67BD28);

EXTERN_GLOBAL(Fix16, DAT_0067BBE4);
EXTERN_GLOBAL(Fix16, DAT_0067BBE8);
EXTERN_GLOBAL(Fix16, DAT_0067BA20);

MATCH_FUNC(0x49cf10)
DoorData_10* Door_4D4::sub_49CF10(u8 gr_id, char_type x, char_type y, char_type z, s32 face, char_type a6)
{
    DoorData_10* tmp = gDoor_10_Pool_67BD28->Allocate();
    tmp->sub_49c340(gr_id, x, y, z, face, a6);
    return tmp;
}

MATCH_FUNC(0x49cf50)
Door_38* Door_4D4::RegisterSingleDoorNoCheck_49CF50(u8 gr_id, u8 x, u8 y, u8 z, u32 face, u8 flip, u8 reversed)
{
    Door_38* pDVar1 = sub_49D3A0();
    field_4D0_count++;
    pDVar1->field_2A = flip;
    pDVar1->field_2B = reversed;
    pDVar1->sub_49C8D0(field_4D0_count + -1, gr_id, x, y, z, face);
    return pDVar1;
}

WIP_FUNC(0x49cfa0)
Door_38* Door_4D4::RegisterDoubleDoorNoCheck_49CFA0(u8 gr_id, u8 x, u8 y, u8 z, s32 face, u8 flip, u8 reversed)
{
    WIP_IMPLEMENTED;

    Door_38* pNewDoor; // eax
    Door_38* pDoor; // ebx
    u8 x_; // al
    Fix16 v12; // ebp
    Fix16 v13; // esi
    Fix16 v14; // edi
    char_type y_; // [esp+30h] [ebp+1Ch]
    Fix16 tmp;

    pNewDoor = sub_49D3A0();
    ++this->field_4D0_count;
    pDoor = pNewDoor;
    pNewDoor->field_2B = reversed;
    pNewDoor->field_2A = flip;
    x_ = x;
    y_ = y;
    switch (face)
    {
        case 1:
            v12 = DAT_0067BBE4;
            y_ = y - 1;
            tmp = DAT_0067BBE8;
            v13 = Fix16(x) - DAT_0067BA20;
            v14 = Fix16(y);
            break;
        case 2:
            v12 = DAT_0067BBE4;
            v13 = DAT_0067BA20 + Fix16(x + 1);
            y_ = y + 1;
            tmp = DAT_0067BBE8;
            v14 = Fix16(y + 1);
            break;
        case 3:
            v12 = DAT_0067BBE8;
            ++x;
            tmp = DAT_0067BBE4;
            v13 = Fix16(x_ + 1);
            v14 = Fix16(y) - DAT_0067BA20;
            break;
        case 4:
            v12 = DAT_0067BBE8;
            v13 = Fix16(x);
            tmp = DAT_0067BBE4;
            v14 = DAT_0067BA20 + Fix16(y + 1);
            --x;
            break;
        default:
            v13 = tmp;
            v14 = tmp;
            v12 = tmp;
            break;
    }
    pDoor->sub_49CA50(gr_id, x_, y, z, face);
    pDoor->sub_49CA50(gr_id, x, y_, z, face);
    pDoor->sub_49CC00(pDoor->field_0_primary_door_data, 1, (u8)(this->field_4D0_count) - 1, v13, v14, Fix16(z), v12, tmp);
    return pDoor;
}

MATCH_FUNC(0x49d170)
Door_38* Door_4D4::RegisterSingleDoor_49D170(u8 gr_id,
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
    this_00->sub_49CAC0(this_00->field_0_primary_door_data, 1, field_4D0_count + -1, check_x, check_y, check_z, check_width, check_height);

    return this_00;
}

MATCH_FUNC(0x49d1f0)
Door_38* Door_4D4::RegisterDoubleDoor_49D1F0(u8 gr_id,
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
    this_00->sub_49CC00(this_00->field_0_primary_door_data, 1, field_4D0_count - 1, check_x, check_y, check_z, check_width, check_height);
    return this_00;
}

MATCH_FUNC(0x49d2d0)
void Door_4D4::RegisterDoorInfo_49D2D0(s16 start_frame, s16 end_frame, char_type speed)
{
    DoorAnimInfo_A* psVar3 = &word_67BB38[0];
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
    gGtx_0x106C_703DD4->SetTileRemap_5AA930(sVar2, psVar3->field_0_start_frame);
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

inline bool Door_38_inline_unknown(Door_38* pDoor)
{
    if (pDoor->field_0_primary_door_data->field_0 == 2) // TODO: Use sub_44C860()
    {
        if (pDoor->field_24 == 3 || pDoor->field_24 == 0)
        {
            return true;
        }
    }
    return false;
}

// https://decomp.me/scratch/dGqMg
MATCH_FUNC(0x49d3c0)
char_type Door_4D4::sub_49D3C0(Sprite* pSprite, u8 door_idx)
{
    Car_BC* pCar = pSprite->AsCar_40FEB0();
    if (pCar)
    {

        if (!Door_38_inline_unknown(&field_0[door_idx]))
        {
            if (!field_0[door_idx].sub_49C6D0(pCar))
            {
                return 1;
            }
        }

        return 0;
    }

    Char_B4* pB4 = pSprite->AsCharB4_40FEA0();
    if (pB4)
    {
        if (!Door_38_inline_unknown(&field_0[door_idx]))
        {
            if (!field_0[door_idx].sub_49C7F0(pB4->field_7C_pPed))
            {
                return 1;
            }
        }

        return 0;
    }

    return 1;
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
        gDoor_10_Pool_67BD28 = new DoorData_10_Pool();
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
void DoorData_10::PoolAllocate()
{
    field_0 = 0;
}

MATCH_FUNC(0x4DEEB0)
s32 DoorData_10::sub_4DEEB0(s32 v)
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
void DoorData_10::sub_49c340(u8 gr_id, u8 x, u8 y, u8 z, u32 face, u8 a6)
{
    NOT_IMPLEMENTED;
}

MATCH_FUNC(0x49c4e0)
void DoorData_10::sub_49C4E0(u8 a1)
{
    DoorAnimInfo_A* tmp = &word_67BB38[field_7_gr_id];
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
void DoorData_10::sub_49C590(u8 a1)
{
    DoorAnimInfo_A* tmp = &word_67BB38[field_7_gr_id];
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