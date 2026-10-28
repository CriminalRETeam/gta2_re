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
    u16 field_0_start_frame;
    u16 field_2_end_frame;
    s16 field_4_internal_tile_idx;
    s16 field_6_open_internal_tile_idx;
    u8 field_8_speed;
    s8 field_9_unused;
};
GTA2_ASSERT_SIZEOF_ALWAYS(DoorAnimInfo, 0xA);

class Door
{
  public:
    // 9.6f 0x476A90
    inline s32 get_open_type_476A90() const
    {
        return field_20_open_type;
    }

    EXPORT Door();
    EXPORT ~Door();
    EXPORT bool IsSpriteClearOfDoorCollision_49C6A0(Sprite* pSprite);
    EXPORT bool CanOpen_49C6D0(Car* pCar);
    EXPORT bool CanOpenForPed_49C7F0(Ped* pPed);
    EXPORT void Open_49C840();
    EXPORT void TryOpenForCar_49C870(Car* pCar);
    EXPORT void TryOpenForPed_49C8A0(Ped* pPed);
    EXPORT void InitSingleNoCheck_49C8D0(u8 door_group, u8 gr_id, u8 x, u8 y, u8 z, s32 facing);
    EXPORT void AddDoorData_49CA50(u8 gr_id, char_type x, char_type y, char_type z, s32 face);
    EXPORT void InitSingle_49CAC0(DoorData* pDoorData, char_type bFlip, u8 obj_id, Fix16 trigger_x, Fix16 trigger_y, Fix16 trigger_z, Fix16 trigger_w, Fix16 trigger_h);
    EXPORT void InitDouble_49CC00(DoorData* pDoorData, char_type bFlip, u8 obj_id, Fix16 trigger_x, Fix16 trigger_y, Fix16 trigger_z, Fix16 trigger_w, Fix16 trigger_h);
    EXPORT void UpdateAutoClose_49CD90();
    EXPORT bool Service_49CE90();
    EXPORT void get_door_xyz_face_49CEE0(u8* pX, u8* pY, u8* pZ, u32* pFace);
    
    // 9.6f 0x44C860, defined in DoorPool.hpp (needs DoorData)
    inline bool IsOpen_44C860() const;

    // 9.6f 0x476A30, defined in DoorPool.hpp (needs DoorData)
    inline void CloseDoors_476A30();

    // 9.6f 0x4895F0
    inline void ClearF2C_4895F0()
    {
        field_2C_bCanAutoOpen = false;
    }

    // 9.6f 0x489600
    inline bool IsDoubleDoor_489600() const
    {
        return field_0_primary_door_data && field_4_secondary_door_data;
    }

    // inlined in 0x476990
    // 9.6f 0x476990
    inline void set_field_20(u32 v)
    {
        field_20_open_type = v;
    }

    // inlined in 0x476a10
    inline void set_close_type(u32 v)
    {
        field_24_close_type = v;
    }

    // inlined in 0x476a20
    inline void set_close_delay(s16 v)
    {
        field_1C_close_delay = v;
        field_1E_close_timer = v;
    }

    // inlined in 0x4769e0
    inline void set_open_details_car_bc(u32 open, Car* car_bc_ptr)
    {
        field_20_open_type = open;
        field_10_car_bc = car_bc_ptr;
        field_2C_bCanAutoOpen = true;
    }

    // inlined in 0x4769a0
    inline void set_open_details_model_id(u32 open, s32 id)
    {
        field_20_open_type = open;
        field_10_model_id = id;
        field_2C_bCanAutoOpen = true;
    }

    // inlined in 0x4769c0
    inline void set_open_details_ped(u32 open, Ped* ped_ptr)
    {
        field_20_open_type = open;
        field_10_ped = ped_ptr;
        field_2C_bCanAutoOpen = true;
    }

    // inlined in 0x476a00
    inline void set_target_id(s32 id)
    {
        field_14_target_id = id;
        field_2C_bCanAutoOpen = true;
    }

    DoorData* field_0_primary_door_data;
    DoorData* field_4_secondary_door_data; // Only active when this door is a double door
    Object* field_8_door_obj;
    Object* field_C_trigger_obj;
    union
    {
        Ped* field_10_ped; // Active when field_20_open_type is door_open_type::one_char_on_foot
        Car* field_10_car_bc; // Active when field_20_open_type is door_open_type::one_car
        s32 field_10_model_id; // Active when field_20_open_type is door_open_type::one_model
    };
    s32 field_14_target_id;
    s32 field_18_unused;
    s16 field_1C_close_delay;
    u16 field_1E_close_timer;
    s32 field_20_open_type;
    s32 field_24_close_type;
    bool field_28_bClosing;
    bool field_29_bAuto;
    char_type field_2A_bDoFlip;
    char_type field_2B_bReversed;
    bool field_2C_bCanAutoOpen;
    bool field_2D_play_open_sound;
    char_type field_2E_unused;
    char_type field_2F_unused;
    Fix16 field_30_x;
    Fix16 field_34_y;
};
GTA2_ASSERT_SIZEOF_ALWAYS(Door, 0x38);

EXTERN_GLOBAL_ARRAY(DoorAnimInfo, gDoorAnimInfo_67BB38, 5);
