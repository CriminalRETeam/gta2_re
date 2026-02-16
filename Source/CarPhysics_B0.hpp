#pragma once

#include "Car_BC.hpp"
#include "Function.hpp"
#include "ang16.hpp"
#include "fix16.hpp"

class Sprite;
class Car_BC;
class Trailer;
class Sprite_4C;
class Ped;
class ModelPhysics_48;
class CarInfo_2C;
class Car_78;
struct Fix16_Point_POD;

EXTERN_GLOBAL(Fix16, kFP16Zero_6FE20C);
EXTERN_GLOBAL(ModelPhysics_48*, dword_6FE258);

class CarPhysics_B0
{
  public:
    // TODO: Ordering
    EXPORT Fix16 ComputeZPosition_559E90();

    EXPORT Fix16_Point get_cp1_40B560();
    EXPORT ~CarPhysics_B0();
    EXPORT void ShowPhysicsDebug_559430();
    EXPORT void sub_5597B0();
    EXPORT bool IsNotMoving_5599D0();
    EXPORT void sub_559A40();
    EXPORT void sub_559B40();
    EXPORT void sub_559B50();
    EXPORT void set_field_A0_559B90(const s32& a2);
    EXPORT void SpinOutOnOil_559BA0();
    EXPORT void ScarePedsOnDrivingFast_559C30();
    EXPORT void sub_559DD0();
    EXPORT void sub_559E20(Object_2C* a2);
    EXPORT Fix16_Point ComputeCombinedCenterOfMass_559EC0();
    EXPORT Fix16 CalculateMass_559FF0();
    EXPORT Fix16 sub_55A050();
    EXPORT u8 IsInAir_55A0B0();
    EXPORT Fix16 GetTrailerAwareTurnRatio_55A100();
    EXPORT char_type IsFootBrakeOn_55A150();
    EXPORT char_type IsAccelerationOrReverseOn_55A180();
    EXPORT s32 sub_55A1D0(s32 a2, s32 a3, s32 a4, u32* a5);
    EXPORT void restore_saved_physics_state_55A400();
    EXPORT void save_physics_state_55A4B0();
    EXPORT void sub_55A550();
    EXPORT void sub_55A600();
    EXPORT u32* sub_55A6A0(u32* a2);
    EXPORT void ResetForceAccumulators_55A840();
    EXPORT void
    HandleUserInputs_55A860(char_type bForwardGasOn, char_type bFootBrakeOn, char_type a4, char_type a5, char_type bHandBrakeOn);
    EXPORT void HandleGravityOnSlope_55AA00();
    EXPORT s32* sub_55AB50(s32* a2, Sprite_4C** a3);
    EXPORT s32 sub_55AD90(Fix16 a2);
    EXPORT void sub_55B3F0(Fix16 a2);
    EXPORT s32 sub_55B4F0(Fix16 a2);
    EXPORT void sub_55B7B0(Fix16 a2);
    EXPORT void EmitImpactParticles_55B7E0(u8 apply_to_corners_mask);
    EXPORT char_type ProcessGroundCollisionAndSurfaceType_55B970(char_type* a2);
    EXPORT void ProcessGroundCollisionAndEmitImpactParticles_55BFE0();
    EXPORT char_type sub_55C150();
    EXPORT char_type sub_55C3B0(Sprite_4C** a2, Sprite_4C** a3);
    EXPORT void sub_55C560(Fix16 a2, Fix16 a3);
    EXPORT s32 sub_55C5C0(u32* a2, s32 a3);
    EXPORT s32 sub_55C820(u32* a2, s32 a3);
    EXPORT void sub_55CA70(Fix16_Point a2, Ang16 a3);
    EXPORT void sub_55CBB0(Fix16 a2, Fix16 a3);
    EXPORT void sub_55D200(s32 a2, Sprite_4C* a3, s32 a4, s32 a5);
    EXPORT char_type UpdateWheelSkidEffects_55DC00();
    EXPORT void DoSkidmarks_55E260();
    EXPORT char_type StepMovementAndCollisions_55E470();
    EXPORT char_type CheckAndHandleCarAndTrailerCollisions_55EB80();
    EXPORT s32 ApplyForwardEngineForce_55EC30();
    EXPORT s32 ApplyReverseEngineForce_55EF20();
    EXPORT s32 ApplyTurningForce_55F020();
    EXPORT char_type ApplyMovementCommand_55F240();
    EXPORT char_type ProcessCollisionAndClampVelocity_55F280();
    EXPORT void sub_55F330();
    EXPORT char_type sub_55F360();
    EXPORT void sub_55F740(Fix16_Point* a2, Fix16_Point* a3);
    EXPORT void sub_55F7A0(Fix16_Point* a2, Fix16_Point a3);
    EXPORT s32 sub_55F800(Fix16_Point* a2, Fix16_Point* a3, s32 a4);
    EXPORT void sub_55F930(Fix16_Point* a2);
    EXPORT void sub_55F970(Fix16 a2);
    EXPORT void ApplyForceScaledByMass_55F9A0(Fix16_Point_POD& pForce);
    EXPORT void sub_55FA10(Fix16_Point* a2);
    EXPORT u32* sub_55FA60(u32* a2, s32* a3, s32* a4, s32 a5);
    EXPORT Ped* sub_55FC30(s32* a2, s32 a3);
    EXPORT s32 sub_55FD00(s32 a2);
    EXPORT Car_78* sub_55FF20(Car_BC* a2);
    EXPORT void sub_5606C0(s32 a2, char_type a3);
    EXPORT void sub_560B40(s32 a2, s32 a3);
    EXPORT void UpdateLinearAndAngularAccel_560EB0();
    EXPORT void sub_560F20(Fix16 a2);
    EXPORT void IntegrateAndClampVelocities_5610B0();
    EXPORT Fix16_Point sub_561130(Fix16_Point* a3);
    EXPORT Fix16_Point sub_561350(Fix16_Point* a3);
    EXPORT u32* sub_561380(u32* a2, u32* a3);
    EXPORT s32* sub_5615D0(s32* a2, s32* a3, s32 a4, u32* a5, s32 a6);
    EXPORT s32 get_revs_561940();
    EXPORT u32* sub_561970(u32* a2);
    EXPORT u32* sub_561DD0(u32* a2);
    EXPORT Fix16 CalculateFrontSkid_561E50();
    EXPORT Fix16 CalculateRearSkid_5620D0();
    // 0x62450 moved to Sprite
    EXPORT void ApplyThrottleInput_562480();
    EXPORT void ApplyBrakePhysics_5624F0();
    EXPORT void UpdateSteeringAngle_562560();
    EXPORT s32 IsGasPedalPressedEnough_5626A0();
    EXPORT Fix16 MinGasPedalPressure_5626C0();
    EXPORT char_type ApplyArrowSteerAssist_5626F0();
    EXPORT void StabilizeVelocityAtSpeed_562910();
    EXPORT void RotateVelocity_562C20(const Ang16& a2);
    EXPORT void EnforceGearSensitiveMaxSpeed_562D00();
    EXPORT void sub_562EB0();
    EXPORT void sub_562ED0();
    EXPORT void SetCurrentCarInfoAndModelPhysics_562EF0();
    EXPORT void ApplyInputsAndIntegratePhysics_562F30();
    EXPORT char_type UpdateLastMovementTimer_562FA0();
    EXPORT bool ProcessCarPhysicsStateMachine_562FE0();
    EXPORT void sub_563280();
    EXPORT void UpdateCenterOfMassPoint_563350();
    EXPORT void UpdateReferencePoint_563460();
    EXPORT void SetSprite_563560(Sprite* a2);
    EXPORT void SnapVelocityToSpriteDirection_563590(Sprite* a2);
    EXPORT void sub_563670();
    EXPORT void sub_5636C0();
    EXPORT bool IsNearlyStopped_5636E0();
    EXPORT void Init_5637A0();
    EXPORT void PoolAllocate();

    EXPORT Fix16_Point get_linvel_447010();

    void PoolDeallocate()
    {
    }

    EXPORT void SetCar_5638C0(Car_BC* pBC);
    EXPORT CarPhysics_B0();

    inline Fix16 sub_4211A0()
    {
        return field_40_linvel_1.GetLength_41E260();
    }

    inline char_type is_backward_gas_on_411810()
    {
        return field_94_is_backward_gas_on;
    }

    inline Fix16 get_car_velocity_4211C0()
    {
        return field_0_vel_read_only.GetLength_41E260();
    }

    inline bool sub_49EF80()
    {
        return field_40_linvel_1.x == kFP16Zero_6FE20C && field_40_linvel_1.y == kFP16Zero_6FE20C &&
            field_74_ang_vel_rad == kFP16Zero_6FE20C;
    }

    Fix16_Point field_0_vel_read_only;
    s32 field_8_total_damage_q;
    CarPhysics_B0* mpNext;
    Fix16_Point field_10[4];
    Fix16_Point field_30_cm1;
    Fix16_Point field_38_cp1;
    Fix16_Point field_40_linvel_1;
    Fix16_Point_POD field_48;
    Fix16_Point_POD field_50;
    Ang16 field_58_theta;
    s16 field_5A;
    Car_BC* field_5C_pCar;
    Fix16 field_60_gas_pedal;
    Fix16 field_64;
    Fix16 field_68_z_pos;
    Fix16 field_6C_cp3;
    Fix16 field_70;
    Fix16 field_74_ang_vel_rad;
    Fix16 field_78_pointing_ang_rad;
    Fix16 field_7C;
    Fix16 field_80;
    Fix16 field_84_front_skid;
    Fix16 field_88_rear_skid;
    s32 field_8C_state;
    u8 field_90_timer_since_last_move;
    char_type field_91_is_foot_brake_on;
    char_type field_92_is_hand_brake_on;
    char_type field_93_is_forward_gas_on;
    char_type field_94_is_backward_gas_on;
    char_type field_95;
    char_type field_96;
    char_type field_97;
    s32 field_98_surface_type;
    s32 field_9C;
    s32 field_A0;
    char_type field_A4;
    char_type field_A5_current_slope_length;
    char_type field_A6_current_slope_left_tiles;
    char_type field_A7_current_tile_z;
    char_type field_A8_hand_brake_force;
    char_type field_A9_car_model;
    char_type field_AA_sbw;
    char_type field_AB_tpa;
    char_type field_AC_drive_wheels_locked_q;
    char_type field_AD_turn_direction;
    char_type field_AE;
    char_type field_AF;
};

class CarPhyisicsPool : public PoolBasic<CarPhysics_B0, 306>
{
  public:
    //Inlined in Car_6C constructor 9.6f -> 0x420f80
};

EXTERN_GLOBAL(CarPhyisicsPool*, gCarPhysicsPool_6FE3E0);
