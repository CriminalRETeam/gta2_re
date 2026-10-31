#pragma once

#include "Car.hpp"
#include "Function.hpp"
#include "fix16.hpp"

class Sprite;
class Car;
class Ped;

EXTERN_GLOBAL(Fix16, kFpEight_676840);

EXTERN_GLOBAL(Fix16, kZero_676818);

EXTERN_GLOBAL(Fix16, kOne_67681C);

EXTERN_GLOBAL(Fix16, kFpEight_702E04);
EXTERN_GLOBAL(Fix16, kFpOne_702DE4);

namespace camera_follow_mode
{
enum
{
    none_0 = 0,
    smooth_1 = 1, // eases towards the target (Update)
    snap_2 = 2, // copies the target straight in (debug/free camera)
};
} // namespace camera_follow_mode

struct CameraPos
{
    Fix16 mX; // 0x0
    Fix16 mY; // 0x4
    Fix16 mZ; // 0x8
    Fix16 mZoom; // 0xC
};

struct WorldRect
{
    Fix16 mLeft; // 0x0
    Fix16 mRight; // 0x4
    Fix16 mTop; // 0x8
    Fix16 mBottom; // 0xC
};

class Camera
{
  public:
    // 9.6f 0x416B70
    inline Ped* GetFollowedPed() const
    {
        return mTargetPed;
    }

    EXPORT bool IsSpriteTheCameraSubject(Sprite* pSprite) const;
    EXPORT bool IsSpriteInView(Sprite* pSprite, s32 wide_view);
    EXPORT void SavePrevCamPos();
    EXPORT void IncreaseElevation();
    EXPORT void DecreaseElevation();
    EXPORT void ResetElevation();
    EXPORT void ClampTargetZ();
    EXPORT void ApplyMovementDeltaFrom(Camera* pCamera);
    EXPORT Ang16 ComputeTargetFacingAngle();
    EXPORT Fix16 ReturnOwnerVelocity();
    EXPORT s32 IsCoordsPosVisible(Fix16 x, Fix16 y, Fix16 z);
    EXPORT void UpdateBoundaries();
    EXPORT void MoveTarget(bool move_north, bool move_south, bool move_west, bool move_east, bool zoom_in, bool zoom_out);
    EXPORT void ResetCameraSmoothing();
    EXPORT void AccumulateSuspicionOnDriver(Car* pCar);
    EXPORT void Update();
    EXPORT void RefreshBoundaries();
    EXPORT void SetShake(Fix16 amount);
    EXPORT void ApplyShake();
    EXPORT void SetScreenSize(u32 width, u32 height);
    EXPORT void ApplyCarVelocityCameraOffset(Car* pCar, Fix16* pOutX, Fix16* pOutY, Fix16* pOutZ);
    EXPORT void UpdateFollowCarCamera(Car* pCar);
    EXPORT void UpdateFollowPedCamera(Ped* pPed);
    EXPORT void HandlePanning(bool forward_gas_on, bool foot_brake_on, bool left_on, bool right_on);
    EXPORT void ResetPanning();
    EXPORT void ApplyPanOffset(Ped* pPed, Fix16& x_pos, Fix16& y_pos, Fix16 z_pos);
    EXPORT Camera();
    EXPORT ~Camera();
    EXPORT void SetTarget(Fix16 x, Fix16 y, Fix16 z, Fix16 zoom);
    EXPORT bool IsPointInBoundaries(Fix16 x, Fix16 y);

    void ProjectWorldToScreen_4B90E0(Fix16 x, Fix16 y, Fix16 z, Fix16* pOutX, Fix16* pOutY)
    {
        Fix16 scale = kFpOne_702DE4 / ((kFpEight_702E04 - z) + mCurPos.mZ);
        *pOutX = (((x - mCurPos.mX) * mScreenScale.x) * scale) + Fix16(mScreenPxCenterX);
        *pOutY = (((y - mCurPos.mY) * mScreenScale.x) * scale) + Fix16(mScreenPxCenterY);
    }

    // ProjectWorldToScreen_4B90E0 with the out-of-line Fix16 helpers (ExplodingScore::DrawNumbers)
    void ProjectWorldToScreen_OutOfLine_4B90E0(Fix16 x, Fix16 y, Fix16 z, Fix16* pOutX, Fix16* pOutY)
    {
        Fix16 scale = kFpOne_702DE4.Divide((mCurPos.mZ - z) + kFpEight_702E04);

        *pOutX = (x.Subtract(mCurPos.mX).Multiply(mScreenScale.x).Multiply(scale))
                     .Add(Fix16(mScreenPxCenterX));

        *pOutY = (y.Subtract(mCurPos.mY).Multiply(mScreenScale.x).Multiply(scale))
                     .Add(Fix16(mScreenPxCenterY));
    }

    // 9.6f 0x475B60
    inline void SetSmoothFollow()
    {
        mFollowMode = camera_follow_mode::smooth_1;
    }

    // 9.6f 0x4A5070
    inline void SetSnapFollow()
    {
        mFollowMode = camera_follow_mode::snap_2;
    }

    // FUNCTION: 96f 0x40cfc0
    inline Fix16_Point_POD WorldToScreen_40CFC0(Fix16 x, Fix16 y, Fix16 z)
    {
        Fix16_Point_POD screen_pos;
        Fix16 u = mCurPos.mZ - z;
        Fix16 t(kOne_67681C / Fix16(u.mValue + kFpEight_676840.mValue, 0));

        screen_pos.x = (((x - mCurPos.mX) * mScreenScale.y) * t) + Fix16(320);
        screen_pos.y = (((y - mCurPos.mY) * mScreenScale.y) * t) + Fix16(240);

        return screen_pos;
    }

    // 9.6f 0x45AEA0
    inline bool IsSpriteInBounds(Sprite* pSprite, Fix16 margin)
    {
        return pSprite->mXy.x >= mPaddedViewBounds.mLeft - margin &&
            pSprite->mXy.x <= mPaddedViewBounds.mRight + margin &&
            pSprite->mXy.y >= mPaddedViewBounds.mTop - margin &&
            pSprite->mXy.y <= mPaddedViewBounds.mBottom + margin;
    }

    // 9.6f 0x45AF40
    inline bool IsRectInBounds(Fix16_Rect* pRect)
    {
        return pRect->get_top() <= mPaddedViewBounds.mBottom &&
            pRect->get_bottom() >= mPaddedViewBounds.mTop &&
            pRect->get_left() <= mPaddedViewBounds.mRight &&
            pRect->get_right() >= mPaddedViewBounds.mLeft;
    }

    // 9.6f 0x40CF60
    inline bool IsPointInViewBounds(Fix16 x, Fix16 y) const
    {
        return x >= mViewBounds.mLeft && x <= mViewBounds.mRight &&
            y >= mViewBounds.mTop && y <= mViewBounds.mBottom;
    }

    // 9.6f 0x41E410
    inline void CommitPendingTarget()
    {
        mTargetPos = mPendingTargetPos;
    }

    inline void ResetPendingTarget()
    {
        mPendingTargetPos = mTargetPos;
    }

    // 9.6f inline 0x433E90
    inline bool HasFollowTarget() const
    {
        return mTargetPed || mTargetCar;
    }

    // 9.6f 0x41E3D0
    inline void SetPendingTarget(Fix16 xpos, Fix16 ypos, Fix16 zpos, Fix16 zoom)
    {
        mPendingTargetPos.mX = xpos;
        mPendingTargetPos.mY = ypos;
        mPendingTargetPos.mZ = zpos + mTargetElevation;
        mPendingTargetPos.mZoom = zoom;
    }

    // 9.6f 0x4B3110
    Fix16 GetPaddedViewBoundsWidth() const
    {
        return mPaddedViewBounds.mRight - mPaddedViewBounds.mLeft;
    }

    // 9.6f 0x4B3130
    Fix16 GetPaddedViewBoundsHeight() const
    {
        return mPaddedViewBounds.mBottom - mPaddedViewBounds.mTop;
    }

    CameraPos mTargetPos; // 0x0
    CameraPos mPendingTargetPos; // 0x10
    WorldRect mPaddedViewBounds; // 0x20
    Fix16 mShakeAmount; // 0x30
    Ped* mTargetPed; // 0x34
    Car* mTargetCar; // 0x38
    s32 mFollowMode; // 0x3C
    Fix16 mTargetElevation; // 0x40
    u8 mSuspicion; // 0x44
    char_type mUnused45; // 0x45
    char_type mUnused46; // 0x46
    char_type mUnused47; // 0x47
    Fix16 mPanX; // 0x48
    Fix16 mPanY; // 0x4C
    Fix16 mPanSpeedUp; // 0x50
    Fix16 mPanSpeedDown; // 0x54
    Fix16 mPanSpeedLeft; // 0x58
    Fix16 mPanSpeedRight; // 0x5C
    Fix16_Point mScreenScale; // 0x60: x: screen width * zoom, y: 640 * zoom
    s32 mScreenPxWidth; // 0x68
    s32 mScreenPxHeight; // 0x6C
    s32 mScreenPxCenterX; // 0x70
    s32 mScreenPxCenterY; // 0x74
    WorldRect mViewBounds; // 0x78
    CameraPos mPrevPos; // 0x88
    CameraPos mCurPos; // 0x98
    Fix16 mUiScale; // 0xA8
    CameraPos mCamVelocity; // 0xAC
};
GTA2_ASSERT_SIZEOF_ALWAYS(Camera, 0xBC)

EXTERN_GLOBAL(Camera*, gViewCamera);

extern inline Fix16 Max_41E130(Fix16 a, Fix16 b);
