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
    bool field_0_is_hijacked; // 0x0
    char_type field_1_unused; // 0x1
    bool field_2_passengers_fleeing; // 0x2
    char_type field_3_unused; // 0x3
    s16 field_4_wait_timer; // 0x4
    char_type field_6_unused; // 0x6
    char_type field_7_unused; // 0x7
    s32 field_8_alloc_state; // 0x8
    Car* field_C_carriages[11]; // 0xC
    char_type field_38_explosion_timers[11]; // 0x38
    u8 field_43_wagon_count; // 0x43
    char_type field_44_unused; // 0x44
    char_type field_45_unused; // 0x45
    char_type field_46_unused; // 0x46
    char_type field_47_unused; // 0x47
    s32 field_48_stop_phase; // 0x48
    TrainStation* field_4C_station; // 0x4C
    s32 field_50_state; // 0x50
    char_type field_54_passenger_timer; // 0x54
    char_type field_55_unused; // 0x55
    char_type field_56_passenger_count; // 0x56
    char_type field_57_track_idx; // 0x57
};
GTA2_ASSERT_SIZEOF_ALWAYS(Train, 0x58);

class TrainStation
{
  public:
    // returns a car_model_enum
    EXPORT s32 GetWagonType_577f80(u8 idx) const;
    EXPORT TrainStation();
    EXPORT ~TrainStation();

    // NOTE: This function defined out of order for some reason
    EXPORT void CalculateWagonCount_578820(const u8* pWagons);

    s32 field_0_station_type; // 0x0
    gmp_map_zone* field_4_entry_point; // 0x4
    gmp_map_zone* field_8_exit_point; // 0x8
    gmp_map_zone* field_C_stop_point; // 0xC
    gmp_map_zone* field_10_pZone; // 0x10
    s32 field_14_used; // 0x14
    Train* field_18_pStoppedTrain; // 0x18
    s32 field_1C_platform_state; // 0x1C
    TrainStation* field_20_next_station; // 0x20
    u8 field_24_train_wagons[10]; // 0x24
    char_type field_2E_wagons_number; // 0x2E
    char_type field_2F_track_idx; // 0x2F
    char_type field_30_unused; // 0x30
    char_type field_31_unused; // 0x31
    char_type field_32_unused; // 0x32
    char_type field_33_unused; // 0x33
};
GTA2_ASSERT_SIZEOF_ALWAYS(TrainStation, 0x34);

// Plain array/u8 globals instead of these structs change the registers of SetupTrainAndBusStops_5794B0
struct TrainStationList
{
    TrainStation* field_0_list[5]; // 0x0
};

struct TrainStopCount
{
    u8 field_0_count; // 0x0
};

EXTERN_GLOBAL(TrainStationList, gTrainStationList_6FEE68);
EXTERN_GLOBAL(TrainStopCount, gTrainStopCount_6FEFFC);

class PublicTransport
{
  private:
    EXPORT Train* AllocateTrain_578790();
    EXPORT TrainStation* AllocateTrainStation_5787E0();

  public:
    STATIC_MEMBER(u8, sStationCount);
    STATIC_MEMBER(bool, sBusSpawned);
    STATIC_MEMBER(s32, sBusSpawnDelay);
    EXPORT void SpawnTrainsFromStations_578860();

  private:
    EXPORT void InitStationsLinkedList_5793E0();
    EXPORT void InitTrainStations_579440();

  public:
    EXPORT void SetupTrainAndBusStops_5794B0();

  private:
    EXPORT TrainStation* GetBusStopOnScreen_5799B0();

  public:
    EXPORT void OnBusStopMarkerHit_579A30(Car* pToFind);
    EXPORT bool is_bus_579AA0(Car* pCar) const;
    EXPORT Car* GetBusIfBoarding_579AD0() const;
    EXPORT bool is_bus_full_579AF0() const;
    EXPORT void IncrementBusPassengerCount_579B10();
    EXPORT void KillAllPassengers_579B20();
    EXPORT Car** GetCarArrayFromLeadCar_579B40(Car* toFind);
    EXPORT bool GetTrainSpeed_579B90(Car* pToFind, Fix16* pOutSpeed);

  private:
    EXPORT void BusesService_579CA0();

  public:
    EXPORT void PublicTransportService_57A7A0();
    EXPORT TrainStation* TrainStationForZone_57B4B0(gmp_map_zone* pZone);
    EXPORT Car* GetLeadTrainCar_57B540(Car* pCar);
    EXPORT Train* GetTrainFromCar_57B5C0(Car* pToFind);
    EXPORT Train* GetTrainFromCarExcludingLeadCar_57B6A0(Car* pToFind);
    EXPORT bool AreTrainsOnDifferentTracks_57B740(Car* pCar1, Car* pCar2);
    EXPORT PublicTransport();
    EXPORT ~PublicTransport();

    TrainStation field_0_stations[100]; // 0x0
    Train field_1450_train_array[10]; // 0x1450
    Train field_17C0_bus; // 0x17C0
    bool field_1818_stop_getting_off_bus; // 0x1818
    char_type field_1819_unused; // 0x1819
    char_type field_181A_unused; // 0x181A
    char_type field_181B_unused; // 0x181B
};
GTA2_ASSERT_SIZEOF_ALWAYS(PublicTransport, 0x181C);

EXTERN_GLOBAL(PublicTransport*, gPublicTransport_181C_6FF1D4);

EXPORT bool __stdcall CompareNameChars_577E90(const char_type* pChar1, const char_type* pChar2);

EXPORT gmp_map_zone* __stdcall FindStationZoneByName_577EE0(const char_type* pChar, u8 case_value);
