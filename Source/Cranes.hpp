#pragma once

#include "Car.hpp"
#include "Function.hpp"
#include "Object_5C.hpp"
#include "ang16.hpp"
#include "fix16.hpp"

class SoundObject;
class Car;

class Crane
{
  public:
    STATIC_MEMBER(Fix16, kHomeHookRadius);
    STATIC_MEMBER(Fix16, kFpTwo);
    // 9.6f 0x447F60
    inline void SetDisabled_447F60(s32 v)
    {
        mDisabled = v;
    }

    // 9.6f 0x476900
    inline void SetHomeCrane_476900(Crane* v)
    {
        mHomeCrane = v;
    }

    EXPORT ~Crane();
    EXPORT Crane();
    EXPORT void ComputeHookPos_47E620(Fix16 radius, Ang16 ang, Fix16_Point* pOutPoint);
    EXPORT void ComputeHookPos_47E730(Ang16 radius, Fix16 ang, Fix16_Point* pOutPoint);
    EXPORT void ComputeHookOffset(Ang16 ang, Fix16_Point* pOutPoint);
    EXPORT bool IsDropPositionClear();
    EXPORT bool IsTarget1PositionClear();
    EXPORT void DropHookedCar();
    EXPORT void DropHookedCarOnTransporter();
    EXPORT void HookTransporterCargo();
    EXPORT void HookPickupCar();
    EXPORT void DropInPlaceOrReturnHome();
    EXPORT void SetTransporterCargoTarget(Fix16 radius, Fix16 angle, Sprite* pCargo, Sprite* pTransporter);
    EXPORT void SetDropTransporterTarget(Fix16 radius, Fix16 angle, Sprite* pSprite);
    EXPORT void SetPickupCarTarget(Fix16 radius, Fix16 angle, Sprite* pSprite);
    EXPORT bool IsTransporterCargoTargetValid();
    EXPORT bool IsPickupCarTargetValid();
    EXPORT bool IsDropTransporterTargetValid();
    EXPORT void UpdateCraneTargets();
    EXPORT bool ComputeHookPolar(Fix16_Point& pPoint, Fix16* pOutF16, Fix16* pOutAng);
    EXPORT void TargetTransporter(Car* pCar);
    EXPORT void PickUpCar(Car* pCar);
    EXPORT void TryNextDropPosition();
    EXPORT s32 MoveTowardsTargets();
    EXPORT void ReleaseHookedCar();
    EXPORT void UpdateCraneTick();
    EXPORT void UpdateCraneSprites();
    EXPORT void Service();
    EXPORT void InitCrane(Fix16 x_pos, Fix16 y_pos, char_type obj_id);
    EXPORT void SetTarget1(Fix16 xpos, Fix16 ypos, Ang16 ang);
    EXPORT void SetTarget2(Fix16 xpos, Fix16 ypos, Ang16 ang);
    EXPORT Car* GetCarFromCrane();

    inline void SetHomeRotation_4768E0(Ang16 rotation)
    {
        mHomeAngle = Ang16::Ang16_to_Fix16(rotation);
    }

    // 9.6f 0x411A00
    inline Sprite* GetRotorSprite_411A00() const
    {
        return mRotorObj->mSprite;
    }

    // FUNCTION: 96f 0x447f40
    inline s32 IsAtHomeAngle_447F40() const
    {
        return mCraneAngle == mHomeAngle;
    }

    Fix16_Point mHookedSpriteOffsetTarget; // 0x0
    Fix16_Point mDropOffset; // 0x8
    Fix16_Point mHookedSpriteOffset; // 0x10
    Fix16_Point mTarget1Offset; // 0x18
    Fix16_Point mTarget2Offset; // 0x20
    SpriteList mStrct4; // 0x28
    Object* mRotorObj; // 0x2C
    Object* mPartObj0; // 0x30
    Object* mPartObj1; // 0x34
    Object* mPartObj2; // 0x38
    Object* mPartObj3; // 0x3C
    Object* mPartObj4; // 0x40
    Object* mPartObj5; // 0x44
    Object* mPartObj6; // 0x48
    Object* mPartObj7; // 0x4C
    Object* mPartObj8; // 0x50
    Object* mHookObj; // 0x54
    Object* mCraneBaseObj; // 0x58
    Object* mCounterweightObj; // 0x5C
    Sprite* mProbeSprite; // 0x60
    Sprite* mDropTransporter; // 0x64
    Sprite* mPickupCar; // 0x68
    Sprite* mTransporterCargo; // 0x6C
    Sprite* mCargoTransporter; // 0x70
    Sprite* mpSpriteOnHook; // 0x74
    Crane* mHomeCrane; // 0x78
    SoundObject* mSound; // 0x7C
    Fix16 mGroundZ; // 0x80
    Fix16 mHookDepth; // 0x84
    Fix16 mHookDepthSpeed; // 0x88
    Fix16 mCraneAngle; // 0x8C:  It's not Ang16, maybe Ang32
    Fix16 mHookRadius; // 0x90:  Radial distance
    Fix16 mHookRadiusSpeed; // 0x94
    Fix16 mCraneAngleSpeed; // 0x98
    Fix16 mUnused9C; // 0x9C
    Fix16 mHookAxialAngle;  // 0xA0: The angle in its own rotation axis, e.g. the angle of the lifted car
    Fix16 mHookAxialAngleSpeed; // 0xA4
    Fix16 mHomeAngle; // 0xA8
    Fix16 mCraneAngleTarget;  // 0xAC: the crane will rotate until match this angle
    Fix16 mHookRadiusTarget; // 0xB0
    Fix16 mHookAngleTarget; // 0xB4
    Fix16 mHookDepthTarget; // 0xB8
    Fix16 mDropTransporterRadius; // 0xBC
    Fix16 mDropTransporterAngle; // 0xC0
    Fix16_Point_POD mDropTransporterPos; // 0xC4
    Fix16 mDropTransporterZ; // 0xCC
    Fix16 mDropTransporterRot; // 0xD0
    Fix16 mDropTransporterHookDepth; // 0xD4
    Fix16 mPickupCarRadius; // 0xD8
    Fix16 mPickupCarAngle; // 0xDC
    Fix16 mPickupCarX; // 0xE0
    Fix16 mPickupCarY; // 0xE4
    Fix16 mPickupCarZ; // 0xE8
    Fix16 mPickupCarRot; // 0xEC
    Fix16 mPickupCarHookDepth; // 0xF0
    Fix16 mTransporterCargoRadius; // 0xF4
    Fix16 mTransporterCargoAngle; // 0xF8
    Fix16 mTransporterCargoX; // 0xFC
    Fix16 mTransporterCargoY; // 0x100
    Fix16 mTransporterCargoZ; // 0x104
    Fix16 mTransporterCargoRot; // 0x108
    Fix16 mTransporterCargoHookDepth; // 0x10C
    Fix16 mDropAngle;  // 0x110: something to do with the crane angle
    Fix16 mDropRadius;  // 0x114: something to do with the hook radius
    Fix16 mDropRot; // 0x118
    Fix16 mDropHookDepth; // 0x11C
    Fix16 mTarget1Radius; // 0x120
    Fix16 mTarget1Angle; // 0x124
    Fix16 mTarget1Rot; // 0x128
    Fix16 mTarget1HookDepth; // 0x12C
    Fix16 mTarget2Radius; // 0x130
    Fix16 mTarget2Angle; // 0x134
    Fix16 mTarget2Rot; // 0x138
    Fix16 mTarget2HookDepth; // 0x13C
    s16 mPowerupCmd; // 0x140
    s16 mUnused142; // 0x142
    s32 mCraneMode; // 0x144
    s32 mDisabled; // 0x148
    char_type mReturnDelay; // 0x14C
    bool mIsBusy; // 0x14D
    char_type mUnused14E; // 0x14E
    char_type mUnused14F; // 0x14F
    s32 mHookJob; // 0x150
    char_type mUnused154; // 0x154
    char_type mLoadPhase; // 0x155
    char_type mIsRotating; // 0x156
    char_type mIsRadiusChanging; // 0x157
    char_type mIsHookDepthChanging; // 0x158
    bool mHookedCarThisFrame; // 0x159
    char_type mUnused15A; // 0x15A
    char_type mUnused15B; // 0x15B
};
GTA2_ASSERT_SIZEOF_ALWAYS(Crane, 0x15C);

class CranePool
{
  public:
    EXPORT void PickUpCar(Car* pCar, u8 crane_idx);
    EXPORT void CranesService();
    EXPORT Crane* NewCrane(Fix16 x_pos, Fix16 y_pos);
    EXPORT CranePool();
    EXPORT ~CranePool();

    Crane mCranes[10]; // 0x0
    s32 mCount; // 0xD98
};
GTA2_ASSERT_SIZEOF_ALWAYS(CranePool, 0xD9C);

EXTERN_GLOBAL(CranePool*, gCranePool_D9C);
