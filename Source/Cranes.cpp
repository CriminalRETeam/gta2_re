#include "Cranes.hpp"
#include "Char_Pool.hpp"
#include "Globals.hpp"
#include "Hud.hpp"
#include "Object_5C.hpp"
#include "PurpleDoom.hpp"
#include "debug.hpp"
#include "frosty_pasteur_0xC1EA8.hpp"
#include "map_0x370.hpp"
#include "rng.hpp"
#include "root_sound.hpp"
#include "sprite.hpp"

// TODO: Move
EXPORT void __stdcall SmoothApproachAngle_405CE0(Fix16& a1, Fix16& a2, Fix16& a3, Fix16& a4, Fix16& a5);
EXPORT void __stdcall SmoothApproach_4F7540(Fix16& Coord_1, Fix16& Velocity_1, Fix16& Coord_2, Fix16& Velocity_2, Fix16& Velocity_3);

DEFINE_GLOBAL_INIT(Fix16, kHomeHookRadius_679E58, Fix16(0x2000, 0), 0x679E58);
DEFINE_GLOBAL_INIT(Fix16, kZero_679E70, Fix16(0), 0x679E70);
DEFINE_GLOBAL_INIT(Fix16, kFpTwo_679E78, Fix16(2), 0x679E78);
DEFINE_GLOBAL_INIT(Fix16, kFpTwo_679C78, kFpTwo_679E78, 0x679C78);
DEFINE_GLOBAL_INIT(Ang16, kAngZero_679FC4, Ang16(0), 0x679FC4);
DEFINE_GLOBAL(CranePool_D9C*, gCranePool_D9C_679FD4, 0x679FD4);
DEFINE_GLOBAL_INIT(Fix16, kHomeHookAxialAngle_679D50, kZero_679E70, 0x679D50);
DEFINE_GLOBAL_INIT(Fix16, kFpTwo_679F8C, kFpTwo_679E78, 0x679F8C);
DEFINE_GLOBAL_INIT(Fix16, kFpThree_679E7C, Fix16(0xC000, 0), 0x679E7C);
DEFINE_GLOBAL_INIT(Fix16, kFpThree_679F88, kFpThree_679E7C, 0x679F88);
DEFINE_GLOBAL_INIT(Fix16, kAngFix16OneDegree_679FC8, Fix16(0x11C, 0), 0x679FC8);
DEFINE_GLOBAL_INIT(Fix16, kDropRetryAngleStep_679F64, kAngFix16OneDegree_679FC8 * 20, 0x679F64);
DEFINE_GLOBAL_INIT(Fix16, kAngFix16FullCircle_679F58, Fix16(0x18F60, 0), 0x679F58);
DEFINE_GLOBAL_INIT(Fix16, kFpFour_679E80, Fix16(0x10000, 0), 0x679E80);
DEFINE_GLOBAL_INIT(Fix16, kMaxHookRadius_679F68, kFpFour_679E80, 0x679F68);
DEFINE_GLOBAL_INIT(Fix16, kFpHalf_679CB0, Fix16(0x2000, 0), 0x679CB0);
DEFINE_GLOBAL_INIT(Fix16, kMinHookRadius_679C3C, kFpHalf_679CB0, 0x679C3C);
DEFINE_GLOBAL_INIT(Fix16, kFpOne_679E74, Fix16(0x4000, 0), 0x679E74);
DEFINE_GLOBAL_INIT(Fix16, kFpHalf_679D64, Fix16(0x2000, 0), 0x679D64);
DEFINE_GLOBAL_INIT(Fix16, kFpHalf_679D34, kFpHalf_679D64, 0x679D34);
DEFINE_GLOBAL_INIT(Fix16, kFpThreeAndHalf_679D28, kFpThree_679E7C + kFpHalf_679D64, 0x679D28);
DEFINE_GLOBAL_INIT(Fix16, kFpTwoAndHalf_679D2C, kFpTwo_679E78 + kFpHalf_679D64, 0x679D2C);
DEFINE_GLOBAL_INIT(Fix16, kFpOneAndHalf_679D30, kFpOne_679E74 + kFpHalf_679D64, 0x679D30);

DEFINE_GLOBAL_INIT(Fix16, kFpOne64th_679E20, Fix16(0x100, 0), 0x679E20);
DEFINE_GLOBAL_INIT(Fix16, kFpOne64th_679F28, kFpOne64th_679E20, 0x679F28);
DEFINE_GLOBAL_INIT(Fix16, kHookRadiusAccel_679C14, kFpOne64th_679F28, 0x679C14);
DEFINE_GLOBAL_INIT(Fix16, kHookRadiusMaxSpeed_679E6C, kFpOne64th_679F28 * 4, 0x679E6C);
DEFINE_GLOBAL_INIT(Fix16, kCraneAngleAccel_679F70, kAngFix16OneDegree_679FC8* kFpHalf_679D64, 0x679F70);
DEFINE_GLOBAL_INIT(Fix16, kCraneAngleMaxSpeed_679DEC, kAngFix16OneDegree_679FC8 * 2, 0x679DEC);
DEFINE_GLOBAL_INIT(Fix16, kHookAxialAngleAccel_679D70, kAngFix16OneDegree_679FC8, 0x679D70);
DEFINE_GLOBAL_INIT(Fix16, kHookAxialAngleMaxSpeed_679C40, kAngFix16OneDegree_679FC8 * 4, 0x679C40);
DEFINE_GLOBAL_INIT(Fix16, kHookDepthAccel_679DC0, kFpOne64th_679F28, 0x679DC0);
DEFINE_GLOBAL_INIT(Fix16, kHookDepthMaxSpeed_679DC8, kFpOne64th_679F28 * 4, 0x679DC8);
DEFINE_GLOBAL_INIT(Fix16, kAngFix16HalfCircle_679EB8, Fix16(0xC7B0, 0), 0x679EB8);

// FUNCTION: 96f 0x40e790
inline Fix16 __stdcall WrapAngle_40E790(Fix16 a2)
{
    while (a2 < kZero_679E70)
    {
        a2 += kAngFix16FullCircle_679F58;
    }

    if (a2 >= kAngFix16FullCircle_679F58)
    {
        do
        {
            a2 -= kAngFix16FullCircle_679F58;
        } while (a2 >= kAngFix16FullCircle_679F58);
    }
    return a2;
}

// TODO: The original has an EH frame in state 4, so field_0-field_20_target2_offset are probably Fix16_Point
// (non-trivial dtor). Changing them makes this match, but then the ctor stops matching because
// the implicit Fix16_Point_POD ctor isn't inlined.
WIP_FUNC(0x47e5b0)
Crane_15C::~Crane_15C()
{
    WIP_IMPLEMENTED;
    if (field_7C_sound)
    {
        gRoot_sound_66B038.DestroySoundObj_40FE60(field_7C_sound);
        field_7C_sound = 0;
    }
}

MATCH_FUNC(0x47e610)
Crane_15C::Crane_15C()
{
    field_7C_sound = 0;
}

MATCH_FUNC(0x47e620)
void Crane_15C::ComputeHookPos_47E620(Fix16 radius, Ang16 ang, Fix16_Point* pOutPoint)
{
    pOutPoint->SetXY_432860(kZero_679E70, radius);

    // RotateByAngle_40F6B0, but the y part uses the out-of-line Fix16 operators
    Fix16 sin = Ang16::sine_40F500(ang);
    Fix16 cos = Ang16::cosine_40F520(ang);
    Fix16 x_old = pOutPoint->x;
    pOutPoint->x = (pOutPoint->x * cos) + (pOutPoint->y * sin);
    pOutPoint->y = (const Fix16&)x_old.Negate_4086A0().Multiply_408680(sin) + (pOutPoint->y * cos);

    *pOutPoint += field_2C_rotor_obj->field_4->get_x_y_443580();
}

WIP_FUNC(0x47e730)
void Crane_15C::ComputeHookPos_47E730(Ang16 radius, Fix16 ang, Fix16_Point* pOutPoint)
{
    WIP_IMPLEMENTED;
    pOutPoint->SetXY_432860(kZero_679E70, ang);
    pOutPoint->RotateByAngle_40F6B0(radius);
    *pOutPoint += field_2C_rotor_obj->field_4->get_x_y_443580();
}

// 9.6f 0x448030
WIP_FUNC(0x47e840)
void Crane_15C::ComputeHookOffset_47E840(Ang16 ang, Fix16_Point* pOutPoint)
{
    WIP_IMPLEMENTED;

    pOutPoint->SetXY_432860(kZero_679E70, -kFpHalf_679D64);
    pOutPoint->RotateByAngle_40F6B0(ang);
    *pOutPoint += field_2C_rotor_obj->field_4->get_x_y_443580();
}

// 9.6f 0x448090
MATCH_FUNC(0x47e920)
bool Crane_15C::IsDropPositionClear_47E920()
{
    Fix16_Point pos;
    ComputeHookPos_47E620(field_114_drop_radius, Ang16::Fix16_To_Ang16_40F540(field_110_drop_angle), &pos);
    pos += field_8_drop_offset;

    field_60_probe_sprite->set_xyz_lazy_420600(pos.x, pos.y, this->field_80_ground_z - this->field_11C_drop_hook_depth);
    field_60_probe_sprite->set_ang_lazy_420690(Ang16::Fix16_To_Ang16_40F540(field_118_drop_rot));

    return !gPurpleDoom_1_679208->FindNearestSpriteOfType_477E60(field_60_probe_sprite, sprite_types_enum::unknown_0) &&
        !field_60_probe_sprite->CheckSpriteMovementRegion_5A2500();
}

// 9.6f 0x448150
MATCH_FUNC(0x47eb00)
bool Crane_15C::IsTarget1PositionClear_47EB00()
{
    Fix16_Point hookPos;
    ComputeHookPos_47E620(field_120_target1_radius, Ang16::Fix16_To_Ang16_40F540(field_124_target1_angle), &hookPos);
    hookPos += field_18_target1_offset;

    field_60_probe_sprite->set_xyz_lazy_420600(hookPos.x, hookPos.y, field_80_ground_z - field_12C_target1_hook_depth);
    field_60_probe_sprite->set_ang_lazy_420690(Ang16::Fix16_To_Ang16_40F540(field_128_target1_rot));

    return gPurpleDoom_1_679208->FindNearestSpriteOfType_477E60(field_60_probe_sprite, sprite_types_enum::unknown_0) == 0;
}

MATCH_FUNC(0x47ecc0)
void Crane_15C::DropHookedCar_47ECC0()
{
    Car_BC* pCar = field_74_pSprite_on_hook->AsCar_40FEB0();
    gPurpleDoom_1_679208->AddToRegionBuckets_477B20(field_74_pSprite_on_hook);
    pCar->sub_4435F0();
    pCar->SetupCarPhysicsAndSpriteBinding_43BCA0();
    gCar_BC_Pool_67792C->UpdateNextPrev(pCar);
    pCar->SetF_88_447ea0();

    if (field_150 != 3)
    {
        if (field_144 == 2)
        {
            if (field_155 == 1)
            {
                field_155 = 2;
            }
            else
            {
                field_155 = 1;
                field_28_strct4.AddSprite_5A6CD0(field_74_pSprite_on_hook);
            }
        }
        else
        {
            field_28_strct4.AddSprite_5A6CD0(field_74_pSprite_on_hook);
        }
    }
    field_74_pSprite_on_hook = 0;
    field_150 = 0;
    Crane_15C::sub_47F170();
}

MATCH_FUNC(0x47ed60)
void Crane_15C::DropHookedCarOnTransporter_47ED60()
{
    Car_BC* pCar = field_74_pSprite_on_hook->AsCar_40FEB0();
    gCar_BC_Pool_67792C->UpdateNextPrev(pCar);
    pCar->SetF_88_447ea0();
    gPurpleDoom_1_679208->AddToRegionBuckets_477B20(field_74_pSprite_on_hook);
    field_64_drop_transporter->DispatchCollisionEvent_5A3100(field_74_pSprite_on_hook, kZero_679E70, kZero_679E70, kAngZero_679FC4);
    field_28_strct4.AddSprite_5A6CD0(field_64_drop_transporter);
    field_74_pSprite_on_hook = 0;
    field_150 = 0;
    field_64_drop_transporter = 0;
    Crane_15C::sub_47F170();
}

// 9.6 0x448300
MATCH_FUNC(0x47edf0)
void Crane_15C::HookTransporterCargo_47EDF0()
{
    Car_BC* pCar = field_70_cargo_transporter->AsCar_40FEB0();

    pCar->field_0_qq.RemoveSprite_5A6B10(field_6C_transporter_cargo);
    gPurpleDoom_3_679210->Remove_477B00(field_6C_transporter_cargo);

    this->field_74_pSprite_on_hook = this->field_6C_transporter_cargo;
    this->field_10_hooked_sprite_offset = field_74_pSprite_on_hook->get_x_y_443580() - field_54_hook_obj->field_4->get_x_y_443580();

    field_60_probe_sprite->field_C_sprite_4c_ptr->CopyXYZ_447DF0(field_74_pSprite_on_hook->field_C_sprite_4c_ptr);

    this->field_6C_transporter_cargo = 0;

    if (field_144 == 1 || field_144 == 2 || field_144 == 3)
    {
        this->field_150 = 4;
        this->field_114_drop_radius = this->field_120_target1_radius;
        this->field_110_drop_angle = this->field_124_target1_angle;
        this->field_118_drop_rot = this->field_128_target1_rot;
        this->field_8_drop_offset = this->field_18_target1_offset;
        this->field_11C_drop_hook_depth = this->field_12C_target1_hook_depth;
    }
    else
    {
        this->field_150 = 2;
        this->field_114_drop_radius = kFpTwo_679E78;

        this->field_110_drop_angle = WrapAngle_40E790(field_8C_crane_angle + kAngFix16HalfCircle_679EB8);
        this->field_118_drop_rot = field_A0_hook_axial_angle;
        this->field_8_drop_offset = this->field_10_hooked_sprite_offset;
        this->field_11C_drop_hook_depth = kFpTwo_679C78;
    }
}

// 9.6f 0x448450
MATCH_FUNC(0x47ef80)
void Crane_15C::HookPickupCar_47EF80()
{
    this->field_159_hooked_car_this_frame = 1;

    Car_BC* pCar = field_68_pickup_car->AsCar_40FEB0();
    if (pCar->field_95_player_ped_id)
    {
        Ped* pPed = gPedManager_6787BC->PedById(pCar->field_95_player_ped_id);
        if (pPed)
        {
            if (pPed->is_player_41B0A0())
            {
                pCar->field_95_player_ped_id = 0;
            }
        }
    }

    gCar_BC_Pool_67792C->field_0_pool.UnlinkFromActiveList_420F30(pCar);

    pCar->SetF_88_4214E0();
    pCar->DeAllocateCarPhysics_43BD00();
    gPurpleDoom_1_679208->AddToSpriteRectBuckets_477B60(field_68_pickup_car);

    this->field_74_pSprite_on_hook = this->field_68_pickup_car;
    this->field_10_hooked_sprite_offset = field_74_pSprite_on_hook->get_x_y_443580() - field_54_hook_obj->field_4->get_x_y_443580();

    field_60_probe_sprite->field_C_sprite_4c_ptr->CopyXYZ_447DF0(field_74_pSprite_on_hook->field_C_sprite_4c_ptr);

    this->field_68_pickup_car = 0;

    if ((this->field_144 == 2 || this->field_144 == 3) && pCar->Is_F9_Eq7_447EB0())
    {
        this->field_114_drop_radius = this->field_130_target2_radius;
        this->field_110_drop_angle = this->field_134_target2_angle;
        this->field_118_drop_rot = this->field_138_target2_rot;
        this->field_8_drop_offset = this->field_20_target2_offset;
        this->field_11C_drop_hook_depth = this->field_13C_target2_hook_depth;
    }
    else
    {
        if (this->field_144 != 1 && this->field_144 != 2)
        {
            this->field_150 = 1;
            return;
        }
        this->field_114_drop_radius = this->field_120_target1_radius;
        this->field_110_drop_angle = this->field_124_target1_angle;
        this->field_118_drop_rot = this->field_128_target1_rot;
        this->field_8_drop_offset = this->field_18_target1_offset;
        this->field_11C_drop_hook_depth = this->field_12C_target1_hook_depth;
    }

    this->field_150 = 4;
}

// 9.6f 0x447D40
MATCH_FUNC(0x47f170)
void Crane_15C::sub_47F170()
{
    if (field_150)
    {
        field_150 = 3;
        field_114_drop_radius = field_90_hook_radius;
        field_110_drop_angle = field_8C_crane_angle;
        field_118_drop_rot = field_A0_hook_axial_angle;
        field_11C_drop_hook_depth = kFpTwo_679C78;
        field_B0_hook_radius_target = field_90_hook_radius;
        field_AC_crane_angle_target = field_8C_crane_angle;
        field_B4_hook_angle_target = field_A0_hook_axial_angle;
        field_B8_hook_depth_target = field_11C_drop_hook_depth;
    }
    else
    {
        field_B0_hook_radius_target = kHomeHookRadius_679E58;
        field_AC_crane_angle_target = field_A8_home_angle;
        field_B4_hook_angle_target = kHomeHookAxialAngle_679D50;
        field_B8_hook_depth_target = kZero_679E70;
    }
    field_14D_is_busy = 0;
}

MATCH_FUNC(0x47f220)
void Crane_15C::SetTransporterCargoTarget_47F220(Fix16 a2, Fix16 a3, Sprite* a4, Sprite* a5)
{
    field_F4_transporter_cargo_radius = a2;
    field_F8_transporter_cargo_angle = a3;
    field_6C_transporter_cargo = a4;
    field_70_cargo_transporter = a5;
    field_108_transporter_cargo_rot = Ang16::Ang16_to_Fix16(a4->field_0);
    field_FC_transporter_cargo_x = a4->field_14_xy.x;
    field_100_transporter_cargo_y = a4->field_14_xy.y;
    field_104_transporter_cargo_z = a4->field_1C_zpos;
    field_10C_transporter_cargo_hook_depth = field_80_ground_z - field_104_transporter_cargo_z;
}

MATCH_FUNC(0x47f290)
void Crane_15C::SetDropTransporterTarget_47F290(Fix16 a2, Fix16 a3, Sprite* a4)
{
    field_BC_drop_transporter_radius = a2;
    field_C0_drop_transporter_angle = a3;
    field_64_drop_transporter = a4;
    field_D0_drop_transporter_rot = Ang16::Ang16_to_Fix16(a4->field_0);
    field_C4_drop_transporter_pos.x = a4->field_14_xy.x;
    field_C4_drop_transporter_pos.y = a4->field_14_xy.y;
    field_CC_drop_transporter_z = a4->field_1C_zpos;
    field_D4_drop_transporter_hook_depth = field_80_ground_z - field_CC_drop_transporter_z;
}

MATCH_FUNC(0x47f2f0)
void Crane_15C::SetPickupCarTarget_47F2F0(Fix16 a2, Fix16 a3, Sprite* a4)
{
    field_D8_pickup_car_radius = a2;
    field_DC_pickup_car_angle = a3;
    field_68_pickup_car = a4;
    field_EC_pickup_car_rot = Ang16::Ang16_to_Fix16(a4->field_0);
    field_E0_pickup_car_x = a4->field_14_xy.x;
    field_E4_pickup_car_y = a4->field_14_xy.y;
    field_E8_pickup_car_z = a4->field_1C_zpos;
    field_F0_pickup_car_hook_depth = field_80_ground_z - field_E8_pickup_car_z;
}

// 9.6f 0x448730
MATCH_FUNC(0x47f350)
bool Crane_15C::IsTransporterCargoTargetValid_47F350()
{
    Car_BC* pCar1 = field_70_cargo_transporter->AsCar_40FEB0();
    if (!pCar1->IsDespawning_4215B0())
    {
        Sprite *sp = field_6C_transporter_cargo;
        Car_BC* pCar2 = sp->AsCar_40FEB0();
        if (!(pCar2->IsDespawning_4215B0() || field_FC_transporter_cargo_x != sp->field_14_xy.x || field_100_transporter_cargo_y != sp->field_14_xy.y ||
            field_104_transporter_cargo_z != sp->field_1C_zpos || field_108_transporter_cargo_rot != Ang16::Ang16_to_Fix16(sp->field_0)))
        {
            return true;
        }
    }
    return false;
}

// 9.6f 0x4487D0
MATCH_FUNC(0x47f3d0)
bool Crane_15C::IsPickupCarTargetValid_47F3D0()
{
    Car_BC* v2 = field_68_pickup_car->AsCar_40FEB0();
    if (!v2->IsDespawning_4215B0() && this->field_E0_pickup_car_x == field_68_pickup_car->field_14_xy.x && this->field_E4_pickup_car_y == field_68_pickup_car->field_14_xy.y &&
        this->field_E8_pickup_car_z == field_68_pickup_car->field_1C_zpos && this->field_EC_pickup_car_rot == Ang16::Ang16_to_Fix16(field_68_pickup_car->field_0) && !v2->field_54_driver &&
        v2->AreAllDoorsClosed_441A40())
    {
        return true;
    }
    return false;
}

MATCH_FUNC(0x47f450)
bool Crane_15C::IsDropTransporterTargetValid_47F450()
{
    Car_BC* pCar = field_64_drop_transporter->AsCar_40FEB0();
    if (!pCar->IsDespawning_4215B0() && this->field_C4_drop_transporter_pos.x == field_64_drop_transporter->field_14_xy.x && this->field_C4_drop_transporter_pos.y == field_64_drop_transporter->field_14_xy.y &&
        this->field_CC_drop_transporter_z == field_64_drop_transporter->field_1C_zpos && this->field_D0_drop_transporter_rot == Ang16::Ang16_to_Fix16(field_64_drop_transporter->field_0))
    {
        return pCar->field_0_qq.FirstSpriteOfType_5A6CA0(sprite_types_enum::car_2) ? false : true;
    }
    return false;
}

MATCH_FUNC(0x47f4c0)
void Crane_15C::UpdateCraneTargets_47F4C0()
{
    if (this->field_6C_transporter_cargo)
    {
        if (!IsTransporterCargoTargetValid_47F350())
        {
            this->field_6C_transporter_cargo = 0;
            this->field_14C_return_delay = 60;
        }
    }

    if (this->field_68_pickup_car)
    {
        if (!IsPickupCarTargetValid_47F3D0())
        {
            this->field_68_pickup_car = 0;
            this->field_14C_return_delay = 60;
        }
    }

    if (this->field_64_drop_transporter)
    {
        if (!IsDropTransporterTargetValid_47F450())
        {
            this->field_64_drop_transporter = 0;
            this->field_14C_return_delay = 60;
        }
    }

    if (field_150 == 2 || field_150 == 3 || field_150 == 4)
    {
        if (IsDropPositionClear_47E920())
        {
            this->field_14D_is_busy = 1;
        }
        else
        {
            this->field_14D_is_busy = 0;
            if (field_150 != 4)
            {
                TryNextDropPosition_47FB40();
            }
        }
    }

    if (field_150 == 2 || field_150 == 3 || field_150 == 4)
    {
        this->field_B0_hook_radius_target = field_114_drop_radius;
        this->field_AC_crane_angle_target = field_110_drop_angle;
        this->field_B4_hook_angle_target = field_118_drop_rot;
        this->field_0_hooked_sprite_offset_target = this->field_8_drop_offset;
        this->field_B8_hook_depth_target = field_11C_drop_hook_depth;
    }
    else if (field_150 == 1 && this->field_64_drop_transporter)
    {
        this->field_B0_hook_radius_target = field_BC_drop_transporter_radius;
        this->field_0_hooked_sprite_offset_target.reset();
        this->field_AC_crane_angle_target = field_C0_drop_transporter_angle;
        this->field_14D_is_busy = 1;
        this->field_B4_hook_angle_target = field_D0_drop_transporter_rot;
        this->field_B8_hook_depth_target = field_D4_drop_transporter_hook_depth;
    }
    else if (field_6C_transporter_cargo)
    {
        this->field_B0_hook_radius_target = field_F4_transporter_cargo_radius;
        this->field_AC_crane_angle_target = field_F8_transporter_cargo_angle;
        this->field_B4_hook_angle_target = field_108_transporter_cargo_rot;
        this->field_14D_is_busy = 1;
        this->field_B8_hook_depth_target = field_10C_transporter_cargo_hook_depth;
    }
    else if (field_68_pickup_car && (field_64_drop_transporter || field_144 == 1 || field_144 == 2 || field_144 == 3))
    {
        this->field_B0_hook_radius_target = field_D8_pickup_car_radius;
        this->field_AC_crane_angle_target = field_DC_pickup_car_angle;
        this->field_B4_hook_angle_target = field_EC_pickup_car_rot;
        this->field_14D_is_busy = 1;
        this->field_B8_hook_depth_target = field_F0_pickup_car_hook_depth;
    }
    else
    {
        field_14D_is_busy = 0;

        if (!field_14C_return_delay)
        {
            sub_47F170();
        }
        else
        {
            field_14C_return_delay--;
        }
    }
}

// 9.6f 0x448900
MATCH_FUNC(0x47f6c0)
bool Crane_15C::ComputeHookPolar_47F6C0(Fix16_Point& pPoint, Fix16* pOutF16, Fix16* pOutAng)
{
    Fix16_Point v10 = (pPoint - field_2C_rotor_obj->field_4->get_x_y_443580());
    *pOutF16 = v10.GetLength_no_sqrt_inline(); // TODO: Uses kZero_679E70 as Zero

    // TODO: 1st check is removed in 9.6f ??
    if (*pOutF16 <= kMaxHookRadius_679F68 && *pOutF16 >= kMinHookRadius_679C3C)
    {
        *pOutAng = Ang16::Ang16_to_Fix16(v10.atan2_40F790());
        return true;
    }
    else
    {
        return false;
    }
}

// 9.6f 0x448980
// 10.5 https://decomp.me/scratch/XYPfQ
MATCH_FUNC(0x47f7f0)
void Crane_15C::TargetTransporter_47F7F0(Car_BC* pCar)
{
    Fix16 point;
    Fix16 t;
    Sprite* pFoundSprite = pCar->field_0_qq.FirstSpriteOfType_5A6CA0(sprite_types_enum::car_2);
    if (pFoundSprite)
    {
        if (!field_150)
        {
            if (ComputeHookPolar_47F6C0(pFoundSprite->get_x_y_443580(), &point, &t))
            {
                if (field_6C_transporter_cargo == 0 || pFoundSprite == field_6C_transporter_cargo)
                {
                    SetTransporterCargoTarget_47F220(point, t, pFoundSprite, pCar->field_50_car_sprite);
                    field_64_drop_transporter = 0;
                    field_68_pickup_car = 0;
                }
            }
        }
    }
    else if (field_6C_transporter_cargo == 0 && (field_150 == 0 || field_150 == 1) && field_144 == 0)
    {
        Sprite* pSprt = pCar->field_50_car_sprite;
        if (ComputeHookPolar_47F6C0(pSprt->get_x_y_443580(), &point, &t))
        {
            if (field_64_drop_transporter == 0 || pSprt == field_64_drop_transporter)
            {
                SetDropTransporterTarget_47F290(point, t, pSprt);
            }
        }
    }
}

// 9.6f 0x448A80
// 10.5 https://decomp.me/scratch/HB5R5 return jump issue
WIP_FUNC(0x47f930)
void Crane_15C::PickUpCar_47F930(Car_BC* pCar)
{
    WIP_IMPLEMENTED;

    if (!pCar->IsDespawning_4215B0() && !field_28_strct4.TagSpriteWithRng_5A6C10(pCar->field_50_car_sprite))
    {
        if (pCar->Is_TRUKTRNS_447EC0())
        {
            TargetTransporter_47F7F0(pCar);
        }
        else if (field_64_drop_transporter || (field_144 == 1) || (field_144 == 2 || field_144 == 3) && field_155 == 1 && !pCar->Is_F9_Eq7_447EB0() ||
                 field_155 == 2 && pCar->Is_F9_Eq7_447EB0())
        {
            if (!field_150 && !pCar->field_54_driver)
            {
                if (pCar->AreAllDoorsClosed_441A40())
                {
                    if (pCar->CanBeLiftedByCrane_447F00())
                    {
                        Fix16 a2a;
                        Fix16 angTmp;
                        Sprite* pSprt = pCar->field_50_car_sprite;
                        if (ComputeHookPolar_47F6C0(pSprt->get_x_y_443580(), &a2a, &angTmp))
                        {
                            if (field_144 != 1 || IsTarget1PositionClear_47EB00())
                            {
                                if (field_68_pickup_car == 0 || pSprt == field_68_pickup_car)
                                {
                                    SetPickupCarTarget_47F2F0(a2a, angTmp, pSprt);
                                }
                            }
                        }
                    }
                    else
                    {
                        Trailer* pTrailer = pCar->field_64_pTrailer;
                        if (!pTrailer || pTrailer->field_C_pCarOnTrailer == 0 || !pTrailer->field_C_pCarOnTrailer->Is_TRUKTRNS_447EC0())
                        {
                            gHud_2B00_706620->field_DC_brief.SetHudBrief_5D4400(1, "nespray");
                            field_28_strct4.AddSprite_5A6CD0(pCar->field_50_car_sprite);
                            field_28_strct4.TagSpriteWithRng_5A6C10(pCar->field_50_car_sprite);
                        }
                    }
                }
            }
        }
    }
}

// 9.6f 0x448C00
MATCH_FUNC(0x47fb40)
void Crane_15C::TryNextDropPosition_47FB40()
{
    if (field_114_drop_radius == kFpTwo_679F8C)
    {
        field_114_drop_radius = kFpThree_679F88;
    }
    else
    {
        field_114_drop_radius = kFpTwo_679F8C;
        field_110_drop_angle = WrapAngle_40E790(field_110_drop_angle + kDropRetryAngleStep_679F64);
    }
}

MATCH_FUNC(0x47fba0)
s32 Crane_15C::MoveTowardsTargets_47FBA0()
{
    if (this->field_84_hook_depth == kZero_679E70)
    {
        SmoothApproach_4F7540(this->field_B0_hook_radius_target, this->field_94_hook_radius_speed, this->field_90_hook_radius, kHookRadiusAccel_679C14, kHookRadiusMaxSpeed_679E6C);
        SmoothApproachAngle_405CE0(field_AC_crane_angle_target, field_98_crane_angle_speed, field_8C_crane_angle, kCraneAngleAccel_679F70, kCraneAngleMaxSpeed_679DEC);
        SmoothApproachAngle_405CE0(field_B4_hook_angle_target, field_A4_hook_axial_angle_speed, field_A0_hook_axial_angle, kHookAxialAngleAccel_679D70, kHookAxialAngleMaxSpeed_679C40);
        if (this->field_74_pSprite_on_hook)
        {
            this->field_10_hooked_sprite_offset = this->field_0_hooked_sprite_offset_target;
        }
    }

    bool bUnknown;
    Fix16 f_B8_hook_depth_target;
    if (this->field_90_hook_radius == this->field_B0_hook_radius_target &&
        this->field_8C_crane_angle == this->field_AC_crane_angle_target &&
        this->field_A0_hook_axial_angle == this->field_B4_hook_angle_target && this->field_14D_is_busy)
    {
        bUnknown = 1;
        f_B8_hook_depth_target = this->field_B8_hook_depth_target;
    }
    else
    {
        bUnknown = 0;
        f_B8_hook_depth_target = kZero_679E70;
    }

    SmoothApproach_4F7540(f_B8_hook_depth_target, this->field_88_hook_depth_speed, this->field_84_hook_depth, kHookDepthAccel_679DC0, kHookDepthMaxSpeed_679DC8);

    this->field_156_is_rotating = this->field_8C_crane_angle != this->field_AC_crane_angle_target;
    this->field_157_is_radius_changing = this->field_90_hook_radius != this->field_B0_hook_radius_target;
    this->field_158_is_hook_depth_changing = f_B8_hook_depth_target != field_84_hook_depth;
    return bUnknown && field_84_hook_depth == f_B8_hook_depth_target;
}

MATCH_FUNC(0x47fd10)
void Crane_15C::ReleaseHookedCar_47FD10()
{
    if (field_140_powerup_cmd)
    {
        gfrosty_pasteur_6F8060->sub_511B10(field_140_powerup_cmd);
    }

    switch (field_150)
    {
        case 1:
            DropHookedCarOnTransporter_47ED60();
            break;

        case 2:
        case 3:
        case 4:
            DropHookedCar_47ECC0();
            break;
    }
}

MATCH_FUNC(0x47fd50)
void Crane_15C::UpdateCraneTick_47FD50()
{
    Fix16 old_crane_angle = field_8C_crane_angle;
    Fix16 old_hook_radius = field_90_hook_radius;
    Fix16 old_hook_axial_angle = field_A0_hook_axial_angle;
    Fix16 old_hook_depth = field_84_hook_depth;

    u8 v6 = Crane_15C::MoveTowardsTargets_47FBA0();

    if (old_crane_angle != field_8C_crane_angle || old_hook_radius != field_90_hook_radius ||
        old_hook_axial_angle != field_A0_hook_axial_angle || old_hook_depth != field_84_hook_depth)
    {
        Crane_15C::UpdateCraneSprites_47FE10();
    }
    if (v6 != 0)
    {
        if (field_6C_transporter_cargo)
        {
            Crane_15C::HookTransporterCargo_47EDF0();
        }
        else if (field_68_pickup_car && (field_64_drop_transporter || (field_144 == 1) || field_144 == 2 || field_144 == 3))
        {
            Crane_15C::HookPickupCar_47EF80();
        }
        else
        {
            Crane_15C::ReleaseHookedCar_47FD10();
        }
    }
}

// 9.6f 0x448E30
MATCH_FUNC(0x47fe10)
void Crane_15C::UpdateCraneSprites_47FE10()
{
    Fix16_Point a4;
    Ang16 a2 = Ang16::Fix16_To_Ang16_40F540(field_8C_crane_angle);

    field_30->RemoveFromCollisionBuckets_527D00();
    field_34->RemoveFromCollisionBuckets_527D00();
    field_38->RemoveFromCollisionBuckets_527D00();
    field_3C->RemoveFromCollisionBuckets_527D00();
    field_40->RemoveFromCollisionBuckets_527D00();
    field_44->RemoveFromCollisionBuckets_527D00();
    field_48->RemoveFromCollisionBuckets_527D00();
    field_4C->RemoveFromCollisionBuckets_527D00();
    field_50->RemoveFromCollisionBuckets_527D00();
    field_54_hook_obj->RemoveFromCollisionBuckets_527D00();

    ComputeHookPos_47E730(a2, kFpHalf_679D34, &a4);
    field_30->field_4->set_ang_lazy_420690(a2);
    field_30->field_4->set_xy_lazy_447E20(a4.x, a4.y);

    field_40->field_4->set_ang_lazy_420690(a2);
    field_40->field_4->set_xy_lazy_447E20(a4.x, a4.y);

    ComputeHookPos_47E730(a2, kFpOneAndHalf_679D30, &a4);
    field_34->field_4->set_ang_lazy_420690(a2);
    field_34->field_4->set_xy_lazy_447E20(a4.x, a4.y);

    field_44->field_4->set_ang_lazy_420690(a2);
    field_44->field_4->set_xy_lazy_447E20(a4.x, a4.y);

    ComputeHookPos_47E730(a2, kFpTwoAndHalf_679D2C, &a4);
    field_38->field_4->set_ang_lazy_420690(a2);
    field_38->field_4->set_xy_lazy_447E20(a4.x, a4.y);

    field_48->field_4->set_ang_lazy_420690(a2);
    field_48->field_4->set_xy_lazy_447E20(a4.x, a4.y);

    ComputeHookPos_47E730(a2, kFpThreeAndHalf_679D28, &a4);
    field_3C->field_4->set_ang_lazy_420690(a2);
    field_3C->field_4->set_xy_lazy_447E20(a4.x, a4.y);

    field_4C->field_4->set_ang_lazy_420690(a2);
    field_4C->field_4->set_xy_lazy_447E20(a4.x, a4.y);

    field_50->field_4->set_ang_lazy_420690(a2);

    ComputeHookOffset_47E840(a2, &a4);
    field_5C_counterweight_obj->field_4->set_ang_lazy_420690(a2);
    field_5C_counterweight_obj->field_4->set_xy_lazy_447E20(a4.x, a4.y);

    ComputeHookPos_47E620(field_90_hook_radius, a2, &a4);
    field_50->field_4->set_xy_lazy_447E20(a4.x, a4.y);

    field_54_hook_obj->field_4->set_xyz_lazy_420600(a4.x, a4.y, field_80_ground_z - field_84_hook_depth);
    field_54_hook_obj->field_4->set_ang_lazy_420690(Ang16::Fix16_To_Ang16_40F540(field_A0_hook_axial_angle));

    if (field_74_pSprite_on_hook)
    {
        a4 += field_10_hooked_sprite_offset;
        field_74_pSprite_on_hook->set_xyz_lazy_inlined_420600(a4.x, a4.y, field_80_ground_z - field_84_hook_depth); // INLINED_MODE required
        field_74_pSprite_on_hook->set_ang_lazy_420690(Ang16::Fix16_To_Ang16_40F540(field_A0_hook_axial_angle));
    }

    field_30->AssignToBucket_527AE0();
    field_34->AssignToBucket_527AE0();
    field_38->AssignToBucket_527AE0();
    field_3C->AssignToBucket_527AE0();
    field_40->AssignToBucket_527AE0();
    field_44->AssignToBucket_527AE0();
    field_48->AssignToBucket_527AE0();
    field_4C->AssignToBucket_527AE0();
    field_50->AssignToBucket_527AE0();
    field_54_hook_obj->AssignToBucket_527AE0();
}

MATCH_FUNC(0x480310)
void Crane_15C::Service_480310()
{
    field_159_hooked_car_this_frame = 0;
    field_28_strct4.RemoveByRngValue_5A6C40(gpRng_67AB34->get_cur_rng_41CFE0() - 1);
    if (field_74_pSprite_on_hook)
    {
        gPurpleDoom_3_679210->Remove_477B00(field_74_pSprite_on_hook);
    }
    Crane_15C::UpdateCraneTargets_47F4C0();
    if (!field_148_disabled)
    {
        if (field_78_maybe_homecrane == NULL || !check_8c_a8_447f40() || field_78_maybe_homecrane->check_8c_a8_447f40())
        {
            Crane_15C::UpdateCraneTick_47FD50();
        }
    }

    if (field_74_pSprite_on_hook)
    {
        field_74_pSprite_on_hook->AsCar_40FEB0()->sub_443330();
    }

    if (field_74_pSprite_on_hook)
    {
        gPurpleDoom_3_679210->AddToSingleBucket_477AE0(field_74_pSprite_on_hook);
    }
}

MATCH_FUNC(0x4803b0)
void Crane_15C::InitCrane_4803B0(Fix16 x_pos, Fix16 y_pos, char_type a4)
{
    field_144 = 0;
    set_field_148_447F60(0);

    field_80_ground_z = gMap_0x370_6F6268->FindGroundZForCoord_4E5B60(x_pos, y_pos);
    field_2C_rotor_obj = gObject_5C_6F8F84->NewPhysicsObj_5299B0(objects::crane_rotor_135, x_pos, y_pos, field_80_ground_z, kAngZero_679FC4);
    field_30 = gObject_5C_6F8F84->NewPhysicsObj_5299B0(objects::crane_unknown_134, x_pos, y_pos, field_80_ground_z, kAngZero_679FC4);
    field_34 = gObject_5C_6F8F84->NewPhysicsObj_5299B0(objects::crane_unknown_134, x_pos, y_pos, field_80_ground_z, kAngZero_679FC4);
    field_38 = gObject_5C_6F8F84->NewPhysicsObj_5299B0(objects::crane_unknown_134, x_pos, y_pos, field_80_ground_z, kAngZero_679FC4);
    field_3C = gObject_5C_6F8F84->NewPhysicsObj_5299B0(objects::crane_unknown_134, x_pos, y_pos, field_80_ground_z, kAngZero_679FC4);
    field_40 = gObject_5C_6F8F84->NewPhysicsObj_5299B0(252, x_pos, y_pos, field_80_ground_z, kAngZero_679FC4);
    field_44 = gObject_5C_6F8F84->NewPhysicsObj_5299B0(260, x_pos, y_pos, field_80_ground_z, kAngZero_679FC4);
    field_48 = gObject_5C_6F8F84->NewPhysicsObj_5299B0(261, x_pos, y_pos, field_80_ground_z, kAngZero_679FC4);
    field_4C = gObject_5C_6F8F84->NewPhysicsObj_5299B0(262, x_pos, y_pos, field_80_ground_z, kAngZero_679FC4);
    field_50 = gObject_5C_6F8F84->NewPhysicsObj_5299B0(263, x_pos, y_pos, field_80_ground_z, kAngZero_679FC4);
    field_5C_counterweight_obj = gObject_5C_6F8F84->NewPhysicsObj_5299B0(objects::crane_counterweight_140, x_pos, y_pos, field_80_ground_z, kAngZero_679FC4);
    field_54_hook_obj = gObject_5C_6F8F84->NewPhysicsObj_5299B0(objects::crane_hook_136, x_pos, y_pos, field_80_ground_z, kAngZero_679FC4);

    field_58_crane_base_obj = gObject_5C_6F8F84->NewPhysicsObj_5299B0(objects::crane_base_137, x_pos, y_pos, field_80_ground_z - kFpTwo_679C78, kAngZero_679FC4);
    field_58_crane_base_obj->set_field_26(a4);
    field_78_maybe_homecrane = 0;
    field_94_hook_radius_speed = kZero_679E70;
    field_98_crane_angle_speed = kZero_679E70;
    field_9C = kZero_679E70;
    field_A8_home_angle = kZero_679E70;
    field_8C_crane_angle = kZero_679E70;
    field_90_hook_radius = kHomeHookRadius_679E58;
    field_84_hook_depth = kZero_679E70;
    field_88_hook_depth_speed = kZero_679E70;
    field_A0_hook_axial_angle = kZero_679E70;
    field_A4_hook_axial_angle_speed = kZero_679E70;
    field_AC_crane_angle_target = field_8C_crane_angle;
    field_B0_hook_radius_target = field_90_hook_radius;
    field_14D_is_busy = 0;
    field_0_hooked_sprite_offset_target.x = 0;
    field_0_hooked_sprite_offset_target.y = 0;
    field_B4_hook_angle_target = field_8C_crane_angle;
    field_B8_hook_depth_target = kZero_679E70;
    field_68_pickup_car = 0;
    field_E0_pickup_car_x = kZero_679E70;
    field_E4_pickup_car_y = kZero_679E70;
    field_E8_pickup_car_z = kZero_679E70;
    field_EC_pickup_car_rot = kZero_679E70;
    field_F0_pickup_car_hook_depth = kZero_679E70;
    field_6C_transporter_cargo = 0;
    field_70_cargo_transporter = 0;
    field_FC_transporter_cargo_x = kZero_679E70;
    field_100_transporter_cargo_y = kZero_679E70;
    field_104_transporter_cargo_z = kZero_679E70;
    field_108_transporter_cargo_rot = kZero_679E70;
    field_10C_transporter_cargo_hook_depth = kZero_679E70;
    field_74_pSprite_on_hook = 0;
    field_154 = 0;
    field_150 = 0;
    field_114_drop_radius = kZero_679E70;
    field_110_drop_angle = kZero_679E70;
    field_118_drop_rot = kZero_679E70;
    field_8_drop_offset.x = 0;
    field_8_drop_offset.y = 0;
    field_11C_drop_hook_depth = kZero_679E70;

    Sprite* current_sprite = gSprite_Pool_703818->get_new_sprite();
    field_60_probe_sprite = current_sprite;
    current_sprite->AllocInternal_59F950(kZero_679E70, kZero_679E70, kZero_679E70);
    field_14C_return_delay = 60;
    field_BC_drop_transporter_radius = kZero_679E70;
    field_C0_drop_transporter_angle = kZero_679E70;
    field_64_drop_transporter = 0;
    field_D0_drop_transporter_rot = kZero_679E70;
    field_C4_drop_transporter_pos.x = kZero_679E70;
    field_C4_drop_transporter_pos.y = kZero_679E70;
    field_CC_drop_transporter_z = kZero_679E70;
    field_D4_drop_transporter_hook_depth = kZero_679E70;
    field_120_target1_radius = kZero_679E70;
    field_124_target1_angle = kZero_679E70;
    field_128_target1_rot = kZero_679E70;
    field_18_target1_offset.x = 0;
    field_18_target1_offset.y = 0;
    field_12C_target1_hook_depth = kZero_679E70;
    field_130_target2_radius = kZero_679E70;
    field_134_target2_angle = kZero_679E70;
    field_138_target2_rot = kZero_679E70;
    field_20_target2_offset.x = 0;
    field_20_target2_offset.y = 0;
    field_13C_target2_hook_depth = kZero_679E70;
    field_155 = 1;
    Crane_15C::UpdateCraneSprites_47FE10();
    field_156_is_rotating = 0;
    field_157_is_radius_changing = 0;
    field_158_is_hook_depth_changing = 0;
    field_159_hooked_car_this_frame = 0;
    field_140_powerup_cmd = 0;
    if (!field_7C_sound && !bSkip_audio_67D6BE)
    {
        field_7C_sound = gRoot_sound_66B038.CreateSoundObject_40EF40(this, SoundObjectTypeEnum::Crane_15C_8);
    }
    field_28_strct4.ResetHead_4207E0();
}

MATCH_FUNC(0x480900)
void Crane_15C::CraneTargetPickupCheck_480900(Fix16 xpos, Fix16 ypos, Ang16 ang)
{
    Fix16_Point v10(xpos, ypos);
    Fix16_Point t;
    ComputeHookPolar_47F6C0(v10, &field_120_target1_radius, &field_124_target1_angle);

    field_128_target1_rot = Ang16::Ang16_to_Fix16(ang);

    if (field_144 == 3)
    {
        field_144 = 2;
        field_155 = 1;
    }
    else
    {
        field_144 = 1;
    }

    ComputeHookPos_47E620(field_120_target1_radius, Ang16::Fix16_To_Ang16_40F540(field_124_target1_angle), &t);
    field_18_target1_offset = v10 - t;

    field_12C_target1_hook_depth = field_80_ground_z - gMap_0x370_6F6268->FindGroundZForCoord_4E5B60(xpos, ypos);
}

// 9.6f 0x4496E0
MATCH_FUNC(0x480b60)
void Crane_15C::ComputePickupAlignment_480B60(Fix16 xpos, Fix16 ypos, Ang16 ang)
{
    Fix16_Point v10(xpos, ypos);
    Fix16_Point t;
    ComputeHookPolar_47F6C0(v10, &field_130_target2_radius, &field_134_target2_angle);

    field_138_target2_rot = Ang16::Ang16_to_Fix16(ang);

    if (field_144 == 1)
    {
        field_144 = 2;
    }
    else
    {
        field_144 = 3;
        field_155 = 2;
    }

    ComputeHookPos_47E620(field_130_target2_radius, Ang16::Fix16_To_Ang16_40F540(field_134_target2_angle), &t);
    field_20_target2_offset = v10 - t;

    field_13C_target2_hook_depth = field_80_ground_z - gMap_0x370_6F6268->FindGroundZForCoord_4E5B60(xpos, ypos);
}

MATCH_FUNC(0x480da0)
Car_BC* Crane_15C::GetCarFromCrane_480DA0()
{
    if (field_74_pSprite_on_hook)
    {
        return field_74_pSprite_on_hook->AsCar_40FEB0();
    }
    return 0;
}

MATCH_FUNC(0x480e00)
void CranePool_D9C::PickUpCar_480E00(Car_BC* a2, u8 a3)
{
    field_0_cranes[a3].PickUpCar_47F930(a2);
}

MATCH_FUNC(0x480e50)
void CranePool_D9C::CranesService_480E50()
{
    s32 i = 0;
    Crane_15C* pIter = field_0_cranes;
    while (i < field_D98_count)
    {
        pIter->Service_480310();
        i++;
        pIter++;
    }
}

MATCH_FUNC(0x480ec0)
Crane_15C* CranePool_D9C::NewCrane_480EC0(Fix16 x_pos, Fix16 y_pos)
{
    Crane_15C* pNewCrane = &field_0_cranes[field_D98_count];
    pNewCrane->InitCrane_4803B0(x_pos, y_pos, field_D98_count);
    field_D98_count++;
    return pNewCrane;
}

MATCH_FUNC(0x480f50)
CranePool_D9C::CranePool_D9C()
{
    field_D98_count = 0;
}

MATCH_FUNC(0x4bbbf0)
CranePool_D9C::~CranePool_D9C()
{
}