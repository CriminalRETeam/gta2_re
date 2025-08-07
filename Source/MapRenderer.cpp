#include "MapRenderer.hpp"
#include "DrawUnk_0xBC.hpp"
#include "Globals.hpp"
#include "debug.hpp"
#include "fix16.hpp"
#include "map_0x370.hpp"

DEFINE_GLOBAL(MapRenderer*, gpMapRenderer_6F66E4, 0x6F66E4);
DEFINE_GLOBAL(Fix16_Point, stru_6F6484, 0x6F6484);
DEFINE_GLOBAL(u16, gBlockLeft_6F62F6, 0x6F62F6);
DEFINE_GLOBAL(u16, gBlockTop_6F62F4, 0x6F62F4);
DEFINE_GLOBAL(u16, gBlockRight_6F63C6, 0x6F63C6);
DEFINE_GLOBAL(u16, gBlockBottom_6F6468, 0x6F6468);
DEFINE_GLOBAL(u16, gLidType_6F6274, 0x6F6274);
DEFINE_GLOBAL(Fix16, gXCoord_6F63AC, 0x6F63AC);
DEFINE_GLOBAL(Fix16, gYCoord_6F63B8, 0x6F63B8);
DEFINE_GLOBAL(s32, gZCoord_6F63E0, 0x6F63E0);
DEFINE_GLOBAL(u32, dword_6F6480, 0x6F6480);
DEFINE_GLOBAL(u32, dword_6F647C, 0x6F647C);
DEFINE_GLOBAL(gmp_block_info*, gpBlock_6F6478, 0x6F6478);
DEFINE_GLOBAL(gmp_map_slope, dword_6F646C, 0x6F646C);
DEFINE_GLOBAL_ARRAY(Vert, gTileVerts_6F65A8, 8, 0x6F65A8);

// FUNCTION: 105 0x4e9d50
void MapRenderer::sub_4E9D50(s32& target_level, u16& cycles)
{
    field_4 = Fix16(target_level, 0);

    if (cycles > 0)
    {
        field_8 = (field_4 - field_0_ambient) / Fix16(cycles);
    }
    else
    {
        field_0_ambient = field_4;
        field_8 = stru_6F6484.x;
    }
}

STUB_FUNC(0x4e9db0)
char_type MapRenderer::set_shading_lev_4E9DB0(u8 shading_lev)
{
    NOT_IMPLEMENTED;
    return 0;
}

// FUNCTION: 105 0x4e9ea0
void MapRenderer::ambient_light_tick_4E9EA0()
{
    field_0_ambient = field_8 + field_0_ambient;

    if (field_8 > stru_6F6484.x)
    {
        if (field_0_ambient >= field_4)
        {
            field_8 = stru_6F6484.x;
            field_0_ambient = field_4;
        }
    }
    else if (field_8 < stru_6F6484.x)
    {
        if (field_0_ambient <= field_4)
        {
            field_8 = stru_6F6484.x;
            field_0_ambient = field_4;
        }
    }
}

STUB_FUNC(0x4ea390)
void MapRenderer::sub_4EA390(u16* a2)
{
    NOT_IMPLEMENTED;
}

// FUNCTION: 105 0x4ead90
void MapRenderer::set_vert_xyz_relative_to_cam_4EAD90(Fix16 xCoord, Fix16 yCoord, Fix16 z_val, Vert* pVerts)
{
    DrawUnk_0xBC* pCam = gViewCamera_676978;

    s32 next_idx = (pVerts - gTileVerts_6F65A8) + 4;

    gTileVerts_6F65A8[next_idx].x = (xCoord + pCam->field_98_cam_pos2.field_0_x).ToFloat();
    gTileVerts_6F65A8[next_idx].y = (yCoord + pCam->field_98_cam_pos2.field_4_y).ToFloat();
    gTileVerts_6F65A8[next_idx].z = z_val.ToFloat();
}

STUB_FUNC(0x4eae00)
s32 MapRenderer::sub_4EAE00(s32* a2, s32* a3, Vert* a4)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x4eaea0)
s32 MapRenderer::sub_4EAEA0(s32* a2, s32* a3, Vert* a4)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x4eaf40)
void MapRenderer::sub_4EAF40(u16* a2)
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x4eba60)
void MapRenderer::sub_4EBA60(u16* a2)
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x4ec450)
s16 MapRenderer::sub_4EC450(u16* a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x4ec7a0)
void MapRenderer::sub_4EC7A0(u16* a2)
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x4ecaf0)
s16 MapRenderer::sub_4ECAF0(u16* a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x4ece40)
s16 MapRenderer::sub_4ECE40(u16* a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x4ed290)
void MapRenderer::draw_bottom_4ED290(u16* a2)
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x4ee130)
void MapRenderer::draw_lid_4EE130()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x4eeaf0)
void MapRenderer::sub_4EEAF0()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x4eee60)
void MapRenderer::sub_4EEE60()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x4ef1c0)
void MapRenderer::sub_4EF1C0()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x4ef520)
void MapRenderer::sub_4EF520()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x4ef880)
void MapRenderer::sub_4EF880()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x4efb20)
void MapRenderer::sub_4EFB20()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x4efdb0)
void MapRenderer::sub_4EFDB0()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x4f0030)
void MapRenderer::sub_4F0030()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x4f02d0)
void MapRenderer::sub_4F02D0()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x4f0340)
void MapRenderer::sub_4F0340()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x4f0420)
void MapRenderer::sub_4F0420()
{
    NOT_IMPLEMENTED;
}

// FUNCTION: 105 0x4f0bd0
char_type MapRenderer::GetColour_4F0BD0(s32 lid_type)
{
    switch (lid_type)
    {
        case 0:
            return -1;
        case 1:
            return this->field_C_colour_t1;
        case 2:
            return this->field_E_colour_t2;
        case 3:
            return this->field_F_colour_t3;
        default:
            return 0;
    }
}

STUB_FUNC(0x4f1660)
void MapRenderer::sub_4F1660()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x4f22f0)
void MapRenderer::sub_4F22F0()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x4f33b0)
void MapRenderer::sub_4F33B0()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x4f3c00)
void MapRenderer::draw_left_4F3C00(u16* arg0, s32* pVertIdx, s32 a2, Fix16_Point* a5)
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x4f4190)
void MapRenderer::Set_UV_4F4190(Fix16_Point* a1, Fix16_Point* a2, u32* pVertIdx)
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x4f4250)
void MapRenderer::sub_4F4250(u16* arg0, s32* pVertIdx, s32 a2, Fix16_Point* a5)
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x4f4600)
void MapRenderer::sub_4F4600(u16* a2, s32 a3, Fix16_Point* a4, u32* a5)
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x4f49b0)
void MapRenderer::sub_4F49B0(u16* a2, s32 a1, Fix16_Point* a4, u32* pVertIdx)
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x4f4d60)
void MapRenderer::draw_lid_4F4D60(Fix16_Point* xpos, Fix16_Point* diffuse_colour, s32 arg_8, u32* a5)
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x4f6580)
void MapRenderer::draw_slope_4F6580()
{
    NOT_IMPLEMENTED;
}

// FUNCTION: 105 0x4f6630
void MapRenderer::draw_slope_4F6630()
{
    u32 slope_idx = (gpBlock_6F6478->field_B_slope_type >> 2);

    if (!bSkip_slopes_67D505)
    {
        u8 tmp = update_and_get_f0(slope_idx);

        switch (tmp)
        {
            case 1:
                MapRenderer::sub_4F0420();
                break;
            case 2:
                MapRenderer::sub_4F1660();
                break;
            case 3:
                MapRenderer::sub_4F22F0();
                break;
            case 4:
                MapRenderer::sub_4F33B0();
                break;
            default:
                return;
        }
    }
}

// https://decomp.me/scratch/8po7Q  instruction swap at lines (0xbc vs 0xc2) and (0x125 vs 0x12b)
STUB_FUNC(0x4f66c0)
void MapRenderer::sub_4F66C0()
{
    NOT_IMPLEMENTED;
    u16 v6;
    dword_6F646C.field_0_gradient_direction = 0;

    if (gBlockLeft_6F62F6 && gBlockRight_6F63C6)
    {
        if ((gBlockRight_6F63C6 & 0x1000) != 0)
        {
            v6 = gBlockLeft_6F62F6 | 0x1000;
            MapRenderer::draw_left_4F3C00(&v6, &stru_6F6484.y.mValue, (s32)&stru_6F6484, (Fix16_Point*)&stru_6F6484.y);
        }
        if ((gBlockLeft_6F62F6 & 0x1000) != 0)
        {
            v6 = gBlockRight_6F63C6 | 0x1000;
            MapRenderer::sub_4F4250(&v6, &stru_6F6484.x.mValue, (s32)&stru_6F6484, (Fix16_Point*)&stru_6F6484.y);
        }
    }

    if (gBlockTop_6F62F4 && gBlockBottom_6F6468)
    {
        if ((gBlockBottom_6F6468 & 0x1000) != 0)
        {
            v6 = gBlockTop_6F62F4 | 0x1000;
            MapRenderer::sub_4F4600(&v6, (s32)&stru_6F6484, (Fix16_Point*)&stru_6F6484.y, (u32*)&stru_6F6484.y.mValue);
        }
        if ((gBlockTop_6F62F4 & 0x1000) != 0)
        {
            v6 = *(u32*)&gBlockBottom_6F6468 | 0x1000;
            MapRenderer::sub_4F49B0(&v6, (s32)&stru_6F6484, (Fix16_Point*)&stru_6F6484.y, (u32*)&stru_6F6484.y.mValue);
        }
    }
    if (gBlockLeft_6F62F6) // line 103
    {
        if (gBlockRight_6F63C6 == 0 || (gBlockRight_6F63C6 & 0x1000) == 0 || (gBlockLeft_6F62F6 & 0x1000) != 0)
        {
            MapRenderer::sub_4EA390(&gBlockLeft_6F62F6);
        }
    }
    if (gBlockRight_6F63C6)
    {
        if (gBlockLeft_6F62F6 == 0 || (gBlockLeft_6F62F6 & 0x1000) == 0 || (gBlockRight_6F63C6 & 0x1000) != 0)
        {
            MapRenderer::sub_4EAF40(&gBlockRight_6F63C6);
        }
    }
    if (gBlockTop_6F62F4)
    {
        if (!gBlockBottom_6F6468 || (gBlockBottom_6F6468 & 0x1000) == 0 || (gBlockTop_6F62F4 & 0x1000) != 0)
        {
            MapRenderer::sub_4EBA60(&gBlockTop_6F62F4);
        }
    }
    if (gBlockBottom_6F6468)
    {
        if (gBlockTop_6F62F4 == 0 || (gBlockTop_6F62F4 & 0x1000) == 0 || (gBlockBottom_6F6468 & 0x1000) != 0)
        {
            MapRenderer::draw_bottom_4ED290(&gBlockBottom_6F6468);
        }
    }
    if (gLidType_6F6274)
    {
        MapRenderer::draw_lid_4EE130();
    }
}

// FUNCTION: 105 0x4f6880
void MapRenderer::sub_4F6880(s32& pXCoord, s32& pYCoord)
{
    gmp_block_info* pBlock = gMap_0x370_6F6268->sub_4DFEE0(pXCoord, pYCoord, gZCoord_6F63E0);
    gpBlock_6F6478 = pBlock;
    if (pBlock)
    {
        gBlockLeft_6F62F6 = pBlock->field_0_left;
        gBlockRight_6F63C6 = pBlock->field_2_right;
        gBlockTop_6F62F4 = pBlock->field_4_top;
        gBlockBottom_6F6468 = pBlock->field_6_bottom;

        u16 v8 = pBlock->field_8_lid;
        gLidType_6F6274 = v8;

        if (bShow_hidden_faces_67D5CD)
        {
            if (!v8 && (pBlock->field_B_slope_type & 3) != 0)
            {
                gLidType_6F6274 = 0x260;
            }
            if ((gBlockLeft_6F62F6 & 0x400) != 0 && (gBlockLeft_6F62F6 & 0x3FF) == 0)
            {
                gBlockLeft_6F62F6 = gBlockLeft_6F62F6 | 0x260;
            }
            if ((gBlockRight_6F63C6 & 0x400) != 0 && (gBlockRight_6F63C6 & 0x3FF) == 0)
            {
                gBlockRight_6F63C6 |= 0x260u;
            }
            if ((gBlockTop_6F62F4 & 0x400) != 0 && (gBlockTop_6F62F4 & 0x3FF) == 0)
            {
                gBlockTop_6F62F4 |= 0x260u;
            }
            if ((gBlockBottom_6F6468 & 0x400) != 0 && (gBlockBottom_6F6468 & 0x3FF) == 0)
            {
                gBlockBottom_6F6468 = gBlockBottom_6F6468 | 0x260;
            }
        }
        gXCoord_6F63AC = Fix16(pXCoord) - gViewCamera_676978->field_98_cam_pos2.field_0_x;
        gYCoord_6F63B8 = Fix16(pYCoord) - gViewCamera_676978->field_98_cam_pos2.field_4_y;

        u8 v6 = pBlock->field_B_slope_type & 0xFC;

        if (v6 < 0xB4u || v6 > 0xC0u)
        {
            if (v6 < 0xC4u || v6 > 0xD0u)
            {
                if (v6 < 0xD4u || v6 > 0xF4u)
                {
                    if (v6 > 0 && v6 < 0xB4u)
                    {
                        MapRenderer::draw_slope_4F6630();
                    }
                    else
                    {
                        MapRenderer::sub_4F66C0(); // !
                    }
                }
                else
                {
                    MapRenderer::draw_slope_4F6580();
                }
            }
            else
            {
                MapRenderer::sub_4F0340();
            }
        }
        else
        {
            MapRenderer::sub_4F02D0();
        }
    }
}

// FUNCTION: 105 0x4f6a10
void MapRenderer::ClearDrawnTileCount_4F6A10()
{
    field_2F00_drawn_tile_count = 0;
}

STUB_FUNC(0x4f6a20)
void MapRenderer::Draw_4F6A20()
{
    NOT_IMPLEMENTED;
}