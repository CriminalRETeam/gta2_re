#define FIX16_POINT_ZERO kZero_676818
#include "Camera.hpp"
#include "Car.hpp"
#include "CarPhysics.hpp"
#include "Function.hpp"
#include "Game.hpp"
#include "Globals.hpp"
#include "CarChaseTaskTable.hpp"
#include "Ped.hpp"
#include "Police.hpp"
#include "sprite.hpp"

// Forward declarations: the functions below are in address order
EXPORT void __stdcall SmoothApproach(Fix16& Coord_1, Fix16& Velocity_1, Fix16& Coord_2, Fix16& Velocity_2, Fix16& Velocity_3);
EXPORT void __stdcall SmoothApproachClamped(Fix16* target_coord,
                                                   Fix16* coord_velocity,
                                                   Fix16* curr_coord,
                                                   Fix16* velocity_1,
                                                   Fix16* velocity_2,
                                                   Fix16* velocity_3,
                                                   Fix16* max_neg_velocity);

DEFINE_GLOBAL(Camera*, gViewCamera, 0x676978);
DEFINE_GLOBAL_INIT(Fix16, kFpEight_676840, Fix16(8), 0x676840);
DEFINE_GLOBAL_INIT(Fix16, kFpHalf_67671C, Fix16(0x2000, 0), 0x67671C);
DEFINE_GLOBAL_INIT(Fix16, kZero_676818, Fix16(0), 0x676818);
DEFINE_GLOBAL_INIT(Fix16, kOne_67681C, Fix16(1), 0x67681C);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kDefaultZoom, Fix16(0x38CC, 0), 0x6766D4);
DEFINE_GLOBAL_INIT(Fix16, kFpFive_67682C, Fix16(5), 0x67682C);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kInitialPanSpeed, kFpFive_67682C, 0x6766E4);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kMaxPanY, Fix16(220), 0x6768F0);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kPanAcceleration, kFpFive_67682C, 0x676910);
DEFINE_GLOBAL_INIT(Fix16, kMaxTgtElevation, Fix16(20), 0x676894);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kMaxPanSpeed, kMaxTgtElevation, 0x676608);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kMaxPanX, Fix16(300), 0x6768C0);
DEFINE_GLOBAL_INIT(Fix16, kFpOne64th_6767D0, Fix16(256, 0), 0x6767D0);
DEFINE_GLOBAL_INIT(Fix16, kFpPoint1_676664, Fix16(1638, 0), 0x676664);
DEFINE_GLOBAL_INIT(Fix16, kFpHalf_676678, Fix16(0x2000, 0), 0x676678);
DEFINE_GLOBAL_INIT(Fix16, kFpOne64th_6768D8, kFpOne64th_6767D0, 0x6768D8);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpThreeSixteenths, kFpOne64th_6768D8 * 12, 0x676918);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpOne128th, kFpOne64th_6768D8* kFpHalf_676678, 0x6767B8);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpFiveEighths, kFpOne64th_6768D8 * 40, 0x676638);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpOne640th, kFpOne64th_6768D8* kFpPoint1_676664, 0x676834);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpFiveSixtyFourths, kFpOne64th_6768D8 * 5, 0x6765FC);
DEFINE_GLOBAL_INIT(Fix16, kFpOne64th_6766FC, kFpOne64th_6768D8, 0x6766FC);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpOneSixteenth, kFpOne64th_6768D8 * 4, 0x6766A4);
DEFINE_GLOBAL_INIT(Fix16, kFpOne64th_676740, kFpOne64th_6768D8* kOne_67681C, 0x676740);
DEFINE_GLOBAL_INIT(Fix16, kFpSeven_676838, Fix16(7), 0x676838);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kMaxMapCoord, Fix16(0x3FFFFF, 0), 0x67668C);
DEFINE_GLOBAL_INIT(Fix16, kFpThreeQuarters_6768E0, Fix16(0x3000, 0), 0x6768E0);
DEFINE_GLOBAL_INIT(Fix16, kFpThreeQuarters_67691C, kFpThreeQuarters_6768E0, 0x67691C);
DEFINE_GLOBAL_INIT(Fix16, kFpThreeQuarters_6766F4, Fix16(0x3000, 0), 0x6766F4);
DEFINE_GLOBAL_INIT(Fix16, kTwo_676820, Fix16(2), 0x676820);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpThreePoint55, Fix16(0xE333, 0), 0x6767B4);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kMaxCamZ, Fix16(14), 0x676898);
DEFINE_GLOBAL_INIT(Fix16, kZero_6F6C50, Fix16(0), 0x6F6C50);

DEFINE_GLOBAL_INIT(Fix16, kFpEight_702E04, Fix16(8), 0x702E04);
DEFINE_GLOBAL_INIT(Fix16, kFpOne_702DE4, Fix16(1), 0x702DE4);

DEFINE_GLOBAL_INIT(Fix16, kFpPoint2_67666C, Fix16(0xCCC, 0), 0x67666C);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpPoint2, kFpPoint2_67666C, 0x676694);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpPoint8, Fix16(0x3333, 0), 0x676684);

DEFINE_GLOBAL_INIT(Fix16, kFpSixteen_6768B4, Fix16(16), 0x6768B4);
DEFINE_GLOBAL_INIT(Fix16, kFpSixtyFour, Fix16(64), 0x6768E4);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpHalf, kFpHalf_676678, 0x676900);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpSixteen, kFpSixteen_6768B4, 0x67696C);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpOneAndHalf, kOne_67681C + kFpHalf_676678, 0x67674C);

DEFINE_GLOBAL_INIT(Ang16, kAng45_6766DC, Ang16(0x00B4, 0), 0x6766DC);
DEFINE_GLOBAL_INIT(Ang16, kAng135_676790, Ang16(0x021C, 0), 0x676790);
DEFINE_STATIC_GLOBAL_INIT(Ang16, kAng225, Ang16(0x0384, 0), 0x676764);
DEFINE_STATIC_GLOBAL_INIT(Ang16, kAng315, Ang16(0x04EC, 0), 0x67679C);
// Defined in sprite.cpp: when this TU sees the definition, VC6 reads the s16 with a 32-bit mov and adds with
// lea; the original's 16-bit `mov 0x676772,%cx; add %dx,%cx` needs an extern (ComputeTargetFacingAngle)
EXTERN_GLOBAL(Ang16, kAng180_676772);
DEFINE_GLOBAL_INIT(Ang16, kAngZero_676964, Ang16(0), 0x676964);


static inline Fix16 Max_41E130(Fix16 a, Fix16 b)
{
    if (a > b)
    {
        return a;
    }
    return b;
}

MATCH_FUNC(0x4355D0)
bool Camera::IsSpriteTheCameraSubject(Sprite* pSprite) const
{
    Car* pCar = pSprite->AsCar();
    if (pCar)
    {
        if (mTargetCar == pCar)
        {
            return true;
        }
        if (mTargetPed && mTargetPed->get_car() == pCar)
        {
            return true;
        }
    }
    else
    {
        Char* pChar = pSprite->AsCharB4();
        if (pChar)
        {
            if (mTargetPed)
            {
                if (mTargetPed->mGameObject == pChar)
                {
                    return true;
                }
            }
        }
    }
    return false;
}

MATCH_FUNC(0x435630)
bool Camera::IsSpriteInView(Sprite* pSprite, s32 wide_view)
{
    Fix16 half_width;
    half_width = mCurPos.mZ - pSprite->mZpos + kFpEight_676840;
    half_width = half_width / (mCurPos.mZoom * kTwo_676820);

    if (wide_view == 1)
    {
        half_width = (half_width * kFpPoint8);
    }

    Fix16 half_height = (half_width * kFpThreeQuarters_6766F4);

    Fix16_Rect rect;
    rect.SetHiLowZ(pSprite->mZpos, pSprite->mZpos);
    rect.SetRect_41E350(mCurPos.mX - half_width,
                        mCurPos.mX + half_width,
                        mCurPos.mY - half_height,
                        mCurPos.mY + half_height);

    Fix16_Rect* pBox = &pSprite->mpCollisionShape->mBoundingBox;
    if (rect.mLeft.IntervalIntersectsRange_438FB0_inline(rect.mRight, pBox->mLeft, pBox->mRight) &&
        IntervalIntersectsRange(rect.mTop, rect.mBottom, pBox->mTop, pBox->mBottom) &&
        IntervalIntersectsRange(rect.mLowZ, rect.mHighZ, pBox->mLowZ, pBox->mHighZ))
    {
        SpriteShape* pShape = pSprite->mpCollisionShape;
        if ((pShape->mWidth == pShape->mHeight && pShape->mWidth <= kFpPoint2) || pSprite->mRotation.rValue == 0 ||
            pSprite->mRotation == 360 || pSprite->mRotation == 720 || pSprite->mRotation == 1080 || pSprite->IntersectsRectSAT(&rect) ||
            rect.IntersectsSpriteRenderingRect(pSprite))
        {
            return true;
        }
    }
    return false;
}

MATCH_FUNC(0x4357B0)
void Camera::SavePrevCamPos()
{
    mPrevPos.mX = mCurPos.mX;
    mPrevPos.mY = mCurPos.mY;
    mPrevPos.mZ = mCurPos.mZ;
    mPrevPos.mZoom = mCurPos.mZoom;
}

MATCH_FUNC(0x4357F0)
void Camera::IncreaseElevation()
{
    if (mTargetElevation < kMaxTgtElevation)
    {
        mTargetElevation += kFpHalf_676678;
    }
}

MATCH_FUNC(0x435810)
void Camera::DecreaseElevation()
{
    if (mTargetElevation > kZero_676818)
    {
        mTargetElevation -= kFpHalf_676678;
    }
}

MATCH_FUNC(0x435830)
void Camera::ResetElevation()
{
    mTargetElevation = kZero_676818;
}

MATCH_FUNC(0x435840)
void Camera::ClampTargetZ()
{
    if (mPendingTargetPos.mZ < kZero_676818)
    {
        mPendingTargetPos.mZ = kZero_676818;
    }

    if (mPendingTargetPos.mZ > kMaxCamZ)
    {
        mPendingTargetPos.mZ = kMaxCamZ;
    }
}

MATCH_FUNC(0x435860)
void Camera::ApplyMovementDeltaFrom(Camera* pCamera)
{
    mPendingTargetPos.mX += pCamera->mCurPos.mX - pCamera->mPrevPos.mX;
    mPendingTargetPos.mY += pCamera->mCurPos.mY - pCamera->mPrevPos.mY;
    mPendingTargetPos.mZ += pCamera->mCurPos.mZ - pCamera->mPrevPos.mZ;
    mPendingTargetPos.mZoom += pCamera->mCurPos.mZoom - pCamera->mPrevPos.mZoom;
    ClampTargetZ();
}

// matches on decompme: https://decomp.me/scratch/NpBvl
MATCH_FUNC(0x4358D0)
Ang16 Camera::ComputeTargetFacingAngle()
{
    Ang16 CarRotation;
    Ang16 PedRotation;

    if (mTargetPed)
    {
        Car* mCar = mTargetPed->get_car();
        if (mCar)
        {
            CarRotation = mCar->GetCarRotation();
            CarPhysics* pCarPhysics = mCar->mPhysics;

            if (pCarPhysics && pCarPhysics->is_backward_gas_on())
            {
                return CarRotation + kAng180_676772;
            }
            return CarRotation;
        }
        PedRotation = mTargetPed->GetRotation();
        Char* mGameObject = mTargetPed->mGameObject;
        if (mGameObject && mGameObject->mFlagsBf.bReverseVelocity)
        {
            return PedRotation + kAng180_676772;
        }
        else
        {
            return PedRotation;
        }
    }
    else
    {
        if (mTargetCar)
        {
            CarRotation = mTargetCar->GetCarRotation();
            CarPhysics* pCarPhysics = mTargetCar->mPhysics;

            if (pCarPhysics && pCarPhysics->is_backward_gas_on())
            {
                return CarRotation + kAng180_676772;
            }
            return CarRotation;
        }
        else
        {
            return kAngZero_676964;
        }
    }
}

MATCH_FUNC(0x435A20)
Fix16 Camera::ReturnOwnerVelocity()
{
    Ped* pPed = mTargetPed;
    if (pPed)
    {
        return pPed->GetPedVelocity();
    }

    Car* pCar = mTargetCar;
    if (pCar)
    {
        return pCar->GetCarLinearSpeed();
    }
    else
    {
        return kZero_676818;
    }
}

// Defined ahead of IsCoordsPosVisible (out of address order): that function needs the out-of-line
// Fix16::operator* copy, and once C2 has compiled the copy it knows the call can't throw, so the inline
// FromPolar_41E210 here would drop offset.x's store before its second multiply. The original keeps it, as if
// the copy had not been compiled yet (the same effect as Fix16_Point::operator+ in Weapon, see
// docs/matching_quirks.md).
MATCH_FUNC(0x436200)
void Camera::ApplyCarVelocityCameraOffset(Car* pCar, Fix16* pOutX, Fix16* pOutY, Fix16* pOutZ)
{
    // 9.6f 0x41EBF0: this source matches it exactly when built with VC7 (compile_vc7.sh, permuter_score.py --96f)
    Ang16 angle;
    Fix16_Point vel;
    Fix16_Point offset;
    Fix16 radius;
    Fix16 speed;

    if (pCar->IsTrainModel())
    {
        speed = kFpHalf * kFpSixteen;
    }
    else
    {
        vel = pCar->GetLinearVelocity() * kFpSixteen;
        speed = vel.GetLength_41E260();
    }

    if (speed > kFpOneAndHalf)
    {
        *pOutZ += speed;

        if (!pCar->IsTrainModel() && !pCar->IsTank())
        {
            angle = vel.atan2_40F790();

            Fix16 dist;
            if ((angle > kAng45_6766DC && angle < kAng135_676790) || (angle > kAng225 && angle < kAng315))
            {
                dist = Fix16(240);
            }
            else
            {
                dist = Fix16(180);
            }

            if (pCar->IsTrailerCab())
            {
                dist *= kFpThreeQuarters_6768E0;
            }

            if (this->mSuspicion)
            {
                Fix16 suspicion;
                if (this->mSuspicion > 64u)
                {
                    suspicion = kFpSixtyFour;
                }
                else
                {
                    suspicion = Fix16(this->mSuspicion);
                }
                dist *= kOne_67681C - suspicion / 128;
            }

            radius = dist * (Fix16(8) - pCar->mCarSprite->mZpos + *pOutZ) / mScreenScale.y;
            offset.FromPolar_41E210(radius, angle);
            *pOutX += offset.x;
            *pOutY += offset.y;
        }
    }
}

MATCH_FUNC(0x435A70)
s32 Camera::IsCoordsPosVisible(Fix16 x, Fix16 y, Fix16 z)
{
    Fix16_Point_POD pos = WorldToScreen_40CFC0(x, y, z);
    if (pos.x >= kZero_676818 && pos.x < Fix16(640) && pos.y >= kZero_676818 && pos.y < Fix16(480))
    {
        return 1;
    }
    return 0;
}

// https://decomp.me/scratch/YoPmg Is mScreenScale really a Fix16_Point ?
MATCH_FUNC(0x435B90)
void Camera::UpdateBoundaries()
{
    mScreenScale.x = Fix16(mScreenPxWidth) * mCurPos.mZoom;
    mScreenScale.y = Fix16(640) * mCurPos.mZoom;

    Fix16 v = (kFpSeven_676838 + mCurPos.mZ) * (kOne_67681C / mCurPos.mZoom) * kFpHalf_67671C;

    mViewBounds.mLeft = mCurPos.mX - v;
    if (mViewBounds.mLeft < kZero_676818)
    {
        mViewBounds.mLeft = Fix16(0);
    }
    else if (mViewBounds.mLeft > kMaxMapCoord)
    {
        mViewBounds.mLeft = kMaxMapCoord;
    }

    mViewBounds.mRight = mCurPos.mX + v;
    if (mViewBounds.mRight < kZero_676818)
    {
        mViewBounds.mRight = Fix16(0);
    }
    else if (mViewBounds.mRight > kMaxMapCoord)
    {
        mViewBounds.mRight = kMaxMapCoord;
    }

    v *= kFpThreeQuarters_6768E0;

    mViewBounds.mTop = mCurPos.mY - v;
    if (mViewBounds.mTop < kZero_676818)
    {
        mViewBounds.mTop = Fix16(0);
    }
    else if (mViewBounds.mTop > kMaxMapCoord)
    {
        mViewBounds.mTop = kMaxMapCoord;
    }

    mViewBounds.mBottom = mCurPos.mY + v;
    if (mViewBounds.mBottom < kZero_676818)
    {
        mViewBounds.mBottom = Fix16(0);
    }
    else if (mViewBounds.mBottom > kMaxMapCoord)
    {
        mViewBounds.mBottom = kMaxMapCoord;
    }

    mPaddedViewBounds.mLeft = mViewBounds.mLeft - kFpThreeQuarters_67691C;
    mPaddedViewBounds.mRight = mViewBounds.mRight + kFpThreeQuarters_67691C;
    mPaddedViewBounds.mTop = mViewBounds.mTop - kFpThreeQuarters_67691C;
    mPaddedViewBounds.mBottom = mViewBounds.mBottom + kFpThreeQuarters_67691C;
}

MATCH_FUNC(0x435D20)
void Camera::MoveTarget(bool move_north, bool move_south, bool move_west, bool move_east, bool zoom_in, bool zoom_out)
{
    ResetPendingTarget();
    if (move_north)
    {
        mPendingTargetPos.mY -= kFpHalf_67671C;
    }

    if (move_south)
    {
        mPendingTargetPos.mY += kFpHalf_67671C;
    }

    if (move_west)
    {
        mPendingTargetPos.mX -= kFpHalf_67671C;
    }

    if (move_east)
    {
        mPendingTargetPos.mX += kFpHalf_67671C;
    }

    if (zoom_in)
    {
        mPendingTargetPos.mZ += kOne_67681C;
    }

    if (zoom_out)
    {
        mPendingTargetPos.mZ -= kOne_67681C;
    }
    ClampTargetZ();
}

MATCH_FUNC(0x435DD0)
void Camera::ResetCameraSmoothing()
{
    mCurPos.mX = mTargetPos.mX;
    mCurPos.mY = mTargetPos.mY;
    mCurPos.mZ = mTargetPos.mZ;
    mCurPos.mZoom = mTargetPos.mZoom;

    mCamVelocity.mX = kZero_676818;
    mCamVelocity.mY = kZero_676818;
    mCamVelocity.mZ = kZero_676818;
    mCamVelocity.mZoom = kZero_676818;
}

MATCH_FUNC(0x435F90)
void Camera::AccumulateSuspicionOnDriver(Car* pCar)
{
    if (pCar->mDriver &&
        (gPolice_7B8->IsActivelyChased(pCar->mDriver) ||
         gCarChaseTaskTable->HasAnyPursuer(pCar->mDriver)))
    {
        mSuspicion++;
        if (mSuspicion > 80u)
        {
            mSuspicion = 80;
        }
    }
    else
    {
        if (mSuspicion > 0)
        {
            mSuspicion--;
        }
    }
}

MATCH_FUNC(0x435FF0)
void Camera::Update()
{
    Camera::SavePrevCamPos();
    Fix16 scale = mCurPos.mZ * kFpThreeSixteenths;

    switch (mFollowMode)
    {
        case camera_follow_mode::smooth_1:
            SmoothApproach(mTargetPos.mX,
                                  mCamVelocity.mX,
                                  mCurPos.mX,
                                  kFpOne64th_676740,
                                  scale);
            SmoothApproach(mTargetPos.mY,
                                  mCamVelocity.mY,
                                  mCurPos.mY,
                                  kFpOne64th_676740,
                                  scale);
            SmoothApproachClamped(&mTargetPos.mZ,
                                         &mCamVelocity.mZ,
                                         &mCurPos.mZ,
                                         &kFpOne128th,
                                         &kFpFiveEighths,
                                         &kFpOne640th,
                                         &kFpFiveSixtyFourths);
            SmoothApproach(mTargetPos.mZoom,
                                  mCamVelocity.mZoom,
                                  mCurPos.mZoom,
                                  kFpOne64th_6766FC,
                                  kFpOneSixteenth);
            break;
        case camera_follow_mode::snap_2:
            mCurPos.mX = mTargetPos.mX;
            mCurPos.mY = mTargetPos.mY;
            mCurPos.mZ = mTargetPos.mZ;
            mCurPos.mZoom = mTargetPos.mZoom;
            break;
    }
    if (mShakeAmount != kZero_676818)
    {
        Camera::ApplyShake();
    }
    Camera::UpdateBoundaries();
    mTargetPos = mPendingTargetPos;
}

MATCH_FUNC(0x436110)
void Camera::RefreshBoundaries()
{
    UpdateBoundaries();
}

MATCH_FUNC(0x436120)
void Camera::SetShake(Fix16 amount)
{
    mShakeAmount = amount * kFpOne64th_6768D8;
}

MATCH_FUNC(0x436140)
void Camera::ApplyShake()
{
    mCurPos.mX += mShakeAmount;
    mCurPos.mY += mShakeAmount;
    mCurPos.mZ += mShakeAmount;
    if (mCurPos.mZ < kZero_676818)
    {
        mCurPos.mZ = kZero_676818;
    }
    mShakeAmount = -mShakeAmount;
    mShakeAmount = kFpThreeQuarters_6766F4 * mShakeAmount;
}

MATCH_FUNC(0x4361B0)
void Camera::SetScreenSize(u32 width, u32 height)
{
    mScreenPxWidth = width;
    mScreenPxHeight = height;

    mScreenPxCenterX = width / 2;
    mScreenPxCenterY = height / 2;

    mScreenScale.x = Fix16(-1);
    mScreenScale.y = Fix16(-1);

    mUiScale = Fix16(width) / 640;
}


MATCH_FUNC(0x4364A0)
void Camera::UpdateFollowCarCamera(Car* pCar)
{

    this->mTargetPed = NULL;
    this->mTargetCar = pCar;

    if (pCar)
    {
        Fix16 new_x;
        new_x = pCar->GetX();
        Fix16 new_y;
        new_y = pCar->GetY();
        Fix16 new_z;
        new_z = Max_41E130(pCar->GetZ() - kTwo_676820, kOne_67681C);
        Fix16 zoom = kDefaultZoom;

        AccumulateSuspicionOnDriver(pCar);
        ApplyCarVelocityCameraOffset(pCar, &new_x, &new_y, &new_z);
        SetPendingTarget(new_x, new_y, new_z, zoom);
    }
}

MATCH_FUNC(0x436540)
void Camera::UpdateFollowPedCamera(Ped* pPed)
{
    Car* pCar_2;
    Fix16 zposToUse;

    mTargetCar = NULL;
    mTargetPed = pPed;
    if (pPed != NULL)
    {
        Fix16 xpos;
        Fix16 ypos;
        Fix16 zpos;
        Car* pCar = pPed->get_car();
        if (pCar || (pCar_2 = pPed->GetCarBeingEnteredOrExited(), pCar_2 == NULL))
        {
            xpos = pPed->get_cam_x();
            ypos = pPed->get_cam_y();
            zpos = Max_41E130(pPed->get_cam_z() - kTwo_676820, kOne_67681C);
            zposToUse = zpos;
            if (pCar)
            {
                Camera::AccumulateSuspicionOnDriver(pCar);
                Camera::ApplyCarVelocityCameraOffset(pCar, &xpos, &ypos, &zposToUse);
            }
        }
        else
        {
            xpos = pCar_2->GetX();
            ypos = pCar_2->GetY();
            zpos = Max_41E130(pCar_2->GetZ() - kTwo_676820, kOne_67681C);
            zposToUse = zpos;
            Camera::AccumulateSuspicionOnDriver(pCar_2);
            Camera::ApplyCarVelocityCameraOffset(pCar_2, &xpos, &ypos, &zposToUse);
        }
        Fix16 zoom = kFpThreePoint55;
        if (pPed->GetPedState() != ped_state_1::dead_9)
        {
            zoom = kDefaultZoom;
        }
        Camera::ApplyPanOffset(pPed, xpos, ypos, zposToUse);
        SetPendingTarget(xpos, ypos, zposToUse, zoom);
    }
}

MATCH_FUNC(0x436710)
void Camera::HandlePanning(bool forward_gas_on, bool foot_brake_on, bool left_on, bool right_on)
{
    if (forward_gas_on)
    {
        mPanY -= mPanSpeedUp;
        if (mPanY < -kMaxPanY)
        {
            mPanY = -kMaxPanY;
        }

        mPanSpeedUp += kPanAcceleration;
        if (mPanSpeedUp > kMaxPanSpeed)
        {
            mPanSpeedUp = kMaxPanSpeed;
        }
    }
    else
    {
        mPanSpeedUp = kInitialPanSpeed;
    }

    if (foot_brake_on)
    {
        mPanY += mPanSpeedDown;
        if (mPanY > kMaxPanY)
        {
            mPanY = kMaxPanY;
        }

        mPanSpeedDown += kPanAcceleration;
        if (mPanSpeedDown > kMaxPanSpeed)
        {
            mPanSpeedDown = kMaxPanSpeed;
        }
    }
    else
    {
        mPanSpeedDown = kInitialPanSpeed;
    }

    if (left_on)
    {
        mPanX -= mPanSpeedLeft;
        if (mPanX < -kMaxPanX)
        {
            mPanX = -kMaxPanX;
        }

        mPanSpeedLeft += kPanAcceleration;
        if (mPanSpeedLeft > kMaxPanSpeed)
        {
            mPanSpeedLeft = kMaxPanSpeed;
        }
    }
    else
    {
        mPanSpeedLeft = kInitialPanSpeed;
    }

    if (right_on)
    {
        mPanX += mPanSpeedRight;
        if (mPanX > kMaxPanX)
        {
            mPanX = kMaxPanX;
        }

        mPanSpeedRight += kPanAcceleration;
        if (mPanSpeedRight > kMaxPanSpeed)
        {
            mPanSpeedRight = kMaxPanSpeed;
        }
    }
    else
    {
        mPanSpeedRight = kInitialPanSpeed;
    }
}

MATCH_FUNC(0x436830)
void Camera::ResetPanning()
{
    mPanX = 0;
    mPanY = 0;
    mPanSpeedLeft = kInitialPanSpeed;
    mPanSpeedRight = kInitialPanSpeed;
    mPanSpeedUp = kInitialPanSpeed;
    mPanSpeedDown = kInitialPanSpeed;
}

MATCH_FUNC(0x436860)
void Camera::ApplyPanOffset(Ped* pPed, Fix16& x_pos, Fix16& y_pos, Fix16 z_pos)
{
    Fix16 depth_scale = (z_pos - pPed->get_cam_z() + Fix16(8)) / mScreenScale.y;
    x_pos += mPanX * depth_scale;
    y_pos += mPanY * depth_scale;
}

MATCH_FUNC(0x4368E0)
Camera::Camera()
{
    mScreenPxWidth = 0;
    mScreenPxHeight = 0;
    ResetElevation();
    mCurPos.mZoom = kDefaultZoom;
    SetTarget(-1, -1, -1, kDefaultZoom);
    CommitPendingTarget();
    mScreenScale.x = Fix16(-1);
    mScreenScale.y = Fix16(-1);
    mCamVelocity.mX = kZero_676818;
    mCamVelocity.mY = kZero_676818;
    mCamVelocity.mZ = kZero_676818;
    mFollowMode = camera_follow_mode::none_0;
    mShakeAmount = kZero_676818;
    mTargetPed = NULL;
    SetScreenSize(640, 480);
    mSuspicion = 0;
    ResetPanning();
}

MATCH_FUNC(0x4369E0)
Camera::~Camera()
{
}

MATCH_FUNC(0x4397D0)
void Camera::SetTarget(Fix16 x, Fix16 y, Fix16 z, Fix16 zoom)
{
    mPendingTargetPos.mX = x;
    mPendingTargetPos.mY = y;
    z += mTargetElevation;
    mPendingTargetPos.mZ = z;
    mPendingTargetPos.mZoom = zoom;
}

// https://decomp.me/scratch/qYIak
MATCH_FUNC(0x4F7540)
EXPORT void __stdcall SmoothApproach(Fix16& Coord_1, Fix16& Velocity_1, Fix16& Coord_2, Fix16& Velocity_2, Fix16& Velocity_3)
{
    // One shared `Coord_2 += Velocity_1` at the end: VC6 copies it into the clamp paths
    Fix16 DeltaCoord = Coord_1 - Coord_2;
    if (DeltaCoord > kZero_6F6C50)
    {
        if (Velocity_1 >= kZero_6F6C50)
        {
            if (Velocity_1 + Velocity_2 <= DeltaCoord)
            {
                Velocity_1 += Velocity_2;
                if (Velocity_1 > Velocity_3)
                {
                    Velocity_1 = Velocity_3;
                }
            }
            else
            {
                Velocity_1 = DeltaCoord;
            }
        }
        else
        {
            Velocity_1 = kZero_6F6C50;
        }
    }
    else if (DeltaCoord < kZero_6F6C50)
    {
        if (Velocity_1 <= kZero_6F6C50)
        {
            if (Velocity_1 - Velocity_2 >= DeltaCoord)
            {
                Velocity_1 -= Velocity_2;
                if (Velocity_1 < -Velocity_3)
                {
                    Velocity_1 = -Velocity_3;
                }
            }
            else
            {
                Velocity_1 = DeltaCoord;
            }
        }
        else
        {
            Velocity_1 = kZero_6F6C50;
        }
    }
    else
    {
        Velocity_1 = kZero_6F6C50;
    }
    Coord_2 += Velocity_1;
}

// https://decomp.me/scratch/kwM8W
MATCH_FUNC(0x4F75D0)
EXPORT void __stdcall SmoothApproachClamped(Fix16* target_coord,
                                                   Fix16* coord_velocity,
                                                   Fix16* curr_coord,
                                                   Fix16* velocity_1,
                                                   Fix16* velocity_2,
                                                   Fix16* velocity_3,
                                                   Fix16* max_neg_velocity)
{
    Fix16 DeltaCoord = *target_coord - *curr_coord;
    if (DeltaCoord > kZero_6F6C50)
    {
        if (*coord_velocity >= kZero_6F6C50)
        {
            if (*coord_velocity + *velocity_1 <= DeltaCoord)
            {
                *coord_velocity += *velocity_1;
                if (*coord_velocity > *velocity_2)
                {
                    *coord_velocity = *velocity_2;
                }
            }
            else
            {
                *coord_velocity = DeltaCoord;
            }
        }
        else
        {
            *coord_velocity = kZero_6F6C50;
        }
    }
    else
    {
        if (DeltaCoord >= kZero_6F6C50 || *coord_velocity > kZero_6F6C50)
        {
            *coord_velocity = kZero_6F6C50;
        }
        else
        {
            if (*coord_velocity - *velocity_3 >= DeltaCoord)
            {
                *coord_velocity -= *velocity_3;
                if (*coord_velocity < -*max_neg_velocity)
                {
                    *coord_velocity = -*max_neg_velocity;
                }
            }
            else
            {
                *coord_velocity = DeltaCoord;
            }
        }
    }

    *curr_coord += *coord_velocity;
}

MATCH_FUNC(0x58CF10)
bool Camera::IsPointInBoundaries(Fix16 x, Fix16 y)
{
    return x >= mViewBounds.mLeft && x <= mViewBounds.mRight &&
        y >= mViewBounds.mTop && y <= mViewBounds.mBottom;
}