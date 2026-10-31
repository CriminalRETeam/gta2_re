#define FIX16_POINT_ZERO kFP16Zero_6FD9E4
#include "ped_death_cause.hpp"
#include "ped_jump_over_mode.hpp"
#include "char_b4_state_more.hpp"
#include "char_anim_state_more.hpp"
#include "ped_voice_event.hpp"
#include "explosion_type.hpp"
#include "voice_line.hpp"
#include "char.hpp"
#include "CarAI.hpp"
#include "CarPhysics.hpp"
#include "Car.hpp"
#include "Char_Pool.hpp"
#include "DoorPool.hpp"
#include "Game.hpp"
#include "Gang.hpp"
#include "Garage.hpp"
#include "Globals.hpp"
#include "Hud.hpp"
#include "Object_3C.hpp"
#include "Object_5C.hpp"
#include "ParticleManager.hpp"
#include "PedGroup.hpp"
#include "Player.hpp"
#include "Police.hpp"
#include "SpriteGrid.hpp"
#include "PedRefTable.hpp"
#include "Weapon.hpp"
#include "debug.hpp"
#include "error.hpp"
#include "ScriptManager.hpp"
#include "rng.hpp"
#include "root_sound.hpp"
#include "sprite.hpp"
#include "winmain.hpp"
#include "ped_graphic_type.hpp"
#include "block_face.hpp"

// Ped.cpp
EXTERN_GLOBAL(Fix16, kFpPoint02_6FD9AC);
EXTERN_GLOBAL(Fix16, kFpPoint3_6FD830);

DEFINE_STATIC_GLOBAL(s8, sCharB4UpdateCounter, 0x6FDB48);
DEFINE_STATIC_GLOBAL(u32, sB4Id, 0x6FDB4C);

DEFINE_STATIC_GLOBAL_INIT(s32, sCharB4PathDirection, path_direction::up_1, 0x623F44);
DEFINE_STATIC_MEMBER(Fix16, Char, sCharB4DistanceToTarget, 0x6FD80C);

DEFINE_GLOBAL_INIT(Ang16, kAng180_6FD936, Ang16(720), 0x6FD936);
DEFINE_GLOBAL_INIT(Ang16, kAng315_6FD938, Ang16(1260), 0x6FD938);

DEFINE_GLOBAL(u8, gCharB4_Unused_6FDB55, 0x6FDB55);
DEFINE_STATIC_GLOBAL_INIT(Ang16, kAng4, Ang16(16), 0x6FDB2E);

DEFINE_STATIC_GLOBAL(u8, sCharCollisionNoJump, 0x6FDB58);
DEFINE_GLOBAL(u8, gCharB4_HitByMine_6FDB59, 0x6FDB59);

DEFINE_GLOBAL_INIT(Fix16, kFP16Zero_6FD9E4, Fix16(0), 0x6FD9E4);
DEFINE_GLOBAL_INIT(Fix16, gCharB4_Saved_Zpos, kFP16Zero_6FD9E4, 0x6FD7FC);
DEFINE_GLOBAL_INIT(Fix16, gCharB4_Saved_Ypos, kFP16Zero_6FD9E4, 0x6FD800);
DEFINE_GLOBAL_INIT(Fix16, gCharB4_Saved_Xpos, kFP16Zero_6FD9E4, 0x6FD7F8);
DEFINE_GLOBAL_INIT(Fix16, kFP16Four_6FD9F4, Fix16(4), 0x6FD9F4);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFP16Quarter, Fix16(0x1000, 0), 0x6FD7A4);
DEFINE_STATIC_GLOBAL_INIT(Fix16, sCharB4SavedSlopeGradDir, kFP16Zero_6FD9E4, 0x6FD7B0);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kZeroVelocity, kFP16Zero_6FD9E4, 0x6FD7C0);
DEFINE_GLOBAL_INIT(Fix16, gCharB4_Unused_6FD7DC, kFP16Zero_6FD9E4, 0x6FD7DC);
DEFINE_GLOBAL_INIT(Fix16, kFpOne64th_6FD868, Fix16(256, 0), 0x6FD868);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFP16Half, Fix16(0x2000, 0), 0x6FD9B4);
DEFINE_STATIC_GLOBAL_INIT(Fix16, sRunOrJumpSpeed, kFP16Four_6FD9F4* kFpOne64th_6FD868, 0x6FD7D0);
DEFINE_GLOBAL_INIT(Fix16, gCharB4_Unused_6FD8B4, kFP16Zero_6FD9E4, 0x6FD8B4);
DEFINE_GLOBAL_INIT(Fix16, gCharB4_StepXpos, kFP16Zero_6FD9E4, 0x6FD8B8);
DEFINE_STATIC_GLOBAL_INIT(Fix16, sCharB4StepYpos, kFP16Zero_6FD9E4, 0x6FD8BC);
DEFINE_GLOBAL_INIT(Fix16, kFpPoint2_6FD82C, Fix16(0xCCC, 0), 0x6FD82C);
DEFINE_GLOBAL_INIT(Fix16, gCharB4_WorldCollisionOffset, kFpPoint2_6FD82C, 0x6FD8D8);
DEFINE_GLOBAL_INIT(Fix16, kFpOneThirtySecond_6FD870, kFpOne64th_6FD868 * 2, 0x6FD870);
DEFINE_GLOBAL_INIT(Fix16, k_CollisionRepulsionSpeed, kFpOneThirtySecond_6FD870, 0x6FD7BC);
DEFINE_GLOBAL_INIT(Fix16, kFpPoint05_6FD9B0, Fix16(0x333, 0), 0x6FD9B0);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpPoint05, kFpPoint05_6FD9B0, 0x6FDAE4);
DEFINE_GLOBAL_INIT(Fix16, gFix16_Two, Fix16(2), 0x6FD9EC);
DEFINE_GLOBAL_INIT(Fix16, kFP16One_6FD9E8, Fix16(1), 0x6FD9E8);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpPoint03, Fix16(0x1EB, 0), 0x6FD9A8);
DEFINE_GLOBAL_INIT(Fix16, kFP16Three_6FD9F0, Fix16(3), 0x6FD9F0);
DEFINE_GLOBAL_INIT(Fix16, kFP16Five_6FD9F8, Fix16(5), 0x6FD9F8);
DEFINE_GLOBAL_INIT(Fix16, kFP16Six, Fix16(6), 0x6FD9FC);
DEFINE_GLOBAL_INIT(Fix16, kFP16Seven, Fix16(7), 0x6FDA00);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpThreeThirtySeconds, kFpOne64th_6FD868 * 6, 0x6FDAC8);
DEFINE_GLOBAL_INIT(Fix16, kFpOne256th_6FD99C, kFpOne64th_6FD868 / kFP16Four_6FD9F4, 0x6FD99C);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpOne64th, kFpOne64th_6FD868, 0x6FD7B8);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpThreeSixtyFourths, kFP16Three_6FD9F0* kFpOne64th_6FD868, 0x6FD7CC);

DEFINE_GLOBAL_INIT(Fix16, kFpOne_6FDB20, kFpOne64th_6FD868 * 64, 0x6FDB20);

DEFINE_GLOBAL_INIT(Ang16, kAng12_6FD892, Ang16(48), 0x6FD892);

DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpPoint1, Fix16(0x666, 0), 0x6FD8DC);
DEFINE_GLOBAL_INIT(Fix16, kFP16Half_6FD8E4, Fix16(0x2000, 0), 0x6FD8E4);
DEFINE_GLOBAL_INIT(Fix16, kFpHalf_6FDB28, kFP16Half_6FD8E4, 0x6FDB28);

DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpOneSixteenth, kFpOne64th_6FD868 * 4, 0x6FD87C);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpOneEighth, kFpOne64th_6FD868 * 8, 0x6FD88C);
DEFINE_GLOBAL_INIT(Fix16, kFP16Quarter_6FD828, Fix16(0x1000, 0), 0x6FD828);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFP16Eighth, Fix16(0x800, 0), 0x6FDB04);
DEFINE_GLOBAL_INIT(Fix16, kFpPoint01_6FD9A0, Fix16(0xA3, 0), 0x6FD9A0);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpOneThirtySecond, Fix16(0x200, 0), 0x6FDB24);

DEFINE_GLOBAL_INIT(Fix16, kFpOne64th_6FDA9C, Fix16(0x100, 0), 0x6FDA9C);

DEFINE_GLOBAL(u16, gNumPedsOnScreen, 0x6787EC);

DEFINE_STATIC_GLOBAL(u8, sCharB4TurnToFreeFace, 0x6FDB51);
DEFINE_GLOBAL(u8, gCharB4_VeerFromWalls, 0x6FDB52);
DEFINE_GLOBAL(u8, gCharB4_Unused_6FDB53, 0x6FDB53);
DEFINE_STATIC_GLOBAL(u8, sCharB4GradientSlopeBelow, 0x6FDB54);

DEFINE_STATIC_GLOBAL(u8, sCharB4MoveIgnorePeds, 0x6FDB56);
DEFINE_STATIC_GLOBAL_INIT(u8, sCharB4IgnorePeds, 1, 0x623F48);
DEFINE_STATIC_GLOBAL(u8, sCharB4SavedTileX, 0x6FDAD8);
DEFINE_STATIC_GLOBAL(u8, sCharB4SavedTileY, 0x6FDAD9);
DEFINE_STATIC_GLOBAL(u8, sCharB4StepPosValid, 0x6FDB57);

DEFINE_STATIC_GLOBAL_INIT(Ang16, sAng16AngleOfCollision, Ang16(0), 0x6FD808);
DEFINE_GLOBAL_INIT(Ang16, kAng90_6FD8A2, Ang16(360), 0x6FD8A2);
DEFINE_GLOBAL_INIT(Ang16, kAng16_6FD940, Ang16(64), 0x6FD940);
DEFINE_GLOBAL_INIT(Ang16, kAng344, Ang16(1376), 0x6FD8F8);
DEFINE_GLOBAL_INIT(Ang16, kAng270_6FD94C, Ang16(1080), 0x6FD94C);
DEFINE_GLOBAL_INIT(Ang16, kAng270_6FD95C, Ang16(1080), 0x6FD95C);
DEFINE_GLOBAL_INIT(Ang16, kAng90_6FD854, Ang16(360), 0x6FD854);

DEFINE_GLOBAL_INIT(Ang16, kAng180_6FD8E8, Ang16(0x2D0), 0x6FD8E8);
DEFINE_STATIC_GLOBAL(Ang16, kAng0, 0x6FDB3C);
DEFINE_GLOBAL_INIT(Ang16, kAng90_6FDA64, Ang16(0x168), 0x6FDA64);
DEFINE_STATIC_GLOBAL_INIT(Ang16, kAng270, Ang16(0x438), 0x6FD904);

DEFINE_STATIC_GLOBAL_INIT(Ang16, kAng6, Ang16(0x18), 0x6FDA54); // TODO: Init via func 0x54A300
DEFINE_GLOBAL_INIT(Ang16, kAng180_6FD920, kAng180_6FD936, 0x6FD920);
DEFINE_GLOBAL_INIT(Ang16, kAng354, Ang16(0x588), 0x6FD9D8); // TODO: Init via func 0x54A270
DEFINE_STATIC_GLOBAL_INIT(Ang16, kAng24, Ang16(96), 0x6FD890);
DEFINE_GLOBAL_INIT(Ang16, kAng45_6FD89C, Ang16(180), 0x6FD89C);

DEFINE_GLOBAL_INIT(Fix16, kFpHalf_6FDAB0, kFP16Half_6FD8E4, 0x6FDAB0);

EXTERN_GLOBAL(Ang16, kAng0_6FDB34);

DEFINE_GLOBAL_INIT(Fix16, kFP16Half_6F67B0, Fix16(0x2000, 0), 0x6F67B0);
DEFINE_GLOBAL_INIT(Fix16, kFpHalf_6FDB18, kFpOne64th_6FD868 * 32, 0x6FDB18);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpThreeSixteenths, kFpOne64th_6FD868 * 12, 0x6FDB08);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpPoint3, Fix16(0x1333, 0), 0x6FD91C);
DEFINE_GLOBAL_INIT(Fix16, kFP16Eight_6FDA08, Fix16(8), 0x6FDA08);
DEFINE_STATIC_GLOBAL_INIT(Fix16, sFP16CollisionCheckRadius, Fix16(0x800, 0), 0x6FDACC);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFP16MinusOne, Fix16(-1), 0x6FD790);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpFour, kFpOne64th_6FD868 * 256, 0x6FDA04);


MATCH_FUNC(0x4056C0)
EXPORT Ang16 __stdcall ComputeShortestAngleDelta(Ang16& angle1, Ang16& angle2)
{
    Ang16 delta = angle1 - angle2;
    if (delta > kAng180_669156)
    {
        delta = -delta;
    }
    return delta;
}

// No callers. The angle halfway between a and b, going the short way round. The normalizations of delta
// are kept at the top level (in a register, where VC6 turns the loops into its division form): through the
// unary operator- the nested Normalize runs out of inline budget and is called out of line.
MATCH_FUNC(0x4057B0)
EXPORT Ang16 __stdcall MidAngle(Ang16& a, Ang16& b)
{
    Ang16 half;
    Ang16 delta(b.rValue - a.rValue);
    delta.Normalize();
    if (delta > kAng180_669156)
    {
        delta.rValue = -delta.rValue;
        delta.Normalize();
        half = Ang16(Fix16(delta.rValue) / Fix16(2));
        half.Normalize_406C20();
        return a - half;
    }
    half = Ang16(Fix16(delta.rValue) / Fix16(2));
    half.Normalize_406C20();
    return a + half;
}

MATCH_FUNC(0x46DD50)
void Char::SetRemap(u8 remap)
{
    this->mRemap = remap;
    if (remap != 0xFF)
    {
        mSpritePtr->SetRemap(remap);
    }
}

MATCH_FUNC(0x4F79B0)
EXPORT Fix16 __stdcall SnapZTo16(Fix16 zpos)
{
    return ((kFP16Half_6F67B0 + (zpos * 1000)).GetFloorValue()) / 1000;
}

MATCH_FUNC(0x529050)
EXPORT void __stdcall UnpackSignedNibbles(u8 packed, s8* pX, s8* pY)
{
    *pX = (packed >> 4) - 7;
    *pY = (packed & 0xF) - 7;
}

//https://decomp.me/scratch/iQH9l
MATCH_FUNC(0x544F70)
void __stdcall ResetCharUpdateGlobals()
{
    gCharB4_Saved_Xpos = kFP16Zero_6FD9E4;
    gCharB4_Saved_Ypos = kFP16Zero_6FD9E4;
    gCharB4_Saved_Zpos = kFP16Zero_6FD9E4;
    gCharB4_Unused_6FD7DC = kFP16Zero_6FD9E4;
    sCharB4SavedSlopeGradDir = kFP16Zero_6FD9E4;
    gCharB4_StepXpos = kFP16Zero_6FD9E4;
    sCharB4StepYpos = kFP16Zero_6FD9E4;
    gCharB4_Unused_6FD8B4 = kFP16Zero_6FD9E4;
    sCharB4TurnToFreeFace = 0;
    gCharB4_VeerFromWalls = 0;
    gCharB4_Unused_6FDB53 = 0;
    sCharB4GradientSlopeBelow = 0;
    gCharB4_Unused_6FDB55 = 0;
    sCharB4PathDirection = path_direction::up_1;
    sCharB4MoveIgnorePeds = 0;
    sCharB4IgnorePeds = 1;
    sCharB4SavedTileX = kFP16Zero_6FD9E4.ToUInt8();
    sCharB4SavedTileY = kFP16Zero_6FD9E4.ToUInt8();
    sCharB4StepPosValid = 0;
    sCharCollisionNoJump = 0;
}

// https://decomp.me/scratch/ZsDjc
MATCH_FUNC(0x544ff0)
Char::Char()
{
    mId = 0;
    mUnused4 = 0;
    mRemap = -1;
    mUnused6 = 0;
    mPedState1 = 11;
    mPedState2 = ped_state_2::initial_28;
    mCharState = Char_B4_state::Interacting_Car_Door_36;
    mTargetRotation = kAng0_6FDB34;
    mStateInitPending = false;
    mCollidedEntity = NULL;
    mPrevCollidedEntity = NULL;
    mCollidedEntityType = 0;
    mSlideState = 3;
    mSlideRotation = kAng0_6FDB34;
    mSlideTurn = kAng0_6FDB34;
    mAng = kAng0_6FDB34;
    mUnused30 = 4;
    mUnused34 = 0;
    mVelocity = kZeroVelocity;
    mRunOrJumpSpeed = sRunOrJumpSpeed;
    mRotation = kAng0_6FDB34;
    mRotationJitter = kAng0_6FDB34;
    mBlockType = 0;
    mSlopeGradientDirection = 0;
    mJumpOverTimer = 0;
    mTimer = 0;
    mLyingOnFloorTimer = 0;
    mpNext = NULL;
    mpPed = NULL;
    mSpritePtr = NULL;
    mAnimationFrame = 0;
    mIsCollidingWithSprite = false;
    mRepulsionRotation = kAng0_6FDB34;
    mRepulsionTimer = 0;
    mTargetCar = NULL;
    mFlagsBf.bOnSlope = false;
    mObj2c.DestroyAllSprites();
    mJumpBaseZ = kFP16Zero_6FD9E4;
    mFlagsBf.bFlag2 = false;
    mAnimationState = Char_Anim_state::Initial_18;
    mFrameTimer = 0;
    mFrameDelay = 0;
    mFallSpeed = kFP16Zero_6FD9E4;
    mFallZSpeed = kFP16Zero_6FD9E4;
    mFlagsBf.bFlag1 = false;
    mVelocityVector.x = kFP16Zero_6FD9E4;
    mVelocityVector.y = kFP16Zero_6FD9E4;
    mFlagsBf.bReverseVelocity = false;
    mFlagsBf.bTeleported = false;
    mXpos = kFP16Zero_6FD9E4;
    mYpos = kFP16Zero_6FD9E4;
    mZpos = kFP16Zero_6FD9E4;
    mTimeout = 0;
}

MATCH_FUNC(0x5451A0)
Char::~Char()
{
    mCollidedEntity = NULL;
    mPrevCollidedEntity = NULL;
    mpNext = NULL;
    mpPed = NULL;
    mSpritePtr = NULL;
    mTargetCar = NULL;
}

MATCH_FUNC(0x5451C0)
bool Char::HasShadows() const
{
    if (mPedState1 == ped_state_1::dead_9)
    {
        return false;
    }

    if (mPedState2 != ped_state_2::lying_on_floor_22 && mCharState != Char_B4_state::Jumping_15 && mPedState2 != ped_state_2::electrocuted_27 &&
        !mpPed->IsInvisibleOnFoot_433DA0())
    {
        return true;
    }

    return false;
}

MATCH_FUNC(0x545200)
void Char::PoolAllocate()
{
    mId = sB4Id++;
    Char::InitSprite();
    mUnused4 = 1;
    mRemap = -1;
    mUnused6 = 0;
    mPedState1 = 11;
    mPedState2 = ped_state_2::initial_28;
    mCharState = Char_B4_state::Interacting_Car_Door_36;
    mTargetRotation = kAng0_6FDB34;
    mStateInitPending = false;
    mCollidedEntity = NULL;
    mPrevCollidedEntity = NULL;
    mCollidedEntityType = 0;
    mSlideState = 3;
    mSlideRotation = kAng0_6FDB34;
    mSlideTurn = kAng0_6FDB34;
    mAng = kAng0_6FDB34;
    mUnused30 = 4;
    mUnused34 = 0;
    mVelocity = kZeroVelocity;
    mRunOrJumpSpeed = sRunOrJumpSpeed;
    mRotation = kAng0_6FDB34;
    mRotationJitter = kAng0_6FDB34;
    mBlockType = 0;
    mSlopeGradientDirection = 0;
    mJumpOverTimer = 0;
    mTimer = 0;
    mLyingOnFloorTimer = 0;
    mTimeout = 500;
    mpNext = NULL;
    mpPed = NULL;
    mAnimationFrame = 0;
    mIsCollidingWithSprite = false;
    mRepulsionRotation = kAng0_6FDB34;
    mRepulsionTimer = 0;
    mTargetCar = NULL;
    mFlagsBf.bOnSlope = false;
    mObj2c.DestroyAllSprites();
    mJumpBaseZ = kFP16Zero_6FD9E4;
    mFlagsBf.bFlag2 = false;
    mAnimationState = Char_Anim_state::Initial_18;
    mFrameTimer = 0;
    mFrameDelay = 0;
    mFallSpeed = kFP16Zero_6FD9E4;
    mFallZSpeed = kFP16Zero_6FD9E4;
    mFlagsBf.bFlag1 = false;
    mVelocityVector.x = kFP16Zero_6FD9E4;
    mVelocityVector.y = kFP16Zero_6FD9E4;
    mFlagsBf.bTeleported = false;
    mFlagsBf.bReverseVelocity = false;
    mXpos = kFP16Zero_6FD9E4;
    mYpos = kFP16Zero_6FD9E4;
    mZpos = kFP16Zero_6FD9E4;
    mFlagsBf.bDoorOffsetLarge = false;
    mConveyorDx = kFP16Zero_6FD9E4;
    mConveyorDy = kFP16Zero_6FD9E4;
    mNextTileX = kFP16Zero_6FD9E4.ToInt();
    mNextTileY = kFP16Zero_6FD9E4.ToInt();
    mFlagsBf.bJumpedOffEdge = false;
    mFlagsBf.bDetouring = false;
    mUnused60 = 0;
    mUnused64 = 0;
    mNextTileTimer = 0;
    mForceTopLayer = false;
    mScreamTimer = -1;
}

MATCH_FUNC(0x5453d0)
void Char::PoolDeallocate()
{
    if (mSpritePtr)
    {
        gSpriteGrid_1->RemoveFromRegionBuckets(mSpritePtr);
        gSpritePool->remove(mSpritePtr);
        mSpritePtr = NULL;
    }
    mObj2c.DestroyAllSprites();
    mScreamTimer = -1;
}

MATCH_FUNC(0x545430)
void Char::DrawFlamesAndStartScreamTimer()
{
    // Spawn fire
    Object* pObject = gObject_5C->NewPhysicsObj(objects::fire_197, 0, 0, 0, kAng0_6FDB34);
    mSpritePtr->AttachSprite(pObject->mSprite, 0, 0, kAng0_6FDB34);
    mScreamTimer = 10; // Start screaming timer
}

MATCH_FUNC(0x5454B0)
void Char::RemoveFireSprites()
{
    mScreamTimer = -1;
    mObj2c.CleanupSpriteList();
}

MATCH_FUNC(0x5454d0)
void Char::DoJump()
{
    if (mPedState1 != ped_state_1::immobilized_8)
    {
        if (mCharState == Char_B4_state::Jumping_15 && mAnimationState == Char_Anim_state::Jumping_5)
        {
            if (mAnimationFrame >= 5u)
            {
                mAnimationFrame = 5;
                mFrameDelay = 2;
                mFrameTimer = 0;
            }
        }
        else
        {
            mCharState = Char_B4_state::Jumping_15;
            mAnimationState = Char_Anim_state::Jumping_5;
            mAnimationFrame = 0;
            mVelocity = sRunOrJumpSpeed;
            mJumpBaseZ = Fix16(mSpritePtr->mZpos.ToUInt8());
        }
    }
}

MATCH_FUNC(0x545530)
void Char::Teleport(Fix16 xpos, Fix16 ypos, Fix16 zpos)
{
    mFlagsBf.bTeleported = true;
    mXpos = xpos;
    mYpos = ypos;
    mZpos = zpos;
}

MATCH_FUNC(0x545570)
s32 Char::IsOnWater() const
{
    return mSpritePtr->IsOnWater();
}

// 9.6f 0x493780
MATCH_FUNC(0x545580)
Fix16_Point Char::GetRunVelocity() const
{
    Fix16_Point p;
    p.FromPolar_41E210(-sRunOrJumpSpeed, mSpritePtr->mRotation);
    return p;
}

MATCH_FUNC(0x5455f0)
void Char::KillPed()
{
    mpPed->Kill();
}

MATCH_FUNC(0x545600)
void Char::ClearCollisionState()
{
    mCollidedEntity = NULL;
    mPrevCollidedEntity = NULL;
    mCollidedEntityType = 0;
    mAng = kAng0_6FDB34;
    mIsCollidingWithSprite = false;
    mSlideState = 0;
    mSlideRotation = kAng0_6FDB34;
    mSlideTurn = kAng0_6FDB34;
    mAng = kAng0_6FDB34;
}

MATCH_FUNC(0x545640)
void Char::GetTileFracX64(Fix16 coord, s16* output) const
{
    *output = (coord.GetFracValue() * Fix16(64)).ToInt();
}

MATCH_FUNC(0x545670)
void Char::GetTileFracY64(Fix16 coord, s16* output) const
{
    *output = (coord.GetFracValue() * Fix16(64)).ToInt();
}

MATCH_FUNC(0x5456a0)
void Char::InitSprite()
{
    Sprite* pFirst = gSpritePool->get_new_sprite();
    mSpritePtr = pFirst;
    pFirst->SetType(sprite_types_enum::ped_3);
    mSpritePtr->AllocCollisionShape(gCharB4_WorldCollisionOffset, gCharB4_WorldCollisionOffset, kFP16Quarter);
    mSpritePtr->SetChar(this);
    mSpritePtr->CreateSoundObj();
}

MATCH_FUNC(0x545700)
bool Char::IsOnScreen() const
{
    return gGame_0x40->IsSpriteInAnyPlayerBounds(this->mSpritePtr, kFP16Zero_6FD9E4) == 1;
}

MATCH_FUNC(0x545720)
void Char::Update(Fix16 distance_to_target)
{
    if (++sCharB4UpdateCounter > 20)
    {
        sCharB4UpdateCounter = 0;
    }
    if (mTimeout > 0)
    {
        mTimeout--;
    }
    sCharB4DistanceToTarget = distance_to_target;

    if (mJumpOverTimer > 0)
    {
        mJumpOverTimer--;
    }
    ResetCharUpdateGlobals();
    gCharB4_Saved_Xpos = mSpritePtr->mXy.x;
    gCharB4_Saved_Ypos = mSpritePtr->mXy.y;
    gCharB4_Saved_Zpos = mSpritePtr->mZpos;
    sCharB4SavedSlopeGradDir = Fix16(mSlopeGradientDirection);
    gCharB4_Unused_6FDB55 = 0;
    sCharCollisionNoJump = 0;

    gSpriteGrid_1->RemoveFromRegionBuckets(mSpritePtr);

    if (mFlagsBf.bTeleported)
    {
        mSpritePtr->set_xyz_lazy_420600(mXpos, mYpos, mZpos);
        mFlagsBf.bTeleported = false;
    }
    else
    {
        switch (mPedState1)
        {
            case ped_state_1::walking_0:
                Char::UpdateWalking();
                break;
            case ped_state_1::flee_or_running_1:
                Char::UpdateFleeOrRunning();
                break;
            case ped_state_1::findind_path_2:
                Char::UpdateFindingPath();
                break;
            case ped_state_1::entering_car_3:
                Char::UpdateEnteringCar();
                break;
            case ped_state_1::exiting_car_4:
                Char::UpdateExitingCar();
                break;
            case ped_state_1::entering_train_5:
                Char::UpdateEnteringTrain();
                break;
            case ped_state_1::leaving_train_6:
                UpdateLeavingTrain();
                break;
            case ped_state_1::standing_still_7:
                Char::UpdateStandingStill();
                break;
            case ped_state_1::immobilized_8:
                Char::UpdateImmobilized();
                break;
            case ped_state_1::dead_9:
                Char::UpdateDead();
                break;
            default:
                break;
        }
        if (mForceTopLayer)
        {
            if (mPedState1 != ped_state_1::dead_9)
            {
                mSpritePtr->SetDrawLayer(34);
            }
        }
        if (mCharState == Char_B4_state::Jumping_15)
        {
            if (mAnimationState != Char_Anim_state::Jumping_5 && mPedState1 != ped_state_1::dead_9)
            {
                Char::DoJump();
            }
        }
        else
        {
            mForceTopLayer = false;
        }
        Char::UpdateAnimState();

        mSpritePtr->set_ang_lazy(mRotation);

        if (mFlagsBf.bReverseVelocity)
        {
            mVelocityVector.SetFromPolar_41E210(-mVelocity, mRotation);
        }
        else
        {
            mVelocityVector.SetFromPolar_41E210(mVelocity, mRotation);
        }
    }
    gSpriteGrid_1->AddToRegionBuckets(mSpritePtr);
    if (mObj2c.mP18)
    {
        mObj2c.PoolUpdate(mSpritePtr);
    }
    if (mRepulsionTimer > 0)
    {
        mRepulsionTimer--;
    }
}

MATCH_FUNC(0x5459c0)
void Char::CheckAndHandleCollisions()
{
    gCharB4_HitByMine_6FDB59 = 0;
    gSpriteGrid_2->CheckAndHandleCollisionInStrips(mSpritePtr);
}

MATCH_FUNC(0x5459e0)
void Char::DrownPed()
{
    mpPed->ChangeNextPedState1(ped_state_1::immobilized_8);
    mpPed->ChangeNextPedState2(ped_state_2::sinking_20);
    mStateInitPending = true;

    gParticle_8->EmitWaterSplash(mSpritePtr->mXy.x,
                                               mSpritePtr->mXy.y,
                                               mSpritePtr->mZpos,
                                               kAng180_6FD936 + mSpritePtr->mRotation,
                                               1);
    mpPed->SetVoiceEventIfNotOnFire(28);
    s32 ped_killer_id = mpPed->mKillerId;
    if (ped_killer_id)
    {
        if (gPedManager->PedById(ped_killer_id))
        {
            mpPed->mDeathCause = ped_death_cause::drowned_5;
            mpPed->mKillerIdTimer = 50;
        }
    }
}

// 9.6f 0x497DF0
// By-value angle, like TurnBy16Deg_548BD0: the copy makes VC6 load field_40 first and add kAng180
static inline Ang16 TurnBy180Deg_546360(Ang16 a)
{
    return a + kAng180_6FD936;
}

// Frame delay of the lying-on-floor animations (cases 15/16): an if/return inline returning u8
// gives the original's byte compare with the delay in al and field_68 kept in cl
static inline u8 FrameDelay_546360(Char* pThis)
{
    if (pThis->mAnimationFrame > 5)
    {
        return 3;
    }
    return 4;
}

MATCH_FUNC(0x546360)
void Char::UpdateAnimState()
{
    s16 newId = 0;
    u8 frame_limit = 0;

    mFrameTimer++;

    Fix16 groundZ = kFP16Zero_6FD9E4;
    Fix16 newx = kFP16Zero_6FD9E4;
    Fix16 newy = kFP16Zero_6FD9E4;

    // Declared here, before the switch, to get the original stack layout
    u8 weapon_ofs = 0;
    bool bThrowWeapon = false;
    s32 baseId;

    switch (mpPed->mGraphicType)
    {
        case ped_graphic_type::civilian_0:
            baseId = 0;
            break;
        case ped_graphic_type::character_1:
            baseId = 158;
            break;
        case ped_graphic_type::cop_2:
            baseId = 316;
            break;
        default:
            baseId = 158;
            break;
    }

    if (mpPed->get_remap() > -1)
    {
        mSpritePtr->SetRemap(static_cast<u8>(mpPed->get_remap()));
    }
    else
    {
        mSpritePtr->SetPaletteSprites();
    }

    if (mpPed->mPlayer && mPedState2 != ped_state_2::falling_19)
    {
        if (mpPed->mSelectedWeapon &&
            (mpPed->mSelectedWeapon->mWeaponType == weapon_type::molotov ||
             mpPed->mSelectedWeapon->mWeaponType == weapon_type::grenade))
        {
            bThrowWeapon = true;
        }

        if (mpPed->mSelectedWeapon && !mpPed->mBf21C.bAttacking)
        {
            if ((mpPed->mSelectedWeapon->mWeaponType == weapon_type::molotov ||
                 mpPed->mSelectedWeapon->mWeaponType == weapon_type::grenade) &&
                mpPed->mPlayer->mAttackChanged)
            {
                mAnimationState = Char_Anim_state::Punching_4;
                mAnimationFrame = 0;
            }
        }
        else if (mpPed->mSelectedWeapon && mpPed->mBf21C.bAttacking)
        {
            if (mpPed->mSelectedWeapon->mWeaponType == weapon_type::molotov ||
                mpPed->mSelectedWeapon->mWeaponType == weapon_type::grenade)
            {
                mpPed->mBf21C.bAttacking = false;
            }
        }
    }
    else
    {
        if (mpPed->mSelectedWeapon && mpPed->mBf21C.bAttacking &&
            (mpPed->mSelectedWeapon->mWeaponType == weapon_type::molotov ||
             mpPed->mSelectedWeapon->mWeaponType == weapon_type::grenade))
        {
            mAnimationState = Char_Anim_state::Punching_4;
            bThrowWeapon = true;
        }
    }

    switch (mAnimationState)
    {
        case Char_Anim_state::Walking_0:
        {
            if (static_cast<u8>(mFrameTimer) > 2)
            {
                mAnimationFrame++;
                if (mAnimationFrame > 7)
                {
                    mAnimationFrame = 0;
                }
                mFrameTimer = 0;
            }

            if (mpPed->mSelectedWeapon && !bThrowWeapon)
            {
                weapon_ofs = 37;
            }
            if (mCharState == 0)
            {
                if (mAnimationFrame > 5)
                {
                    mAnimationFrame = 0;
                }
                newId = mAnimationFrame + baseId + 143;
            }
            else
            {
                newId = baseId + weapon_ofs + mAnimationFrame;
            }
            break;
        }

        case Char_Anim_state::Running_1:
        {
            if (static_cast<u8>(mFrameTimer) > 1)
            {
                mAnimationFrame++;
                if (mAnimationFrame > 7)
                {
                    mAnimationFrame = 0;
                }
                mFrameTimer = 0;
            }

            if (mpPed->mSelectedWeapon && !bThrowWeapon)
            {
                weapon_ofs = 37;
            }
            if (mCharState == 0)
            {
                if (mAnimationFrame > 5)
                {
                    mAnimationFrame = 0;
                }
                newId = mAnimationFrame + baseId + 135;
            }
            else
            {
                newId = baseId + weapon_ofs + mAnimationFrame + 8;
            }
            break;
        }

        case Char_Anim_state::Standing_2:
            if (mpPed->mBf21C.bAttacking == 1)
            {
                newId = baseId + 139;
            }
            else
            {
                u8 smoke_ofs;
                if (mCharState != Char_B4_state::Smoking_35)
                {
                    if (static_cast<u8>(mFrameTimer) > 8)
                    {
                        mAnimationFrame++;
                        if (mAnimationFrame > 3)
                        {
                            mAnimationFrame = 0;
                        }
                        mFrameTimer = 0;
                    }
                    smoke_ofs = 0;
                }
                else
                {
                    u8 limit;
                    if (mAnimationFrame == 5)
                    {
                        limit = 40;
                        if (mFrameTimer == 1)
                        {
                            gParticle_8->SpawnCigaretteSmokePuff(mSpritePtr, 1);
                        }
                    }
                    else
                    {
                        limit = 3;
                    }

                    smoke_ofs = 4;
                    if (static_cast<u8>(mFrameTimer) > limit)
                    {
                        mAnimationFrame++;
                        if (mAnimationFrame > 7)
                        {
                            mAnimationFrame = 0;
                            mCharState = Char_B4_state::Standing_7;
                        }
                        mFrameTimer = 0;
                    }
                }
                newId = mAnimationFrame + (smoke_ofs + baseId) + 53;
            }
            break;

        case 3:
            newId = baseId + 139;
            break;

        case Char_Anim_state::Punching_4:
            if (mVelocity > kZeroVelocity)
            {
                if (static_cast<u8>(mFrameTimer) > 2)
                {
                    mAnimationFrame++;
                    if (mAnimationFrame > 7)
                    {
                        mAnimationFrame = 0;
                    }
                    mFrameTimer = 0;
                }
                newId = mAnimationFrame + baseId + 123;
            }
            else
            {
                if (static_cast<u8>(mFrameTimer) > 2)
                {
                    mAnimationFrame++;
                    if (mAnimationFrame > 7)
                    {
                        mAnimationFrame = 0;
                    }
                    mFrameTimer = 0;
                }
                newId = mAnimationFrame + baseId + 115;
            }
            break;

        case Char_Anim_state::Entering_Car_6:
        case Char_Anim_state::Carjacking_9:
        {
            Car* pCar = mTargetCar;
            pCar->mLastSeenTimer = 0;
            switch (mAnimationFrame)
            {
                case 0:
                {
                    Car_Door* pDoor = mTargetCar->GetDoor(mpPed->get_target_car_door());
                    if (mFlagsBf.bDoorOffsetLarge)
                    {
                        frame_limit = 1;
                        pDoor->Open();
                        newId = baseId + mAnimationFrame;
                        CarDoorAlignmentSolver(7, pCar, mpPed->get_target_car_door(), newx, newy, mRotation);
                    }
                    else
                    {
                        if (mFrameTimer == 1 && pDoor->IsStateActive())
                        {
                            mAnimationFrame = 4;
                            mFrameDelay = 0;
                        }
                        CarDoorAlignmentSolver(mAnimationFrame,
                                                      pCar,
                                                      mpPed->get_target_car_door(),
                                                      newx,
                                                      newy,
                                                      mRotation);
                        newId = mAnimationFrame + baseId + 24;
                        frame_limit = 5;
                    }
                    break;
                }

                case 1:
                case 2:
                case 3:
                    if (mFlagsBf.bDoorOffsetLarge)
                    {
                        frame_limit = 1;
                        CarDoorAlignmentSolver(7 - mAnimationFrame,
                                                      pCar,
                                                      mpPed->get_target_car_door(),
                                                      newx,
                                                      newy,
                                                      mRotation);
                        mSpritePtr->SetDrawLayer(9);
                        newId = baseId + mAnimationFrame;
                    }
                    else
                    {
                        CarDoorAlignmentSolver(mAnimationFrame,
                                                      pCar,
                                                      mpPed->get_target_car_door(),
                                                      newx,
                                                      newy,
                                                      mRotation);
                        frame_limit = 3;
                        newId = mAnimationFrame + baseId + 24;
                    }
                    break;

                case 4:
                case 5:
                case 6:
                case 7:
                    if (mFlagsBf.bDoorOffsetLarge)
                    {
                        frame_limit = 1;
                        CarDoorAlignmentSolver(7 - mAnimationFrame,
                                                      pCar,
                                                      mpPed->get_target_car_door(),
                                                      newx,
                                                      newy,
                                                      mRotation);
                        newId = baseId + mAnimationFrame;
                        mSpritePtr->SetDrawLayer(9);
                    }
                    else
                    {
                        CarDoorAlignmentSolver(mAnimationFrame,
                                                      pCar,
                                                      mpPed->get_target_car_door(),
                                                      newx,
                                                      newy,
                                                      mRotation);
                        frame_limit = 1;
                        mSpritePtr->SetDrawLayer(9);
                        newId = mAnimationFrame + baseId + 28;
                    }
                    break;

                case 8:
                    CarDoorAlignmentSolver(mAnimationFrame,
                                                  pCar,
                                                  mpPed->get_target_car_door(),
                                                  newx,
                                                  newy,
                                                  mRotation);
                    mSpritePtr->SetDrawLayer(9);
                    mpPed->ChangeNextPedState2(ped_state_2::ped2_driving_10);
                    mpPed->ChangeNextPedState1(ped_state_1::in_car_10);
                    mPedState2 = ped_state_2::ped2_driving_10;
                    mPedState1 = ped_state_1::in_car_10;
                    return;

                case 9:
                {
                    frame_limit = 2;
                    newId = mAnimationFrame + baseId + 19;
                    Ped* pDriver = pCar->mDriver;
                    if (pDriver)
                    {
                        switch (pDriver->get_occupation())
                        {
                            case ped_ocupation_enum::dummy_driver_4:
                                pDriver = gPedManager->SpawnRunAwayGuy();
                                if (pCar->mCarInfoIdx == car_model_enum::apc ||
                                    pCar->mCarInfoIdx == car_model_enum::JEEP ||
                                    pCar->mCarInfoIdx == car_model_enum::TANK)
                                {
                                    pDriver->mGraphicType = ped_graphic_type::cop_2;
                                    pDriver->set_remap(ped_remap_enum::ped_remap_army);
                                }
                                pDriver->SetStolenCar(pCar);
                                pDriver->mCarThief = mpPed;
                                break;

                            case ped_ocupation_enum::driver:
                                pDriver->set_occupation(ped_ocupation_enum::robbed_driver_10);
                                pDriver->SetStolenCar(pCar);
                                pDriver->mCarThief = mpPed;
                                break;

                            case ped_ocupation_enum::police:
                                if (mpPed->mWantedPoints < cop_level_ped_enum::cop_1_stars)
                                {
                                    gPolice_7B8->RegisterCriminal(mpPed);
                                    mpPed->mWantedPoints = cop_level_ped_enum::cop_1_stars;
                                }
                                gPolice_7B8->UpdateLastSeenCoordsForCriminal(mpPed);
                                if (pDriver->get_objective() == objectives_enum::patrol_in_car_43)
                                {
                                    gPolice_7B8->TryAssignCarCrewToCriminal(pDriver->mCar, mpPed);
                                }
                                pDriver->SetObjective(objectives_enum::no_obj_0);
                                break;

                            default:
                                if (pDriver->mpGang)
                                {
                                    pDriver->SetObjective2(objectives_enum::kill_char_on_foot_20);
                                    pDriver->SetInternalTargetPed(mpPed);
                                }
                                pDriver->SetStolenCar(pCar);
                                pDriver->mCarThief = mpPed;
                                break;
                        }

                        if (mpPed->GetInternalObjective() == objectives_enum::enter_car_as_driver_35 && mpPed->GetInternalObjectiveStatus() == objective_status::passed_1)
                        {
                            mpPed->SetInternalObjectiveStatus(objective_status::not_finished_0);
                        }
                        pDriver->ChangeNextPedState1(ped_state_1::immobilized_8);
                        pDriver->ChangeNextPedState2(ped_state_2::ped2_at_car_door_17);
                        pCar->ClearDriver();
                        CarDoorAlignmentSolver(8, pCar, mpPed->get_target_car_door(), newx, newy, mRotation);
                        pDriver->AllocCharB4(
                            newx,
                            newy,
                            gMap_0x370->FindGroundZAboveCoord(newx, newy, pCar->mCarSprite->mZpos));
                        pDriver->mGameObject->set_rotation(pCar->mCarSprite->mRotation);
                        pDriver->mGameObject->Set_F8_ped_state_1_433910(ped_state_1::immobilized_8);
                        pDriver->set_target_car_door(mpPed->get_target_car_door());
                        pDriver->mGameObject->SetPedState2(ped_state_2::ped2_at_car_door_17);
                        pDriver->mGameObject->mTargetCar = pCar;
                        pDriver->mGameObject->mSpritePtr->SetDrawLayer(6);
                        pDriver->SetRemap(pDriver->get_remap());
                        pDriver->mCar = NULL;
                        pDriver->SetStateInitPending();
                        if (!pDriver->is_player())
                        {
                            pDriver->SetVoiceEventIfNotOnFire(12);
                        }
                    }
                    CarDoorAlignmentSolver(mAnimationFrame,
                                                  pCar,
                                                  mpPed->get_target_car_door(),
                                                  newx,
                                                  newy,
                                                  mRotation);
                    break;
                }

                case 10:
                case 11:
                case 12:
                    CarDoorAlignmentSolver(mAnimationFrame,
                                                  pCar,
                                                  mpPed->get_target_car_door(),
                                                  newx,
                                                  newy,
                                                  mRotation);
                    newId = mAnimationFrame + baseId + 19;
                    frame_limit = 2;
                    break;
            }

            mSpritePtr->set_xyz_lazy_420600(
                newx,
                newy,
                gMap_0x370->FindGroundZAboveCoord(newx, newy, pCar->mCarSprite->mZpos));

            if (mFlagsBf.bDoorOffsetLarge)
            {
                mRotation = TurnBy180Deg_546360(mRotation);
            }

            if (static_cast<u8>(mFrameTimer) > frame_limit)
            {
                mAnimationFrame++;
                mFrameTimer = 0;
                if (mAnimationFrame == 13)
                {
                    mAnimationFrame = 4;
                }
            }
            else if (mFrameTimer == 5 && mAnimationFrame == 0)
            {
                Car_Door* pDoor = mTargetCar->GetDoor(mpPed->get_target_car_door());
                if (!mFlagsBf.bDoorOffsetLarge)
                {
                    pDoor->Open();
                    pDoor->set_ped(mpPed);
                }
            }
            break;
        }

        case Char_Anim_state::Getting_pulled_out_of_car_8:
        {
            Car* pCar = mTargetCar;
            pCar->mLastSeenTimer = 0;
            switch (mAnimationFrame)
            {
                case 0:
                    CarDoorAlignmentSolver(4, pCar, mpPed->get_target_car_door(), newx, newy, mRotation);
                    break;
                case 1:
                    CarDoorAlignmentSolver(2, pCar, mpPed->get_target_car_door(), newx, newy, mRotation);
                    break;
                case 2:
                    CarDoorAlignmentSolver(99, pCar, mpPed->get_target_car_door(), newx, newy, mRotation);
                    break;
            }

            mSpritePtr->set_xyz_lazy_420600(newx, newy, pCar->mCarSprite->mZpos);
            mSpritePtr->SetDrawLayer(6);
            sCharB4GradientSlopeBelow = gMap_0x370->IsGradientSlopeAt(mSpritePtr->mXy.x.ToInt(),
                                                                      mSpritePtr->mXy.y.ToInt(),
                                                                      mSpritePtr->mZpos.ToInt() - 1);
            ManageZCoordAndSlopes();
            if (static_cast<u8>(mFrameTimer) > 2)
            {
                mFrameTimer = 0;
                mAnimationFrame++;
            }
            newId = baseId + 36;
            break;
        }

        case Char_Anim_state::Exiting_Car_7:
        {
            Car* pCar = mTargetCar;
            pCar->mLastSeenTimer = 0;
            switch (mAnimationFrame)
            {
                case 0:
                    if (mFlagsBf.bDoorOffsetLarge)
                    {
                        mTargetCar->GetDoor(mpPed->get_target_car_door())->Open();
                        newId = baseId + mAnimationFrame;
                        frame_limit = 0;
                        CarDoorAlignmentSolver(0, pCar, mpPed->get_target_car_door(), newx, newy, mRotation);
                    }
                    else
                    {
                        CarDoorAlignmentSolver(7, pCar, mpPed->get_target_car_door(), newx, newy, mRotation);
                        frame_limit = 3;
                        mSpritePtr->SetDrawLayer(9);
                        newId = baseId + 36;
                    }
                    break;

                case 1:
                    if (mFlagsBf.bDoorOffsetLarge)
                    {
                        newId = baseId + mAnimationFrame;
                        frame_limit = 0;
                        CarDoorAlignmentSolver(1, pCar, mpPed->get_target_car_door(), newx, newy, mRotation);
                    }
                    else
                    {
                        CarDoorAlignmentSolver(6, pCar, mpPed->get_target_car_door(), newx, newy, mRotation);
                        frame_limit = 3;
                        mSpritePtr->SetDrawLayer(9);
                        newId = baseId + 35;
                    }
                    break;

                case 2:
                    if (mFlagsBf.bDoorOffsetLarge)
                    {
                        newId = baseId + mAnimationFrame;
                        frame_limit = 0;
                        CarDoorAlignmentSolver(2, pCar, mpPed->get_target_car_door(), newx, newy, mRotation);
                    }
                    else
                    {
                        CarDoorAlignmentSolver(5, pCar, mpPed->get_target_car_door(), newx, newy, mRotation);
                        frame_limit = 3;
                        mSpritePtr->SetDrawLayer(9);
                        newId = baseId + 34;
                    }
                    break;

                case 3:
                    if (mFlagsBf.bDoorOffsetLarge)
                    {
                        newId = baseId + mAnimationFrame;
                        frame_limit = 0;
                        CarDoorAlignmentSolver(3, pCar, mpPed->get_target_car_door(), newx, newy, mRotation);
                    }
                    else
                    {
                        CarDoorAlignmentSolver(4, pCar, mpPed->get_target_car_door(), newx, newy, mRotation);
                        frame_limit = 3;
                        mSpritePtr->SetDrawLayer(9);
                        newId = baseId + 33;
                    }
                    break;

                case 4:
                    if (mFlagsBf.bDoorOffsetLarge)
                    {
                        newId = baseId + mAnimationFrame;
                        frame_limit = 0;
                        CarDoorAlignmentSolver(4, pCar, mpPed->get_target_car_door(), newx, newy, mRotation);
                    }
                    else
                    {
                        CarDoorAlignmentSolver(0, pCar, mpPed->get_target_car_door(), newx, newy, mRotation);
                        frame_limit = 1;
                        mSpritePtr->SetDrawLayer(23);
                        newId = mAnimationFrame + baseId + 24;
                    }
                    break;

                case 5:
                    if (mFlagsBf.bDoorOffsetLarge)
                    {
                        newId = baseId + mAnimationFrame;
                        frame_limit = 0;
                        CarDoorAlignmentSolver(5, pCar, mpPed->get_target_car_door(), newx, newy, mRotation);
                    }
                    else
                    {
                        CarDoorAlignmentSolver(1, pCar, mpPed->get_target_car_door(), newx, newy, mRotation);
                        frame_limit = 1;
                        mSpritePtr->SetDrawLayer(23);
                        newId = mAnimationFrame + baseId + 24;
                    }
                    break;

                case 6:
                    if (mFlagsBf.bDoorOffsetLarge)
                    {
                        newId = baseId + mAnimationFrame;
                        frame_limit = 0;
                        CarDoorAlignmentSolver(6, pCar, mpPed->get_target_car_door(), newx, newy, mRotation);
                    }
                    else
                    {
                        CarDoorAlignmentSolver(2, pCar, mpPed->get_target_car_door(), newx, newy, mRotation);
                        frame_limit = 1;
                        mSpritePtr->SetDrawLayer(23);
                        newId = mAnimationFrame + baseId + 24;
                    }
                    break;

                case 7:
                    if (mFlagsBf.bDoorOffsetLarge)
                    {
                        newId = baseId + mAnimationFrame;
                        frame_limit = 0;
                        CarDoorAlignmentSolver(7, pCar, mpPed->get_target_car_door(), newx, newy, mRotation);
                    }
                    else
                    {
                        CarDoorAlignmentSolver(3, pCar, mpPed->get_target_car_door(), newx, newy, mRotation);
                        frame_limit = 1;
                        mSpritePtr->SetDrawLayer(23);
                        newId = mAnimationFrame + baseId + 24;
                    }
                    break;

                case 8:
                {
                    mSpritePtr->SetDrawLayer(23);
                    mpPed->ChangeNextPedState2(ped_state_2::ped2_walking_0);
                    mpPed->ChangeNextPedState1(ped_state_1::walking_0);
                    mPedState2 = ped_state_2::ped2_walking_0;
                    mPedState1 = ped_state_1::walking_0;
                    Car_Door* pDoor = mTargetCar->GetDoor(mpPed->get_target_car_door());
                    if (!mFlagsBf.bDoorOffsetLarge)
                    {
                        pDoor->Close();
                    }
                    pDoor->set_ped(NULL);

                    if (static_cast<u8>(IsOnWater()))
                    {
                        mpPed->PutOutFire();
                        DrownPed();
                    }
                    else
                    {
                        sCharB4GradientSlopeBelow = gMap_0x370->IsGradientSlopeAt(mSpritePtr->mXy.x.ToInt(),
                                                                                  mSpritePtr->mXy.y.ToInt(),
                                                                                  mSpritePtr->mZpos.ToInt() - 1);
                        ManageZCoordAndSlopes();
                    }
                    return;
                }
            }

            mSpritePtr->set_xyz_lazy_420600(
                newx,
                newy,
                gMap_0x370->FindGroundZAboveCoord(newx, newy, pCar->mCarSprite->mZpos));

            if (static_cast<u8>(mFrameTimer) > frame_limit)
            {
                mAnimationFrame++;
                mFrameTimer = 0;
                if (mAnimationFrame == 13)
                {
                    mAnimationFrame = 4;
                }
            }
            else if (mFrameTimer == 1 && mAnimationFrame == 0)
            {
                Car_Door* pDoor = mTargetCar->GetDoor(mpPed->get_target_car_door());
                if (!mFlagsBf.bDoorOffsetLarge)
                {
                    pDoor->Open();
                    pDoor->set_ped(mpPed);
                }
            }
            break;
        }

        case Char_Anim_state::Jumping_5:
        {
            if (mpPed->mPlayer)
            {
                mFrameDelay = 2;
            }
            else
            {
                mFrameDelay = 1;
            }

            switch (mAnimationFrame)
            {
                case 8:
                {
                    mCharState = Char_B4_state::Walking_1;
                    mAnimationState = Char_Anim_state::Standing_2;
                    mAnimationFrame = 0;
                    s16 landId = baseId + 23;
                    groundZ = gMap_0x370->FindGroundZForCoord(mSpritePtr->mXy.x,
                                                                            mSpritePtr->mXy.y);
                    gMap_0x370->UpdateZFromSlopeAtCoord(mSpritePtr->mXy.x,
                                                                      mSpritePtr->mXy.y,
                                                                      groundZ);
                    Fix16 fall = mSpritePtr->mZpos - groundZ;
                    if (fall > kFP16Zero_6FD9E4)
                    {
                        gCharB4_Saved_Zpos -= fall;
                        gCharB4_Saved_Zpos = SnapZTo16(gCharB4_Saved_Zpos);
                        ManageZCoordAndSlopes();
                    }
                    else
                    {
                        gCharB4_Saved_Zpos = groundZ;
                    }

                    if (mSpritePtr->mZpos < mJumpBaseZ)
                    {
                        mSpritePtr->set_xyz_lazy_420600(mSpritePtr->mXy.x,
                                                                 mSpritePtr->mXy.y,
                                                                 mJumpBaseZ);
                    }
                    mSpritePtr->set_id_lazy(landId);
                    return;
                }

                case 6:
                case 7:
                {
                    groundZ = gMap_0x370->FindGroundZForCoord(mSpritePtr->mXy.x,
                                                                            mSpritePtr->mXy.y);
                    gMap_0x370->UpdateZFromSlopeAtCoord(mSpritePtr->mXy.x,
                                                                      mSpritePtr->mXy.y,
                                                                      groundZ);
                    Fix16 fall = (mSpritePtr->mZpos - groundZ) / gFix16_Two;
                    if (fall > kFP16Zero_6FD9E4)
                    {
                        gCharB4_Saved_Zpos -= fall;
                        gCharB4_Saved_Zpos = SnapZTo16(gCharB4_Saved_Zpos);
                        ManageZCoordAndSlopes();
                    }
                    else
                    {
                        gCharB4_Saved_Zpos = groundZ;
                    }
                    break;
                }

                case 5:
                    mFrameDelay = 3;
                    mJumpScaleCounter = 12;
                    break;

                case 1:
                case 2:
                case 3:
                case 4:
                    gCharB4_Saved_Zpos += kFpPoint05;
                    gCharB4_Saved_Zpos = SnapZTo16(gCharB4_Saved_Zpos);
                    if (mFlagsBf.bOnSlope)
                    {
                        sCharB4GradientSlopeBelow = 0;
                    }
                    ManageZCoordAndSlopes();
                    break;

                case 0:
                    mJumpScaleCounter = 0;
                    gCharB4_Saved_Zpos += kFpPoint05;
                    gCharB4_Saved_Zpos = SnapZTo16(gCharB4_Saved_Zpos);
                    if (mFlagsBf.bOnSlope)
                    {
                        sCharB4GradientSlopeBelow = 0;
                    }
                    ManageZCoordAndSlopes();
                    break;
            }

            mJumpScaleCounter++;
            s16 jumpId = mAnimationFrame + baseId + 16;
            if (static_cast<u8>(mFrameTimer) > static_cast<u8>(mFrameDelay))
            {
                mFrameTimer = 0;
                mAnimationFrame++;
            }
            mSpritePtr->set_id_lazy(197);
            mSpritePtr->set_id_lazy(jumpId);

            if (static_cast<u8>(mJumpScaleCounter) < 12)
            {
                mSpritePtr->ApplyScaleToDimensions(kFP16One_6FD9E8 + Fix16(static_cast<u8>(mJumpScaleCounter)) * kFpPoint03,
                                                                   0);
            }
            else if (static_cast<u8>(mJumpScaleCounter) < 24)
            {
                mSpritePtr->ApplyScaleToDimensions(kFP16One_6FD9E8 +
                                                                       Fix16(25 - static_cast<u8>(mJumpScaleCounter)) * kFpPoint03,
                                                                   0);
            }
            return;
        }

        case Char_Anim_state::Normal_Fall_11:
        case Char_Anim_state::Lethal_Fall_12:
            mFrameDelay = 1;
            switch (mAnimationFrame)
            {
                case 10:
                case 11:
                case 12:
                case 13:
                    mpPed->TakeDamage(4);
                    // fall through
                case 0:
                case 1:
                case 2:
                case 3:
                case 4:
                case 5:
                case 6:
                case 7:
                case 8:
                case 9:
                    newId = mAnimationFrame + baseId + 81;
                    break;
                case 14:
                case 15:
                case 16:
                    newId = mAnimationFrame + baseId + 81;
                    mAnimationState = Char_Anim_state::Lethal_Fall_12;
                    break;
            }

            if (static_cast<u8>(mFrameTimer) > static_cast<u8>(mFrameDelay))
            {
                if (mAnimationFrame < 16)
                {
                    mAnimationFrame++;
                }
                mFrameTimer = 0;
            }
            break;

        case Char_Anim_state::Lying_On_Floor_10:
            newId = baseId + 72;
            break;

        case Char_Anim_state::Lying_Pose_14:
            newId = baseId + 80;
            break;

        case Char_Anim_state::Lying_Pose_13:
            newId = baseId + 72;
            break;

        case Char_Anim_state::Lying_Pose_19:
            newId = baseId + 156;
            break;

        case Char_Anim_state::Lying_Pose_20:
            newId = baseId + 157;
            break;

        case Char_Anim_state::Charred_Corpse_21:
            newId = baseId + 155;
            break;

        case Char_Anim_state::Knocked_Down_Hit_Front_15:
            if (static_cast<u8>(mFrameTimer) > FrameDelay_546360(this))
            {
                mAnimationFrame++;
                if (mAnimationFrame > 7)
                {
                    switch (gRng.get_int(2))
                    {
                        case 0:
                            mAnimationState = Char_Anim_state::Lying_Pose_19;
                            break;
                        case 1:
                            mAnimationState = Char_Anim_state::Lying_Pose_13;
                            break;
                    }
                    mAnimationFrame = 7;
                }
            }
            newId = mAnimationFrame + baseId + 65;
            break;

        case Char_Anim_state::Knocked_Down_Hit_Behind_16:
            if (static_cast<u8>(mFrameTimer) > FrameDelay_546360(this))
            {
                mAnimationFrame++;
                if (mAnimationFrame > 7)
                {
                    switch (gRng.get_int(3))
                    {
                        case 0:
                            mAnimationState = Char_Anim_state::Lying_Pose_14;
                            break;
                        case 1:
                            mAnimationState = Char_Anim_state::Lying_Pose_19;
                            break;
                        case 2:
                            mAnimationState = Char_Anim_state::Lying_Pose_20;
                            break;
                    }
                    mAnimationFrame = 7;
                }
            }
            newId = mAnimationFrame + baseId + 73;
            break;

        case Char_Anim_state::Electrocuted_17:
            if (static_cast<u8>(mFrameTimer) > 3)
            {
                mAnimationFrame++;
                if (mAnimationFrame > 4)
                {
                    mAnimationFrame = 0;
                }
                mFrameTimer = 0;
            }

            if (mAnimationFrame)
            {
                mSpritePtr->SetPaletteSprites();
            }
            mSpritePtr->SetDrawLayer(6);
            newId = mAnimationFrame + baseId + 151;
            break;
    }

    mSpritePtr->set_id_lazy(newId);
}

MATCH_FUNC(0x548590)
void Char::ManageZCoordAndSlopes()
{
    Fix16 zpos = kFP16Zero_6FD9E4;
    zpos = mSpritePtr->mZpos;

    if (sCharB4GradientSlopeBelow == 1)
    {
        zpos--;
    }
    u8 gradient_direction =
        gMap_0x370->UpdateZFromSlopeAtCoord(mSpritePtr->mXy.x, mSpritePtr->mXy.y, zpos);

    if (gradient_direction == NO_GRADIENT_SLOPE_0)
    {
        if (mSlopeGradientDirection)
        {
            Fix16 frac = zpos.GetFracValue();
            zpos = zpos.GetFloorValue();

            if (frac > kFpHalf_6FDB28)
            {
                zpos++;
            }
        }
        else
        {
            mFlagsBf.bOnSlope = false;
            zpos = Fix16(zpos.ToUInt8());
        }
        mSlopeGradientDirection = 0;
    }
    else
    {
        mSlopeGradientDirection = gradient_direction;
        mFlagsBf.bOnSlope = true;
    }
    mSpritePtr->set_xyz_lazy_420600(mSpritePtr->mXy.x, mSpritePtr->mXy.y, zpos);
}

DEFINE_GLOBAL_INIT(Ang16, kAng16_6FD888, Ang16(64), 0x6FD888);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpOne128th, Fix16(0x80, 0), 0x6FD860);

// The angle is a by-value parameter: the original keeps it in memory and adds it as a dword
static inline Ang16 TurnBy16Deg_548BD0(Ang16 angle)
{
    return kAng16_6FD888 + angle;
}

// 9.6f 0x499F00
MATCH_FUNC(0x548670)
void Char::DispatchCollision(char_type bIgnorePeds)
{
    u8 bIgnorePedHit;
    if (this->mIsCollidingWithSprite == 1 && this->mCollidedEntityType && this->mSlideState != 3)
    {
        bIgnorePedHit = 1;
    }
    else
    {
        bIgnorePedHit = bIgnorePeds;
    }

    Sprite* pNearSprite = mSpritePtr->QuerySpriteCollision(sprite_types_enum::car_2);
    if (pNearSprite)
    {
        s32 sprite_type_enum = pNearSprite->mSpriteTypeEnum;
        switch (pNearSprite->GetType())
        {

            case sprite_types_enum::ped_3:
                if (bIgnorePedHit == sCharB4IgnorePeds)
                {
                    this->mCollidedEntity = NULL;
                }
                else
                {
                    if (mpPed->GetPedType() >= ped_type::player_2 && mpPed->mPedType <= ped_type::dummy_with_occupation_6)
                    {
                        Char::HandlePedCollision(pNearSprite->AsCharB4());
                        this->mCollidedEntity = NULL;
                    }
                }
                break;

            case sprite_types_enum::car_2:
            {
                Sprite* pNearSprite2 = mSpritePtr->QuerySpriteCollision(sprite_types_enum::object_1);
                if (pNearSprite2->GetType() == sprite_types_enum::object_1)
                {
                    if (pNearSprite2->AsObject2C())
                    {
                        Char::HandleObjectCollision(pNearSprite2->AsObject2C());
                        this->mCollidedEntityType = 3;
                    }
                }
                else
                {
                    Char::HandleGenericCollision(pNearSprite->AsCar(), NULL, NULL);
                    this->mCollidedEntityType = 1;
                }
                break;
            }
            case sprite_types_enum::object_1:
            case sprite_types_enum::code_obj1_4:
            case sprite_types_enum::map_obj_5:
                Char::HandleObjectCollision(pNearSprite->AsObject2C());
                this->mCollidedEntityType = 3;
                break;

            default:
                return;
        }
    }
    else
    {
        if (mCharState != Char_B4_state::Avoiding_Ped_10)
        {
            if (mCharState == Char_B4_state::Colliding_With_Car_27)
            {
                this->mCharState = Char_B4_state::Walking_1;
                this->mPrevCollidedEntity = NULL;
                this->mCollidedEntityType = 0;
                this->mCollidedEntity = NULL;
            }
            else
            {
                this->mCollidedEntity = NULL;
            }
        }
        else
        {
            this->mCharState = Char_B4_state::Walking_1;
            this->mCollidedEntity = NULL;
        }

        if (mVelocity == kZeroVelocity || mpPed->PedTypeIs(ped_type::player_2) && !this->mPedState1)
        {
            this->mPrevCollidedEntity = NULL;
            this->mCollidedEntityType = 0;
            this->mIsCollidingWithSprite = false;
            this->mSlideTurn = kAng0_6FDB34;
        }
    }
}

// PolarToCartesian_41FC20 as ApplyMovement expands it: the big function is past the
// inline budget, so the original calls Multiply instead of inlining operator*
static inline void PolarToCartesian_OutOfLineMul(Ang16& angle, const Fix16& radius, Fix16& ret1, Fix16& ret2)
{
    ret1 = Ang16::sine(angle).Multiply(radius);
    ret2 = Ang16::cosine(angle).Multiply(radius);
}

// Ang16 operator+ with the normalizing ctor called out of line (AssignNormalized)
static inline Ang16 AddAngles_ool_54B8F0(const Ang16& a, const Ang16& b)
{
    s16 sum = a.rValue + b.rValue;
    return Ang16(reinterpret_cast<Ang16&>(sum), 0);
}

static inline bool CanJumpOver_54A530(Char* pThis, Char* pChar)
{
    if (!pThis->mpPed->PedTypeIs(ped_type::player_2))
    {
        if (!pChar)
        {
            return true;
        }
        return false;
    }
    if (pThis->mPedState1 == ped_state_1::entering_car_3)
    {
        return true;
    }
    return false;
}

MATCH_FUNC(0x548840)
void Char::HandleObjectCollision(Object* pObj)
{
    Fix16_Point new_pos;
    Fix16_Point point;
    // Unused: the original enters with EH state 4, so three more Fix16_Point locals were constructed up front
    Fix16_Point unused_1, unused_2, unused_3;
    u8 side_self;
    u8 out2;
    u8 out3 = 0;
    bool bUnknown = false;

    s32 phi_type = pObj->mDefinition->mBehaviorType;

    if (!pObj->UsesParamAsPedRef() || (pObj->GetPedRefIdx()) == 0 ||
        pObj->GetPedRefIdx() != this->mpPed->mPedRefIdx)
    {
        if (!pObj->CanBePushed())
        {
            goto generic_collision;
        }

        if (mpPed->PedTypeIs(ped_type::player_2))
        {
            if (this->mCharState == Char_B4_state::Jumping_15)
            {
                goto generic_collision;
            }
        }
        else
        {
            if (pObj->mDefinition->mCollisionResponse == 3)
            {
                bUnknown = true;
            }

            if (pObj->mDefinition->mCollisionResponse && !bUnknown)
            {
                goto generic_collision;
            }
        }

        if (pObj->GetMass() < kFpHalf_6FDAB0)
        {

            mSpritePtr->set_xyz_lazy_420600(gCharB4_Saved_Xpos, gCharB4_Saved_Ypos, gCharB4_Saved_Zpos);

            point = mSpritePtr
                        ->FindCollisionIntersectionPoint(pObj->mSprite, new_pos, mSpritePtr->mRotation, side_self, out2, out3);
            pObj->ResolveCollisionWithPed(this, &point, 1);
            return;
        }
        this->mJumpOverTimer = 10;

    // shared exit for the three not-pushable checks: goto-free forms change the code
    generic_collision:
        HandleGenericCollision(NULL, pObj, NULL);
    }
}

// 9.6f 0x41FA70
static inline Ang16& compound_add(Ang16& a, const Ang16& b)
{
    a.rValue += b.rValue;
    a.Normalize();
    return a;
}

// 9.6f 0x493040 (a Ped method there)
static inline void CollideWithElvis(Ped* pPed, Char* pChar)
{
    if (reinterpret_cast<s32>(pChar) != pPed->mCollidedCharB4)
    {
        pPed->mCollidedCharB4 = reinterpret_cast<s32>(pChar);
        pPed->mBf224.bCollidedWithPed = false;
        pChar->mpPed->mCollidedCharB4 = reinterpret_cast<s32>(pPed->mGameObject);
        pPed->SetVoiceEventIfNotOnFire(ped_voice_event::elvis_bumped_22);
    }
}

// 9.6f 0x493000 (a Ped method there)
static inline void CollideWithPed(Ped* pPed, Char* pOther)
{
    if (reinterpret_cast<s32>(pOther) != pPed->mCollidedCharB4)
    {
        pPed->mCollidedCharB4 = reinterpret_cast<s32>(pOther);
        pPed->mBf224.bCollidedWithPed = true;
        pOther->mpPed->mCollidedCharB4 = reinterpret_cast<s32>(pPed->mGameObject);
    }
}

// What happens when this ped walks into pOther, by the types of both peds (field_238)
MATCH_FUNC(0x548bd0)
void Char::HandlePedCollision(Char* pOther)
{
    s32 my_type = mpPed->GetPedType();
    s32 other_type = pOther->mpPed->GetPedType();
    if (pOther->get_ped_state_1() == ped_state_1::dead_9 || pOther->get_ped_state_1() == ped_state_1::immobilized_8 || mFlagsBf.bDetouring || mCharState == Char_B4_state::Jumping_15)
    {
        return;
    }

    if (mpPed->is_player() && pOther->mpPed->Is_occupation_elvis())
    {
        CollideWithElvis(pOther->mpPed, mpPed->mGameObject);
    }
    else if (mpPed->is_player() || !(mpPed->mId & 3))
    {
        if (!pOther->mpPed->is_player())
        {
            CollideWithPed(mpPed, pOther);
        }
    }

    switch (my_type)
    {
        case ped_type::player_2:
            switch (other_type)
            {
                case ped_type::script_created_5:
                    if (mpPed->mPedGroup && pOther->mpPed->mPedGroup == mpPed->mPedGroup)
                    {
                        break;
                    }
                    if (pOther->mpPed->get_occupation() != ped_ocupation_enum::stand_still_bloke)
                    {
                        pOther->mRepulsionTimer = 4;
                        pOther->mRepulsionRotation = Fix16::atan2_fixed(mSpritePtr->mXy.y - pOther->mSpritePtr->mXy.y,
                                                         mSpritePtr->mXy.x - pOther->mSpritePtr->mXy.x) + kAng180_6FD936;
                    }
                    else
                    {
                        mSpritePtr->set_xyz_lazy_420600(gCharB4_Saved_Xpos, gCharB4_Saved_Ypos, gCharB4_Saved_Zpos);
                    }
                    break;
                case ped_type::player_2:
                    mSpritePtr->set_xyz_lazy_420600(gCharB4_Saved_Xpos, gCharB4_Saved_Ypos, gCharB4_Saved_Zpos);
                    break;
                case ped_type::dummy_3:
                case ped_type::special_ped_4:
                case ped_type::dummy_with_occupation_6:
                    mCharState = Char_B4_state::Walking_1;
                    if (mVelocity == kZeroVelocity)
                    {
                        break;
                    }
                    if (mpPed->mPedGroup && pOther->mpPed->mPedGroup == mpPed->mPedGroup)
                    {
                        break;
                    }
                    pOther->mRepulsionTimer = 4;
                    pOther->mRepulsionRotation = Fix16::atan2_fixed(mSpritePtr->mXy.y - pOther->mSpritePtr->mXy.y,
                                                         mSpritePtr->mXy.x - pOther->mSpritePtr->mXy.x) + kAng180_6FD936;
                    break;
            }
            break;

        case ped_type::script_created_5:
            switch (other_type)
            {
                case ped_type::script_created_5:
                    if (mCharState == Char_B4_state::Avoiding_Ped_10 || mpPed->get_id() >= pOther->mpPed->get_id())
                    {
                        mCharState = Char_B4_state::Walking_1;
                    }
                    else if (!mpPed->mPedGroup || mpPed->mPedGroup->mPanicking)
                    {
                        if (mPedState1 != ped_state_1::entering_car_3)
                        {
                            if (mpPed->GetInternalObjective() != objectives_enum::stay_close_to_char_11)
                            {
                                compound_add(mRotation, kAng16_6FD888);
                                mCharState = Char_B4_state::Avoiding_Ped_10;
                            }
                            else
                            {
                                mCharState = Char_B4_state::Avoiding_Ped_10;
                            }
                        }
                        else
                        {
                            mVelocity = kZeroVelocity;
                        }
                    }
                    break;
                case ped_type::player_2:
                    // Nothing to do, but the original's jump table starts at 2 (an empty case is dropped)
                    return;
                case ped_type::dummy_3:
                case ped_type::special_ped_4:
                case ped_type::dummy_with_occupation_6:
                    pOther->mRepulsionTimer = 4;
                    pOther->mRepulsionRotation = Fix16::atan2_fixed(get_sprite_ptr()->mXy.y - pOther->mSpritePtr->mXy.y,
                                                         mSpritePtr->mXy.x - pOther->mSpritePtr->mXy.x) + kAng180_6FD936;
                    break;
            }
            break;

        case ped_type::special_ped_4:
        case ped_type::dummy_with_occupation_6:
            switch (other_type)
            {
                case ped_type::special_ped_4:
                case ped_type::dummy_with_occupation_6:
                    if (mCharState == Char_B4_state::Avoiding_Ped_10 || mpPed->get_id() >= pOther->mpPed->get_id())
                    {
                        mCharState = Char_B4_state::Walking_1;
                        // A return, not a break: the original keeps the type 5 copy of this tail
                        return;
                    }
                    else if (!mpPed->mPedGroup || mpPed->mPedGroup->mPanicking)
                    {
                        if (mPedState1 != ped_state_1::entering_car_3)
                        {
                            if (mpPed->GetInternalObjective() != objectives_enum::stay_close_to_char_11)
                            {
                                compound_add(mRotation, kAng16_6FD888);
                                mCharState = Char_B4_state::Avoiding_Ped_10;
                            }
                            else
                            {
                                mCharState = Char_B4_state::Avoiding_Ped_10;
                            }
                        }
                    }
                    break;
                case ped_type::player_2:
                    if (pOther->mpPed->mWantedPoints >= 1)
                    {
                        mpPed->IsLawEnforcement();
                    }
                    break;
                case ped_type::dummy_3:
                    pOther->mRepulsionTimer = 4;
                    pOther->mRepulsionRotation = Fix16::atan2_fixed(mSpritePtr->mXy.y - pOther->mSpritePtr->mXy.y,
                                                         mSpritePtr->mXy.x - pOther->mSpritePtr->mXy.x) + kAng180_6FD936;
                    break;
            }
            break;

        case ped_type::dummy_3:
            if (mpPed->GetInternalObjective() == objectives_enum::crossing_road_48 || mpPed->GetInternalObjective() == objectives_enum::enter_train_37 ||
                mpPed->GetInternalObjective() == objectives_enum::leave_train_38 || mpPed->GetInternalObjective() == objectives_enum::goto_area_on_foot_12)
            {
                mVelocity = kFpThreeSixtyFourths;
                break;
            }
            switch (other_type)
            {
                case ped_type::player_2:
                    if (pOther->mpPed->mWantedPoints >= 1 && mpPed->IsLawEnforcement())
                    {
                        break;
                    }
                    if (pOther->mVelocity != kZeroVelocity)
                    {
                        mVelocity = kFpOne128th;
                    }
                    else
                    {
                        mRotation = Fix16::atan2_fixed(mSpritePtr->mXy.y - pOther->mSpritePtr->mXy.y,
                                                         mSpritePtr->mXy.x - pOther->mSpritePtr->mXy.x);
                    }
                    break;
                case ped_type::script_created_5:
                    if (pOther->mVelocity != kZeroVelocity)
                    {
                        mVelocity = kFpOne128th;
                    }
                    else
                    {
                        mRotation = Fix16::atan2_fixed(mSpritePtr->mXy.y - pOther->mSpritePtr->mXy.y,
                                                         mSpritePtr->mXy.x - pOther->mSpritePtr->mXy.x);
                    }
                    break;
                case ped_type::special_ped_4:
                case ped_type::dummy_with_occupation_6:
                    mVelocity = kFpOne128th;
                    break;
                case ped_type::dummy_3:
                    if (!mCollidedEntityType)
                    {
                        pOther->mRepulsionTimer = 4;
                        pOther->mRepulsionRotation = TurnBy16Deg_548BD0(Fix16::atan2_fixed(mSpritePtr->mXy.y - pOther->mSpritePtr->mXy.y,
                                                         mSpritePtr->mXy.x - pOther->mSpritePtr->mXy.x) + kAng180_6FD936);
                    }
                    break;
            }
            break;
    }
}

// https://decomp.me/scratch/ph2wn
// 9.6f 0x4948C0. The 18 Ang16 normalisations are the plain operators (`-pSprt->field_0`, `kAng180 +
// corner_ang`, `pSprt->field_0 - kAng90`): the first 9 go out of line (one Normalize target), the rest
// inline, and the inline budget only cuts there (side 0 out of line, side 1 inline) when the function's
// front-end size is in a window of about 60 nodes (Scripts/inline_budget/inlsim.py --scan). The
// `!= 0` / `== 0` tests, the declared-then-assigned xpos/ypos/target_y, the `default: break;` and the
// explicit `Fix16(...)` copies in the saved-position restores are code-neutral spellings that put it
// there. step_x/step_y are the one pair of function-scope locals for both PolarToCartesian calls (the
// player arm's too): separate block locals flip the player arm's imul operand order. The width/height
// getters (9.6f GetW/GetH) do the same, so those stay field accesses.
MATCH_FUNC(0x54a530)
void Char::HandleGenericCollision(Car* pCar, Object* pObj, Char* pChar)
{
    Ang16 corner_ang;
    Ang16 hit_ang;
    Sprite* pSprt;
    Fix16 rot_x;
    Fix16 rot_y;
    Fix16 target_x;
    Fix16 target_y;
    Fix16 step_x;
    Fix16 step_y;
    target_y = kFP16Zero_6FD9E4;
    u8 side = 0;
    bool bNoJump = false;

    Fix16 xpos;
    Fix16 ypos;
    xpos = mSpritePtr->mXy.x;
    ypos = mSpritePtr->mXy.y;

    if (mCharState != Char_B4_state::Jumping_15)
    {
        mSpritePtr->set_xyz_lazy_420600(Fix16(gCharB4_Saved_Xpos), Fix16(gCharB4_Saved_Ypos), Fix16(gCharB4_Saved_Zpos));
        mSlopeGradientDirection = sCharB4SavedSlopeGradDir.ToInt();
        mCharState = Char_B4_state::Colliding_With_Car_27;
    }

    if (mIsCollidingWithSprite == 0)
    {
        if (pCar != NULL)
        {
            corner_ang = pCar->GetCornerAngle();
            pSprt = pCar->mCarSprite;
            if (gGarage_48->IsParkingCar(pCar) != 0)
            {
                bNoJump = true;
                sCharCollisionNoJump = 1;
            }
        }
        else if (pObj != NULL)
        {
            pSprt = pObj->mSprite;
            Fix16 half_w;
            Fix16 half_h;
            half_w = pSprt->mpCollisionShape->mWidth / 2;
            half_h = pSprt->mpCollisionShape->mHeight / 2;
            corner_ang = Fix16::atan2_fixed(half_h, half_w);
            if (pSprt->mpCollisionShape->GetDepth() > kFP16Half_6FD8E4)
            {
                bNoJump = true;
                sCharCollisionNoJump = 1;
            }
        }
        else
        {
            pSprt = pChar->mSpritePtr;
            corner_ang = kAng45_6FD89C;
            pSprt->mRotation = kAng0_6FDB34;
        }

        if ((mpPed->GetJumpOverMode() == ped_jump_over_mode::can_jump_2 || mJumpOverTimer > 0) && mCharState != Char_B4_state::Jumping_15)
        {
            if (bNoJump == 0)
            {
                if (mpPed->PedTypeIs(ped_type::player_2) == 0)
                {
                    if (pChar == NULL)
                    {
                        Char::DoJump();
                    }
                }
                else if (mPedState1 == ped_state_1::entering_car_3)
                {
                    Char::DoJump();
                }
            }
        }

        if (bNoJump != 0)
        {
            mSlideTurn = kAng90_6FD854;
            if (mpPed->PedTypeIs(ped_type::player_2) != 0)
            {
                if (mCharState != Char_B4_state::Jumping_15 && mPedState1 != ped_state_1::entering_car_3)
                {
                    mSpritePtr->set_xyz_lazy_420600(Fix16(gCharB4_Saved_Xpos), Fix16(gCharB4_Saved_Ypos), Fix16(gCharB4_Saved_Zpos));
                    mSlopeGradientDirection = sCharB4SavedSlopeGradDir.ToInt();
                    mCharState = Char_B4_state::Colliding_With_Car_27;
                    return;
                }
                else if (mCollidedEntityType == 1)
                {
                    mSpritePtr->set_xyz_lazy_420600(Fix16(gCharB4_Saved_Xpos), Fix16(gCharB4_Saved_Ypos), Fix16(gCharB4_Saved_Zpos));
                    mRotation = mSpritePtr->mRotation + kAng180_6FD936;
                    return;
                }
                else
                {
                    mSpritePtr->set_xyz_lazy_420600(Fix16(gCharB4_Saved_Xpos), Fix16(gCharB4_Saved_Ypos), Fix16(gCharB4_Saved_Zpos));
                    mRotation = pSprt->mRotation;
                    Ang16::PolarToCartesian_41FC20(mRotation, mVelocity, step_x, step_y);
                    mSpritePtr->set_xyz_lazy_420600(mSpritePtr->mXy.x + step_x,
                                                             mSpritePtr->mXy.y + step_y,
                                                             mSpritePtr->mZpos);
                }
            }
            else
            {
                if (mCharState == Char_B4_state::Jumping_15 && mAnimationFrame >= 5)
                {
                    mAnimationFrame = 5;
                    mFrameDelay = 2;
                    mFrameTimer = 0;
                }
            }
        }
        else
        {
            if (mCharState == Char_B4_state::Jumping_15)
            {
                if (mAnimationFrame >= 5)
                {
                    mAnimationFrame = 5;
                    mFrameDelay = 2;
                    mFrameTimer = 0;
                }
                mIsCollidingWithSprite = false;

                if (pCar != NULL)
                {
                    if (pCar->mCarSprite->mZpos >= mSpritePtr->mZpos)
                    {
                        mForceTopLayer = true;
                        mSpritePtr->ResolveZOrder(pCar->mCarSprite);
                    }
                }
                return;
            }
        }

        // Work out which side of the obstacle we hit, in the obstacle's local space
        RotateAndTranslatePoint_42A720(mSpritePtr->mXy.x,
                                       mSpritePtr->mXy.y,
                                       -pSprt->mRotation,
                                       pSprt->mXy.x,
                                       pSprt->mXy.y,
                                       rot_x,
                                       rot_y);
        hit_ang = Fix16::atan2_fixed(rot_y, rot_x);

        target_x = mpPed->GetMoveTargetX();
        target_y = mpPed->GetMoveTargetY();
        if (target_x == kFP16MinusOne || target_y == kFP16MinusOne)
        {
            Ang16::PolarToCartesian_41FC20(mRotation, kFpFour, target_x, target_y);
            target_x += mSpritePtr->mXy.x;
            target_y += mSpritePtr->mXy.y;
        }

        RotateAndTranslatePoint_42A720(target_x, target_y, -pSprt->mRotation, pSprt->mXy.x, pSprt->mXy.y, rot_x, rot_y);
        target_x = pSprt->mXy.x + rot_x;
        target_y = pSprt->mXy.y + rot_y;
        RotateAndTranslatePoint_42A720(xpos, ypos, -pSprt->mRotation, pSprt->mXy.x, pSprt->mXy.y, rot_x, rot_y);
        xpos = pSprt->mXy.x + rot_x;
        ypos = pSprt->mXy.y + rot_y;

        if (hit_ang < -corner_ang)
        {
            side = 3;
            if (hit_ang < kAng180_6FD936 + corner_ang)
            {
                side = 2;
                if (hit_ang < kAng180_6FD936 - corner_ang)
                {
                    side = 1;
                    if (hit_ang < corner_ang)
                    {
                        side = 0;
                        if (target_x < xpos)
                        {
                            mSlideRotation = pSprt->mRotation - kAng90_6FD854;
                            mSlideTurn = kAng90_6FD8A2;
                        }
                        else
                        {
                            mSlideRotation = pSprt->mRotation + kAng90_6FD854;
                            mSlideTurn = kAng270_6FD94C;
                        }
                    }
                    else if (target_y < ypos)
                    {
                        mSlideRotation = pSprt->mRotation + kAng180_6FD936;
                        mSlideTurn = kAng270_6FD94C;
                    }
                    else
                    {
                        mSlideRotation = pSprt->mRotation;
                        mSlideTurn = kAng90_6FD8A2;
                    }
                }
                else if (target_x < xpos)
                {
                    mSlideRotation = pSprt->mRotation - kAng90_6FD854;
                    mSlideTurn = kAng270_6FD94C;
                }
                else
                {
                    mSlideRotation = pSprt->mRotation + kAng90_6FD854;
                    mSlideTurn = kAng90_6FD8A2;
                }
            }
            else if (target_y < ypos)
            {
                if ((mSlideTurn == kAng90_6FD8A2) != 0)
                {
                    mSlideRotation = pSprt->mRotation + kAng180_6FD936;
                    mSlideTurn = kAng90_6FD8A2;
                }
                else
                {
                    mSlideRotation = pSprt->mRotation;
                    mSlideTurn = kAng270_6FD94C;
                }
            }
            else
            {
                if ((mSlideTurn == kAng90_6FD8A2) != 0)
                {
                    mSlideRotation = pSprt->mRotation + kAng180_6FD936;
                    mSlideTurn = kAng90_6FD8A2;
                }
                else
                {
                    mSlideRotation = pSprt->mRotation;
                    mSlideTurn = kAng270_6FD94C;
                }
            }
        }
        else if (target_x < xpos)
        {
            mSlideRotation = pSprt->mRotation - kAng90_6FD854;
            mSlideTurn = kAng90_6FD8A2;
        }
        else
        {
            mSlideRotation = pSprt->mRotation + kAng90_6FD854;
            mSlideTurn = kAng270_6FD94C;
        }

        mIsCollidingWithSprite = true;
        mSpritePtr->set_xyz_lazy_420600(Fix16(gCharB4_Saved_Xpos), Fix16(gCharB4_Saved_Ypos), Fix16(gCharB4_Saved_Zpos));

        if (mpPed->PedTypeIs(ped_type::player_2) != 0)
        {
            if (mpPed->GetInternalObjective() != objectives_enum::enter_car_as_driver_35 ||
                gGarage_48->IsParkingCar(mpPed->get_target_to_enter()) != 0)
            {
                return;
            }
        }

        Ang16::PolarToCartesian_41FC20(mSlideRotation, mVelocity, step_x, step_y);
        mSpritePtr->set_xyz_lazy_420600(mSpritePtr->mXy.x + step_x,
                                                 mSpritePtr->mXy.y + step_y,
                                                 mSpritePtr->mZpos);
        Char::DispatchCollision(sCharB4IgnorePeds);

        if (mCollidedEntity != NULL)
        {
            switch (side)
            {
                case 1:
                    mSlideRotation = pSprt->mRotation - kAng90_6FD854;
                    break;
                case 2:
                    mSlideRotation = pSprt->mRotation;
                    break;
                case 3:
                    mSlideRotation = pSprt->mRotation - kAng90_6FD854;
                    break;
                default:
                    break;
            }

            if (mCharState != Char_B4_state::Jumping_15)
            {
                mSpritePtr->set_xy_lazy(gCharB4_Saved_Xpos, gCharB4_Saved_Ypos);
            }
            else if (mAnimationFrame >= 5)
            {
                mAnimationFrame = 5;
                mFrameDelay = 2;
                mFrameTimer = 0;
            }
        }

        if (pCar != NULL)
        {
            mPrevCollidedEntity = pCar;
            mCollidedEntity = pCar;
        }
        else if (pObj != NULL)
        {
            mPrevCollidedEntity = pObj;
            mCollidedEntity = pObj;
        }
        else
        {
            mPrevCollidedEntity = pChar;
            mCollidedEntity = pChar;
        }
    }
    else
    {
        void* pAny = pCar;
        if (pCar == NULL)
        {
            pAny = pObj;
            if (pObj != NULL)
            {
                if (pObj->mSprite->mpCollisionShape->GetDepth() > kFP16Half_6FD8E4)
                {
                    bNoJump = true;
                }
            }
            else
            {
                pAny = pChar;
            }
        }

        if (mPrevCollidedEntity == pAny || mPrevCollidedEntity == NULL)
        {
            if (mCharState != Char_B4_state::Jumping_15)
            {
                mPrevCollidedEntity = pAny;
                mCollidedEntity = pAny;
            }
            else
            {
                mIsCollidingWithSprite = false;
                mSlideTurn = kAng0_6FDB34;
            }
        }
        else if (mCollidedEntityType != 2 && bNoJump == 0)
        {
            if (mCharState != Char_B4_state::Jumping_15)
            {
                Char::DoJump();
            }
            else if (mAnimationFrame >= 5)
            {
                mAnimationFrame = 5;
                mFrameDelay = 2;
                mFrameTimer = 0;
            }
        }
    }
}

// https://decomp.me/scratch/aRlEV
MATCH_FUNC(0x54b8f0)
bool Char::ContinueMovementAfterCollision()
{
    Fix16 x_vec;
    Fix16 y_vec = kFP16Zero_6FD9E4;
    // Every path past the early return leaves through `done`: the original pushes ebx/ebp/edi only
    // after the Jumping_15 check, which needs a single exit block (separate returns keep the pushes
    // at entry). bMoved stays in memory, as in the original.
    bool bMoved = false;
    if (mCharState == Char_B4_state::Jumping_15)
    {
        return true;
    }

    if (mCollidedEntity)
    {
        mRotation = mSlideRotation;
        mSlideState = 1;
        PolarToCartesian_OutOfLineMul(mRotation, mVelocity, x_vec, y_vec);
        mSpritePtr->set_xyz_lazy_420600(mSpritePtr->mXy.x + x_vec,
                                                 mSpritePtr->mXy.y + y_vec,
                                                 mSpritePtr->mZpos);
        Char::DispatchCollision(sCharB4IgnorePeds);
        if (mCollidedEntity)
        {
            mSpritePtr->set_xy_lazy(gCharB4_Saved_Xpos, gCharB4_Saved_Ypos);
            mSlideRotation.rValue += mSlideTurn.rValue;
            mSlideRotation.Normalize_406C20();
            PolarToCartesian_OutOfLineMul(mRotation, mVelocity, x_vec, y_vec);
            mSpritePtr->set_xyz_lazy_420600(mSpritePtr->mXy.x + x_vec,
                                                     mSpritePtr->mXy.y + y_vec,
                                                     mSpritePtr->mZpos);
            Char::DispatchCollision(sCharB4IgnorePeds);
            if (mCollidedEntity)
            {
                mSpritePtr->set_xy_lazy(gCharB4_Saved_Xpos, gCharB4_Saved_Ypos);
                goto done;
            }
        }
        bMoved = true;
    }
    else
    {
        PolarToCartesian_OutOfLineMul(mRotation, mVelocity * gFix16_Two, x_vec, y_vec);
        mSpritePtr->set_xyz_lazy_420600(mSpritePtr->mXy.x + x_vec,
                                                 mSpritePtr->mXy.y + y_vec,
                                                 mSpritePtr->mZpos);
        Char::DispatchCollision(sCharB4IgnorePeds);
        if (mCollidedEntity)
        {
            mSpritePtr->set_xy_lazy(gCharB4_Saved_Xpos, gCharB4_Saved_Ypos);

            // Try stepping back away from the obstacle
            mRotation = mSlideRotation;
            Ang16 back_angle = AddAngles_ool_54B8F0(kAng180_6FD936, AddAngles_ool_54B8F0(mSlideRotation, mSlideTurn));
            PolarToCartesian_OutOfLineMul(back_angle, mVelocity * kFP16Four_6FD9F4, x_vec, y_vec);
            mSpritePtr->set_xyz_lazy_420600(mSpritePtr->mXy.x + x_vec,
                                                     mSpritePtr->mXy.y + y_vec,
                                                     mSpritePtr->mZpos);
            Char::DispatchCollision(sCharB4IgnorePeds);
            if (!mCollidedEntity && mSlideState != 2)
            {
                // Free behind: turn around
                mSpritePtr->set_xy_lazy(gCharB4_Saved_Xpos, gCharB4_Saved_Ypos);
                mSlideRotation.rValue += AddAngles_ool_54B8F0(kAng180_6FD936, mSlideTurn).rValue;
                mSlideRotation.Normalize_406C20();
                mSlideState = 2;
                mRotation = mSlideRotation;
                PolarToCartesian_OutOfLineMul(mRotation, mVelocity * gFix16_Two, x_vec, y_vec);
                mSpritePtr->set_xyz_lazy_420600(mSpritePtr->mXy.x + x_vec,
                                                         mSpritePtr->mXy.y + y_vec,
                                                         mSpritePtr->mZpos);
                Char::DispatchCollision(sCharB4IgnorePeds);
                if (mCollidedEntity)
                {
                    mSpritePtr->set_xy_lazy(gCharB4_Saved_Xpos, gCharB4_Saved_Ypos);
                    mSlideRotation.rValue += mSlideTurn.rValue;
                    mSlideRotation.Normalize_406C20();
                    mSlideState = 0;
                    mRotation = mSlideRotation;
                    PolarToCartesian_OutOfLineMul(mRotation, mVelocity, x_vec, y_vec);
                    mSpritePtr->set_xyz_lazy_420600(mSpritePtr->mXy.x + x_vec,
                                                             mSpritePtr->mXy.y + y_vec,
                                                             mSpritePtr->mZpos);
                    bMoved = true;
                    Char::DispatchCollision(sCharB4IgnorePeds);
                    if (mCollidedEntity)
                    {
                        mSpritePtr->set_xy_lazy(gCharB4_Saved_Xpos, gCharB4_Saved_Ypos);
                    }
                    goto done;
                }
            }
            else
            {
                mSpritePtr->set_xy_lazy(gCharB4_Saved_Xpos, gCharB4_Saved_Ypos);
                PolarToCartesian_OutOfLineMul(mRotation, mVelocity, x_vec, y_vec);
                mSpritePtr->set_xyz_lazy_451950(mSpritePtr->mXy.x + x_vec,
                                                         mSpritePtr->mXy.y + y_vec,
                                                         mSpritePtr->mZpos);
                Char::DispatchCollision(sCharB4IgnorePeds);
                if (mCollidedEntity)
                {
                    mSpritePtr->setxy_lazy(gCharB4_Saved_Xpos, gCharB4_Saved_Ypos);
                    goto done;
                }
            }
        }
        else
        {
            mIsCollidingWithSprite = false;
            mSpritePtr->setxy_lazy(gCharB4_Saved_Xpos, gCharB4_Saved_Ypos);
            Ang16::PolarToCartesian_451730(mRotation, mVelocity, x_vec, y_vec);
            mSpritePtr->set_xyz_lazy_451950(mSpritePtr->mXy.x + x_vec,
                                                     mSpritePtr->mXy.y + y_vec,
                                                     mSpritePtr->mZpos);
            Char::DispatchCollision(sCharB4IgnorePeds);
            if (mCollidedEntity)
            {
                mSpritePtr->setxy_lazy(gCharB4_Saved_Xpos, gCharB4_Saved_Ypos);
                goto done;
            }
            bMoved = true;
            goto done;
        }
        bMoved = true;
    }
done:
    return bMoved;
}

// 9.6f 0x491F10: the face (1-4) next to `face`, clockwise or not
static inline s32 __stdcall RotateFace(s32* face, bool* clockwise)
{
    switch (*face)
    {
        case 1:
            return *clockwise ? 4 : 3;
        case 3:
            return *clockwise ? 1 : 2;
        case 2:
            return *clockwise ? 3 : 4;
        case 4:
            return *clockwise ? 2 : 1;
        default:
            return 2;
    }
}

// 9.6f 0x492400
inline void Char::SetTurnTarget(Ang16 target_rotation)
{
    mCharState = Char_B4_state::Turning_To_Target_25;
    mTargetRotation = target_rotation;
    mTimer = 255;
}

MATCH_FUNC(0x54c090)
void Char::SnapRotationToFace()
{

    s32 face = Ang16::GetAngleFace(mRotation);
    if (!CanMoveOntoSlope(face))
    {
        u8 bPastHalf;
        s32 new_dir;
        switch (face)
        {
            case path_direction::up_1:
                bPastHalf = mRotation > kAng180_6FD8E8;
                break;
            case path_direction::right_3: // 10.5.exe's jump table: face 3 compares with 90 degrees, face 2 with kAng0
                bPastHalf = mRotation > kAng90_6FDA64;
                break;
            case path_direction::down_2:
                bPastHalf = mRotation > kAng0;
                break;
            case path_direction::left_4:
                bPastHalf = mRotation > kAng270;
                break;
        }

        switch (face)
        {
            case path_direction::up_1:
                new_dir = bPastHalf ? path_direction::left_4 : path_direction::right_3;
                break;
            case path_direction::right_3:
                new_dir = bPastHalf ? path_direction::up_1 : path_direction::down_2;
                break;
            case path_direction::down_2:
                new_dir = bPastHalf ? path_direction::right_3 : path_direction::left_4;
                break;
            case path_direction::left_4:
                new_dir = bPastHalf ? path_direction::down_2 : path_direction::up_1;
                break;
            default:
                new_dir = path_direction::down_2;
                break;
        }
        face = new_dir;
    }

    mRotation = ReturnAngleFromRoadDirection(face);

    if (mCharState != Char_B4_state::Jumping_15)
    {
        this->mCharState = Char_B4_state::Walking_1;
    }
    this->mTimer = 100;
}

MATCH_FUNC(0x54c1a0)
bool Char::CanMoveOntoSlope(s32 path_direction)
{
    u8 slope_type = 0;
    bool bMayLeavePavement = false;
    Fix16 x_fp = mSpritePtr->mXy.x;
    Fix16 y_fp = mSpritePtr->mXy.y;
    Fix16 zpos = mSpritePtr->mZpos;
    s32 check_z = (zpos.ToInt()) - 1;
    if (mCharState == Char_B4_state::Jumping_15 || mPedState1 == ped_state_1::dead_9 || mpPed->PedTypeIs(ped_type::player_2) ||
        mpPed->get_occupation() == ped_ocupation_enum::drone)
    {
        bMayLeavePavement = true;
    }
    if ((mFlags & 1) == 1)
    {
        check_z = zpos.ToInt();
    }

    s32 x = x_fp.ToInt();
    s32 y = y_fp.ToInt();
    s32 z = zpos.ToInt();
    u8 slope_z_delta;
    if (gMap_0x370->IsMoveOntoSlopeTileBlocked(x, y, z, path_direction, &slope_z_delta, false))
    {
        return false;
    }


    switch (path_direction)
    {
        case path_direction::up_1:
            slope_type = gMap_0x370->GetBlockTypeAtCoord(x_fp.ToInt(), y_fp.ToInt() - 1, check_z);
            break;
        case path_direction::right_3:
            slope_type = gMap_0x370->GetBlockTypeAtCoord(x_fp.ToInt() + 1, y_fp.ToInt(), check_z);
            break;
        case path_direction::down_2:
            slope_type = gMap_0x370->GetBlockTypeAtCoord(x_fp.ToInt(), y_fp.ToInt() + 1, check_z);
            break;
        case path_direction::left_4:
            slope_type = gMap_0x370->GetBlockTypeAtCoord(x_fp.ToInt() - 1, y_fp.ToInt(), check_z);
            break;
        default:
            break;
    }

    Fix16 zpos_frac;
    bool result;

    switch (slope_type)
    {
        case AIR:
            if ((mFlags & 1) != 1)
            {
                return false;
            }
            zpos_frac = zpos.GetFracValue();

            if (zpos_frac < kFpPoint1)
            {
                mFlags &= ~1;
                result = CanMoveOntoSlope(path_direction);
                mFlags |= 1;
                return result;
            }

            if (zpos_frac <= kFP16Half_6FD8E4)
            {
                return false;
            }

            mFlags &= ~1;
            mSpritePtr->mZpos++;
            result = CanMoveOntoSlope(path_direction);
            mSpritePtr->mZpos--;
            mFlags |= 1u;
            break;
        case ROAD:
        case FIELD:
            if (bMayLeavePavement)
            {
                return true;
            }
            return false;
        case PAVEMENT:
        case 4:
            return true;
        default:
            return false;
    }
    return result;
}

// 9.6f 0x495470
MATCH_FUNC(0x54c3e0)
void Char::TurnToFreeAdjacentFace()
{
    bool cw_free = false;
    s32 face = Ang16::GetAngleFace(mRotation);
    if (!CanMoveOntoSlope(face))
    {
        bool clockwise = true;
        s32 cw_face = RotateFace(&face, &clockwise);
        if (CanMoveOntoSlope(cw_face) == 1)
        {
            cw_free = true;
        }

        clockwise = false;
        s32 ccw_face = RotateFace(&face, &clockwise);
        if (CanMoveOntoSlope(ccw_face) == 1)
        {
            if (cw_free == 1)
            {
                if (!(sCharB4UpdateCounter % 2))
                {
                    SetTurnTarget(ReturnAngleFromRoadDirection(ccw_face));
                }
                else
                {
                    SetTurnTarget(ReturnAngleFromRoadDirection(cw_face));
                }
            }
            else
            {
                SetTurnTarget(ReturnAngleFromRoadDirection(ccw_face));
            }
        }
        else if (cw_free == 1)
        {
            SetTurnTarget(ReturnAngleFromRoadDirection(cw_face));
        }
    }
}

// 9.6f 0x495540
MATCH_FUNC(0x54c500)
bool Char::CanMoveToTile(char_type x, char_type y)
{
    Fix16 tx = mSpritePtr->mXy.x;
    Fix16 ty = mSpritePtr->mXy.y;
    char_type dx = x - tx.ToInt();
    char_type dy = y - ty.ToInt();

    // No movement
    if (dx == 0 && dy == 0)
    {
        return true;
    }

    // Diagonal movement not allowed
    if (dx != 0 && dy != 0)
    {
        return false;
    }

    // Pure horizontal
    if (dx != 0)
    {
        return (dx == -1) ? Char::CanMoveOntoSlope(path_direction::left_4) : Char::CanMoveOntoSlope(path_direction::right_3);
    }

    return (dy == -1) ? Char::CanMoveOntoSlope(path_direction::up_1) : Char::CanMoveOntoSlope(path_direction::down_2);
}

MATCH_FUNC(0x54c580)
void Char::SelectRandomIdleBehavior()
{
    if (!this->mTimer)
    {
        switch (sCharB4UpdateCounter)
        {
            case 0:
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
            case 6:
            case 7:
            case 8:
            {
                this->mCharState = Char_B4_state::Walking_1;
                this->mAnimationState = Char_Anim_state::Walking_0;
                this->mTimer = gRng.get_int(400);
                break;
            }
            case 9:
            case 10:
            case 11:
            case 12:
            case 13:
            case 14:
            {
                this->mCharState = Char_B4_state::Walking_Fast_3;
                this->mAnimationState = Char_Anim_state::Walking_0;
                this->mTimer = gRng.get_int(200);
                break;
            }
            case 15:
            {
                this->mCharState = Char_B4_state::Running_4;
                this->mAnimationState = Char_Anim_state::Running_1;
                this->mTimer = gRng.get_int(200);
                break;
            }
            case 20:
            {
                this->mCharState = Char_B4_state::Standing_7;
                this->mAnimationState = Char_Anim_state::Standing_2;
                this->mTimer = gRng.get_int(200);
                break;
            }
            default:
                break;
        }
    }

    if (--this->mTimer == 255 && this->mCharState != Char_B4_state::Turning_To_Target_25)
    {
        this->mTimer = 0;
    }
}

MATCH_FUNC(0x54c6c0)
void Char::ApplyRandomRotationJitter()
{
    if (gRng.get_int(32) > 22)
    {
        Ang16 old_angle = mRotationJitter;
        mRotationJitter = Ang16::Fix16_To_Ang16_inlined_40F540(kFpOne64th_6FD868 * (Fix16(gRng.get_int(16)) - kFP16Eight_6FDA08));
        if (old_angle > kAng180_6FD936 && mRotationJitter > kAng180_6FD936)
        {
            mRotationJitter = -mRotationJitter;
        }
        if (old_angle < kAng180_6FD936 && mRotationJitter < kAng180_6FD936)
        {
            mRotationJitter = -mRotationJitter;
        }
        AddAngle(mRotationJitter);
    }
}

MATCH_FUNC(0x54c900)
void Char::TickMovementStateMachine()
{
    switch (this->mCharState)
    {
        case Char_B4_state::Walking_1:
            ApplyRandomRotationJitter();
            this->mVelocity = kFpOne64th;
            sCharB4TurnToFreeFace = 1;
            gCharB4_VeerFromWalls = 1;
            gCharB4_Unused_6FDB53 = 0;
            break;

        case Char_B4_state::Walking_Fast_3:
            if (this->mTimer)
            {
                this->mVelocity = k_CollisionRepulsionSpeed;
                ApplyRandomRotationJitter();
            }
            else
            {
                this->mVelocity = kFpOne64th;
                this->mCharState = Char_B4_state::Walking_1;
                this->mAnimationState = Char_Anim_state::Walking_0;
            }
            gCharB4_Unused_6FDB53 = 0;
            sCharB4TurnToFreeFace = 1;
            gCharB4_VeerFromWalls = 1;
            break;

        case Char_B4_state::Running_4:
            if (this->mTimer)
            {
                this->mVelocity = kFpThreeSixtyFourths;
                ApplyRandomRotationJitter();
            }
            else
            {
                this->mVelocity = kFpOne64th;
                this->mCharState = Char_B4_state::Walking_1;
                this->mAnimationState = Char_Anim_state::Walking_0;
            }
            gCharB4_Unused_6FDB53 = 0;
            sCharB4TurnToFreeFace = 1;
            gCharB4_VeerFromWalls = 1;
            break;

        case Char_B4_state::Standing_7:
            if (this->mTimer)
            {
                this->mVelocity = kZeroVelocity;
                this->mAnimationState = Char_Anim_state::Standing_2;
                this->mAnimationFrame = 0;
            }
            else
            {
                this->mAnimationState = Char_Anim_state::Walking_0;
                this->mAnimationFrame = 0;
                this->mCharState = Char_B4_state::Walking_1;
                this->mVelocity = kFpOne64th;
            }
            gCharB4_Unused_6FDB53 = 0;
            sCharB4TurnToFreeFace = 1;
            gCharB4_VeerFromWalls = 1;
            break;

        case Char_B4_state::Veer_Right_From_Wall_8:
        case Char_B4_state::Veer_Left_From_Wall_9:
            if (!this->mTimer)
            {
                this->mTimer = 100;
                mRotation.SnapToAng4();
                this->mCharState = Char_B4_state::Walking_1;
                this->mAnimationState = Char_Anim_state::Walking_0;
            }
            gCharB4_VeerFromWalls = 0;
            gCharB4_Unused_6FDB53 = 0;
            sCharB4TurnToFreeFace = 1;
            break;

        case Char_B4_state::Turning_To_Target_25:
            TurnTowardsAngle();
            if (ComputeShortestAngleDelta(this->mRotation, this->mTargetRotation) < kAng12_6FD892)
            {
                this->mVelocity = kFpOne64th;
                this->mCharState = Char_B4_state::Walking_1;
                this->mAnimationState = Char_Anim_state::Walking_0;
                this->mTimer = 0;
            }
            sCharB4TurnToFreeFace = 0;
            gCharB4_VeerFromWalls = 0;
            gCharB4_Unused_6FDB53 = 0;
            break;

        case Char_B4_state::Interacting_Car_Door_36:
            this->mCharState = Char_B4_state::Walking_1;
            break;

        default:
            return;
    }
}

MATCH_FUNC(0x54cae0)
void Char::TurnTowardsAngle()
{
    // Each half jumps into the other half's add block (shared blocks in the original layout),
    // which VC6 only reproduces with these gotos.
    if (mTargetRotation > mRotation)
    {
        if (mTargetRotation - mRotation <= kAng180_6FD920)
        {
            goto add_6FDA54;
        }
    add_6FD9D8:
        this->mRotation += kAng354;
    }
    else
    {
        if (mRotation - mTargetRotation <= kAng180_6FD920)
        {
            goto add_6FD9D8;
        }
    add_6FDA54:
        this->mRotation += kAng6;
    }
}

// https://decomp.me/scratch/RGvHW
MATCH_FUNC(0x54cc40)
void Char::ApplyMovement()
{

    Fix16 xpos;
    Fix16 ypos;

    u8 octant = mRotation.GetOctant();

    switch (sCharB4PathDirection)
    {
        case path_direction::up_1:
            switch (octant)
            {
                case 4:
                case 5:
                case 6:
                case 7:
                    PolarToCartesian_OutOfLineMul(kAng270, mVelocity, xpos, ypos);
                    xpos += gCharB4_Saved_Xpos;
                    ypos += gCharB4_Saved_Ypos;
                    sAng16AngleOfCollision = kAng270;
                    if (Char::CanStepToTile(xpos.ToInt(), ypos.ToInt()))
                    {
                        PolarToCartesian_OutOfLineMul(kAng270, mVelocity / gFix16_Two, xpos, ypos);
                        mSpritePtr->set_xyz_lazy_420600(mSpritePtr->mXy.x + xpos,
                                                                 mSpritePtr->mXy.y + ypos,
                                                                 mSpritePtr->mZpos);
                    }
                    else
                    {
                        PolarToCartesian_OutOfLineMul(kAng0, mVelocity, xpos, ypos);
                        xpos += gCharB4_Saved_Xpos;
                        ypos += gCharB4_Saved_Ypos;
                        sAng16AngleOfCollision = kAng0;
                        if (Char::CanStepToTile(xpos.ToInt(), ypos.ToInt()))
                        {
                            PolarToCartesian_OutOfLineMul(kAng0, mVelocity / gFix16_Two, xpos, ypos);
                            mSpritePtr->set_xyz_lazy_420600(mSpritePtr->mXy.x + xpos,
                                                                     mSpritePtr->mXy.y + ypos,
                                                                     mSpritePtr->mZpos);
                        }
                    }
                    break;
                case 0:
                case 1:
                case 2:
                case 3:
                    PolarToCartesian_OutOfLineMul(kAng90_6FDA64, mVelocity, xpos, ypos);
                    xpos += gCharB4_Saved_Xpos;
                    ypos += gCharB4_Saved_Ypos;
                    sAng16AngleOfCollision = kAng90_6FDA64;
                    if (Char::CanStepToTile(xpos.ToInt(), ypos.ToInt()))
                    {
                        PolarToCartesian_OutOfLineMul(kAng90_6FDA64, mVelocity / gFix16_Two, xpos, ypos);
                        mSpritePtr->set_xyz_lazy_420600(mSpritePtr->mXy.x + xpos,
                                                                 mSpritePtr->mXy.y + ypos,
                                                                 mSpritePtr->mZpos);
                    }
                    else
                    {
                        PolarToCartesian_OutOfLineMul(kAng0, mVelocity, xpos, ypos);
                        xpos += gCharB4_Saved_Xpos;
                        ypos += gCharB4_Saved_Ypos;
                        sAng16AngleOfCollision = kAng0;
                        if (Char::CanStepToTile(xpos.ToInt(), ypos.ToInt()))
                        {
                            PolarToCartesian_OutOfLineMul(kAng0, mVelocity / gFix16_Two, xpos, ypos);
                            mSpritePtr->set_xyz_lazy_420600(mSpritePtr->mXy.x + xpos,
                                                                     mSpritePtr->mXy.y + ypos,
                                                                     mSpritePtr->mZpos);
                        }
                    }
                    break;
                default:
                    return;
            }
            break;

        case path_direction::right_3:
            switch (octant)
            {
                case 2:
                case 3:
                case 4:
                case 5:
                    PolarToCartesian_OutOfLineMul(kAng180_6FD8E8, mVelocity, xpos, ypos);
                    xpos += gCharB4_Saved_Xpos;
                    ypos += gCharB4_Saved_Ypos;
                    sAng16AngleOfCollision = kAng180_6FD8E8;
                    if (Char::CanStepToTile(xpos.ToInt(), ypos.ToInt()))
                    {
                        PolarToCartesian_OutOfLineMul(kAng180_6FD8E8, mVelocity / gFix16_Two, xpos, ypos);
                        mSpritePtr->set_xyz_lazy_420600(mSpritePtr->mXy.x + xpos,
                                                                 mSpritePtr->mXy.y + ypos,
                                                                 mSpritePtr->mZpos);
                    }
                    break;
                case 0:
                case 1:
                case 6:
                case 7:
                    PolarToCartesian_OutOfLineMul(kAng0, mVelocity, xpos, ypos);
                    xpos += gCharB4_Saved_Xpos;
                    ypos += gCharB4_Saved_Ypos;
                    sAng16AngleOfCollision = kAng0;
                    if (Char::CanStepToTile(xpos.ToInt(), ypos.ToInt()))
                    {
                        PolarToCartesian_OutOfLineMul(kAng0, mVelocity / gFix16_Two, xpos, ypos);
                        mSpritePtr->set_xyz_lazy_420600(mSpritePtr->mXy.x + xpos,
                                                                 mSpritePtr->mXy.y + ypos,
                                                                 mSpritePtr->mZpos);
                    }
                    break;
                default:
                    return;
            }
            break;

        case path_direction::down_2:
            switch (octant)
            {
                case 0:
                case 1:
                case 2:
                case 3:
                    PolarToCartesian_OutOfLineMul(kAng90_6FDA64, mVelocity, xpos, ypos);
                    xpos += gCharB4_Saved_Xpos;
                    ypos += gCharB4_Saved_Ypos;
                    sAng16AngleOfCollision = kAng90_6FDA64;
                    if (Char::CanStepToTile(xpos.ToInt(), ypos.ToInt()))
                    {
                        PolarToCartesian_OutOfLineMul(kAng90_6FDA64, mVelocity / gFix16_Two, xpos, ypos);
                        mSpritePtr->set_xyz_lazy_420600(mSpritePtr->mXy.x + xpos,
                                                                 mSpritePtr->mXy.y + ypos,
                                                                 mSpritePtr->mZpos);
                    }
                    else
                    {
                        PolarToCartesian_OutOfLineMul(kAng180_6FD8E8, mVelocity, xpos, ypos);
                        xpos += gCharB4_Saved_Xpos;
                        ypos += gCharB4_Saved_Ypos;
                        sAng16AngleOfCollision = kAng180_6FD8E8;
                        if (Char::CanStepToTile(xpos.ToInt(), ypos.ToInt()))
                        {
                            PolarToCartesian_OutOfLineMul(kAng180_6FD8E8, mVelocity / gFix16_Two, xpos, ypos);
                            mSpritePtr->set_xyz_lazy_420600(mSpritePtr->mXy.x + xpos,
                                                                     mSpritePtr->mXy.y + ypos,
                                                                     mSpritePtr->mZpos);
                        }
                    }
                    break;
                case 4:
                case 5:
                case 6:
                case 7:
                    PolarToCartesian_OutOfLineMul(kAng270, mVelocity, xpos, ypos);
                    xpos += gCharB4_Saved_Xpos;
                    ypos += gCharB4_Saved_Ypos;
                    sAng16AngleOfCollision = kAng270;
                    if (Char::CanStepToTile(xpos.ToInt(), ypos.ToInt()))
                    {
                        PolarToCartesian_OutOfLineMul(kAng270, mVelocity / gFix16_Two, xpos, ypos);
                        mSpritePtr->set_xyz_lazy_420600(mSpritePtr->mXy.x + xpos,
                                                                 mSpritePtr->mXy.y + ypos,
                                                                 mSpritePtr->mZpos);
                    }
                    else
                    {
                        PolarToCartesian_OutOfLineMul(kAng180_6FD8E8, mVelocity, xpos, ypos);
                        xpos += gCharB4_Saved_Xpos;
                        ypos += gCharB4_Saved_Ypos;
                        sAng16AngleOfCollision = kAng180_6FD8E8;
                        if (Char::CanStepToTile(xpos.ToInt(), ypos.ToInt()))
                        {
                            PolarToCartesian_OutOfLineMul(kAng180_6FD8E8, mVelocity / gFix16_Two, xpos, ypos);
                            mSpritePtr->set_xyz_lazy_420600(mSpritePtr->mXy.x + xpos,
                                                                     mSpritePtr->mXy.y + ypos,
                                                                     mSpritePtr->mZpos);
                        }
                    }
                    break;

                default:
                    return;
            }
            break;

        case path_direction::left_4:
            switch (octant)
            {
                case 2:
                case 3:
                case 4:
                case 5:
                    PolarToCartesian_OutOfLineMul(kAng180_6FD8E8, mVelocity, xpos, ypos);
                    xpos += gCharB4_Saved_Xpos;
                    ypos += gCharB4_Saved_Ypos;
                    sAng16AngleOfCollision = kAng180_6FD8E8;
                    if (Char::CanStepToTile(xpos.ToInt(), ypos.ToInt()))
                    {
                        PolarToCartesian_OutOfLineMul(kAng180_6FD8E8, mVelocity / gFix16_Two, xpos, ypos);
                        mSpritePtr->set_xyz_lazy_451950(mSpritePtr->mXy.x + xpos,
                                                                 mSpritePtr->mXy.y + ypos,
                                                                 mSpritePtr->mZpos);
                    }
                    break;
                case 0:
                case 1:
                case 6:
                case 7:
                    Ang16::PolarToCartesian_451730(kAng0, mVelocity, xpos, ypos);
                    xpos += gCharB4_Saved_Xpos;
                    ypos += gCharB4_Saved_Ypos;
                    sAng16AngleOfCollision = kAng0;
                    if (Char::CanStepToTile(xpos.ToInt(), ypos.ToInt()))
                    {
                        // Out-of-line operators (inline budget)
                        Ang16::PolarToCartesian_451730(kAng0, mVelocity.Divide(gFix16_Two), xpos, ypos);
                        mSpritePtr->set_xyz_lazy_451950(mSpritePtr->mXy.x.Add(xpos),
                                                                 mSpritePtr->mXy.y.Add(ypos),
                                                                 mSpritePtr->mZpos);
                    }
                    break;

                default:
                    return;
            }
            break;
        default:
            return;
    }
}

MATCH_FUNC(0x54dd70)
void Char::CheckCollisionsAndUpdateAnim()
{
    if (this->mPedState1 != ped_state_1::dead_9 && this->mCharState != Char_B4_state::Jumping_15)
    {
        CheckAndHandleCollisions();

        if (mpPed->GetAttacking() && mpPed->mBf21C.bNoWeapon)
        {
            if (this->mAnimationState != Char_Anim_state::Punching_4)
            {
                this->mAnimationState = Char_Anim_state::Punching_4;
                this->mAnimationFrame = 0;
            }
            else
            {
                mpPed->HandleClosePedInteraction();
            }
        }
        else if (this->mAnimationState != Char_Anim_state::Punching_4 || this->mAnimationFrame == 6)
        {
            if (mVelocity != kZeroVelocity)
            {
                this->mAnimationState = mVelocity > k_CollisionRepulsionSpeed;
            }
            else
            {
                this->mAnimationState = Char_Anim_state::Standing_2;
            }
        }
    }
}

// Inline helper: written out in the caller, VC6 lays the out-of-range `return false` inline and
// merges the later `return false` into it. As an inline returning true/false it keeps the original's
// inline `return true` and one shared `return false` at the end.
static inline bool IsBlockTypeInRange_1_4(u8 block_type)
{
    if (block_type > 0 && block_type <= 4)
    {
        return true;
    }
    return false;
}

// https://decomp.me/scratch/56qkT
MATCH_FUNC(0x54ddf0)
void Char::UpdateWalking()
{
    char_type bCanMove;
    char_type bAllowStateChange;
    u8 bTileOk;

    sCharB4TurnToFreeFace = 1;
    gCharB4_VeerFromWalls = 1;
    gCharB4_Unused_6FDB53 = 0;
    sCharB4GradientSlopeBelow = 0;
    this->mTimeout = 500;
    bAllowStateChange = 1;
    bCanMove = 0;
    u8 bNoNewRotation = 1;
    Ang16 side_angle(0);
    Ang16 new_rotation(0);


    u8 block_type;
    gmp_block_info* pBlock;

    if ((mFlags & 1) == 0 && gCharB4_Saved_Zpos.GetFracValue() == kFP16Zero_6FD9E4)
    {
        block_type = gMap_0x370->GetBlockTypeAtCoord(gCharB4_Saved_Xpos.ToInt(), gCharB4_Saved_Ypos.ToInt(), gCharB4_Saved_Zpos.ToInt() - 1);
        pBlock = gMap_0x370->GetBlock(gCharB4_Saved_Xpos.ToInt(), gCharB4_Saved_Ypos.ToInt(), gCharB4_Saved_Zpos.ToInt() - 1);
    }
    else
    {
        block_type = gMap_0x370->GetBlockTypeAtCoord(gCharB4_Saved_Xpos.ToInt(), gCharB4_Saved_Ypos.ToInt(), gCharB4_Saved_Zpos.ToInt());
        pBlock = gMap_0x370->GetBlock(gCharB4_Saved_Xpos.ToInt(), gCharB4_Saved_Ypos.ToInt(), gCharB4_Saved_Zpos.ToInt());

        if (pBlock == NULL || block_type == 0)
        {
            block_type =
                gMap_0x370->GetBlockTypeAtCoord(gCharB4_Saved_Xpos.ToInt(), gCharB4_Saved_Ypos.ToInt(), gCharB4_Saved_Zpos.ToInt() - 1);
            pBlock = gMap_0x370->GetBlock(gCharB4_Saved_Xpos.ToInt(), gCharB4_Saved_Ypos.ToInt(), gCharB4_Saved_Zpos.ToInt() - 1);
        }
    }

    if (pBlock)
    {
        mFlagsBf.bOnSlope = GET_SLOPE_BITS(pBlock->mSlopeType) != 0 && GET_SLOPE_BITS(pBlock->mSlopeType) != 0xFC;
        if (gGtx_0x106C->IsElectrifiedFloorType(pBlock->mLid & block_face::tile_idx_mask) &&
            mCharState != Char_B4_state::Jumping_15)
        {
            if (!mpPed->mBf21C.bLeftVehicle)
            {
                mpPed->mShockCounter += 3;
                s32 killer_id = mpPed->mKillerId;
                if (killer_id != 0)
                {
                    if (gPedManager->PedById(killer_id))
                    {
                        mpPed->mDeathCause = ped_death_cause::electrocuted_2;
                        mpPed->mKillerIdTimer = 50;
                    }
                }
            }
        }
    }
    if (static_cast<u8>(Char::IsOnWater()))
    {
        mpPed->PutOutFire();
        Char::DrownPed();
        return;
    }

    Fix16 ret1;
    Fix16 ret2;

    if (block_type == 0 && (mFlags & 1) == 0)
    {
        // 10.5 has a separate copy for the non-player case, so this is a nested if, not one && chain
        if (mpPed->mPlayer)
        {
            if (mpPed->GetMoveDirection() < kFP16Zero_6FD9E4 || (mFlags & 8) != 0)
            {
                Ang16::PolarToCartesian_41FC20(mRotation + kAng180_6FD936, kFpThreeThirtySeconds, ret1, ret2);
            }
            else
            {
                Ang16::PolarToCartesian_41FC20(mRotation, kFpThreeThirtySeconds, ret1, ret2);
            }
        }
        else
        {
            Ang16::PolarToCartesian_41FC20(mRotation, kFpThreeThirtySeconds, ret1, ret2);
        }

        ret1 += gCharB4_Saved_Xpos;
        ret2 += gCharB4_Saved_Ypos;

        // One GetBlockTypeAtCoord call per z case: VC6 tail-merges the two copies (gMap in edx, the
        // null arm first and the unfolded test after the merge)
        if (gCharB4_Saved_Zpos.GetFracValue() == kFP16Zero_6FD9E4)
        {
            block_type = gMap_0x370->GetBlockTypeAtCoord(ret1.ToInt(), ret2.ToInt(), gCharB4_Saved_Zpos.ToInt() - 1);
        }
        else
        {
            block_type = gMap_0x370->GetBlockTypeAtCoord(ret1.ToInt(), ret2.ToInt(), gCharB4_Saved_Zpos.ToInt());
        }
        if (block_type == AIR)
        {
            if (mCharState != Char_B4_state::Jumping_15)
            {
                // Ped is falling; the end-of-jump path below jumps here (goto-free forms change the code)
            start_falling:
                mAnimationState = Char_Anim_state::Normal_Fall_11;
                mAnimationFrame = 0;
                mpPed->ChangeNextPedState1(ped_state_1::immobilized_8);
                mpPed->ChangeNextPedState2(ped_state_2::falling_19);
                mPedState1 = ped_state_1::immobilized_8;
                mPedState2 = ped_state_2::falling_19;
                if (mpPed->mPlayer)
                {
                    if (mpPed->GetMoveDirection() < kFP16Zero_6FD9E4 || (mFlags & 8) != 0)
                    {
                        mVelocity = -mVelocity;
                    }
                }
                mFallSpeed = mVelocity;
                mFallZSpeed = kFP16Zero_6FD9E4;
                if (mVelocity == kZeroVelocity)
                {
                    mStateInitPending = true;
                }
                Char::UpdateImmobilized();
                return;
            }
            // A jumping ped only starts falling once the jump is over
            if (this->mAnimationFrame == 7 || this->mpPed->mBf21C.bAttacking)
            {
                this->mCharState = Char_B4_state::Walking_1;
                mpPed->ClearAttacking_403A40();
                goto start_falling;
            }
        }
    }

    mBlockType = block_type;
    Fix16 radius;
    if (mpPed->PedTypeIs(ped_type::player_2) == true)
    {
        radius = kFpThreeThirtySeconds;
        if (this->mCharState)
        {
            this->mTimer = 9999;
        }
        mRotation += mpPed->GetTurnSpeed();

        if (this->mCharState == Char_B4_state::Jumping_15)
        {
            mpPed->ClearAttacking_403A40();
            if (this->mAnimationState != Char_Anim_state::Jumping_5)
            {
                this->mAnimationState = Char_Anim_state::Jumping_5;
                this->mAnimationFrame = 0;
            }
            if (mpPed->GetMoveDirection() > kFP16Zero_6FD9E4)
            {
                if (mVelocity == kZeroVelocity)
                {
                    this->mVelocity = this->mRunOrJumpSpeed;
                }
                else
                {
                    if (mRunOrJumpSpeed > mVelocity)
                    {
                        this->mVelocity += kFpOne256th_6FD99C;
                    }
                    else
                    {
                        if (mVelocity > mRunOrJumpSpeed)
                        {
                            this->mVelocity -= kFpOne256th_6FD99C;
                        }
                    }
                }
            }
            else
            {
                this->mVelocity = sRunOrJumpSpeed;
            }
        }
        else
        {
            if (mpPed->GetMoveDirection() > kFP16Zero_6FD9E4)
            {
                if (mVelocity == kZeroVelocity)
                {
                    this->mVelocity = this->mRunOrJumpSpeed;
                    mFlags &= ~8;
                }
                else
                {
                    if (mVelocity < mRunOrJumpSpeed)
                    {
                        this->mVelocity += kFpOne256th_6FD99C;
                        mFlags &= ~8;
                    }
                    else
                    {
                        if (mVelocity > mRunOrJumpSpeed)
                        {
                            this->mVelocity -= kFpOne256th_6FD99C;
                        }
                        mFlags &= ~8;
                    }
                }
            }
            else
            {
                if (mpPed->GetMoveDirection() < kFP16Zero_6FD9E4)
                {
                    new_rotation = this->mRotation;
                    this->mFlags = this->mFlags | 8;
                    mRotation += kAng180_6FD936;

                    if (mVelocity == kZeroVelocity)
                    {
                        bNoNewRotation = 0;
                        this->mVelocity = this->mRunOrJumpSpeed;
                    }
                    else
                    {
                        if (mVelocity < mRunOrJumpSpeed)
                        {
                            bNoNewRotation = 0;
                            this->mVelocity += kFpOne256th_6FD99C;
                        }
                        else
                        {
                            if (mVelocity > mRunOrJumpSpeed)
                            {
                                this->mVelocity -= kFpOne256th_6FD99C;
                            }
                            bNoNewRotation = 0;
                        }
                    }
                }
                else
                {
                    if (this->mRepulsionTimer)
                    {
                        sCharB4TurnToFreeFace = 0;
                        gCharB4_VeerFromWalls = 0;
                        new_rotation = mRotation;
                        this->mRotation = this->mRepulsionRotation;

                        bNoNewRotation = 0;
                        this->mVelocity = k_CollisionRepulsionSpeed;
                    }
                    else
                    {
                        this->mVelocity = kZeroVelocity;
                        if (this->mCharState)
                        {
                            this->mCharState = Char_B4_state::Standing_7;
                        }
                    }
                }
            }
        }

        if (this->mpPed->mBf21C.bHitByAttacker)
        {
            this->mVelocity = k_CollisionRepulsionSpeed;
        }
    }
    else
    {
        radius = kFpThreeThirtySeconds;
        if (this->mIsCollidingWithSprite || this->mCharState == Char_B4_state::Jumping_15)
        {
            bAllowStateChange = 0;
            mpPed->mBf21C.bAttacking = false;
            sCharB4TurnToFreeFace = 0;
            gCharB4_VeerFromWalls = 0;
        }
        else
        {
            if (this->mRepulsionTimer)
            {
                bAllowStateChange = 0;
                sCharB4TurnToFreeFace = 0;
                gCharB4_VeerFromWalls = 0;
                new_rotation = mRotation;
                this->mRotation = this->mRepulsionRotation;
                this->mVelocity = k_CollisionRepulsionSpeed;
                bNoNewRotation = 0;
            }
            if (this->mVelocity < kZeroVelocity)
            {
                Ang16 rotation = mRotation;
                new_rotation = rotation;
                rotation += kAng180_6FD936;
                mRotation = rotation;
                mFlags = mFlags | 0x8;
                mVelocity = Fix16::Abs(mVelocity);
                bNoNewRotation = 0;
            }
        }
        if (mCharState)
        {
            if (mCharState != Char_B4_state::Jumping_15 && this->mPedState1 != ped_state_1::dead_9)
            {
                switch (block_type)
                {
                    case ROAD:
                    case FIELD:
                        if (mpPed->get_objective() == objectives_enum::no_obj_0 ||
                            mpPed->get_objective() == objectives_enum::left_behind_by_leader_8)
                        {
                            mpPed->SetObjective2(objectives_enum::walk_to_point_17);
                        }
                        break;
                    default:
                        if (this->mPedState2 == ped_state_2::ped2_walking_0 && bAllowStateChange)
                        {
                            Char::SelectRandomIdleBehavior();
                            Char::TickMovementStateMachine();
                        }
                        break;
                }
            }
        }
    }

    if (bNoNewRotation)
    {
        new_rotation = mRotation;
    }
    if (mVelocity == kFP16Zero_6FD9E4)
    {
        mFlags &= ~8;
    }

    Ang16::PolarToCartesian_41FC20(mRotation, radius, ret1, ret2);

    ret1 += gCharB4_Saved_Xpos;
    ret2 += gCharB4_Saved_Ypos;
    ret1 += mConveyorDx;
    ret2 += mConveyorDy;

    if (!Char::IsNearTileCentre())
    {
        if (mpPed->PedTypeIs(ped_type::player_2) == true)
        {
            bCanMove = 1;
            sAng16AngleOfCollision = mRotation;
            u8 bDiagonalOk = Char::CanStepToTile(ret1.ToInt(), ret2.ToInt());
            if (gMap_0x370->TakeBlockedByTerrain() != 1 && !bDiagonalOk)
            {
                Char::ApplyMovement();
                bCanMove = 0;
                goto after_move;
            }
        }
        else
        {
            bTileOk = Char::CanMoveToTile(ret1.ToInt(), ret2.ToInt());

            if (sCharB4TurnToFreeFace)
            {
                Char::TurnToFreeAdjacentFace();
            }

            if (bTileOk == 1)
            {
                bCanMove = 1;
                if (gCharB4_VeerFromWalls)
                {
                    side_angle = mRotation;
                    side_angle.SnapToAng4();
                    side_angle += kAng90_6FD8A2;
                    Fix16 xpos_3;
                    Fix16 ypos_3;
                    Ang16::PolarToCartesian_41FC20(side_angle, kFP16Half, xpos_3, ypos_3);

                    xpos_3 += gCharB4_Saved_Xpos;
                    ypos_3 += gCharB4_Saved_Ypos;

                    if (Char::CanMoveToTile(xpos_3.ToInt(), ypos_3.ToInt()) == 1)
                    {
                        side_angle = mRotation;
                        side_angle.SnapToAng4();
                        side_angle += kAng270_6FD94C;

                        Fix16 xpos_4;
                        Fix16 ypos_4;

                        Ang16::PolarToCartesian_41FC20(side_angle, kFP16Half, xpos_4, ypos_4);

                        xpos_4 += gCharB4_Saved_Xpos;
                        ypos_4 += gCharB4_Saved_Ypos;

                        if (!Char::CanMoveToTile(xpos_4.ToInt(), ypos_4.ToInt()))
                        {
                            VeerLeftFromWall();
                        }
                    }
                    else
                    {
                        VeerRightFromWall();
                    }
                }
            }
            else
            {
                Char::SnapRotationToFace();
                goto after_move;
            }
        }
    }

    Ang16::PolarToCartesian_41FC20(mRotation, mVelocity, ret1, ret2);
    ret1 += mConveyorDx;
    ret2 += mConveyorDy;
    mSpritePtr->set_xyz_lazy_420600(mSpritePtr->mXy.x + ret1,
                                             mSpritePtr->mXy.y + ret2,
                                             mSpritePtr->mZpos);

// shared exit for the two blocked-move paths: goto-free forms change the code
after_move:
    mConveyorDx = kFP16Zero_6FD9E4;
    mConveyorDy = kFP16Zero_6FD9E4;
    if (bCanMove == 1 || (mFlags & 1) == 1)
    {
        sCharB4GradientSlopeBelow = gMap_0x370->IsGradientSlopeAt(mSpritePtr->mXy.x.ToInt(),
                                                                  mSpritePtr->mXy.y.ToInt(),
                                                                  mSpritePtr->mZpos.ToInt() - 1);
        Char::ManageZCoordAndSlopes();
    }

    Char::DispatchCollision(sCharB4MoveIgnorePeds);

    if (!mpPed->PedTypeIs(ped_type::player_2))
    {
        if (mIsCollidingWithSprite)
        {
            if (Char::ContinueMovementAfterCollision() == 1)
            {
                // The sprite position before the reset: CanReachTile tests it and it is put back afterwards
                Fix16 old_xpos = mSpritePtr->mXy.x;
                Fix16 old_ypos = mSpritePtr->mXy.y;
                char_type tile_x = old_xpos.ToInt();
                char_type tile_y = old_ypos.ToInt();
                if (static_cast<char_type>(gCharB4_Saved_Xpos.ToInt()) != tile_x || static_cast<char_type>(gCharB4_Saved_Ypos.ToInt()) != tile_y)
                {
                    mSpritePtr->set_xyz_lazy_420600(gCharB4_Saved_Xpos, gCharB4_Saved_Ypos, gCharB4_Saved_Zpos);

                    mSlopeGradientDirection = sCharB4SavedSlopeGradDir.ToInt();
                    if (!Char::CanReachTile(tile_x, tile_y))
                    {
                        mIsCollidingWithSprite = false;
                        mJumpOverTimer = 10;
                    }
                    else
                    {
                        mSpritePtr->set_xyz_lazy_420600(old_xpos, old_ypos, gCharB4_Saved_Zpos);
                        sCharB4GradientSlopeBelow = gMap_0x370->IsGradientSlopeAt(mSpritePtr->mXy.x.ToInt(),
                                                                    mSpritePtr->mXy.y.ToInt(),
                                                                    (mSpritePtr->mZpos - kFP16One_6FD9E8).ToInt());
                        Char::ManageZCoordAndSlopes();
                    }
                }
            }
        }
    }

    if (!bNoNewRotation)
    {
        mRotation = new_rotation;
    }

    Char::CheckCollisionsAndUpdateAnim();
}

// https://decomp.me/scratch/Zk9Eh
MATCH_FUNC(0x54ecb0)
bool Char::CanStepForwardWithRegionCheck(s32 direction)
{
    u8 slope_z_delta;

    Fix16 xpos = mSpritePtr->mXy.x;
    Fix16 ypos = mSpritePtr->mXy.y;
    Fix16 zpos = mSpritePtr->mZpos;

    s32 new_zpos = (zpos).ToInt() - 1;
    s32 below_z = (zpos - kFP16One_6FD9E8).ToInt();

    if (gMap_0x370->IsGradientSlopeAt(xpos.ToInt(), ypos.ToInt(), below_z))
    {
        new_zpos = zpos.ToInt();
    }
    if (gMap_0x370->IsMoveOntoSlopeTileBlocked(xpos.ToInt(), ypos.ToInt(), zpos.ToInt(), direction, &slope_z_delta, false))
    {
        sCharB4PathDirection = direction;
        return false;
    }
    if (sCharB4StepPosValid)
    {
        mSpritePtr->set_xyz_lazy_420600(gCharB4_StepXpos, sCharB4StepYpos, zpos);
        Char::ManageZCoordAndSlopes();
        if (mSpritePtr->CheckSpriteMovementRegion())
        {
            sCharB4PathDirection = direction;

            mSpritePtr->set_xyz_lazy_420600(xpos, ypos, zpos);
            gMap_0x370->ClearBlockedByTerrain();
            return false;
        }
        mSpritePtr->set_xyz_lazy_420600(xpos, ypos, zpos);
        Char::ManageZCoordAndSlopes();
    }

    u8 block_type;

    switch (direction)
    {
        case path_direction::up_1:
            block_type = gMap_0x370->GetBlockTypeAtCoord(xpos.ToInt(), ypos.ToInt() - 1, new_zpos);
            break;
        case path_direction::right_3:
            block_type = gMap_0x370->GetBlockTypeAtCoord(xpos.ToInt() + 1, ypos.ToInt(), new_zpos);
            break;
        case path_direction::down_2:
            block_type = gMap_0x370->GetBlockTypeAtCoord(xpos.ToInt(), ypos.ToInt() + 1, new_zpos);
            break;
        case path_direction::left_4:
            block_type = gMap_0x370->GetBlockTypeAtCoord(xpos.ToInt() - 1, ypos.ToInt(), new_zpos);
            break;
        default:
            block_type = AIR;
            break;
    }

    if (block_type != AIR)
    {
        return IsBlockTypeInRange_1_4(block_type);
    }

    if ((mFlags & 1) == 1)
    {
        if (zpos.GetFracValue() < kFpPoint3)
        {
            mFlags &= ~1u;
            bool result = Char::CanStepForward(direction);
            mFlags |= 1u;
            return result;
        }
        if (!mpPed->PedTypeIs(ped_type::player_2))
        {
            return false;
        }
    }
    return true;
}

// https://decomp.me/scratch/Ub1EN
// 9.6f 0x495BF0: the collision offset division is written at each use (VC6 CSEs it), bCanStep is set
// in each leaf and leaves 2-4 return explicitly (leaf 1's tail is the cross-jump keeper).
MATCH_FUNC(0x54EF60)
bool Char::CanStepToTile(char_type target_x, char_type target_y)
{
    bool bIsNearXposBlockBoundary = true;
    bool bIsNearYposBlockBoundary = true;

    Fix16 sprite_xpos = mSpritePtr->mXy.x;
    Fix16 sprite_ypos = mSpritePtr->mXy.y;
    Fix16 sprite_zpos = mSpritePtr->mZpos;

    u8 old_slope_dir = mSlopeGradientDirection;
    sCharB4StepPosValid = 1;
    Fix16 xpos, ypos;
    Ang16::PolarToCartesian_41FC20(sAng16AngleOfCollision, sFP16CollisionCheckRadius, xpos, ypos);
    gCharB4_StepXpos = gCharB4_Saved_Xpos + xpos;
    sCharB4StepYpos = gCharB4_Saved_Ypos + ypos;


    // Get the x diff of the neareast block on left/right side, if it's very close to them
    // Test if it is near right block
    s8 x_diff = (gCharB4_StepXpos + gCharB4_WorldCollisionOffset / gFix16_Two).ToInt() - (sprite_xpos).ToInt();

    if (x_diff == 0)
    {
        // Test if it is near left block
        x_diff = (gCharB4_StepXpos - gCharB4_WorldCollisionOffset / gFix16_Two).ToInt() - (sprite_xpos).ToInt();
        if (x_diff == 0)
        {
            bIsNearXposBlockBoundary = false;
        }
    }

    // Get the y diff of the neareast block on north/south side, if it's very close to them
    // Test if it is near south block
    s8 y_diff = (sCharB4StepYpos + gCharB4_WorldCollisionOffset / gFix16_Two).ToInt() - (sprite_ypos).ToInt();

    if (y_diff == 0)
    {
        // Test if it is near north block
        y_diff = (sCharB4StepYpos - gCharB4_WorldCollisionOffset / gFix16_Two).ToInt() - (sprite_ypos).ToInt();
        if (y_diff == 0)
        {
            bIsNearYposBlockBoundary = false;
        }
    }

    if (!bIsNearXposBlockBoundary && !bIsNearYposBlockBoundary)
    {
        x_diff = target_x - mSpritePtr->mXy.x.ToInt();
        y_diff = target_y - mSpritePtr->mXy.y.ToInt();
        if (x_diff == 0 && y_diff == 0)
        {
            return true;
        }
    }

    if (x_diff != 0 && y_diff != 0)
    {
        // Diagonal step: try both axis neighbours of the target block
        bool bCanStep;
        if (x_diff == -1)
        {
            if (y_diff == -1)
            {
                bCanStep = true;
                mSpritePtr->mXy.x.subtract_one();
                if (mFlagsBf.bOnSlope)
                {
                    mSpritePtr->mZpos =
                        gMap_0x370->FindGroundZForCoord(mSpritePtr->mXy.x, mSpritePtr->mXy.y);
                }
                if (mSpritePtr->mZpos == kFP16Zero_6FD9E4)
                {
                    mSpritePtr->mZpos = sprite_zpos;
                }
                if (!CanStepForwardWithRegionCheck(path_direction::up_1) && !gMap_0x370->TakeBlockedByTerrain())
                {
                    sCharB4PathDirection = path_direction::up_1;
                    bCanStep = false;
                }
                mSpritePtr->mXy.x = sprite_xpos;
                mSpritePtr->mXy.y.subtract_one();
                if (mFlagsBf.bOnSlope)
                {
                    mSpritePtr->mZpos =
                        gMap_0x370->FindGroundZForCoord(mSpritePtr->mXy.x, mSpritePtr->mXy.y);
                }
                if (mSpritePtr->mZpos == kFP16Zero_6FD9E4)
                {
                    mSpritePtr->mZpos = sprite_zpos;
                }
                if (!CanStepForwardWithRegionCheck(path_direction::left_4) && !gMap_0x370->TakeBlockedByTerrain())
                {
                    sCharB4PathDirection = path_direction::left_4;
                    bCanStep = false;
                }
                mSpritePtr->mXy.x = sprite_xpos;
                mSpritePtr->mXy.y = sprite_ypos;
                mSpritePtr->mZpos = sprite_zpos;

                if (!bCanStep)
                {
                    gCharB4_StepXpos = gCharB4_Saved_Xpos;
                    sCharB4StepYpos = gCharB4_Saved_Ypos;
                    if (!CanStepForwardWithRegionCheck(path_direction::left_4) && !gMap_0x370->TakeBlockedByTerrain())
                    {
                        sCharB4PathDirection = path_direction::left_4;
                    }
                    else
                    {
                        sCharB4PathDirection = path_direction::up_1;
                        bCanStep = true;
                        if (!CanStepForwardWithRegionCheck(path_direction::up_1) && !gMap_0x370->TakeBlockedByTerrain())
                        {
                            bCanStep = false;
                        }
                        else
                        {
                            sCharB4PathDirection = path_direction::left_4;
                        }
                    }
                }
                else
                {
                    if (!CanStepForwardWithRegionCheck(path_direction::left_4) && !gMap_0x370->TakeBlockedByTerrain())
                    {
                        bCanStep = false;
                        sCharB4PathDirection = path_direction::left_4;
                    }
                    if (!CanStepForwardWithRegionCheck(path_direction::up_1) && !gMap_0x370->TakeBlockedByTerrain())
                    {
                        bCanStep = false;
                        sCharB4PathDirection = path_direction::down_2;
                    }
                }
                mSlopeGradientDirection = old_slope_dir;
            }
            else
            {
                bCanStep = true;
                mSpritePtr->mXy.x.subtract_one();
                if (mFlagsBf.bOnSlope)
                {
                    mSpritePtr->mZpos =
                        gMap_0x370->FindGroundZForCoord(mSpritePtr->mXy.x, mSpritePtr->mXy.y);
                }
                if (mSpritePtr->mZpos == kFP16Zero_6FD9E4)
                {
                    mSpritePtr->mZpos = sprite_zpos;
                }
                if (!CanStepForwardWithRegionCheck(path_direction::down_2) && !gMap_0x370->TakeBlockedByTerrain())
                {
                    sCharB4PathDirection = path_direction::down_2;
                    bCanStep = false;
                }
                mSpritePtr->mXy.x = sprite_xpos;
                mSpritePtr->mXy.y.add_one();
                if (mFlagsBf.bOnSlope)
                {
                    mSpritePtr->mZpos =
                        gMap_0x370->FindGroundZForCoord(mSpritePtr->mXy.x, mSpritePtr->mXy.y);
                }
                if (mSpritePtr->mZpos == kFP16Zero_6FD9E4)
                {
                    mSpritePtr->mZpos = sprite_zpos;
                }
                if (!CanStepForwardWithRegionCheck(path_direction::left_4) && !gMap_0x370->TakeBlockedByTerrain())
                {
                    sCharB4PathDirection = path_direction::left_4;
                    bCanStep = false;
                }
                mSpritePtr->mXy.x = sprite_xpos;
                mSpritePtr->mXy.y = sprite_ypos;
                mSpritePtr->mZpos = sprite_zpos;

                if (!bCanStep)
                {
                    gCharB4_StepXpos = gCharB4_Saved_Xpos;
                    sCharB4StepYpos = gCharB4_Saved_Ypos;
                    if (!CanStepForwardWithRegionCheck(path_direction::left_4) && !gMap_0x370->TakeBlockedByTerrain())
                    {
                        sCharB4PathDirection = path_direction::left_4;
                    }
                    else
                    {
                        sCharB4PathDirection = path_direction::down_2;
                        bCanStep = true;
                        if (!CanStepForwardWithRegionCheck(path_direction::down_2) && !gMap_0x370->TakeBlockedByTerrain())
                        {
                            bCanStep = false;
                        }
                        else
                        {
                            sCharB4PathDirection = path_direction::left_4;
                        }
                    }
                }
                else
                {
                    if (!CanStepForwardWithRegionCheck(path_direction::left_4) && !gMap_0x370->TakeBlockedByTerrain())
                    {
                        bCanStep = false;
                        sCharB4PathDirection = path_direction::left_4;
                    }
                    if (!CanStepForwardWithRegionCheck(path_direction::down_2) && !gMap_0x370->TakeBlockedByTerrain())
                    {
                        bCanStep = false;
                        sCharB4PathDirection = path_direction::down_2;
                    }
                }
                mSlopeGradientDirection = old_slope_dir;
                return bCanStep;
            }
        }
        else
        {
            if (y_diff == -1)
            {
                bCanStep = true;
                mSpritePtr->mXy.x.add_one();
                if (mFlagsBf.bOnSlope)
                {
                    mSpritePtr->mZpos =
                        gMap_0x370->FindGroundZForCoord(mSpritePtr->mXy.x, mSpritePtr->mXy.y);
                }
                if (mSpritePtr->mZpos == kFP16Zero_6FD9E4)
                {
                    mSpritePtr->mZpos = sprite_zpos;
                }
                if (!CanStepForwardWithRegionCheck(path_direction::up_1) && !gMap_0x370->TakeBlockedByTerrain())
                {
                    sCharB4PathDirection = path_direction::up_1;
                    bCanStep = false;
                }
                mSpritePtr->mXy.x = sprite_xpos;
                mSpritePtr->mXy.y.subtract_one();
                if (mFlagsBf.bOnSlope)
                {
                    mSpritePtr->mZpos =
                        gMap_0x370->FindGroundZForCoord(mSpritePtr->mXy.x, mSpritePtr->mXy.y);
                }
                if (mSpritePtr->mZpos == kFP16Zero_6FD9E4)
                {
                    mSpritePtr->mZpos = sprite_zpos;
                }
                if (!CanStepForwardWithRegionCheck(path_direction::right_3) && !gMap_0x370->TakeBlockedByTerrain())
                {
                    sCharB4PathDirection = path_direction::right_3;
                    bCanStep = false;
                }
                mSpritePtr->mXy.x = sprite_xpos;
                mSpritePtr->mXy.y = sprite_ypos;
                mSpritePtr->mZpos = sprite_zpos;

                if (!bCanStep)
                {
                    gCharB4_StepXpos = gCharB4_Saved_Xpos;
                    sCharB4StepYpos = gCharB4_Saved_Ypos;
                    if (!CanStepForwardWithRegionCheck(path_direction::right_3) && !gMap_0x370->TakeBlockedByTerrain())
                    {
                        sCharB4PathDirection = path_direction::right_3;
                    }
                    else
                    {
                        sCharB4PathDirection = path_direction::up_1;
                        bCanStep = true;
                        if (!CanStepForwardWithRegionCheck(path_direction::up_1) && !gMap_0x370->TakeBlockedByTerrain())
                        {
                            bCanStep = false;
                        }
                        else
                        {
                            sCharB4PathDirection = path_direction::right_3;
                        }
                    }
                }
                else
                {
                    if (!CanStepForwardWithRegionCheck(path_direction::right_3) && !gMap_0x370->TakeBlockedByTerrain())
                    {
                        bCanStep = false;
                        sCharB4PathDirection = path_direction::right_3;
                    }
                    if (!CanStepForwardWithRegionCheck(path_direction::up_1) && !gMap_0x370->TakeBlockedByTerrain())
                    {
                        bCanStep = false;
                        sCharB4PathDirection = path_direction::up_1;
                    }
                }
                mSlopeGradientDirection = old_slope_dir;
                return bCanStep;
            }
            else
            {
                bCanStep = true;
                mSpritePtr->mXy.x.add_one();
                if (mFlagsBf.bOnSlope)
                {
                    mSpritePtr->mZpos =
                        gMap_0x370->FindGroundZForCoord(mSpritePtr->mXy.x, mSpritePtr->mXy.y);
                }
                if (mSpritePtr->mZpos == kFP16Zero_6FD9E4)
                {
                    mSpritePtr->mZpos = sprite_zpos;
                }
                if (!CanStepForwardWithRegionCheck(path_direction::down_2) && !gMap_0x370->TakeBlockedByTerrain())
                {
                    sCharB4PathDirection = path_direction::down_2;
                    bCanStep = false;
                }
                mSpritePtr->mXy.x = sprite_xpos;
                mSpritePtr->mZpos = sprite_zpos;
                mSpritePtr->mXy.y.add_one();
                mSpritePtr->mZpos =
                    gMap_0x370->FindGroundZForCoord(mSpritePtr->mXy.x, mSpritePtr->mXy.y);
                if (mSpritePtr->mZpos == kFP16Zero_6FD9E4)
                {
                    mSpritePtr->mZpos = sprite_zpos;
                }
                if (!CanStepForwardWithRegionCheck(path_direction::right_3) && !gMap_0x370->TakeBlockedByTerrain())
                {
                    sCharB4PathDirection = path_direction::right_3;
                    bCanStep = false;
                }
                mSpritePtr->mXy.x = sprite_xpos;
                mSpritePtr->mXy.y = sprite_ypos;
                mSpritePtr->mZpos = sprite_zpos;

                if (!bCanStep)
                {
                    gCharB4_StepXpos = gCharB4_Saved_Xpos;
                    sCharB4StepYpos = gCharB4_Saved_Ypos;
                    if (!CanStepForwardWithRegionCheck(path_direction::right_3) && !gMap_0x370->TakeBlockedByTerrain())
                    {
                        sCharB4PathDirection = path_direction::right_3;
                    }
                    else
                    {
                        sCharB4PathDirection = path_direction::down_2;
                        bCanStep = true;
                        if (!CanStepForwardWithRegionCheck(path_direction::down_2) && !gMap_0x370->TakeBlockedByTerrain())
                        {
                            bCanStep = false;
                        }
                        else
                        {
                            sCharB4PathDirection = path_direction::right_3;
                        }
                    }
                }
                else
                {
                    if (!CanStepForwardWithRegionCheck(path_direction::right_3) && !gMap_0x370->TakeBlockedByTerrain())
                    {
                        bCanStep = false;
                        sCharB4PathDirection = path_direction::right_3;
                    }
                    if (!CanStepForwardWithRegionCheck(path_direction::down_2) && !gMap_0x370->TakeBlockedByTerrain())
                    {
                        bCanStep = false;
                        sCharB4PathDirection = path_direction::down_2;
                    }
                }
                mSlopeGradientDirection = old_slope_dir;
                return bCanStep;
            }
        }
        return bCanStep;
    }

    // Straight step along one axis
    if (x_diff != 0)
    {
        if (x_diff == -1)
        {
            return CanStepForwardWithRegionCheck(path_direction::left_4);
        }
        return CanStepForwardWithRegionCheck(path_direction::right_3);
    }
    if (y_diff == -1)
    {
        return CanStepForwardWithRegionCheck(path_direction::up_1);
    }
    return CanStepForwardWithRegionCheck(path_direction::down_2);
}

// https://decomp.me/scratch/xc0PO
MATCH_FUNC(0x54fec0)
bool Char::CanStepForward(s32 direction)
{
    bool result;

    Fix16 z_frac;
    u8 block_type;

    Fix16 xpos = mSpritePtr->mXy.x;
    Fix16 ypos = mSpritePtr->mXy.y;
    Fix16 zpos = mSpritePtr->mZpos;
    s8 slope_z_adjust = 0;
    s32 zpos_int = zpos.ToInt();

    s32 check_z = zpos_int - 1;

    if ((mFlags & 1) == 1)
    {
        check_z = zpos_int;
    }

    if (gMap_0x370->IsMoveOntoSlopeTileBlocked(xpos.ToInt(), ypos.ToInt(), zpos_int, direction, reinterpret_cast<u8*>(&slope_z_adjust), false))
    {
        sCharB4PathDirection = direction;
        return false;
    }

    check_z += slope_z_adjust;

    if (check_z < 0)
    {
        return false;
    }

    switch (direction)
    {
        case path_direction::up_1:
            block_type = gMap_0x370->GetBlockTypeAtCoord(xpos.ToInt(), ypos.ToInt() - 1, check_z);
            break;
        case path_direction::right_3:
            block_type = gMap_0x370->GetBlockTypeAtCoord(xpos.ToInt() + 1, ypos.ToInt(), check_z);
            break;
        case path_direction::down_2:
            block_type = gMap_0x370->GetBlockTypeAtCoord(xpos.ToInt(), ypos.ToInt() + 1, check_z);
            break;
        case path_direction::left_4:
            block_type = gMap_0x370->GetBlockTypeAtCoord(xpos.ToInt() - 1, ypos.ToInt(), check_z);
            break;
        default:
            block_type = AIR;
            break;
    }

    if (block_type != AIR)
    {
        return IsBlockTypeInRange_1_4(block_type);
    }

    if ((mFlags & 1) == 1)
    {
        z_frac = zpos.GetFracValue();
        if (z_frac < kFP16Half_6FD8E4)
        {
            mFlags &= ~1u;
            result = Char::CanStepForward(direction);
            mFlags |= 1u;
            return result;
        }
        if (mpPed->PedTypeIs(ped_type::player_2))
        {
            return true;
        }
        if (z_frac > kFP16Half_6FD8E4)
        {
            mFlags &= ~1u;
            mSpritePtr->mZpos += Fix16(1);
            result = Char::CanStepForward(direction);
            mSpritePtr->mZpos -= Fix16(1);
            mFlags |= 1u;
            return result;
        }
        return false;
    }
    return true;
}

MATCH_FUNC(0x550090)
bool Char::CanReachTile(u8 xpos, u8 ypos)
{
    bool bRes_1;
    bool bRes_2;

    Fix16 original_x = mSpritePtr->mXy.x;
    Fix16 original_y = mSpritePtr->mXy.y;

    char_type diff_x = xpos - original_x.ToInt();
    char_type diff_y = ypos - original_y.ToInt();

    if (diff_x == 0)
    {
        if (diff_y == 0)
        {
            return true;
        }
    }

    if (diff_x)
    {
        if (diff_y)
        {
            if (diff_x == -1)
            {
                bRes_1 = true;
                if (diff_y == -1)
                {
                    mSpritePtr->mXy.x.subtract_one();

                    if (!Char::CanReachTile(xpos, ypos))
                    {
                        bRes_1 = false;
                    }
                    mSpritePtr->mXy.x = original_x;
                    mSpritePtr->mXy.y.subtract_one();
                    if (!Char::CanReachTile(xpos, ypos))
                    {
                        bRes_1 = false;
                    }
                    mSpritePtr->mXy.x = original_x;
                    mSpritePtr->mXy.y = original_y;
                    if (!Char::CanReachTile((gCharB4_Saved_Xpos - kFP16One_6FD9E8).ToInt(), gCharB4_Saved_Ypos.ToInt()))
                    {
                        bRes_1 = false;
                    }
                    if (!Char::CanReachTile(gCharB4_Saved_Xpos.ToInt(), (gCharB4_Saved_Ypos - kFP16One_6FD9E8).ToInt()))
                    {
                        bRes_1 = false;
                    }
                }
                else
                {
                    mSpritePtr->mXy.x.subtract_one();

                    if (!Char::CanReachTile(xpos, ypos))
                    {
                        bRes_1 = false;
                    }

                    mSpritePtr->mXy.x = original_x;
                    mSpritePtr->mXy.y.add_one();
                    if (!Char::CanReachTile(xpos, ypos))
                    {
                        bRes_1 = false;
                    }
                    mSpritePtr->mXy.x = original_x;
                    mSpritePtr->mXy.y = original_y;
                    if (!Char::CanReachTile((mSpritePtr->mXy.x - kFP16One_6FD9E8).ToInt(),
                                                      mSpritePtr->mXy.y.ToInt()))
                    {
                        bRes_1 = false;
                    }

                    if (!Char::CanReachTile(mSpritePtr->mXy.x.ToInt(),
                                                      (kFP16One_6FD9E8 + mSpritePtr->mXy.y).ToInt()))
                    {
                        bRes_1 = false;
                    }
                }
            }
            else
            {
                bRes_2 = true;
                if (diff_y == -1)
                {
                    mSpritePtr->mXy.x.add_one();
                    if (!Char::CanReachTile(xpos, ypos))
                    {
                        bRes_2 = false;
                        sCharB4PathDirection = path_direction::right_3;
                    }
                    mSpritePtr->mXy.x = original_x;
                    mSpritePtr->mXy.y.subtract_one();
                    if (!Char::CanReachTile(xpos, ypos))
                    {
                        bRes_2 = false;
                        sCharB4PathDirection = path_direction::up_1;
                    }
                    mSpritePtr->mXy.x = original_x;
                    mSpritePtr->mXy.y = original_y;
                    if (!Char::CanReachTile((kFP16One_6FD9E8 + mSpritePtr->mXy.x).ToInt(),
                                                      mSpritePtr->mXy.y.ToInt()))
                    {
                        bRes_2 = false;
                        sCharB4PathDirection = path_direction::up_1;
                    }
                    if (!Char::CanReachTile(mSpritePtr->mXy.x.ToInt(),
                                                      (mSpritePtr->mXy.y - kFP16One_6FD9E8).ToInt()))
                    {
                        bRes_2 = false;
                        sCharB4PathDirection = path_direction::right_3;
                    }
                    return bRes_2;
                }
                else
                {
                    mSpritePtr->mXy.x.add_one();
                    if (!Char::CanReachTile(xpos, ypos))
                    {
                        bRes_1 = false;
                    }

                    mSpritePtr->mXy.x = original_x;
                    mSpritePtr->mXy.y.add_one();
                    if (!Char::CanReachTile(xpos, ypos))
                    {
                        bRes_1 = false;
                    }
                    mSpritePtr->mXy.x = original_x;
                    mSpritePtr->mXy.y = original_y;
                    if (!Char::CanReachTile((kFP16One_6FD9E8 + mSpritePtr->mXy.x).ToInt(),
                                                      mSpritePtr->mXy.y.ToInt()))
                    {
                        bRes_1 = false;
                    }
                    if (!Char::CanReachTile(mSpritePtr->mXy.x.ToInt(),
                                                      (kFP16One_6FD9E8 + mSpritePtr->mXy.y).ToInt()))
                    {
                        bRes_1 = false;
                    }
                }
            }
            return bRes_1;
        }
        else // diff_y = 0
        {
            if (diff_x == -1)
            {
                if (Char::CanStepForward(path_direction::left_4))
                {
                    return true;
                }
                else
                {
                    sCharB4PathDirection = path_direction::left_4;
                    return false;
                }
            }
            else if (Char::CanStepForward(path_direction::right_3))
            {
                return true;
            }
            else
            {
                sCharB4PathDirection = path_direction::right_3;
                return false;
            }
        }
    }
    else // diff_x = 0
    {
        if (diff_y == 0)
        {
            return true;
        }
        if (diff_y == -1)
        {
            if (Char::CanStepForward(path_direction::up_1))
            {
                return true;
            }
            else
            {
                sCharB4PathDirection = path_direction::up_1;
                return false;
            }
        }
        else if (Char::CanStepForward(path_direction::down_2))
        {
            return true;
        }
        else
        {
            sCharB4PathDirection = path_direction::down_2;
            return false;
        }
    }
    return false;
}

static inline s32 IsOnSlopeBlock(gmp_block_info* pBlock)
{
    if (GET_SLOPE_BITS(pBlock->mSlopeType) != 0 && GET_SLOPE_BITS(pBlock->mSlopeType) != 0xFC)
    {
        return 1;
    }
    return 0;
}

// MaxAbsDistance as UpdateFleeOrRunning inlines it: past the nested inline budget, Max is the named
// out-of-line copy Max_44E540 (as in Weapon's MaxAbsDistance_5DF270)
static inline Fix16 MaxAbsDistance_5504F0(Fix16& x1, Fix16& y1, Fix16& x2, Fix16& y2)
{
    Fix16 diff_x;
    diff_x = x2 - x1;
    Fix16 diff_y;
    diff_y = y2 - y1;
    Fix16 result;
    result = Fix16::Max_44E540(Fix16::Abs(diff_x), Fix16::Abs(diff_y));
    return result;
}

// https://decomp.me/scratch/chZqY
MATCH_FUNC(0x5504f0)
void Char::UpdateFleeOrRunning()
{
    Ang16 saved_rotation;
    Ang16 prev_angle;
    Ang16 cur_rotation;
    Ang16 next_rotation;
    char_type bSnapRotation;
    char_type bMoved;
    Fix16 probe_x;
    Fix16 probe_y;

    Ang16 desired_angle;
    Ang16 target_rotation;
    u8 x_u8;
    u8 y_u8;

    mTimeout = 500;
    Ang16 angle = 0;
    mFlagsBf.bReverseVelocity = false;
    mFlagsBf.bJumpedOffEdge = false;
    bMoved = 0;
    saved_rotation = 0;
    bSnapRotation = 0;
    sCharB4GradientSlopeBelow = 0;
    sCharB4SavedTileX = gCharB4_Saved_Xpos.ToUInt8();
    sCharB4SavedTileY = gCharB4_Saved_Ypos.ToUInt8();
    u8 start_tile_x = gCharB4_Saved_Xpos.ToUInt8();
    u8 start_tile_y = gCharB4_Saved_Ypos.ToUInt8();
    u8 block_type;
    gmp_block_info* pBlock;
    if (!mFlagsBf.bOnSlope && gCharB4_Saved_Zpos.GetFracValue() == kFP16Zero_6FD9E4 &&
        gCharB4_Saved_Zpos > kFP16Zero_6FD9E4)
    {
        block_type = gMap_0x370->GetBlockTypeAtCoord(gCharB4_Saved_Xpos.ToInt(),
                                                                   gCharB4_Saved_Ypos.ToInt(),
                                                                   gCharB4_Saved_Zpos.ToInt() - 1);
        pBlock = gMap_0x370->GetBlock(gCharB4_Saved_Xpos.ToInt(),
                                                     gCharB4_Saved_Ypos.ToInt(),
                                                     gCharB4_Saved_Zpos.ToInt() - 1);
    }
    else
    {
        block_type = gMap_0x370->GetBlockTypeAtCoord(gCharB4_Saved_Xpos.ToInt(),
                                                                   gCharB4_Saved_Ypos.ToInt(),
                                                                   gCharB4_Saved_Zpos.ToInt());
        pBlock = gMap_0x370->GetBlock(gCharB4_Saved_Xpos.ToInt(),
                                                     gCharB4_Saved_Ypos.ToInt(),
                                                     gCharB4_Saved_Zpos.ToInt());
    }

    if (pBlock)
    {
        mFlagsBf.bOnSlope = IsOnSlopeBlock(pBlock);
        if (gGtx_0x106C->IsElectrifiedFloorType(pBlock->mLid & block_face::tile_idx_mask) && mCharState != Char_B4_state::Jumping_15)
        {
            if (!mpPed->mBf21C.bLeftVehicle)
            {
                mpPed->mShockCounter += 3;
                if (mpPed->mKillerId)
                {
                    if (gPedManager->PedById(mpPed->mKillerId))
                    {
                        mpPed->mDeathCause = ped_death_cause::electrocuted_2;
                        mpPed->mKillerIdTimer = 50;
                    }
                }
            }
        }
    }

    if (static_cast<u8>(Char::IsOnWater()))
    {
        mpPed->PutOutFire();
        Char::DrownPed();
        return;
    }

    if (!block_type && !mFlagsBf.bOnSlope)
    {
        if (mCharState != Char_B4_state::Jumping_15)
        {
            // The z argument is the raw subtraction: Fix16::operator- here gives other registers
            if (gMap_0x370->HasBlockAnyArrows(gCharB4_Saved_Xpos.ToInt(),
                                                            gCharB4_Saved_Ypos.ToInt(),
                                                            (gCharB4_Saved_Zpos.mValue - kFP16One_6FD9E8.mValue) >> 14))
            {
                Char::DoJump();
                mVelocity = kZeroVelocity;
                mFlagsBf.bJumpedOffEdge = true;
                mRotation.SnapToAng4();
            }
            else
            {
                mpPed->ChangeNextPedState1(ped_state_1::immobilized_8);
                mpPed->ChangeNextPedState2(ped_state_2::falling_19);
                mStateInitPending = true;
                return;
            }
        }
        else
        {
            if (gMap_0x370->HasBlockAnyArrows(gCharB4_Saved_Xpos.ToInt(),
                                                            gCharB4_Saved_Ypos.ToInt(),
                                                            (gCharB4_Saved_Zpos.mValue - kFP16One_6FD9E8.mValue) >> 14))
            {
                Char::DoJump();
                mFlagsBf.bJumpedOffEdge = true;
                mRotation.SnapToAng4();
            }
        }
    }

    mFlagsBf.bReverseVelocity = false;
    if (mVelocity < kFP16Zero_6FD9E4)
    {
        mFlagsBf.bReverseVelocity = true;
        mRotation = Ang16(mRotation.rValue + kAng180_6FD936.rValue, static_cast<u8>(0));
        mVelocity = -mVelocity;
    }
    mBlockType = block_type;

    if (mFlagsBf.bJumpedOffEdge)
    {
        desired_angle = mRotation;
    }
    else
    {
        desired_angle = mpPed->GetTargetFacingAngle();
    }
    target_rotation = desired_angle;
    if (mCharState != Char_B4_state::Avoiding_Ped_10)
    {
        if (mCharState == Char_B4_state::Jumping_15)
        {
            if (mAnimationState != Char_Anim_state::Jumping_5)
            {
                mAnimationState = Char_Anim_state::Jumping_5;
                mAnimationFrame = 0;
                mFrameDelay = 4;
            }
            else if (!mFlagsBf.bJumpedOffEdge)
            {
                if (!mpPed->mBf224.bWaitingOnTarget)
                {
                    desired_angle = mpPed->GetTargetFacingAngle();
                }
                else
                {
                    desired_angle = mRotation;
                }
                target_rotation = desired_angle;
            }

            if (!mFlagsBf.bJumpedOffEdge)
            {
                if (mpPed->mBf224.bWaitingOnTarget)
                {
                    if (sCharB4DistanceToTarget < kFpOneSixteenth)
                    {
                        mVelocity = kFpOne64th;
                    }
                    else
                    {
                        if (sCharB4DistanceToTarget < kFpOneEighth)
                        {
                            mVelocity = k_CollisionRepulsionSpeed;
                        }
                        else
                        {
                            mVelocity = sRunOrJumpSpeed;
                        }
                    }
                }
                else
                {
                    mVelocity = sRunOrJumpSpeed;
                }
            }
        }
    }
    else
    {
        if (mVelocity > k_CollisionRepulsionSpeed)
        {
            mVelocity = kFpThreeSixtyFourths;
        }
        bSnapRotation = 1;
    }
    if (mNextTileTimer > 0)
    {
        mNextTileTimer--;
    }

    if (mVelocity != kZeroVelocity)
    {
        if (mIsCollidingWithSprite)
        {
            bSnapRotation = 1;
            mVelocity = sRunOrJumpSpeed;
        }

        if (mFlagsBf.bJumpedOffEdge || mpPed->IsPedGoingToEnterCar())
        {
            bSnapRotation = 1;
        }
        angle = desired_angle;
        if (mFlagsBf.bDetouring)
        {
            if (MaxAbsDistance_5504F0(gCharB4_Saved_Xpos,
                                      gCharB4_Saved_Ypos,
                                      kFP16Half_6FD8E4 + Fix16(mNextTileX),
                                      kFP16Half_6FD8E4 + Fix16(mNextTileY)) < kFP16Quarter_6FD828)
            {
                mFlagsBf.bDetouring = false;
            }
            else
            {
                target_rotation = Fix16::atan2_fixed(kFP16Half_6FD8E4 + Fix16(mNextTileY) - gCharB4_Saved_Ypos,
                                                kFP16Half_6FD8E4 + Fix16(mNextTileX) - gCharB4_Saved_Xpos);
                angle = target_rotation;
            }
        }
        else
        {
            if (mRepulsionTimer && mPedState2 != ped_state_2::running_away_3)
            {
                prev_angle = mRepulsionRotation;
                cur_rotation = mRotation;
                mVelocity = k_CollisionRepulsionSpeed;
                target_rotation = prev_angle;
                saved_rotation = cur_rotation;
                bSnapRotation = 1;
            }
        }
        if (mPedState2 == ped_state_2::running_away_3)
        {
            if (mNextTileTimer > 0)
            {
                if (!mFlagsBf.bDetouring)
                {
                    target_rotation = mSpritePtr->mRotation;
                    angle = target_rotation;
                }
            }
            Ang16::PolarToCartesian_41FC20(target_rotation, kFP16Half_6FD8E4, probe_x, probe_y);
        }
        else
        {
            Ang16::PolarToCartesian_41FC20(target_rotation, kFP16Quarter_6FD828, probe_x, probe_y);
        }
        probe_x += gCharB4_Saved_Xpos;
        probe_y += gCharB4_Saved_Ypos;
        x_u8 = probe_x.ToUInt8();
        y_u8 = probe_y.ToUInt8();
        if (start_tile_x != x_u8 || start_tile_y != y_u8)
        {
            Char::ManageZCoordAndSlopes();

            if (!Char::CanReachTile(x_u8, y_u8))
            {
                mFlagsBf.bDetouring = true;
                if (mPedState2 == ped_state_2::running_away_3)
                {
                    Char::ChooseNextMovementTile();
                }
                else
                {
                    Char::SelectNextTileFast();
                }
                target_rotation = sAng16AngleOfCollision;
                angle = sAng16AngleOfCollision;
            }
        }
    }

    if (mCharState == 28 || mCharState == 29 || bSnapRotation)
    {
        mRotation = target_rotation;
    }
    else
    {
        next_rotation = Char::GetNextRotationToward(target_rotation);
        if (Char::CanStepInDirection(next_rotation) == 1)
        {
            mRotation = next_rotation;
            bMoved = 1;
        }
        else if (angle != target_rotation)
        {
            mRotation = angle;
        }
        else
        {
            mRotation = target_rotation;
        }
    }

    if (mFlagsBf.bJumpedOffEdge)
    {
        mVelocity = sRunOrJumpSpeed;
    }

    Fix16 nx = mSpritePtr->mXy.x + Ang16::sine(mRotation) * mVelocity;
    Fix16 ny = mSpritePtr->mXy.y + Ang16::cosine(mRotation) * mVelocity;
    mSpritePtr->set_xyz_lazy_420600(nx, ny, mSpritePtr->mZpos);

    mSpritePtr->set_ang_lazy(mRotation);
    if (mIsCollidingWithSprite)
    {
        bMoved = 1;
    }

    if (mpPed->IsPedGoingToEnterCar())
    {
        bMoved = 1;
    }
    if (bMoved == 1 || mFlagsBf.bOnSlope == true)
    {
        sCharB4GradientSlopeBelow = gMap_0x370->IsGradientSlopeAt(mSpritePtr->mXy.x.ToInt(),
                                                                  mSpritePtr->mXy.y.ToInt(),
                                                                  mSpritePtr->mZpos.ToInt() - 1);

        Char::ManageZCoordAndSlopes();
    }
    Char::DispatchCollision(sCharB4MoveIgnorePeds);

    if (mIsCollidingWithSprite)
    {
        if (Char::ContinueMovementAfterCollision() == 1)
        {
            Fix16 x = mSpritePtr->mXy.x;
            Fix16 y = mSpritePtr->mXy.y;

            x_u8 = x.ToUInt8();
            y_u8 = y.ToUInt8();
            if (gCharB4_Saved_Xpos.ToUInt8() != x_u8 || gCharB4_Saved_Ypos.ToUInt8() != y_u8)
            {
                mSpritePtr->set_xyz_lazy_420600(gCharB4_Saved_Xpos, gCharB4_Saved_Ypos, gCharB4_Saved_Zpos);

                mSlopeGradientDirection = sCharB4SavedSlopeGradDir.ToUInt8();
                if (!Char::CanReachTile(x_u8, y_u8))
                {
                    mIsCollidingWithSprite = false;
                    mJumpOverTimer = 10;
                    if (sCharCollisionNoJump)
                    {
                        s16 rnd = gRng.get_int(4);
                        switch (rnd)
                        {
                            case 0:
                                mRotation = kAng0_6FDB34;
                                break;
                            case 1:
                                mRotation = kAng180_6FD936;
                                break;
                            case 2:
                                mRotation = kAng270_6FD95C;
                                break;
                            default:
                                mRotation = kAng90_6FD854;
                                break;
                        }
                    }
                }
                else
                {
                    mSpritePtr->set_xyz_lazy_420600(x, y, gCharB4_Saved_Zpos);

                    if (bMoved == 1 || mFlagsBf.bOnSlope == true)
                    {

                        sCharB4GradientSlopeBelow =
                            gMap_0x370->IsGradientSlopeAt(mSpritePtr->mXy.x.ToInt(),
                                                                        mSpritePtr->mXy.y.ToInt(),
                                                                        (mSpritePtr->mZpos - kFP16One_6FD9E8).ToInt());
                        Char::ManageZCoordAndSlopes();
                    }
                }
            }
        }
        else
        {
            mIsCollidingWithSprite = false;
            mSlideTurn = kAng0_6FDB34;
            mJumpOverTimer = 10;
        }
    }

    if (mRepulsionTimer)
    {
        mRotation = saved_rotation;
    }
    if (mFlagsBf.bReverseVelocity)
    {
        mRotation = Ang16(mRotation.rValue + kAng180_6FD936.rValue, static_cast<u8>(0));
        mVelocity = -mVelocity;
    }
    Char::CheckCollisionsAndUpdateAnim();
}

// https://decomp.me/scratch/4eLOQ
MATCH_FUNC(0x550f60)
Ang16 Char::GetNextRotationToward(Ang16 inputAng)
{
    u8 side_curr = mRotation.ToAng4();
    u8 side_input_ang = inputAng.ToAng4();

    // 9.6f has no trace of this; 10.5 constructs the step angle from it and then overwrites it.
    // The product is a temporary inside the inline: the step angle sits in the temporaries area.
    Ang16 max_turn = kAng4.MultiplyByFix16(mVelocity);
    Ang16 result;

    if (mCharState == Char_B4_state::Avoiding_Ped_10)
    {
        return inputAng;
    }

    if (mVelocity > kZeroVelocity)
    {
        if (mVelocity > k_CollisionRepulsionSpeed)
        {
            max_turn = kAng12_6FD892;
        }
        else
        {
            max_turn = kAng24;
        }
    }
    else
    {
        max_turn = kAng45_6FD89C;
    }

    switch (side_input_ang)
    {
        case 3: // west
            if (side_curr <= 1)
            {
                if (ComputeShortestAngleDelta(inputAng, mRotation) > kAng180_6FD936)
                {
                    result = mRotation + max_turn;
                }
                else
                {
                    result = mRotation - max_turn;
                }
                return result;
            }
            break;
        case 2: // north
            if (side_curr == 0)
            {
                if (ComputeShortestAngleDelta(inputAng, mRotation) > kAng180_6FD936)
                {
                    result = mRotation + max_turn;
                }
                else
                {
                    result = mRotation - max_turn;
                }
                return result;
            }
            break;
        case 1: // east
            if (side_curr == 3)
            {
                if (ComputeShortestAngleDelta(mRotation, inputAng) < kAng180_6FD936)
                {
                    result = mRotation - max_turn;
                }
                else
                {
                    result = mRotation + max_turn;
                }
                return result;
            }
            break;
        case 0: // south
            switch (side_curr)
            {
                case 2:
                    if (ComputeShortestAngleDelta(mRotation, inputAng) < kAng180_6FD936)
                    {
                        result = mRotation - max_turn;
                    }
                    else
                    {
                        result = mRotation + max_turn;
                    }
                    return result;
                    break;
                case 3:
                    if (ComputeShortestAngleDelta(inputAng, mRotation) < max_turn)
                    {
                        return inputAng;
                    }
                    else
                    {
                        return mRotation + max_turn;
                    }
                    break;
            }
            break;
    }

    // Close enough to the target: snap to it (the original returns inputAng here, not mRotation)
    if (inputAng > mRotation)
    {
        if (ComputeShortestAngleDelta(inputAng, mRotation) > max_turn)
        {
            result = mRotation + max_turn;
        }
        else
        {
            result = inputAng;
        }
        return result;
    }
    else
    {
        if (ComputeShortestAngleDelta(mRotation, inputAng) > max_turn)
        {
            result = mRotation - max_turn;
        }
        else
        {
            result = inputAng;
        }
    }
    return result;
}

MATCH_FUNC(0x551350)
bool Char::CanStepInDirection(Ang16 ang)
{
    Fix16 old_x = mSpritePtr->mXy.x;
    Fix16 old_y = mSpritePtr->mXy.y;
    Fix16 x_pos;
    Fix16 y_pos;

    if (mPedState2 == ped_state_2::running_away_3)
    {
        Ang16::PolarToCartesian_41FC20(ang, kFpHalf_6FDB18, x_pos, y_pos);
    }
    else
    {
        Ang16::PolarToCartesian_41FC20(ang, kFpThreeSixteenths, x_pos, y_pos);
    }
    x_pos += old_x;
    y_pos += old_y;
    return Char::CanReachTile(x_pos.ToInt(), y_pos.ToInt());
}

MATCH_FUNC(0x551400)
void Char::ChooseNextMovementTile()
{
    s32 saved_direction = sCharB4PathDirection;
    switch (sCharB4PathDirection)
    {
        case path_direction::up_1:
        case path_direction::down_2:
            if (mpPed->GetMoveTargetX() <= gCharB4_Saved_Xpos)
            {
                sAng16AngleOfCollision = kAng90_6FDA64;
                if (Char::CanReachTile(sCharB4SavedTileX + 1, sCharB4SavedTileY))
                {
                    mNextTileX = sCharB4SavedTileX + 1;
                    mNextTileY = sCharB4SavedTileY;
                    mUnused60 = 3;
                    mNextTileTimer = 40;
                    return;
                }
                else
                {
                    sAng16AngleOfCollision = kAng270;
                    if (Char::CanReachTile(sCharB4SavedTileX - 1, sCharB4SavedTileY))
                    {
                        mNextTileX = sCharB4SavedTileX - 1;
                        mNextTileY = sCharB4SavedTileY;
                        mUnused60 = 4;
                        mNextTileTimer = 40;
                        return;
                    }
                }
            }
            else
            {
                sAng16AngleOfCollision = kAng270;
                if (Char::CanReachTile(sCharB4SavedTileX - 1, sCharB4SavedTileY))
                {
                    mNextTileX = sCharB4SavedTileX - 1;
                    mNextTileY = sCharB4SavedTileY;
                    mUnused60 = 4;
                    mNextTileTimer = 40;
                    return;
                }
                else
                {
                    sAng16AngleOfCollision = kAng90_6FDA64;
                    if (Char::CanReachTile(sCharB4SavedTileX + 1, sCharB4SavedTileY))
                    {
                        mNextTileX = sCharB4SavedTileX + 1;
                        mNextTileY = sCharB4SavedTileY;
                        mUnused60 = 3;
                        mNextTileTimer = 40;
                        return;
                    }
                }
            }

            if (saved_direction == path_direction::up_1)
            {
                mNextTileX = sCharB4SavedTileX;
                mNextTileY = sCharB4SavedTileY + 1;
            }
            else
            {
                mNextTileX = sCharB4SavedTileX;
                mNextTileY = sCharB4SavedTileY - 1;
            }
            break;

        case path_direction::right_3:
        case path_direction::left_4:

            if (mpPed->GetMoveTargetY() <= gCharB4_Saved_Ypos)
            {
                sAng16AngleOfCollision = kAng0;
                if (Char::CanReachTile(sCharB4SavedTileX, sCharB4SavedTileY + 1))
                {
                    mNextTileX = sCharB4SavedTileX;
                    mNextTileY = sCharB4SavedTileY + 1;
                    mNextTileTimer = 40;
                    return;
                }
                else
                {
                    sAng16AngleOfCollision = kAng180_6FD8E8;
                    if (Char::CanReachTile(sCharB4SavedTileX, sCharB4SavedTileY - 1))
                    {
                        mNextTileX = sCharB4SavedTileX;
                        mNextTileY = sCharB4SavedTileY - 1;
                        mNextTileTimer = 40;
                        return;
                    }
                }
            }
            else
            {
                sAng16AngleOfCollision = kAng180_6FD8E8;
                if (Char::CanReachTile(sCharB4SavedTileX, sCharB4SavedTileY - 1))
                {
                    mNextTileX = sCharB4SavedTileX;
                    mNextTileY = sCharB4SavedTileY - 1;
                    mNextTileTimer = 40;
                    return;
                }
                else
                {
                    sAng16AngleOfCollision = kAng0;
                    if (Char::CanReachTile(sCharB4SavedTileX, sCharB4SavedTileY + 1))
                    {
                        mNextTileX = sCharB4SavedTileX;
                        mNextTileY = sCharB4SavedTileY + 1;
                        mNextTileTimer = 40;
                        return;
                    }
                }
            }

            if (saved_direction == path_direction::right_3)
            {
                mNextTileX = sCharB4SavedTileX - 1;
                mNextTileY = sCharB4SavedTileY;
            }
            else
            {
                mNextTileX = sCharB4SavedTileX + 1;
                mNextTileY = sCharB4SavedTileY;
            }
            break;
        default:
            break;
    }
    mNextTileTimer = 40;
}

MATCH_FUNC(0x5516f0)
void Char::SelectNextTileFast()
{
    s32 saved_direction = sCharB4PathDirection;
    switch (sCharB4PathDirection)
    {
        case path_direction::up_1:
        case path_direction::down_2:
            if (mpPed->GetMoveTargetX() >= gCharB4_Saved_Xpos)
            {
                sAng16AngleOfCollision = kAng90_6FDA64;
                if (Char::CanReachTile(sCharB4SavedTileX + 1, sCharB4SavedTileY))
                {
                    mNextTileX = sCharB4SavedTileX + 1;
                    mNextTileY = sCharB4SavedTileY;
                    return;
                }
                else
                {
                    sAng16AngleOfCollision = kAng270;
                    if (Char::CanReachTile(sCharB4SavedTileX - 1, sCharB4SavedTileY))
                    {
                        mNextTileX = sCharB4SavedTileX - 1;
                        mNextTileY = sCharB4SavedTileY;
                        return;
                    }
                }
            }
            else
            {
                sAng16AngleOfCollision = kAng270;
                if (Char::CanReachTile(sCharB4SavedTileX - 1, sCharB4SavedTileY))
                {
                    mNextTileX = sCharB4SavedTileX - 1;
                    mNextTileY = sCharB4SavedTileY;
                    return;
                }
                else
                {
                    sAng16AngleOfCollision = kAng90_6FDA64;
                    if (Char::CanReachTile(sCharB4SavedTileX + 1, sCharB4SavedTileY))
                    {
                        mNextTileX = sCharB4SavedTileX + 1;
                        mNextTileY = sCharB4SavedTileY;
                        return;
                    }
                }
            }

            if (saved_direction == path_direction::up_1)
            {
                mNextTileX = sCharB4SavedTileX;
                mNextTileY = sCharB4SavedTileY + 1;
            }
            else
            {
                mNextTileX = sCharB4SavedTileX;
                mNextTileY = sCharB4SavedTileY - 1;
            }
            break;

        case path_direction::right_3:
        case path_direction::left_4:

            if (mpPed->GetMoveTargetY() >= gCharB4_Saved_Ypos)
            {
                sAng16AngleOfCollision = kAng0;
                if (Char::CanReachTile(sCharB4SavedTileX, sCharB4SavedTileY + 1))
                {
                    mNextTileX = sCharB4SavedTileX;
                    mNextTileY = sCharB4SavedTileY + 1;
                    return;
                }
                else
                {
                    sAng16AngleOfCollision = kAng180_6FD8E8;
                    if (Char::CanReachTile(sCharB4SavedTileX, sCharB4SavedTileY - 1))
                    {
                        mNextTileX = sCharB4SavedTileX;
                        mNextTileY = sCharB4SavedTileY - 1;
                        return;
                    }
                }
            }
            else
            {
                sAng16AngleOfCollision = kAng180_6FD8E8;
                if (Char::CanReachTile(sCharB4SavedTileX, sCharB4SavedTileY - 1))
                {
                    mNextTileX = sCharB4SavedTileX;
                    mNextTileY = sCharB4SavedTileY - 1;
                    return;
                }
                else
                {
                    sAng16AngleOfCollision = kAng0;
                    if (Char::CanReachTile(sCharB4SavedTileX, sCharB4SavedTileY + 1))
                    {
                        mNextTileX = sCharB4SavedTileX;
                        mNextTileY = sCharB4SavedTileY + 1;
                        return;
                    }
                }
            }

            if (saved_direction == path_direction::right_3)
            {
                mNextTileX = sCharB4SavedTileX - 1;
                mNextTileY = sCharB4SavedTileY;
            }
            else
            {
                mNextTileX = sCharB4SavedTileX + 1;
                mNextTileY = sCharB4SavedTileY;
            }
            break;
        default:
            break;
    }
}

MATCH_FUNC(0x5519F0)
void Char::UpdateFindingPath()
{
    Char::UpdateFleeOrRunning();
}

MATCH_FUNC(0x551A00)
void Char::UpdateEnteringCar()
{
    if (mPedState2 == ped_state_2::ped2_following_a_car_4)
    {
        mFlagsBf.bDetouring = false;
        Char::UpdateFleeOrRunning();
        if (!mpPed->GetAttacking() && mCharState != Char_B4_state::Jumping_15)
        {
            if (mVelocity > k_CollisionRepulsionSpeed)
            {
                mAnimationState = Char_Anim_state::Running_1;
            }
            else
            {
                if (mVelocity != kZeroVelocity)
                {
                    mAnimationState = Char_Anim_state::Walking_0;
                }
                else
                {
                    mAnimationState = Char_Anim_state::Standing_2;
                    if (mTargetCar)
                    {
                        mRotation = mTargetCar->mCarSprite->mRotation;
                    }
                }
            }
        }
    }
    else if (mPedState2 == ped_state_2::ped2_entering_a_car_6)
    {
        mCharState = Char_B4_state::Interacting_Car_Door_36;
        Sprite* pNearestSprite = gSpriteGrid_1->FindCollidingSpritePreferType(mSpritePtr, sprite_types_enum::none_0);
        if (!pNearestSprite || pNearestSprite->GetType() != sprite_types_enum::car_2 ||
            (pNearestSprite->mCarBcPtr == mpPed->get_target_to_enter()) ||
            pNearestSprite->mCarBcPtr->IsTrailerCar() || mpPed->CloseDoorIfCarApproaching(pNearestSprite->mCarBcPtr))
        {
            if (mAnimationState != Char_Anim_state::Entering_Car_6)
            {
                mRotation = mTargetCar->mCarSprite->mRotation;
                mAnimationState = Char_Anim_state::Entering_Car_6;
                mAnimationFrame = 0;
                if (mTargetCar->IsDoorOffsetLarge(mpPed->get_target_car_door()))
                {
                    mFlagsBf.bDoorOffsetLarge = true;
                }
                else
                {
                    mFlagsBf.bDoorOffsetLarge = false;
                }
                mFrameTimer = 0;
            }
        }
    }
}

MATCH_FUNC(0x551B30)
void Char::UpdateExitingCar()
{
    if (mAnimationState != Char_Anim_state::Exiting_Car_7)
    {
        mAnimationState = Char_Anim_state::Exiting_Car_7;
        mAnimationFrame = 0;
        s8 target_door = mpPed->get_target_car_door();
        if (mTargetCar->IsDoorOffsetLarge(target_door))
        {
            mFlagsBf.bDoorOffsetLarge = true;
        }
        else
        {
            mFlagsBf.bDoorOffsetLarge = false;
        }
        mFrameTimer = 0;
    }
    if (mCharState == Char_B4_state::Jumping_15)
    {
        mpPed->ChangeNextPedState1(ped_state_1::walking_0);
        mpPed->ChangeNextPedState2(ped_state_2::ped2_walking_0);
    }
    if (static_cast<u8>(Char::IsOnWater()))
    {
        mpPed->PutOutFire();
        Char::DrownPed();
    }
}

MATCH_FUNC(0x551BB0)
void Char::UpdateEnteringTrain()
{
    if (mPedState2 == ped_state_2::ped2_following_a_car_4)
    {
        this->mFlagsBf.bDetouring = false;

        UpdateFleeOrRunning();

        if (!this->mpPed->GetAttacking() && this->mCharState != Char_B4_state::Jumping_15)
        {
            if (mVelocity > k_CollisionRepulsionSpeed)
            {

                this->mAnimationState = Char_Anim_state::Running_1;
            }
            else if (mVelocity != kZeroVelocity)
            {
                this->mAnimationState = Char_Anim_state::Walking_0;
            }
            else
            {
                this->mAnimationState = Char_Anim_state::Standing_2;
                this->mRotation = mTargetCar->mCarSprite->mRotation;
            }
        }
    }
    else if (mPedState2 == ped_state_2::ped2_entering_a_car_6 && this->mAnimationState != Char_Anim_state::Entering_Car_6)
    {
        this->mRotation = mTargetCar->mCarSprite->mRotation;
        mTargetCar->GetDoorWorldPosition(mpPed->get_target_car_door(),
                                              &mSpritePtr->mXy.x,
                                              &mSpritePtr->mXy.y);
        this->mAnimationState = Char_Anim_state::Entering_Car_6;
        this->mAnimationFrame = 0;
        if (mTargetCar->IsDoorOffsetLarge(mpPed->get_target_car_door()))
        {
            this->mFrameTimer = 3;
        }
        else
        {
            this->mFrameTimer = 0;
        }
    }
}

// https://decomp.me/scratch/qNjdM
MATCH_FUNC(0x551CB0)
void Char::UpdateStandingStill()
{
    u8 block_type;
    gmp_block_info* pBlockAtPos;
    mVelocity = kZeroVelocity;
    if (mCharState != Char_B4_state::Jumping_15)
    {
        Char::CheckAndHandleCollisions();
    }
    if (mpPed->PedTypeIs(ped_type::player_2) == true)
    {
        mRotation += mpPed->GetTurnSpeed();

        if (mCharState == Char_B4_state::Jumping_15)
        {
            Char::UpdateWalking();
            SetPedState1_433910(0);
            SetPedState2(0);
            return;
        }
        if (mFlagsBf.bOnSlope || gCharB4_Saved_Zpos.GetFracValue() != kFP16Zero_6FD9E4)
        {
            block_type = gMap_0x370->GetBlockTypeAtCoord(gCharB4_Saved_Xpos.ToInt(), gCharB4_Saved_Ypos.ToInt(), gCharB4_Saved_Zpos.ToInt());
            pBlockAtPos = gMap_0x370->GetBlock(gCharB4_Saved_Xpos.ToInt(), gCharB4_Saved_Ypos.ToInt(), gCharB4_Saved_Zpos.ToInt());
        }
        else
        {
            block_type = gMap_0x370->GetBlockTypeAtCoord(gCharB4_Saved_Xpos.ToInt(), gCharB4_Saved_Ypos.ToInt(), gCharB4_Saved_Zpos.ToInt() - 1);
            pBlockAtPos = gMap_0x370->GetBlock(gCharB4_Saved_Xpos.ToInt(), gCharB4_Saved_Ypos.ToInt(), gCharB4_Saved_Zpos.ToInt() - 1);
        }
        if (pBlockAtPos)
        {
            if (gGtx_0x106C->IsElectrifiedFloorType(pBlockAtPos->mLid & block_face::tile_idx_mask))
            {
                if (!mpPed->mBf21C.bLeftVehicle)
                {
                    mpPed->mShockCounter += 3;
                    if (mpPed->mKillerId != 0)
                    {
                        if (gPedManager->PedById(mpPed->mKillerId))
                        {
                            mpPed->mDeathCause = ped_death_cause::electrocuted_2;
                            mpPed->mKillerIdTimer = 50;
                        }
                    }
                }
            }
        }
        if (mAnimationState != Char_Anim_state::Punching_4)
        {
            mAnimationState = Char_Anim_state::Standing_2;
        }
    }
    else
    {
        mRotation = mpPed->GetTargetFacingAngle();
    }
    if (mCharState == Char_B4_state::Jumping_15)
    {
        mVelocity = sRunOrJumpSpeed;
    }
    if (mRepulsionTimer > 0 || mCharState == Char_B4_state::Jumping_15 || mConveyorDx != kFP16Zero_6FD9E4 || mConveyorDy != kFP16Zero_6FD9E4)
    {
        Set_F8_ped_state_1_433910(ped_state_1::walking_0);
        mPedState2 = ped_state_2::ped2_walking_0;
        Char::UpdateWalking();
        Set_F8_ped_state_1_433910(ped_state_1::standing_still_7);
        mPedState2 = ped_state_2::ped2_staying_14;
        if (mCharState == Char_B4_state::Jumping_15)
        {
            return;
        }
    }
    else
    {
        // Raw compares: the Fix16 operator== inlines push this function over VC6's inline budget
        if (!mFlagsBf.bOnSlope &&
            (gCharB4_Saved_Zpos.mValue == kFP16One_6FD9E8.mValue || gCharB4_Saved_Zpos.mValue == gFix16_Two.mValue ||
             gCharB4_Saved_Zpos.mValue == kFP16Three_6FD9F0.mValue || gCharB4_Saved_Zpos.mValue == kFP16Four_6FD9F4.mValue ||
             gCharB4_Saved_Zpos.mValue == kFP16Five_6FD9F8.mValue || gCharB4_Saved_Zpos.mValue == kFP16Six.mValue ||
             gCharB4_Saved_Zpos.mValue == kFP16Seven.mValue))
        {
            block_type = gMap_0x370->GetBlockTypeAtCoord(gCharB4_Saved_Xpos.ToInt(), gCharB4_Saved_Ypos.ToInt(), gCharB4_Saved_Zpos.ToInt() - 1);
        }
        else
        {
            block_type = gMap_0x370->GetBlockTypeAtCoord(gCharB4_Saved_Xpos.ToInt(), gCharB4_Saved_Ypos.ToInt(), gCharB4_Saved_Zpos.ToInt());
        }

        if (static_cast<u8>(Char::IsOnWater()))
        {
            mpPed->PutOutFire();
            Char::DrownPed();
            return;
        }
        if (block_type == AIR && !mFlagsBf.bOnSlope && mCharState != Char_B4_state::Jumping_15)
        {
            mpPed->ChangeNextPedState1(ped_state_1::immobilized_8);
            mpPed->ChangeNextPedState2(ped_state_2::falling_19);
            mStateInitPending = true;
            return;
        }
    }

    switch (mPedState2)
    {
        case ped_state_2::ped2_staying_14:
            if (mpPed->GetAttacking() == 1)
            {
                if (mpPed->mBf21C.bNoWeapon)
                {
                    if (mAnimationState != Char_Anim_state::Punching_4)
                    {
                        mAnimationState = Char_Anim_state::Punching_4;
                        mAnimationFrame = 0;
                    }
                }
                else
                {
                    if (mAnimationState == Char_Anim_state::Punching_4)
                    {
                        if (mAnimationFrame == 0)
                        {
                            mAnimationState = Char_Anim_state::Standing_2;
                        }
                    }
                    else
                    {
                        mAnimationState = Char_Anim_state::Standing_2;
                    }
                }
            }
            else
            {
                if (mAnimationState == Char_Anim_state::Punching_4)
                {
                    if (mAnimationFrame == 0)
                    {
                        mAnimationState = Char_Anim_state::Standing_2;
                    }
                }
                else
                {
                    if ((mpPed->PedTypeIs(ped_type::script_created_5) || mpPed->PedTypeIs(ped_type::player_2)) && !mTimeout)
                    {
                        if (gRng.get_int(600) < 4u && mCharState != Char_B4_state::Smoking_35)
                        {
                            mCharState = Char_B4_state::Smoking_35;
                            mAnimationFrame = 0;
                            mFrameTimer = 0;
                        }
                    }
                    mAnimationState = Char_Anim_state::Standing_2;
                }
            }
            break;
        case ped_state_2::carjack_opening_door_8:
            mAnimationState = Char_Anim_state::Carjacking_9;
            break;
        case ped_state_2::carjack_pulling_out_9:
            mAnimationState = Char_Anim_state::Carjacking_9;
            break;
        default:
            mAnimationState = Char_Anim_state::Standing_2;
            break;
    }
}

// https://decomp.me/scratch/x9UcS
MATCH_FUNC(0x5520A0)
void Char::UpdateImmobilized()
{
    Fix16 nudge_x;
    Fix16 nudge_y;
    Fix16 ground_z;

    Sprite* pNearestSprite;
    Fix16 fall_x;
    Fix16 fall_y;
    Sprite* pAttachSprite;
    Sprite* pPedObjSprite;

    Ang16 rotation;

    s16 tile_frac_x = 0;
    s16 tile_frac_y = 0;

    mpPed->ClearAttacking_403A40();
    if (mStateInitPending == 1)
    {
        mStateInitPending = false;
        switch (mPedState2)
        {
            case ped_state_2::lying_on_floor_22:
                if (mpPed->PedTypeIs(ped_type::player_2))
                {
                    mLyingOnFloorTimer = 20;
                }
                else
                {
                    mLyingOnFloorTimer = 60;
                }

                switch (mCharState)
                {
                    case Char_B4_state::Hit_From_Front_33:
                        if (mAnimationState != Char_Anim_state::Knocked_Down_Hit_Front_15)
                        {
                            mAnimationState = Char_Anim_state::Knocked_Down_Hit_Front_15;
                            mAnimationFrame = 0;
                        }
                        break;
                    case Char_B4_state::Hit_From_Behind_34:
                        if (mAnimationState != Char_Anim_state::Knocked_Down_Hit_Behind_16)
                        {
                            mRotation = mRotation + kAng180_6FD936;
                            mAnimationState = Char_Anim_state::Knocked_Down_Hit_Behind_16;
                            mAnimationFrame = 0;
                        }
                        break;
                    default:
                        mAnimationState = Char_Anim_state::Lying_On_Floor_10;
                        break;
                }
                mSpritePtr->SetDrawLayer(6);
                break;

            case ped_state_2::falling_19:
                mAnimationState = Char_Anim_state::Normal_Fall_11;
                mFallSpeed = mVelocity;
                mFallZSpeed = kFP16Zero_6FD9E4;
                mAnimationFrame = 0;
                GetTileFracX64(gCharB4_Saved_Xpos, &tile_frac_x);
                GetTileFracY64(gCharB4_Saved_Ypos, &tile_frac_y);
                nudge_x = kFP16Zero_6FD9E4;
                nudge_y = kFP16Zero_6FD9E4;
                if (tile_frac_x < 10)
                {
                    nudge_x = kFpOneSixteenth;
                }
                if (tile_frac_x > 54)
                {
                    nudge_x = -kFpOneSixteenth;
                }
                if (tile_frac_y < 10)
                {
                    nudge_y = kFpOneSixteenth;
                }
                if (tile_frac_y > 54)
                {
                    nudge_y = -kFpOneSixteenth;
                }
                mSpritePtr->set_xyz_lazy_420600(nudge_x + mSpritePtr->mXy.x,
                                                         nudge_y + mSpritePtr->mXy.y,
                                                         mSpritePtr->mZpos);
                break;

            case ped_state_2::sinking_20:
                mAnimationState = Char_Anim_state::Standing_2;
                mTimer = 10;
                break;

            case ped_state_2::knocked_recover_24:
            case ped_state_2::knocked_down_25:
            case ped_state_2::knocked_fatal_26:
                if (!mpPed->mpObj2C)
                {
                    mpPed->Kill();
                    Set_F8_ped_state_1_433910(ped_state_1::dead_9);
                    mPedState2 = ped_state_2::ped2_dead_15;
                    mpPed->mPedState1 = ped_state_1::dead_9;
                    mpPed->mPedState2 = ped_state_2::ped2_dead_15;
                    return;
                }
                if (!mpPed->mpObj2C->mObj3c)
                {
                    mpPed->Kill();
                    Set_F8_ped_state_1_433910(ped_state_1::dead_9);
                    mPedState2 = ped_state_2::ped2_dead_15;
                    mpPed->mPedState1 = ped_state_1::dead_9;
                    mpPed->mPedState2 = ped_state_2::ped2_dead_15;
                    return;
                }
                if (!mpPed->mpObj2C->mSprite)
                {
                    mpPed->Kill();
                    Set_F8_ped_state_1_433910(ped_state_1::dead_9);
                    mPedState2 = ped_state_2::ped2_dead_15;
                    mpPed->mPedState1 = ped_state_1::dead_9;
                    mpPed->mPedState2 = ped_state_2::ped2_dead_15;
                    return;
                }
                mAnimationState = Char_Anim_state::Lethal_Fall_12;
                mSpritePtr->set_xyz_lazy_420600(mpPed->mpObj2C->mSprite->mXy.x,
                                                         mpPed->mpObj2C->mSprite->mXy.y,
                                                         mpPed->mpObj2C->mSprite->mZpos);
                if (mPedState2 != ped_state_2::knocked_recover_24)
                {
                    mRotation = mpPed->mpObj2C->mSprite->mRotation;
                }
                if (!mpPed->mpObj2C->mObj3c->mSpriteCollideMode)
                {
                    mSpritePtr->SetDrawLayer(6);
                }
                mTimeout = 300;
                break;

            case ped_state_2::ped2_at_car_door_17:
                mAnimationState = Char_Anim_state::Getting_pulled_out_of_car_8;
                mAnimationFrame = 0;
                break;

            case ped_state_2::electrocuted_27:
                mAnimationState = Char_Anim_state::Electrocuted_17;
                mAnimationFrame = 0;
                mTimer = 0;
                break;
            default:
                break;
        }
    }
    else
    {
        switch (mPedState2)
        {
            case ped_state_2::sinking_20:
                --mTimer;
                rotation = kAng180_6FD936 + mSpritePtr->mRotation;
                gParticle_8->EmitWaterSplash(mSpritePtr->mXy.x,
                                                           mSpritePtr->mXy.y,
                                                           mSpritePtr->mZpos,
                                                           rotation,
                                                           1);
                if ((mTimer % 2) == 0)
                {
                    mSpritePtr->ShrinkSprite(kFpOneThirtySecond, kFpOneThirtySecond, 0);
                }
                if (mTimer == 0)
                {
                    // Ped sunk
                    mpPed->Kill();
                    mpPed->ChangeNextPedState1(ped_state_1::dead_9);
                    mpPed->ChangeNextPedState2(ped_state_2::ped2_dead_15);
                    mpPed->RestorePreviousPedState();
                    mSpritePtr->set_z_lazy(kFP16Zero_6FD9E4);
                    mpPed->mBf224.bDropsWeaponOnDeath = false;
                }
                break;

            case ped_state_2::lying_on_floor_22:

                if (mLyingOnFloorTimer == 0)
                {
                    if (!mpPed->mBf21C.bBusted)
                    {
                        mpPed->mShockCounter = 0;
                        if (mpPed->mStoredPedState1 == ped_state_1::dead_9)
                        {
                            mpPed->RestorePreviousPedState();
                            if (!mpPed->PedTypeIs(ped_type::player_2) && !mpPed->PedTypeIs(ped_type::script_created_5))
                            {
                                mpPed->SetObjective(objectives_enum::dead_patient_28);
                            }
                        }
                        else
                        {
                            if (mpPed->get_health() == 0)
                            {
                                mpPed->Kill();
                            }
                            else
                            {
                                mpPed->ChangeNextPedState1(ped_state_1::walking_0);
                                mpPed->ChangeNextPedState2(ped_state_2::ped2_walking_0);
                                mpPed->RestorePreviousPedState();
                            }

                            mAnimationState = Char_Anim_state::Walking_0;
                        }
                        gSpriteGrid_2->CheckAndHandleCollisionInStrips(mSpritePtr);
                        if (mConveyorDx != kFP16Zero_6FD9E4 || mConveyorDy != kFP16Zero_6FD9E4)
                        {
                            mSpritePtr->set_xyz_lazy_420600(mConveyorDx + mSpritePtr->mXy.x,
                                                                     mConveyorDy + mSpritePtr->mXy.y,
                                                                     mSpritePtr->mZpos);
                        }
                    }
                }

                break;

            case ped_state_2::ped2_at_car_door_17:
                if (!mTargetCar->IsDoorAccessible(mpPed->mTargetCarDoor))
                {
                    mAnimationFrame = 3;
                }
                if (mAnimationFrame == 3)
                {

                    if (mTargetCar->mPhysics)
                    {
                        mpPed->mpObj2C =
                            gObject_5C->NewMovingObj(objects::ped_pushed_110,
                                                                   mSpritePtr->mXy.x,
                                                                   mSpritePtr->mXy.y,
                                                                   mSpritePtr->mZpos,
                                                                   mTargetCar->mCarSprite->mRotation,
                                                                   mSpritePtr->mRotation,
                                                                   mTargetCar->mPhysics->vec_len(),
                                                                   -kFpOne64th_6FD868,
                                                                   0);
                    }
                    else
                    {
                        mpPed->mpObj2C =
                            gObject_5C->NewMovingObj(objects::ped_pushed_110,
                                                                   mSpritePtr->mXy.x,
                                                                   mSpritePtr->mXy.y,
                                                                   mSpritePtr->mZpos,
                                                                   mTargetCar->mCarSprite->mRotation,
                                                                   mSpritePtr->mRotation,
                                                                   kFpOneSixteenth,
                                                                   -kFpOne64th_6FD868,
                                                                   0);
                    }

                    mpPed->mStoredPedState1 = ped_state_1::walking_0;
                    mpPed->mStoredPedState2 = ped_state_2::ped2_walking_0;
                    mpPed->ChangeNextPedState1(ped_state_1::immobilized_8);
                    gParticle_8->EmitBloodBurst(mSpritePtr->mXy.x,
                                                              mSpritePtr->mXy.y,
                                                              mSpritePtr->mZpos,
                                                              mSpritePtr->mRotation);
                    mpPed->ChangeNextPedState2(ped_state_2::knocked_down_25);
                    mPedState2 = ped_state_2::knocked_recover_24;
                    mAnimationState = Char_Anim_state::Lying_On_Floor_10;
                }
                break;
            case ped_state_2::falling_19:
                ground_z = gMap_0x370->FindGroundZBelowCoord(gCharB4_Saved_Xpos, gCharB4_Saved_Ypos, gCharB4_Saved_Zpos);
                if (gCharB4_Saved_Zpos < ground_z || gCharB4_Saved_Zpos >= ground_z + kFP16Eighth)
                {
                    Ang16::PolarToCartesian_41FC20(mRotation, mFallSpeed, fall_x, fall_y);
                    fall_x += gCharB4_Saved_Xpos;
                    fall_y += gCharB4_Saved_Ypos;
                    if (Char::CanMoveToTile(fall_x.ToInt(), fall_y.ToInt()))
                    {
                        mSpritePtr->set_xyz_lazy_420600(fall_x, fall_y, mSpritePtr->mZpos);
                    }
                    if (mFallZSpeed < kFpPoint1_6FD824)
                    {
                        mFallZSpeed += kFpPoint01_6FD9A0;
                    }
                    mSpritePtr->mZpos -= mFallZSpeed;
                    if (mFallSpeed > kFP16One_6FD9E8)
                    {
                        mFallSpeed -= kFpPoint2_6FD82C;
                    }
                }
                else
                {
                    mSpritePtr->mZpos = ground_z;
                    mpPed->RestorePreviousPedState();
                    Set_F8_ped_state_1_433910(mpPed->GetPedState());
                    mPedState2 = mpPed->GetPedState2();
                    pNearestSprite = gSpriteGrid_1->FindCollidingSpritePreferType(mSpritePtr, sprite_types_enum::none_0);
                    if (mAnimationState == Char_Anim_state::Lethal_Fall_12)
                    {
                        if (!mpPed->mBf21C.bOnFire)
                        {
                            mpPed->mVoiceEvent = 27;
                        }
                        if (pNearestSprite)
                        {
                            mpPed->mpObj2C = gObject_5C->NewMovingObj(objects::ped_pushed_110,
                                                                                                     mSpritePtr->mXy.x,
                                                                                                     mSpritePtr->mXy.y,
                                                                                                     mSpritePtr->mZpos,
                                                                                                     mSpritePtr->mRotation,
                                                                                                     mSpritePtr->mRotation,
                                                                                                     kFpPoint1_6FD824,
                                                                                                     -kFpPoint01_6FD9A0,
                                                                                                     kFP16Zero_6FD9E4);

                            mpPed->ChangeNextPedState1(ped_state_1::immobilized_8);
                            mpPed->ChangeNextPedState2(ped_state_2::knocked_fatal_26);
                            // 9.6f: Ped::SetStateInitPending here and below (inlined, using it makes the diff worse)
                            mpPed->mGameObject->mStateInitPending = true;
                            return;
                        }

                        if (mpPed->mInvulnerability > 0)
                        {
                            SetPedState1_433910(0);
                            SetPedState2(0);
                        }
                        else
                        {
                            mpPed->Kill();
                        }
                    }
                    else
                    {
                        mpPed->SetVoiceEventIfNotOnFire(26);
                        if (pNearestSprite)
                        {
                            Char::DoJump();
                        }
                    }
                }
                break;

            case ped_state_2::knocked_recover_24:
            case ped_state_2::knocked_down_25:
            case ped_state_2::knocked_fatal_26:

                if (mTimeout > 0)
                {
                    mTimeout--;
                }
                if (mTimeout == 0)
                {
                    mPedState2 = ped_state_2::knocked_fatal_26;
                }
                if (!mpPed->mpObj2C)
                {
                    mpPed->Kill();
                    Set_F8_ped_state_1_433910(ped_state_1::dead_9);
                    mPedState2 = ped_state_2::ped2_dead_15;
                    mpPed->mPedState1 = ped_state_1::dead_9;
                    mpPed->mPedState2 = ped_state_2::ped2_dead_15;
                    return;
                }
                if (mpPed->mpObj2C->GetEffectiveSpeed() == kFP16Zero_6FD9E4 || !mTimeout)
                {
                    switch (mPedState2)
                    {
                        case ped_state_2::knocked_recover_24:
                            mpPed->RestorePreviousPedState();
                            Set_F8_ped_state_1_433910(mpPed->mPedState1);
                            mPedState2 = mpPed->mPedState2;
                            break;
                        case ped_state_2::knocked_down_25:
                            mpPed->ChangeNextPedState1(ped_state_1::immobilized_8);
                            mpPed->mPedState2 = ped_state_2::lying_on_floor_22;
                            mpPed->mGameObject->mStateInitPending = true;
                            Set_F8_ped_state_1_433910(ped_state_1::immobilized_8);
                            mPedState2 = ped_state_2::lying_on_floor_22;
                            break;
                        case ped_state_2::knocked_fatal_26:
                            mpPed->RestorePreviousPedState();
                            mpPed->Kill();
                            mpPed->SetVoiceEventIfNotOnFire(27);
                            break;
                    }
                    pPedObjSprite = mpPed->mpObj2C->mSprite;

                    mSpritePtr->set_xyz_lazy_420600(pPedObjSprite->mXy.x, pPedObjSprite->mXy.y, pPedObjSprite->mZpos);

                    mSpritePtr->set_ang_lazy(mpPed->mpObj2C->mSprite->mRotation);

                    if (!mpPed->mpObj2C->mObj3c->mSpriteCollideMode)
                    {
                        mSpritePtr->SetDrawLayer(6);
                    }
                    mpPed->mpObj2C->RequestRemoval();
                    mpPed->mpObj2C = NULL;
                    Char::ManageZCoordAndSlopes();
                    if (static_cast<u8>(Char::IsOnWater()))
                    {
                        mpPed->PutOutFire();
                        Char::DrownPed();
                        return;
                    }

                    gSpriteGrid_2->CheckAndHandleCollisionInStrips(mSpritePtr);

                    if (mConveyorDx != kFP16Zero_6FD9E4 || mConveyorDy != kFP16Zero_6FD9E4)
                    {

                        mSpritePtr->set_xyz_lazy_420600(mConveyorDx + mSpritePtr->mXy.x,
                                                                 mConveyorDy + mSpritePtr->mXy.y,
                                                                 mSpritePtr->mZpos);
                    }
                }
                else
                {
                    pAttachSprite = mpPed->mpObj2C->mSprite;
                    mSpritePtr->set_xyz_lazy_420600(pAttachSprite->mXy.x, pAttachSprite->mXy.y, pAttachSprite->mZpos);

                    if (mPedState2 != ped_state_2::knocked_recover_24)
                    {
                        mRotation = mpPed->mpObj2C->mSprite->mRotation;
                    }
                    if (!mpPed->mpObj2C->mObj3c->mSpriteCollideMode)
                    {
                        mSpritePtr->SetDrawLayer(6);
                    }
                }
                break;

            case ped_state_2::electrocuted_27:
                ++mTimer;
                if (!bStartNetworkGame)
                {
                    if (mTimer == 5)
                    {
                        mpPed->UpdateStatsForKiller();
                    }
                }
                else
                {
                    if (mpPed->mKillerId)
                    {
                        mpPed->mKillerIdTimer = 99;
                    }
                }
                if (mTimer > 100u)
                {
                    if (!bStartNetworkGame)
                    {
                        mpPed->mKillerIdTimer = 0;
                        mpPed->mKillerId = 0;
                    }
                    Set_F8_ped_state_1_433910(ped_state_1::walking_0);
                    mPedState2 = ped_state_2::ped2_walking_0;
                    mpPed->mPedState2 = ped_state_2::ped2_walking_0;
                    mpPed->mPedState1 = ped_state_1::walking_0;
                    mpPed->mBf224.bDropsWeaponOnDeath = false;
                    if (mpPed->mPlayer)
                    {
                        gRoot_sound.PlayVoice(voice_line::shocking_25);
                        mpPed->mPlayer->SetDeathType(4);
                    }
                    mpPed->Kill();
                    Set_F8_ped_state_1_433910(ped_state_1::dead_9);
                    mPedState2 = ped_state_2::ped2_dead_15;
                    mAnimationState = Char_Anim_state::Charred_Corpse_21;
                    mCharState = Char_B4_state::Walking_1;
                }

                break;
            default:
                break;
        }
    }
    if (mpPed->mShockCounter < static_cast<u32>(mpPed->mElectrocutionThreshold))
    {
        --mLyingOnFloorTimer;
    }
}

// https://decomp.me/scratch/4c8aU
MATCH_FUNC(0x552DE0)
Fix16 CarPhysics::vec_len() // Weird location, I'm putting this here to preserve ordering
{
    return mLinvel1.GetLength_41E260();
}

MATCH_FUNC(0x552E90)
void Char::UpdateDead()
{
    s32 rng;

    mpPed->ClearAttacking_403A40();
    if (mStateInitPending == 1)
    {
        switch (mCharState)
        {
            case Char_B4_state::Hit_From_Front_33:
                if (mAnimationState != Char_Anim_state::Knocked_Down_Hit_Front_15)
                {
                    mAnimationState = Char_Anim_state::Knocked_Down_Hit_Front_15;
                    mAnimationFrame = 0;
                }
                break;
            case Char_B4_state::Hit_From_Behind_34:
                if (mAnimationState != Char_Anim_state::Knocked_Down_Hit_Behind_16)
                {
                    mRotation = mRotation + kAng180_6FD936;
                    mAnimationState = Char_Anim_state::Knocked_Down_Hit_Behind_16;
                    mAnimationFrame = 0;
                }
                break;
            default:
                if (mAnimationState != Char_Anim_state::Charred_Corpse_21)
                {
                    rng = gRng.get_int(3);

                    switch (rng)
                    {
                        case 0:
                            mAnimationState = Char_Anim_state::Lying_Pose_14;
                            break;
                        case 1:
                            mAnimationState = Char_Anim_state::Lying_Pose_19;
                            break;
                        case 2:
                            mAnimationState = Char_Anim_state::Lying_Pose_20;
                            break;
                    }
                }
                break;
        }

        mStateInitPending = false;
        if (mpPed->mPedGroup)
        {
            if (mpPed->mPedGroup->mRemoveDeadMembers)
            {
                mpPed->mPedGroup->RemovePed(mpPed);
                mpPed->mPedGroup = NULL;
            }
        }

        mUnused34 = 0;
        gSpriteGrid_2->CheckAndHandleCollisionInStrips(mSpritePtr);
        Char::ManageZCoordAndSlopes();
        mSpritePtr->set_xyz_lazy_420600(mConveyorDx + mSpritePtr->mXy.x,
                                                 mConveyorDy + mSpritePtr->mXy.y,
                                                 mSpritePtr->mZpos);
    }
    else
    {
        switch (mAnimationState)
        {
            case Char_Anim_state::Knocked_Down_Hit_Front_15:
                if (mSpritePtr->mZpos.ToUInt8() != 0 && mSpritePtr->mXy.x.ToUInt8() > 1 &&
                    mSpritePtr->mXy.y.ToUInt8() > 1 && mSpritePtr->mXy.x.ToUInt8() < 254 &&
                    mSpritePtr->mXy.y.ToUInt8() < 254)
                {
                    mVelocity = -k_CollisionRepulsionSpeed;
                    Char::UpdateWalking();
                }
                else
                {
                    mAnimationState = Char_Anim_state::Lying_Pose_14;
                }
                break;
            case Char_Anim_state::Knocked_Down_Hit_Behind_16:
                if (mSpritePtr->mZpos.ToUInt8() != 0 && mSpritePtr->mXy.x.ToUInt8() > 1 &&
                    mSpritePtr->mXy.y.ToUInt8() > 1 && mSpritePtr->mXy.x.ToUInt8() < 254 &&
                    mSpritePtr->mXy.y.ToUInt8() < 254)
                {
                    mVelocity = -k_CollisionRepulsionSpeed;
                    Char::UpdateWalking();
                }
                else
                {
                    mAnimationState = Char_Anim_state::Lying_Pose_14;
                }
                break;
            case Char_Anim_state::Lying_Pose_13:
            case Char_Anim_state::Lying_Pose_14:
            case Char_Anim_state::Lying_Pose_19:
            case Char_Anim_state::Lying_Pose_20:
                if (mConveyorDx != kFP16Zero_6FD9E4 || mConveyorDy != kFP16Zero_6FD9E4)
                {
                    mConveyorDx = kFP16Zero_6FD9E4;
                    mConveyorDy = kFP16Zero_6FD9E4;
                    gSpriteGrid_2->CheckAndHandleCollisionInStrips(mSpritePtr);

                    mSpritePtr->set_xyz_lazy_420600(mConveyorDx + mSpritePtr->mXy.x,
                                                             mConveyorDy + mSpritePtr->mXy.y,
                                                             mSpritePtr->mZpos);
                }

                mSpritePtr->mDrawLayer = 6;
                break;
            default:
                if (mAnimationState != Char_Anim_state::Charred_Corpse_21)
                {
                    rng = gRng.get_int(3);
                    switch (rng)
                    {
                        case 0:
                            mAnimationState = Char_Anim_state::Lying_Pose_14;
                            break;
                        case 1:
                            mAnimationState = Char_Anim_state::Lying_Pose_19;
                            break;
                        case 2:
                            mAnimationState = Char_Anim_state::Lying_Pose_20;
                            break;
                    }
                }
                mAnimationFrame = 0;
                mSpritePtr->SetDrawLayer(6);
                break;
        }
    }
}

MATCH_FUNC(0x5532C0)
bool Char::IsNearTileCentre() const
{
    s16 frac_x = 0;
    s16 frac_y = 0;
    Char::GetTileFracX64(gCharB4_Saved_Xpos, &frac_x);
    Char::GetTileFracY64(gCharB4_Saved_Ypos, &frac_y);
    if (frac_x > 20 && frac_x < 44 && frac_y > 20 && frac_y < 44)
    {
        return true;
    }
    return false;
}

MATCH_FUNC(0x553330)
bool Char::IsThreatToSearchingPed() const
{
    return mpPed->IsThreatToSearchingPed();
}

// 9.6f 0x497480
MATCH_FUNC(0x553340)
bool Char::ShouldCollideWithSprite(Sprite* pSprite) const
{
    Ped* pPed;

    if (pSprite)
    {
        switch (pSprite->GetType())
        {
            case sprite_types_enum::car_2:
                break;

            case sprite_types_enum::ped_3:
                break;

            default:
            {
                Object* pObj = pSprite->AsObject2C();
                if (pObj)
                {
                    u8 idx = mpPed->get_ped_ref_idx();
                    if (pObj->IsOwnedByPedRef(idx))
                    {
                        return false;
                    }

                    if (pObj->IsSpriteGrid2Region3())
                    {
                        return false;
                    }
                }
            }
            break;
        }
    }

    switch (this->mPedState2)
    {
        case ped_state_2::ped2_entering_a_car_6:
        case ped_state_2::ped2_getting_out_a_car_7:
        case ped_state_2::carjack_pulling_out_9:
        case ped_state_2::ped2_driving_10:
        case ped_state_2::ped2_at_car_door_17:
        case ped_state_2::falling_19:
        case ped_state_2::knocked_recover_24:
        case ped_state_2::knocked_down_25:
        case ped_state_2::knocked_fatal_26:
        case ped_state_2::electrocuted_27:
            return false;

        case ped_state_2::carjack_opening_door_8:
            if (pSprite == NULL)
            {
                return true;
            }
            return false;

        case ped_state_2::lying_on_floor_22:
            break;

        default:
            pPed = this->mpPed;
            if (pPed->GetPedState() == ped_state_1::dead_9 &&
                (!pPed->IsOnFire() || !pSprite || !pSprite->IsObjectModelEqual(objects::water_hitting_198)))
            {
                return false;
            }

            if (this->mCharState == Char_B4_state::Jumping_15 && pSprite)
            {
                switch (pSprite->GetType())
                {
                    case sprite_types_enum::car_2:
                        return false;
                    case sprite_types_enum::code_obj1_4:
                        break;
                    default:
                        return false;
                }
            }
            break;
    }
    return true;
}

MATCH_FUNC(0x5535B0)
bool Char::PhoneTouched(Object* pObject)
{
    Ped* pPed = mpPed;
    if (pPed->mPlayer)
    {
        return gScriptManager->AnswerPhone(pPed->get_id(), pObject->mId);
    }
    else
    {
        return false;
    }
}

MATCH_FUNC(0x553640)
bool Char::OnObjectTouched(Object* pObject)
{
    s8 conveyor_x;
    s8 conveyor_y;

    if (pObject->IsPickup())
    {
        return get_ped()->HandlePickupCollision(pObject);
    }

    switch (pObject->mModel)
    {
        case objects::secret_token_266: 
            return get_ped()->HandlePickupCollision(pObject);

        case 257:
        case objects::ped_crossing_trigger_258:
            if (mPedState1 != ped_state_1::dead_9 && mPedState1 != ped_state_1::immobilized_8)
            {
                get_ped()->HandlePedCrossingTrigger(pObject);
            }
            break;

        case objects::savepoint_161:
            gCarThreadTriggers_214->CheckThreadTrigger(pObject->mThreadTriggerIdx, this->mSpritePtr);
            break;

        case objects::blue_phone_164:
        case objects::red_phone_177:
        case objects::yellow_phone_179:
        case objects::green_phone_181:
            return PhoneTouched(pObject);

        case objects::door_trigger_167:
            gDoor_4D4->TryOpenDoorForPed(get_ped(), pObject->GetParam());
            break;

        case objects::destructor_141:
            get_ped()->Kill();
            break;

        case objects::conveyor_139:
            pObject->GetConveyorDirection(&conveyor_x, &conveyor_y);
            this->mConveyorDx = kFpOne64th_6FDA9C * conveyor_x;
            this->mConveyorDy = kFpOne64th_6FDA9C * conveyor_y;
            break;

        default:
            break;
    }
    return false;
}

MATCH_FUNC(0x5537F0)
bool Char::HandlePedObjectHit(Object* pObject)
{
    const u8 idx = pObject->GetPedRefIdx();
    const s32 pedId = gPedRefTable_7F8->GetPedId(idx);
    if (pObject->mModel == objects::rocket_bullet_128 || pObject->mModel == objects::moving_molotov_138 || pObject->mModel == objects::mine_10 && !gCharB4_HitByMine_6FDB59)
    {
        gObject_5C->CreateExplosion(mSpritePtr->mXy.x,
                                                  mSpritePtr->mXy.y,
                                                  mSpritePtr->mZpos,
                                                  kAng0_6FDB34,
                                                  explosion_type::small_18,
                                                  pedId);
        if (pObject->mModel == objects::mine_10)
        {
            gCharB4_HitByMine_6FDB59 = 1;
        }
    }

    if (pObject->GetPedRefIdx() != mpPed->get_ped_ref_idx())
    {
        return mpPed->HandlePedHitByObject(pObject);
    }
    else
    {
        return false;
    }
}

// GetLength_41E260 as inlined into HandleCarImpact: kFP16Zero_6FD9E4 as the zero, out of line helpers
static inline Fix16 GetLength_inline_5538A0(Fix16_Point& p)
{
    if (p.x == kFP16Zero_6FD9E4)
    {
        return Fix16::Abs_negate_out_of_line(p.y);
    }
    else if (p.y == kFP16Zero_6FD9E4)
    {
        return Fix16::Abs_negate_out_of_line(p.x);
    }
    else
    {
        return Fix16::SquareRoot_436A70(p.x.Multiply(p.x).Add(p.y.Multiply(p.y)));
    }
}

MATCH_FUNC(0x5538A0)
#define SPAWN_KNOCKDOWN_OBJ(speed)                                                      \
    gObject_5C->NewMovingObj(objects::ped_pushed_110,                     \
                                           this->mSpritePtr->mXy.x,    \
                                           this->mSpritePtr->mXy.y,    \
                                           this->mSpritePtr->mZpos,    \
                                           impact_angle,                                \
                                           this->mSpritePtr->mRotation, \
                                           speed,                                       \
                                           -kFpPoint1_6FD824,                           \
                                           kFP16Zero_6FD9E4)
void Char::HandleCarImpact(Car* pCar, u8 not_used, Fix16 x, Fix16 y)
{
    Fix16_Point p(x, y);
    Fix16 vecLen = GetLength_inline_5538A0(p);
    Ang16 impact_angle;
    impact_angle = p.atan2_40F790();

    if (this->mpPed->mInvulnerability > 0)
    {
        DoJump();
    }
    else
    {
        if (vecLen < sRunOrJumpSpeed)
        {
            vecLen = sRunOrJumpSpeed;
        }

        if (!this->mpPed->mPedRefIdx)
        {
            this->mpPed->mPedRefIdx = gPedRefTable_7F8->AllocForPed(this->mpPed->mId);
        }

        this->mAnimationState = Char_Anim_state::Lethal_Fall_12;
        this->mAnimationFrame = 0;

        if (pCar->mDriver)
        {
            if (!pCar->mDriver->mBf21C.bDriveAggressively && pCar->mDriver->PedTypeIs(ped_type::dummy_3))
            {
                if (pCar->mAI)
                {
                    pCar->mAI->mForcedStopTimer = 100;
                }
            }
        }

        if (vecLen < kFpPoint1_6FD824)
        {
            if (this->mPedState2 == ped_state_2::lying_on_floor_22)
            {
                this->mpPed->Kill();
            }
            else
            {
                this->mpPed->mpObj2C = SPAWN_KNOCKDOWN_OBJ(vecLen);
                this->mpPed->mpObj2C->mObj3c->mZSpeed = kFP16Zero_6FD9E4;
                this->mpPed->mpObj2C->SetDamageOwner(this->mpPed->mPedRefIdx);
                this->mpPed->ChangeNextPedState1(ped_state_1::immobilized_8);
                this->mpPed->ChangeNextPedState2(ped_state_2::knocked_recover_24);
                this->mPedState2 = ped_state_2::knocked_recover_24;
            }
        }
        else if (this->mpPed->PedTypeIs(ped_type::dummy_3))
        {
            this->mpPed->Kill();
            gParticle_8->EmitBloodBurst(this->mSpritePtr->mXy.x,
                                                      this->mSpritePtr->mXy.y,
                                                      this->mSpritePtr->mZpos,
                                                      impact_angle);
        }
        else if (vecLen < kFpPoint3_6FD830)
        {
            if (vecLen < kFpPoint2_6FD82C)
            {
                if (this->mPedState2 == ped_state_2::lying_on_floor_22)
                {
                    this->mpPed->Kill();
                }
                else
                {
                    this->mpPed->mpObj2C = SPAWN_KNOCKDOWN_OBJ(vecLen / gFix16_Two);
                    this->mpPed->mpObj2C->mObj3c->mZSpeed = kFpPoint05_6FD9B0;
                    this->mpPed->mpObj2C->mObj3c->mbAirborne = 1;
                    this->mpPed->mpObj2C->SetDamageOwner(this->mpPed->mPedRefIdx);
                    this->mpPed->ChangeNextPedState1(ped_state_1::immobilized_8);

                    if (this->mpPed->mInvulnerability > 0)
                    {
                        this->mpPed->ChangeNextPedState2(ped_state_2::knocked_recover_24);
                    }
                    else if (pCar->mPedType == ped_type::player_2)
                    {
                        this->mpPed->ChangeNextPedState2(ped_state_2::knocked_fatal_26);
                    }
                    else
                    {
                        this->mpPed->ChangeNextPedState2(ped_state_2::knocked_down_25);
                    }

                    this->mPedState2 = ped_state_2::knocked_recover_24;

                    gParticle_8->EmitBloodBurst(this->mSpritePtr->mXy.x,
                                                              this->mSpritePtr->mXy.y,
                                                              this->mSpritePtr->mZpos,
                                                              impact_angle);
                }
            }
            else
            {
                if (this->mpPed->PedTypeIs(ped_type::player_2) && pCar->mPedType != ped_type::player_2)
                {
                    if (this->mPedState2 == ped_state_2::lying_on_floor_22)
                    {
                        this->mpPed->Kill();
                        return;
                    }
                    this->mpPed->ChangeNextPedState1(ped_state_1::immobilized_8);
                    this->mpPed->mpObj2C = SPAWN_KNOCKDOWN_OBJ(vecLen / gFix16_Two);
                    this->mpPed->mpObj2C->mObj3c->mZSpeed = kFpPoint1_6FD824;
                    this->mpPed->mpObj2C->mObj3c->mbAirborne = 1;
                    this->mpPed->mpObj2C->SetDamageOwner(this->mpPed->mPedRefIdx);
                    if (mpPed->mInvulnerability > 0)
                    {
                        mpPed->ChangeNextPedState2(ped_state_2::knocked_recover_24);
                    }
                    else
                    {
                        mpPed->ChangeNextPedState2(ped_state_2::knocked_down_25);
                    }

                    this->mPedState2 = ped_state_2::knocked_recover_24;
                }
                else
                {
                    this->mpPed->Kill();
                }
                gParticle_8->EmitBloodBurst(this->mSpritePtr->mXy.x,
                                                          this->mSpritePtr->mXy.y,
                                                          this->mSpritePtr->mZpos,
                                                          impact_angle);
            }
        }
        else
        {
            if (this->mpPed->mInvulnerability > 0)
            {
                this->mpPed->mpObj2C = SPAWN_KNOCKDOWN_OBJ(vecLen / gFix16_Two);
                this->mpPed->mpObj2C->mObj3c->mZSpeed = kFP16Zero_6FD9E4;
                this->mpPed->mpObj2C->SetDamageOwner(this->mpPed->mPedRefIdx);
                this->mpPed->ChangeNextPedState1(ped_state_1::immobilized_8);
                this->mpPed->ChangeNextPedState2(ped_state_2::knocked_recover_24);
                this->mPedState2 = ped_state_2::knocked_recover_24;
            }
            else
            {
                this->mpPed->Kill();
            }
            gParticle_8->EmitBloodBurst(this->mSpritePtr->mXy.x,
                                                      this->mSpritePtr->mXy.y,
                                                      this->mSpritePtr->mZpos,
                                                      impact_angle);
        }
    }
}
#undef SPAWN_KNOCKDOWN_OBJ

MATCH_FUNC(0x553E00)
void Char::HandleGenericImpact(Ang16 ang, Fix16 impact_x, Fix16 impact_z, char_type impact_type)
{

    if (mpPed->mInvulnerability > 0)
    {
        impact_type = 0;
    }

    if (mpPed->GetPedState() != ped_state_1::dead_9)
    {
        this->mStateInitPending = true;
        this->mpPed->mpObj2C = gObject_5C->NewMovingObj(objects::ped_pushed_110,
                                                                                       mSpritePtr->mXy.x,
                                                                                       mSpritePtr->mXy.y,
                                                                                       mSpritePtr->mZpos,
                                                                                       ang,
                                                                                       mSpritePtr->mRotation,
                                                                                       impact_x,
                                                                                       -kFpPoint02_6FD9AC,
                                                                                       impact_z);
        if (!mpPed->mPedRefIdx)
        {
            mpPed->mPedRefIdx = gPedRefTable_7F8->AllocForPed(mpPed->mId);
        }
        mpPed->mpObj2C->SetDamageOwner(mpPed->mPedRefIdx);
        mpPed->ChangeNextPedState1(ped_state_1::immobilized_8);

        switch (impact_type)
        {
            case 0:
                mpPed->mpObj2C->mObj3c->mZSpeed = kFP16Zero_6FD9E4;
                mpPed->ChangeNextPedState2(ped_state_2::knocked_recover_24);
                mPedState2 = ped_state_2::knocked_recover_24;
                break;

            case 1:
                mpPed->mpObj2C->mObj3c->mZSpeed = impact_z;
                if (mpPed->mInvulnerability > 0)
                {

                    mpPed->ChangeNextPedState2(ped_state_2::knocked_recover_24);
                }
                else
                {
                    if (mpPed->PedTypeIs(ped_type::player_2))
                    {
                        mpPed->ChangeNextPedState2(ped_state_2::knocked_down_25);
                    }
                    else
                    {
                        mpPed->ChangeNextPedState2(ped_state_2::knocked_fatal_26);
                    }
                }

                this->mPedState2 = ped_state_2::knocked_recover_24;
                gParticle_8->EmitBloodBurst(this->mSpritePtr->mXy.x,
                                                          this->mSpritePtr->mXy.y,
                                                          this->mSpritePtr->mZpos,
                                                          ang);
                break;

            case 2:
                mpPed->mpObj2C->mObj3c->mZSpeed = impact_z;
                if (mpPed->mInvulnerability > 0)
                {
                    mpPed->ChangeNextPedState2(ped_state_2::knocked_recover_24);
                }
                else
                {
                    mpPed->ChangeNextPedState2(ped_state_2::knocked_fatal_26);
                }

                this->mPedState2 = ped_state_2::knocked_recover_24;
                gParticle_8->EmitBloodBurst(this->mSpritePtr->mXy.x,
                                                          this->mSpritePtr->mXy.y,
                                                          this->mSpritePtr->mZpos,
                                                          ang);
                break;
        }
    }
}

MATCH_FUNC(0x553F90)
void __stdcall ResetCharStatics()
{
    LOCAL_STATIC(s8, sCharB4Unused, 0x6FDB49);
    sCharB4UpdateCounter = 0;
    sCharB4Unused = 0;
    sB4Id = 0;
}

MATCH_FUNC(0x551CA0)
EXPORT void Char::UpdateLeavingTrain()
{
    // State 6 deliberately does nothing in the original game.
}
