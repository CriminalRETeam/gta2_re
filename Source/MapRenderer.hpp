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
    // 9.6f 0x46BB60
    inline bool IsEqual(const DrawListBlock* other) const
    {
        return mX == other->mX && mY == other->mY;
    }
    s32 mX; // 0x0
    s32 mY; // 0x4
};
GTA2_ASSERT_SIZEOF_ALWAYS(DrawListBlock, 0x8);

EXTERN_GLOBAL(Fix16, gXCoord);

EXTERN_GLOBAL(Fix16, gYCoord);

EXTERN_GLOBAL(u32, gZCoord);


EXTERN_GLOBAL(gmp_map_slope, gCurrentSlope);


EXTERN_GLOBAL(u32, gGradientSize);

EXTERN_GLOBAL(u32, gGradientLevel);

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
        mAmbient = kInitialAmbient;
        mTargetAmbient = kInitialAmbient;
        mAmbientStep.mValue = 0;
        mDrawnTileCount = 0;
        mCurrDrawLayerSize = 0;
        set_shading_lev(15u);
    }

    EXPORT void SetAmbientLevel(s32& target_level, u16& cycles);
    EXPORT void set_shading_lev(u8 shading_lev);
    EXPORT void draw_4E9EE0(u16& word_side, const bool& bFourSided, u8 colour);
    EXPORT void ambient_light_tick();
    EXPORT void draw_4EA190(u16& rotation_and_flip);
    EXPORT void DrawLeftSide(u16& left_word);
    EXPORT void ProjectVertTop(Fix16& xpos, Fix16& ypos, Vert* pVert);
    EXPORT void ProjectVertBottom(Fix16& xCoord, Fix16& yCoord, Vert* pVert);
    EXPORT void DrawRightSide(u16& right_word);
    EXPORT void ProjectVert(Fix16& xpos, Fix16& ypos, Fix16& zpos, Vert* pVert);
    EXPORT void DrawTopSide(u16& top_word);
    EXPORT void DrawDiagonalUpLeftFace(u16& left_word);
    EXPORT void DrawDiagonalUpRightFace(u16& right_word);
    EXPORT void DrawDiagonalDownLeftFace(u16& left_word);
    EXPORT void DrawDiagonalDownRightFace(u16& right_word);
    EXPORT void draw_bottom_4ED290(u16& bottom_word);
    EXPORT void draw_lid_4EE130();

    // Note: These are func chunks
    void DrawDiagonalWallUpLeft();
    void DrawDiagonalWallUpRight();
    void DrawDiagonalWallDownLeft();
    void DrawDiagonalWallDownRight();
    EXPORT void DrawPartialBlockLeft();
    EXPORT void DrawPartialBlockRight();
    EXPORT void DrawPartialBlockTop();
    EXPORT void DrawPartialBlockBottom();
    EXPORT void DrawPartialBlockTopLeftCorner();
    EXPORT void DrawPartialBlockTopRightCorner();
    EXPORT void DrawPartialBlockBottomRightCorner();
    EXPORT void DrawPartialBlockBottomLeftCorner();
    EXPORT void DrawPartialCentreBlock();

    EXPORT void Draw3SidedDiagonalUpLeft();
    EXPORT void Draw3SidedDiagonalUpRight();
    EXPORT void Draw3SidedDiagonalDownLeft();
    EXPORT void Draw3SidedDiagonalDownRight();
    EXPORT void Draw4SidedDiagonalUpLeft();
    EXPORT void Draw4SidedDiagonalUpRight();
    EXPORT void Draw4SidedDiagonalDownLeft();
    EXPORT void Draw4SidedDiagonalDownRight();
    EXPORT void DrawDiagonalWall();
    EXPORT void DrawTriangularDiagonal();
    EXPORT void DrawGradientSlopeNorthwards();
    EXPORT char_type GetColour_4F0BD0(s32 lid_type);
    EXPORT void DrawGradientSlopeSouthwards();
    EXPORT void DrawGradientSlopeWestwards();
    EXPORT void DrawGradientSlopeEastwards();
    EXPORT void draw_left(u16& side_word, Fix16& x_off, Fix16& y_off_a, Fix16& y_off_b);
    EXPORT void draw_right(u16& side_word, Fix16& x_off, Fix16& y_off_a, Fix16& y_off_b);
    EXPORT void draw_top(u16& side_word, Fix16& x_off_a, Fix16& x_off_b, Fix16& y_off);
    EXPORT void draw_bottom_4F49B0(u16& side_word, Fix16& x_off_a, Fix16& x_off_b, Fix16& y_off);
    EXPORT void draw_lid_4F4D60(Fix16& x_min, Fix16& x_max, Fix16& y_min, Fix16& y_max);
    EXPORT void DrawPartialBlocks();
    EXPORT void DrawGradientSlope();
    EXPORT void RenderFlatBlock();
    EXPORT void RenderBlockAt(s32& xpos, s32& ypos);
    EXPORT void ClearDrawnTileCount();
    EXPORT void Draw();

    inline u32 update_and_get_gradient_direction(u32 idx)
    {
        gGradientSize = gGmpSlopes[idx].mGradientSize;
        gGradientLevel = gGmpSlopes[idx].mGradientLevel;
        gCurrentSlope.mGradientDirection = gGmpSlopes[idx].mGradientDirection;
        return gCurrentSlope.mGradientDirection;
    }

    // 9.6f 0x46B5E0
    inline u8 GetColour(s32 light_level)
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

    // 9.6f 0x46BB90
    inline void AddToDrawList(s32& x, s32& y)
    {
        DrawListBlock* pPos = &mDrawList[mCurrDrawLayerSize];
        pPos->mX = x;
        pPos->mY = y;
        
        DrawListBlock* pIter = &mDrawList[mCurrDrawLayerSize-1];
        for (s32 i = mCurrDrawLayerSize - 1; i >= 0; i--, pIter--)
        {
            if (pIter->IsEqual(pPos))
            {
                return;
            }
        }
        ++mCurrDrawLayerSize;
    }

    // 9.6f 0x45B040
    inline void ResetCount()
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

EXPORT void __stdcall set_vert_xyz_relative_to_cam(Fix16 xCoord, Fix16 yCoord, Fix16 z_val, Vert* pVerts);

EXPORT void __stdcall draw_4F3FB0(s32 uv_flags);

EXPORT void __stdcall Set_UV(Fix16& edge01_frac, Fix16& edge03_frac, const u32& vert_idx);

EXTERN_GLOBAL(MapRenderer*, gpMapRenderer);

struct BlockSideWord
{
    u32 tile_idx : 10;
    u32 wall : 1;
    u32 bullet_wall : 1;
    u32 flat : 1;
    u32 flip : 1;
    u32 rotation_code : 2;
};
