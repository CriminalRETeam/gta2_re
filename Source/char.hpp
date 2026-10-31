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

EXPORT void __stdcall UnpackSignedNibbles_529050(u8 packed, s8* pX, s8* pY);
EXPORT Ang16 __stdcall ComputeShortestAngleDelta_4056C0(Ang16& angle1, Ang16& angle2);

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

    // strange, the same 9.6f func but Ped::StayCloseToTargetPed_46B170 only matches if it pass by ref
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

    // strange, the same 9.6f func but Ped::StayCloseToTargetPed_46B170 only matches if it pass by ref
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
        mRotation.SnapToAng4_405640();
        mRotation += kAng16_6FD940;
        mCharState = Char_B4_state::Veer_Left_From_Wall_9;
        mTimer = 10;
    }

    inline void VeerRightFromWall_4923A0()
    {
        mRotation.SnapToAng4_405640();
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

    EXPORT void DrawFlamesAndStartScreamTimer_545430();
    EXPORT bool HasShadows_5451C0() const;

    EXPORT Fix16_Point GetRunVelocity_545580() const;
    EXPORT void SetRemap_46DD50(u8 remap);

    // Inlined copy of SetRemap_46DD50
    inline void SetRemap_Inline(u8 remap)
    {
        this->mRemap = remap;
        if (remap != 0xFF)
        {
            mSpritePtr->SetRemap(remap);
        }
    }

    EXPORT void RemoveFireSprites_5454B0();
    EXPORT void DoJump_5454D0();
    EXPORT void Teleport_545530(Fix16 xpos, Fix16 ypos, Fix16 zpos);

  private:
    EXPORT s32 IsOnWater_545570() const;
    EXPORT void KillPed_5455F0();

  public:
    EXPORT void ClearCollisionState_545600();

  private:
    EXPORT void GetTileFracX64_545640(Fix16 coord, s16* output) const;
    EXPORT void GetTileFracY64_545670(Fix16 coord, s16* output) const;
    EXPORT void InitSprite_5456A0();

  public:
    EXPORT bool IsOnScreen_545700() const;
    EXPORT void Update_545720(Fix16 distance_to_target);

  private:
    EXPORT void CheckAndHandleCollisions_5459C0();

  public:
    EXPORT void DrownPed_5459E0();

  private:
    EXPORT void UpdateAnimState_546360();
    EXPORT void ManageZCoordAndSlopes_548590();
    EXPORT void DispatchCollision_548670(char_type bIgnorePeds);
    EXPORT void HandleObjectCollision_548840(Object* pObj);
    EXPORT void HandlePedCollision_548BD0(Char* pOther);
    EXPORT void HandleGenericCollision_54A530(Car* pCar, Object* pObj, Char* pChar);
    EXPORT bool ContinueMovementAfterCollision_54B8F0();
    EXPORT void SnapRotationToFace_54C090();
    EXPORT bool CanMoveOntoSlope_54C1A0(s32 path_direction);
    inline void SetTurnTarget_492400(Ang16 target_rotation);
    EXPORT void TurnToFreeAdjacentFace_54C3E0();
    EXPORT bool CanMoveToTile_54C500(char_type x, char_type y);
    EXPORT void SelectRandomIdleBehavior_54C580();
    EXPORT void ApplyRandomRotationJitter_54C6C0();
    EXPORT void TickMovementStateMachine_54C900();
    EXPORT void TurnTowardsAngle_54CAE0();
    EXPORT void ApplyMovement_54CC40();
    EXPORT void CheckCollisionsAndUpdateAnim_54DD70();
    EXPORT void UpdateWalking_54DDF0();
    EXPORT bool CanStepForwardWithRegionCheck_54ECB0(s32 direction);
    EXPORT bool CanStepToTile_54EF60(char_type target_x, char_type target_y);
    EXPORT bool CanStepForward_54FEC0(s32 direction);
    EXPORT bool CanReachTile_550090(u8 xpos, u8 ypos);
    EXPORT void UpdateFleeOrRunning_5504F0();
    EXPORT Ang16 GetNextRotationToward_550F60(Ang16 inputAng);
    EXPORT bool CanStepInDirection_551350(Ang16 angle);
    EXPORT void ChooseNextMovementTile_551400();
    EXPORT void SelectNextTileFast_5516F0();
    EXPORT void UpdateFindingPath_5519F0();
    EXPORT void UpdateEnteringCar_551A00();
    EXPORT void UpdateExitingCar_551B30();
    EXPORT void UpdateEnteringTrain_551BB0();
    EXPORT void UpdateStandingStill_551CB0();
    EXPORT void UpdateImmobilized_5520A0();
    EXPORT void UpdateDead_552E90();

    EXPORT bool IsNearTileCentre_5532C0() const;

  public:
    EXPORT bool IsThreatToSearchingPed_553330() const;
    EXPORT bool ShouldCollideWithSprite_553340(Sprite* pSprite) const;

  private:
    EXPORT bool PhoneTouched_5535B0(Object* pObject);

  public:
    EXPORT bool OnObjectTouched_553640(Object* pObject);
    EXPORT bool HandlePedObjectHit_5537F0(Object* pObject);
    EXPORT void HandleCarImpact_5538A0(Car* pCar, u8 not_used, Fix16 x, Fix16 y);
    EXPORT void HandleGenericImpact_553E00(Ang16 ang, Fix16 impact_x, Fix16 impact_z, char_type impact_type);

  private:
    EXPORT void UpdateLeavingTrain_551CA0();
};
GTA2_ASSERT_SIZEOF_ALWAYS(Char, 0xB4);

EXPORT void __stdcall ResetCharUpdateGlobals_544F70();
EXPORT void __stdcall ResetCharStatics_553F90();

EXTERN_GLOBAL(u8, bThreateningPedAdded_6787EF);

EXTERN_GLOBAL(u16, gNumPedsOnScreen_6787EC);
