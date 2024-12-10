#pragma once

#include "Car_10.hpp"
#include "Function.hpp"
#include "Ped_Unknown_4.hpp"
#include <wchar.h>

class Car_78;
class Car_B0;
class Car_A4_10;
class Car_3C;
class cool_nash_0x294;
class Char_8;
class Hamburger_40;
class Ped_Unknown_4;
class car_info;
class infallible_turing;
class Sprite_4C;

class Car_3C
{
  public:
    EXPORT void sub_451950(s32 a2, s32 a3, s32 a4);
    EXPORT void sub_59E2E0();
    EXPORT Car_3C* sub_59E300();
    EXPORT s32 sub_59E320(char_type a2);
    EXPORT bool sub_59E390(s32 a2, s32 a3, s32 a4);
    EXPORT s32 sub_59E4C0(s32 a2, s32 a3);
    EXPORT void sub_59E7B0();
    EXPORT Car_3C* sub_59E7D0(s32 a2);
    EXPORT s32 sub_59E960();
    EXPORT Car_3C* sub_59E9C0();
    EXPORT s16 sub_59EA00(s16 a2);
    EXPORT s16 sub_59EAA0();
    EXPORT char_type sub_59EAE0();
    EXPORT void sub_59EB30(s32 a2, f32* a3);
    EXPORT void ShowHorn_59EE40(s32 a2, s32 a3);
    EXPORT void sub_59EFF0();
    EXPORT void sub_59F950(s32 a2, Sprite_4C* a3, Car_BC* a4);
    EXPORT Sprite_4C* sub_59F990();
    EXPORT Sprite_4C* sub_59FA40();
    EXPORT Sprite_4C* sub_59FAD0();
    EXPORT char_type sub_5A0150(s32 a2, u8* a3, u8* a4);
    EXPORT char_type sub_5A0320(u32* a2, u32* a3, u8* a4, u8* a5);
    EXPORT char_type sub_5A1030(Car_3C* a2, Car_3C** a3, u8* a4);
    EXPORT bool sub_5A1490(s32 a2, s32 a3);
    EXPORT char_type sub_5A19C0();
    EXPORT char_type sub_5A1A60();
    EXPORT char_type sub_5A1B30(Car_3C* a2);
    EXPORT char_type sub_5A1BD0();
    EXPORT char_type sub_5A1CA0(u32* a2);
    EXPORT char_type sub_5A2500();
    EXPORT s32* sub_5A2710(s32* a2, Car_3C* a3, s32* a4, s32 a5, u8* a6, u8* a7, char_type* a8);
    EXPORT void sub_5A29D0();
    EXPORT infallible_turing* sub_5A2A00();
    EXPORT void sub_5A2A30();
    EXPORT s32 sub_5A2CF0();
    EXPORT infallible_turing* sub_5A3030();
    EXPORT Car_BC* sub_5A3100(Car_3C* a2, s32 a3, s32 a4, s16 a5);
    EXPORT void dtor_5A3540();
    EXPORT void ctor_5A5E50();

    s32 field_0;
    Sprite_4C* field_4_0x4C_len;
    Car_BC* field_8_uni;
    Car_3C* field_C_car_or_sprite;
    infallible_turing* field_10;
    s32 field_14_xpos;
    s32 field_18_ypos;
    s32 field_1C_zpos;
    s16 field_20;
    s16 field_22;
    s16 field_24;
    char_type field_26;
    char_type field_27;
    Car_3C* field_28_uni;
    char_type field_2C;
    char_type field_2D;
    char_type field_2E;
    char_type field_2F;
    s32 field_30;
    s32 field_34;
    char_type field_38;
    char_type field_39;
    char_type field_3A;
    char_type field_3B;
};

class Car_18
{
  public:
    s32 field_0;
    s32 field_4;
    char_type field_8;
    char_type field_9;
    char_type field_A;
    char_type field_B;
    s32 field_C;
    s32 field_10;
    s16 field_14;
    char_type field_16;
    char_type field_17;
};

class Car_214
{
  public:
    EXPORT s32 sub_5C8680(u8 a2);
    EXPORT char_type sub_5C86C0(s32* a2, u32* a3, s32 a4, s32 a5, s32 a6, s32 a7, s32 a8, s32 a9);
    EXPORT void sub_5C8750();
    EXPORT u16* sub_5C8780(u8 a2, Car_3C* pCarSprite);
    s32 field_0;
    Car_18 field_4[22];
};

EXPORT_VAR extern Car_214* gCar_214_705F20;

class Car_2
{
  public:
    EXPORT Car_2* sub_47BD00();
    EXPORT void sub_47BD90();
    u16 field_0;
};

class Car_6C
{
  public:
    EXPORT void sub_444980();
    EXPORT u32 sub_444AB0(s32 a2, u16* a3, s32 a4, u16* a5);
    EXPORT Car_BC* sub_444CF0(s32 a1, s32 a2, s32 a3, s32 a4);
    EXPORT Car_BC* sub_444F80(s32 a1, s32 a2, s32 a3, cool_nash_0x294* a4);
    EXPORT s32 sub_444FA0(s32 x, s32 y, s32 z, s32 a5);
    EXPORT Car_BC* sub_4458B0(s32 arg0, s32 a3, s32 a4, s32 a2);
    EXPORT Car_BC* sub_446230(s32 arg0, s32 a3, s32 a4, s16 a5, s32 a2, s32 a7);
    EXPORT Car_A4_10* sub_446530(s32 a2, s32 a3, Car_BC* a4, s32 a5, s32 a6);
    EXPORT s32 sub_4466C0(s32 a2);
    EXPORT void sub_446760();
    EXPORT void sub_446790();
    EXPORT bool sub_446870(s32 a2);
    EXPORT u32 sub_446930(s32 a2);
    EXPORT Car_6C();
    EXPORT ~Car_6C();

    Car_2 field_0;
    s16 field_2;
    cool_nash_0x294* field_4;
    char_type field_8;
    u8 field_9;
    u8 field_A;
    u8 field_B;
    s32 field_C;
    Car_2 field_10_idx;
    Car_2 field_12;
    s32 field_14;
    char_type field_18;
    char_type field_19;
    char_type field_1A;
    char_type field_1B;
    s32 field_1C;
    s32 field_20;
    s32 field_24;
    s32 field_28_recycled_cars;
    s32 field_2C;
    s32 field_30;
    s32 field_34_unit_cars;
    s32 field_38;
    s32 field_3C_mission_cars;
    s32 field_40_proto_recycled_cars;
    s32 field_44;
    s32 field_48;
    s32 field_4C;
    s32 field_50_tv_van_dir;
    char_type field_54;
    char_type field_55;
    char_type field_56;
    char_type field_57;
    s32 field_58;
    char_type field_5C;
    char_type field_5D;
    char_type field_5E;
    char_type field_5F;
    s32 field_60;
    s32 field_64;
    char_type field_68;
    char_type field_69_do_free_shopping;
    char_type field_6A;
    char_type field_6B;
};

EXPORT_VAR extern Car_6C* gCar_6C_677930;

class Car_BC
{
  public:
    EXPORT bool sub_439EC0();
    EXPORT u32* sub_439EE0(u32* a2);
    EXPORT u32* sub_439F30(u32* a2);
    EXPORT wchar_t* GetCarStr_439F80();
    EXPORT u32* sub_439FB0(u32* a2);
    EXPORT u32* sub_43A0E0(u32* a2);
    EXPORT u32* sub_43A120(u32* a2);
    EXPORT s32* sub_43A1D0(s32* a2);
    EXPORT bool is_bus_43A1F0();
    EXPORT bool sub_43A230();
    EXPORT s32* sub_43A240(s32* a2);
    EXPORT bool sub_43A3C0();
    EXPORT s16* sub_43A3E0(s16* a2);
    EXPORT u32* sub_43A450(u32* a2);
    EXPORT s32* sub_43A4C0(s32* a2);
    EXPORT u32* sub_43A590(u32* a2);
    EXPORT u32* sub_43A5B0(u32* a2);
    EXPORT void sub_43A600();
    EXPORT s32 sub_43A680();
    EXPORT s32 sub_43A6F0(u8 a2);
    EXPORT s32 sub_43A730(u8 a2);
    EXPORT void sub_43A780(u8 a2);
    EXPORT void sub_43A7D0();
    EXPORT char_type sub_43A850();
    EXPORT Car_B0* sub_43A950();
    EXPORT Car_B0* sub_43A970();
    EXPORT void sub_43A9A0(cool_nash_0x294* a2);
    EXPORT void sub_43A9F0();
    EXPORT Car_BC* sub_43AA60();
    EXPORT char_type sub_43AAF0(s32 a2);
    EXPORT s32 sub_43ADC0(s32 a2);
    EXPORT bool sub_43AF10();
    EXPORT char_type sub_43AF40();
    EXPORT Car_78* sub_43AF60();
    EXPORT char_type sub_43AFE0(s32 a2);
    EXPORT bool sub_43B140(s32 a2);
    EXPORT bool sub_43B2B0(cool_nash_0x294* a2);
    EXPORT Car_10* sub_43B340(u8 a2);
    EXPORT char_type sub_43B360();
    EXPORT void sub_43B380();
    EXPORT char_type sub_43B3D0();
    EXPORT s32 sub_43B420(s32 a2, u32* a3, u32* a4);
    EXPORT s32* sub_43B5A0(s32 a2, u32* a3, s32* a4);
    EXPORT char_type sub_43B730();
    EXPORT char_type sub_43B750();
    EXPORT Char_8* sub_43B770();
    EXPORT char_type sub_43B7B0(Car_BC* a2);
    EXPORT bool sub_43B850(s32 a2);
    EXPORT void sub_43B870(s32 a2, s32 a3);
    EXPORT s32 sub_43BB90(char_type a1);
    EXPORT char_type sub_43BBC0();
    EXPORT void sub_43BC30();
    EXPORT void sub_43BCA0();
    EXPORT Car_B0* sub_43BD00();
    EXPORT char_type sub_43BD40();
    EXPORT s32 sub_43BF10();
    EXPORT u32 sub_43BF70();
    EXPORT char_type sub_43BFE0();
    EXPORT char_type sub_43C0C0();
    EXPORT u32 sub_43C1C0();
    EXPORT car_info* sub_43C260();
    EXPORT car_info* sub_43C310();
    EXPORT car_info* sub_43C3C0();
    EXPORT void sub_43C470();
    EXPORT s32 sub_43C500();
    EXPORT s32 sub_43C650();
    EXPORT s32 sub_43C700();
    EXPORT void sub_43C840();
    EXPORT s32 sub_43C920();
    EXPORT s16 sub_43C9D0();
    EXPORT s16 sub_43CA80();
    EXPORT s32 sub_43CBE0();
    EXPORT Car_BC* sub_43CDF0(char_type a2);
    EXPORT char_type sub_43CF30(s32 a2);
    EXPORT bool sub_43D1C0(s32 a2);
    EXPORT void sub_43D2C0(char_type a2, s32 a3);
    EXPORT s32 sub_43D400();
    EXPORT s32* sub_43D690(Car_BC* a1, s32 a2, s32 a3, s32 a4, s32 a5);
    EXPORT void sub_43D7B0(s32 a2);
    EXPORT void sub_43D840(s32 a2);
    EXPORT s16 sub_43DA90(s16 a2, s32 a3);
    EXPORT Char_8** sub_43DB80();
    EXPORT void sub_43DBD0();
    EXPORT bool sub_43DC00();
    EXPORT bool sub_43DC80(s32 a2, s32 a3);
    EXPORT char_type sub_43DD50();
    EXPORT void sub_43DD60();
    EXPORT char_type sub_43E560();
    EXPORT Car_BC* sub_43E8D0();
    EXPORT cool_nash_0x294* sub_43E990();
    EXPORT char_type sub_43EA60(s32 a2);
    EXPORT char_type sub_43F130(s32 a2);
    EXPORT u32* sub_440510(u32* a2);
    EXPORT char_type GetPassengersCount_440570();
    EXPORT void sub_440590();
    EXPORT void sub_4405F0();
    EXPORT void sub_440630(s32 a2);
    EXPORT s32 sub_440660(u8 a2);
    EXPORT u8* sub_4406B0(cool_nash_0x294* a2);
    EXPORT void sub_4406E0(cool_nash_0x294* a2);
    EXPORT void sub_4407F0();
    EXPORT Car_3C* sub_440840();
    EXPORT s32 sub_440AC0();
    EXPORT s32 sub_440B10();
    EXPORT s32 sub_440B60();
    EXPORT s32 sub_440BB0();
    EXPORT char_type sub_440C10(char_type a2);
    EXPORT char_type sub_440D90(char_type a2);
    EXPORT void sub_440F90(char_type a2);
    EXPORT char_type sub_441030(u8 a2, u8 a3, u8 a4, s32 a5);
    EXPORT char_type sub_441080(u8 a2, u8 a3, u8 a4, s32 a5);
    EXPORT char_type sub_4410D0(s16 a2, u8* a3, s32 a4, s32 a5);
    EXPORT char_type sub_441360();
    EXPORT void sub_441380();
    EXPORT char_type sub_4413B0(s32 a2, s32 a3, s32 a4);
    EXPORT void sub_441520();
    EXPORT void sub_4415C0();
    EXPORT u32* sub_441600(u32* a2);
    EXPORT char_type sub_4416D0(s32 a2);
    EXPORT void sub_4417D0();
    EXPORT void sub_4417F0();
    EXPORT char_type sub_441800(char_type a2);
    EXPORT Car_A4_10* sub_4418A0();
    EXPORT Car_A4_10* sub_4418B0();
    EXPORT char_type
    sub_4418D0(char_type a2, char_type a3, char_type a4, char_type a5, char_type a6, char_type a7, char_type a8, char_type a9);
    EXPORT void sub_4419E0();
    EXPORT Car_B0* sub_441A10();
    EXPORT char_type sub_441A40();
    EXPORT char_type sub_441A70();
    EXPORT void sub_441B00();
    EXPORT s16 sub_441B20();
    EXPORT char_type sub_441B50();
    EXPORT void sub_441C00();
    EXPORT s16 sub_441D40();
    EXPORT void sub_441E70();
    EXPORT bool sub_442170();
    EXPORT void sub_442190();
    EXPORT char_type sub_4421B0();
    EXPORT bool sub_442200();
    EXPORT void sub_442310();
    EXPORT char_type sub_4424C0();
    EXPORT s16* sub_442520(s16* a2);
    EXPORT void sub_4425D0();
    EXPORT void sub_4426D0();
    EXPORT Car_A4_10* sub_442760();
    EXPORT Car_BC* sub_4427A0(Car_BC* a2);
    EXPORT s32 sub_442810();
    EXPORT s32* sub_442D10();
    EXPORT char_type sub_442D70();
    EXPORT char_type sub_443130();
    EXPORT char_type sub_443170();
    EXPORT char_type sub_443330(s32 a2);
    EXPORT char_type sub_443360(s32 a2, s32 a3, s32 a4, s16 a5);
    EXPORT char_type sub_4435A0();
    EXPORT s32 sub_4435B0();
    EXPORT void sub_4435F0();
    EXPORT Car_6C* sub_443710(s32 a2);
    EXPORT void sub_443AE0(s32 a2);
    EXPORT void sub_443BD0(s32 a2);
    EXPORT void sub_443C40(s32 a2);
    EXPORT s32 sub_443D00(s32 xpos, s32 ypos, s32 zpos);
    EXPORT void sub_443D70(s32 a2);
    EXPORT Car_6C* sub_443DA0(s32 a2);
    EXPORT Car_6C* sub_443E50();
    EXPORT void sub_443E80();
    EXPORT void sub_443EB0(s32 a2);
    EXPORT Car_6C* sub_443EE0(s32 a2);
    EXPORT s32 sub_443F30(s32 a2, s32 a3, s32 a4, s32 a5);
    EXPORT char_type sub_444020();
    EXPORT u32 sub_444090(Car_BC* a1);
    EXPORT void sub_4441B0();
    EXPORT Car_6C* sub_444490();
    EXPORT void sub_4446E0();
    EXPORT s32 sub_4447D0();

    Car_BC(); // 0x444860
    ~Car_BC(); // 0x444960

    EXPORT s32 sub_446730(Car_BC* a1);
    EXPORT char_type sub_447360();
    EXPORT Car_3C* sub_52A6D0(Car_3C* a2);
    EXPORT bool sub_564300();

    s32 field_0_qq;
    Ped_Unknown_4 field_4;
    s32 field_8;
    Car_10 field_C[3];
    s32 field_3C;
    s32 field_40;
    s32 field_44;
    s32 field_48;
    Car_BC* field_4C_next;
    Car_3C* field_50_car_sprite;
    cool_nash_0x294* field_54_driver;
    Car_78* field_58_uni_Car78_or_Car_B0;
    Car_78* field_5C;
    Hamburger_40* field_60;
    Car_A4_10* field_64;
    s32 field_68;
    s32 field_6C_maybe_id;
    s32 field_70;
    s16 field_74_damage;
    s16 field_76;
    u8 field_78_flags;
    u8 field_79;
    s16 field_7A;
    cool_nash_0x294* field_7C_uni_num;
    char_type field_80;
    char_type field_81;
    char_type field_82;
    char_type field_83;
    s32 field_84_car_info_idx;
    s32 field_88;
    char_type field_8C;
    char_type field_8D;
    char_type field_8E;
    char_type field_8F;
    s32 field_90;
    char_type field_94;
    char_type field_95;
    char_type field_96;
    char_type field_97;
    s32 field_98;
    s32 field_9C;
    s32 field_A0;
    char_type field_A4;
    char_type field_A5;
    char_type field_A6;
    u8 field_A7;
    char_type field_A8;
    char_type field_A9;
    char_type field_AA;
    char_type field_AB;
    s32 field_AC;
    s32 field_B0;
    s32 field_B4;
    char_type field_B8;
    char_type field_B9;
    char_type field_BA;
    char_type field_BB;
};
GTA2_ASSERT_SIZEOF_ALWAYS(Car_BC, 0xBC)

struct Car_E0C4
{
    Car_BC* field_0;
    Car_BC* field_4_firstCar;
    Car_BC field_8_cars[306];
    s16 field_E0C0_cars_count;
    s16 field_E0C2;
};

class Car_8
{
  public:
    EXPORT void dtor_40AC40();
    EXPORT Car_8* ctor_563970();
    Sprite_4C* field_0;
    Sprite_4C* field_4;
};

EXPORT_VAR extern Car_E0C4* gCar_E0C4_67792C;
