#pragma once

#include "Function.hpp"
#include "fix16.hpp"

class Object;
class DoorData;
class SpriteShape;
class Ped;
class Car;
class Sprite;

namespace door_open_type
{
enum
{
    none = 0,
    any_player = 1,
    any_car = 2,
    one_car = 3,
    one_model = 4,
    one_char_on_foot = 5,
    any_player_one_car = 6,
};
} // namespace door_open_type

namespace door_close_type
{
enum
{
    none = 0,
    close_time_delay = 1,
    close_when_clear = 2,
    close_never = 3,
    close_when_open_rule_fails = 4,
};
} // namespace door_close_type

struct DoorAnimInfo
{
    u16 mStartFrame; // 0x0
    u16 mEndFrame; // 0x2
    s16 mInternalTileIdx; // 0x4
    s16 mOpenInternalTileIdx; // 0x6
    u8 mSpeed8; // 0x8
    s8 mUnused9; // 0x9
};
GTA2_ASSERT_SIZEOF_ALWAYS(DoorAnimInfo, 0xA);

class Door
{
  public:
    // 9.6f 0x476A90
    inline s32 get_open_type_476A90() const
    {
        return mOpenType;
    }

    EXPORT Door();
    EXPORT ~Door();
    EXPORT bool IsSpriteClearOfDoorCollision(Sprite* pSprite);
    EXPORT bool CanOpen(Car* pCar);
    EXPORT bool CanOpenForPed(Ped* pPed);
    EXPORT void Open();
    EXPORT void TryOpenForCar(Car* pCar);
    EXPORT void TryOpenForPed(Ped* pPed);
    EXPORT void InitSingleNoCheck(u8 door_group, u8 gr_id, u8 x, u8 y, u8 z, s32 facing);
    EXPORT void AddDoorData(u8 gr_id, char_type x, char_type y, char_type z, s32 face);
    EXPORT void InitSingle(DoorData* pDoorData, char_type bFlip, u8 obj_id, Fix16 trigger_x, Fix16 trigger_y, Fix16 trigger_z, Fix16 trigger_w, Fix16 trigger_h);
    EXPORT void InitDouble(DoorData* pDoorData, char_type bFlip, u8 obj_id, Fix16 trigger_x, Fix16 trigger_y, Fix16 trigger_z, Fix16 trigger_w, Fix16 trigger_h);
    EXPORT void UpdateAutoClose();
    EXPORT bool Service();
    EXPORT void get_door_xyz_face(u8* pX, u8* pY, u8* pZ, u32* pFace);
    
    // 9.6f 0x44C860, defined in DoorPool.hpp (needs DoorData)
    inline bool IsOpen_44C860() const;

    // 9.6f 0x476A30, defined in DoorPool.hpp (needs DoorData)
    inline void CloseDoors_476A30();

    // 9.6f 0x4895F0
    inline void ClearF2C_4895F0()
    {
        mbCanAutoOpen = false;
    }

    // 9.6f 0x489600
    inline bool IsDoubleDoor_489600() const
    {
        return mPrimaryDoorData && mSecondaryDoorData;
    }

    // inlined in 0x476990
    // 9.6f 0x476990
    inline void SetOpenType_476990(u32 v)
    {
        mOpenType = v;
    }

    // inlined in 0x476a10
    inline void set_close_type(u32 v)
    {
        mCloseType = v;
    }

    // inlined in 0x476a20
    inline void set_close_delay(s16 v)
    {
        mCloseDelay = v;
        mCloseTimer = v;
    }

    // inlined in 0x4769e0
    inline void set_open_details_car_bc(u32 open, Car* car_bc_ptr)
    {
        mOpenType = open;
        mCarBc = car_bc_ptr;
        mbCanAutoOpen = true;
    }

    // inlined in 0x4769a0
    inline void set_open_details_model_id(u32 open, s32 id)
    {
        mOpenType = open;
        mModelId = id;
        mbCanAutoOpen = true;
    }

    // inlined in 0x4769c0
    inline void set_open_details_ped(u32 open, Ped* ped_ptr)
    {
        mOpenType = open;
        mPed = ped_ptr;
        mbCanAutoOpen = true;
    }

    // inlined in 0x476a00
    inline void set_target_id(s32 id)
    {
        mTargetId = id;
        mbCanAutoOpen = true;
    }

    DoorData* mPrimaryDoorData; // 0x0
    DoorData* mSecondaryDoorData; // 0x4: Only active when this door is a double door
    Object* mDoorObj; // 0x8
    Object* mTriggerObj; // 0xC
    union
    {
        Ped* mPed; // 0x10: Active when mOpenType is door_open_type::one_char_on_foot
        Car* mCarBc; // 0x10: Active when mOpenType is door_open_type::one_car
        s32 mModelId; // 0x10: Active when mOpenType is door_open_type::one_model
    };
    s32 mTargetId; // 0x14
    s32 mUnused18; // 0x18
    s16 mCloseDelay; // 0x1C
    u16 mCloseTimer; // 0x1E
    s32 mOpenType; // 0x20
    s32 mCloseType; // 0x24
    bool mbClosing; // 0x28
    bool mbAuto; // 0x29
    char_type mbDoFlip; // 0x2A
    char_type mbReversed; // 0x2B
    bool mbCanAutoOpen; // 0x2C
    bool mPlayOpenSound; // 0x2D
    char_type mUnused2E; // 0x2E
    char_type mUnused2F; // 0x2F
    Fix16 mX; // 0x30
    Fix16 mY; // 0x34
};
GTA2_ASSERT_SIZEOF_ALWAYS(Door, 0x38);

EXTERN_GLOBAL_ARRAY(DoorAnimInfo, gDoorAnimInfo, 5);
