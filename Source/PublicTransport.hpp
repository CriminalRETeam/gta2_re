#pragma once

#include "Function.hpp"

class Car_BC;
class gmp_map_zone;


class Train_58
{
  public:
    EXPORT void sub_578030();
    EXPORT void sub_578180();
    EXPORT void sub_5781F0();
    EXPORT Train_58();
    EXPORT ~Train_58();
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
    Car_BC* field_C_first_carriage;
    Car_BC* field_10[10];
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

class TrainStation_34
{
  public:
    // returns a car_model_enum
    EXPORT s32 GetWagonType_577f80(u8 idx);
    EXPORT TrainStation_34();
    EXPORT ~TrainStation_34();

    // NOTE: This function defined out of order for some reason
    EXPORT void sub_578820(u8* a2); //  Maybe 'a2' is the train wagon array of size 10 bytes

    s32 field_0_bus_or_train;
    gmp_map_zone* field_4_entry_point;
    gmp_map_zone* field_8_exit_point;
    gmp_map_zone* field_C_stop_point;
    gmp_map_zone* field_10_pZone;
    s32 field_14;
    s32 field_18;
    s32 field_1C;
    TrainStation_34* field_20_next_station;
    u8 field_24_train_wagons[10];
    char_type field_2E_wagons_number;
    char_type field_2F;
    char_type field_30;
    char_type field_31;
    char_type field_32;
    char_type field_33;
};

class PublicTransport_181C
{
  public:
    EXPORT Train_58* AllocateTrain_578790();
    EXPORT TrainStation_34* AllocateTrainStation_5787E0();
    EXPORT void sub_578860();
    EXPORT char_type sub_5793E0();
    EXPORT gmp_map_zone* sub_579440();
    EXPORT gmp_map_zone* sub_5794B0();
    EXPORT char_type* sub_5799B0();
    EXPORT void sub_579A30(Car_BC* a2);
    EXPORT bool is_bus_579AA0(Car_BC* pCar);
    EXPORT Car_BC* sub_579AD0();
    EXPORT bool is_bus_full_579AF0();
    EXPORT void sub_579B10();
    EXPORT void KillAllPassengers_579B20();
    EXPORT Car_BC** sub_579B40(Car_BC* toFind);
    EXPORT bool sub_579B90(Car_BC* a2, u32* a3);
    EXPORT void BusesService_579CA0();
    EXPORT void PublicTransportService_57A7A0();
    EXPORT TrainStation_34* TrainStationForZone_57B4B0(gmp_map_zone* pZone);
    EXPORT Car_BC* sub_57B540(Car_BC* a2);
    EXPORT Train_58* GetTrainFromCar_57B5C0(Car_BC* a2);
    EXPORT Train_58* sub_57B6A0(Car_BC* a2);
    EXPORT bool sub_57B740(Car_BC* a2, Car_BC* a3);
    EXPORT PublicTransport_181C();
    EXPORT ~PublicTransport_181C();

    TrainStation_34 field_0[100];
    Train_58 field_1450_train_array[10];
    Train_58 field_17C0_bus;
    char_type field_1818_stop_getting_off_bus;
    char_type field_1819;
    char_type field_181A;
    char_type field_181B;
};

EXTERN_GLOBAL(PublicTransport_181C*, gPublicTransport_181C_6FF1D4);
