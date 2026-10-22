#include "DoorPool.hpp"
#include "Garage.hpp"
#include "Globals.hpp"
#include "Object_5C.hpp"
#include "Ped.hpp"
#include "TileAnimManager.hpp"
#include "error.hpp"
#include "gtx_0x106C.hpp"
#include "map_0x370.hpp"
#include "block_face.hpp"

DEFINE_GLOBAL(DoorDataPool*, gDoor_10_Pool_67BD28, 0x67BD28);

EXTERN_GLOBAL(Fix16, kFpOne_67BBE4);
EXTERN_GLOBAL(Fix16, kFpTwo_67BBE8);
EXTERN_GLOBAL(Fix16, kFpHalf_67BA20);

MATCH_FUNC(0x49c320)
void DoorData::PoolAllocate()
{
    field_0_state = 0;
}

MATCH_FUNC(0x49c340)
void DoorData::Init_49C340(u8 id, u8 x, u8 y, u8 z, u32 face, u8 bDoFlip)
{

    gmp_block_info blockData; // [esp+10h] [ebp-Ch] BYREF

    this->field_4_x = x;
    this->field_5_y = y;
    this->field_6_z = z;

    this->field_8_face = face;
    this->field_7_gr_id = id;
    this->field_0_state = door_state::closed_1;

    const u8& anim_id = id;
    DoorAnimInfo* pInfo = &gDoorAnimInfo_67BB38[anim_id];
    gGtx_0x106C_703DD4->SetTileRemap_5AA930(pInfo->field_4_internal_tile_idx, pInfo->field_0_start_frame);

    u16 tile_word = pInfo->field_4_internal_tile_idx | (block_face::wall | block_face::bullet_wall | block_face::flat);
    if (bDoFlip)
    {
        tile_word |= block_face::flip;
    }

    if (gMap_0x370_6F6268->GetBlock_4DFE10(this->field_4_x, this->field_5_y, this->field_6_z))
    {
        gMap_0x370_6F6268->ChangeBlock_4E8620(this->field_4_x, this->field_5_y, this->field_6_z, this->field_8_face, tile_word);
        gMap_0x370_6F6268->ChangeBlock_4E8620(this->field_4_x, this->field_5_y, this->field_6_z, GetOppositeFace_4DEEB0(this->field_8_face), pInfo->field_4_internal_tile_idx);
    }
    else
    {
        blockData.init_44C840();

        // Remaining diff: the original stores tile_word before loading the tile idx in each case
        switch (field_8_face)
        {
            case 1:
                blockData.field_0_left = tile_word;
                blockData.field_2_right = pInfo->field_4_internal_tile_idx;
                break;
            case 2:
                blockData.field_2_right = tile_word;
                blockData.field_0_left = pInfo->field_4_internal_tile_idx;
                break;
            case 3:
                blockData.field_4_top = tile_word;
                blockData.field_6_bottom = pInfo->field_4_internal_tile_idx;
                break;
            case 4:
                blockData.field_6_bottom = tile_word;
                blockData.field_4_top = pInfo->field_4_internal_tile_idx;
                break;
            default:
                break;
        }
        gMap_0x370_6F6268->AddNewBlock_4E87C0(this->field_4_x, this->field_5_y, this->field_6_z, &blockData);
    }
}

MATCH_FUNC(0x49c4e0)
void DoorData::Open_49C4E0(u8 bFlip)
{
    DoorAnimInfo* tmp = &gDoorAnimInfo_67BB38[field_7_gr_id];
    if (field_0_state != door_state::open_2)
    {
        field_0_state = door_state::open_2;
        s16 tile_idx = tmp->field_6_open_internal_tile_idx | block_face::flat;
        if (bFlip)
        {
            tile_idx |= block_face::flip;
        }
        gMap_0x370_6F6268->ChangeBlock_4E8620(field_4_x, field_5_y, field_6_z, field_8_face, tile_idx);
        gMap_0x370_6F6268->ChangeBlock_4E8620(field_4_x, field_5_y, field_6_z, GetOppositeFace_4DEEB0(field_8_face), tmp->field_6_open_internal_tile_idx);
        gTileAnim_2_7052C4->AddAnim_5BC260(tmp->field_6_open_internal_tile_idx, tmp->field_0_start_frame, tmp->field_2_end_frame, tmp->field_8_speed, 1);
    }
}

MATCH_FUNC(0x49c590)
void DoorData::Close_49C590(u8 bDoFlip)
{
    DoorAnimInfo* tmp = &gDoorAnimInfo_67BB38[field_7_gr_id];
    if (field_0_state != door_state::closed_1)
    {
        field_0_state = door_state::closed_1;
        s16 block_side_word = tmp->field_4_internal_tile_idx | (block_face::wall | block_face::bullet_wall | block_face::flat);
        if (bDoFlip)
        {
            block_side_word |= block_face::flip;
        }
        gMap_0x370_6F6268->ChangeBlock_4E8620(field_4_x, field_5_y, field_6_z, field_8_face, block_side_word);
        gMap_0x370_6F6268->ChangeBlock_4E8620(field_4_x, field_5_y, field_6_z, GetOppositeFace_4DEEB0(field_8_face), tmp->field_4_internal_tile_idx);
        gTileAnim_2_7052C4->AddAnim_5BC260(tmp->field_4_internal_tile_idx, tmp->field_2_end_frame, tmp->field_0_start_frame, tmp->field_8_speed, 1);
    }
}

MATCH_FUNC(0x49cf10)
DoorData* DoorPool::AllocDoorData_49CF10(u8 gr_id, char_type x, char_type y, char_type z, s32 face, char_type bDoFlip)
{
    DoorData* tmp = gDoor_10_Pool_67BD28->Allocate();
    tmp->Init_49C340(gr_id, x, y, z, face, bDoFlip);
    return tmp;
}

MATCH_FUNC(0x49cf50)
Door* DoorPool::RegisterSingleDoorNoCheck_49CF50(u8 gr_id, u8 x, u8 y, u8 z, u32 face, u8 flip, u8 reversed)
{
    Door* pDVar1 = GetNextFreeDoor_49D3A0();
    field_4D0_count++;
    pDVar1->field_2A_bDoFlip = flip;
    pDVar1->field_2B_bReversed = reversed;
    pDVar1->InitSingleNoCheck_49C8D0(field_4D0_count + -1, gr_id, x, y, z, face);
    return pDVar1;
}

// 9.6f 0x44D430 ?
MATCH_FUNC(0x49cfa0)
Door* DoorPool::RegisterDoubleDoorNoCheck_49CFA0(u8 gr_id, u8 x, u8 y, u8 z, s32 face, u8 flip, u8 reversed)
{
    Door* pDoor;
    u8 x2;
    u8 y2;
    Fix16 check_z;
    Fix16 check_x;
    Fix16 check_y;
    Fix16 check_width;
    Fix16 check_height;

    pDoor = GetNextFreeDoor_49D3A0();
    field_4D0_count++;
    pDoor->field_2A_bDoFlip = flip;
    pDoor->field_2B_bReversed = reversed;

    x2 = x;
    y2 = y;
    check_z = Fix16(z);
    switch (face)
    {
        case 2:
            y2 = y + 1;
            check_x = Fix16(x + 1) + kFpHalf_67BA20;
            check_width = kFpOne_67BBE4;
            check_height = kFpTwo_67BBE8;
            check_y = Fix16(y + 1);
            break;
        case 3:
            x2 = x + 1;
            check_height = kFpOne_67BBE4;
            check_width = kFpTwo_67BBE8;
            check_x = Fix16(x + 1);
            check_y = Fix16(y) - kFpHalf_67BA20;
            break;
        case 1:
            y2 = y - 1;
            check_height = kFpTwo_67BBE8;
            check_width = kFpOne_67BBE4;
            check_x = Fix16(x) - kFpHalf_67BA20;
            check_y = Fix16(y);
            break;
        case 4:
            check_width = kFpTwo_67BBE8;
            check_x = Fix16(x);
            check_height = kFpOne_67BBE4;
            check_y = Fix16(y + 1) + kFpHalf_67BA20;
            x2 = x - 1;
            break;
        default:
            // The original leaves the check rect uninitialised for any other face
            break;
    }
    pDoor->AddDoorData_49CA50(gr_id, x, y, z, face);
    pDoor->AddDoorData_49CA50(gr_id, x2, y2, z, face);
    pDoor->InitDouble_49CC00(pDoor->field_0_primary_door_data, 1, field_4D0_count - 1, check_x, check_y, check_z, check_width, check_height);
    return pDoor;
}

MATCH_FUNC(0x49d170)
Door* DoorPool::RegisterSingleDoor_49D170(u8 gr_id,
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
    Door* this_00 = GetNextFreeDoor_49D3A0();
    field_4D0_count++;
    this_00->field_2A_bDoFlip = flip;
    this_00->field_2B_bReversed = reversed;
    this_00->AddDoorData_49CA50(gr_id, x, y, z, face);
    this_00->InitSingle_49CAC0(this_00->field_0_primary_door_data, 1, field_4D0_count + -1, check_x, check_y, check_z, check_width, check_height);

    return this_00;
}

MATCH_FUNC(0x49d1f0)
Door* DoorPool::RegisterDoubleDoor_49D1F0(u8 gr_id,
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
    Door* this_00 = GetNextFreeDoor_49D3A0();
    field_4D0_count++;
    this_00->field_2A_bDoFlip = flip;
    this_00->field_2B_bReversed = reversed;
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
    this_00->AddDoorData_49CA50(gr_id, x, y, z, face);
    this_00->AddDoorData_49CA50(gr_id, reversed, flip, z, face);
    this_00->InitDouble_49CC00(this_00->field_0_primary_door_data, 1, field_4D0_count - 1, check_x, check_y, check_z, check_width, check_height);
    return this_00;
}

inline bool Door_38_inline_unknown(Door* pDoor)
{
    if (pDoor->IsOpen_44C860())
    {
        if (pDoor->field_24_close_type == 3 || pDoor->field_24_close_type == 0)
        {
            return true;
        }
    }
    return false;
}

MATCH_FUNC(0x49d2d0)
void DoorPool::RegisterDoorInfo_49D2D0(s16 start_frame, s16 end_frame, char_type speed)
{
    DoorAnimInfo* psVar3 = &gDoorAnimInfo_67BB38[0];
    u8 idx = 0;
    do
    {
        if (psVar3->field_0_start_frame == 0 && psVar3->field_2_end_frame == 0)
        {
            break;
        }
        psVar3++;
        idx++;
    } while (idx < 5);

    psVar3->field_0_start_frame = start_frame;
    psVar3->field_2_end_frame = end_frame;
    psVar3->field_8_speed = speed;
    s16 reserved_tile_idx = gGtx_0x106C_703DD4->GetFirstFreeReservedTileIdx_5AA890();
    psVar3->field_4_internal_tile_idx = reserved_tile_idx;
    gGtx_0x106C_703DD4->SetTileRemap_5AA930(reserved_tile_idx, psVar3->field_0_start_frame);
    reserved_tile_idx = gGtx_0x106C_703DD4->GetFirstFreeReservedTileIdx_5AA890();
    psVar3->field_6_open_internal_tile_idx = reserved_tile_idx;
}

MATCH_FUNC(0x49d340)
void DoorPool::TryOpenDoorForCar_49D340(Car* pCar, u8 door_idx)
{
    field_0_doors[door_idx].TryOpenForCar_49C870(pCar);
}

MATCH_FUNC(0x49d370)
void DoorPool::TryOpenDoorForPed_49D370(Ped* pPed, u8 idx)
{
    field_0_doors[idx].TryOpenForPed_49C8A0(pPed);
}

MATCH_FUNC(0x49d3a0)
Door* DoorPool::GetNextFreeDoor_49D3A0()
{
    return &field_0_doors[field_4D0_count];
}

// https://decomp.me/scratch/dGqMg
MATCH_FUNC(0x49d3c0)
char_type DoorPool::CheckDoorAccess_49D3C0(Sprite* pSprite, u8 door_idx)
{
    Car* pCar = pSprite->AsCar_40FEB0();
    if (pCar)
    {

        if (!Door_38_inline_unknown(&field_0_doors[door_idx]))
        {
            if (!field_0_doors[door_idx].CanOpen_49C6D0(pCar))
            {
                return 1;
            }
        }

        return 0;
    }

    Char* pB4 = pSprite->AsCharB4_40FEA0();
    if (pB4)
    {
        if (!Door_38_inline_unknown(&field_0_doors[door_idx]))
        {
            // 9.6f: Char::get_ped_433A20 (inlined, using it changes the code)
            if (!field_0_doors[door_idx].CanOpenForPed_49C7F0(pB4->field_7C_pPed))
            {
                return 1;
            }
        }

        return 0;
    }

    return 1;
}

MATCH_FUNC(0x49d460)
void DoorPool::DoorsService_49D460()
{
    for (u16 i = 0; i < field_4D0_count; ++i)
    {
        field_0_doors[i].Service_49CE90();
    }
}

MATCH_FUNC(0x49d4a0)
DoorPool::DoorPool()
{
    if (!gDoor_10_Pool_67BD28)
    {
        gDoor_10_Pool_67BD28 = new DoorDataPool();
        if (!gDoor_10_Pool_67BD28)
        {
            FatalError_4A38C0(Gta2Error::OutOfMemoryNewOperator, "C:\\Splitting\\Gta2\\Source\\door.cpp", 1194);
        }
    }
    memset(gDoorAnimInfo_67BB38, 0, sizeof(gDoorAnimInfo_67BB38));
    this->field_4D0_count = 0;
    this->field_4D2_unused = 205;
}

// The EH frame needs <new> kept out of the TU (see gbh_graphics.hpp)
MATCH_FUNC(0x49d570)
DoorPool::~DoorPool()
{
    if (gDoor_10_Pool_67BD28)
    {
        GTA2_DELETE_AND_NULL(gDoor_10_Pool_67BD28);
    }
}

MATCH_FUNC(0x4DEEB0)
s32 DoorData::GetOppositeFace_4DEEB0(s32 v)
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