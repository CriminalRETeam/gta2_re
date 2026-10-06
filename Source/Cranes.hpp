#pragma once

#include "Car_BC.hpp"
#include "Function.hpp"
#include "Object_5C.hpp"
#include "ang16.hpp"
#include "fix16.hpp"

class infallible_turing;
class Car_BC;

class Crane_15C
{
  public:
    // 9.6f 0x447F60
    inline void set_field_148_447F60(s32 v)
    {
        field_148_disabled = v;
    }

    // 9.6f 0x476900
    inline void set_maybe_homecrane_476900(Crane_15C* v)
    {
        field_78_maybe_homecrane = v;
    }

    EXPORT ~Crane_15C();
    EXPORT Crane_15C();
    EXPORT void ComputeHookPos_47E620(Fix16 radius, Ang16 ang, Fix16_Point* pOutPoint);
    EXPORT void ComputeHookPos_47E730(Ang16 radius, Fix16 ang, Fix16_Point* pOutPoint);
    EXPORT void ComputeHookOffset_47E840(Ang16 ang, Fix16_Point* pOutPoint);
    EXPORT bool IsDropPositionClear_47E920();
    EXPORT bool IsTarget1PositionClear_47EB00();
    EXPORT void DropHookedCar_47ECC0();
    EXPORT void DropHookedCarOnTransporter_47ED60();
    EXPORT void HookTransporterCargo_47EDF0();
    EXPORT void HookPickupCar_47EF80();
    EXPORT void sub_47F170();
    EXPORT void SetTransporterCargoTarget_47F220(Fix16 a2, Fix16 a3, Sprite* a4, Sprite* a5);
    EXPORT void SetDropTransporterTarget_47F290(Fix16 a2, Fix16 a3, Sprite* a4);
    EXPORT void SetPickupCarTarget_47F2F0(Fix16 a2, Fix16 a3, Sprite* a4);
    EXPORT bool IsTransporterCargoTargetValid_47F350();
    EXPORT bool IsPickupCarTargetValid_47F3D0();
    EXPORT bool IsDropTransporterTargetValid_47F450();
    EXPORT void UpdateCraneTargets_47F4C0();
    EXPORT bool ComputeHookPolar_47F6C0(Fix16_Point& pPoint, Fix16* pOutF16, Fix16* pOutAng);
    EXPORT void TargetTransporter_47F7F0(Car_BC* a2);
    EXPORT void PickUpCar_47F930(Car_BC* a2);
    EXPORT void TryNextDropPosition_47FB40();
    EXPORT s32 MoveTowardsTargets_47FBA0();
    EXPORT void ReleaseHookedCar_47FD10();
    EXPORT void UpdateCraneTick_47FD50();
    EXPORT void UpdateCraneSprites_47FE10();
    EXPORT void Service_480310();
    EXPORT void InitCrane_4803B0(Fix16 a2, Fix16 a3, char_type a4);
    EXPORT void CraneTargetPickupCheck_480900(Fix16 a2, Fix16 a3, Ang16 a4);
    EXPORT void ComputePickupAlignment_480B60(Fix16 a2, Fix16 a3, Ang16 a4);
    EXPORT Car_BC* GetCarFromCrane_480DA0();

    inline void SetHomeRotation_4768E0(Ang16 rotation)
    {
        field_A8_home_angle = Ang16::Ang16_to_Fix16(rotation);
    }

    // 9.6f 0x411A00
    inline Sprite* GetRotorSprite_411A00()
    {
        return field_2C_rotor_obj->field_4;
    }

    // FUNCTION: 96f 0x447f40
    inline s32 check_8c_a8_447f40()
    {
        return field_8C_crane_angle == field_A8_home_angle;
    }

    Fix16_Point field_0_hooked_sprite_offset_target;
    Fix16_Point field_8_drop_offset;
    Fix16_Point field_10_hooked_sprite_offset;
    Fix16_Point field_18_target1_offset;
    Fix16_Point field_20_target2_offset;
    struct_4 field_28_strct4;
    Object_2C* field_2C_rotor_obj;
    Object_2C* field_30;
    Object_2C* field_34;
    Object_2C* field_38;
    Object_2C* field_3C;
    Object_2C* field_40;
    Object_2C* field_44;
    Object_2C* field_48;
    Object_2C* field_4C;
    Object_2C* field_50;
    Object_2C* field_54_hook_obj;
    Object_2C* field_58_crane_base_obj;
    Object_2C* field_5C_counterweight_obj;
    Sprite* field_60_probe_sprite;
    Sprite* field_64_drop_transporter;
    Sprite* field_68_pickup_car;
    Sprite* field_6C_transporter_cargo;
    Sprite* field_70_cargo_transporter;
    Sprite* field_74_pSprite_on_hook;
    Crane_15C* field_78_maybe_homecrane;
    infallible_turing* field_7C_sound;
    Fix16 field_80_ground_z;
    Fix16 field_84_hook_depth;
    Fix16 field_88_hook_depth_speed;
    Fix16 field_8C_crane_angle; //  It's not Ang16, maybe Ang32
    Fix16 field_90_hook_radius; //  Radial distance
    Fix16 field_94_hook_radius_speed;
    Fix16 field_98_crane_angle_speed;
    Fix16 field_9C;
    Fix16 field_A0_hook_axial_angle;  // The angle in its own rotation axis, e.g. the angle of the lifted car
    Fix16 field_A4_hook_axial_angle_speed;
    Fix16 field_A8_home_angle;
    Fix16 field_AC_crane_angle_target;  // the crane will rotate until match this angle
    Fix16 field_B0_hook_radius_target;
    Fix16 field_B4_hook_angle_target;
    Fix16 field_B8_hook_depth_target;
    Fix16 field_BC_drop_transporter_radius;
    Fix16 field_C0_drop_transporter_angle;
    Fix16_Point_POD field_C4_drop_transporter_pos;
    Fix16 field_CC_drop_transporter_z;
    Fix16 field_D0_drop_transporter_rot;
    Fix16 field_D4_drop_transporter_hook_depth;
    Fix16 field_D8_pickup_car_radius;
    Fix16 field_DC_pickup_car_angle;
    Fix16 field_E0_pickup_car_x;
    Fix16 field_E4_pickup_car_y;
    Fix16 field_E8_pickup_car_z;
    Fix16 field_EC_pickup_car_rot;
    Fix16 field_F0_pickup_car_hook_depth;
    Fix16 field_F4_transporter_cargo_radius;
    Fix16 field_F8_transporter_cargo_angle;
    Fix16 field_FC_transporter_cargo_x;
    Fix16 field_100_transporter_cargo_y;
    Fix16 field_104_transporter_cargo_z;
    Fix16 field_108_transporter_cargo_rot;
    Fix16 field_10C_transporter_cargo_hook_depth;
    Fix16 field_110_drop_angle;  // something to do with the crane angle
    Fix16 field_114_drop_radius;  // something to do with the hook radius
    Fix16 field_118_drop_rot;
    Fix16 field_11C_drop_hook_depth;
    Fix16 field_120_target1_radius;
    Fix16 field_124_target1_angle;
    Fix16 field_128_target1_rot;
    Fix16 field_12C_target1_hook_depth;
    Fix16 field_130_target2_radius;
    Fix16 field_134_target2_angle;
    Fix16 field_138_target2_rot;
    Fix16 field_13C_target2_hook_depth;
    s16 field_140_powerup_cmd;
    s16 field_142;
    s32 field_144;
    s32 field_148_disabled;
    char_type field_14C_return_delay;
    char_type field_14D_is_busy;
    char_type field_14E;
    char_type field_14F;
    s32 field_150;
    char_type field_154;
    char_type field_155;
    char_type field_156_is_rotating;
    char_type field_157_is_radius_changing;
    char_type field_158_is_hook_depth_changing;
    char_type field_159_hooked_car_this_frame;
    char_type field_15A;
    char_type field_15B;
};

class CranePool_D9C
{
  public:
    EXPORT void PickUpCar_480E00(Car_BC* a2, u8 a3);
    EXPORT void CranesService_480E50();
    EXPORT Crane_15C* NewCrane_480EC0(Fix16 a2, Fix16 a3);
    EXPORT CranePool_D9C();
    EXPORT ~CranePool_D9C();

    Crane_15C field_0_cranes[10];
    s32 field_D98_count;
};

EXTERN_GLOBAL(CranePool_D9C*, gCranePool_D9C_679FD4);
