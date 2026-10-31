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

DEFINE_STATIC_MEMBER(DoorDataPool*, DoorPool, sDoor10Pool, 0x67BD28);

EXTERN_GLOBAL(Fix16, kFpOne_67BBE4);
EXTERN_GLOBAL(Fix16, kFpTwo_67BBE8);
EXTERN_GLOBAL(Fix16, kFpHalf_67BA20);

MATCH_FUNC(0x49c320)
void DoorData::PoolAllocate()
{
    mState = 0;
}

MATCH_FUNC(0x49c340)
void DoorData::Init(u8 id, u8 x, u8 y, u8 z, u32 face, u8 bDoFlip)
{

    gmp_block_info blockData;

    this->mX = x;
    this->mY = y;
    this->mZ = z;

    this->mFace = face;
    this->mGrId = id;
    this->mState = door_state::closed_1;

    const u8& anim_id = id;
    DoorAnimInfo* pInfo = &gDoorAnimInfo[anim_id];
    gGtx_0x106C->SetTileRemap(pInfo->mInternalTileIdx, pInfo->mStartFrame);

    u16 tile_word = pInfo->mInternalTileIdx | (block_face::wall | block_face::bullet_wall | block_face::flat);
    if (bDoFlip)
    {
        tile_word |= block_face::flip;
    }

    if (gMap_0x370->GetBlock(this->mX, this->mY, this->mZ))
    {
        gMap_0x370->ChangeBlock(this->mX, this->mY, this->mZ, this->mFace, tile_word);
        gMap_0x370->ChangeBlock(this->mX, this->mY, this->mZ, GetOppositeFace(this->mFace), pInfo->mInternalTileIdx);
    }
    else
    {
        blockData.init_44C840();

        // Remaining diff: the original stores tile_word before loading the tile idx in each case
        switch (mFace)
        {
            case 1:
                blockData.mLeft = tile_word;
                blockData.mRight = pInfo->mInternalTileIdx;
                break;
            case 2:
                blockData.mRight = tile_word;
                blockData.mLeft = pInfo->mInternalTileIdx;
                break;
            case 3:
                blockData.mTop = tile_word;
                blockData.mBottom = pInfo->mInternalTileIdx;
                break;
            case 4:
                blockData.mBottom = tile_word;
                blockData.mTop = pInfo->mInternalTileIdx;
                break;
            default:
                break;
        }
        gMap_0x370->AddNewBlock(this->mX, this->mY, this->mZ, &blockData);
    }
}

MATCH_FUNC(0x49c4e0)
void DoorData::Open(u8 bFlip)
{
    DoorAnimInfo* pAnimInfo = &gDoorAnimInfo[mGrId];
    if (mState != door_state::open_2)
    {
        mState = door_state::open_2;
        s16 tile_idx = pAnimInfo->mOpenInternalTileIdx | block_face::flat;
        if (bFlip)
        {
            tile_idx |= block_face::flip;
        }
        gMap_0x370->ChangeBlock(mX, mY, mZ, mFace, tile_idx);
        gMap_0x370->ChangeBlock(mX, mY, mZ, GetOppositeFace(mFace), pAnimInfo->mOpenInternalTileIdx);
        gTileAnim_2->AddAnim(pAnimInfo->mOpenInternalTileIdx, pAnimInfo->mStartFrame, pAnimInfo->mEndFrame, pAnimInfo->mSpeed8, 1);
    }
}

MATCH_FUNC(0x49c590)
void DoorData::Close(u8 bDoFlip)
{
    DoorAnimInfo* pAnimInfo = &gDoorAnimInfo[mGrId];
    if (mState != door_state::closed_1)
    {
        mState = door_state::closed_1;
        s16 block_side_word = pAnimInfo->mInternalTileIdx | (block_face::wall | block_face::bullet_wall | block_face::flat);
        if (bDoFlip)
        {
            block_side_word |= block_face::flip;
        }
        gMap_0x370->ChangeBlock(mX, mY, mZ, mFace, block_side_word);
        gMap_0x370->ChangeBlock(mX, mY, mZ, GetOppositeFace(mFace), pAnimInfo->mInternalTileIdx);
        gTileAnim_2->AddAnim(pAnimInfo->mInternalTileIdx, pAnimInfo->mEndFrame, pAnimInfo->mStartFrame, pAnimInfo->mSpeed8, 1);
    }
}

MATCH_FUNC(0x49cf10)
DoorData* DoorPool::AllocDoorData(u8 gr_id, char_type x, char_type y, char_type z, s32 face, char_type bDoFlip)
{
    DoorData* pDoorData = sDoor10Pool->Allocate();
    pDoorData->Init(gr_id, x, y, z, face, bDoFlip);
    return pDoorData;
}

MATCH_FUNC(0x49cf50)
Door* DoorPool::RegisterSingleDoorNoCheck(u8 gr_id, u8 x, u8 y, u8 z, u32 face, u8 flip, u8 reversed)
{
    Door* pDoor = GetNextFreeDoor();
    mCount++;
    pDoor->mbDoFlip = flip;
    pDoor->mbReversed = reversed;
    pDoor->InitSingleNoCheck(mCount + -1, gr_id, x, y, z, face);
    return pDoor;
}

// 9.6f 0x44D430 ?
MATCH_FUNC(0x49cfa0)
Door* DoorPool::RegisterDoubleDoorNoCheck(u8 gr_id, u8 x, u8 y, u8 z, s32 face, u8 flip, u8 reversed)
{
    Door* pDoor;
    u8 x2;
    u8 y2;
    Fix16 check_z;
    Fix16 check_x;
    Fix16 check_y;
    Fix16 check_width;
    Fix16 check_height;

    pDoor = GetNextFreeDoor();
    mCount++;
    pDoor->mbDoFlip = flip;
    pDoor->mbReversed = reversed;

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
    pDoor->AddDoorData(gr_id, x, y, z, face);
    pDoor->AddDoorData(gr_id, x2, y2, z, face);
    pDoor->InitDouble(pDoor->mPrimaryDoorData, 1, mCount - 1, check_x, check_y, check_z, check_width, check_height);
    return pDoor;
}

MATCH_FUNC(0x49d170)
Door* DoorPool::RegisterSingleDoor(u8 gr_id,
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
    Door* pDoor = GetNextFreeDoor();
    mCount++;
    pDoor->mbDoFlip = flip;
    pDoor->mbReversed = reversed;
    pDoor->AddDoorData(gr_id, x, y, z, face);
    pDoor->InitSingle(pDoor->mPrimaryDoorData, 1, mCount + -1, check_x, check_y, check_z, check_width, check_height);

    return pDoor;
}

MATCH_FUNC(0x49d1f0)
Door* DoorPool::RegisterDoubleDoor(u8 gr_id,
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
    Door* pDoor = GetNextFreeDoor();
    mCount++;
    pDoor->mbDoFlip = flip;
    pDoor->mbReversed = reversed;
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
    pDoor->AddDoorData(gr_id, x, y, z, face);
    pDoor->AddDoorData(gr_id, reversed, flip, z, face);
    pDoor->InitDouble(pDoor->mPrimaryDoorData, 1, mCount - 1, check_x, check_y, check_z, check_width, check_height);
    return pDoor;
}

inline bool IsDoorOpenWithoutAutoClose(Door* pDoor)
{
    if (pDoor->IsOpen_44C860())
    {
        if (pDoor->mCloseType == door_close_type::close_never || pDoor->mCloseType == door_close_type::none)
        {
            return true;
        }
    }
    return false;
}

MATCH_FUNC(0x49d2d0)
void DoorPool::RegisterDoorInfo(s16 start_frame, s16 end_frame, char_type speed)
{
    DoorAnimInfo* pAnimInfo = &gDoorAnimInfo[0];
    u8 idx = 0;
    do
    {
        if (pAnimInfo->mStartFrame == 0 && pAnimInfo->mEndFrame == 0)
        {
            break;
        }
        pAnimInfo++;
        idx++;
    } while (idx < 5);

    pAnimInfo->mStartFrame = start_frame;
    pAnimInfo->mEndFrame = end_frame;
    pAnimInfo->mSpeed8 = speed;
    s16 reserved_tile_idx = gGtx_0x106C->GetFirstFreeReservedTileIdx();
    pAnimInfo->mInternalTileIdx = reserved_tile_idx;
    gGtx_0x106C->SetTileRemap(reserved_tile_idx, pAnimInfo->mStartFrame);
    reserved_tile_idx = gGtx_0x106C->GetFirstFreeReservedTileIdx();
    pAnimInfo->mOpenInternalTileIdx = reserved_tile_idx;
}

MATCH_FUNC(0x49d340)
void DoorPool::TryOpenDoorForCar(Car* pCar, u8 door_idx)
{
    mDoors[door_idx].TryOpenForCar(pCar);
}

MATCH_FUNC(0x49d370)
void DoorPool::TryOpenDoorForPed(Ped* pPed, u8 idx)
{
    mDoors[idx].TryOpenForPed(pPed);
}

MATCH_FUNC(0x49d3a0)
Door* DoorPool::GetNextFreeDoor()
{
    return &mDoors[mCount];
}

// https://decomp.me/scratch/dGqMg
MATCH_FUNC(0x49d3c0)
bool DoorPool::CheckDoorAccess(Sprite* pSprite, u8 door_idx)
{
    Car* pCar = pSprite->AsCar_40FEB0();
    if (pCar)
    {

        if (!IsDoorOpenWithoutAutoClose(&mDoors[door_idx]))
        {
            if (!mDoors[door_idx].CanOpen(pCar))
            {
                return true;
            }
        }

        return false;
    }

    Char* pChar = pSprite->AsCharB4_40FEA0();
    if (pChar)
    {
        if (!IsDoorOpenWithoutAutoClose(&mDoors[door_idx]))
        {
            // 9.6f: Char::get_ped_433A20 (inlined, using it changes the code)
            if (!mDoors[door_idx].CanOpenForPed(pChar->mpPed))
            {
                return true;
            }
        }

        return false;
    }

    return true;
}

MATCH_FUNC(0x49d460)
void DoorPool::DoorsService()
{
    for (u16 i = 0; i < mCount; ++i)
    {
        mDoors[i].Service();
    }
}

MATCH_FUNC(0x49d4a0)
DoorPool::DoorPool()
{
    if (!sDoor10Pool)
    {
        sDoor10Pool = new DoorDataPool();
        if (!sDoor10Pool)
        {
            FatalError_4A38C0(Gta2Error::OutOfMemoryNewOperator, "C:\\Splitting\\Gta2\\Source\\door.cpp", 1194);
        }
    }
    memset(gDoorAnimInfo, 0, sizeof(gDoorAnimInfo));
    this->mCount = 0;
    this->mUnused4D2 = 205;
}

// The EH frame needs <new> kept out of the TU (see gbh_graphics.hpp)
MATCH_FUNC(0x49d570)
DoorPool::~DoorPool()
{
    if (sDoor10Pool)
    {
        GTA2_DELETE_AND_NULL(sDoor10Pool);
    }
}

MATCH_FUNC(0x4DEEB0)
s32 DoorData::GetOppositeFace(s32 v)
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