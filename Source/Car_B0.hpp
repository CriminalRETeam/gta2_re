#pragma once

#include "Car_BC.hpp"
#include "Function.hpp"

class Sprite;
class Car_BC;
class Car_A4_10;
class Sprite_4C;
class Ped;
class CarInfo_48;
class CarInfo_2C;
class Car_78;

class Car_B0
{
  public:
    EXPORT s16* sub_4403A0(s16* a2);
    EXPORT ~Car_B0();
    EXPORT void sub_559430();
    EXPORT void sub_5597B0();
    EXPORT bool sub_5599D0();
    EXPORT void sub_559A40();
    EXPORT void sub_559B40();
    EXPORT void sub_559B50();
    EXPORT void sub_559B90(const Fix16 &a2);
    EXPORT u32 sub_559BA0();
    EXPORT void sub_559C30();
    EXPORT void sub_559DD0();
    EXPORT u32 sub_559E20(s32 a2);
    EXPORT s32* sub_559EC0(s32* a2);
    EXPORT u32* sub_559FF0(u32* a2);
    EXPORT u32* sub_55A050(u32* a2);
    EXPORT s32 sub_55A0B0();
    EXPORT u32* sub_55A100(u32* a2);
    EXPORT char_type sub_55A150();
    EXPORT char_type sub_55A180();
    EXPORT s32 sub_55A1D0(s32 a2, s32 a3, s32 a4, u32* a5);
    EXPORT void sub_55A400();
    EXPORT void sub_55A4B0();
    EXPORT void sub_55A550();
    EXPORT void sub_55A600();
    EXPORT u32* sub_55A6A0(u32* a2);
    EXPORT void sub_55A840();
    EXPORT char_type sub_55A860(char_type bForwardGasOn, char_type bFootBrakeOn, char_type a4, char_type a5, char_type bHandBrakeOn);
    EXPORT void sub_55AA00();
    EXPORT s32* sub_55AB50(s32* a2, Sprite_4C** a3);
    EXPORT s32 sub_55AD90(Sprite_4C* a2);
    EXPORT s32 sub_55B3F0(s32 a2);
    EXPORT s32 sub_55B4F0(Sprite_4C* a2);
    EXPORT char_type sub_55B970(char_type* a2);
    EXPORT void sub_55BFE0();
    EXPORT char_type sub_55C150();
    EXPORT char_type sub_55C3B0(Sprite_4C** a2, Sprite_4C** a3);
    EXPORT s32 sub_55C560(s32* a2, s32* a3);
    EXPORT s32 sub_55C5C0(u32* a2, s32 a3);
    EXPORT s32 sub_55C820(u32* a2, s32 a3);
    EXPORT void sub_55CA70(s32 a2, s32 a3);
    EXPORT void sub_55CBB0(s32 a2, s32 a3);
    EXPORT void sub_55D200(s32 a2, Sprite_4C* a3, s32 a4, s32 a5);
    EXPORT char_type sub_55DC00();
    EXPORT void sub_55E260();
    EXPORT char_type sub_55E470();
    EXPORT char_type sub_55EB80();
    EXPORT s32 sub_55EC30();
    EXPORT s32 sub_55EF20();
    EXPORT s32 sub_55F020();
    EXPORT char_type sub_55F240();
    EXPORT s32 sub_55F280();
    EXPORT void sub_55F330();
    EXPORT char_type sub_55F360();
    EXPORT s32 sub_55F740(s32* a2, s32* a3);
    EXPORT s32 sub_55F7A0(s32* a2, s32 a3, s32 a4);
    EXPORT s32 sub_55F800(s32* a2, s32* a3, s32 a4);
    EXPORT s32 sub_55F930(s32* a2);
    EXPORT __int64 sub_55F970(s32 a2);
    EXPORT s32 sub_55F9A0(s32* a2);
    EXPORT s32 sub_55FA10(s32* a2);
    EXPORT u32* sub_55FA60(u32* a2, s32* a3, s32* a4, s32 a5);
    EXPORT Ped* sub_55FC30(s32* a2, s32 a3);
    EXPORT s32 sub_55FD00(s32 a2);
    EXPORT Car_78* sub_55FF20(Car_BC* a2);
    EXPORT void sub_5606C0(s32 a2, char_type a3);
    EXPORT void sub_560B40(s32 a2, s32 a3);
    EXPORT __int64 sub_560EB0();
    EXPORT void sub_560F20(s32 a2);
    EXPORT s32 sub_5610B0();
    EXPORT u32* sub_561130(u32* a2, u32* a3);
    EXPORT u32* sub_561350(u32* a2, u32* a3);
    EXPORT u32* sub_561380(u32* a2, u32* a3);
    EXPORT s32* sub_5615D0(s32* a2, s32* a3, s32 a4, u32* a5, s32 a6);
    EXPORT s32 get_revs_561940();
    EXPORT u32* sub_561970(u32* a2);
    EXPORT u32* sub_561DD0(u32* a2);
    EXPORT Sprite_4C** sub_561E50(Sprite_4C** a2);
    EXPORT u32* sub_5620D0(u32* a2);
    EXPORT u32* sub_562450(u32* a2, s32 a3);
    EXPORT s32 sub_562480();
    EXPORT void sub_5624F0();
    EXPORT s32 sub_562560();
    EXPORT s32 sub_5626A0();
    EXPORT u32* sub_5626C0(u32* a2);
    EXPORT char_type sub_5626F0();
    EXPORT void sub_562910();
    EXPORT __int64 sub_562C20(s16* a2);
    EXPORT void sub_562D00();
    EXPORT CarInfo_48* sub_562EB0();
    EXPORT CarInfo_2C* sub_562ED0();
    EXPORT s32 sub_562EF0();
    EXPORT s32 sub_562F30();
    EXPORT char_type sub_562FA0();
    EXPORT bool sub_562FE0();
    EXPORT s32* sub_563280();
    EXPORT s32* sub_563350();
    EXPORT s32* sub_563460();
    EXPORT void sub_563560(Sprite* a2);
    EXPORT void sub_563590(Sprite* a2);
    EXPORT s32 sub_563670();
    EXPORT void sub_5636C0();
    EXPORT bool sub_5636E0();
    EXPORT void sub_5637A0();
    EXPORT void sub_563890();
    EXPORT void sub_5638C0(Car_BC* pBC);
    EXPORT Car_B0();

    Car_8 field_0_vel_read_only;
    s32 field_8_total_damage_q;
    Car_B0* field_C_pNext;
    Car_8 field_10[4];
    Car_8 field_30_cm1;
    Car_8 field_38_cp1;
    Car_8 field_40_linvel_1;
    s32 field_48;
    s32 field_4C;
    s32 field_50;
    s32 field_54;
    Ang16 field_58_theta;
    s16 field_5A;
    Car_BC* field_5C_pPrev;
    Fix16 field_60_gas_pedal;
    s32 field_64;
    Fix16 field_68_z_pos;
    Fix16 field_6C_cp3;
    Fix16 field_70;
    Fix16 field_74_ang_vel_rad;
    s32 field_78_pointing_ang_rad;
    s32 field_7C;
    s32 field_80;
    Fix16 field_84_front_skid;
    Fix16 field_88_rear_skid;
    s32 field_8C;
    u8 field_90_timer_since_last_move;
    char_type field_91_is_foot_brake_on;
    char_type field_92_is_hand_brake_on;
    char_type field_93_is_forward_gas_on;
    char_type field_94_is_backward_gas_on;
    char_type field_95;
    char_type field_96;
    char_type field_97;
    s32 field_98_surface_type;
    s32 field_9C;
    Fix16 field_A0;
    char_type field_A4;
    char_type field_A5_current_slope_length;
    char_type field_A6_current_slope_left_tiles;
    char_type field_A7_current_tile_z;
    char_type field_A8_hand_brake_force;
    char_type field_A9_car_model;
    char_type field_AA_sbw;
    char_type field_AB_tpa;
    char_type field_AC_drive_wheels_locked_q;
    char_type field_AD_turn_direction;
    char_type field_AE;
    char_type field_AF;
};

struct Car_D264
{
  //Inlined in Car_6C constructor 9.6f -> 0x420f80
    EXPORT Car_D264()
    {
      Car_B0* pIter = &field_4[0];
      for (s32 i = 0; i < 305; i++)
      {
        pIter->field_C_pNext = pIter + 1;
        pIter++;
      }

      field_4[0x131].field_C_pNext =NULL;
      field_0 = field_4;
    }

    Car_B0* field_0;
    Car_B0 field_4[306];
};

EXPORT_VAR extern Car_D264* gCar_D264_6FE3E0;