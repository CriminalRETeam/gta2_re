#include "Nanobotz.hpp"
#include "Globals.hpp"

EXPORT_VAR Nanobotz* gpNanobotz_6F66E4;
GLOBAL(gpNanobotz_6F66E4, 0x6F66E4);

EXPORT_VAR Fix16 dword_6F6484;
GLOBAL(dword_6F6484, 0x6F6484);

MATCH_FUNC(0x4e9d50)
void Nanobotz::sub_4E9D50(s32& target_level, u16& cycles)
{
    Fix16 tgt_level(target_level, 0);
    field_4 = tgt_level;

    if (cycles > 0)
    {
        Fix16 v7;
        v7.FromUnsignedShort(cycles);
        Fix16 v6 = tgt_level - field_0_ambient;
        field_8 = v6.inline_divide_by(v7);
    }
    else
    {
        field_0_ambient = tgt_level;
        field_8 = dword_6F6484;
    }
}

STUB_FUNC(0x4e9db0)
char_type Nanobotz::set_shading_lev_4E9DB0(u8 shading_lev)
{
    return 0;
}

STUB_FUNC(0x4e9ea0)
void Nanobotz::ambient_light_tick_4E9EA0()
{
}

STUB_FUNC(0x4ea390)
void Nanobotz::sub_4EA390(u16* a2)
{
}

STUB_FUNC(0x4ead90)
f32* Nanobotz::set_vert_xyz_relative_to_cam_4EAD90(s32 xCoord, s32 yCoord, s32 z_val, Vert_DX* pVerts)
{
    return 0;
}

STUB_FUNC(0x4eae00)
s32 Nanobotz::sub_4EAE00(s32* a2, s32* a3, Vert_DX* a4)
{
    return 0;
}

STUB_FUNC(0x4eaea0)
s32 Nanobotz::sub_4EAEA0(s32* a2, s32* a3, Vert_DX* a4)
{
    return 0;
}

STUB_FUNC(0x4eaf40)
void Nanobotz::sub_4EAF40(u16* a2)
{
}

STUB_FUNC(0x4eba60)
void Nanobotz::sub_4EBA60(u16* a2)
{
}

STUB_FUNC(0x4ec450)
s16 Nanobotz::sub_4EC450(u16* a2)
{
    return 0;
}

STUB_FUNC(0x4ec7a0)
void Nanobotz::sub_4EC7A0(u16* a2)
{
}

STUB_FUNC(0x4ecaf0)
s16 Nanobotz::sub_4ECAF0(u16* a2)
{
    return 0;
}

STUB_FUNC(0x4ece40)
s16 Nanobotz::sub_4ECE40(u16* a2)
{
    return 0;
}

STUB_FUNC(0x4ed290)
void Nanobotz::draw_bottom_4ED290(u16* a2)
{
}

STUB_FUNC(0x4ee130)
void Nanobotz::draw_lid_4EE130()
{
}

STUB_FUNC(0x4eeaf0)
void Nanobotz::sub_4EEAF0()
{
}

STUB_FUNC(0x4eee60)
void Nanobotz::sub_4EEE60()
{
}

STUB_FUNC(0x4ef1c0)
void Nanobotz::sub_4EF1C0()
{
}

STUB_FUNC(0x4ef520)
void Nanobotz::sub_4EF520()
{
}

STUB_FUNC(0x4ef880)
void Nanobotz::sub_4EF880()
{
}

STUB_FUNC(0x4efb20)
void Nanobotz::sub_4EFB20()
{
}

STUB_FUNC(0x4efdb0)
void Nanobotz::sub_4EFDB0()
{
}

STUB_FUNC(0x4f0030)
void Nanobotz::sub_4F0030()
{
}

STUB_FUNC(0x4f02d0)
void Nanobotz::sub_4F02D0()
{
}

STUB_FUNC(0x4f0340)
void Nanobotz::sub_4F0340()
{
}

STUB_FUNC(0x4f0420)
void Nanobotz::sub_4F0420()
{
}

STUB_FUNC(0x4f0bd0)
char_type Nanobotz::sub_4F0BD0(s32 a2)
{
    return 0;
}

STUB_FUNC(0x4f1660)
void Nanobotz::sub_4F1660()
{
}

STUB_FUNC(0x4f22f0)
void Nanobotz::sub_4F22F0()
{
}

STUB_FUNC(0x4f33b0)
void Nanobotz::sub_4F33B0()
{
}

STUB_FUNC(0x4f3c00)
void Nanobotz::draw_left_4F3C00(u16* arg0, s32* pVertIdx, s32 a2, Fix16_2* a5)
{
}

STUB_FUNC(0x4f4190)
void Nanobotz::Set_UV_4F4190(Fix16_2* a1, Fix16_2* a2, u32* pVertIdx)
{
}

STUB_FUNC(0x4f4250)
void Nanobotz::sub_4F4250(u16* arg0, s32* pVertIdx, s32 a2, Fix16_2* a5)
{
}

STUB_FUNC(0x4f4600)
void Nanobotz::sub_4F4600(u16* a2, s32 a3, Fix16_2* a4, u32* a5)
{
}

STUB_FUNC(0x4f49b0)
void Nanobotz::sub_4F49B0(u16* a2, s32 a1, Fix16_2* a4, u32* pVertIdx)
{
}

STUB_FUNC(0x4f4d60)
void Nanobotz::draw_lid_4F4D60(Fix16_2* xpos, Fix16_2* diffuse_colour, s32 arg_8, u32* a5)
{
}

STUB_FUNC(0x4f6580)
void Nanobotz::draw_slope_4F6580()
{
}

STUB_FUNC(0x4f6630)
void Nanobotz::draw_slope_4F6630()
{
}

STUB_FUNC(0x4f66c0)
void Nanobotz::sub_4F66C0()
{
}

STUB_FUNC(0x4f6880)
void Nanobotz::sub_4F6880(s32* pXCoord, s32* pYCoord)
{
}

MATCH_FUNC(0x4f6a10)
void Nanobotz::ClearDrawnTileCount_4F6A10()
{
    field_2F00_drawn_tile_count = 0;
}

STUB_FUNC(0x4f6a20)
void Nanobotz::Draw_4F6A20()
{
}
