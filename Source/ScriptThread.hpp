#pragma once

#include "Fix16_Point.hpp"
#include "Function.hpp"
#include "Pool.hpp"
#include "ang16.hpp"
#include "fix16.hpp"
#include "miss2_xyz.hpp"

class Ped;
class Generator;
class Crusher;
class ScriptGosubStack;
class Car;
class Object;
class Crane;
class Light;
class Door;
class ScriptThread;
class Hud_Arrow;
class AttachedSprite;

struct SCR_CMD_HEADER
{
    s16 mCmdThis; // 0x0
    u16 mType; // 0x2
    s16 mCmdNext; // 0x4
    s16 mReturnValue; // 0x6
};

struct SCR_ONEVAR_RECT : SCR_CMD_HEADER
{
    u16 mVarname; // 0x8
    u16 mUnusedA; // 0xA
    SCR_Rect_f mRect; // 0xC
};

struct SCR_PLAYER_PED
{
    SCR_CMD_HEADER mHeader; // 0x0
    Ped* mPed; // 0x8
    Fix16_Vec mPos; // 0xC
    Ang16 mRot; // 0x18
    s16 mRemap; // 0x1A
};

struct SCR_CAR_DATA_DEC : SCR_CMD_HEADER
{
    u16 mCarIdx; // 0x8
    u16 mUnusedA; // 0xA
    Fix16_Vec mPos; // 0xC
    Ang16 mRot; // 0x18
    s16 mRemap; // 0x1A
    s16 mCarId; // 0x1C
    s16 mTrailerId; // 0x1E
};

struct SCR_CHAR_DATA_DEC : SCR_CMD_HEADER
{
    Ped* mChar; // 0x8
    Fix16_Vec mPos; // 0xC
    Ang16 mRot; // 0x18
    s16 mRemap; // 0x1A
    s16 mOccupation; // 0x1C
    u16 mPadding1E; // 0x1E
};

struct SCR_OBJ_DATA : SCR_CMD_HEADER
{
    Object* mObj; // 0x8
    Fix16_Vec mPos; // 0xC
    u16 mObjId; // 0x18
    Ang16 mRot; // 0x1A
    u8 mValueShopType; // 0x1C
};

struct SCR_TWO_PARAMS : SCR_CMD_HEADER
{
    union
    {
        struct
        {
            u16 mUnsigned1; // 0x8
            u16 mUnsigned2; // 0xA
        };
        struct
        {
            s16 mSigned1; // 0x8
            s16 mSigned2; // 0xA
        };
        u32 mU32; // 0x8
        s32 mS32; // 0x8
    };
};

struct SCR_FOUR_PARAMS : SCR_CMD_HEADER
{
    union
    {
        struct
        {
            u16 mUnsigned1; // 0x8
            u16 mUnsigned2; // 0xA
        };
        struct
        {
            s16 mSigned1; // 0x8
            s16 mSigned2; // 0xA
        };
    };
    union
    {
        struct
        {
            u16 mUnsigned3; // 0xC
            u16 mUnsigned4; // 0xE
        };
        struct
        {
            s16 mSigned3; // 0xC
            s16 mSigned4; // 0xE
        };
        u32 mU32; // 0xC
        s32 mS32; // 0xC
    };
};

struct SCR_POINTER : SCR_CMD_HEADER
{
    union
    {
        Ped* mChar; // 0x8
        Car* mCar; // 0x8
        Object* mObj; // 0x8
        Crane* mCrane; // 0x8
        Light* mLight; // 0x8
        Door* mDoor; // 0x8
        Crusher* mCrusher; // 0x8
        s32 mCounter; // 0x8
        u16 mIndex; // 0x8
        union
        {
            u16 mUnused8_1; // 0x8
            u16 mUnused8_2; // 0x8
        } words;
        Generator* mGenerator; // 0x8
        Hud_Arrow* mArrow; // 0x8
        AttachedSprite* mSprite; // 0x8
    };
};

struct SCR_CMD_ALTER_WANTED_LEVEL : SCR_CMD_HEADER
{
    u8 pad1;
    u8 pad2;
    u8 mWantedLevel; // 0xC
};

struct SCR_LOC_SEC_CHAR : SCR_CMD_HEADER
{
    u16 mChar1Idx; // 0x8
    u16 mChar2Idx; // 0xA
    Fix16 mXRange; // 0xC
    Fix16 mYRange; // 0x10
};

struct SCR_THREAD : SCR_CMD_HEADER
{
    ScriptThread* mScriptThread; // 0x8
    s16 mThreadId; // 0xC
    u16 mThreadStartCmd; // 0xE
    union
    {
        struct
        {
            u16 mCharVarIdx; // 0x10
            u16 mCarOrPhoneIdx; // 0x12
        };
        struct
        {
            u16 mCharIdx10; // 0x10
            u8 mX; // 0x12
            u8 mY; // 0x13
            u8 mZ; // 0x14
            char_type mTriggerIdx; // 0x15
            u8 mTriggerAdded; // 0x16
        };
    };
};

struct SCR_CHAR_AREA_ANY : SCR_CMD_HEADER
{
    ScriptThread* mScriptThread; // 0x8
    s16 mUnusedC; // 0xC
    u16 mUnusedE; // 0xE
    Fix16 mX; // 0x10
    Fix16 mY; // 0x14
    Fix16 mZ; // 0x18
    Fix16 mW; // 0x1C
    Fix16 mH; // 0x20
    u16 mCharIdx24; // 0x24
    char_type mTriggerIdx; // 0x26
    u8 mTriggerAdded; // 0x27
};

struct SCR_CHAR_OBJECTIVE : SCR_CMD_HEADER
{
    u16 mCharIdx; // 0x8:  SCR_SET_CHAR_OBJ1
    s16 mObjective; // 0xA
    union
    {
        struct
        {
            u16 mSecondItemIdx; // 0xC:  SCR_SET_CHAR_OBJ2
            u16 mVariant; // 0xE
        };
        struct
        {
            u16 mCarIdx; // 0xC:  SET_CHAR_OBJ_FOLLOW
            u16 mRotation; // 0xE
            s32 mOffset; // 0x12
        };
    };
};

struct SCR_CHAR_OBJ3 : SCR_CMD_HEADER
{
    u16 mCharIdx; // 0x8
    s16 mObjective; // 0xA
    Fix16_Vec mPos; // 0xC:  SCR_SET_CHAR_OBJ3
};

struct SCR_EXPLODE : SCR_CMD_HEADER
{
    Fix16_Vec mPos; // 0x8
};

struct SCR_EXPLODE_WALL : SCR_CMD_HEADER
{
    Fix16_Vec mPos; // 0x8
    u8 mFace; // 0x14
    u8 mPadding15; // 0x15
    u16 mPadding16; // 0x16
};

struct SCR_SOUND_DECSET : SCR_CMD_HEADER
{
    u16 mVarname; // 0x8
    u16 mUnusedA; // 0xA
    Fix16_Vec mPos; // 0xC
    u8 mSoundId; // 0x18
    u8 mPlayType; // 0x19
    u16 mPadding1B; // 0x1B
};

struct SCR_SET_STATION : SCR_CMD_HEADER
{
    u16 mPlatform; // 0x8
    u8 mNumPassenger; // 0xA
    u8 mNumFreight; // 0xB
    u8 mNumBoxcar; // 0xC
    u8 mPaddingD; // 0xD
    u16 mPaddingE; // 0xE
};

struct SCR_RADIOSTATION_DEC : SCR_CMD_HEADER
{
    SCR_XY_f mPos; // 0x8
    u8 mStationType; // 0x10
    u8 mPadding11; // 0x11
    u16 mPadding12; // 0x12
};

struct SCR_CRANE_TARGET_DEC : SCR_CMD_HEADER
{
    Crane* mCrane; // 0x8
    u32 mHomecrane; // 0xC
    SCR_XY_f mPos; // 0x10
    u16 mHomeRotation; // 0x18
    u16 mHomeCranetype; // 0x1A: crane_home_target
    SCR_XY_f mTargetPos; // 0x1C
    u16 mTargetRotation; // 0x24
    u16 mUnused26; // 0x26
};

struct SCR_CRANE_BASIC_DEC : SCR_CMD_HEADER
{
    Crane* mCrane; // 0x8
    u32 mHomecrane; // 0xC
    SCR_XY_f mPos; // 0x10
    u16 mHomeRotation; // 0x18
    u16 mHomeCranetype; // 0x1A: crane_home_target
    SCR_XY_f mTargetPos; // 0x1C
    u16 mTargetRotation; // 0x24
    u16 mSecondRotation; // 0x26
    SCR_XY_f mSecondPos; // 0x28
};

struct SCR_DECLARE_CRANE_POWERUP : SCR_CMD_HEADER
{
    u16 mCraneIdx; // 0x8
    u16 mGenerator; // 0xA
    SCR_XYZ_uc mPos; // 0xC
    u8 mPadding18; // 0x18
};

struct SCR_MAP_ZONE_SET : SCR_CMD_HEADER
{
    u16 mCarDensity; // 0x8
    u16 mGoodcarRatio; // 0xA
    u16 mBadcarRatio; // 0xC
    u16 mPolicecarRatio; // 0xE
    u16 mPedDensity; // 0x10
    u16 mMuggerRatio; // 0x12
    u16 mCarthiefRatio; // 0x14
    u16 mElvisRatio; // 0x16
    u16 mGangcharRatio; // 0x18
    u16 mPolicepedRatio; // 0x1A
    u16 mGangcarRatio; // 0x1C
    u16 mPadding1E; // 0x1E
};

struct SCR_SET_GANG_INFO : SCR_CMD_HEADER
{
    u16 mGangname; // 0x8
    s8 mRemap; // 0xA
    u8 mWeapon1; // 0xB
    u8 mWeapon2; // 0xC
    u8 mWeapon3; // 0xD
    u8 mArrowId; // 0xE
    u8 mKillRespectChange; // 0xF
    Fix16_Vec mPos; // 0x10
    u16 mCarModel; // 0x1C
    s16 mCarRemap; // 0x1E
};

struct SCR_GENERATOR : SCR_CMD_HEADER
{
    u16 mPointerLo; // 0x8
    u16 mPointerHi; // 0xA
    Fix16_Vec mPos; // 0xC
    Ang16 mRot; // 0x18
    u16 mObjId; // 0x1A
    u16 mMindelay; // 0x1C
    u16 mMaxdelay; // 0x1E
    u8 mAmmo; // 0x20
    u8 mPadding21; // 0x21
    u16 mPadding22; // 0x22
};

struct SCR_PHONE_TEMPLATE : SCR_CMD_HEADER
{
    u16 mBriefId; // 0x8
    u16 mCounter1; // 0xA
    u16 mCounter3b; // 0xC
    u16 mMission2; // 0xE
    u16 mCounter2; // 0x10
    u16 mCounter3; // 0x12
    u16 mCounter4; // 0x14
    u16 mCounter5; // 0x16
    u16 mGangname; // 0x18
    u16 mRespect; // 0x1A
    u16 mMission1; // 0x1C
    u16 mPadding1E; // 0x1E
};

struct SCR_START_BONUS : SCR_CMD_HEADER
{
    u16 mZoneStrId; // 0x8
    u8 mSubtype; // 0xA
    s8 mRemap; // 0xB
    s8 mTargetCount; // 0xC
    u8 mbExclusive; // 0xD
    u16 mId; // 0xE
    u16 mTimeLimit; // 0x10
    s16 mAltCarModel; // 0x12
    u16 mReward; // 0x14
    u16 mUnused16; // 0x16
    u16 mPtr; // 0x18
    u8 mDeathCause; // 0x1A
    u8 mUnused1B; // 0x1B
};

struct SCR_LOWER_LEVEL : SCR_CMD_HEADER
{
    SCR_XY_uc mMinPos; // 0x8
    SCR_XY_uc mMaxPos; // 0xA
};

struct SCR_CHAR_INTO_CAR : SCR_CMD_HEADER
{
    u16 mCharIdx; // 0x8
    u16 mUnusedA; // 0xA
    u8 mRemap; // 0xC
    u8 mUnusedD; // 0xD
    s16 mOccupation; // 0xE
    u16 mCarIdx; // 0x10
};

struct SCR_ADD_BLOCK : SCR_CMD_HEADER
{
    u8 mXpos; // 0x8
    u8 mYpos; // 0x9
    u8 mZpos; // 0xA
};

struct SCR_REMOVE_BLOCK : SCR_CMD_HEADER
{
    SCR_XYZ_uc mPos; // 0x8
    u8 mDoDrop; // 0xB
};

struct SCR_CONVEYOR : SCR_CMD_HEADER
{
    u16 mPointerLo; // 0x8
    u16 mPointerHi; // 0xA
    SCR_Rect_f mRect; // 0xC
    SCR_XY_uc mSpeed; // 0x18
    u16 mPadding1A; // 0x1A
};

struct SCR_DESTRUCTOR : SCR_CMD_HEADER
{
    u16 mPointerLo; // 0x8
    u16 mPointerHi; // 0xA
    SCR_Rect_f mRect; // 0xC
};

struct SCR_CRUSHER_BASIC : SCR_CMD_HEADER
{
    u16 mPointerLo; // 0x8
    u16 mPointerHi; // 0xA
    SCR_XY_f mPos; // 0xC
};

struct SCR_SET_AMBIENT : SCR_CMD_HEADER
{
    s32 mLevel; // 0x8
    u16 mTime; // 0xC
    u16 mPaddingE; // 0xE
};

struct SCR_IF_JUMP : SCR_CMD_HEADER
{
    u8 is_or;
    u8 padding;
    u16 else_endif_pointer;
};

struct SCR_DOOR_DATA_DEC : SCR_CMD_HEADER
{
    Door* mDoor; // 0x8
    SCR_XYZ_uc mBlock; // 0xC
    u8 mFace; // 0xF
    u8 mGrId; // 0x10
    u8 mOpenType; // 0x11: SCR_DOOR_OPENTYPES
    u8 mCloseType; // 0x12: SCR_DOOR_CLOSETYPES
    u8 mDelay; // 0x13
    SCR_Rect_f mCheck; // 0x14
    u8 mFlip; // 0x28
    u8 mReversed; // 0x29
    u16 mOpenTarget; // 0x2A: ONE_CAR/ONE_CHAR_ON_FOOT: script car/char index, ONE_MODEL: car model
};

struct SCR_UPDATE_DOOR : SCR_CMD_HEADER
{
    u16 mDoorIdx; // 0x8
    u16 mTargetIdx; // 0xA
};

struct SCR_SET_CHAR_GRAPHIC_TYPE : SCR_CMD_HEADER
{
    u16 mVarname; // 0x8
    s16 mRemap; // 0xA
    u16 mGraphicType; // 0xC
    u16 mPaddingE; // 0xE
};

struct SCR_LEVEL_END_ARROW2 : SCR_CMD_HEADER
{
    Fix16_Vec mPos; // 0x8
    u16 mArrowname; // 0x14
    u16 mPadding16; // 0x16
};

struct SCR_IS_CAR_IN_BLOCK : SCR_CMD_HEADER
{
    u16 mVarname; // 0x8
    u16 mUnusedA; // 0xA
    Fix16_Vec mPos; // 0xC
};

struct SCR_CHECK_SCORE_GREATER : SCR_CMD_HEADER
{
    u32 mCharIdx; // 0x8
    s32 mTargetScore; // 0xC
};

struct SCR_WARP_FROM_CAR : SCR_CMD_HEADER
{
    u16 mCharIdx; // 0x8
    Ang16 mRotation; // 0xA
    Fix16_Vec mPos; // 0xC
};

struct SCR_SET_GROUP_TYPE : SCR_CMD_HEADER
{
    u32 mCharIdx; // 0x8
    u32 mGroupType; // 0xC
};

struct SCR_CHECK_CURRENT_WEAPON : SCR_CMD_HEADER
{
    u16 mPedIdx; // 0x8
    u16 mPaddingA; // 0xA
    u32 mWeaponIdx; // 0xC
};

struct SCR_DECLARE_POLICELEVEL : SCR_CMD_HEADER
{
    u16 mUnused8; // 0x8
    u16 mWantedLevel; // 0xA
};

struct SCR_ADD_ONSCREEN_COUNTER : SCR_CMD_HEADER
{
    u16 mOnscreenCounterIdx; // 0x8
    u16 mCounterIdx; // 0xA
};

struct SCR_SET_KF_WEAPON : SCR_CMD_HEADER
{
    u16 mPedIdx; // 0x8
    u16 mUnusedA; // 0xA
    u32 mWeaponIdx; // 0xC
};

struct SCR_SET_FAV_CAR : SCR_CMD_HEADER
{
    u16 mPedIdx; // 0x8
    u16 mUnusedA; // 0xA
    u32 mFavouriteCar; // 0xC
};

struct SCR_SET_PLAYER_CONTROLS : SCR_CMD_HEADER
{
    u16 mPedIdx; // 0x8
    u8 mNewState; // 0xA
};

struct SCR_STORE_BONUS_COUNT : SCR_CMD_HEADER
{
    u16 mBonus; // 0x8
    u16 mCounterIdx; // 0xA
};

struct SCR_SET_STATE : SCR_CMD_HEADER
{
    u16 mVarname; // 0x8
    u8 mStatus; // 0xA
};

struct SCR_SET_DIR_OF_TVVAN : SCR_CMD_HEADER
{
    SCR_XY_f mPos; // 0x8
};

struct SCR_CHECK_WEAPONHIT : SCR_CMD_HEADER
{
    union
    {
        u16 mCarIdx; // 0x8
        u16 mPedIdx; // 0x8
    };
    u16 mStatus; // 0xA
    u16 mWeapon; // 0xC
};

struct SCR_FINISH_LEVEL : SCR_CMD_HEADER
{
    u16 mUnused8; // 0x8
    s16 mBonusType; // 0xA
};

struct SCR_CHECK_HEADS_GREATER : SCR_CMD_HEADER
{
    u16 mPedIdx; // 0x8
    s16 mWantedLevel; // 0xA
};

struct SCR_SET_CAR_GRAPHIC : SCR_CMD_HEADER
{
    u16 mCarIdx; // 0x8
    u16 mUnusedA; // 0xA
    u16 mNumber; // 0xC
};

struct SCR_CHANGE_BLOCK : SCR_CMD_HEADER
{
    u8 mX; // 0x8
    u8 mY; // 0x9
    u8 mZ; // 0xA
    u8 mChangeType; // 0xB
    u16 mInfoWord; // 0xC
};

struct SCR_CHECK_CAR_SPEED : SCR_CMD_HEADER
{
    u16 mCarIdx; // 0x8
    s32 mValue; // 0xA
};

struct SCR_MAP_SET_ZONE_INFO : SCR_CMD_HEADER
{
    u16 mZoneIdx; // 0x8
    u16 mUnusedA; // 0xA
    u16 mValue; // 0xC
};

struct SCR_START_BASIC_KF_TEMPLATE : SCR_CMD_HEADER
{
    u16 mTriggername; // 0x8
    u16 mBriefId; // 0xA
    u16 mObjname; // 0xC
    u16 mPlayername; // 0xE
    u16 mWeapon; // 0x10
    u16 mUnused12; // 0x12
};

struct SCR_DO_BASIC_KF_TEMPLATE : SCR_CMD_HEADER
{
    u16 mBonusname; // 0x8
    u16 mTimername; // 0xA
    u32 mTimeLimit; // 0xC
    u16 mOnscreenname; // 0x10
    u16 mCountername; // 0x12
    u16 mTargetTotal; // 0x14
    u16 mBriefId; // 0x16
    u16 mPlayername; // 0x18
    u16 mRewardtype; // 0x1A
    u32 mRewardvalue; // 0x1C
};

struct SCR_SET_CAR_JAMMED : SCR_CMD_HEADER
{
    u16 mCarIdx; // 0x8
    u8 mStatus; // 0xA
};

struct SCR_SET_CAR_DAMAGE_PROOF : SCR_CMD_HEADER
{
    u16 mCarIdx; // 0x8
    u8 mStatus; // 0xA
};

struct SCR_SET_RUN_SPEED : SCR_CMD_HEADER
{
    u16 mPedIdx; // 0x8
    Fix16 mValue; // 0xA
};

struct SCR_SET_CHAR_DRIVE_AGGR : SCR_CMD_HEADER
{
    u16 mPedIdx; // 0x8
    u8 mStatus; // 0xA
};

struct SCR_OPERATE_COUNTER_AND_OPERAND_INT : SCR_CMD_HEADER
{
    u16 mDestinationCounterIdx; // 0x8
    u16 mOperandCounterIdx; // 0xA
    s16 mValue; // 0xC
};

struct SCR_OPERATE_COUNTER_AND_COUNTER : SCR_CMD_HEADER
{
    u16 mDestinationCounterIdx; // 0x8
    u16 mFirstOperandCounterIdx; // 0xA
    u16 mSecondOperandCounterIdx; // 0xC
};

struct SCR_PUT_CAR_ON_TRAILER : SCR_CMD_HEADER
{
    u16 mCarIdx; // 0x8
    u16 mTrailerCarIdx; // 0xA
};

struct SCR_CREATE_LIGHT : SCR_CMD_HEADER
{
    u16 mLightIdx; // 0x8
    u16 mUnusedA; // 0xA
    Fix16 mXpos; // 0xC
    Fix16 mYpos; // 0x10
    Fix16 mZpos; // 0x14
    s32 mArgb; // 0x18
    Fix16 mRadius; // 0x1C
    u8 mIntensity; // 0x20
    u8 mOnTime; // 0x21
    u8 mOffTime; // 0x22
    u8 mShape; // 0x23
};

struct SCR_ANSWER_PHONE : SCR_CMD_HEADER
{
    u16 mObjIdx; // 0x8
    u16 mUnusedA; // 0xA
    u16 mPedIdx; // 0xC
    u16 mUnusedE; // 0xE
    s16 mTimer; // 0x10
    u8 mAnswered; // 0x12
};

struct SCR_ADD_PATROL_POINT : SCR_CMD_HEADER
{
    u16 mVarname; // 0x8
    u16 mUnusedA; // 0xA
    Fix16_Vec mPos; // 0xC
};

struct SCR_CHAR_ENTER_CAR : SCR_CMD_HEADER
{
    u16 mPedIdx; // 0x8
    u16 mCarIdx; // 0xA
    Fix16_Vec mPos; // 0xC
};

struct SCR_GET_SCORE : SCR_CMD_HEADER
{
    u16 mPedIdx; // 0x8
    u16 mCounterIdx; // 0xA
};

struct SCR_IS_CHAR_IN_GANG_ZONE : SCR_CMD_HEADER
{
    u16 mPedIdx; // 0x8
    u16 mGangIdx; // 0xA
};

struct SCR_CHANGE_CAR_LOCK : SCR_CMD_HEADER
{
    u16 mCarIdx; // 0x8
    s16 mLockType; // 0xA
};

struct SCR_IS_POINT_ON_SCREEN : SCR_CMD_HEADER
{
    Fix16 mXpos; // 0x8
    Fix16 mYpos; // 0xC
};

struct SCR_GET_NUM_LIVES_MULTIPLIERS : SCR_CMD_HEADER
{
    u16 mPlayerPedIdx; // 0x8
    s16 mCounterIdx; // 0xA
};

struct SCR_REMOTE_CONTROL : SCR_CMD_HEADER
{
    u16 mPlayerPedIdx; // 0x8
    u16 mCarIdx; // 0xA
};

struct SCR_SWITCH_ROAD_ON_OFF : SCR_CMD_HEADER
{
    u8 mXpos; // 0x8
    u8 mYpos; // 0x9
    u8 mZpos; // 0xA
    u8 mStatus; // 0xB
};

struct SCR_CHANGE_LIGHT_INTENSITY : SCR_CMD_HEADER
{
    u16 mLightIdx; // 0x8
    u16 mIntensity; // 0xA
};

struct SCR_CHANGE_LIGHT_RADIUS : SCR_CMD_HEADER
{
    u16 mLightIdx; // 0x8
    Fix16 mRadius; // 0xA
};

struct SCR_DO_SAVE_GAME : SCR_CMD_HEADER
{
    u16 mTriggername; // 0x8
    u16 mUnusedA; // 0xA
    SCR_Rect_f mRect; // 0xC
};

struct SCR_DELETE_SOUND : SCR_CMD_HEADER
{
    Fix16 mXpos; // 0x8
    Fix16 mYpos; // 0xC
};

struct SCR_OPERATE_COUNTER_AND_COUNTER_2 : SCR_CMD_HEADER
{
    u16 mLeftCounterIdx; // 0x8
    u16 mRightCounterIdx; // 0xA
};

struct SCR_OPERATE_COUNTER_AND_INT : SCR_CMD_HEADER
{
    u16 mCounterIdx; // 0x8
    s16 mValue; // 0xA
};

struct SCR_OPERATE_INT_AND_COUNTER : SCR_CMD_HEADER
{
    s16 mValue; // 0x8
    u16 mCounterIdx; // 0xA
};

struct SCR_POINT_ARROW_AT : SCR_CMD_HEADER
{
    u16 mTargetEntityIdx; // 0x8
    u16 mUnusedA; // 0xA
    u16 mArrowIdx; // 0xC
};

struct SCR_ARROW_ENTITY : SCR_CMD_HEADER
{
    union
    {
        Car* mCar; // 0x8
        Ped* mChar; // 0x8
        Object* mObj; // 0x8
    };
};

struct SCR_ARROW_XYZ : SCR_CMD_HEADER
{
    Fix16_Vec mCoord; // 0x8
};

struct SCR_SET_COUNTER : SCR_CMD_HEADER
{
    u16 mCounterIdx; // 0x8
    s16 mValue; // 0xA
};

namespace SCR_DOOR_OPENTYPES
{
enum
{
    any_player = 0,
    any_car = 1,
    one_car = 2,
    one_model = 3,
    one_char_on_foot = 4,
    none = 5,
    any_player_one_car = 6,
};
} // namespace SCR_DOOR_OPENTYPES

namespace SCR_DOOR_CLOSETYPES
{
enum
{
    close_never = 0,
    close_time_delay = 1,
    close_when_clear = 2,
    close_when_open_rule_fails = 3,
};
} // namespace SCR_DOOR_CLOSETYPES

namespace SCR_BONUSES
{
enum
{
    NO_BONUS = 0,
    BONUS_1 = 1,
    BONUS_2 = 2,
    BONUS_3 = 3,
};
} // namespace SCR_BONUSES

EXTERN_GLOBAL(Fix16, kFpHalf_6F75F0);
EXTERN_GLOBAL(Fix16, kFpZero_6F77C0);
EXTERN_GLOBAL(Fix16, kFpOne_6F77C4);
EXTERN_GLOBAL(Fix16, kFpTwo_6F77C8);

class ScriptThread
{
  public:
    STATIC_MEMBER(Fix16, kFp255);
    EXPORT void MissionFailOnDeathOrArrest();
    EXPORT u8 GetEntityTypeOfCommand(u32 opcode);
    EXPORT void PoolAllocate();
    EXPORT void Log();
    EXPORT void StopThread();
    EXPORT void Next(SCR_CMD_HEADER* pCmd);
    EXPORT void JumpToCmd(u16 cmdIdx);
    EXPORT void EndCmd();
    EXPORT void SCRCMD_OBJ_DECSET_2D_3D(SCR_OBJ_DATA* pCmd, SCR_POINTER* pPointer);
    EXPORT void SCRCMD_OBJ_DECSET(SCR_OBJ_DATA* pCmd, SCR_POINTER* pPointer);
    EXPORT void Handle_PLAYER_PED(SCR_PLAYER_PED* pCmd);
    EXPORT void SCRCMD_CAR_DECSET(SCR_CAR_DATA_DEC* pCmd, SCR_POINTER* pPointer);
    EXPORT void SCRCMD_PARKED_CAR_DECSET(SCR_POINTER* pCmd);
    EXPORT void SCRCMD_CHAR_DECSET_2D_3D(SCR_CHAR_DATA_DEC* pCmd, SCR_POINTER* pPointer);
    EXPORT void SetCharObjective(SCR_CHAR_OBJECTIVE* pCmd, SCR_POINTER* pPointer);
    EXPORT void Handle_MAP_ZONE_SET(SCR_MAP_ZONE_SET* pCmd);
    EXPORT void Handle_ARROW_DEC(SCR_TWO_PARAMS* pCmd);
    EXPORT void SCRCMD_CRANE(SCR_CRANE_TARGET_DEC* pTargetCmd, SCR_CRANE_BASIC_DEC* pBasicCmd);
    EXPORT void SCRCMD_CONVEYOR_DECSET1_2(SCR_CONVEYOR* pCmd, SCR_POINTER* pPointer);
    EXPORT void SCRCMD_GENERATOR_DECSET(SCR_GENERATOR* pCmd, SCR_POINTER* pPointer);
    EXPORT void SCRCMD_DESTRUCTOR_DECSET(SCR_DESTRUCTOR* pCmd, SCR_POINTER* pPointer);
    EXPORT void Handle_CRUSHER_BASIC(SCR_CRUSHER_BASIC* pCmd, SCR_POINTER* pPointer);
    EXPORT void Handle_THREAD_DECLARE2(SCR_THREAD* pCmd, s16* pThreadIdx);
    EXPORT void Handle_THREAD_DECLARE3(SCR_THREAD* pCmd);
    EXPORT void Handle_THREAD_DECLARE5(SCR_CHAR_AREA_ANY* pCmd);
    EXPORT void Handle_THREAD_DECLARE4(SCR_THREAD* pCmd, s16* pThreadIdx);
    EXPORT void ApplyGangInfo(SCR_SET_GANG_INFO* pCmd);
    EXPORT void Handle_SET_DOOR_INFO(SCR_FOUR_PARAMS* pCmd);
    EXPORT void SCRCMD_DOOR_DECLARE_D1_S1(SCR_DOOR_DATA_DEC* pCmd);
    EXPORT void SCRCMD_DOOR_DECLARE_D2_S2(SCR_DOOR_DATA_DEC* pCmd);
    EXPORT void Handle_DECLARE_MISSION(SCR_TWO_PARAMS* pCmd);
    EXPORT void CreateLight(SCR_CREATE_LIGHT* pCmd, SCR_POINTER* pPointer);
    EXPORT void SCRCMD_SET_STATION_EMPTY_STATION(SCR_SET_STATION* pCmd);
    EXPORT void Handle_RADIOSTATION_DEC(SCR_RADIOSTATION_DEC* pCmd);
    EXPORT void SCRCMD_SET_TRAIN_STATIONS(SCR_TWO_PARAMS* pCmd);
    EXPORT void Handle_DECLARE_POLICE(SCR_DECLARE_POLICELEVEL* pCmd);
    EXPORT void Handle_SOUND_DECSET(SCR_SOUND_DECSET* pCmd, SCR_POINTER* pPointer);
    EXPORT void SCRCMD_SET_MISSIONS_TOTAL(SCR_TWO_PARAMS* pCmd);
    EXPORT void SCRCMD_PASSED_FAILED_FLAGS(SCR_TWO_PARAMS* pCmd);
    EXPORT void Handle_FINISH_SCORE(SCR_TWO_PARAMS* pCmd);
    EXPORT void Handle_DECLARE_CARLIST(SCR_TWO_PARAMS* pCmd);
    EXPORT void DisableThread(u16 idx);
    EXPORT void DeallocOrDeleteItem(u16 idx);
    EXPORT s16 GetBonusResult(u16 idx);
    EXPORT void SCRCMD_DISPLAY_MESSAGES(SCR_TWO_PARAMS* pCmd);
    EXPORT void SCRCMD_DEC_DEATH_BASE(SCR_FOUR_PARAMS* pCmd);
    EXPORT void Handle_DO_CRANE_POWERUP(SCR_DECLARE_CRANE_POWERUP* pCmd);
    EXPORT void RegisterBasicKf(u16* pKfValue);
    EXPORT void ExecOpCode();
    EXPORT void Handle_LEVELSTART();
    EXPORT void Handle_LEVELEND();
    EXPORT void Handle_CREATE_THREAD();
    EXPORT void Handle_STOP_THREAD();
    EXPORT void Handle_IF_JUMP();
    EXPORT void Handle_GOSUB();
    EXPORT void Handle_RETURN();
    EXPORT u8 GetOperationType(u32 opcode);
    EXPORT void SCRCMD_S_OP_I();
    EXPORT void SCRCMD_S_OP_S();
    EXPORT void SCRCMD_I_OP_S();
    EXPORT void Handle_NOT();
    EXPORT void Handle_START_EXEC();
    EXPORT void Handle_STOP_EXEC();
    EXPORT void Handle_INCREMENT();
    EXPORT void Handle_DECREMENT();
    EXPORT void SCRCMD_CREATE_OBJ();
    EXPORT void SCRCMD_CREATE_CAR();
    EXPORT void Handle_MAKE_CAR_DUMMY();
    EXPORT void SCRCMD_SET_DENSITY();
    EXPORT void Handle_POINT_ARROW_3D();
    EXPORT void Handle_POINT_ARROW_AT();
    EXPORT void Handle_ARROW_COLOUR();
    EXPORT void Handle_REMOVE_ARROW();
    EXPORT void Handle_CHECK_CAR_DAMAGE();
    EXPORT void Handle_CHECK_HEALTH();
    EXPORT void Handle_STORE_CAR_INFO();
    EXPORT void Handle_DISPLAY_MESSAGE();
    EXPORT void Handle_DISPLAY_TIMER();
    EXPORT void SCRCMD_CREATE_CHAR();
    EXPORT void Handle_SET_CHAR_OCCUPATION();
    EXPORT void Handle_SET_CHAR_OBJ1();
    EXPORT void Handle_IS_CHAR_OBJ_PASS();
    EXPORT void Handle_IS_CHAR_OBJ_FAIL();
    EXPORT void SCRCMD_GIVE_WEAPON();
    EXPORT void Handle_IS_CAR_IN_BLOCK();
    EXPORT void Handle_CAR_IN_AREA();
    EXPORT void Handle_HAS_CHAR_DIED();
    EXPORT void Handle_IS_CHAR_IN_CAR();
    EXPORT void Handle_IS_CHAR_IN_MODEL();
    EXPORT void Handle_IS_CHAR_IN_ANY_CAR();
    EXPORT void Handle_DELETE_ITEM();
    EXPORT void SCRCMD_ADD_SCORE();
    EXPORT void Handle_ADD_SCORE2();
    EXPORT void Handle_IS_CHAR_STOPPED();
    EXPORT void Handle_EXPLODE();
    EXPORT void Handle_EXPLODE_BUILDING();
    EXPORT void Locate();
    EXPORT void Handle_SET_CHAR_OBJ2();
    EXPORT void Handle_SET_CHAR_OBJ3();
    EXPORT void Handle_SET_CHAR_OBJ_FOLLOW();
    EXPORT void Handle_CHANGE_CAR_REMAP();
    EXPORT void Handle_CHANGE_CHAR_REMAP();
    EXPORT void Handle_CHECK_CAR_MODEL();
    EXPORT void Handle_CHECK_CAR_REMAP();
    EXPORT void Handle_CHECK_CAR_BOTH();
    EXPORT void IsOnScreen();
    EXPORT void Handle_DELAY_HERE();
    EXPORT void Handle_DELAY();
    EXPORT void EnableThread(u16 idx);
    EXPORT void Handle_DISABLE_THREAD();
    EXPORT void Handle_ENABLE_THREAD();
    EXPORT void SCRCMD_SET_GANG_KILL_REACTION();
    EXPORT void Handle_CHANGE_RESPECT();
    EXPORT void RespectOperator();
    EXPORT void Handle_ADD_PATROL_POINT();
    EXPORT void Handle_SET_AMBIENT();
    EXPORT void Handle_ANSWER_PHONE();
    EXPORT void Handle_CHECK_PHONE();
    EXPORT void Handle_CHECK_PHONETIMER();
    EXPORT void Handle_STOP_PHONE_RING();
    EXPORT void Handle_IS_CHAR_FIRE_ONSCREEN();
    EXPORT void Handle_CAR_DRIVE_AWAY();
    EXPORT void Handle_GET_CAR_FROM_CRANE();
    EXPORT void Handle_CHAR_TO_DRIVE_CAR();
    EXPORT void Handle_DRIVER_OUT_CAR();
    EXPORT void Handle_GIVE_DRIVER_BRAKE();
    EXPORT void Handle_SET_GANG_INFO1();
    EXPORT void Handle_CLEAR_TIMERS();
    EXPORT void Handle_CHECK_SCORE();
    EXPORT void Handle_GET_SCORE();
    EXPORT void Handle_IS_CHAR_IN_GANG();
    EXPORT void Handle_SET_NO_COLLIDE();
    EXPORT void Handle_IS_CHAR_FIRING_AREA();
    EXPORT void Handle_GET_PASSENGER_NUM();
    EXPORT void Handle_CLEAR_WANTED_LEVEL();
    EXPORT void Handle_ALT_WANTED_LEVEL();
    EXPORT void Handle_CAR_WRECK_IN_LOCATION();
    EXPORT void Handle_SEND_CAR_TO_BLOCK();
    EXPORT void SCRCMD_THREAT_SEARCH_REACT_ADD_GROUP();
    EXPORT void Handle_CHECK_NUM_ALIVE();
    EXPORT void Handle_SET_MIN_ALIVE();
    EXPORT void SCRCMD_CHAR_GROUP();
    EXPORT void SCRCMD_SET_COUNTER();
    EXPORT void Handle_HAS_CAR_WEAPON();
    EXPORT void Handle_IS_CHAR_HORN();
    EXPORT void Handle_CHECK_MAX_PASS();
    EXPORT void Handle_IS_CHAR_IN_ZONE();
    EXPORT void Handle_SET_PHONE_DEAD();
    EXPORT void Handle_IS_TRAILER_ATT();
    EXPORT void Handle_IS_CAR_ON_TRAIL();
    EXPORT void SCRCMD_ENABLE_DISABLE_CRANE();
    EXPORT void Handle_CAR_GOT_DRIVER();
    EXPORT void Handle_SPOTTED_PLAYER();
    EXPORT void Handle_GET_LAST_PUNCHED();
    EXPORT void Handle_IS_CHAR_STUNNED();
    EXPORT void Handle_KILL_ALL_PASSENG();
    EXPORT void Handle_IS_GROUP_IN_CAR();
    EXPORT void Handle_PUNCHED_SOMEONE();
    EXPORT void Handle_ADD_CHAR_TO_GANG();
    EXPORT void Handle_EXPLODE_ITEM();
    EXPORT void Handle_PARK();
    EXPORT void Handle_BEEN_PUNCHED_BY();
    EXPORT void Handle_UPDATE_DOOR();
    EXPORT void SCRCMD_DOOR();
    EXPORT void Handle_REMOVE_WEAPON();
    EXPORT void Handle_REMOVE_BLOCK();
    EXPORT void Handle_LOWER_LEVEL();
    EXPORT void SCRCMD_CHANGE_BLOCK();
    EXPORT void Handle_ADD_NEW_BLOCK();
    EXPORT void Handle_ROAD_ON_OFF();
    EXPORT void Handle_CHECK_CAR_DRIVER();
    EXPORT void Handle_SWITCH_GENERATOR1();
    EXPORT void Handle_SWITCH_GENERATOR3();
    EXPORT void Handle_CAR_DAMAGE_POS();
    EXPORT void Handle_PARK_FINISHED();
    EXPORT void Gosub(SCR_CMD_HEADER* pCmd, u16 cmd_idx);
    EXPORT void Handle_PHONE_TEMPLATE();
    EXPORT void Handle_REMOTE_CONTROL();
    EXPORT void Handle_LAUNCH_MISSION();
    EXPORT void Handle_SAVE_GAME();
    EXPORT void Handle_DO_SAVE_GAME();
    EXPORT void Handle_CHANGE_CAR_LOCK();
    EXPORT void Handle_CHANGE_INTENSITY();
    EXPORT void Handle_CHANGE_COLOUR();
    EXPORT void Handle_CHANGE_RADIUS();
    EXPORT void SCRCMD_CREATE_LIGHT();
    EXPORT void GetSetLivesAndMultiplier();
    EXPORT void SCRCMD_GET_LIVES_MULT();
    EXPORT void Handle_SET_DIR_OF_TVVAN();
    EXPORT void Handle_POINT_ONSCREEN();
    EXPORT void Handle_CHAR_IN_AIR();
    EXPORT void Handle_CHAR_SUNK();
    EXPORT void StartBonus();
    EXPORT void SCRCMD_CHECK_BONUS();
    EXPORT void Handle_SETUP_MODEL_CHECK();
    EXPORT void Handle_MODEL_CHECK();
    EXPORT void GetSpeed();
    EXPORT void Handle_CHECK_CAR_SPEED();
    EXPORT void Handle_SET_CAR_GRAPHIC();
    EXPORT void Handle_CHAR_ARRESTED();
    EXPORT void SCRCMD_S_IS_S_OP_S();
    EXPORT void SCRCMD_S_IS_S_OP_I();
    EXPORT void Handle_CHAR_DRIVE_AGGR();
    EXPORT void SCRCMD_SET_SPEED();
    EXPORT void Handle_GIVE_CAR_ALARM();
    EXPORT void SCRCMD_CAR_WEAPON_KIND_PROOF();
    EXPORT void Handle_PUT_CAR_ON_TRAILER();
    EXPORT void Handle_CLEAR_BRIEFS();
    EXPORT void Handle_CHECK_HEADS();
    EXPORT void Handle_FINISH_LEVEL();
    EXPORT void Handle_CHECK_WEAPONHIT();
    EXPORT void Handle_WEAP_HIT_CAR();
    EXPORT void Handle_IS_CHAR_ON_FIRE();
    EXPORT void Handle_BRIEF_ONSCREEN();
    EXPORT void Handle_CREATE_SOUND();
    EXPORT void Handle_DO_EASY_PHONE();
    EXPORT void Handle_CHAR_INTO_CAR();
    EXPORT void Handle_DECIDE_POWERUP();
    EXPORT void Handle_SUPPRESS_MODEL();
    EXPORT void Handle_WARP_CHAR();
    EXPORT void Handle_SET_GROUP_TYPE();
    EXPORT void Handle_CHAR_DO_NOTHING();
    EXPORT void Handle_EMERG_LIGHTS();
    EXPORT void Handle_CHECK_OBJ_MODEL();
    EXPORT void Handle_PED_GRAPHIC();
    EXPORT void SCRCMD_SET_CHAR_FLAGS();
    EXPORT void Handle_MAKE_MUGGERS();
    EXPORT void LocSecChar();
    EXPORT void Handle_STOP_CAR_DRIVE();
    EXPORT void Handle_IS_BUS_FULL();
    EXPORT void Handle_NO_CHARS_OFF_BUS();
    EXPORT void Handle_KILL_CHAR();
    EXPORT void Handle_SET_SHADING_LEV();
    EXPORT void Handle_SET_CAR_JAMMED();
    EXPORT void Handle_FINISH_MISSION();
    EXPORT void Handle_STORE_BONUS();
    EXPORT void Handle_SET_ENTER_STATUS();
    EXPORT void Handle_SET_FAV_CAR();
    EXPORT void GroupInArea();
    EXPORT void Handle_SET_KF_WEAPON();
    EXPORT void Handle_CLEAR_KF_WEAPON();
    EXPORT void Handle_ADD_ONSCREEN_COUNTER();
    EXPORT void Handle_CLEAR_COUNTER();
    EXPORT void Handle_CHANGE_POLICE();
    EXPORT void Handle_DESTROY_GROUP();
    EXPORT void Handle_CHECK_CURRENT_WEAPON();
    EXPORT void Handle_START_BASIC_KF();
    EXPORT void Handle_DO_BASIC_KF();
    EXPORT void Handle_SET_BONUS_RATING();
    EXPORT void Handle_ADD_TIME();
    EXPORT void Handle_SET_MODEL_WANTED();
    EXPORT void Handle_CHECK_DEATH_ARR();
    EXPORT void Handle_FORCE_CLEANUP();
    EXPORT void SCRCMD_SAVE_RESTORE_RESPECT();
    EXPORT void PreExecOpCode();
    EXPORT bool PoolUpdate();
    EXPORT void InitThread(char_type flags, u16 start_cmd);
    EXPORT ScriptThread* SpawnThread(u16 start_cmd);
    EXPORT void launch_mission(SCR_CMD_HEADER* pCmd, const char_type* pMissionName);
    EXPORT ScriptThread();
    EXPORT void DisableBasicKfsFromSave();
    EXPORT ~ScriptThread();

    ScriptThread* mpNext;
    u16 mNextCmd; // 0x4
    char_type mFlags; // 0x6
    char_type mUnused7; // 0x7
    s32 mCondResult; // 0x8
    char_type mCmdInitialised; // 0xC
    char_type mUnusedD; // 0xD
    s16 mPhoneTimer; // 0xE
    char_type mStopped; // 0x10
    char_type mUnused11; // 0x11
    s16 mExecFlag; // 0x12
    char_type mStr[255]; // 0x14
    char_type mUnused113; // 0x113
    ScriptGosubStack* mGosubStack; // 0x114
    bool mInLevelStart; // 0x118
    char_type mUnused119; // 0x119
    s16 mThreadId; // 0x11A
};
GTA2_ASSERT_SIZEOF_ALWAYS(ScriptThread, 0x11C)

class ScriptThreadPool
{
  public:
    // 9.6f 0x4767A0
    ScriptThread* Allocate_4767A0()
    {
        return mPool.Allocate();
    }

    // 9.6f inlined ctor 0x481310
    ScriptThreadPool()
    {
    }

    void LogAll_47F4D0()
    {
        for (ScriptThread* pThread = mPool.mpPrev; pThread; pThread = pThread->mpNext)
        {
            pThread->Log();
        }
    }

    EXPORT ~ScriptThreadPool();

    Pool<ScriptThread, 8> mPool;
};

EXTERN_GLOBAL(ScriptThreadPool*, miss2_0x11C_Pool);