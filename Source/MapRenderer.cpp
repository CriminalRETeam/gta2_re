#include "MapRenderer.hpp"
#include "Camera.hpp"
#include "winmain.hpp"
#include "gbh_graphics.hpp"
#include "Globals.hpp"
#include "debug.hpp"
#include "fix16.hpp"
#include "Light_1D4CC.hpp"
#include "map_0x370.hpp"
#include "Montana.hpp"
#include "sharp_pare_0x15D8.hpp"
#include "winmain.hpp"

DEFINE_GLOBAL(MapRenderer*, gpMapRenderer_6F66E4, 0x6F66E4);
DEFINE_GLOBAL_INIT(Fix16_Point, stru_6F6484, Fix16_Point(Fix16(0), Fix16(1)), 0x6F6484);
DEFINE_GLOBAL(u16, gBlockLeft_6F62F6, 0x6F62F6);
DEFINE_GLOBAL(u16, gBlockTop_6F62F4, 0x6F62F4);
DEFINE_GLOBAL(u16, gBlockRight_6F63C6, 0x6F63C6);
DEFINE_GLOBAL(u16, gBlockBottom_6F6468, 0x6F6468);
DEFINE_GLOBAL(u16, gLidType_6F6274, 0x6F6274);
DEFINE_GLOBAL(Fix16, gRelativeXCoord_6F63AC, 0x6F63AC);
DEFINE_GLOBAL(Fix16, gRelativeYCoord_6F63B8, 0x6F63B8);
DEFINE_GLOBAL(s32, gZCoord_6F63E0, 0x6F63E0);
DEFINE_GLOBAL(u32, dword_6F6480, 0x6F6480);
DEFINE_GLOBAL(u32, dword_6F647C, 0x6F647C);
DEFINE_GLOBAL(gmp_block_info*, gpBlock_6F6478, 0x6F6478);
DEFINE_GLOBAL(gmp_map_slope, dword_6F646C, 0x6F646C);
DEFINE_GLOBAL_ARRAY(Vert, gTileVerts_6F65A8, 8, 0x6F65A8);

DEFINE_GLOBAL(s32, dword_6F62B0, 0x6F62B0);
DEFINE_GLOBAL(u32, dword_6F6560, 0x6F6560);
DEFINE_GLOBAL(Fix16, dword_6F628C, 0x6F628C);
DEFINE_GLOBAL(Fix16, dword_6F656C, 0x6F656C);
DEFINE_GLOBAL(Fix16, dword_6F6318, 0x6F6318);
DEFINE_GLOBAL(Fix16, dword_6F633C, 0x6F633C);
DEFINE_GLOBAL(Fix16, dword_6F6518, 0x6F6518);
DEFINE_GLOBAL_INIT(s8, byte_620F20, -1, 0x620F20);  // OBS: workaround
DEFINE_GLOBAL_ARRAY_INIT(s32, dword_620FA4, 8, 0x620FA4, 0, 8, 0x20, 0x28, 0x40, 0x48, 0x60, 0x68);
DEFINE_GLOBAL_ARRAY_INIT(s32, dword_621004, 8, 0x621004, 0x65, 0x75, 5, 0x15, 0x25, 0x35, 0x45, 0x55);
DEFINE_GLOBAL_ARRAY_INIT(s32, dword_621024, 8, 0x621024, 5, 0x0D, 0x25, 0x2D, 0x45, 0x4D, 0x65, 0x6D);
DEFINE_GLOBAL_ARRAY_INIT(s32, dword_620FE4, 8, 0x620FE4, 0x25, 0x35, 0x45, 0x55, 0x65, 0x75, 5, 0x15);
DEFINE_GLOBAL_ARRAY_INIT(s32, dword_620F84, 8, 0x620F84, 2, 0xA, 0x22, 0x2A, 0x42, 0x4A, 0x62, 0x6A);
DEFINE_GLOBAL_ARRAY_INIT(s32, dword_620F44, 8, 0x620F44, 0x64, 0x74, 0x4, 0x14, 0x24, 0x34, 0x44, 0x54);
DEFINE_GLOBAL_ARRAY_INIT(s32, dword_620F24, 8, 0x620F24, 0x23, 0x33, 0x43, 0x53, 0x63, 0x73, 0x3, 0x13);
DEFINE_GLOBAL_ARRAY_INIT(s32, dword_620F64, 8, 0x620F64, 0x41, 0x49, 0x61, 0x69, 0x1, 0x9, 0x21, 0x29);
DEFINE_GLOBAL_INIT(Fix16, dword_6F638C, Fix16(0x3000,0), 0x6F638C);

static inline void sub_46BD40(Fix16& x, Fix16& y, Vert* pVert)
{
    set_vert_xyz_relative_to_cam_4EAD90(x, y, dword_6F62B0, pVert);
    pVert->x = (x.ToFloat()) * (dword_6F628C.ToFloat()) + gViewCamera_676978->field_70_screen_px_center_x;
    pVert->y = (y.ToFloat()) * (dword_6F628C.ToFloat()) + gViewCamera_676978->field_74_screen_px_center_y;
    pVert->z = dword_6F656C.ToFloat();
}

MATCH_FUNC(0x4e9d50)
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

MATCH_FUNC(0x4e9ea0)
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

// https://decomp.me/scratch/FGVOK
STUB_FUNC(0x4ea390)
void MapRenderer::sub_4EA390(u16& left_word)
{
    NOT_IMPLEMENTED;

    if (!bSkip_left_67D6BF)
    {
        if (dword_6F646C.field_0_gradient_direction == 1)
        {
            set_vert_xyz_relative_to_cam_4EAD90(gRelativeXCoord_6F63AC, gRelativeYCoord_6F63B8, gZCoord_6F63E0, &gTileVerts_6F65A8[0]);

            // ...
        }
        else if (dword_6F646C.field_0_gradient_direction == 2)
        {
            // ...
        }
        else
        {
            // Flat blocks
            MapRenderer::sub_4EAEA0(gRelativeXCoord_6F63AC, gRelativeYCoord_6F63B8, &gTileVerts_6F65A8[0]);
            MapRenderer::sub_4EAE00(gRelativeXCoord_6F63AC, gRelativeYCoord_6F63B8, &gTileVerts_6F65A8[1]);
            if (gTileVerts_6F65A8[0].x > gTileVerts_6F65A8[1].x)
            {
                return;
            }
            MapRenderer::sub_4EAEA0(gRelativeXCoord_6F63AC, gRelativeYCoord_6F63B8 + stru_6F6484.y, &gTileVerts_6F65A8[3]);
            MapRenderer::sub_4EAE00(gRelativeXCoord_6F63AC, gRelativeYCoord_6F63B8 + stru_6F6484.y, &gTileVerts_6F65A8[2]);
            dword_6F6560 = dword_620F24[left_word >> 13];
        }

        u16 texture_idx = gGtx_0x106C_703DD4->sub_5AA870(left_word & 0x3FF);
        if (texture_idx)
        {
            BlockSideWord block_side_word = *(BlockSideWord*)&left_word;
            //if ((*((_BYTE*)v3 + 1) & 0x10) != 0)
            if (block_side_word.flat)
            {
                dword_6F6560 = dword_6F6560 | 0x80;
            }
            pgbh_DrawTile(dword_6F6560 | gLightingDrawFlag_7068F4,
                          gSharp_pare_0x15D8_705064->field_0_textures1[texture_idx],
                          gTileVerts_6F65A8,
                          field_C_colour_t1);
            ++field_2F00_drawn_tile_count;
        }
    }
}

MATCH_FUNC(0x4ead90)
void __stdcall set_vert_xyz_relative_to_cam_4EAD90(Fix16 xCoord, Fix16 yCoord, Fix16 z_val, Vert* pVerts)
{
    Camera_0xBC* pCam = gViewCamera_676978;

    s32 next_idx = (pVerts - gTileVerts_6F65A8) + 4;

    gTileVerts_6F65A8[next_idx].x = (xCoord + pCam->field_98_cam_pos2.field_0_x).ToFloat();
    gTileVerts_6F65A8[next_idx].y = (yCoord + pCam->field_98_cam_pos2.field_4_y).ToFloat();
    gTileVerts_6F65A8[next_idx].z = z_val.ToFloat();
}

// This function matches, but we need to replace "0x4C0A8C" by "0x6F628C"
// and "0x4C0A80" by "0x6F656C" on compiled assembly
STUB_FUNC(0x4eae00)
void MapRenderer::sub_4EAE00(Fix16& xpos, Fix16& ypos, Vert* pVert)
{
    set_vert_xyz_relative_to_cam_4EAD90(xpos, ypos, dword_6F62B0, pVert);
    
    pVert->x = xpos.ToFloat() * dword_6F628C.ToFloat() + (u32)gViewCamera_676978->field_70_screen_px_center_x;
    pVert->y = ypos.ToFloat() * dword_6F628C.ToFloat() + (u32)gViewCamera_676978->field_74_screen_px_center_y;
    pVert->z = dword_6F656C.ToFloat();
}

// This function matches, but we need to replace "0x4C0AAC" by "0x6F633C"
// and "0x4C0BCC" by "0x6F6318" on compiled assembly
STUB_FUNC(0x4eaea0)
void MapRenderer::sub_4EAEA0(Fix16& xCoord, Fix16& yCoord, Vert* pVert)
{
    set_vert_xyz_relative_to_cam_4EAD90(xCoord, yCoord, gZCoord_6F63E0, pVert);

    pVert->x = (u32)gViewCamera_676978->field_70_screen_px_center_x + dword_6F633C.ToFloat() * xCoord.ToFloat();
    pVert->y = (u32)gViewCamera_676978->field_74_screen_px_center_y + dword_6F633C.ToFloat() * yCoord.ToFloat();
    pVert->z = dword_6F6318.ToFloat();
}

// https://decomp.me/scratch/mWsfM
STUB_FUNC(0x4eaf40)
void MapRenderer::sub_4EAF40(u16& right_word)
{
    NOT_IMPLEMENTED;

    if (!bSkip_right_67D4E4)
    {
        if (dword_6F646C.field_0_gradient_direction == 1)
        {
            set_vert_xyz_relative_to_cam_4EAD90(gRelativeXCoord_6F63AC + stru_6F6484.y, gRelativeYCoord_6F63B8, gZCoord_6F63E0, &gTileVerts_6F65A8[1]);

            // ...
        }
        else if (dword_6F646C.field_0_gradient_direction == 2)
        {
            // ...
        }
        else
        {
            // Flat blocks
            MapRenderer::sub_4EAEA0(gRelativeXCoord_6F63AC + stru_6F6484.y, gRelativeYCoord_6F63B8, &gTileVerts_6F65A8[1]);
            MapRenderer::sub_4EAE00(gRelativeXCoord_6F63AC + stru_6F6484.y, gRelativeYCoord_6F63B8, &gTileVerts_6F65A8[0]);
            if (gTileVerts_6F65A8[0].x > gTileVerts_6F65A8[1].x)
            {
                return;
            }
            MapRenderer::sub_4EAEA0(gRelativeXCoord_6F63AC + stru_6F6484.y, gRelativeYCoord_6F63B8 + stru_6F6484.y, &gTileVerts_6F65A8[2]);
            MapRenderer::sub_4EAE00(gRelativeXCoord_6F63AC + stru_6F6484.y, gRelativeYCoord_6F63B8 + stru_6F6484.y, &gTileVerts_6F65A8[3]);
            dword_6F6560 = dword_620F44[right_word >> 13];
        }

        u16 texture_idx = gGtx_0x106C_703DD4->sub_5AA870(right_word & 0x3FF);
        if (texture_idx)
        {
            BlockSideWord block_side_word = *(BlockSideWord*)&right_word;
            //if ((*((_BYTE*)v6 + 1) & 0x10) != 0)
            if (block_side_word.flat)
            {
                dword_6F6560 = dword_6F6560 | 0x80;
            }
            pgbh_DrawTile(dword_6F6560 | gLightingDrawFlag_7068F4,
                          gSharp_pare_0x15D8_705064->field_0_textures1[texture_idx],
                          gTileVerts_6F65A8,
                          0); // field_D
            ++field_2F00_drawn_tile_count;
        }
    }
}

// https://decomp.me/scratch/a6z18
STUB_FUNC(0x4eba60)
void MapRenderer::sub_4EBA60(u16& top_word)
{
    NOT_IMPLEMENTED;

    if (!bSkip_top_67D574)
    {
        if (dword_6F646C.field_0_gradient_direction == 3)
        {
            set_vert_xyz_relative_to_cam_4EAD90(gRelativeXCoord_6F63AC, gRelativeYCoord_6F63B8, gZCoord_6F63E0, &gTileVerts_6F65A8[3]);

            // ...
        }
        else if (dword_6F646C.field_0_gradient_direction == 4)
        {
            // ...
        }
        else
        {
            // Flat blocks
            MapRenderer::sub_4EAEA0(gRelativeXCoord_6F63AC, gRelativeYCoord_6F63B8, &gTileVerts_6F65A8[0]);
            MapRenderer::sub_4EAE00(gRelativeXCoord_6F63AC, gRelativeYCoord_6F63B8, &gTileVerts_6F65A8[3]);
            if (gTileVerts_6F65A8[0].y > gTileVerts_6F65A8[3].y)
            {
                return;
            }
            MapRenderer::sub_4EAEA0(gRelativeXCoord_6F63AC + stru_6F6484.y, gRelativeYCoord_6F63B8, &gTileVerts_6F65A8[1]);
            MapRenderer::sub_4EAE00(gRelativeXCoord_6F63AC + stru_6F6484.y, gRelativeYCoord_6F63B8, &gTileVerts_6F65A8[2]);
            dword_6F6560 = dword_620F64[top_word >> 13];
        }

        u16 texture_idx = gGtx_0x106C_703DD4->sub_5AA870(top_word & 0x3FF);
        if (texture_idx != 0)
        {
            BlockSideWord block_side_word = *(BlockSideWord*)&top_word;
            //if ((*((u8*)top_word + 1) & 0x10) != 0)
            if (block_side_word.flat)
            {
                dword_6F6560 = dword_6F6560 | 0x80;
            }
            pgbh_DrawTile(dword_6F6560 | gLightingDrawFlag_7068F4,
                          gSharp_pare_0x15D8_705064->field_0_textures1[texture_idx],
                          gTileVerts_6F65A8,
                          0); // field_E_colour_t2
            ++field_2F00_drawn_tile_count;
        }
    }
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

// https://decomp.me/scratch/4EDti
STUB_FUNC(0x4ed290)
void MapRenderer::draw_bottom_4ED290(u16& bottom_word)
{
    NOT_IMPLEMENTED;

    if (!bSkip_bottom_67D4E7)
    {
        if (dword_6F646C.field_0_gradient_direction == 3)
        {
            set_vert_xyz_relative_to_cam_4EAD90(gRelativeXCoord_6F63AC, gRelativeYCoord_6F63B8 + stru_6F6484.y, gZCoord_6F63E0, &gTileVerts_6F65A8[3]);

            // ...
        }
        else if (dword_6F646C.field_0_gradient_direction == 4)
        {
            // ...
        }
        else
        {
            // Flat blocks
            MapRenderer::sub_4EAEA0(gRelativeXCoord_6F63AC, gRelativeYCoord_6F63B8 + stru_6F6484.y, &gTileVerts_6F65A8[3]);
            MapRenderer::sub_4EAE00(gRelativeXCoord_6F63AC, gRelativeYCoord_6F63B8 + stru_6F6484.y, &gTileVerts_6F65A8[0]);
            if (gTileVerts_6F65A8[0].y > gTileVerts_6F65A8[3].y)
            {
                return;
            }
            MapRenderer::sub_4EAEA0(gRelativeXCoord_6F63AC + stru_6F6484.y, gRelativeYCoord_6F63B8 + stru_6F6484.y, &gTileVerts_6F65A8[2]);
            MapRenderer::sub_4EAE00(gRelativeXCoord_6F63AC + stru_6F6484.y, gRelativeYCoord_6F63B8 + stru_6F6484.y, &gTileVerts_6F65A8[1]);
            dword_6F6560 = dword_620F84[bottom_word >> 13];
        }

        u16 texture_idx = gGtx_0x106C_703DD4->sub_5AA870(bottom_word & 0x3FF);
        if (texture_idx != 0)
        {
            BlockSideWord block_word = *(BlockSideWord*)&bottom_word;
            if (block_word.flat)
            {
                dword_6F6560 = dword_6F6560 | 0x80;
            }
            
            pgbh_DrawTile(dword_6F6560 | gLightingDrawFlag_7068F4,
                          gSharp_pare_0x15D8_705064->field_0_textures1[texture_idx],
                          gTileVerts_6F65A8,
                          0);   // field_E_colour_t2
            ++field_2F00_drawn_tile_count;
        }
    }
}

// https://decomp.me/scratch/pJOfC
STUB_FUNC(0x4ee130)
void MapRenderer::draw_lid_4EE130()
{
    if (!bSkip_lid_67D546)
    {
        sub_46BD40(gRelativeXCoord_6F63AC, gRelativeYCoord_6F63B8, &gTileVerts_6F65A8[0]);
        sub_46BD40(gRelativeXCoord_6F63AC + stru_6F6484.y, gRelativeYCoord_6F63B8, &gTileVerts_6F65A8[1]);
        sub_46BD40(gRelativeXCoord_6F63AC + stru_6F6484.y, gRelativeYCoord_6F63B8 + stru_6F6484.y, &gTileVerts_6F65A8[2]);
        sub_46BD40(gRelativeXCoord_6F63AC, gRelativeYCoord_6F63B8 + stru_6F6484.y, &gTileVerts_6F65A8[3]);

        u16 texture_idx = gGtx_0x106C_703DD4->sub_5AA870(gLidType_6F6274 & 0x3FF); // tile idx

        if (texture_idx)
        {
            switch (byte_620F20)
            {
                case -1:
                    dword_6F6560 = dword_620FA4[(u16)gLidType_6F6274 >> 13];
                    break;
                case 0:
                    memcpy(&gTileVerts_6F65A8[0], &gTileVerts_6F65A8[1], sizeof(Vert));
                    memcpy(&gTileVerts_6F65A8[4], &gTileVerts_6F65A8[5], sizeof(Vert));
                    memcpy(&gTileVerts_6F65A8[1], &gTileVerts_6F65A8[2], sizeof(Vert));
                    memcpy(&gTileVerts_6F65A8[5], &gTileVerts_6F65A8[6], sizeof(Vert));
                    memcpy(&gTileVerts_6F65A8[2], &gTileVerts_6F65A8[3], sizeof(Vert));
                    memcpy(&gTileVerts_6F65A8[6], &gTileVerts_6F65A8[7], sizeof(Vert));
                    memcpy(&gTileVerts_6F65A8[3], &gTileVerts_6F65A8[0], sizeof(Vert));
                    memcpy(&gTileVerts_6F65A8[7], &gTileVerts_6F65A8[4], sizeof(Vert));
                    dword_6F6560 = dword_621004[(u16)gLidType_6F6274 >> 13];
                    break;
                case 1:
                    memcpy(&gTileVerts_6F65A8[1], &gTileVerts_6F65A8[2], sizeof(Vert));
                    memcpy(&gTileVerts_6F65A8[5], &gTileVerts_6F65A8[6], sizeof(Vert));
                    dword_6F6560 = dword_621024[(u16)gLidType_6F6274 >> 13];
                    break;
                case 2:
                    memcpy(&gTileVerts_6F65A8[2], &gTileVerts_6F65A8[1], sizeof(Vert));
                    memcpy(&gTileVerts_6F65A8[6], &gTileVerts_6F65A8[5], sizeof(Vert));
                    memcpy(&gTileVerts_6F65A8[1], &gTileVerts_6F65A8[0], sizeof(Vert));
                    memcpy(&gTileVerts_6F65A8[5], &gTileVerts_6F65A8[4], sizeof(Vert));
                    memcpy(&gTileVerts_6F65A8[0], &gTileVerts_6F65A8[3], sizeof(Vert));
                    memcpy(&gTileVerts_6F65A8[4], &gTileVerts_6F65A8[7], sizeof(Vert));
                    dword_6F6560 = dword_620FE4[(u16)gLidType_6F6274 >> 13];
                    break;
                case 3:
                    memcpy(&gTileVerts_6F65A8[3], &gTileVerts_6F65A8[0], sizeof(Vert));
                    memcpy(&gTileVerts_6F65A8[7], &gTileVerts_6F65A8[4], sizeof(Vert));
                    dword_6F6560 = dword_621024[(u16)gLidType_6F6274 >> 13];
                    break;
                default:
                    break;
            }
            if ((gLidType_6F6274 & 0x1000) != 0)
            {
                dword_6F6560 |= 0x80;
            }
            //u8 diffuseColour = sub_46B5E0((gLidType_6F6274 >> 10) & 3);
            pgbh_DrawTile(dword_6F6560 | gLightingDrawFlag_7068F4,
                          gSharp_pare_0x15D8_705064->sub_46BB50(texture_idx),
                          gTileVerts_6F65A8,
                          sub_46B5E0((gLidType_6F6274 >> 10) & 3));
            ++field_2F00_drawn_tile_count;
        }
    }
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

MATCH_FUNC(0x4f0340)
void MapRenderer::sub_4F0340()
{
    u8 field_B_slope_type = gpBlock_6F6478->field_B_slope_type;
    dword_6F6480 = 1;
    s32 slope_type = field_B_slope_type & 0xFC;
    dword_6F647C = 0;
    if (gLidType_6F6274 == 1023)
    {
        switch (slope_type)
        {
            case 196:
                MapRenderer::sub_4EEAF0();
                break;
            case 200:
                MapRenderer::sub_4EEE60();
                break;
            case 204:
                MapRenderer::sub_4EF1C0();
                break;
            case 208:
                MapRenderer::sub_4EF520();
                break;
            default:
                return;
        }
    }
    else
    {
        dword_6F646C.field_0_gradient_direction = 0;
        switch (slope_type)
        {
            case 196:
                MapRenderer::sub_4EF880();
                break;
            case 200:
                MapRenderer::sub_4EFB20();
                break;
            case 204:
                MapRenderer::sub_4EFDB0();
                break;
            case 208:
                MapRenderer::sub_4F0030();
                break;
            default:
                return;
        }
    }
}

STUB_FUNC(0x4f0420)
void MapRenderer::sub_4F0420()
{
    NOT_IMPLEMENTED;
}

MATCH_FUNC(0x4f0bd0)
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

MATCH_FUNC(0x4f6630)
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
void MapRenderer::RenderFlatBlock_4F66C0()
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
            MapRenderer::sub_4EA390(gBlockLeft_6F62F6);
        }
    }
    if (gBlockRight_6F63C6)
    {
        if (gBlockLeft_6F62F6 == 0 || (gBlockLeft_6F62F6 & 0x1000) == 0 || (gBlockRight_6F63C6 & 0x1000) != 0)
        {
            MapRenderer::sub_4EAF40(gBlockRight_6F63C6);
        }
    }
    if (gBlockTop_6F62F4)
    {
        if (!gBlockBottom_6F6468 || (gBlockBottom_6F6468 & 0x1000) == 0 || (gBlockTop_6F62F4 & 0x1000) != 0)
        {
            MapRenderer::sub_4EBA60(gBlockTop_6F62F4);
        }
    }
    if (gBlockBottom_6F6468)
    {
        if (gBlockTop_6F62F4 == 0 || (gBlockTop_6F62F4 & 0x1000) == 0 || (gBlockBottom_6F6468 & 0x1000) != 0)
        {
            MapRenderer::draw_bottom_4ED290(gBlockBottom_6F6468);
        }
    }
    if (gLidType_6F6274)
    {
        MapRenderer::draw_lid_4EE130();
    }
}

MATCH_FUNC(0x4f6880)
void MapRenderer::RenderBlockAt_4F6880(s32& pXCoord, s32& pYCoord)
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
        gRelativeXCoord_6F63AC = Fix16(pXCoord) - gViewCamera_676978->field_98_cam_pos2.field_0_x;
        gRelativeYCoord_6F63B8 = Fix16(pYCoord) - gViewCamera_676978->field_98_cam_pos2.field_4_y;

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
                        MapRenderer::RenderFlatBlock_4F66C0(); // !
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

MATCH_FUNC(0x4f6a10)
void MapRenderer::ClearDrawnTileCount_4F6A10()
{
    field_2F00_drawn_tile_count = 0;
}

// https://decomp.me/scratch/VJVUz
MATCH_FUNC(0x4f6a20)
void MapRenderer::Draw_4F6A20()
{
    NOT_IMPLEMENTED;

    // set ambient level
    if (gLighting_626A09)
    {
        pgbh_SetAmbient(field_0_ambient.ToFloat());
        MapRenderer::ambient_light_tick_4E9EA0();
    }

    // render all things
    for (s32 zLayer = 0; zLayer < 7; zLayer++)
    {
        if (zLayer != 0)
        {
            gMontana_67B580->Draw_495560(zLayer); // draw all sprites at zLayer
        }

        // render blocks
        if (!bSkip_tiles_67D655)
        {
            s32 zpos_inverse = 8 - zLayer;
            // compute tile rendering boundaries
            Fix16 layer_row_width = (gViewCamera_676978->field_98_cam_pos2.field_8_z + Fix16(zpos_inverse)) / gViewCamera_676978->field_98_cam_pos2.field_C_zoom;

            // narrow the y direction because of assymetric monitor resolution
            Fix16 layer_column_width = layer_row_width * dword_6F638C;  
            
            // compute x boundary
            
            s32 min_x = (gViewCamera_676978->field_98_cam_pos2.field_0_x - (layer_row_width / 2)).ToInt();
            s32 max_x = (gViewCamera_676978->field_98_cam_pos2.field_0_x + (layer_row_width / 2)).ToInt();
            
            s32 x_semi_distance = (max_x - min_x + 1) / 2;
            if (x_semi_distance % 2 != 1)
            {
                x_semi_distance += 1;
            }
            
            // compute y boundary
            s32 min_y = (gViewCamera_676978->field_98_cam_pos2.field_4_y - (layer_column_width / 2)).ToInt();
            s32 max_y = (gViewCamera_676978->field_98_cam_pos2.field_4_y + (layer_column_width / 2)).ToInt();

            s32 y_semi_distance = (max_y - min_y + 1) / 2;
            if (y_semi_distance % 2 != 1)
            {
                y_semi_distance += 1;
            }
            
            // update global Z coordinate
            gZCoord_6F63E0 = zLayer;    // or maybe zLayer + 1 ?
            dword_6F6518 = Fix16(zLayer);

            // Not known yet
            Fix16 unknown_1;
            Fix16 unk_Z_Factor = gViewCamera_676978->field_98_cam_pos2.field_8_z + Fix16(zpos_inverse);
            if (unk_Z_Factor == stru_6F6484.x) //  != 0
            {
                unknown_1 = stru_6F6484.x;
            }
            else
            {
                unknown_1 = stru_6F6484.y / unk_Z_Factor; //  = 1 / unk_Z_Factor
            }

            // Setting some unknown global vars...
            
            dword_6F6318 = unknown_1; // TODO: not used for now
            dword_6F633C = unknown_1 * gViewCamera_676978->field_60.x;  // TODO: Is this really Fix16_Point?
            dword_6F62B0 = zLayer + 1;
            
            // Not known yet
            Fix16 unknown_2;
            Fix16 unk_Z_Factor_2 = gViewCamera_676978->field_98_cam_pos2.field_8_z + Fix16(8 - (zLayer + 1));
            if (unk_Z_Factor_2 == stru_6F6484.x) //  != 0
            {
                unknown_2 = stru_6F6484.x;
            }
            else
            {
                unknown_2 = stru_6F6484.y / unk_Z_Factor_2; //  = 1 / unk_Z_Factor
            }

            dword_6F656C = unknown_2;
            dword_6F628C = unknown_2 * gViewCamera_676978->field_60.x; // tile scale ?

            // if zLayer = 0, reset lights
            if (zLayer == 0 && gLighting_626A09)
            {
                pgbh_ResetLights();
                pgbh_SetCamera((f32)min_x, (f32)min_y, (f32)max_x, (f32)max_y);
                Light::sub_4D6E50(min_x, min_y, max_x, max_y);
            }

            // Reset the size of draw layer size
            ResetCount_45B040();

            // Now iter over all blocks at zLayer and set up their rendering order
            // Begin with the blocks at the center of the camera and go away

            // In the end render in reverse order: far blocks to the nearest ones

            s32 x_to_render;
            s32 y_to_render;

            for (s32 ypos_rel = y_semi_distance; ypos_rel >= 0; ypos_rel--)
            {
                s32 ypos_downwards = max_y - ypos_rel;
                s32 ypos_upwards = min_y + ypos_rel;
                for (s32 xpos_rel = x_semi_distance; xpos_rel >= 0; xpos_rel--)
                {
                    s32 xpos_right = max_x - xpos_rel;
                    s32 xpos_left = min_x + xpos_rel;
                    sub_46BB90(xpos_right, ypos_downwards);
                    sub_46BB90(xpos_left, ypos_downwards);
                    sub_46BB90(xpos_left, ypos_upwards);
                    sub_46BB90(xpos_right, ypos_upwards);
                }
            }

            // Now draw tiles in reverse order

            Nanobotz_8* pIter = &field_1C[field_2EFC_curr_draw_layer_size-1];
            for (s32 j = field_2EFC_curr_draw_layer_size - 1; j >= 0; j--, pIter--)
            {
                MapRenderer::RenderBlockAt_4F6880(pIter->field_0_x, 
                                        pIter->field_4_y);
            }
        }
    }
    gMontana_67B580->Draw_495560(7);    // draw all sprites on the highest layer
}