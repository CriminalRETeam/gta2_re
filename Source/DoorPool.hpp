#pragma once

#include "Door.hpp"
#include "Function.hpp"
#include "Pool.hpp"
#include "fix16.hpp"

class Object;
class DoorData;
class SpriteShape;
class Ped;
class Car;

// DoorData::mState
namespace door_state
{
enum
{
    closed_1 = 1,
    open_2 = 2,
};
} // namespace door_state

class DoorData // Contains only position, face and DoorInfo id
{
  public:
    EXPORT void PoolAllocate();
    EXPORT void Init(u8 id, u8 x, u8 y, u8 z, u32 face, u8 bDoFlip);
    EXPORT void Open(u8 bFlip);
    EXPORT void Close(u8 bDoFlip);
    EXPORT static s32 __stdcall GetOppositeFace(s32 v);

    // 9.6f 0x489640
    u8 get_z() const
    {
        return mZ;
    }

    // 9.6f 0x489630
    u8 get_y() const
    {
        return mY;
    }

    // 9.6f 0x489620
    u8 get_x() const
    {
        return mX;
    }

    s32 mState; // 0x0
    u8 mX; // 0x4
    u8 mY; // 0x5
    u8 mZ; // 0x6
    u8 mGrId; // 0x7
    s32 mFace; // 0x8
    DoorData* mpNext;
};
GTA2_ASSERT_SIZEOF_ALWAYS(DoorData, 0x10);

// 9.6f 0x44C860
inline bool Door::IsOpen() const
{
    return mPrimaryDoorData->mState == door_state::open_2;
}

// 9.6f 0x476A30
inline void Door::CloseDoors()
{
    if (mbClosing)
    {
        if (mPrimaryDoorData)
        {
            mPrimaryDoorData->Close(0);
        }
        if (mSecondaryDoorData)
        {
            mSecondaryDoorData->Close(mbDoFlip);
        }
    }
}

class DoorDataPool
{
  public:
    // inlined 0x44c830
    DoorData* Allocate()
    {
        DoorData* pNew = mPool.mpHead;
        mPool.mpHead = pNew->mpNext;
        pNew->PoolAllocate();
        return pNew;
    }

    // inlined 0x44C800
    DoorDataPool()
    {
    }

    // 9.6f 0x44C7F0
    ~DoorDataPool()
    {
        mPool.mpHead = NULL;
    }

    PoolBasic<DoorData, 44> mPool; // 0x0
};

class DoorPool
{
  public:
    STATIC_MEMBER(DoorDataPool*, sDoor10Pool);
    EXPORT DoorData* AllocDoorData(u8 gr_id, char_type x, char_type y, char_type z, s32 face, char_type bDoFlip);
    EXPORT Door* RegisterSingleDoorNoCheck(u8 gr_id, u8 x, u8 y, u8 z, u32 face, u8 flip, u8 reversed);
    EXPORT Door* RegisterDoubleDoorNoCheck(u8 gr_id, u8 x, u8 y, u8 z, s32 face, u8 flip, u8 reversed);
    EXPORT Door* RegisterSingleDoor(u8 gr_id,
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
                                              u8 reversed);
    EXPORT Door* RegisterDoubleDoor(u8 gr_id,
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
                                              u8 reversed);
    EXPORT void RegisterDoorInfo(s16 start_frame, s16 end_frame, char_type speed);
    EXPORT void TryOpenDoorForCar(Car* pCar, u8 door_idx);
    EXPORT void TryOpenDoorForPed(Ped* pPed, u8 idx);
    EXPORT Door* GetNextFreeDoor();
    EXPORT bool CheckDoorAccess(Sprite* pSprite, u8 door_idx);
    EXPORT void DoorsService();
    EXPORT DoorPool();
    EXPORT ~DoorPool();

    Door mDoors[22]; // 0x0
    u16 mCount; // 0x4D0
    s16 mUnused4D2; // 0x4D2
};
GTA2_ASSERT_SIZEOF_ALWAYS(DoorPool, 0x4D4);

EXTERN_GLOBAL(DoorPool*, gDoor_4D4);
