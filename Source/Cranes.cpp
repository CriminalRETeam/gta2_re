#include "Cranes.hpp"
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

DEFINE_GLOBAL_INIT(Fix16, dword_679E58, Fix16(0x2000, 0), 0x679E58);
DEFINE_GLOBAL_INIT(Fix16, dword_679E70, Fix16(0), 0x679E70);
DEFINE_GLOBAL_INIT(Fix16, dword_679E78, Fix16(2), 0x679E78);
DEFINE_GLOBAL_INIT(Fix16, dword_679C78, dword_679E78, 0x679C78);
DEFINE_GLOBAL_INIT(Ang16, word_679FC4, Ang16(0), 0x679FC4);
DEFINE_GLOBAL(CranePool_D9C*, gCranePool_D9C_679FD4, 0x679FD4);
DEFINE_GLOBAL_INIT(Fix16, dword_679D50, dword_679E70, 0x679D50);
DEFINE_GLOBAL_INIT(Fix16, dword_679F8C, dword_679E78, 0x679F8C);
DEFINE_GLOBAL_INIT(Fix16, dword_679E7C, Fix16(0xC000, 0), 0x679E7C);
DEFINE_GLOBAL_INIT(Fix16, dword_679F88, dword_679E7C, 0x679F88);
DEFINE_GLOBAL_INIT(Fix16, dword_679FC8, Fix16(0x11C, 0), 0x679FC8);
DEFINE_GLOBAL_INIT(Fix16, dword_679F64, dword_679FC8 * 20, 0x679F64);
DEFINE_GLOBAL_INIT(Fix16, dword_679F58, Fix16(0x18F60, 0), 0x679F58);
DEFINE_GLOBAL_INIT(Fix16, dword_679E80, Fix16(0x10000, 0), 0x679E80);
DEFINE_GLOBAL_INIT(Fix16, dword_679F68, dword_679E80, 0x679F68);
DEFINE_GLOBAL_INIT(Fix16, dword_679CB0, Fix16(0x2000, 0), 0x679CB0);
DEFINE_GLOBAL_INIT(Fix16, dword_679C3C, dword_679CB0, 0x679C3C);
DEFINE_GLOBAL_INIT(Fix16, dword_679E74, Fix16(0x4000, 0), 0x679E74);

// TODO: Should match but doesn't
WIP_FUNC(0x47e5b0)
Crane_15C::~Crane_15C()
{
    WIP_IMPLEMENTED;
    if (field_7C)
    {
        gRoot_sound_66B038.DestroySoundObj_40FE60(field_7C);
        field_7C = 0;
    }
}

MATCH_FUNC(0x47e610)
Crane_15C::Crane_15C()
{
    field_7C = 0;
}

WIP_FUNC(0x47e620)
void Crane_15C::ComputeHookPos_47E620(Fix16 radius, Ang16 ang, Fix16_Point* pOutPoint)
{
    WIP_IMPLEMENTED;
    pOutPoint->SetXY_432860(dword_679E70, radius);
    pOutPoint->RotateByAngle_40F6B0(ang);
    *pOutPoint += field_2C->field_4->get_x_y_443580();
}

WIP_FUNC(0x47e730)
void Crane_15C::ComputeHookPos_47E730(Fix16 radius, Ang16 ang, Fix16_Point* pOutPoint)
{
    WIP_IMPLEMENTED;
    pOutPoint->SetXY_432860(dword_679E70, radius);
    pOutPoint->RotateByAngle_40F6B0(ang);
    *pOutPoint += field_2C->field_4->get_x_y_443580();
}

STUB_FUNC(0x47e840)
s32 Crane_15C::sub_47E840(s32 a2, s32 a3)
{
    NOT_IMPLEMENTED;
    return 0;
}

// 9.6f 0x448090
MATCH_FUNC(0x47e920)
bool Crane_15C::sub_47E920()
{
    Fix16_Point pos;
    ComputeHookPos_47E620(field_114, Ang16::Fix16_To_Ang16_40F540(field_110), &pos);
    pos += field_8;

    field_60->set_xyz_lazy_420600(pos.x, pos.y, this->field_80 - this->field_11C);
    field_60->set_ang_lazy_420690(Ang16::Fix16_To_Ang16_40F540(field_118));

    return !gPurpleDoom_1_679208->FindNearestSpriteOfType_477E60(field_60, sprite_types_enum::unknown_0) &&
        !field_60->CheckSpriteMovementRegion_5A2500();
}

// 9.6f 0x448150
MATCH_FUNC(0x47eb00)
bool Crane_15C::sub_47EB00()
{
    Fix16_Point hookPos;
    ComputeHookPos_47E620(field_120, Ang16::Fix16_To_Ang16_40F540(field_124), &hookPos);
    hookPos += field_18;

    field_60->set_xyz_lazy_420600(hookPos.x, hookPos.y, field_80 - field_12C);
    field_60->set_ang_lazy_420690(Ang16::Fix16_To_Ang16_40F540(field_128));

    return gPurpleDoom_1_679208->FindNearestSpriteOfType_477E60(field_60, sprite_types_enum::unknown_0) == 0;
}

MATCH_FUNC(0x47ecc0)
void Crane_15C::sub_47ECC0()
{
    Car_BC* pCar = field_74->AsCar_40FEB0();
    gPurpleDoom_1_679208->AddToRegionBuckets_477B20(field_74);
    pCar->sub_4435F0();
    pCar->SetupCarPhysicsAndSpriteBinding_43BCA0();
    gCar_BC_Pool_67792C->UpdateNextPrev(pCar);
    pCar->field_88 = 1;

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
                field_28_strct4.AddSprite_5A6CD0(field_74);
            }
        }
        else
        {
            field_28_strct4.AddSprite_5A6CD0(field_74);
        }
    }
    field_74 = 0;
    field_150 = 0;
    Crane_15C::sub_47F170();
}

MATCH_FUNC(0x47ed60)
void Crane_15C::sub_47ED60()
{
    Car_BC* pCar = field_74->AsCar_40FEB0();
    gCar_BC_Pool_67792C->UpdateNextPrev(pCar);
    pCar->field_88 = 1;
    gPurpleDoom_1_679208->AddToRegionBuckets_477B20(field_74);
    field_64->DispatchCollisionEvent_5A3100(field_74, dword_679E70, dword_679E70, word_679FC4);
    field_28_strct4.AddSprite_5A6CD0(field_64);
    field_74 = 0;
    field_150 = 0;
    field_64 = 0;
    Crane_15C::sub_47F170();
}

// 9.6 0x448300
STUB_FUNC(0x47edf0)
s32 Crane_15C::sub_47EDF0()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x47ef80)
s32 Crane_15C::sub_47EF80()
{
    NOT_IMPLEMENTED;
    return 0;
}

// 9.6f 0x447D40
WIP_FUNC(0x47f170)
void Crane_15C::sub_47F170()
{
    WIP_IMPLEMENTED;

    if (this->field_150)
    {
        this->field_150 = 3;
        this->field_114 = field_90_hook_radius;
        this->field_110 = field_8C_crane_angle;
        this->field_118 = field_A0_hook_axial_angle;
        this->field_11C = dword_679C78;
        this->field_B0_hook_radius_target = field_90_hook_radius;
        this->field_B4_hook_angle_target = field_A0_hook_axial_angle;
        this->field_AC_crane_angle_target = field_8C_crane_angle;
        this->field_B8_hook_depth_target = dword_679C78;
    }
    else
    {
        this->field_B0_hook_radius_target = dword_679E58;
        this->field_AC_crane_angle_target = field_A8;
        this->field_B4_hook_angle_target = dword_679D50;
        this->field_B8_hook_depth_target = dword_679E70;
    }
    this->field_14D_is_busy = 0;
}

MATCH_FUNC(0x47f220)
s32 Crane_15C::sub_47F220(s32 a2, s32 a3, Sprite* a4, Sprite* a5)
{
    field_F4 = a2;
    field_F8 = a3;
    field_6C = a4;
    field_70 = a5;
    field_108 = Ang16::Ang16_to_Fix16(a4->field_0);
    field_FC = a4->field_14_xy.x;
    field_100 = a4->field_14_xy.y;
    s32 result = a4->field_1C_zpos.mValue;
    field_104.mValue = result;
    field_10C = field_80.mValue - result;
    return result;
}

MATCH_FUNC(0x47f290)
void Crane_15C::sub_47F290(Fix16 a2, Fix16 a3, Sprite* a4)
{
    field_BC = a2;
    field_C0 = a3;
    field_64 = a4;
    field_D0 = Ang16::Ang16_to_Fix16(a4->field_0);
    field_C4.x = a4->field_14_xy.x;
    field_C4.y = a4->field_14_xy.y;
    field_CC = a4->field_1C_zpos;
    field_D4 = field_80 - field_CC;
}

MATCH_FUNC(0x47f2f0)
void Crane_15C::sub_47F2F0(s32 a2, s32 a3, Sprite* a4)
{
    field_D8 = a2;
    field_DC = a3;
    field_68 = a4;
    field_EC = Ang16::Ang16_to_Fix16(a4->field_0);
    field_E0 = a4->field_14_xy.x;
    field_E4 = a4->field_14_xy.y;
    Fix16 field_1C_zpos = a4->field_1C_zpos;
    field_E8 = field_1C_zpos;
    field_F0 = field_80.mValue - field_1C_zpos.mValue;
}

MATCH_FUNC(0x47f350)
bool Crane_15C::sub_47F350()
{
    Car_BC* v2 = field_70->AsCar_40FEB0();
    if (v2->field_88 != 5)
    {
        Sprite* v3 = field_6C;
        Car_BC* v4 = v3->field_30_sprite_type_enum == 2 ? v3->field_8_car_bc_ptr : 0;
        if (v4->field_88 != 5 && field_FC == v3->field_14_xy.x && field_100 == v3->field_14_xy.y && field_104 == v3->field_1C_zpos &&
            field_108 == Ang16::Ang16_to_Fix16(v3->field_0))
        {
            return true;
        }
    }
    return false;
}

STUB_FUNC(0x47f3d0)
bool Crane_15C::sub_47F3D0()
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x47f450)
bool Crane_15C::sub_47F450()
{
    Car_BC* pCar = field_64->AsCar_40FEB0();
    if (!pCar->sub_4215B0() && this->field_C4.x == field_64->field_14_xy.x && this->field_C4.y == field_64->field_14_xy.y &&
        this->field_CC == field_64->field_1C_zpos && this->field_D0 == Ang16::Ang16_to_Fix16(field_64->field_0))
    {
        return pCar->field_0_qq.FirstSpriteOfType_5A6CA0(sprite_types_enum::car) ? false : true;
    }
    return false;
}

STUB_FUNC(0x47f4c0)
void Crane_15C::UpdateCraneTargets_47F4C0()
{
    NOT_IMPLEMENTED;
}

// 9.6f 0x448900
WIP_FUNC(0x47f6c0)
char_type Crane_15C::ComputeHookPolar_47F6C0(Fix16_Point* pPoint, Fix16* pOutF16, Ang16* pOutAng)
{
    WIP_IMPLEMENTED;

    Fix16_Point v10 = (*pPoint - field_2C->field_4->get_x_y_443580());
    *pOutF16 = v10.GetLength_2(); // TODO: Uses dword_679E70 as Zero

    // TODO: 1st check is removed in 9.6f ??
    if (*pOutF16 > dword_679F68 || *pOutF16 < dword_679C3C)
    {
        return 0;
    }

    *pOutAng = Ang16::Ang16_to_Fix16(v10.atan2_40F790());
    return 1;
}

STUB_FUNC(0x47f7f0)
char_type Crane_15C::sub_47F7F0(Car_BC* a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

// 9.6f 0x448A80
WIP_FUNC(0x47f930)
void Crane_15C::PickUpCar_47F930(Car_BC* pCar)
{
    WIP_IMPLEMENTED;

    if (pCar->field_88 != 5 && !field_28_strct4.TagSpriteWithRng_5A6C10(pCar->field_50_car_sprite))
    {
        if (pCar->Is_TRUKTRNS_447EC0())
        {
            sub_47F7F0(pCar);
        }
        else if (this->field_64 || (field_144 == 1) || (field_144 == 2 || field_144 == 3) && field_155 == 1 && pCar->field_9C != 7 ||
                 this->field_155 == 2 && pCar->Is_F9_Eq7_447EB0())
        {
            if (!this->field_150 && !pCar->field_54_driver)
            {
                if (pCar->sub_441A40())
                {
                    if (pCar->sub_447F00())
                    {
                        Trailer* pTrailer = pCar->field_64_pTrailer;
                        if (!pTrailer || pTrailer->field_C_pCarOnTrailer == 0 || !pTrailer->field_C_pCarOnTrailer->Is_TRUKTRNS_447EC0())
                        {
                            gHud_2B00_706620->field_DC.SetHudBrief_5D4400(1, "nespray");
                            field_28_strct4.AddSprite_5A6CD0(pCar->field_50_car_sprite);
                            field_28_strct4.TagSpriteWithRng_5A6C10(pCar->field_50_car_sprite);
                        }
                    }
                    else
                    {
                        Fix16_Point sprite_xy = pCar->field_50_car_sprite->get_x_y_443580();
                        Fix16 a2a;
                        Ang16 angTmp;
                        if (ComputeHookPolar_47F6C0(&sprite_xy, &a2a, &angTmp))
                        {
                            if (this->field_144 != 1 || sub_47EB00())
                            {
                                if (field_68 == 0 || pCar->field_50_car_sprite == field_68)
                                {
                                    // TODO: Args
                                    sub_47F2F0(a2a.mValue, angTmp.rValue, pCar->field_50_car_sprite);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

inline Fix16 __stdcall sub_40E790(Fix16 a2)
{
    while (a2 < dword_679E70)
    {
        a2 += dword_679F58;
    }

    if (a2 >= dword_679F58)
    {
        do
        {
            a2 -= dword_679F58;
        } while (a2 >= dword_679F58);
    }
    return a2;
}

// 9.6f 0x448C00
MATCH_FUNC(0x47fb40)
void Crane_15C::sub_47FB40()
{
    if (field_114 == dword_679F8C)
    {
        field_114 = dword_679F88;
    }
    else
    {
        field_114 = dword_679F8C;
        field_110 = sub_40E790(field_110 + dword_679F64);
    }
}

STUB_FUNC(0x47fba0)
s32 Crane_15C::sub_47FBA0()
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x47fd10)
void Crane_15C::sub_47FD10()
{
    if (field_140)
    {
        gfrosty_pasteur_6F8060->sub_511B10(field_140);
    }

    switch (field_150)
    {
        case 1:
            sub_47ED60();
            break;

        case 2:
        case 3:
        case 4:
            sub_47ECC0();
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

    u8 v6 = Crane_15C::sub_47FBA0();

    if (old_crane_angle != field_8C_crane_angle || old_hook_radius != field_90_hook_radius ||
        old_hook_axial_angle != field_A0_hook_axial_angle || old_hook_depth != field_84_hook_depth)
    {
        Crane_15C::sub_47FE10();
    }
    if (v6 != 0)
    {
        if (field_6C)
        {
            Crane_15C::sub_47EDF0();
        }
        else if (field_68 && (field_64 || (field_144 == 1) || field_144 == 2 || field_144 == 3))
        {
            Crane_15C::sub_47EF80();
        }
        else
        {
            Crane_15C::sub_47FD10();
        }
    }
}

STUB_FUNC(0x47fe10)
void Crane_15C::sub_47FE10()
{
    NOT_IMPLEMENTED;
}

MATCH_FUNC(0x480310)
void Crane_15C::Service_480310()
{
    field_159 = 0;
    field_28_strct4.RemoveByRngValue_5A6C40(rng_dword_67AB34->field_0_rng - 1);
    if (field_74)
    {
        gPurpleDoom_3_679210->Remove_477B00(field_74);
    }
    Crane_15C::UpdateCraneTargets_47F4C0();
    if (!field_148)
    {
        if (!field_78_maybe_homecrane || field_8C_crane_angle != field_A8 ||
            (field_78_maybe_homecrane->field_8C_crane_angle == field_78_maybe_homecrane->field_A8))
        {
            Crane_15C::UpdateCraneTick_47FD50();
        }
    }

    if (field_74)
    {
        field_74->AsCar_40FEB0()->sub_443330();
    }

    if (field_74)
    {
        gPurpleDoom_3_679210->AddToSingleBucket_477AE0(field_74);
    }
}

MATCH_FUNC(0x4803b0)
void Crane_15C::sub_4803B0(Fix16 x_pos, Fix16 y_pos, char_type a4)
{
    field_144 = 0;
    field_148 = 0;

    Fix16 temp_z;
    Fix16 v6 = *gMap_0x370_6F6268->FindGroundZForCoord_4E5B60(&temp_z, x_pos, y_pos);

    field_80 = v6;
    field_2C = gObject_5C_6F8F84->NewPhysicsObj_5299B0(135, x_pos, y_pos, v6, word_679FC4);
    field_30 = gObject_5C_6F8F84->NewPhysicsObj_5299B0(134, x_pos, y_pos, field_80, word_679FC4);
    field_34 = gObject_5C_6F8F84->NewPhysicsObj_5299B0(134, x_pos, y_pos, field_80, word_679FC4);
    field_38 = gObject_5C_6F8F84->NewPhysicsObj_5299B0(134, x_pos, y_pos, field_80, word_679FC4);
    field_3C = gObject_5C_6F8F84->NewPhysicsObj_5299B0(134, x_pos, y_pos, field_80, word_679FC4);
    field_40 = gObject_5C_6F8F84->NewPhysicsObj_5299B0(252, x_pos, y_pos, field_80, word_679FC4);
    field_44 = gObject_5C_6F8F84->NewPhysicsObj_5299B0(260, x_pos, y_pos, field_80, word_679FC4);
    field_48 = gObject_5C_6F8F84->NewPhysicsObj_5299B0(261, x_pos, y_pos, field_80, word_679FC4);
    field_4C = gObject_5C_6F8F84->NewPhysicsObj_5299B0(262, x_pos, y_pos, field_80, word_679FC4);
    field_50 = gObject_5C_6F8F84->NewPhysicsObj_5299B0(263, x_pos, y_pos, field_80, word_679FC4);
    field_5C = gObject_5C_6F8F84->NewPhysicsObj_5299B0(140, x_pos, y_pos, field_80, word_679FC4);
    field_54 = gObject_5C_6F8F84->NewPhysicsObj_5299B0(136, x_pos, y_pos, field_80, word_679FC4);

    field_58 = gObject_5C_6F8F84->NewPhysicsObj_5299B0(137, x_pos, y_pos, field_80 - dword_679C78, word_679FC4);
    field_58->field_26_varrok_idx = a4;
    field_78_maybe_homecrane = 0;
    field_94 = dword_679E70;
    field_98 = dword_679E70;
    field_9C = dword_679E70;
    field_A8 = dword_679E70;
    field_8C_crane_angle = dword_679E70;
    field_90_hook_radius = dword_679E58;
    field_84_hook_depth = dword_679E70;
    field_88 = dword_679E70;
    field_A0_hook_axial_angle = dword_679E70;
    field_A4 = dword_679E70;
    field_AC_crane_angle_target = field_8C_crane_angle;
    field_B0_hook_radius_target = field_90_hook_radius;
    field_14D_is_busy = 0;
    field_0 = 0;
    field_4 = 0;
    field_B4_hook_angle_target = field_8C_crane_angle;
    field_B8_hook_depth_target = dword_679E70;
    field_68 = 0;
    field_E0 = dword_679E70;
    field_E4 = dword_679E70;
    field_E8 = dword_679E70;
    field_EC = dword_679E70;
    field_F0 = dword_679E70;
    field_6C = 0;
    field_70 = 0;
    field_FC = dword_679E70;
    field_100 = dword_679E70;
    field_104 = dword_679E70;
    field_108 = dword_679E70;
    field_10C = dword_679E70;
    field_74 = 0;
    field_154 = 0;
    field_150 = 0;
    field_114 = dword_679E70;
    field_110 = dword_679E70;
    field_118 = dword_679E70;
    field_8.x = 0;
    field_8.y = 0;
    field_11C = dword_679E70;

    Sprite* current_sprite = gSprite_Pool_703818->get_new_sprite();
    field_60 = current_sprite;
    current_sprite->AllocInternal_59F950(dword_679E70, dword_679E70, dword_679E70);
    field_14C = 60;
    field_BC = dword_679E70;
    field_C0 = dword_679E70;
    field_64 = 0;
    field_D0 = dword_679E70;
    field_C4.x = dword_679E70;
    field_C4.y = dword_679E70;
    field_CC = dword_679E70;
    field_D4 = dword_679E70;
    field_120 = dword_679E70;
    field_124 = dword_679E70;
    field_128 = dword_679E70;
    field_18.x = 0;
    field_18.y = 0;
    field_12C = dword_679E70;
    field_130 = dword_679E70;
    field_134 = dword_679E70;
    field_138 = dword_679E70;
    field_20.x = 0;
    field_20.y = 0;
    field_13C = dword_679E70;
    field_155 = 1;
    Crane_15C::sub_47FE10();
    field_156 = 0;
    field_157 = 0;
    field_158 = 0;
    field_159 = 0;
    field_140 = 0;
    if (!field_7C && !bSkip_audio_67D6BE)
    {
        field_7C = gRoot_sound_66B038.CreateSoundObject_40EF40(this, SoundObjectTypeEnum::Crane_15C_8);
    }
    field_28_strct4.field_0_p18 = 0;
}

STUB_FUNC(0x480900)
s32* Crane_15C::sub_480900(Fix16 a2, Fix16 a3, Ang16 a4)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x480b60)
s32* Crane_15C::sub_480B60(Fix16 a2, Fix16 a3, Ang16 a4)
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x480da0)
Car_BC* Crane_15C::GetCarFromCrane_480DA0()
{
    if (field_74)
    {
        return field_74->AsCar_40FEB0();
    }
    return 0;
}

MATCH_FUNC(0x480e00)
void CranePool_D9C::PickUpCar_480E00(Car_BC* a2, u8 a3)
{
    field_0[a3].PickUpCar_47F930(a2);
}

MATCH_FUNC(0x480e50)
void CranePool_D9C::CranesService_480E50()
{
    s32 i = 0;
    Crane_15C* pIter = field_0;
    while (i < field_D98_count)
    {
        pIter->Service_480310();
        i++;
        pIter++;
    }
}

MATCH_FUNC(0x480ec0)
Crane_15C* CranePool_D9C::sub_480EC0(Fix16 x_pos, Fix16 y_pos)
{
    Crane_15C* v4 = &field_0[field_D98_count];
    v4->sub_4803B0(x_pos, y_pos, field_D98_count);
    field_D98_count++;
    return v4;
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