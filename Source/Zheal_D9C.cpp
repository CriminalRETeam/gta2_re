#include "Zheal_D9C.hpp"
#include "Globals.hpp"
#include "root_sound.hpp"
#include "map_0x370.hpp"
#include "Object_5C.hpp"
#include "sprite.hpp"
#include "debug.hpp"

EXPORT_VAR s32 dword_679E58;
GLOBAL(dword_679E58, 0x679E58);

EXPORT_VAR Fix16 dword_679E70;
GLOBAL(dword_679E70, 0x679E70);

EXPORT_VAR Fix16 dword_679C78;
GLOBAL(dword_679C78, 0x679C78);

EXPORT_VAR Ang16 dword_679FC4;
GLOBAL(dword_679FC4, 0x679FC4);

// TODO: Should match but doesn't
STUB_FUNC(0x47e5b0)
Crane_15C::~Crane_15C()
{
    if (field_7C)
    {
        gRoot_sound_66B038.DestroySoundObj_40FE60(field_7C);
        field_7C = 0;
    }
}

MATCH_FUNC(0x47e610)
Crane_15C::Crane_15C()
{
    field_28 = 0;
    field_7C = 0;
}

STUB_FUNC(0x47e620)
s32 Crane_15C::sub_47E620(s32 a2, s32 a3, s32* a4)
{
    return 0;
}

STUB_FUNC(0x47e730)
s32 Crane_15C::sub_47E730(s32 a2, s32 a3, u32 a4)
{
    return 0;
}

STUB_FUNC(0x47e840)
s32 Crane_15C::sub_47E840(s32 a2, s32 a3)
{
    return 0;
}

STUB_FUNC(0x47e920)
bool Crane_15C::sub_47E920()
{
    return 0;
}

STUB_FUNC(0x47eb00)
bool Crane_15C::sub_47EB00()
{
    return 0;
}

STUB_FUNC(0x47ecc0)
s32 Crane_15C::sub_47ECC0()
{
    return 0;
}

STUB_FUNC(0x47ed60)
s32 Crane_15C::sub_47ED60()
{
    return 0;
}

STUB_FUNC(0x47edf0)
s32 Crane_15C::sub_47EDF0()
{
    return 0;
}

STUB_FUNC(0x47ef80)
s32 Crane_15C::sub_47EF80()
{
    return 0;
}

STUB_FUNC(0x47f170)
s32 Crane_15C::sub_47F170()
{
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
    field_FC = a4->field_14_xpos;
    field_100 = a4->field_18_ypos;
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
    field_C4 = a4->field_14_xpos;
    field_C8 = a4->field_18_ypos;
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
    field_E0 = a4->field_14_xpos;
    field_E4 = a4->field_18_ypos;
    Fix16 field_1C_zpos = a4->field_1C_zpos;
    field_E8 = field_1C_zpos;
    field_F0 = field_80.mValue - field_1C_zpos.mValue;
}

MATCH_FUNC(0x47f350)
bool Crane_15C::sub_47F350()
{
    Car_BC* v2;
    Sprite* v1 = field_70;

    if (v1->field_30_sprite_type_enum == 2)
    {
        v2 = v1->field_8_car_bc_ptr;
    }
    else
    {
        v2 = 0;
    }

    bool result = false;

    if (v2->field_88 != 5)
    {
        Sprite* v3 = field_6C;
        Car_BC* v4 = v3->field_30_sprite_type_enum == 2 ? v3->field_8_car_bc_ptr : 0;
        if (v4->field_88 != 5 
            && field_FC == v3->field_14_xpos 
            && field_100 == v3->field_18_ypos 
            && field_104 == v3->field_1C_zpos
            && field_108 == Ang16::Ang16_to_Fix16(v3->field_0))
        {
            return true;
        }
    }
    return result;
}

STUB_FUNC(0x47f3d0)
bool Crane_15C::sub_47F3D0()
{
    return 0;
}

STUB_FUNC(0x47f450)
bool Crane_15C::sub_47F450()
{
    return 0;
}

STUB_FUNC(0x47f4c0)
void Crane_15C::sub_47F4C0()
{
}

STUB_FUNC(0x47f6c0)
char_type Crane_15C::sub_47F6C0(u32* a2, s32* a3, u32* a4)
{
    return 0;
}

STUB_FUNC(0x47f7f0)
char_type Crane_15C::sub_47F7F0(u32* a2)
{
    return 0;
}

STUB_FUNC(0x47f930)
void Crane_15C::sub_47F930(Car_BC* a2)
{
}

STUB_FUNC(0x47fb40)
s32 Crane_15C::sub_47FB40()
{
    return 0;
}

STUB_FUNC(0x47fba0)
s32 Crane_15C::sub_47FBA0()
{
    return 0;
}

STUB_FUNC(0x47fd10)
s32 Crane_15C::sub_47FD10()
{
    return 0;
}

STUB_FUNC(0x47fd50)
char_type Crane_15C::sub_47FD50()
{
    return 0;
}

STUB_FUNC(0x47fe10)
void Crane_15C::sub_47FE10()
{
}

STUB_FUNC(0x480310)
void Crane_15C::sub_480310()
{
}

MATCH_FUNC(0x4803b0)
infallible_turing* Crane_15C::sub_4803B0(Fix16 x_pos, Fix16 y_pos, char_type a4)
{
    infallible_turing* result;

    field_144 = 0;
    field_148 = 0;

    Fix16 temp_z;
    Fix16 v6 = *gMap_0x370_6F6268->FindGroundZForCoord_4E5B60(&temp_z, x_pos, y_pos);

    field_80 = v6;
    field_2C = gObject_5C_6F8F84->sub_5299B0(135, x_pos, y_pos, v6, dword_679FC4);
    field_30 = gObject_5C_6F8F84->sub_5299B0(134, x_pos, y_pos, field_80, dword_679FC4);
    field_34 = gObject_5C_6F8F84->sub_5299B0(134, x_pos, y_pos, field_80, dword_679FC4);
    field_38 = gObject_5C_6F8F84->sub_5299B0(134, x_pos, y_pos, field_80, dword_679FC4);
    field_3C = gObject_5C_6F8F84->sub_5299B0(134, x_pos, y_pos, field_80, dword_679FC4);
    field_40 = gObject_5C_6F8F84->sub_5299B0(252, x_pos, y_pos, field_80, dword_679FC4);
    field_44 = gObject_5C_6F8F84->sub_5299B0(260, x_pos, y_pos, field_80, dword_679FC4);
    field_48 = gObject_5C_6F8F84->sub_5299B0(261, x_pos, y_pos, field_80, dword_679FC4);
    field_4C = gObject_5C_6F8F84->sub_5299B0(262, x_pos, y_pos, field_80, dword_679FC4);
    field_50 = gObject_5C_6F8F84->sub_5299B0(263, x_pos, y_pos, field_80, dword_679FC4);
    field_5C = gObject_5C_6F8F84->sub_5299B0(140, x_pos, y_pos, field_80, dword_679FC4);
    field_54 = gObject_5C_6F8F84->sub_5299B0(136, x_pos, y_pos, field_80, dword_679FC4);

    field_58 = gObject_5C_6F8F84->sub_5299B0(137, x_pos, y_pos, field_80 - dword_679C78, dword_679FC4);
    field_58->field_26 = a4;
    field_78 = 0;
    field_94 = dword_679E70;
    field_98 = dword_679E70;
    field_9C = dword_679E70;
    field_A8 = dword_679E70;
    field_8C = dword_679E70;
    field_90 = dword_679E58;
    field_84 = dword_679E70;
    field_88 = dword_679E70;
    field_A0 = dword_679E70;
    field_A4 = dword_679E70;
    field_AC = field_8C;
    field_B0 = field_90;
    field_14D = 0;
    field_0 = 0;
    field_4 = 0;
    field_B4 = field_8C;
    field_B8 = dword_679E70;
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

    Sprite* current_sprite = gSprite_49B28_703818->field_0_first_free;
    gSprite_49B28_703818->field_0_first_free = gSprite_49B28_703818->field_0_first_free->field_C_sprite_next_ptr;
    current_sprite->sub_5A2CF0();
    field_60 = current_sprite;
    current_sprite->sub_59F950(dword_679E70, dword_679E70, dword_679E70);
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
    result = field_7C;
    field_156 = 0;
    field_157 = 0;
    field_158 = 0;
    field_159 = 0;
    field_140 = 0;
    if (!result && !bSkip_audio_67D6BE)
    {
        result = gRoot_sound_66B038.CreateSoundObject_40EF40(this, 8);
        field_7C = result;
    }
    field_28 = 0;
    return result;
}

STUB_FUNC(0x480900)
s32* Crane_15C::sub_480900(s32 a2, s32 a3, s32 a4)
{
    return 0;
}

STUB_FUNC(0x480b60)
s32* Crane_15C::sub_480B60(s32 a2, s32 a3, s32 a4)
{
    return 0;
}

MATCH_FUNC(0x480e00)
void Zheal_D9C::sub_480E00(Car_BC* a2, u8 a3)
{
    field_0[a3].sub_47F930(a2);
}

MATCH_FUNC(0x480e50)
void Zheal_D9C::sub_480E50()
{
    s32 i = 0;
    Crane_15C* pIter = field_0;
    while (i < field_D98_count)
    {
        pIter->sub_480310();
        i++;
        pIter++;
    }
}

MATCH_FUNC(0x480ec0)
Crane_15C* Zheal_D9C::sub_480EC0(Fix16 x_pos, Fix16 y_pos)
{
    Crane_15C* v4 = &field_0[field_D98_count];
    v4->sub_4803B0(x_pos, y_pos, field_D98_count);
    field_D98_count++;
    return v4;
}

MATCH_FUNC(0x480f50)
Zheal_D9C::Zheal_D9C()
{
    field_D98_count = 0;
}

MATCH_FUNC(0x4bbbf0)
Zheal_D9C::~Zheal_D9C()
{
}
