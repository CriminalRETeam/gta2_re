// This TU's copy of the Fix16_Point length zero (see Fix16_Point.hpp)
#define FIX16_POINT_ZERO kZero_679E70
#include "Cranes.hpp"
#include "Char_Pool.hpp"
#include "Globals.hpp"
#include "Hud.hpp"
#include "hud_brief_priority.hpp"
#include "Object_5C.hpp"
#include "SpriteGrid.hpp"
#include "debug.hpp"
#include "ScriptManager.hpp"
#include "map_0x370.hpp"
#include "rng.hpp"
#include "root_sound.hpp"
#include "sprite.hpp"

EXPORT void __stdcall SmoothApproachAngle(Fix16& target_angle, Fix16& angle_speed, Fix16& angle, Fix16& accel, Fix16& max_speed);
EXPORT void __stdcall SmoothApproach(Fix16& Coord_1, Fix16& Velocity_1, Fix16& Coord_2, Fix16& Velocity_2, Fix16& Velocity_3);

DEFINE_STATIC_MEMBER_INIT(Fix16, Crane, kHomeHookRadius, Fix16(0x2000, 0), 0x679E58);
DEFINE_GLOBAL_INIT(Fix16, kZero_679E70, Fix16(0), 0x679E70);
DEFINE_GLOBAL_INIT(Fix16, kFpTwo_679E78, Fix16(2), 0x679E78);
DEFINE_STATIC_MEMBER_INIT(Fix16, Crane, kFpTwo, kFpTwo_679E78, 0x679C78);
DEFINE_GLOBAL_INIT(Ang16, kAngZero_679FC4, Ang16(0), 0x679FC4);
DEFINE_GLOBAL(CranePool*, gCranePool_D9C, 0x679FD4);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kHomeHookAxialAngle, kZero_679E70, 0x679D50);
DEFINE_GLOBAL_INIT(Fix16, kFpTwo_679F8C, kFpTwo_679E78, 0x679F8C);
DEFINE_GLOBAL_INIT(Fix16, kFpThree_679E7C, Fix16(3), 0x679E7C);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpThree, kFpThree_679E7C, 0x679F88);
DEFINE_GLOBAL_INIT(Fix16, kAngFix16OneDegree_679FC8, Fix16(0x11C, 0), 0x679FC8);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kDropRetryAngleStep, kAngFix16OneDegree_679FC8 * 20, 0x679F64);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kAngFix16FullCircle, Fix16(0x18F60, 0), 0x679F58);
DEFINE_GLOBAL_INIT(Fix16, kFpFour_679E80, Fix16(4), 0x679E80);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kMaxHookRadius, kFpFour_679E80, 0x679F68);
DEFINE_GLOBAL_INIT(Fix16, kFpHalf_679CB0, Fix16(0x2000, 0), 0x679CB0);
DEFINE_GLOBAL_INIT(Fix16, kMinHookRadius, kFpHalf_679CB0, 0x679C3C);
DEFINE_GLOBAL_INIT(Fix16, kFpOne_679E74, Fix16(1), 0x679E74);
DEFINE_GLOBAL_INIT(Fix16, kFpHalf_679D64, Fix16(0x2000, 0), 0x679D64);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpHalf, kFpHalf_679D64, 0x679D34);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpThreeAndHalf, kFpThree_679E7C + kFpHalf_679D64, 0x679D28);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpTwoAndHalf, kFpTwo_679E78 + kFpHalf_679D64, 0x679D2C);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpOneAndHalf, kFpOne_679E74 + kFpHalf_679D64, 0x679D30);

DEFINE_GLOBAL_INIT(Fix16, kFpOne64th_679E20, Fix16(0x100, 0), 0x679E20);
DEFINE_GLOBAL_INIT(Fix16, kFpOne64th_679F28, kFpOne64th_679E20, 0x679F28);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kHookRadiusAccel, kFpOne64th_679F28, 0x679C14);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kHookRadiusMaxSpeed, kFpOne64th_679F28 * 4, 0x679E6C);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kCraneAngleAccel, kAngFix16OneDegree_679FC8* kFpHalf_679D64, 0x679F70);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kCraneAngleMaxSpeed, kAngFix16OneDegree_679FC8 * 2, 0x679DEC);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kHookAxialAngleAccel, kAngFix16OneDegree_679FC8, 0x679D70);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kHookAxialAngleMaxSpeed, kAngFix16OneDegree_679FC8 * 4, 0x679C40);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kHookDepthAccel, kFpOne64th_679F28, 0x679DC0);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kHookDepthMaxSpeed, kFpOne64th_679F28 * 4, 0x679DC8);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kAngFix16HalfCircle, Fix16(0xC7B0, 0), 0x679EB8);

// FUNCTION: 96f 0x40e790
inline Fix16 __stdcall WrapAngle(Fix16 angle)
{
    while (angle < kZero_679E70)
    {
        angle += kAngFix16FullCircle;
    }

    if (angle >= kAngFix16FullCircle)
    {
        do
        {
            angle -= kAngFix16FullCircle;
        } while (angle >= kAngFix16FullCircle);
    }
    return angle;
}

// The EH frame in state 4 comes from field_0-mTarget2Offset, which are Fix16_Point (destructor)
MATCH_FUNC(0x47e5b0)
Crane::~Crane()
{
    if (mSound)
    {
        gRoot_sound.DestroySoundObj(mSound);
        mSound = NULL;
    }
}

MATCH_FUNC(0x47e610)
Crane::Crane()
{
    mSound = NULL;
}

MATCH_FUNC(0x47e620)
void Crane::ComputeHookPos_47E620(Fix16 radius, Ang16 ang, Fix16_Point* pOutPoint)
{
    pOutPoint->SetXY(kZero_679E70, radius);

    // RotateByAngle_40F6B0, but the y part uses the out-of-line Fix16 operators
    Fix16 sin = Ang16::sine(ang);
    Fix16 cos = Ang16::cosine(ang);
    Fix16 x_old = pOutPoint->x;
    pOutPoint->x = (pOutPoint->x * cos) + (pOutPoint->y * sin);
    pOutPoint->y = x_old.Negate_4086A0().Multiply(sin).Add(pOutPoint->y * cos);

    *pOutPoint += mRotorObj->mSprite->get_x_y();
}

MATCH_FUNC(0x47e730)
void Crane::ComputeHookPos_47E730(Ang16 radius, Fix16 ang, Fix16_Point* pOutPoint)
{
    pOutPoint->SetXY(kZero_679E70, ang);
    // Inlined RotateByAngle_40F6B0, but with the y part using the out-of-line Fix16 operators
    Fix16 sin = Ang16::sine(radius);
    Fix16 cos = Ang16::cosine(radius);
    Fix16 x_old = pOutPoint->x;
    pOutPoint->x = (pOutPoint->x * cos) + (pOutPoint->y * sin);
    pOutPoint->y = x_old.Negate_4086A0().Multiply(sin).Add(pOutPoint->y * cos);
    *pOutPoint += mRotorObj->mSprite->get_x_y();
}

// 9.6f 0x448030
MATCH_FUNC(0x47e840)
void Crane::ComputeHookOffset(Ang16 ang, Fix16_Point* pOutPoint)
{

    pOutPoint->SetXY(kZero_679E70, -kFpHalf_679D64);
    // RotateByAngle_40F6B0, but the y part uses the out-of-line Fix16 operators
    Fix16 sin = Ang16::sine(ang);
    Fix16 cos = Ang16::cosine(ang);
    Fix16 x_old = pOutPoint->x;
    pOutPoint->x = (pOutPoint->x * cos) + (pOutPoint->y * sin);
    pOutPoint->y = (-x_old).Multiply(sin).Add(pOutPoint->y.Multiply(cos));
    *pOutPoint += mRotorObj->mSprite->get_x_y();
}

// 9.6f 0x448090
MATCH_FUNC(0x47e920)
bool Crane::IsDropPositionClear()
{
    Fix16_Point pos;
    ComputeHookPos_47E620(mDropRadius, Ang16::Fix16_To_Ang16_40F540(mDropAngle), &pos);
    pos += mDropOffset;

    mProbeSprite->set_xyz_lazy_420600(pos.x, pos.y, this->mGroundZ - this->mDropHookDepth);
    mProbeSprite->set_ang_lazy(Ang16::Fix16_To_Ang16_40F540(mDropRot));

    return !gSpriteGrid_1->FindCollidingSpritePreferType(mProbeSprite, sprite_types_enum::none_0) &&
        !mProbeSprite->CheckSpriteMovementRegion();
}

// 9.6f 0x448150
MATCH_FUNC(0x47eb00)
bool Crane::IsTarget1PositionClear()
{
    Fix16_Point hookPos;
    ComputeHookPos_47E620(mTarget1Radius, Ang16::Fix16_To_Ang16_40F540(mTarget1Angle), &hookPos);
    hookPos += mTarget1Offset;

    mProbeSprite->set_xyz_lazy_420600(hookPos.x, hookPos.y, mGroundZ - mTarget1HookDepth);
    mProbeSprite->set_ang_lazy(Ang16::Fix16_To_Ang16_40F540(mTarget1Rot));

    return gSpriteGrid_1->FindCollidingSpritePreferType(mProbeSprite, sprite_types_enum::none_0) == NULL;
}

MATCH_FUNC(0x47ecc0)
void Crane::DropHookedCar()
{
    Car* pCar = mpSpriteOnHook->AsCar();
    gSpriteGrid_1->AddToRegionBuckets(mpSpriteOnHook);
    pCar->UpdateDrawLayer();
    pCar->SetupCarPhysicsAndSpriteBinding_43BCA0();
    gCar_BC_Pool->UpdateNextPrev(pCar);
    pCar->ResetDespawnStatus();

    if (mHookJob != 3)
    {
        if (mCraneMode == 2)
        {
            if (mLoadPhase == 1)
            {
                mLoadPhase = 2;
            }
            else
            {
                mLoadPhase = 1;
                mStrct4.AddSprite(mpSpriteOnHook);
            }
        }
        else
        {
            mStrct4.AddSprite(mpSpriteOnHook);
        }
    }
    mpSpriteOnHook = NULL;
    mHookJob = 0;
    Crane::DropInPlaceOrReturnHome();
}

MATCH_FUNC(0x47ed60)
void Crane::DropHookedCarOnTransporter()
{
    Car* pCar = mpSpriteOnHook->AsCar();
    gCar_BC_Pool->UpdateNextPrev(pCar);
    pCar->ResetDespawnStatus();
    gSpriteGrid_1->AddToRegionBuckets(mpSpriteOnHook);
    mDropTransporter->AttachSprite(mpSpriteOnHook, kZero_679E70, kZero_679E70, kAngZero_679FC4);
    mStrct4.AddSprite(mDropTransporter);
    mpSpriteOnHook = NULL;
    mHookJob = 0;
    mDropTransporter = NULL;
    Crane::DropInPlaceOrReturnHome();
}

// 9.6 0x448300
MATCH_FUNC(0x47edf0)
void Crane::HookTransporterCargo()
{
    Car* pCar = mCargoTransporter->AsCar();

    pCar->mAttachments.RemoveSprite(mTransporterCargo);
    gSpriteGrid_3->Remove(mTransporterCargo);

    this->mpSpriteOnHook = this->mTransporterCargo;
    this->mHookedSpriteOffset = mpSpriteOnHook->get_x_y() - mHookObj->mSprite->get_x_y();

    mProbeSprite->mpCollisionShape->CopyDimensions(mpSpriteOnHook->mpCollisionShape);

    this->mTransporterCargo = NULL;

    if (mCraneMode == 1 || mCraneMode == 2 || mCraneMode == 3)
    {
        this->mHookJob = 4;
        this->mDropRadius = this->mTarget1Radius;
        this->mDropAngle = this->mTarget1Angle;
        this->mDropRot = this->mTarget1Rot;
        this->mDropOffset = this->mTarget1Offset;
        this->mDropHookDepth = this->mTarget1HookDepth;
    }
    else
    {
        this->mHookJob = 2;
        this->mDropRadius = kFpTwo_679E78;

        this->mDropAngle = WrapAngle(mCraneAngle + kAngFix16HalfCircle);
        this->mDropRot = mHookAxialAngle;
        this->mDropOffset = this->mHookedSpriteOffset;
        this->mDropHookDepth = kFpTwo;
    }
}

// 9.6f 0x448450
MATCH_FUNC(0x47ef80)
void Crane::HookPickupCar()
{
    this->mHookedCarThisFrame = true;

    Car* pCar = mPickupCar->AsCar();
    if (pCar->mPlayerPedId)
    {
        Ped* pPed = gPedManager->PedById(pCar->mPlayerPedId);
        if (pPed)
        {
            if (pPed->is_player())
            {
                pCar->mPlayerPedId = 0;
            }
        }
    }

    gCar_BC_Pool->mPool.UnlinkFromActiveList(pCar);

    pCar->SetDeactivatedStatus();
    pCar->DeAllocateCarPhysics_43BD00();
    gSpriteGrid_1->RemoveFromRegionBuckets(mPickupCar);

    this->mpSpriteOnHook = this->mPickupCar;
    this->mHookedSpriteOffset = mpSpriteOnHook->get_x_y().Sub(mHookObj->mSprite->get_x_y());

    mProbeSprite->mpCollisionShape->CopyDimensions(mpSpriteOnHook->mpCollisionShape);

    this->mPickupCar = NULL;

    if ((this->mCraneMode == 2 || this->mCraneMode == 3) && pCar->IsEngineCrushed())
    {
        this->mDropRadius = this->mTarget2Radius;
        this->mDropAngle = this->mTarget2Angle;
        this->mDropRot = this->mTarget2Rot;
        this->mDropOffset = this->mTarget2Offset;
        this->mDropHookDepth = this->mTarget2HookDepth;
    }
    else
    {
        if (this->mCraneMode != 1 && this->mCraneMode != 2)
        {
            this->mHookJob = 1;
            return;
        }
        this->mDropRadius = this->mTarget1Radius;
        this->mDropAngle = this->mTarget1Angle;
        this->mDropRot = this->mTarget1Rot;
        this->mDropOffset = this->mTarget1Offset;
        this->mDropHookDepth = this->mTarget1HookDepth;
    }

    this->mHookJob = 4;
}

// 9.6f 0x447D40
MATCH_FUNC(0x47f170)
void Crane::DropInPlaceOrReturnHome()
{
    if (mHookJob)
    {
        mHookJob = 3;
        mDropRadius = mHookRadius;
        mDropAngle = mCraneAngle;
        mDropRot = mHookAxialAngle;
        mDropHookDepth = kFpTwo;
        mHookRadiusTarget = mHookRadius;
        mCraneAngleTarget = mCraneAngle;
        mHookAngleTarget = mHookAxialAngle;
        mHookDepthTarget = mDropHookDepth;
    }
    else
    {
        mHookRadiusTarget = kHomeHookRadius;
        mCraneAngleTarget = mHomeAngle;
        mHookAngleTarget = kHomeHookAxialAngle;
        mHookDepthTarget = kZero_679E70;
    }
    mIsBusy = false;
}

MATCH_FUNC(0x47f220)
void Crane::SetTransporterCargoTarget(Fix16 radius, Fix16 angle, Sprite* pCargo, Sprite* pTransporter)
{
    mTransporterCargoRadius = radius;
    mTransporterCargoAngle = angle;
    mTransporterCargo = pCargo;
    mCargoTransporter = pTransporter;
    mTransporterCargoRot = Ang16::Ang16_to_Fix16(pCargo->mRotation);
    mTransporterCargoX = pCargo->mXy.x;
    mTransporterCargoY = pCargo->mXy.y;
    mTransporterCargoZ = pCargo->mZpos;
    mTransporterCargoHookDepth = mGroundZ - mTransporterCargoZ;
}

MATCH_FUNC(0x47f290)
void Crane::SetDropTransporterTarget(Fix16 radius, Fix16 angle, Sprite* pSprite)
{
    mDropTransporterRadius = radius;
    mDropTransporterAngle = angle;
    mDropTransporter = pSprite;
    mDropTransporterRot = Ang16::Ang16_to_Fix16(pSprite->mRotation);
    mDropTransporterPos.x = pSprite->mXy.x;
    mDropTransporterPos.y = pSprite->mXy.y;
    mDropTransporterZ = pSprite->mZpos;
    mDropTransporterHookDepth = mGroundZ - mDropTransporterZ;
}

MATCH_FUNC(0x47f2f0)
void Crane::SetPickupCarTarget(Fix16 radius, Fix16 angle, Sprite* pSprite)
{
    mPickupCarRadius = radius;
    mPickupCarAngle = angle;
    mPickupCar = pSprite;
    mPickupCarRot = Ang16::Ang16_to_Fix16(pSprite->mRotation);
    mPickupCarX = pSprite->mXy.x;
    mPickupCarY = pSprite->mXy.y;
    mPickupCarZ = pSprite->mZpos;
    mPickupCarHookDepth = mGroundZ - mPickupCarZ;
}

// 9.6f 0x448730
MATCH_FUNC(0x47f350)
bool Crane::IsTransporterCargoTargetValid()
{
    Car* pCar1 = mCargoTransporter->AsCar();
    if (!pCar1->IsDespawning())
    {
        Sprite *sp = mTransporterCargo;
        Car* pCar2 = sp->AsCar();
        if (!(pCar2->IsDespawning() || mTransporterCargoX != sp->mXy.x || mTransporterCargoY != sp->mXy.y ||
            mTransporterCargoZ != sp->mZpos || mTransporterCargoRot != Ang16::Ang16_to_Fix16(sp->mRotation)))
        {
            return true;
        }
    }
    return false;
}

// 9.6f 0x4487D0
MATCH_FUNC(0x47f3d0)
bool Crane::IsPickupCarTargetValid()
{
    Car* pCar = mPickupCar->AsCar();
    if (!pCar->IsDespawning() && this->mPickupCarX == mPickupCar->mXy.x && this->mPickupCarY == mPickupCar->mXy.y &&
        this->mPickupCarZ == mPickupCar->mZpos && this->mPickupCarRot == Ang16::Ang16_to_Fix16(mPickupCar->mRotation) && !pCar->mDriver &&
        pCar->AreAllDoorsClosed())
    {
        return true;
    }
    return false;
}

MATCH_FUNC(0x47f450)
bool Crane::IsDropTransporterTargetValid()
{
    Car* pCar = mDropTransporter->AsCar();
    if (!pCar->IsDespawning() && this->mDropTransporterPos.x == mDropTransporter->mXy.x && this->mDropTransporterPos.y == mDropTransporter->mXy.y &&
        this->mDropTransporterZ == mDropTransporter->mZpos && this->mDropTransporterRot == Ang16::Ang16_to_Fix16(mDropTransporter->mRotation))
    {
        return pCar->mAttachments.FirstSpriteOfType(sprite_types_enum::car_2) ? false : true;
    }
    return false;
}

MATCH_FUNC(0x47f4c0)
void Crane::UpdateCraneTargets()
{
    if (this->mTransporterCargo)
    {
        if (!IsTransporterCargoTargetValid())
        {
            this->mTransporterCargo = NULL;
            this->mReturnDelay = 60;
        }
    }

    if (this->mPickupCar)
    {
        if (!IsPickupCarTargetValid())
        {
            this->mPickupCar = NULL;
            this->mReturnDelay = 60;
        }
    }

    if (this->mDropTransporter)
    {
        if (!IsDropTransporterTargetValid())
        {
            this->mDropTransporter = NULL;
            this->mReturnDelay = 60;
        }
    }

    if (mHookJob == 2 || mHookJob == 3 || mHookJob == 4)
    {
        if (IsDropPositionClear())
        {
            this->mIsBusy = true;
        }
        else
        {
            this->mIsBusy = false;
            if (mHookJob != 4)
            {
                TryNextDropPosition();
            }
        }
    }

    if (mHookJob == 2 || mHookJob == 3 || mHookJob == 4)
    {
        this->mHookRadiusTarget = mDropRadius;
        this->mCraneAngleTarget = mDropAngle;
        this->mHookAngleTarget = mDropRot;
        this->mHookedSpriteOffsetTarget = this->mDropOffset;
        this->mHookDepthTarget = mDropHookDepth;
    }
    else if (mHookJob == 1 && this->mDropTransporter)
    {
        this->mHookRadiusTarget = mDropTransporterRadius;
        this->mHookedSpriteOffsetTarget.reset();
        this->mCraneAngleTarget = mDropTransporterAngle;
        this->mIsBusy = true;
        this->mHookAngleTarget = mDropTransporterRot;
        this->mHookDepthTarget = mDropTransporterHookDepth;
    }
    else if (mTransporterCargo)
    {
        this->mHookRadiusTarget = mTransporterCargoRadius;
        this->mCraneAngleTarget = mTransporterCargoAngle;
        this->mHookAngleTarget = mTransporterCargoRot;
        this->mIsBusy = true;
        this->mHookDepthTarget = mTransporterCargoHookDepth;
    }
    else if (mPickupCar && (mDropTransporter || mCraneMode == 1 || mCraneMode == 2 || mCraneMode == 3))
    {
        this->mHookRadiusTarget = mPickupCarRadius;
        this->mCraneAngleTarget = mPickupCarAngle;
        this->mHookAngleTarget = mPickupCarRot;
        this->mIsBusy = true;
        this->mHookDepthTarget = mPickupCarHookDepth;
    }
    else
    {
        mIsBusy = false;

        if (!mReturnDelay)
        {
            DropInPlaceOrReturnHome();
        }
        else
        {
            mReturnDelay--;
        }
    }
}

// 9.6f 0x448900
MATCH_FUNC(0x47f6c0)
bool Crane::ComputeHookPolar(Fix16_Point& pPoint, Fix16* pOutF16, Fix16* pOutAng)
{
    Fix16_Point to_hook = (pPoint.Sub(mRotorObj->mSprite->get_x_y()));
    *pOutF16 = to_hook.GetLength_41E260();

    // 9.6f (0x448900) only has the minimum radius test
    if (*pOutF16 <= kMaxHookRadius && *pOutF16 >= kMinHookRadius)
    {
        *pOutAng = Ang16::Ang16_to_Fix16(to_hook.GetAngle());
        return true;
    }
    else
    {
        return false;
    }
}

// 9.6f 0x448980
// 10.5 https://decomp.me/scratch/XYPfQ
MATCH_FUNC(0x47f7f0)
void Crane::TargetTransporter(Car* pCar)
{
    Fix16 point;
    Fix16 t;
    Sprite* pFoundSprite = pCar->mAttachments.FirstSpriteOfType(sprite_types_enum::car_2);
    if (pFoundSprite)
    {
        if (!mHookJob)
        {
            if (ComputeHookPolar(pFoundSprite->get_x_y_443580(), &point, &t))
            {
                if (mTransporterCargo == NULL || pFoundSprite == mTransporterCargo)
                {
                    SetTransporterCargoTarget(point, t, pFoundSprite, pCar->mCarSprite);
                    mDropTransporter = NULL;
                    mPickupCar = NULL;
                }
            }
        }
    }
    else if (mTransporterCargo == NULL && (mHookJob == 0 || mHookJob == 1) && mCraneMode == 0)
    {
        Sprite* pSprt = pCar->mCarSprite;
        if (ComputeHookPolar(pSprt->get_x_y_443580(), &point, &t))
        {
            if (mDropTransporter == NULL || pSprt == mDropTransporter)
            {
                SetDropTransporterTarget(point, t, pSprt);
            }
        }
    }
}

// 9.6f 0x448A80
// 10.5 https://decomp.me/scratch/HB5R5 return jump issue
MATCH_FUNC(0x47f930)
void Crane::PickUpCar(Car* pCar)
{

    if (!pCar->IsDespawning() && !mStrct4.TagSpriteWithRng(pCar->mCarSprite))
    {
        if (pCar->IsTruckTransport())
        {
            TargetTransporter(pCar);
        }
        else if (mDropTransporter || (mCraneMode == 1) || (mCraneMode == 2 || mCraneMode == 3) && mLoadPhase == 1 && !pCar->IsEngineCrushed() ||
                 mLoadPhase == 2 && pCar->IsEngineCrushed())
        {
            if (!mHookJob && !pCar->mDriver)
            {
                if (pCar->AreAllDoorsClosed())
                {
                    if (pCar->CanBeLiftedByCrane())
                    {
                        Fix16 hook_radius;
                        Fix16 hook_angle;
                        Sprite* pSprt = pCar->mCarSprite;
                        if (ComputeHookPolar(pSprt->get_x_y(), &hook_radius, &hook_angle))
                        {
                            if (mCraneMode != 1 || IsTarget1PositionClear())
                            {
                                if (mPickupCar == NULL || pSprt == mPickupCar)
                                {
                                    SetPickupCarTarget(hook_radius, hook_angle, pSprt);
                                }
                            }
                        }
                    }
                    else
                    {
                        Trailer* pTrailer = pCar->mpTrailer;
                        if (!pTrailer || pTrailer->mTrailerCar == NULL || !pTrailer->mTrailerCar->IsTruckTransport())
                        {
                            gHud_2B00->mBrief.SetHudBrief_5D4400(hud_brief_priority::normal_1, "nespray");
                            mStrct4.AddSprite(pCar->mCarSprite);
                            mStrct4.TagSpriteWithRng(pCar->mCarSprite);
                        }
                    }
                }
            }
        }
    }
}

// 9.6f 0x448C00
MATCH_FUNC(0x47fb40)
void Crane::TryNextDropPosition()
{
    if (mDropRadius == kFpTwo_679F8C)
    {
        mDropRadius = kFpThree;
    }
    else
    {
        mDropRadius = kFpTwo_679F8C;
        mDropAngle = WrapAngle(mDropAngle + kDropRetryAngleStep);
    }
}

MATCH_FUNC(0x47fba0)
s32 Crane::MoveTowardsTargets()
{
    if (this->mHookDepth == kZero_679E70)
    {
        SmoothApproach(this->mHookRadiusTarget, this->mHookRadiusSpeed, this->mHookRadius, kHookRadiusAccel, kHookRadiusMaxSpeed);
        SmoothApproachAngle(mCraneAngleTarget, mCraneAngleSpeed, mCraneAngle, kCraneAngleAccel, kCraneAngleMaxSpeed);
        SmoothApproachAngle(mHookAngleTarget, mHookAxialAngleSpeed, mHookAxialAngle, kHookAxialAngleAccel, kHookAxialAngleMaxSpeed);
        if (this->mpSpriteOnHook)
        {
            this->mHookedSpriteOffset = this->mHookedSpriteOffsetTarget;
        }
    }

    bool bLowerHook;
    Fix16 f_B8_hook_depth_target;
    if (this->mHookRadius == this->mHookRadiusTarget &&
        this->mCraneAngle == this->mCraneAngleTarget &&
        this->mHookAxialAngle == this->mHookAngleTarget && this->mIsBusy)
    {
        bLowerHook = true;
        f_B8_hook_depth_target = this->mHookDepthTarget;
    }
    else
    {
        bLowerHook = false;
        f_B8_hook_depth_target = kZero_679E70;
    }

    SmoothApproach(f_B8_hook_depth_target, this->mHookDepthSpeed, this->mHookDepth, kHookDepthAccel, kHookDepthMaxSpeed);

    this->mIsRotating = this->mCraneAngle != this->mCraneAngleTarget;
    this->mIsRadiusChanging = this->mHookRadius != this->mHookRadiusTarget;
    this->mIsHookDepthChanging = f_B8_hook_depth_target != mHookDepth;
    return bLowerHook && mHookDepth == f_B8_hook_depth_target;
}

MATCH_FUNC(0x47fd10)
void Crane::ReleaseHookedCar()
{
    if (mPowerupCmd)
    {
        gScriptManager->AddCranePowerupTrigger(mPowerupCmd);
    }

    switch (mHookJob)
    {
        case 1:
            DropHookedCarOnTransporter();
            break;

        case 2:
        case 3:
        case 4:
            DropHookedCar();
            break;
    }
}

MATCH_FUNC(0x47fd50)
void Crane::UpdateCraneTick()
{
    Fix16 old_crane_angle = mCraneAngle;
    Fix16 old_hook_radius = mHookRadius;
    Fix16 old_hook_axial_angle = mHookAxialAngle;
    Fix16 old_hook_depth = mHookDepth;

    u8 bMoved = Crane::MoveTowardsTargets();

    if (old_crane_angle != mCraneAngle || old_hook_radius != mHookRadius ||
        old_hook_axial_angle != mHookAxialAngle || old_hook_depth != mHookDepth)
    {
        Crane::UpdateCraneSprites();
    }
    if (bMoved != 0)
    {
        if (mTransporterCargo)
        {
            Crane::HookTransporterCargo();
        }
        else if (mPickupCar && (mDropTransporter || (mCraneMode == 1) || mCraneMode == 2 || mCraneMode == 3))
        {
            Crane::HookPickupCar();
        }
        else
        {
            Crane::ReleaseHookedCar();
        }
    }
}

// 9.6f 0x448E30
MATCH_FUNC(0x47fe10)
void Crane::UpdateCraneSprites()
{
    Fix16_Point pos;
    Ang16 crane_angle = Ang16::Fix16_To_Ang16_40F540(mCraneAngle);

    mPartObj0->RemoveFromCollisionBuckets();
    mPartObj1->RemoveFromCollisionBuckets();
    mPartObj2->RemoveFromCollisionBuckets();
    mPartObj3->RemoveFromCollisionBuckets();
    mPartObj4->RemoveFromCollisionBuckets();
    mPartObj5->RemoveFromCollisionBuckets();
    mPartObj6->RemoveFromCollisionBuckets();
    mPartObj7->RemoveFromCollisionBuckets();
    mPartObj8->RemoveFromCollisionBuckets();
    mHookObj->RemoveFromCollisionBuckets();

    ComputeHookPos_47E730(crane_angle, kFpHalf, &pos);
    mPartObj0->mSprite->set_ang_lazy(crane_angle);
    mPartObj0->mSprite->set_xy_lazy(pos.x, pos.y);

    mPartObj4->mSprite->set_ang_lazy(crane_angle);
    mPartObj4->mSprite->set_xy_lazy(pos.x, pos.y);

    ComputeHookPos_47E730(crane_angle, kFpOneAndHalf, &pos);
    mPartObj1->mSprite->set_ang_lazy(crane_angle);
    mPartObj1->mSprite->set_xy_lazy(pos.x, pos.y);

    mPartObj5->mSprite->set_ang_lazy(crane_angle);
    mPartObj5->mSprite->set_xy_lazy(pos.x, pos.y);

    ComputeHookPos_47E730(crane_angle, kFpTwoAndHalf, &pos);
    mPartObj2->mSprite->set_ang_lazy(crane_angle);
    mPartObj2->mSprite->set_xy_lazy(pos.x, pos.y);

    mPartObj6->mSprite->set_ang_lazy(crane_angle);
    mPartObj6->mSprite->set_xy_lazy(pos.x, pos.y);

    ComputeHookPos_47E730(crane_angle, kFpThreeAndHalf, &pos);
    mPartObj3->mSprite->set_ang_lazy(crane_angle);
    mPartObj3->mSprite->set_xy_lazy(pos.x, pos.y);

    mPartObj7->mSprite->set_ang_lazy(crane_angle);
    mPartObj7->mSprite->set_xy_lazy(pos.x, pos.y);

    mPartObj8->mSprite->set_ang_lazy(crane_angle);

    ComputeHookOffset(crane_angle, &pos);
    mCounterweightObj->mSprite->set_ang_lazy(crane_angle);
    mCounterweightObj->mSprite->set_xy_lazy(pos.x, pos.y);

    ComputeHookPos_47E620(mHookRadius, crane_angle, &pos);
    mPartObj8->mSprite->set_xy_lazy(pos.x, pos.y);

    mHookObj->mSprite->set_xyz_lazy_420600(pos.x, pos.y, mGroundZ - mHookDepth);
    mHookObj->mSprite->set_ang_lazy(Ang16::Fix16_To_Ang16_40F540(mHookAxialAngle));

    if (mpSpriteOnHook)
    {
        pos += mHookedSpriteOffset;
        mpSpriteOnHook->set_xyz_lazy_inlined_420600(pos.x, pos.y, mGroundZ - mHookDepth); // INLINED_MODE required
        mpSpriteOnHook->set_ang_lazy(Ang16::Fix16_To_Ang16_40F540(mHookAxialAngle));
    }

    mPartObj0->AssignToBucket();
    mPartObj1->AssignToBucket();
    mPartObj2->AssignToBucket();
    mPartObj3->AssignToBucket();
    mPartObj4->AssignToBucket();
    mPartObj5->AssignToBucket();
    mPartObj6->AssignToBucket();
    mPartObj7->AssignToBucket();
    mPartObj8->AssignToBucket();
    mHookObj->AssignToBucket();
}

MATCH_FUNC(0x480310)
void Crane::Service()
{
    mHookedCarThisFrame = false;
    mStrct4.RemoveAllExceptRngValue(gpRng->get_cur_rng() - 1);
    if (mpSpriteOnHook)
    {
        gSpriteGrid_3->Remove(mpSpriteOnHook);
    }
    Crane::UpdateCraneTargets();
    if (!mDisabled)
    {
        if (mHomeCrane == NULL || !IsAtHomeAngle() || mHomeCrane->IsAtHomeAngle())
        {
            Crane::UpdateCraneTick();
        }
    }

    if (mpSpriteOnHook)
    {
        mpSpriteOnHook->AsCar()->UpdateOnCraneHook();
    }

    if (mpSpriteOnHook)
    {
        gSpriteGrid_3->AddToSingleBucket_477AE0(mpSpriteOnHook);
    }
}

MATCH_FUNC(0x4803b0)
void Crane::InitCrane(Fix16 x_pos, Fix16 y_pos, char_type obj_id)
{
    mCraneMode = 0;
    SetDisabled(0);

    mGroundZ = gMap_0x370->FindGroundZForCoord(x_pos, y_pos);
    mRotorObj = gObject_5C->NewPhysicsObj(objects::crane_rotor_135, x_pos, y_pos, mGroundZ, kAngZero_679FC4);
    mPartObj0 = gObject_5C->NewPhysicsObj(objects::crane_cables_134, x_pos, y_pos, mGroundZ, kAngZero_679FC4);
    mPartObj1 = gObject_5C->NewPhysicsObj(objects::crane_cables_134, x_pos, y_pos, mGroundZ, kAngZero_679FC4);
    mPartObj2 = gObject_5C->NewPhysicsObj(objects::crane_cables_134, x_pos, y_pos, mGroundZ, kAngZero_679FC4);
    mPartObj3 = gObject_5C->NewPhysicsObj(objects::crane_cables_134, x_pos, y_pos, mGroundZ, kAngZero_679FC4);
    mPartObj4 = gObject_5C->NewPhysicsObj(objects::crane_arm_252, x_pos, y_pos, mGroundZ, kAngZero_679FC4);
    mPartObj5 = gObject_5C->NewPhysicsObj(260, x_pos, y_pos, mGroundZ, kAngZero_679FC4);
    mPartObj6 = gObject_5C->NewPhysicsObj(261, x_pos, y_pos, mGroundZ, kAngZero_679FC4);
    mPartObj7 = gObject_5C->NewPhysicsObj(262, x_pos, y_pos, mGroundZ, kAngZero_679FC4);
    mPartObj8 = gObject_5C->NewPhysicsObj(263, x_pos, y_pos, mGroundZ, kAngZero_679FC4);
    mCounterweightObj = gObject_5C->NewPhysicsObj(objects::crane_counterweight_140, x_pos, y_pos, mGroundZ, kAngZero_679FC4);
    mHookObj = gObject_5C->NewPhysicsObj(objects::crane_hook_136, x_pos, y_pos, mGroundZ, kAngZero_679FC4);

    mCraneBaseObj = gObject_5C->NewPhysicsObj(objects::crane_base_137, x_pos, y_pos, mGroundZ - kFpTwo, kAngZero_679FC4);
    mCraneBaseObj->SetParam(obj_id);
    mHomeCrane = NULL;
    mHookRadiusSpeed = kZero_679E70;
    mCraneAngleSpeed = kZero_679E70;
    mUnused9C = kZero_679E70;
    mHomeAngle = kZero_679E70;
    mCraneAngle = kZero_679E70;
    mHookRadius = kHomeHookRadius;
    mHookDepth = kZero_679E70;
    mHookDepthSpeed = kZero_679E70;
    mHookAxialAngle = kZero_679E70;
    mHookAxialAngleSpeed = kZero_679E70;
    mCraneAngleTarget = mCraneAngle;
    mHookRadiusTarget = mHookRadius;
    mIsBusy = false;
    mHookedSpriteOffsetTarget.x = 0;
    mHookedSpriteOffsetTarget.y = 0;
    mHookAngleTarget = mCraneAngle;
    mHookDepthTarget = kZero_679E70;
    mPickupCar = NULL;
    mPickupCarX = kZero_679E70;
    mPickupCarY = kZero_679E70;
    mPickupCarZ = kZero_679E70;
    mPickupCarRot = kZero_679E70;
    mPickupCarHookDepth = kZero_679E70;
    mTransporterCargo = NULL;
    mCargoTransporter = NULL;
    mTransporterCargoX = kZero_679E70;
    mTransporterCargoY = kZero_679E70;
    mTransporterCargoZ = kZero_679E70;
    mTransporterCargoRot = kZero_679E70;
    mTransporterCargoHookDepth = kZero_679E70;
    mpSpriteOnHook = NULL;
    mUnused154 = 0;
    mHookJob = 0;
    mDropRadius = kZero_679E70;
    mDropAngle = kZero_679E70;
    mDropRot = kZero_679E70;
    mDropOffset.x = 0;
    mDropOffset.y = 0;
    mDropHookDepth = kZero_679E70;

    Sprite* current_sprite = gSpritePool->get_new_sprite();
    mProbeSprite = current_sprite;
    current_sprite->AllocCollisionShape(kZero_679E70, kZero_679E70, kZero_679E70);
    mReturnDelay = 60;
    mDropTransporterRadius = kZero_679E70;
    mDropTransporterAngle = kZero_679E70;
    mDropTransporter = NULL;
    mDropTransporterRot = kZero_679E70;
    mDropTransporterPos.x = kZero_679E70;
    mDropTransporterPos.y = kZero_679E70;
    mDropTransporterZ = kZero_679E70;
    mDropTransporterHookDepth = kZero_679E70;
    mTarget1Radius = kZero_679E70;
    mTarget1Angle = kZero_679E70;
    mTarget1Rot = kZero_679E70;
    mTarget1Offset.x = 0;
    mTarget1Offset.y = 0;
    mTarget1HookDepth = kZero_679E70;
    mTarget2Radius = kZero_679E70;
    mTarget2Angle = kZero_679E70;
    mTarget2Rot = kZero_679E70;
    mTarget2Offset.x = 0;
    mTarget2Offset.y = 0;
    mTarget2HookDepth = kZero_679E70;
    mLoadPhase = 1;
    Crane::UpdateCraneSprites();
    mIsRotating = 0;
    mIsRadiusChanging = 0;
    mIsHookDepthChanging = 0;
    mHookedCarThisFrame = false;
    mPowerupCmd = 0;
    if (!mSound && !bSkip_audio)
    {
        mSound = gRoot_sound.CreateSoundObject(this, SoundObjectTypeEnum::Crane_15C_8);
    }
    mStrct4.ResetHead();
}

MATCH_FUNC(0x480900)
void Crane::SetTarget1(Fix16 xpos, Fix16 ypos, Ang16 ang)
{
    Fix16_Point target_pos(xpos, ypos);
    Fix16_Point t;
    ComputeHookPolar(target_pos, &mTarget1Radius, &mTarget1Angle);

    mTarget1Rot = Ang16::Ang16_to_Fix16(ang);

    if (mCraneMode == 3)
    {
        mCraneMode = 2;
        mLoadPhase = 1;
    }
    else
    {
        mCraneMode = 1;
    }

    ComputeHookPos_47E620(mTarget1Radius, Ang16::Fix16_To_Ang16_40F540(mTarget1Angle), &t);
    mTarget1Offset = target_pos - t;

    mTarget1HookDepth = mGroundZ - gMap_0x370->FindGroundZForCoord(xpos, ypos);
}

// 9.6f 0x4496E0
MATCH_FUNC(0x480b60)
void Crane::SetTarget2(Fix16 xpos, Fix16 ypos, Ang16 ang)
{
    Fix16_Point target_pos(xpos, ypos);
    Fix16_Point t;
    ComputeHookPolar(target_pos, &mTarget2Radius, &mTarget2Angle);

    mTarget2Rot = Ang16::Ang16_to_Fix16(ang);

    if (mCraneMode == 1)
    {
        mCraneMode = 2;
    }
    else
    {
        mCraneMode = 3;
        mLoadPhase = 2;
    }

    ComputeHookPos_47E620(mTarget2Radius, Ang16::Fix16_To_Ang16_40F540(mTarget2Angle), &t);
    mTarget2Offset = target_pos - t;

    mTarget2HookDepth = mGroundZ - gMap_0x370->FindGroundZForCoord(xpos, ypos);
}

MATCH_FUNC(0x480da0)
Car* Crane::GetCarFromCrane()
{
    if (mpSpriteOnHook)
    {
        return mpSpriteOnHook->AsCar();
    }
    return NULL;
}

MATCH_FUNC(0x480e00)
void CranePool::PickUpCar(Car* pCar, u8 crane_idx)
{
    mCranes[crane_idx].PickUpCar(pCar);
}

MATCH_FUNC(0x480e50)
void CranePool::CranesService()
{
    s32 i = 0;
    Crane* pIter = mCranes;
    while (i < mCount)
    {
        pIter->Service();
        i++;
        pIter++;
    }
}

MATCH_FUNC(0x480ec0)
Crane* CranePool::NewCrane(Fix16 x_pos, Fix16 y_pos)
{
    Crane* pNewCrane = &mCranes[mCount];
    pNewCrane->InitCrane(x_pos, y_pos, mCount);
    mCount++;
    return pNewCrane;
}

MATCH_FUNC(0x480f50)
CranePool::CranePool()
{
    mCount = 0;
}

MATCH_FUNC(0x4bbbf0)
CranePool::~CranePool()
{
}