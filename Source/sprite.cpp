#include "sprite.hpp"
#include "explosion_type.hpp"
#include "car_despawn_status.hpp"
#include "CarInfoTable.hpp" // for gPixelsToFix16
#include "Car.hpp"
#include "Globals.hpp"
#include "Ped.hpp"
#include "Frontend.hpp"
#include "Camera.hpp"
#include "Draw.hpp"
#include "ObjectPool.hpp"
#include "Object_5C.hpp"
#include "PathFinder.hpp"
#include "Player.hpp"
#include "Police.hpp"
#include "SpriteGrid.hpp"
#include "CollisionSoundQueue.hpp"
#include "char.hpp"
#include "crt_stubs.hpp"
#include "debug.hpp"
#include "enums.hpp"
#include "error.hpp"
#include "gbh_graphics.hpp"
#include "gtx_0x106C.hpp"
#include "map_0x370.hpp"
#include "memory.hpp"
#include "root_sound.hpp"
#include "TextureCache.hpp"
#include "winmain.hpp" // for gLighting
#include "block_face.hpp"

// Forward declarations: the functions below are in address order
EXPORT void __stdcall ProjectOntoAxis(Fix16& xpos1, Fix16& ypos1, Ang16& angle, Fix16& xpos2, Fix16& ypos2, Fix16& outX, Fix16& outY);
static inline void ProjectToScreen_5A5690(Fix16 x, Fix16 y, Fix16 z, Fix16* pOut1, Fix16* pOut2);
EXTERN_GLOBAL(u16, gDebugColour);

DEFINE_GLOBAL(SpriteManager_8*, gSpriteManager, 0x703820);
// Used by map_0x370.cpp and Camera.cpp, which must only see them as extern (see the notes there)
DEFINE_GLOBAL(s16, gFaceCollisionMask, 0x6F6002);
DEFINE_GLOBAL_INIT(Ang16, kAng180_676772, Ang16(720), 0x676772);
DEFINE_GLOBAL(SpriteShapePool*, gSpriteShapePool, 0x70381C);
DEFINE_GLOBAL(SpritePool*, gSpritePool, 0x703818);
DEFINE_GLOBAL(SpriteMaskCache*, gSpriteMaskCache, 0x67AF1C);
DEFINE_GLOBAL(AttachedSpritePool*, gAttachedSpritePool, 0x703B80);
DEFINE_STATIC_GLOBAL(Sprite*, sSprite, 0x703814);
DEFINE_GLOBAL(Sprite*, gMapCollisionSprite, 0x6F61E8);
DEFINE_GLOBAL_INIT(Fix16, kFP16One_7035C4, Fix16(1), 0x7035C4);
DEFINE_GLOBAL_INIT(Fix16, kFpZero_7035C0, Fix16(0), 0x7035C0);
DEFINE_GLOBAL_INIT(Ang16, gAng16, Ang16(0), 0x703804);
DEFINE_GLOBAL_INIT(Fix16, kFpPoint2_703424, Fix16(0xCCC, 0), 0x703424);
DEFINE_GLOBAL_INIT(Fix16, kFP16Eight_7035E4, Fix16(8), 0x7035E4);
DEFINE_GLOBAL_INIT(Fix16, kFpOne64th_703578, Fix16(256, 0), 0x703578);
DEFINE_GLOBAL_INIT(Fix16, kFpOne64th_703678, kFpOne64th_703578, 0x703678);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kMaxSpriteZ, kFP16Eight_7035E4 - kFpOne64th_703678, 0x7033C0);
DEFINE_GLOBAL_INIT(Ang16, kAng90_70344C, Ang16(360), 0x70344C);
DEFINE_GLOBAL_INIT(Ang16, kAng180_70351E, Ang16(720), 0x70351E);
DEFINE_GLOBAL_INIT(Ang16, kAng270_703544, Ang16(1080), 0x703544);
DEFINE_GLOBAL_ARRAY(Vert, gSpriteVerts, 8, 0x7036D0);
DEFINE_GLOBAL_INIT(u32, kGlobalMask0, 0x0C00060, 0x61A9AC); // BitSet32 flag
DEFINE_GLOBAL_INIT(u32, kGlobalMask1, 0x0C70060, 0x61A9A8); // BitSet32 flag
DEFINE_GLOBAL_INIT(u32, kGlobalMask2, 0x0C78060, 0x61A9A4); // BitSet32 flag
DEFINE_GLOBAL(u32, gFlags, 0x67ACF8); // BitSet32 flag
DEFINE_GLOBAL_INIT(Fix16, kFP16Zero_703A38, Fix16(0), 0x703A38);

DEFINE_GLOBAL_INIT(Fix16, kFp255_7033B4, Fix16(255), 0x7033B4);
DEFINE_GLOBAL_INIT(Fix16, kFpSeven_7035DC, Fix16(7), 0x7035DC);
DEFINE_GLOBAL_INIT(Fix16, kFP16Two_7035C8, Fix16(2), 0x7035C8);

EXTERN_GLOBAL(s32, window_width);
EXTERN_GLOBAL(s32, window_height);

// matched
// 9.6f 0x4B9C70
static inline Fix16 __stdcall RoundZToLayer(Fix16& in)
{
    return Fix16((in.mValue + 0x80) & 0xFFFFFF00, 0);
}

// matched
// 9.6f 0x4B9990
static inline void __stdcall SaveUnprojectedVertex(f32 xCoord, f32 yCoord, f32 z_val, Vert* pVerts)
{
    s32 next_idx = (pVerts - gSpriteVerts) + 4;

    gSpriteVerts[next_idx].x = xCoord;
    gSpriteVerts[next_idx].y = yCoord;
    gSpriteVerts[next_idx].z = z_val;
}

// partially matched: https://decomp.me/scratch/qtmIe
// 9.6f 0x4BA4D0 takes zpos by value (on the stack, ret $4); the body compiles to the 9.6f code with VC7.
// The extra parentheses are x87 no-op nodes for VC6's scheduler (VC7 ignores them): they put the 81-node
// window breaks of Sprite::Draw's four expansions where the original's are (Scripts/x87_sched).
static inline void ProjectWorldPointToScreen(Fix16_Point& point, Vert* pVert, Fix16 zpos)
{
    SaveUnprojectedVertex(point.x.ToFloat(), point.y.ToFloat(), zpos.ToFloat(), pVert);
    pVert->z = static_cast<f32>(1.0 / (gViewCamera->mCurPos.mZ.ToFloat() + (8.0 - zpos.ToFloat())));
    pVert->x = ((((gViewCamera->mScreenScale.x.ToFloat())) * (((point.x.ToFloat()) - (gViewCamera->mCurPos.mX.ToFloat())))) * (pVert->z)) +
        static_cast<u32>(gViewCamera->mScreenPxCenterX);
    pVert->y = (((((gViewCamera->mScreenScale.x.ToFloat())) * ((point.y.ToFloat() - (gViewCamera->mCurPos.mY.ToFloat())))) * (pVert->z)) +
        static_cast<u32>(gViewCamera->mScreenPxCenterY));
}

static inline f32 Fix16ToFloat_Paren(const Fix16& v)
{
    return ((v.mValue / 16384.0f));
}

// The first expansion in Draw: four more no-op nodes before the y line (point.x/point.y) and four
// fewer in it, so the window break falls between the field_60 load and its fmul and the y line's fmulp
// waits behind the centre store, as in the original (regsearch: window 9 limit 76, window 10 limit 84).
static inline void ProjectWorldPointToScreen_First_4BA4D0(Fix16_Point& point, Vert* pVert, Fix16 zpos)
{
    SaveUnprojectedVertex((Fix16ToFloat_Paren(point.x)), (point.y.ToFloat()), zpos.ToFloat(), pVert);
    pVert->z = static_cast<f32>(1.0 / (gViewCamera->mCurPos.mZ.ToFloat() + (8.0 - zpos.ToFloat())));
    pVert->x = ((((gViewCamera->mScreenScale.x.ToFloat())) * (((point.x.ToFloat()) - (gViewCamera->mCurPos.mX.ToFloat())))) * (pVert->z)) +
        static_cast<u32>(gViewCamera->mScreenPxCenterX);
    pVert->y = ((gViewCamera->mScreenScale.x.ToFloat())) * ((point.y.ToFloat() - (gViewCamera->mCurPos.mY.ToFloat()))) * pVert->z +
        static_cast<u32>(gViewCamera->mScreenPxCenterY);
}

// not exactly __fastcall but something similar... https://decomp.me/scratch/N5g2D
static inline void __fastcall SetTileVertUVs(f32& u, f32& v)
{
    gSpriteVerts[0].u = 0.30000001f;
    gSpriteVerts[0].v = 0.30000001f;

    gSpriteVerts[1].u = u;
    gSpriteVerts[1].v = 0.30000001f;

    gSpriteVerts[2].u = u;
    gSpriteVerts[2].v = v;

    gSpriteVerts[3].u = 0.30000001f;
    gSpriteVerts[3].v = v;
}

// matched https://decomp.me/scratch/RhBO0
// 9.6f 0x4B9C20
static inline Fix16 __stdcall SignAsFix16(s32& value)
{
    if (value > 0)
    {
        return Fix16(1);
    }
    else if (value < 0)
    {
        return Fix16(-1);
    }
    else
    {
        return Fix16(0);
    }
}

// 9.6f 0x4207B0
MATCH_FUNC(0x443580)
Fix16_Point Sprite::get_x_y_443580()
{
    return Fix16_Point(mXy.x, mXy.y);
}

MATCH_FUNC(0x451950)
void Sprite::set_xyz_lazy_451950(Fix16 xpos, Fix16 ypos, Fix16 zpos)
{
    if (mXy.x != xpos || mXy.y != ypos || mZpos != zpos)
    {
        mXy.x = xpos;
        mXy.y = ypos;
        mZpos = zpos;
        InvalidateZColAndBBoxes();
    }
}

MATCH_FUNC(0x4833B0)
void Sprite::set_angle(Ang16 ang)
{
    if (ang != mRotation)
    {
        mRotation = ang;
        InvalidateZColAndBBoxes();
    }
}

MATCH_FUNC(0x48F5A0)
void SpriteMask::MarkUsed()
{
    SpriteMaskCache* pCache = gSpriteMaskCache;
    s32 new_idx = ++pCache->mUseCounter;
    mLastUsed = new_idx;
}

MATCH_FUNC(0x48F5C0)
EXPORT void SpriteMask::ClearMask(u8 xCount, u8 yCount)
{
    u8* pData = this->mPixels;
    for (s32 y = 0; y < yCount; y++)
    {
        memset(pData, 0, xCount);
        pData += 256;
    }
}

MATCH_FUNC(0x48f600)
SpriteMask* SpriteMaskCache::FindCachedMask(const u16& sprite_idx, const u32* pFlag, const u32* pDeltaMask, const u16* pExtra)
{
    s32 final_idx;
    s32 start_idx = 0;
    SpriteMask* pPartialMatch = NULL;
    if (!*pFlag)
    {
        final_idx = 32;
    }
    else
    {
        start_idx = 32;
        final_idx = 48;
    }

    s32 count = start_idx;
    for (SpriteMask* pIter = &mMasks[start_idx]; count < final_idx; count++, ++pIter)
    {
        s32 sprite_idx_copy = sprite_idx;
        if (pIter->mSpriteIdx == sprite_idx_copy && pIter->mPalette == *pExtra)
        {
            if (static_cast<u32>(pIter->mDeltaMask) == *pDeltaMask)
            {
                return pIter;
            }
            if (static_cast<u32>(pIter->mDeltaMask) < *pDeltaMask && (pIter->mDeltaMask | (*pDeltaMask - pIter->mDeltaMask)) == *pDeltaMask)
            {
                pPartialMatch = pIter;
            }
        }
    }
    return pPartialMatch;
}

MATCH_FUNC(0x48f690)
SpriteMask* SpriteMaskCache::FindLeastRecentlyUsed(const u32* pKey)
{
    u32 min_value = -1;
    s32 start_idx;
    s32 final_idx;
    SpriteMask* pOldest = NULL;
    if (!*pKey)
    {
        start_idx = 0;
        final_idx = 32;
    }
    else
    {
        start_idx = 32;
        final_idx = 48;
    }

    s32 count = start_idx;
    for (SpriteMask* pIter = &mMasks[start_idx]; count < final_idx; count++, ++pIter)
    {
        if (static_cast<u32>(pIter->mLastUsed) < min_value)
        {
            min_value = pIter->mLastUsed;
            pOldest = pIter;
        }
    }
    return pOldest;
}

MATCH_FUNC(0x48f6e0)
void SpriteMaskCache::InvalidateMasksBySpriteIdx(const u16* sprite_idx)
{
    s32 count = 0;
    for (SpriteMask* pIter = &this->mMasks[0]; count < 48; count++, ++pIter)
    {
        if (pIter->mSpriteIdx == *sprite_idx)
        {
            pIter->Invalidate();
        }
    }
}

MATCH_FUNC(0x48f710)
void SpriteMaskCache::InvalidateAllMasks()
{
    SpriteMask* pMask = mMasks;
    s32 remaining = 0x30;
    do
    {
        pMask->Invalidate();
        pMask++;
        remaining--;
    } while (remaining != 0);
}

MATCH_FUNC(0x48f730)
SpriteMaskCache::SpriteMaskCache()
{
    u32 row;
    mUseCounter = 0;
    mAlignedBuffer = NULL;
    mUnalignedAlloc = NULL;

    // By the way this is later used, it seems to be an array of a structure of size 0x40.
    // For now, it's a s32* as it make the code to match.
    u8* pPixels = static_cast<u8*>(Memory::Aligned_malloc(0x40000, reinterpret_cast<void**>(&mUnalignedAlloc)));
    mAlignedBuffer = pPixels;

    SpriteMask* pMask = mMasks;
    for (row = 8; row != 0; row--, pPixels += 0x4000, pMask += 4)
    {
        pMask[0].mPixels = pPixels;
        pMask[1].mPixels = (pPixels + 0x40);
        pMask[2].mPixels = (pPixels + 0x80);
        pMask[3].mPixels = (pPixels + 0xC0);
    }

    pMask = mMasks + 33;
    for (row = 4; row != 0; row--, pPixels += 0x8000, pMask += 4)
    {
        // I don't know why this one starts at -1...
        // Maybe an artifact of the decompilation
        pMask[-1].mPixels = pPixels;
        pMask[0].mPixels = (pPixels + 0x40);
        pMask[1].mPixels = (pPixels + 0x80);
        pMask[2].mPixels = (pPixels + 0xC0);
    }

    for (u16 mask_idx = 0; mask_idx < 48; mask_idx++)
    {
        mMasks[mask_idx].mMaskIdx = mask_idx;
    }
}

MATCH_FUNC(0x48F7F0)
SpriteMaskCache::~SpriteMaskCache()
{
    if (this->mUnalignedAlloc)
    {
        crt::free(this->mUnalignedAlloc);
    }
    this->mAlignedBuffer = NULL;
    this->mUnalignedAlloc = NULL;
}

MATCH_FUNC(0x48F820)
void DamageAreaFlags::ApplyDamageDeltas(u16& sprite_idx, u8* pArray, u32& delta_mask, u8& width)
{
    u32 remaining_mask = delta_mask;
    u32 delta_idx = 0;
    for (u32 i = 1; remaining_mask; i = i << 1, delta_idx++)
    {
        if ((i & remaining_mask) != 0)
        {
            if (delta_idx >= 22)
            {
                sprite_delta* pDelta = gGtx_0x106C->get_delta(sprite_idx, delta_idx - 17);
                if (pDelta)
                {
                    pDelta->ApplyMirroredToPixels(pArray, width);
                }
            }
            else
            {
                sprite_delta* pDelta = gGtx_0x106C->get_delta(sprite_idx, delta_idx);
                if (pDelta)
                {
                    pDelta->ApplyToPixels(pArray);
                }
            }
            remaining_mask &= ~i;
        }
    }
}

// 9.6f 0x4BA230
inline u16 Sprite::GetTrueSpriteIdx()
{
    return gGtx_0x106C->GetSpriteTrueIndex(mSpriteTypeEnum, mTypeSpriteIdx);
}

MATCH_FUNC(0x48F8B0)
s16 DamageAreaFlags::GetOrBuildDamageMask(u16& sprite_idx, u8& bRet, const u16& palette, const u32& bClearMask)
{
    if (!m_var)
    {
        return -1;
    }
    sprite_index* pSpriteIndex = gGtx_0x106C->get_sprite_index(sprite_idx);
    u32 bLargeMask = pSpriteIndex->mHeight > 0x40u;
    SpriteMask* pMask = gSpriteMaskCache->FindCachedMask(sprite_idx, &bLargeMask, &m_var, &palette);
    if (pMask)
    {
        if (static_cast<u32>(pMask->mDeltaMask) < m_var)
        {
            bRet = 1;
            u32 sub = m_var - pMask->mDeltaMask;
            ApplyDamageDeltas(sprite_idx, pMask->mPixels, sub, pSpriteIndex->mWidth);
            pMask->SetKey(sprite_idx, m_var, palette);
        }
        else
        {
            bRet = 0;
        }
    }
    else
    {
        bRet = 1;
        pMask = gSpriteMaskCache->FindLeastRecentlyUsed(&bLargeMask);
        if (!bClearMask)
        {
            pSpriteIndex->CopyPixels(pMask->mPixels);
        }
        else
        {
            pMask->ClearMask(pSpriteIndex->mWidth, pSpriteIndex->mHeight);
        }
        
        ApplyDamageDeltas(sprite_idx, pMask->mPixels, m_var, pSpriteIndex->mWidth);
        pMask->SetKey(sprite_idx, m_var, palette);
    }
    
    pMask->MarkUsed();
    return pMask->mMaskIdx;
}

MATCH_FUNC(0x4F76A0)
EXPORT bool __stdcall ComputeScanlineIntersectionY(Fix16& minX, Fix16& minY, Fix16& scanLineY, Fix16_Point& p0, Fix16_Point& p1)
{
    // Raw compares: with Fix16 operators VC6 runs out of inline budget and calls the Fix16_Point ctor
    // out of line. The out-of-line Sub gives the original's EH frame for pd. Remaining diff: the
    // original's success returns are `mov $1,%al; jmp` to one shared epilogue, ours copy the epilogue.
    Fix16_Point pd;

    if (p0.y == p1.y)
    {
        return false;
    }

    if (p0.y <= scanLineY && p1.y >= scanLineY)
    {
        pd = p1.Sub(p0);
        Fix16 x = p0.x + (((scanLineY - p0.y) * ((pd.x) / pd.y)));
        if (x.mValue >= minX.mValue && x.mValue <= minY.mValue)
        {
            gCollisionTarget.mVsegX = x;
            return true;
        }
    }
    else if (p1.y <= scanLineY && p0.y >= scanLineY)
    {
        pd = p0.Sub(p1);
        Fix16 x = p1.x + (((scanLineY - p1.y) * ((pd.x) / pd.y)));
        if (x.mValue >= minX.mValue && x.mValue <= minY.mValue)
        {
            gCollisionTarget.mVsegX = x;
            return true;
        }
    }
    return false;
}

MATCH_FUNC(0x4F77D0)
EXPORT bool __stdcall ComputeScanlineIntersectionX(Fix16& minX, Fix16& minY, Fix16& scanLineX, Fix16_Point& p0, Fix16_Point& p1)
{
    // Same shape as ComputeScanlineIntersectionY, same leftover diff.
    Fix16_Point pd;

    if (p0.x == p1.x)
    {
        return false;
    }

    if (p0.x <= scanLineX && p1.x >= scanLineX)
    {
        pd = p1 - p0;
        Fix16 y = p0.y + (((scanLineX - p0.x) * ((pd.y) / pd.x)));
        if (y.mValue >= minX.mValue && y.mValue <= minY.mValue)
        {
            gCollisionTarget.mHsegY = y;
            return true;
        }
    }
    else if (p1.x <= scanLineX && p0.x >= scanLineX)
    {
        pd = p0 - p1;
        Fix16 y = p1.y + (((scanLineX - p1.x) * ((pd.y) / pd.x)));
        if (y.mValue >= minX.mValue && y.mValue <= minY.mValue)
        {
            gCollisionTarget.mHsegY = y;
            return true;
        }
    }
    return false;
}

MATCH_FUNC(0x54EC80)
void Sprite::setxy_lazy(Fix16 xpos, Fix16 ypos)
{
    if (this->mXy.x != xpos || this->mXy.y != ypos)
    {
        this->mXy.x = xpos;
        this->mXy.y = ypos;
        InvalidateZColAndBBoxes();
    }
}

MATCH_FUNC(0x59E170)
bool Sprite::IsControlledByRemotePlayer()
{
    Ped* pPed = GetPed();
    if (!pPed)
    {
        Car* pCar = AsCar();
        if (pCar)
        {
            pPed = pCar->GetEffectiveDriver();
        }
    }

    if (pPed)
    {
        if (pPed->mPlayer)
        {
            if (!pPed->mPlayer->IsUser())
            {
                return true;
            }
        }
    }

    return false;
}

MATCH_FUNC(0x59E1B0)
Ped* Sprite::GetPed() const
{
    if (this->mSpriteTypeEnum == sprite_types_enum::ped_3 && this->mCharB4Ptr != NULL)
    {
        return mCharB4Ptr->mpPed;
    }
    else
    {
        return NULL;
    }
}

MATCH_FUNC(0x59E1D0)
s32 Sprite::IsOnWater()
{
    gmp_block_info* pBlock;

    const Fix16 zpos = mZpos;
    if (zpos > kFpZero_7035C0)
    {
        pBlock = gMap_0x370->GetBlock(mXy.x.ToInt(), mXy.y.ToInt(), (zpos - kFP16One_7035C4).ToInt());
    }
    else
    {
        pBlock = gMap_0x370->GetBlock(mXy.x.ToInt(), mXy.y.ToInt(), zpos.ToInt());
    }

    if (pBlock)
    {
        const u16 lid_idx = pBlock->mLid & block_face::tile_idx_mask;
        if (gGtx_0x106C->IsRemappedWaterTile(lid_idx))
        {
            return true;
        }
    }
    return false;
}

MATCH_FUNC(0x59E250)
u8 Sprite::GetWaterCornerMask()
{
    u8 bits = 0;
    Fix16 zpos_delta = this->mZpos - kFP16One_7035C4;
    UpdateCollisionBoundsIfNeeded();

    Fix16_Point* pBBox = mpCollisionShape->mCorners;
    for (s32 i = 0; i < 4; i++)
    {
        Fix16_Point* pIter = &pBBox[i];
        if (gMap_0x370->IsWaterBlockAt(pIter->x.ToInt(), pIter->y.ToInt(), zpos_delta.ToInt()))
        {
            bits |= 1 << i;
        }
    }

    return bits;
}

MATCH_FUNC(0x59e2e0)
void Sprite::CopyCollisionShapeToVisual(void)
{
    EnsureVisualShape();
    memcpy(mpVisualShape, mpCollisionShape, sizeof(SpriteShape));
}

MATCH_FUNC(0x59e300)
void Sprite::CopyVisualShapeToCollision()
{
    memcpy(mpCollisionShape, mpVisualShape, sizeof(SpriteShape));
}

MATCH_FUNC(0x59e320)
void Sprite::IncreaseZoom(char_type delta)
{
    u16 sprite_idx = GetTrueSpriteIdx();
    sprite_index* pSpriteIndex = gGtx_0x106C->get_sprite_index(sprite_idx);
    u32 height = pSpriteIndex->mHeight;
    u32 width = pSpriteIndex->mWidth;

    s32 max_zoom;
    if (static_cast<s32>(width & 0xFFFFFFFE) < static_cast<s32>(height & 0xFFFFFFFE))
    {
        max_zoom = width >> 1;
    }
    else
    {
        max_zoom = height >> 1;
    }
    mZoom = delta + mZoom;
    if ((mZoom & 0xFF) > max_zoom)
    {
        mZoom = max_zoom;
    }
}

// https://decomp.me/scratch/dijmx
MATCH_FUNC(0x59e390)
bool Sprite::ShrinkSprite(Fix16 xoff, Fix16 yoff, s32 bShrinkCollisionShape)
{
    EnsureVisualShape();

    Fix16 w_val;
    Fix16 h_val;
    Fix16 d_val;
    mpVisualShape->GetDimensions(&w_val, &h_val, &d_val);

    bool bHOrWLimit = mpVisualShape->ReduceWidthBy(xoff);
    bHOrWLimit |= mpVisualShape->ReduceHeightBy(yoff);

    if (bShrinkCollisionShape == 1)
    {
        Fix16 w_val1;
        Fix16 h_val1;
        Fix16 d_val1;
        mpCollisionShape->GetDimensions(&w_val1, &h_val1, &d_val1);

        Fix16 w_val2;
        Fix16 h_val2;
        Fix16 d_val2;
        mpVisualShape->GetDimensions(&w_val2, &h_val2, &d_val2);

        Fix16 new_w;
        if (w_val != kFpZero_7035C0)
        {
            new_w = (w_val1 / w_val) * w_val2;
        }
        else
        {
            new_w = kFpZero_7035C0;
        }

        Fix16 new_h;
        if (h_val != kFpZero_7035C0)
        {
            new_h = ((h_val1 / h_val) * h_val2);
        }
        else
        {
            new_h = kFpZero_7035C0;
        }

        mpCollisionShape->SetWidthHeightLazy(new_w, new_h);
    }
    return bHOrWLimit;
}

MATCH_FUNC(0x59e4c0)
void Sprite::ApplyScaleToDimensions(Fix16 scale, s32 bScaleCollisionShape)
{
    Sprite::EnsureVisualShape();

    mpVisualShape->ScaleWidthHeight(scale);

    if (bScaleCollisionShape == 1)
    {
        mpCollisionShape->ScaleWidthHeight(scale);
    }
}

// https://decomp.me/scratch/GIpfM
MATCH_FUNC(0x59E590)
bool Sprite::CollidesWith(Sprite* pOther)
{
    if (mpCollisionShape->mBoundingBox.AABB_Intersects(&pOther->mpCollisionShape->mBoundingBox))
    {
        if ((mpCollisionShape->IsZeroWidth() || mRotation.jIsAxisAligned()) &&
            (pOther->mpCollisionShape->IsZeroWidth() || pOther->mRotation.jIsAxisAligned()))
        {
            return true;
        }

        if (RotatedRectCollisionSAT(pOther) || pOther->RotatedRectCollisionSAT(this))
        {
            return true;
        }
    }

    return false;
}

// 9.6f 0x4BDF20
MATCH_FUNC(0x59E680)
bool Sprite::CheckDirectionalSliceCollision(Fix16 sliceLen, Sprite* pSprite)
{
    Fix16_Point point;
    point.FromPolar_41E210(-sliceLen / 2, pSprite->mRotation);

    sSprite->mpCollisionShape->CopyDimensions(pSprite->mpCollisionShape);
    sSprite->mpCollisionShape->ReduceHeightNoClamp(sliceLen);

    sSprite->mZpos = pSprite->mZpos;
    sSprite->mRotation = pSprite->mRotation;

    sSprite->mXy.x = point.x + pSprite->mXy.x;
    sSprite->mXy.y = point.y + pSprite->mXy.y;
    sSprite->InvalidateZColAndBBoxes();
    sSprite->UpdateCollisionBoundsIfNeeded();
    return CollidesWith(sSprite);
}

MATCH_FUNC(0x59e7b0)
void Sprite::InvalidateZColAndBBoxes()
{
    mZCol = -1;
    if (mpCollisionShape != NULL)
    {
        mpCollisionShape->mBboxUpToDate = false;
    }
    if (mpVisualShape != NULL)
    {
        mpVisualShape->mBboxUpToDate = false;
    }
}

static inline void __stdcall DrawTextScaled_4BA2C0(const wchar_t* pStr, Fix16 x, Fix16 y, u16 font)
{
    s32 palette_type = palette_types_enum::sprites_2;
    Fix16 scale_y = y * gViewCamera->mUiScale;
    DrawString(pStr, x * gViewCamera->mUiScale, scale_y, font, gViewCamera->mUiScale, palette_type, 0, 0, 0);
}

MATCH_FUNC(0x59e7d0)
Sprite* Sprite::QuerySpriteCollision(s32 sprite_type)
{
    Sprite* result;

    UpdateCollisionBoundsIfNeeded();
    mpCollisionShape->SetCurrentRect();
    gMapCollisionSprite = this;
    if (gMap_0x370->SpriteHitsDiagonalWall(mZpos.ToInt()))
    {
        return gCollisionTarget.mpHitSprite;
    }
    result = gSpriteGrid_1->FindCollidingSpritePreferType(this, sprite_type);
    if (result)
    {
        gCollisionTarget.SetSprite(result);
    }
    return result;
}

MATCH_FUNC(0x59E830)
bool Sprite::IsThreatToSearchingPed()
{
    switch (this->mSpriteTypeEnum)
    {
        case sprite_types_enum::car_2: // car
            return this->mCarBcPtr->IsThreatToSearchingPed();

        case sprite_types_enum::ped_3: // char_b4
            return this->mCharB4Ptr->IsThreatToSearchingPed();

        default:
            return true;
    }
}

MATCH_FUNC(0x59E850)
char_type Sprite::ShouldCollideWithSprite(Sprite* pSprite)
{
    switch (this->mSpriteTypeEnum)
    {
        case sprite_types_enum::object_1:
        case sprite_types_enum::code_obj1_4:
        case sprite_types_enum::map_obj_5:
            return mObject2CPtr->CanObjectCollideWithSprite(pSprite);
        case sprite_types_enum::car_2:
            return mCarBcPtr->CanCarCollideWithSprite(pSprite);
        case sprite_types_enum::ped_3:
            return mCharB4Ptr->ShouldCollideWithSprite(pSprite);
        default:
            return 1;
    }
}

MATCH_FUNC(0x59E8C0)
void Sprite::HandleObjectCollision(Sprite* pSprite)
{
    // 9.6f: AsObject2C on pSprite, then HandleCollisionWithObject_484DD0 (using AsObject2C here changes the code)
    s32 sprite_type = this->mSpriteTypeEnum;
    if (sprite_type == sprite_types_enum::object_1 || sprite_type > sprite_types_enum::ped_3 && sprite_type <= sprite_types_enum::map_obj_5)
    {
        mObject2CPtr->HandleImpact(pSprite);
        s32 type = pSprite->mSpriteTypeEnum;
        if (type == sprite_types_enum::code_obj1_4 || type == sprite_types_enum::map_obj_5 || type == sprite_types_enum::object_1)
        {
            if (pSprite->mObject2CPtr)
            {
                mObject2CPtr->HandleCollisionWithObject(pSprite->mObject2CPtr);
            }
        }
    }
}

MATCH_FUNC(0x59E910)
void Sprite::ProcessCarToCarImpactIfCar(Sprite* pSprite)
{
    if (mSpriteTypeEnum == sprite_types_enum::car_2)
    {
        mCarBcPtr->ProcessCarToCarImpact(pSprite);
    }
}

MATCH_FUNC(0x59E930)
bool Sprite::IsObjectModelEqual(s32 model) const
{
    return (IsObject2CType() && mObject2CPtr->mModel == model) ? true : false;
}

MATCH_FUNC(0x59e960)
void Sprite::SetDefaultDrawLayerBySpriteType()
{
    switch (this->mSpriteTypeEnum)
    {
        case sprite_types_enum::car_2:
            this->mDrawLayer = 15;
            break;
        case sprite_types_enum::ped_3:
            this->mDrawLayer = 23;
            break;
        case sprite_types_enum::code_obj1_4:
            this->mDrawLayer = 2;
            break;
        case sprite_types_enum::map_obj_5:
            this->mDrawLayer = 2;
            break;
        case sprite_types_enum::code_obj2_8:
            this->mDrawLayer = 33;
            break;
        case sprite_types_enum::none_0:
            this->mDrawLayer = 0;
            break;
        case sprite_types_enum::object_1:
            this->mDrawLayer = 0;
            break;
        default:
            return;
    }
}

// RotateAndTranslatePoint with the out-of-line Fix16 operator copies
static inline void __stdcall RotateAndTranslatePoint_ool_42A720(Fix16& pInX,
                                                                Fix16& pInY,
                                                                Ang16& pRotAng,
                                                                Fix16& pTransX,
                                                                Fix16& pTransY,
                                                                Fix16& pRotTransX,
                                                                Fix16& pRotTransY)
{
    pRotTransX = ((pInX - pTransX) * Ang16::cosine(pRotAng))
                     .Add((pInY - pTransY) * Ang16::sine(pRotAng));
    pRotTransY = ((pInX - pTransX).Negated() * Ang16::sine(pRotAng))
                     .Add((pInY - pTransY) * Ang16::cosine(pRotAng));
}

// SpriteShape::GetHalfExtents with the out-of-line Fix16 / s32 copy
static inline void GetHalfExtents_ool_4BA0A0(SpriteShape* pThis, Fix16* pHalfW, Fix16* pHalfH)
{
    *pHalfW = pThis->mWidth.DivideInt(2);
    *pHalfH = pThis->mHeight.DivideInt(2);
}

MATCH_FUNC(0x59e9c0)
void Sprite::UpdateCollisionBoundsIfNeeded()
{
    if (!mpCollisionShape->mBboxUpToDate)
    {
        if (mpCollisionShape->IsZeroWidth())
        {
            mpCollisionShape->UpdateRotatedBoundingBox(mXy.x, mXy.y, mZpos, gAng16);
        }
        else
        {
            mpCollisionShape->UpdateRotatedBoundingBox(mXy.x, mXy.y, mZpos, mRotation);
        }
    }
}

MATCH_FUNC(0x59ea00)
void Sprite::SetRemap(s16 remap)
{
    switch (mSpriteTypeEnum)
    {
        case sprite_types_enum::car_2:
            mPaletteType = palette_types_enum::car_remaps_3;
            break;
        case sprite_types_enum::ped_3:
            mPaletteType = palette_types_enum::ped_remaps_4;
            break;
        case sprite_types_enum::code_obj1_4:
        case sprite_types_enum::code_obj2_8:
            mPaletteType = palette_types_enum::code_obj_remaps_5;
            break;
        case sprite_types_enum::map_obj_5:
            mPaletteType = palette_types_enum::map_obj_remaps_6;
            break;
        case sprite_types_enum::user_6:
            mPaletteType = palette_types_enum::user_remaps_7;
            break;
        case sprite_types_enum::font_7:
            mPaletteType = palette_types_enum::font_remaps_8;
        default:
            break;
    }
    mRemap = remap;
}

MATCH_FUNC(0x59eaa0)
s16 Sprite::GetTruePalette() const
{
    if (mPaletteType == palette_types_enum::sprites_2)
    {
        s16 sprite_idx = gGtx_0x106C->GetSpriteTrueIndex(mSpriteTypeEnum, mTypeSpriteIdx);
        return gGtx_0x106C->GetTruePalette(palette_types_enum::sprites_2, sprite_idx);
    }
    return gGtx_0x106C->GetTruePalette(mPaletteType, mRemap);
}

// https://decomp.me/scratch/5emc4
// RotateAndTranslatePoint as RotatedRectCollisionSAT gets it once it has run out of
// inline expansions: the unary minus is the out-of-line copy (Negated)
static inline void __stdcall RotateAndTranslatePoint_NegOOL_42A720(Fix16& pInX,
                                                                  Fix16& pInY,
                                                                  Ang16& pRotAng,
                                                                  Fix16& pTransX,
                                                                  Fix16& pTransY,
                                                                  Fix16& pRotTransX,
                                                                  Fix16& pRotTransY)
{
    pRotTransX = (((pInX - pTransX) * Ang16::cosine(pRotAng)) + ((pInY - pTransY) * Ang16::sine(pRotAng)));
    pRotTransY = (((pInX - pTransX).Negated() * Ang16::sine(pRotAng)) + ((pInY - pTransY) * Ang16::cosine(pRotAng)));
}

MATCH_FUNC(0x59eae0)
char_type Sprite::CastsShadow()
{
    switch (mSpriteTypeEnum)
    {
        case sprite_types_enum::ped_3:
            return mCharB4Ptr->HasShadows();
        case sprite_types_enum::car_2:
            return 1;
        case sprite_types_enum::object_1:
        case sprite_types_enum::code_obj1_4:
        case sprite_types_enum::map_obj_5:
            return mObject2CPtr->mDefinition->mHasShadows;
        case sprite_types_enum::user_6:
        case sprite_types_enum::font_7:
        case sprite_types_enum::code_obj2_8:
            return 0;
        default:
            break;
    }
    return 0;
}

MATCH_FUNC(0x59EB30)
void Sprite::ShowId(f32& x, f32& y)
{
    if (bDo_show_ids)
    {
        Car* pCar = AsCar();
        if (pCar)
        {
            Fix16 xpos;
            xpos = Fix16(static_cast<s32>((x / static_cast<f32>(static_cast<u32>(window_width))) * 640.0f));
            Fix16 ypos;
            ypos = Fix16(static_cast<s32>((y / static_cast<f32>(static_cast<u32>(window_height))) * 480.0f));
            swprintf(tmpBuff, L"%d", pCar->mCarId);
            DrawTextScaled_4BA2C0(tmpBuff, xpos, ypos, gStyleTextFont);
        }
        else
        {
            Char* pChar = AsCharB4();
            if (pChar)
            {
                Fix16 xpos;
                xpos = Fix16(static_cast<s32>((x / static_cast<f32>(static_cast<u32>(window_width))) * 640.0f));
                Fix16 ypos;
                ypos = Fix16(static_cast<s32>((y / static_cast<f32>(static_cast<u32>(window_height))) * 480.0f));
                swprintf(tmpBuff, L"%d", pChar->mpPed->mId);
                s32 palette_type = palette_types_enum::sprites_2;
                DrawString(tmpBuff,
                           xpos * gViewCamera->mUiScale,
                           ypos * gViewCamera->mUiScale,
                           gStyleTextFont,
                           gViewCamera->mUiScale,
                           palette_type,
                           0,
                           0,
                           0);
            }
        }
    }

    if (bDo_show_object_ids)
    {
        Object* pObj = AsObject2C();
        if (pObj)
        {
            Fix16 xpos;
            xpos = Fix16(static_cast<s32>((x / static_cast<f32>(static_cast<u32>(window_width))) * 640.0f));
            Fix16 ypos;
            ypos = Fix16(static_cast<s32>((y / static_cast<f32>(static_cast<u32>(window_height))) * 480.0f));
            swprintf(tmpBuff, L"%d:%d", pObj->mModel, pObj->mId);
            s32 palette_type = palette_types_enum::sprites_2;
            DrawString(tmpBuff,
                       xpos * gViewCamera->mUiScale,
                       ypos * gViewCamera->mUiScale,
                       gStyleTextFont,
                       gViewCamera->mUiScale,
                       palette_type,
                       0,
                       0,
                       0);
        }
    }
}

MATCH_FUNC(0x59ee40)
void Sprite::ShowHorn(f32& x, f32& y)
{
    // 9.6f 0x4BAEF0
    if (bDo_show_horn)
    {
        Car* pCar = AsCar();
        if (pCar)
        {
            Fix16 xpos(static_cast<s32>((x / static_cast<f32>(static_cast<u32>(window_width))) * 640.0f));
            Fix16 ypos(static_cast<s32>((y / static_cast<f32>(static_cast<u32>(window_height))) * 480.0f));

            if (pCar->IsEmittingHorn())
            {
                DrawTextScaled_4BA2C0(L"H", xpos, ypos, gStyleTextFont);
            }

            Ped* pDriver = pCar->mDriver;
            if (pDriver)
            {
                if (gPolice_7B8->IsActivelyChased(pDriver))
                {
                    DrawTextScaled_4BA2C0(L"P", xpos, ypos, gStyleTextFont);
                }
            }
        }
    }
}

// https://decomp.me/scratch/EHeIY
MATCH_FUNC(0x59eff0)
void Sprite::Draw()
{
    sprite_index* pSpriteIndex;
    u16 true_sprite_idx;
    sprite_index* pSourceSpriteIndex;
    u16 pal;
    STexture* pTexture;
    f32 u;
    f32 v;

    u16 sprite_idx;
    u32 car_flags;

    Sprite::EnsureVisualShape();
    if (mZoom != 0)
    {
        sprite_idx = gGtx_0x106C->mSpriteIndexCount - 1;
        pSpriteIndex = gGtx_0x106C->get_sprite_index(sprite_idx);
        true_sprite_idx = gGtx_0x106C->GetSpriteTrueIndex(mSpriteTypeEnum, mTypeSpriteIdx);
        pSourceSpriteIndex = gGtx_0x106C->get_sprite_index(true_sprite_idx);
        pSpriteIndex->mWidth = pSourceSpriteIndex->mWidth - 2 * mZoom;
        pSpriteIndex->mHeight = pSourceSpriteIndex->mHeight - 2 * mZoom;
        if (!pSpriteIndex->mWidth || !pSpriteIndex->mHeight)
        {
            return;
        }
        pSpriteIndex->mpData = &pSourceSpriteIndex->mpData[257 * static_cast<u8>(mZoom)];
        pal = Sprite::GetTruePalette();
        pTexture = gTextureCache_15D8->SetSharedTextureData(pSpriteIndex->mWidth,
                                                         pSpriteIndex->mHeight,
                                                         pSpriteIndex->mpData,
                                                         pal);
    }
    else
    {
        sprite_idx = gGtx_0x106C->GetSpriteTrueIndex(mSpriteTypeEnum, mTypeSpriteIdx);
        pSpriteIndex = gGtx_0x106C->get_sprite_index(sprite_idx);
        pTexture = gTextureCache_15D8->GetSpriteTexture(mSpriteTypeEnum, mTypeSpriteIdx, mPaletteType, mRemap);
    }

    if (!mpVisualShape->mBboxUpToDate)
    {
        mpVisualShape->UpdateRotatedBoundingBox(mXy.x, mXy.y, mZpos, mRotation);
    }

    Fix16_Point* pRect = mpVisualShape->mCorners;

    u = pSpriteIndex->mWidth - 0.30000001f;
    v = pSpriteIndex->mHeight - 0.30000001f;

    Fix16 new_zpos = RoundZToLayer(mZpos);

    ProjectWorldPointToScreen_First_4BA4D0(pRect[0], &gSpriteVerts[0], new_zpos);
    ProjectWorldPointToScreen(pRect[1], &gSpriteVerts[1], new_zpos);
    ProjectWorldPointToScreen(pRect[2], &gSpriteVerts[2], new_zpos);
    ProjectWorldPointToScreen(pRect[3], &gSpriteVerts[3], new_zpos);
    SetTileVertUVs(u, v);

    Car* pCar = AsCar();
    if (pCar)
    {
        if (mZoom)
        {
            gSpriteMaskCache->InvalidateMasksBySpriteIdx(&sprite_idx);
        }
        car_flags = pCar->mDamagedAreas.GetRaw();
        if (gLighting)
        {
            if (pCar->HasBottomLeftRoofLight())
            {
                pCar->mDamagedAreas.SetGlobal0(); // sets a global to another
            }
            else
            {
                if (pCar->IsFbiCar())
                {
                    pCar->mDamagedAreas.SetGlobal1();
                }
                else
                {
                    pCar->mDamagedAreas.SetGlobal2();
                }
            }
            pCar->mDamagedAreas.ClearGlobalFlags();
        }
        u8 bRet;
        u16 damage_mask_idx = pCar->mDamagedAreas.GetOrBuildDamageMask(sprite_idx, bRet, Sprite::GetTruePalette(), false);
        if (damage_mask_idx != 0xFFFF)
        {
            if (bRet)
            {
                gTextureCache_15D8->SetCarDamageTextureSizeAndPalette(damage_mask_idx,
                                                      pSpriteIndex->mWidth,
                                                      pSpriteIndex->mHeight,
                                                      Sprite::GetTruePalette());
            }
            pTexture = gTextureCache_15D8->GetCarDamageTexture(damage_mask_idx);
        }
        pCar->mDamagedAreas.m_var = car_flags;
    }
    else
    {
        Char* pChar = AsCharB4();
        if (pChar)
        {
            Ped* pPed = pChar->get_ped();
            if (pPed && pPed->mPlayer)
            {
                if (pPed->mBf21C.bInvisible && !pPed->mPlayer->IsUser())
                {
                    return;
                }
            }
        }
    }

    if (Sprite::CastsShadow())
    {
        Vert pShadowVerts[4];

        memcpy(&pShadowVerts[0], &gSpriteVerts[0], sizeof(Vert));

        memcpy(&pShadowVerts[1], &gSpriteVerts[1], sizeof(Vert));
        memcpy(&pShadowVerts[2], &gSpriteVerts[2], sizeof(Vert));
        memcpy(&pShadowVerts[3], &gSpriteVerts[3], sizeof(Vert));

        f32 shadow_offset = (gViewCamera->mUiScale * 5).ToFloat();
        pShadowVerts[0].diff = 0xA0000000;
        pShadowVerts[1].diff = 0xA0000000;
        pShadowVerts[2].diff = 0xA0000000;
        pShadowVerts[3].diff = 0xA0000000;

        pShadowVerts[0].x = gSpriteVerts[0].x + shadow_offset;
        pShadowVerts[1].x = gSpriteVerts[1].x + shadow_offset;
        pShadowVerts[2].x = gSpriteVerts[2].x + shadow_offset;
        pShadowVerts[3].x = gSpriteVerts[3].x + shadow_offset;

        pShadowVerts[0].y += shadow_offset;
        pShadowVerts[1].y += shadow_offset;
        pShadowVerts[2].y += shadow_offset;
        pShadowVerts[3].y += shadow_offset;

        pgbh_DrawQuad(8576, pTexture, pShadowVerts, 255);
    }

    pgbh_DrawQuad(Sprite::ComputeDrawFlags(), pTexture, gSpriteVerts, 255);

    if (pCar && gLighting)
    {
        pCar->mDamagedAreas.MaskWithGlobalFlags();

        u8 bRet;
        u16 damage_mask_idx = pCar->mDamagedAreas.GetOrBuildDamageMask(sprite_idx, bRet, Sprite::GetTruePalette(), true);
        if (damage_mask_idx != 0xFFFF)
        {
            if (bRet)
            {
                gTextureCache_15D8->SetCarDamageTextureSizeAndPalette(damage_mask_idx,
                                                      pSpriteIndex->mWidth,
                                                      pSpriteIndex->mHeight,
                                                      Sprite::GetTruePalette());
            }
            pTexture = gTextureCache_15D8->GetCarDamageTexture(damage_mask_idx);
            SetTileVertUVs(u, v);
            pgbh_DrawQuad(128, pTexture, gSpriteVerts, 255);
        }
        pCar->mDamagedAreas.m_var = car_flags;
    }

    ++gSpriteManager->mDrawnSpriteCount;
    if (bDo_show_collision_box)
    {
        if (mpCollisionShape)
        {
            mpCollisionShape->DrawCollisionBox(mZpos);
        }
    }

    Sprite::ShowId(gSpriteVerts[1].x, gSpriteVerts[1].y);
    Sprite::ShowHorn(gSpriteVerts[3].x, gSpriteVerts[3].y);
}

MATCH_FUNC(0x59f950)
void Sprite::AllocCollisionShape(Fix16 width, Fix16 height, Fix16 depth)
{
    if (mpCollisionShape == NULL)
    {
        mpCollisionShape = gSpriteShapePool->Allocate();
    }

    mpCollisionShape->SetDimensions(width, height, depth);
}

MATCH_FUNC(0x59f990)
void Sprite::EnsureVisualShape()
{
    if (this->mpVisualShape == NULL)
    {
        this->mpVisualShape = gSpriteShapePool->Allocate();

        const u16 sprite_idx = GetTrueSpriteIdx();
        const sprite_index* pSpriteIndex = gGtx_0x106C->get_sprite_index(sprite_idx);

        Fix16 w;
        Fix16 h;

        if (this->mSpriteTypeEnum == sprite_types_enum::code_obj2_8)
        {
            w = gPixelsToFix16.list[pSpriteIndex->mWidth] / 2;
            h = gPixelsToFix16.list[pSpriteIndex->mHeight] / 2;
        }
        else
        {
            w = gPixelsToFix16.list[pSpriteIndex->mWidth];
            h = gPixelsToFix16.list[pSpriteIndex->mHeight];
        }
        mpVisualShape->SetWidthHeight(w, h);
    }
}

MATCH_FUNC(0x59fa40)
void Sprite::UpdateDimensionsFromSpriteIndex()
{
    if (mpVisualShape)
    {
        const u16 idx = GetTrueSpriteIdx();
        sprite_index* pSpriteIndex = gGtx_0x106C->get_sprite_index(idx);

        const Fix16 height = gPixelsToFix16.list[pSpriteIndex->mHeight];
        const Fix16 width = gPixelsToFix16.list[pSpriteIndex->mWidth];

        mpVisualShape->SetWidthHeightLazy(width, height);
    }
}

// 9.6f 0x401C80: Ang16::operator-(), the normalizing ctor called out of line (AssignNormalized)
static inline Ang16 NegateAng16(const Ang16& angle)
{
    s16 value = -angle.rValue;
    return Ang16(&value, 0);
}

MATCH_FUNC(0x59fad0)
void Sprite::FreeShapes()
{
    if (mpCollisionShape)
    {
        gSpriteShapePool->Remove(mpCollisionShape);
        mpCollisionShape = NULL;
    }

    if (mpVisualShape)
    {
        gSpriteShapePool->Remove(mpVisualShape);
        mpVisualShape = NULL;
    }
}

MATCH_FUNC(0x59FB10)
bool Sprite::IntersectsRectSAT(Fix16_Rect* pOtherRect)
{
    Fix16 half_width;
    Fix16 half_height;

    GetHalfExtents_ool_4BA0A0(mpCollisionShape, &half_width, &half_height);

    Fix16 rot_x;
    Fix16 rot_y;

    // First rotation
    RotateAndTranslatePoint_ool_42A720(pOtherRect->get_left(),
                                   pOtherRect->get_top(),
                                   -mRotation,
                                   mXy.x,
                                   mXy.y,
                                   rot_x,
                                   rot_y);

    if (rot_x >= -half_width && rot_x <= half_width)
    {
        if (rot_y >= -half_height && rot_y <= half_height)
        {
            return true;
        }
    }

    // Second rotation
    RotateAndTranslatePoint_ool_42A720(pOtherRect->get_right(),
                                   pOtherRect->get_top(),
                                   -mRotation,
                                   mXy.x,
                                   mXy.y,
                                   rot_x,
                                   rot_y);

    if (rot_x >= -half_width && rot_x <= half_width)
    {
        if (rot_y >= -half_height && rot_y <= half_height)
        {
            return true;
        }
    }

    // Third rotation
    RotateAndTranslatePoint_ool_42A720(pOtherRect->get_right(),
                                   pOtherRect->get_bottom(),
                                   -mRotation,
                                   mXy.x,
                                   mXy.y,
                                   rot_x,
                                   rot_y);

    if (rot_x >= -half_width && rot_x <= half_width)
    {
        if (rot_y >= -half_height && rot_y <= half_height)
        {
            return true;
        }
    }

    // Fourth rotation
    RotateAndTranslatePoint_ool_42A720(pOtherRect->get_left(),
                                   pOtherRect->get_bottom(),
                                   -mRotation,
                                   mXy.x,
                                   mXy.y,
                                   rot_x,
                                   rot_y);

    if (rot_x >= -half_width && rot_x <= half_width)
    {
        if (rot_y >= -half_height && rot_y <= half_height)
        {
            return true;
        }
    }

    // Last rotation
    // In 9.6f, it's RotateAndTranslatePoint. Here the function has run out of inline
    // expansions: the rotation is the out-of-line copy and only the y sum stays inline. The
    // operators are the named out-of-line copies used above, so each has a single callee.
    ProjectOntoAxis(((pOtherRect->get_left()).Add(pOtherRect->get_right())).DivideInt(2),
                    (pOtherRect->get_top() + pOtherRect->get_bottom()).DivideInt(2),
                    -mRotation,
                    mXy.x,
                    mXy.y,
                    rot_x,
                    rot_y);

    if (rot_x >= half_width.Negated() && rot_x <= half_width)
    {
        if (rot_y >= half_height.Negated() && rot_y <= half_height)
        {
            return true;
        }
    }

    return false;
}

// 9.6f 0x4B9F80: clamp the current rect to the map
static inline void ClampGridRectToMap()
{
    if (gSpriteGrid_left < 0)
    {
        gSpriteGrid_left = 0;
    }

    if (gSpriteGrid_right > 255)
    {
        gSpriteGrid_right = 255;
    }

    if (gSpriteGrid_top < 0)
    {
        gSpriteGrid_top = 0;
    }

    if (gSpriteGrid_bottom > 255)
    {
        gSpriteGrid_bottom = 255;
    }
}

MATCH_FUNC(0x5a0150)
char_type Sprite::FindOverlappingBoundingBoxCorners(Sprite* pOther, u8* pOut1, u8* pOut2)
{
    u8 i = 0;
    char_type counter = 0;

    Fix16_Point* pBBox = pOther->mpCollisionShape->mCorners;

    Fix16 half_w;
    Fix16 half_h;
    // corner_x/y declared up here: two fewer inline candidates after GetHalfExtents keep both of its
    // divisions inline
    Fix16 corner_x;
    Fix16 corner_y;
    mpCollisionShape->GetHalfExtents(&half_w, &half_h);

    for (i = 0; i < 4; i++)
    {
        Ang16 t = -mRotation;
        Fix16_Point* pCorner = &pBBox[i];
        RotateAndTranslatePoint(pCorner->x, pCorner->y, t, mXy.x, mXy.y, corner_x, corner_y);

        if (((corner_x >= -half_w) && (corner_x <= half_w) && (corner_y >= -half_h) && (corner_y <= half_h)))
        {
            counter++;
            if (counter == 1)
            {
                *pOut1 = i;
            }
            else if (counter == 2)
            {
                *pOut2 = i;
            }
            else
            {
                return 0;
            }
        }
    }
    return counter;
}

MATCH_FUNC(0x5a0320)
char_type Sprite::CountCornersInRect(Fix16* pXY1, Fix16* pXY2, u8* pCollisionIdx1, u8* pCollisionIdx2)
{
    u8 overlapCount = 0;
    const Fix16_Point* pBoundingBox = &mpCollisionShape->mCorners[0];

    for (u8 i = 0; i < 4; i++)
    {
        if ((pBoundingBox[i].x > pXY1[0] && pBoundingBox[i].y > pXY1[1]) && (pBoundingBox[i].x < pXY2[0] && pBoundingBox[i].y < pXY2[1]))
        {
            // If we find the first valid match, store index in pCollisionIdx1
            overlapCount++;

            if (overlapCount == 1)
            {
                *pCollisionIdx1 = i;
            }
            // If we find the second valid match, store index in pCollisionIdx2
            else if (overlapCount == 2)
            {
                *pCollisionIdx2 = i;
            }
        }
    }

    return overlapCount;
}

MATCH_FUNC(0x5a0380)
bool Sprite::RotatedRectCollisionSAT(Sprite* pOther)
{
    if (mpCollisionShape->IsZeroWidth())
    {
        return mpCollisionShape->mBoundingBox.IntersectsSpriteRenderingRect(pOther);
    }

    Fix16 half_width;
    Fix16 half_height;

    Fix16_Point* pOtherRenderRect = pOther->mpCollisionShape->mCorners;

    mpCollisionShape->GetHalfExtents(&half_width, &half_height);

    Fix16 rot_x;
    Fix16 rot_y;

    // First rotation
    RotateAndTranslatePoint_NegOOL_42A720(pOtherRenderRect[0].x,
                                   pOtherRenderRect[0].y,
                                   -mRotation,
                                   mXy.x,
                                   mXy.y,
                                   rot_x,
                                   rot_y);

    if (rot_x >= -half_width && rot_x <= half_width)
    {
        if (rot_y >= -half_height && rot_y <= half_height)
        {
            return true;
        }
    }

    // Second rotation
    RotateAndTranslatePoint_NegOOL_42A720(pOtherRenderRect[1].x,
                                   pOtherRenderRect[1].y,
                                   -mRotation,
                                   mXy.x,
                                   mXy.y,
                                   rot_x,
                                   rot_y);

    if (rot_x >= -half_width && rot_x <= half_width)
    {
        if (rot_y >= -half_height && rot_y <= half_height)
        {
            return true;
        }
    }

    // Third rotation
    RotateAndTranslatePoint_NegOOL_42A720(pOtherRenderRect[2].x,
                                   pOtherRenderRect[2].y,
                                   -mRotation,
                                   mXy.x,
                                   mXy.y,
                                   rot_x,
                                   rot_y);

    if (rot_x >= -half_width && rot_x <= half_width)
    {
        if (rot_y >= -half_height && rot_y <= half_height)
        {
            return true;
        }
    }

    // Fourth rotation
    RotateAndTranslatePoint_NegOOL_42A720(pOtherRenderRect[3].x,
                                   pOtherRenderRect[3].y,
                                   -mRotation,
                                   mXy.x,
                                   mXy.y,
                                   rot_x,
                                   rot_y);

    if (rot_x >= half_width.Negated() && rot_x <= half_width)
    {
        if (rot_y >= half_height.Negated() && rot_y <= half_height)
        {
            return true;
        }
    }

    // Last rotation (it's different)
    // Out-of-line copies: the function has run out of inline expansions
    ProjectOntoAxis(pOther->mXy.x,
                    pOther->mXy.y,
                    GetNegatedAngle(),
                    mXy.x,
                    mXy.y,
                    rot_x,
                    rot_y);

    if (rot_x >= half_width.Negated() && rot_x <= half_width)
    {
        if (rot_y >= half_height.Negated() && rot_y <= half_height)
        {
            return true;
        }
    }
    return false;
}

MATCH_FUNC(0x5A0970)
bool Sprite::CheckBBoxScanlineIntersection(Fix16 scanXMin, Fix16 scanXMax, Fix16 scanY)
{
    Fix16_Point* pBBox = mpCollisionShape->mCorners;
    if (ComputeScanlineIntersectionY(scanXMin, scanXMax, scanY, pBBox[0], pBBox[1]) ||
        ComputeScanlineIntersectionY(scanXMin, scanXMax, scanY, pBBox[1], pBBox[2]) ||
        ComputeScanlineIntersectionY(scanXMin, scanXMax, scanY, pBBox[2], pBBox[3]) ||
        ComputeScanlineIntersectionY(scanXMin, scanXMax, scanY, pBBox[3], pBBox[0]))
    {
        // 9.6f: CollisionSoundQueue::SetHorizontalSegment (inlined, using it changes the store order)
        gCollisionTarget.mType = collision_target_type::horizontal_edge_1;
        gCollisionTarget.mHsegXMin = scanXMin;
        gCollisionTarget.mHsegXMax = scanXMax;
        gCollisionTarget.mHsegY = scanY;
        gCollisionTarget.mpHitSprite = NULL;
        return true;
    }
    return false;
}

// https://decomp.me/scratch/EK1Y2
MATCH_FUNC(0x5A0A70)
bool Sprite::GetNearestHorizontalEdgeToCoordinate(Fix16 line_y, Fix16_Point& point, u8& nearest_corner_idx)
{
    // Same as GetNearestVerticalEdgeToCoordinate
    UpdateCollisionBoundsIfNeeded();
    Fix16_Point* pCorners = mpCollisionShape->mCorners;

    Fix16 diff = pCorners[0].y - line_y;
    Fix16 sign = SignAsFix16(diff.mValue);
    Fix16 least_abs;
    least_abs = Fix16::Abs(diff);

    point = pCorners[0];
    nearest_corner_idx = 0;

    diff = pCorners[1].y - line_y;
    if (SignAsFix16(diff.mValue) != sign)
    {
        return false;
    }

    diff = Fix16::Abs(diff);
    if (diff < least_abs)
    {
        least_abs = diff;
        point.x = pCorners[1].x;
        point.y = pCorners[1].y;
        nearest_corner_idx = 1;
    }
    else if (diff == least_abs)
    {
        point.x = (point.x + pCorners[1].x) / kFP16Two_7035C8;
        nearest_corner_idx = 5;
    }

    diff = pCorners[2].y - line_y;
    if (SignAsFix16(diff.mValue) != sign)
    {
        return false;
    }

    diff = Fix16::Abs(diff);
    if (diff < least_abs)
    {
        least_abs = diff;
        point.x = pCorners[2].x;
        point.y = pCorners[2].y;
        nearest_corner_idx = 2;
    }
    else if (diff == least_abs)
    {
        point.x = (point.x + pCorners[2].x) / kFP16Two_7035C8;
        nearest_corner_idx = 5;
    }

    diff = pCorners[3].y - line_y;
    if (SignAsFix16(diff.mValue) != sign)
    {
        return false;
    }

    diff = Fix16::Abs(diff);
    if (diff < least_abs)
    {
        point.x = pCorners[3].x;
        point.y = pCorners[3].y;
        nearest_corner_idx = 3;
    }
    else if (diff == least_abs)
    {
        point.x = (point.x + pCorners[3].x) / kFP16Two_7035C8;
        nearest_corner_idx = 5;
    }

    return true;
}

MATCH_FUNC(0x5A0EF0)
bool Sprite::HitTestVerticalLine(Fix16 x, Fix16 y1, Fix16 y2)
{
    Fix16_Point* pBBox = this->mpCollisionShape->mCorners;
    if (ComputeScanlineIntersectionX(x, y1, y2, pBBox[0], pBBox[1]) ||
        ComputeScanlineIntersectionX(x, y1, y2, pBBox[1], pBBox[2]) ||
        ComputeScanlineIntersectionX(x, y1, y2, pBBox[2], pBBox[3]) ||
        ComputeScanlineIntersectionX(x, y1, y2, pBBox[3], pBBox[0]))
    {
        gCollisionTarget.SetVerticalSegment(x, y1, y2);
        return true;
    }
    return false;
}

// https://decomp.me/scratch/ScgaC
MATCH_FUNC(0x5a1030)
bool Sprite::GetNearestVerticalEdgeToCoordinate(Fix16 line_x, Fix16_Point& point, u8& nearest_corner_idx)
{
    // least_abs assigned (not initialised) so it lives in a register. point set with one point copy (smaller
    // than the two field copies): the inline budget then leaves the negation in all four Abs out of line
    Fix16_Point* pCorners = mpCollisionShape->mCorners;
    UpdateCollisionBoundsIfNeeded();

    Fix16 diff = pCorners[0].x - line_x;
    Fix16 sign = SignAsFix16(diff.mValue);
    Fix16 least_abs;
    least_abs = Fix16::Abs(diff);

    point = pCorners[0];
    nearest_corner_idx = 0;

    diff = pCorners[1].x - line_x;
    if (SignAsFix16(diff.mValue) != sign)
    {
        return false;
    }

    diff = Fix16::Abs(diff);
    if (diff < least_abs)
    {
        least_abs = diff;
        point.x = pCorners[1].x;
        point.y = pCorners[1].y;
        nearest_corner_idx = 1;
    }
    else if (diff == least_abs)
    {
        point.y = (point.y + pCorners[1].y) / kFP16Two_7035C8;
        nearest_corner_idx = 5;
    }

    diff = pCorners[2].x - line_x;
    if (SignAsFix16(diff.mValue) != sign)
    {
        return false;
    }

    diff = Fix16::Abs(diff);
    if (diff < least_abs)
    {
        least_abs = diff;
        point.x = pCorners[2].x;
        point.y = pCorners[2].y;
        nearest_corner_idx = 2;
    }
    else if (diff == least_abs)
    {
        point.y = (point.y + pCorners[2].y) / kFP16Two_7035C8;
        nearest_corner_idx = 5;
    }

    diff = pCorners[3].x - line_x;
    if (SignAsFix16(diff.mValue) != sign)
    {
        return false;
    }

    diff = Fix16::Abs(diff);
    if (diff < least_abs)
    {
        point.x = pCorners[3].x;
        point.y = pCorners[3].y;
        nearest_corner_idx = 3;
    }
    else if (diff == least_abs)
    {
        point.y = (point.y + pCorners[3].y) / kFP16Two_7035C8;
        nearest_corner_idx = 5;
    }

    return true;
}

// https://decomp.me/scratch/2RoLd
MATCH_FUNC(0x5a1490)
bool Sprite::PointInsideRotatedBounds(Fix16_Point& point1, Fix16_Point& point2)
{

    Fix16_Point rotated_1;
    Fix16_Point rotated_2;
    Fix16 half_width;
    Fix16 half_height;
    mpCollisionShape->GetHalfExtents(&half_width, &half_height);

    // The plain mValue compares are cheaper than the Fix16 operators (inline budget).
    RotateAndTranslatePoint(point1.x, point1.y, NegateAng16(mRotation), mXy.x, mXy.y, rotated_1.x, rotated_1.y);
    if (rotated_1.x.mValue >= -half_width.mValue && rotated_1.x.mValue <= half_width.mValue && rotated_1.y.mValue >= -half_height.mValue && rotated_1.y.mValue <= half_height.mValue)
    {
        return true;
    }
    RotateAndTranslatePoint(point2.x, point2.y, NegateAng16(mRotation), mXy.x, mXy.y, rotated_2.x, rotated_2.y);
    if (rotated_2.x.mValue >= -half_width.mValue && rotated_2.x.mValue <= half_width.mValue && rotated_2.y.mValue >= -half_height.mValue && rotated_2.y.mValue <= half_height.mValue)
    {
        return true;
    }
    if (ComputeScanlineIntersectionX(-half_height, half_height, -half_width, rotated_1, rotated_2))
    {
        return true;
    }
    if (ComputeScanlineIntersectionX(-half_height, half_height, half_width, rotated_1, rotated_2))
    {
        return true;
    }
    if (ComputeScanlineIntersectionY(-half_width, half_width, -half_height, rotated_1, rotated_2))
    {
        return true;
    }
    return ComputeScanlineIntersectionY(-half_width, half_width, half_height, rotated_1, rotated_2) != 0;
}

MATCH_FUNC(0x5a19c0)
bool Sprite::IsStraddlingSlopeAndFlat()
{
    EnsureVisualShape();

    if (!mpVisualShape->mBboxUpToDate)
    {
        mpVisualShape->UpdateRotatedBoundingBox(mXy.x, mXy.y, mZpos, mRotation);
    }
    mpVisualShape->SetCurrentRect();
    return gMap_0x370->SlopeBordersFlatInSpriteGrid(mZpos);
}

MATCH_FUNC(0x5a1a60)
char_type Sprite::CheckSlopeTopInFootprint()
{
    EnsureVisualShape();

    SpriteShape* pShape = this->mpVisualShape;

    if (!pShape->mBboxUpToDate)
    {
        pShape->UpdateRotatedBoundingBox(this->mXy.x, this->mXy.y, this->mZpos, this->mRotation);
    }

    mpVisualShape->SetCurrentRect();

    ClampGridRectToMap();

    return gMap_0x370->CheckSlopeTopInSpriteGrid(mZpos);
}

MATCH_FUNC(0x5a1b30)
void Sprite::ResolveZOrder(Sprite* pOther)
{
    const char_type their_z = pOther->ComputeZLayer();
    ComputeZLayer();
    if (their_z < mZCol)
    {
        pOther->set_z_col(mZCol);
    }
    else
    {
        this->set_z_col(their_z);
    }
}

MATCH_FUNC(0x5a1bd0)
char_type Sprite::ComputeZLayer()
{
    if (this->mZCol == -1)
    {
        if (this->mSpriteTypeEnum == sprite_types_enum::car_2 && mCarBcPtr->IsTrainModel())
        {
            this->mZCol = (this->mZpos - kFP16One_7035C4).ToInt();
        }
        else
        {
            this->mZCol = (this->mZpos - kFP16One_7035C4).ToInt() + CheckSlopeTopInFootprint();
        }
    }
    return mZCol;
}

MATCH_FUNC(0x5a1ca0)
char_type Sprite::CheckCornerZCollisions(u32* pCount)
{
    char_type flags;

    UpdateCollisionBoundsIfNeeded();

    SpriteShape* pShape = mpCollisionShape;
    // 9.6f: SpriteShape::GetDepth for the depth (inlined, using it changes the code)
    Fix16 z_min = mZpos - (pShape->mDepth / 2);
    Fix16 z_max = mZpos + (pShape->mDepth / 2);

    if (z_max > kMaxSpriteZ)
    {
        z_max = kMaxSpriteZ;
    }

    if (gMap_0x370->CheckZCollisionAtCoord(pShape->mCorners[0].x, pShape->mCorners[0].y, z_min, z_max))
    {
        flags = 1;
        *pCount = 1;
    }
    else
    {
        flags = 0;
        *pCount = 0;
    }

    if (gMap_0x370->CheckZCollisionAtCoord(pShape->mCorners[1].x, pShape->mCorners[1].y, z_min, z_max))
    {
        ++*pCount;
        flags |= 2u;
    }

    if (gMap_0x370->CheckZCollisionAtCoord(pShape->mCorners[2].x, pShape->mCorners[2].y, z_min, z_max))
    {
        ++*pCount;
        flags |= 4u;
    }

    if (gMap_0x370->CheckZCollisionAtCoord(pShape->mCorners[3].x, pShape->mCorners[3].y, z_min, z_max))
    {
        ++*pCount;
        flags |= 8;
    }

    return flags;
}

MATCH_FUNC(0x5A1EB0)
bool Sprite::IsTouchingSlopeBlock()
{
    s32 zpos_int = this->mZpos.ToInt();

    UpdateCollisionBoundsIfNeeded();
    Fix16_Point* pBBox = this->mpCollisionShape->mCorners;

    if (gMap_0x370->IsGradientSlopeAt(pBBox[0].x.ToInt(), pBBox[0].y.ToInt(), zpos_int))
    {
        return true;
    }
    if (gMap_0x370->IsGradientSlopeAt(pBBox[1].x.ToInt(), pBBox[1].y.ToInt(), zpos_int))
    {
        return true;
    }
    if (gMap_0x370->IsGradientSlopeAt(pBBox[2].x.ToInt(), pBBox[2].y.ToInt(), zpos_int))
    {
        return true;
    }
    if (gMap_0x370->IsGradientSlopeAt(pBBox[3].x.ToInt(), pBBox[3].y.ToInt(), zpos_int))
    {
        return true;
    }

    if (zpos_int <= 0)
    {
        return false;
    }

    s32 zpos_m1 = zpos_int - 1;
    if (gMap_0x370->IsGradientSlopeAt(pBBox[0].x.ToInt(), pBBox[0].y.ToInt(), zpos_m1))
    {
        return true;
    }
    if (gMap_0x370->IsGradientSlopeAt(pBBox[1].x.ToInt(), pBBox[1].y.ToInt(), zpos_m1))
    {
        return true;
    }
    if (gMap_0x370->IsGradientSlopeAt(pBBox[2].x.ToInt(), pBBox[2].y.ToInt(), zpos_m1))
    {
        return true;
    }
    if (gMap_0x370->IsGradientSlopeAt(pBBox[3].x.ToInt(), pBBox[3].y.ToInt(), zpos_m1))
    {
        return true;
    }

    return false;
}

MATCH_FUNC(0x5A21F0)
bool Sprite::CheckMapZCollision()
{
    Fix16 half = this->mpCollisionShape->mDepth / 2;
    Fix16 zLow = mZpos - half;
    Fix16 zHigh = mZpos + half;
    if (zHigh > kMaxSpriteZ)
    {
        zHigh = kMaxSpriteZ;
    }
    return gMap_0x370->CheckZCollisionAtCoord(mXy.x, mXy.y, zLow, zHigh);
}

MATCH_FUNC(0x5A22B0)
Fix16 Sprite::MinDistanceToAnySpriteBBoxCorner(Sprite* pOther)
{
    Fix16 xy_pos_max = Fix16::MaxAbsDistance(mXy.x, mXy.y, pOther->mXy.x, pOther->mXy.y);

    s32 box_idx = 0;
    s32 k4Counter = 4;
    do
    {
        SpriteShape* pShape = pOther->mpCollisionShape;
        Fix16 d = Fix16::MaxAbsDistance(mXy.x, mXy.y, pShape->mCorners[box_idx].x, pShape->mCorners[box_idx].y);
        if (d < xy_pos_max)
        {
            xy_pos_max = d;
        }

        ++box_idx;
        --k4Counter;
    } while (k4Counter);

    return xy_pos_max;
}

MATCH_FUNC(0x5A2440)
char_type Sprite::CheckBulletWallCollision()
{
    UpdateCollisionBoundsIfNeeded();
    mpCollisionShape->SetCurrentRect();
    char_type result = gMap_0x370->IsMovementRegionBlocked(mXy.x.ToInt(),
                                                                              mXy.y.ToInt(),
                                                                              mZpos.ToInt(),
                                                                              this,
                                                                              2048);
    if (result)
    {
        gCollisionTarget.SetMapZ(mZpos);
    }
    return result;
}

MATCH_FUNC(0x5a2500)
char_type Sprite::CheckSpriteMovementRegion()
{
    if (mZpos >= kFpSeven_7035DC)
    {
        if (mXy.x < kFP16One_7035C4)
        {
            gCollisionTarget.SetVerticalSegment(mXy.y.GetFloorValue(), (mXy.y.GetFloorValue()) + kFP16One_7035C4, kFP16One_7035C4);
            gCollisionTarget.SetMapZ(mZpos);
            return 1;
        }

        if (mXy.x > kFp255_7033B4)
        {
            gCollisionTarget.SetVerticalSegment(mXy.y.GetFloorValue(), (mXy.y.GetFloorValue()) + kFP16One_7035C4, kFp255_7033B4);
            gCollisionTarget.SetMapZ(mZpos);
            return 1;
        }

        if (mXy.y < kFP16One_7035C4)
        {
            gCollisionTarget.SetHorizontalSegment(mXy.x.GetFloorValue(), (mXy.x.GetFloorValue()) + kFP16One_7035C4, kFP16One_7035C4);
            gCollisionTarget.SetMapZ(mZpos);
            return 1;
        }

        if (mXy.y > kFp255_7033B4)
        {
            gCollisionTarget.SetHorizontalSegment(mXy.x.GetFloorValue(), (mXy.x.GetFloorValue()) + kFP16One_7035C4, kFp255_7033B4);
            gCollisionTarget.SetMapZ(mZpos);
            return 1;
        }
    }

    UpdateCollisionBoundsIfNeeded();
    mpCollisionShape->SetCurrentRect();

    Object* pObject = AsObject2C();
    s16 region_size;
    if (pObject)
    {
        region_size = pObject->GetMovementRegionSize();
    }
    else
    {
        region_size = 1024;
    }

    char_type result = gMap_0x370->IsMovementRegionBlocked(mXy.x.ToInt(),
                                                                              mXy.y.ToInt(),
                                                                              mZpos.ToInt(),
                                                                              this,
                                                                              region_size);
    if (result)
    {
        gCollisionTarget.SetMapZ(mZpos);
    }
    return result;
}

MATCH_FUNC(0x5A26E0)
Ang16 Sprite::GetNegatedAngle()
{
    return -mRotation;
}

// https://decomp.me/scratch/tc76b
MATCH_FUNC(0x5a2710)
Fix16_Point Sprite::FindCollisionIntersectionPoint(Sprite* pOther,
                                                   Fix16_Point& newPos,
                                                   Ang16 newAng,
                                                   u8& self_corner_idx,
                                                   u8& other_corner_idx,
                                                   u8& overlap_count)
{
    u8 idx1;
    u8 idx2;
    Fix16_Point result;

    Fix16_Point SpritePos = get_x_y_443580();
    Ang16 SpriteAng = mRotation;

    self_corner_idx = 5;
    other_corner_idx = 5;

    set_xyz_lazy(newPos.x, newPos.y, mZpos);
    set_ang_lazy(newAng);
    UpdateCollisionBoundsIfNeeded();

    overlap_count = pOther->FindOverlappingBoundingBoxCorners(this, &idx1, &idx2);

    if (overlap_count != 0)
    {
        set_xyz_lazy(SpritePos.x, SpritePos.y, mZpos);
        set_ang_lazy(SpriteAng);
        UpdateCollisionBoundsIfNeeded();
        if (overlap_count == 1)
        {
            result = GetBoundingBoxCorner(idx1);
            self_corner_idx = idx1;
        }
        else
        {
            result = (GetBoundingBoxCorner(idx1) + GetBoundingBoxCorner(idx2)) / 2;
        }
    }
    else
    {
        overlap_count = FindOverlappingBoundingBoxCorners(pOther, &idx1, &idx2);
        set_xyz_lazy(SpritePos.x, SpritePos.y, mZpos);
        set_ang_lazy(SpriteAng);

        if (overlap_count != 0)
        {
            if (overlap_count == 1)
            {
                result = pOther->GetBoundingBoxCorner(idx1);
                other_corner_idx = idx1;
            }
            else
            {
                result = (pOther->GetBoundingBoxCorner(idx1) + pOther->GetBoundingBoxCorner(idx2)) / 2;
            }
        }
        else
        {
            result = (pOther->get_x_y_443580() + SpritePos) / 2;
        }
    }
    return result;
}

MATCH_FUNC(0x5a29d0)
void Sprite::CreateSoundObj()
{
    if (!mSound)
    {
        if (!bSkip_audio)
        {
            mSound = gRoot_sound.CreateSoundObject(this, SoundObjectTypeEnum::Sprite_1);
        }
    }
}

MATCH_FUNC(0x5a2a00)
void Sprite::FreeSound()
{
    if (mSound)
    {
        gRoot_sound.DestroySoundObj(mSound);
        mSound = NULL;
    }
}

MATCH_FUNC(0x5a2a30)
void Sprite::ResolveCollisionWithCarPedOrObject()
{
    SpriteList collisions;
    collisions.mP18 = NULL;
    gSpriteGrid_1->CollectRectCollisions(&mpCollisionShape->mBoundingBox, false, sprite_types_enum::none_0, this, &collisions);
    for (AttachedSprite* pCollisionIter = collisions.mP18; pCollisionIter; pCollisionIter = pCollisionIter->mpNext)
    {
        Sprite* pCurrent = pCollisionIter->mpSprite;
        if (pCurrent->GetType() == sprite_types_enum::car_2)
        {
            Car* pIterCar = pCurrent->AsCar();
            if (mSpriteTypeEnum != sprite_types_enum::car_2 ||
                (!pIterCar->IsCargoCarOf(mCarBcPtr) && !pIterCar->HasAttachedTrailer(mCarBcPtr)))
            {
                u8 x = mXy.x.ToInt();
                u8 y = mXy.y.ToInt();
                u8 z = mZpos.ToInt();

                // car shoving / overlap resolution ?
                if (gPathFinder->FindNearbyTileMatchingSlopeType(ROAD, &x, &y, &z, true))
                {
                    gSpriteGrid_1->RemoveFromRegionBuckets(pCurrent);
                    pIterCar->SnapCarToGreenArrow(x, y, z);
                    gSpriteGrid_1->AddToRegionBuckets(pCurrent);
                }
            }
        }
        else if (pCurrent->GetType() == sprite_types_enum::ped_3)
        {
            Char* pChar = pCurrent->AsCharB4();
            pChar->mpPed->Deallocate();
        }
        else if (pCurrent->IsObject2CType())
        {
            Object* pObj = pCurrent->AsObject2C();
            pObj->Dealloc();
        }
    }
}

MATCH_FUNC(0x5a2cf0)
void Sprite::PoolAllocate()
{
    this->mFlags = 0;
    this->mDrawLayer = 0;
    this->mCarBcPtr = NULL;
    this->mXy.x = kFpZero_7035C0;
    this->mXy.y = kFpZero_7035C0;
    this->mZpos = kFpZero_7035C0;
    this->mZCol = -1;
    this->mRotation = gAng16;
    this->mTypeSpriteIdx = 0;
    this->mRemap = 0;
    this->mPaletteType = palette_types_enum::sprites_2;
    this->mpCollisionShape = NULL;
    this->mpVisualShape = NULL;
    this->mZoom = 0;
    this->mSound = NULL;

    this->mInstanceId = gSpriteManager->mIdBase++;
    if (!gSpriteManager->mIdBase)
    {
        gSpriteManager->mIdBase = 1;
    }
}

MATCH_FUNC(0x5a3030)
void Sprite::PoolDeallocate()
{
    FreeShapes();
    this->mInstanceId = 0;
    FreeSound();
}

MATCH_FUNC(0x5a3100)
void Sprite::AttachSprite(Sprite* pSprite, Fix16 x, Fix16 y, Ang16 ang)
{
    Object* pObject;

    switch (mSpriteTypeEnum)
    {
        case sprite_types_enum::ped_3:
            mCharB4Ptr->mObj2c.PushAttachedSprite(pSprite, x, y, ang);
            break;
        case sprite_types_enum::car_2:
            mCarBcPtr->mAttachments.PushAttachedSprite(pSprite, x, y, ang);
            break;
        case sprite_types_enum::object_1:
        case sprite_types_enum::code_obj1_4:
        case sprite_types_enum::map_obj_5:
            pObject = mObject2CPtr;
            if (!pObject->mObj3c)
            {
                pObject->EnsureObject3C();
            }
            mObject2CPtr->mObj3c->mAttachedSprites.PushAttachedSprite(pSprite, x, y, ang);
            break;
        default:
            break;
    }

    switch (pSprite->mSpriteTypeEnum)
    {
        case sprite_types_enum::car_2:
            pSprite->mCarBcPtr->Deactivate();
            break;
        case sprite_types_enum::object_1:
        case sprite_types_enum::code_obj1_4:
        case sprite_types_enum::map_obj_5:
            pSprite->mObject2CPtr->OnAttachedToSprite(this);
            break;
        default:
            return;
    }
}

MATCH_FUNC(0x5a3540)
Sprite::~Sprite()
{
    FreeSound();
}

// https://decomp.me/scratch/RAdGk
MATCH_FUNC(0x5A3550)
void SpriteShape::UpdateRotatedBoundingBox(Fix16 xpos, Fix16 ypos, Fix16 zpos, Ang16 rotation)
{
    // The five points are constructed up front (EH state 4 at the start), in this order to get the
    // original's stack slots, and the corners
    // (renderingRect[i] = point + corner_i) are shared by every branch.
    Fix16_Point corner1;
    Fix16_Point corner2;
    Fix16_Point corner3;
    Fix16_Point corner0;
    Fix16_Point point;

    Fix16 width_over_2 = mWidth / 2;
    Fix16 height_over_2 = mHeight / 2;
    Fix16 depth_over_2 = mDepth / 2;

    point.SetXY(xpos, ypos);

    if (rotation == gAng16) // = 0
    {
        corner0.SetXY(-width_over_2, -height_over_2);
        corner1.SetXY(width_over_2, -height_over_2);
        corner2.SetXY(width_over_2, height_over_2);
        corner3.SetXY(-width_over_2, height_over_2);

        mCorners[0] = point + corner0;
        mCorners[1] = point + corner1;
        mCorners[2] = point + corner2;
        mCorners[3] = point + corner3;

        mBoundingBox.SetBounds(xpos - width_over_2, xpos + width_over_2, ypos - height_over_2, ypos + height_over_2);
    }
    else if (rotation == kAng90_70344C) // = 360
    {
        corner0.SetXY(-height_over_2, width_over_2);
        corner1.SetXY(-height_over_2, -width_over_2);
        corner2.SetXY(height_over_2, -width_over_2);
        corner3.SetXY(height_over_2, width_over_2);

        mCorners[0] = point + corner0;
        mCorners[1] = point + corner1;
        mCorners[2] = point + corner2;
        mCorners[3] = point + corner3;

        mBoundingBox.SetBounds(xpos - height_over_2, xpos + height_over_2, ypos - width_over_2, ypos + width_over_2);
    }
    else if (rotation == kAng180_70351E) // = 720
    {
        corner0.SetXY(width_over_2, height_over_2);
        corner1.SetXY(-width_over_2, height_over_2);
        corner2.SetXY(-width_over_2, -height_over_2);
        corner3.SetXY(width_over_2, -height_over_2);

        mCorners[0] = point + corner0;
        mCorners[1] = point + corner1;
        mCorners[2] = point + corner2;
        mCorners[3] = point + corner3;

        mBoundingBox.SetBounds(xpos - width_over_2, xpos + width_over_2, ypos - height_over_2, ypos + height_over_2);
    }
    else if (rotation == kAng270_703544) // = 1080
    {
        corner0.SetXY(height_over_2, -width_over_2);
        corner1.SetXY(height_over_2, width_over_2);
        corner2.SetXY(-height_over_2, width_over_2);
        corner3.SetXY(-height_over_2, -width_over_2);

        mCorners[0] = point + corner0;
        mCorners[1] = point + corner1;
        mCorners[2] = point + corner2;
        mCorners[3] = point + corner3;

        mBoundingBox.SetBounds(xpos - height_over_2, xpos + height_over_2, ypos - width_over_2, ypos + width_over_2);
    }
    else
    {
        corner0.SetXY(-width_over_2, -height_over_2);
        corner1.SetXY(width_over_2, -height_over_2);
        corner2.SetXY(width_over_2, height_over_2);
        corner3.SetXY(-width_over_2, height_over_2);

        corner0.RotateByAngle(rotation);
        corner1.RotateByAngle(rotation);
        corner2.RotateByAngle(rotation);
        corner3.RotateByAngle(rotation);

        mCorners[0] = point + corner0;
        mCorners[1] = point + corner1;
        mCorners[2] = point + corner2;
        mCorners[3] = point + corner3;

        // Declared, then assigned through the getters (9.6f calls them): initialised, VC6 interleaves the copies
        Fix16 left, right, top, bottom;
        left = mBoundingBox.get_left();
        right = mBoundingBox.get_right();
        top = mBoundingBox.get_top();
        bottom = mBoundingBox.get_bottom();

        FindMinMax(left,
                   right,
                   mCorners[0].x,
                   mCorners[1].x,
                   mCorners[2].x,
                   mCorners[3].x);

        FindMinMax(top,
                   bottom,
                   mCorners[0].y,
                   mCorners[1].y,
                   mCorners[2].y,
                   mCorners[3].y);

        mBoundingBox.SetBounds_5A5E30(left, right, top, bottom);
    }
    mBoundingBox.SetHiLowZ(zpos - depth_over_2, zpos + depth_over_2);
    mBboxUpToDate = true;

    // Not in 9.6f: the corners are set and rotated again, and the results are never used.
    corner0.SetXY(-width_over_2, -height_over_2);
    corner1.SetXY(width_over_2, -height_over_2);
    corner2.SetXY(width_over_2, height_over_2);
    corner3.SetXY(-width_over_2, height_over_2);

    corner0.RotateVelocity(rotation);
    corner1.RotateVelocity(rotation);
    corner2.RotateVelocity(rotation);
    corner3.RotateVelocity(rotation);
}

MATCH_FUNC(0x5A4D90)
void SpriteShape::SetCurrentRect()
{
    mBoundingBox.DoSetCurrentRect();
}

MATCH_FUNC(0x5A4DA0)
void SpriteShape::DrawCollisionBox(Fix16 zpos)
{
    // The bounding box sides go through the Fix16_Rect getters (by-value returns): they decide
    // the argument copies' stack slots and use up the inline budget like the original.
    if (mBboxUpToDate)
    {
        Fix16 x1;
        Fix16 y1;
        Fix16 x2;
        Fix16 y2;
        Fix16 x3;
        Fix16 y3;
        Fix16 x4;
        Fix16 y4;
        ProjectToScreen_5A5690(mBoundingBox.get_left(), mBoundingBox.get_top(), zpos, &x1, &y1);
        ProjectToScreen_5A5690(mBoundingBox.get_right(), mBoundingBox.get_top(), zpos, &x2, &y2);
        ProjectToScreen_5A5690(mBoundingBox.get_right(), mBoundingBox.get_bottom(), zpos, &x3, &y3);
        ProjectToScreen_5A5690(mBoundingBox.get_left(), mBoundingBox.get_bottom(), zpos, &x4, &y4);
        DrawDebugLine(static_cast<s16>(x1.ToInt()), static_cast<s16>(y1.ToInt()), static_cast<s16>(x2.ToInt()), static_cast<s16>(y2.ToInt()), gDebugColour);
        DrawDebugLine(static_cast<s16>(x2.ToInt()), static_cast<s16>(y2.ToInt()), static_cast<s16>(x3.ToInt()), static_cast<s16>(y3.ToInt()), gDebugColour);
        DrawDebugLine(static_cast<s16>(x3.ToInt()), static_cast<s16>(y3.ToInt()), static_cast<s16>(x4.ToInt()), static_cast<s16>(y4.ToInt()), gDebugColour);
        DrawDebugLine(static_cast<s16>(x4.ToInt()), static_cast<s16>(y4.ToInt()), static_cast<s16>(x1.ToInt()), static_cast<s16>(y1.ToInt()), gDebugColour);

        gDebugColour = 0xFFFF;
        ProjectToScreen_5A5690(mCorners[0].x, mCorners[0].y, zpos, &x1, &y1);
        ProjectToScreen_5A5690(mCorners[1].x, mCorners[1].y, zpos, &x2, &y2);
        ProjectToScreen_5A5690(mCorners[2].x, mCorners[2].y, zpos, &x3, &y3);
        ProjectWorldToScreen(mCorners[3].x, mCorners[3].y, zpos, &x4, &y4);
        DrawDebugLine(static_cast<s16>(x1.ToInt()), static_cast<s16>(y1.ToInt()), static_cast<s16>(x2.ToInt()), static_cast<s16>(y2.ToInt()), gDebugColour);
        DrawDebugLine(static_cast<s16>(x2.ToInt()), static_cast<s16>(y2.ToInt()), static_cast<s16>(x3.ToInt()), static_cast<s16>(y3.ToInt()), gDebugColour);
        DrawDebugLine(static_cast<s16>(x3.ToInt()), static_cast<s16>(y3.ToInt()), static_cast<s16>(x4.ToInt()), static_cast<s16>(y4.ToInt()), gDebugColour);
        DrawDebugLine(static_cast<s16>(x4.ToInt()), static_cast<s16>(y4.ToInt()), static_cast<s16>(x1.ToInt()), static_cast<s16>(y1.ToInt()), gDebugColour);
    }
}

MATCH_FUNC(0x5A5690)
EXPORT void __stdcall ProjectWorldToScreen(Fix16 x, Fix16 y, Fix16 z, Fix16* pOut1, Fix16* pOut2)
{
    z = kFP16One_7035C4 / ((kFP16Eight_7035E4 - z) + gViewCamera->mCurPos.mZ);
    *pOut1 = (((x - gViewCamera->mCurPos.mX) * gViewCamera->mScreenScale.x) * z) +
        Fix16(gViewCamera->mScreenPxCenterX);
    *pOut2 = (((y - gViewCamera->mCurPos.mY) * gViewCamera->mScreenScale.x) * z) +
        Fix16(gViewCamera->mScreenPxCenterY);
}

// World to screen pixels. DrawCollisionBox expands the inline, except for its last call, which
// is the out-of-line copy ProjectWorldToScreen. The inline budget leaves the operators out of line.
static inline void ProjectToScreen_5A5690(Fix16 x, Fix16 y, Fix16 z, Fix16* pOut1, Fix16* pOut2)
{
    Fix16 scale;
    scale = kFP16One_7035C4 / ((kFP16Eight_7035E4 - z) + gViewCamera->mCurPos.mZ);
    *pOut1 = (((x - gViewCamera->mCurPos.mX) * gViewCamera->mScreenScale.x) * scale) +
        Fix16(gViewCamera->mScreenPxCenterX);
    *pOut2 = (((y - gViewCamera->mCurPos.mY) * gViewCamera->mScreenScale.x) * scale) +
        Fix16(gViewCamera->mScreenPxCenterY);
}

DEFINE_GLOBAL_INIT(u16, gDebugColour, 0x1111, 0x626260);

MATCH_FUNC(0x5a57a0)
s32 SpriteShape::PoolAllocate()
{
    mpNext = NULL;
    mBboxUpToDate = false;
    return 0;
}

MATCH_FUNC(0x5a57b0)
SpriteShape::SpriteShape()
{
    PoolAllocate();
}

MATCH_FUNC(0x5a5840)
SpriteShape::~SpriteShape()
{
}

MATCH_FUNC(0x5a5860)
void SpriteManager_8::ResetDrawnSpriteCount()
{
    mDrawnSpriteCount = 0;
}

MATCH_FUNC(0x5a5870)
void SpriteManager_8::AllocScratchSprite()
{
    sSprite = gSpritePool->get_new_sprite();
    sSprite->AllocCollisionShape(kFpZero_7035C0, kFpZero_7035C0, kFpZero_7035C0);
}

MATCH_FUNC(0x5a58a0)
SpriteManager_8::SpriteManager_8()
{
    gSpritePool = new SpritePool();
    if (gSpritePool == NULL)
    {
        FatalError(Gta2Error::OutOfMemoryNewOperator, "C:\\Splitting\\Gta2\\Source\\sprite.cpp", 3239);
    }

    gSpriteShapePool = new SpriteShapePool();
    if (gSpriteShapePool == NULL)
    {
        FatalError(Gta2Error::OutOfMemoryNewOperator, "C:\\Splitting\\Gta2\\Source\\sprite.cpp", 3241);
    }

    gSpriteMaskCache = new SpriteMaskCache();
    if (gSpriteMaskCache == NULL)
    {
        FatalError(Gta2Error::OutOfMemoryNewOperator, "C:\\Splitting\\Gta2\\Source\\sprite.cpp", 3243);
    }

    gAttachedSpritePool = new AttachedSpritePool();
    if (gAttachedSpritePool == NULL)
    {
        FatalError(Gta2Error::OutOfMemoryNewOperator, "C:\\Splitting\\Gta2\\Source\\sprite.cpp", 3246);
    }

    mDrawnSpriteCount = 0;
    mIdBase = 1;
}

MATCH_FUNC(0x5A5AA0)
EXPORT void __stdcall ProjectOntoAxis(Fix16& xpos1, Fix16& ypos1, Ang16& angle, Fix16& xpos2, Fix16& ypos2, Fix16& outX, Fix16& outY)
{
    outX = (Ang16::cosine(angle) * (xpos1 - xpos2)) + (Ang16::sine(angle) * (ypos1 - ypos2));
    outY = (Ang16::sine(angle) * (xpos2 - xpos1)) + (Ang16::cosine(angle) * (ypos1 - ypos2));
}

MATCH_FUNC(0x5a5b50)
SpriteManager_8::~SpriteManager_8()
{
    GTA2_DELETE_AND_NULL(gSpritePool);
    GTA2_DELETE_AND_NULL(gSpriteShapePool);
    GTA2_DELETE_AND_NULL(gSpriteMaskCache);
    GTA2_DELETE_AND_NULL(gAttachedSpritePool);
    sSprite = NULL;
}

MATCH_FUNC(0x5a5be0)
SpriteShapePool::~SpriteShapePool()
{
    mPool.mpHead = NULL;
}

MATCH_FUNC(0x5A5C00)
SpritePool::~SpritePool()
{
    mPool.mpHead = NULL;
}

MATCH_FUNC(0x5a5c20)
AttachedSpritePool::~AttachedSpritePool()
{
    mPool.mpHead = NULL;
}

MATCH_FUNC(0x5a5c40)
AttachedSprite::~AttachedSprite()
{
}

MATCH_FUNC(0x5a5c50)
AttachedSprite::AttachedSprite()
{
    mOffsetRot = 0;
}

MATCH_FUNC(0x5a5e50)
Sprite::Sprite() : mRotation(gAng16)
{
    mpVisualShape = NULL;
    mXy.x = kFpZero_7035C0;
    mXy.y = kFpZero_7035C0;
    mZpos = kFpZero_7035C0;
    mInstanceId = 0;
    mTypeSpriteIdx = 0;
    mRemap = 0;
    mDrawLayer = 0;
    mFlags = 0;
    mSpriteTypeEnum = sprite_types_enum::none_0;
    mPaletteType = 0;
    mZoom = 0;
    mZCol = -1;
    mCarBcPtr = NULL;
    mpNext = NULL;
    mSound = NULL;
}

MATCH_FUNC(0x5A6910)
bool AttachedSprite::UpdateAttached(Sprite* pSprite)
{
    bool bRet;
    Object* pObject = mpSprite->AsObject2C();
    if (pObject)
    {
        bRet = pObject->UpdateMovementAndEffects(pSprite, mOffsetX, mOffsetY, mOffsetRot);
        if (bRet)
        {
            gObject_2C_Pool->DeAllocate(pObject);
            return bRet;
        }
    }
    else
    {
        Car* pCar = mpSprite->AsCar();
        if (pCar)
        {
            bRet = pCar->UpdateAttachedToSprite(pSprite, mOffsetX, mOffsetY, mOffsetRot);
            if (bRet)
            {
                gCar_BC_Pool->Remove(pCar);
            }
        }
        else
        {
            Char* pChar = mpSprite->AsCharB4();
            bRet = false;
            pChar->Update(kFP16Zero_703A38);
        }
    }
    return bRet;
}

MATCH_FUNC(0x5A69E0)
void AttachedSprite::DestroyAttached()
{
    Object* pObject = mpSprite->AsObject2C();
    if (pObject)
    {
        pObject->OnAttachedSpriteDestroyed();
    }
    else
    {
        Car* pCar = mpSprite->AsCar();
        if (pCar)
        {
            pCar->TriggerExplosion(explosion_type::item_19);
        }
    }
}

MATCH_FUNC(0x5A6A20)
void AttachedSprite::SinkAttachedCar()
{
    Car* pCar = this->mpSprite->AsCar();
    if (pCar)
    {
        if (pCar->mDespawnStatus != car_despawn_status::despawn_pending_2 && pCar->mDespawnStatus != car_despawn_status::marked_for_despawn_4 && pCar->mDespawnStatus != car_despawn_status::despawn_soon_3)
        {
            pCar->DestroyWhileSinking();
        }
    }
}
