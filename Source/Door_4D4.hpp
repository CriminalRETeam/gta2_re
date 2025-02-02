#pragma once

#include "Function.hpp"
#include "fix16.hpp"

class Object_2C;
class Door_10;
class Sprite_4C;
class Ped;

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
    EXPORT Object_2C* sub_49C8D0(s32 arg0, u8 a1, u8 a2, u8 a3, char_type a4, s32 a5);
    EXPORT void sub_49CA50(u8 a1, char_type a2, char_type a3, char_type a4, s32 a5);
    EXPORT Object_2C* sub_49CAC0(s32 a2, char_type a3, u8 a4, s32 a5, s32 a6, s32 a7, s32 a8, Sprite_4C* a9);
    EXPORT void sub_49CC00(Door_10* a1, char_type a2, u8 a3, Fix16 a4, Fix16 a5, Fix16 a6, Fix16 a7, Fix16 a8);
    EXPORT void sub_49CD90();
    EXPORT char_type sub_49CE90();

    Door_10* field_0;
    Door_10* field_4;
    s32 field_8;
    s32 field_C;
    Ped* field_10;
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
    EXPORT Door_38* sub_49CF50(u8 a2, char_type a3, s32 a4, char_type a5, s32 a6, char_type a7, char_type a8);
    EXPORT Door_38* sub_49CFA0(u8 a1, u8 a2, u8 a3, u8 a4, s32 a5, s32 a7, char_type a8);
    EXPORT Door_38* sub_49D170(u8 a1,
                               char_type a2,
                               char_type a3,
                               char_type a4,
                               s32 a5,
                               s32 a7,
                               s32 a8,
                               s32 a9,
                               s32 a10,
                               s32 a11,
                               char_type a12,
                               char_type a13);
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
    s32 field_0;
    char field_4;
    char field_5;
    u8 field_6;
    char field_7;
    s32 field_8;
    Door_10* field_C;
};

class Door_2C4
{
  public:
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
