#pragma once

#include "Fix16_Point.hpp"
#include "Function.hpp"
#include "Object_3C.hpp"
#include "Object_8.hpp"
#include "ObjectDefinitions.hpp"
#include "ang16.hpp"
#include "fix16.hpp"
#include "sprite.hpp"

EXTERN_GLOBAL(Fix16_Point, gZeroVector);
EXTERN_GLOBAL(s32, gObj2C_id);

EXPORT void __stdcall UnpackSignedNibbles(u8 packed, s8* pX, s8* pY);

class Object;
class Sprite;
class SpriteShape;
class Car;
class ObjectDefinition;
class Object_3C;
class ObjectPool;
class Explosion;
class Object_8;
class Light;

namespace object_pool_state
{
enum
{
    none_0 = 0,
    in_pool_list_1 = 1, // linked into the pool's list of live objects (PoolGive)
    out_of_pool_list_2 = 2, // allocated but not linked (PoolTake, AllocateUntracked)
};
} // namespace object_pool_state

namespace object_removal_state
{
enum
{
    none_0 = 0,
    requested_1 = 1, // RequestRemoval
    when_off_screen_2 = 2, // RequestRemovalWhenOffScreen
};
} // namespace object_removal_state

class Object
{
  public:
    STATIC_MEMBER(s32, sObject2CWallHitSide);
    STATIC_MEMBER(Sprite*, sObject2CLastCollisionSprite);
    STATIC_MEMBER(bool, sObject2CHitWallOrDoor);
    // 9.6f 0x4C4F10
    inline void SetThreadTriggerIdx(char_type v)
    {
        mThreadTriggerIdx = v;
    }

    EXPORT Object();
    EXPORT void PoolDeallocate();
    EXPORT bool IsSpriteOwnerPed(Sprite* pSprite) const;
    EXPORT s32 GetMovementRegionSize() const;
    EXPORT void UpdatePhysics();
    EXPORT void PoolGive();
    EXPORT void PoolTake();
    EXPORT bool ShouldCollideWith(Sprite* pSprite) const;
    EXPORT bool ShouldStopAtTrafficLight(Sprite* pSprite) const;
    EXPORT bool ShouldCollideWithSprite_522430(Sprite* pSprite) const;
    EXPORT bool SelectCollisionSprite(Sprite* pProbe);
    EXPORT void SetMovementVector(Fix16_Point& speed);
    EXPORT void SetMovementVectorWithRandomState(Fix16_Point& point);
    EXPORT void SetConveyorPush(char_type conveyor_speed);
    EXPORT void ResolveCollisionWithObject(Object* pObj, Fix16_Point* pPoint);
    EXPORT void ResolveCollisionWithPed(Char* pChar, Fix16_Point* pPoint, u8 not_used);
    EXPORT void ResolveCollisionWithWorld(Fix16_Point* pCollisionPoint, Fix16_Point* pPoint, Fix16_Point* speed);
    EXPORT void ResolveCollisionWithHorizontalEdge(Fix16_Point* pPoint);
    EXPORT void ResolveCollisionWithVerticalEdge(Fix16_Point* pPoint);
    EXPORT void HandleCollision(Fix16_Point* pPoint);
    EXPORT bool TryStartSlide(Fix16 speed);
    EXPORT void HandleCollisionOutcome(Fix16_Point point, bool bHitFloorBelow, bool bHitBlockAtZ);
    EXPORT bool HandleSpriteGroundAndCollision(Sprite* pSprite, Fix16_Point* pPoint, bool* pHitWall, Fix16 found_z);
    EXPORT bool HandleSpriteGroundAndCollisionSimple(Sprite* pSprite, Fix16_Point* pPoint, bool* pHitWall, bool* pHitBlockAtZ);
    EXPORT bool HandleSpriteZCollision(Sprite* pSprite, Fix16_Point* pPoint, bool* pHitWall, bool* pHitFloorBelow, Fix16 found_z);
    EXPORT void ComputeWallHitSide();
    EXPORT void IntegrateHorizontalMovementAndCollisions(Fix16 move_len, Ang16 move_angle);
    EXPORT void OnExplosionNearby(u8 ped_ref_idx);
    EXPORT void UpdateAnimation();
    EXPORT bool DispatchFrameAction();
    EXPORT bool ShouldCollideWithSprite_525370(Sprite* pSprite) const;
    EXPORT void CheckCollisionForModel_139_And_141();
    EXPORT void SpawnSpriteParticlesForRocketBullet();
    EXPORT bool CheckWaterDeath();
    EXPORT void UpdatePhysicsAndMovement();
    EXPORT void UpdatePhysicsMovementAndAnimation();
    EXPORT void Update();
    EXPORT bool PoolUpdate();
    EXPORT void TriggerCarExplosionIfApplicable(Sprite* pSprite);
    EXPORT static s32 __stdcall GetExplosionTypeForItemDefinition(s32 type);
    EXPORT void InheritMotionFromSprite(Sprite* pSprite);
    EXPORT bool UpdateMovementAndEffects(Sprite* pSprite, Fix16 x, Fix16 y, Ang16 rot);
    EXPORT void InitializeObject(s32 object_type, Fix16 xpos, Fix16 ypos, Fix16 zpos, Ang16 rotation);
    EXPORT void TurnOffLight();
    EXPORT void AssignToBucket();
    EXPORT void RemoveFromCollisionBuckets();
    EXPORT void ReleaseSubObjects();
    EXPORT void NewObj3C(Fix16_Point& speed);
    EXPORT bool ChangeInstanceLimitKind(s32 current, s32 desired);
    EXPORT void TickObject(s32 obj_type);
    EXPORT bool OnObjectTouched(Sprite* pSprite);
    EXPORT void HandleWaterDeath();
    EXPORT static s32 __stdcall GetExplosionTypeForWallSide(s32 wall_side);
    EXPORT bool HandleObjectHitIfExplosive(Object* pOther);
    EXPORT bool HandleObjectHit(Sprite* pSprite);
    EXPORT void ProcessObjectExplosionImpact(Object* pObj);
    EXPORT void HandleImpactNoSprite();
    EXPORT void HandleImpact(Sprite* pSprite);
    EXPORT void HandleCollisionWithObject(Object* pObj);
    EXPORT void SetConveyorSpeed(s8 speed_x, s8 speed_y);
    EXPORT void ApplyConveyorSpeedFrom(Object* pObj);
    EXPORT s32 GetExplosionTypeForDiagonalWall() const;
    EXPORT s32 GetMapBlockSoundSpec() const;
    EXPORT void SetDefaultWeaponAmmo();
    EXPORT void SetDamageOwner(u8 ped_ref_idx);
    EXPORT void RequestRemoval();
    EXPORT void RequestRemovalWhenOffScreen();
    EXPORT void Dealloc();
    EXPORT void PoolGiveAndMarkDone();
    EXPORT void PoolGiveAndQueueObjectType(s32 object_type);
    EXPORT bool IsNotModel_174() const;
    EXPORT ~Object();
    EXPORT void EnsureObject3C();
    EXPORT void OnAttachedToSprite(Sprite* pSprite);
    EXPORT Fix16_Point GetXY() const;
    EXPORT Fix16_Point GetSpeedVector_52AE90() const;
    // 9.6f 0x482C50; GetSpeedVector_52AE90 is its out-of-line copy. It returns a Fix16_Point (which has
    // a destructor), so VC6 never expands it: it is charged to the caller's inline budget and called
    // out of line (SetMovementVector)
    inline Fix16_Point GetSpeedVector_482C50() const
    {
        if (mObj3c)
        {
            return mObj3c->GetSpeedVector_482BA0();
        }
        return gZeroVector;
    }

    EXPORT void SetSpriteIdxOffset(u8 offset);
    EXPORT Fix16 GetEffectiveSpeed() const;
    EXPORT bool IsFireExplosion() const;
    EXPORT void UpdateEffectPool();
    EXPORT void UpdateLight();
    EXPORT void OnAttachedSpriteDestroyed();

    EXPORT void IntegrateMovementAndCollisions(Fix16 mov_speed, Ang16 angle);
    EXPORT void Sprite_UpdateZFromSlopeAndTile(Sprite* pSprite);

    // 9.6f 0x475A70
    inline bool IsStaticCollisionCheck() const
    {
        return mDefinition->mBehaviorType == object_behavior_type::static_collision_check_1;
    }

    // 9.6f 0x475A60
    inline bool IsShop() const
    {
        s32 behavior_type = mDefinition->mBehaviorType;
        return behavior_type == object_behavior_type::shop_10;
    }

    // 9.6f 0x475A80
    bool IsAnimatedBehavior() const
    {
        s32 behavior_type = this->mDefinition->mBehaviorType;
        return behavior_type == object_behavior_type::animated_2 || behavior_type == object_behavior_type::physics_animated_4 || behavior_type == object_behavior_type::pickup_animated_8 || behavior_type == object_behavior_type::pickup_physics_animated_9;
    }

    // 9.6f 0x434140
    inline bool IsDestroyedPowergen() const
    {
        if (mModel == objects::destroyed_invisible_powergen_285 || mModel == objects::destroyed_powergen_282)
        {
            return true;
        }
        return false;
    }

    // 9.6f 0x482C90
    bool CanBePushed() const
    {
        s32 behavior_type = mDefinition->mBehaviorType;
        if (behavior_type == object_behavior_type::physics_3 || behavior_type == object_behavior_type::physics_animated_4 ||
            ((behavior_type == object_behavior_type::static_object_0 || behavior_type == object_behavior_type::static_collision_check_1 || behavior_type == object_behavior_type::animated_2) && mDefinition->mTouchBehavior == 2))
        {
            return true;
        }
        return false;
    }

    // 9.6f 0x421080
    bool UsesParamAsPedRef() const
    {
        s32 type = this->mDefinition->mBehaviorType;
        return type != object_behavior_type::pickup_6 && type != object_behavior_type::pickup_physics_7 &&
            type != object_behavior_type::pickup_animated_8 && type != object_behavior_type::pickup_physics_animated_9 &&
            type != object_behavior_type::shop_10 && type != object_behavior_type::static_collision_check_1 &&
            type != object_behavior_type::effect_only_12;
    }

    // 9.6f 0x4973E0
    bool IsOwnedByPedRef(u8 ped_ref_idx) const
    {
        return UsesParamAsPedRef() && mPedRefIdx == ped_ref_idx;
    }

    // 9.6f 0x4210B0
    bool IsSpriteGrid2Region3() const
    {
        return mDefinition->mCollisionBucketCategory == collision_bucket_category::sprite_grid_2_region_3;
    }

    // Inlined on version 9.6f 0x447e90 (also 9.6f copies 0x45E0A0 and 0x482C00)
    inline void SetParam(u8 v)
    {
        mPedRefIdx = v;
    }

    // Inlined on version 9.6f 0x420FF0
    inline u8 GetPedRefIdx() const
    {
        return mPedRefIdx;
    }

    // 9.6f 0x421050
    inline u8 GetParam() const
    {
        return mPedRefIdx;
    }

    // 9.6f 0x4340A0
    inline u8 GetAmmo() const
    {
        return mPedRefIdx;
    }

    // 9.6f 0x434130
    inline void SetO8Timer(s16 timer)
    {
        mpAny.o8->mTimer = timer;
    }

    // 9.6f 0x40FEF0
    s32 GetModel() const
    {
        return mModel;
    }

    // 9.6f 0x40FF00
    inline char_type MakesCollisionSound() const
    {
        return mDefinition->mMakesCollisionSound;
    }

    // 9.6f 0x475AA0
    inline bool IsModel176To181() const
    {
        s32 model = mModel;
        return model == 176 || model == 177 || model == 178 || model == 179 || model == 180 || model == 181;
    }

    // 9.6f 0x421060
    bool IsPickup() const
    {
        return mDefinition->mBehaviorType == object_behavior_type::pickup_6 ||
            mDefinition->mBehaviorType == object_behavior_type::pickup_physics_7 ||
            mDefinition->mBehaviorType == object_behavior_type::pickup_animated_8 ||
            mDefinition->mBehaviorType == object_behavior_type::pickup_physics_animated_9;
    }

    // 9.6f 0x482490
    inline void PoolAllocate()
    {
        mId = gObj2C_id++;
        mRemovalState = object_removal_state::none_0;
        mPedRefIdx = 99;
        mObj3c = NULL;
        mpAny.o8 = NULL;
        mHasExplosion = false;
        mSprite = NULL;
        mModel = 0;
    }

    // 9.6f 0x4340D0
    inline Fix16 GetX() const
    {
        return mSprite->mXy.x;
    }

    // 9.6f 0x4340E0
    inline Fix16 GetY() const
    {
        return mSprite->mXy.y;
    }

    // 9.6f 0x4340F0
    inline Fix16 GetZ() const
    {
        return mSprite->mZpos;
    }

    // 9.6f 0x4BE850
    bool IsGangIcon() const
    {
        return mModel >= objects::loonies_icon_287 && mModel <= objects::russian_mafia_icon_293;
    }

    // 9.6f 0x4BE830
    bool IsFlameObject() const
    {
        if (this->mModel == objects::fire_197)
        {
            return true;
        }

        if (IsFireExplosion())
        {
            return true;
        }

        return false;
    }

    // 9.6f 0x482400
    static inline bool IsExplosiveItemDefinition(s32 idx)
    {
        return idx >= 39 && idx <= 42;
    }

    // 9.6f 0x482C80
    inline Fix16 GetMass() const
    {
        return mDefinition->mMass;
    }

    // 9.6f 0x482BE0
    void ApplyDefinitionDrawLayer()
    {
        mSprite->SetDrawLayer(mDefinition->mDrawLayer);
    }

    // 9.6f 0x482C10, defined in Object_5C.cpp (needs Object_8)
    inline bool IsAnimFinished() const;

    // 9.6f 0x493090
    inline void GetConveyorDirection(s8* pX, s8* pY)
    {
        UnpackSignedNibbles(GetParam(), pX, pY);
    }

    // 9.6f 0x482BF0
    void SetTopDrawLayer()
    {
        mSprite->SetDrawLayer(29);
    }

    Object* mpNext;
    Sprite* mSprite; // 0x4
    ObjectDefinition* mDefinition; // 0x8
    union TAny
    {
        Explosion* pExplosion;
        Object_8* o8;
        Light* pLight;
    };
    TAny mpAny; // 0xC
    Object_3C* mObj3c; // 0x10
    s32 mId; // 0x14
    s32 mModel; // 0x18
    bool mHasExplosion; // 0x1C
    char_type mUnused1D; // 0x1D
    char_type mUnused1E; // 0x1E
    char_type mUnused1F; // 0x1F
    s32 mPoolListState; // 0x20
    u8 mTouchedResult; // 0x24
    u8 mRemovalState; // 0x25
    u8 mPedRefIdx; // 0x26: Seems to be a generic index. Sometimes it's the current idx of its object type. For many objects, it's unused (equal to 99).
    char_type mThreadTriggerIdx; // 0x27
    char_type mUnused28; // 0x28
    char_type mUnused29; // 0x29
    char_type mUnused2A; // 0x2A
    char_type mUnused2B; // 0x2B
};
GTA2_ASSERT_SIZEOF_ALWAYS(Object, 0x2C);

struct SavedObjectData;

class Object_5C
{
  public:
    EXPORT void TrimSpriteList();
    EXPORT void ObjectsService();
    EXPORT Object_5C();
    EXPORT ~Object_5C();
    EXPORT void CreateDiagonalWallObjects();
    EXPORT Object* GetDirectionalObject(s32 slope_type) const;
    EXPORT Object* NewTouchPoint(s32 object_type, Fix16 x, Fix16 y, Fix16 z, Ang16 rot, Fix16 w, Fix16 h, Fix16 depth);
    EXPORT Object* NewPhysicsObj(s32 object_type, Fix16 xpos, Fix16 ypos, Fix16 zpos, Ang16 rotation);
    EXPORT Object* NewWithPedRefIdx(s32 object_type, u32 ped_ref_idx, Fix16 xpos, Fix16 ypos, Fix16 zpos);
    EXPORT Object* NewLight_529A40(Fix16 xpos, Fix16 ypos, Fix16 zpos, s32 argb, Fix16 radius, u8 intensity);
    EXPORT Object* NewLight_529AB0(s32 light_type, Fix16 xpos, Fix16 ypos, Fix16 zpos, u32 argb, Fix16 radius, u8 intensity);
    EXPORT Object* NewFlashingLight(Fix16 xpos, Fix16 ypos, Fix16 zpos, s32 argb, Fix16 radius, u8 intensity, u8 on_time, u8 off_time, u8 shape);
    EXPORT Object* NewPhysicsObjUnlessBlocked(s32 object_type, Fix16 xpos, Fix16 ypos, Fix16 zpos, Ang16 rotation);
    EXPORT Object* CreateObject(s32 object_type, Fix16 xpos, Fix16 ypos, Fix16 zpos, Ang16 rotation, bool fail_if_blocked);
    EXPORT u8 SetPendingDamageOwner(u8 ped_ref_idx);
    EXPORT Object* NewMovingObj(s32 object_type,
                                        Fix16 xpos,
                                        Fix16 ypos,
                                        Fix16 zpos,
                                        Ang16 move_angle,
                                        Ang16 rotation,
                                        Fix16 speed,
                                        Fix16 friction,
                                        Fix16 z_speed);
    EXPORT Object* NewMovingObjUnlessBlocked(s32 object_type,
                                 Fix16 xpos,
                                 Fix16 ypos,
                                 Fix16 zpos,
                                 Ang16 move_angle,
                                 Ang16 rotation,
                                 Fix16 speed,
                                 Fix16 friction,
                                 Fix16 z_speed);
    EXPORT Object* CreateMovingObj(s32 object_type,
                                 Fix16 xpos,
                                 Fix16 ypos,
                                 Fix16 zpos,
                                 Ang16 move_angle,
                                 Ang16 rotation,
                                 Fix16 speed,
                                 Fix16 friction,
                                 Fix16 z_speed,
                                 bool fail_if_blocked);
    EXPORT Object* CreateExplosion(Fix16 x, Fix16 y, Fix16 z, Ang16 rot, s32 kind, s32 ped_id);

    EXPORT void SaveObjects(SavedObjectData* pData);
    EXPORT void RestoreObjects(SavedObjectData* pData);
    EXPORT void RemoveAndFree(Object* pObject);

    Object* mDiagonalWallAng315; // 0x0
    Object* mDiagonalWallAng135; // 0x4
    Object* mDiagonalWallAng225; // 0x8
    Object* mDiagonalWallAng45; // 0xC
    s32 mLimitedObjectCount; // 0x10
    s32 mListedSpriteCount; // 0x14
    u8 mPendingDamageOwner; // 0x18
    u8 mUnused19; // 0x19
    u16 mUnused1A; // 0x1A
    SpriteList mSpriteList; // 0x1C
    bool mUncollectedTokens[50]; // 0x20
    u16 mUnused52; // 0x52: pad?
    s32 mUncollectedTokenIndex; // 0x54
    Sprite* mCollisionProbeSprite; // 0x58
};

EXTERN_GLOBAL(Object_5C*, gObject_5C);
