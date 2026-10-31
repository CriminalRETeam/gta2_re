#include "Door.hpp"
#include "DoorPool.hpp"
#include "Garage.hpp"
#include "Globals.hpp"
#include "Object_5C.hpp"
#include "Ped.hpp"
#include "TileAnimManager.hpp"
#include "error.hpp"
#include "gtx_0x106C.hpp"
#include "map_0x370.hpp"

DEFINE_GLOBAL(s32, kZero_67BBE0, 0x67BBE0);
DEFINE_GLOBAL_ARRAY(DoorAnimInfo, gDoorAnimInfo, 5, 0x67BB38);
DEFINE_GLOBAL(DoorPool*, gDoor_4D4, 0x67BD2C);
DEFINE_GLOBAL_INIT(Fix16, kFpHalf_67BA20, Fix16(0x2000, 0), 0x67BA20);
DEFINE_GLOBAL_INIT(Fix16, kFpOne_67BBE4, Fix16(1), 0x67BBE4);
DEFINE_GLOBAL_INIT(Fix16, kFpTwo_67BBE8, Fix16(2), 0x67BBE8);
DEFINE_GLOBAL_INIT(Ang16, kAng90_67BA38, Ang16(0x168), 0x67BA38);
DEFINE_GLOBAL_INIT(Ang16, kAng270_67BB2C, Ang16(0x438), 0x67BB2C);
DEFINE_GLOBAL(Ang16, DAT_0067BD18, 0x67BD18);

MATCH_FUNC(0x49c640)
Door::Door()
{
    mPrimaryDoorData = NULL;
    mSecondaryDoorData = NULL;
    mDoorObj = NULL;
    mTriggerObj = NULL;
    mOpenType = door_open_type::none;
    mCloseType = door_close_type::none;
    mModelId = 0;
    mUnused18 = 0;
    mbClosing = true;
    mbCanAutoOpen = true;
    mCloseDelay = 0;
    mCloseTimer = 0;
    mTargetId = 0;
    mbAuto = true;
    mbDoFlip = 0;
    mbReversed = 0;
    mX = kZero_67BBE0;
    mY = kZero_67BBE0;
    mPlayOpenSound = false;
}

MATCH_FUNC(0x49c690)
Door::~Door()
{
    mPrimaryDoorData = NULL;
    mSecondaryDoorData = NULL;
    mDoorObj = NULL;
    mTriggerObj = NULL;
}

MATCH_FUNC(0x49c6a0)
bool Door::IsSpriteClearOfDoorCollision(Sprite* pSprite)
{
    if (pSprite)
    {
        return mDoorObj->mSprite->RotatedRectCollisionSAT(pSprite) == false;
    }
    return true;
}

MATCH_FUNC(0x49c6d0)
bool Door::CanOpen(Car* pCar)
{
    bool ret = false;
    switch (mOpenType)
    {
        case door_open_type::any_player:
            if (!pCar->IsDrivenByPlayer())
            {
                if (ped_type::player_2 != pCar->mPedType)
                {
                    break;
                }
            }
            ret = true;
            break;
        case door_open_type::one_car:
            if (this->mCarBc && pCar == this->mCarBc && pCar->mCarId == this->mTargetId)
            {
                ret = true;
                break;
            }

            break;

        case door_open_type::one_model:
            if (pCar->mCarInfoIdx != this->mModelId)
            {
                break;
            }
            ret = true;
            break;

        case door_open_type::any_player_one_car:
            if (this->mCarBc == NULL)
            {
                break;
            }
            if (pCar != this->mCarBc)
            {
                break;
            }
            if (pCar->mCarId != this->mTargetId)
            {
                break;
            }

            if (IsSpriteClearOfDoorCollision(pCar->mCarSprite) && (!pCar->IsDrivenByPlayer() || gGarage_48->IsParkedCar(pCar)))
            {
                if (pCar->IsDrivenByPlayer())
                {
                    break;
                }
                if (!gGarage_48->IsParkedCar(pCar))
                {
                    break;
                }
            }
            ret = true;
            break;
        case door_open_type::any_car:
            ret = true;
            break;
    }
    return ret;
}

MATCH_FUNC(0x49c7f0)
bool Door::CanOpenForPed(Ped* pPed)
{
    bool ret = false;
    switch (mOpenType)
    {
        case door_open_type::any_player:
            if (pPed->is_player())
            {
                ret = true;
            }
            break;

        case door_open_type::one_char_on_foot:
            if (mPed)
            {
                if (pPed == mPed)
                {
                    if (pPed->mId == static_cast<u32>(this->mTargetId))
                    {
                        ret = true;
                    }
                }
            }
            break;
    }
    return ret;
}

MATCH_FUNC(0x49c840)
void Door::Open()
{
    DoorData* pDoorData = this->mPrimaryDoorData;
    if (pDoorData != NULL)
    {
        if (pDoorData->mState != door_state::open_2)
        {
            this->mPlayOpenSound = true;
        }
        pDoorData->Open(0);
    }
    if (this->mSecondaryDoorData != NULL)
    {
        this->mSecondaryDoorData->Open(this->mbDoFlip);
    }
    this->mbClosing = false;
}

MATCH_FUNC(0x49c870)
void Door::TryOpenForCar(Car* pCar)
{
    if (mbAuto)
    {
        if (CanOpen(pCar))
        {
            if (mbCanAutoOpen)
            {
                Open();
            }
        }
    }
}

MATCH_FUNC(0x49c8a0)
void Door::TryOpenForPed(Ped* pPed)
{
    if (mbAuto)
    {
        if (CanOpenForPed(pPed))
        {
            if (mbCanAutoOpen)
            {
                Open();
            }
        }
    }
}

MATCH_FUNC(0x49c8d0)
void Door::InitSingleNoCheck(u8 door_group, u8 gr_id, u8 x, u8 y, u8 z, s32 facing)
{
    Fix16 trigger_x;
    Fix16 trigger_y;

    AddDoorData(gr_id, x, y, z, facing);

    Fix16 door_x = Fix16(x) + kFpHalf_67BA20;
    Fix16 door_y = Fix16(y) + kFpHalf_67BA20;

    switch (facing)
    {
        case 1:
            trigger_x = Fix16(x) - kFpHalf_67BA20;
            trigger_y = Fix16(y) + kFpHalf_67BA20;
            if (this->mbReversed)
            {
                door_x -= kFpOne_67BBE4;
            }
            break;
        case 2:
            trigger_x = Fix16(x) + kFpOne_67BBE4 + kFpHalf_67BA20;
            trigger_y = Fix16(y) + kFpHalf_67BA20;
            if (this->mbReversed)
            {
                door_x += kFpOne_67BBE4;
            }
            break;
        case 3:
            trigger_x = Fix16(x) + kFpHalf_67BA20;
            trigger_y = Fix16(y) - kFpHalf_67BA20;
            if (this->mbReversed)
            {
                door_y -= kFpOne_67BBE4;
            }
            break;
        case 4:
            trigger_x = Fix16(x) + kFpHalf_67BA20;
            trigger_y = Fix16(y) + kFpOne_67BBE4 + kFpHalf_67BA20;
            if (this->mbReversed)
            {
                door_y += kFpOne_67BBE4;
            }
            break;
    }

    mbClosing = true;
    mbCanAutoOpen = true;

    mCloseTimer = mCloseDelay = (gDoorAnimInfo[gr_id].mEndFrame - gDoorAnimInfo[gr_id].mStartFrame) * gDoorAnimInfo[gr_id].mSpeed8;

    mTriggerObj = gObject_5C->NewPhysicsObj(objects::door_trigger_167, trigger_x, trigger_y, z, DAT_0067BD18);
    mTriggerObj->SetParam(door_group);

    mDoorObj = gObject_5C->NewPhysicsObj(objects::door_collision_obj_169, door_x, door_y, z, DAT_0067BD18);
    mDoorObj->SetParam(door_group);
    mX = door_x;
    mY = door_y;
}

MATCH_FUNC(0x49ca50)
void Door::AddDoorData(u8 gr_id, char_type x, char_type y, char_type z, s32 face)
{
    if (!mPrimaryDoorData)
    {
        mPrimaryDoorData = gDoor_4D4->AllocDoorData(gr_id, x, y, z, face, 0);
        return;
    }

    if (!mSecondaryDoorData)
    {
        mSecondaryDoorData = gDoor_4D4->AllocDoorData(gr_id, x, y, z, face, mbDoFlip);
    }
}

MATCH_FUNC(0x49cac0)
void Door::InitSingle(DoorData* pDoorData, char_type bFlip, u8 obj_id, Fix16 trigger_x, Fix16 trigger_y, Fix16 trigger_z, Fix16 trigger_w, Fix16 trigger_h)
{
    Fix16 z(pDoorData->mZ);
    Fix16 x = Fix16(pDoorData->mX) + kFpHalf_67BA20;
    Fix16 y = Fix16(pDoorData->mY) + kFpHalf_67BA20;
    switch (pDoorData->mFace)
    {
        case 1:
            if (this->mbReversed)
            {
                x -= kFpOne_67BBE4;
            }
            break;
        case 2:
            if (this->mbReversed)
            {
                x += kFpOne_67BBE4;
            }
            break;
        case 3:
            if (this->mbReversed)
            {
                y -= kFpOne_67BBE4;
            }
            break;
        case 4:
            if (this->mbReversed)
            {
                y += kFpOne_67BBE4;
            }
            break;
    }
    mbClosing = true;
    mbCanAutoOpen = true;
    u16 anim_frames = (gDoorAnimInfo[pDoorData->mGrId].mEndFrame - gDoorAnimInfo[pDoorData->mGrId].mStartFrame);
    mCloseTimer = mCloseDelay = anim_frames * gDoorAnimInfo[pDoorData->mGrId].mSpeed8 + 0x28;
    if (bFlip)
    {
        mTriggerObj = gObject_5C->NewTouchPoint(objects::door_trigger_167,
                                                trigger_x,
                                                trigger_y,
                                                trigger_z,
                                                DAT_0067BD18,
                                                trigger_w,
                                                trigger_h,
                                                kFpOne_67BBE4);
        mTriggerObj->SetParam(obj_id);
    }
    mDoorObj = gObject_5C->NewPhysicsObj(objects::door_collision_obj_169, x, y, z, DAT_0067BD18);
    mDoorObj->SetParam(obj_id);
    mX = x;
    mY = y;
}

MATCH_FUNC(0x49cc00)
void Door::InitDouble(DoorData* pDoorData, char_type bFlip, u8 obj_id, Fix16 trigger_x, Fix16 trigger_y, Fix16 trigger_z, Fix16 trigger_w, Fix16 trigger_h)
{
    Ang16 door_angle;
    Fix16 z(pDoorData->mZ);
    Fix16 x = Fix16(pDoorData->mX) + kFpHalf_67BA20;
    Fix16 y = Fix16(pDoorData->mY) + kFpHalf_67BA20;

    switch (pDoorData->mFace)
    {
        case 3:
            x += kFpHalf_67BA20;
            door_angle = kAng270_67BB2C;
            if (this->mbReversed)
            {
                y -= kFpOne_67BBE4;
            }
            break;
        case 1:
            y -= kFpHalf_67BA20;
            door_angle = DAT_0067BD18;
            if (this->mbReversed)
            {
                x -= kFpOne_67BBE4;
            }
            break;
        case 2:
            y += kFpHalf_67BA20;
            door_angle = DAT_0067BD18;
            if (this->mbReversed)
            {
                x += kFpOne_67BBE4;
            }
            break;
        case 4:
            x -= kFpHalf_67BA20;
            door_angle = kAng90_67BA38;
            if (this->mbReversed)
            {
                y += kFpOne_67BBE4;
            }
            break;
    }
    mbClosing = true;
    mbCanAutoOpen = true;
    u16 anim_frames = (gDoorAnimInfo[pDoorData->mGrId].mEndFrame - gDoorAnimInfo[pDoorData->mGrId].mStartFrame);
    mCloseTimer = mCloseDelay = anim_frames * gDoorAnimInfo[pDoorData->mGrId].mSpeed8 + 0x28;
    if (bFlip)
    {
        mTriggerObj = gObject_5C->NewTouchPoint(objects::door_trigger_167,
                                                trigger_x,
                                                trigger_y,
                                                trigger_z,
                                                DAT_0067BD18,
                                                trigger_w,
                                                trigger_h,
                                                kFpOne_67BBE4);
        mTriggerObj->SetParam(obj_id);
    }
    mDoorObj =
        gObject_5C
            ->NewTouchPoint(objects::door_collision_obj_169, x, y, z, door_angle, kFpOne_67BBE4, kFpTwo_67BBE8, kFpOne_67BBE4);
    mDoorObj->SetParam(obj_id);
    mX = x;
    mY = y;
}

MATCH_FUNC(0x49cd90)
void Door::UpdateAutoClose()
{
    if (mbAuto)
    {
        if ((mPrimaryDoorData != NULL && mPrimaryDoorData->mState == door_state::open_2) || (mSecondaryDoorData != NULL && mSecondaryDoorData->mState == door_state::open_2))
        {
            if (mCloseType != door_close_type::close_never && mCloseType != door_close_type::none)
            {
                if (mCloseType == door_close_type::close_when_open_rule_fails && mCarBc != NULL)
                {
                    if (CanOpen(mCarBc) == false && CanOpenForPed(mPed) == false &&
                        !IsSpriteClearOfDoorCollision(mCarBc->mCarSprite))
                    {
                        mbClosing = true;
                        mCloseTimer = 0;
                    }
                }
                if (mbClosing)
                {
                    if (mCloseTimer > 0)
                    {
                        mCloseTimer--;
                    }
                }
                if (mbClosing && mCloseTimer == 0)
                {
                    mCloseTimer = mCloseDelay;
                    if (mPrimaryDoorData != NULL)
                    {
                        gObject_5C->NewWithPedRefIdx(0x117, 0x33, mX, mY, mPrimaryDoorData->mZ);
                        mPrimaryDoorData->Close(false);
                    }
                    if (mSecondaryDoorData != NULL)
                    {
                        mSecondaryDoorData->Close(mbDoFlip);
                    }
                }
            }
        }
    }
}

MATCH_FUNC(0x49ce90)
bool Door::Service()
{
    if (mPlayOpenSound)
    {
        gObject_5C->NewWithPedRefIdx(0x117, 0x32, mX, mY, mPrimaryDoorData->mZ);
        mPlayOpenSound = false;
    }

    if (mbAuto)
    {
        UpdateAutoClose();
    }
    mbClosing = true;
    return 0;
}

MATCH_FUNC(0x49CEE0)
void Door::get_door_xyz_face(u8* pX, u8* pY, u8* pZ, u32* pFace)
{
    *pX = mPrimaryDoorData->mX;
    *pY = mPrimaryDoorData->mY;
    *pZ = mPrimaryDoorData->mZ;
    *pFace = mPrimaryDoorData->mFace;
}
