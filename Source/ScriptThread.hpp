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
    EXPORT void MissionFailOnDeathOrArrest_503200();
    EXPORT u8 GetEntityTypeOfCommand_503410(u32 opcode);
    EXPORT void PoolAllocate();
    EXPORT void Log_5035D0();
    EXPORT void StopThread_503610();
    EXPORT void Next_503620(SCR_CMD_HEADER* pCmd);
    EXPORT void JumpToCmd_503650(u16 cmdIdx);
    EXPORT void EndCmd_503670();
    EXPORT void SCRCMD_OBJ_DECSET_2D_3D_503680(SCR_OBJ_DATA* pCmd, SCR_POINTER* pPointer);
    EXPORT void SCRCMD_OBJ_DECSET_5038D0(SCR_OBJ_DATA* pCmd, SCR_POINTER* pPointer);
    EXPORT void SCRCMD_PLAYER_PED_503A20(SCR_PLAYER_PED* pCmd);
    EXPORT void SCRCMD_CAR_DECSET_503BC0(SCR_CAR_DATA_DEC* pCmd, SCR_POINTER* pPointer);
    EXPORT void SCRCMD_PARKED_CAR_DECSET_503F80(SCR_POINTER* pCmd);
    EXPORT void SCRCMD_CHAR_DECSET_2D_3D_503FB0(SCR_CHAR_DATA_DEC* pCmd, SCR_POINTER* pPointer);
    EXPORT void SetCharObjective_504110(SCR_CHAR_OBJECTIVE* pCmd, SCR_POINTER* pPointer);
    EXPORT void SCRCMD_MAP_ZONE_SET_504150(SCR_MAP_ZONE_SET* pCmd);
    EXPORT void SCRCMD_ARROW_DEC_5041B0(SCR_TWO_PARAMS* pCmd);
    EXPORT void SCRCMD_CRANE_5041C0(SCR_CRANE_TARGET_DEC* pTargetCmd, SCR_CRANE_BASIC_DEC* pBasicCmd);
    EXPORT void SCRCMD_CONVEYOR_DECSET1_2_5043A0(SCR_CONVEYOR* pCmd, SCR_POINTER* pPointer);
    EXPORT void SCRCMD_GENERATOR_DECSET_504420(SCR_GENERATOR* pCmd, SCR_POINTER* pPointer);
    EXPORT void SCRCMD_DESTRUCTOR_DECSET_504530(SCR_DESTRUCTOR* pCmd, SCR_POINTER* pPointer);
    EXPORT void SCRCMD_CRUSHER_BASIC_5045A0(SCR_CRUSHER_BASIC* pCmd, SCR_POINTER* pPointer);
    EXPORT void SCRCMD_THREAD_DECLARE2_5045D0(SCR_THREAD* pCmd, s16* pThreadIdx);
    EXPORT void SCRCMD_THREAD_DECLARE3_504660(SCR_THREAD* pCmd);
    EXPORT void SCRCMD_THREAD_DECLARE5_504710(SCR_CHAR_AREA_ANY* pCmd);
    EXPORT void SCRCMD_THREAD_DECLARE4_5047C0(SCR_THREAD* pCmd, s16* pThreadIdx);
    EXPORT void SCRCMD_SET_GANG_INFO1_504830(SCR_SET_GANG_INFO* pCmd);
    EXPORT void SCRCMD_SET_DOOR_INFO_504950(SCR_FOUR_PARAMS* pCmd);
    EXPORT void SCRCMD_DOOR_DECLARE_D1_S1_504970(SCR_DOOR_DATA_DEC* pCmd);
    EXPORT void SCRCMD_DOOR_DECLARE_D2_S2_504B80(SCR_DOOR_DATA_DEC* pCmd);
    EXPORT void SCRCMD_DECLARE_MISSION_504DD0(SCR_TWO_PARAMS* pCmd);
    EXPORT void CreateLight_504EE0(SCR_CREATE_LIGHT* pCmd, SCR_POINTER* pPointer);
    EXPORT void SCRCMD_SET_STATION_EMPTY_STATION_505030(SCR_SET_STATION* pCmd);
    EXPORT void SCRCMD_RADIOSTATION_DEC_5051D0(SCR_RADIOSTATION_DEC* pCmd);
    EXPORT void SCRCMD_SET_TRAIN_STATIONS_505210(SCR_TWO_PARAMS* pCmd);
    EXPORT void SCRCMD_DECLARE_POLICE_5052C0(SCR_DECLARE_POLICELEVEL* pCmd);
    EXPORT void SCRCMD_SOUND_DECSET_505340(SCR_SOUND_DECSET* pCmd, SCR_POINTER* pPointer);
    EXPORT void SCRCMD_SET_MISSIONS_TOTAL_505430(SCR_TWO_PARAMS* pCmd);
    EXPORT void SCRCMD_PASSED_FAILED_FLAGS_505580(SCR_TWO_PARAMS* pCmd);
    EXPORT void SCRCMD_FINISH_SCORE_505710(SCR_TWO_PARAMS* pCmd);
    EXPORT void SCRCMD_DECLARE_CARLIST_505750(SCR_TWO_PARAMS* pCmd);
    EXPORT void DisableThread_505790(u16 idx);
    EXPORT void DeallocOrDeleteItem_505B10(u16 idx);
    EXPORT s16 GetBonusResult_505EA0(u16 idx);
    EXPORT void SCRCMD_DISPLAY_MESSAGES_505F50(SCR_TWO_PARAMS* pCmd);
    EXPORT void SCRCMD_DEC_DEATH_BASE_506010(SCR_FOUR_PARAMS* pCmd);
    EXPORT void SCRCMD_DO_CRANE_POWERUP_5060D0(SCR_DECLARE_CRANE_POWERUP* pCmd);
    EXPORT void SCRCMD_START_BASIC_KF_506140(u16* pKfValue);
    EXPORT void ExecOpCode_5061C0();
    EXPORT void SCRCMD_LEVELSTART_5069C0();
    EXPORT void SCRCMD_LEVELEND_5069F0();
    EXPORT void SCRCMD_CREATE_THREAD_506A00();
    EXPORT void SCRCMD_STOP_THREAD_506A60();
    EXPORT void SCRCMD_IF_JUMP_506AF0();
    EXPORT void SCRCMD_GOSUB_506B30();
    EXPORT void SCRCMD_RETURN_506B80();
    EXPORT u8 GetOperationType_506BC0(u32 opcode);
    EXPORT void SCRCMD_S_OP_I_506D60();
    EXPORT void SCRCMD_S_OP_S_506ED0();
    EXPORT void SCRCMD_I_OP_S_507110();
    EXPORT void SCRCMD_NOT_507750();
    EXPORT void SCRCMD_START_EXEC_5078D0();
    EXPORT void SCRCMD_STOP_EXEC_5079A0();
    EXPORT void SCRCMD_INCREMENT_507A70();
    EXPORT void SCRCMD_DECREMENT_507B50();
    EXPORT void SCRCMD_CREATE_OBJ_507CE0();
    EXPORT void SCRCMD_CREATE_CAR_507F80();
    EXPORT void SCRCMD_MAKE_CAR_DUMMY_508220();
    EXPORT void SCRCMD_SET_DENSITY_508280();
    EXPORT void SCRCMD_POINT_ARROW_3D_508550();
    EXPORT void SCRCMD_POINT_ARROW_AT_5086F0();
    EXPORT void SCRCMD_ARROW_COLOUR_508DC0();
    EXPORT void SCRCMD_REMOVE_ARROW_508E80();
    EXPORT void SCRCMD_CHECK_CAR_DAMAGE_508F00();
    EXPORT void SCRCMD_CHECK_HEALTH_509030();
    EXPORT void SCRCMD_STORE_CAR_INFO_509180();
    EXPORT void SCRCMD_DISPLAY_MESSAGE_5093C0();
    EXPORT void SCRCMD_DISPLAY_TIMER_5096E0();
    EXPORT void SCRCMD_CREATE_CHAR_509730();
    EXPORT void SCRCMD_SET_CHAR_OCCUPATION_509790();
    EXPORT void SCRCMD_SET_CHAR_OBJ1_5097D0();
    EXPORT void SCRCMD_IS_CHAR_OBJ_PASS_509810();
    EXPORT void SCRCMD_IS_CHAR_OBJ_FAIL_509880();
    EXPORT void SCRCMD_GIVE_WEAPON_5098E0();
    EXPORT void SCRCMD_IS_CAR_IN_BLOCK_509990();
    EXPORT void SCRCMD_CAR_IN_AREA_509A70();
    EXPORT void SCRCMD_HAS_CHAR_DIED_509BB0();
    EXPORT void SCRCMD_IS_CHAR_IN_CAR_509C10();
    EXPORT void SCRCMD_IS_CHAR_IN_MODEL_509C90();
    EXPORT void SCRCMD_IS_CHAR_IN_ANY_CAR_509D00();
    EXPORT void SCRCMD_DELETE_ITEM_509D60();
    EXPORT void SCRCMD_ADD_SCORE_509D90();
    EXPORT void SCRCMD_ADD_SCORE2_509E00();
    EXPORT void SCRCMD_IS_CHAR_STOPPED_509E70();
    EXPORT void SCRCMD_EXPLODE_509ED0();
    EXPORT void SCRCMD_EXPLODE_BUILDING_509F60();
    EXPORT void Locate_509FD0();
    EXPORT void SCRCMD_SET_CHAR_OBJ2_50A200();
    EXPORT void SCRCMD_SET_CHAR_OBJ3_50A3E0();
    EXPORT void SCRCMD_SET_CHAR_OBJ_FOLLOW_50A460();
    EXPORT void SCRCMD_CHANGE_CAR_REMAP_50A570();
    EXPORT void SCRCMD_CHANGE_CHAR_REMAP_50A5B0();
    EXPORT void SCRCMD_CHECK_CAR_MODEL_50A610();
    EXPORT void SCRCMD_CHECK_CAR_REMAP_50A670();
    EXPORT void SCRCMD_CHECK_CAR_BOTH_50A6E0();
    EXPORT void IsOnScreen_50A760();
    EXPORT void SCRCMD_DELAY_HERE_50A940();
    EXPORT void SCRCMD_DELAY_50A980();
    EXPORT void EnableThread_50A9E0(u16 idx);
    EXPORT void SCRCMD_DISABLE_THREAD_50ABC0();
    EXPORT void SCRCMD_ENABLE_THREAD_50ABF0();
    EXPORT void SCRCMD_SET_GANG_KILL_REACTION_50AC20();
    EXPORT void SCRCMD_CHANGE_RESPECT_50ACF0();
    EXPORT void RespectOperator_50AEF0();
    EXPORT void SCRCMD_ADD_PATROL_POINT_50B0E0();
    EXPORT void SCRCMD_SET_AMBIENT_50B150();
    EXPORT void SCRCMD_ANSWER_PHONE_50B180();
    EXPORT void SCRCMD_CHECK_PHONE_50B230();
    EXPORT void SCRCMD_CHECK_PHONETIMER_50B2C0();
    EXPORT void SCRCMD_STOP_PHONE_RING_50B360();
    EXPORT void SCRCMD_IS_CHAR_FIRE_ONSCREEN_50B3D0();
    EXPORT void SCRCMD_CAR_DRIVE_AWAY_50B440();
    EXPORT void SCRCMD_GET_CAR_FROM_CRANE_50B470();
    EXPORT void SCRCMD_CHAR_TO_DRIVE_CAR_50B4F0();
    EXPORT void SCRCMD_DRIVER_OUT_CAR_50B5A0();
    EXPORT void SCRCMD_GIVE_DRIVER_BRAKE_50B600();
    EXPORT void SCRCMD_SET_GANG_INFO1_50B670();
    EXPORT void SCRCMD_CLEAR_TIMERS_50B690();
    EXPORT void SCRCMD_CHECK_SCORE_50B6F0();
    EXPORT void SCRCMD_GET_SCORE_50B760();
    EXPORT void SCRCMD_IS_CHAR_IN_GANG_50B7D0();
    EXPORT void SCRCMD_SET_NO_COLLIDE_50B8B0();
    EXPORT void SCRCMD_IS_CHAR_FIRING_AREA_50B910();
    EXPORT void SCRCMD_GET_PASSENGER_NUM_50B9C0();
    EXPORT void SCRCMD_CLEAR_WANTED_LEVEL_50BA30();
    EXPORT void SCRCMD_ALT_WANTED_LEVEL_50BA70();
    EXPORT void SCRCMD_CAR_WRECK_IN_LOCATION_50BAD0();
    EXPORT void SCRCMD_SEND_CAR_TO_BLOCK_50BB80();
    EXPORT void SCRCMD_THREAT_SEARCH_REACT_ADD_GROUP_50BBD0();
    EXPORT void SCRCMD_CHECK_NUM_ALIVE_50BC60();
    EXPORT void SCRCMD_SET_MIN_ALIVE_50BCD0();
    EXPORT void SCRCMD_CHAR_GROUP_50BD10();
    EXPORT void SCRCMD_SET_COUNTER_50BDC0();
    EXPORT void SCRCMD_HAS_CAR_WEAPON_50BE00();
    EXPORT void SCRCMD_IS_CHAR_HORN_50BE70();
    EXPORT void SCRCMD_CHECK_MAX_PASS_50BED0();
    EXPORT void SCRCMD_IS_CHAR_IN_ZONE_50BF40();
    EXPORT void SCRCMD_SET_PHONE_DEAD_50C040();
    EXPORT void SCRCMD_IS_TRAILER_ATT_50C0E0();
    EXPORT void SCRCMD_IS_CAR_ON_TRAIL_50C1B0();
    EXPORT void SCRCMD_ENABLE_DISABLE_CRANE_50C230();
    EXPORT void SCRCMD_CAR_GOT_DRIVER_50C2A0();
    EXPORT void SCRCMD_SPOTTED_PLAYER_50C2F0();
    EXPORT void SCRCMD_GET_LAST_PUNCHED_50C350();
    EXPORT void SCRCMD_IS_CHAR_STUNNED_50C3B0();
    EXPORT void SCRCMD_KILL_ALL_PASSENG_50C410();
    EXPORT void SCRCMD_IS_GROUP_IN_CAR_50C470();
    EXPORT void SCRCMD_PUNCHED_SOMEONE_50C4E0();
    EXPORT void SCRCMD_ADD_CHAR_TO_GANG_50C540();
    EXPORT void SCRCMD_EXPLODE_50C5A0();
    EXPORT void SCRCMD_PARK_50C6F0();
    EXPORT void SCRCMD_BEEN_PUNCHED_BY_50C760();
    EXPORT void SCRCMD_UPDATE_DOOR_50C7D0();
    EXPORT void SCRCMD_DOOR_50C8A0();
    EXPORT void SCRCMD_REMOVE_WEAPON_50C990();
    EXPORT void SCRCMD_REMOVE_BLOCK_50C9F0();
    EXPORT void SCRCMD_LOWER_LEVEL_50CA30();
    EXPORT void SCRCMD_CHANGE_BLOCK_50CA70();
    EXPORT void SCRCMD_ADD_NEW_BLOCK_50CAB0();
    EXPORT void SCRCMD_ROAD_ON_OFF_50CB20();
    EXPORT void SCRCMD_CHECK_CAR_DRIVER_50CB70();
    EXPORT void SCRCMD_SWITCH_GENERATOR1_50CCB0();
    EXPORT void SCRCMD_SWITCH_GENERATOR3_50CD30();
    EXPORT void SCRCMD_CAR_DAMAGE_POS_50CDB0();
    EXPORT void SCRCMD_PARK_FINISHED_50CE10();
    EXPORT void Gosub_50CE50(SCR_CMD_HEADER* pCmd, u16 cmd_idx);
    EXPORT void SCRCMD_PHONE_TEMPLATE_50CE90();
    EXPORT void SCRCMD_REMOTE_CONTROL_50D200();
    EXPORT void SCRCMD_LAUNCH_MISSION_50D2E0();
    EXPORT void SCRCMD_SAVE_GAME_50D340();
    EXPORT void SCRCMD_DO_SAVE_GAME_50D3C0();
    EXPORT void SCRCMD_CHANGE_CAR_LOCK_50D680();
    EXPORT void SCRCMD_CHANGE_INTENSITY_50D870();
    EXPORT void SCRCMD_CHANGE_COLOUR_50D900();
    EXPORT void SCRCMD_CHANGE_RADIUS_50D9A0();
    EXPORT void SCRCMD_CREATE_LIGHT_50DA50();
    EXPORT void GetSetLivesAndMultiplier_50DB70();
    EXPORT void SCRCMD_GET_LIVES_MULT_50DD00();
    EXPORT void SCRCMD_SET_DIR_OF_TVVAN_50DD90();
    EXPORT void SCRCMD_POINT_ONSCREEN_50DE00();
    EXPORT void SCRCMD_CHAR_IN_AIR_50DE50();
    EXPORT void SCRCMD_CHAR_SUNK_50DEB0();
    EXPORT void StartBonus_50DF10();
    EXPORT void SCRCMD_CHECK_BONUS_50E0B0();
    EXPORT void SCRCMD_SETUP_MODEL_CHECK_50E120();
    EXPORT void SCRCMD_MODEL_CHECK_50E150();
    EXPORT void GetSpeed_50E190();
    EXPORT void SCRCMD_CHECK_CAR_SPEED_50E360();
    EXPORT void SCRCMD_SET_CAR_GRAPHIC_50E460();
    EXPORT void SCRCMD_CHAR_ARRESTED_50E4A0();
    EXPORT void SCRCMD_S_IS_S_OP_S_50E4F0();
    EXPORT void SCRCMD_S_IS_S_OP_I_50E610();
    EXPORT void SCRCMD_CHAR_DRIVE_AGGR_50E730();
    EXPORT void SCRCMD_SET_SPEED_50E780();
    EXPORT void SCRCMD_GIVE_CAR_ALARM_50E7F0();
    EXPORT void SCRCMD_CAR_WEAPON_KIND_PROOF_50E820();
    EXPORT void SCRCMD_PUT_CAR_ON_TRAILER_50E900();
    EXPORT void SCRCMD_CLEAR_BRIEFS_50E9A0();
    EXPORT void SCRCMD_CHECK_HEADS_50E9E0();
    EXPORT void SCRCMD_FINISH_LEVEL_50EA40();
    EXPORT void SCRCMD_CHECK_WEAPONHIT_50EB00();
    EXPORT void SCRCMD_WEAP_HIT_CAR_50EBD0();
    EXPORT void SCRCMD_IS_CHAR_ON_FIRE_50ECE0();
    EXPORT void SCRCMD_BRIEF_ONSCREEN_50ED40();
    EXPORT void SCRCMD_CREATE_SOUND_50ED80();
    EXPORT void SCRCMD_DO_EASY_PHONE_50EDC0();
    EXPORT void SCRCMD_CHAR_INTO_CAR_50F060();
    EXPORT void SCRCMD_DECIDE_POWERUP_50F150();
    EXPORT void SCRCMD_SUPPRESS_MODEL_50F220();
    EXPORT void SCRCMD_WARP_CHAR_50F270();
    EXPORT void SCRCMD_SET_GROUP_TYPE_50F3D0();
    EXPORT void SCRCMD_CHAR_DO_NOTHING_50F410();
    EXPORT void SCRCMD_EMERG_LIGHTS_50F450();
    EXPORT void SCRCMD_CHECK_OBJ_MODEL_50F4D0();
    EXPORT void SCRCMD_PED_GRAPHIC_50F550();
    EXPORT void SCRCMD_SET_CHAR_FLAGS_50F5E0();
    EXPORT void SCRCMD_MAKE_MUGGERS_50F770();
    EXPORT void LocSecChar_50F7B0();
    EXPORT void SCRCMD_STOP_CAR_DRIVE_50F900();
    EXPORT void SCRCMD_IS_BUS_FULL_50F940();
    EXPORT void SCRCMD_NO_CHARS_OFF_BUS_50F9B0();
    EXPORT void SCRCMD_KILL_CHAR_50FA00();
    EXPORT void SCRCMD_SET_SHADING_LEV_50FA40();
    EXPORT void SCRCMD_SET_CAR_JAMMED_50FA70();
    EXPORT void SCRCMD_FINISH_MISSION_50FAD0();
    EXPORT void SCRCMD_STORE_BONUS_50FAF0();
    EXPORT void SCRCMD_SET_ENTER_STATUS_50FB60();
    EXPORT void SCRCMD_SET_FAV_CAR_50FC20();
    EXPORT void GroupInArea_50FC60();
    EXPORT void SCRCMD_SET_KF_WEAPON_50FE00();
    EXPORT void SCRCMD_CLEAR_KF_WEAPON_50FED0();
    EXPORT void SCRCMD_ADD_ONSCREEN_COUNTER_50FF50();
    EXPORT void SCRCMD_CLEAR_COUNTER_50FFB0();
    EXPORT void SCRCMD_CHANGE_POLICE_510030();
    EXPORT void SCRCMD_DESTROY_GROUP_510050();
    EXPORT void SCRCMD_CHECK_CURRENT_WEAPON_510090();
    EXPORT void SCRCMD_START_BASIC_KF_510100();
    EXPORT void SCRCMD_DO_BASIC_KF_510280();
    EXPORT void SCRCMD_SET_BONUS_RATING_510530();
    EXPORT void SCRCMD_ADD_TIME_510560();
    EXPORT void SCRCMD_SET_MODEL_WANTED_5105B0();
    EXPORT void SCRCMD_CHECK_DEATH_ARR_510600();
    EXPORT void SCRCMD_FORCE_CLEANUP_510660();
    EXPORT void SCRCMD_SAVE_RESTORE_RESPECT_510780();
    EXPORT void PreExecOpCode_5108D0();
    EXPORT bool PoolUpdate();
    EXPORT void InitThread_511930(char_type flags, u16 start_cmd);
    EXPORT ScriptThread* SpawnThread_511960(u16 start_cmd);
    EXPORT void launch_mission_5119A0(SCR_CMD_HEADER* pCmd, const char_type* pMissionName);
    EXPORT ScriptThread();
    EXPORT void DisableBasicKfsFromSave_511CD0();
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
            pThread->Log_5035D0();
        }
    }

    EXPORT ~ScriptThreadPool();

    Pool<ScriptThread, 8> mPool;
};

EXTERN_GLOBAL(ScriptThreadPool*, miss2_0x11C_Pool_6F8064);