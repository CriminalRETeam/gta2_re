#pragma once

#include "Function.hpp"

class Car_BC;
class gmp_map_zone;

class Sero_58
{
  public:
    EXPORT void sub_578030();
    EXPORT void sub_578180();
    EXPORT void sub_5781F0();
    EXPORT void ctor_578260();
    EXPORT void dtor_5782C0();
    EXPORT void sub_5782D0();
    EXPORT void sub_578300();
    EXPORT void sub_578330();
    EXPORT void sub_578360();
    EXPORT void sub_578390();
    EXPORT u8 sub_578670();
    char_type field_0;
    char_type field_1;
    char_type field_2;
    char_type field_3;
    s16 field_4;
    char_type field_6;
    char_type field_7;
    s32 field_8;
    Car_BC* field_C[11];
    char_type field_38[10];
    char_type field_42;
    u8 field_43_idx;
    char_type field_44;
    char_type field_45;
    char_type field_46;
    char_type field_47;
    s32 field_48;
    s32 field_4C;
    s32 field_50;
    char_type field_54;
    char_type field_55;
    char_type field_56_passenger_count;
    char_type field_57;
};

class Sero_34
{
  public:
    EXPORT void ctor_577FD0();
    EXPORT void dtor_578010();
    s32 field_0;
    s32 field_4;
    s32 field_8;
    s32 field_C;
    gmp_map_zone* field_10_pZone;
    s32 field_14;
    s32 field_18;
    s32 field_1C;
    s32 field_20;
    s32 field_24;
    s32 field_28;
    s16 field_2C;
    char_type field_2E;
    char_type field_2F;
    s32 field_30;
};

class Sero_181C
{
  public:
    EXPORT Sero_58* sub_578790();
    EXPORT Sero_34* sub_5787E0();
    EXPORT void sub_578820(s32 a2);
    EXPORT void sub_578860();
    EXPORT char_type sub_5793E0();
    EXPORT gmp_map_zone* sub_579440();
    EXPORT gmp_map_zone* sub_5794B0();
    EXPORT char_type* sub_5799B0();
    EXPORT void sub_579A30(Car_BC* a2);
    EXPORT bool is_bus_579AA0(Car_BC* pCar);
    EXPORT Car_BC* sub_579AD0();
    EXPORT bool is_bus_full_579AF0();
    EXPORT char_type sub_579B10();
    EXPORT void sub_579B20();
    EXPORT Car_BC** sub_579B40(Car_BC* toFind);
    EXPORT bool sub_579B90(Car_BC* a2, u32* a3);
    EXPORT void sub_579CA0();
    EXPORT void sub_57A7A0();
    EXPORT Sero_34* sub_57B4B0(gmp_map_zone* pZone);
    EXPORT Car_BC* sub_57B540(Car_BC* a2);
    EXPORT Sero_58* sub_57B5C0(Car_BC* a2);
    EXPORT Sero_58* sub_57B6A0(Car_BC* a2);
    EXPORT bool sub_57B740(Car_BC* a2, Car_BC* a3);
    EXPORT Sero_181C();
    EXPORT ~Sero_181C();

    Sero_34 field_0[100];
    Sero_58 field_1450[10];
    Sero_58 field_17C0;
    char_type field_1818;
    char_type field_1819;
    char_type field_181A;
    char_type field_181B;
};

EXPORT_VAR extern Sero_181C* gSero_181C_6FF1D4;
