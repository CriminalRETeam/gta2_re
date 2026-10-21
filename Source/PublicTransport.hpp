#pragma once

#include "Function.hpp"

class Car_BC;
class gmp_map_zone;
class Fix16;
class TrainStation_34;

class Train_58
{
  public:
    EXPORT void ReassignTrainHead_578030();
    EXPORT void IncrementState_578180();
    EXPORT void DecrementState_5781F0();
    EXPORT Train_58();
    EXPORT ~Train_58();
    EXPORT void sub_5782D0();
    EXPORT void Stop_578300();
    EXPORT void CloseCarriageDoors_578330();
    EXPORT void OpenCarriageDoors_578360();
    EXPORT void UpdatePassengerAI_578390();
    EXPORT void ProcessTrainExplosionChain_578670();
    char_type field_0;
    char_type field_1_unused;
    char_type field_2;
    char_type field_3_unused;
    s16 field_4_wait_timer;
    char_type field_6_unused;
    char_type field_7_unused;
    s32 field_8_alloc_state;
    Car_BC* field_C_carriages[11];
    char_type field_38_explosion_timers[11];
    u8 field_43_idx;
    char_type field_44_unused;
    char_type field_45_unused;
    char_type field_46_unused;
    char_type field_47_unused;
    s32 field_48_stop_phase;
    TrainStation_34* field_4C_maybe_train_station;
    s32 field_50_state;
    char_type field_54_passenger_timer;
    char_type field_55_unused;
    char_type field_56_passenger_count;
    char_type field_57_track_idx;
};

class TrainStation_34
{
  public:
    // returns a car_model_enum
    EXPORT s32 GetWagonType_577f80(u8 idx);
    EXPORT TrainStation_34();
    EXPORT ~TrainStation_34();

    // NOTE: This function defined out of order for some reason
    EXPORT void CalculateWagonCount_578820(u8* pWagons); //  Maybe 'a2' is the train wagon array of size 10 bytes

    s32 field_0_station_type;
    gmp_map_zone* field_4_entry_point;
    gmp_map_zone* field_8_exit_point;
    gmp_map_zone* field_C_stop_point;
    gmp_map_zone* field_10_pZone;
    s32 field_14_used;
    Train_58* field_18_pStoppedTrain; // train stopped at the station?
    s32 field_1C_bOccupied;
    TrainStation_34* field_20_next_station;
    u8 field_24_train_wagons[10];
    char_type field_2E_wagons_number;
    char_type field_2F_track_idx;
    char_type field_30_unused;
    char_type field_31_unused;
    char_type field_32_unused;
    char_type field_33_unused;
};

// Plain array/u8 globals instead of these structs change the registers of SetupTrainAndBusStops_5794B0
struct TrainStationList
{
    TrainStation_34* field_0_list[5];
};

struct TrainStopCount
{
    u8 field_0_count;
};

EXTERN_GLOBAL(TrainStationList, gTrainStationList_6FEE68);
EXTERN_GLOBAL(TrainStopCount, gTrainStopCount_6FEFFC);

class PublicTransport_181C
{
  public:
    EXPORT Train_58* AllocateTrain_578790();
    EXPORT TrainStation_34* AllocateTrainStation_5787E0();
    EXPORT void SpawnTrainsFromStations_578860();
    EXPORT void InitStationsLinkedList_5793E0();
    EXPORT void InitTrainStations_579440();
    EXPORT void SetupTrainAndBusStops_5794B0();
    EXPORT TrainStation_34* GetBusStopOnScreen_5799B0();
    EXPORT void OnBusStopMarkerHit_579A30(Car_BC* pToFind);
    EXPORT bool is_bus_579AA0(Car_BC* pCar);
    EXPORT Car_BC* sub_579AD0();
    EXPORT bool is_bus_full_579AF0();
    EXPORT void IncrementBusPassengerCount_579B10();
    EXPORT void KillAllPassengers_579B20();
    EXPORT Car_BC** GetCarArrayFromLeadCar_579B40(Car_BC* toFind);
    EXPORT bool GetTrainSpeed_579B90(Car_BC* pToFind, Fix16* pF16Unk);
    EXPORT void BusesService_579CA0();
    EXPORT void PublicTransportService_57A7A0();
    EXPORT TrainStation_34* TrainStationForZone_57B4B0(gmp_map_zone* pZone);
    EXPORT Car_BC* GetLeadTrainCar_57B540(Car_BC* pCar);
    EXPORT Train_58* GetTrainFromCar_57B5C0(Car_BC* pToFind);
    EXPORT Train_58* GetTrainFromCarExcludingLeadCar_57B6A0(Car_BC* pToFind);
    EXPORT bool AreCarsInDifferentTrains_57B740(Car_BC* pCar1, Car_BC* pCar2);
    EXPORT PublicTransport_181C();
    EXPORT ~PublicTransport_181C();

    TrainStation_34 field_0_stations[100];
    Train_58 field_1450_train_array[10];
    Train_58 field_17C0_bus;
    char_type field_1818_stop_getting_off_bus;
    char_type field_1819_unused;
    char_type field_181A_unused;
    char_type field_181B_unused;
};

EXTERN_GLOBAL(PublicTransport_181C*, gPublicTransport_181C_6FF1D4);

EXPORT bool __stdcall CompareNameChars_577E90(char_type* pChar1, char_type* pChar2);

EXPORT gmp_map_zone* __stdcall FindStationZoneByName_577EE0(char_type* pChar, u8 case_value);
