#include "MapRenderer.hpp"
#include "Camera.hpp"
#include "winmain.hpp"
#include "gbh_graphics.hpp"
#include "Globals.hpp"
#include "debug.hpp"
#include "fix16.hpp"
#include "LightPool.hpp"
#include "map_0x370.hpp"
#include "SpriteRenderer.hpp"
#include "TextureCache.hpp"
#include "winmain.hpp"
#include "block_face.hpp"

DEFINE_GLOBAL(MapRenderer*, gpMapRenderer, 0x6F66E4);
DEFINE_GLOBAL_INIT(Fix16_Point, kZeroOnePoint, Fix16_Point(Fix16(0), Fix16(1)), 0x6F6484);
EXTERN_GLOBAL(u16, gBlockLeft);
DEFINE_STATIC_MEMBER(u16, MapRenderer, sBlockTop, 0x6F62F4);
EXTERN_GLOBAL(u16, gBlockRight);
DEFINE_STATIC_MEMBER(u16, MapRenderer, sBlockBottom, 0x6F6468);
DEFINE_STATIC_MEMBER(u16, MapRenderer, sLidType, 0x6F6274);
DEFINE_GLOBAL(Fix16, gXCoord, 0x6F63AC);
DEFINE_GLOBAL(Fix16, gYCoord, 0x6F63B8);
DEFINE_GLOBAL(u32, gZCoord, 0x6F63E0); // u32: the projection helpers pass it to Fix16(u32) (0x4926F0)
DEFINE_GLOBAL(u32, gGradientSize, 0x6F6480);
DEFINE_GLOBAL(u32, gGradientLevel, 0x6F647C);
DEFINE_STATIC_MEMBER(gmp_block_info*, MapRenderer, spBlock, 0x6F6478);
DEFINE_GLOBAL(gmp_map_slope, gCurrentSlope, 0x6F646C);
DEFINE_GLOBAL_ARRAY(Vert, gTileVerts_6F65A8, 8, 0x6F65A8);

DEFINE_GLOBAL(u32, gZCoordTop, 0x6F62B0); // u32: as gZCoord
DEFINE_STATIC_MEMBER(u32, MapRenderer, sTileDrawFlags, 0x6F6560);
DEFINE_GLOBAL(Fix16, gScreenScaleTop, 0x6F628C);
DEFINE_GLOBAL(Fix16, gInvDepthTop, 0x6F656C);
DEFINE_GLOBAL(Fix16, gInvDepthBottom, 0x6F6318);
DEFINE_GLOBAL(Fix16, gScreenScaleBottom, 0x6F633C);
DEFINE_STATIC_MEMBER(Fix16, MapRenderer, sZCoordFp, 0x6F6518);
DEFINE_STATIC_MEMBER_INIT(s8, MapRenderer, sDiagonalLidType, -1, 0x620F20);  // OBS: workaround
DEFINE_STATIC_MEMBER_ARRAY_INIT(s32, MapRenderer, kLidFlags, 8, 0x620FA4, 0 COMMA 8 COMMA 0x20 COMMA 0x28 COMMA 0x40 COMMA 0x48 COMMA 0x60 COMMA 0x68);
DEFINE_STATIC_MEMBER_ARRAY_INIT(s32, MapRenderer, kDiagonalRightFlags, 8, 0x621004, 0x65 COMMA 0x75 COMMA 5 COMMA 0x15 COMMA 0x25 COMMA 0x35 COMMA 0x45 COMMA 0x55);
DEFINE_STATIC_MEMBER_ARRAY_INIT(s32, MapRenderer, kDiagonalLeftFlags, 8, 0x620FE4, 0x25 COMMA 0x35 COMMA 0x45 COMMA 0x55 COMMA 0x65 COMMA 0x75 COMMA 5 COMMA 0x15);
DEFINE_STATIC_MEMBER_ARRAY_INIT(s32, MapRenderer, kBottomSideFlags, 8, 0x620F84, 2 COMMA 0xA COMMA 0x22 COMMA 0x2A COMMA 0x42 COMMA 0x4A COMMA 0x62 COMMA 0x6A);
DEFINE_STATIC_MEMBER_ARRAY_INIT(s32, MapRenderer, kRightSideFlags, 8, 0x620F44, 0x64 COMMA 0x74 COMMA 0x4 COMMA 0x14 COMMA 0x24 COMMA 0x34 COMMA 0x44 COMMA 0x54);
DEFINE_STATIC_MEMBER_ARRAY_INIT(s32, MapRenderer, kLeftSideFlags, 8, 0x620F24, 0x23 COMMA 0x33 COMMA 0x43 COMMA 0x53 COMMA 0x63 COMMA 0x73 COMMA 0x3 COMMA 0x13);
DEFINE_STATIC_MEMBER_ARRAY_INIT(s32, MapRenderer, kTopSideFlags, 8, 0x620F64, 0x41 COMMA 0x49 COMMA 0x61 COMMA 0x69 COMMA 0x1 COMMA 0x9 COMMA 0x21 COMMA 0x29);
DEFINE_STATIC_MEMBER_ARRAY_INIT(s32, MapRenderer, kSlopeLidFlags, 8, 0x620FC4, 0x0 COMMA 0x8 COMMA 0x20 COMMA 0x28 COMMA 0x40 COMMA 0x48 COMMA 0x60 COMMA 0x68);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kScreenAspectRatio, Fix16(0x3000,0), 0x6F638C);
DEFINE_STATIC_GLOBAL_INIT(Ang16, kAng315, Ang16(1260), 0x6F6420);
DEFINE_GLOBAL_INIT(Ang16, kAng135_6F6414, Ang16(540), 0x6F6414);
DEFINE_GLOBAL_INIT(Ang16, kAng45_6F637C, Ang16(180), 0x6F637C);
DEFINE_STATIC_GLOBAL_INIT(Ang16, kAng225, Ang16(900), 0x6F63EC);
DEFINE_GLOBAL_INIT(Fix16, kTileTexSize, Fix16(64), 0x6F6548);
DEFINE_GLOBAL_INIT(Fix16, kFpEpsilon_6F64B4, Fix16(1, 0), 0x6F64B4);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kTileTexMax, kTileTexSize - kFpEpsilon_6F64B4, 0x6F6578);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpThreeEighths, Fix16(0x1800, 0), 0x6F6428);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpFiveEighths, Fix16(0x2800, 0), 0x6F6430);
DEFINE_GLOBAL_INIT(Fix16, kFpOneSixtyFourth_6F6434, Fix16(0x100, 0), 0x6F6434);
DEFINE_STATIC_GLOBAL(Fix16_Point, sUVCorner1, 0x6F6588);
DEFINE_STATIC_GLOBAL(Fix16_Point, sUVCorner3, 0x6F6598);
DEFINE_STATIC_GLOBAL(Fix16_Point, sUVCorner2, 0x6F6590);
DEFINE_STATIC_GLOBAL(Fix16_Point, sUVEdge03, 0x6F62A0);
DEFINE_STATIC_GLOBAL(Fix16_Point, sUVEdge01, 0x6F62A8);
DEFINE_STATIC_GLOBAL(Fix16_Point, sUVCorner0, 0x6F6580);

// The vertex projection helpers (9.6f: Nanobotz::ProjectVertTop, ProjectVertBottom and
// ProjectVert, called on the caller's this). In 10.5 they are inlined. Called through a global
// object (a constant this), VC6 inlines them and the Fix16 operator+ temporaries in the big Draw*Side
// functions exactly as often as the original did; free functions or members called on a register this
// leave extra operator+ calls out of line (DrawTopSide only matches this way).
// The y lines convert through an f32 local (Fix16ToF32_Rounded): the store to it is a no-op node
// between the fildl and the fmuls (the "rounding node" of Set_UV), which lets the scheduler put
// the centre load and the dead zero hi dword between them, as the original does. Written in the helper
// body the local would cost inline size (draw_left & co. end their budget at exactly 0); as a
// free inline (size <= 40) it costs nothing.
static inline f32 Fix16ToF32_Rounded(const Fix16& v)
{
    f32 f = static_cast<f32>(v.mValue);
    return f / 16384.0f;
}

// ProjectVert in the Draw*Side functions converts the camera's field_60 first (fildl 0x60(%eax)
// before the x/y load): the extra parentheses make that operand the heavier subtree, so VC6 evaluates
// it first. As a free inline (size <= 40) they cost no inline size; written in the helper body they
// would, and DrawLeftSide ends its budget at exactly 0. The slope functions inline
// VertProjector2/VertProjector3::ProjectVert instead (see Fix16ToF32_Rounded2/3).
static inline f32 Fix16ToFloat_Paren(const Fix16& v)
{
    return ((v.mValue / 16384.0f));
}

// The slope functions' ProjectVert (VertProjector2/VertProjector3 below) converts x/y and the
// camera's field_60 through f32 locals with two or three parentheses. The f32 local is the "rounding
// node" between the fildl and the fmuls (see Fix16ToF32_Rounded), the parentheses are no-op nodes after
// it; both only move the scheduler's integer instructions (the next vertex's idiv, loads and temp
// stores) between the x87 ones. The heavier operand is loaded first: DrawGradientSlopeNorthwards and
// Westwards load x/y first, Southwards and Eastwards field_60 first, which is why there are two copies.
// Free inlines (size <= 40), so they don't change the inline budget.
static inline f32 Fix16ToF32_Rounded2(const Fix16& v)
{
    f32 f = static_cast<f32>(v.mValue);
    return ((f / 16384.0f));
}

static inline f32 Fix16ToF32_Rounded3(const Fix16& v)
{
    f32 f = static_cast<f32>(v.mValue);
    return (((f / 16384.0f)));
}

static inline f32 Fix16ToF32_P3(const Fix16& v)
{
    return (((v.mValue / 16384.0f)));
}

struct VertProjector
{
    // 9.6f 0x46BD40
    inline void ProjectVertTop(Fix16& x, Fix16& y, Vert* pVert)
    {
        set_vert_xyz_relative_to_cam(x, y, gZCoordTop, pVert);
        pVert->x = x.ToFloat() * gScreenScaleTop.ToFloat() + static_cast<u32>(gViewCamera->mScreenPxCenterX);
        pVert->y = y.ToFloat() * gScreenScaleTop.ToFloat() + static_cast<u32>(gViewCamera->mScreenPxCenterY);
        pVert->z = gInvDepthTop.ToFloat();
    }

    // 9.6f 0x46BDF0
    inline void ProjectVertBottom(Fix16& xpos, Fix16& ypos, Vert* pVert)
    {
        set_vert_xyz_relative_to_cam(xpos, ypos, gZCoord, pVert);
    
        pVert->x = gScreenScaleBottom.ToFloat() * xpos.ToFloat() + static_cast<u32>(gViewCamera->mScreenPxCenterX);
        pVert->y = gScreenScaleBottom.ToFloat() * ypos.ToFloat() + static_cast<u32>(gViewCamera->mScreenPxCenterY);
        pVert->z = gInvDepthBottom.ToFloat();
    }

    // 9.6f 0x46BC70
    inline void ProjectVert(Fix16& xpos, Fix16& ypos, Fix16& zpos, Vert* pVert)
    {
        set_vert_xyz_relative_to_cam(xpos, ypos, zpos, pVert);
        pVert->z = 1.0f / (gViewCamera->mCurPos.mZ.ToFloat() + (8.0f - zpos.ToFloat()));
        pVert->x = xpos.ToFloat() * Fix16ToFloat_Paren(gViewCamera->mScreenScale.x) * pVert->z + static_cast<u32>(gViewCamera->mScreenPxCenterX);
        pVert->y = ypos.ToFloat() * Fix16ToFloat_Paren(gViewCamera->mScreenScale.x) * pVert->z + static_cast<u32>(gViewCamera->mScreenPxCenterY);
    }
};

static VertProjector gVertProjector;

// The same Top/Bottom projection as VertProjector, in the form every draw function except the four
// Draw*Side functions (4EA390, 4EAF40, 4EBA60, 4ED290) inlines. 10.5 has two versions of these helpers:
// the out-of-line copies MapRenderer::ProjectVertTop/ProjectVertBottom are called only
// from the Draw*Side functions and stop matching with the bodies below, while the diagonal, lid,
// 3/4-sided, slope and draw_left/right/top/bottom functions only match with these.
// The extra parentheses are there to reproduce the original's expression nodes: VC6 keeps a no-op IL
// node for each parenthesised float subexpression (VC7, and so 9.6f, drops them). The nodes cost no
// code but count towards the x87 scheduler's window of 81 instruction nodes, which decides where the
// integer loads and stores are placed between the x87 ops. Each paren also adds 1 to the helper's inline
// size, which is why the Draw*Side functions can't share these bodies: they end their inline budget with
// exactly 0 left (DrawLeftSide's last operator+), so a bigger helper sends calls out of line.
struct VertProjector2
{
    inline void ProjectVertTop(Fix16& x, Fix16& y, Vert* pVert)
    {
        set_vert_xyz_relative_to_cam(x, y, gZCoordTop, pVert);
        pVert->x = (((x.ToFloat())) * gScreenScaleTop.ToFloat()) + static_cast<u32>(gViewCamera->mScreenPxCenterX);
        pVert->y = (((Fix16ToF32_Rounded(y)) * gScreenScaleTop.ToFloat()) + static_cast<u32>(gViewCamera->mScreenPxCenterY));
        pVert->z = gInvDepthTop.ToFloat();
    }

    inline void ProjectVertBottom(Fix16& xpos, Fix16& ypos, Vert* pVert)
    {
        set_vert_xyz_relative_to_cam(xpos, ypos, gZCoord, pVert);

        pVert->x = (gScreenScaleBottom.ToFloat() * ((xpos.ToFloat()))) + static_cast<u32>(gViewCamera->mScreenPxCenterX);
        pVert->y = ((gScreenScaleBottom.ToFloat() * (Fix16ToF32_Rounded(ypos))) + static_cast<u32>(gViewCamera->mScreenPxCenterY));
        pVert->z = gInvDepthBottom.ToFloat();
    }

    inline void ProjectVert(Fix16& xpos, Fix16& ypos, Fix16& zpos, Vert* pVert)
    {
        set_vert_xyz_relative_to_cam(xpos, ypos, zpos, pVert);
        pVert->z = 1.0f / (gViewCamera->mCurPos.mZ.ToFloat() + (8.0f - zpos.ToFloat()));
        pVert->x = Fix16ToF32_Rounded3(xpos) * Fix16ToF32_Rounded2(gViewCamera->mScreenScale.x) * pVert->z + static_cast<u32>(gViewCamera->mScreenPxCenterX);
        pVert->y = Fix16ToF32_Rounded3(ypos) * Fix16ToF32_Rounded2(gViewCamera->mScreenScale.x) * pVert->z + static_cast<u32>(gViewCamera->mScreenPxCenterY);
    }
};

static VertProjector2 gVertProjector2;

// DrawGradientSlopeSouthwards and Eastwards_4F33B0 load field_60 before x/y (see above).
struct VertProjector3
{
    inline void ProjectVert(Fix16& xpos, Fix16& ypos, Fix16& zpos, Vert* pVert)
    {
        set_vert_xyz_relative_to_cam(xpos, ypos, zpos, pVert);
        pVert->z = 1.0f / (gViewCamera->mCurPos.mZ.ToFloat() + (8.0f - zpos.ToFloat()));
        pVert->x = Fix16ToF32_Rounded2(xpos) * Fix16ToF32_Rounded3(gViewCamera->mScreenScale.x) * pVert->z + static_cast<u32>(gViewCamera->mScreenPxCenterX);
        // No f32 local for field_60 and a no-op after the product: S 8 -> 4, E 16 -> 8 (docs/match_attempts.md).
        pVert->y = Fix16ToF32_Rounded2(ypos) * Fix16ToF32_P3(gViewCamera->mScreenScale.x) * (pVert->z) + static_cast<u32>(gViewCamera->mScreenPxCenterY);
    }
};

static VertProjector3 gVertProjector3;

// Converts through two f32 locals (the stores are x87 no-op nodes).
static inline f32 Fix16ToF32_Stored(const Fix16& v)
{
    f32 f = static_cast<f32>(v.mValue);
    f32 g = f / 16384.0f;
    return g;
}

// As VertProjector3, with field_60 converted first in the y line and both conversions stored to f32
// locals. Used for the vertex right before one whose z needs an idiv: the extra no-op nodes keep the
// field_60 fmuls behind that idiv, as in the original.
struct VertProjector4
{
    inline void ProjectVert(Fix16& xpos, Fix16& ypos, Fix16& zpos, Vert* pVert)
    {
        set_vert_xyz_relative_to_cam(xpos, ypos, zpos, pVert);
        pVert->z = 1.0f / (gViewCamera->mCurPos.mZ.ToFloat() + (8.0f - zpos.ToFloat()));
        pVert->x = Fix16ToF32_Rounded2(xpos) * Fix16ToF32_Rounded3(gViewCamera->mScreenScale.x) * (pVert->z) + static_cast<u32>(gViewCamera->mScreenPxCenterX);
        pVert->y = (Fix16ToF32_Stored(gViewCamera->mScreenScale.x) * Fix16ToF32_Stored(ypos)) * (pVert->z) + static_cast<u32>(gViewCamera->mScreenPxCenterY);
    }
};

static VertProjector4 gVertProjector4;

static inline void set_vert_xyz_relative_to_cam_inlined(Fix16 xCoord, Fix16 yCoord, Fix16 z_val, Vert* pVerts)
{
    Camera* pCam = gViewCamera;

    s32 next_idx = (pVerts - gTileVerts_6F65A8) + 4;

    // Only ProjectVert inlines this copy. The f32 locals (Fix16ToF32_Rounded) move the index
    // computation's mov behind the z temp store, as in the original (22 -> 4 lines).
    gTileVerts_6F65A8[next_idx].x = Fix16ToF32_Rounded(xCoord + pCam->mCurPos.mX);
    gTileVerts_6F65A8[next_idx].y = Fix16ToF32_Rounded(yCoord + pCam->mCurPos.mY);
    gTileVerts_6F65A8[next_idx].z = z_val.ToFloat();
}

// 9.6f 0x46B910
static inline void TransformTriangleUVs(u16& rotation_and_flip)
{
    s32 vert_idx;
    
    switch (rotation_and_flip)
    {
        case 0x2000:
            for (vert_idx = 0; vert_idx <= 2; vert_idx++)
            {
                if (gTileVerts_6F65A8[vert_idx].u == 0.0f)
                {
                    gTileVerts_6F65A8[vert_idx].u = 63.999901f;
                }
                else if (gTileVerts_6F65A8[vert_idx].u == 63.999901f)
                {
                    gTileVerts_6F65A8[vert_idx].u = 0.0f;
                }
            }
            break;
        case 0x4000:
            for (vert_idx = 0; vert_idx <= 2; vert_idx++)
            {
                if (gTileVerts_6F65A8[vert_idx].u == 0.0f)
                {
                    gTileVerts_6F65A8[vert_idx].v = 63.999901f;
                }
                else if (gTileVerts_6F65A8[vert_idx].u == 32.0f)
                {
                    gTileVerts_6F65A8[vert_idx].u = 63.999901f;
                    gTileVerts_6F65A8[vert_idx].v = 32.0f;
                }
                else
                {
                    gTileVerts_6F65A8[vert_idx].u = 0.0f;
                }
            }
            break;
        case 0x6000:
            for (vert_idx = 0; vert_idx <= 2; vert_idx++)
            {
                if (gTileVerts_6F65A8[vert_idx].u == 63.999901f)
                {
                    gTileVerts_6F65A8[vert_idx].u = 0.0f;
                    gTileVerts_6F65A8[vert_idx].v = 63.999901f;
                }
                else if (gTileVerts_6F65A8[vert_idx].u == 32.0f)
                {
                    gTileVerts_6F65A8[vert_idx].u = 63.999901f;
                    gTileVerts_6F65A8[vert_idx].v = 32.0f;
                }
            }
            break;
        case 0x8000:
            for (vert_idx = 0; vert_idx <= 2; vert_idx++)
            {
                if (gTileVerts_6F65A8[vert_idx].u == 0.0f)
                {
                    gTileVerts_6F65A8[vert_idx].u = 63.999901f;
                    gTileVerts_6F65A8[vert_idx].v = 63.999901f;
                }
                else if (gTileVerts_6F65A8[vert_idx].u == 32.0f)
                {
                    gTileVerts_6F65A8[vert_idx].v = 0.0f;
                }
                else
                {
                    gTileVerts_6F65A8[vert_idx].u = 0.0f;
                    gTileVerts_6F65A8[vert_idx].v = 63.999901f;
                }
            }
            break;
        case 0xA000:
            for (vert_idx = 0; vert_idx <= 2; vert_idx++)
            {
                if (gTileVerts_6F65A8[vert_idx].u == 0.0f
                   || gTileVerts_6F65A8[vert_idx].u != 32.0f)
                {
                    gTileVerts_6F65A8[vert_idx].v = 63.999901f;
                }
                else
                {
                    gTileVerts_6F65A8[vert_idx].v = 0.0f;
                }
            }
            break;
        case 0xC000:
            for (vert_idx = 0; vert_idx <= 2; vert_idx++)
            {
                if (gTileVerts_6F65A8[vert_idx].u == 0.0f)
                {
                    gTileVerts_6F65A8[vert_idx].u = 63.999901f;
                }
                else if (gTileVerts_6F65A8[vert_idx].u == 32.0f)
                {
                    gTileVerts_6F65A8[vert_idx].u = 0.0f;
                    gTileVerts_6F65A8[vert_idx].v = 32.0f;
                }
                else
                {
                    gTileVerts_6F65A8[vert_idx].v = 63.999901f;
                }
            }
            break;
        case 0xE000:
            for (vert_idx = 0; vert_idx <= 2; vert_idx++)
            {
                if (gTileVerts_6F65A8[vert_idx].u == 32.0f)
                {
                    gTileVerts_6F65A8[vert_idx].u = 0.0f;
                    gTileVerts_6F65A8[vert_idx].v = 32.0f;
                }
                else if (gTileVerts_6F65A8[vert_idx].u == 0.0f)
                {
                    gTileVerts_6F65A8[vert_idx].u = 63.999901f;
                    gTileVerts_6F65A8[vert_idx].v = 63.999901f;
                }
            }
            break;
    }
}

MATCH_FUNC(0x4e9d50)
void MapRenderer::SetAmbientLevel(s32& target_level, u16& cycles)
{
    mTargetAmbient = Fix16(target_level, 0);

    if (cycles > 0)
    {
        mAmbientStep = (mTargetAmbient - mAmbient) / Fix16(cycles);
    }
    else
    {
        mAmbient = mTargetAmbient;
        mAmbientStep = kZeroOnePoint.x;
    }
}

// https://decomp.me/scratch/SrbHE
MATCH_FUNC(0x4e9db0)
void MapRenderer::set_shading_lev(u8 shading_lev)
{
    u8 mid_shade = 2 * shading_lev / 3;
    u8 base_shade = 255 - 4 * shading_lev;
    u8 shading_leva = 5 * (51 - shading_lev);
    u8 top_shade = base_shade + 2 * mid_shade;
    u8 v2_v6 = mid_shade + base_shade;
    u8 lev_v6 = shading_leva - mid_shade;
    u8 lev_v6_2 = shading_leva - 2 * mid_shade;

    mColourT1 = top_shade;
    mRightColour = shading_leva - 3 * mid_shade;
    mColourT2 = base_shade;
    mColourT3 = lev_v6;
    mDiagUpLeftColour = base_shade;
    mDiagUpRightColour = lev_v6_2;
    mDiagDownLeftColour = v2_v6;
    mDiagDownRightColour = lev_v6;
    mDcolour = v2_v6;
    mSlopeSouthColour = shading_leva;
    mSlopeWestColour = lev_v6_2;
    mSlopeEastColour = base_shade + 3 * mid_shade;
    mColor = v2_v6;
    mTriDiagUpRightColour = lev_v6;
    mTriDiagDownLeftColour = top_shade;
    mTriDiagDownRightColour = shading_leva;
}

MATCH_FUNC(0x4e9ea0)
void MapRenderer::ambient_light_tick()
{
    mAmbient = mAmbientStep + mAmbient;

    if (mAmbientStep > kZeroOnePoint.x)
    {
        if (mAmbient >= mTargetAmbient)
        {
            mAmbientStep = kZeroOnePoint.x;
            mAmbient = mTargetAmbient;
        }
    }
    else if (mAmbientStep < kZeroOnePoint.x)
    {
        if (mAmbient <= mTargetAmbient)
        {
            mAmbientStep = kZeroOnePoint.x;
            mAmbient = mTargetAmbient;
        }
    }
}

// this function matches, but some "fcomps" offsets are wrong
MATCH_FUNC(0x4E9EE0)
void MapRenderer::draw_triangle(u16& word_side, const bool& bFourSided, u8 colour)
{
    u16 texture_idx = gGtx_0x106C->GetTile(word_side & block_face::tile_idx_mask);
    if (texture_idx)
    {
        if (bFourSided)
        {
            u16 rotation_and_flip = word_side & block_face::flip_and_rotation_mask;
            TransformTriangleUVs(rotation_and_flip);
        }
        else
        {
            u16 rotation_and_flip = word_side & block_face::flip_and_rotation_mask;
            TransformTriangleUVsThreeSided(rotation_and_flip);
        }
        sTileDrawFlags = (word_side >> 5) & 0x80;
        pgbh_DrawTriangle(sTileDrawFlags | gLightingDrawFlag,
                          gTextureCache_15D8->GetTexture(texture_idx),
                          gTileVerts_6F65A8,
                          colour);
        ++mDrawnTileCount;
    }
}

// this function matches, but some "fcomps" offsets are wrong
MATCH_FUNC(0x4EA190)
void MapRenderer::TransformTriangleUVsThreeSided(u16& rotation_and_flip) 
{
    s32 vert_idx;
    
    switch (rotation_and_flip)
    {
        case 0x2000:
            for (vert_idx = 0; vert_idx <= 2; vert_idx++)
            {
                if (gTileVerts_6F65A8[vert_idx].u == 0.0f)
                {
                    gTileVerts_6F65A8[vert_idx].u = 63.999901f;
                }
                else if (gTileVerts_6F65A8[vert_idx].u == 63.999901f)
                {
                    gTileVerts_6F65A8[vert_idx].u = 0.0f;
                }
            }
            break;
        case 0x4000:
            for (vert_idx = 0; vert_idx <= 2; vert_idx++)
            {
                if (gTileVerts_6F65A8[vert_idx].u == 0.0f)
                {
                    gTileVerts_6F65A8[vert_idx].u = 63.999901f;
                }
                else if (gTileVerts_6F65A8[vert_idx].u == 32.0f)
                {
                    gTileVerts_6F65A8[vert_idx].u = 0.0f;
                    gTileVerts_6F65A8[vert_idx].v = 32.0f;
                }
                else
                {
                    gTileVerts_6F65A8[vert_idx].v = 0.0f;
                }
            }
            break;
        case 0x6000:
            for (vert_idx = 0; vert_idx <= 2; vert_idx++)
            {
                if (gTileVerts_6F65A8[vert_idx].u == 0.0f)
                {
                    gTileVerts_6F65A8[vert_idx].u = 63.999901f;
                    gTileVerts_6F65A8[vert_idx].v = 0.0f;
                }
                else if (gTileVerts_6F65A8[vert_idx].u == 32.0f)
                {
                    gTileVerts_6F65A8[vert_idx].u = 0.0f;
                    gTileVerts_6F65A8[vert_idx].v = 32.0f;
                }
            }
            break;
        case 0x8000:
            for (vert_idx = 0; vert_idx <= 2; vert_idx++)
            {
                if (gTileVerts_6F65A8[vert_idx].u == 0.0f)
                {
                    gTileVerts_6F65A8[vert_idx].u = 63.999901f;
                    gTileVerts_6F65A8[vert_idx].v = 0.0f;
                }
                else if (gTileVerts_6F65A8[vert_idx].u == 32.0f)
                {
                    gTileVerts_6F65A8[vert_idx].v = 63.999901f;
                }
                else
                {
                    gTileVerts_6F65A8[vert_idx].u = 0.0f;
                    gTileVerts_6F65A8[vert_idx].v = 0.0f;
                }
            }
            break;
        case 0xA000:
            for (vert_idx = 0; vert_idx <= 2; vert_idx++)
            {
                if (gTileVerts_6F65A8[vert_idx].u == 0.0f
                   || gTileVerts_6F65A8[vert_idx].u != 32.0f)
                {
                    gTileVerts_6F65A8[vert_idx].v = 0.0f;
                }
                else
                {
                    gTileVerts_6F65A8[vert_idx].v = 63.999901f;
                }
            }
            break;
        case 0xC000:
            for (vert_idx = 0; vert_idx <= 2; vert_idx++)
            {
                if (gTileVerts_6F65A8[vert_idx].u == 0.0f)
                {
                    gTileVerts_6F65A8[vert_idx].v = 0.0f;
                }
                else if (gTileVerts_6F65A8[vert_idx].u == 32.0f)
                {
                    gTileVerts_6F65A8[vert_idx].u = 63.999901f;
                    gTileVerts_6F65A8[vert_idx].v = 32.0f;
                }
                else
                {
                    gTileVerts_6F65A8[vert_idx].u = 0.0f;
                }
            }
            break;
        case 0xE000:
            for (vert_idx = 0; vert_idx <= 2; vert_idx++)
            {
                if (gTileVerts_6F65A8[vert_idx].u == 32.0f)
                {
                    gTileVerts_6F65A8[vert_idx].u = 63.999901f;
                    gTileVerts_6F65A8[vert_idx].v = 32.0f;
                }
                else if (gTileVerts_6F65A8[vert_idx].u == 63.999901f)
                {
                    gTileVerts_6F65A8[vert_idx].u = 0.0f;
                    gTileVerts_6F65A8[vert_idx].v = 0.0f;
                }
            }
            break;
    }
}

// https://decomp.me/scratch/FGVOK
MATCH_FUNC(0x4ea390)
void MapRenderer::DrawLeftSide(u16& left_word)
{
    if (!bSkip_left)
    {
        switch (gCurrentSlope.mGradientDirection)
        {
            case NORTH_1:
                gVertProjector.ProjectVertBottom(gXCoord, gYCoord, &gTileVerts_6F65A8[0]);
                if (!gGradientLevel)
                {
                    gVertProjector.ProjectVertTop(gXCoord, gYCoord, &gTileVerts_6F65A8[1]);
                    gTileVerts_6F65A8[1].v = 0.0f;
                }
                else
                {
                    Fix16 slope_z_1 = Fix16((gGradientSize - gGradientLevel)) / gGradientSize;
                    gVertProjector.ProjectVert(gXCoord, gYCoord, sZCoordFp + slope_z_1, &gTileVerts_6F65A8[1]);
                    gTileVerts_6F65A8[1].v = 63.999901f - (slope_z_1 * kTileTexSize).ToFloat();
                }

                if (gTileVerts_6F65A8[0].x > gTileVerts_6F65A8[1].x)
                {
                    return;
                }

                gVertProjector.ProjectVertBottom(gXCoord, gYCoord + kZeroOnePoint.y, &gTileVerts_6F65A8[3]);

                if (gGradientLevel == gGradientSize - 1)
                {
                    memcpy(&gTileVerts_6F65A8[2], &gTileVerts_6F65A8[3], sizeof(Vert));
                    gTileVerts_6F65A8[2].v = 63.999901f;
                }
                else
                {
                    Fix16 slope_z_2 = Fix16((gGradientSize - gGradientLevel - 1)) / gGradientSize;
                    gVertProjector.ProjectVert(gXCoord, gYCoord + kZeroOnePoint.y, sZCoordFp + slope_z_2, &gTileVerts_6F65A8[2]);
                    gTileVerts_6F65A8[2].v = 63.999901f - (slope_z_2 * kTileTexSize).ToFloat();
                }

                // now set u
                sTileDrawFlags = 16389;
                if ((*(reinterpret_cast<u8*>(&left_word) + 1) & 0x20) != 0)
                {
                    gTileVerts_6F65A8[0].u = 63.999901f;
                    gTileVerts_6F65A8[1].u = 63.999901f;
                    gTileVerts_6F65A8[2].u = 0.0f;
                    gTileVerts_6F65A8[3].u = 0.0f;
                }
                else
                {
                    gTileVerts_6F65A8[0].u = 0.0f;
                    gTileVerts_6F65A8[1].u = 0.0f;
                    gTileVerts_6F65A8[2].u = 63.999901f;
                    gTileVerts_6F65A8[3].u = 63.999901f;
                }
                gTileVerts_6F65A8[0].v = 63.999901f;
                gTileVerts_6F65A8[3].v = 63.999901f;
                break;

            case SOUTH_2:
                gVertProjector.ProjectVertBottom(gXCoord, gYCoord + kZeroOnePoint.y, &gTileVerts_6F65A8[3]);
                if (gGradientLevel == 0)
                {
                    gVertProjector.ProjectVertTop(gXCoord, gYCoord + kZeroOnePoint.y, &gTileVerts_6F65A8[2]);
                    gTileVerts_6F65A8[2].v = 0.0f;
                }
                else
                {
                    Fix16 slope_z_3 = Fix16(gGradientSize - gGradientLevel) / gGradientSize;
                    gVertProjector.ProjectVert(gXCoord, gYCoord + kZeroOnePoint.y, sZCoordFp + slope_z_3, &gTileVerts_6F65A8[2]);
                    gTileVerts_6F65A8[2].v = 63.999901f - (slope_z_3 * kTileTexSize).ToFloat();
                }

                if (gTileVerts_6F65A8[3].x > gTileVerts_6F65A8[2].x)
                {
                    return;
                }
                gVertProjector.ProjectVertBottom(gXCoord, gYCoord, &gTileVerts_6F65A8[0]);

                if (gGradientLevel == gGradientSize - 1)
                {
                    memcpy(&gTileVerts_6F65A8[1], &gTileVerts_6F65A8[0], sizeof(Vert));
                    gTileVerts_6F65A8[1].v = 63.999901f;
                }
                else
                {
                    Fix16 slope_z_4 = Fix16(gGradientSize - gGradientLevel - 1) / gGradientSize;
                    gVertProjector.ProjectVert(gXCoord, gYCoord, sZCoordFp + slope_z_4, &gTileVerts_6F65A8[1]);
                    gTileVerts_6F65A8[1].v = 63.999901f - (slope_z_4 * kTileTexSize).ToFloat();
                }
                // now set u
                sTileDrawFlags = 16389;
                if ((*(reinterpret_cast<u8*>(&left_word) + 1) & 0x20) != 0)
                {
                    gTileVerts_6F65A8[0].u = 63.999901f;
                    gTileVerts_6F65A8[1].u = 63.999901f;
                    gTileVerts_6F65A8[2].u = 0.0f;
                    gTileVerts_6F65A8[3].u = 0.0f;
                }
                else
                {
                    gTileVerts_6F65A8[0].u = 0.0f;
                    gTileVerts_6F65A8[1].u = 0.0f;
                    gTileVerts_6F65A8[2].u = 63.999901f;
                    gTileVerts_6F65A8[3].u = 63.999901f;
                }
                gTileVerts_6F65A8[0].v = 63.999901f;
                gTileVerts_6F65A8[3].v = 63.999901f;
                break;
            default:
                // Flat blocks
                MapRenderer::ProjectVertBottom(gXCoord, gYCoord, &gTileVerts_6F65A8[0]);
                MapRenderer::ProjectVertTop(gXCoord, gYCoord, &gTileVerts_6F65A8[1]);
                if (gTileVerts_6F65A8[0].x > gTileVerts_6F65A8[1].x)
                {
                    return;
                }
                MapRenderer::ProjectVertBottom(gXCoord, gYCoord + kZeroOnePoint.y, &gTileVerts_6F65A8[3]);
                MapRenderer::ProjectVertTop(gXCoord, gYCoord + kZeroOnePoint.y, &gTileVerts_6F65A8[2]);
                sTileDrawFlags = kLeftSideFlags[left_word >> 13];
                break;
        }

        u16 texture_idx = gGtx_0x106C->GetTile(left_word & block_face::tile_idx_mask);
        if (texture_idx)
        {
            if ((*(reinterpret_cast<u8*>(&left_word) + 1) & 0x10) != 0)
            {
                sTileDrawFlags = sTileDrawFlags | 0x80;
            }
            pgbh_DrawTile(sTileDrawFlags | gLightingDrawFlag,
                          gTextureCache_15D8->GetTexture(texture_idx),
                          gTileVerts_6F65A8,
                          mColourT1);
            ++mDrawnTileCount;
        }
    }
}

MATCH_FUNC(0x4ead90)
void __stdcall set_vert_xyz_relative_to_cam(Fix16 xCoord, Fix16 yCoord, Fix16 z_val, Vert* pVerts)
{
    Camera* pCam = gViewCamera;

    s32 next_idx = (pVerts - gTileVerts_6F65A8) + 4;

    // Only ProjectVert inlines this copy. The f32 locals (Fix16ToF32_Rounded) move the index
    // computation's mov behind the z temp store, as in the original (22 -> 4 lines).
    gTileVerts_6F65A8[next_idx].x = Fix16ToF32_Rounded(xCoord + pCam->mCurPos.mX);
    gTileVerts_6F65A8[next_idx].y = Fix16ToF32_Rounded(yCoord + pCam->mCurPos.mY);
    gTileVerts_6F65A8[next_idx].z = z_val.ToFloat();
}

// This function matches, but the offsets of gScreenScaleTop and gInvDepthTop are wrong
MATCH_FUNC(0x4eae00)
void MapRenderer::ProjectVertTop(Fix16& xpos, Fix16& ypos, Vert* pVert)
{
    set_vert_xyz_relative_to_cam(xpos, ypos, gZCoordTop, pVert);
    
    pVert->x = xpos.ToFloat() * gScreenScaleTop.ToFloat() + static_cast<u32>(gViewCamera->mScreenPxCenterX);
    pVert->y = ypos.ToFloat() * gScreenScaleTop.ToFloat() + static_cast<u32>(gViewCamera->mScreenPxCenterY);
    pVert->z = gInvDepthTop.ToFloat();
}

MATCH_FUNC(0x4eaea0)
void MapRenderer::ProjectVertBottom(Fix16& xCoord, Fix16& yCoord, Vert* pVert)
{
    set_vert_xyz_relative_to_cam(xCoord, yCoord, gZCoord, pVert);

    pVert->x = static_cast<u32>(gViewCamera->mScreenPxCenterX) + gScreenScaleBottom.ToFloat() * xCoord.ToFloat();
    pVert->y = static_cast<u32>(gViewCamera->mScreenPxCenterY) + gScreenScaleBottom.ToFloat() * yCoord.ToFloat();
    pVert->z = gInvDepthBottom.ToFloat();
}

// https://decomp.me/scratch/mWsfM
MATCH_FUNC(0x4eaf40)
void MapRenderer::DrawRightSide(u16& right_word)
{
    // Declared up front and assigned later: VC6 numbers it before sZCoordFp, so the z sum for
    // the out-of-line ProjectVert is `add %esi,%ecx` into the gZ load (like the original), not a `lea`
    Fix16 slope_z_4;
    if (!bSkip_right)
    {
        switch (gCurrentSlope.mGradientDirection)
        {
            case NORTH_1:
                gVertProjector.ProjectVertBottom(gXCoord + kZeroOnePoint.y, gYCoord, &gTileVerts_6F65A8[1]);

                if (gGradientLevel == 0)
                {
                    gVertProjector.ProjectVertTop(gXCoord + kZeroOnePoint.y, gYCoord, &gTileVerts_6F65A8[0]);
                    gTileVerts_6F65A8[0].v = 0.0f;
                }
                else
                {
                    Fix16 slope_z_1 = Fix16((gGradientSize - gGradientLevel)) / gGradientSize;
                    gVertProjector.ProjectVert(gXCoord + kZeroOnePoint.y, gYCoord, sZCoordFp + slope_z_1, &gTileVerts_6F65A8[0]);
                    gTileVerts_6F65A8[0].v = 63.999901f - (slope_z_1 * kTileTexSize).ToFloat();
                }

                if (gTileVerts_6F65A8[0].x > gTileVerts_6F65A8[1].x)
                {
                    return;
                }

                gVertProjector.ProjectVertBottom(gXCoord + kZeroOnePoint.y, gYCoord + kZeroOnePoint.y, &gTileVerts_6F65A8[2]);

                if (gGradientLevel == gGradientSize - 1)
                {
                    memcpy(&gTileVerts_6F65A8[3], &gTileVerts_6F65A8[2], sizeof(Vert));
                    gTileVerts_6F65A8[3].v = 63.999901f;
                }
                else
                {
                    Fix16 slope_z_2 = Fix16((gGradientSize - gGradientLevel - 1)) / gGradientSize;
                    gVertProjector.ProjectVert(gXCoord + kZeroOnePoint.y,
                               gYCoord + kZeroOnePoint.y,
                               sZCoordFp + slope_z_2,
                               &gTileVerts_6F65A8[3]);
                    gTileVerts_6F65A8[3].v = 63.999901f - (slope_z_2 * kTileTexSize).ToFloat();
                }

                // now set u
                sTileDrawFlags = 16389;
                if ((*(reinterpret_cast<u8*>(&right_word) + 1) & 0x20) != 0)
                {
                    gTileVerts_6F65A8[0].u = 0.0f;
                    gTileVerts_6F65A8[1].u = 0.0f;
                    gTileVerts_6F65A8[2].u = 63.999901f;
                    gTileVerts_6F65A8[3].u = 63.999901f;
                }
                else
                {
                    gTileVerts_6F65A8[0].u = 63.999901f;
                    gTileVerts_6F65A8[1].u = 63.999901f;
                    gTileVerts_6F65A8[2].u = 0.0f;
                    gTileVerts_6F65A8[3].u = 0.0f;
                }
                gTileVerts_6F65A8[1].v = 63.999901f;
                gTileVerts_6F65A8[2].v = 63.999901f;
                break;

            case SOUTH_2:
                gVertProjector.ProjectVertBottom(gXCoord + kZeroOnePoint.y, gYCoord + kZeroOnePoint.y, &gTileVerts_6F65A8[2]);
                if (gGradientLevel == 0)
                {
                    gVertProjector.ProjectVertTop(gXCoord + kZeroOnePoint.y, gYCoord + kZeroOnePoint.y, &gTileVerts_6F65A8[3]);
                    gTileVerts_6F65A8[3].v = 0.0f;
                }
                else
                {
                    Fix16 slope_z_3 = Fix16(gGradientSize - gGradientLevel) / gGradientSize;
                    gVertProjector.ProjectVert(gXCoord + kZeroOnePoint.y,
                               gYCoord + kZeroOnePoint.y,
                               sZCoordFp + slope_z_3,
                               &gTileVerts_6F65A8[3]);
                    gTileVerts_6F65A8[3].v = 63.999901f - (slope_z_3 * kTileTexSize).ToFloat();
                }

                if (gTileVerts_6F65A8[3].x > gTileVerts_6F65A8[2].x)
                {
                    return;
                }
                gVertProjector.ProjectVertBottom(gXCoord + kZeroOnePoint.y, gYCoord, &gTileVerts_6F65A8[1]);

                if (gGradientLevel == gGradientSize - 1)
                {
                    memcpy(&gTileVerts_6F65A8[0], &gTileVerts_6F65A8[1], sizeof(Vert));
                    gTileVerts_6F65A8[0].v = 63.999901f;
                }
                else
                {
                    slope_z_4 = Fix16(gGradientSize - gGradientLevel - 1) / gGradientSize;
                    ProjectVert(gXCoord + kZeroOnePoint.y, gYCoord, sZCoordFp + slope_z_4, &gTileVerts_6F65A8[0]);
                    gTileVerts_6F65A8[0].v = 63.999901f - (slope_z_4 * kTileTexSize).ToFloat();
                }
                // now set u
                sTileDrawFlags = 16389;
                if ((*(reinterpret_cast<u8*>(&right_word) + 1) & 0x20) != 0)
                {
                    gTileVerts_6F65A8[0].u = 0.0f;
                    gTileVerts_6F65A8[1].u = 0.0f;
                    gTileVerts_6F65A8[2].u = 63.999901f;
                    gTileVerts_6F65A8[3].u = 63.999901f;
                }
                else
                {
                    gTileVerts_6F65A8[0].u = 63.999901f;
                    gTileVerts_6F65A8[1].u = 63.999901f;
                    gTileVerts_6F65A8[2].u = 0.0f;
                    gTileVerts_6F65A8[3].u = 0.0f;
                }
                gTileVerts_6F65A8[1].v = 63.999901f;
                gTileVerts_6F65A8[2].v = 63.999901f;
                break;
            default:
                // Flat blocks
                MapRenderer::ProjectVertBottom(gXCoord + kZeroOnePoint.y, gYCoord, &gTileVerts_6F65A8[1]);
                MapRenderer::ProjectVertTop(gXCoord + kZeroOnePoint.y, gYCoord, &gTileVerts_6F65A8[0]);
                if (gTileVerts_6F65A8[0].x > gTileVerts_6F65A8[1].x)
                {
                    return;
                }
                MapRenderer::ProjectVertBottom(gXCoord + kZeroOnePoint.y, gYCoord + kZeroOnePoint.y, &gTileVerts_6F65A8[2]);
                MapRenderer::ProjectVertTop(gXCoord + kZeroOnePoint.y, gYCoord + kZeroOnePoint.y, &gTileVerts_6F65A8[3]);
                sTileDrawFlags = kRightSideFlags[right_word >> 13];
                break;
        }

        u16 texture_idx = gGtx_0x106C->GetTile(right_word & block_face::tile_idx_mask);
        if (texture_idx)
        {
            if ((*(reinterpret_cast<u8*>(&right_word) + 1) & 0x10) != 0)
            {
                sTileDrawFlags = sTileDrawFlags | 0x80;
            }
            pgbh_DrawTile(sTileDrawFlags | gLightingDrawFlag,
                          gTextureCache_15D8->GetTexture(texture_idx),
                          gTileVerts_6F65A8,
                          mRightColour);
            ++mDrawnTileCount;
        }
    }
}

// https://decomp.me/scratch/GkGnQ
MATCH_FUNC(0x4EB940)
void MapRenderer::ProjectVert(Fix16& xpos, Fix16& ypos, Fix16& zpos, Vert* pVert)
{
    set_vert_xyz_relative_to_cam_inlined(xpos, ypos, zpos, pVert);

    // The same conversion forms as VertProjector3::ProjectVert (field_60 is loaded first here too).
    pVert->z = 1.0f / (gViewCamera->mCurPos.mZ.ToFloat() + (8.0f - zpos.ToFloat()));
    pVert->x = Fix16ToF32_Rounded2(xpos) * Fix16ToF32_Rounded3(gViewCamera->mScreenScale.x) * pVert->z + static_cast<u32>(gViewCamera->mScreenPxCenterX);
    // The y line's parentheses are x87 no-op nodes: the one after the product delays `pop ebx` past
    // `fmuls 8(%ecx)` in the epilogue (see Scripts/x87_sched).
    pVert->y = (Fix16ToF32_Rounded2(ypos) * Fix16ToF32_Rounded3(gViewCamera->mScreenScale.x)) * (pVert->z) + static_cast<u32>(gViewCamera->mScreenPxCenterY);
}

// https://decomp.me/scratch/a6z18
MATCH_FUNC(0x4eba60)
void MapRenderer::DrawTopSide(u16& top_word)
{
    if (!bSkip_top)
    {
        switch (gCurrentSlope.mGradientDirection)
        {
            case WEST_3:
                gVertProjector.ProjectVertBottom(gXCoord, gYCoord, &gTileVerts_6F65A8[0]);
                if (gGradientLevel == 0)
                {
                    gVertProjector.ProjectVertTop(gXCoord, gYCoord, &gTileVerts_6F65A8[3]);
                    gTileVerts_6F65A8[3].v = 0.0f;
                }
                else
                {
                    Fix16 slope_z_1 = Fix16(gGradientSize - gGradientLevel) / gGradientSize;
                    gVertProjector.ProjectVert(gXCoord, gYCoord, sZCoordFp + slope_z_1, &gTileVerts_6F65A8[3]);
                    gTileVerts_6F65A8[3].v = 63.999901f - (slope_z_1 * kTileTexSize).ToFloat();
                }

                if (gTileVerts_6F65A8[0].y > gTileVerts_6F65A8[3].y)
                {
                    return;
                }

                gVertProjector.ProjectVertBottom(gXCoord + kZeroOnePoint.y, gYCoord, &gTileVerts_6F65A8[1]);

                if (gGradientLevel == gGradientSize - 1)
                {
                    memcpy(&gTileVerts_6F65A8[2], &gTileVerts_6F65A8[1], sizeof(Vert));
                    gTileVerts_6F65A8[2].v = 63.999901f;
                }
                else
                {
                    Fix16 slope_z_2 = Fix16(gGradientSize - gGradientLevel - 1) / gGradientSize;
                    gVertProjector.ProjectVert(gXCoord + kZeroOnePoint.y, gYCoord, sZCoordFp + slope_z_2, &gTileVerts_6F65A8[2]);
                    gTileVerts_6F65A8[2].v = 63.999901f - (slope_z_2 * kTileTexSize).ToFloat();
                }
                sTileDrawFlags = 16389;
                if ((*(reinterpret_cast<u8*>(&top_word) + 1) & 0x20) != 0)
                {
                    gTileVerts_6F65A8[0].u = 0.0f;
                    gTileVerts_6F65A8[1].u = 63.999901f;
                    gTileVerts_6F65A8[2].u = 63.999901f;
                    gTileVerts_6F65A8[3].u = 0.0f;
                }
                else
                {
                    gTileVerts_6F65A8[0].u = 63.999901f;
                    gTileVerts_6F65A8[1].u = 0.0f;
                    gTileVerts_6F65A8[2].u = 0.0f;
                    gTileVerts_6F65A8[3].u = 63.999901f;
                }
                gTileVerts_6F65A8[0].v = 63.999901f;
                gTileVerts_6F65A8[1].v = 63.999901f;

                break;
            case EAST_4:

                if (gGradientLevel == 0)
                {
                    gVertProjector.ProjectVertTop(gXCoord + kZeroOnePoint.y, gYCoord, &gTileVerts_6F65A8[2]);
                    gTileVerts_6F65A8[2].v = 0.0f;
                }
                else
                {
                    Fix16 slope_z_3 = Fix16(gGradientSize - gGradientLevel) / gGradientSize;
                    gVertProjector.ProjectVert(gXCoord + kZeroOnePoint.y, gYCoord, sZCoordFp + slope_z_3, &gTileVerts_6F65A8[2]);
                    gTileVerts_6F65A8[2].v = 63.999901f - (slope_z_3 * kTileTexSize).ToFloat();
                }

                gVertProjector.ProjectVertBottom(gXCoord + kZeroOnePoint.y, gYCoord, &gTileVerts_6F65A8[1]);

                if (gTileVerts_6F65A8[1].y > gTileVerts_6F65A8[2].y)
                {
                    return;
                }

                gVertProjector.ProjectVertBottom(gXCoord, gYCoord, &gTileVerts_6F65A8[0]);

                if (gGradientLevel == gGradientSize - 1)
                {
                    memcpy(&gTileVerts_6F65A8[3], &gTileVerts_6F65A8[0], sizeof(Vert));
                    gTileVerts_6F65A8[3].v = 63.999901f;
                }
                else
                {
                    Fix16 slope_z_4 = Fix16(gGradientSize - gGradientLevel - 1) / gGradientSize;
                    gVertProjector.ProjectVert(gXCoord, gYCoord, sZCoordFp + slope_z_4, &gTileVerts_6F65A8[3]);
                    gTileVerts_6F65A8[3].v = 63.999901f - (slope_z_4 * kTileTexSize).ToFloat();
                }

                sTileDrawFlags = 16389;
                if ((*(reinterpret_cast<u8*>(&top_word) + 1) & 0x20) != 0)
                {
                    gTileVerts_6F65A8[0].u = 0.0f;
                    gTileVerts_6F65A8[1].u = 63.999901f;
                    gTileVerts_6F65A8[2].u = 63.999901f;
                    gTileVerts_6F65A8[3].u = 0.0f;
                }
                else
                {
                    gTileVerts_6F65A8[0].u = 63.999901f;
                    gTileVerts_6F65A8[1].u = 0.0f;
                    gTileVerts_6F65A8[2].u = 0.0f;
                    gTileVerts_6F65A8[3].u = 63.999901f;
                }
                gTileVerts_6F65A8[0].v = 63.999901f;
                gTileVerts_6F65A8[1].v = 63.999901f;

                break;
            default:
                // Flat blocks
                MapRenderer::ProjectVertBottom(gXCoord, gYCoord, &gTileVerts_6F65A8[0]);
                MapRenderer::ProjectVertTop(gXCoord, gYCoord, &gTileVerts_6F65A8[3]);
                if (gTileVerts_6F65A8[0].y > gTileVerts_6F65A8[3].y)
                {
                    return;
                }
                MapRenderer::ProjectVertBottom(gXCoord + kZeroOnePoint.y, gYCoord, &gTileVerts_6F65A8[1]);
                MapRenderer::ProjectVertTop(gXCoord + kZeroOnePoint.y, gYCoord, &gTileVerts_6F65A8[2]);
                sTileDrawFlags = kTopSideFlags[top_word >> 13];
                break;
        }

        u16 texture_idx = gGtx_0x106C->GetTile(top_word & block_face::tile_idx_mask);
        if (texture_idx != 0)
        {
            if ((*(reinterpret_cast<u8*>(&top_word) + 1) & 0x10) != 0)
            {
                sTileDrawFlags = sTileDrawFlags | 0x80;
            }
            pgbh_DrawTile(sTileDrawFlags | gLightingDrawFlag,
                          gTextureCache_15D8->mTileTextures[texture_idx],
                          gTileVerts_6F65A8,
                          mColourT3);
            ++mDrawnTileCount;
        }
    }
}

// https://decomp.me/scratch/MyepN
MATCH_FUNC(0x4ec450)
void MapRenderer::DrawDiagonalUpLeftFace(u16& left_word)
{
    Ang16 rotation;
    gVertProjector2.ProjectVertBottom(gXCoord + kZeroOnePoint.y, gYCoord, &gTileVerts_6F65A8[0]);
    gVertProjector2.ProjectVertTop(gXCoord + kZeroOnePoint.y, gYCoord, &gTileVerts_6F65A8[1]);

    rotation = Fix16::atan2_fixed(Fix16(gTileVerts_6F65A8[1].y - gTileVerts_6F65A8[0].y),
                                         Fix16(gTileVerts_6F65A8[1].x - gTileVerts_6F65A8[0].x));

    if (rotation < kAng135_6F6414 || rotation > kAng315)
    {
        gVertProjector2.ProjectVertTop(gXCoord, gYCoord + kZeroOnePoint.y, &gTileVerts_6F65A8[2]);
        gVertProjector2.ProjectVertBottom(gXCoord, gYCoord + kZeroOnePoint.y, &gTileVerts_6F65A8[3]);

        sTileDrawFlags = kDiagonalLeftFlags[left_word >> 13];
        u16 texture_idx = gGtx_0x106C->GetTile(left_word & block_face::tile_idx_mask);
        if (texture_idx)
        {
            pgbh_DrawTile(sTileDrawFlags | gLightingDrawFlag,
                          gTextureCache_15D8->GetTexture(texture_idx),
                          gTileVerts_6F65A8,
                          mDiagUpLeftColour);
            ++mDrawnTileCount;
        }
    }
}

// https://decomp.me/scratch/8QjCQ
MATCH_FUNC(0x4ec7a0)
void MapRenderer::DrawDiagonalUpRightFace(u16& right_word)
{
    Ang16 rotation;
    gVertProjector2.ProjectVertTop(gXCoord, gYCoord, &gTileVerts_6F65A8[0]);
    gVertProjector2.ProjectVertBottom(gXCoord, gYCoord, &gTileVerts_6F65A8[1]);

    rotation = Fix16::atan2_fixed(Fix16(gTileVerts_6F65A8[1].y - gTileVerts_6F65A8[0].y),
                                         Fix16(gTileVerts_6F65A8[1].x - gTileVerts_6F65A8[0].x));

    if (rotation > kAng45_6F637C && rotation < kAng225)
    {
        gVertProjector2.ProjectVertBottom(gXCoord + kZeroOnePoint.y, gYCoord + kZeroOnePoint.y, &gTileVerts_6F65A8[2]);
        gVertProjector2.ProjectVertTop(gXCoord + kZeroOnePoint.y, gYCoord + kZeroOnePoint.y, &gTileVerts_6F65A8[3]);

        sTileDrawFlags = kDiagonalRightFlags[right_word >> 13];
        u16 texture_idx = gGtx_0x106C->GetTile(right_word & block_face::tile_idx_mask);
        if (texture_idx)
        {
            pgbh_DrawTile(sTileDrawFlags | gLightingDrawFlag,
                          gTextureCache_15D8->GetTexture(texture_idx),
                          gTileVerts_6F65A8,
                          mDiagUpRightColour);
            ++mDrawnTileCount;
        }
    }
}

// https://decomp.me/scratch/eGEBV
// 9.6f 0x46D680
MATCH_FUNC(0x4ecaf0)
void MapRenderer::DrawDiagonalDownLeftFace(u16& left_word)
{
    Ang16 angle;
    gVertProjector2.ProjectVertBottom(gXCoord, gYCoord, &gTileVerts_6F65A8[0]);
    gVertProjector2.ProjectVertTop(gXCoord, gYCoord, &gTileVerts_6F65A8[1]);

    angle = Fix16::atan2_fixed(Fix16(gTileVerts_6F65A8[1].y - gTileVerts_6F65A8[0].y),
                                      Fix16(gTileVerts_6F65A8[1].x - gTileVerts_6F65A8[0].x));

    if (angle > kAng45_6F637C && angle < kAng225)
    {
        gVertProjector2.ProjectVertTop(gXCoord + kZeroOnePoint.y, gYCoord + kZeroOnePoint.y, &gTileVerts_6F65A8[2]);
        gVertProjector2.ProjectVertBottom(gXCoord + kZeroOnePoint.y, gYCoord + kZeroOnePoint.y, &gTileVerts_6F65A8[3]);
        sTileDrawFlags = kDiagonalLeftFlags[left_word >> 13];
        u16 texture_idx = gGtx_0x106C->GetTile(left_word & block_face::tile_idx_mask);
        if (texture_idx)
        {
            pgbh_DrawTile(sTileDrawFlags | gLightingDrawFlag,
                          gTextureCache_15D8->GetTexture(texture_idx),
                          gTileVerts_6F65A8,
                          mDiagDownLeftColour);
            ++mDrawnTileCount;
        }
    }
}

// https://decomp.me/scratch/3kT2F
// 9.6f 0x46D810
MATCH_FUNC(0x4ece40)
void MapRenderer::DrawDiagonalDownRightFace(u16& right_word)
{
    Ang16 angle;
    gVertProjector2.ProjectVertTop(gXCoord + kZeroOnePoint.y, gYCoord, &gTileVerts_6F65A8[0]);
    gVertProjector2.ProjectVertBottom(gXCoord + kZeroOnePoint.y, gYCoord, &gTileVerts_6F65A8[1]);

    angle = Fix16::atan2_fixed(Fix16(gTileVerts_6F65A8[1].y - gTileVerts_6F65A8[0].y),
                                     Fix16(gTileVerts_6F65A8[1].x - gTileVerts_6F65A8[0].x));

    if (angle < kAng135_6F6414 || angle > kAng315)
    {
        gVertProjector2.ProjectVertBottom(gXCoord, gYCoord + kZeroOnePoint.y, &gTileVerts_6F65A8[2]);
        gVertProjector2.ProjectVertTop(gXCoord, gYCoord + kZeroOnePoint.y, &gTileVerts_6F65A8[3]);
        sTileDrawFlags = kDiagonalRightFlags[right_word >> 13];
        u16 texture_idx = gGtx_0x106C->GetTile(right_word & block_face::tile_idx_mask);
        if (texture_idx)
        {
            pgbh_DrawTile(sTileDrawFlags | gLightingDrawFlag,
                          gTextureCache_15D8->GetTexture(texture_idx),
                          gTileVerts_6F65A8,
                          mDiagDownRightColour);
            ++mDrawnTileCount;
        }
    }
}

// https://decomp.me/scratch/4EDti
// 9.6f 0x46D9A0
MATCH_FUNC(0x4ed290)
void MapRenderer::draw_bottom(u16& bottom_word)
{
    // See DrawRightSide: declared up front for the operand order of the z sum. That raises the
    // inline budget; `if (texture_idx)` below takes it back so the default case's operator+ stays out of line
    Fix16 slope_z_4;
    if (!bSkip_bottom)
    {
        switch (gCurrentSlope.mGradientDirection)
        {
            case WEST_3:
                gVertProjector.ProjectVertBottom(gXCoord, gYCoord + kZeroOnePoint.y, &gTileVerts_6F65A8[3]);
                
                if (gGradientLevel == 0)
                {
                    gVertProjector.ProjectVertTop(gXCoord, gYCoord + kZeroOnePoint.y, &gTileVerts_6F65A8[0]);
                    gTileVerts_6F65A8[0].v = 0.0f;
                }
                else
                {
                    Fix16 slope_z_1 = Fix16(gGradientSize - gGradientLevel) / gGradientSize;
                    gVertProjector.ProjectVert(gXCoord,
                              gYCoord + kZeroOnePoint.y,
                              sZCoordFp + slope_z_1,
                              &gTileVerts_6F65A8[0]);
                    gTileVerts_6F65A8[0].v = 63.999901f - (slope_z_1 * kTileTexSize).ToFloat();
                }

                if ( gTileVerts_6F65A8[0].y > gTileVerts_6F65A8[3].y )
                {
                    return;
                }

                gVertProjector.ProjectVertBottom(gXCoord + kZeroOnePoint.y, 
                           gYCoord + kZeroOnePoint.y,
                          &gTileVerts_6F65A8[2]);
                
                if ( gGradientLevel == gGradientSize - 1 )
                {
                    memcpy(&gTileVerts_6F65A8[1], &gTileVerts_6F65A8[2], sizeof(Vert));
                    gTileVerts_6F65A8[1].v = 63.999901f;
                }
                else
                {
                    Fix16 slope_z_2 = Fix16(gGradientSize - gGradientLevel - 1) / gGradientSize;
                    gVertProjector.ProjectVert(gXCoord + kZeroOnePoint.y, 
                               gYCoord + kZeroOnePoint.y,
                               sZCoordFp + slope_z_2,
                               &gTileVerts_6F65A8[1]);
                    gTileVerts_6F65A8[1].v = 63.999901f - (slope_z_2 * kTileTexSize).ToFloat();
                }

                sTileDrawFlags = 16389;
                if ((*(reinterpret_cast<u8*>(&bottom_word) + 1) & 0x20) != 0)
                {
                    gTileVerts_6F65A8[0].u = 63.999901f;
                    gTileVerts_6F65A8[1].u = 0.0f;
                    gTileVerts_6F65A8[2].u = 0.0f;
                    gTileVerts_6F65A8[3].u = 63.999901f;
                }
                else
                {
                    gTileVerts_6F65A8[0].u = 0.0f;
                    gTileVerts_6F65A8[1].u = 63.999901f;
                    gTileVerts_6F65A8[2].u = 63.999901f;
                    gTileVerts_6F65A8[3].u = 0.0f;
                }
                gTileVerts_6F65A8[2].v = 63.999901f;
                gTileVerts_6F65A8[3].v = 63.999901f;
                break;
            
            case EAST_4:
                
                gVertProjector.ProjectVertBottom(gXCoord + kZeroOnePoint.y, 
                           gYCoord + kZeroOnePoint.y,
                           &gTileVerts_6F65A8[2]);
                
                if (gGradientLevel == 0)
                {
                    gVertProjector.ProjectVertTop(gXCoord + kZeroOnePoint.y, 
                              gYCoord + kZeroOnePoint.y,
                              &gTileVerts_6F65A8[1]);
                    gTileVerts_6F65A8[1].v = 0.0f;
                }
                else
                {
                    Fix16 slope_z_3 = Fix16(gGradientSize - gGradientLevel) / gGradientSize;
                    gVertProjector.ProjectVert(gXCoord + kZeroOnePoint.y, 
                              gYCoord + kZeroOnePoint.y,
                              sZCoordFp + slope_z_3,
                              &gTileVerts_6F65A8[1]);
                    gTileVerts_6F65A8[1].v = 63.999901f - (slope_z_3 * kTileTexSize).ToFloat();
                }

                if (gTileVerts_6F65A8[1].y > gTileVerts_6F65A8[2].y)
                {
                    return;
                }

                gVertProjector.ProjectVertBottom(gXCoord, gYCoord + kZeroOnePoint.y, &gTileVerts_6F65A8[3]);

                if ( gGradientLevel == gGradientSize - 1 )
                {
                    memcpy(&gTileVerts_6F65A8[0], &gTileVerts_6F65A8[3], sizeof(Vert));
                    gTileVerts_6F65A8[0].v = 63.999901f;
                }
                else
                {
                    slope_z_4 = Fix16(gGradientSize - gGradientLevel - 1) / gGradientSize;
                    ProjectVert(gXCoord, 
                               gYCoord + kZeroOnePoint.y,
                               sZCoordFp + slope_z_4,
                               &gTileVerts_6F65A8[0]);
                    gTileVerts_6F65A8[0].v = 63.999901f - (slope_z_4 * kTileTexSize).ToFloat();
                }
                sTileDrawFlags = 16389;
                if ((*(reinterpret_cast<u8*>(&bottom_word) + 1) & 0x20) != 0)
                {
                    gTileVerts_6F65A8[0].u = 63.999901f;
                    gTileVerts_6F65A8[1].u = 0.0f;
                    gTileVerts_6F65A8[2].u = 0.0f;
                    gTileVerts_6F65A8[3].u = 63.999901f;
                }
                else
                {
                    gTileVerts_6F65A8[0].u = 0.0f;
                    gTileVerts_6F65A8[1].u = 63.999901f;
                    gTileVerts_6F65A8[2].u = 63.999901f;
                    gTileVerts_6F65A8[3].u = 0.0f;
                }
                gTileVerts_6F65A8[2].v = 63.999901f;
                gTileVerts_6F65A8[3].v = 63.999901f;
                break;
            default:
                // Flat blocks
                MapRenderer::ProjectVertBottom(gXCoord, gYCoord + kZeroOnePoint.y, &gTileVerts_6F65A8[3]);
                MapRenderer::ProjectVertTop(gXCoord, gYCoord + kZeroOnePoint.y, &gTileVerts_6F65A8[0]);
                if (gTileVerts_6F65A8[0].y > gTileVerts_6F65A8[3].y)
                {
                    return;
                }
                MapRenderer::ProjectVertBottom(gXCoord + kZeroOnePoint.y, gYCoord + kZeroOnePoint.y, &gTileVerts_6F65A8[2]);
                MapRenderer::ProjectVertTop(gXCoord + kZeroOnePoint.y, gYCoord + kZeroOnePoint.y, &gTileVerts_6F65A8[1]);
                sTileDrawFlags = kBottomSideFlags[bottom_word >> 13];
                break;
        }

        u16 texture_idx = gGtx_0x106C->GetTile(bottom_word & block_face::tile_idx_mask);
        if (texture_idx)
        {
            if ((*(reinterpret_cast<u8*>(&bottom_word) + 1) & 0x10) != 0)
            {
                sTileDrawFlags = sTileDrawFlags | 0x80;
            }
            pgbh_DrawTile(sTileDrawFlags | gLightingDrawFlag,
                                 gTextureCache_15D8->GetTexture(texture_idx),
                                 gTileVerts_6F65A8,
                                 mColourT2);
            ++mDrawnTileCount;
        }
    }
}

// https://decomp.me/scratch/pJOfC
MATCH_FUNC(0x4ee130)
void MapRenderer::draw_lid()
{
    LOCAL_STATIC_ARRAY_INIT(s32, kDiagonalLidFlags, 8, 0x621024, 5 COMMA 0x0D COMMA 0x25 COMMA 0x2D COMMA 0x45 COMMA 0x4D COMMA 0x65 COMMA 0x6D);
    if (!bSkip_lid)
    {
        gVertProjector2.ProjectVertTop(gXCoord, gYCoord, &gTileVerts_6F65A8[0]);
        gVertProjector2.ProjectVertTop(gXCoord + kZeroOnePoint.y, gYCoord, &gTileVerts_6F65A8[1]);
        gVertProjector2.ProjectVertTop(gXCoord + kZeroOnePoint.y, gYCoord + kZeroOnePoint.y, &gTileVerts_6F65A8[2]);
        gVertProjector2.ProjectVertTop(gXCoord, gYCoord + kZeroOnePoint.y, &gTileVerts_6F65A8[3]);

        u16 texture_idx = gGtx_0x106C->GetTile(sLidType & block_face::tile_idx_mask);

        if (texture_idx)
        {
            switch (sDiagonalLidType)
            {
                case -1:
                    sTileDrawFlags = kLidFlags[static_cast<u16>(sLidType) >> 13];
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
                    sTileDrawFlags = kDiagonalRightFlags[static_cast<u16>(sLidType) >> 13];
                    break;
                case 1:
                    memcpy(&gTileVerts_6F65A8[1], &gTileVerts_6F65A8[2], sizeof(Vert));
                    memcpy(&gTileVerts_6F65A8[5], &gTileVerts_6F65A8[6], sizeof(Vert));
                    sTileDrawFlags = kDiagonalLidFlags[static_cast<u16>(sLidType) >> 13];
                    break;
                case 2:
                    memcpy(&gTileVerts_6F65A8[2], &gTileVerts_6F65A8[1], sizeof(Vert));
                    memcpy(&gTileVerts_6F65A8[6], &gTileVerts_6F65A8[5], sizeof(Vert));
                    memcpy(&gTileVerts_6F65A8[1], &gTileVerts_6F65A8[0], sizeof(Vert));
                    memcpy(&gTileVerts_6F65A8[5], &gTileVerts_6F65A8[4], sizeof(Vert));
                    memcpy(&gTileVerts_6F65A8[0], &gTileVerts_6F65A8[3], sizeof(Vert));
                    memcpy(&gTileVerts_6F65A8[4], &gTileVerts_6F65A8[7], sizeof(Vert));
                    sTileDrawFlags = kDiagonalLeftFlags[static_cast<u16>(sLidType) >> 13];
                    break;
                case 3:
                    memcpy(&gTileVerts_6F65A8[3], &gTileVerts_6F65A8[0], sizeof(Vert));
                    memcpy(&gTileVerts_6F65A8[7], &gTileVerts_6F65A8[4], sizeof(Vert));
                    sTileDrawFlags = kDiagonalLidFlags[static_cast<u16>(sLidType) >> 13];
                    break;
                default:
                    break;
            }
            if ((sLidType & block_face::flat) != 0)
            {
                sTileDrawFlags |= 0x80;
            }
            pgbh_DrawTile(sTileDrawFlags | gLightingDrawFlag,
                          gTextureCache_15D8->GetTexture(texture_idx),
                          gTileVerts_6F65A8,
                          GetColour((sLidType >> 10) & 3));
            ++mDrawnTileCount;
        }
    }
}

MATCH_FUNC(0x4EE7D0)
void MapRenderer::DrawDiagonalWallUpLeft()
{
    if (gBlockRight)
    {
        MapRenderer::DrawRightSide(gBlockRight);
    }
    if (gBlockLeft)
    {
        MapRenderer::DrawDiagonalUpLeftFace(gBlockLeft);
    }
    if (static_cast<u16>(sBlockBottom))
    {
        MapRenderer::draw_bottom(sBlockBottom);
    }
    if (static_cast<u16>(sLidType))
    {
        sDiagonalLidType = 0;
        MapRenderer::draw_lid();
        sDiagonalLidType = -1;
    }
}

MATCH_FUNC(0x4EE8A0)
void MapRenderer::DrawDiagonalWallUpRight()
{
    if (gBlockLeft)
    {
        MapRenderer::DrawLeftSide(gBlockLeft);
    }
    if (gBlockRight)
    {
        MapRenderer::DrawDiagonalUpRightFace(gBlockRight);
    }
    if (static_cast<u16>(sBlockBottom))
    {
        MapRenderer::draw_bottom(sBlockBottom);
    }
    if (static_cast<u16>(sLidType))
    {
        sDiagonalLidType = 1;
        MapRenderer::draw_lid();
        sDiagonalLidType = -1;
    }
}

MATCH_FUNC(0x4EE970)
void MapRenderer::DrawDiagonalWallDownLeft()
{
    if (gBlockLeft)
    {
        MapRenderer::DrawDiagonalDownLeftFace(gBlockLeft);
    }
    if (gBlockRight)
    {
        MapRenderer::DrawRightSide(gBlockRight);
    }
    if (sBlockTop)
    {
        MapRenderer::DrawTopSide(sBlockTop);
    }
    if (static_cast<u16>(sLidType))
    {
        sDiagonalLidType = 3;
        MapRenderer::draw_lid();
        sDiagonalLidType = -1;
    }
}

MATCH_FUNC(0x4EEA40)
void MapRenderer::DrawDiagonalWallDownRight()
{
    if (gBlockLeft)
    {
        MapRenderer::DrawLeftSide(gBlockLeft);
    }
    if (gBlockRight)
    {
        MapRenderer::DrawDiagonalDownRightFace(gBlockRight);
    }
    if (sBlockTop)
    {
        MapRenderer::DrawTopSide(sBlockTop);
    }
    if (static_cast<u16>(sLidType))
    {
        sDiagonalLidType = 2;
        MapRenderer::draw_lid();
        sDiagonalLidType = -1;
    }
}

// https://decomp.me/scratch/husii
MATCH_FUNC(0x4eeaf0)
void MapRenderer::Draw3SidedDiagonalUpLeft()
{
    if (gBlockRight)
    {
        gCurrentSlope.mGradientDirection = SOUTH_2;
        MapRenderer::DrawRightSide(gBlockRight);
    }

    if (sBlockBottom)
    {
        gCurrentSlope.mGradientDirection = EAST_4;
        MapRenderer::draw_bottom(sBlockBottom);
    }
    
    if (gBlockLeft)
    {
        gVertProjector2.ProjectVertTop(gXCoord + kZeroOnePoint.y, gYCoord + kZeroOnePoint.y, &gTileVerts_6F65A8[0]);
        gTileVerts_6F65A8[0].u = 32.0f;
        gTileVerts_6F65A8[0].v = 0.0f;
        gVertProjector2.ProjectVertBottom(gXCoord, gYCoord + kZeroOnePoint.y, &gTileVerts_6F65A8[1]);
        gTileVerts_6F65A8[1].u = 63.999901f;
        gTileVerts_6F65A8[1].v = 63.999901f;
        gVertProjector2.ProjectVertBottom(gXCoord + kZeroOnePoint.y, gYCoord, &gTileVerts_6F65A8[2]);
        gTileVerts_6F65A8[2].u = 0.0f;
        gTileVerts_6F65A8[2].v = 63.999901f;
        draw_triangle(gBlockLeft, false, mColor);
    }
}

// https://decomp.me/scratch/XGan6
MATCH_FUNC(0x4eee60)
void MapRenderer::Draw3SidedDiagonalUpRight()
{
    
    if (gBlockLeft)
    {
        gCurrentSlope.mGradientDirection = SOUTH_2;
        MapRenderer::DrawLeftSide(gBlockLeft);
    }

    if (sBlockBottom)
    {
        gCurrentSlope.mGradientDirection = WEST_3;
        MapRenderer::draw_bottom(sBlockBottom);
    }

    if (gBlockRight)
    {
        gVertProjector2.ProjectVertTop(gXCoord, gYCoord + kZeroOnePoint.y, &gTileVerts_6F65A8[0]);
        gTileVerts_6F65A8[0].u = 32.0f;
        gTileVerts_6F65A8[0].v = 0.0f;
        gVertProjector2.ProjectVertBottom(gXCoord, gYCoord, &gTileVerts_6F65A8[1]);
        gTileVerts_6F65A8[1].u = 63.999901f;
        gTileVerts_6F65A8[1].v = 63.999901f;
        gVertProjector2.ProjectVertBottom(gXCoord + kZeroOnePoint.y, gYCoord + kZeroOnePoint.y, &gTileVerts_6F65A8[2]);
        gTileVerts_6F65A8[2].u = 0.0f;
        gTileVerts_6F65A8[2].v = 63.999901f;
        draw_triangle(gBlockRight, false, mTriDiagUpRightColour);
    }
}

// https://decomp.me/scratch/O0oa4
MATCH_FUNC(0x4ef1c0)
void MapRenderer::Draw3SidedDiagonalDownLeft()
{
    
    if (gBlockRight)
    {
        gCurrentSlope.mGradientDirection = NORTH_1;
        MapRenderer::DrawRightSide(gBlockRight);
    }
    
    if (sBlockTop)
    {
        gCurrentSlope.mGradientDirection = EAST_4;
        MapRenderer::DrawTopSide(sBlockTop);
    }
    
    if (gBlockLeft)
    {
        gVertProjector2.ProjectVertTop(gXCoord + kZeroOnePoint.y, gYCoord, &gTileVerts_6F65A8[0]);
        gTileVerts_6F65A8[0].u = 32.0f;
        gTileVerts_6F65A8[0].v = 0.0f;
        gVertProjector2.ProjectVertBottom(gXCoord + kZeroOnePoint.y, gYCoord + kZeroOnePoint.y, &gTileVerts_6F65A8[1]);
        gTileVerts_6F65A8[1].u = 63.999901f;
        gTileVerts_6F65A8[1].v = 63.999901f;
        gVertProjector2.ProjectVertBottom(gXCoord, gYCoord, &gTileVerts_6F65A8[2]);
        gTileVerts_6F65A8[2].u = 0.0f;
        gTileVerts_6F65A8[2].v = 63.999901f;
        draw_triangle(gBlockLeft, false, mTriDiagDownLeftColour);
    }
}

// https://decomp.me/scratch/6WLOw
MATCH_FUNC(0x4ef520)
void MapRenderer::Draw3SidedDiagonalDownRight()
{

    if (gBlockLeft)
    {
        gCurrentSlope.mGradientDirection = NORTH_1;
        MapRenderer::DrawLeftSide(gBlockLeft);
    }
    
    if (sBlockTop)
    {
        gCurrentSlope.mGradientDirection = WEST_3;
        MapRenderer::DrawTopSide(sBlockTop);
    }
    
    if (gBlockRight)
    {
        gVertProjector2.ProjectVertTop(gXCoord, gYCoord, &gTileVerts_6F65A8[0]);
        gTileVerts_6F65A8[0].u = 32.0f;
        gTileVerts_6F65A8[0].v = 0.0f;
        gVertProjector2.ProjectVertBottom(gXCoord + kZeroOnePoint.y, gYCoord, &gTileVerts_6F65A8[1]);
        gTileVerts_6F65A8[1].u = 63.999901f;
        gTileVerts_6F65A8[1].v = 63.999901f;
        gVertProjector2.ProjectVertBottom(gXCoord, gYCoord + kZeroOnePoint.y, &gTileVerts_6F65A8[2]);
        gTileVerts_6F65A8[2].u = 0.0f;
        gTileVerts_6F65A8[2].v = 63.999901f;
        draw_triangle(gBlockRight, false, mTriDiagDownRightColour);
    }
}

// https://decomp.me/scratch/YoWOt
MATCH_FUNC(0x4ef880)
void MapRenderer::Draw4SidedDiagonalUpLeft()
{
    if (gBlockRight)
    {
        MapRenderer::DrawRightSide(gBlockRight);
    }
    if (gBlockLeft)
    {
        gVertProjector2.ProjectVertTop(gXCoord + kZeroOnePoint.y, gYCoord, &gTileVerts_6F65A8[0]);
        gTileVerts_6F65A8[0].u = 0.0f;
        gTileVerts_6F65A8[0].v = 0.0f;
        gVertProjector2.ProjectVertBottom(gXCoord, gYCoord, &gTileVerts_6F65A8[1]);
        gTileVerts_6F65A8[1].u = 32.0f;
        gTileVerts_6F65A8[1].v = 63.999901f;
        gVertProjector2.ProjectVertTop(gXCoord, gYCoord + kZeroOnePoint.y, &gTileVerts_6F65A8[2]);
        gTileVerts_6F65A8[2].u = 63.999901f;
        gTileVerts_6F65A8[2].v = 0.0f;
        draw_triangle(gBlockLeft, true, mColor);
    }
    if (sBlockBottom)
    {
        MapRenderer::draw_bottom(sBlockBottom);
    }
    if (sLidType)
    {
        sDiagonalLidType = 0;
        MapRenderer::draw_lid();
        sDiagonalLidType = -1;
    }
}

// https://decomp.me/scratch/ZTyHA
MATCH_FUNC(0x4efb20)
void MapRenderer::Draw4SidedDiagonalUpRight()
{
    if (gBlockLeft)
    {
        MapRenderer::DrawLeftSide(gBlockLeft);
    }
    if (gBlockRight)
    {
        gVertProjector2.ProjectVertTop(gXCoord + kZeroOnePoint.y, gYCoord + kZeroOnePoint.y, &gTileVerts_6F65A8[0]);
        gTileVerts_6F65A8[0].u = 0.0f;
        gTileVerts_6F65A8[0].v = 0.0f;
        gVertProjector2.ProjectVertBottom(gXCoord + kZeroOnePoint.y, gYCoord, &gTileVerts_6F65A8[1]);
        gTileVerts_6F65A8[1].u = 32.0f;
        gTileVerts_6F65A8[1].v = 63.999901f;
        gVertProjector2.ProjectVertTop(gXCoord, gYCoord, &gTileVerts_6F65A8[2]);
        gTileVerts_6F65A8[2].u = 63.999901f;
        gTileVerts_6F65A8[2].v = 0.0f;
        draw_triangle(gBlockRight, true, mTriDiagUpRightColour);
    }
    if (sBlockBottom)
    {
        MapRenderer::draw_bottom(sBlockBottom);
    }
    if (sLidType)
    {
        sDiagonalLidType = 1;
        MapRenderer::draw_lid();
        sDiagonalLidType = -1;
    }
}

// https://decomp.me/scratch/IdZ0n
MATCH_FUNC(0x4efdb0)
void MapRenderer::Draw4SidedDiagonalDownLeft()
{

    if (gBlockLeft)
    {
        gVertProjector2.ProjectVertTop(gXCoord, gYCoord, &gTileVerts_6F65A8[0]);
        gTileVerts_6F65A8[0].u = 0.0f;
        gTileVerts_6F65A8[0].v = 0.0f;
        gVertProjector2.ProjectVertBottom(gXCoord, gYCoord + kZeroOnePoint.y, &gTileVerts_6F65A8[1]);
        gTileVerts_6F65A8[1].u = 32.0f;
        gTileVerts_6F65A8[1].v = 63.999901f;
        gVertProjector2.ProjectVertTop(gXCoord + kZeroOnePoint.y, gYCoord + kZeroOnePoint.y, &gTileVerts_6F65A8[2]);
        gTileVerts_6F65A8[2].u = 63.999901f;
        gTileVerts_6F65A8[2].v = 0.0f;
        draw_triangle(gBlockLeft, true, mTriDiagDownLeftColour);
    }
    if (gBlockRight)
    {
        MapRenderer::DrawRightSide(gBlockRight);
    }
    if (sBlockTop)
    {
        MapRenderer::DrawTopSide(sBlockTop);
    }
    if (sLidType)
    {
        sDiagonalLidType = 3;
        MapRenderer::draw_lid();
        sDiagonalLidType = -1;
    }
}

// https://decomp.me/scratch/AKo7h
MATCH_FUNC(0x4f0030)
void MapRenderer::Draw4SidedDiagonalDownRight()
{
    if (gBlockLeft)
    {
        MapRenderer::DrawLeftSide(gBlockLeft);
    }
    if (gBlockRight)
    {
        gVertProjector2.ProjectVertTop(gXCoord, gYCoord + kZeroOnePoint.y, &gTileVerts_6F65A8[0]);
        gTileVerts_6F65A8[0].u = 0.0f;
        gTileVerts_6F65A8[0].v = 0.0f;
        gVertProjector2.ProjectVertBottom(gXCoord + kZeroOnePoint.y, gYCoord + kZeroOnePoint.y, &gTileVerts_6F65A8[1]);
        gTileVerts_6F65A8[1].u = 32.0f;
        gTileVerts_6F65A8[1].v = 63.999901f;
        gVertProjector2.ProjectVertTop(gXCoord + kZeroOnePoint.y, gYCoord, &gTileVerts_6F65A8[2]);
        gTileVerts_6F65A8[2].u = 63.999901f;
        gTileVerts_6F65A8[2].v = 0.0f;
        draw_triangle(gBlockRight, true, mTriDiagDownRightColour);
    }
    if (sBlockTop)
    {
        MapRenderer::DrawTopSide(sBlockTop);
    }
    if (sLidType)
    {
        sDiagonalLidType = 2;
        MapRenderer::draw_lid();
        sDiagonalLidType = -1;
    }
}

MATCH_FUNC(0x4f02d0)
void MapRenderer::DrawDiagonalWall()
{
    u8 mSlopeType = spBlock->mSlopeType;
    gCurrentSlope.mGradientDirection = 0;
    switch (GET_SLOPE_BITS(mSlopeType))
    {
        case DIAGONAL_WALL_UP_LEFT:
            MapRenderer::DrawDiagonalWallUpLeft();
            break;
        case DIAGONAL_WALL_UP_RIGHT:
            MapRenderer::DrawDiagonalWallUpRight();
            break;
        case DIAGONAL_WALL_DOWN_LEFT:
            MapRenderer::DrawDiagonalWallDownLeft();
            break;
        case DIAGONAL_WALL_DOWN_RIGHT:
            MapRenderer::DrawDiagonalWallDownRight();
            break;
        default:
            return;
    }
}

MATCH_FUNC(0x4f0340)
void MapRenderer::DrawTriangularDiagonal()
{
    u8 mSlopeType = spBlock->mSlopeType;
    gGradientSize = 1;
    s32 slope_type = GET_SLOPE_BITS(mSlopeType);
    gGradientLevel = 0;
    if (sLidType == 1023)
    {
        switch (slope_type)
        {
            case TRIANGULAR_SIDES_DIAGONAL_UP_LEFT:
                MapRenderer::Draw3SidedDiagonalUpLeft();
                break;
            case TRIANGULAR_SIDES_DIAGONAL_UP_RIGHT:
                MapRenderer::Draw3SidedDiagonalUpRight();
                break;
            case TRIANGULAR_SIDES_DIAGONAL_DOWN_LEFT:
                MapRenderer::Draw3SidedDiagonalDownLeft();
                break;
            case TRIANGULAR_SIDES_DIAGONAL_DOWN_RIGHT:
                MapRenderer::Draw3SidedDiagonalDownRight();
                break;
            default:
                return;
        }
    }
    else
    {
        gCurrentSlope.mGradientDirection = 0;
        switch (slope_type)
        {
            case TRIANGULAR_SIDES_DIAGONAL_UP_LEFT:
                MapRenderer::Draw4SidedDiagonalUpLeft();
                break;
            case TRIANGULAR_SIDES_DIAGONAL_UP_RIGHT:
                MapRenderer::Draw4SidedDiagonalUpRight();
                break;
            case TRIANGULAR_SIDES_DIAGONAL_DOWN_LEFT:
                MapRenderer::Draw4SidedDiagonalDownLeft();
                break;
            case TRIANGULAR_SIDES_DIAGONAL_DOWN_RIGHT:
                MapRenderer::Draw4SidedDiagonalDownRight();
                break;
            default:
                return;
        }
    }
}

// https://decomp.me/scratch/MyE2X
// 9.6f 0x46EF10
MATCH_FUNC(0x4f0420)
void MapRenderer::DrawGradientSlopeNorthwards()
{
    if (gBlockLeft)
    {
        if (gBlockRight)
        {
            u16 side_word;
            if ((gBlockRight & block_face::flat) != 0)
            {
                gXCoord = gXCoord + kZeroOnePoint.y;
                side_word = gBlockLeft | 0x1000;
                MapRenderer::DrawLeftSide(side_word);
                gXCoord = gXCoord - kZeroOnePoint.y;
            }
            
            if ((gBlockLeft & block_face::flat) != 0)
            {
                gXCoord = gXCoord - kZeroOnePoint.y;
                side_word = gBlockRight | 0x1000;
                MapRenderer::DrawRightSide(side_word);
                gXCoord = gXCoord + kZeroOnePoint.y;
            }
            
        }
    }
    
    if (sBlockTop)
    {
        if (sBlockBottom)
        {
            if ((sBlockTop & block_face::flat) != 0)
            {
                u16 side_word;
                gYCoord = gYCoord - kZeroOnePoint.y;
                side_word = sBlockBottom | 0x1000;
                MapRenderer::draw_bottom(side_word);
                gYCoord = gYCoord + kZeroOnePoint.y;
            }
        }
    }

    if (gBlockLeft)
    {
        if (!gBlockRight || (gBlockRight & block_face::flat) == 0 || (gBlockLeft & block_face::flat) != 0)
        {
            MapRenderer::DrawLeftSide(gBlockLeft);
        }
    }
    
    if (gBlockRight)
    {
        if (!gBlockLeft || (gBlockLeft & block_face::flat) == 0 || (gBlockRight & block_face::flat) != 0)
        {
            MapRenderer::DrawRightSide(gBlockRight);
        }
    }
    
    if (sBlockTop)
    {
        if (!sBlockBottom || (sBlockBottom & block_face::flat) == 0 || (sBlockTop & block_face::flat) != 0)
        {
            MapRenderer::DrawTopSide(sBlockTop);
        }
    }

    if (sLidType && !bSkip_lid)
    {
        if (gGradientLevel == gGradientSize - 1)
        {
            gVertProjector2.ProjectVertBottom(gXCoord + kZeroOnePoint.y, gYCoord + kZeroOnePoint.y, &gTileVerts_6F65A8[2]);
            gVertProjector2.ProjectVertBottom(gXCoord, gYCoord + kZeroOnePoint.y, &gTileVerts_6F65A8[3]);
        }
        else
        {
            gVertProjector2.ProjectVert(gXCoord + kZeroOnePoint.y,
                       gYCoord + kZeroOnePoint.y,
                       sZCoordFp + (Fix16(gGradientSize - gGradientLevel - 1) / gGradientSize),
                       &gTileVerts_6F65A8[2]);
            gVertProjector2.ProjectVert(gXCoord,
                       gYCoord + kZeroOnePoint.y,
                       sZCoordFp + (Fix16(gGradientSize - gGradientLevel - 1) / gGradientSize),
                       &gTileVerts_6F65A8[3]);
        }

        if (!gGradientLevel)
        {
            gVertProjector2.ProjectVertTop(gXCoord, gYCoord, &gTileVerts_6F65A8[0]);
            gVertProjector2.ProjectVertTop(gXCoord + kZeroOnePoint.y, gYCoord, &gTileVerts_6F65A8[1]);
        }
        else
        {
            gVertProjector2.ProjectVert(gXCoord,
                       gYCoord,
                       sZCoordFp + (Fix16(gGradientSize - gGradientLevel) / gGradientSize),
                       &gTileVerts_6F65A8[0]);
            ProjectVert(gXCoord + kZeroOnePoint.y,
                       gYCoord,
                       sZCoordFp + (Fix16(gGradientSize - gGradientLevel) / gGradientSize),
                       &gTileVerts_6F65A8[1]);
        }

        // now draw

        sTileDrawFlags = kSlopeLidFlags[sLidType >> 13] | 1;
        if ((sLidType & block_face::flat) != 0)
        {
            sTileDrawFlags = sTileDrawFlags | 0x80;
        }
        u16 texture_idx = gGtx_0x106C->GetTile(sLidType & block_face::tile_idx_mask);
        u8 colour;
        u16 colour_sel = (sLidType >> 10) & 3;
        if (colour_sel == 0)
        {
            colour = mDcolour;
        }
        else
        {
            colour = MapRenderer::GetColour_4F0BD0(colour_sel);
        }
        pgbh_DrawTile(sTileDrawFlags | gLightingDrawFlag,
                      gTextureCache_15D8->GetTexture(texture_idx),
                      gTileVerts_6F65A8,
                      colour);
        ++mDrawnTileCount;
    }
}

MATCH_FUNC(0x4f0bd0)
char_type MapRenderer::GetColour_4F0BD0(s32 lid_type)
{
    switch (lid_type)
    {
        case 0:
            return -1;
        case 1:
            return mColourT1;
        case 2:
            return mColourT2;
        case 3:
            return mColourT3;
        default:
            return 0;
    }
}

// https://decomp.me/scratch/HfCiT
MATCH_FUNC(0x4f1660)
void MapRenderer::DrawGradientSlopeSouthwards()
{
    
    if (gBlockLeft)
    {
        if (gBlockRight)
        {
            u16 side_word;
            if ((gBlockRight & block_face::flat) != 0)
            {
                gXCoord = gXCoord + kZeroOnePoint.y;
                side_word = gBlockLeft | 0x1000;
                MapRenderer::DrawLeftSide(side_word);
                gXCoord = gXCoord - kZeroOnePoint.y;
            }
            
            if ((gBlockLeft & block_face::flat) != 0)
            {
                gXCoord = gXCoord - kZeroOnePoint.y;
                side_word = gBlockRight | 0x1000;
                MapRenderer::DrawRightSide(side_word);
                gXCoord = gXCoord + kZeroOnePoint.y;
            }
            
        }
    }
    
    if (sBlockTop)
    {
        if (sBlockBottom)
        {
            u16 side_word;
            if ((sBlockBottom & block_face::flat) != 0)
            {
                gYCoord = gYCoord + kZeroOnePoint.y;
                side_word = sBlockTop | 0x1000;
                MapRenderer::DrawTopSide(side_word);
                gYCoord = gYCoord - kZeroOnePoint.y;
            }
        }
    }
    
    if (gBlockLeft)
    {
        if (!gBlockRight || (gBlockRight & block_face::flat) == 0 || (gBlockLeft & block_face::flat) != 0)
        {
            MapRenderer::DrawLeftSide(gBlockLeft);
        }
    }
    
    if (gBlockRight)
    {
        if (!gBlockLeft || (gBlockLeft & block_face::flat) == 0 || (gBlockRight & block_face::flat) != 0)
        {
            MapRenderer::DrawRightSide(gBlockRight);
        }
    }
    
    if (sBlockBottom)
    {
        if (!sBlockTop || (sBlockTop & block_face::flat) == 0 || (sBlockBottom & block_face::flat) != 0)
        {
            MapRenderer::draw_bottom(sBlockBottom);
        }
    }

    if (sLidType && !bSkip_lid)
    {
        if (!gGradientLevel)
        {
            gVertProjector2.ProjectVertTop(gXCoord + kZeroOnePoint.y, gYCoord + kZeroOnePoint.y, &gTileVerts_6F65A8[2]);
            gVertProjector2.ProjectVertTop(gXCoord, gYCoord + kZeroOnePoint.y, &gTileVerts_6F65A8[3]);
        }
        else
        {
            gVertProjector3.ProjectVert(gXCoord + kZeroOnePoint.y,
                       gYCoord + kZeroOnePoint.y,
                       sZCoordFp + (Fix16(gGradientSize - gGradientLevel) / gGradientSize),
                       &gTileVerts_6F65A8[2]);
            gVertProjector3.ProjectVert(gXCoord,
                       gYCoord + kZeroOnePoint.y,
                       sZCoordFp + (Fix16(gGradientSize - gGradientLevel) / gGradientSize),
                       &gTileVerts_6F65A8[3]);
        }

        if (gGradientLevel == gGradientSize - 1)
        {
            gVertProjector2.ProjectVertBottom(gXCoord, gYCoord, &gTileVerts_6F65A8[0]);
            gVertProjector2.ProjectVertBottom(gXCoord + kZeroOnePoint.y, gYCoord, &gTileVerts_6F65A8[1]);
        }
        else
        {
            gVertProjector4.ProjectVert(gXCoord,
                       gYCoord,
                       sZCoordFp + (Fix16(gGradientSize - gGradientLevel - 1) / gGradientSize),
                       &gTileVerts_6F65A8[0]);
            ProjectVert(gXCoord + kZeroOnePoint.y,
                       gYCoord,
                       sZCoordFp + (Fix16(gGradientSize - gGradientLevel - 1) / gGradientSize),
                       &gTileVerts_6F65A8[1]);
        }

        sTileDrawFlags = kSlopeLidFlags[sLidType >> 13] | 2;
        if ((sLidType & block_face::flat) != 0)
        {
            sTileDrawFlags = sTileDrawFlags | 0x80;
        }
        u16 texture_idx = gGtx_0x106C->GetTile(sLidType & block_face::tile_idx_mask);
        u8 colour;
        u16 colour_sel = (sLidType >> 10) & 3;
        if (colour_sel == 0)
        {
            colour = mSlopeSouthColour;
        }
        else
        {
            colour = MapRenderer::GetColour_4F0BD0(colour_sel);
        }
        pgbh_DrawTile(sTileDrawFlags | gLightingDrawFlag,
                      gTextureCache_15D8->GetTexture(texture_idx),
                      gTileVerts_6F65A8,
                      colour);
        ++mDrawnTileCount;
    }
}

// https://decomp.me/scratch/6Zrn3
MATCH_FUNC(0x4f22f0)
void MapRenderer::DrawGradientSlopeWestwards()
{
    if (gBlockLeft)
    {
        if (gBlockRight)
        {
            u16 side_word;
            if ((gBlockLeft & block_face::flat) != 0)
            {
                gXCoord = gXCoord - kZeroOnePoint.y;
                side_word = gBlockRight | 0x1000;
                MapRenderer::DrawRightSide(side_word);
                gXCoord = gXCoord + kZeroOnePoint.y;
            }
        }
    }

    if (sBlockTop)
    {
        if (sBlockBottom)
        {
            u16 side_word;
            if ((sBlockBottom & block_face::flat) != 0)
            {
                gYCoord = gYCoord + kZeroOnePoint.y;
                side_word = sBlockTop | 0x1000;
                MapRenderer::DrawTopSide(side_word);
                gYCoord = gYCoord - kZeroOnePoint.y;
            }
            
            if ((sBlockTop & block_face::flat) != 0)
            {
                gYCoord = gYCoord - kZeroOnePoint.y;
                side_word = sBlockBottom | 0x1000;
                MapRenderer::draw_bottom(side_word);
                gYCoord = gYCoord + kZeroOnePoint.y;
            }
        }
    }
    
    if (gBlockLeft)
    {
        if (!gBlockRight || (gBlockRight & block_face::flat) == 0 || (gBlockLeft & block_face::flat) != 0)
        {
            MapRenderer::DrawLeftSide(gBlockLeft);
        }
    }
    
    if (sBlockTop)
    {
        if (!sBlockBottom || (sBlockBottom & block_face::flat) == 0 || (sBlockTop & block_face::flat) != 0)
        {
            MapRenderer::DrawTopSide(sBlockTop);
        }
    }
    
    if (sBlockBottom)
    {
        if (!sBlockTop || (sBlockTop & block_face::flat) == 0 || (sBlockBottom & block_face::flat) != 0)
        {
            MapRenderer::draw_bottom(sBlockBottom);
        }
    }

    if (sLidType && !bSkip_lid)
    {
        if (!gGradientLevel)
        {
            gVertProjector2.ProjectVertTop(gXCoord, gYCoord, &gTileVerts_6F65A8[0]);
            gVertProjector2.ProjectVertTop(gXCoord, gYCoord + kZeroOnePoint.y, &gTileVerts_6F65A8[3]);
        }
        else
        {
            gVertProjector2.ProjectVert(gXCoord,
                       gYCoord,
                       sZCoordFp + (Fix16(gGradientSize - gGradientLevel) / gGradientSize),
                       &gTileVerts_6F65A8[0]);
            gVertProjector2.ProjectVert(gXCoord,
                       gYCoord + kZeroOnePoint.y,
                       sZCoordFp + (Fix16(gGradientSize - gGradientLevel) / gGradientSize),
                       &gTileVerts_6F65A8[3]);
        }

        if (gGradientLevel == gGradientSize - 1)
        {
            gVertProjector2.ProjectVertBottom(gXCoord + kZeroOnePoint.y, gYCoord, &gTileVerts_6F65A8[1]);
            gVertProjector2.ProjectVertBottom(gXCoord + kZeroOnePoint.y, gYCoord + kZeroOnePoint.y, &gTileVerts_6F65A8[2]);
        }
        else
        {
            gVertProjector2.ProjectVert(gXCoord + kZeroOnePoint.y,
                       gYCoord,
                       sZCoordFp + (Fix16(gGradientSize - gGradientLevel - 1) / gGradientSize),
                       &gTileVerts_6F65A8[1]);
            ProjectVert(gXCoord + kZeroOnePoint.y,
                       gYCoord + kZeroOnePoint.y,
                       sZCoordFp + (Fix16(gGradientSize - gGradientLevel - 1) / gGradientSize),
                       &gTileVerts_6F65A8[2]);
        }

        sTileDrawFlags = kSlopeLidFlags[sLidType >> 13] | 3;
        if ((sLidType & block_face::flat) != 0)
        {
            sTileDrawFlags = sTileDrawFlags | 0x80;
        }
        u16 texture_idx = gGtx_0x106C->GetTile(sLidType & block_face::tile_idx_mask);
        u8 colour;
        u16 colour_sel = (sLidType >> 10) & 3;
        if (colour_sel == 0)
        {
            colour = mSlopeWestColour;
        }
        else
        {
            colour = MapRenderer::GetColour_4F0BD0(colour_sel);
        }
        pgbh_DrawTile(sTileDrawFlags | gLightingDrawFlag,
                      gTextureCache_15D8->GetTexture(texture_idx),
                      gTileVerts_6F65A8,
                      colour);
        ++mDrawnTileCount;
    }
}

// https://decomp.me/scratch/cvW2D
MATCH_FUNC(0x4f33b0)
void MapRenderer::DrawGradientSlopeEastwards()
{
    
    if (gBlockLeft)
    {
        if (gBlockRight)
        {
            u16 side_word;
            if ((gBlockRight & block_face::flat) != 0)
            {
                gXCoord = gXCoord + kZeroOnePoint.y;
                side_word = gBlockLeft | 0x1000;
                MapRenderer::DrawLeftSide(side_word);
                gXCoord = gXCoord - kZeroOnePoint.y;
            }
        }
    }
    
    if (sBlockTop)
    {
        if (sBlockBottom)
        {
            u16 side_word;
            if ((sBlockBottom & block_face::flat) != 0)
            {
                gYCoord = gYCoord + kZeroOnePoint.y;
                side_word = sBlockTop | 0x1000;
                MapRenderer::DrawTopSide(side_word);
                gYCoord = gYCoord - kZeroOnePoint.y;
            }
            
            if ((sBlockTop & block_face::flat) != 0)
            {
                gYCoord = gYCoord - kZeroOnePoint.y;
                side_word = sBlockBottom | 0x1000;
                MapRenderer::draw_bottom(side_word);
                gYCoord = gYCoord + kZeroOnePoint.y;
            }
        }
    }
    
    if (gBlockRight)
    {
        if (!gBlockLeft || (gBlockLeft & block_face::flat) == 0 || (gBlockRight & block_face::flat) != 0)
        {
            MapRenderer::DrawRightSide(gBlockRight);
        }
    }
    
    if (sBlockTop)
    {
        if (!sBlockBottom || (sBlockBottom & block_face::flat) == 0 || (sBlockTop & block_face::flat) != 0)
        {
            MapRenderer::DrawTopSide(sBlockTop);
        }
    }
    
    if (sBlockBottom)
    {
        if (!sBlockTop || (sBlockTop & block_face::flat) == 0 || (sBlockBottom & block_face::flat) != 0)
        {
            MapRenderer::draw_bottom(sBlockBottom);
        }
    }

    if (sLidType && !bSkip_lid)
    {
        if (gGradientLevel == gGradientSize - 1)
        {
            gVertProjector2.ProjectVertBottom(gXCoord, gYCoord, &gTileVerts_6F65A8[0]);
            gVertProjector2.ProjectVertBottom(gXCoord, gYCoord + kZeroOnePoint.y, &gTileVerts_6F65A8[3]);
        }
        else
        {
            gVertProjector4.ProjectVert(gXCoord,
                       gYCoord,
                       sZCoordFp + (Fix16(gGradientSize - gGradientLevel - 1) / gGradientSize),
                       &gTileVerts_6F65A8[0]);
            gVertProjector3.ProjectVert(gXCoord,
                       gYCoord + kZeroOnePoint.y,
                       sZCoordFp + (Fix16(gGradientSize - gGradientLevel - 1) / gGradientSize),
                       &gTileVerts_6F65A8[3]);
        }

        if (!gGradientLevel)
        {
            gVertProjector2.ProjectVertTop(gXCoord + kZeroOnePoint.y, gYCoord, &gTileVerts_6F65A8[1]);
            gVertProjector2.ProjectVertTop(gXCoord + kZeroOnePoint.y, gYCoord + kZeroOnePoint.y, &gTileVerts_6F65A8[2]);
        }
        else
        {
            gVertProjector4.ProjectVert(gXCoord + kZeroOnePoint.y,
                       gYCoord,
                       sZCoordFp + (Fix16(gGradientSize - gGradientLevel) / gGradientSize),
                       &gTileVerts_6F65A8[1]);
            ProjectVert(gXCoord + kZeroOnePoint.y,
                       gYCoord + kZeroOnePoint.y,
                       sZCoordFp + (Fix16(gGradientSize - gGradientLevel) / gGradientSize),
                       &gTileVerts_6F65A8[2]);
        }

        sTileDrawFlags = kSlopeLidFlags[sLidType >> 13] | 4;
        if ((sLidType & block_face::flat) != 0)
        {
            sTileDrawFlags = sTileDrawFlags | 0x80;
        }
        u16 texture_idx = gGtx_0x106C->GetTile(sLidType & block_face::tile_idx_mask);
        u8 colour;
        u16 colour_sel = (sLidType >> 10) & 3;
        if (colour_sel == 0)
        {
            colour = mSlopeEastColour;
        }
        else
        {
            colour = MapRenderer::GetColour_4F0BD0(colour_sel);
        }
        pgbh_DrawTile(sTileDrawFlags | gLightingDrawFlag,
                      gTextureCache_15D8->GetTexture(texture_idx),
                      gTileVerts_6F65A8,
                      colour);
        ++mDrawnTileCount;
    }
}

// https://decomp.me/scratch/zty5O
MATCH_FUNC(0x4f3c00)
void MapRenderer::draw_left(u16& side_word, Fix16& x_off, Fix16& y_off_a, Fix16& y_off_b)
{
    if (!bSkip_left)
    {
        gVertProjector2.ProjectVertBottom(gXCoord + x_off, gYCoord + y_off_a, &gTileVerts_6F65A8[0]);
        gVertProjector2.ProjectVertTop(gXCoord + x_off, gYCoord + y_off_a, &gTileVerts_6F65A8[1]);
        if (gTileVerts_6F65A8[0].x > gTileVerts_6F65A8[1].x)
        {
            return;
        }
        gVertProjector2.ProjectVertBottom(gXCoord + x_off, gYCoord + y_off_b, &gTileVerts_6F65A8[3]);
        gVertProjector2.ProjectVertTop(gXCoord + x_off, gYCoord + y_off_b, &gTileVerts_6F65A8[2]);

        SetUVCorners(kLeftSideFlags[side_word >> 13]);

        Set_UV(kZeroOnePoint.x, y_off_a, 0);
        Set_UV(kZeroOnePoint.y, y_off_a, 1);
        Set_UV(kZeroOnePoint.y, y_off_b, 2);
        Set_UV(kZeroOnePoint.x, y_off_b, 3);

        sTileDrawFlags = 16389;
        u16 texture_idx = gGtx_0x106C->GetTile(side_word & block_face::tile_idx_mask);
        if (texture_idx)
        {
            if ((*(reinterpret_cast<u8*>(&side_word) + 1) & 0x10) != 0)
            {
                sTileDrawFlags = sTileDrawFlags | 0x80;
            }
            pgbh_DrawTile(sTileDrawFlags | gLightingDrawFlag,
                          gTextureCache_15D8->GetTexture(texture_idx),
                          gTileVerts_6F65A8,
                          mColourT1);
            ++mDrawnTileCount;
        }
    }
}

MATCH_FUNC(0x4F3FB0)
void __stdcall SetUVCorners(s32 uv_flags)
{
    Fix16_Point tmp;
    switch (uv_flags & 0x60)
    {
        case 32: // rotation code 0
            sUVCorner0.SetXY(kZeroOnePoint.x, kTileTexMax);
            sUVCorner1.SetXY(kZeroOnePoint.x, kZeroOnePoint.x);
            sUVCorner2.SetXY(kTileTexMax, kZeroOnePoint.x);
            sUVCorner3.SetXY(kTileTexMax, kTileTexMax);
            break;
        case 96: // rotation code 2
            sUVCorner0.SetXY(kTileTexMax, kZeroOnePoint.x);
            sUVCorner1.SetXY(kTileTexMax, kTileTexMax);
            sUVCorner2.SetXY(kZeroOnePoint.x, kTileTexMax);
            sUVCorner3.SetXY(kZeroOnePoint.x, kZeroOnePoint.x);
            break;
        case 64: // rotation code 1
            uv_flags ^= 0x18;
        default: // rotation code 3
            sUVCorner0.SetXY(kZeroOnePoint.x, kZeroOnePoint.x);
            sUVCorner1.SetXY(kTileTexMax, kZeroOnePoint.x);
            sUVCorner2.SetXY(kTileTexMax, kTileTexMax);
            sUVCorner3.SetXY(kZeroOnePoint.x, kTileTexMax);
            break;
    }

    if ((uv_flags & 0x8) != 0) // maybe rotation code 1
    {
        tmp = sUVCorner0;
        sUVCorner0 = sUVCorner1;
        sUVCorner1 = tmp;

        tmp = sUVCorner3;
        sUVCorner3 = sUVCorner2;
        sUVCorner2 = tmp;
    }
    if ((uv_flags & 0x10) != 0) // flip
    {
        tmp = sUVCorner0;
        sUVCorner0 = sUVCorner3;
        sUVCorner3 = tmp;

        tmp = sUVCorner1;
        sUVCorner1 = sUVCorner2;
        sUVCorner2 = tmp;
    }

    sUVEdge01 = sUVCorner1 - sUVCorner0;
    sUVEdge03 = sUVCorner3 - sUVCorner0;
}

// https://decomp.me/scratch/NaMFS or https://decomp.me/scratch/hij63
MATCH_FUNC(0x4f4190)
void __stdcall Set_UV(Fix16& edge01_frac, Fix16& edge03_frac, const u32& vert_idx)
{
    Fix16_Point uv_coords = ((sUVEdge01 * edge01_frac) + (sUVEdge03 * edge03_frac)) + sUVCorner0;
    // Converting through f32 locals (rather than ToFloat()) adds a narrowing step that makes
    // VC6 schedule the index load before the fmuls, like the original.
    f32 u = static_cast<f32>(uv_coords.x.mValue);
    gTileVerts_6F65A8[vert_idx].u = u / 16384.0f;
    f32 v = static_cast<f32>(uv_coords.y.mValue);
    gTileVerts_6F65A8[vert_idx].v = v / 16384.0f;
}

// https://decomp.me/scratch/dGqKl
MATCH_FUNC(0x4f4250)
void MapRenderer::draw_right(u16& side_word, Fix16& x_off, Fix16& y_off_a, Fix16& y_off_b)
{
    if (!bSkip_right)
    {
        gVertProjector2.ProjectVertBottom(gXCoord + x_off, gYCoord + y_off_a, &gTileVerts_6F65A8[1]);
        gVertProjector2.ProjectVertTop(gXCoord + x_off, gYCoord + y_off_a, &gTileVerts_6F65A8[0]);
        if (gTileVerts_6F65A8[0].x > gTileVerts_6F65A8[1].x)
        {
            return;
        }
        gVertProjector2.ProjectVertBottom(gXCoord + x_off, gYCoord + y_off_b, &gTileVerts_6F65A8[2]);
        gVertProjector2.ProjectVertTop(gXCoord + x_off, gYCoord + y_off_b, &gTileVerts_6F65A8[3]);

        SetUVCorners(kRightSideFlags[side_word >> 13]);

        Set_UV(kZeroOnePoint.x, y_off_a, 0);
        Set_UV(kZeroOnePoint.y, y_off_a, 1);
        Set_UV(kZeroOnePoint.y, y_off_b, 2);
        Set_UV(kZeroOnePoint.x, y_off_b, 3);

        sTileDrawFlags = 16389;
        u16 texture_idx = gGtx_0x106C->GetTile(side_word & block_face::tile_idx_mask);
        if (texture_idx)
        {
            if ((*(reinterpret_cast<u8*>(&side_word) + 1) & 0x10) != 0)
            {
                sTileDrawFlags = sTileDrawFlags | 0x80;
            }
            pgbh_DrawTile(sTileDrawFlags | gLightingDrawFlag,
                          gTextureCache_15D8->GetTexture(texture_idx),
                          gTileVerts_6F65A8,
                          mRightColour);
            ++mDrawnTileCount;
        }
    }
}

// https://decomp.me/scratch/7xF8P
MATCH_FUNC(0x4f4600)
void MapRenderer::draw_top(u16& side_word, Fix16& x_off_a, Fix16& x_off_b, Fix16& y_off)
{
    if (!bSkip_top)
    {
        gVertProjector2.ProjectVertBottom(gXCoord + x_off_a, gYCoord + y_off, &gTileVerts_6F65A8[0]);
        gVertProjector2.ProjectVertTop(gXCoord + x_off_a, gYCoord + y_off, &gTileVerts_6F65A8[3]);
        if (gTileVerts_6F65A8[0].y > gTileVerts_6F65A8[3].y)
        {
            return;
        }
        gVertProjector2.ProjectVertBottom(gXCoord + x_off_b, gYCoord + y_off, &gTileVerts_6F65A8[1]);
        gVertProjector2.ProjectVertTop(gXCoord + x_off_b, gYCoord + y_off, &gTileVerts_6F65A8[2]);

        SetUVCorners(kTopSideFlags[side_word >> 13]);

        Set_UV(x_off_a, kZeroOnePoint.x, 0);
        Set_UV(x_off_b, kZeroOnePoint.x, 1);
        Set_UV(x_off_b, kZeroOnePoint.y, 2);
        Set_UV(x_off_a, kZeroOnePoint.y, 3);

        sTileDrawFlags = 16389;
        u16 texture_idx = gGtx_0x106C->GetTile(side_word & block_face::tile_idx_mask);
        if (texture_idx)
        {
            if ((*(reinterpret_cast<u8*>(&side_word) + 1) & 0x10) != 0)
            {
                sTileDrawFlags = sTileDrawFlags | 0x80;
            }
            pgbh_DrawTile(sTileDrawFlags | gLightingDrawFlag,
                          gTextureCache_15D8->GetTexture(texture_idx),
                          gTileVerts_6F65A8,
                          mColourT3);
            ++mDrawnTileCount;
        }
    }
}

// https://decomp.me/scratch/QiYzx
MATCH_FUNC(0x4f49b0)
void MapRenderer::draw_partial_bottom(u16& side_word, Fix16& x_off_a, Fix16& x_off_b, Fix16& y_off)
{
    if (!bSkip_bottom)
    {
        gVertProjector2.ProjectVertBottom(gXCoord + x_off_a, gYCoord + y_off, &gTileVerts_6F65A8[3]);
        gVertProjector2.ProjectVertTop(gXCoord + x_off_a, gYCoord + y_off, &gTileVerts_6F65A8[0]);
        if (gTileVerts_6F65A8[0].y > gTileVerts_6F65A8[3].y)
        {
            return;
        }
        gVertProjector2.ProjectVertBottom(gXCoord + x_off_b, gYCoord + y_off, &gTileVerts_6F65A8[2]);
        gVertProjector2.ProjectVertTop(gXCoord + x_off_b, gYCoord + y_off, &gTileVerts_6F65A8[1]);

        SetUVCorners(kBottomSideFlags[side_word >> 13]);

        Set_UV(x_off_a, kZeroOnePoint.x, 0);
        Set_UV(x_off_b, kZeroOnePoint.x, 1);
        Set_UV(x_off_b, kZeroOnePoint.y, 2);
        Set_UV(x_off_a, kZeroOnePoint.y, 3);

        sTileDrawFlags = 16389;
        u16 texture_idx = gGtx_0x106C->GetTile(side_word & block_face::tile_idx_mask);
        if (texture_idx)
        {
            if ((*(reinterpret_cast<u8*>(&side_word) + 1) & 0x10) != 0)
            {
                sTileDrawFlags = sTileDrawFlags | 0x80;
            }
            pgbh_DrawTile(sTileDrawFlags | gLightingDrawFlag,
                          gTextureCache_15D8->GetTexture(texture_idx),
                          gTileVerts_6F65A8,
                          mColourT2);
            ++mDrawnTileCount;
        }
    }
}

// https://decomp.me/scratch/sV2SV
MATCH_FUNC(0x4f4d60)
void MapRenderer::draw_partial_lid(Fix16& x_min, Fix16& x_max, Fix16& y_min, Fix16& y_max)
{
    if (!bSkip_lid)
    {
        gVertProjector2.ProjectVertTop(gXCoord + x_min, gYCoord + y_min, &gTileVerts_6F65A8[0]);
        gVertProjector2.ProjectVertTop(gXCoord + x_max, gYCoord + y_min, &gTileVerts_6F65A8[1]);
        gVertProjector2.ProjectVertTop(gXCoord + x_max, gYCoord + y_max, &gTileVerts_6F65A8[2]);
        gVertProjector2.ProjectVertTop(gXCoord + x_min, gYCoord + y_max, &gTileVerts_6F65A8[3]);

        SetUVCorners(kLidFlags[static_cast<u16>(sLidType) >> 13]);

        Set_UV(x_min, y_min, 0);
        Set_UV(x_max, y_min, 1);
        Set_UV(x_max, y_max, 2);
        Set_UV(x_min, y_max, 3);

        u16 texture_idx = gGtx_0x106C->GetTile(sLidType & block_face::tile_idx_mask);
        if (texture_idx)
        {
            sTileDrawFlags = 16389;
            if ((sLidType & block_face::flat) != 0)
            {
                sTileDrawFlags = 16517;
            }

            // 9.6f: GetColour (inlined, using it changes the code: 43 -> 105)
            u8 diffuse_color;

            switch ((static_cast<u16>(sLidType) >> 10) & 3)
            {
                case 0:
                    diffuse_color = -1;
                    break;
                case 1:
                    diffuse_color = mColourT1;
                    break;
                case 2:
                    diffuse_color = mColourT2;
                    break;
                case 3:
                    diffuse_color = mColourT3;
                    break;
                default:
                    diffuse_color = 0;
                    break;
            }
            pgbh_DrawTile(sTileDrawFlags | gLightingDrawFlag,
                          gTextureCache_15D8->GetTexture(texture_idx),
                          gTileVerts_6F65A8,
                          diffuse_color);
            ++mDrawnTileCount;
        }
    }
}

// DrawPartialBlocks tail-jumps (jmp) to the nine functions below; IDA treats them as chunks
// of it, so they are not in its function list.
MATCH_FUNC(0x4F5160)
void MapRenderer::DrawPartialBlockLeft()
{
    u16 side_word;
    if (gBlockLeft)
    {
        if (gBlockRight)
        {
            if ((gBlockRight & block_face::flat) != 0)
            {
                side_word = gBlockLeft | 0x1000;
                MapRenderer::draw_left(side_word, kFpThreeEighths, kZeroOnePoint.x, kZeroOnePoint.y);
            }
            if ((gBlockLeft & block_face::flat) != 0)
            {
                side_word = gBlockRight | 0x1000;
                MapRenderer::draw_right(side_word, kZeroOnePoint.x, kZeroOnePoint.x, kZeroOnePoint.y);
            }
        }
    }
    if (sBlockTop)
    {
        if (sBlockBottom)
        {
            if ((sBlockBottom & block_face::flat) != 0)
            {
                side_word = sBlockTop | 0x1000;
                MapRenderer::draw_top(side_word, kZeroOnePoint.x, kFpThreeEighths, kZeroOnePoint.y);
            }
            if ((sBlockTop & block_face::flat) != 0)
            {
                side_word = sBlockBottom | 0x1000;
                MapRenderer::draw_partial_bottom(side_word, kZeroOnePoint.x, kFpThreeEighths, kZeroOnePoint.x);
            }
        }
    }
    if (gBlockLeft)
    {
        if (!gBlockRight || (gBlockRight & block_face::flat) == 0 || (gBlockLeft & block_face::flat) != 0)
        {
            MapRenderer::DrawLeftSide(gBlockLeft);
        }
    }
    if (gBlockRight)
    {
        if (!gBlockLeft || (gBlockLeft & block_face::flat) == 0 || (gBlockRight & block_face::flat) != 0)
        {
            MapRenderer::draw_right(gBlockRight, kFpThreeEighths, kZeroOnePoint.x, kZeroOnePoint.y);
        }
    }
    if (sBlockTop)
    {
        if (!sBlockBottom || (sBlockBottom & block_face::flat) == 0 || (sBlockTop & block_face::flat) != 0)
        {
            MapRenderer::draw_top(sBlockTop, kZeroOnePoint.x, kFpThreeEighths, kZeroOnePoint.x);
        }
    }
    if (sBlockBottom)
    {
        if (!sBlockTop || (sBlockTop & block_face::flat) == 0 || (sBlockBottom & block_face::flat) != 0)
        {
            MapRenderer::draw_partial_bottom(sBlockBottom, kZeroOnePoint.x, kFpThreeEighths, kZeroOnePoint.y);
        }
    }
    if (sLidType)
    {
        MapRenderer::draw_partial_lid(kZeroOnePoint.x, kFpThreeEighths, kZeroOnePoint.x, kZeroOnePoint.y);
    }
}

MATCH_FUNC(0x4F5360)
void MapRenderer::DrawPartialBlockRight()
{
    u16 side_word;
    if (gBlockLeft)
    {
        if (gBlockRight)
        {
            if ((gBlockRight & block_face::flat) != 0)
            {
                side_word = gBlockLeft | 0x1000;
                MapRenderer::draw_left(side_word, kZeroOnePoint.y, kZeroOnePoint.x, kZeroOnePoint.y);
            }
            if ((gBlockLeft & block_face::flat) != 0)
            {
                side_word = gBlockRight | 0x1000;
                MapRenderer::draw_right(side_word, kFpFiveEighths, kZeroOnePoint.x, kZeroOnePoint.y);
            }
        }
    }
    if (sBlockTop)
    {
        if (sBlockBottom)
        {
            if ((sBlockBottom & block_face::flat) != 0)
            {
                side_word = sBlockTop | 0x1000;
                MapRenderer::draw_top(side_word, kFpFiveEighths, kZeroOnePoint.y, kZeroOnePoint.y);
            }
            if ((sBlockTop & block_face::flat) != 0)
            {
                side_word = sBlockBottom | 0x1000;
                MapRenderer::draw_partial_bottom(side_word, kFpFiveEighths, kZeroOnePoint.y, kZeroOnePoint.x);
            }
        }
    }
    if (gBlockLeft)
    {
        if (!gBlockRight || (gBlockRight & block_face::flat) == 0 || (gBlockLeft & block_face::flat) != 0)
        {
            MapRenderer::draw_left(gBlockLeft, kFpFiveEighths, kZeroOnePoint.x, kZeroOnePoint.y);
        }
    }
    if (gBlockRight)
    {
        if (!gBlockLeft || (gBlockLeft & block_face::flat) == 0 || (gBlockRight & block_face::flat) != 0)
        {
            MapRenderer::DrawRightSide(gBlockRight);
        }
    }
    if (sBlockTop)
    {
        if (!sBlockBottom || (sBlockBottom & block_face::flat) == 0 || (sBlockTop & block_face::flat) != 0)
        {
            MapRenderer::draw_top(sBlockTop, kFpFiveEighths, kZeroOnePoint.y, kZeroOnePoint.x);
        }
    }
    if (sBlockBottom)
    {
        if (!sBlockTop || (sBlockTop & block_face::flat) == 0 || (sBlockBottom & block_face::flat) != 0)
        {
            MapRenderer::draw_partial_bottom(sBlockBottom, kFpFiveEighths, kZeroOnePoint.y, kZeroOnePoint.y);
        }
    }
    if (sLidType)
    {
        MapRenderer::draw_partial_lid(kFpFiveEighths, kZeroOnePoint.y, kZeroOnePoint.x, kZeroOnePoint.y);
    }
}

MATCH_FUNC(0x4F5560)
void MapRenderer::DrawPartialBlockTop()
{
    u16 side_word;
    if (gBlockLeft)
    {
        if (gBlockRight)
        {
            if ((gBlockRight & block_face::flat) != 0)
            {
                side_word = gBlockLeft | 0x1000;
                MapRenderer::draw_left(side_word, kZeroOnePoint.y, kZeroOnePoint.x, kFpThreeEighths);
            }
            if ((gBlockLeft & block_face::flat) != 0)
            {
                side_word = gBlockRight | 0x1000;
                MapRenderer::draw_right(side_word, kZeroOnePoint.x, kZeroOnePoint.x, kFpThreeEighths);
            }
        }
    }
    if (sBlockTop)
    {
        if (sBlockBottom)
        {
            if ((sBlockBottom & block_face::flat) != 0)
            {
                side_word = sBlockTop | 0x1000;
                MapRenderer::draw_top(side_word, kZeroOnePoint.x, kZeroOnePoint.y, kFpThreeEighths);
            }
            if ((sBlockTop & block_face::flat) != 0)
            {
                side_word = sBlockBottom | 0x1000;
                MapRenderer::draw_partial_bottom(side_word, kZeroOnePoint.x, kZeroOnePoint.y, kZeroOnePoint.x);
            }
        }
    }
    if (gBlockLeft)
    {
        if (!gBlockRight || (gBlockRight & block_face::flat) == 0 || (gBlockLeft & block_face::flat) != 0)
        {
            MapRenderer::draw_left(gBlockLeft, kZeroOnePoint.x, kZeroOnePoint.x, kFpThreeEighths);
        }
    }
    if (gBlockRight)
    {
        if (!gBlockLeft || (gBlockLeft & block_face::flat) == 0 || (gBlockRight & block_face::flat) != 0)
        {
            MapRenderer::draw_right(gBlockRight, kZeroOnePoint.y, kZeroOnePoint.x, kFpThreeEighths);
        }
    }
    if (sBlockTop)
    {
        if (!sBlockBottom || (sBlockBottom & block_face::flat) == 0 || (sBlockTop & block_face::flat) != 0)
        {
            MapRenderer::DrawTopSide(sBlockTop);
        }
    }
    if (sBlockBottom)
    {
        if (!sBlockTop || (sBlockTop & block_face::flat) == 0 || (sBlockBottom & block_face::flat) != 0)
        {
            MapRenderer::draw_partial_bottom(sBlockBottom, kZeroOnePoint.x, kZeroOnePoint.y, kFpThreeEighths);
        }
    }
    if (sLidType)
    {
        MapRenderer::draw_partial_lid(kZeroOnePoint.x, kZeroOnePoint.y, kZeroOnePoint.x, kFpThreeEighths);
    }
}

MATCH_FUNC(0x4F5760)
void MapRenderer::DrawPartialBlockBottom()
{
    u16 side_word;
    if (gBlockLeft)
    {
        if (gBlockRight)
        {
            if ((gBlockRight & block_face::flat) != 0)
            {
                side_word = gBlockLeft | 0x1000;
                MapRenderer::draw_left(side_word, kZeroOnePoint.y, kFpFiveEighths, kZeroOnePoint.y);
            }
            if ((gBlockLeft & block_face::flat) != 0)
            {
                side_word = gBlockRight | 0x1000;
                MapRenderer::draw_right(side_word, kZeroOnePoint.x, kFpFiveEighths, kZeroOnePoint.y);
            }
        }
    }
    if (sBlockTop)
    {
        if (sBlockBottom)
        {
            if ((sBlockBottom & block_face::flat) != 0)
            {
                side_word = sBlockTop | 0x1000;
                MapRenderer::draw_top(side_word, kZeroOnePoint.x, kZeroOnePoint.y, kZeroOnePoint.y);
            }
            if ((sBlockTop & block_face::flat) != 0)
            {
                side_word = sBlockBottom | 0x1000;
                MapRenderer::draw_partial_bottom(side_word, kZeroOnePoint.x, kZeroOnePoint.y, kFpFiveEighths);
            }
        }
    }
    if (gBlockLeft)
    {
        if (!gBlockRight || (gBlockRight & block_face::flat) == 0 || (gBlockLeft & block_face::flat) != 0)
        {
            MapRenderer::draw_left(gBlockLeft, kZeroOnePoint.x, kFpFiveEighths, kZeroOnePoint.y);
        }
    }
    if (gBlockRight)
    {
        if (!gBlockLeft || (gBlockLeft & block_face::flat) == 0 || (gBlockRight & block_face::flat) != 0)
        {
            MapRenderer::draw_right(gBlockRight, kZeroOnePoint.y, kFpFiveEighths, kZeroOnePoint.y);
        }
    }
    if (sBlockTop)
    {
        if (!sBlockBottom || (sBlockBottom & block_face::flat) == 0 || (sBlockTop & block_face::flat) != 0)
        {
            MapRenderer::draw_top(sBlockTop, kZeroOnePoint.x, kZeroOnePoint.y, kFpFiveEighths);
        }
    }
    if (sBlockBottom)
    {
        if (!sBlockTop || (sBlockTop & block_face::flat) == 0 || (sBlockBottom & block_face::flat) != 0)
        {
            MapRenderer::draw_bottom(sBlockBottom);
        }
    }
    if (sLidType)
    {
        MapRenderer::draw_partial_lid(kZeroOnePoint.x, kZeroOnePoint.y, kFpFiveEighths, kZeroOnePoint.y);
    }
}

MATCH_FUNC(0x4F5960)
void MapRenderer::DrawPartialBlockTopLeftCorner()
{
    u16 side_word;
    if (gBlockLeft)
    {
        if (gBlockRight)
        {
            if ((gBlockRight & block_face::flat) != 0)
            {
                side_word = gBlockLeft | 0x1000;
                MapRenderer::draw_left(side_word, kFpThreeEighths, kZeroOnePoint.x, kFpThreeEighths);
            }
            if ((gBlockLeft & block_face::flat) != 0)
            {
                side_word = gBlockRight | 0x1000;
                MapRenderer::draw_right(side_word, kZeroOnePoint.x, kZeroOnePoint.x, kFpThreeEighths);
            }
        }
    }
    if (sBlockTop)
    {
        if (sBlockBottom)
        {
            if ((sBlockBottom & block_face::flat) != 0)
            {
                side_word = sBlockTop | 0x1000;
                MapRenderer::draw_top(side_word, kZeroOnePoint.x, kFpThreeEighths, kFpThreeEighths);
            }
            if ((sBlockTop & block_face::flat) != 0)
            {
                side_word = sBlockBottom | 0x1000;
                MapRenderer::draw_partial_bottom(side_word, kZeroOnePoint.x, kFpThreeEighths, kZeroOnePoint.x);
            }
        }
    }
    if (gBlockLeft)
    {
        if (!gBlockRight || (gBlockRight & block_face::flat) == 0 || (gBlockLeft & block_face::flat) != 0)
        {
            MapRenderer::draw_left(gBlockLeft, kZeroOnePoint.x, kZeroOnePoint.x, kFpThreeEighths);
        }
    }
    if (gBlockRight)
    {
        if (!gBlockLeft || (gBlockLeft & block_face::flat) == 0 || (gBlockRight & block_face::flat) != 0)
        {
            MapRenderer::draw_right(gBlockRight, kFpThreeEighths, kZeroOnePoint.x, kFpThreeEighths);
        }
    }
    if (sBlockTop)
    {
        if (!sBlockBottom || (sBlockBottom & block_face::flat) == 0 || (sBlockTop & block_face::flat) != 0)
        {
            MapRenderer::draw_top(sBlockTop, kZeroOnePoint.x, kFpThreeEighths, kZeroOnePoint.x);
        }
    }
    if (sBlockBottom)
    {
        if (!sBlockTop || (sBlockTop & block_face::flat) == 0 || (sBlockBottom & block_face::flat) != 0)
        {
            MapRenderer::draw_partial_bottom(sBlockBottom, kZeroOnePoint.x, kFpThreeEighths, kFpThreeEighths);
        }
    }
    if (sLidType)
    {
        MapRenderer::draw_partial_lid(kZeroOnePoint.x, kFpThreeEighths, kZeroOnePoint.x, kFpThreeEighths);
    }
}

MATCH_FUNC(0x4F5B70)
void MapRenderer::DrawPartialBlockTopRightCorner()
{
    u16 side_word;
    if (gBlockLeft)
    {
        if (gBlockRight)
        {
            if ((gBlockRight & block_face::flat) != 0)
            {
                side_word = gBlockLeft | 0x1000;
                MapRenderer::draw_left(side_word, kZeroOnePoint.y, kZeroOnePoint.x, kFpThreeEighths);
            }
            if ((gBlockLeft & block_face::flat) != 0)
            {
                side_word = gBlockRight | 0x1000;
                MapRenderer::draw_right(side_word, kFpFiveEighths, kZeroOnePoint.x, kFpThreeEighths);
            }
        }
    }
    if (sBlockTop)
    {
        if (sBlockBottom)
        {
            if ((sBlockBottom & block_face::flat) != 0)
            {
                side_word = sBlockTop | 0x1000;
                MapRenderer::draw_top(side_word, kFpFiveEighths, kZeroOnePoint.y, kFpThreeEighths);
            }
            if ((sBlockTop & block_face::flat) != 0)
            {
                side_word = sBlockBottom | 0x1000;
                MapRenderer::draw_partial_bottom(side_word, kFpFiveEighths, kZeroOnePoint.y, kZeroOnePoint.x);
            }
        }
    }
    if (gBlockLeft)
    {
        if (!gBlockRight || (gBlockRight & block_face::flat) == 0 || (gBlockLeft & block_face::flat) != 0)
        {
            MapRenderer::draw_left(gBlockLeft, kFpFiveEighths, kZeroOnePoint.x, kFpThreeEighths);
        }
    }
    if (gBlockRight)
    {
        if (!gBlockLeft || (gBlockLeft & block_face::flat) == 0 || (gBlockRight & block_face::flat) != 0)
        {
            MapRenderer::draw_right(gBlockRight, kZeroOnePoint.y, kZeroOnePoint.x, kFpThreeEighths);
        }
    }
    if (sBlockTop)
    {
        if (!sBlockBottom || (sBlockBottom & block_face::flat) == 0 || (sBlockTop & block_face::flat) != 0)
        {
            MapRenderer::draw_top(sBlockTop, kFpFiveEighths, kZeroOnePoint.y, kZeroOnePoint.x);
        }
    }
    if (sBlockBottom)
    {
        if (!sBlockTop || (sBlockTop & block_face::flat) == 0 || (sBlockBottom & block_face::flat) != 0)
        {
            MapRenderer::draw_partial_bottom(sBlockBottom, kFpFiveEighths, kZeroOnePoint.y, kFpThreeEighths);
        }
    }
    if (sLidType)
    {
        MapRenderer::draw_partial_lid(kFpFiveEighths, kZeroOnePoint.y, kZeroOnePoint.x, kFpThreeEighths);
    }
}

MATCH_FUNC(0x4F5D80)
void MapRenderer::DrawPartialBlockBottomRightCorner()
{
    u16 side_word;
    if (gBlockLeft)
    {
        if (gBlockRight)
        {
            if ((gBlockRight & block_face::flat) != 0)
            {
                side_word = gBlockLeft | 0x1000;
                MapRenderer::draw_left(side_word, kZeroOnePoint.y, kFpFiveEighths, kZeroOnePoint.y);
            }
            if ((gBlockLeft & block_face::flat) != 0)
            {
                side_word = gBlockRight | 0x1000;
                MapRenderer::draw_right(side_word, kFpFiveEighths, kFpFiveEighths, kZeroOnePoint.y);
            }
        }
    }
    if (sBlockTop)
    {
        if (sBlockBottom)
        {
            if ((sBlockBottom & block_face::flat) != 0)
            {
                side_word = sBlockTop | 0x1000;
                MapRenderer::draw_top(side_word, kFpFiveEighths, kZeroOnePoint.y, kZeroOnePoint.y);
            }
            if ((sBlockTop & block_face::flat) != 0)
            {
                side_word = sBlockBottom | 0x1000;
                MapRenderer::draw_partial_bottom(side_word, kFpFiveEighths, kZeroOnePoint.y, kFpFiveEighths);
            }
        }
    }
    if (gBlockLeft)
    {
        if (!gBlockRight || (gBlockRight & block_face::flat) == 0 || (gBlockLeft & block_face::flat) != 0)
        {
            MapRenderer::draw_left(gBlockLeft, kFpFiveEighths, kFpFiveEighths, kZeroOnePoint.y);
        }
    }
    if (gBlockRight)
    {
        if (!gBlockLeft || (gBlockLeft & block_face::flat) == 0 || (gBlockRight & block_face::flat) != 0)
        {
            MapRenderer::draw_right(gBlockRight, kZeroOnePoint.y, kFpFiveEighths, kZeroOnePoint.y);
        }
    }
    if (sBlockTop)
    {
        if (!sBlockBottom || (sBlockBottom & block_face::flat) == 0 || (sBlockTop & block_face::flat) != 0)
        {
            MapRenderer::draw_top(sBlockTop, kFpFiveEighths, kZeroOnePoint.y, kFpFiveEighths);
        }
    }
    if (sBlockBottom)
    {
        if (!sBlockTop || (sBlockTop & block_face::flat) == 0 || (sBlockBottom & block_face::flat) != 0)
        {
            MapRenderer::draw_partial_bottom(sBlockBottom, kFpFiveEighths, kZeroOnePoint.y, kZeroOnePoint.y);
        }
    }
    if (sLidType)
    {
        MapRenderer::draw_partial_lid(kFpFiveEighths, kZeroOnePoint.y, kFpFiveEighths, kZeroOnePoint.y);
    }
}

MATCH_FUNC(0x4F5FD0)
void MapRenderer::DrawPartialBlockBottomLeftCorner()
{
    u16 side_word;
    if (gBlockLeft)
    {
        if (gBlockRight)
        {
            if ((gBlockRight & block_face::flat) != 0)
            {
                side_word = gBlockLeft | 0x1000;
                MapRenderer::draw_left(side_word, kFpThreeEighths, kFpFiveEighths, kZeroOnePoint.y);
            }
            if ((gBlockLeft & block_face::flat) != 0)
            {
                side_word = gBlockRight | 0x1000;
                MapRenderer::draw_right(side_word, kZeroOnePoint.x, kFpFiveEighths, kZeroOnePoint.y);
            }
        }
    }
    if (sBlockTop)
    {
        if (sBlockBottom)
        {
            if ((sBlockBottom & block_face::flat) != 0)
            {
                side_word = sBlockTop | 0x1000;
                MapRenderer::draw_top(side_word, kZeroOnePoint.x, kFpThreeEighths, kZeroOnePoint.y);
            }
            if ((sBlockTop & block_face::flat) != 0)
            {
                side_word = sBlockBottom | 0x1000;
                MapRenderer::draw_partial_bottom(side_word, kZeroOnePoint.x, kFpThreeEighths, kFpFiveEighths);
            }
        }
    }
    if (gBlockLeft)
    {
        if (!gBlockRight || (gBlockRight & block_face::flat) == 0 || (gBlockLeft & block_face::flat) != 0)
        {
            MapRenderer::draw_left(gBlockLeft, kZeroOnePoint.x, kFpFiveEighths, kZeroOnePoint.y);
        }
    }
    if (gBlockRight)
    {
        if (!gBlockLeft || (gBlockLeft & block_face::flat) == 0 || (gBlockRight & block_face::flat) != 0)
        {
            MapRenderer::draw_right(gBlockRight, kFpThreeEighths, kFpFiveEighths, kZeroOnePoint.y);
        }
    }
    if (sBlockTop)
    {
        if (!sBlockBottom || (sBlockBottom & block_face::flat) == 0 || (sBlockTop & block_face::flat) != 0)
        {
            MapRenderer::draw_top(sBlockTop, kZeroOnePoint.x, kFpThreeEighths, kFpFiveEighths);
        }
    }
    if (sBlockBottom)
    {
        if (!sBlockTop || (sBlockTop & block_face::flat) == 0 || (sBlockBottom & block_face::flat) != 0)
        {
            MapRenderer::draw_partial_bottom(sBlockBottom, kZeroOnePoint.x, kFpThreeEighths, kZeroOnePoint.y);
        }
    }
    if (sLidType)
    {
        MapRenderer::draw_partial_lid(kZeroOnePoint.x, kFpThreeEighths, kFpFiveEighths, kZeroOnePoint.y);
    }
}

MATCH_FUNC(0x4F6350)
void MapRenderer::DrawPartialCentreBlock()
{
    u16 side_word;
    if (gBlockLeft)
    {
        if (gBlockRight)
        {
            if ((gBlockRight & block_face::flat) != 0)
            {
                side_word = gBlockLeft | 0x1000;
                MapRenderer::draw_left(side_word, kFpFiveEighths, kFpThreeEighths, kFpFiveEighths);
            }
            if ((gBlockLeft & block_face::flat) != 0)
            {
                side_word = gBlockRight | 0x1000;
                MapRenderer::draw_right(side_word, kFpThreeEighths, kFpThreeEighths, kFpFiveEighths);
            }
        }
    }
    if (sBlockTop)
    {
        if (sBlockBottom)
        {
            if ((sBlockBottom & block_face::flat) != 0)
            {
                side_word = sBlockTop | 0x1000;
                MapRenderer::draw_top(side_word, kFpThreeEighths, kFpFiveEighths, kFpFiveEighths);
            }
            if ((sBlockTop & block_face::flat) != 0)
            {
                side_word = sBlockBottom | 0x1000;
                MapRenderer::draw_partial_bottom(side_word, kFpThreeEighths, kFpFiveEighths, kFpThreeEighths);
            }
        }
    }
    if (gBlockLeft)
    {
        if (!gBlockRight || (gBlockRight & block_face::flat) == 0 || (gBlockLeft & block_face::flat) != 0)
        {
            MapRenderer::draw_left(gBlockLeft, kFpThreeEighths, kFpThreeEighths, kFpFiveEighths);
        }
    }
    if (gBlockRight)
    {
        if (!gBlockLeft || (gBlockLeft & block_face::flat) == 0 || (gBlockRight & block_face::flat) != 0)
        {
            MapRenderer::draw_right(gBlockRight, kFpFiveEighths, kFpThreeEighths, kFpFiveEighths);
        }
    }
    if (sBlockTop)
    {
        if (!sBlockBottom || (sBlockBottom & block_face::flat) == 0 || (sBlockTop & block_face::flat) != 0)
        {
            MapRenderer::draw_top(sBlockTop, kFpThreeEighths, kFpFiveEighths, kFpThreeEighths);
        }
    }
    if (sBlockBottom)
    {
        if (!sBlockTop || (sBlockTop & block_face::flat) == 0 || (sBlockBottom & block_face::flat) != 0)
        {
            MapRenderer::draw_partial_bottom(sBlockBottom, kFpThreeEighths, kFpFiveEighths, kFpFiveEighths);
        }
    }
    if (sLidType)
    {
        MapRenderer::draw_partial_lid(kFpThreeEighths, kFpFiveEighths, kFpThreeEighths, kFpFiveEighths);
    }
}

MATCH_FUNC(0x4f6580)
void MapRenderer::DrawPartialBlocks()
{
    u8 slope_byte = spBlock->mSlopeType;
    gCurrentSlope.mGradientDirection = NO_GRADIENT_SLOPE_0;
    switch (GET_SLOPE_BITS(slope_byte))
    {
        case PARTIAL_BLOCK_LEFT: // 53
            DrawPartialBlockLeft();
            break;
        case PARTIAL_BLOCK_RIGHT: // 54
            DrawPartialBlockRight();
            break;
        case PARTIAL_BLOCK_TOP: // 55
            DrawPartialBlockTop();
            break;
        case PARTIAL_BLOCK_BOTTOM: // 56
            DrawPartialBlockBottom();
            break;
        case PARTIAL_TOP_LEFT_CORNER: // 57
            DrawPartialBlockTopLeftCorner();
            break;
        case PARTIAL_TOP_RIGHT_CORNER: // 58
            DrawPartialBlockTopRightCorner();
            break;
        case PARTIAL_BOTTOM_RIGHT_CORNER: // 59
            DrawPartialBlockBottomRightCorner();
            break;
        case PARTIAL_BOTTOM_LEFT_CORNER: // 60
            DrawPartialBlockBottomLeftCorner();
            break;
        case PARTIAL_CENTRE_BLOCK: // 61
            DrawPartialCentreBlock();
            break;
        default:
            break;
    }
}


MATCH_FUNC(0x4f6630)
void MapRenderer::DrawGradientSlope()
{
    u32 slope_idx = (spBlock->mSlopeType >> 2);

    if (!bSkip_slopes)
    {
        u8 gradient_direction = update_and_get_gradient_direction(slope_idx);

        switch (gradient_direction)
        {
            case NORTH_1:
                MapRenderer::DrawGradientSlopeNorthwards();
                break;
            case SOUTH_2:
                MapRenderer::DrawGradientSlopeSouthwards();
                break;
            case WEST_3:
                MapRenderer::DrawGradientSlopeWestwards();
                break;
            case EAST_4:
                MapRenderer::DrawGradientSlopeEastwards();
                break;
            default:
                return;
        }
    }
}

// https://decomp.me/scratch/8po7Q
MATCH_FUNC(0x4f66c0)
void MapRenderer::RenderFlatBlock()
{
    u16 side_word;
    gCurrentSlope.mGradientDirection = 0;

    if (gBlockLeft && gBlockRight)
    {
        if ((gBlockRight & block_face::flat) != 0)
        {
            side_word = gBlockLeft | 0x1000;
            MapRenderer::draw_left(side_word, kZeroOnePoint.y, kZeroOnePoint.x, kZeroOnePoint.y);
        }
        if ((gBlockLeft & block_face::flat) != 0)
        {
            side_word = gBlockRight | 0x1000;
            MapRenderer::draw_right(side_word, kZeroOnePoint.x, kZeroOnePoint.x, kZeroOnePoint.y);
        }
    }

    if (sBlockTop && sBlockBottom)
    {
        if ((sBlockBottom & block_face::flat) != 0)
        {
            side_word = sBlockTop | 0x1000;
            MapRenderer::draw_top(side_word, kZeroOnePoint.x, kZeroOnePoint.y, kZeroOnePoint.y);
        }
        if ((sBlockTop & block_face::flat) != 0)
        {
            side_word = sBlockBottom | 0x1000;
            MapRenderer::draw_partial_bottom(side_word, kZeroOnePoint.x, kZeroOnePoint.y, kZeroOnePoint.x);
        }
    }
    if (gBlockLeft)
    {
        if (gBlockRight == 0 || (gBlockRight & block_face::flat) == 0 || (gBlockLeft & block_face::flat) != 0)
        {
            MapRenderer::DrawLeftSide(gBlockLeft);
        }
    }
    if (gBlockRight)
    {
        if (gBlockLeft == 0 || (gBlockLeft & block_face::flat) == 0 || (gBlockRight & block_face::flat) != 0)
        {
            MapRenderer::DrawRightSide(gBlockRight);
        }
    }
    if (sBlockTop)
    {
        if (!sBlockBottom || (sBlockBottom & block_face::flat) == 0 || (sBlockTop & block_face::flat) != 0)
        {
            MapRenderer::DrawTopSide(sBlockTop);
        }
    }
    if (sBlockBottom)
    {
        if (sBlockTop == 0 || (sBlockTop & block_face::flat) == 0 || (sBlockBottom & block_face::flat) != 0)
        {
            MapRenderer::draw_bottom(sBlockBottom);
        }
    }
    if (sLidType)
    {
        MapRenderer::draw_lid();
    }
}

MATCH_FUNC(0x4f6880)
void MapRenderer::RenderBlockAt(s32& xpos, s32& ypos)
{
    gmp_block_info* pBlock = gMap_0x370->GetBlockClamped(xpos, ypos, gZCoord);
    spBlock = pBlock;
    if (pBlock)
    {
        gBlockLeft = pBlock->mLeft;
        gBlockRight = pBlock->mRight;
        sBlockTop = pBlock->mTop;
        sBlockBottom = pBlock->mBottom;

        u16 lid_idx = pBlock->mLid;
        sLidType = lid_idx;

        if (bShow_hidden_faces)
        {
            if (!lid_idx && !IS_AIR_TYPE(pBlock->mSlopeType))
            {
                sLidType = 0x260;
            }
            if ((gBlockLeft & block_face::wall) != 0 && (gBlockLeft & block_face::tile_idx_mask) == 0)
            {
                gBlockLeft = gBlockLeft | 0x260;
            }
            if ((gBlockRight & block_face::wall) != 0 && (gBlockRight & block_face::tile_idx_mask) == 0)
            {
                gBlockRight |= 0x260u;
            }
            if ((sBlockTop & block_face::wall) != 0 && (sBlockTop & block_face::tile_idx_mask) == 0)
            {
                sBlockTop |= 0x260u;
            }
            if ((sBlockBottom & block_face::wall) != 0 && (sBlockBottom & block_face::tile_idx_mask) == 0)
            {
                sBlockBottom = sBlockBottom | 0x260;
            }
        }
        gXCoord = Fix16(xpos) - gViewCamera->mCurPos.mX;
        gYCoord = Fix16(ypos) - gViewCamera->mCurPos.mY;

        u8 slope = GET_SLOPE_BITS(pBlock->mSlopeType);

        if (slope < DIAGONAL_WALL_UP_LEFT || slope > DIAGONAL_WALL_DOWN_RIGHT)
        {
            if (slope < TRIANGULAR_SIDES_DIAGONAL_UP_LEFT || slope > TRIANGULAR_SIDES_DIAGONAL_DOWN_RIGHT)
            {
                if (slope < PARTIAL_BLOCK_LEFT || slope > PARTIAL_CENTRE_BLOCK)
                {
                    if (slope > 0 && slope < DIAGONAL_WALL_UP_LEFT)
                    {
                        MapRenderer::DrawGradientSlope();
                    }
                    else
                    {
                        MapRenderer::RenderFlatBlock();
                    }
                }
                else
                {
                    MapRenderer::DrawPartialBlocks();
                }
            }
            else
            {
                MapRenderer::DrawTriangularDiagonal();
            }
        }
        else
        {
            MapRenderer::DrawDiagonalWall();
        }
    }
}

MATCH_FUNC(0x4f6a10)
void MapRenderer::ClearDrawnTileCount()
{
    mDrawnTileCount = 0;
}

// AddToDrawList with by-value coordinates, for Draw: the original's block loops are
// fully strength-reduced (both `max - rel` and `min + rel` as induction variables and a count-down
// counter), which VC6 only does when the expressions are written in the calls themselves (with
// `xpos_right`/`ypos_upwards` locals it reduces one of the two), and the header's `s32&` parameters
// can't take those rvalues.
static inline void AddToDrawListByVal(MapRenderer* pThis, s32 x, s32 y)
{
    DrawListBlock* pPos = &pThis->mDrawList[pThis->mCurrDrawLayerSize];
    pPos->mX = x;
    pPos->mY = y;

    DrawListBlock* pIter = &pThis->mDrawList[pThis->mCurrDrawLayerSize - 1];
    for (s32 i = pThis->mCurrDrawLayerSize - 1; i >= 0; i--, pIter--)
    {
        if (pIter->IsEqual(pPos))
        {
            return;
        }
    }
    ++pThis->mCurrDrawLayerSize;
}

// https://decomp.me/scratch/VJVUz
MATCH_FUNC(0x4f6a20)
void MapRenderer::Draw()
{
    // set ambient level
    if (gLighting)
    {
        pgbh_SetAmbient(mAmbient.ToFloat());
        MapRenderer::ambient_light_tick();
    }

    // render all things
    for (s32 zLayer = 0; zLayer < 7; zLayer++)
    {
        if (zLayer != 0)
        {
            gSpriteRenderer->Draw(zLayer); // draw all sprites at zLayer
        }

        // render blocks
        if (!bSkip_tiles)
        {
            // compute tile rendering boundaries
            Fix16 z_distance = gViewCamera->mCurPos.mZ - Fix16(zLayer) + Fix16(8);
            // Empty (debug) block: it keeps z_distance a variable, so VC6 loads the zoom before shifting z_distance
            if (bDo_show_camera)
            {
            }
            Fix16 layer_row_width = z_distance / gViewCamera->mCurPos.mZoom;

            // narrow the y direction because of assymetric monitor resolution
            Fix16 layer_column_width = layer_row_width * kScreenAspectRatio;

            // compute x boundary
            Fix16 half_x = layer_row_width / 2;
            s32 min_x = (gViewCamera->mCurPos.mX - half_x).ToInt();
            s32 max_x = (gViewCamera->mCurPos.mX + half_x).ToInt();

            // 10.5: the half-widths grow by one when the range is even, and the loops below start at
            // half-width - 1 (an earlier version here left the increment out and started at half-width).
            s32 x_semi_distance = (max_x - min_x + 1) / 2;
            if ((max_x - min_x) % 2 != 1)
            {
                x_semi_distance++;
            }

            // compute y boundary
            Fix16 half_y = layer_column_width / 2;
            s32 min_y = (gViewCamera->mCurPos.mY - half_y).ToInt();
            s32 max_y = (gViewCamera->mCurPos.mY + half_y).ToInt();

            s32 y_semi_distance = (max_y - min_y + 1) / 2;
            if ((max_y - min_y) % 2 != 1)
            {
                y_semi_distance++;
            }

            // update global Z coordinate
            gZCoord = zLayer;
            // A multiply, not Fix16(zLayer): VC6 must not reuse the shift of the first Fix16(zLayer) here
            sZCoordFp = Fix16(zLayer * 16384, 0);

            Fix16 depth_bottom = gViewCamera->mCurPos.mZ - Fix16(zLayer) + Fix16(8);
            Fix16 inv_depth_bottom;
            if (depth_bottom == kZeroOnePoint.x)
            {
                inv_depth_bottom = kZeroOnePoint.x;
            }
            else
            {
                inv_depth_bottom = kZeroOnePoint.y / depth_bottom;
            }

            gInvDepthBottom = inv_depth_bottom;
            gScreenScaleBottom = inv_depth_bottom * gViewCamera->mScreenScale.x;
            s32 top_layer = zLayer + 1;
            gZCoordTop = top_layer;

            Fix16 inv_depth_top;
            Fix16 depth_top = gViewCamera->mCurPos.mZ - Fix16(top_layer) + Fix16(8);
            if (depth_top == kZeroOnePoint.x)
            {
                inv_depth_top = kZeroOnePoint.x;
            }
            else
            {
                inv_depth_top = kZeroOnePoint.y / depth_top;
            }

            gInvDepthTop = inv_depth_top;
            gScreenScaleTop = inv_depth_top * gViewCamera->mScreenScale.x;

            // if zLayer = 0, reset lights
            if (zLayer == 0 && gLighting)
            {
                pgbh_ResetLights();
                pgbh_SetCamera(static_cast<f32>(min_x), static_cast<f32>(min_y), static_cast<f32>(max_x), static_cast<f32>(max_y));
                LightGrid::SubmitLightsInArea(min_x, min_y, max_x, max_y);
            }

            // Reset the size of draw layer size
            ResetCount();

            // Now iter over all blocks at zLayer and set up their rendering order
            // Begin with the blocks at the center of the camera and go away
            // In the end render in reverse order: far blocks to the nearest ones
            // (the coordinates are written in the calls, see AddToDrawListByVal)
            for (s32 ypos_rel = y_semi_distance - 1; ypos_rel >= 0; ypos_rel--)
            {
                for (s32 xpos_rel = x_semi_distance - 1; xpos_rel >= 0; xpos_rel--)
                {
                    AddToDrawListByVal(this, max_x - xpos_rel, max_y - ypos_rel);
                    AddToDrawListByVal(this, min_x + xpos_rel, max_y - ypos_rel);
                    AddToDrawListByVal(this, max_x - xpos_rel, min_y + ypos_rel);
                    AddToDrawListByVal(this, min_x + xpos_rel, min_y + ypos_rel);
                }
            }

            // Now draw tiles in reverse order
            s32 j = mCurrDrawLayerSize - 1;
            DrawListBlock* pIter = &mDrawList[mCurrDrawLayerSize - 1];
            while (j >= 0)
            {
                MapRenderer::RenderBlockAt(pIter->mX, pIter->mY);
                j--;
                pIter--;
            }
        }
    }
    gSpriteRenderer->Draw(7);    // draw all sprites on the highest layer
}