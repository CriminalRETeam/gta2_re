// GetLength_41E260 compares against this TU's zero
#define FIX16_POINT_ZERO kFpZero_6F8E10
#include "Object_5C.hpp"
#include "explosion_type.hpp"
#include "instance_limit_kind.hpp"
#include "ped_death_cause.hpp"
#include "CarPhysics.hpp"
#include "Car.hpp"
#include "Char_Pool.hpp"
#include "DoorPool.hpp"
#include "Game.hpp"
#include "Globals.hpp"
#include "LightPool.hpp"
#include "ObjectPool.hpp"
#include "Object_3C_Pool.hpp"
#include "Object_8_Pool.hpp"
#include "ParticleManager.hpp"
#include "ObjectDefinitions.hpp"
#include "SpriteGrid.hpp"
#include "CollisionSoundQueue.hpp"
#include "TrafficLights.hpp"
#include "PedRefTable.hpp"
#include "WeaponManager.hpp"
#include "Explosion.hpp"
#include "collide.hpp"
#include "enums.hpp"
#include "error.hpp"
#include "ScriptManager.hpp"
#include "map_0x370.hpp"
#include "sprite.hpp"
#include "block_face.hpp"

DEFINE_GLOBAL(Object_5C*, gObject_5C, 0x6F8F84);
DEFINE_STATIC_GLOBAL(s32, sObjectCounterUnused, 0x6f8f88);
DEFINE_STATIC_MEMBER(s32, Object, sObject2CWallHitSide, 0x6F8F90);
DEFINE_GLOBAL_INIT(Ang16, kAng180_6F8D62, Ang16(0x2D0), 0x6F8D62);

DEFINE_GLOBAL(Fix16_Point, gZeroVector, 0x6F8EF0);
DEFINE_GLOBAL_INIT(Fix16, kFpZero_6F8E10, Fix16(0), 0x6F8E10);
DEFINE_GLOBAL_INIT(Fix16, kFpOne64th_6F8C9C, Fix16(0x100, 0), 0x6F8C9C);

DEFINE_STATIC_GLOBAL(u8, sInObjectMovementUpdate, 0x6F8C68);
DEFINE_STATIC_GLOBAL(u8, sObjectRemovedFromBuckets, 0x6F8C4C);
DEFINE_STATIC_GLOBAL(u8, sObjectReBucketed, 0x6F8F40);
DEFINE_GLOBAL(u32, gObj3C_live_count, 0x6F8E54);
DEFINE_GLOBAL(u32, gObj8_count, 0x6F8F18);
DEFINE_GLOBAL(u32, gObj8_dealloc_count, 0x6F8DC0);
DEFINE_GLOBAL(u32, gObj3C_dealloc_count, 0x6F8F0C);
DEFINE_GLOBAL(bool, gDoorAccessCheckActive, 0x6F8EDC);

DEFINE_STATIC_GLOBAL_INIT(Ang16, kAng315, Ang16(0x4EC), 0x6F8D8C);
DEFINE_GLOBAL_INIT(Ang16, kAng135_6F8D80, Ang16(0x21C), 0x6F8D80);
DEFINE_STATIC_GLOBAL_INIT(Ang16, kAng225, Ang16(0x384), 0x6F8D54);
DEFINE_GLOBAL_INIT(Ang16, kAng45_6F8CD0, Ang16(0xB4), 0x6F8CD0);

DEFINE_STATIC_MEMBER(Sprite*, Object, sObject2CLastCollisionSprite, 0x6F8F8C);
DEFINE_STATIC_MEMBER(bool, Object, sObject2CHitWallOrDoor, 0x6F8F94);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpOneThirtySecond, Fix16(0x200, 0), 0x6F8F5C);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kObjectGravity, Fix16(0xFFFFFFF8, 0), 0x6F8DA8);
DEFINE_GLOBAL_INIT(Fix16, kFP16One_6F8E14, Fix16(1), 0x6F8E14);
DEFINE_GLOBAL_INIT(Fix16, kFpPoint1_6F8C58, Fix16(0x666, 0), 0x6F8C58);

DEFINE_GLOBAL_INIT(Fix16, kFpOne64th_6F8DC8, Fix16(256, 0), 0x6F8DC8);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpEighteen, Fix16(18), 0x6F8CE8);
DEFINE_GLOBAL_INIT(Fix16, kFP16One_6F8CEC, Fix16(1), 0x6F8CEC);
DEFINE_GLOBAL_INIT(Fix16, kFpOne64th_6F8ECC, kFpOne64th_6F8DC8, 0x6F8ECC);

DEFINE_GLOBAL_INIT(Ang16, kAng90_6F8C88, Ang16(0x168), 0x6F8C88); // TODO: Init via func 0x5269F0
DEFINE_GLOBAL_INIT(Ang16, kAng270_6F8D88, Ang16(0x438), 0x6F8D88); // TODO: Init via func 0x526E70

DEFINE_GLOBAL_INIT(Fix16, kFpOne256th_6F8CF0, Fix16(0x40, 0), 0x6F8CF0);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kMaxZ, Fix16(0x1FF00, 0), 0x6F8BFC);
DEFINE_GLOBAL_INIT(Fix16, kFP16Half_6F8D10, Fix16(0x2000, 0), 0x6F8D10);

DEFINE_GLOBAL_INIT(u8, gCarAIProbingAhead, 0, 0x6771DC);
DEFINE_GLOBAL_INIT(s32, gObj2C_id, 1, 0x623EC0);

DEFINE_GLOBAL_INIT(Fix16, kFP16One_6F8D38, kFP16One_6F8E14, 0x6F8D38);
DEFINE_GLOBAL_INIT(Fix16, kFP16One_6F8CE0, kFP16One_6F8E14, 0x6F8CE0);
DEFINE_GLOBAL_INIT(Fix16, kFP16One_6F8F74, kFP16One_6F8E14, 0x6F8F74);
DEFINE_GLOBAL_INIT(Fix16, kFP16Half_6F8EE4, kFP16Half_6F8D10, 0x6F8EE4);

DEFINE_GLOBAL_INIT(Fix16, kFP16Half_6F8D3C, kFP16Half_6F8D10, 0x6F8D3C);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFP16MinusOne, Fix16(-1), 0x6F8BE8); // Fix16(0xFFFFC000, 0)

DEFINE_GLOBAL_INIT(Fix16, kFP16Half_6F8F2C, kFP16Half_6F8D10, 0x6F8F2C);
DEFINE_GLOBAL_INIT(Fix16, kFP16One_6F8D24, kFP16One_6F8E14, 0x6F8D24);

DEFINE_STATIC_GLOBAL(Fix16, sObj2CLastValidX, 0x6F8F00);
DEFINE_STATIC_GLOBAL(Fix16, sObj2CLastValidY, 0x6F8EF8);
DEFINE_GLOBAL(Fix16, gObj2C_LastValidZ, 0x6F8EFC);
DEFINE_STATIC_GLOBAL_INIT(Ang16, sObj2CLastValidAng, Ang16(0), 0x6F8D1C);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kMapSize, Fix16(256), 0x6F8BF4);

DEFINE_GLOBAL_INIT(Fix16, kFpPoint7_6F8C78, Fix16(0x2CCC, 0), 0x6F8C78);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kObjectBounceRestitution, kFpPoint7_6F8C78, 0x6F8DBC);

// 9.6f 0x482C10
inline bool Object::IsAnimFinished_482C10() const
{
    return !mpAny.o8->mTimer && !mpAny.o8->mFrameIdx;
}

MATCH_FUNC(0x522140)
Object::Object()
{
    mpNext = NULL;
    mSprite = NULL;
    mModel = 0;
    mDefinition = NULL;
    mpAny.o8 = NULL;
    mObj3c = NULL;
    mId = 99;
    mTouchedResult = 0;
    mRemovalState = object_removal_state::none_0;
    mPedRefIdx = 99;
    mPoolListState = object_pool_state::none_0;
    mThreadTriggerIdx = 0;
    mUnused28 = -51;
    mHasExplosion = false;
}

MATCH_FUNC(0x522180)
void Object::PoolDeallocate()
{
    this->mModel = 0;

    const s32 limit_kind = mDefinition->mInstanceLimitKind;
    if (limit_kind == instance_limit_kind::limited_count_2)
    {
        --gObject_5C->mLimitedObjectCount;
    }
    else if (limit_kind == instance_limit_kind::sprite_list_3)
    {
        --gObject_5C->mListedSpriteCount;
        gObject_5C->mSpriteList.RemoveSpriteSafe(this->mSprite);
    }

    --sObjectCounterUnused;

    if (UsesParamAsPedRef_421080())
    {
        if (mPedRefIdx > 0)
        {
            gPedRefTable_7F8->DecrementRefCount(mPedRefIdx);
            this->mPedRefIdx = 0;
        }
    }

    Object::ReleaseSubObjects();
    if (mSprite)
    {
        gSpritePool->remove(mSprite);
        this->mSprite = NULL;
    }
    this->mId = 0;
}

MATCH_FUNC(0x522250)
bool Object::IsSpriteOwnerPed(Sprite* pSprite) const
{
    if (UsesParamAsPedRef_421080())
    {
        u8 ped_ref_idx = this->GetPedRefIdx_420FF0();
        if (ped_ref_idx > 0)
        {
            if (pSprite)
            {
                Char* pChar = pSprite->AsCharB4_40FEA0();
                if (pChar)
                {
                    if (pChar->mpPed->get_ped_ref_idx_420B50() == ped_ref_idx)
                    {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

MATCH_FUNC(0x5222b0)
s32 Object::GetMovementRegionSize() const
{
    return this->mDefinition->mUsesBulletWalls != 0 ? 2048 : 1024;
}

MATCH_FUNC(0x5222d0)
void Object::UpdatePhysics()
{
    if (mObj3c->mbAirborne == 1)
    {
        mObj3c->mZAccel += kObjectGravity;
        mObj3c->mZSpeed += mObj3c->mZAccel;
    }
    else
    {
        mObj3c->mSpeed += mObj3c->mFriction;

        if (mObj3c->mSpeed < kFpZero_6F8E10)
        {
            mObj3c->mSpeed = kFpZero_6F8E10;
            mObj3c->mFriction = kFpZero_6F8E10;
        }

        if (mObj3c->mZSpeed < kFpZero_6F8E10)
        {
            mObj3c->mZSpeed = kFpZero_6F8E10;
        }
    }
}

MATCH_FUNC(0x522340)
void Object::PoolGive()
{
    if (mPoolListState == object_pool_state::out_of_pool_list_2)
    {
        gObject_2C_Pool->UpdateNextPrev(this);
        mPoolListState = object_pool_state::in_pool_list_1;
    }
}

MATCH_FUNC(0x522360)
void Object::PoolTake()
{
    if (mPoolListState == object_pool_state::in_pool_list_1)
    {
        Object* pLast = NULL;
        for (Object* pIter = gObject_2C_Pool->mpPrev; pIter; pIter = pIter->mpNext)
        {
            if (pIter == this)
            {
                if (pLast)
                {
                    pLast->mpNext = pIter->mpNext;
                }
                else
                {
                    gObject_2C_Pool->mpPrev = pIter->mpNext;
                }
                pIter->mpNext = NULL;
                break;
            }
            else
            {
                pLast = pIter;
            }
        }
        mPoolListState = object_pool_state::out_of_pool_list_2;
    }
}

MATCH_FUNC(0x5223c0)
bool Object::ShouldCollideWith(Sprite* pSprite) const
{
    s32 sprite_type;

    if (!pSprite)
    {
        return false;
    }
    switch (mDefinition->mReactToCollisionsWith)
    {
        case CollisionReaction::Always_0:
            return true;
        case CollisionReaction::OnlyCars_1:
            if (pSprite->GetType_416B40() == sprite_types_enum::car_2)
            {
                return false;
            }
            break;
        case CollisionReaction::OnlyPeds_2:
            if (pSprite->GetType_416B40() == sprite_types_enum::ped_3)
            {
                return false;
            }
            break;
        case CollisionReaction::OnlyObjects_3:
            sprite_type = pSprite->GetType_416B40();
            if (sprite_type != sprite_types_enum::code_obj1_4 && sprite_type != sprite_types_enum::map_obj_5 &&
                sprite_type != sprite_types_enum::object_1)
            {
                return true;
            }
            else
            {
                return false;
            }
            break;
        case CollisionReaction::Never_4:
            return false;
            break;
        default:
            return true;
            break;
    }
    return true;
}

MATCH_FUNC(0x522430)
bool Object::ShouldCollideWithSprite_522430(Sprite* pSprite) const
{
    return (pSprite && ShouldCollideWith(pSprite) && !IsSpriteOwnerPed(pSprite)) ? true : false;
}

MATCH_FUNC(0x522460)
bool Object::SelectCollisionSprite(Sprite* pProbe)
{
    sObject2CHitWallOrDoor = false;

    Sprite* pSprite = pProbe->QuerySpriteCollision(sprite_types_enum::car_2);
    if (pSprite && sObject2CLastCollisionSprite && pSprite->GetType_416B40() == sprite_types_enum::car_2 // IsCar
        || !ShouldCollideWithSprite_522430(pSprite) || pSprite == sObject2CLastCollisionSprite)
    {
        return false;
    }

    if (pSprite->mObject2CPtr->mModel == objects::diagonal_wall_collision_obj_166 ||
        pSprite->mObject2CPtr->mModel == objects::door_collision_obj_169)
    {
        sObject2CHitWallOrDoor = true;
    }
    gCollisionTarget.SetSprite_40FEE0(pSprite);
    return true;
}

MATCH_FUNC(0x5224e0)
void Object::SetMovementVector(Fix16_Point& speed)
{
    Fix16_Point new_velocity;
    if (mObj3c)
    {
        new_velocity = GetSpeedVector_482C50() + speed;
        this->mObj3c->mSpeed = new_velocity.GetLength_41E260();
        this->mObj3c->mAngle = new_velocity.atan2_40F790();
        this->mObj3c->mFriction = this->mDefinition->mFriction;
    }
    else
    {
        NewObj3C(speed);
    }
}

MATCH_FUNC(0x522640)
void Object::SetMovementVectorWithRandomState(Fix16_Point& point)
{
    SetMovementVector(point);

    if (mDefinition->mCollisionResponse == 3 && mObj3c->mSpriteCollideMode == 2)
    {
        const s16 rng = gRng.get_int(9);
        if (rng < 6)
        {
            if (rng < 3)
            {
                mObj3c->mSpriteCollideMode = 1;
            }
            else
            {
                mObj3c->mSpriteCollideMode = 0;
            }
        }
    }
}

MATCH_FUNC(0x5226a0)
void Object::SetConveyorPush(char_type conveyor_speed)
{
    Fix16_Point point;
    if (!mObj3c)
    {
        NewObj3C(gZeroVector);
    }
    mObj3c->mConveyorSpeed = conveyor_speed;
}

MATCH_FUNC(0x522710)
void Object::ResolveCollisionWithObject(Object* pObj, Fix16_Point* pPoint)
{
    Fix16_Point offset;
    Fix16_Point rel_velocity;
    Fix16_Point impulse;

    Fix16_Point other_pos = pObj->GetXY();
    Fix16_Point my_pos = GetXY();

    Ang16 impact_angle = 0;
    offset = (my_pos - *pPoint);

    if (pObj->CanBePushed_482C90())
    {
        rel_velocity = (GetSpeedVector_52AE90() - pObj->GetSpeedVector_52AE90());
        impulse = ComputeCollisionImpulse(GetMass_482C80(),
                                                 pObj->GetMass_482C80(),
                                                 rel_velocity,
                                                 offset,
                                                 *pPoint,
                                                 my_pos,
                                                 other_pos,
                                                 kFP16One_6F8D38,
                                                 kFP16One_6F8D38,
                                                 kFP16One_6F8D24);

        pObj->SetMovementVectorWithRandomState(impulse.Negate_40ACB0() / pObj->GetMass_482C80());
    }
    else
    {
        rel_velocity = GetSpeedVector_52AE90();
        if (offset.x == kFpZero_6F8E10 && offset.y == kFpZero_6F8E10)
        {
            offset = rel_velocity;
        }

        impulse = ComputeCollisionImpulse(GetMass_482C80(),
                                                 kFP16MinusOne,
                                                 rel_velocity,
                                                 offset,
                                                 *pPoint,
                                                 my_pos,
                                                 other_pos,
                                                 kFP16One_6F8D38,
                                                 kFP16One_6F8D38,
                                                 kFP16Half_6F8F2C);
    }

    if (mObj3c)
    {
        impact_angle = mObj3c->mAngle;
    }

    SetMovementVectorWithRandomState(impulse / GetMass_482C80());

    if (sObject2CHitWallOrDoor)
    {
        if (mObj3c)
        {
            if (ComputeShortestAngleDelta(mObj3c->mAngle, impact_angle) < kAng90_6F8C88)
            {
                Ang16& angle = mObj3c->mAngle;
                angle.rValue += kAng180_6F8D62.rValue;
                angle.Normalize_406C20();
            }
        }
    }

    pObj->HandleImpact(mSprite);
    HandleImpact(pObj->mSprite);
}

// 9.6f 0x4867E0
MATCH_FUNC(0x5229b0)
void Object::ResolveCollisionWithPed(Char* pChar, Fix16_Point* pPoint, u8 not_used)
{
    Fix16_Point posDelta;
    Fix16_Point impulse;
    Fix16_Point ped_impulse;
    Ped* pPed = pChar->mpPed;
    Fix16_Point camPos(pPed->get_cam_x(), pPed->get_cam_y());

    Fix16_Point spritePos = GetXY();
    posDelta = (spritePos - *pPoint);

    impulse = ComputeCollisionImpulse(GetMass_482C80(),
                                                    kFP16Half_6F8EE4,
                                                    pChar->GetRunVelocity(),
                                                    posDelta,
                                                    *pPoint,
                                                    spritePos,
                                                    camPos,
                                                    kFP16One_6F8D38,
                                                    kFP16One_6F8CE0,
                                                    kFP16One_6F8F74);
    SetMovementVectorWithRandomState(impulse.Divide(GetMass_482C80()));
    ped_impulse = impulse.Negate_40ACB0().Divide(kFP16Half_6F8EE4);
    // The ped impact was compiled out: ped_impulse is overwritten with zero
    ped_impulse.x = kFpZero_6F8E10;
    ped_impulse.y = kFpZero_6F8E10;

    HandleImpact(pChar->get_sprite_ptr_4338D0());
}

MATCH_FUNC(0x522b20)
void Object::ResolveCollisionWithWorld(Fix16_Point* pCollisionPoint, Fix16_Point* pPoint, Fix16_Point* speed)
{
    // GetXY() is a temporary argument, not a local: that gives the original EH states (0, 2, 3) and frame
    Fix16_Point impulse = ComputeCollisionImpulse(GetMass_482C80(),
                                                        kFP16MinusOne,
                                                        *speed,
                                                        *pPoint,
                                                        *pCollisionPoint,
                                                        GetXY(),
                                                        gZeroVector,
                                                        kFP16One_6F8D38,
                                                        kFpZero_6F8E10,
                                                        kFP16Half_6F8D3C);
    SetMovementVectorWithRandomState(impulse.Divide(GetMass_482C80()));
    HandleImpact(NULL);
}

MATCH_FUNC(0x522be0)
void Object::ResolveCollisionWithHorizontalEdge(Fix16_Point* pPoint)
{
    Fix16_Point reflect_dir;
    Fix16_Point hit_point;
    Fix16_Point obj_speed;
    obj_speed = GetSpeedVector_52AE90();
    u8 edge_flag;
    if (this->mSprite->GetNearestHorizontalEdgeToCoordinate(gCollisionTarget.mHsegY, hit_point, edge_flag))
    {
        reflect_dir.SetXY_432860(Fix16(0), mSprite->mXy.y - gCollisionTarget.mHsegY);
    }
    else
    {
        hit_point.SetXY_432860(gCollisionTarget.mVsegX, gCollisionTarget.mHsegY);
        hit_point -= *pPoint;
        hit_point.x += mSprite->mXy.x;
        hit_point.y = gCollisionTarget.mHsegY;

        Fix16 abs1 = Fix16::Abs(hit_point.x - gCollisionTarget.mHsegXMax);
        Fix16 abs2 = Fix16::Abs(hit_point.x - gCollisionTarget.mHsegXMin);

        if (abs2 < abs1)
        {
            hit_point.x = gCollisionTarget.mHsegXMin;
        }
        else
        {
            hit_point.x = gCollisionTarget.mHsegXMax;
        }

        reflect_dir = -obj_speed;
    }

    ResolveCollisionWithWorld(&hit_point, &reflect_dir, &obj_speed);
}

// https://decomp.me/scratch/PMCb4
MATCH_FUNC(0x522d00)
void Object::ResolveCollisionWithVerticalEdge(Fix16_Point* pPoint)
{
    u8 edge_flag;
    Fix16_Point reflect_dir;
    Fix16_Point hit_point;
    Fix16_Point obj_speed;
    obj_speed = Object::GetSpeedVector_52AE90();
    if (mSprite->GetNearestVerticalEdgeToCoordinate(gCollisionTarget.mVsegX, hit_point, edge_flag))
    {
        reflect_dir.SetXY_432860(mSprite->mXy.x - gCollisionTarget.mVsegX, Fix16(0));
    }
    else
    {
        hit_point.SetXY_432860(gCollisionTarget.mVsegX, gCollisionTarget.mHsegY);
        hit_point -= *pPoint;
        hit_point.y += mSprite->mXy.y;
        hit_point.x = gCollisionTarget.mVsegX;

        Fix16 abs1 = Fix16::Abs(hit_point.y - gCollisionTarget.mVsegYMax);
        Fix16 abs2 = Fix16::Abs(hit_point.y - gCollisionTarget.mVsegYMin);

        if (abs2 < abs1)
        {
            hit_point.y = gCollisionTarget.mVsegYMin;
        }
        else
        {
            hit_point.y = gCollisionTarget.mVsegYMax;
        }

        reflect_dir = -obj_speed;
    }
    Object::ResolveCollisionWithWorld(&hit_point, &reflect_dir, &obj_speed);
}

MATCH_FUNC(0x522e10)
void Object::HandleCollision(Fix16_Point* pPoint)
{

    Fix16_Point hit_point;
    switch (gCollisionTarget.mType)
    {
        case collision_target_type::horizontal_edge_1:
            ResolveCollisionWithHorizontalEdge(pPoint);
            break;
        case collision_target_type::vertical_edge_2:
            ResolveCollisionWithVerticalEdge(pPoint);
            break;

        case collision_target_type::sprite_3:
        {
            u8 side_self;
            u8 side_other;
            u8 overlap_count;
            hit_point = mSprite->FindCollisionIntersectionPoint(gCollisionTarget.mpHitSprite, *pPoint, mSprite->mRotation, side_self, side_other, overlap_count);
            Car* pCar = gCollisionTarget.mpHitSprite->AsCar_40FEB0();
            if (pCar)
            {
                pCar->SetupCarPhysicsAndSpriteBinding_43BCA0();
                pCar->mPhysics->HandleObjectCollisionAt_482CC0(this, hit_point, side_other);
            }
            else
            {
                Char* pChar = gCollisionTarget.mpHitSprite->AsCharB4_40FEA0();
                if (pChar)
                {
                    ResolveCollisionWithPed(pChar, &hit_point, overlap_count);
                    HandleImpact(gCollisionTarget.mpHitSprite);
                }
                else
                {
                    Object* pObj = gCollisionTarget.mpHitSprite->AsObject2C_40FEC0();
                    ResolveCollisionWithObject(pObj, &hit_point);
                }
            }
            break;
        }
    }
}

MATCH_FUNC(0x522FA0)
void Object::Sprite_UpdateZFromSlopeAndTile(Sprite* pSprite)
{
    Fix16 z_val = pSprite->mZpos;
    if (gMap_0x370->IsGradientSlopeAt_466CF0(pSprite->mXy.x.ToInt(), pSprite->mXy.y.ToInt(), (z_val.ToInt()) - 1))
    {
        z_val -= Fix16(1);
    }

    if (gMap_0x370->UpdateZFromSlopeAtCoord(pSprite->mXy.x, pSprite->mXy.y, z_val) == 0)
    {
        Fix16 z_frac = z_val.GetFracValue();
        if (z_frac != kFpZero_6F8E10)
        {
            z_val = z_val.GetFloorValue();
            if (z_frac > kFP16Half_6F8D10)
            {
                z_val += Fix16(1);
            }

            if (z_val > kMaxZ)
            {
                z_val = kMaxZ;
            }
        }
    }

    pSprite->set_z_lazy_420660(z_val);
}

MATCH_FUNC(0x5233a0)
bool Object::TryStartSlide(Fix16 speed)
{
    LOCAL_STATIC_INIT(u8, sObject2CUnused, 1, 0x623EC4);
    sObject2CLastCollisionSprite = NULL;

    if (SelectCollisionSprite(this->mSprite))
    {

        sObject2CUnused = 0;
        if (sObject2CHitWallOrDoor)
        {
            return false;
        }

        sObject2CLastCollisionSprite = gCollisionTarget.mpHitSprite;
        if (speed > kFpPoint1_6F8C58)
        {
            return false;
        }

        if (this->mObj3c->mSpriteCollideMode == 1)
        {
            SetTopDrawLayer_482BF0();
        }
        else
        {
            this->mSprite->SetDrawLayer_40F7B0(6);
        }
        this->mObj3c->mSpeed = kFpPoint1_6F8C58;
        this->mObj3c->mFriction = this->mDefinition->mFriction;
        return true;
    }
    this->mObj3c->mSpriteCollideMode = 2;
    return false;
}

MATCH_FUNC(0x523440)
void Object::HandleCollisionOutcome(Fix16_Point point, bool bHitFloorBelow, bool bHitBlockAtZ)
{
    if (static_cast<u8>(mSprite->IsOnWater()))
    {
        this->mObj3c->mSpeed = kFpZero_6F8E10;
        this->mObj3c->mZSpeed = kFpZero_6F8E10;
        return;
    }

    switch (this->mDefinition->mCollisionResponse)
    {
        case 4:
            if (gCollisionTarget.mpHitSprite)
            {
                mSprite->set_z_lazy_420660(gCollisionTarget.mpHitSprite->mZpos);
            }

            if (!bHitFloorBelow || gCollisionTarget.mType == collision_target_type::sprite_3)
            {
                if (bHitBlockAtZ && gCollisionTarget.mType != collision_target_type::sprite_3)
                {
                    gCollisionTarget.SetBlockAtZ_482A80();
                }
            }
            else
            {
                gCollisionTarget.SetFloorBelow_482A70();
            }

            HandleImpact(gCollisionTarget.mpHitSprite);
            return;

        case 0:
        case 1:
            if (bHitFloorBelow)
            {
                gCollisionTarget.SetFloorBelow_482A70();
            }
            else if (bHitBlockAtZ)
            {
                gCollisionTarget.SetBlockAtZ_482A80();
            }
            HandleImpact(gCollisionTarget.mpHitSprite);
            return;

        case 2:
        case 3:
            HandleCollision(&point);
            return;
        default:
            return;
    }
}

MATCH_FUNC(0x5235b0)
bool Object::HandleSpriteGroundAndCollision(Sprite* pSprite, Fix16_Point* pPoint, bool* pHitWall, Fix16 found_z)
{
    Fix16 ground_z;
    bool bOnGround;
    mObj3c->mbAirborne = 0;
    if (gMap_0x370->IsGradientSlopeAt_466CF0(pSprite->mXy.x.ToInt(), pSprite->mXy.y.ToInt(), pSprite->mZpos.ToInt()))
    {
        ground_z = gMap_0x370->GetGroundZBelowCoordEx(pSprite->mXy.x, pSprite->mXy.y, pSprite->mZpos, bOnGround);

        if (!bOnGround)
        {
            if (ground_z < pSprite->mZpos)
            {
                Sprite_UpdateZFromSlopeAndTile(pSprite);
                mObj3c->mZSpeed = kObjectGravity;
            }
        }
        else
        {
            Sprite_UpdateZFromSlopeAndTile(pSprite);
        }
        mObj3c->mbOnSlope = true;
    }
    else
    {
        if (gMap_0x370->GetBlockTypeAtCoord_420420(pSprite->mXy.x.ToInt(),
                                                          pSprite->mXy.y.ToInt(),
                                                          pSprite->mZpos.ToInt() - 1) ||
            mObj3c->mbOnSlope)
        {
            if (mObj3c->mbOnSlope)
            {
                Sprite_UpdateZFromSlopeAndTile(pSprite);
            }
            mObj3c->mZSpeed = kFpZero_6F8E10;
            mObj3c->mbAirborne = 0;
        }
        else
        {
            mObj3c->mZSpeed = kObjectGravity;
            mObj3c->mbAirborne = 1;
        }
        mObj3c->mbOnSlope = false;
    }

    if (pSprite->CheckSpriteMovementRegion())
    {
        *pPoint = pSprite->get_x_y();
        *pHitWall = true;
        ComputeWallHitSide();
        return true;
    }
    else if (SelectCollisionSprite(pSprite))
    {
        mObj3c->mSavedSpeed = mObj3c->mSpeed.mValue;
        mObj3c->mZSpeed = kFpZero_6F8E10;
        mObj3c->mbAirborne = 0;
        *pPoint = pSprite->get_x_y();
        return true;
    }
    else
    {
        return false;
    }
}

// https://decomp.me/scratch/soZJL
MATCH_FUNC(0x523770)
bool Object::HandleSpriteGroundAndCollisionSimple(Sprite* pSprite, Fix16_Point* pPoint, bool* pHitWall, bool* pHitBlockAtZ)
{
    mObj3c->mbAirborne = 1;

    if (pSprite->mZpos > kMaxZ)
    {
        pSprite->mZpos = kMaxZ;
        pSprite->InvalidateZColAndBBoxes();
        mObj3c->mZSpeed = kFpZero_6F8E10;
        mObj3c->mZAccel = kFpZero_6F8E10;
    }

    if (pSprite->CheckMapZCollision())
    {
        *pHitBlockAtZ = true;
        mObj3c->mZSpeed = kFpZero_6F8E10;
        mObj3c->mZAccel = kFpZero_6F8E10;
        if (mDefinition->mZCollisionResponse == 4)
        {
            return true;
        }
    }

    if (pSprite->CheckSpriteMovementRegion())
    {
        mObj3c->mSavedSpeed = mObj3c->mSpeed;
        *pPoint = pSprite->get_x_y();
        *pHitWall = true;
        ComputeWallHitSide();
        return true;
    }
    else if (SelectCollisionSprite(pSprite))
    {
        if (sObject2CHitWallOrDoor)
        {
            mObj3c->mSavedSpeed = mObj3c->mSpeed;
            *pPoint = pSprite->get_x_y();
            *pHitWall = true;
        }
        else
        {
            mObj3c->mSavedSpeed = mObj3c->mSpeed;
            *pPoint = pSprite->get_x_y();
        }
        return true;
    }
    else
    {
        return false;
    }
}

// https://decomp.me/scratch/1RN5H
MATCH_FUNC(0x5238B0)
bool Object::HandleSpriteZCollision(Sprite* pSprite, Fix16_Point* pPoint, bool* pHitWall, bool* pHitFloorBelow, Fix16 found_z)
{
    Fix16 ground_z;
    bool bOverGap = false;
    // The landing code after the slope/non-slope split is reached through this flag, not a goto: VC6
    // threads the `if (bLanded)` test away on every path, which puts the `mZCollisionResponse != 1` arms after the
    // non-slope `bAirborne = 1` block (and lets the two `bAirborne = 1` tails merge there).
    bool bLanded = false;

    if (gMap_0x370->IsGradientSlopeAt_466CF0(pSprite->mXy.x.ToInt(), pSprite->mXy.y.ToInt(), pSprite->mZpos.ToInt()))
    {
        if (gMap_0x370->GetBlockTypeAtCoord_420420(pSprite->mXy.x.ToInt(),
                                                           pSprite->mXy.y.ToInt(),
                                                           pSprite->mZpos.ToInt()) != AIR)
        {
            ground_z = gMap_0x370->GetGroundZBelowCoordEx(pSprite->mXy.x, pSprite->mXy.y, pSprite->mZpos, bOverGap);
            if (!bOverGap)
            {
                if (ground_z > pSprite->mZpos)
                {
                    pSprite->set_xyz_lazy_420600(pSprite->mXy.x, pSprite->mXy.y, ground_z);
                    Object::Sprite_UpdateZFromSlopeAndTile(pSprite);
                    mObj3c->mbAirborne = 0;
                    bLanded = true;
                }
                else
                {
                    mObj3c->mbAirborne = 1;
                }
            }
            else
            {
                Object::Sprite_UpdateZFromSlopeAndTile(pSprite);
                mObj3c->mbAirborne = 0;
                bLanded = true;
            }
        }
        else
        {
            mObj3c->mbAirborne = 1;
        }
    }
    else
    {
        if (found_z > pSprite->mZpos)
        {
            pSprite->set_xyz_lazy_420600(pSprite->mXy.x, pSprite->mXy.y, found_z);
            Object::Sprite_UpdateZFromSlopeAndTile(pSprite);
            mObj3c->mbAirborne = 0;

            if (mObj3c->mZSpeed < kObjectGravity)
            {
                if (!sObject2CLastCollisionSprite)
                {
                    if (mObj3c->mZSpeed >= -kFpPoint1_6F8C58)
                    {
                        *pHitFloorBelow = true;
                        mObj3c->mZSpeed = kFpZero_6F8E10;
                        mObj3c->mSpeed = kFpZero_6F8E10;
                    }
                    else
                    {
                        bLanded = true;
                    }
                }
                else
                {
                    mObj3c->mZSpeed = kFpZero_6F8E10;
                    pSprite->set_xyz_lazy_420600(pSprite->mXy.x, pSprite->mXy.y, mSprite->mZpos);
                    *pHitFloorBelow = true;
                }
            }
            else
            {
                if (mObj3c->mSpeed == kFpZero_6F8E10 && mObj3c->mZSpeed == kObjectGravity)
                {
                    mObj3c->mZSpeed = kFpZero_6F8E10;
                }
            }
        }
        else
        {
            mObj3c->mbAirborne = 1;
        }
    }

    if (bLanded)
    {
        if (mDefinition->mZCollisionResponse == 1)
        {
            *pHitFloorBelow = true;
            mObj3c->mZSpeed = kFpZero_6F8E10;
            mObj3c->mSpeed = kFpZero_6F8E10;
        }
        else
        {
            if (mDefinition->mZCollisionResponse == 4)
            {
                *pHitFloorBelow = true;
                return true;
            }
            *pHitFloorBelow = true;
            mObj3c->mZSpeed = kObjectBounceRestitution * (-mObj3c->mZSpeed);
        }
    }

    if (pSprite->CheckSpriteMovementRegion())
    {
        mObj3c->mSavedSpeed = mObj3c->mSpeed;
        mObj3c->mZSpeed = kFpZero_6F8E10;
        mObj3c->mbAirborne = 0;
        *pPoint = pSprite->get_x_y();
        *pHitWall = true;
        Object::ComputeWallHitSide();
        return true;
    }
    else if (Object::SelectCollisionSprite(pSprite))
    {
        *pHitFloorBelow = true;
        if (mDefinition->mZCollisionResponse == 4)
        {
            return true;
        }
        else if (sObject2CHitWallOrDoor)
        {
            mObj3c->mSavedSpeed = mObj3c->mSpeed.mValue;
            *pPoint = pSprite->get_x_y();
            *pHitWall = true;
            return true;
        }
        else
        {
            Object_3C* pObj3C = mObj3c;
            if (pObj3C->mSpriteCollideMode == 2)
            {
                pObj3C->mSpriteCollideMode = 1;
                ApplyDefinitionDrawLayer_482BE0();
                mObj3c->mZSpeed = kObjectBounceRestitution * (-mObj3c->mZSpeed);
            }
            return true;
        }
    }
    else
    {
        return false;
    }
}

MATCH_FUNC(0x523BF0)
void Object::IntegrateMovementAndCollisions(Fix16 mov_speed, Ang16 angle)
{
    // Declaration order matters: the scalars are initialised before the two points are
    // constructed (EH state 1 is set after the byte stores)
    Ang16 prev_rotation(0);
    Fix16 mov_speed_copy = mov_speed;
    bool bHitSomething = false;
    bool bHitBlockAtZ = false;
    bool bHitWall = false;
    bool bHandled = false;
    Fix16_Point point;
    Fix16_Point point2;
    Fix16 step_count;
    Sprite* pSprt = gObject_5C->mCollisionProbeSprite;
    sObject2CWallHitSide = 0;
    ApplyDefinitionDrawLayer_482BE0();
    if (Object::TryStartSlide(mov_speed))
    {
        mov_speed_copy = mObj3c->mSpeed;
    }
    if (mov_speed_copy != kFpZero_6F8E10 || mObj3c->mbAirborne || mObj3c->mZSpeed != kFpZero_6F8E10)
    {
        gCollisionTarget.Reset_4637B0();
        pSprt->set_xyz_lazy_420600(mSprite->mXy.x, mSprite->mXy.y, mSprite->mZpos);
        pSprt->set_ang_lazy_420690(mSprite->mRotation);
        pSprt->AllocCollisionShape(mDefinition->mWidth, mDefinition->mHeight, mDefinition->mDepth);
        pSprt->SetType_4206F0(mSprite->GetType_416B40());
        pSprt->SetObject2C_482A30(mSprite->mObject2CPtr);
        if (gMap_0x370->IsGradientSlopeAt_466CF0(pSprt->mXy.x.ToInt(), pSprt->mXy.y.ToInt(), pSprt->mZpos.ToInt()))
        {
            mObj3c->mbOnSlope = true;
        }
        else
        {
            mObj3c->mbOnSlope = false;
        }
        Fix16 radius;
        Fix16 step_dz;
        if (mov_speed_copy != kFpZero_6F8E10)
        {
            step_count = mov_speed_copy / mDefinition->mMinSize;
            radius = mov_speed_copy / step_count;
            step_dz = mObj3c->mZSpeed / step_count;
        }
        else
        {
            step_count = kFpZero_6F8E10;
            radius = kFpZero_6F8E10;
        }

        if (step_count < kFP16One_6F8E14)
        {
            step_count = kFP16One_6F8E14;
            radius = mov_speed_copy;
            step_dz = mObj3c->mZSpeed;
        }

        Fix16 step_dx;
        Fix16 step_dy;
        Fix16 prev_x;
        Fix16 prev_y;
        Fix16 prev_z;
        {
            // The original passes a block-scoped copy of radius (9.6f copies it too); its slot is
            // shared with tmp_radius below
            Fix16 r = radius;
            Ang16::PolarToCartesian_41FC20(angle, r, step_dx, step_dy);
        }

        for (u8 i = 1; i <= step_count.ToInt(); i++)
        {
            prev_x = pSprt->mXy.x;
            prev_y = pSprt->mXy.y;
            prev_z = pSprt->mZpos;
            prev_rotation = pSprt->mRotation;
            Fix16 found_z;
            found_z = gMap_0x370->FindGroundZBelowCoord(pSprt->mXy.x, pSprt->mXy.y, pSprt->mZpos);
            pSprt->set_xyz_lazy_420600(pSprt->mXy.x + step_dx, pSprt->mXy.y + step_dy, pSprt->mZpos + step_dz);
            pSprt->set_ang_lazy_420690(angle);
            if (step_dz == kFpZero_6F8E10)
            {
                bHandled = Object::HandleSpriteGroundAndCollision(pSprt, &point, &bHitWall, found_z);
            }
            else if (step_dz > kFpZero_6F8E10)
            {
                bHandled = Object::HandleSpriteGroundAndCollisionSimple(pSprt, &point, &bHitWall, &bHitBlockAtZ);
            }
            else
            {
                bHandled = Object::HandleSpriteZCollision(pSprt, &point, &bHitWall, &bHitSomething, found_z);
            }
        }

        if (bHitSomething)
        {
            gCollisionSoundQueue->AddFloorCollision(mSprite);
        }
        if (!bHandled)
        {
            mSprite->set_xyz_lazy_420600(pSprt->mXy.x, pSprt->mXy.y, pSprt->mZpos);
            mSprite->set_ang_lazy_420690(pSprt->mRotation);
            if (bHitSomething)
            {
                if (static_cast<u8>(mSprite->IsOnWater()))
                {
                    mObj3c->mSpeed = kFpZero_6F8E10;
                    mObj3c->mZSpeed = kFpZero_6F8E10;
                }
                gCollisionSoundQueue->AddFloorCollision(mSprite);
            }
            else if (bHitBlockAtZ)
            {
                gCollisionSoundQueue->AddBlockCollision(mSprite);
            }
        }
        else
        {
            sObj2CLastValidX = prev_x;
            sObj2CLastValidY = prev_y;
            gObj2C_LastValidZ = prev_z;
            sObj2CLastValidAng = prev_rotation;
            Fix16 tmp_radius = radius;
            pSprt->set_xyz_lazy_420600(sObj2CLastValidX, sObj2CLastValidY, gObj2C_LastValidZ);
            pSprt->set_ang_lazy_420690(prev_rotation);
            Object::Sprite_UpdateZFromSlopeAndTile(pSprt);
            bool bLastStepClear;

            for (s32 j = 0; j < 3; j++)
            {
                tmp_radius = tmp_radius / 2;
                Ang16::PolarToCartesian_41FC20(angle, tmp_radius, step_dx, step_dy);
                pSprt->set_xyz_lazy_420600(pSprt->mXy.x + step_dx, pSprt->mXy.y + step_dy, pSprt->mZpos);
                pSprt->set_ang_lazy_420690(angle);
                Object::Sprite_UpdateZFromSlopeAndTile(pSprt);
                bool bHitSprite = false;
                if (mObj3c->mSpriteCollideMode == 2 && Object::SelectCollisionSprite(pSprt))
                {
                    bHitSprite = true;
                }
                if (pSprt->CheckSpriteMovementRegion() || bHitSprite)
                {
                    point = pSprt->get_x_y_443580();
                    pSprt->set_xyz_lazy_420600(sObj2CLastValidX, sObj2CLastValidY, gObj2C_LastValidZ);
                    pSprt->set_ang_lazy_420690(sObj2CLastValidAng);
                    bLastStepClear = false;
                }
                else
                {
                    bLastStepClear = true;
                    sObj2CLastValidX = pSprt->mXy.x;
                    sObj2CLastValidY = pSprt->mXy.y;
                    gObj2C_LastValidZ = pSprt->mZpos;
                    sObj2CLastValidAng = pSprt->mRotation;
                }
            }
            if (!bLastStepClear)
            {
                pSprt->set_xyz_lazy_420600(sObj2CLastValidX, sObj2CLastValidY, gObj2C_LastValidZ);
                pSprt->set_ang_lazy_420690(sObj2CLastValidAng);
            }
            mSprite->set_xyz_lazy_420600(pSprt->mXy.x, pSprt->mXy.y, pSprt->mZpos);
            mSprite->set_ang_lazy_420690(pSprt->mRotation);
            if (bHitSomething || bHitWall || bHitBlockAtZ)
            {
                Object::HandleCollisionOutcome(point, bHitSomething, bHitBlockAtZ);
            }
        }

        Object::TryStartSlide(mov_speed);
        if (mSprite->mXy.x < kFpZero_6F8E10 || mSprite->mXy.x >= kMapSize || mSprite->mXy.y < kFpZero_6F8E10 ||
            mSprite->mXy.y >= kMapSize)
        {
            Object::RequestRemoval();
        }
    }
}

MATCH_FUNC(0x524550)
void Object::ComputeWallHitSide()
{
    if (gCollisionTarget.mType == collision_target_type::horizontal_edge_1)
    {
        if (mObj3c->mAngle >= kAng270_6F8D88 || mObj3c->mAngle <= kAng90_6F8C88)
        {
            sObject2CWallHitSide = 3;
        }
        else
        {
            sObject2CWallHitSide = 4;
        }
    }
    else if (gCollisionTarget.mType == collision_target_type::vertical_edge_2)
    {
        if (mObj3c->mAngle >= kAng180_6F8D62)
        {
            sObject2CWallHitSide = 2;
        }
        else
        {
            sObject2CWallHitSide = 1;
        }
    }
}

MATCH_FUNC(0x524630)
void Object::IntegrateHorizontalMovementAndCollisions(Fix16 move_len, Ang16 move_angle)
{
    bool bOnSlope = false;
    Fix16_Point hitPoint;
    Sprite* pProbe = gObject_5C->mCollisionProbeSprite;
    sObject2CLastCollisionSprite = NULL;
    if (move_len == kFpZero_6F8E10 && !this->mObj3c->mbAirborne)
    {
        return;
    }
    gCollisionTarget.Reset_4637B0();

    pProbe->set_xyz_lazy_420600(mSprite->mXy.x, mSprite->mXy.y, mSprite->mZpos);
    pProbe->set_ang_lazy_420690(mSprite->mRotation);
    pProbe->AllocCollisionShape(this->mDefinition->mWidth, this->mDefinition->mHeight, this->mDefinition->mDepth);
    pProbe->SetType_4206F0(this->mSprite->GetType_416B40());
    pProbe->SetObject2C_482A30(this->mSprite->mObject2CPtr);

    Fix16 step_count;
    Fix16 step_len;
    if (move_len != kFpZero_6F8E10)
    {
        step_count = (move_len) / this->mDefinition->mMinSize;
        step_len = (move_len) / step_count;
    }
    else
    {
        step_count = kFpZero_6F8E10;
        step_len = kFpZero_6F8E10;
    }

    if (step_count < kFP16One_6F8E14)
    {
        step_count = kFP16One_6F8E14;
        step_len = move_len;
    }

    Fix16 step_dx;
    Fix16 step_dy;
    {
        // 9.6f: PolarToCartesian_41FC20 on a copy of the step
        Fix16 radius = step_len;
        step_dx = Ang16::sine_40F500(move_angle) * radius;
        step_dy = Ang16::cosine_40F520(move_angle).Multiply(radius);
    } // 9.6f inlined: PolarToCartesian_41FC20

    Ang16 start_rotation;
    Fix16 start_y;
    Fix16 start_x;

    s32 num_steps = step_count.ToInt();
    // The while (1) never iterates (the body returns), but its dead exit edge makes VC6 visit the LABEL_48
    // code first in its reverse-postorder layout, which puts the no-hit tail between the hit blocks and the
    // half-step loop; a rotated for/while test gives the `jle top; jmp tail` latch. Both are needed to match.
    while (1)
    {
        for (u8 step_idx = 1; step_idx <= num_steps; step_idx++)
        {
            start_x = pProbe->mXy.x;
            start_y = pProbe->mXy.y;
            start_rotation = pProbe->mRotation;
            mObj3c->mbAirborne = 0;

            if (gMap_0x370->IsGradientSlopeAt_466CF0(pProbe->mXy.x.ToInt(), pProbe->mXy.y.ToInt(), pProbe->mZpos.ToInt()))
            {
                bOnSlope = true;
            }

            pProbe->set_xy_lazy_447E20(pProbe->mXy.x + step_dx, pProbe->mXy.y + step_dy);
            pProbe->set_ang_lazy_420690(move_angle);

            if (gMap_0x370->IsGradientSlopeAt_466CF0(pProbe->mXy.x.ToInt(), pProbe->mXy.y.ToInt(), pProbe->mZpos.ToInt()))
            {
                Sprite_UpdateZFromSlopeAndTile(pProbe);
            }

            if (!gMap_0x370->GetBlockTypeAtCoord_420420(pProbe->mXy.x.ToInt(),
                                                               pProbe->mXy.y.ToInt(),
                                                               (pProbe->mZpos - kFP16One_6F8E14).ToInt()))
            {
                if (gMap_0x370->IsGradientSlopeAt_466CF0(pProbe->mXy.x.ToInt(),
                                                                pProbe->mXy.y.ToInt(),
                                                                (pProbe->mZpos - kFP16One_6F8E14).ToInt()))
                {
                    Sprite_UpdateZFromSlopeAndTile(pProbe);
                }
            }

            if (pProbe->CheckSpriteMovementRegion())
            {
                if (bOnSlope == 1)
                {
                    pProbe->mZpos = pProbe->mZpos.GetFloorValue();
                    if (pProbe->mZpos > kFP16Half_6F8D10)
                    {
                        pProbe->mZpos++;
                    }

                    if (pProbe->CheckSpriteMovementRegion() || SelectCollisionSprite(pProbe))
                    {
                        hitPoint = pProbe->get_x_y();
                        ComputeWallHitSide();
                        goto LABEL_48;
                    }
                }
                else
                {
                    hitPoint = pProbe->get_x_y();
                    ComputeWallHitSide();
                    goto LABEL_48;
                }
            }
            else if (SelectCollisionSprite(pProbe))
            {
                hitPoint = pProbe->get_x_y();
                goto LABEL_48;
            }
        }
        mSprite->set_xyz_lazy_420600(pProbe->mXy.x, pProbe->mXy.y, pProbe->mZpos);
        mSprite->set_ang_lazy_420690(pProbe->mRotation);
        return;
    }

LABEL_48:
    Fix16 step = step_len;

    pProbe->set_xy_lazy_447E20(start_x, start_y);
    pProbe->set_ang_lazy_420690(start_rotation);

    Sprite_UpdateZFromSlopeAndTile(pProbe);
    s32 tries = 3;
    do
    {
        Fix16 old_x = pProbe->mXy.x;
        Fix16 old_y = pProbe->mXy.y;
        start_rotation = pProbe->mRotation;
        step = step / 2;
        step_dx = Ang16::sine_40F500(move_angle) * step;
        step_dy = Ang16::cosine_40F520(move_angle) * step;
        pProbe->set_xy_lazy_447E20(pProbe->mXy.x + step_dx, pProbe->mXy.y + step_dy);
        pProbe->set_ang_lazy_420690(move_angle);

        if (pProbe->CheckSpriteMovementRegion() || SelectCollisionSprite(pProbe))
        {
            hitPoint = pProbe->get_x_y();
            pProbe->set_xy_lazy_447E20(old_x, old_y);
            pProbe->set_ang_lazy_420690(start_rotation);
            Sprite_UpdateZFromSlopeAndTile(pProbe);
        }
    } while (--tries);

    mSprite->set_xyz_lazy_420600(pProbe->mXy.x, pProbe->mXy.y, pProbe->mZpos);
    mSprite->set_ang_lazy_420690(pProbe->mRotation);

    switch (this->mDefinition->mCollisionResponse)
    {
        case 0:
        case 1:
            HandleImpact(gCollisionTarget.mpHitSprite);
            break;
        case 2:
        case 3:
            HandleCollision(&hitPoint);
            break;
        default:
            return;
    }
}

MATCH_FUNC(0x525100)
void Object::OnAttachedSpriteDestroyed()
{
    const s32 behavior = mDefinition->mBehaviorType;
    if (behavior >= object_behavior_type::static_object_0 && behavior <= object_behavior_type::static_collision_check_1)
    {
        if (mModel != objects::tank_cannon_148)
        {
            RequestRemoval();
        }
        else
        {
            SetSpriteIdxOffset(1u);
        }
    }
}

// https://decomp.me/scratch/jLuSq
MATCH_FUNC(0x525190)
void Object::OnExplosionNearby(u8 ped_ref_idx)
{

    if (IsExplosiveItemDefinition_482400(mDefinition->mNextDefinitionIdx))
    {
        PoolGiveAndQueueObjectType(mDefinition->mNextDefinitionIdx);
    }
    else if (mDefinition->mProjectileHitType == 13)
    {
        PoolGiveAndMarkDone();
        Object* pExplosion = gObject_5C->CreateExplosion(this->mSprite->mXy.x,
                                                                          this->mSprite->mXy.y,
                                                                          this->mSprite->mZpos,
                                                                          kZeroAng_6F8F68,
                                                                          explosion_type::item_19,
                                                                          gPedRefTable_7F8->GetPedId_420F10(ped_ref_idx));
        if (pExplosion)
        {
            pExplosion->SetDamageOwner(ped_ref_idx);
        }
    }
}

// Turns traffic lights into collision objects so AI stops at red lights
MATCH_FUNC(0x525290)
bool Object::ShouldStopAtTrafficLight(Sprite* pSprite) const
{
    Car* pCar = pSprite->AsCar_40FEB0();
    if (pCar)
    {
        if (gCarAIProbingAhead)
        {
            if ((mSprite->mRotation != kZeroAng_6F8F68 || gTrafficLights_194->mPhase != traffic_light_phase::north_south_green_1) &&
                (mSprite->mRotation != kAng90_6F8C88 || gTrafficLights_194->mPhase != traffic_light_phase::east_west_green_4))
            {
                return true;
            }
        }
    }
    return false;
}

MATCH_FUNC(0x525370)
bool Object::ShouldCollideWithSprite_525370(Sprite* pSprite) const
{
    switch (this->mModel)
    {
        case objects::pedestrian_crossing_marker_122:
            if (pSprite)
            {
                return ShouldStopAtTrafficLight(pSprite);
            }
            break;

        case objects::conveyor_139:
            break;

        case objects::door_collision_obj_169:
            if (pSprite)
            {
                return gDoor_4D4->CheckDoorAccess(pSprite, this->GetParam_421050());
            }
            else
            {
                return gDoorAccessCheckActive;
            }

        case objects::tunnel_blocker_294:
            if (pSprite)
            {
                Car* pCar = pSprite->AsCar_40FEB0();
                if (pCar)
                {
                    return !pCar->IsTrainModel_403BA0();
                }
            }
            // Fall through
        default:
            if (gCollisionCounters->mbDoorAccessCheck == 1)
            {
                if (CanBePushed_482C90())
                {
                    return false;
                }
            }

            if (mObj3c)
            {
                if (mObj3c->mSpriteCollideMode != 2)
                {
                    return false;
                }

                if (pSprite)
                {
                    if (pSprite->GetType_416B40() != sprite_types_enum::ped_3)
                    {
                        if (mDefinition->mCollisionResponse == 3)
                        {
                            return false;
                        }

                        if (mDefinition->mCollisionResponse == 0)
                        {
                            return false;
                        }
                    }
                }
                else
                {
                    if (mDefinition->mCollisionResponse == 3)
                    {
                        return false;
                    }
                    if (mDefinition->mCollisionResponse == 0)
                    {
                        return false;
                    }
                }
            }

            if (pSprite)
            {
                Object* pObject = pSprite->AsObject2C_40FEC0();
                if (pObject)
                {
                    if (pObject->mDefinition->mTouchBehavior == 7 || pObject->mDefinition->mTouchBehavior == 8)
                    {
                        switch (mDefinition->mProjectileHitType)
                        {
                            case 0:
                                return false;
                            case 12:
                                return true;
                            case 13:
                                return true;
                        }
                    }

                    if (pObject->mModel == objects::conveyor_139)
                    {
                        break;
                    }

                    // Only pickups collide with pickups
                    if (pObject->IsPickup_421060() && !IsPickup_421060())
                    {
                        return false;
                    }
                }
            }
            return true;
    }
    return false;
}

MATCH_FUNC(0x5257d0)
void Object::UpdateAnimation()
{
    ++this->mpAny.o8->mFrameCounter;
    if (mpAny.o8->mFrameCounter >= this->mDefinition->mNextFrameMax)
    {
        mpAny.o8->mFrameCounter = 0;
        ++this->mpAny.o8->mFrameIdx;
        if (mpAny.o8->mFrameIdx >= this->mDefinition->mNumFrames)
        {
            mpAny.o8->mFrameIdx = 0;
            if (mpAny.o8->mTimer > 0)
            {
                mpAny.o8->mTimer--;
            }
        }
        const s16 target_id = this->mDefinition->mBaseSpriteIdx + this->mpAny.o8->mFrameIdx;

        mSprite->set_id_lazy_4206C0(target_id);

        if (IsAnimFinished_482C10())
        {
            Object::TickObject(this->mDefinition->mNextDefinitionIdx);
        }
    }
}

MATCH_FUNC(0x525910)
bool Object::DispatchFrameAction()
{
    if (mTouchedResult)
    {
        switch (mDefinition->mTouchBehavior)
        {
            case 1:
            case 2:
            case 5:
            case 6:
            case 8:
            case 11:
                if (mTouchedResult == 1)
                {
                    TickObject(mDefinition->mImpactDefinitionIdx);
                }
                else
                {
                    TickObject(mTouchedResult);
                }
                mTouchedResult = 0;
                return true;
            case 4:
                if (mTouchedResult != 1)
                {
                    TickObject(mTouchedResult);
                }
                mTouchedResult = 0;
                return true;
            case 7:
            case 10:
                mTouchedResult = 0;
                RequestRemoval();
                return true;
            default:
                mTouchedResult = 0;
                return false;
        }
    }
    else if (mDefinition->mTouchBehavior == 3 || mDefinition->mTouchBehavior == 4)
    {
        TickObject(mDefinition->mImpactDefinitionIdx);
        return true;
    }

    return false;
}

MATCH_FUNC(0x525AC0)
bool Object::IsFireExplosion() const
{
    if (mModel == objects::explosion_113)
    {
        return mpAny.pExplosion->IsFireType();
    }
    else
    {
        return false;
    }
}

MATCH_FUNC(0x525AE0)
void Object::CheckCollisionForModel_139_And_141()
{
    switch (mModel)
    {
        case objects::conveyor_139:
        case objects::destructor_141:
            gSpriteGrid_1->CheckAndHandleCollisionInStrips(mSprite);
            gSpriteGrid_2->CheckAndHandleCollisionInStrips(mSprite);
            break;
        default:
            break;
    }
}

MATCH_FUNC(0x525B20)
void Object::UpdateEffectPool()
{
    if (mObj3c)
    {
        if (mObj3c->mAttachedSprites.mP18)
        {
            mObj3c->mAttachedSprites.PoolUpdate(mSprite);
        }
    }
}

MATCH_FUNC(0x525b40)
void Object::SpawnSpriteParticlesForRocketBullet()
{
    if (mModel == objects::rocket_bullet_128)
    {
        gParticle_8->SpawnParticleSprite(mSprite);
    }
}

MATCH_FUNC(0x525B60)
bool Object::CheckWaterDeath()
{
    // The original tests only al, though IsOnWater sets all of eax
    const u8 isWater = mSprite->IsOnWater();
    if (isWater)
    {
        HandleWaterDeath();
        return true;
    }
    return false;
}

MATCH_FUNC(0x525b80)
void Object::UpdatePhysicsAndMovement()
{
    Ang16 ang;

    UpdatePhysics();
    Fix16 mov_speed;
    mObj3c->GetMovementSpeedAndAngle(mov_speed, ang);

    if (!mDefinition->mUses3dMovement)
    {
        IntegrateHorizontalMovementAndCollisions(mov_speed, ang);
    }
    else
    {
        IntegrateMovementAndCollisions(mov_speed, ang);
    }
    SpawnSpriteParticlesForRocketBullet();

    mObj3c->mSpeed = mObj3c->mSpeed.ZeroIfNegligible_482730();

    if (!DispatchFrameAction())
    {
        if (mObj3c->mAttachedSprites.mP18)
        {
            mObj3c->mAttachedSprites.PoolUpdate(mSprite);
        }

        if (mov_speed != kFpZero_6F8E10 || (mObj3c->mbAirborne) || mObj3c->mSpriteCollideMode == 2 || CheckWaterDeath())
        {
            if (mObj3c->mNextDefinitionTimer > 0)
            {
                mObj3c->mNextDefinitionTimer--;
                if (mObj3c->mNextDefinitionTimer == 0)
                {
                    if (GetPedRefIdx_420FF0() > 0)
                    {
                        s32 ped_id = gPedRefTable_7F8->GetPedId_420F10(GetPedRefIdx_420FF0());
                        if (ped_id)
                        {
                            Ped* pPed = gPedManager->PedById(ped_id);
                            if (pPed)
                            {
                                pPed->HandleWeaponFireEnd(mModel);
                            }
                        }
                    }
                    TickObject(mDefinition->mNextDefinitionIdx);
                }
            }
        }
        else
        {
            this->mObj3c->mZAccel = kFpZero_6F8E10;
            this->mObj3c->mZSpeed = kFpZero_6F8E10;

            TickObject(mDefinition->mImpactDefinitionIdx);
            if (UsesParamAsPedRef_421080())
            {
                if (GetPedRefIdx_420FF0() > 0)
                {
                    gPedRefTable_7F8->DecrementRefCount(mPedRefIdx);
                    this->mPedRefIdx = 0;
                }
            }
        }
    }
}

MATCH_FUNC(0x525d90)
void Object::UpdatePhysicsMovementAndAnimation()
{
    Ang16 ang;
    Sprite* pSprite = this->mSprite;
    Fix16 x_val = pSprite->mXy.x;
    Fix16 y_val = pSprite->mXy.y;
    Fix16 zpos = pSprite->mZpos;

    UpdatePhysics();

    Fix16 mov_speed;
    mObj3c->GetMovementSpeedAndAngle(mov_speed, ang);

    if (!mDefinition->mUses3dMovement)
    {
        IntegrateHorizontalMovementAndCollisions(mov_speed, ang);
    }
    else
    {
        IntegrateMovementAndCollisions(mov_speed, ang);
    }

    mObj3c->mSpeed = mObj3c->mSpeed.ZeroIfNegligible_482730();

    ObjectDefinition* pDefinition = this->mDefinition;
    if (pDefinition->mNextDefinitionTimer != -1 || pDefinition->mBehaviorType == object_behavior_type::pickup_physics_animated_9 || (mSprite->mXy.x != x_val) || mSprite->mXy.y != y_val ||
        mSprite->mZpos != zpos)
    {
        if (!this->mObj3c->mbSkipAnim)
        {
            UpdateAnimation();
        }
    }

    if (!DispatchFrameAction())
    {
        if (mObj3c->mAttachedSprites.mP18)
        {
            mObj3c->mAttachedSprites.PoolUpdate(mSprite);
            if (!mObj3c->mAttachedSprites.mP18 && mModel == objects::explosion_fragments_127)
            {
                RequestRemoval();
            }
        }

        if (mov_speed == kFpZero_6F8E10)
        {
            if (!mObj3c->mbAirborne && mObj3c->mSpriteCollideMode == 2 && !CheckWaterDeath() && !sObject2CLastCollisionSprite)
            {
                mObj3c->mZAccel = kFpZero_6F8E10;
                mObj3c->mZSpeed = kFpZero_6F8E10;

                if (mModel != objects::grenade_obj_183)
                {
                    if (UsesParamAsPedRef_421080())
                    {
                        // 9.6f: Object::GetPedRefIdx_420FF0 (inlined, using it changes the code)
                        if (mPedRefIdx > 0)
                        {
                            gPedRefTable_7F8->DecrementRefCount(mPedRefIdx);
                            this->mPedRefIdx = 0;
                        }
                    }
                }
                TickObject(mDefinition->mImpactDefinitionIdx);
            }
        }
    }
}

MATCH_FUNC(0x525f30)
void Object::Update()
{
    Explosion* pExplosion;

    sObject2CLastCollisionSprite = NULL;
    while (2)
    {
        switch (this->mDefinition->mBehaviorType)
        {
            case object_behavior_type::static_object_0:
                if (!DispatchFrameAction())
                {
                    UpdateEffectPool();
                    return;
                }
                return;
            case object_behavior_type::static_collision_check_1:
                if (!DispatchFrameAction())
                {
                    UpdateEffectPool();
                    CheckCollisionForModel_139_And_141();
                }
                return;

            case object_behavior_type::explosion_5:
                pExplosion = this->mpAny.pExplosion;
                if (pExplosion)
                {
                    if (pExplosion->Update(kFpZero_6F8E10, kZeroAng_6F8F68))
                    {
                        this->mRemovalState = object_removal_state::requested_1;
                    }
                }
                return;

            case object_behavior_type::physics_3:
            case object_behavior_type::pickup_physics_7:
                RemoveFromCollisionBuckets();
                sObjectRemovedFromBuckets = 1;
                UpdatePhysicsAndMovement();
                if (!sObjectReBucketed)
                {
                    AssignToBucket();
                }
                return;

            case object_behavior_type::animated_2:
            case object_behavior_type::pickup_animated_8:
                if (!DispatchFrameAction())
                {
                    UpdateEffectPool();
                    UpdateAnimation();
                }
                return;

            case object_behavior_type::physics_animated_4:
            case object_behavior_type::pickup_physics_animated_9:
                RemoveFromCollisionBuckets();
                sObjectRemovedFromBuckets = 1;
                UpdatePhysicsMovementAndAnimation();
                if (!sObjectReBucketed)
                {
                    AssignToBucket();
                }
                return;

            case object_behavior_type::pickup_6:
            case object_behavior_type::shop_10:
            case object_behavior_type::light_11:
                DispatchFrameAction();
                UpdateEffectPool();
                return;

            case object_behavior_type::effect_only_12:
                UpdateEffectPool();
                return;

            default:
                continue;
        }
    }
}

MATCH_FUNC(0x5263d0)
bool Object::PoolUpdate()
{
    sObjectRemovedFromBuckets = 0;
    sObjectReBucketed = 0;
    if (mRemovalState != object_removal_state::requested_1)
    {
        Object::Update();
    }

    if (mRemovalState <= object_removal_state::none_0 || (mRemovalState == object_removal_state::when_off_screen_2 && gGame_0x40->IsSpriteInAnyPlayerBounds(mSprite, kFpZero_6F8E10)))
    {
        return false;
    }
    else
    {
        Object::RemoveFromCollisionBuckets();
        return true;
    }
}

// 9.6f 0x4837F0
MATCH_FUNC(0x526790)
void Object::TriggerCarExplosionIfApplicable(Sprite* pSprite)
{
    ObjectDefinition* pDefinition = this->mDefinition;
    if (IsAnimatedBehavior_475A80())
    {
        if (IsAnimFinished_482C10())
        {
            if (IsExplosiveItemDefinition_482400(pDefinition->mNextDefinitionIdx))
            {
                Car* pCar = pSprite->AsCar_40FEB0();
                if (pCar)
                {
                    if (this->mModel == objects::car_bomb_132)
                    {
                        s32 ped_id = gPedRefTable_7F8->GetPedId_420F10(GetPedRefIdx_420FF0());
                        if (ped_id)
                        {
                            pCar->mExploderPedId = ped_id;
                            pCar->mDeathCause = ped_death_cause::bomb_12;
                            pCar->mExploderTimer = 50;
                        }
                    }
                    s32 explosion_kind = GetExplosionTypeForItemDefinition(mDefinition->mNextDefinitionIdx);
                    pCar->HandleCarExplosion(explosion_kind);
                }
            }
        }
    }
}

MATCH_FUNC(0x526830)
s32 __stdcall Object::GetExplosionTypeForItemDefinition(s32 type)
{
    switch (type)
    {
        case 39:
            return explosion_type::small_18;
        case 40:
            return explosion_type::small_33;
        case 41:
            return explosion_type::item_19;
        case 42:
            return explosion_type::large_20;
    }
    // The original reloads type from its stack slot here instead of keeping it in a register
    return *static_cast<volatile s32*>(&type);
}

MATCH_FUNC(0x526b40)
void Object::InheritMotionFromSprite(Sprite* pSprite)
{
    Object_3C* pObject3C;

    switch (pSprite->GetType_416B40())
    {
        case sprite_types_enum::ped_3:
            this->mObj3c->mSpeed = (pSprite->mCharB4Ptr->get_velocity_41B080() * kFpOne64th_6F8C9C);
            this->mObj3c->mAngle = pSprite->mRotation;
            this->mSprite->SetDrawLayer_40F7B0(27);
            break;

        case sprite_types_enum::car_2:
            this->mObj3c->mSpeed = pSprite->mCarBcPtr->GetCarLinearSpeed();
            this->mObj3c->mAngle = pSprite->mCarBcPtr->GetOrientationAngle();
            this->mSprite->SetDrawLayer_40F7B0(pSprite->AsCar_40FEB0()->GetAttachedDrawLayer());
            break;

        case sprite_types_enum::code_obj1_4:
        case sprite_types_enum::map_obj_5:
            pObject3C = pSprite->mObject2CPtr->mObj3c;
            if (pObject3C)
            {
                this->mObj3c->mSpeed = pObject3C->mSpeed;
                this->mObj3c->mAngle = pSprite->mObject2CPtr->mObj3c->mAngle;
                this->mObj3c->mZSpeed = pSprite->mObject2CPtr->mObj3c->mZSpeed;
            }
            this->mSprite->SetDrawLayer_40F7B0(12);
            break;

        default:
            return;
    }
}

// https://decomp.me/scratch/zqssg
MATCH_FUNC(0x527070)
bool Object::UpdateMovementAndEffects(Sprite* pSprite, Fix16 x, Fix16 y, Ang16 rot)
{
    sInObjectMovementUpdate = 1;

    if (mObj3c)
    {
        InheritMotionFromSprite(pSprite);
    }

    if (mDefinition->mBehaviorType != object_behavior_type::light_11)
    {
        gSpriteGrid_3->Remove(mSprite);
    }

    PoolTake();

    if (!mRemovalState)
    {
        Fix16 xpos;
        Fix16 ypos;
        if (x == kFpZero_6F8E10 && y == kFpZero_6F8E10)
        {
            xpos = pSprite->mXy.x;
            ypos = pSprite->mXy.y;
        }
        else
        {
            xpos = x;
            ypos = y;
            Ang16::RotateVector_41FC90(xpos, ypos, pSprite->mRotation);
            xpos += pSprite->mXy.x;
            ypos += pSprite->mXy.y;
        }

        mSprite->set_xyz_lazy_420600(xpos, ypos, pSprite->mZpos);
        mSprite->set_ang_lazy_420690(pSprite->mRotation + rot);

        switch (mDefinition->mBehaviorType)
        {
            case object_behavior_type::animated_2:
                UpdateAnimation();
                break;

            case object_behavior_type::explosion_5:
            {
                if (mpAny.pExplosion)
                {
                    switch (pSprite->GetType_416B40())
                    {
                        case sprite_types_enum::code_obj1_4:
                        case sprite_types_enum::map_obj_5:
                            if (mpAny.pExplosion->Update(mObj3c->mSpeed, mObj3c->mAngle))
                            {
                                sInObjectMovementUpdate = 0;
                                return true;
                            }
                            break;

                        case sprite_types_enum::car_2:
                            if (mpAny.pExplosion->IsFireType())
                            {
                                mSprite->set_z_lazy_420660(pSprite->mCarBcPtr->GetZPos());
                                pSprite->mCarBcPtr->AccumulateDamage(1, &gZeroVector);
                            }

                            mpAny.pExplosion->mpAttachedSprite = pSprite;

                            if (mpAny.pExplosion->Update(mObj3c->mSpeed, mObj3c->mAngle))
                            {
                                sInObjectMovementUpdate = 0;
                                return true;
                            }
                            break;

                        case sprite_types_enum::ped_3:
                            if (mpAny.pExplosion->Update(mObj3c->mSpeed, mObj3c->mAngle))
                            {
                                sInObjectMovementUpdate = 0;
                                return true;
                            }
                            break;

                        default:
                            break;
                    }
                }
                break;
            }

            case object_behavior_type::light_11:
                mpAny.pLight->SetPosition_482D30(mSprite->mXy.x, mSprite->mXy.y, mSprite->mZpos);
                break;

            default:
                break;
        }

        if (mObj3c && mObj3c->mAttachedSprites.mP18)
        {
            mObj3c->mAttachedSprites.PoolUpdate(mSprite);
        }

        if (mDefinition->mBehaviorType != object_behavior_type::light_11)
        {
            gSpriteGrid_3->AddToSingleBucket_477AE0(mSprite);
        }
        sInObjectMovementUpdate = 0;
        return false;
    }
    else
    {
        TriggerCarExplosionIfApplicable(pSprite);
        sInObjectMovementUpdate = 0;
        return true;
    }
}

MATCH_FUNC(0x527630)
void Object::InitializeObject(s32 object_type, Fix16 xpos, Fix16 ypos, Fix16 zpos, Ang16 rotation)
{
    ObjectDefinition* pDefinition = gObjectDefinitions->GetObjectDefinition(object_type);
    mDefinition = pDefinition;
    mModel = object_type;
    mTouchedResult = 0;

    if (mSprite)
    {
        pDefinition->ApplyDefinitionToSprite(mSprite);
    }
    else
    {
        mSprite = pDefinition->CreateSpriteFromDefinition();
    }

    mSprite->set_xyz_lazy_420600(xpos, ypos, zpos);
    mSprite->set_ang_lazy_420690(rotation);

    mSprite->SetObject2C_482A30(this);
}

MATCH_FUNC(0x527990)
void Object::TurnOffLight()
{
    mpAny.pLight->mIntensityRadius.flag &= ~0xFF;
}

MATCH_FUNC(0x527A30)
void Object::UpdateLight()
{
    mpAny.pLight->SetCurrentIntensity_45B2D0(mpAny.pLight->mIntensity);
}

MATCH_FUNC(0x527ae0)
void Object::AssignToBucket()
{
    switch (mDefinition->mCollisionBucketCategory)
    {
        case collision_bucket_category::sprite_grid_3_single_cell_0:
        case collision_bucket_category::sprite_grid_3_single_cell_1:
            gSpriteGrid_3->AddToSingleBucket_477AE0(mSprite);
            return;
        case collision_bucket_category::sprite_grid_2_region_3:
            sObjectCounterUnused++;
            gSpriteGrid_2->AddToRegionBuckets(mSprite);
            return;
        case collision_bucket_category::sprite_grid_1_region_4:
            gSpriteGrid_1->AddToRegionBuckets(mSprite);
            return;
        case collision_bucket_category::none_2:
            return;
    }
}

MATCH_FUNC(0x527d00)
void Object::RemoveFromCollisionBuckets()
{
    switch (mDefinition->mCollisionBucketCategory)
    {
        case collision_bucket_category::sprite_grid_3_single_cell_0:
        case collision_bucket_category::sprite_grid_3_single_cell_1:
            gSpriteGrid_3->Remove(mSprite);
            break;
        case collision_bucket_category::sprite_grid_2_region_3:
            --sObjectCounterUnused;
            gSpriteGrid_2->RemoveFromRegionBuckets(mSprite);
            break;
        case collision_bucket_category::sprite_grid_1_region_4:
            gSpriteGrid_1->RemoveFromRegionBuckets(mSprite);
            break;
        default:
            return;
    }
}

// 9.6f 0x484760
MATCH_FUNC(0x527f10)
void Object::ReleaseSubObjects()
{

    if (mpAny.o8)
    {
        if (this->mDefinition->mBehaviorType == object_behavior_type::light_11)
        {
            gLight_1D4CC->DeallocLight_47F4F0(mpAny.pLight);
            this->mpAny.o8 = NULL;
        }
        else if (this->mHasExplosion)
        {
            mpAny.pExplosion->Release();
            this->mpAny.o8 = NULL;
        }
        else
        {
            gObject_8_Pool->DeAllocate(mpAny.o8);
            this->mpAny.o8 = NULL;
        }
    }

    if (mObj3c)
    {
        if (mObj3c->mAttachedSprites.mP18)
        {
            mObj3c->mAttachedSprites.DestroyAllSprites();
        }

        gObject_3C_Pool->DeAllocate(mObj3c);
        mObj3c = NULL;
    }
}

// 9.6f 0x4847D0
MATCH_FUNC(0x528130)
void Object::NewObj3C(Fix16_Point& speed)
{
    Object_3C* pNewObj = gObject_3C_Pool->Allocate();

    this->mObj3c = pNewObj;
    pNewObj->mObj2cId = mId;

    this->mObj3c->mSpeed = speed.GetLength_41E260();
    this->mObj3c->mAngle = speed.atan2_40F790();
}

MATCH_FUNC(0x528240)
bool Object::ChangeInstanceLimitKind(s32 current, s32 desired)
{
    switch (current)
    {
        case instance_limit_kind::unlimited_1:
            switch (desired)
            {
                case instance_limit_kind::limited_count_2:
                    if (gObject_5C->mLimitedObjectCount == 360)
                    {
                        RequestRemoval();
                        return true;
                    }
                    gObject_5C->mLimitedObjectCount++;
                    break;

                case instance_limit_kind::sprite_list_3:
                    gObject_5C->mListedSpriteCount++;
                    gObject_5C->mSpriteList.AddSprite(mSprite);
                    break;
            }
            break;

        case instance_limit_kind::limited_count_2:
            switch (desired)
            {
                case instance_limit_kind::unlimited_1:
                    gObject_5C->mLimitedObjectCount--;
                    break;

                case instance_limit_kind::sprite_list_3:
                    gObject_5C->mLimitedObjectCount--;
                    gObject_5C->mListedSpriteCount++;
                    gObject_5C->mSpriteList.AddSprite(mSprite);
                    break;
            }
            break;

        case instance_limit_kind::sprite_list_3:
            switch (desired)
            {
                case instance_limit_kind::unlimited_1:
                    gObject_5C->mListedSpriteCount--;
                    gObject_5C->mSpriteList.RemoveSprite(mSprite);
                    break;

                case instance_limit_kind::limited_count_2:
                    if (gObject_5C->mLimitedObjectCount == 360)
                    {
                        RequestRemoval();
                        return true;
                    }

                    gObject_5C->mListedSpriteCount--;
                    gObject_5C->mSpriteList.RemoveSprite(mSprite);
                    gObject_5C->mLimitedObjectCount++;
                    break;
            }
            break;
    }
    return false;
}

MATCH_FUNC(0x5283c0)
void Object::TickObject(s32 obj_type)
{

    Fix16_Point dir;

    if (!obj_type)
    {
        Object::RequestRemoval();
    }
    else
    {
        if (IsExplosiveItemDefinition_482400(obj_type))
        {
            Object* pExplosion = gObject_5C->CreateExplosion(mSprite->mXy.x,
                                                                              mSprite->mXy.y,
                                                                              mSprite->mZpos,
                                                                              kZeroAng_6F8F68,
                                                                              GetExplosionTypeForItemDefinition(obj_type),
                                                                              gPedRefTable_7F8->GetPedId_420F10(mPedRefIdx));
            if (pExplosion)
            {
                pExplosion->SetDamageOwner(this->mPedRefIdx);
            }
            Object::RequestRemoval();
        }
        else
        {
            if (obj_type == 259)
            {
                dir.SetFromPolar_41E210(kFpOne64th_6F8C9C, mSprite->mRotation);
                gParticle_8->EmitImpactParticles(mSprite->mXy.x,
                                                               mSprite->mXy.y,
                                                               mSprite->mZpos,
                                                               dir);
                Object::RequestRemoval();
            }
            else
            {
                ObjectDefinition* pDefinition = gObjectDefinitions->GetObjectDefinition(obj_type);
                if (mDefinition->mInstanceLimitKind == pDefinition->mInstanceLimitKind ||
                    !Object::ChangeInstanceLimitKind(mDefinition->mInstanceLimitKind, pDefinition->mInstanceLimitKind))
                {
                    if (!sObjectRemovedFromBuckets && !sInObjectMovementUpdate)
                    {
                        Object::RemoveFromCollisionBuckets();
                    }
                    mSprite->FreeShapes();

                    switch (pDefinition->mBehaviorType)
                    {
                        case object_behavior_type::physics_3:
                        case object_behavior_type::pickup_physics_7:
                            if (!this->mObj3c)
                            {
                                Object_3C* pNew3C = gObject_3C_Pool->Allocate();
                                this->mObj3c = pNew3C;
                                pNew3C->mObj2cId = mId;
                            }

                            if (mpAny.o8)
                            {
                                gObject_8_Pool->DeAllocate(mpAny.o8);
                                this->mpAny.o8 = NULL;
                            }

                            this->mObj3c->mFriction = pDefinition->mFriction;
                            this->mObj3c->mZAccel = kFpZero_6F8E10;
                            this->mObj3c->mZSpeed = kFpZero_6F8E10;
                            this->mObj3c->mNextDefinitionTimer = pDefinition->mNextDefinitionTimer;
                            Object::InitializeObject(obj_type,
                                                               this->mSprite->mXy.x,
                                                               this->mSprite->mXy.y,
                                                               this->mSprite->mZpos,
                                                               this->mSprite->mRotation);
                            break;

                        case object_behavior_type::animated_2:
                        case object_behavior_type::pickup_animated_8:
                        {
                            if (mObj3c)
                            {
                                if (!mObj3c->mAttachedSprites.mP18)
                                {
                                    gObject_3C_Pool->DeAllocate(mObj3c);
                                    this->mObj3c = NULL;
                                }
                            }

                            bool bNewObj8;
                            if (this->mpAny.o8)
                            {
                                bNewObj8 = false;
                            }
                            else
                            {
                                bNewObj8 = true;
                                Object_8* pNewObj8 = gObject_8_Pool->Allocate();
                                this->mpAny.o8 = pNewObj8;
                            }

                            if (pDefinition->mNextDefinitionTimer != mDefinition->mNextDefinitionTimer || bNewObj8)
                            {
                                this->mpAny.o8->mTimer = pDefinition->mNextDefinitionTimer;
                                this->mpAny.o8->mFrameIdx = 0;
                                this->mpAny.o8->mFrameCounter = 0;
                            }
                            else if (pDefinition->mNumFrames < mDefinition->mNumFrames)
                            {
                                this->mpAny.o8->mFrameIdx = 0;
                            }
                            Object::InitializeObject(obj_type,
                                                               this->mSprite->mXy.x,
                                                               this->mSprite->mXy.y,
                                                               this->mSprite->mZpos,
                                                               this->mSprite->mRotation);
                            mSprite->set_id_lazy_4206C0(this->mDefinition->mBaseSpriteIdx + this->mpAny.o8->mFrameIdx);
                            break;
                        }

                        case object_behavior_type::physics_animated_4:
                        case object_behavior_type::pickup_physics_animated_9:
                        {
                            if (!this->mObj3c)
                            {
                                Object_3C* pNew3C = gObject_3C_Pool->Allocate();
                                this->mObj3c = pNew3C;
                                pNew3C->mObj2cId = mId;
                            }

                            bool bNewObj8;
                            if (this->mpAny.o8)
                            {
                                bNewObj8 = false;
                            }
                            else
                            {
                                bNewObj8 = true;
                                Object_8* pNewObj8 = gObject_8_Pool->Allocate();
                                this->mpAny.o8 = pNewObj8;
                            }

                            if (pDefinition->mNextDefinitionTimer != mDefinition->mNextDefinitionTimer || bNewObj8)
                            {
                                this->mpAny.o8->mTimer = pDefinition->mNextDefinitionTimer;
                                this->mpAny.o8->mFrameIdx = 0;
                                this->mpAny.o8->mFrameCounter = 0;
                            }
                            else if (pDefinition->mNumFrames < mDefinition->mNumFrames)
                            {
                                this->mpAny.o8->mFrameIdx = 0;
                            }

                            Object::InitializeObject(obj_type,
                                                               this->mSprite->mXy.x,
                                                               this->mSprite->mXy.y,
                                                               this->mSprite->mZpos,
                                                               this->mSprite->mRotation);
                            mSprite->set_id_lazy_4206C0(this->mDefinition->mBaseSpriteIdx + this->mpAny.o8->mFrameIdx);
                            this->mObj3c->mFriction = pDefinition->mFriction;
                            this->mObj3c->mZAccel = kFpZero_6F8E10;
                            this->mObj3c->mZSpeed = kFpZero_6F8E10;
                            break;
                        }

                        case object_behavior_type::static_object_0:
                        case object_behavior_type::static_collision_check_1:
                        case object_behavior_type::pickup_6:
                        case object_behavior_type::shop_10:
                        case object_behavior_type::effect_only_12:
                            Object::ReleaseSubObjects();
                            Object::InitializeObject(obj_type,
                                                               this->mSprite->mXy.x,
                                                               this->mSprite->mXy.y,
                                                               this->mSprite->mZpos,
                                                               this->mSprite->mRotation);
                            if (this->mObj3c)
                            {
                                return;
                            }
                            break;

                        default:
                            break;
                    }

                    if (!pDefinition->mInPoolList && !this->mRemovalState)
                    {
                        Object::PoolTake();
                    }
                    if (!sInObjectMovementUpdate)
                    {
                        sObjectReBucketed = 1;
                        Object::AssignToBucket();
                    }
                }
            }
        }
    }
}

MATCH_FUNC(0x5288B0)
bool Object::OnObjectTouched(Sprite* pSprite)
{
    if (!pSprite)
    {
        return false;
    }

    Char* pChar = pSprite->AsCharB4_40FEA0();
    if (pChar)
    {
        return pChar->OnObjectTouched(this);
    }

    Car* pCar = pSprite->AsCar_40FEB0();
    if (pCar)
    {
        return pCar->OnObjectTouched(this);
    }

    return false;
}

MATCH_FUNC(0x528900)
void Object::HandleWaterDeath()
{
    if (mObj3c)
    {
        mObj3c->mbSkipAnim = true;
    }

    if ((gpRng->get_cur_rng_41CFE0() & 3) == 0)
    {
        mSprite->IncreaseZoom(1);
        if (mSprite->ShrinkSprite(kFpOneThirtySecond, kFpOneThirtySecond, 0))
        {
            mSprite->set_z_lazy_420660(kFpZero_6F8E10);
            RequestRemoval();
        }
    }
}

MATCH_FUNC(0x528960)
bool Object::HandleObjectHitIfExplosive(Object* pOther)
{
    // Check is explosive range
    if (mDefinition->mProjectileHitType < 12 || mDefinition->mProjectileHitType > 13)
    {
        return false;
    }

    pOther->ProcessObjectExplosionImpact(this);
    return true;
}

MATCH_FUNC(0x528990)
bool Object::HandleObjectHit(Sprite* pSprite)
{
    Char* pChar = pSprite->AsCharB4_40FEA0();
    if (pChar)
    {
        return pChar->HandlePedObjectHit(this);
    }

    Car* pCar = pSprite->AsCar_40FEB0();
    if (pCar)
    {
        return pCar->HandleCarHitByObject(this);
    }

    Object* pObject = pSprite->AsObject2C_40FEC0();

    if (gPedRefTable_7F8->GetPedId_420F10(GetPedRefIdx_420FF0()))
    {
        Ped* pPed = gPedManager->PedById(gPedRefTable_7F8->GetPedId_420F10(GetPedRefIdx_420FF0()));
        if (pPed)
        {
            pPed->ProcessWeaponHitResponse(pObject);
        }
    }
    return pObject->HandleObjectHitIfExplosive(this);
}

MATCH_FUNC(0x528A20)
void Object::ProcessObjectExplosionImpact(Object* pObj)
{
    switch (this->mModel)
    {
        case objects::rocket_bullet_128:
        case objects::moving_molotov_138:
        {
            s32 explosion_kind;
            if (pObj->mModel == objects::diagonal_wall_collision_obj_166)
            {
                explosion_kind = pObj->GetExplosionTypeForDiagonalWall();
            }
            else
            {
                if (pObj->mDefinition->mProjectileHitType == 13)
                {
                    explosion_kind = explosion_type::item_19;
                }
                else
                {
                    explosion_kind = explosion_type::small_18;
                }
            }
            Object* pExplosion = gObject_5C->CreateExplosion(this->mSprite->mXy.x,
                                                                              this->mSprite->mXy.y,
                                                                              this->mSprite->mZpos,
                                                                              kZeroAng_6F8F68,
                                                                              explosion_kind,
                                                                              gPedRefTable_7F8->GetPedId_420F10(GetPedRefIdx_420FF0()));
            if (pExplosion)
            {
                pExplosion->SetDamageOwner(GetPedRefIdx_420FF0());
            }
            break;
        }

        case objects::invisible_bullet_154:
        case objects::invisible_rocket_159:
        case objects::shotgun_bullet_192:
        case objects::invisible_shotgun_bullet_193:
        case objects::fire_hitting_194:
        case objects::flamethrower_bullet_195:
        case objects::water_hitting_198:
        case objects::fire_truck_gun_bullet_199:
        case objects::machine_gun_bullet_254:
        case objects::pistol_bullet_265:
        case objects::electrobaton_bullet_277:
            break;
        default:
            return;
    }

    Ped* pPed;
    s32 ped_id = gPedRefTable_7F8->GetPedId_420F10(GetPedRefIdx_420FF0());
    if (ped_id)
    {
        pPed = gPedManager->PedById(ped_id);
    }
    else
    {
        pPed = NULL;
    }

    if (pObj->mModel == objects::diagonal_wall_collision_obj_166)
    {
        if (pPed)
        {
            pPed->NotifyWeaponHit(this->mSprite->mXy.x, this->mSprite->mXy.y, this->mModel);
        }
    }
}

MATCH_FUNC(0x528ba0)
void Object::HandleImpactNoSprite()
{
    Fix16_Point point;

    switch (this->mModel)
    {
        case objects::rocket_bullet_128:
        case objects::moving_molotov_138:
        {
            Object* pExplosion = gObject_5C->CreateExplosion(mSprite->mXy.x,
                                                                              mSprite->mXy.y,
                                                                              mSprite->mZpos,
                                                                              kZeroAng_6F8F68,
                                                                              GetExplosionTypeForWallSide(sObject2CWallHitSide),
                                                                              gPedRefTable_7F8->GetPedId_420F10(GetPedRefIdx_420FF0()));
            if (pExplosion)
            {
                pExplosion->SetDamageOwner(GetPedRefIdx_420FF0());
            }
            // Fall through
        }

        case objects::machine_gun_bullet_254:
        case objects::pistol_bullet_265:
        {
            point.FromPolar_41E210(kFpOne64th_6F8C9C, mSprite->mRotation + kAng180_6F8D62);
            gParticle_8->EmitImpactParticles(mSprite->mXy.x,
                                                           mSprite->mXy.y,
                                                           mSprite->mZpos,
                                                           point);
            break;
        }

        case objects::invisible_bullet_154:
        case objects::invisible_rocket_159:
        case objects::shotgun_bullet_192:
        case objects::invisible_shotgun_bullet_193:
        case objects::fire_hitting_194:
        case objects::flamethrower_bullet_195:
        case objects::water_hitting_198:
        case objects::fire_truck_gun_bullet_199:
        case objects::electrobaton_bullet_277:
            break;

        default:
            return;
    }

    const s32 ped_id = gPedRefTable_7F8->GetPedId_420F10(GetPedRefIdx_420FF0());
    if (ped_id)
    {
        Ped* pPed = gPedManager->PedById(ped_id);
        if (pPed)
        {
            pPed->NotifyWeaponHit(mSprite->mXy.x, mSprite->mXy.y, mModel);
        }
    }
}

MATCH_FUNC(0x528E00)
s32 __stdcall Object::GetExplosionTypeForWallSide(s32 wall_side)
{
    s32 result;

    switch (wall_side)
    {
        case 0:
            result = explosion_type::small_18;
            break;
        case 4:
            result = explosion_type::building_315_25;
            break;
        case 1:
            result = explosion_type::building_225_23;
            break;
        case 2:
            result = explosion_type::building_45_22;
            break;
        default:
            result = explosion_type::building_135_24;
            break;
    }
    return result;
}

MATCH_FUNC(0x528e50)
void Object::HandleImpact(Sprite* pSprite)
{
    // Case order decides VC6's block layout and tail merging; case 11 has to come before case 3.
    if (!this->mTouchedResult && (!pSprite || ShouldCollideWith(pSprite)))
    {
        this->mTouchedResult = 1;
        switch (mDefinition->mTouchBehavior)
        {
            case 1:
            case 2:
                PoolGive(); // destroy
                break;

            case 7:
            case 8:
                if (pSprite)
                {
                    this->mTouchedResult = HandleObjectHit(pSprite);
                }
                else
                {
                    HandleImpactNoSprite();
                }

                if (!mTouchedResult)
                {
                    return;
                }
                PoolGive();
                break;

            case 6:
                this->mTouchedResult = OnObjectTouched(pSprite);
                if (mTouchedResult)
                {
                    PoolGive(); // destroy
                }
                break;

            case 9:
                this->mTouchedResult = OnObjectTouched(pSprite);
                break;

            case 11:
                this->mTouchedResult = 0;
                break;

            case 3:
                if (pSprite->AsCar_40FEB0())
                {
                    this->mTouchedResult = 1;
                }
                else
                {
                    this->mTouchedResult = 0;
                }

                if (!mTouchedResult)
                {
                    return;
                }
                PoolGive();
                break;

            case 4:
            case 5:
                if (pSprite->AsCharB4_40FEA0())
                {
                    this->mTouchedResult = 1;
                }
                else
                {
                    this->mTouchedResult = 0;
                }

                if (!mTouchedResult)
                {
                    return;
                }
                PoolGive();
                break;

            case 10:
                if (!pSprite)
                {
                    break;
                }
                if (pSprite->AsCharB4_40FEA0())
                {
                    this->mTouchedResult = 1;
                }
                else
                {
                    this->mTouchedResult = 0;
                }
                break;

            default:
                break;
        }

        if (this->mTouchedResult)
        {
            if (pSprite)
            {
                if (!IsPickup_421060() || !pSprite->IsControlledByRemotePlayer())
                {
                    gCollisionSoundQueue->AddSpriteCollision(mSprite, pSprite);
                }
            }
            else if (gCollisionTarget.mType == collision_target_type::floor_below_4)
            {
                gCollisionSoundQueue->AddFloorCollision(mSprite);
            }
            else if (gCollisionTarget.mType == collision_target_type::block_at_z_5)
            {
                gCollisionSoundQueue->AddBlockCollision(mSprite);
            }
            else
            {
                gCollisionSoundQueue->AddCollision(mSprite, kFpZero_6F8E10);
            }
        }
    }
}

MATCH_FUNC(0x529000)
void Object::HandleCollisionWithObject(Object* pObj)
{
    switch (pObj->mModel)
    {
        case objects::conveyor_139:
            ApplyConveyorSpeedFrom(pObj);
            break;

        case objects::destructor_141:
            if (mDefinition->mDestroyedByDestructor)
            {
                Dealloc();
            }
            break;
    }
}

MATCH_FUNC(0x529030)
void Object::SetConveyorSpeed(s8 speed_x, s8 speed_y)
{
    mPedRefIdx = (speed_y + 7) | (16 * (speed_x + 7)); // x and y as nibbles, read back by UnpackSignedNibbles
}

MATCH_FUNC(0x529070)
void Object::ApplyConveyorSpeedFrom(Object* pObj)
{
    SetConveyorPush(pObj->GetParam_421050());
}

MATCH_FUNC(0x529080)
void Object::SetDamageOwner(u8 ped_ref_idx)
{
    mPedRefIdx = ped_ref_idx;
    gPedRefTable_7F8->IncrementRefCount(ped_ref_idx);
}

MATCH_FUNC(0x5290a0)
void Object::RequestRemoval()
{
    mRemovalState = object_removal_state::requested_1;
}

MATCH_FUNC(0x5290B0)
void Object::RequestRemovalWhenOffScreen()
{
    mRemovalState = object_removal_state::when_off_screen_2;
}

MATCH_FUNC(0x5290C0)
void Object::SetSpriteIdxOffset(u8 offset)
{
    s16 new_id = offset + mDefinition->mBaseSpriteIdx;
    Sprite* pSprite = mSprite;
    if (pSprite->mTypeSpriteIdx != new_id)
    {
        pSprite->mTypeSpriteIdx = new_id;
        pSprite->UpdateDimensionsFromSpriteIndex();
    }
}

MATCH_FUNC(0x5290F0)
Fix16 Object::GetEffectiveSpeed() const
{
    if (mObj3c)
    {
        if (!mObj3c->mbAirborne)
        {
            if (mObj3c->mSpeed == kFpZero_6F8E10)
            {
                return mObj3c->mZSpeed;
            }
            return mObj3c->mSpeed;
        }
        else
        {
            if (mObj3c->mSpeed == kFpZero_6F8E10)
            {
                if (mObj3c->mZSpeed == kFpZero_6F8E10)
                {
                    if (this->mModel == objects::explosion_113)
                    {
                        return kFpZero_6F8E10;
                    }
                    return kFP16One_6F8E14;
                }
                return mObj3c->mZSpeed;
            }
            return mObj3c->mSpeed;
        }
    }
    return kFpZero_6F8E10;
}

MATCH_FUNC(0x5291b0)
void Object::Dealloc()
{
    PoolGive();
    RequestRemoval();
}

MATCH_FUNC(0x5291d0)
void Object::PoolGiveAndMarkDone()
{
    PoolGive();
    mTouchedResult = 1;
}

MATCH_FUNC(0x5291E0)
void Object::PoolGiveAndQueueObjectType(s32 object_type)
{
    PoolGive();
    mTouchedResult = object_type;
}

MATCH_FUNC(0x529200)
bool Object::IsNotModel_174() const
{
    return (mModel != 174) ? true : false;
}

// https://decomp.me/scratch/lXvKN
MATCH_FUNC(0x529210)
s32 Object::GetExplosionTypeForDiagonalWall() const
{
    switch (GetParam_421050())
    {
        case 46: // diagonal up right
        case 48: // diagonal down right
            return explosion_type::building_45_22;
        case 45: // diagonal up left
        case 47: // diagonal down left
            return explosion_type::building_225_23;
    }
    return explosion_type::building_225_23;
}

MATCH_FUNC(0x529240)
s32 Object::GetMapBlockSoundSpec() const
{
    s32 result;
    gmp_block_info* pBlockInfo =
        gMap_0x370->GetBlock(mSprite->mXy.x.ToInt(), mSprite->mXy.y.ToInt(), mSprite->mZpos.ToInt());
    switch (GetParam_421050())
    {
        case 45u:
        case 47u:
            result = gGtx_0x106C->GetSpecIgnoringRoadJunction_462FD0(pBlockInfo->mLeft & block_face::tile_idx_mask);
            break;

        case 46u:
        case 48u:
            result = gGtx_0x106C->GetSpecIgnoringRoadJunction_462FD0(pBlockInfo->mRight & block_face::tile_idx_mask);
            break;

        default:
            result = 0;
            break;
    }
    return result;
}

MATCH_FUNC(0x5292D0)
void Object::SetDefaultWeaponAmmo()
{
    s32 weapon_kind;
    if (mModel >= 200)
    {
        weapon_kind = mModel - 200;
    }
    else
    {
        weapon_kind = mModel - 64;
    }

    if (weapon_kind <= 27)
    {
        SetParam_447E90(gWeapon_8->get_defalt_ammo(weapon_kind));
    }
}

MATCH_FUNC(0x529300)
void Object_5C::TrimSpriteList()
{
    for (s32 i = mListedSpriteCount - 88; i >= 0; i--)
    {
        Sprite* pSprite = mSpriteList.PopBackSprite();
        Object* pObject = pSprite->AsObject2C_40FEC0();
        if (pObject->mModel == objects::mine_10)
        {
            if (gGame_0x40->IsSpriteInAnyPlayerBounds(pSprite, kFpZero_6F8E10))
            {
                Object_5C::CreateExplosion(pSprite->mXy.x,
                                                  pSprite->mXy.y,
                                                  pSprite->mZpos,
                                                  kZeroAng_6F8F68,
                                                  explosion_type::small_18,
                                                  gPedRefTable_7F8->GetPedId_420F10(pObject->GetPedRefIdx_420FF0()));
            }
        }
        pObject->Dealloc();
    }
}

MATCH_FUNC(0x5293a0)
void Object_5C::ObjectsService()
{
    TrimSpriteList();

    gObject_2C_Pool->UpdatePool();
}

MATCH_FUNC(0x529430)
Object_5C::Object_5C()
{
    for (u8 i = 0; i < 50; i++)
    {
        mUncollectedTokens[i] = true;
    }

    mUncollectedTokenIndex = 0;
    sInObjectMovementUpdate = 0;
    sObjectRemovedFromBuckets = 0;
    sObjectReBucketed = 0;
    mLimitedObjectCount = 0;
    mListedSpriteCount = 0;
    gObj3C_live_count = 0;
    gObj8_count = 0;
    gObj8_dealloc_count = 0;
    gObj3C_dealloc_count = 0;
    mCollisionProbeSprite = NULL;
    mPendingDamageOwner = 0;
    gDoorAccessCheckActive = false;

    if (!gObject_2C_Pool)
    {
        gObject_2C_Pool = new TObjectPool();
        if (!gObject_2C_Pool)
        {
            FatalError_4A38C0(Gta2Error::OutOfMemoryNewOperator, "C:\\Splitting\\Gta2\\Source\\object.cpp", 4239);
        }
    }

    gObject_8_Pool = new TObject_8_Pool();
    if (!gObject_8_Pool)
    {
        FatalError_4A38C0(Gta2Error::OutOfMemoryNewOperator, "C:\\Splitting\\Gta2\\Source\\object.cpp", 4243);
    }

    gObject_3C_Pool = new TObject_3C_Pool();
    if (!gObject_3C_Pool)
    {
        FatalError_4A38C0(Gta2Error::OutOfMemoryNewOperator, "C:\\Splitting\\Gta2\\Source\\object.cpp", 4245);
    }

    mCollisionProbeSprite = gSpritePool->get_new_sprite();
    mCollisionProbeSprite->set_xyz_lazy_451950(0, 0, 0);

    mCollisionProbeSprite->set_ang_lazy_420690(kZeroAng_6F8F68);
    mCollisionProbeSprite->AllocCollisionShape(0, 0, 0);

    mDiagonalWallAng315 = NULL;
    mDiagonalWallAng45 = NULL;
    mDiagonalWallAng225 = NULL;
    mDiagonalWallAng135 = NULL;
}

MATCH_FUNC(0x529750)
Object_5C::~Object_5C()
{
    if (mCollisionProbeSprite)
    {
        gSpritePool->remove(mCollisionProbeSprite);
        mCollisionProbeSprite = NULL;
    }

    if (gObject_2C_Pool)
    {
        GTA2_DELETE_AND_NULL(gObject_2C_Pool);
    }

    if (gObject_8_Pool)
    {
        GTA2_DELETE_AND_NULL(gObject_8_Pool);
    }

    if (gObject_3C_Pool)
    {
        GTA2_DELETE_AND_NULL(gObject_3C_Pool);
    }

    mDiagonalWallAng315 = NULL;
    mDiagonalWallAng45 = NULL;
    mDiagonalWallAng225 = NULL;
    mDiagonalWallAng135 = NULL;
}

MATCH_FUNC(0x5297f0)
void Object_5C::CreateDiagonalWallObjects()
{
    mDiagonalWallAng315 = Object_5C::NewPhysicsObj(objects::diagonal_wall_collision_obj_166, 0, 0, 0, kAng315);
    mDiagonalWallAng315->SetParam_447E90(45);

    mDiagonalWallAng135 = Object_5C::NewPhysicsObj(objects::diagonal_wall_collision_obj_166, 0, 0, 0, kAng135_6F8D80);
    mDiagonalWallAng135->SetParam_447E90(48);

    mDiagonalWallAng225 = Object_5C::NewPhysicsObj(objects::diagonal_wall_collision_obj_166, 0, 0, 0, kAng225);
    mDiagonalWallAng225->SetParam_447E90(46);

    mDiagonalWallAng45 = Object_5C::NewPhysicsObj(objects::diagonal_wall_collision_obj_166, 0, 0, 0, kAng45_6F8CD0);
    mDiagonalWallAng45->SetParam_447E90(47);
}

MATCH_FUNC(0x5298e0)
Object* Object_5C::GetDirectionalObject(s32 slope_type) const
{
    Object* result;
    switch (slope_type)
    {
        case 180:
        case 196:
            result = this->mDiagonalWallAng315;
            break;
        case 184:
        case 200:
            result = this->mDiagonalWallAng225;
            break;
        case 188:
        case 204:
            result = this->mDiagonalWallAng45;
            break;
        case 192:
        case 208:
            result = this->mDiagonalWallAng135;
            break;
        default:
            result = NULL;
            break;
    }
    return result;
}

MATCH_FUNC(0x529950)
Object* Object_5C::NewTouchPoint(s32 object_type, Fix16 x, Fix16 y, Fix16 z, Ang16 rot, Fix16 w, Fix16 h, Fix16 depth)
{
    Object* pNewObj = CreateObject(object_type, x, y, z, rot, false);
    if (pNewObj)
    {
        pNewObj->mSprite->SetDimensions_482A40(w, h, depth);
        pNewObj->AssignToBucket();
    }
    return pNewObj;
}

MATCH_FUNC(0x5299b0)
Object* Object_5C::NewPhysicsObj(s32 object_type, Fix16 xpos, Fix16 ypos, Fix16 zpos, Ang16 rotation)
{
    Object* pNewObj = CreateObject(object_type, xpos, ypos, zpos, rotation, false);
    if (pNewObj)
    {
        pNewObj->AssignToBucket();
    }
    return pNewObj;
}

MATCH_FUNC(0x5299f0)
Object* Object_5C::NewWithPedRefIdx(s32 object_type, u32 ped_ref_idx, Fix16 xpos, Fix16 ypos, Fix16 zpos)
{
    Object* pNewObj = CreateObject(object_type, xpos, ypos, zpos, kZeroAng_6F8F68, false);
    if (pNewObj)
    {
        pNewObj->SetParam_447E90(ped_ref_idx);
        if (object_type == objects::sound_object_type_2_279)
        {
            pNewObj->RequestRemoval();
        }
    }
    return pNewObj;
}

MATCH_FUNC(0x529a40)
Object* Object_5C::NewLight_529A40(Fix16 xpos, Fix16 ypos, Fix16 zpos, s32 argb, Fix16 radius, u8 intensity)
{
    Object* pNewObj = CreateObject(objects::light_165, xpos, ypos, zpos, kZeroAng_6F8F68, false);
    if (pNewObj)
    {
        pNewObj->mpAny.pLight->SetColourRadiusIntensity_482D60(argb, radius, intensity);
    }
    return pNewObj;
}

MATCH_FUNC(0x529ab0)
Object* Object_5C::NewLight_529AB0(s32 light_type, Fix16 xpos, Fix16 ypos, Fix16 zpos, u32 argb, Fix16 radius, u8 intensity)
{
    Object* pNewObj = Object_5C::CreateObject(light_type, xpos, ypos, zpos, kZeroAng_6F8F68, false);
    if (pNewObj)
    {
        pNewObj->mpAny.pLight->SetColourRadiusIntensity_482D60(argb, radius, intensity);
    }
    return pNewObj;
}

MATCH_FUNC(0x529B20)
Object* Object_5C::NewFlashingLight(Fix16 xpos,
                                             Fix16 ypos,
                                             Fix16 zpos,
                                             s32 argb,
                                             Fix16 radius,
                                             u8 intensity,
                                             u8 on_time,
                                             u8 off_time,
                                             u8 shape)
{
    Object* pNewObj = CreateObject(objects::light_165, xpos, ypos, zpos, kZeroAng_6F8F68, false);
    if (pNewObj)
    {
        pNewObj->mpAny.pLight->SetColourRadiusIntensity_482D60(argb, radius, intensity);
        gLight_1D4CC->SetFlashing_469070(pNewObj->mpAny.pLight, on_time, off_time, shape);
    }
    return pNewObj;
}

MATCH_FUNC(0x529bc0)
Object* Object_5C::NewPhysicsObjUnlessBlocked(s32 object_type, Fix16 xpos, Fix16 ypos, Fix16 zpos, Ang16 rotation)
{
    Object* pNewObj = CreateObject(object_type, xpos, ypos, zpos, rotation, true);
    if (pNewObj)
    {
        pNewObj->AssignToBucket();
    }
    return pNewObj;
}

// 9.6f 0x484E00
MATCH_FUNC(0x529c00)
Object* Object_5C::CreateObject(s32 object_type, Fix16 xpos, Fix16 ypos, Fix16 zpos, Ang16 rotation, bool fail_if_blocked)
{
    if (object_type == objects::secret_token_266)
    {
        if (!mUncollectedTokens[mUncollectedTokenIndex])
        {
            mUncollectedTokenIndex++;
            return NULL;
        }
    }

    ObjectDefinition* pDefinition = gObjectDefinitions->GetObjectDefinition(object_type);
    if (pDefinition->mInstanceLimitKind == instance_limit_kind::limited_count_2)
    {
        if (mLimitedObjectCount == 360)
        {
            return NULL;
        }
        mLimitedObjectCount++;
    }

    Object* pNewObject;
    if (pDefinition->mInPoolList)
    {
        pNewObject = gObject_2C_Pool->Allocate();
        pNewObject->mPoolListState = object_pool_state::in_pool_list_1;
    }
    else
    {
        pNewObject = gObject_2C_Pool->AllocateUntracked();
        pNewObject->mPoolListState = object_pool_state::out_of_pool_list_2;
    }

    pNewObject->InitializeObject(object_type, xpos, ypos, zpos, rotation);
    if (mPendingDamageOwner)
    {
        pNewObject->SetDamageOwner(mPendingDamageOwner);
        mPendingDamageOwner = 0;
    }

    if (fail_if_blocked &&
        (pNewObject->mSprite->QuerySpriteCollision(sprite_types_enum::none_0) ||
         (pDefinition->mCollisionBucketCategory == collision_bucket_category::sprite_grid_2_region_3 &&
          gSpriteGrid_2->FindCollidingSpritePreferType(pNewObject->mSprite, sprite_types_enum::none_0))))
    {
        if (pNewObject->mPoolListState == object_pool_state::in_pool_list_1)
        {
            gObject_2C_Pool->FindAndDeAllocate(pNewObject);
        }
        else
        {
            gObject_2C_Pool->DeAllocate(pNewObject);
        }
        return NULL;
    }

    if (pDefinition->mInstanceLimitKind == instance_limit_kind::sprite_list_3)
    {
        ++mListedSpriteCount;
        mSpriteList.AddSprite(pNewObject->mSprite);
    }

    switch (pDefinition->mBehaviorType)
    {
        case object_behavior_type::static_object_0:
        case object_behavior_type::static_collision_check_1:
        case object_behavior_type::pickup_6:
        case object_behavior_type::shop_10:
        case object_behavior_type::effect_only_12:
            pNewObject->mpAny.o8 = NULL;
            pNewObject->mObj3c = NULL;
            break;

        case object_behavior_type::explosion_5:
        {
            Explosion* pExplosion = gExplosionPool_7A8->Allocate();
            pNewObject->mpAny.pExplosion = pExplosion;
            if (pExplosion)
            {
                pNewObject->mHasExplosion = true;
            }
            else
            {
                return NULL;
            }

            break;
        }

        case object_behavior_type::animated_2:
        case object_behavior_type::pickup_animated_8:
        {
            Object_8* pNew8 = gObject_8_Pool->Allocate();
            pNewObject->mpAny.o8 = pNew8;
            pNewObject->mpAny.o8->mFrameIdx = 0;
            pNewObject->mpAny.o8->mTimer = pDefinition->mNextDefinitionTimer;
            pNewObject->mpAny.o8->mFrameCounter = 0;
            break;
        }

        case object_behavior_type::physics_3:
        case object_behavior_type::pickup_physics_7:
        {
            Object_3C* pNew3C = gObject_3C_Pool->Allocate();
            pNewObject->mObj3c = pNew3C;
            pNew3C->mObj2cId = pNewObject->mId;
            pNewObject->mObj3c->mSpeed = pNewObject->mDefinition->mSpeed;
            pNewObject->mObj3c->mFriction = pDefinition->mFriction;
            pNewObject->mObj3c->mAngle = rotation;
            pNewObject->mObj3c->mNextDefinitionTimer = pNewObject->mDefinition->mNextDefinitionTimer;
            pNewObject->mObj3c->mZSpeed = kFpZero_6F8E10;
            pNewObject->mObj3c->mZAccel = kFpZero_6F8E10;
            break;
        }

        case object_behavior_type::physics_animated_4:
        case object_behavior_type::pickup_physics_animated_9:
        {
            Object_3C* pNew3C = gObject_3C_Pool->Allocate();
            pNewObject->mObj3c = pNew3C;
            pNew3C->mObj2cId = pNewObject->mId;
            pNewObject->mObj3c->mSpeed = pNewObject->mDefinition->mSpeed;
            pNewObject->mObj3c->mFriction = pDefinition->mFriction;
            pNewObject->mObj3c->mZSpeed = kFpZero_6F8E10;
            pNewObject->mObj3c->mZAccel = kFpZero_6F8E10;
            pNewObject->mObj3c->mAngle = rotation;
            pNewObject->mObj3c->mNextDefinitionTimer = pNewObject->mDefinition->mNextDefinitionTimer;

            Object_8* pNew8 = gObject_8_Pool->Allocate();
            pNewObject->mpAny.o8 = pNew8;
            pNewObject->mpAny.o8->mFrameIdx = 0;
            pNewObject->mpAny.o8->mTimer = pDefinition->mNextDefinitionTimer;
            pNewObject->mpAny.o8->mFrameCounter = 0;
            break;
        }

        case object_behavior_type::light_11:
            pNewObject->mpAny.pLight = gLight_1D4CC->CreateLight(xpos, ypos, zpos, 0, 0, 0);
            break;

        default:
            break;
    }
    ++sObjectCounterUnused;

    pNewObject->mModel = object_type;
    if (pNewObject->IsPickup_421060())
    {
        pNewObject->SetDefaultWeaponAmmo();
    }

    if (pNewObject->mModel == objects::power_generator_281)
    {
        Object* pLight = NewPhysicsObj(objects::powergen_light_284, kFpZero_6F8E10, kFpZero_6F8E10, kFpZero_6F8E10, kZeroAng_6F8F68);
        pNewObject->mSprite->AttachSprite(pLight->mSprite,
                                                       (kFpEighteen * kFpOne64th_6F8ECC),
                                                       (kFP16One_6F8CEC * kFpOne64th_6F8ECC),
                                                       kZeroAng_6F8F68);
    }
    else
    {
        if (pNewObject->mModel == objects::secret_token_266)
        {
            pNewObject->SetParam_447E90(mUncollectedTokenIndex);
            mUncollectedTokenIndex++;
        }
    }
    return pNewObject;
}

MATCH_FUNC(0x52a210)
u8 Object_5C::SetPendingDamageOwner(u8 ped_ref_idx)
{
    mPendingDamageOwner = ped_ref_idx;
    return ped_ref_idx;
}

MATCH_FUNC(0x52a240)
Object* Object_5C::NewMovingObj(s32 object_type,
                                        Fix16 xpos,
                                        Fix16 ypos,
                                        Fix16 zpos,
                                        Ang16 move_angle,
                                        Ang16 rotation,
                                        Fix16 speed,
                                        Fix16 friction,
                                        Fix16 z_speed)
{
    return Object_5C::CreateMovingObj(object_type, xpos, ypos, zpos, move_angle, rotation, speed, friction, z_speed, false);
}

MATCH_FUNC(0x52a280)
Object* Object_5C::NewMovingObjUnlessBlocked(s32 object_type,
                                 Fix16 xpos,
                                 Fix16 ypos,
                                 Fix16 zpos,
                                 Ang16 move_angle,
                                 Ang16 rotation,
                                 Fix16 speed,
                                 Fix16 friction,
                                 Fix16 z_speed)
{
    return Object_5C::CreateMovingObj(object_type, xpos, ypos, zpos, move_angle, rotation, speed, friction, z_speed, true);
}

MATCH_FUNC(0x52a2c0)
Object* Object_5C::CreateMovingObj(s32 object_type,
                                 Fix16 xpos,
                                 Fix16 ypos,
                                 Fix16 zpos,
                                 Ang16 move_angle,
                                 Ang16 rotation,
                                 Fix16 speed,
                                 Fix16 friction,
                                 Fix16 z_speed,
                                 bool fail_if_blocked)
{
    Object* pNewObj = Object_5C::CreateObject(object_type, xpos, ypos, zpos, rotation, fail_if_blocked);

    if (pNewObj)
    {
        pNewObj->AssignToBucket();
        ObjectDefinition* pDefinition = gObjectDefinitions->GetObjectDefinition(object_type);
        if (!pNewObj->mObj3c)
        {
            Object_3C* pObject3C = gObject_3C_Pool->Allocate();
            pNewObj->mObj3c = pObject3C;
            if (!pObject3C)
            {
                pNewObj->RequestRemoval();
                return NULL;
            }
        }
        pNewObj->mObj3c->mSpeed = speed;
        pNewObj->mObj3c->mZSpeed = z_speed;
        pNewObj->mObj3c->mSavedSpeed = speed;
        pNewObj->mObj3c->mZAccel = kObjectGravity;
        pNewObj->mObj3c->mFriction = friction;
        pNewObj->mObj3c->mAngle = move_angle;
        pNewObj->mObj3c->mNextDefinitionTimer = pDefinition->mNextDefinitionTimer;
        Object_3C* pObj3C = pNewObj->mObj3c;
        if (pObj3C->mZSpeed != kFpZero_6F8E10)
        {
            pObj3C->mbAirborne = 1;
        }
    }
    return pNewObj;
}

MATCH_FUNC(0x52a3d0)
Object* Object_5C::CreateExplosion(Fix16 x, Fix16 y, Fix16 z, Ang16 rot, s32 kind, s32 ped_id)
{
    Object* pNewObject = NewPhysicsObj(objects::explosion_113, x, y, z, rot);
    if (pNewObject)
    {
        pNewObject->mHasExplosion = true;

        if (!pNewObject->mpAny.pExplosion)
        {
            pNewObject->mpAny.pExplosion = gExplosionPool_7A8->Allocate();
        }

        if (!pNewObject->mpAny.pExplosion)
        {
            return NULL;
        }

        pNewObject->mpAny.pExplosion->SetType_482A90(kind);
        pNewObject->mpAny.pExplosion->SetObject(pNewObject);
        pNewObject->mpAny.pExplosion->mOwnerPedId = ped_id;

        switch (kind)
        {
            case explosion_type::small_18:
            case explosion_type::item_19:
            case explosion_type::large_20:
            case explosion_type::building_45_22:
            case explosion_type::building_225_23:
            case explosion_type::building_135_24:
            case explosion_type::building_315_25:
            case explosion_type::no_ring_32:
            case explosion_type::small_33:
                pNewObject->mpAny.pExplosion->mTimer = explosion_timer::start_100;
                pNewObject->mpAny.pExplosion->mParticleSpread = 0;
                break;
            default:
                pNewObject->mpAny.pExplosion->mTimer = explosion_timer::forever_9999;
                break;
        }

        if (!pNewObject->mObj3c)
        {
            Object_3C* pNew3C = gObject_3C_Pool->Allocate();
            pNewObject->mObj3c = pNew3C;
            pNew3C->mObj2cId = pNewObject->mId;
            pNewObject->mObj3c->mSpeed = kFpZero_6F8E10;
            pNewObject->mObj3c->mZSpeed = kFpZero_6F8E10;
        }
        return pNewObject;
    }
    return NULL;
}

MATCH_FUNC(0x52A500)
void Object_5C::SaveObjects(SavedObjectData* pData)
{
    u16 saved_count = 0;
    Object* pObj = gObject_2C_Pool->mPool;
    for (u16 i = 0; i < 3825; pObj++, i++)
    {
        if (pObj->mModel && pObj->IsPickup_421060())
        {
            pData->mObjX[0][saved_count].mValue = pObj->GetX_4340D0();
            pData->mObjX[1][saved_count].mValue = pObj->GetY_4340E0();
            pData->mObjX[2][saved_count].mValue = pObj->GetZ_4340F0();
            pData->mModel[saved_count] = pObj->mModel;
            pData->mPickupAmmo[saved_count] = pObj->GetAmmo_4340A0();
            saved_count++;
            if (saved_count >= 20)
            {
                return;
            }
        }
    }
}

MATCH_FUNC(0x52A590)
void Object_5C::RestoreObjects(SavedObjectData* pData)
{
    for (u16 i = 0; i < 20; i++)
    {
        if (pData->mObjX[0][i].mValue != kFpZero_6F8E10 && pData->mObjX[1][i].mValue != kFpZero_6F8E10)
        {
            Object* pNewObj = NewPhysicsObj(pData->mModel[i],
                                                   pData->mObjX[0][i].mValue,
                                                   pData->mObjX[1][i].mValue,
                                                   pData->mObjX[2][i].mValue,
                                                   kZeroAng_6F8F68);
            pNewObj->SetParam_447E90(pData->mPickupAmmo[i]);
        }
    }

    memset(pData, 0, 0x12Cu); // everything before mObj5CBUnCollectedTokens
}

MATCH_FUNC(0x52A610)
void Object_5C::RemoveAndFree(Object* pObject)
{
    if (pObject->mDefinition->mBehaviorType != object_behavior_type::light_11)
    {
        gSpriteGrid_3->Remove(pObject->mSprite);
    }
    gObject_2C_Pool->DeAllocate(pObject);
}

MATCH_FUNC(0x52A650)
void Object::EnsureObject3C()
{
    if (!mObj3c)
    {
        Object_3C* pObject3C = gObject_3C_Pool->Allocate();
        mObj3c = pObject3C;
        pObject3C->mObj2cId = mId;
        mObj3c->mSpeed = kFpZero_6F8E10;
        mObj3c->mZSpeed = kFpZero_6F8E10;
    }
    Object::PoolGive();
}

MATCH_FUNC(0x52a6d0)
void Object::OnAttachedToSprite(Sprite* pSprite)
{
    RemoveFromCollisionBuckets();

    if (mDefinition->mBehaviorType != object_behavior_type::light_11)
    {
        gSpriteGrid_3->AddToSingleBucket_477AE0(mSprite);
    }

    PoolTake();

    Car* pCar = pSprite->AsCar_40FEB0();
    if (pCar)
    {
        mSprite->SetDrawLayer_40F7B0(pCar->GetAttachedDrawLayer());
    }
}

MATCH_FUNC(0x52ae60)
Object::~Object()
{
    mpNext = NULL;
    mSprite = NULL;
    mDefinition = NULL;
    mObj3c = NULL;
}

MATCH_FUNC(0x52ae70)
Fix16_Point Object::GetXY() const
{
    return mSprite->get_x_y();
}

MATCH_FUNC(0x52ae90)
Fix16_Point Object::GetSpeedVector_52AE90() const
{
    if (mObj3c)
    {
        return mObj3c->GetSpeedVector_52ADF0();
    }
    return gZeroVector;
}