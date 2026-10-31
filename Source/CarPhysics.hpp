#pragma once

#include "CarInfoTable.hpp"
#include "Car.hpp"
#include "Function.hpp"
#include "ang16.hpp"
#include "fix16.hpp"

class Sprite;
class Car;
class Trailer;
class SpriteShape;
class Ped;
class ModelPhysics;
class CarAI;

EXTERN_GLOBAL(Fix16, kFP16Zero_6FE20C);
EXTERN_GLOBAL(Fix16, kFpZero_6FD120);
EXTERN_GLOBAL(ModelPhysics*, gCarInfo_48);
EXTERN_GLOBAL(Ang16, kAng90_6FE00C);
EXTERN_GLOBAL(Ang16, kAng270_6FE154);
EXTERN_GLOBAL(CarInfo_2C*, gCarInfo_2C);
EXTERN_GLOBAL(Fix16, gDamageSpeedFactor);
EXTERN_GLOBAL(Fix16, kDrowningMaxSpeed);
EXTERN_GLOBAL(Fix16_Point, gConveyorVelocity);
EXTERN_GLOBAL(Fix16, gArrowSteerAngVel);
EXTERN_GLOBAL(Fix16, kF16Zero);
EXTERN_GLOBAL(Fix16_Point, CollisionIntersectionPoint);

EXPORT Fix16_Point __stdcall ComputeCollisionImpulse(Fix16 OwnerMass,
                                                                Fix16 TargetMass,
                                                                Fix16_Point& RelativeVelocity,
                                                                Fix16_Point& DistToCollision_ByRef,
                                                                Fix16_Point& CollisionIntersectPoint,
                                                                Fix16_Point& CoM_related,
                                                                Fix16_Point& TargetCoM,
                                                                Fix16 OwnerMomOfInertia,
                                                                Fix16 TargetMomOfInertia,
                                                                Fix16 offset);

namespace car_turn_direction
{
enum
{
    clockwise_m1 = -1,
    none_0 = 0,
    anticlockwise_1 = 1,
};
} // namespace car_turn_direction
                                                                
class CarPhysics
{
  public:
    STATIC_MEMBER(Fix16_Point, sTrailerCm1);
    STATIC_MEMBER(Fix16, sF70);
    STATIC_MEMBER(Fix16, sCp3);
    STATIC_MEMBER(Fix16_Point, sCp1);
    STATIC_MEMBER(Fix16_Point, sCm1);
    STATIC_MEMBER(Fix16, sAirZVel);
    STATIC_MEMBER(Fix16, sTrailerF70);
    STATIC_MEMBER(Fix16, sTrailerCp3);
    STATIC_MEMBER(Fix16_Point, sTrailerCp1);
    STATIC_MEMBER(Fix16, sTrailerAirZVel);
    STATIC_MEMBER(Fix16, sSavedTrailerF70);
    STATIC_MEMBER(Fix16_Point, sSavedTrailerCp1);
    STATIC_MEMBER(Fix16_Point, sSavedTrailerCm1);
    STATIC_MEMBER(Fix16, sSavedTrailerAirZVel);
    STATIC_MEMBER(Fix16, sSavedTrailedCp3);
    STATIC_MEMBER(Fix16, sSavedF70);
    STATIC_MEMBER(Fix16, sSavedCp3);
    STATIC_MEMBER(Fix16_Point, sSavedCp1);
    STATIC_MEMBER(Fix16_Point, sSavedCm1);
    STATIC_MEMBER(Fix16, sSavedAirZVel);
    STATIC_MEMBER(Fix16, sRemainingTimeStep);
    STATIC_MEMBER(u8, sOtherCollisionArea);
    STATIC_MEMBER(Fix16_Point, sCollisionNormal);
    STATIC_MEMBER(u8, sCollisionArea);
    STATIC_MEMBER(Fix16, sBrakeForce);
    // 9.6f 0x40F800
    inline Fix16 get_cp3()
    {
        return mCp3;
    }

    // 9.6f 0x40F810
    inline void set_cp3(Fix16 v)
    {
        mCp3 = v;
    }

    // 9.6f 0x40F820
    inline Ang16 get_theta()
    {
        return mTheta;
    }

    // 9.6f 0x40F830
    inline void set_theta(Ang16 v)
    {
        mTheta = v;
    }

    // 9.6f 0x40F7E0
    inline void set_cp1(const Fix16_Point& v)
    {
        mCp1.x = v.x;
        mCp1.y = v.y;
    }

    // 9.6f 0x421170
    inline void set_linvel(const Fix16_Point& v)
    {
        mLinvel1.x = v.x;
        mLinvel1.y = v.y;
    }

    // 9.6f 0x4895D0
    inline void StopMoving()
    {
        mAngVelRad = kFpZero_6FD120;
        mLinvel1.clear_41E1E0();
    }

    // 9.6f 0x4211E0
    inline Ang16 GetLinvelAngle()
    {
        return mLinvel1.GetAngle();
    }

    // 9.6f 0x421130
    inline bool IsDamageWindowOpen()
    {
        return static_cast<u32>(gpRng->get_cur_rng()) < mDamagedUntilCycle;
    }

    // 9.6f 0x421260
    inline void SetHandBrakeOff_421260()
    {
        mIsHandBrakeOn = 0;
    }

    // 9.6f 0x421250
    inline void SetHandBrakeOn()
    {
        mIsHandBrakeOn = 1;
    }

    // 9.6f 0x421270
    inline char_type get_is_hand_brake_on()
    {
        return mIsHandBrakeOn;
    }

    EXPORT Fix16 ComputeZPosition();

    EXPORT Fix16_Point get_cp1();
    EXPORT ~CarPhysics();
    EXPORT void ShowPhysicsDebug();
    EXPORT void ShowSpeedRevsDamage();
    EXPORT bool IsNotMoving();
    EXPORT void UpdateTrailerPhysicsFromTowingCar();
    EXPORT void UpdateTrailerAlignment();
    EXPORT void EnforceTrailerControlLimits();
    EXPORT void SetOilSpinDir(const s32& spin_dir);
    EXPORT void SpinOutOnOil();
    EXPORT void ScarePedsOnDrivingFast();
    EXPORT void ApplyForcedSteering();
    EXPORT void ApplyConveyorPush(Object* pObj);
    EXPORT Fix16_Point ComputeCombinedCenterOfMass();
    EXPORT Fix16 CalculateMass();
    EXPORT Fix16 GetEffectiveMomentOfInertia();
    EXPORT bool IsInAir();
    EXPORT Fix16 GetTrailerAwareTurnRatio();
    // Inlined in UpdateSteeringAngle. The scale is computed before the turn ratio call, which
    // needs it as a by-value argument ahead of the turn direction (arguments are evaluated right to left)
    inline Fix16 GetScaledTurnRatio(Fix16 scale, s32 turn_direction)
    {
        return GetTrailerAwareTurnRatio() * turn_direction * scale;
    }
    EXPORT char_type IsFootBrakeOn();
    EXPORT bool IsAccelerationOrReverseOn();
    EXPORT void SetVelocityTowardTarget(Fix16 targetX, Fix16 targetY, Fix16 targetAngle, s32* rotationMode);
    EXPORT void restore_saved_physics_state();
    EXPORT void save_physics_state();
    EXPORT void restore_state();
    EXPORT void save_state();
    EXPORT Fix16 ComputeRequiredSweepSteps();
    EXPORT void ResetForceAccumulators();
    EXPORT void HandleUserInputs(char_type bForwardGasOn,
                                            char_type bFootBrakeOn,
                                            char_type bLeftOn,
                                            char_type bRightOn,
                                            char_type bHandBrakeOn);
    EXPORT void HandleGravityOnSlope();
    EXPORT Fix16 ComputeSlopeCorrection(Fix16* pOutY);
    EXPORT void UpdateZPhysics(Fix16 dt);
    EXPORT void SyncZWithTrailer(Fix16 dt);
    EXPORT void UpdateRailwayZPosition(Fix16 dt);
    EXPORT void UpdateRailwayZPositionThunk(Fix16 dt);
    EXPORT void EmitImpactParticles(u8 apply_to_corners_mask);
    EXPORT char_type ProcessGroundCollisionAndSurfaceType(char_type* check_mask);
    EXPORT void ProcessGroundCollisionAndEmitImpactParticles();
    EXPORT bool TestCollision();
    EXPORT bool SweepTestMovementForCollision(Fix16* outHitStep, Fix16* outNoHitStep);
    EXPORT void BinarySearchCollisionTime(Fix16& low, Fix16& high);
    EXPORT void HandleMapBoundaryCollisionY(Fix16_Point& point, Ang16 angle);
    EXPORT void HandleMapBoundaryCollisionX(Fix16_Point& point, Ang16 angle);
    EXPORT void DispatchCollision(Fix16_Point& point, Ang16 angle);
    EXPORT void ReplayAndDispatchCollision(Fix16 step_a, Fix16 step_b);
    EXPORT void SpawnSkidSegment(s32 box_idx, Fix16_Point wheel_offset, s32 surface);
    EXPORT void UpdateWheelSkidEffects();
    EXPORT void DoSkidmarks();
    EXPORT bool StepMovementAndCollisions();
    EXPORT bool CheckAndHandleCarAndTrailerCollisions();
    EXPORT void ApplyVerticalEdgeNudge();
    EXPORT void ApplyHorizontalEdgeNudge();
    EXPORT void ApplyTurningForce();
    EXPORT bool ApplyMovementCommand();
    EXPORT char_type ProcessCollisionAndClampVelocity();
    EXPORT void StepPhysics();
    EXPORT char_type CheckPendingCollision();
    EXPORT void ApplyForceWithTrailerRedirect(Fix16_Point* pPoint, Fix16_Point* pForce);
    EXPORT void ApplyForceAndIntegrate(Fix16_Point* pPoint, Fix16_Point force);
    EXPORT void ApplyForceAtPoint(Fix16_Point* pPoint, Fix16_Point* pForce, s32 bRotate);
    EXPORT void AccumulateImpulse(Fix16_Point* pPoint);
    EXPORT void ApplyAngularImpulse(Fix16 impulse);
    EXPORT void ApplyForceScaledByMass(Fix16_Point& force);
    EXPORT void ApplyImpulseWithTrailerRedirect(Fix16_Point* pPoint);
    EXPORT Fix16 ApplyImpactForcesAndDamage(Fix16_Point& PointOfForce, Fix16_Point& Impulse, s32 base_dmg);
    EXPORT void ApplyImpulseAndDamage(Fix16_Point& point, s32 base_dmg);
    EXPORT void HandleWorldCollision(Fix16_Point& relative_velocity);
    EXPORT void HandleCarCollision(Car* pOtherCar);
    EXPORT void HandleObjectCollision(Object* pObject, char_type damage_area);
    EXPORT void ProcessPedImpact(Char* pChar, u8 overlap_count);
    EXPORT void UpdateLinearAndAngularAccel();
    EXPORT void ApplyMovementStep(Fix16 time_scale);
    EXPORT void IntegrateAndClampVelocities();
    EXPORT Fix16_Point ComputeRelativePointVelocity(Fix16_Point* pPoint);
    EXPORT Fix16_Point GetPointVelocity(Fix16_Point* pPoint);
    EXPORT Fix16_Point ComputePointVelocity(Fix16_Point& point);
    EXPORT Fix16 ApplyDriveForce(Fix16_Point& wheel_point, Ang16 angle, Fix16_Point& friction, Fix16 slip_speed);
    EXPORT bool IsTurboActive();
    EXPORT Fix16 ComputeEngineTorque();
    EXPORT Fix16 ComputeTorqueFromThrottle();

    // FUNCTION: 96f 0x40f840
    bool IsVelocityAlignedWithHeading()
    {
        Ang16 heading_diff = (mLinvel1.GetAngle() - mTheta);
        return heading_diff <= kAng90_6FE00C || heading_diff >= kAng270_6FE154;
    }

    Fix16 inline_ComputeTorqueFromThrottle_561DD0()
    {
        if (IsTurboActive())
        {
            return gCarInfo_2C->mHalfThrust +
                ((mGasPedal * ((gDamageSpeedFactor * gCarInfo_2C->mFithThrust)))) * 2;
        }
        else
        {
            return gCarInfo_2C->mHalfThrust +
                ((mGasPedal * ((gDamageSpeedFactor * gCarInfo_2C->mFithThrust))));
        }
    }

    // ComputeTorqueFromThrottle as inlined into ComputeEngineTorque (out of line operators)
    Fix16 inline_ComputeTorqueFromThrottle_561DD0_ool()
    {
        if (IsTurboActive())
        {
            return gCarInfo_2C->mHalfThrust.Add(
                gCarInfo_2C->mFithThrust.Multiply(gDamageSpeedFactor)
                    .Multiply(mGasPedal)
                    .MultiplyInt(2));
        }
        else
        {
            return gCarInfo_2C->mHalfThrust.Add(
                gCarInfo_2C->mFithThrust.Multiply(gDamageSpeedFactor).Multiply(mGasPedal));
        }
    }

    // ComputeUndamagedThrottleTorque as inlined into ComputeEngineTorque (out of line operators)
    Fix16 ComputeUndamagedThrottleTorque_49E8E0_ool()
    {
        if (IsTurboActive())
        {
            return gCarInfo_2C->mHalfThrust.Add(
                gCarInfo_2C->mFithThrust.Multiply(mGasPedal).MultiplyInt(2));
        }
        else
        {
            return gCarInfo_2C->mHalfThrust.Add(
                gCarInfo_2C->mFithThrust.Multiply(mGasPedal));
        }
    }

    // 9.6f 0x49E8E0
    Fix16 CarPhysics::ComputeUndamagedThrottleTorque()
    {
        if (IsTurboActive())
        {
            return gCarInfo_2C->mHalfThrust + (gCarInfo_2C->mFithThrust * this->mGasPedal) * 2;
        }
        else
        {
            return gCarInfo_2C->mHalfThrust + gCarInfo_2C->mFithThrust * this->mGasPedal;
        }
    }

    // 9.6f 0x421100
    bool CarPhysics::IsDrowning()
    {
        if (mSurfaceType == car_surface_type::water_surface_8)
        {
            if (mLinvel1.GetLength_41E260() <= kDrowningMaxSpeed)
            {
                return true;
            }
        }
        return false;
    }

    EXPORT Fix16 CalculateFrontWheelForce();
    EXPORT Fix16 CalculateRearWheelForce();
    EXPORT void ApplyThrottleInput();
    EXPORT void ApplyBrakePhysics();
    EXPORT void UpdateSteeringAngle();
    EXPORT bool IsGasPedalPressedEnough();
    EXPORT Fix16 MinGasPedalPressure();
    EXPORT void ApplyArrowSteerAssist();
    EXPORT void StabilizeVelocityAtSpeed();
    EXPORT void EnforceGearSensitiveMaxSpeed();
    EXPORT void SetModelPhysicsGlobal();
    EXPORT void SetCarInfoGlobal();
    EXPORT void SetCurrentCarInfoAndModelPhysics();

    // 9.6f 0x482CC0
    inline void HandleObjectCollisionAt(Object* pObj, Fix16_Point& point, char_type damage_area)
    {
        SetCurrentCarInfoAndModelPhysics();
        CollisionIntersectionPoint = point;
        HandleObjectCollision(pObj, damage_area);
    }
    EXPORT void ApplyInputsAndIntegratePhysics();
    EXPORT bool UpdateLastMovementTimer();
    EXPORT bool ProcessCarPhysicsStateMachine();
    EXPORT void UpdateCp1FromCm1();
    EXPORT void UpdateCenterOfMassPoint();
    EXPORT void UpdateReferencePoint();
    EXPORT void SetSprite(Sprite* pSprite);
    EXPORT void SnapVelocityToSpriteDirection(Sprite* pSprt);
    EXPORT void UpdateSpriteFromPhysics();
    EXPORT void UpdateCarAndTrailerSpriteFromPhysics();
    EXPORT bool IsNearlyStopped();
    EXPORT void Init();
    EXPORT void PoolAllocate();

    EXPORT Fix16_Point get_linvel();

    void PoolDeallocate()
    {
    }

    EXPORT void SetCar(Car* pCar);
    EXPORT CarPhysics();

    // 9.6f 0x4211A0
    inline Fix16 GetLinearSpeed()
    {
        return mLinvel1.GetLength_41E260();
    }

    // 9.6f 0x49EF50, defined in CarPhysics.cpp
    inline void AddDamage(s32 damage);

    // 9.6f 0x421260
    inline void ClearHandBrake_421260()
    {
        mIsHandBrakeOn = 0;
    }

    // 9.6f 0x411810
    inline char_type is_backward_gas_on()
    {
        return mIsBackwardGasOn;
    }

    // 9.6f 0x4211C0
    inline Fix16 get_car_velocity()
    {
        return mVelReadOnly.GetLength_41E260();
    }

    // FUNCTION: 96f 0x4212a0
    inline void SetField8C_to_1()
    {
        mState = 1;
    }

    // FUNCTION: 96f 0x4212b0
    inline void SetField8C_to_2()
    {
        mState = 2;
    }

    // 9.6f 0x49EF80
    inline bool IsStationary()
    {
        return mLinvel1.x == kFP16Zero_6FE20C && mLinvel1.y == kFP16Zero_6FE20C &&
            mAngVelRad == kFP16Zero_6FE20C;
    }

    // FUNCTION: 96f 0x49ea60
    inline void Field74Subtract()
    {
        mAngVelRad -= gArrowSteerAngVel;
    }

    // FUNCTION: 96f 0x49f4e0
    inline void Field40Add()
    {
        mLinvel1 += gConveyorVelocity;
    }

    // FUNCTION: 96f 0x49f4f0
    inline void Field40Subtract()
    {
        mLinvel1 -= gConveyorVelocity;
    }

    // FUNCTION: 96f 0x49f500
    inline void ResetPoint6FDF50()
    {
        gConveyorVelocity.reset();
    }

    // FUNCTION: 96f 0x421210
    void Accelerate()
    {
        mIsForwardGasOn = 1;
        mIsFootBrakeOn = 0;
        mIsBackwardGasOn = 0;
        mIsCoasting = false;
    }

    // FUNCTION: 96f 0x42ABC0
    void ForceNeutralInput()
    {
        mIsCoasting = true;
        mIsFootBrakeOn = 0;
        mIsBackwardGasOn = 0;
        mIsForwardGasOn = 0;
    }

    // FUNCTION: 96f 0x42AC00
    void NeutralGear()
    {
        mIsFootBrakeOn = 0;
        mIsForwardGasOn = 0;
        mIsBackwardGasOn = 0;
        mIsCoasting = false;
    }

    // Unknown 9.6f inlined func
    void ForceReverseInput()
    {
        mIsBackwardGasOn = 1;
        mIsFootBrakeOn = 0;
        mIsForwardGasOn = 0;
        mIsCoasting = false;
    }

    // FUNCTION: 96f 0x42ABA0
    void TurnClockwise();

    // FUNCTION: 96f 0x42AB90
    void TurnAntiClockwise()
    {
        mTurnDirection = car_turn_direction::anticlockwise_1;
    }

    // 9.6f 0x42ABB0
    void SetGoStraight()
    {
        mTurnDirection = car_turn_direction::none_0;
        mPointingAngRad = kF16Zero;
    }

    EXPORT Fix16 vec_len(); // Char_B4.cpp func

    EXPORT void ClearDriverInputs();
    EXPORT void ForceNeutralInput_453F50();
    EXPORT void ForceForwardAcceleration();


    Fix16_Point mVelReadOnly; // 0x0
    u32 mDamagedUntilCycle; // 0x8: the tick (gpRng's cycle) plus the damage of the last impact: IsDamageWindowOpen
    CarPhysics* mpNext;
    Fix16_Point mLastSkidPos[4]; // 0x10
    Fix16_Point mCm1; // 0x30
    Fix16_Point mCp1; // 0x38
    Fix16_Point mLinvel1; // 0x40
    Fix16_Point mForceAccum; // 0x48
    Fix16_Point mLinearAccel; // 0x50
    Ang16 mTheta; // 0x58
    s16 mUnused5A; // 0x5A
    Car* mpCar; // 0x5C
    Fix16 mGasPedal; // 0x60
    Fix16 mBrakePressure; // 0x64
    Fix16 mAirZVel; // 0x68
    Fix16 mCp3; // 0x6C
    Fix16 mZVel; // 0x70
    Fix16 mAngVelRad; // 0x74
    Fix16 mPointingAngRad; // 0x78
    Fix16 mTorqueAccum; // 0x7C
    Fix16 mAngularAccel; // 0x80
    Fix16 mFrontSkid; // 0x84
    Fix16 mRearSkid; // 0x88
    s32 mState; // 0x8C
    u8 mTimerSinceLastMove; // 0x90
    char_type mIsFootBrakeOn; // 0x91
    char_type mIsHandBrakeOn; // 0x92
    char_type mIsForwardGasOn; // 0x93
    char_type mIsBackwardGasOn; // 0x94
    bool mIsCoasting; // 0x95
    char_type mUnused96; // 0x96
    char_type mUnused97; // 0x97
    s32 mSurfaceType; // 0x98
    s32 mBlockSpec; // 0x9C
    s32 mOilSpinDir; // 0xA0
    char_type mOilSpinTimer; // 0xA4
    u8 mCurrentSlopeLength; // 0xA5
    char_type mCurrentSlopeLeftTiles; // 0xA6
    u8 mCurrentTileZ; // 0xA7
    char_type mHandBrakeForce; // 0xA8
    char_type mCarModel; // 0xA9
    char_type mSbw; // 0xAA
    char_type mTpa; // 0xAB
    u8 mDriveWheelsLockedQ; // 0xAC
    char_type mTurnDirection; // 0xAD
    char_type mUnusedAE; // 0xAE
    char_type mUnusedAF; // 0xAF
};
GTA2_ASSERT_SIZEOF_ALWAYS(CarPhysics, 0xB0);

#pragma warning(push)
#pragma warning(disable : 4101)
// 9.6f 0x42ABA0
inline void CarPhysics::TurnClockwise()
{
    // Never read: its optimizer entry is needed by CarAI::UpdateStateMachine (docs/match_attempts.md)
    s32 unused;
    mTurnDirection = car_turn_direction::clockwise_m1;
}
#pragma warning(pop)

class CarPhyisicsPool : public PoolBasic<CarPhysics, 306>
{
  public:
    //Inlined in CarManager constructor 9.6f -> 0x420f8
    CarPhyisicsPool()
    {

    }

    ~CarPhyisicsPool()
    {
        mpHead = NULL;
    }
};

EXTERN_GLOBAL(CarPhyisicsPool*, gCarPhysicsPool);
