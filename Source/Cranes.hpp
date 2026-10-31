#pragma once

#include "Car.hpp"
#include "Function.hpp"
#include "Object_5C.hpp"
#include "ang16.hpp"
#include "fix16.hpp"

class SoundObject;
class Car;

class Crane
{
  public:
    STATIC_MEMBER(Fix16, kHomeHookRadius);
    STATIC_MEMBER(Fix16, kFpTwo);
    // 9.6f 0x447F60
    inline void SetDisabled_447F60(s32 v)
    {
        field_148_disabled = v;
    }

    // 9.6f 0x476900
    inline void SetHomeCrane_476900(Crane* v)
    {
        field_78_home_crane = v;
    }

    EXPORT ~Crane();
    EXPORT Crane();
    EXPORT void ComputeHookPos_47E620(Fix16 radius, Ang16 ang, Fix16_Point* pOutPoint);
    EXPORT void ComputeHookPos_47E730(Ang16 radius, Fix16 ang, Fix16_Point* pOutPoint);
    EXPORT void ComputeHookOffset_47E840(Ang16 ang, Fix16_Point* pOutPoint);
    EXPORT bool IsDropPositionClear_47E920();
    EXPORT bool IsTarget1PositionClear_47EB00();
    EXPORT void DropHookedCar_47ECC0();
    EXPORT void DropHookedCarOnTransporter_47ED60();
    EXPORT void HookTransporterCargo_47EDF0();
    EXPORT void HookPickupCar_47EF80();
    EXPORT void DropInPlaceOrReturnHome_47F170();
    EXPORT void SetTransporterCargoTarget_47F220(Fix16 radius, Fix16 angle, Sprite* pCargo, Sprite* pTransporter);
    EXPORT void SetDropTransporterTarget_47F290(Fix16 radius, Fix16 angle, Sprite* pSprite);
    EXPORT void SetPickupCarTarget_47F2F0(Fix16 radius, Fix16 angle, Sprite* pSprite);
    EXPORT bool IsTransporterCargoTargetValid_47F350();
    EXPORT bool IsPickupCarTargetValid_47F3D0();
    EXPORT bool IsDropTransporterTargetValid_47F450();
    EXPORT void UpdateCraneTargets_47F4C0();
    EXPORT bool ComputeHookPolar_47F6C0(Fix16_Point& pPoint, Fix16* pOutF16, Fix16* pOutAng);
    EXPORT void TargetTransporter_47F7F0(Car* pCar);
    EXPORT void PickUpCar_47F930(Car* pCar);
    EXPORT void TryNextDropPosition_47FB40();
    EXPORT s32 MoveTowardsTargets_47FBA0();
    EXPORT void ReleaseHookedCar_47FD10();
    EXPORT void UpdateCraneTick_47FD50();
    EXPORT void UpdateCraneSprites_47FE10();
    EXPORT void Service_480310();
    EXPORT void InitCrane_4803B0(Fix16 x_pos, Fix16 y_pos, char_type obj_id);
    EXPORT void SetTarget1_480900(Fix16 xpos, Fix16 ypos, Ang16 ang);
    EXPORT void SetTarget2_480B60(Fix16 xpos, Fix16 ypos, Ang16 ang);
    EXPORT Car* GetCarFromCrane_480DA0();

    inline void SetHomeRotation_4768E0(Ang16 rotation)
    {
        field_A8_home_angle = Ang16::Ang16_to_Fix16(rotation);
    }

    // 9.6f 0x411A00
    inline Sprite* GetRotorSprite_411A00() const
    {
        return field_2C_rotor_obj->field_4_sprite;
    }

    // FUNCTION: 96f 0x447f40
    inline s32 IsAtHomeAngle_447F40() const
    {
        return field_8C_crane_angle == field_A8_home_angle;
    }

    Fix16_Point field_0_hooked_sprite_offset_target; // 0x0
    Fix16_Point field_8_drop_offset; // 0x8
    Fix16_Point field_10_hooked_sprite_offset; // 0x10
    Fix16_Point field_18_target1_offset; // 0x18
    Fix16_Point field_20_target2_offset; // 0x20
    SpriteList field_28_strct4; // 0x28
    Object* field_2C_rotor_obj; // 0x2C
    Object* field_30_part_obj_0; // 0x30
    Object* field_34_part_obj_1; // 0x34
    Object* field_38_part_obj_2; // 0x38
    Object* field_3C_part_obj_3; // 0x3C
    Object* field_40_part_obj_4; // 0x40
    Object* field_44_part_obj_5; // 0x44
    Object* field_48_part_obj_6; // 0x48
    Object* field_4C_part_obj_7; // 0x4C
    Object* field_50_part_obj_8; // 0x50
    Object* field_54_hook_obj; // 0x54
    Object* field_58_crane_base_obj; // 0x58
    Object* field_5C_counterweight_obj; // 0x5C
    Sprite* field_60_probe_sprite; // 0x60
    Sprite* field_64_drop_transporter; // 0x64
    Sprite* field_68_pickup_car; // 0x68
    Sprite* field_6C_transporter_cargo; // 0x6C
    Sprite* field_70_cargo_transporter; // 0x70
    Sprite* field_74_pSprite_on_hook; // 0x74
    Crane* field_78_home_crane; // 0x78
    SoundObject* field_7C_sound; // 0x7C
    Fix16 field_80_ground_z; // 0x80
    Fix16 field_84_hook_depth; // 0x84
    Fix16 field_88_hook_depth_speed; // 0x88
    Fix16 field_8C_crane_angle; // 0x8C:  It's not Ang16, maybe Ang32
    Fix16 field_90_hook_radius; // 0x90:  Radial distance
    Fix16 field_94_hook_radius_speed; // 0x94
    Fix16 field_98_crane_angle_speed; // 0x98
    Fix16 field_9C_unused; // 0x9C
    Fix16 field_A0_hook_axial_angle;  // 0xA0: The angle in its own rotation axis, e.g. the angle of the lifted car
    Fix16 field_A4_hook_axial_angle_speed; // 0xA4
    Fix16 field_A8_home_angle; // 0xA8
    Fix16 field_AC_crane_angle_target;  // 0xAC: the crane will rotate until match this angle
    Fix16 field_B0_hook_radius_target; // 0xB0
    Fix16 field_B4_hook_angle_target; // 0xB4
    Fix16 field_B8_hook_depth_target; // 0xB8
    Fix16 field_BC_drop_transporter_radius; // 0xBC
    Fix16 field_C0_drop_transporter_angle; // 0xC0
    Fix16_Point_POD field_C4_drop_transporter_pos; // 0xC4
    Fix16 field_CC_drop_transporter_z; // 0xCC
    Fix16 field_D0_drop_transporter_rot; // 0xD0
    Fix16 field_D4_drop_transporter_hook_depth; // 0xD4
    Fix16 field_D8_pickup_car_radius; // 0xD8
    Fix16 field_DC_pickup_car_angle; // 0xDC
    Fix16 field_E0_pickup_car_x; // 0xE0
    Fix16 field_E4_pickup_car_y; // 0xE4
    Fix16 field_E8_pickup_car_z; // 0xE8
    Fix16 field_EC_pickup_car_rot; // 0xEC
    Fix16 field_F0_pickup_car_hook_depth; // 0xF0
    Fix16 field_F4_transporter_cargo_radius; // 0xF4
    Fix16 field_F8_transporter_cargo_angle; // 0xF8
    Fix16 field_FC_transporter_cargo_x; // 0xFC
    Fix16 field_100_transporter_cargo_y; // 0x100
    Fix16 field_104_transporter_cargo_z; // 0x104
    Fix16 field_108_transporter_cargo_rot; // 0x108
    Fix16 field_10C_transporter_cargo_hook_depth; // 0x10C
    Fix16 field_110_drop_angle;  // 0x110: something to do with the crane angle
    Fix16 field_114_drop_radius;  // 0x114: something to do with the hook radius
    Fix16 field_118_drop_rot; // 0x118
    Fix16 field_11C_drop_hook_depth; // 0x11C
    Fix16 field_120_target1_radius; // 0x120
    Fix16 field_124_target1_angle; // 0x124
    Fix16 field_128_target1_rot; // 0x128
    Fix16 field_12C_target1_hook_depth; // 0x12C
    Fix16 field_130_target2_radius; // 0x130
    Fix16 field_134_target2_angle; // 0x134
    Fix16 field_138_target2_rot; // 0x138
    Fix16 field_13C_target2_hook_depth; // 0x13C
    s16 field_140_powerup_cmd; // 0x140
    s16 field_142_unused; // 0x142
    s32 field_144_crane_mode; // 0x144
    s32 field_148_disabled; // 0x148
    char_type field_14C_return_delay; // 0x14C
    bool field_14D_is_busy; // 0x14D
    char_type field_14E_unused; // 0x14E
    char_type field_14F_unused; // 0x14F
    s32 field_150_hook_job; // 0x150
    char_type field_154_unused; // 0x154
    char_type field_155_load_phase; // 0x155
    char_type field_156_is_rotating; // 0x156
    char_type field_157_is_radius_changing; // 0x157
    char_type field_158_is_hook_depth_changing; // 0x158
    bool field_159_hooked_car_this_frame; // 0x159
    char_type field_15A_unused; // 0x15A
    char_type field_15B_unused; // 0x15B
};
GTA2_ASSERT_SIZEOF_ALWAYS(Crane, 0x15C);

class CranePool
{
  public:
    EXPORT void PickUpCar_480E00(Car* pCar, u8 crane_idx);
    EXPORT void CranesService_480E50();
    EXPORT Crane* NewCrane_480EC0(Fix16 x_pos, Fix16 y_pos);
    EXPORT CranePool();
    EXPORT ~CranePool();

    Crane field_0_cranes[10]; // 0x0
    s32 field_D98_count; // 0xD98
};
GTA2_ASSERT_SIZEOF_ALWAYS(CranePool, 0xD9C);

EXTERN_GLOBAL(CranePool*, gCranePool_D9C_679FD4);
