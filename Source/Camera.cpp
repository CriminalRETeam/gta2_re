#define FIX16_POINT_ZERO kZero_676818
#include "Camera.hpp"
#include "Car.hpp"
#include "CarPhysics.hpp"
#include "Function.hpp"
#include "Game.hpp"
#include "Globals.hpp"
#include "CarChaseTaskTable.hpp"
#include "Ped.hpp"
#include "Police.hpp"
#include "sprite.hpp"

// Forward declarations: the functions below are in address order
EXPORT void __stdcall SmoothApproach_4F7540(Fix16& Coord_1, Fix16& Velocity_1, Fix16& Coord_2, Fix16& Velocity_2, Fix16& Velocity_3);
EXPORT void __stdcall SmoothApproachClamped_4F75D0(Fix16* target_coord,
                                                   Fix16* coord_velocity,
                                                   Fix16* curr_coord,
                                                   Fix16* velocity_1,
                                                   Fix16* velocity_2,
                                                   Fix16* velocity_3,
                                                   Fix16* max_neg_velocity);

DEFINE_GLOBAL(Camera*, gViewCamera_676978, 0x676978);
DEFINE_GLOBAL_INIT(Fix16, kFpEight_676840, Fix16(8), 0x676840);
DEFINE_GLOBAL_INIT(Fix16, kFpHalf_67671C, Fix16(0x2000, 0), 0x67671C);
DEFINE_GLOBAL_INIT(Fix16, kZero_676818, Fix16(0), 0x676818);
DEFINE_GLOBAL_INIT(Fix16, kOne_67681C, Fix16(1), 0x67681C);
DEFINE_GLOBAL_INIT(Fix16, kDefaultZoom_6766D4, Fix16(0x38CC, 0), 0x6766D4);
DEFINE_GLOBAL_INIT(Fix16, kFpFive_67682C, Fix16(5), 0x67682C);
DEFINE_GLOBAL_INIT(Fix16, kInitialPanSpeed_6766E4, kFpFive_67682C, 0x6766E4);
DEFINE_GLOBAL_INIT(Fix16, kMaxPanY_6768F0, Fix16(220), 0x6768F0);
DEFINE_GLOBAL_INIT(Fix16, kPanAcceleration_676910, kFpFive_67682C, 0x676910);
DEFINE_GLOBAL_INIT(Fix16, kMaxTgtElevation_676894, Fix16(20), 0x676894);
DEFINE_GLOBAL_INIT(Fix16, kMaxPanSpeed_676608, kMaxTgtElevation_676894, 0x676608);
DEFINE_GLOBAL_INIT(Fix16, kMaxPanX_6768C0, Fix16(300), 0x6768C0);
DEFINE_GLOBAL_INIT(Fix16, kFpOne64th_6767D0, Fix16(256, 0), 0x6767D0);
DEFINE_GLOBAL_INIT(Fix16, kFpPoint1_676664, Fix16(1638, 0), 0x676664);
DEFINE_GLOBAL_INIT(Fix16, kFpHalf_676678, Fix16(0x2000, 0), 0x676678);
DEFINE_GLOBAL_INIT(Fix16, kFpOne64th_6768D8, kFpOne64th_6767D0, 0x6768D8);
DEFINE_GLOBAL_INIT(Fix16, kFpThreeSixteenths_676918, kFpOne64th_6768D8 * 12, 0x676918);
DEFINE_GLOBAL_INIT(Fix16, kFpOne128th_6767B8, kFpOne64th_6768D8* kFpHalf_676678, 0x6767B8);
DEFINE_GLOBAL_INIT(Fix16, kFpFiveEighths_676638, kFpOne64th_6768D8 * 40, 0x676638);
DEFINE_GLOBAL_INIT(Fix16, dword_676834, kFpOne64th_6768D8* kFpPoint1_676664, 0x676834);
DEFINE_GLOBAL_INIT(Fix16, kFpFiveSixtyFourths_6765FC, kFpOne64th_6768D8 * 5, 0x6765FC);
DEFINE_GLOBAL_INIT(Fix16, kFpOne64th_6766FC, kFpOne64th_6768D8, 0x6766FC);
DEFINE_GLOBAL_INIT(Fix16, kFpOneSixteenth_6766A4, kFpOne64th_6768D8 * 4, 0x6766A4);
DEFINE_GLOBAL_INIT(Fix16, kFpOne64th_676740, kFpOne64th_6768D8* kOne_67681C, 0x676740);
DEFINE_GLOBAL_INIT(Fix16, kFpSeven_676838, Fix16(7), 0x676838);
DEFINE_GLOBAL_INIT(Fix16, kMaxMapCoord_67668C, Fix16(0x3FFFFF, 0), 0x67668C);
DEFINE_GLOBAL_INIT(Fix16, kFpThreeQuarters_6768E0, Fix16(0x3000, 0), 0x6768E0);
DEFINE_GLOBAL_INIT(Fix16, kFpThreeQuarters_67691C, kFpThreeQuarters_6768E0, 0x67691C);
DEFINE_GLOBAL_INIT(Fix16, kFpThreeQuarters_6766F4, Fix16(0x3000, 0), 0x6766F4);
DEFINE_GLOBAL_INIT(Fix16, kTwo_676820, Fix16(2), 0x676820);
DEFINE_GLOBAL_INIT(Fix16, kFpThreePoint55_6767B4, Fix16(0xE333, 0), 0x6767B4);
DEFINE_GLOBAL_INIT(Fix16, kMaxCamZ_676898, Fix16(14), 0x676898);
DEFINE_GLOBAL_INIT(Fix16, kZero_6F6C50, Fix16(0), 0x6F6C50);

DEFINE_GLOBAL_INIT(Fix16, kFpEight_702E04, Fix16(8), 0x702E04);
DEFINE_GLOBAL_INIT(Fix16, kFpOne_702DE4, Fix16(1), 0x702DE4);

DEFINE_GLOBAL_INIT(Fix16, kFpPoint2_67666C, Fix16(0xCCC, 0), 0x67666C);
DEFINE_GLOBAL_INIT(Fix16, kFpPoint2_676694, kFpPoint2_67666C, 0x676694);
DEFINE_GLOBAL_INIT(Fix16, kFpPoint8_676684, Fix16(0x3333, 0), 0x676684);

DEFINE_GLOBAL_INIT(Fix16, kFpSixteen_6768B4, Fix16(16), 0x6768B4);
DEFINE_GLOBAL_INIT(Fix16, kFpSixtyFour_6768E4, Fix16(64), 0x6768E4);
DEFINE_GLOBAL_INIT(Fix16, kFpHalf_676900, kFpHalf_676678, 0x676900);
DEFINE_GLOBAL_INIT(Fix16, kFpSixteen_67696C, kFpSixteen_6768B4, 0x67696C);
DEFINE_GLOBAL_INIT(Fix16, kFpOneAndHalf_67674C, kOne_67681C + kFpHalf_676678, 0x67674C);

DEFINE_GLOBAL_INIT(Ang16, kAng45_6766DC, Ang16(0x00B4, 0), 0x6766DC);
DEFINE_GLOBAL_INIT(Ang16, kAng135_676790, Ang16(0x021C, 0), 0x676790);
DEFINE_GLOBAL_INIT(Ang16, kAng225_676764, Ang16(0x0384, 0), 0x676764);
DEFINE_GLOBAL_INIT(Ang16, kAng315_67679C, Ang16(0x04EC, 0), 0x67679C);
// Defined in sprite.cpp: when this TU sees the definition, VC6 reads the s16 with a 32-bit mov and adds with
// lea; the original's 16-bit `mov 0x676772,%cx; add %dx,%cx` needs an extern (ComputeTargetFacingAngle_4358D0)
EXTERN_GLOBAL(Ang16, kAng180_676772);
DEFINE_GLOBAL_INIT(Ang16, kAngZero_676964, Ang16(0), 0x676964);


// TODO: move
static inline Fix16 Max_41E130(Fix16 a, Fix16 b)
{
    if (a > b)
    {
        return a;
    }
    return b;
}

MATCH_FUNC(0x4355D0)
bool Camera::IsSpriteTheCameraSubject_4355D0(Sprite* pSprite)
{
    Car* pCar = pSprite->AsCar_40FEB0();
    if (pCar)
    {
        if (field_38_target_car == pCar)
        {
            return true;
        }
        if (field_34_target_ped && field_34_target_ped->get_car_416B60() == pCar)
        {
            return true;
        }
    }
    else
    {
        Char* pB4 = pSprite->AsCharB4_40FEA0();
        if (pB4)
        {
            if (field_34_target_ped)
            {
                if (field_34_target_ped->field_168_game_object == pB4)
                {
                    return true;
                }
            }
        }
    }
    return false;
}

MATCH_FUNC(0x435630)
char_type Camera::IsSpriteInView_435630(Sprite* pSprite, s32 wide_view)
{
    Fix16 half_width;
    half_width = field_98_cur_pos.field_8_z - pSprite->field_1C_zpos + kFpEight_676840;
    half_width = half_width / (field_98_cur_pos.field_C_zoom * kTwo_676820);

    if (wide_view == 1)
    {
        half_width = (half_width * kFpPoint8_676684);
    }

    Fix16 half_height = (half_width * kFpThreeQuarters_6766F4);

    Fix16_Rect rect;
    rect.SetHiLowZ_41E370(pSprite->field_1C_zpos, pSprite->field_1C_zpos);
    rect.SetRect_41E350(field_98_cur_pos.field_0_x - half_width,
                        field_98_cur_pos.field_0_x + half_width,
                        field_98_cur_pos.field_4_y - half_height,
                        field_98_cur_pos.field_4_y + half_height);

    Fix16_Rect* pBox = &pSprite->field_C_pCollisionShape->field_30_boundingBox;
    if (rect.field_0_left.IntervalIntersectsRange_438FB0_inline(rect.field_4_right, pBox->field_0_left, pBox->field_4_right) &&
        IntervalIntersectsRange_438FB0(rect.field_8_top, rect.field_C_bottom, pBox->field_8_top, pBox->field_C_bottom) &&
        IntervalIntersectsRange_438FB0(rect.field_10_low_z, rect.field_14_high_z, pBox->field_10_low_z, pBox->field_14_high_z))
    {
        SpriteShape* p4C = pSprite->field_C_pCollisionShape;
        if ((p4C->field_0_width == p4C->field_4_height && p4C->field_0_width <= kFpPoint2_676694) || pSprite->field_0_rotation.rValue == 0 ||
            pSprite->field_0_rotation == 360 || pSprite->field_0_rotation == 720 || pSprite->field_0_rotation == 1080 || pSprite->IntersectsRectSAT_59FB10(&rect) ||
            rect.IntersectsSpriteRenderingRect_59DDF0(pSprite))
        {
            return 1;
        }
    }
    return 0;
}

MATCH_FUNC(0x4357B0)
void Camera::SavePrevCamPos_4357B0()
{
    field_88_prev_pos.field_0_x = field_98_cur_pos.field_0_x;
    field_88_prev_pos.field_4_y = field_98_cur_pos.field_4_y;
    field_88_prev_pos.field_8_z = field_98_cur_pos.field_8_z;
    field_88_prev_pos.field_C_zoom = field_98_cur_pos.field_C_zoom;
}

MATCH_FUNC(0x4357F0)
void Camera::IncreaseElevation_4357F0()
{
    if (field_40_target_elevation < kMaxTgtElevation_676894)
    {
        field_40_target_elevation += kFpHalf_676678;
    }
}

MATCH_FUNC(0x435810)
void Camera::DecreaseElevation_435810()
{
    if (field_40_target_elevation > kZero_676818)
    {
        field_40_target_elevation -= kFpHalf_676678;
    }
}

MATCH_FUNC(0x435830)
void Camera::ResetElevation_435830()
{
    field_40_target_elevation = kZero_676818;
}

MATCH_FUNC(0x435840)
void Camera::ClampTargetZ_435840()
{
    if (field_10_pending_target_pos.field_8_z < kZero_676818)
    {
        field_10_pending_target_pos.field_8_z = kZero_676818;
    }

    if (field_10_pending_target_pos.field_8_z > kMaxCamZ_676898)
    {
        field_10_pending_target_pos.field_8_z = kMaxCamZ_676898;
    }
}

MATCH_FUNC(0x435860)
void Camera::ApplyMovementDeltaFrom_435860(Camera* pCamera)
{
    field_10_pending_target_pos.field_0_x += pCamera->field_98_cur_pos.field_0_x - pCamera->field_88_prev_pos.field_0_x;
    field_10_pending_target_pos.field_4_y += pCamera->field_98_cur_pos.field_4_y - pCamera->field_88_prev_pos.field_4_y;
    field_10_pending_target_pos.field_8_z += pCamera->field_98_cur_pos.field_8_z - pCamera->field_88_prev_pos.field_8_z;
    field_10_pending_target_pos.field_C_zoom += pCamera->field_98_cur_pos.field_C_zoom - pCamera->field_88_prev_pos.field_C_zoom;
    ClampTargetZ_435840();
}

// matches on decompme: https://decomp.me/scratch/NpBvl
MATCH_FUNC(0x4358D0)
Ang16 Camera::ComputeTargetFacingAngle_4358D0()
{
    Ang16 CarRotation;
    Ang16 PedRotation;

    if (field_34_target_ped)
    {
        Car* field_16C_car = field_34_target_ped->get_car_416B60();
        if (field_16C_car)
        {
            CarRotation = field_16C_car->GetCarRotation_416BB0();
            CarPhysics* pCarPhysics = field_16C_car->field_58_physics;

            if (pCarPhysics && pCarPhysics->is_backward_gas_on_411810())
            {
                return CarRotation + kAng180_676772;
            }
            return CarRotation;
        }
        PedRotation = field_34_target_ped->GetRotation();
        Char* field_168_game_object = field_34_target_ped->field_168_game_object;
        if (field_168_game_object && field_168_game_object->field_58_flags_bf.bReverseVelocity)
        {
            return PedRotation + kAng180_676772;
        }
        else
        {
            return PedRotation;
        }
    }
    else
    {
        if (field_38_target_car)
        {
            CarRotation = field_38_target_car->GetCarRotation_416BB0();
            CarPhysics* pCarPhysics = field_38_target_car->field_58_physics;

            if (pCarPhysics && pCarPhysics->is_backward_gas_on_411810())
            {
                return CarRotation + kAng180_676772;
            }
            return CarRotation;
        }
        else
        {
            return kAngZero_676964;
        }
    }
}

MATCH_FUNC(0x435A20)
Fix16 Camera::ReturnOwnerVelocity_435A20()
{
    Ped* pPed = field_34_target_ped;
    if (pPed)
    {
        return pPed->GetPedVelocity_45C920();
    }

    Car* pCar = field_38_target_car;
    if (pCar)
    {
        return pCar->GetCarLinearSpeed_43A240();
    }
    else
    {
        return kZero_676818;
    }
}

// Defined ahead of IsCoordsPosVisible_435A70 (out of address order): that function needs the out-of-line
// Fix16::operator* copy, and once C2 has compiled the copy it knows the call can't throw, so the inline
// FromPolar_41E210 here would drop offset.x's store before its second multiply. The original keeps it, as if
// the copy had not been compiled yet (the same effect as Fix16_Point::operator+ in Weapon, see
// docs/matching_quirks.md).
MATCH_FUNC(0x436200)
void Camera::ApplyCarVelocityCameraOffset_436200(Car* pCar, Fix16* pOutX, Fix16* pOutY, Fix16* pOutZ)
{
    // 9.6f 0x41EBF0: this source matches it exactly when built with VC7 (compile_vc7.sh, permuter_score.py --96f)
    Ang16 angle;
    Fix16_Point vel;
    Fix16_Point offset;
    Fix16 radius;
    Fix16 speed;

    if (pCar->IsTrainModel_403BA0())
    {
        speed = kFpHalf_676900 * kFpSixteen_67696C;
    }
    else
    {
        vel = pCar->GetLinearVelocity_43A450() * kFpSixteen_67696C;
        speed = vel.GetLength_41E260();
    }

    if (speed > kFpOneAndHalf_67674C)
    {
        *pOutZ += speed;

        if (!pCar->IsTrainModel_403BA0() && !pCar->IsTank_411900())
        {
            angle = vel.atan2_40F790();

            Fix16 dist;
            if ((angle > kAng45_6766DC && angle < kAng135_676790) || (angle > kAng225_676764 && angle < kAng315_67679C))
            {
                dist = Fix16(240);
            }
            else
            {
                dist = Fix16(180);
            }

            if (pCar->IsTrailerCab_41E460())
            {
                dist *= kFpThreeQuarters_6768E0;
            }

            if (this->field_44_suspicion)
            {
                Fix16 suspicion;
                if (this->field_44_suspicion > 64u)
                {
                    suspicion = kFpSixtyFour_6768E4;
                }
                else
                {
                    suspicion = Fix16(this->field_44_suspicion);
                }
                dist *= kOne_67681C - suspicion / 128;
            }

            radius = dist * (Fix16(8) - pCar->field_50_car_sprite->field_1C_zpos + *pOutZ) / field_60_screen_scale.y;
            offset.FromPolar_41E210(radius, angle);
            *pOutX += offset.x;
            *pOutY += offset.y;
        }
    }
}

MATCH_FUNC(0x435A70)
s32 Camera::IsCoordsPosVisible_435A70(Fix16 x, Fix16 y, Fix16 z)
{
    Fix16_Point_POD pos = WorldToScreen_40CFC0(x, y, z);
    if (pos.x >= kZero_676818 && pos.x < Fix16(640) && pos.y >= kZero_676818 && pos.y < Fix16(480))
    {
        return 1;
    }
    return 0;
}

// https://decomp.me/scratch/YoPmg Is field_60_screen_scale really a Fix16_Point ?
MATCH_FUNC(0x435B90)
void Camera::UpdateBoundaries_435B90()
{
    field_60_screen_scale.x = Fix16(field_68_screen_px_width) * field_98_cur_pos.field_C_zoom;
    field_60_screen_scale.y = Fix16(640) * field_98_cur_pos.field_C_zoom;

    Fix16 v = (kFpSeven_676838 + field_98_cur_pos.field_8_z) * (kOne_67681C / field_98_cur_pos.field_C_zoom) * kFpHalf_67671C;

    field_78_view_bounds.field_0_left = field_98_cur_pos.field_0_x - v;
    if (field_78_view_bounds.field_0_left < kZero_676818)
    {
        field_78_view_bounds.field_0_left = Fix16(0);
    }
    else if (field_78_view_bounds.field_0_left > kMaxMapCoord_67668C)
    {
        field_78_view_bounds.field_0_left = kMaxMapCoord_67668C;
    }

    field_78_view_bounds.field_4_right = field_98_cur_pos.field_0_x + v;
    if (field_78_view_bounds.field_4_right < kZero_676818)
    {
        field_78_view_bounds.field_4_right = Fix16(0);
    }
    else if (field_78_view_bounds.field_4_right > kMaxMapCoord_67668C)
    {
        field_78_view_bounds.field_4_right = kMaxMapCoord_67668C;
    }

    v *= kFpThreeQuarters_6768E0;

    field_78_view_bounds.field_8_top = field_98_cur_pos.field_4_y - v;
    if (field_78_view_bounds.field_8_top < kZero_676818)
    {
        field_78_view_bounds.field_8_top = Fix16(0);
    }
    else if (field_78_view_bounds.field_8_top > kMaxMapCoord_67668C)
    {
        field_78_view_bounds.field_8_top = kMaxMapCoord_67668C;
    }

    field_78_view_bounds.field_C_bottom = field_98_cur_pos.field_4_y + v;
    if (field_78_view_bounds.field_C_bottom < kZero_676818)
    {
        field_78_view_bounds.field_C_bottom = Fix16(0);
    }
    else if (field_78_view_bounds.field_C_bottom > kMaxMapCoord_67668C)
    {
        field_78_view_bounds.field_C_bottom = kMaxMapCoord_67668C;
    }

    field_20_padded_view_bounds.field_0_left = field_78_view_bounds.field_0_left - kFpThreeQuarters_67691C;
    field_20_padded_view_bounds.field_4_right = field_78_view_bounds.field_4_right + kFpThreeQuarters_67691C;
    field_20_padded_view_bounds.field_8_top = field_78_view_bounds.field_8_top - kFpThreeQuarters_67691C;
    field_20_padded_view_bounds.field_C_bottom = field_78_view_bounds.field_C_bottom + kFpThreeQuarters_67691C;
}

MATCH_FUNC(0x435D20)
void Camera::MoveTarget_435D20(char_type move_north, char_type move_south, char_type move_west, char_type move_east, char_type zoom_in, char_type zoom_out)
{
    ResetPendingTarget();
    if (move_north)
    {
        field_10_pending_target_pos.field_4_y -= kFpHalf_67671C;
    }

    if (move_south)
    {
        field_10_pending_target_pos.field_4_y += kFpHalf_67671C;
    }

    if (move_west)
    {
        field_10_pending_target_pos.field_0_x -= kFpHalf_67671C;
    }

    if (move_east)
    {
        field_10_pending_target_pos.field_0_x += kFpHalf_67671C;
    }

    if (zoom_in)
    {
        field_10_pending_target_pos.field_8_z += kOne_67681C;
    }

    if (zoom_out)
    {
        field_10_pending_target_pos.field_8_z -= kOne_67681C;
    }
    ClampTargetZ_435840();
}

MATCH_FUNC(0x435DD0)
void Camera::ResetCameraSmoothing_435DD0()
{
    field_98_cur_pos.field_0_x = field_0_target_pos.field_0_x;
    field_98_cur_pos.field_4_y = field_0_target_pos.field_4_y;
    field_98_cur_pos.field_8_z = field_0_target_pos.field_8_z;
    field_98_cur_pos.field_C_zoom = field_0_target_pos.field_C_zoom;

    field_AC_cam_velocity.field_0_x = kZero_676818;
    field_AC_cam_velocity.field_4_y = kZero_676818;
    field_AC_cam_velocity.field_8_z = kZero_676818;
    field_AC_cam_velocity.field_C_zoom = kZero_676818;
}

MATCH_FUNC(0x435F90)
void Camera::AccumulateSuspicionOnDriver_435F90(Car* pCar)
{
    if (pCar->field_54_driver &&
        (gPolice_7B8_6FEE40->IsActivelyChased_56F880(pCar->field_54_driver) ||
         gCarChaseTaskTable_678E30->HasAnyPursuer_474970(pCar->field_54_driver)))
    {
        field_44_suspicion++;
        if (field_44_suspicion > 80u)
        {
            field_44_suspicion = 80;
        }
    }
    else
    {
        if (field_44_suspicion > 0)
        {
            field_44_suspicion--;
        }
    }
}

MATCH_FUNC(0x435FF0)
void Camera::Update_435FF0()
{
    Camera::SavePrevCamPos_4357B0();
    Fix16 scale = field_98_cur_pos.field_8_z * kFpThreeSixteenths_676918;

    switch (field_3C_follow_mode)
    {
        case camera_follow_mode::smooth_1:
            SmoothApproach_4F7540(field_0_target_pos.field_0_x,
                                  field_AC_cam_velocity.field_0_x,
                                  field_98_cur_pos.field_0_x,
                                  kFpOne64th_676740,
                                  scale);
            SmoothApproach_4F7540(field_0_target_pos.field_4_y,
                                  field_AC_cam_velocity.field_4_y,
                                  field_98_cur_pos.field_4_y,
                                  kFpOne64th_676740,
                                  scale);
            SmoothApproachClamped_4F75D0(&field_0_target_pos.field_8_z,
                                         &field_AC_cam_velocity.field_8_z,
                                         &field_98_cur_pos.field_8_z,
                                         &kFpOne128th_6767B8,
                                         &kFpFiveEighths_676638,
                                         &dword_676834,
                                         &kFpFiveSixtyFourths_6765FC);
            SmoothApproach_4F7540(field_0_target_pos.field_C_zoom,
                                  field_AC_cam_velocity.field_C_zoom,
                                  field_98_cur_pos.field_C_zoom,
                                  kFpOne64th_6766FC,
                                  kFpOneSixteenth_6766A4);
            break;
        case camera_follow_mode::snap_2:
            field_98_cur_pos.field_0_x = field_0_target_pos.field_0_x;
            field_98_cur_pos.field_4_y = field_0_target_pos.field_4_y;
            field_98_cur_pos.field_8_z = field_0_target_pos.field_8_z;
            field_98_cur_pos.field_C_zoom = field_0_target_pos.field_C_zoom;
            break;
    }
    if (field_30_shake_amount != kZero_676818)
    {
        Camera::ApplyShake_436140();
    }
    Camera::UpdateBoundaries_435B90();
    field_0_target_pos = field_10_pending_target_pos;
}

MATCH_FUNC(0x436110)
void Camera::RefreshBoundaries_436110()
{
    UpdateBoundaries_435B90();
}

MATCH_FUNC(0x436120)
void Camera::SetShake_436120(Fix16 amount)
{
    field_30_shake_amount = amount * kFpOne64th_6768D8;
}

MATCH_FUNC(0x436140)
void Camera::ApplyShake_436140()
{
    field_98_cur_pos.field_0_x += field_30_shake_amount;
    field_98_cur_pos.field_4_y += field_30_shake_amount;
    field_98_cur_pos.field_8_z += field_30_shake_amount;
    if (field_98_cur_pos.field_8_z < kZero_676818)
    {
        field_98_cur_pos.field_8_z = kZero_676818;
    }
    field_30_shake_amount = -field_30_shake_amount;
    field_30_shake_amount = kFpThreeQuarters_6766F4 * field_30_shake_amount;
}

MATCH_FUNC(0x4361B0)
void Camera::SetScreenSize_4361B0(u32 width, u32 height)
{
    field_68_screen_px_width = width;
    field_6C_screen_px_height = height;

    field_70_screen_px_center_x = width / 2;
    field_74_screen_px_center_y = height / 2;

    field_60_screen_scale.x = Fix16(-1);
    field_60_screen_scale.y = Fix16(-1);

    field_A8_ui_scale = Fix16(width) / 640;
}


MATCH_FUNC(0x4364A0)
void Camera::UpdateFollowCarCamera_4364A0(Car* pCar)
{

    this->field_34_target_ped = 0;
    this->field_38_target_car = pCar;

    if (pCar)
    {
        Fix16 new_x;
        new_x = pCar->GetX_41E430();
        Fix16 new_y;
        new_y = pCar->GetY_41E440();
        Fix16 new_z;
        new_z = Max_41E130(pCar->GetZ_41E450() - kTwo_676820, kOne_67681C);
        Fix16 zoom = kDefaultZoom_6766D4;

        AccumulateSuspicionOnDriver_435F90(pCar);
        ApplyCarVelocityCameraOffset_436200(pCar, &new_x, &new_y, &new_z);
        SetPendingTarget_41E3D0(new_x, new_y, new_z, zoom);
    }
}

MATCH_FUNC(0x436540)
void Camera::UpdateFollowPedCamera_436540(Ped* pPed)
{
    Car* pCar_2;
    Fix16 zposToUse;

    field_38_target_car = NULL;
    field_34_target_ped = pPed;
    if (pPed != NULL)
    {
        Fix16 xpos;
        Fix16 ypos;
        Fix16 zpos;
        Car* pCar = pPed->get_car_416B60();
        if (pCar || (pCar_2 = pPed->GetCarBeingEnteredOrExited_45BBF0(), pCar_2 == 0))
        {
            xpos = pPed->get_cam_x();
            ypos = pPed->get_cam_y();
            zpos = Max_41E130(pPed->get_cam_z() - kTwo_676820, kOne_67681C);
            zposToUse = zpos;
            if (pCar)
            {
                Camera::AccumulateSuspicionOnDriver_435F90(pCar);
                Camera::ApplyCarVelocityCameraOffset_436200(pCar, &xpos, &ypos, &zposToUse);
            }
        }
        else
        {
            xpos = pCar_2->GetX_41E430();
            ypos = pCar_2->GetY_41E440();
            zpos = Max_41E130(pCar_2->GetZ_41E450() - kTwo_676820, kOne_67681C);
            zposToUse = zpos;
            Camera::AccumulateSuspicionOnDriver_435F90(pCar_2);
            Camera::ApplyCarVelocityCameraOffset_436200(pCar_2, &xpos, &ypos, &zposToUse);
        }
        Fix16 zoom = kFpThreePoint55_6767B4;
        if (pPed->GetPedState_403990() != ped_state_1::dead_9)
        {
            zoom = kDefaultZoom_6766D4;
        }
        Camera::ApplyPanOffset_436860(pPed, xpos, ypos, zposToUse);
        SetPendingTarget_41E3D0(xpos, ypos, zposToUse, zoom);
    }
}

MATCH_FUNC(0x436710)
void Camera::HandlePanning_436710(char_type forward_gas_on, char_type foot_brake_on, char_type left_on, char_type right_on)
{
    if (forward_gas_on)
    {
        field_4C_pan_y -= field_50_pan_speed_up;
        if (field_4C_pan_y < -kMaxPanY_6768F0)
        {
            field_4C_pan_y = -kMaxPanY_6768F0;
        }

        field_50_pan_speed_up += kPanAcceleration_676910;
        if (field_50_pan_speed_up > kMaxPanSpeed_676608)
        {
            field_50_pan_speed_up = kMaxPanSpeed_676608;
        }
    }
    else
    {
        field_50_pan_speed_up = kInitialPanSpeed_6766E4;
    }

    if (foot_brake_on)
    {
        field_4C_pan_y += field_54_pan_speed_down;
        if (field_4C_pan_y > kMaxPanY_6768F0)
        {
            field_4C_pan_y = kMaxPanY_6768F0;
        }

        field_54_pan_speed_down += kPanAcceleration_676910;
        if (field_54_pan_speed_down > kMaxPanSpeed_676608)
        {
            field_54_pan_speed_down = kMaxPanSpeed_676608;
        }
    }
    else
    {
        field_54_pan_speed_down = kInitialPanSpeed_6766E4;
    }

    if (left_on)
    {
        field_48_pan_x -= field_58_pan_speed_left;
        if (field_48_pan_x < -kMaxPanX_6768C0)
        {
            field_48_pan_x = -kMaxPanX_6768C0;
        }

        field_58_pan_speed_left += kPanAcceleration_676910;
        if (field_58_pan_speed_left > kMaxPanSpeed_676608)
        {
            field_58_pan_speed_left = kMaxPanSpeed_676608;
        }
    }
    else
    {
        field_58_pan_speed_left = kInitialPanSpeed_6766E4;
    }

    if (right_on)
    {
        field_48_pan_x += field_5C_pan_speed_right;
        if (field_48_pan_x > kMaxPanX_6768C0)
        {
            field_48_pan_x = kMaxPanX_6768C0;
        }

        field_5C_pan_speed_right += kPanAcceleration_676910;
        if (field_5C_pan_speed_right > kMaxPanSpeed_676608)
        {
            field_5C_pan_speed_right = kMaxPanSpeed_676608;
        }
    }
    else
    {
        field_5C_pan_speed_right = kInitialPanSpeed_6766E4;
    }
}

MATCH_FUNC(0x436830)
void Camera::ResetPanning_436830()
{
    field_48_pan_x = 0;
    field_4C_pan_y = 0;
    field_58_pan_speed_left = kInitialPanSpeed_6766E4;
    field_5C_pan_speed_right = kInitialPanSpeed_6766E4;
    field_50_pan_speed_up = kInitialPanSpeed_6766E4;
    field_54_pan_speed_down = kInitialPanSpeed_6766E4;
}

MATCH_FUNC(0x436860)
void Camera::ApplyPanOffset_436860(Ped* pPed, Fix16& x_pos, Fix16& y_pos, Fix16 z_pos)
{
    Fix16 depth_scale = (z_pos - pPed->get_cam_z() + Fix16(8)) / field_60_screen_scale.y;
    x_pos += field_48_pan_x * depth_scale;
    y_pos += field_4C_pan_y * depth_scale;
}

MATCH_FUNC(0x4368E0)
Camera::Camera()
{
    field_68_screen_px_width = 0;
    field_6C_screen_px_height = 0;
    ResetElevation_435830();
    field_98_cur_pos.field_C_zoom = kDefaultZoom_6766D4;
    SetTarget_4397D0(-1, -1, -1, kDefaultZoom_6766D4);
    CommitPendingTarget_41E410();
    field_60_screen_scale.x = Fix16(-1);
    field_60_screen_scale.y = Fix16(-1);
    field_AC_cam_velocity.field_0_x = kZero_676818;
    field_AC_cam_velocity.field_4_y = kZero_676818;
    field_AC_cam_velocity.field_8_z = kZero_676818;
    field_3C_follow_mode = camera_follow_mode::none_0;
    field_30_shake_amount = kZero_676818;
    field_34_target_ped = NULL;
    SetScreenSize_4361B0(640, 480);
    field_44_suspicion = 0;
    ResetPanning_436830();
}

MATCH_FUNC(0x4369E0)
Camera::~Camera()
{
}

MATCH_FUNC(0x4397D0)
void Camera::SetTarget_4397D0(Fix16 x, Fix16 y, Fix16 z, Fix16 zoom)
{
    field_10_pending_target_pos.field_0_x = x;
    field_10_pending_target_pos.field_4_y = y;
    z += field_40_target_elevation;
    field_10_pending_target_pos.field_8_z = z;
    field_10_pending_target_pos.field_C_zoom = zoom;
}

// TODO: move
// https://decomp.me/scratch/qYIak
MATCH_FUNC(0x4F7540)
EXPORT void __stdcall SmoothApproach_4F7540(Fix16& Coord_1, Fix16& Velocity_1, Fix16& Coord_2, Fix16& Velocity_2, Fix16& Velocity_3)
{
    // One shared `Coord_2 += Velocity_1` at the end: VC6 copies it into the clamp paths
    Fix16 DeltaCoord = Coord_1 - Coord_2;
    if (DeltaCoord > kZero_6F6C50)
    {
        if (Velocity_1 >= kZero_6F6C50)
        {
            if (Velocity_1 + Velocity_2 <= DeltaCoord)
            {
                Velocity_1 += Velocity_2;
                if (Velocity_1 > Velocity_3)
                {
                    Velocity_1 = Velocity_3;
                }
            }
            else
            {
                Velocity_1 = DeltaCoord;
            }
        }
        else
        {
            Velocity_1 = kZero_6F6C50;
        }
    }
    else if (DeltaCoord < kZero_6F6C50)
    {
        if (Velocity_1 <= kZero_6F6C50)
        {
            if (Velocity_1 - Velocity_2 >= DeltaCoord)
            {
                Velocity_1 -= Velocity_2;
                if (Velocity_1 < -Velocity_3)
                {
                    Velocity_1 = -Velocity_3;
                }
            }
            else
            {
                Velocity_1 = DeltaCoord;
            }
        }
        else
        {
            Velocity_1 = kZero_6F6C50;
        }
    }
    else
    {
        Velocity_1 = kZero_6F6C50;
    }
    Coord_2 += Velocity_1;
}

// TODO: move
// https://decomp.me/scratch/kwM8W
MATCH_FUNC(0x4F75D0)
EXPORT void __stdcall SmoothApproachClamped_4F75D0(Fix16* target_coord,
                                                   Fix16* coord_velocity,
                                                   Fix16* curr_coord,
                                                   Fix16* velocity_1,
                                                   Fix16* velocity_2,
                                                   Fix16* velocity_3,
                                                   Fix16* max_neg_velocity)
{
    Fix16 DeltaCoord = *target_coord - *curr_coord;
    if (DeltaCoord > kZero_6F6C50)
    {
        if (*coord_velocity >= kZero_6F6C50)
        {
            if (*coord_velocity + *velocity_1 <= DeltaCoord)
            {
                *coord_velocity += *velocity_1;
                if (*coord_velocity > *velocity_2)
                {
                    *coord_velocity = *velocity_2;
                }
            }
            else
            {
                *coord_velocity = DeltaCoord;
            }
        }
        else
        {
            *coord_velocity = kZero_6F6C50;
        }
    }
    else
    {
        if (DeltaCoord >= kZero_6F6C50 || *coord_velocity > kZero_6F6C50)
        {
            *coord_velocity = kZero_6F6C50;
        }
        else
        {
            if (*coord_velocity - *velocity_3 >= DeltaCoord)
            {
                *coord_velocity -= *velocity_3;
                if (*coord_velocity < -*max_neg_velocity)
                {
                    *coord_velocity = -*max_neg_velocity;
                }
            }
            else
            {
                *coord_velocity = DeltaCoord;
            }
        }
    }

    *curr_coord += *coord_velocity;
}

MATCH_FUNC(0x58CF10)
bool Camera::IsPointInBoundaries_58CF10(Fix16 x, Fix16 y)
{
    return x >= field_78_view_bounds.field_0_left && x <= field_78_view_bounds.field_4_right &&
        y >= field_78_view_bounds.field_8_top && y <= field_78_view_bounds.field_C_bottom;
}