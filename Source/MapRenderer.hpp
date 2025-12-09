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
        return field_0 == other->field_0 && field_4 == other->field_4;
    }
    s32 field_0;  // x?
    s32 field_4;  // y?
};

EXTERN_GLOBAL(Fix16, gRelativeXCoord_6F63AC);

EXTERN_GLOBAL(Fix16, gRelativeYCoord_6F63B8);

EXTERN_GLOBAL(s32, gZCoord_6F63E0);


EXTERN_GLOBAL(gmp_map_slope, dword_6F646C);


EXTERN_GLOBAL(u32, dword_6F6480);

EXTERN_GLOBAL(u32, dword_6F647C);


class MapRenderer
{
  public:
    MapRenderer()
    {
        field_0_ambient = dword_67DCCC;
        field_4 = dword_67DCCC;
        field_8.mValue = 0;
        field_2F00_drawn_tile_count = 0;
        field_2EFC_count = 0;
        set_shading_lev_4E9DB0(15u);
    }

    EXPORT void sub_4E9D50(s32& a2, u16& a3);
    EXPORT char_type set_shading_lev_4E9DB0(u8 shading_lev);
    EXPORT void ambient_light_tick_4E9EA0();
    EXPORT void sub_4EA390(u16* a2);
    EXPORT s32 sub_4EAE00(s32* a2, s32* a3, Vert* a4);
    EXPORT s32 sub_4EAEA0(s32* a2, s32* a3, Vert* a4);
    EXPORT void sub_4EAF40(u16* a2);
    EXPORT void sub_4EBA60(u16* a2);
    EXPORT s16 sub_4EC450(u16* a2);
    EXPORT void sub_4EC7A0(u16* a2);
    EXPORT s16 sub_4ECAF0(u16* a2);
    EXPORT s16 sub_4ECE40(u16* a2);
    EXPORT void draw_bottom_4ED290(u16* a2);
    EXPORT void draw_lid_4EE130();
    EXPORT void sub_4EEAF0();
    EXPORT void sub_4EEE60();
    EXPORT void sub_4EF1C0();
    EXPORT void sub_4EF520();
    EXPORT void sub_4EF880();
    EXPORT void sub_4EFB20();
    EXPORT void sub_4EFDB0();
    EXPORT void sub_4F0030();
    EXPORT void sub_4F02D0();
    EXPORT void sub_4F0340();
    EXPORT void sub_4F0420();
    EXPORT char_type GetColour_4F0BD0(s32 lid_type);
    EXPORT void sub_4F1660();
    EXPORT void sub_4F22F0();
    EXPORT void sub_4F33B0();
    EXPORT void draw_left_4F3C00(u16* arg0, s32* pVertIdx, s32 a2, Fix16_Point* a5);
    EXPORT void Set_UV_4F4190(Fix16_Point* a1, Fix16_Point* a2, u32* pVertIdx);
    EXPORT void sub_4F4250(u16* arg0, s32* pVertIdx, s32 a2, Fix16_Point* a5);
    EXPORT void sub_4F4600(u16* a2, s32 a3, Fix16_Point* a4, u32* a5);
    EXPORT void sub_4F49B0(u16* a2, s32 a1, Fix16_Point* a4, u32* pVertIdx);
    EXPORT void draw_lid_4F4D60(Fix16_Point* xpos, Fix16_Point* diffuse_colour, s32 arg_8, u32* a5);
    EXPORT void draw_slope_4F6580();
    EXPORT void draw_slope_4F6630();
    EXPORT void RenderFlatBlock_4F66C0();
    EXPORT void RenderBlockAt_4F6880(s32& pXCoord, s32& pYCoord);
    EXPORT void ClearDrawnTileCount_4F6A10();
    EXPORT void Draw_4F6A20();

    inline u32 update_and_get_f0(u32 idx)
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

    inline void sub_46BB90(s32* maybe_x, s32* maybe_y)
    {
        Nanobotz_8* pPos = &field_1C[field_2EFC_count];
        pPos->field_0 = *maybe_x;
        pPos->field_4 = *maybe_y;
        
        Nanobotz_8* pIter = &field_1C[field_2EFC_count-1];
        for (s32 i = field_2EFC_count - 1; i >= 0; i--, pIter--)
        {
            if (pIter->IsEqual_46BB60(pPos))
            {
                return;
            }
        }
        ++field_2EFC_count;
    }

    inline void ResetCount_45B040()
    {
        field_2EFC_count = 0;
    }

    Fix16 field_0_ambient;
    Fix16 field_4;
    Fix16 field_8;
    char_type field_C_colour_t1;
    char_type field_D;
    char_type field_E_colour_t2;
    char_type field_F_colour_t3;
    char_type field_10;
    char_type field_11;
    char_type field_12;
    char_type field_13;
    char_type field_14_xcoords;
    char_type field_15;
    char_type field_16;
    char_type field_17;
    char_type field_18_ycoords;
    char_type field_19;
    char_type field_1A;
    char_type field_1B;
    Nanobotz_8 field_1C[1500];
    s32 field_2EFC_count;
    s32 field_2F00_drawn_tile_count;
};

EXPORT void __stdcall set_vert_xyz_relative_to_cam_4EAD90(Fix16 xCoord, Fix16 yCoord, Fix16 z_val, Vert* pVerts);

EXTERN_GLOBAL(MapRenderer*, gpMapRenderer_6F66E4);
