#pragma once

#include "Function.hpp"
#include "fix16.hpp"

class Object_2C;
class DoorData_10;
class Sprite_4C;
class Ped;
class Car_BC;
class Sprite;

namespace door_open_type
{
enum
{
    unknown1 = 0,
    any_player = 1,
    any_car = 2,
    one_car = 3,
    one_model = 4,
    one_char_on_foot = 5,
    any_player_one_car = 6,
};
//static_assert(sizeof(door_open_type) == 4);
} // namespace door_open_type

namespace door_close_type
{
enum
{
    unknown1 = 0,
    close_time_delay = 1,
    close_when_clear = 2,
    close_never = 3,
    close_when_open_rule_fails = 4,
};
//static_assert(sizeof(door_close_type) == 4);
} // namespace door_close_type

struct DoorAnimInfo_A
{
    u16 field_0_start_frame;
    u16 field_2_end_frame;
    s16 field_4_internal_tile_idx;
    s16 field_6_open_internal_tile_idx;
    u8 field_8_speed;
    s8 field_9;
};

class Door_38
{
  public:
    EXPORT Door_38();
    EXPORT ~Door_38();
    EXPORT bool IsSpriteClearOfDoorCollision_49C6A0(Sprite* a1);
    EXPORT bool CanOpen_49C6D0(Car_BC* a2);
    EXPORT bool CanOpenForPed_49C7F0(Ped* a2);
    EXPORT void Open_49C840();
    EXPORT void TryOpenForCar_49C870(Car_BC* a2);
    EXPORT void TryOpenForPed_49C8A0(Ped* a2);
    EXPORT void InitSingleNoCheck_49C8D0(u8 arg0, u8 a1, u8 a2, u8 a3, u8 a4, s32 a5);
    EXPORT void AddDoorData_49CA50(u8 gr_id, char_type x, char_type y, char_type z, s32 face);
    EXPORT void InitSingle_49CAC0(DoorData_10* a2, char_type a3, u8 a4, Fix16 a5, Fix16 a6, Fix16 a7, Fix16 a8, Fix16 a9);
    EXPORT void InitDouble_49CC00(DoorData_10* a1, char_type a2, u8 a3, Fix16 a4, Fix16 a5, Fix16 a6, Fix16 a7, Fix16 a8);
    EXPORT void UpdateAutoClose_49CD90();
    EXPORT char_type Service_49CE90();
    EXPORT void get_door_xyz_face_49CEE0(u8* pX, u8* pY, u8* pZ, u32* pFace);
    
    /*
    // TODO: Causes a circular dependency
    inline bool IsOpen_44C860()
    {
        return field_0_primary_door_data->field_0_state == 2;
    }
    */

    // inlined in 0x476990
    inline void set_field_20(u32 v)
    {
        field_20_state = v;
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
    inline void set_open_details_car_bc(u32 open, Car_BC* car_bc_ptr)
    {
        field_20_state = open;
        field_10_car_bc = car_bc_ptr;
        field_2C = 1;
    }

    // inlined in 0x4769a0
    inline void set_open_details_model_id(u32 open, s32 id)
    {
        field_20_state = open;
        field_10_model_id = id;
        field_2C = 1;
    }

    // inlined in 0x4769c0
    inline void set_open_details_ped(u32 open, Ped* ped_ptr)
    {
        field_20_state = open;
        field_10_ped = ped_ptr;
        field_2C = 1;
    }

    // inlined in 0x476a00
    inline void set_target_id(s32 id)
    {
        field_14_target_id = id;
        field_2C = 1;
    }

    DoorData_10* field_0_primary_door_data;
    DoorData_10* field_4_secondary_door_data; // Only active when this door is a double door
    Object_2C* field_8_door_obj;
    Object_2C* field_C_trigger_obj;
    union
    {
        Ped* field_10_ped; // Active when field_20_state is door_open_type::one_char_on_foot
        Car_BC* field_10_car_bc; // Active when field_20_state is door_open_type::one_car
        s32 field_10_model_id; // Active when field_20_state is door_open_type::one_model
    };
    s32 field_14_target_id;
    s32 field_18;
    s16 field_1C_close_delay;
    u16 field_1E_close_timer;
    s32 field_20_state;
    s32 field_24_close_type;
    char_type field_28;
    char_type field_29_bAuto;
    char_type field_2A_bDoFlip;
    char_type field_2B_bReversed;
    char_type field_2C;
    char_type field_2D;
    char_type field_2E;
    char_type field_2F;
    Fix16 field_30_x;
    Fix16 field_34_y;
};

EXTERN_GLOBAL_ARRAY(DoorAnimInfo_A, gDoorAnimInfo_67BB38, 5);
