#pragma once

#include "Car_BC.hpp"
#include "Function.hpp"
#include "fix16.hpp"

class Sprite;
class Car_BC;
class Ped;

EXTERN_GLOBAL(Fix16, dword_676840);

EXTERN_GLOBAL(Fix16, dword_676818);

EXTERN_GLOBAL(Fix16, dword_67681C);


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

class DrawUnk_0xBC
{
  public:
    EXPORT bool sub_4355D0(Sprite* pSprite);
    EXPORT char_type sub_435630(s16* a2, s32 a3);
    EXPORT void sub_4357B0();
    EXPORT void sub_4357F0();
    EXPORT void sub_435810();
    EXPORT void sub_435830();
    EXPORT void sub_435840();
    EXPORT void sub_435860(DrawUnk_0xBC* a2);
    EXPORT s16* sub_4358D0(s16* a2);
    EXPORT Fix16 sub_435A20();
    EXPORT s32 sub_435A70(Fix16 a2, Fix16 a3, Fix16 a4);
    EXPORT s32 sub_435B90();
    EXPORT void sub_435D20(char_type a2, char_type a3, char_type a4, char_type a5, char_type a6, char_type a7);
    EXPORT void sub_435DD0();
    EXPORT void sub_435F90(Car_BC* a2);
    EXPORT s32 sub_435FF0();
    EXPORT s32 sub_436110();
    EXPORT void sub_436120(Fix16 a2);
    EXPORT void sub_436140();
    EXPORT void sub_4361B0(u32 a2, u32 a3);
    EXPORT s32 sub_436200(s32 a2, s32* a3, s32* a4, s32* a5);
    EXPORT void sub_4364A0(Car_BC* pCar);
    EXPORT void sub_436540(Ped* a2);
    EXPORT void sub_436710(char_type a2, char_type a3, char_type a4, char_type a5);
    EXPORT void sub_436830();
    EXPORT void sub_436860(Ped* a2, Fix16& x_pos, Fix16& y_pos, Fix16 z_pos);
    EXPORT DrawUnk_0xBC(); // 4368E0
    EXPORT ~DrawUnk_0xBC(); // empty 4369E0
    EXPORT void sub_4397D0(Fix16 a2, Fix16 a3, Fix16 a4, Fix16 a5);
    EXPORT bool sub_58CF10(Fix16 a2, Fix16 a3);

    inline void inline_sub_475B60()
    {
        field_3C_followed_ped_id = 1;
    }

    inline Fix16_Point_POD sub_40CFC0(Fix16 x, Fix16 y, Fix16 z)
    {
        Fix16_Point_POD tmp;
        Fix16 u = field_98_cam_pos2.field_8_z - z;
        Fix16 t(dword_67681C / Fix16(u.mValue + dword_676840.mValue, 0));

        tmp.x = (((x - field_98_cam_pos2.field_0_x) * field_60.y) * t) + Fix16(320);
        tmp.y = (((y - field_98_cam_pos2.field_4_y) * field_60.y) * t) + Fix16(240);

        return tmp;
    }

    // inline sub_40CF60
    inline bool check_camera(Fix16 a2_fp, Fix16 a3_fp)
    {
        return a2_fp >= field_78_boundaries_non_neg.field_0_left && a2_fp <= field_78_boundaries_non_neg.field_4_right &&
            a3_fp >= field_78_boundaries_non_neg.field_8_top && a3_fp <= field_78_boundaries_non_neg.field_C_bottom;
    }

    inline void sub_41E410()
    {
        field_0_cam_pos_tgt1 = field_10_cam_pos_tgt2;
    }

    inline void sub_41E410_reversed()
    {
        field_10_cam_pos_tgt2 = field_0_cam_pos_tgt1;
    }

    inline void ctor_inline(s32 x, s32 y)
    {
        sub_41E410();
        field_60.x = Fix16(-1);
        field_60.y = Fix16(-1);
        field_AC_cam_velocity.field_0_x = dword_676818;
        field_AC_cam_velocity.field_4_y = dword_676818;
        field_AC_cam_velocity.field_8_z = dword_676818;

        field_3C_followed_ped_id = 0;
        field_30 = dword_676818;
        field_34_ped = NULL;

        sub_4361B0(x, y);
    }

    CameraPos field_0_cam_pos_tgt1;
    CameraPos field_10_cam_pos_tgt2;
    WorldRect field_20_boundaries;
    Fix16 field_30;
    Ped* field_34_ped;
    Car_BC* field_38_car;
    s32 field_3C_followed_ped_id;
    Fix16 field_40_tgt_elevation;
    u8 field_44;
    char_type field_45;
    char_type field_46;
    char_type field_47;
    Fix16 field_48;
    Fix16 field_4C;
    Fix16 field_50;
    Fix16 field_54;
    Fix16 field_58;
    Fix16 field_5C;
    Fix16_Point field_60;
    s32 field_68_screen_px_width;
    s32 field_6C_screen_px_height;
    s32 field_70_screen_px_center_x;
    s32 field_74_screen_px_center_y;
    WorldRect field_78_boundaries_non_neg;
    CameraPos field_88_cam_pos1;
    CameraPos field_98_cam_pos2;
    Fix16 field_A8_ui_scale;
    CameraPos field_AC_cam_velocity;
};
GTA2_ASSERT_SIZEOF_ALWAYS(DrawUnk_0xBC, 0xBC)

EXTERN_GLOBAL(DrawUnk_0xBC*, gViewCamera_676978);
