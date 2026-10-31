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
    bool mIsHijacked; // 0x0
    char_type mUnused1; // 0x1
    bool mPassengersFleeing; // 0x2
    char_type mUnused3; // 0x3
    s16 mWaitTimer; // 0x4
    char_type mUnused6; // 0x6
    char_type mUnused7; // 0x7
    s32 mAllocState; // 0x8
    Car* mCarriages[11]; // 0xC
    char_type mExplosionTimers[11]; // 0x38
    u8 mWagonCount; // 0x43
    char_type mUnused44; // 0x44
    char_type mUnused45; // 0x45
    char_type mUnused46; // 0x46
    char_type mUnused47; // 0x47
    s32 mStopPhase; // 0x48
    TrainStation* mStation; // 0x4C
    s32 mState; // 0x50
    char_type mPassengerTimer; // 0x54
    char_type mUnused55; // 0x55
    char_type mPassengerCount; // 0x56
    char_type mTrackIdx; // 0x57
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

    s32 mStationType; // 0x0
    gmp_map_zone* mEntryPoint; // 0x4
    gmp_map_zone* mExitPoint; // 0x8
    gmp_map_zone* mStopPoint; // 0xC
    gmp_map_zone* mpZone; // 0x10
    s32 mUsed; // 0x14
    Train* mpStoppedTrain; // 0x18
    s32 mPlatformState; // 0x1C
    TrainStation* mNextStation; // 0x20
    u8 mTrainWagons[10]; // 0x24
    char_type mWagonsNumber; // 0x2E
    char_type mTrackIdx; // 0x2F
    char_type mUnused30; // 0x30
    char_type mUnused31; // 0x31
    char_type mUnused32; // 0x32
    char_type mUnused33; // 0x33
};
GTA2_ASSERT_SIZEOF_ALWAYS(TrainStation, 0x34);

// Plain array/u8 globals instead of these structs change the registers of SetupTrainAndBusStops_5794B0
struct TrainStationList
{
    TrainStation* mList[5]; // 0x0
};

struct TrainStopCount
{
    u8 mCount; // 0x0
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

    TrainStation mStations[100]; // 0x0
    Train mTrainArray[10]; // 0x1450
    Train mBus; // 0x17C0
    bool mStopGettingOffBus; // 0x1818
    char_type mUnused1819; // 0x1819
    char_type mUnused181A; // 0x181A
    char_type mUnused181B; // 0x181B
};
GTA2_ASSERT_SIZEOF_ALWAYS(PublicTransport, 0x181C);

EXTERN_GLOBAL(PublicTransport*, gPublicTransport_181C_6FF1D4);

EXPORT bool __stdcall CompareNameChars_577E90(const char_type* pChar1, const char_type* pChar2);

EXPORT gmp_map_zone* __stdcall FindStationZoneByName_577EE0(const char_type* pChar, u8 case_value);
