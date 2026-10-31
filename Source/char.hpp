#pragma once

#include "char_b4_flags.hpp"
#include "enums.hpp"
#include "BitSet32.hpp"
#include "Function.hpp"
#include "Object_3C.hpp"
#include "Pool.hpp"
#include "ang16.hpp"
#include "sprite.hpp"

class Sprite_3C;
class Ped;
class Car;
class Sprite;

EXTERN_GLOBAL(Fix16, kFpOne256th_678620);
EXTERN_GLOBAL(Fix16, gCharB4_Saved_Xpos_6FD7F8);
EXTERN_GLOBAL(Fix16, gCharB4_Saved_Ypos_6FD800);
EXTERN_GLOBAL(Fix16, gCharB4_Saved_Zpos_6FD7FC);
EXTERN_GLOBAL(Fix16, kFpOne128th_6784BC);

EXTERN_GLOBAL(Ang16, kAng16_6FD940);
EXTERN_GLOBAL(Ang16, kAng344_6FD8F8);

EXPORT void __stdcall UnpackSignedNibbles(u8 packed, s8* pX, s8* pY);
EXPORT Ang16 __stdcall ComputeShortestAngleDelta(Ang16& angle1, Ang16& angle2);

// SpriteList with a (trivial) destructor: the original Char ctor has an EH frame for this member,
// while other owners of a SpriteList (Car, Object_5C, WeaponManager) have none.
class SpriteList_4_dtor : public SpriteList
{
  public:
    ~SpriteList_4_dtor()
    {
    }
};

class Char
{
  public:
    STATIC_MEMBER(Fix16, sCharB4DistanceToTarget);
    // 9.6f 0x41B080
    inline Fix16 get_velocity_41B080() const
    {
        return mVelocity;
    }

    // 9.6f 0x433930
    inline void UseRunOrJumpSpeed_433930()
    {
        mVelocity = mRunOrJumpSpeed;
    }

    // 9.6f 0x4338F0
    inline void SetSpriteNum_4338F0(s32 num)
    {
        mSpritePtr->SetDrawLayer_40F7B0(num);
    }

    // 9.6f 0x41B090
    inline s32 get_ped_state_2_41B090() const
    {
        return mPedState2;
    }

    // 9.6f 0x4338D0
    inline Sprite* get_sprite_ptr_4338D0() const
    {
        return mSpritePtr;
    }

    // 9.6f 0x4338E0
    inline void set_pPed_4338E0(Ped* v)
    {
        mpPed = v;
    }

    s32 mId; // 0x0
    s8 mUnused4; // 0x4
    u8 mRemap; // 0x5
    s8 mUnused6; // 0x6
    s8 mUnused7; // 0x7
    s32 mPedState1; // 0x8
    s32 mPedState2; // 0xC
    s32 mCharState; // 0x10
    Ang16 mTargetRotation; // 0x14
    bool mStateInitPending; // 0x16
    s8 mUnused17; // 0x17
    void* mCollidedEntity; // 0x18
    void* mPrevCollidedEntity; // 0x1C
    s32 mCollidedEntityType; // 0x20
    s32 mSlideState; // 0x24
    Ang16 mSlideRotation; // 0x28
    Ang16 mSlideTurn; // 0x2A
    Ang16 mAng; // 0x2C
    s8 mUnused2E; // 0x2E
    s8 mUnused2F; // 0x2F
    s32 mUnused30; // 0x30
    s16 mUnused34; // 0x34
    s8 mUnused36; // 0x36
    s8 mUnused37; // 0x37
    Fix16 mVelocity; // 0x38
    Fix16 mRunOrJumpSpeed; // 0x3C
    Ang16 mRotation; // 0x40
    Ang16 mRotationJitter; // 0x42
    s8 mBlockType; // 0x44
    u8 mSlopeGradientDirection; // 0x45
    u16 mTimer; // 0x46
    s8 mLyingOnFloorTimer; // 0x48
    s8 mUnused49; // 0x49
    u16 mTimeout; // 0x4A
    Fix16 mConveyorDx; // 0x4C
    Fix16 mConveyorDy; // 0x50
    s8 mJumpScaleCounter; // 0x54
    u8 mNextTileTimer; // 0x55
    s8 mUnused56; // 0x56
    s8 mUnused57; // 0x57
    union
    {
        u32 mFlags; // 0x58
        CharB4Flags32 mFlagsBf; // 0x58
    };
    u8 mJumpOverTimer; // 0x5C
    s8 mUnused5D; // 0x5D
    s8 mUnused5E; // 0x5E
    s8 mUnused5F; // 0x5F
    s32 mUnused60; // 0x60
    s32 mUnused64; // 0x64
    u8 mAnimationFrame; // 0x68
    s8 mIsCollidingWithSprite; // 0x69
    u8 mRepulsionTimer; // 0x6A
    s8 mUnused6B; // 0x6B
    s32 mAnimationState; // 0x6C
    s8 mFrameTimer; // 0x70
    s8 mFrameDelay; // 0x71
    u8 mNextTileX; // 0x72
    u8 mNextTileY; // 0x73
    Ang16 mRepulsionRotation; // 0x74
    s8 mUnused76; // 0x76
    s8 mUnused77; // 0x77
    Char* mpNext;
    Ped* mpPed; // 0x7C
    Sprite* mSpritePtr; // 0x80
    Car* mTargetCar; // 0x84
    SpriteList_4_dtor mObj2c; // 0x88
    Fix16 mJumpBaseZ; // 0x8C
    Fix16 mFallSpeed; // 0x90
    Fix16 mFallZSpeed; // 0x94
    Fix16_Point_POD mVelocityVector; // 0x98
    bool mForceTopLayer; // 0xA0
    s8 mUnusedA1; // 0xA1
    s8 mUnusedA2; // 0xA2
    s8 mUnusedA3; // 0xA3
    Fix16 mXpos; // 0xA4
    Fix16 mYpos; // 0xA8
    Fix16 mZpos; // 0xAC
    s32 mScreamTimer; // 0xB0

    // 9.6f 0x48A4C0
    inline s32 get_ped_state_1_48A4C0() const
    {
        return mPedState1;
    }

    inline void Set_F8_ped_state_1_433910(s32 ped_state)
    {
        mPedState1 = ped_state;
    }

    inline Ang16 get_rotation_433A40() const
    {
        return mRotation;
    }

    void set_rotation_433A30(Ang16 rotation)
    {
        mRotation = rotation;
    }

    // 9.6f 0x4339C0
    inline Fix16 get_sprite_xpos() const
    {
        return mSpritePtr->mXy.x;
    }

    // 9.6f 0x4339E0
    inline Fix16 get_sprite_ypos() const
    {
        return mSpritePtr->mXy.y;
    }

    // 9.6f 0x433A00
    inline Fix16 get_sprite_zpos() const
    {
        return mSpritePtr->mZpos;
    }

    inline void RegulateVelocity_433970(Fix16 threshold)
    {
        if (mVelocity < threshold)
        {
            mVelocity += kFpOne256th_678620;
        }
        else if (mVelocity > threshold)
        {
            mVelocity -= kFpOne256th_678620;
        }
    }

    // strange, the same 9.6f func but Ped::StayCloseToTargetPed only matches if it pass by ref
    inline void RegulateVelocityByRef_433970(Fix16& threshold)
    {
        if (mVelocity < threshold)
        {
            mVelocity += kFpOne256th_678620;
        }
        else if (mVelocity > threshold)
        {
            mVelocity -= kFpOne256th_678620;
        }
    }

    inline void SetVelocity_433920(Fix16 max_speed)
    {
        mVelocity = max_speed;
    }

    // strange, the same 9.6f func but Ped::StayCloseToTargetPed only matches if it pass by ref
    inline void SetVelocityByRef_433920(Fix16& max_speed)
    {
        mVelocity = max_speed;
    }

    inline Ped* get_ped_433A20() const
    {
        return mpPed;
    }

    inline void SetPedState1_433910(s32 new_state)
    {
        mPedState1 = new_state;
    }

    inline void SetPedState2_433A50(s32 new_state)
    {
        mPedState2 = new_state;
    }

    inline void VeerLeftFromWall_4923D0()
    {
        mRotation.SnapToAng4();
        mRotation += kAng16_6FD940;
        mCharState = Char_B4_state::Veer_Left_From_Wall_9;
        mTimer = 10;
    }

    inline void VeerRightFromWall_4923A0()
    {
        mRotation.SnapToAng4();
        mRotation += kAng344_6FD8F8;
        mCharState = Char_B4_state::Veer_Right_From_Wall_8;
        mTimer = 10;
    }

    inline s32 GetCharState_433A80() const
    {
        return mCharState;
    }

    inline void SetCharState_433A60(s32 char_state)
    {
        mCharState = char_state;
    }

    // 9.6f 0x433A90
    inline s8 Get_F44_433A90() const
    {
        return mBlockType;
    }

    // 9.6f 0x403900
    inline Car* Get_F84_403900() const
    {
        return mTargetCar;
    }

    inline void Set_F84_433900(Car* pCar)
    {
        mTargetCar = pCar;
    }

    inline void IncreaseSpeedIfAllowed_433940()
    {
        mVelocity += kFpOne128th_6784BC;
        
        if (mVelocity > mRunOrJumpSpeed)
        {
            mVelocity = mRunOrJumpSpeed;
        }
    }

    inline void AddAngle_4928A0(Ang16 ang)
    {
        mRotation += ang;
    }

    inline bool IsCollidingWithASprite_433AA0() const
    {
        if (mIsCollidingWithSprite)
        {
            return true;
        }
        return false;
    }

    Char();
    ~Char();

    EXPORT void PoolAllocate();
    EXPORT void PoolDeallocate();

    EXPORT void DrawFlamesAndStartScreamTimer();
    EXPORT bool HasShadows() const;

    EXPORT Fix16_Point GetRunVelocity() const;
    EXPORT void SetRemap(u8 remap);

    // Inlined copy of SetRemap
    inline void SetRemap_Inline(u8 remap)
    {
        this->mRemap = remap;
        if (remap != 0xFF)
        {
            mSpritePtr->SetRemap(remap);
        }
    }

    EXPORT void RemoveFireSprites();
    EXPORT void DoJump();
    EXPORT void Teleport(Fix16 xpos, Fix16 ypos, Fix16 zpos);

  private:
    EXPORT s32 IsOnWater() const;
    EXPORT void KillPed();

  public:
    EXPORT void ClearCollisionState();

  private:
    EXPORT void GetTileFracX64(Fix16 coord, s16* output) const;
    EXPORT void GetTileFracY64(Fix16 coord, s16* output) const;
    EXPORT void InitSprite();

  public:
    EXPORT bool IsOnScreen() const;
    EXPORT void Update(Fix16 distance_to_target);

  private:
    EXPORT void CheckAndHandleCollisions();

  public:
    EXPORT void DrownPed();

  private:
    EXPORT void UpdateAnimState();
    EXPORT void ManageZCoordAndSlopes();
    EXPORT void DispatchCollision(char_type bIgnorePeds);
    EXPORT void HandleObjectCollision(Object* pObj);
    EXPORT void HandlePedCollision(Char* pOther);
    EXPORT void HandleGenericCollision(Car* pCar, Object* pObj, Char* pChar);
    EXPORT bool ContinueMovementAfterCollision();
    EXPORT void SnapRotationToFace();
    EXPORT bool CanMoveOntoSlope(s32 path_direction);
    inline void SetTurnTarget_492400(Ang16 target_rotation);
    EXPORT void TurnToFreeAdjacentFace();
    EXPORT bool CanMoveToTile(char_type x, char_type y);
    EXPORT void SelectRandomIdleBehavior();
    EXPORT void ApplyRandomRotationJitter();
    EXPORT void TickMovementStateMachine();
    EXPORT void TurnTowardsAngle();
    EXPORT void ApplyMovement();
    EXPORT void CheckCollisionsAndUpdateAnim();
    EXPORT void UpdateWalking();
    EXPORT bool CanStepForwardWithRegionCheck(s32 direction);
    EXPORT bool CanStepToTile(char_type target_x, char_type target_y);
    EXPORT bool CanStepForward(s32 direction);
    EXPORT bool CanReachTile(u8 xpos, u8 ypos);
    EXPORT void UpdateFleeOrRunning();
    EXPORT Ang16 GetNextRotationToward(Ang16 inputAng);
    EXPORT bool CanStepInDirection(Ang16 angle);
    EXPORT void ChooseNextMovementTile();
    EXPORT void SelectNextTileFast();
    EXPORT void UpdateFindingPath();
    EXPORT void UpdateEnteringCar();
    EXPORT void UpdateExitingCar();
    EXPORT void UpdateEnteringTrain();
    EXPORT void UpdateStandingStill();
    EXPORT void UpdateImmobilized();
    EXPORT void UpdateDead();

    EXPORT bool IsNearTileCentre() const;

  public:
    EXPORT bool IsThreatToSearchingPed() const;
    EXPORT bool ShouldCollideWithSprite(Sprite* pSprite) const;

  private:
    EXPORT bool PhoneTouched(Object* pObject);

  public:
    EXPORT bool OnObjectTouched(Object* pObject);
    EXPORT bool HandlePedObjectHit(Object* pObject);
    EXPORT void HandleCarImpact(Car* pCar, u8 not_used, Fix16 x, Fix16 y);
    EXPORT void HandleGenericImpact(Ang16 ang, Fix16 impact_x, Fix16 impact_z, char_type impact_type);

  private:
    EXPORT void UpdateLeavingTrain();
};
GTA2_ASSERT_SIZEOF_ALWAYS(Char, 0xB4);

EXPORT void __stdcall ResetCharUpdateGlobals();
EXPORT void __stdcall ResetCharStatics();

EXTERN_GLOBAL(u8, bThreateningPedAdded_6787EF);

EXTERN_GLOBAL(u16, gNumPedsOnScreen_6787EC);
