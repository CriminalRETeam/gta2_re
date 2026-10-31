#pragma once

#include "Fix16_Rect.hpp"
#include "Function.hpp"
#include "Pool.hpp"
#include "ang16.hpp"
#include "enums.hpp"
#include "sprite_flags.hpp"
#include "fix16.hpp"
#include "gbh_graphics.hpp"

EXTERN_GLOBAL(Ang16, gAng16);
EXTERN_GLOBAL(Fix16, gFix16_7035C0);

EXTERN_GLOBAL(u32, gLightingDrawFlag);
EXTERN_GLOBAL_ARRAY(Vert, gTileVerts_7036D0, 8);

EXPORT bool __stdcall ComputeScanlineIntersectionX(Fix16& minX, Fix16& minY, Fix16& scanLineX, Fix16_Point& p0, Fix16_Point& p1);
EXPORT bool __stdcall ComputeScanlineIntersectionY(Fix16& minX, Fix16& minY, Fix16& scanLineY, Fix16_Point& p0, Fix16_Point& p1);

class Car;
class Char;
class Object;
class SoundObject;
class Ped;

EXPORT void __stdcall ProjectWorldToScreen(Fix16 x, Fix16 y, Fix16 z, Fix16* pOut1, Fix16* pOut2);

class SpriteShape
{
  public:
    EXPORT s32 PoolAllocate();

    void PoolDeallocate()
    {
    }

    // While the shape is out of the pool, mpNext holds the id of the last collision query that
    // tested it, so a query doesn't test a shape twice
    // 9.6f 0x446920
    void SetCollisionId_446920(u32 id)
    {
        this->mpNext = reinterpret_cast<SpriteShape*>(id);
    }

    bool CollisionIdIs_446930(u32 id) const
    {
        return reinterpret_cast<u32>(mpNext) == id;
    }

    // 9.6f 0x41E390
    // https://decomp.me/scratch/YTMyx
    bool IsZeroWidth_41E390() const
    {
        return (mWidth == mHeight && mWidth <= kSmallWidthEpslion) ? true : false;
    }

    // 9.6f 0x482980
    void SetDimensions_482980(Fix16 w, Fix16 h, Fix16 depth)
    {
        mWidth = w;
        mHeight = h;
        mDepth = depth;
    }

    // 9.6f 0x4BA070
    void SetWidthHeight_4BA070(Fix16 w, Fix16 h)
    {
        mWidth = w;
        mHeight = h;

        mDepth = Fix16(0);
    }

    // 9.6f 0x4BA030
    void SetWidthHeightLazy_4BA030(Fix16 w, Fix16 h)
    {
        // Avoid BB-box recompute if we can
        if (w != mWidth || h != mHeight)
        {
            mWidth = w;
            mHeight = h;
            mBboxUpToDate = false;
        }
    }

    // 9.6f 0x4BA0F0
    void GetDimensions_4BA0F0(Fix16* pOut, Fix16* pOutH, Fix16* pOutDepth) const
    {
        *pOut = this->mWidth;
        *pOutH = this->mHeight;
        *pOutDepth = this->mDepth;
    }

    // 9.6f 0x420590
    Fix16 GetWidth_420590() const
    {
        return mWidth;
    }

    // 9.6f 0x447E10
    Fix16 GetHeight_447E10() const
    {
        return mHeight;
    }

    // 9.6f 0x492170
    Fix16 GetDepth_492170() const
    {
        return mDepth;
    }

    // 9.6f 0x4BA120
    bool ReduceWidthBy_4BA120(Fix16 amount)
    {
        mWidth -= amount;
        if (mWidth <= gFix16_7035C0)
        {
            mWidth = gFix16_7035C0;
        }
        mBboxUpToDate = false;
        return mWidth == gFix16_7035C0 ? true : false;
    }

    // 9.6f 0x4BA110
    void ReduceHeightNoClamp_4BA110(Fix16 amount)
    {
        mHeight -= amount;
    }

    // 9.6f 0x447DF0
    void CopyDimensions_447DF0(const SpriteShape* pOther)
    {
        this->mWidth = pOther->mWidth;
        this->mHeight = pOther->mHeight;
        this->mDepth = pOther->mDepth;
    }

    // 9.6f 0x4BA160
    bool ReduceHeightBy_4BA160(Fix16 amount)
    {
        mHeight -= amount;
        if (mHeight <= gFix16_7035C0)
        {
            mHeight = gFix16_7035C0;
        }
        mBboxUpToDate = false;
        return mHeight == gFix16_7035C0 ? true : false;
    }

    void GetHalfExtents_4BA0A0(Fix16* pHalfW, Fix16* pHalfH)
    {
        *pHalfW = (mWidth / 2);
        *pHalfH = (mHeight / 2);
    }

    // 9.6f 0x4BA1A0
    void ScaleWidthHeight_4BA1A0(Fix16 scale)
    {
        mWidth *= scale;
        if (mWidth <= gFix16_7035C0)
        {
            mWidth = gFix16_7035C0;
        }

        mHeight *= scale;
        if (mHeight <= gFix16_7035C0)
        {
            mHeight = gFix16_7035C0;
        }
        mBboxUpToDate = false;
    }

    EXPORT void SetCurrentRect();
    EXPORT void UpdateRotatedBoundingBox(Fix16 x, Fix16 y, Fix16 z, Ang16 ang);
    EXPORT void DrawCollisionBox(Fix16 zpos);

    EXPORT SpriteShape();
    EXPORT ~SpriteShape();

    Fix16 mWidth; // 0x0
    Fix16 mHeight; // 0x4
    Fix16 mDepth; // 0x8
    Fix16_Point mCorners[4]; // 0xC
    SpriteShape* mpNext;
    Fix16_Rect mBoundingBox; // 0x30
    bool mBboxUpToDate; // 0x48
    char_type mUnused49; // 0x49
    char_type mUnused4A; // 0x4A
    char_type mUnused4B; // 0x4B
};
GTA2_ASSERT_SIZEOF_ALWAYS(SpriteShape, 0x4C);

class Sprite
{
  public:
    // 9.6f 0x41C1F0
    inline s16 get_remap_41C1F0() const
    {
        return mRemap;
    }

    // 9.6f 0x4BA220
    inline void set_z_col_4BA220(char_type v)
    {
        mZCol = v;
    }

    EXPORT Fix16_Point GetBoundingBoxCorner(s32 idx);

    EXPORT Fix16_Point get_x_y_443580();
    // 9.6f 0x4207B0, out-of-line copy get_x_y_443580 (emitted by Car.cpp). Returns a Fix16_Point by
    // value, so VC6 charges it to the caller's inline budget and then calls the copy.
    inline Fix16_Point get_x_y()
    {
        return Fix16_Point(mXy.x, mXy.y);
    }
    // get_x_y with its body defined at the end of Car.cpp (Car::HandleCarHitByObject)
    inline Fix16_Point get_x_y_late_443580();
    EXPORT void set_xyz_lazy_451950(Fix16 xpos, Fix16 ypos, Fix16 zpos);
    EXPORT void setxy_lazy(Fix16 xpos, Fix16 ypos);
    EXPORT bool IsControlledByRemotePlayer();
    EXPORT Ped* GetPed() const;
    EXPORT s32 IsOnWater();
    EXPORT u8 GetWaterCornerMask();
    EXPORT void CopyCollisionShapeToVisual();
    EXPORT void CopyVisualShapeToCollision();
    EXPORT void IncreaseZoom(char_type delta);
    EXPORT bool ShrinkSprite(Fix16 xoff, Fix16 yoff, s32 bShrinkCollisionShape);
    EXPORT void ApplyScaleToDimensions(Fix16 scale, s32 bScaleCollisionShape);
    EXPORT bool CollisionCheck_59E590(Sprite* pOther);
    EXPORT bool CheckDirectionalSliceCollision(Fix16 sliceLen, Sprite* pSprite);
    EXPORT void InvalidateZColAndBBoxes();
    EXPORT Sprite* QuerySpriteCollision(s32 sprite_type);
    EXPORT bool IsThreatToSearchingPed();
    EXPORT char_type ShouldCollideWithSprite(Sprite* pSprite);
    EXPORT void HandleObjectCollision(Sprite* pSprite);
    EXPORT void ProcessCarToCarImpactIfCar(Sprite* pSprite);
    EXPORT void SetDefaultDrawLayerBySpriteType();
    EXPORT void UpdateCollisionBoundsIfNeeded();
    EXPORT void SetRemap(s16 remap);
    EXPORT s16 GetTruePalette() const;
    EXPORT char_type has_shadows_59EAE0();
    EXPORT void ShowId(f32& x, f32& y);
    EXPORT void ShowHorn(f32& x, f32& y);
    EXPORT void Draw();
    EXPORT void AllocCollisionShape(Fix16 width, Fix16 height, Fix16 depth);
    EXPORT void EnsureVisualShape();
    EXPORT void UpdateDimensionsFromSpriteIndex();
    EXPORT void FreeShapes();
    EXPORT bool IntersectsRectSAT(Fix16_Rect* pOtherRect);
    EXPORT char_type FindOverlappingBoundingBoxCorners(Sprite* pOther, u8* pOut1, u8* pOut2);
    EXPORT char_type CollisionCheck_5A0320(Fix16* pXY1, Fix16* pXY2, u8* pCollisionIdx1, u8* pCollisionIdx2);
    EXPORT bool RotatedRectCollisionSAT(Sprite* pOther);
    EXPORT bool CheckBBoxScanlineIntersection(Fix16 scanXMin, Fix16 scanXMax, Fix16 scanY);
    EXPORT bool GetNearestHorizontalEdgeToCoordinate(Fix16 line_y, Fix16_Point& point, u8& nearest_corner_idx);
    EXPORT bool HitTestVerticalLine(Fix16 x, Fix16 y1, Fix16 y2);
    EXPORT bool GetNearestVerticalEdgeToCoordinate(Fix16 line_x, Fix16_Point& point, u8& nearest_corner_idx);
    EXPORT bool PointInsideRotatedBounds(Fix16_Point& point1, Fix16_Point& point2);
    EXPORT bool IsStraddlingSlopeAndFlat();
    EXPORT char_type CheckSlopeTopInFootprint();
    EXPORT void ResolveZOrder(Sprite* pOther);
    EXPORT char_type ComputeZLayer();
    EXPORT char_type CheckCornerZCollisions(u32* pCount);
    EXPORT bool IsTouchingSlopeBlock();
    EXPORT bool CheckMapZCollision();
    EXPORT Fix16 MinDistanceToAnySpriteBBoxCorner(Sprite* pOther);
    EXPORT char_type CheckBulletWallCollision();
    EXPORT char_type CheckSpriteMovementRegion();
    EXPORT Ang16 GetNegatedAngle();
    EXPORT Fix16_Point FindCollisionIntersectionPoint(Sprite* pOther,
                                                              Fix16_Point& newPos,
                                                              Ang16 newAng,
                                                              u8& self_corner_idx,
                                                              u8& other_corner_idx,
                                                              u8& overlap_count);
    EXPORT void CreateSoundObj();
    EXPORT bool IsObjectModelEqual(s32 model) const;
    EXPORT void FreeSound();
    EXPORT void ResolveCollisionWithCarPedOrObject();
    EXPORT void PoolAllocate();
    EXPORT void PoolDeallocate();
    EXPORT void AttachSprite(Sprite* pSprite, Fix16 x, Fix16 y, Ang16 ang);

    EXPORT void set_angle(Ang16 ang);

    EXPORT ~Sprite(); // 0x5a3540

    EXPORT Sprite();

    // The original ctor is an inline that reads a per-TU static zero Ang16 / Fix16 (car.cpp has its own
    // copies at 0x67791C / 0x6777CC). This inline takes them as parameters for the TUs that inline it.
    inline Sprite(const Ang16& kZeroAng, const Fix16& kZero) : mRotation(kZeroAng)
    {
        mpVisualShape = NULL;
        mXy.x = kZero;
        mXy.y = kZero;
        mZpos = kZero;
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

    // 9.6f 0x446940
    inline bool TypeIs_446940(s32 sprite_type) const
    {
        return mSpriteTypeEnum == sprite_type;
    }

    // 9.6f 0x420740
    Fix16 GetWidth_420740() const
    {
        return mpCollisionShape->GetWidth_420590();
    }

    // 9.6f 0x447E70
    Fix16 GetHeight_447E70() const
    {
        return mpCollisionShape->GetHeight_447E10();
    }

    // 9.6f 0x4117B0
    void GetPosition_4117B0(Fix16* pOutX, Fix16* pOutY, Fix16* pOutZ) const
    {
        *pOutX = this->mXy.x;
        *pOutY = this->mXy.y;
        *pOutZ = this->mZpos;
    }

    // 9.6f 0x420700
    inline void SetPaletteSprites_420700()
    {
        mPaletteType = palette_types_enum::sprites_2;
    }

    // 9.6f 0x416B40
    s32 GetType_416B40() const
    {
        return mSpriteTypeEnum;
    }

    // 9.6f 0x40FEB0
    Car* AsCar_40FEB0() const
    {
        if (mSpriteTypeEnum == sprite_types_enum::car_2)
        {
            return mCarBcPtr;
        }
        else
        {
            return NULL;
        }
    }

    // 9.6f 0x40FEA0
    Char* AsCharB4_40FEA0() const
    {
        if (mSpriteTypeEnum == sprite_types_enum::ped_3)
        {
            return mCharB4Ptr;
        }
        else
        {
            return NULL;
        }
    }

    // 9.6f 0x4BA230, defined in sprite.cpp
    inline u16 GetTrueSpriteIdx_4BA230();

    // 9.6f 0x446960
    inline Fix16 ManhattanDistance_446960(const Sprite* pOther) const
    {
        return Fix16::Abs(pOther->mXy.x - mXy.x) + Fix16::Abs(pOther->mXy.y - mXy.y);
    }

    // 9.6f 0x446950
    bool IsTypeAbove1_446950() const
    {
        return mSpriteTypeEnum > sprite_types_enum::object_1;
    }

    // 9.6f 0x40FE80
    bool IsObject2CType_40FE80() const
    {
        return mSpriteTypeEnum == sprite_types_enum::code_obj1_4 || mSpriteTypeEnum == sprite_types_enum::map_obj_5 || mSpriteTypeEnum == sprite_types_enum::object_1;
    }

    // 9.6f 0x40FEC0
    Object* AsObject2C_40FEC0() const
    {
        if (IsObject2CType_40FE80())
        {
            return mObject2CPtr;
        }
        else
        {
            return NULL;
        }
    }

    // 9.6f 0x4BA200
    inline s32 GetBlendMode_4BA200() const
    {
        return mFlags & 3;
    }

    // 9.6f 0x4BA210
    inline u32 GetAlphaBits_4BA210() const
    {
        return mFlags & 0xFFFFFFF8;
    }

    // 9.6f 0x4B9BA0
    inline s32 GetLightingDrawFlag_4B9BA0() const
    {
        if ((mFlags & 4) == 0)
        {
            return gLightingDrawFlag;
        }
        return 0;
    }

    // matched on 9.6f but slight different on 10.5: https://decomp.me/scratch/iNjwT
    // 9.6f 0x4BAC60
    inline u32 ComputeDrawFlags_4BAC60() const
    {
        u32 flags;
        switch (GetBlendMode_4BA200())
        {
            case 0:
                return GetLightingDrawFlag_4B9BA0() | 0x80;
            case 1:
                flags = GetAlphaBits_4BA210();
                gTileVerts_7036D0[0].diff = (flags << 24) | 0xFFFFFF;
                gTileVerts_7036D0[1].diff = (flags << 24) | 0xFFFFFF;
                gTileVerts_7036D0[2].diff = (flags << 24) | 0xFFFFFF;
                gTileVerts_7036D0[3].diff = (flags << 24) | 0xFFFFFF;
                return GetLightingDrawFlag_4B9BA0() | 0x2180;
            case 2:
                flags = GetAlphaBits_4BA210();
                gTileVerts_7036D0[0].diff = (flags << 24) | 0xFFFFFF;
                gTileVerts_7036D0[1].diff = (flags << 24) | 0xFFFFFF;
                gTileVerts_7036D0[2].diff = (flags << 24) | 0xFFFFFF;
                gTileVerts_7036D0[3].diff = (flags << 24) | 0xFFFFFF;
                return GetLightingDrawFlag_4B9BA0() | 0x2280;
            default:
                return 0;
        }
    }

    Ang16 mRotation; // 0x0
    SpriteShape* mpVisualShape; // 0x4
    // Only one field is enable at the same time.
    // The field that is enabled is decided by mSpriteTypeEnum
    // The following list may not be complete.
    // At this moment, it comes from the functions(version 9.6f) at 4b9a10, 4b9a30, 4b9d50
    union
    {
        Car* mCarBcPtr; // 0x8: mSpriteTypeEnum == sprite_types_enum::car
        Char* mCharB4Ptr; // 0x8: mSpriteTypeEnum == sprite_types_enum::ped
        Object* mObject2CPtr; // 0x8: mSpriteTypeEnum == sprite_types_enum::map_obj or code_obj1 or object_1
        Sprite* mpSprite; // 0x8
        class Object_5C* mO5C; // 0x8
    };
    // Only one field is enable at the same time.
    // As far as I understand, the mpNext is only enabled while the instance is being handled by SpritePool.
    // Otherwise, the mpCollisionShape is active. i.e. the instance belongs to another object.
    union
    {
        Sprite* mpNext;
        SpriteShape* mpCollisionShape; // 0xC
        class Object_5C* mO5c; // 0xC
    };
    SoundObject* mSound; // 0x10

    // 9.6f 0x4206F0
    void SetType_4206F0(s32 sprite_type)
    {
        mSpriteTypeEnum = sprite_type;
        SetDefaultDrawLayerBySpriteType(); // Update field_28
    }

    // 9.6f 0x482A30
    void SetObject2C_482A30(Object* pObject)
    {
        this->mObject2CPtr = pObject;
    }

    // 9.6f 0x420710
    void SetCar_420710(Car* pCar)
    {
        this->mCarBcPtr = pCar;
    }

    // 9.6f 0x492180
    void SetChar_492180(Char* pChar)
    {
        this->mCharB4Ptr = pChar;
    }

    // 9.6f 0x4206C0
    void set_id_lazy_4206C0(s16 new_id)
    {
        if (mTypeSpriteIdx != new_id)
        {
            mTypeSpriteIdx = new_id;
            UpdateDimensionsFromSpriteIndex();
        }
    }

    // 9.6f 0x4206E0
    void set_id_4206E0(u16 new_id)
    {
        this->mTypeSpriteIdx = new_id;
    }

    // 9.6f 0x4337F0
    void SetNoLighting_4337F0()
    {
        this->mFlags |= 4u;
    }

    // 9.6f 0x482A40
    void SetDimensions_482A40(Fix16 w, Fix16 h, Fix16 depth)
    {
        mpCollisionShape->SetDimensions_482980(w, h, depth);
        InvalidateZColAndBBoxes();
    }

    // 9.6f 0x433800
    void ClearNoLighting_433800()
    {
        this->mFlags &= ~4u;
    }

    // 9.6f 0x4337D0
    void SetBlendAndAlpha_4337D0(u8 low_bits, u8 high_bits)
    {
        this->mFlags = (high_bits << 3) | low_bits;
    }

    // 9.6f inline 0x420690
    inline void set_ang_lazy_420690(Ang16 ang)
    {
        if (ang != mRotation)
        {
            mRotation = ang;
            InvalidateZColAndBBoxes();
        }
    }

    // 9.6f inline 0x420600
    inline void set_xyz_lazy_420600(Fix16 xpos, Fix16 ypos, Fix16 zpos)
    {
        if (mXy.x != xpos || mXy.y != ypos || mZpos != zpos)
        {
            mXy.x = xpos;
            mXy.y = ypos;
            mZpos = zpos;
            InvalidateZColAndBBoxes();
        }
    }

    // 9.6f inline 0x420600
    // OBS: INLINE_MODE required to match Crane::UpdateCraneSprites
    INLINE_MODE void set_xyz_lazy_inlined_420600(Fix16 xpos, Fix16 ypos, Fix16 zpos)
    {
        if (mXy.x != xpos || mXy.y != ypos || mZpos != zpos)
        {
            mXy.x = xpos;
            mXy.y = ypos;
            mZpos = zpos;
            InvalidateZColAndBBoxes();
        }
    }

    // 9.6f inline 0x447E20
    void set_xy_lazy_447E20(Fix16 x_target, Fix16 y_target)
    {
        if (mXy.x != x_target || mXy.y != y_target)
        {
            mXy.x = x_target;
            mXy.y = y_target;
            InvalidateZColAndBBoxes();
        }
    }

    // 9.6f inline 0x420660
    void set_z_lazy_420660(Fix16 zpos)
    {
        if (mZpos != zpos)
        {
            mZpos = zpos;
            InvalidateZColAndBBoxes();
        }
    }

    inline Fix16 GetXPos() const
    {
        return mXy.x;
    }

    inline Fix16 GetYPos() const
    {
        return mXy.y;
    }

    inline Fix16 GetZPos() const
    {
        return mZpos;
    }

    // 9.6f 0x40F7B0
    inline void SetDrawLayer_40F7B0(s32 new_layer)
    {
        mDrawLayer = new_layer;
    }

    Fix16_Point_POD mXy; // 0x14
    Fix16 mZpos; // 0x1C
    s16 mInstanceId; // 0x20: unique per live sprite (SpriteManager id_base counter)
    s16 mTypeSpriteIdx; // 0x22: sprite number within its sprite type, looked up with GetSpriteTrueIndex
    s16 mRemap; // 0x24
    s32 mDrawLayer; // 0x28: draw order tie-break among sprites at the same z (SpriteTree::AddSprite); > 9 sorts by z
    union
    {
        u8 mFlags; // 0x2C
        SpriteFlags8 mBf2C; // 0x2C
    };
    s32 mSpriteTypeEnum; // 0x30: Uses the enum defined in the namespace sprite_types_enum
    s32 mPaletteType; // 0x34: remap type?
    char_type mZoom; // 0x38
    char_type mZCol; // 0x39
    char_type mUnused3A; // 0x3A
    char_type mUnused3B; // 0x3B
};

class SpriteMask
{
  public:
    EXPORT void MarkUsed();

    EXPORT void ClearMask(u8 xCount, u8 yCount);

    // 9.6f 0x44af30
    EXPORT SpriteMask()
    {
        mLastUsed = 0;
        mDeltaMask = 0;
        mSpriteIdx = -1;
        mPalette = 0;
    }

    // 9.6f 0x44AF70
    void Invalidate_44AF70()
    {
        mSpriteIdx = -1;
    }

    // 9.6f 0x44AF50
    void SetKey_44AF50(u16 sprite_idx, u32 delta_mask, u16 palette)
    {
        mSpriteIdx = sprite_idx;
        mDeltaMask = delta_mask;
        mPalette = palette;
    }

    u8* mPixels; // 0x0
    s16 mSpriteIdx; // 0x4
    char_type mUnused6; // 0x6
    char_type mUnused7; // 0x7
    s32 mDeltaMask; // 0x8
    s32 mLastUsed; // 0xC
    u16 mMaskIdx; // 0x10
    u16 mPalette; // 0x12
};
GTA2_ASSERT_SIZEOF_ALWAYS(SpriteMask, 0x14);

class SpriteMaskCache
{
  public:
    EXPORT SpriteMask* FindCachedMask(const u16& sprite_idx, const u32* pFlag, const u32* pDeltaMask, const u16* pExtra);
    EXPORT SpriteMask* FindLeastRecentlyUsed(const u32* pKey);
    EXPORT void InvalidateMasksBySpriteIdx(const u16* sprite_idx);
    EXPORT void InvalidateAllMasks();
    EXPORT SpriteMaskCache();
    EXPORT ~SpriteMaskCache();

    u8* GetMaskPixels(u16 idx) const
    {
        return mMasks[idx].mPixels;
    }

    SpriteMask mMasks[48]; // 0x0
    s32 mUseCounter; // 0x3C0
    u8* mAlignedBuffer; // 0x3C4
    s32* mUnalignedAlloc; // 0x3C8
};
GTA2_ASSERT_SIZEOF_ALWAYS(SpriteMaskCache, 0x3CC);

class SpriteManager_8
{
  public:
    EXPORT void ResetDrawnSpriteCount();
    EXPORT void AllocScratchSprite();
    EXPORT SpriteManager_8();
    EXPORT ~SpriteManager_8();
    s32 mDrawnSpriteCount; // 0x0
    s16 mIdBase; // 0x4
    s16 mUnused6; // 0x6
};

class AttachedSprite
{
  public:
    EXPORT ~AttachedSprite();
    EXPORT AttachedSprite();

    void PoolAllocate()
    {
    }

    void PoolDeallocate()
    {
    }

    EXPORT void SinkAttachedCar();

    EXPORT void DestroyAttached();

    EXPORT bool UpdateAttached(Sprite* pSprite);

    Sprite* mpSprite; // 0x0
    AttachedSprite* mpNext;
    Fix16 mOffsetX; // 0x8: NOTE: Fix16_Point here breaks match of UpdateAttached
    Fix16 mOffsetY; // 0xC
    Ang16 mOffsetRot; // 0x10
    s16 mUnused12; // 0x12
    s32 mRng; // 0x14
};
GTA2_ASSERT_SIZEOF_ALWAYS(AttachedSprite, 0x18);

class AttachedSpritePool
{
  public:
    // Inlined, from 9.6f at 0x4bdcf0
    EXPORT AttachedSpritePool()
    {
    }

    EXPORT ~AttachedSpritePool();

    AttachedSprite* Allocate()
    {
        return mPool.Allocate();
    }

    void DeAllocate(AttachedSprite* pItem)
    {
        mPool.DeAllocate(pItem);
    }

    PoolBasic<AttachedSprite, 300> mPool; // 0x0
};

class SpriteShapePool
{
  public:
    // Inlined, from 9.6f at 0x4bc9a0
    SpriteShapePool()
    {
    }

    ~SpriteShapePool();

    // 9.6f 0x4BCA10
    void Remove(SpriteShape* pToRemove)
    {
        mPool.DeAllocate(pToRemove);
    }

    // 9.6f 0x4BC9F0
    SpriteShape* Allocate()
    {
        return mPool.Allocate();
    }

    PoolBasic<SpriteShape, 5031> mPool; // 0x0
};

class SpritePool
{
  public:
    // Inlined, from 9.6f at 0x4bca20
    SpritePool()
    {
    }

    EXPORT ~SpritePool();

    // Inlined, from 9.6f at 0x421000
    Sprite* get_new_sprite()
    {
        return mPool.Allocate();
    }

    // Inlined, from 9.6f at 0x421030
    void remove(Sprite* pSprite)
    {
        mPool.DeAllocate(pSprite);
    }

    // 9.6f 0x48A8D0
    inline bool has_free_48A8D0() const
    {
        return mPool.mpHead != NULL;
    }

    PoolBasic<Sprite, 5031> mPool; // 0x0
};
GTA2_ASSERT_SIZEOF_ALWAYS(SpritePool, 0x49B28)

EXTERN_GLOBAL(SpriteManager_8*, gSpriteManager);

EXTERN_GLOBAL(SpritePool*, gSpritePool);

EXTERN_GLOBAL(SpriteShapePool*, gSpriteShapePool);

EXTERN_GLOBAL(SpriteMaskCache*, gSpriteMaskCache);

EXTERN_GLOBAL(AttachedSpritePool*, gAttachedSpritePool);

EXTERN_GLOBAL(Sprite*, gSprite);
