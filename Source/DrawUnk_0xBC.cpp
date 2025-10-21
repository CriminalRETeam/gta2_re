#include "DrawUnk_0xBC.hpp"
#include "Car_BC.hpp"
#include "Function.hpp"
#include "Game_0x40.hpp"
#include "Globals.hpp"
#include "Hamburger_500.hpp"
#include "Police_7B8.hpp"
#include "Ped.hpp"
#include "sprite.hpp"

DEFINE_GLOBAL(DrawUnk_0xBC*, gViewCamera_676978, 0x676978);
DEFINE_GLOBAL(Fix16, dword_676840, 0x676840);
DEFINE_GLOBAL(Fix16, dword_67671C, 0x67671C);
DEFINE_GLOBAL(Fix16, dword_676818, 0x676818);
DEFINE_GLOBAL(Fix16, dword_67681C, 0x67681C);
DEFINE_GLOBAL(Fix16, dword_6766D4, 0x6766D4);
DEFINE_GLOBAL(Fix16, dword_6766E4, 0x6766E4);
DEFINE_GLOBAL(Fix16, dword_6768F0, 0x6768F0);
DEFINE_GLOBAL(Fix16, dword_676910, 0x676910);
DEFINE_GLOBAL(Fix16, dword_676608, 0x676608);
DEFINE_GLOBAL(Fix16, dword_6768C0, 0x6768C0);

MATCH_FUNC(0x4355D0)
bool DrawUnk_0xBC::sub_4355D0(Sprite* pSprite)
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
char_type DrawUnk_0xBC::sub_435630(s16* a2, s32 a3)
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x4357B0)
void DrawUnk_0xBC::sub_4357B0()
{
    field_88_cam_pos1.field_0_x = field_98_cam_pos2.field_0_x;
    field_88_cam_pos1.field_4_y = field_98_cam_pos2.field_4_y;
    field_88_cam_pos1.field_8_z = field_98_cam_pos2.field_8_z;
    field_88_cam_pos1.field_C_zoom = field_98_cam_pos2.field_C_zoom;
}

DEFINE_GLOBAL(Fix16, dword_676894, 0x676894);
DEFINE_GLOBAL(Fix16, dword_676678, 0x676678);

MATCH_FUNC(0x4357F0)
void DrawUnk_0xBC::sub_4357F0()
{
    if (field_40_tgt_elevation < dword_676894)
    {
        field_40_tgt_elevation += dword_676678;
    }
}

MATCH_FUNC(0x435810)
void DrawUnk_0xBC::sub_435810()
{
    if (field_40_tgt_elevation > dword_676818)
    {
        field_40_tgt_elevation -= dword_676678;
    }
}

MATCH_FUNC(0x435830)
void DrawUnk_0xBC::sub_435830()
{
    field_40_tgt_elevation = dword_676818;
}

DEFINE_GLOBAL(Fix16, dword_676898, 0x676898);

MATCH_FUNC(0x435840)
void DrawUnk_0xBC::sub_435840()
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
void DrawUnk_0xBC::sub_435860(DrawUnk_0xBC* a2)
{
    field_10_cam_pos_tgt2.field_0_x += a2->field_98_cam_pos2.field_0_x - a2->field_88_cam_pos1.field_0_x;
    field_10_cam_pos_tgt2.field_4_y += a2->field_98_cam_pos2.field_4_y - a2->field_88_cam_pos1.field_4_y;
    field_10_cam_pos_tgt2.field_8_z += a2->field_98_cam_pos2.field_8_z - a2->field_88_cam_pos1.field_8_z;
    field_10_cam_pos_tgt2.field_C_zoom += a2->field_98_cam_pos2.field_C_zoom - a2->field_88_cam_pos1.field_C_zoom;
    sub_435840();
}

STUB_FUNC(0x4358D0)
s16* DrawUnk_0xBC::sub_4358D0(s16* a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x435A20)
Fix16 DrawUnk_0xBC::sub_435A20()
{
    Ped* pPed = field_34_ped;
    if (pPed)
    {
        return pPed->sub_45C920();
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
s32 DrawUnk_0xBC::sub_435A70(Fix16 x, Fix16 y, Fix16 z)
{
    Fix16_Point_POD pos = sub_40CFC0(x, y, z);
    if (pos.x >= dword_676818 && pos.x < Fix16(640) && pos.y >= dword_676818 && pos.y < Fix16(480))
    {
        return 1;
    }
    return 0;
}

STUB_FUNC(0x435B90)
s32 DrawUnk_0xBC::sub_435B90()
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x435D20)
void DrawUnk_0xBC::sub_435D20(char_type a2, char_type a3, char_type a4, char_type a5, char_type a6, char_type a7)
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
void DrawUnk_0xBC::sub_435DD0()
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
void DrawUnk_0xBC::sub_435F90(Car_BC* a2)
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

STUB_FUNC(0x435FF0)
s32 DrawUnk_0xBC::sub_435FF0()
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x436110)
s32 DrawUnk_0xBC::sub_436110()
{
    return sub_435B90();
}

STUB_FUNC(0x436120)
void DrawUnk_0xBC::sub_436120(Fix16 a2)
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x436140)
void DrawUnk_0xBC::sub_436140()
{
    NOT_IMPLEMENTED;
}

MATCH_FUNC(0x4361B0)
void DrawUnk_0xBC::sub_4361B0(u32 x_pos, u32 y_pos)
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
s32 DrawUnk_0xBC::sub_436200(s32 a2, s32* a3, s32* a4, s32* a5)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x4364A0)
void DrawUnk_0xBC::sub_4364A0(Car_BC* pCar)
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x436540)
void DrawUnk_0xBC::sub_436540(Ped* a2)
{
    NOT_IMPLEMENTED;
}

MATCH_FUNC(0x436710)
void DrawUnk_0xBC::sub_436710(char_type bForwardGasOn, char_type bFootBrakeOn, char_type a4, char_type a5)
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
void DrawUnk_0xBC::sub_436830()
{
    field_48 = 0;
    field_4C = 0;
    field_58 = dword_6766E4;
    field_5C = dword_6766E4;
    field_50 = dword_6766E4;
    field_54 = dword_6766E4;
}

MATCH_FUNC(0x436860)
void DrawUnk_0xBC::sub_436860(Ped* a2, Fix16& x_pos, Fix16& y_pos, Fix16 z_pos)
{
    Fix16 v5 = (z_pos - a2->get_cam_z() + Fix16(8)) / field_60.y;
    x_pos += field_48 * v5;
    y_pos += field_4C * v5;
}

MATCH_FUNC(0x4368E0)
DrawUnk_0xBC::DrawUnk_0xBC()
{
    field_68_screen_px_width = 0;
    field_6C_screen_px_height = 0;
    sub_435830();
    field_98_cam_pos2.field_C_zoom = dword_6766D4;
    sub_4397D0(-1, -1, -1, dword_6766D4);
    ctor_inline(640, 480);
    field_44 = 0;
    sub_436830();
}

STUB_FUNC(0x4369E0)
DrawUnk_0xBC::~DrawUnk_0xBC() // empty 4369E0    Why doesn't it match anymore?
{
}

MATCH_FUNC(0x4397D0)
void DrawUnk_0xBC::sub_4397D0(Fix16 a2, Fix16 a3, Fix16 a4, Fix16 a5)
{
    field_10_cam_pos_tgt2.field_0_x = a2;
    field_10_cam_pos_tgt2.field_4_y = a3;
    a4 += field_40_tgt_elevation;
    field_10_cam_pos_tgt2.field_8_z = a4;
    field_10_cam_pos_tgt2.field_C_zoom = a5;
}

MATCH_FUNC(0x58CF10)
bool DrawUnk_0xBC::sub_58CF10(Fix16 a2, Fix16 a3)
{
    return a2 >= field_78_boundaries_non_neg.field_0_left && a2 <= field_78_boundaries_non_neg.field_4_right &&
        a3 >= field_78_boundaries_non_neg.field_8_top && a3 <= field_78_boundaries_non_neg.field_C_bottom;
}