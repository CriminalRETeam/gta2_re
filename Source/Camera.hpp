#pragma once

#include "Car_BC.hpp"
#include "Function.hpp"
#include "fix16.hpp"

class Sprite;
class Car_BC;
class Ped;

EXTERN_GLOBAL(Fix16, dword_676840);

EXTERN_GLOBAL(Fix16, kZero_676818);

EXTERN_GLOBAL(Fix16, kOne_67681C);

EXTERN_GLOBAL(Fix16, dword_702E04);
EXTERN_GLOBAL(Fix16, dword_702DE4);

namespace camera_follow_mode
{
enum
{
    none_0 = 0,
    smooth_1 = 1, // eases towards the target (Update_435FF0)
    snap_2 = 2, // copies the target straight in (debug/free camera)
};
} // namespace camera_follow_mode

struct CameraPos
{
    Fix16 field_0_x;
    Fix16 field_4_y;
    Fix16 field_8_z;
    Fix16 field_C_zoom;
};

struct WorldRect
{
    Fix16 field_0_left;
    Fix16 field_4_right;
    Fix16 field_8_top;
    Fix16 field_C_bottom;
};

class Camera_0xBC
{
  public:
    // 9.6f 0x416B70
    inline Ped* GetFollowedPed_416B70()
    {
        return field_34_ped;
    }

    EXPORT bool IsSpriteTheCameraSubject_4355D0(Sprite* pSprite);
    EXPORT char_type IsSpriteInView_435630(Sprite* pSprite, s32 wide_view);
    EXPORT void SavePrevCamPos_4357B0();
    EXPORT void IncreaseElevation_4357F0();
    EXPORT void DecreaseElevation_435810();
    EXPORT void ReturnToDefaultZoom_435830();
    EXPORT void ClampTargetZ_435840();
    EXPORT void ApplyMovementDeltaFrom_435860(Camera_0xBC* pCamera);
    EXPORT Ang16 ComputeTargetFacingAngle_4358D0();
    EXPORT Fix16 ReturnOwnerVelocity_435A20();
    EXPORT s32 IsCoordsPosVisible_435A70(Fix16 x, Fix16 y, Fix16 z);
    EXPORT void UpdateBoundaries_435B90();
    EXPORT void MoveTarget_435D20(char_type move_north, char_type move_south, char_type move_west, char_type move_east, char_type zoom_in, char_type zoom_out);
    EXPORT void ResetCameraSmoothing_435DD0();
    EXPORT void AccumulateSuspicionOnDriver_435F90(Car_BC* pCar);
    EXPORT void Update_435FF0();
    EXPORT void RefreshBoundaries_436110();
    EXPORT void SetShake_436120(Fix16 amount);
    EXPORT void ApplyShake_436140();
    EXPORT void SetScreenSize_4361B0(u32 width, u32 height);
    EXPORT void ApplyCarVelocityCameraOffset_436200(Car_BC* pCar, Fix16* pOutX, Fix16* pOutY, Fix16* pOutZ);
    EXPORT void UpdateFollowCarCamera_4364A0(Car_BC* pCar);
    EXPORT void UpdateFollowPedCamera_436540(Ped* pPed);
    EXPORT void HandlePanning_436710(char_type forward_gas_on, char_type foot_brake_on, char_type left_on, char_type right_on);
    EXPORT void ResetPanning_436830();
    EXPORT void ApplyZOffsetToScreenPosition_436860(Ped* pPed, Fix16& x_pos, Fix16& y_pos, Fix16 z_pos);
    EXPORT Camera_0xBC(); // 4368E0
    EXPORT ~Camera_0xBC(); // empty 4369E0
    EXPORT void SetTarget_4397D0(Fix16 x, Fix16 y, Fix16 z, Fix16 zoom);
    EXPORT bool IsPointInBoundaries_58CF10(Fix16 x, Fix16 y);

    void ProjectWorldToScreen_4B90E0(Fix16 x, Fix16 y, Fix16 z, Fix16* pOutX, Fix16* pOutY)
    {
        Fix16 scale = dword_702DE4 / ((dword_702E04 - z) + field_98_cur_pos.field_8_z);
        *pOutX = (((x - field_98_cur_pos.field_0_x) * field_60_screen_scale.x) * scale) + Fix16(field_70_screen_px_center_x);
        *pOutY = (((y - field_98_cur_pos.field_4_y) * field_60_screen_scale.x) * scale) + Fix16(field_74_screen_px_center_y);
    }

    // ProjectWorldToScreen_4B90E0 with the out-of-line Fix16 helpers (ExplodingScore_50::DrawNumbers_596C90)
    void ProjectWorldToScreen_OutOfLine_4B90E0(Fix16 x, Fix16 y, Fix16 z, Fix16* pOutX, Fix16* pOutY)
    {
        Fix16 scale = dword_702DE4.Divide_436A20((field_98_cur_pos.field_8_z - z) + dword_702E04);

        *pOutX = (x.Subtract_436A00(field_98_cur_pos.field_0_x).Multiply_408680(field_60_screen_scale.x).Multiply_408680(scale))
                     .Add_408660(Fix16(field_70_screen_px_center_x));

        *pOutY = (y.Subtract_436A00(field_98_cur_pos.field_4_y).Multiply_408680(field_60_screen_scale.x).Multiply_408680(scale))
                     .Add_408660(Fix16(field_74_screen_px_center_y));
    }

    inline void SetSmoothFollow_475B60()
    {
        field_3C_follow_mode = camera_follow_mode::smooth_1;
    }

    // 9.6f 0x4A5070
    inline void SetSnapFollow_4A5070()
    {
        field_3C_follow_mode = camera_follow_mode::snap_2;
    }

    // FUNCTION: 96f 0x40cfc0
    inline Fix16_Point_POD WorldToScreen_40CFC0(Fix16 x, Fix16 y, Fix16 z)
    {
        Fix16_Point_POD tmp;
        Fix16 u = field_98_cur_pos.field_8_z - z;
        Fix16 t(kOne_67681C / Fix16(u.mValue + dword_676840.mValue, 0));

        tmp.x = (((x - field_98_cur_pos.field_0_x) * field_60_screen_scale.y) * t) + Fix16(320);
        tmp.y = (((y - field_98_cur_pos.field_4_y) * field_60_screen_scale.y) * t) + Fix16(240);

        return tmp;
    }

    // 9.6f 0x45AEA0
    inline bool IsSpriteInBounds_45AEA0(Sprite* pSprite, Fix16 margin)
    {
        return pSprite->field_14_xy.x >= field_20_boundaries.field_0_left - margin &&
            pSprite->field_14_xy.x <= field_20_boundaries.field_4_right + margin &&
            pSprite->field_14_xy.y >= field_20_boundaries.field_8_top - margin &&
            pSprite->field_14_xy.y <= field_20_boundaries.field_C_bottom + margin;
    }

    // 9.6f 0x45AF40
    inline bool IsRectInBounds_45AF40(Fix16_Rect* pRect)
    {
        return pRect->get_top_45ADD0() <= field_20_boundaries.field_C_bottom &&
            pRect->get_bottom_45ADC0() >= field_20_boundaries.field_8_top &&
            pRect->get_left_45ADB0() <= field_20_boundaries.field_4_right &&
            pRect->get_right_45ADA0() >= field_20_boundaries.field_0_left;
    }

    // 9.6f 0x40CF60
    inline bool IsPointInViewBounds_40CF60(Fix16 x, Fix16 y)
    {
        return x >= field_78_boundaries_non_neg.field_0_left && x <= field_78_boundaries_non_neg.field_4_right &&
            y >= field_78_boundaries_non_neg.field_8_top && y <= field_78_boundaries_non_neg.field_C_bottom;
    }

    inline void CommitPendingTarget_41E410()
    {
        field_0_target_pos = field_10_pending_target_pos;
    }

    inline void ResetPendingTarget()
    {
        field_10_pending_target_pos = field_0_target_pos;
    }

    // 9.6f inline 0x433E90
    inline bool HasFollowTarget_433E90()
    {
        return field_34_ped || field_38_car;
    }

    inline void SetPendingTarget_41E3D0(Fix16 xpos, Fix16 ypos, Fix16 zpos, Fix16 zoom)
    {
        field_10_pending_target_pos.field_0_x = xpos;
        field_10_pending_target_pos.field_4_y = ypos;
        field_10_pending_target_pos.field_8_z = zpos + field_40_tgt_elevation;
        field_10_pending_target_pos.field_C_zoom = zoom;
    }

    Fix16 GetBoundariesWidth_4B3110()
    {
        return field_20_boundaries.field_4_right - field_20_boundaries.field_0_left;
    }

    Fix16 GetBoundariesHeight_4B3130()
    {
        return field_20_boundaries.field_C_bottom - field_20_boundaries.field_8_top;
    }

    CameraPos field_0_target_pos;
    CameraPos field_10_pending_target_pos;
    WorldRect field_20_boundaries;
    Fix16 field_30_shake;
    Ped* field_34_ped;
    Car_BC* field_38_car;
    s32 field_3C_follow_mode;
    Fix16 field_40_tgt_elevation;
    u8 field_44_suspicion;
    char_type field_45_unused;
    char_type field_46_unused;
    char_type field_47_unused;
    Fix16 field_48_pan_x;
    Fix16 field_4C_pan_y;
    Fix16 field_50_pan_speed_up;
    Fix16 field_54_pan_speed_down;
    Fix16 field_58_pan_speed_left;
    Fix16 field_5C_pan_speed_right;
    Fix16_Point field_60_screen_scale; // x: screen width * zoom, y: 640 * zoom
    s32 field_68_screen_px_width;
    s32 field_6C_screen_px_height;
    s32 field_70_screen_px_center_x;
    s32 field_74_screen_px_center_y;
    WorldRect field_78_boundaries_non_neg;
    CameraPos field_88_prev_pos;
    CameraPos field_98_cur_pos;
    Fix16 field_A8_ui_scale;
    CameraPos field_AC_cam_velocity;
};
GTA2_ASSERT_SIZEOF_ALWAYS(Camera_0xBC, 0xBC)

EXTERN_GLOBAL(Camera_0xBC*, gViewCamera_676978);

extern inline Fix16 Max_41E130(Fix16 a1, Fix16 a2);
