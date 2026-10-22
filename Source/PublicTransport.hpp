#pragma once

#include "Function.hpp"

class Car;
class gmp_map_zone;
class Fix16;
class TrainStation;

class Train
{
  public:
    EXPORT void ReassignTrainHead_578030();
    EXPORT void IncrementState_578180();
    EXPORT void DecrementState_5781F0();
    EXPORT Train();
    EXPORT ~Train();
    EXPORT void StartIfHasDriver_5782D0();
    EXPORT void Stop_578300();
    EXPORT void CloseCarriageDoors_578330();
    EXPORT void OpenCarriageDoors_578360();
    EXPORT void UpdatePassengerAI_578390();
    EXPORT void ProcessTrainExplosionChain_578670();
    char_type field_0_is_hijacked;
    char_type field_1_unused;
    char_type field_2;
    char_type field_3_unused;
    s16 field_4_wait_timer;
    char_type field_6_unused;
    char_type field_7_unused;
    s32 field_8_alloc_state;
    Car* field_C_carriages[11];
    char_type field_38_explosion_timers[11];
    u8 field_43_idx;
    char_type field_44_unused;
    char_type field_45_unused;
    char_type field_46_unused;
    char_type field_47_unused;
    s32 field_48_stop_phase;
    TrainStation* field_4C_station;
    s32 field_50_state;
    char_type field_54_passenger_timer;
    char_type field_55_unused;
    char_type field_56_passenger_count;
    char_type field_57_track_idx;
};
GTA2_ASSERT_SIZEOF_ALWAYS(Train, 0x58);

class TrainStation
{
  public:
    // returns a car_model_enum
    EXPORT s32 GetWagonType_577f80(u8 idx);
    EXPORT TrainStation();
    EXPORT ~TrainStation();

    // NOTE: This function defined out of order for some reason
    EXPORT void CalculateWagonCount_578820(u8* pWagons); //  Maybe 'a2' is the train wagon array of size 10 bytes

    s32 field_0_station_type;
    gmp_map_zone* field_4_entry_point;
    gmp_map_zone* field_8_exit_point;
    gmp_map_zone* field_C_stop_point;
    gmp_map_zone* field_10_pZone;
    s32 field_14_used;
    Train* field_18_pStoppedTrain; // train stopped at the station?
    s32 field_1C_platform_state;
    TrainStation* field_20_next_station;
    u8 field_24_train_wagons[10];
    char_type field_2E_wagons_number;
    char_type field_2F_track_idx;
    char_type field_30_unused;
    char_type field_31_unused;
    char_type field_32_unused;
    char_type field_33_unused;
};
GTA2_ASSERT_SIZEOF_ALWAYS(TrainStation, 0x34);

// Plain array/u8 globals instead of these structs change the registers of SetupTrainAndBusStops_5794B0
struct TrainStationList
{
    TrainStation* field_0_list[5];
};

struct TrainStopCount
{
    u8 field_0_count;
};

EXTERN_GLOBAL(TrainStationList, gTrainStationList_6FEE68);
EXTERN_GLOBAL(TrainStopCount, gTrainStopCount_6FEFFC);

class PublicTransport
{
  public:
    EXPORT Train* AllocateTrain_578790();
    EXPORT TrainStation* AllocateTrainStation_5787E0();
    EXPORT void SpawnTrainsFromStations_578860();
    EXPORT void InitStationsLinkedList_5793E0();
    EXPORT void InitTrainStations_579440();
    EXPORT void SetupTrainAndBusStops_5794B0();
    EXPORT TrainStation* GetBusStopOnScreen_5799B0();
    EXPORT void OnBusStopMarkerHit_579A30(Car* pToFind);
    EXPORT bool is_bus_579AA0(Car* pCar);
    EXPORT Car* GetBusIfBoarding_579AD0();
    EXPORT bool is_bus_full_579AF0();
    EXPORT void IncrementBusPassengerCount_579B10();
    EXPORT void KillAllPassengers_579B20();
    EXPORT Car** GetCarArrayFromLeadCar_579B40(Car* toFind);
    EXPORT bool GetTrainSpeed_579B90(Car* pToFind, Fix16* pF16Unk);
    EXPORT void BusesService_579CA0();
    EXPORT void PublicTransportService_57A7A0();
    EXPORT TrainStation* TrainStationForZone_57B4B0(gmp_map_zone* pZone);
    EXPORT Car* GetLeadTrainCar_57B540(Car* pCar);
    EXPORT Train* GetTrainFromCar_57B5C0(Car* pToFind);
    EXPORT Train* GetTrainFromCarExcludingLeadCar_57B6A0(Car* pToFind);
    EXPORT bool AreTrainsOnDifferentTracks_57B740(Car* pCar1, Car* pCar2);
    EXPORT PublicTransport();
    EXPORT ~PublicTransport();

    TrainStation field_0_stations[100];
    Train field_1450_train_array[10];
    Train field_17C0_bus;
    char_type field_1818_stop_getting_off_bus;
    char_type field_1819_unused;
    char_type field_181A_unused;
    char_type field_181B_unused;
};
GTA2_ASSERT_SIZEOF_ALWAYS(PublicTransport, 0x181C);

EXTERN_GLOBAL(PublicTransport*, gPublicTransport_181C_6FF1D4);

EXPORT bool __stdcall CompareNameChars_577E90(char_type* pChar1, char_type* pChar2);

EXPORT gmp_map_zone* __stdcall FindStationZoneByName_577EE0(char_type* pChar, u8 case_value);
