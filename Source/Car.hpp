#pragma once

#include "car_flags.hpp"
#include "BitSet32.hpp"
#include "CarAI.hpp"
#include "Car_10.hpp"
#include "Fix16.hpp"
#include "Function.hpp"
#include "Object_3C.hpp"
#include "Ped.hpp"
#include "Ped_List.hpp"
#include "Pool.hpp"
#include "ang16.hpp"
#include "gtx_0x106C.hpp"
#include "miss2_xyz.hpp"
#include "sprite.hpp"
#include "car_despawn_status.hpp"
#include "map_0x370.hpp"
#include <wchar.h>

struct gmp_zone_info;
struct SCR_THREAD;
class Camera;
class Object;
class Fix16_Rect;
class CarAI;
class CarPhysics;
class Trailer;
class Ped;
class PedListNode;
class CarChaseTask;
class Ped_List;
class car_info;
class SoundObject;
class Gang;
class Player;

EXTERN_GLOBAL(Fix16, kFP16One_6777D0);

EXPORT Ang16 __stdcall ReturnAngleFromRoadDirection(s32& road_direction);

namespace car_kind
{
enum
{
    none_0 = 0,
    recycled_1 = 1,
    proto_recycled_2 = 2,
    Unknown_3 = 3,
    paramedic_car_4 = 4,
    firefighter_5 = 5,
    police_6 = 6,
    roadblock_car_7 = 7,
    mission_car_8 = 8,
    parked_car_9 = 9,
    call_police_car_10 = 10,
};
} // namespace car_kind

namespace car_engine_status
{
enum
{
    off_1 = 1,
    stopping_2 = 2, // transient: Car::UpdateEngineStatus turns the headlights off, then off_1
    on_3 = 3,
    starting_4 = 4, // transient: start sound, headlights on, then on_3
    destroying_5 = 5, // transient: headlights off, then destroyed_6
    destroyed_6 = 6,
    crushed_7 = 7,
};
} // namespace car_engine_status

// The original has a separate Fix16 constant before each array (raw 0x100, 0x200, Fix16(5)); splitting them out of
// this struct changes MapRenderer and Draw code (declaration count, docs/matching_quirks.md).
struct car_rng_list
{
    car_rng_list(Fix16 constant)
    {
        this->separate_constant = constant;
    }
    
    Fix16 separate_constant;
    s32 values[256];
};

class CarThreadTrigger
{
  public:
    s32 mpScriptCmd; // 0x0
    Object* mO2C; // 0x4
    s32 mType; // 0x8
    s32 mAction; // 0xC
    u8 mRemapRng; // 0x10
    s32 mEnableState; // 0x14
};
GTA2_ASSERT_SIZEOF_ALWAYS(CarThreadTrigger, 0x18);

class CarThreadTriggers
{
  public:
    EXPORT void FreeTrigger(u8 idx);
    EXPORT char_type AddThreadTrigger(const s32& type, const s32& param, SCR_THREAD* pScriptThread, Fix16 xpos, Fix16 ypos, Fix16 zpos, Fix16 width, Fix16 height);
    EXPORT void Reset();
    EXPORT void CheckThreadTrigger(u8 idx, Sprite* pSprite);

    // 9.6f 0x4768C0
    inline void SetTriggerEnableState(u8 idx, s32 state)
    {
        mTriggers[idx].mEnableState = state;
    }

    CarThreadTrigger mTriggers[22]; // 0x0
    s16 mCount; // 0x210
    s16 mUnused212; // 0x212
};
GTA2_ASSERT_SIZEOF_ALWAYS(CarThreadTriggers, 0x214);

EXTERN_GLOBAL(CarThreadTriggers*, gCarThreadTriggers_214);
EXTERN_GLOBAL_ARRAY(u16, gRngRemapTable, 1000);

class CarRemapRng
{
  public:
    EXPORT CarRemapRng();
    ~CarRemapRng() {}
    EXPORT void IncNextRngRemapIdx();

    // 9.6f 0x4212D0
    u16 GetRngValue() const
    {
        return gRngRemapTable[this->mTableIdx];
    }

    u16 mTableIdx; // 0x0
};
GTA2_ASSERT_SIZEOF_ALWAYS(CarRemapRng, 0x2);

EXTERN_GLOBAL(Fix16, kFpHalf_6F7690);

EXTERN_GLOBAL(Fix16, kFpFive_6F77D4);

EXTERN_GLOBAL(Ang16, kAngZero_6F804C);

EXTERN_GLOBAL(Ang16, kAng180_6F771E);

EXTERN_GLOBAL(Fix16, kFpZero_6F77C0);
EXTERN_GLOBAL(Fix16, kFpOne_6F77C4);
EXTERN_GLOBAL(Fix16, kFpOne_679E74);

class CarManager
{
  public:
    STATIC_MEMBER(Sprite*, sSpriteUnused);
    STATIC_MEMBER(s32, sGoodCarModelsCount);
    STATIC_MEMBER(s32, sBadCarModelsCount);
    STATIC_MEMBER(s32, sAverageCarModelsCount);
    // 9.6f 0x421960
    inline Ped* GetSpareDriver() const
    {
        return mSpareDriver;
    }

    // 9.6f 0x421970
    inline void SetSpareDriver(Ped* pPed)
    {
        mSpareDriver = pPed;
    }

    // 9.6f 0x4C39F0
    void ToggleTrafficLightState()
    {
        if (mLightStateA != 4)
        {
            mLightStateA = 4;
        }
        else
        {
            mLightStateA = 3;
        }

        if (mLightStateB != 1)
        {
            mLightStateB = 1;
        }
        else
        {
            mLightStateB = 2;
        }
    }

    EXPORT void DistributeCarsByRating();
    EXPORT u32 SelectTrafficCarModel(Player* pPlayer, gmp_zone_info* pZoneInfo, Fix16 density_scale, u16* pOutCategory);
    EXPORT bool NoRefs(s32, s32, s32, s32, s32);
    EXPORT Car* SpawnCarAtRoadDirection(s32 car_model_type, Fix16 xpos, Fix16 ypos, Fix16 zpos);
    EXPORT Car* GetNearestCarFromCoord(Fix16 x, Fix16 y, Fix16 z, Ped* pPed);
    EXPORT Car* GetNearestEnterableCarFromCoord(Fix16 x, Fix16 y, Fix16 z, Ped* pPed);
    EXPORT Car* DoGetNearestCarFromCoord(Fix16 xpos,
                                                   Fix16 ypos,
                                                   Fix16 zpos,
                                                   s32 bMatchDriverless,
                                                   Ped* pPed,
                                                   bool bIgnorePedRestrictions);

    EXPORT Car* GetNearestFrontVehicle(Sprite* pSprite, u8 max_rings);
    EXPORT Car* SpawnBusAtValidRoadPosition(Fix16 x, Fix16 y, s32 side, const s32& car_model);
    EXPORT Car* SpawnCarOnRoadNetwork(Fix16 xpos, Fix16 ypos, s32 road_direction, s32 car_model_type);
    EXPORT Car* SpawnCarAt(Fix16 xpos, Fix16 ypos, Fix16 zpos, Ang16 rotation, s32 car_info_idx, Fix16 scale);
    EXPORT Trailer* SpawnCabAndTrailer(Fix16 xpos, Fix16 ypos, Ang16 rotation, s32 car_idx, s32 trailer_idx);
    EXPORT void RemoveFromPoolAndCollision(Car* pCar);

    EXPORT void DecrementAllocatedCarType(s32 car_kind);
    EXPORT void EnableFreeShoppingIfAllKFsPassed();
    EXPORT void CarsService();
    EXPORT bool CanAllocateCarOfKindWithSpare(s32 type) const;
    EXPORT bool CanAllocateCarOfKind(s32 type) const;
    EXPORT CarManager();
    EXPORT ~CarManager();

    // 9.6f inlined
    inline Car* SpawnCar_426E10(Fix16 xpos, Fix16 ypos, Fix16 zpos, Ang16 rotation, s32 car_info_idx)
    {
        return SpawnCarAt(xpos, ypos, zpos, rotation, car_info_idx, kFpOne_6F77C4);
    }

    // SpawnCar_426E10 with the scale passed in: 10.5 loads a different Fix16(1) global per TU
    // (kFpOne_6F77C4 in ScriptThread, kOne_6FE614 in Player)
    inline Car* SpawnCar_426E10(Fix16 xpos, Fix16 ypos, Fix16 zpos, Ang16 rotation, s32 car_info_idx, const Fix16& scale)
    {
        return SpawnCarAt(xpos, ypos, zpos, rotation, car_info_idx, scale);
    }

    // 9.6f inlined
    // 9.6f 0x4764A0
    inline Car* SpawnCar(Fix16 xpos, Fix16 ypos, Fix16 zpos, Ang16 rotation, s32 car_info_idx)
    {
        return SpawnCarAt(xpos, ypos, zpos, rotation, car_info_idx, kFpHalf_6F7690);
    }

    inline Car* SpawnCar_426E10_v2(Fix16 xpos, Fix16 ypos, Fix16 zpos, Ang16 rotation, s32 car_info_idx)
    {
        return SpawnCarAt(xpos, ypos, zpos, rotation, car_info_idx, kFP16One_6777D0);
    }

    // unknown inlined function
    inline Car* SpawnCar_shortened(s32 car_info_idx)
    {
        return SpawnCarAt(kFpFive_6F77D4, kFpFive_6F77D4, kFpZero_6F77C0, kAngZero_6F804C, car_info_idx, kFpOne_6F77C4);
    }

    // As SpawnCarAtCorrectZ with the scale passed in (PublicTransport::SpawnTrainsFromStations)
    inline Car* SpawnCarAtCorrectZ_Scaled(Fix16 xpos, Fix16 ypos, const Ang16& rotation, s32 car_model, const Fix16& scale)
    {
        if (car_model == car_model_enum::TRAIN || car_model == car_model_enum::TRAINCAB || car_model == car_model_enum::TRAINFB ||
            car_model == car_model_enum::boxcar)
        {
            return SpawnCarAt(xpos, ypos, gMap_0x370->GetRailwayZCoordAtXY(xpos, ypos), rotation, car_model, scale);
        }
        else
        {
            return SpawnCarAt(xpos, ypos, gMap_0x370->FindGroundZForCoord(xpos, ypos), rotation, car_model, scale);
        }
    }

    // As SpawnCarAtCorrectZ but with the scale passed in (TrafficSpawner::SpawnTrafficCar passes its own kFpOne_6FF778)
    inline Car* SpawnCarAtCorrectZ_WithScale(Fix16 xpos, Fix16 ypos, Ang16 rotation, s32 car_model, const Fix16& scale)
    {
        if (car_model == car_model_enum::TRAIN || car_model == car_model_enum::TRAINCAB || car_model == car_model_enum::TRAINFB ||
            car_model == car_model_enum::boxcar)
        {
            return SpawnCarAt(xpos, ypos, gMap_0x370->GetRailwayZCoordAtXY(xpos, ypos), rotation, car_model, scale);
        }
        else
        {
            return SpawnCarAt(xpos, ypos, gMap_0x370->FindGroundZForCoord(xpos, ypos), rotation, car_model, scale);
        }
    }

    // 9.6f 0x426E40
    inline Car* SpawnCarAtCorrectZ(Fix16 xpos, Fix16 ypos, Ang16 rotation, s32 car_model)
    {
        if (car_model == car_model_enum::TRAIN || car_model == car_model_enum::TRAINCAB || car_model == car_model_enum::TRAINFB ||
            car_model == car_model_enum::boxcar)
        {
            return SpawnCarAt(xpos, ypos, gMap_0x370->GetRailwayZCoordAtXY(xpos, ypos), rotation, car_model, kFP16One_6777D0);
        }
        else
        {
            return SpawnCarAt(xpos, ypos, gMap_0x370->FindGroundZForCoord(xpos, ypos), rotation, car_model, kFP16One_6777D0);
        }
    }

    CarRemapRng mModelRng; // 0x0
    s16 mUnused2; // 0x2
    Ped* mSpareDriver; // 0x4
    char_type mUnused8; // 0x8
    u8 mGoodCarIdx; // 0x9
    u8 mBadCarIdx; // 0xA
    u8 mAverageCarIdx; // 0xB
    s32 mSuppressedModel; // 0xC
    CarRemapRng mRemapRng; // 0x10
    CarRemapRng mUnused12; // 0x12
    s32 mNextCarId; // 0x14
    char_type mUnused18; // 0x18
    char_type mUnused19; // 0x19
    char_type mFireEffectCycle; // 0x1A
    char_type mUnused1B; // 0x1B
    s32 mExplosionOffsetCycle; // 0x1C
    s32 mLightStateA; // 0x20
    s32 mLightStateB; // 0x24
    s32 mRecycledCars; // 0x28
    s32 mParamedicCars; // 0x2C
    s32 mFirefighterCars; // 0x30
    s32 mUnitCars; // 0x34
    s32 mRoadblockCars; // 0x38
    s32 mMissionCars; // 0x3C
    s32 mProtoRecycledCars; // 0x40
    s32 mParkedCars; // 0x44
    s32 mKind10Cars; // 0x48
    Fix16_Point mTvVanDir; // 0x4C
    u8 mSpawnBlock; // 0x54
    char_type mVisibleCarsCount; // 0x55
    char_type mUnused56; // 0x56
    char_type mUnused57; // 0x57
    s32 mModelToCheckDestroy; // 0x58
    bool mModelCheckDestroyed; // 0x5C
    char_type mUnused5D; // 0x5D
    char_type mUnused5E; // 0x5E
    char_type mUnused5F; // 0x5F
    s32 mImpactMode; // 0x60
    Fix16 mZpos; // 0x64
    bool mObjectHitActive; // 0x68
    bool mDoFreeShopping; // 0x69
    char_type mUnused6A; // 0x6A
    char_type mUnused6B; // 0x6B
};
GTA2_ASSERT_SIZEOF_ALWAYS(CarManager, 0x6C);

EXTERN_GLOBAL(CarManager*, gCarManager_6C);

class Trailer
{
  public:
    EXPORT Ped* GetTruckCabDriver() const;
    EXPORT Car* GetCabOrTrailerCar(Car* pCar) const;
    EXPORT void SetTruckCabAndTrailerCar(Car* pTruckCab, Car* pTrailerCar);
    EXPORT Fix16_Point GetHitchPosition();
    EXPORT void UpdateTrailerAlignment();
    EXPORT bool ServicePhysics();
    EXPORT void SetupCarPhysicsAndSpriteBinding();
    EXPORT void DeAllocateCarPhysics();
    EXPORT bool ExplodeBothIfOneDestroyed();
    EXPORT s32 Update();

    void PoolAllocate()
    {
    }

    void PoolDeallocate()
    {
    }

    // 9.6f 0x49EFB0
    inline char_type IsHitchAngleClamped() const
    {
        return mHitchAngleClamped;
    }

    //Inlined in CarManager constructor 9.6f -> 0x4212d0
    __forceinline Trailer()
    {
        mpNext = NULL;
        mTruckCab = NULL;
        mTrailerCar = NULL;
        mHitchAngleClamped = 0;
    }

    char_type mHitchAngleClamped; // 0x0: set by UpdateTrailerAlignment when the trailer angle hits its window, cleared on hitching
    char_type mUnused1; // 0x1
    char_type mUnused2; // 0x2
    char_type mUnused3; // 0x3
    Trailer* mpNext;
    Car* mTruckCab; // 0x8
    Car* mTrailerCar; // 0xC
};

static inline bool IsArmyModel(s32 car_model)
{
    return car_model == car_model_enum::TANK || car_model == car_model_enum::GUNJEEP || car_model == car_model_enum::JEEP || car_model == car_model_enum::apc;
}

class Car
{
  public:
    // 9.6f 0x42AC70
    inline void SetTurnRight()
    {
        mBfA6.bTurnRight = true;
    }

    // 9.6f 0x42AC80
    inline void SetTurnLeft()
    {
        mBfA6.bTurnLeft = true;
    }

    // 9.6f 0x42AC90
    inline void SetTurnRightReady()
    {
        mBfA6.bTurnRightReady = true;
    }

    // 9.6f 0x421510
    inline void AllocateAI_421510()
    {
        if (!mAI)
        {
            mAI = gCarAI_78_Pool->Allocate();
        }
    }

    // 9.6f 0x421530
    inline void SetTurnLeftReady()
    {
        mBfA6.bTurnLeftReady = true;
    }

    // 9.6f 0x421540
    inline void SetAIHalted()
    {
        mBfA6.bAIHalted = true;
    }

    // 9.6f 0x421550
    inline void ClearAIHalted()
    {
        mBfA6.bAIHalted = false;
    }

    // 9.6f 0x42ACA0
    inline void ClearTurnRight()
    {
        mBfA6.bTurnRight = false;
    }

    // 9.6f 0x42ACB0
    inline void ClearTurnLeft()
    {
        mBfA6.bTurnLeft = false;
    }

    // 9.6f 0x42AC60
    inline bool IsAIHalted() const
    {
        return mBfA6.bAIHalted == 1;
    }

    // 9.6f 0x42AC20
    inline bool IsTurnRight() const
    {
        return mBfA6.bTurnRight == 1;
    }

    // 9.6f 0x42AC30
    inline bool IsTurnLeft() const
    {
        return mBfA6.bTurnLeft == 1;
    }

    // 9.6f 0x42AC40
    inline bool IsTurnRightReady() const
    {
        return mBfA6.bTurnRightReady == 1;
    }

    // 9.6f 0x42AC50
    inline bool IsTurnLeftReady() const
    {
        return mBfA6.bTurnLeftReady == 1;
    }

    // 9.6f 0x4118C0
    inline bool IsEngineOn() const
    {
        return mEngineStatus == car_engine_status::on_3;
    }

    // 9.6f 0x421610
    inline bool IsTrain() const
    {
        return mCarInfoIdx == car_model_enum::TRAIN;
    }

    // 9.6f 0x421950
    inline void SetShopEvent(s32 v)
    {
        mShopEvent = v;
    }

    // 9.6f 0x4A51B0
    inline void SetDamage(s16 v)
    {
        mDamage = v;
    }

    EXPORT bool CanBeEnteredByPed(Ped* pPed) const;
    EXPORT bool IsEnterable() const;
    EXPORT Ang16 GetCornerAngle() const;

    EXPORT s16 ApplyImpactDamage(Fix16 damage);

    EXPORT bool IsPoliceCar() const;
    EXPORT Fix16 GetDamageFactorOnSpeed() const;
    EXPORT Fix16 GetMaxSpeed() const;
    EXPORT wchar_t* GetCarStr();
    EXPORT Fix16_Point GetHitchPoint() const;
    EXPORT Fix16_Point GetRearWheelOffset() const;
    EXPORT Fix16 GetMass() const;
    EXPORT Fix16 GetAntiStrength() const;
    EXPORT bool IsBus();
    EXPORT bool HasSpriteZoom() const;
    EXPORT Fix16 GetCarLinearSpeed();
    EXPORT bool IsCarInAir() const;
    EXPORT Ang16 GetOrientationAngle() const;
    EXPORT Fix16_Point GetLinearVelocity();
    EXPORT Fix16 GetVelocity() const;
    EXPORT Fix16 GetMomentOfInertia();
    EXPORT Fix16 GetMinDimension() const;
    EXPORT void RemoveAllDamage();
    EXPORT bool AllowResprayOrPlates();
    EXPORT bool IsNotCurrentRemap(u8 remap) const;
    EXPORT bool IsNotCurrentRemapOfCarAndTrailerCar(u8 remap) const;
    EXPORT void SetCarRemap(u8 remap);
    EXPORT void AssignRandomRemap();
    EXPORT char_type GetCarModelForPhysics();
    EXPORT void DoBrake();
    EXPORT void DoBrakeAndHandbrake();
    EXPORT void SetDriver(Ped* pNewDriver);
    EXPORT void TriggerCarAlarm();
    EXPORT void SetDummyControl();
    EXPORT void StartLightFlashing();
    EXPORT void Deactivate();
    EXPORT bool IsThreatToSearchingPed() const;
    EXPORT bool CanCarCollideWithSprite(Sprite* pSprite);
    EXPORT void ProcessCarToCarImpact(Sprite* pSprite);
    EXPORT bool CanExitCar() const;
    EXPORT void ResumeAIDriving();
    EXPORT void HaltAIDriving();
    EXPORT bool IsDoorAccessible(u8 target_door);
    EXPORT bool IsStoppedWithPavementAtDoor(u8 target_car_door);
    EXPORT bool IsDoorLockedForPed(Ped* pPed) const;
    EXPORT Car_Door* GetDoor(u8 door_idx);
    EXPORT char_type GetDoorCount() const;
    EXPORT void OpenAllDoors();
    EXPORT void CloseAllDoors();
    EXPORT void GetDoorWorldPos(u8 door_idx, Fix16* pOutX, Fix16* pOutY);
    EXPORT bool IsDoorOffsetLarge(u8 targetDoor) const;
    EXPORT void GetDoorWorldPosition(u8 targetDoor, Fix16* pOutX, Fix16* pOutY);
    EXPORT bool IsOnScreenForAnyPlayer() const;
    EXPORT bool IsVisibleToAnyPlayer() const;
    EXPORT void AssignKillerToOccupants();
    EXPORT void AssignDriverBlameForExplosion(Car* pCar);
    EXPORT bool IsImmuneToExplosionType(s32 explosion_type) const;
    EXPORT void SpawnDamageFireEffect(s32 fire_size, const Fix16_Point* pPos);
    // Called without this in SpawnDamageFireEffect: a static __stdcall
    EXPORT static s32 __stdcall GetFireExplosionType(u8 fire_level);
    EXPORT void SpawnFire();
    EXPORT void SetupCarPhysicsAndSpriteBinding_43BC30();
    EXPORT void SetupCarPhysicsAndSpriteBinding_43BCA0();
    EXPORT void DeAllocateCarPhysics_43BD00();
    EXPORT void FlashHeadlights();
    EXPORT void BrakeLightsOn();
    EXPORT void BrakeLightsOff();
    EXPORT void HeadlightsOn();
    EXPORT void HeadlightsOff();
    EXPORT void PrepareForExplosion();
  private:
    EXPORT void UpdateTopRightRoofLight();
    EXPORT void ResetTopRightRoofLight();
    EXPORT void UpdateTopLeftRoofLight();
    EXPORT void ResetTopLeftRoofLight();
    EXPORT void UpdateRoofLights();
    EXPORT void ResetRoofLights();
    EXPORT void UpdateBottomLeftRoofLight();
    EXPORT void ResetBottomLeftRoofLight();
  public:
    EXPORT void ActivateEmergencyLights();
    EXPORT void DeactivateEmergencyLights();
    EXPORT void ToggleEmergencyLights();
    EXPORT void StopLightFlashing();
    EXPORT void SetCarGraphic(char_type graphic_idx);
    EXPORT void DamageArea(s32 damage_area);
    EXPORT bool IsAreaDamaged(s32 damage_area);
    EXPORT void TryDamageArea(u8 damage_area, s32 damageAmount);
    EXPORT void RepairDamage();
    EXPORT void EmitExplosion(s32 type, Fix16 x, Fix16 y);
    EXPORT void TriggerExplosion(s32 type);
    EXPORT void HandleCarExplosion(s32 type);
    EXPORT s16 AccumulateDamage(s16 damage, const Fix16_Point* pVec);
    // HandleCarHitByObject passes the operator temporaries straight in (push %eax of the result)
    inline s16 AccumulateDamage(s16 damage, const Fix16_Point& vec)
    {
        return AccumulateDamage(damage, &vec);
    }
    EXPORT void KillContainedPeds();
    EXPORT void StopMovement();
    EXPORT bool IsSpriteShrunk() const;
    EXPORT bool CarShrinkSprite(s32 xoff, s32 yoff);
    EXPORT bool IsBeingCrushed();
    EXPORT void DestroyWhileSinking();
    EXPORT bool ManageDrowning();
    EXPORT Car* GetCabOrSelf();
    EXPORT Ped* GetEffectiveDriver() const;
    EXPORT bool OnObjectTouched(Object* pObj);
    EXPORT bool HandleCarHitByObject(Object* pObj);
    EXPORT Fix16 GetMaxTurnRate();
    EXPORT char_type GetMaxPassengers() const;
    EXPORT void InitCarAIControl();
    EXPORT void SpawnDriverPed();
    EXPORT void AddGangDriver(Gang* pGang);
    EXPORT void AttachGangIcon(u8 arrow_colour);
    EXPORT void ShowCarName(Ped* pPed);
    EXPORT void AssignDriver(Ped* pPed);
    EXPORT void ClearDriver();
    EXPORT Sprite* GetSprite() const;
    EXPORT void PutWaterCannonOnRoof();
    EXPORT void PutTankCannonOnRoof();
    EXPORT void PutMachineGunOnRoof();
    EXPORT void PutTV_Antenna();
    EXPORT bool RotateRoofObjectTowardTarget(Ang16 targetAngle);
    EXPORT bool HandleRoofTurretRotation(char_type left_on);
    EXPORT void FireCarBomb(char_type instant_bomb);
    EXPORT void GoToBlockTest(u8 x, u8 y, u8 z, s32 direction);
    EXPORT void GotoBlock(u8 x, u8 y, u8 z, s32 direction);
    EXPORT char_type CountConsecutiveArrowBlocks(Ang16 ang, s8* pOutCount, Fix16 sprite_x, Fix16 sprite_y);
    EXPORT Fix16 GetZPos() const;
    EXPORT void CountDownToWreck();
    EXPORT void TurnToWreck();
    EXPORT void StartWreckCountdown();
    EXPORT void UpdateTrainCarriagesOnTrack(Fix16 xpos, Fix16 ypos, Fix16 zpos);
    EXPORT void UpdateEngineStatus();
    EXPORT void UpdateBrakeLights();
    EXPORT Fix16_Rect GetOrientedHalfExtentsRect() const;
    EXPORT void TryHonkHorn(s32 horn_mode);
    EXPORT void UpdateHorn();
    EXPORT void HonkHorn();
    EXPORT void HandleSpecialInput(char_type now_special_pressed);
    EXPORT void DoDetachTrailer();
    EXPORT void FlashAndDetachTrailer();
    EXPORT void HandleUserInput(char_type forward_gas_on,
                                    char_type foot_brake_on,
                                    char_type left_on,
                                    char_type right_on,
                                    char_type hand_brake_on,
                                    char_type now_special_pressed,
                                    char_type was_special_pressed,
                                    char_type attack);
    EXPORT void AllocCarPhysics();
    EXPORT void DeAllocateCarPhysics_441A10();
    EXPORT bool AreAllDoorsClosed();
  private:
    EXPORT void InitDoors_441A70();
    EXPORT void ServiceDoors();
    EXPORT void UpdateTaxiLight();
    EXPORT void UpdateRoofLightFlasher();
    EXPORT void UpdatePopupHeadlights();
    EXPORT void UpdateFbiPopupSiren();
    EXPORT void UpdateLights();
    EXPORT bool IsCarInConditionsToDespawn() const;
    EXPORT void ServicePhysics();
    EXPORT bool IsMissionRelated();
    EXPORT bool CanDespawn();
    EXPORT void ManageDespawning();
  public:
    EXPORT bool UpdateCarDespawnStatus();
    EXPORT Ang16 GetRadioTowerAngle() const;
    EXPORT void ManageTVAntenna();
    EXPORT void ServiceDoorsLightsAndHorn();
    EXPORT void DetachTrailer();
    EXPORT void AttachTrailer(Car* pTrailerCar);
    EXPORT void TryHitchTrailer();
    EXPORT void LightUpdate();
    EXPORT bool TrainUpdate();
    EXPORT bool TrailerUpdate();
    EXPORT bool PoolUpdate();
    EXPORT void UpdateOnCraneHook();
    EXPORT bool UpdateAttachedToSprite(Sprite* pSprite, Fix16 x, Fix16 y, Ang16 rot);
    EXPORT void InitDoors_4435A0();
    EXPORT s32 GetAttachedDrawLayer() const;
    EXPORT void UpdateDrawLayer();
    EXPORT void ApplyExplosionImpulse(Fix16_Point* xy);
    EXPORT static s32 __stdcall GetCarWeaponCost(s32 weapon_kind);
    EXPORT void BuyCarWeapon(s32 weapon_kind);
    EXPORT static void __stdcall ShowCantAffordMessage(Player* pPlayer, s32 weapon_cost);
    EXPORT void ResprayOrChangePlates(u8 remap);
    EXPORT void ResprayOrCleanPlates(u8 remap);
    EXPORT void HandleShops(Object* pObj);
    EXPORT void SetPosition(Fix16 xpos, Fix16 ypos, Fix16 zpos);
    EXPORT void IncrementCarStats(s32 car_kind);
    EXPORT void IncrementAllocatedCarType(s32 car_kind);
    EXPORT void MarkProtoRecycled();
    EXPORT void MarkRecycled();
    EXPORT void ReassignCarStats(s32 car_kind);
    EXPORT void ReassignAllocatedCarType(s32 car_kind);
    EXPORT void AttachLight(s32 object_type, s32 argb, s32 x_offset, s32 y_offset);
    EXPORT void UpdateExploderTimer();
    EXPORT u32 GetEffectiveDriverPedId();
    EXPORT void SetSirens();
    EXPORT void PoolAllocate();
    EXPORT void DeAllocateAI();
    EXPORT void PoolDeallocate();

    Car(); // 0x444860
    ~Car(); // 0x444960

    EXPORT void ClearHeadlightDeltas_447360();
    EXPORT bool IsDrivenByNonPlayer();

    // 9.6f 0x411930
    inline bool IsFlagSet(u16 flag) const
    {
        if (mFlags & flag)
        {
            return true;
        }
        return false;
    }

    // 9.6f 0x425770
    inline bool IsFlameproof() const
    {
        return IsFlagSet(car_flag::Flameproof);
    }

    // 9.6f 0x414F70
    inline bool IsCrushing() const
    {
        return IsFlagSet(car_flag::Crushing);
    }

    // FUNCTION: 96f 0x4118d0
    s32 IsDrivenByPlayer() const
    {
        if (mDriver != NULL && mDriver->mPlayer != NULL)
        {
            return true;
        }
        return false;
    }

    // Inlined 0x421720
    bool IsTrailerCar() const
    {
        // We are on the trailer
        return mpTrailer && mpTrailer->mTrailerCar == this;
    }

    // 9.6f 0x4118B0
    Ped* GetDriver() const
    {
        return mDriver;
    }

    // Inlined 0x41E460
    bool IsTrailerCab() const
    {
        // We are the cab of the trailer
        return mpTrailer && mpTrailer->mTruckCab == this;
    }

    // Inlined 0x475E60
    bool HasAttachedTrailer(Car* pTrailerCar) const
    {
        return mpTrailer && mpTrailer->mTrailerCar == pTrailerCar && this != pTrailerCar;
    }

    // 9.6f 0x4BA390
    bool IsCargoCarOf(Car* pOther) const
    {
        return mpTrailer && mpTrailer->mTruckCab == pOther && this != pOther;
    }

    inline bool IsWithinArea(SCR_Rect_f* rect)
    {
        Fix16 x_pos = mCarSprite->mXy.x;
        Fix16 width = rect->mSize.mX;
        Fix16 y_pos, z_pos;
        Fix16 height;
        Fix16 z_target;
        return (
            x_pos >= rect->mPos.x - width && x_pos <= rect->mPos.x + width &&
            (y_pos = mCarSprite->mXy.y,
             height = rect->mSize.mY,
             y_pos >= rect->mPos.y - height) &&
            mCarSprite->mXy.y <= rect->mPos.y + height &&
            (z_pos = mCarSprite->mZpos, z_target = rect->mPos.z, z_pos.ToUInt8() == z_target.ToUInt8()));
    }

    inline bool IsWithinBlock(Fix16_Vec* pos)
    {
        Sprite* pSprite = mCarSprite;
        Fix16 car_z_pos;
        return (pSprite->mXy.x >= pos->x.ToUInt8() && pSprite->mXy.x < (pos->x.ToUInt8() + 1)

                && pSprite->mXy.y >= pos->y.ToUInt8() && pSprite->mXy.y < (pos->y.ToUInt8() + 1)

                && (car_z_pos = pSprite->mZpos, car_z_pos.ToUInt8() == pos->z.ToUInt8()));
    }

    // 9.6f 0x403BA0
    bool IsTrainModel() const
    {
        return mCarInfoIdx == car_model_enum::TRAIN || mCarInfoIdx == car_model_enum::TRAINCAB ||
            mCarInfoIdx == car_model_enum::TRAINFB || mCarInfoIdx == car_model_enum::boxcar;
    }

    // 9.6f inline 0x421560
    void SetDriverPedType(s32 driver_ped_type)
    {
        mPedType = driver_ped_type;
        mLastSeenTimer = 0;
    }

    // 9.6f tests the flag in a car_info method (0x4112A0); adding that method to car_info changes code
    // elsewhere (shared header names, docs/matching_quirks.md)
    inline bool HasEmergencyInfoFlag() const
    {
        return (gGtx_0x106C->get_car_info_5AA3B0(mCarInfoIdx)->info_flags & car_info_flag::emergency) == car_info_flag::emergency;
    }

    // 9.6f 0x40F890
    bool IsMaxDamage() const
    {
        return mDamage == 32001;
    }

    // 9.6f 0x414F80
    inline bool AreEmergencyLightsOn() const
    {
        return HasEmergencyLights() && mBfA4.bEmergencyLights;
    }

    // 9.6f 0x4119D0
    inline s32 TakeShopEvent()
    {
        s32 ret = mShopEvent;
        mShopEvent = 0;
        return ret;
    }

    // 9.6f 0x411950
    inline s32 TakeSoundEvent()
    {
        s32 ret = mSoundEvent;
        mSoundEvent = 0;
        return ret;
    }

    // FUNCTION: 96f 0x411940
    inline s32 GetCarInfoIdx() const
    {
        return mCarInfoIdx;
    }

    // 9.6f 0x414F20
    bool HasEmergencyLights() const
    {
        return HasEmergencyInfoFlag() || IsFbiCar();
    }

    // 9.6f inline 0x421700
    inline bool HasPopupHeadlights() const
    {
        return (gGtx_0x106C->get_car_info_5AA3B0(mCarInfoIdx)->info_flags_2 & car_info_flag2::popup_headlights) == car_info_flag2::popup_headlights;
    }

    // 9.6f inline 0x421660
    inline bool HasRoofLight() const
    {
        return (gGtx_0x106C->get_car_info_5AA3B0(mCarInfoIdx)->info_flags & car_info_flag::roof_light) == car_info_flag::roof_light;
    }

    // 9.6f 0x421640
    inline bool IsTrailerInfo() const
    {
        return (gGtx_0x106C->get_car_info_5AA3B0(mCarInfoIdx)->info_flags & car_info_flag::trailer) == car_info_flag::trailer;
    }

    // 9.6f inline 0x4216C0
    inline bool HasTaxiLight() const
    {
        return (gGtx_0x106C->get_car_info_5AA3B0(mCarInfoIdx)->info_flags & car_info_flag::taxi_light) == car_info_flag::taxi_light;
    }

    // 9.6f inline 0x421680
    inline bool HasBottomLeftRoofLight() const
    {
        return (gGtx_0x106C->get_car_info_5AA3B0(mCarInfoIdx)->info_flags & car_info_flag::bottom_roof_light) == car_info_flag::bottom_roof_light;
    }

    // 9.6f inline 0x4216A0
    inline bool HasTopRoofLights() const
    {
        return (gGtx_0x106C->get_car_info_5AA3B0(mCarInfoIdx)->info_flags & car_info_flag::top_roof_lights) == car_info_flag::top_roof_lights;
    }

    // 9.6f 0x421810
    void SetBottomLeftRoofLightDelta()
    {
        mDamagedAreas.set_bit(CarDeltaBitsEnum::BottomLeftRoofLight_15);
    }

    // 9.6f 0x4218C0
    void SetBulletproof()
    {
        AddFlags(car_flag::Bulletproof);
    }

    // 9.6f 0x4218D0
    void SetFlameproof()
    {
        AddFlags(car_flag::Flameproof);
    }

    // 9.6f 0x44A3E0
    void SetNoCollide()
    {
        AddFlags(car_flag::NoCollide);
    }

    // 9.6f 0x475C10
    inline void LockUnlessPermanent()
    {
        if (mDoorLock != Car_Door_Lock::locked_permanently_4)
        {
            mDoorLock = Car_Door_Lock::locked_1;
        }
    }

    // 9.6f 0x476360
    inline void SetCarThreadPending()
    {
        mCarThreadFlags |= 1;
    }

    // 9.6f 0x475C40
    inline void LockoutThiefUnlessPermanent()
    {
        if (mDoorLock != Car_Door_Lock::locked_permanently_4)
        {
            mDoorLock = Car_Door_Lock::lockout_thief_2;
        }
    }

    // 9.6f 0x476270
    inline void SetMiniCar()
    {
        AddFlags(car_flag::MiniCar);
    }

    // 9.6f 0x475C80
    inline void UnlockUnlessPermanent()
    {
        if (mDoorLock != Car_Door_Lock::locked_permanently_4)
        {
            mDoorLock = Car_Door_Lock::unlocked_3;
        }
    }

    // 9.6f 0x475C30
    inline void LockPermanently()
    {
        mDoorLock = Car_Door_Lock::locked_permanently_4;
    }

    // 9.6f 0x421490
    inline void SetDespawn3IfNot5_421490()
    {
        if (mDespawnStatus != car_despawn_status::despawning_5)
        {
            mDespawnStatus = car_despawn_status::despawn_soon_3;
        }
    }

    // 9.6f 0x4218A0
    void SetNoCollisionDamage()
    {
        AddFlags(car_flag::NoCollisionDamage);
    }

    // 9.6f 0x4218B0
    void SetTrain()
    {
        AddFlags(car_flag::Train);
    }

    // 9.6f 0x4343B0
    inline s32 GetCarKind() const
    {
        return mCarKind;
    }

    // 9.6f inline 0x425650
    inline void ClearHeadlightDeltas_425650()
    {
        if (HasPopupHeadlights())
        {
            // clear the popup headlight door frames
            mDamagedAreas.clear_bit(CarDeltaBitsEnum::TopRightDoor1_11);
            mDamagedAreas.clear_bit(CarDeltaBitsEnum::TopRightDoor2_12);
            mDamagedAreas.clear_bit(CarDeltaBitsEnum::TopRightDoor3_13);
            mDamagedAreas.clear_bit(CarDeltaBitsEnum::TopRightDoor4_14);

            mDamagedAreas.clear_bit(CarDeltaBitsEnum::TopLeftDoor1_28);
            mDamagedAreas.clear_bit(CarDeltaBitsEnum::TopLeftDoor2_29);
            mDamagedAreas.clear_bit(CarDeltaBitsEnum::TopLeftDoor3_30);
            mDamagedAreas.clear_bit(CarDeltaBitsEnum::TopLeftDoor4_31);
        }
        mDamagedAreas.clear_bit(CarDeltaBitsEnum::FrontRightHeadlight_6); // clear left front headlight
        mDamagedAreas.clear_bit(CarDeltaBitsEnum::FrontLeftHeadlight_23); // clear right front headlight
        if (HasRoofLight())
        {
            mDamagedAreas.clear_bit(CarDeltaBitsEnum::BottomLeftRoofLight_15); // clear right siren light or roof light
        }
    }

    // 9.6f inline 0x421430
    inline void ClearBrakeLightDeltas()
    {
        mDamagedAreas.clear_bit(CarDeltaBitsEnum::BackRightBrakeLight_5); // clear left rear brake light
        mDamagedAreas.clear_bit(CarDeltaBitsEnum::BackLeftBrakeLight_22); // clear right rear brake light
    }

    // 9.6f inline 0x4213D0
    inline void SetBrakeLightDeltas()
    {
        if (!mDamagedAreas.mask_bit(CarDeltaBitsEnum::TopRightDamage_1)) // if not rear left damage
        {
            mDamagedAreas.set_bit(CarDeltaBitsEnum::BackRightBrakeLight_5); // set left rear brake light
        }
        if (!mDamagedAreas.mask_bit(CarDeltaBitsEnum::TopLeftDamage_0)) // if not rear right damage
        {
            mDamagedAreas.set_bit(CarDeltaBitsEnum::BackLeftBrakeLight_22); // set right rear brake light
        }
    }

    // 9.6f inline 0x425590
    inline void SetHeadlightDeltas()
    {
        if (!mDamagedAreas.mask_bit(CarDeltaBitsEnum::BottomRightDamage_2)) // if not front left damage
        {
            if (HasPopupHeadlights())
            {
                mDamagedAreas.set_bit(CarDeltaBitsEnum::TopRightDoor1_11);
            }
            else
            {
                mDamagedAreas.set_bit(CarDeltaBitsEnum::FrontRightHeadlight_6); // set left front headlight
            }
        }

        if (!mDamagedAreas.mask_bit(CarDeltaBitsEnum::BottomLeftDamage_3)) // if not front right damage
        {
            if (HasPopupHeadlights())
            {
                mDamagedAreas.set_bit(CarDeltaBitsEnum::TopLeftDoor1_28);
            }
            else
            {
                mDamagedAreas.set_bit(CarDeltaBitsEnum::FrontLeftHeadlight_23); // set right front headlight
            }
        }
        if (HasRoofLight())
        {
            mDamagedAreas.set_bit(CarDeltaBitsEnum::BottomLeftRoofLight_15); // set right siren light or roof light
        }
    }

    // 0x48A930
    Fix16 GetCarWidth() const
    {
        return mCarSprite->GetWidth();
    }

    // 0x48A950
    Fix16 GetCarHeight() const
    {
        return mCarSprite->GetHeight();
    }

    // 9.6f 0x4214B0
    bool IsMarkedForDespawn() const
    {
        return mDespawnStatus == car_despawn_status::despawn_pending_2 || mDespawnStatus == car_despawn_status::marked_for_despawn_4 || mDespawnStatus == car_despawn_status::despawn_soon_3;
    }

    // 9.6f 0x421870
    inline Player* GetDriverPlayer() const
    {
        return mDriver->mPlayer;
    }

    // 9.6f 0x421460
    inline void ClearHorn()
    {
        mHorn = 0;
    }

    // 9.6f 0x4215F0
    inline void CancelDespawn()
    {
        if (mDespawnStatus == car_despawn_status::despawn_pending_2 || mDespawnStatus == car_despawn_status::marked_for_despawn_4 || mDespawnStatus == car_despawn_status::despawn_soon_3)
        {
            mDespawnStatus = car_despawn_status::active_1;
        }
    }

    // 9.6f 0x4218F0
    inline s32 IsCarThreadPending() const
    {
        return mCarThreadFlags & 1;
    }

    // 9.6f 0x4218E0
    inline void ClearCarThreadPending()
    {
        mCarThreadFlags &= ~1u;
    }

    // 9.6f 0x421570
    inline void ResetEngineStatus()
    {
        if (mEngineStatus == car_engine_status::destroyed_6 || mEngineStatus == car_engine_status::destroying_5)
        {
            mEngineStatus = car_engine_status::off_1;
        }
    }

    // 9.6f 0x421590
    inline void SetEngineDamaged()
    {
        if (mEngineStatus != car_engine_status::destroyed_6)
        {
            mEngineStatus = car_engine_status::destroying_5;
        }
    }

    // 9.6f 0x421490
    inline void DespawnSoon_421490()
    {
        if (mDespawnStatus != car_despawn_status::despawning_5)
        {
            mDespawnStatus = car_despawn_status::despawn_soon_3;
        }
    }

    // 9.6f 0x421470
    void MarkForDespawn()
    {
        if (mDespawnStatus != car_despawn_status::despawning_5 && mDespawnStatus != car_despawn_status::despawn_pending_2 && mDespawnStatus != car_despawn_status::despawn_soon_3)
        {
            mDespawnStatus = car_despawn_status::marked_for_despawn_4;
        }
    }

    // 9.6f 0x4214D0
    bool IsDeactivated() const
    {
        return mDespawnStatus == car_despawn_status::deactivated_7;
    }

    // 9.6f 0x4214E0
    void SetDeactivatedStatus()
    {
        mDespawnStatus = car_despawn_status::deactivated_7;
    }

    // FUNCTION: 96f 0x447ea0
    void ResetDespawnStatus()
    {
        mDespawnStatus = car_despawn_status::active_1;
    }

    // 9.6f 0x411910
    bool IsGunJeep() const
    {
        return mCarInfoIdx == car_model_enum::GUNJEEP;
    }

    // 9.6f 0x411900
    bool IsTank() const
    {
        return mCarInfoIdx == car_model_enum::TANK;
    }

    // 9.6f 0x4217A0
    bool IsSwatVan() const
    {
        return mCarInfoIdx == car_model_enum::SWATVAN;
    }

    // 9.6f 0x403BC0
    inline bool IsSwatVanOrBankVan() const
    {
        return mCarInfoIdx == car_model_enum::SWATVAN || mCarInfoIdx == car_model_enum::bank_van;
    }

    // 9.6f 0x4119F0
    inline bool IsTurretRotated() const
    {
        return mTurretRotated;
    }

    // 9.6f 0x4118F0
    bool IsFireTruck() const
    {
        return mCarInfoIdx == car_model_enum::FIRETRUK;
    }

    // 9.6f 0x421780
    bool IsMediCar() const
    {
        return mCarInfoIdx == car_model_enum::MEDICAR;
    }

    // 9.6f 0x421790
    bool IsCopCar() const
    {
        return mCarInfoIdx == car_model_enum::COPCAR;
    }

    // 9.6f 0x411920
    bool IsFbiCar() const
    {
        return mCarInfoIdx == car_model_enum::EDSELFBI;
    }

    // 9.6f 0x4217D0
    bool IsGt24640() const
    {
        return mCarInfoIdx == car_model_enum::GT24640;
    }

    // 9.6f 0x4217E0
    bool IsTvVan() const
    {
        return this->mCarInfoIdx == car_model_enum::TVVAN;
    }

    // 9.6f 0x447EC0
    bool IsTruckTransport() const
    {
        return this->mCarInfoIdx == car_model_enum::TRUKTRNS;
    }

    // 9.6f 0x4215C0
    bool HasPlayerDriver() const
    {
        return mDriver && mDriver->mPlayer &&
            mDriver->get_occupation() != ped_ocupation_enum::empty;
    }

    // 9.6f 0x447EB0
    bool IsEngineCrushed() const
    {
        return this->mEngineStatus == car_engine_status::crushed_7;
    }

    // 9.6f 0x421910
    Fix16 ApplyScale(Fix16 value) const
    {
        if (mScale != kFP16One_6777D0)
        {
            return value * mScale;
        }
        else
        {
            return value;
        }
    }

    // 9.6f 0x41E430
    Fix16 GetX() const
    {
        return mCarSprite->mXy.x;
    }

    // 9.6f 0x41E440
    Fix16 GetY() const
    {
        return mCarSprite->mXy.y;
    }

    // 9.6f 0x41E450
    Fix16 GetZ() const
    {
        return mCarSprite->mZpos;
    }

    inline bool IsEmittingHorn_411970() const
    {
        return mHorn > 0 && mHorn <= 0xF8u;
    }

    inline bool IsEmittingHorn_411990() const
    {
        return mHorn <= 0xF8u && mHorn > 0;
    }

    // 9.6f 0x4A9AD0
    s16 GetLastSeenTimer() const
    {
        return mLastSeenTimer;
    }

    // FUNCTION: 96f 0x4215b0
    bool IsDespawning() const
    {
        return mDespawnStatus == car_despawn_status::despawning_5;
    }

    // 9.6f 0x421620
    bool IsTruckCab() const
    {
        car_info* pCarInfo =  gGtx_0x106C->get_car_info_5AA3B0(mCarInfoIdx);
        return pCarInfo->IsTruckCab();
    }

    // 9.6f 0x447ED0
    bool IsLongerThanOneBlock() const
    {
        return (mCarSprite->GetHeight() > kFpOne_679E74) ? true : false;
    }

    // 9.6f 0x447F00
    bool CanBeLiftedByCrane() const
    {
        return !IsTruckCab() && !gGtx_0x106C->get_car_info_5AA3B0(mCarInfoIdx)->IsTrailer() && !IsTrainModel() && !IsLongerThanOneBlock();
    }

    // 9.6f 0x426E00
    void StartEngine()
    {
        mEngineStatus = car_engine_status::on_3;
        HeadlightsOn();
    }

    // 9.6f 0x416B80
    inline void GetXYZ(Fix16* pX, Fix16* pY, Fix16* pZ) const
    {
        *pX = mCarSprite->mXy.x;
        *pY = mCarSprite->mXy.y;
        *pZ = mCarSprite->mZpos;
    }

    // 9.6f 0x416BB0
    inline Ang16 GetCarRotation() const
    {
        return mCarSprite->mRotation;
    }

    // 9.6f 0x4216E0
    inline bool CanCollideOver() const
    {
        car_info* pCarInfo = gGtx_0x106C->get_car_info_4BF1F0(mCarInfoIdx);
        return pCarInfo->IsHeavy();
    }

    // 9.6f 0x4214F0
    inline bool HasNonPlayerDriver() const
    {
        return mDriver && !mDriver->PedTypeIs(ped_type::player_2);
    }

    // 9.6f 0x49EFE0
    inline bool CanCrushCars() const
    {
        return CanCollideOver() && (IsTank() || !HasNonPlayerDriver());
    }

    // 9.6f 0x421890
    inline void AddFlags(u16 flag)
    {
        mFlags |= flag;
    }

    // 9.6f 0x49EFC0
    inline void SetCrushFlag()
    {
        AddFlags(car_flag::Crushing);
    }

    // 9.6f 0x49EFD0
    inline void ClearCrushFlag()
    {
        mFlags &= static_cast<u16>(~car_flag::Crushing);
    }

    EXPORT bool SnapCarToGreenArrow(Fix16 xpos, Fix16 ypos, Fix16 zpos);
    EXPORT bool TrySnapCarToNearestDrivableRoadAndDriveForward(Fix16 xpos, Fix16 ypos, s32 direction);

    SpriteList mAttachments; // 0x0
    Ped_List mPassengersList; // 0x4
    DamageAreaFlags mDamagedAreas; // 0x8
    Car_Door mDoors[4]; // 0xC
    Car* mpNext;
    Sprite* mCarSprite; // 0x50
    Ped* mDriver; // 0x54
    CarPhysics* mPhysics; // 0x58
    CarAI* mAI; // 0x5C
    CarChaseTask* mpChaseTask; // 0x60
    Trailer* mpTrailer; // 0x64
    Fix16 mScale; // 0x68
    s32 mCarId; // 0x6C
    s32 mExploderPedId; // 0x70
    s16 mDamage; // 0x74
    s16 mLastSeenTimer; // 0x76
    u16 mFlags; // 0x78: car_flag masks
    s32 mPedType; // 0x7C
    bool mAbandoned; // 0x80
    s32 mCarInfoIdx; // 0x84
    s32 mDespawnStatus; // 0x88: car_despawn_status
    u8 mDamageLevel; // 0x8C
    char_type mCarThreadFlags; // 0x8D
    char_type mFlashCount; // 0x8E
    s32 mDeathCause; // 0x90
    u8 mExploderTimer; // 0x94
    u8 mPlayerPedId; // 0x95
    s32 mDoorLock; // 0x98
    s32 mEngineStatus; // 0x9C
    s32 mCarKind; // 0xA0: police car, fire engine, mission car etc
    union
    {
        char_type mLightFlags; // 0xA4
        CarLightFlags8 mBfA4; // 0xA4
    };
    char_type mFlashPhaseCounter; // 0xA5
    union
    {
        char_type mTurnFlags; // 0xA6
        CarTurnFlags8 mBfA6; // 0xA6
    };
    u8 mHorn; // 0xA7
    u8 mHornCooldown; // 0xA8
    char_type mWreckCountdown; // 0xA9
    s32 mSoundEvent; // 0xAC
    s32 mEmitterStatus; // 0xB0
    s32 mShopEvent; // 0xB4
    bool mTurretRotated; // 0xB8
};
GTA2_ASSERT_SIZEOF_ALWAYS(Car, 0xBC)

struct CarPool
{
    //Inlined in CarManager constructor 9.6f -> 0x426db0
    CarPool()
    {
    }

    ~CarPool()
    {

    }

    // 9.6f 0x40F490
    void Remove(Car* pCar)
    {
        mPool.DeAllocate(pCar);
    }

    // 9.6f 0x45AD30
    inline s16 get_cars_count_45AD30()
    {
        return mPool.field_X_count;
    }

    // 9.6f 0x420E50
    inline Car* GetFirstCar_420E50()
    {
        return mPool.mpPrev;
    }

    // FUNCTION: 96f 0x420f20
    void UpdateNextPrev(Car* pItem)
    {
        mPool.UpdateNextPrev(pItem);
    }

    Pool<Car, 306> mPool;
};

class TrailerPool
{
  public:
    //Inlined in CarManager constructor 9.6f -> 0x425500
    TrailerPool()
    {
    }

    ~TrailerPool()
    {
       mPool.mpHead = NULL;
    }

    // 9.6f 0x425580
    inline void Remove(Trailer* pTrailer)
    {
        mPool.DeAllocate(pTrailer);
    }

    PoolBasic<Trailer, 10> mPool; // 0x0
};

struct TrafficSpawner
{
    EXPORT TrafficSpawner();
    EXPORT ~TrafficSpawner();
    EXPORT bool IsInsideCameraEdge(s32 direction, Fix16 x, Fix16 y);
    EXPORT void MakeTrafficForCurrCamera();
    EXPORT bool SpawnTrafficCarFacing(s32 angle_face);
    EXPORT void GenerateTraffic();

    // 9.6f 0x4B3230. A thiscall member: SpawnTrafficCar sets ecx = this before calling it
    EXPORT Fix16 GetRandomTrafficSpeed(Fix16 max_speed, u8* pOutSpeedBand);

    EXPORT bool SpawnTrafficCar(s32 spawn_type, s32 arrow_direction, s32 blocked_direction);

    EXPORT bool IsFlatRoadAt(Fix16 xpos, Fix16 ypos);

    EXPORT s32 GetLaneOffset(u8 lane_idx);
    EXPORT char_type ResolveSpawnDirection(char_type spawn_pos_idx, char_type road_len);

    Camera* mCam; // 0x0
    Fix16_Rect* mRect; // 0x4
    s8 mSpawnSide; // 0x8
    s8 mTrafficCarModel; // 0x9
    s8 mUnusedA; // 0xA
    s8 mUnusedB; // 0xB
    Player* mPlayer; // 0xC
    gmp_zone_info* mZone; // 0x10
};
GTA2_ASSERT_SIZEOF_ALWAYS(TrafficSpawner, 0x14);

EXTERN_GLOBAL(CarPool*, gCar_BC_Pool);

EXTERN_GLOBAL(Sprite*, gSprite);

EXTERN_GLOBAL(TrailerPool*, gTrailerPool);

EXTERN_GLOBAL(TrafficSpawner*, gTrafficSpawner_14);

EXTERN_GLOBAL(bool, gbRngRemapTableDone);

EXPORT s32 __stdcall GetDamageMultiplier(Ped* pPed);
EXPORT s32 __stdcall GetDeathCauseForObjectModel(s32 model);
EXPORT bool __stdcall IsNoKillCreditObject(s32 model);