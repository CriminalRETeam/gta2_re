#pragma once

#include "Car_BC.hpp"
#include "Function.hpp"
#include "fix16.hpp"

class Object_2C;
class Door_10;
class Sprite_4C;
class Ped;

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
    EXPORT s32 sub_49C6A0(s32 a1);
    EXPORT char_type sub_49C6D0(u32* a2);
    EXPORT bool sub_49C7F0(Ped* a2);
    EXPORT s32 sub_49C840();
    EXPORT void sub_49C870(u32* a2);
    EXPORT void sub_49C8A0(Ped* a2);
    EXPORT void sub_49C8D0(u8 arg0, u8 a1, u8 a2, u8 a3, u8 a4, s32 a5);
    EXPORT void sub_49CA50(u8 a1, char_type a2, char_type a3, char_type a4, s32 a5);
    EXPORT void sub_49CAC0(Door_10* a2, char_type a3, u8 a4, Fix16 a5, Fix16 a6, Fix16 a7, Fix16 a8, Fix16 a9);
    EXPORT void sub_49CC00(Door_10* a1, char_type a2, u8 a3, Fix16 a4, Fix16 a5, Fix16 a6, Fix16 a7, Fix16 a8);
    EXPORT void sub_49CD90();
    EXPORT char_type sub_49CE90();

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
    s16 field_1E;
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

class Door_4D4
{
  public:
    EXPORT Door_10* sub_49CF10(u8 a1, char_type a2, char_type a3, char_type a4, s32 a5, char_type a6);
    EXPORT Door_38* sub_49CF50(u8 a2, char_type a3, char_type a4, char_type a5, u32 a6, char_type a7, char_type a8);
    EXPORT Door_38* sub_49CFA0(u8 gr_id, u8 x, u8 y, u8 z, s32 face, u8 a7, u8 a8);
    EXPORT Door_38* sub_49D170(u8 a1,
                               char_type a2,
                               char_type a3,
                               char_type a4,
                               s32 a5,
                               Fix16 a6,
                               Fix16 a7,
                               Fix16 a8,
                               Fix16 a9,
                               Fix16 a10,
                               char_type a11,
                               char_type a12);
    EXPORT Door_38* sub_49D1F0(u8 a1,
                               char_type a2,
                               char_type a3,
                               char_type a4,
                               s32 a5,
                               Fix16 a6,
                               Fix16 a7,
                               Fix16 a8,
                               Fix16 a9,
                               Fix16 a10,
                               char_type a11,
                               char_type a12);
    EXPORT void sub_49D2D0(s16 start_frame, s16 end_frame, char_type speed);
    EXPORT void sub_49D340(u32* a2, u8 a3);
    EXPORT void sub_49D370(Ped* a2, u8 idx);
    EXPORT Door_38* sub_49D3A0();
    EXPORT char_type sub_49D3C0(s32 a2, u8 a3);
    EXPORT void sub_49D460();
    EXPORT Door_4D4();
    EXPORT ~Door_4D4();

    Door_38 field_0[22];
    u16 field_4D0_count;
    s16 field_4D2;
};

class Door_10
{
  public:
    EXPORT void sub_49C320();
    EXPORT void sub_49c340(u8 a1, u8 a2, u8 a3, u8 a4, u32 a5, u8 a6);
    EXPORT static s32 __stdcall sub_4DEEB0(s32 v);

    s32 field_0;
    u8 field_4;
    u8 field_5;
    u8 field_6;
    u8 field_7;
    s32 field_8;
    Door_10* field_C;
};

class Door_2C4
{
  public:
    // inlined 0x44c830
    Door_10* get_new_door_10()
    {
        Door_10* tmp = field_0;
        field_0 = tmp->field_C;
        tmp->sub_49C320();

        return tmp;
    }

    // inlined 0x44C800
    Door_2C4()
    {
        Door_10* pIter = field_4;
        for (s32 i = 0; i < GTA2_COUNTOF(field_4); i++)
        {
            pIter->field_C = pIter + 1;
            pIter++;
        }
        field_0 = field_4;
        field_2C0 = 0;
    }

    ~Door_2C4()
    {
        field_0 = 0;
    }

    Door_10* field_0;
    Door_10 field_4[43];
    s32 field_2B4;
    s32 field_2B8;
    s32 field_2BC;
    s32 field_2C0;
};
EXPORT_VAR extern Door_4D4* gDoor_4D4_67BD2C;
