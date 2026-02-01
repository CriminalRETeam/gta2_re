#pragma once

#include "Function.hpp"
#include "fix16.hpp"
#include "Door_38.hpp"
#include "Pool.hpp"

class Object_2C;
class DoorData_10;
class Sprite_4C;
class Ped;
class Car_BC;

class DoorData_10 // Contains only position, face and DoorInfo id
{
  public:
    EXPORT void PoolAllocate();
    EXPORT void sub_49c340(u8 gr_id, u8 x, u8 y, u8 z, u32 face, u8 a6);
    EXPORT void sub_49C4E0(u8 a1);
    EXPORT void sub_49C590(u8 a1);
    EXPORT static s32 __stdcall sub_4DEEB0(s32 v);

    s32 field_0;
    u8 field_4_x;
    u8 field_5_y;
    u8 field_6_z;
    u8 field_7_gr_id;
    s32 field_8_face;
    DoorData_10* mpNext;
};

class DoorData_10_Pool
{
  public:
    // inlined 0x44c830
    DoorData_10* Allocate()
    {
        DoorData_10* tmp = field_0_pool.field_0_pHead;
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
    DoorData_10_Pool()
    {

    }

    // 0x44C7F0
    ~DoorData_10_Pool()
    {

    }

    PoolBasic<DoorData_10, 44> field_0_pool;
};

class Door_4D4
{
  public:
    EXPORT DoorData_10* sub_49CF10(u8 gr_id, char_type x, char_type y, char_type z, s32 face, char_type a6);
    EXPORT Door_38* RegisterSingleDoorNoCheck_49CF50(u8 gr_id, u8 x, u8 y, u8 z, u32 face, u8 flip, u8 reversed);
    EXPORT Door_38* RegisterDoubleDoorNoCheck_49CFA0(u8 gr_id, u8 x, u8 y, u8 z, s32 face, u8 flip, u8 reversed);
    EXPORT Door_38* RegisterSingleDoor_49D170(u8 gr_id,
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
    EXPORT Door_38* RegisterDoubleDoor_49D1F0(u8 gr_id,
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
    EXPORT void sub_49D340(Car_BC* a2, u8 a3);
    EXPORT void sub_49D370(Ped* a2, u8 idx);
    EXPORT Door_38* sub_49D3A0();
    EXPORT char_type sub_49D3C0(Sprite* pSprite, u8 door_idx);
    EXPORT void DoorsService_49D460();
    EXPORT Door_4D4();
    EXPORT ~Door_4D4();

    Door_38 field_0[22];
    u16 field_4D0_count;
    s16 field_4D2;
};

EXTERN_GLOBAL(Door_4D4*, gDoor_4D4_67BD2C);
