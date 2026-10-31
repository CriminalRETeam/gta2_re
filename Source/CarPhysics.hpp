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
EXTERN_GLOBAL(ModelPhysics*, gCarInfo_48_6FE258);
EXTERN_GLOBAL(Ang16, kAng90_6FE00C);
EXTERN_GLOBAL(Ang16, kAng270_6FE154);
EXTERN_GLOBAL(CarInfo_2C*, gCarInfo_2C_6FE0E4);
EXTERN_GLOBAL(Fix16, gDamageSpeedFactor_6FE348);
EXTERN_GLOBAL(Fix16, kDrowningMaxSpeed_677794);
EXTERN_GLOBAL(Fix16_Point, gConveyorVelocity_6FDF50);
EXTERN_GLOBAL(Fix16, gArrowSteerAngVel_6FE0B0);
EXTERN_GLOBAL(Fix16, kF16Zero_677B90);
EXTERN_GLOBAL(Fix16_Point, CollisionIntersectionPoint_6FE1A0);

EXPORT Fix16_Point __stdcall ComputeCollisionImpulse_55F3B0(Fix16 OwnerMass,
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
    inline Fix16 get_cp3_40F800()
    {
        return mCp3;
    }

    // 9.6f 0x40F810
    inline void set_cp3_40F810(Fix16 v)
    {
        mCp3 = v;
    }

    // 9.6f 0x40F820
    inline Ang16 get_theta_40F820()
    {
        return mTheta;
    }

    // 9.6f 0x40F830
    inline void set_theta_40F830(Ang16 v)
    {
        mTheta = v;
    }

    // 9.6f 0x40F7E0
    inline void set_cp1_40F7E0(const Fix16_Point& v)
    {
        mCp1.x = v.x;
        mCp1.y = v.y;
    }

    // 9.6f 0x421170
    inline void set_linvel_421170(const Fix16_Point& v)
    {
        mLinvel1.x = v.x;
        mLinvel1.y = v.y;
    }

    // 9.6f 0x4895D0
    inline void StopMoving_4895D0()
    {
        mAngVelRad = kFpZero_6FD120;
        mLinvel1.clear_41E1E0();
    }

    // 9.6f 0x4211E0
    inline Ang16 GetLinvelAngle_4211E0()
    {
        return mLinvel1.atan2_40F790();
    }

    // 9.6f 0x421130
    inline bool IsDamageWindowOpen_421130()
    {
        return static_cast<u32>(gpRng_67AB34->get_cur_rng_41CFE0()) < mDamagedUntilCycle;
    }

    // 9.6f 0x421260
    inline void SetHandBrakeOff_421260()
    {
        mIsHandBrakeOn = 0;
    }

    // 9.6f 0x421250
    inline void SetHandBrakeOn_421250()
    {
        mIsHandBrakeOn = 1;
    }

    // 9.6f 0x421270
    inline char_type get_is_hand_brake_on_421270()
    {
        return mIsHandBrakeOn;
    }

    EXPORT Fix16 ComputeZPosition_559E90();

    EXPORT Fix16_Point get_cp1_40B560();
    EXPORT ~CarPhysics();
    EXPORT void ShowPhysicsDebug_559430();
    EXPORT void ShowSpeedRevsDamage_5597B0();
    EXPORT bool IsNotMoving_5599D0();
    EXPORT void UpdateTrailerPhysicsFromTowingCar_559A40();
    EXPORT void UpdateTrailerAlignment_559B40();
    EXPORT void EnforceTrailerControlLimits_559B50();
    EXPORT void SetOilSpinDir_559B90(const s32& spin_dir);
    EXPORT void SpinOutOnOil_559BA0();
    EXPORT void ScarePedsOnDrivingFast_559C30();
    EXPORT void ApplyForcedSteering_559DD0();
    EXPORT void ApplyConveyorPush_559E20(Object* pObj);
    EXPORT Fix16_Point ComputeCombinedCenterOfMass_559EC0();
    EXPORT Fix16 CalculateMass_559FF0();
    EXPORT Fix16 GetEffectiveMomentOfInertia_55A050();
    EXPORT bool IsInAir_55A0B0();
    EXPORT Fix16 GetTrailerAwareTurnRatio_55A100();
    // Inlined in UpdateSteeringAngle_562560. The scale is computed before the turn ratio call, which
    // needs it as a by-value argument ahead of the turn direction (arguments are evaluated right to left)
    inline Fix16 GetScaledTurnRatio(Fix16 scale, s32 turn_direction)
    {
        return GetTrailerAwareTurnRatio_55A100() * turn_direction * scale;
    }
    EXPORT char_type IsFootBrakeOn_55A150();
    EXPORT bool IsAccelerationOrReverseOn_55A180();
    EXPORT void SetVelocityTowardTarget_55A1D0(Fix16 targetX, Fix16 targetY, Fix16 targetAngle, s32* rotationMode);
    EXPORT void restore_saved_physics_state_55A400();
    EXPORT void save_physics_state_55A4B0();
    EXPORT void restore_state_55A550();
    EXPORT void save_state_55A600();
    EXPORT Fix16 ComputeRequiredSweepSteps_55A6A0();
    EXPORT void ResetForceAccumulators_55A840();
    EXPORT void HandleUserInputs_55A860(char_type bForwardGasOn,
                                            char_type bFootBrakeOn,
                                            char_type bLeftOn,
                                            char_type bRightOn,
                                            char_type bHandBrakeOn);
    EXPORT void HandleGravityOnSlope_55AA00();
    EXPORT Fix16 ComputeSlopeCorrection_55AB50(Fix16* pOutY);
    EXPORT void UpdateZPhysics_55AD90(Fix16 dt);
    EXPORT void SyncZWithTrailer_55B3F0(Fix16 dt);
    EXPORT void UpdateZPosition_55B4F0(Fix16 dt);
    EXPORT void UpdateZPosition_55B7B0(Fix16 dt);
    EXPORT void EmitImpactParticles_55B7E0(u8 apply_to_corners_mask);
    EXPORT char_type ProcessGroundCollisionAndSurfaceType_55B970(char_type* check_mask);
    EXPORT void ProcessGroundCollisionAndEmitImpactParticles_55BFE0();
    EXPORT bool TestCollision_55C150();
    EXPORT bool SweepTestMovementForCollision_55C3B0(Fix16* outHitStep, Fix16* outNoHitStep);
    EXPORT void BinarySearchCollisionTime_55C560(Fix16& low, Fix16& high);
    EXPORT void HandleMapBoundaryCollisionY_55C5C0(Fix16_Point& point, Ang16 angle);
    EXPORT void HandleMapBoundaryCollisionX_55C820(Fix16_Point& point, Ang16 angle);
    EXPORT void DispatchCollision_55CA70(Fix16_Point& point, Ang16 angle);
    EXPORT void ReplayAndDispatchCollision_55CBB0(Fix16 step_a, Fix16 step_b);
    EXPORT void SpawnSkidSegment_55D200(s32 box_idx, Fix16_Point wheel_offset, s32 surface);
    EXPORT void UpdateWheelSkidEffects_55DC00();
    EXPORT void DoSkidmarks_55E260();
    EXPORT bool StepMovementAndCollisions_55E470();
    EXPORT bool CheckAndHandleCarAndTrailerCollisions_55EB80();
    EXPORT void ApplyVerticalEdgeNudge_55EC30();
    EXPORT void ApplyHorizontalEdgeNudge_55EF20();
    EXPORT void ApplyTurningForce_55F020();
    EXPORT bool ApplyMovementCommand_55F240();
    EXPORT char_type ProcessCollisionAndClampVelocity_55F280();
    EXPORT void StepPhysics_55F330();
    EXPORT char_type CheckPendingCollision_55F360();
    EXPORT void ApplyForceWithTrailerRedirect_55F740(Fix16_Point* pPoint, Fix16_Point* pForce);
    EXPORT void ApplyForceAndIntegrate_55F7A0(Fix16_Point* pPoint, Fix16_Point force);
    EXPORT void ApplyForceAtPoint_55F800(Fix16_Point* pPoint, Fix16_Point* pForce, s32 bRotate);
    EXPORT void AccumulateImpulse_55F930(Fix16_Point* pPoint);
    EXPORT void ApplyAngularImpulse_55F970(Fix16 impulse);
    EXPORT void ApplyForceScaledByMass_55F9A0(Fix16_Point& force);
    EXPORT void ApplyImpulseWithTrailerRedirect_55FA10(Fix16_Point* pPoint);
    EXPORT Fix16 ApplyImpactForcesAndDamage_55FA60(Fix16_Point& PointOfForce, Fix16_Point& Impulse, s32 base_dmg);
    EXPORT void AccumulateImpulse_55FC30(Fix16_Point& point, s32 base_dmg);
    EXPORT void HandleWorldCollision_55FD00(Fix16_Point& relative_velocity);
    EXPORT void HandleCarCollision_55FF20(Car* pOtherCar);
    EXPORT void HandleObjectCollision_5606C0(Object* pObject, char_type damage_area);
    EXPORT void ProcessPedImpact_560B40(Char* pChar, u8 overlap_count);
    EXPORT void UpdateLinearAndAngularAccel_560EB0();
    EXPORT void ApplyMovementStep_560F20(Fix16 time_scale);
    EXPORT void IntegrateAndClampVelocities_5610B0();
    EXPORT Fix16_Point ComputeRelativePointVelocity_561130(Fix16_Point* pPoint);
    EXPORT Fix16_Point GetPointVelocity_561350(Fix16_Point* pPoint);
    EXPORT Fix16_Point ComputePointVelocity_561380(Fix16_Point& point);
    EXPORT Fix16 ApplyDriveForce_5615D0(Fix16_Point& wheel_point, Ang16 angle, Fix16_Point& friction, Fix16 slip_speed);
    EXPORT bool IsTurboActive_561940();
    EXPORT Fix16 ComputeEngineTorque_561970();
    EXPORT Fix16 ComputeTorqueFromThrottle_561DD0();

    // FUNCTION: 96f 0x40f840
    bool IsVelocityAlignedWithHeading_40F840()
    {
        Ang16 heading_diff = (mLinvel1.atan2_40F790() - mTheta);
        return heading_diff <= kAng90_6FE00C || heading_diff >= kAng270_6FE154;
    }

    Fix16 inline_ComputeTorqueFromThrottle_561DD0()
    {
        if (IsTurboActive_561940())
        {
            return gCarInfo_2C_6FE0E4->mHalfThrust +
                ((mGasPedal * ((gDamageSpeedFactor_6FE348 * gCarInfo_2C_6FE0E4->mFithThrust)))) * 2;
        }
        else
        {
            return gCarInfo_2C_6FE0E4->mHalfThrust +
                ((mGasPedal * ((gDamageSpeedFactor_6FE348 * gCarInfo_2C_6FE0E4->mFithThrust))));
        }
    }

    // ComputeTorqueFromThrottle_561DD0 as inlined into ComputeEngineTorque_561970 (out of line operators)
    Fix16 inline_ComputeTorqueFromThrottle_561DD0_ool()
    {
        if (IsTurboActive_561940())
        {
            return gCarInfo_2C_6FE0E4->mHalfThrust.Add_408660(
                gCarInfo_2C_6FE0E4->mFithThrust.Multiply_408680(gDamageSpeedFactor_6FE348)
                    .Multiply_408680(mGasPedal)
                    .MultiplyInt_561DB0(2));
        }
        else
        {
            return gCarInfo_2C_6FE0E4->mHalfThrust.Add_408660(
                gCarInfo_2C_6FE0E4->mFithThrust.Multiply_408680(gDamageSpeedFactor_6FE348).Multiply_408680(mGasPedal));
        }
    }

    // ComputeUndamagedThrottleTorque_49E8E0 as inlined into ComputeEngineTorque_561970 (out of line operators)
    Fix16 ComputeUndamagedThrottleTorque_49E8E0_ool()
    {
        if (IsTurboActive_561940())
        {
            return gCarInfo_2C_6FE0E4->mHalfThrust.Add_408660(
                gCarInfo_2C_6FE0E4->mFithThrust.Multiply_408680(mGasPedal).MultiplyInt_561DB0(2));
        }
        else
        {
            return gCarInfo_2C_6FE0E4->mHalfThrust.Add_408660(
                gCarInfo_2C_6FE0E4->mFithThrust.Multiply_408680(mGasPedal));
        }
    }

    Fix16 CarPhysics::ComputeUndamagedThrottleTorque_49E8E0()
    {
        if (IsTurboActive_561940())
        {
            return gCarInfo_2C_6FE0E4->mHalfThrust + (gCarInfo_2C_6FE0E4->mFithThrust * this->mGasPedal) * 2;
        }
        else
        {
            return gCarInfo_2C_6FE0E4->mHalfThrust + gCarInfo_2C_6FE0E4->mFithThrust * this->mGasPedal;
        }
    }

    bool CarPhysics::IsDrowning_421100()
    {
        if (mSurfaceType == car_surface_type::water_surface_8)
        {
            if (mLinvel1.GetLength_41E260() <= kDrowningMaxSpeed_677794)
            {
                return true;
            }
        }
        return false;
    }

    EXPORT Fix16 CalculateFrontWheelForce_561E50();
    EXPORT Fix16 CalculateRearWheelForce_5620D0();
    EXPORT void ApplyThrottleInput_562480();
    EXPORT void ApplyBrakePhysics_5624F0();
    EXPORT void UpdateSteeringAngle_562560();
    EXPORT bool IsGasPedalPressedEnough_5626A0();
    EXPORT Fix16 MinGasPedalPressure_5626C0();
    EXPORT void ApplyArrowSteerAssist_5626F0();
    EXPORT void StabilizeVelocityAtSpeed_562910();
    EXPORT void EnforceGearSensitiveMaxSpeed_562D00();
    EXPORT void SetModelPhysicsGlobal_562EB0();
    EXPORT void SetCarInfoGlobal_562ED0();
    EXPORT void SetCurrentCarInfoAndModelPhysics_562EF0();

    // 9.6f 0x482CC0
    inline void HandleObjectCollisionAt_482CC0(Object* pObj, Fix16_Point& point, char_type damage_area)
    {
        SetCurrentCarInfoAndModelPhysics_562EF0();
        CollisionIntersectionPoint_6FE1A0 = point;
        HandleObjectCollision_5606C0(pObj, damage_area);
    }
    EXPORT void ApplyInputsAndIntegratePhysics_562F30();
    EXPORT bool UpdateLastMovementTimer_562FA0();
    EXPORT bool ProcessCarPhysicsStateMachine_562FE0();
    EXPORT void UpdateCp1FromCm1_563280();
    EXPORT void UpdateCenterOfMassPoint_563350();
    EXPORT void UpdateReferencePoint_563460();
    EXPORT void SetSprite_563560(Sprite* pSprite);
    EXPORT void SnapVelocityToSpriteDirection_563590(Sprite* pSprt);
    EXPORT void UpdateSpriteFromPhysics_563670();
    EXPORT void UpdateCarAndTrailerSpriteFromPhysics_5636C0();
    EXPORT bool IsNearlyStopped_5636E0();
    EXPORT void Init_5637A0();
    EXPORT void PoolAllocate();

    EXPORT Fix16_Point get_linvel_447010();

    void PoolDeallocate()
    {
    }

    EXPORT void SetCar_5638C0(Car* pCar);
    EXPORT CarPhysics();

    inline Fix16 GetLinearSpeed_4211A0()
    {
        return mLinvel1.GetLength_41E260();
    }

    // 9.6f 0x49EF50, defined in CarPhysics.cpp
    inline void AddDamage_49EF50(s32 damage);

    // 9.6f 0x421260
    inline void ClearHandBrake_421260()
    {
        mIsHandBrakeOn = 0;
    }

    inline char_type is_backward_gas_on_411810()
    {
        return mIsBackwardGasOn;
    }

    inline Fix16 get_car_velocity_4211C0()
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

    inline bool IsStationary_49EF80()
    {
        return mLinvel1.x == kFP16Zero_6FE20C && mLinvel1.y == kFP16Zero_6FE20C &&
            mAngVelRad == kFP16Zero_6FE20C;
    }

    // FUNCTION: 96f 0x49ea60
    inline void Field74Subtract()
    {
        mAngVelRad -= gArrowSteerAngVel_6FE0B0;
    }

    // FUNCTION: 96f 0x49f4e0
    inline void Field40Add()
    {
        mLinvel1 += gConveyorVelocity_6FDF50;
    }

    // FUNCTION: 96f 0x49f4f0
    inline void Field40Subtract()
    {
        mLinvel1 -= gConveyorVelocity_6FDF50;
    }

    // FUNCTION: 96f 0x49f500
    inline void ResetPoint6FDF50()
    {
        gConveyorVelocity_6FDF50.reset();
    }

    // FUNCTION: 96f 0x421210
    void Accelerate_421210()
    {
        mIsForwardGasOn = 1;
        mIsFootBrakeOn = 0;
        mIsBackwardGasOn = 0;
        mIsCoasting = false;
    }

    // FUNCTION: 96f 0x42ABC0
    void ForceNeutralInput_42ABC0()
    {
        mIsCoasting = true;
        mIsFootBrakeOn = 0;
        mIsBackwardGasOn = 0;
        mIsForwardGasOn = 0;
    }

    // FUNCTION: 96f 0x42AC00
    void NeutralGear_42AC00()
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
    void TurnClockwise_42ABA0();

    // FUNCTION: 96f 0x42AB90
    void TurnAntiClockwise_42AB90()
    {
        mTurnDirection = car_turn_direction::anticlockwise_1;
    }

    void SetGoStraight_42ABB0()
    {
        mTurnDirection = car_turn_direction::none_0;
        mPointingAngRad = kF16Zero_677B90;
    }

    EXPORT Fix16 vec_len_552DE0(); // Char_B4.cpp func

    EXPORT void ClearDriverInputs_453F90();
    EXPORT void ForceNeutralInput_453F50();
    EXPORT void ForceForwardAcceleration_453F70();


    Fix16_Point mVelReadOnly; // 0x0
    u32 mDamagedUntilCycle; // 0x8: the tick (gpRng_67AB34's cycle) plus the damage of the last impact: IsDamageWindowOpen_421130
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
inline void CarPhysics::TurnClockwise_42ABA0()
{
    // Never read: its optimizer entry is needed by CarAI::UpdateStateMachine_44E560 (docs/match_attempts.md)
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

EXTERN_GLOBAL(CarPhyisicsPool*, gCarPhysicsPool_6FE3E0);
