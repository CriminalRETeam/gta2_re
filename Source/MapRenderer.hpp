#pragma once

#include "Function.hpp"
#include "Game.hpp"
#include "map_0x370.hpp"
#include "fix16.hpp"
#include "3rdParty/GTA2Hax/d3ddll/d3ddll.hpp" // Vert

class Fix16_Point;

class DrawListBlock
{
  public:
    inline bool IsEqual_46BB60(const DrawListBlock* other) const
    {
        return mX == other->mX && mY == other->mY;
    }
    s32 mX; // 0x0
    s32 mY; // 0x4
};
GTA2_ASSERT_SIZEOF_ALWAYS(DrawListBlock, 0x8);

EXTERN_GLOBAL(Fix16, gXCoord_6F63AC);

EXTERN_GLOBAL(Fix16, gYCoord_6F63B8);

EXTERN_GLOBAL(u32, gZCoord_6F63E0);


EXTERN_GLOBAL(gmp_map_slope, gCurrentSlope_6F646C);


EXTERN_GLOBAL(u32, gGradientSize_6F6480);

EXTERN_GLOBAL(u32, gGradientLevel_6F647C);

class MapRenderer
{
  public:
    STATIC_MEMBER_ARRAY(s32, kTopSideFlags, 8);
    STATIC_MEMBER_ARRAY(s32, kSlopeLidFlags, 8);
    STATIC_MEMBER_ARRAY(s32, kRightSideFlags, 8);
    STATIC_MEMBER_ARRAY(s32, kLidFlags, 8);
    STATIC_MEMBER_ARRAY(s32, kLeftSideFlags, 8);
    STATIC_MEMBER_ARRAY(s32, kDiagonalRightFlags, 8);
    STATIC_MEMBER_ARRAY(s32, kDiagonalLeftFlags, 8);
    STATIC_MEMBER_ARRAY(s32, kBottomSideFlags, 8);
    STATIC_MEMBER(gmp_block_info*, spBlock);
    STATIC_MEMBER(Fix16, sZCoordFp);
    STATIC_MEMBER(u32, sTileDrawFlags);
    STATIC_MEMBER(u16, sLidType);
    STATIC_MEMBER(s8, sDiagonalLidType);
    STATIC_MEMBER(u16, sBlockTop);
    STATIC_MEMBER(u16, sBlockBottom);
    MapRenderer()
    {
        mAmbient = kInitialAmbient_67DCCC;
        mTargetAmbient = kInitialAmbient_67DCCC;
        mAmbientStep.mValue = 0;
        mDrawnTileCount = 0;
        mCurrDrawLayerSize = 0;
        set_shading_lev_4E9DB0(15u);
    }

    EXPORT void SetAmbientLevel_4E9D50(s32& target_level, u16& cycles);
    EXPORT void set_shading_lev_4E9DB0(u8 shading_lev);
    EXPORT void draw_4E9EE0(u16& word_side, const bool& bFourSided, u8 colour);
    EXPORT void ambient_light_tick_4E9EA0();
    EXPORT void draw_4EA190(u16& rotation_and_flip);
    EXPORT void DrawLeftSide_4EA390(u16& left_word);
    EXPORT void ProjectVertTop_4EAE00(Fix16& xpos, Fix16& ypos, Vert* pVert);
    EXPORT void ProjectVertBottom_4EAEA0(Fix16& xCoord, Fix16& yCoord, Vert* pVert);
    EXPORT void DrawRightSide_4EAF40(u16& right_word);
    EXPORT void ProjectVert_4EB940(Fix16& xpos, Fix16& ypos, Fix16& zpos, Vert* pVert);
    EXPORT void DrawTopSide_4EBA60(u16& top_word);
    EXPORT void DrawDiagonalUpLeftFace_4EC450(u16& left_word);
    EXPORT void DrawDiagonalUpRightFace_4EC7A0(u16& right_word);
    EXPORT void DrawDiagonalDownLeftFace_4ECAF0(u16& left_word);
    EXPORT void DrawDiagonalDownRightFace_4ECE40(u16& right_word);
    EXPORT void draw_bottom_4ED290(u16& bottom_word);
    EXPORT void draw_lid_4EE130();

    // Note: These are func chunks
    void DrawDiagonalWallUpLeft_4EE7D0();
    void DrawDiagonalWallUpRight_4EE8A0();
    void DrawDiagonalWallDownLeft_4EE970();
    void DrawDiagonalWallDownRight_4EEA40();
    EXPORT void DrawPartialBlockLeft_4F5160();
    EXPORT void DrawPartialBlockRight_4F5360();
    EXPORT void DrawPartialBlockTop_4F5560();
    EXPORT void DrawPartialBlockBottom_4F5760();
    EXPORT void DrawPartialBlockTopLeftCorner_4F5960();
    EXPORT void DrawPartialBlockTopRightCorner_4F5B70();
    EXPORT void DrawPartialBlockBottomRightCorner_4F5D80();
    EXPORT void DrawPartialBlockBottomLeftCorner_4F5FD0();
    EXPORT void DrawPartialCentreBlock_4F6350();

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
    EXPORT void draw_left_4F3C00(u16& side_word, Fix16& x_off, Fix16& y_off_a, Fix16& y_off_b);
    EXPORT void draw_right_4F4250(u16& side_word, Fix16& x_off, Fix16& y_off_a, Fix16& y_off_b);
    EXPORT void draw_top_4F4600(u16& side_word, Fix16& x_off_a, Fix16& x_off_b, Fix16& y_off);
    EXPORT void draw_bottom_4F49B0(u16& side_word, Fix16& x_off_a, Fix16& x_off_b, Fix16& y_off);
    EXPORT void draw_lid_4F4D60(Fix16& x_min, Fix16& x_max, Fix16& y_min, Fix16& y_max);
    EXPORT void DrawPartialBlocks_4F6580();
    EXPORT void DrawGradientSlope_4F6630();
    EXPORT void RenderFlatBlock_4F66C0();
    EXPORT void RenderBlockAt_4F6880(s32& xpos, s32& ypos);
    EXPORT void ClearDrawnTileCount_4F6A10();
    EXPORT void Draw_4F6A20();

    inline u32 update_and_get_gradient_direction(u32 idx)
    {
        gGradientSize_6F6480 = gGmpSlopes_6F5BA8[idx].mGradientSize;
        gGradientLevel_6F647C = gGmpSlopes_6F5BA8[idx].mGradientLevel;
        gCurrentSlope_6F646C.mGradientDirection = gGmpSlopes_6F5BA8[idx].mGradientDirection;
        return gCurrentSlope_6F646C.mGradientDirection;
    }

    inline u8 GetColour_46B5E0(s32 light_level)
    {
        switch (light_level)
        {
            case 0u:
                return -1;
                break;
            case 1u:
                return mColourT1;
                break;
            case 2u:
                return mColourT2;
                break;
            case 3u:
                return mColourT3;
                break;
            default:
                return 0;
                break;
        }
    }

    inline void AddToDrawList_46BB90(s32& x, s32& y)
    {
        DrawListBlock* pPos = &mDrawList[mCurrDrawLayerSize];
        pPos->mX = x;
        pPos->mY = y;
        
        DrawListBlock* pIter = &mDrawList[mCurrDrawLayerSize-1];
        for (s32 i = mCurrDrawLayerSize - 1; i >= 0; i--, pIter--)
        {
            if (pIter->IsEqual_46BB60(pPos))
            {
                return;
            }
        }
        ++mCurrDrawLayerSize;
    }

    inline void ResetCount_45B040()
    {
        mCurrDrawLayerSize = 0;
    }

    Fix16 mAmbient; // 0x0
    Fix16 mTargetAmbient; // 0x4
    Fix16 mAmbientStep; // 0x8
    u8 mColourT1; // 0xC
    u8 mRightColour; // 0xD
    u8 mColourT2; // 0xE
    u8 mColourT3; // 0xF
    u8 mDiagUpLeftColour; // 0x10
    u8 mDiagUpRightColour; // 0x11
    u8 mDiagDownLeftColour; // 0x12
    u8 mDiagDownRightColour; // 0x13
    u8 mDcolour; // 0x14
    u8 mSlopeSouthColour; // 0x15
    u8 mSlopeWestColour; // 0x16
    u8 mSlopeEastColour; // 0x17
    u8 mColor; // 0x18
    u8 mTriDiagUpRightColour; // 0x19
    u8 mTriDiagDownLeftColour; // 0x1A
    u8 mTriDiagDownRightColour; // 0x1B
    DrawListBlock mDrawList[1500]; // 0x1C
    s32 mCurrDrawLayerSize; // 0x2EFC
    s32 mDrawnTileCount; // 0x2F00
};

EXPORT void __stdcall set_vert_xyz_relative_to_cam_4EAD90(Fix16 xCoord, Fix16 yCoord, Fix16 z_val, Vert* pVerts);

EXPORT void __stdcall draw_4F3FB0(s32 uv_flags);

EXPORT void __stdcall Set_UV_4F4190(Fix16& edge01_frac, Fix16& edge03_frac, const u32& vert_idx);

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
