#pragma once

#include "Function.hpp"
#include "fix16.hpp"
#include "Door_38.hpp"

class Object_2C;
class Door_10;
class Sprite_4C;
class Ped;
class Car_BC;

class Door_10
{
  public:
    EXPORT void sub_49C320();
    EXPORT void sub_49c340(u8 a1, u8 a2, u8 a3, u8 a4, u32 a5, u8 a6);
    EXPORT void sub_49C4E0(u8 a1);
    EXPORT void sub_49C590(u8 a1);
    EXPORT static s32 __stdcall sub_4DEEB0(s32 v);

    s32 field_0;
    u8 field_4_x;
    u8 field_5_y;
    u8 field_6_z;
    u8 field_7_gr_id;
    s32 field_8_face;
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
        for (s32 i = 0; i < GTA2_COUNTOF(field_4)-1; i++)
        {
            pIter->field_C = pIter + 1;
            pIter++;
        }
        field_0 = field_4;
        field_4[44-1].field_C = 0;
    }

    // 0x44C7F0
    ~Door_2C4()
    {
        field_0 = 0;
    }

    Door_10* field_0;
    Door_10 field_4[44];
};

class Door_4D4
{
  public:
    EXPORT Door_10* sub_49CF10(u8 a1, char_type a2, char_type a3, char_type a4, s32 a5, char_type a6);
    EXPORT Door_38* sub_49CF50(u8 gr_id, u8 x, u8 y, u8 z, u32 face, u8 flip, u8 reversed);
    EXPORT Door_38* sub_49CFA0(u8 gr_id, u8 x, u8 y, u8 z, s32 face, u8 flip, u8 reversed);
    EXPORT Door_38* sub_49D170(u8 gr_id,
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
    EXPORT Door_38* sub_49D1F0(u8 gr_id,
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
    EXPORT void sub_49D2D0(s16 start_frame, s16 end_frame, char_type speed);
    EXPORT void sub_49D340(Car_BC* a2, u8 a3);
    EXPORT void sub_49D370(Ped* a2, u8 idx);
    EXPORT Door_38* sub_49D3A0();
    EXPORT char_type sub_49D3C0(s32 a2, u8 a3);
    EXPORT void DoorsService_49D460();
    EXPORT Door_4D4();
    EXPORT ~Door_4D4();

    Door_38 field_0[22];
    u16 field_4D0_count;
    s16 field_4D2;
};

EXTERN_GLOBAL(Door_4D4*, gDoor_4D4_67BD2C);
