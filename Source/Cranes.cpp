#include "Cranes.hpp"
#include "Globals.hpp"
#include "root_sound.hpp"
#include "map_0x370.hpp"
#include "Object_5C.hpp"
#include "sprite.hpp"
#include "debug.hpp"
#include "PurpleDoom.hpp"
#include "rng.hpp"

DEFINE_GLOBAL(s32, dword_679E58, 0x679E58);
DEFINE_GLOBAL_INIT(Fix16, dword_679E70, Fix16(0), 0x679E70);
DEFINE_GLOBAL_INIT(Fix16, dword_679E78, Fix16(2), 0x679E78);
DEFINE_GLOBAL_INIT(Fix16, dword_679C78, dword_679E78, 0x679C78);
DEFINE_GLOBAL_INIT(Ang16, word_679FC4, Ang16(0), 0x679FC4);
DEFINE_GLOBAL(CranePool_D9C*, gCranePool_D9C_679FD4, 0x679FD4);

// TODO: Should match but doesn't
STUB_FUNC(0x47e5b0)
Crane_15C::~Crane_15C()
{
    NOT_IMPLEMENTED;
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

STUB_FUNC(0x47e620)
s32 Crane_15C::sub_47E620(s32 a2, s32 a3, s32* a4)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x47e730)
s32 Crane_15C::sub_47E730(s32 a2, s32 a3, u32 a4)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x47e840)
s32 Crane_15C::sub_47E840(s32 a2, s32 a3)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x47e920)
bool Crane_15C::sub_47E920()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x47eb00)
bool Crane_15C::sub_47EB00()
{
    NOT_IMPLEMENTED;
    return 0;
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

STUB_FUNC(0x47f170)
s32 Crane_15C::sub_47F170()
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x47f220)
s32 Crane_15C::sub_47F220(s32 a2, s32 a3, Sprite* a4, Sprite* a5)
{
    field_F4 = a2;
    field_F8 = a3;
    field_6C = a4;
    field_70 = a5;
    field_108 = Ang16::Ang16_to_Fix16(a4->field_0);
    field_FC = a4->field_14_xpos.x;
    field_100 = a4->field_14_xpos.y;
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
    field_C4 = a4->field_14_xpos.x;
    field_C8 = a4->field_14_xpos.y;
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
    field_E0 = a4->field_14_xpos.x;
    field_E4 = a4->field_14_xpos.y;
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
        if (v4->field_88 != 5 
            && field_FC == v3->field_14_xpos.x 
            && field_100 == v3->field_14_xpos.y 
            && field_104 == v3->field_1C_zpos
            && field_108 == Ang16::Ang16_to_Fix16(v3->field_0))
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

STUB_FUNC(0x47f450)
bool Crane_15C::sub_47F450()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x47f4c0)
void Crane_15C::UpdateCraneTargets_47F4C0()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x47f6c0)
char_type Crane_15C::sub_47F6C0(u32* a2, s32* a3, u32* a4)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x47f7f0)
char_type Crane_15C::sub_47F7F0(u32* a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x47f930)
void Crane_15C::sub_47F930(Car_BC* a2)
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x47fb40)
s32 Crane_15C::sub_47FB40()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x47fba0)
s32 Crane_15C::sub_47FBA0()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x47fd10)
s32 Crane_15C::sub_47FD10()
{
    NOT_IMPLEMENTED;
    return 0;
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
        if (!field_78_maybe_homecrane || field_8C_crane_angle != field_A8 || (field_78_maybe_homecrane->field_8C_crane_angle == field_78_maybe_homecrane->field_A8))
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
    field_8 = 0;
    field_C = 0;
    field_11C = dword_679E70;

    Sprite* current_sprite = gSprite_Pool_703818->get_new_sprite();
    field_60 = current_sprite;
    current_sprite->AllocInternal_59F950(dword_679E70, dword_679E70, dword_679E70);
    field_14C = 60;
    field_BC = dword_679E70;
    field_C0 = dword_679E70;
    field_64 = 0;
    field_D0 = dword_679E70;
    field_C4 = dword_679E70;
    field_C8 = dword_679E70;
    field_CC = dword_679E70;
    field_D4 = dword_679E70;
    field_120 = dword_679E70;
    field_124 = dword_679E70;
    field_128 = dword_679E70;
    field_18 = 0;
    field_1C = 0;
    field_12C = dword_679E70;
    field_130 = dword_679E70;
    field_134 = dword_679E70;
    field_138 = dword_679E70;
    field_20 = 0;
    field_24 = 0;
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
s32* Crane_15C::sub_480900(s32 a2, s32 a3, s32 a4)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x480b60)
s32* Crane_15C::sub_480B60(s32 a2, s32 a3, s32 a4)
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
    field_0[a3].sub_47F930(a2);
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