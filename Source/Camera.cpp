#include "Camera.hpp"
#include "Car_BC.hpp"
#include "Function.hpp"
#include "Game_0x40.hpp"
#include "Globals.hpp"
#include "Hamburger_500.hpp"
#include "Police_7B8.hpp"
#include "Ped.hpp"
#include "sprite.hpp"

DEFINE_GLOBAL(Camera_0xBC*, gViewCamera_676978, 0x676978);
DEFINE_GLOBAL_INIT(Fix16, dword_676840, Fix16(0x20000, 0), 0x676840);
DEFINE_GLOBAL_INIT(Fix16, dword_67671C, Fix16(0x2000, 0), 0x67671C);
DEFINE_GLOBAL_INIT(Fix16, dword_676818, Fix16(0), 0x676818);
DEFINE_GLOBAL_INIT(Fix16, dword_67681C, Fix16(1), 0x67681C);
DEFINE_GLOBAL_INIT(Fix16, dword_6766D4, Fix16(0x38CC, 0), 0x6766D4);
DEFINE_GLOBAL_INIT(Fix16, dword_67682C, Fix16(0x14000, 0), 0x67682C);
DEFINE_GLOBAL_INIT(Fix16, dword_6766E4, dword_67682C, 0x6766E4);
DEFINE_GLOBAL_INIT(Fix16, dword_6768F0, Fix16(0x370000, 0), 0x6768F0);
DEFINE_GLOBAL_INIT(Fix16, dword_676910, dword_67682C, 0x676910);
DEFINE_GLOBAL_INIT(Fix16, dword_676894, Fix16(0x50000, 0), 0x676894);
DEFINE_GLOBAL_INIT(Fix16, dword_676608, dword_676894, 0x676608);
DEFINE_GLOBAL_INIT(Fix16, dword_6768C0, Fix16(0x4B0000, 0), 0x6768C0);
DEFINE_GLOBAL_INIT(Fix16, dword_6767D0, Fix16(256, 0), 0x6767D0);
DEFINE_GLOBAL_INIT(Fix16, dword_676664, Fix16(1638, 0), 0x676664);
DEFINE_GLOBAL_INIT(Fix16, dword_676678, Fix16(2000, 0), 0x676678);
DEFINE_GLOBAL_INIT(Fix16, dword_6768D8, dword_6767D0, 0x6768D8);
DEFINE_GLOBAL_INIT(Fix16, dword_676918, dword_6768D8 * 12, 0x676918);
DEFINE_GLOBAL_INIT(Fix16, dword_6767B8, dword_6768D8 * dword_676678, 0x6767B8);
DEFINE_GLOBAL_INIT(Fix16, dword_676638, dword_6768D8 * 40, 0x676638);
DEFINE_GLOBAL_INIT(Fix16, dword_676834, dword_6768D8 * dword_676664, 0x676834);
DEFINE_GLOBAL_INIT(Fix16, dword_6765FC, dword_6768D8 * 5, 0x6765FC);
DEFINE_GLOBAL_INIT(Fix16, dword_6766FC, dword_6768D8, 0x6766FC);
DEFINE_GLOBAL_INIT(Fix16, dword_6766A4, dword_6768D8 * 4, 0x6766A4);
DEFINE_GLOBAL_INIT(Fix16, dword_676740, dword_6768D8 * dword_67681C, 0x676740);
DEFINE_GLOBAL_INIT(Fix16, dword_676838, Fix16(0x1C000, 0), 0x676838);
DEFINE_GLOBAL_INIT(Fix16, dword_67668C, Fix16(0x3FFFFF, 0), 0x67668C);
DEFINE_GLOBAL_INIT(Fix16, dword_6768E0, Fix16(0x3000, 0), 0x6768E0);
DEFINE_GLOBAL_INIT(Fix16, dword_67691C, dword_6768E0, 0x67691C);
DEFINE_GLOBAL_INIT(Fix16, dword_6766F4, Fix16(0x3000, 0), 0x6766F4);
DEFINE_GLOBAL_INIT(Fix16, dword_676820, Fix16(2), 0x676820);
DEFINE_GLOBAL_INIT(Fix16, dword_6767B4, Fix16(0xE333, 0), 0x6767B4);
DEFINE_GLOBAL_INIT(Fix16, dword_676898, Fix16(14), 0x676898);
DEFINE_GLOBAL_INIT(Fix16, kZero_6F6C50, Fix16(0), 0x6F6C50);


DEFINE_GLOBAL(Fix16, dword_702E04, 0x702E04);
DEFINE_GLOBAL(Fix16, dword_702DE4, 0x702DE4);

// TODO: move
static inline Fix16 sub_41E130(Fix16 a1, Fix16 a2)
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
    if (pSprite->field_30_sprite_type_enum == sprite_types_enum::car && pSprite->field_8_car_bc_ptr != 0)
    {
        if (field_38_car == pSprite->field_8_car_bc_ptr)
        {
            return true;
        }
        if (field_34_ped && field_34_ped->field_16C_car == pSprite->field_8_car_bc_ptr)
        {
            return true;
        }
    }
    else if (pSprite->field_30_sprite_type_enum == sprite_types_enum::ped)
    {
        Char_B4* pB4 = pSprite->field_8_char_b4_ptr;
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

STUB_FUNC(0x435630)
char_type Camera_0xBC::sub_435630(s16* a2, s32 a3)
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x4357B0)
void Camera_0xBC::sub_4357B0()
{
    field_88_cam_pos1.field_0_x = field_98_cam_pos2.field_0_x;
    field_88_cam_pos1.field_4_y = field_98_cam_pos2.field_4_y;
    field_88_cam_pos1.field_8_z = field_98_cam_pos2.field_8_z;
    field_88_cam_pos1.field_C_zoom = field_98_cam_pos2.field_C_zoom;
}

MATCH_FUNC(0x4357F0)
void Camera_0xBC::sub_4357F0()
{
    if (field_40_tgt_elevation < dword_676894)
    {
        field_40_tgt_elevation += dword_676678;
    }
}

MATCH_FUNC(0x435810)
void Camera_0xBC::sub_435810()
{
    if (field_40_tgt_elevation > dword_676818)
    {
        field_40_tgt_elevation -= dword_676678;
    }
}

MATCH_FUNC(0x435830)
void Camera_0xBC::ReturnToDefaultZoom_435830()
{
    field_40_tgt_elevation = dword_676818;
}

MATCH_FUNC(0x435840)
void Camera_0xBC::sub_435840()
{
    if (field_10_cam_pos_tgt2.field_8_z < dword_676818)
    {
        field_10_cam_pos_tgt2.field_8_z = dword_676818;
    }

    if (field_10_cam_pos_tgt2.field_8_z > dword_676898)
    {
        field_10_cam_pos_tgt2.field_8_z = dword_676898;
    }
}

MATCH_FUNC(0x435860)
void Camera_0xBC::sub_435860(Camera_0xBC* a2)
{
    field_10_cam_pos_tgt2.field_0_x += a2->field_98_cam_pos2.field_0_x - a2->field_88_cam_pos1.field_0_x;
    field_10_cam_pos_tgt2.field_4_y += a2->field_98_cam_pos2.field_4_y - a2->field_88_cam_pos1.field_4_y;
    field_10_cam_pos_tgt2.field_8_z += a2->field_98_cam_pos2.field_8_z - a2->field_88_cam_pos1.field_8_z;
    field_10_cam_pos_tgt2.field_C_zoom += a2->field_98_cam_pos2.field_C_zoom - a2->field_88_cam_pos1.field_C_zoom;
    sub_435840();
}

STUB_FUNC(0x4358D0)
Ang16 Camera_0xBC::sub_4358D0()
{
    NOT_IMPLEMENTED;
    return Ang16(0);
}

MATCH_FUNC(0x435A20)
Fix16 Camera_0xBC::sub_435A20()
{
    Ped* pPed = field_34_ped;
    if (pPed)
    {
        return pPed->GetPedVelocity_45C920();
    }

    Car_BC* pCar = field_38_car;
    if (pCar)
    {
        return pCar->sub_43A240();
    }
    else
    {
        return dword_676818;
    }
}

MATCH_FUNC(0x435A70)
s32 Camera_0xBC::IsCoordsPosVisible_435A70(Fix16 x, Fix16 y, Fix16 z)
{
    Fix16_Point_POD pos = sub_40CFC0(x, y, z);
    if (pos.x >= dword_676818 && pos.x < Fix16(640) && pos.y >= dword_676818 && pos.y < Fix16(480))
    {
        return 1;
    }
    return 0;
}

// https://decomp.me/scratch/YoPmg Is field_60 really a Fix16_Point ?
STUB_FUNC(0x435B90)
void Camera_0xBC::sub_435B90()
{
    field_60.x = Fix16(field_68_screen_px_width) * field_98_cam_pos2.field_C_zoom;
    field_60.y = Fix16(640) * field_98_cam_pos2.field_C_zoom;

    Fix16 v3 = dword_67671C * ((dword_676838 + field_98_cam_pos2.field_8_z) * (dword_67681C / field_98_cam_pos2.field_C_zoom));
    Fix16 v5 = dword_67671C * ((dword_676838 + field_98_cam_pos2.field_8_z) * (dword_67681C / field_98_cam_pos2.field_C_zoom));

    Fix16 x_pos = field_98_cam_pos2.field_0_x;

    field_78_boundaries_non_neg.field_0_left = x_pos - v3;

    if (field_78_boundaries_non_neg.field_0_left < dword_676818)
    {
        field_78_boundaries_non_neg.field_0_left = 0;
    }
    else
    {
        if (field_78_boundaries_non_neg.field_0_left > dword_67668C)
        {
            field_78_boundaries_non_neg.field_0_left = dword_67668C;
        }
    }

    field_78_boundaries_non_neg.field_4_right = x_pos + v5;
    if (x_pos + v5 < dword_676818)
    {
        field_78_boundaries_non_neg.field_4_right = 0;
    }
    else
    {
        if (x_pos + v5 > dword_67668C)
        {
            field_78_boundaries_non_neg.field_4_right = dword_67668C;
        }
    }

    Fix16 v7 = v5 * dword_6768E0;
    Fix16 v7_high = field_98_cam_pos2.field_4_y;
    field_78_boundaries_non_neg.field_8_top = v7_high - v7;

    if (field_78_boundaries_non_neg.field_8_top < dword_676818)
    {
        field_78_boundaries_non_neg.field_8_top = 0;
    }
    else
    {
        if (field_78_boundaries_non_neg.field_8_top > dword_67668C)
        {
            field_78_boundaries_non_neg.field_8_top = dword_67668C;
        }
    }

    field_78_boundaries_non_neg.field_C_bottom = v7_high + v7;
    if (field_78_boundaries_non_neg.field_C_bottom < dword_676818)
    {
        field_78_boundaries_non_neg.field_C_bottom = 0;
    }
    else
    {
        if (field_78_boundaries_non_neg.field_C_bottom > dword_67668C)
        {
            field_78_boundaries_non_neg.field_C_bottom = dword_67668C;
        }
    }

    field_20_boundaries.field_0_left = field_78_boundaries_non_neg.field_0_left - dword_67691C;
    field_20_boundaries.field_4_right = dword_67691C + field_78_boundaries_non_neg.field_4_right;
    field_20_boundaries.field_8_top = field_78_boundaries_non_neg.field_8_top - dword_67691C;
    field_20_boundaries.field_C_bottom = dword_67691C + field_78_boundaries_non_neg.field_C_bottom;
}

MATCH_FUNC(0x435D20)
void Camera_0xBC::sub_435D20(char_type a2, char_type a3, char_type a4, char_type a5, char_type a6, char_type a7)
{
    sub_41E410_reversed();
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
        field_10_cam_pos_tgt2.field_8_z += dword_67681C;
    }

    if (a7)
    {
        field_10_cam_pos_tgt2.field_8_z -= dword_67681C;
    }
    sub_435840();
}

MATCH_FUNC(0x435DD0)
void Camera_0xBC::sub_435DD0()
{
    field_98_cam_pos2.field_0_x = field_0_cam_pos_tgt1.field_0_x;
    field_98_cam_pos2.field_4_y = field_0_cam_pos_tgt1.field_4_y;
    field_98_cam_pos2.field_8_z = field_0_cam_pos_tgt1.field_8_z;
    field_98_cam_pos2.field_C_zoom = field_0_cam_pos_tgt1.field_C_zoom;

    field_AC_cam_velocity.field_0_x = dword_676818;
    field_AC_cam_velocity.field_4_y = dword_676818;
    field_AC_cam_velocity.field_8_z = dword_676818;
    field_AC_cam_velocity.field_C_zoom = dword_676818;
}

MATCH_FUNC(0x435F90)
void Camera_0xBC::sub_435F90(Car_BC* a2)
{
    if (a2->field_54_driver &&
        (gPolice_7B8_6FEE40->sub_56F880(a2->field_54_driver) || gHamburger_500_678E30->sub_474970(a2->field_54_driver)))
    {
        field_44++;
        if (field_44 > 80u)
        {
            field_44 = 80;
        }
    }
    else
    {
        if (field_44 > 0)
        {
            field_44--;
        }
    }
}

// TODO: move
// https://decomp.me/scratch/qYIak
WIP_FUNC(0x4F7540)
EXPORT void __stdcall SmoothApproach_4F7540(Fix16& Coord_1, Fix16& Velocity_1, Fix16& Coord_2, Fix16& Velocity_2, Fix16& Velocity_3)
{
    WIP_IMPLEMENTED;

    Fix16 DeltaCoord = Coord_1 - Coord_2;
    if (DeltaCoord > kZero_6F6C50)
    {
        if (Velocity_1 >= kZero_6F6C50)
        {
            Fix16 v6 = Velocity_2 + Velocity_1;
            if (v6 <= DeltaCoord)
            {
                Velocity_1 = v6;
                if (Velocity_1 > Velocity_3) // line 38   jle  7e
                {
                    Velocity_1 = Velocity_3;
                    Coord_2 += Velocity_3;
                }
                else
                {
                    Coord_2 += Velocity_1;
                }
            }
            else
            {
                Velocity_1 = DeltaCoord;
                Coord_2 += DeltaCoord;
            }
        }
        else
        {
            Velocity_1 = kZero_6F6C50;
            Coord_2 += Velocity_1;
        }
    }
    else if (DeltaCoord < kZero_6F6C50 && Velocity_1 <= kZero_6F6C50)
    {
        Fix16 DeltaVel = Velocity_1 - Velocity_2;
        if (DeltaVel >= DeltaCoord)
        {
            Velocity_1 = DeltaVel;
            DeltaCoord = -Velocity_3;
            if (Velocity_1 < DeltaCoord)
            {
                Velocity_1 = DeltaCoord;
                Coord_2 += DeltaCoord;
            }
            else
            {
                Coord_2 += Velocity_1;
            }
        }
        else
        {
            Velocity_1 = DeltaCoord;
            Coord_2 += DeltaCoord;
        }
    }
    else
    {
        Velocity_1 = kZero_6F6C50;
        Coord_2 += Velocity_1;
    }
}

// TODO: move
// https://decomp.me/scratch/kwM8W
WIP_FUNC(0x4F75D0)
EXPORT void __stdcall SmoothApproachClamped_4F75D0(Fix16* target_coord,
                                                   Fix16* coord_velocity,
                                                   Fix16* curr_coord,
                                                   Fix16* velocity_1,
                                                   Fix16* velocity_2,
                                                   Fix16* velocity_3,
                                                   Fix16* maybe_decrement)
{
    WIP_IMPLEMENTED;
    Fix16 DeltaCoord = *target_coord - *curr_coord;
    if (DeltaCoord > kZero_6F6C50)
    {
        if (*coord_velocity >= kZero_6F6C50)
        {
            Fix16 v8 = *velocity_1 + *coord_velocity;
            if (v8 <= DeltaCoord)
            {
                *coord_velocity = v8;
                if (v8 > *velocity_2)
                {
                    *coord_velocity = *velocity_2;
                    *curr_coord += *velocity_2;
                }
                else
                {
                    *curr_coord += *coord_velocity;
                }
            }
            else
            {
                *coord_velocity = DeltaCoord;
                *curr_coord += DeltaCoord;
            }
        }
        else
        {
            *coord_velocity = kZero_6F6C50;
            *curr_coord += *coord_velocity;
        }
    }
    else if (DeltaCoord >= kZero_6F6C50 || *coord_velocity > kZero_6F6C50)
    {
        *coord_velocity = kZero_6F6C50;
        *curr_coord += *coord_velocity;
    }
    else
    {
        Fix16 DeltaVel = *coord_velocity - *velocity_3;
        if (DeltaVel < DeltaCoord)
        {
            *coord_velocity = DeltaCoord;
            *curr_coord += DeltaCoord;
        }
        else
        {
            *coord_velocity = DeltaVel;
            DeltaCoord = -*maybe_decrement;
            if (DeltaVel < DeltaCoord)
            {
                *coord_velocity = DeltaCoord;
                *curr_coord += DeltaCoord;
            }
            else
            {
                *curr_coord += *coord_velocity;
            }
        }
    }
}

MATCH_FUNC(0x435FF0)
void Camera_0xBC::sub_435FF0()
{
    Camera_0xBC::sub_4357B0();
    Fix16 v5 = field_98_cam_pos2.field_8_z * dword_676918;

    switch (field_3C_followed_ped_id)
    {
        case 1:
            SmoothApproach_4F7540(field_0_cam_pos_tgt1.field_0_x, field_AC_cam_velocity.field_0_x, field_98_cam_pos2.field_0_x, dword_676740, v5);
            SmoothApproach_4F7540(field_0_cam_pos_tgt1.field_4_y, field_AC_cam_velocity.field_4_y, field_98_cam_pos2.field_4_y, dword_676740, v5);
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
    if (field_30 != dword_676818)
    {
        Camera_0xBC::sub_436140();
    }
    Camera_0xBC::sub_435B90();
    field_0_cam_pos_tgt1 = field_10_cam_pos_tgt2;
}

MATCH_FUNC(0x436110)
void Camera_0xBC::sub_436110()
{
    sub_435B90();
}

MATCH_FUNC(0x436120)
void Camera_0xBC::sub_436120(Fix16 a2)
{
    field_30 = a2 * dword_6768D8;
}

MATCH_FUNC(0x436140)
void Camera_0xBC::sub_436140()
{
    field_98_cam_pos2.field_0_x += field_30;
    field_98_cam_pos2.field_4_y += field_30;
    field_98_cam_pos2.field_8_z += field_30;
    if (field_98_cam_pos2.field_8_z < dword_676818)
    {
        field_98_cam_pos2.field_8_z = dword_676818;
    }
    field_30 = -field_30;
    field_30 = dword_6766F4 * field_30;
}

MATCH_FUNC(0x4361B0)
void Camera_0xBC::sub_4361B0(u32 x_pos, u32 y_pos)
{
    field_68_screen_px_width = x_pos;
    field_6C_screen_px_height = y_pos;

    field_70_screen_px_center_x = x_pos / 2;
    field_74_screen_px_center_y = y_pos / 2;

    field_60.x = Fix16(-1);
    field_60.y = Fix16(-1);

    field_A8_ui_scale = Fix16(x_pos) / 640;
}

STUB_FUNC(0x436200)
s32 Camera_0xBC::sub_436200(Car_BC* a2, Fix16* a3, Fix16* a4, Fix16* a5)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x4364A0)
void Camera_0xBC::sub_4364A0(Car_BC* pCar)
{
    NOT_IMPLEMENTED;
}

MATCH_FUNC(0x436540)
void Camera_0xBC::sub_436540(Ped* pPed)
{
    Car_BC* pCar_2;
    Fix16 a5;

    field_38_car = NULL;
    field_34_ped = pPed;
    if (pPed != NULL)
    {
        Fix16 xpos;
        Fix16 ypos;
        Fix16 zpos;
        Car_BC* pCar = pPed->field_16C_car;
        if (pCar || (pCar_2 = pPed->sub_45BBF0(), pCar_2 == 0))
        {
            xpos = pPed->get_cam_x();
            ypos = pPed->get_cam_y();
            zpos = pPed->get_cam_z() - dword_676820;
            if (zpos <= dword_67681C)
            {
                zpos = dword_67681C;
            }
            a5 = zpos;
            if (pCar)
            {
                Camera_0xBC::sub_435F90(pCar);
                Camera_0xBC::sub_436200(pCar, &xpos, &ypos, &a5);
            }
        }
        else
        {
            xpos = pCar_2->get_x_41E430();
            ypos = pCar_2->get_y_41E440();
            zpos = sub_41E130(pCar_2->get_z_41E450() - dword_676820, dword_67681C);
            a5 = zpos;
            Camera_0xBC::sub_435F90(pCar_2);
            Camera_0xBC::sub_436200(pCar_2, &xpos, &ypos, &a5);
        }
        Fix16 v10 = dword_6767B4;
        if (pPed->get_ped_state1() != 9)
        {
            v10 = dword_6766D4;
        }
        Camera_0xBC::sub_436860(pPed, xpos, ypos, a5);
        SetCamera_41E3D0(xpos, ypos, a5, v10);
    }
}

MATCH_FUNC(0x436710)
void Camera_0xBC::HandlePanning_436710(char_type bForwardGasOn, char_type bFootBrakeOn, char_type a4, char_type a5)
{
    if (bForwardGasOn)
    {
        field_4C -= field_50;
        if (field_4C < -dword_6768F0)
        {
            field_4C = -dword_6768F0;
        }

        field_50 += dword_676910;
        if (field_50 > dword_676608)
        {
            field_50 = dword_676608;
        }
    }
    else
    {
        field_50 = dword_6766E4;
    }

    if (bFootBrakeOn)
    {
        field_4C += field_54;
        if (field_4C > dword_6768F0)
        {
            field_4C = dword_6768F0;
        }

        field_54 += dword_676910;
        if (field_54 > dword_676608)
        {
            field_54 = dword_676608;
        }
    }
    else
    {
        field_54 = dword_6766E4;
    }

    if (a4)
    {
        field_48 -= field_58;
        if (field_48 < -dword_6768C0)
        {
            field_48 = -dword_6768C0;
        }

        field_58 += dword_676910;
        if (field_58 > dword_676608)
        {
            field_58 = dword_676608;
        }
    }
    else
    {
        field_58 = dword_6766E4;
    }

    if (a5)
    {
        field_48 += field_5C;
        if (field_48 > dword_6768C0)
        {
            field_48 = dword_6768C0;
        }

        field_5C += dword_676910;
        if (field_5C > dword_676608)
        {
            field_5C = dword_676608;
        }
    }
    else
    {
        field_5C = dword_6766E4;
    }
}

MATCH_FUNC(0x436830)
void Camera_0xBC::sub_436830()
{
    field_48 = 0;
    field_4C = 0;
    field_58 = dword_6766E4;
    field_5C = dword_6766E4;
    field_50 = dword_6766E4;
    field_54 = dword_6766E4;
}

MATCH_FUNC(0x436860)
void Camera_0xBC::sub_436860(Ped* a2, Fix16& x_pos, Fix16& y_pos, Fix16 z_pos)
{
    Fix16 v5 = (z_pos - a2->get_cam_z() + Fix16(8)) / field_60.y;
    x_pos += field_48 * v5;
    y_pos += field_4C * v5;
}

MATCH_FUNC(0x4368E0)
Camera_0xBC::Camera_0xBC()
{
    field_68_screen_px_width = 0;
    field_6C_screen_px_height = 0;
    ReturnToDefaultZoom_435830();
    field_98_cam_pos2.field_C_zoom = dword_6766D4;
    sub_4397D0(-1, -1, -1, dword_6766D4);
    ctor_inline(640, 480);
    field_44 = 0;
    sub_436830();
}

STUB_FUNC(0x4369E0)
Camera_0xBC::~Camera_0xBC() // empty 4369E0    Why doesn't it match anymore?
{
}

MATCH_FUNC(0x4397D0)
void Camera_0xBC::sub_4397D0(Fix16 a2, Fix16 a3, Fix16 a4, Fix16 a5)
{
    field_10_cam_pos_tgt2.field_0_x = a2;
    field_10_cam_pos_tgt2.field_4_y = a3;
    a4 += field_40_tgt_elevation;
    field_10_cam_pos_tgt2.field_8_z = a4;
    field_10_cam_pos_tgt2.field_C_zoom = a5;
}

MATCH_FUNC(0x58CF10)
bool Camera_0xBC::sub_58CF10(Fix16 a2, Fix16 a3)
{
    return a2 >= field_78_boundaries_non_neg.field_0_left && a2 <= field_78_boundaries_non_neg.field_4_right &&
        a3 >= field_78_boundaries_non_neg.field_8_top && a3 <= field_78_boundaries_non_neg.field_C_bottom;
}