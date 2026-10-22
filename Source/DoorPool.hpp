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

// DoorData::field_0_state
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
    EXPORT void Init_49C340(u8 id, u8 x, u8 y, u8 z, u32 face, u8 bDoFlip);
    EXPORT void Open_49C4E0(u8 bFlip);
    EXPORT void Close_49C590(u8 bDoFlip);
    EXPORT static s32 __stdcall GetOppositeFace_4DEEB0(s32 v);

    u8 get_z_489640() const
    {
        return field_6_z;
    }

    u8 get_y_489630() const
    {
        return field_5_y;
    }

    u8 get_x_489620() const
    {
        return field_4_x;
    }

    s32 field_0_state;
    u8 field_4_x;
    u8 field_5_y;
    u8 field_6_z;
    u8 field_7_gr_id;
    s32 field_8_face;
    DoorData* mpNext;
};
GTA2_ASSERT_SIZEOF_ALWAYS(DoorData, 0x10);

inline bool Door::IsOpen_44C860()
{
    return field_0_primary_door_data->field_0_state == door_state::open_2;
}

inline void Door::CloseDoors_476A30()
{
    if (field_28_bClosing)
    {
        if (field_0_primary_door_data)
        {
            field_0_primary_door_data->Close_49C590(0);
        }
        if (field_4_secondary_door_data)
        {
            field_4_secondary_door_data->Close_49C590(field_2A_bDoFlip);
        }
    }
}

class DoorDataPool
{
  public:
    // inlined 0x44c830
    DoorData* Allocate()
    {
        DoorData* tmp = field_0_pool.field_0_pHead;
        field_0_pool.field_0_pHead = tmp->mpNext;
        tmp->PoolAllocate();
        return tmp;
    }

    // TODO: Pools - figure out why this won't match (probably doesn't get inlined??)
    /*
    Door_10* Allocate()
    {
        return field_0_pool.Allocate();
    }*/

    // inlined 0x44C800
    DoorDataPool()
    {
    }

    // 9.6f 0x44C7F0
    ~DoorDataPool()
    {
        field_0_pool.field_0_pHead = 0;
    }

    PoolBasic<DoorData, 44> field_0_pool;
};

class DoorPool
{
  public:
    EXPORT DoorData* AllocDoorData_49CF10(u8 gr_id, char_type x, char_type y, char_type z, s32 face, char_type bDoFlip);
    EXPORT Door* RegisterSingleDoorNoCheck_49CF50(u8 gr_id, u8 x, u8 y, u8 z, u32 face, u8 flip, u8 reversed);
    EXPORT Door* RegisterDoubleDoorNoCheck_49CFA0(u8 gr_id, u8 x, u8 y, u8 z, s32 face, u8 flip, u8 reversed);
    EXPORT Door* RegisterSingleDoor_49D170(u8 gr_id,
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
    EXPORT Door* RegisterDoubleDoor_49D1F0(u8 gr_id,
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
    EXPORT void RegisterDoorInfo_49D2D0(s16 start_frame, s16 end_frame, char_type speed);
    EXPORT void TryOpenDoorForCar_49D340(Car* pCar, u8 door_idx);
    EXPORT void TryOpenDoorForPed_49D370(Ped* pPed, u8 idx);
    EXPORT Door* GetNextFreeDoor_49D3A0();
    EXPORT bool CheckDoorAccess_49D3C0(Sprite* pSprite, u8 door_idx);
    EXPORT void DoorsService_49D460();
    EXPORT DoorPool();
    EXPORT ~DoorPool();

    Door field_0_doors[22];
    u16 field_4D0_count;
    s16 field_4D2_unused;
};
GTA2_ASSERT_SIZEOF_ALWAYS(DoorPool, 0x4D4);

EXTERN_GLOBAL(DoorPool*, gDoor_4D4_67BD2C);
