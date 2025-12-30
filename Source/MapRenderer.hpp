#pragma once

#include "Function.hpp"
#include "Game_0x40.hpp"
#include "map_0x370.hpp"
#include "fix16.hpp"
#include "3rdParty/GTA2Hax/d3ddll/d3ddll.hpp" // Vert

class Fix16_Point;

class Nanobotz_8  // Maybe Fix16_Point
{
  public:
    inline s32 IsEqual_46BB60(Nanobotz_8* other)
    {
        return field_0_x == other->field_0_x && field_4_y == other->field_4_y;
    }
    s32 field_0_x;  // x?
    s32 field_4_y;  // y?
};

EXTERN_GLOBAL(Fix16, gXCoord_6F63AC);

EXTERN_GLOBAL(Fix16, gYCoord_6F63B8);

EXTERN_GLOBAL(s32, gZCoord_6F63E0);


EXTERN_GLOBAL(gmp_map_slope, dword_6F646C);


EXTERN_GLOBAL(u32, dword_6F6480);

EXTERN_GLOBAL(u32, dword_6F647C);

enum MapSlopes
{
    DIAGONAL_WALL_UP_LEFT = 0xB4,
    DIAGONAL_WALL_UP_RIGHT = 0xB8,
    DIAGONAL_WALL_DOWN_LEFT = 0xBC,
    DIAGONAL_WALL_DOWN_RIGHT = 0xC0,
    TRIANGULAR_SIDES_DIAGONAL_UP_LEFT = 0xC4,
    TRIANGULAR_SIDES_DIAGONAL_UP_RIGHT = 0xC8,
    TRIANGULAR_SIDES_DIAGONAL_DOWN_LEFT = 0xCC,
    TRIANGULAR_SIDES_DIAGONAL_DOWN_RIGHT = 0xD0,
};

class MapRenderer
{
  public:
    MapRenderer()
    {
        field_0_ambient = dword_67DCCC;
        field_4 = dword_67DCCC;
        field_8.mValue = 0;
        field_2F00_drawn_tile_count = 0;
        field_2EFC_curr_draw_layer_size = 0;
        set_shading_lev_4E9DB0(15u);
    }

    EXPORT void sub_4E9D50(s32& a2, u16& a3);
    EXPORT char_type set_shading_lev_4E9DB0(u8 shading_lev);
    EXPORT void draw_4E9EE0(u16& word_side, const bool& bUnk, u8& colour);
    EXPORT void ambient_light_tick_4E9EA0();
    EXPORT void draw_4EA190(u16& rotation_and_flip);
    EXPORT void DrawLeftSide_4EA390(u16& left_word);
    EXPORT void sub_4EAE00(Fix16& xpos, Fix16& ypos, Vert* pVert);
    EXPORT void sub_4EAEA0(Fix16& xCoord, Fix16& yCoord, Vert* pVert);
    EXPORT void DrawRightSide_4EAF40(u16& right_word);
    EXPORT void DrawTopSide_4EBA60(u16& top_word);
    EXPORT void sub_4EC450(u16& left_word);
    EXPORT void sub_4EC7A0(u16& right_word);
    EXPORT void sub_4ECAF0(u16& left_word);
    EXPORT void sub_4ECE40(u16& right_word);
    EXPORT void draw_bottom_4ED290(u16& a2);
    EXPORT void draw_lid_4EE130();
    void DrawDiagonalWallUpLeft_4EE7D0();
    void DrawDiagonalWallUpRight_4EE8A0();
    void DrawDiagonalWallDownLeft_4EE970();
    void DrawDiagonalWallDownRight_4EEA40();
    EXPORT void Draw3SidedDiagonalUpLeft_4EEAF0();
    EXPORT void Draw3SidedDiagonalUpRight_4EEE60();
    EXPORT void Draw3SidedDiagonalDownLeft_4EF1C0();
    EXPORT void Draw3SidedDiagonalDownRight_4EF520();
    EXPORT void Draw4SidedDiagonalUpLeft_4EF880();
    EXPORT void Draw4SidedDiagonalUpRight_4EFB20();
    EXPORT void Draw4SidedDiagonalDownLeft_4EFDB0();
    EXPORT void Draw4SidedDiagonalDownRight_4F0030();
    EXPORT void DrawDiagonalWall_4F02D0();
    EXPORT void DrawTriangularDiagonal_4F0340();
    EXPORT void DrawGradientSlopeNorthwards_4F0420();
    EXPORT char_type GetColour_4F0BD0(s32 lid_type);
    EXPORT void DrawGradientSlopeSouthwards_4F1660();
    EXPORT void DrawGradientSlopeWestwards_4F22F0();
    EXPORT void DrawGradientSlopeEastwards_4F33B0();
    EXPORT void draw_left_4F3C00(u16* arg0, s32* pVertIdx, s32 a2, Fix16_Point* a5);
    EXPORT void Set_UV_4F4190(Fix16& a1, Fix16& a2, const u32& pVertIdx);
    EXPORT void sub_4F4250(u16* arg0, s32* pVertIdx, s32 a2, Fix16_Point* a5);
    EXPORT void sub_4F4600(u16* a2, s32 a3, Fix16_Point* a4, u32* a5);
    EXPORT void sub_4F49B0(u16* a2, s32 a1, Fix16_Point* a4, u32* pVertIdx);
    EXPORT void draw_lid_4F4D60(Fix16_Point* xpos, Fix16_Point* diffuse_colour, s32 arg_8, u32* a5);
    EXPORT void DrawPartialBlocks_4F6580();
    EXPORT void DrawGradientSlope_4F6630();
    EXPORT void RenderFlatBlock_4F66C0();
    EXPORT void RenderBlockAt_4F6880(s32& pXCoord, s32& pYCoord);
    EXPORT void ClearDrawnTileCount_4F6A10();
    EXPORT void Draw_4F6A20();

    inline u32 update_and_get_gradient_direction(u32 idx)
    {
        dword_6F6480 = byte_6F5BA8[idx].field_1_gradient_size;
        dword_6F647C = byte_6F5BA8[idx].field_2_gradient_level;
        dword_6F646C.field_0_gradient_direction = byte_6F5BA8[idx].field_0_gradient_direction;
        return dword_6F646C.field_0_gradient_direction;
    }

    inline u8 sub_46B5E0(s32 a1)
    {
        //u8 diffuseColour;
        switch (a1)
        {
            case 0u:
                return -1;
                break;
            case 1u:
                return field_C_colour_t1;
                break;
            case 2u:
                return field_E_colour_t2;
                break;
            case 3u:
                return field_F_colour_t3;
                break;
            default:
                return 0;
                break;
        }
        //return diffuseColour;
    }

    inline void sub_46BB90(s32& maybe_x, s32& maybe_y)
    {
        Nanobotz_8* pPos = &field_1C[field_2EFC_curr_draw_layer_size];
        pPos->field_0_x = maybe_x;
        pPos->field_4_y = maybe_y;
        
        Nanobotz_8* pIter = &field_1C[field_2EFC_curr_draw_layer_size-1];
        for (s32 i = field_2EFC_curr_draw_layer_size - 1; i >= 0; i--, pIter--)
        {
            if (pIter->IsEqual_46BB60(pPos))
            {
                return;
            }
        }
        ++field_2EFC_curr_draw_layer_size;
    }

    inline void ResetCount_45B040()
    {
        field_2EFC_curr_draw_layer_size = 0;
    }

    Fix16 field_0_ambient;
    Fix16 field_4;
    Fix16 field_8;
    u8 field_C_colour_t1;
    u8 field_D;
    u8 field_E_colour_t2;
    u8 field_F_colour_t3;
    u8 field_10;
    u8 field_11;
    u8 field_12;
    u8 field_13;
    u8 field_14_dcolour;
    u8 field_15;
    u8 field_16;
    u8 field_17;
    u8 field_18_color;
    u8 field_19;
    u8 field_1A;
    u8 field_1B;
    Nanobotz_8 field_1C[1500];
    s32 field_2EFC_curr_draw_layer_size;
    s32 field_2F00_drawn_tile_count;
};

EXPORT void __stdcall set_vert_xyz_relative_to_cam_4EAD90(Fix16 xCoord, Fix16 yCoord, Fix16 z_val, Vert* pVerts);

EXPORT void __stdcall sub_4EB940(Fix16& xpos, Fix16& ypos, Fix16& zpos, Vert* pVert);

EXTERN_GLOBAL(MapRenderer*, gpMapRenderer_6F66E4);

struct BlockSideWord
{
    u32 tile_idx : 10;
    u32 wall : 1;
    u32 bullet_wall : 1;
    u32 flat : 1;
    u32 flip : 1;
    u32 rotation_code : 2;
};
