#define FIX16_POINT_ZERO kZero_676818
#include "Camera.hpp"
#include "Car_BC.hpp"
#include "CarPhysics_B0.hpp"
#include "Function.hpp"
#include "Game_0x40.hpp"
#include "Globals.hpp"
#include "Hamburger_500.hpp"
#include "Ped.hpp"
#include "Police_7B8.hpp"
#include "sprite.hpp"

DEFINE_GLOBAL(Camera_0xBC*, gViewCamera_676978, 0x676978);
DEFINE_GLOBAL_INIT(Fix16, dword_676840, Fix16(0x20000, 0), 0x676840);
DEFINE_GLOBAL_INIT(Fix16, dword_67671C, Fix16(0x2000, 0), 0x67671C);
DEFINE_GLOBAL_INIT(Fix16, kZero_676818, Fix16(0), 0x676818);
DEFINE_GLOBAL_INIT(Fix16, kOne_67681C, Fix16(1), 0x67681C);
DEFINE_GLOBAL_INIT(Fix16, kDefaultZoom_6766D4, Fix16(0x38CC, 0), 0x6766D4);
DEFINE_GLOBAL_INIT(Fix16, dword_67682C, Fix16(0x14000, 0), 0x67682C);
DEFINE_GLOBAL_INIT(Fix16, kInitialPanSpeed_6766E4, dword_67682C, 0x6766E4);
DEFINE_GLOBAL_INIT(Fix16, kMaxPanY_6768F0, Fix16(0x370000, 0), 0x6768F0);
DEFINE_GLOBAL_INIT(Fix16, kPanAcceleration_676910, dword_67682C, 0x676910);
DEFINE_GLOBAL_INIT(Fix16, kMaxTgtElevation_676894, Fix16(0x50000, 0), 0x676894);
DEFINE_GLOBAL_INIT(Fix16, kMaxPanSpeed_676608, kMaxTgtElevation_676894, 0x676608);
DEFINE_GLOBAL_INIT(Fix16, kMaxPanX_6768C0, Fix16(0x4B0000, 0), 0x6768C0);
DEFINE_GLOBAL_INIT(Fix16, dword_6767D0, Fix16(256, 0), 0x6767D0);
DEFINE_GLOBAL_INIT(Fix16, dword_676664, Fix16(1638, 0), 0x676664);
DEFINE_GLOBAL_INIT(Fix16, dword_676678, Fix16(0x2000, 0), 0x676678);
DEFINE_GLOBAL_INIT(Fix16, dword_6768D8, dword_6767D0, 0x6768D8);
DEFINE_GLOBAL_INIT(Fix16, dword_676918, dword_6768D8 * 12, 0x676918);
DEFINE_GLOBAL_INIT(Fix16, dword_6767B8, dword_6768D8* dword_676678, 0x6767B8);
DEFINE_GLOBAL_INIT(Fix16, dword_676638, dword_6768D8 * 40, 0x676638);
DEFINE_GLOBAL_INIT(Fix16, dword_676834, dword_6768D8* dword_676664, 0x676834);
DEFINE_GLOBAL_INIT(Fix16, dword_6765FC, dword_6768D8 * 5, 0x6765FC);
DEFINE_GLOBAL_INIT(Fix16, dword_6766FC, dword_6768D8, 0x6766FC);
DEFINE_GLOBAL_INIT(Fix16, dword_6766A4, dword_6768D8 * 4, 0x6766A4);
DEFINE_GLOBAL_INIT(Fix16, dword_676740, dword_6768D8* kOne_67681C, 0x676740);
DEFINE_GLOBAL_INIT(Fix16, dword_676838, Fix16(0x1C000, 0), 0x676838);
DEFINE_GLOBAL_INIT(Fix16, kMaxMapCoord_67668C, Fix16(0x3FFFFF, 0), 0x67668C);
DEFINE_GLOBAL_INIT(Fix16, dword_6768E0, Fix16(0x3000, 0), 0x6768E0);
DEFINE_GLOBAL_INIT(Fix16, dword_67691C, dword_6768E0, 0x67691C);
DEFINE_GLOBAL_INIT(Fix16, dword_6766F4, Fix16(0x3000, 0), 0x6766F4);
DEFINE_GLOBAL_INIT(Fix16, kTwo_676820, Fix16(2), 0x676820);
DEFINE_GLOBAL_INIT(Fix16, dword_6767B4, Fix16(0xE333, 0), 0x6767B4);
DEFINE_GLOBAL_INIT(Fix16, kMaxCamZ_676898, Fix16(14), 0x676898);
DEFINE_GLOBAL_INIT(Fix16, kZero_6F6C50, Fix16(0), 0x6F6C50);

DEFINE_GLOBAL_INIT(Fix16, dword_702E04, Fix16(0x20000, 0), 0x702E04);
DEFINE_GLOBAL_INIT(Fix16, dword_702DE4, Fix16(0x4000, 0), 0x702DE4);

DEFINE_GLOBAL_INIT(Fix16, dword_67666C, Fix16(0xCCC, 0), 0x67666C);
DEFINE_GLOBAL_INIT(Fix16, dword_676694, dword_67666C, 0x676694);
DEFINE_GLOBAL_INIT(Fix16, dword_676684, Fix16(0x3333, 0), 0x676684);

DEFINE_GLOBAL_INIT(Fix16, dword_6768B4, Fix16(0x40000, 0), 0x6768B4);
DEFINE_GLOBAL_INIT(Fix16, dword_6768E4, Fix16(0x100000, 0), 0x6768E4);
DEFINE_GLOBAL_INIT(Fix16, dword_676900, dword_676678, 0x676900);
DEFINE_GLOBAL_INIT(Fix16, dword_67696C, dword_6768B4, 0x67696C);
DEFINE_GLOBAL_INIT(Fix16, dword_67674C, kOne_67681C + dword_676678, 0x67674C);

DEFINE_GLOBAL_INIT(Ang16, kAng45_6766DC, Ang16(0x00B4, 0), 0x6766DC);
DEFINE_GLOBAL_INIT(Ang16, kAng135_676790, Ang16(0x021C, 0), 0x676790);
DEFINE_GLOBAL_INIT(Ang16, kAng225_676764, Ang16(0x0384, 0), 0x676764);
DEFINE_GLOBAL_INIT(Ang16, kAng315_67679C, Ang16(0x04EC, 0), 0x67679C);
// Defined in sprite.cpp: when this TU sees the definition, VC6 reads the s16 with a 32-bit mov and adds with
// lea; the original's 16-bit `mov 0x676772,%cx; add %dx,%cx` needs an extern (ComputeTargetFacingAngle_4358D0)
EXTERN_GLOBAL(Ang16, kAng180_676772);
DEFINE_GLOBAL_INIT(Ang16, kAngZero_676964, Ang16(0), 0x676964);


// TODO: move
static inline Fix16 Max_41E130(Fix16 a1, Fix16 a2)
{
    if (a1 > a2)
    {
        return a1;
    }
    return a2;
}

MATCH_FUNC(0x4355D0)
bool Camera_0xBC::IsSpriteTheCameraSubject_4355D0(Sprite* pSprite)
{
    Car_BC* pCar = pSprite->AsCar_40FEB0();
    if (pCar)
    {
        if (field_38_car == pCar)
        {
            return true;
        }
        if (field_34_ped && field_34_ped->get_car_416B60() == pCar)
        {
            return true;
        }
    }
    else
    {
        Char_B4* pB4 = pSprite->AsCharB4_40FEA0();
        if (pB4)
        {
            if (field_34_ped)
            {
                if (field_34_ped->field_168_game_object == pB4)
                {
                    return true;
                }
            }
        }
    }
    return false;
}

MATCH_FUNC(0x435630)
char_type Camera_0xBC::IsSpriteInView_435630(Sprite* pSprite, s32 bUnknown)
{
    Fix16 v5;
    v5 = field_98_cam_pos2.field_8_z - pSprite->field_1C_zpos + dword_676840;
    v5 = v5 / (field_98_cam_pos2.field_C_zoom * kTwo_676820);

    if (bUnknown == 1)
    {
        v5 = (v5 * dword_676684);
    }

    Fix16 v6 = (v5 * dword_6766F4);

    Fix16_Rect rect;
    rect.SetHiLowZ_41E370(pSprite->field_1C_zpos, pSprite->field_1C_zpos);
    rect.SetRect_41E350(field_98_cam_pos2.field_0_x - v5,
                        field_98_cam_pos2.field_0_x + v5,
                        field_98_cam_pos2.field_4_y - v6,
                        field_98_cam_pos2.field_4_y + v6);

    Fix16_Rect* pBox = &pSprite->field_C_sprite_4c_ptr->field_30_boundingBox;
    if (rect.field_0_left.IntervalIntersectsRange_438FB0_inline(rect.field_4_right, pBox->field_0_left, pBox->field_4_right) &&
        IntervalIntersectsRange_438FB0(rect.field_8_top, rect.field_C_bottom, pBox->field_8_top, pBox->field_C_bottom) &&
        IntervalIntersectsRange_438FB0(rect.field_10_low_z, rect.field_14_high_z, pBox->field_10_low_z, pBox->field_14_high_z))
    {
        Sprite_4C* p4C = pSprite->field_C_sprite_4c_ptr;
        if ((p4C->field_0_width == p4C->field_4_height && p4C->field_0_width <= dword_676694) || pSprite->field_0.rValue == 0 ||
            pSprite->field_0 == 360 || pSprite->field_0 == 720 || pSprite->field_0 == 1080 || pSprite->IntersectsRectSAT_59FB10(&rect) ||
            rect.IntersectsSpriteRenderingRect_59DDF0(pSprite))
        {
            return 1;
        }
    }
    return 0;
}

MATCH_FUNC(0x4357B0)
void Camera_0xBC::SavePrevCamPos_4357B0()
{
    field_88_cam_pos1.field_0_x = field_98_cam_pos2.field_0_x;
    field_88_cam_pos1.field_4_y = field_98_cam_pos2.field_4_y;
    field_88_cam_pos1.field_8_z = field_98_cam_pos2.field_8_z;
    field_88_cam_pos1.field_C_zoom = field_98_cam_pos2.field_C_zoom;
}

MATCH_FUNC(0x4357F0)
void Camera_0xBC::IncreaseElevation_4357F0()
{
    if (field_40_tgt_elevation < kMaxTgtElevation_676894)
    {
        field_40_tgt_elevation += dword_676678;
    }
}

MATCH_FUNC(0x435810)
void Camera_0xBC::DecreaseElevation_435810()
{
    if (field_40_tgt_elevation > kZero_676818)
    {
        field_40_tgt_elevation -= dword_676678;
    }
}

MATCH_FUNC(0x435830)
void Camera_0xBC::ReturnToDefaultZoom_435830()
{
    field_40_tgt_elevation = kZero_676818;
}

MATCH_FUNC(0x435840)
void Camera_0xBC::ClampTargetZ_435840()
{
    if (field_10_cam_pos_tgt2.field_8_z < kZero_676818)
    {
        field_10_cam_pos_tgt2.field_8_z = kZero_676818;
    }

    if (field_10_cam_pos_tgt2.field_8_z > kMaxCamZ_676898)
    {
        field_10_cam_pos_tgt2.field_8_z = kMaxCamZ_676898;
    }
}

MATCH_FUNC(0x435860)
void Camera_0xBC::ApplyMovementDeltaFrom_435860(Camera_0xBC* a2)
{
    field_10_cam_pos_tgt2.field_0_x += a2->field_98_cam_pos2.field_0_x - a2->field_88_cam_pos1.field_0_x;
    field_10_cam_pos_tgt2.field_4_y += a2->field_98_cam_pos2.field_4_y - a2->field_88_cam_pos1.field_4_y;
    field_10_cam_pos_tgt2.field_8_z += a2->field_98_cam_pos2.field_8_z - a2->field_88_cam_pos1.field_8_z;
    field_10_cam_pos_tgt2.field_C_zoom += a2->field_98_cam_pos2.field_C_zoom - a2->field_88_cam_pos1.field_C_zoom;
    ClampTargetZ_435840();
}

// matches on decompme: https://decomp.me/scratch/NpBvl
MATCH_FUNC(0x4358D0)
Ang16 Camera_0xBC::ComputeTargetFacingAngle_4358D0()
{
    Ang16 CarRotation;
    Ang16 PedRotation;

    if (field_34_ped)
    {
        Car_BC* field_16C_car = field_34_ped->get_car_416B60();
        if (field_16C_car)
        {
            CarRotation = field_16C_car->get_car_rotation_416BB0();
            CarPhysics_B0* pCarPhysics = field_16C_car->field_58_physics;

            if (pCarPhysics && pCarPhysics->is_backward_gas_on_411810())
            {
                return CarRotation + kAng180_676772;
            }
            return CarRotation;
        }
        PedRotation = field_34_ped->GetRotation();
        Char_B4* field_168_game_object = field_34_ped->field_168_game_object;
        if (field_168_game_object && (field_168_game_object->field_58_flags & 8) != 0)
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
        if (field_38_car)
        {
            CarRotation = field_38_car->get_car_rotation_416BB0();
            CarPhysics_B0* pCarPhysics = field_38_car->field_58_physics;

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
Fix16 Camera_0xBC::ReturnOwnerVelocity_435A20()
{
    Ped* pPed = field_34_ped;
    if (pPed)
    {
        return pPed->GetPedVelocity_45C920();
    }

    Car_BC* pCar = field_38_car;
    if (pCar)
    {
        return pCar->GetCarLinearSpeed_43A240();
    }
    else
    {
        return kZero_676818;
    }
}

MATCH_FUNC(0x435A70)
s32 Camera_0xBC::IsCoordsPosVisible_435A70(Fix16 x, Fix16 y, Fix16 z)
{
    Fix16_Point_POD pos = WorldToScreen_40CFC0(x, y, z);
    if (pos.x >= kZero_676818 && pos.x < Fix16(640) && pos.y >= kZero_676818 && pos.y < Fix16(480))
    {
        return 1;
    }
    return 0;
}

// https://decomp.me/scratch/YoPmg Is field_60 really a Fix16_Point ?
MATCH_FUNC(0x435B90)
void Camera_0xBC::UpdateBoundaries_435B90()
{
    field_60.x = Fix16(field_68_screen_px_width) * field_98_cam_pos2.field_C_zoom;
    field_60.y = Fix16(640) * field_98_cam_pos2.field_C_zoom;

    Fix16 v = (dword_676838 + field_98_cam_pos2.field_8_z) * (kOne_67681C / field_98_cam_pos2.field_C_zoom) * dword_67671C;

    field_78_boundaries_non_neg.field_0_left = field_98_cam_pos2.field_0_x - v;
    if (field_78_boundaries_non_neg.field_0_left < kZero_676818)
    {
        field_78_boundaries_non_neg.field_0_left = Fix16(0);
    }
    else if (field_78_boundaries_non_neg.field_0_left > kMaxMapCoord_67668C)
    {
        field_78_boundaries_non_neg.field_0_left = kMaxMapCoord_67668C;
    }

    field_78_boundaries_non_neg.field_4_right = field_98_cam_pos2.field_0_x + v;
    if (field_78_boundaries_non_neg.field_4_right < kZero_676818)
    {
        field_78_boundaries_non_neg.field_4_right = Fix16(0);
    }
    else if (field_78_boundaries_non_neg.field_4_right > kMaxMapCoord_67668C)
    {
        field_78_boundaries_non_neg.field_4_right = kMaxMapCoord_67668C;
    }

    v *= dword_6768E0;

    field_78_boundaries_non_neg.field_8_top = field_98_cam_pos2.field_4_y - v;
    if (field_78_boundaries_non_neg.field_8_top < kZero_676818)
    {
        field_78_boundaries_non_neg.field_8_top = Fix16(0);
    }
    else if (field_78_boundaries_non_neg.field_8_top > kMaxMapCoord_67668C)
    {
        field_78_boundaries_non_neg.field_8_top = kMaxMapCoord_67668C;
    }

    field_78_boundaries_non_neg.field_C_bottom = field_98_cam_pos2.field_4_y + v;
    if (field_78_boundaries_non_neg.field_C_bottom < kZero_676818)
    {
        field_78_boundaries_non_neg.field_C_bottom = Fix16(0);
    }
    else if (field_78_boundaries_non_neg.field_C_bottom > kMaxMapCoord_67668C)
    {
        field_78_boundaries_non_neg.field_C_bottom = kMaxMapCoord_67668C;
    }

    field_20_boundaries.field_0_left = field_78_boundaries_non_neg.field_0_left - dword_67691C;
    field_20_boundaries.field_4_right = field_78_boundaries_non_neg.field_4_right + dword_67691C;
    field_20_boundaries.field_8_top = field_78_boundaries_non_neg.field_8_top - dword_67691C;
    field_20_boundaries.field_C_bottom = field_78_boundaries_non_neg.field_C_bottom + dword_67691C;
}

MATCH_FUNC(0x435D20)
void Camera_0xBC::MoveTarget_435D20(char_type a2, char_type a3, char_type a4, char_type a5, char_type a6, char_type a7)
{
    ResetPendingCameraTarget();
    if (a2)
    {
        field_10_cam_pos_tgt2.field_4_y -= dword_67671C;
    }

    if (a3)
    {
        field_10_cam_pos_tgt2.field_4_y += dword_67671C;
    }

    if (a4)
    {
        field_10_cam_pos_tgt2.field_0_x -= dword_67671C;
    }

    if (a5)
    {
        field_10_cam_pos_tgt2.field_0_x += dword_67671C;
    }

    if (a6)
    {
        field_10_cam_pos_tgt2.field_8_z += kOne_67681C;
    }

    if (a7)
    {
        field_10_cam_pos_tgt2.field_8_z -= kOne_67681C;
    }
    ClampTargetZ_435840();
}

MATCH_FUNC(0x435DD0)
void Camera_0xBC::ResetCameraSmoothing_435DD0()
{
    field_98_cam_pos2.field_0_x = field_0_cam_pos_tgt1.field_0_x;
    field_98_cam_pos2.field_4_y = field_0_cam_pos_tgt1.field_4_y;
    field_98_cam_pos2.field_8_z = field_0_cam_pos_tgt1.field_8_z;
    field_98_cam_pos2.field_C_zoom = field_0_cam_pos_tgt1.field_C_zoom;

    field_AC_cam_velocity.field_0_x = kZero_676818;
    field_AC_cam_velocity.field_4_y = kZero_676818;
    field_AC_cam_velocity.field_8_z = kZero_676818;
    field_AC_cam_velocity.field_C_zoom = kZero_676818;
}

MATCH_FUNC(0x435F90)
void Camera_0xBC::AccumulateSuspicionOnDriver_435F90(Car_BC* a2)
{
    if (a2->field_54_driver &&
        (gPolice_7B8_6FEE40->IsPedActiveCriminal_56F880(a2->field_54_driver) ||
         gHamburger_500_678E30->HasAnyFollower_474970(a2->field_54_driver)))
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
                                                   Fix16* maybe_decrement)
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
                if (*coord_velocity < -*maybe_decrement)
                {
                    *coord_velocity = -*maybe_decrement;
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

MATCH_FUNC(0x435FF0)
void Camera_0xBC::Update_435FF0()
{
    Camera_0xBC::SavePrevCamPos_4357B0();
    Fix16 v5 = field_98_cam_pos2.field_8_z * dword_676918;

    switch (field_3C_followed_ped_id)
    {
        case 1:
            SmoothApproach_4F7540(field_0_cam_pos_tgt1.field_0_x,
                                  field_AC_cam_velocity.field_0_x,
                                  field_98_cam_pos2.field_0_x,
                                  dword_676740,
                                  v5);
            SmoothApproach_4F7540(field_0_cam_pos_tgt1.field_4_y,
                                  field_AC_cam_velocity.field_4_y,
                                  field_98_cam_pos2.field_4_y,
                                  dword_676740,
                                  v5);
            SmoothApproachClamped_4F75D0(&field_0_cam_pos_tgt1.field_8_z,
                                         &field_AC_cam_velocity.field_8_z,
                                         &field_98_cam_pos2.field_8_z,
                                         &dword_6767B8,
                                         &dword_676638,
                                         &dword_676834,
                                         &dword_6765FC);
            SmoothApproach_4F7540(field_0_cam_pos_tgt1.field_C_zoom,
                                  field_AC_cam_velocity.field_C_zoom,
                                  field_98_cam_pos2.field_C_zoom,
                                  dword_6766FC,
                                  dword_6766A4);
            break;
        case 2:
            field_98_cam_pos2.field_0_x = field_0_cam_pos_tgt1.field_0_x;
            field_98_cam_pos2.field_4_y = field_0_cam_pos_tgt1.field_4_y;
            field_98_cam_pos2.field_8_z = field_0_cam_pos_tgt1.field_8_z;
            field_98_cam_pos2.field_C_zoom = field_0_cam_pos_tgt1.field_C_zoom;
            break;
    }
    if (field_30_shake != kZero_676818)
    {
        Camera_0xBC::ApplyShake_436140();
    }
    Camera_0xBC::UpdateBoundaries_435B90();
    field_0_cam_pos_tgt1 = field_10_cam_pos_tgt2;
}

MATCH_FUNC(0x436110)
void Camera_0xBC::RefreshBoundaries_436110()
{
    UpdateBoundaries_435B90();
}

MATCH_FUNC(0x436120)
void Camera_0xBC::SetShake_436120(Fix16 a2)
{
    field_30_shake = a2 * dword_6768D8;
}

MATCH_FUNC(0x436140)
void Camera_0xBC::ApplyShake_436140()
{
    field_98_cam_pos2.field_0_x += field_30_shake;
    field_98_cam_pos2.field_4_y += field_30_shake;
    field_98_cam_pos2.field_8_z += field_30_shake;
    if (field_98_cam_pos2.field_8_z < kZero_676818)
    {
        field_98_cam_pos2.field_8_z = kZero_676818;
    }
    field_30_shake = -field_30_shake;
    field_30_shake = dword_6766F4 * field_30_shake;
}

MATCH_FUNC(0x4361B0)
void Camera_0xBC::SetScreenSize_4361B0(u32 x_pos, u32 y_pos)
{
    field_68_screen_px_width = x_pos;
    field_6C_screen_px_height = y_pos;

    field_70_screen_px_center_x = x_pos / 2;
    field_74_screen_px_center_y = y_pos / 2;

    field_60.x = Fix16(-1);
    field_60.y = Fix16(-1);

    field_A8_ui_scale = Fix16(x_pos) / 640;
}

WIP_FUNC(0x436200)
void Camera_0xBC::ApplyCarVelocityCameraOffset_436200(Car_BC* pCar, Fix16* pX, Fix16* pY, Fix16* pZ)
{
    WIP_IMPLEMENTED;

    // v25 at function scope: it gets its own slot instead of the dead pZ parameter slot
    Fix16 v25;
    // ret before the points: one site less after their ctors, which keeps both inline (inline budget)
    Fix16 ret;
    Fix16_Point v10;
    Fix16_Point offset;

    if (pCar->IsTrainModel_403BA0())
    {
        ret = (dword_676900 * dword_67696C);
    }
    else
    {
        v10 = (pCar->get_linvel_43A450() * dword_67696C);

        ret = v10.GetLength_41E260();
    }

    if (ret.mValue > dword_67674C.mValue)
    {
        pZ->mValue += ret.mValue;

        if (!pCar->IsTrainModel_403BA0() && !pCar->IsTank_411900())
        {
            // 9.6f inlined: sub_40F790 (atan2_40F790). Written out, and the compares below on raw values,
            // so the Fix16_Point ctors stay inline (VC6 inline budget)
            // The atan2 result goes through a block-scoped copy: once its scope closes, its slot (the dead
            // pCar parameter) is reused by the sine temp below, and the angle stays in a register.
            Ang16 v16;
            {
                Ang16 t = Fix16::atan2_fixed_405320(v10.y, v10.x);
                v16 = t;
            }
            Fix16 v17;
            if ((v16.rValue <= kAng45_6766DC.rValue || v16.rValue >= kAng135_676790.rValue) && (v16.rValue <= kAng225_676764.rValue || v16.rValue >= kAng315_67679C.rValue))
            {
                v17.mValue = 0x2D0000;
            }
            else
            {
                v17.mValue = 0x3C0000;
            }

            if (pCar->is_trailer_cab_41E460())
            {
                v17 = (v17 * dword_6768E0);
            }

            u8 f44 = this->field_44_suspicion;
            if (f44)
            {
                Fix16 v20;
                if ((u8)f44 > 64u)
                {
                    v20 = dword_6768E4;
                }
                else
                {
                    v20 = Fix16(this->field_44_suspicion);
                }
                v17 = (v17 * (kOne_67681C - v20 / 128));
            }
            v25 = v17 * (*pZ - pCar->field_50_car_sprite->field_1C_zpos + Fix16(8)) / field_60.y;

            offset.FromPolar_41E210(v25, v16);
            *pX += offset.x;
            *pY += offset.y;
        }
    }
}

MATCH_FUNC(0x4364A0)
void Camera_0xBC::UpdateFollowCarCamera_4364A0(Car_BC* pCar)
{

    this->field_34_ped = 0;
    this->field_38_car = pCar;

    if (pCar)
    {
        Fix16 new_x;
        new_x = pCar->get_x_41E430();
        Fix16 new_y;
        new_y = pCar->get_y_41E440();
        Fix16 new_z;
        new_z = Max_41E130(pCar->get_z_41E450() - kTwo_676820, kOne_67681C);
        Fix16 zoom = kDefaultZoom_6766D4;

        AccumulateSuspicionOnDriver_435F90(pCar);
        ApplyCarVelocityCameraOffset_436200(pCar, &new_x, &new_y, &new_z);
        SetCamera_41E3D0(new_x, new_y, new_z, zoom);
    }
}

MATCH_FUNC(0x436540)
void Camera_0xBC::UpdateFollowPedCamera_436540(Ped* pPed)
{
    Car_BC* pCar_2;
    Fix16 zposToUse;

    field_38_car = NULL;
    field_34_ped = pPed;
    if (pPed != NULL)
    {
        Fix16 xpos;
        Fix16 ypos;
        Fix16 zpos;
        Car_BC* pCar = pPed->get_car_416B60();
        if (pCar || (pCar_2 = pPed->GetCarBeingEnteredOrExited_45BBF0(), pCar_2 == 0))
        {
            xpos = pPed->get_cam_x();
            ypos = pPed->get_cam_y();
            zpos = Max_41E130(pPed->get_cam_z() - kTwo_676820, kOne_67681C);
            zposToUse = zpos;
            if (pCar)
            {
                Camera_0xBC::AccumulateSuspicionOnDriver_435F90(pCar);
                Camera_0xBC::ApplyCarVelocityCameraOffset_436200(pCar, &xpos, &ypos, &zposToUse);
            }
        }
        else
        {
            xpos = pCar_2->get_x_41E430();
            ypos = pCar_2->get_y_41E440();
            zpos = Max_41E130(pCar_2->get_z_41E450() - kTwo_676820, kOne_67681C);
            zposToUse = zpos;
            Camera_0xBC::AccumulateSuspicionOnDriver_435F90(pCar_2);
            Camera_0xBC::ApplyCarVelocityCameraOffset_436200(pCar_2, &xpos, &ypos, &zposToUse);
        }
        Fix16 zoom = dword_6767B4;
        if (pPed->GetPedState_403990() != 9)
        {
            zoom = kDefaultZoom_6766D4;
        }
        Camera_0xBC::ApplyZOffsetToScreenPosition_436860(pPed, xpos, ypos, zposToUse);
        SetCamera_41E3D0(xpos, ypos, zposToUse, zoom);
    }
}

MATCH_FUNC(0x436710)
void Camera_0xBC::HandlePanning_436710(char_type bForwardGasOn, char_type bFootBrakeOn, char_type a4, char_type a5)
{
    if (bForwardGasOn)
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

    if (bFootBrakeOn)
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

    if (a4)
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

    if (a5)
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
void Camera_0xBC::ResetPanning_436830()
{
    field_48_pan_x = 0;
    field_4C_pan_y = 0;
    field_58_pan_speed_left = kInitialPanSpeed_6766E4;
    field_5C_pan_speed_right = kInitialPanSpeed_6766E4;
    field_50_pan_speed_up = kInitialPanSpeed_6766E4;
    field_54_pan_speed_down = kInitialPanSpeed_6766E4;
}

MATCH_FUNC(0x436860)
void Camera_0xBC::ApplyZOffsetToScreenPosition_436860(Ped* a2, Fix16& x_pos, Fix16& y_pos, Fix16 z_pos)
{
    Fix16 v5 = (z_pos - a2->get_cam_z() + Fix16(8)) / field_60.y;
    x_pos += field_48_pan_x * v5;
    y_pos += field_4C_pan_y * v5;
}

MATCH_FUNC(0x4368E0)
Camera_0xBC::Camera_0xBC()
{
    field_68_screen_px_width = 0;
    field_6C_screen_px_height = 0;
    ReturnToDefaultZoom_435830();
    field_98_cam_pos2.field_C_zoom = kDefaultZoom_6766D4;
    SetTarget_4397D0(-1, -1, -1, kDefaultZoom_6766D4);
    CommitCameraTarget_41E410();
    field_60.x = Fix16(-1);
    field_60.y = Fix16(-1);
    field_AC_cam_velocity.field_0_x = kZero_676818;
    field_AC_cam_velocity.field_4_y = kZero_676818;
    field_AC_cam_velocity.field_8_z = kZero_676818;
    field_3C_followed_ped_id = 0;
    field_30_shake = kZero_676818;
    field_34_ped = NULL;
    SetScreenSize_4361B0(640, 480);
    field_44_suspicion = 0;
    ResetPanning_436830();
}

MATCH_FUNC(0x4369E0)
Camera_0xBC::~Camera_0xBC()
{
}

MATCH_FUNC(0x4397D0)
void Camera_0xBC::SetTarget_4397D0(Fix16 a2, Fix16 a3, Fix16 a4, Fix16 a5)
{
    field_10_cam_pos_tgt2.field_0_x = a2;
    field_10_cam_pos_tgt2.field_4_y = a3;
    a4 += field_40_tgt_elevation;
    field_10_cam_pos_tgt2.field_8_z = a4;
    field_10_cam_pos_tgt2.field_C_zoom = a5;
}

MATCH_FUNC(0x58CF10)
bool Camera_0xBC::IsPointInBoundaries_58CF10(Fix16 a2, Fix16 a3)
{
    return a2 >= field_78_boundaries_non_neg.field_0_left && a2 <= field_78_boundaries_non_neg.field_4_right &&
        a3 >= field_78_boundaries_non_neg.field_8_top && a3 <= field_78_boundaries_non_neg.field_C_bottom;
}