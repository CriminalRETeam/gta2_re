#pragma once

#include "Function.hpp"
#include "fix16.hpp"

class Object_2C;
class Door_10;
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

struct Door_A
{
    u16 field_0_start_frame;
    u16 field_2_end_frame;
    s16 field_4;
    s16 field_6;
    u8 field_8_speed;
    s8 field_9;
};

class Door_38
{
  public:
    EXPORT Door_38();
    EXPORT ~Door_38();
    EXPORT bool sub_49C6A0(Sprite* a1);
    EXPORT bool sub_49C6D0(Car_BC* a2);
    EXPORT bool sub_49C7F0(Ped* a2);
    EXPORT void sub_49C840();
    EXPORT void sub_49C870(Car_BC* a2);
    EXPORT void sub_49C8A0(Ped* a2);
    EXPORT void sub_49C8D0(u8 arg0, u8 a1, u8 a2, u8 a3, u8 a4, s32 a5);
    EXPORT void sub_49CA50(u8 a1, char_type a2, char_type a3, char_type a4, s32 a5);
    EXPORT void sub_49CAC0(Door_10* a2, char_type a3, u8 a4, Fix16 a5, Fix16 a6, Fix16 a7, Fix16 a8, Fix16 a9);
    EXPORT void sub_49CC00(Door_10* a1, char_type a2, u8 a3, Fix16 a4, Fix16 a5, Fix16 a6, Fix16 a7, Fix16 a8);
    EXPORT void sub_49CD90();
    EXPORT char_type Service_49CE90();
    EXPORT void get_door_xyz_face_49CEE0(u8* pX, u8* pY, u8* pZ, u32* pFace);
    
    // inlined in 0x476990
    inline void set_field_20(u32 v)
    {
        field_20_state = v;
    }

    // inlined in 0x476a10
    void set_field_24(u32 v)
    {
        field_24 = v;
    }

    // inlined in 0x476a20
    void set_field_1c_1e(s16 v)
    {
        field_1C = v;
        field_1E = v;
    }

    // inlined in 0x4769e0
    void set_open_details_car_bc(u32 open, Car_BC* car_bc_ptr)
    {
        field_20_state = open;
        field_10_car_bc = car_bc_ptr;
        field_2C = 1;
    }

    // inlined in 0x4769a0
    void set_open_details_model_id(u32 open, s32 id)
    {
        field_20_state = open;
        field_10_model_id = id;
        field_2C = 1;
    }

    // inlined in 0x4769c0
    void set_open_details_ped(u32 open, Ped* ped_ptr)
    {
        field_20_state = open;
        field_10_ped = ped_ptr;
        field_2C = 1;
    }

    // inlined in 0x476a00
    void set_field_14_id(s32 id)
    {
        field_14 = id;
        field_2C = 1;
    }

    Door_10* field_0;
    Door_10* field_4;
    Object_2C* field_8;
    Object_2C* field_C;
    union
    {
        Ped* field_10_ped; // Active when field_20_state is door_open_type::one_char_on_foot
        Car_BC* field_10_car_bc; // Active when field_20_state is door_open_type::one_car
        s32 field_10_model_id; // Active when field_20_state is door_open_type::one_model
    };
    s32 field_14;
    s32 field_18;
    s16 field_1C;
    u16 field_1E;
    s32 field_20_state;
    s32 field_24;
    char_type field_28;
    char_type field_29;
    char_type field_2A;
    char_type field_2B;
    char_type field_2C;
    char_type field_2D;
    char_type field_2E;
    char_type field_2F;
    Fix16 field_30;
    Fix16 field_34;
};

EXTERN_GLOBAL_ARRAY(Door_A, word_67BB38, 5);
