#pragma once

#include "Function.hpp"
#include "fix16.hpp"
#include "sprite.hpp"
#include "collision_target_type.hpp"

class SoundObject;
class Car;
class Object;

// What the sprite being moved has just collided with: a wall segment or another sprite. One global instance.
class CollisionTarget
{
  public:
    EXPORT bool IsObj2C_477A10() const;

    void Reset_4637B0()
    {
        this->mType = collision_target_type::none_0;
        this->mpHitSprite = NULL;
        this->mpSourceSprite = NULL;
    }

    // 9.6f 0x49EF10
    void SetSourceSprite_49EF10(Sprite* pSprite)
    {
        this->mpSourceSprite = pSprite;
    }

    // 9.6f 0x482A70
    void SetFloorBelow_482A70()
    {
        this->mType = collision_target_type::floor_below_4;
        this->mpHitSprite = NULL;
    }

    // 9.6f 0x482A80
    void SetBlockAtZ_482A80()
    {
        this->mType = collision_target_type::block_at_z_5;
        this->mpHitSprite = NULL;
    }

    void SetMapZ_4BA2B0(Fix16 z)
    {
        this->mZ = z;
    }

    void SetVerticalSegment_4BA280(Fix16 y_min, Fix16 y_max, Fix16 x)
    {
        this->mVsegYMin = y_min;
        this->mType = collision_target_type::vertical_edge_2;
        this->mVsegYMax = y_max;
        this->mVsegX = x;
        this->mpHitSprite = NULL;
    }

    void SetHorizontalSegment_4BA250(Fix16 x_min, Fix16 x_max, Fix16 y)
    {
        this->mHsegXMin = x_min;
        this->mType = collision_target_type::horizontal_edge_1;
        this->mHsegXMax = x_max;
        this->mHsegY = y;
        this->mpHitSprite = NULL;
    }

    bool IsCharB4_49EF20() const
    {
        return this->mType == collision_target_type::sprite_3 && mpHitSprite->AsCharB4_40FEA0();
    }

    void SetSprite_40FEE0(Sprite* pSprite)
    {
        mType = collision_target_type::sprite_3;
        mpHitSprite = pSprite;
    }

    s32 mType; // 0x0: collision_target_type
    Fix16 mHsegXMin; // 0x4
    Fix16 mHsegXMax; // 0x8
    Fix16 mVsegYMin; // 0xC
    Fix16 mVsegYMax; // 0x10
    Fix16 mVsegX; // 0x14
    Fix16 mHsegY; // 0x18
    Fix16 mZ; // 0x1C
    Sprite* mpHitSprite; // 0x20
    Sprite* mpSourceSprite; // 0x24: the car sprite that is being moved, set by CarPhysics
};
GTA2_ASSERT_SIZEOF_ALWAYS(CollisionTarget, 0x28);

// One queued impact for the sound system: where it happened, what collided and how hard
class CollisionEvent
{
  public:
    EXPORT void SetPosition_40B870(Fix16 x, Fix16 y, Fix16 z);
    EXPORT void SetupForCar_40B890(Car* pCar);
    EXPORT void SetupForPed_40B980();
    EXPORT bool SetupForObject_40BA60(Object* pObj);

    void SetImpactStrength_40FF10(const Fix16& strength)
    {
        this->mImpactStrength = strength;
    }

    s32 mType; // 0x0: collision_event_type
    Fix16 mX; // 0x4
    Fix16 mY; // 0x8
    Fix16 mZ; // 0xC
    Car* mCar; // 0x10
    Car* mOtherCar; // 0x14
    s32 mObjectModel; // 0x18: model of the object involved
    s32 mOtherObjectModel; // 0x1C: model of the other object collided with
    s32 mMapBlockSpec; // 0x20
    Fix16 mImpactStrength; // 0x24
};
GTA2_ASSERT_SIZEOF_ALWAYS(CollisionEvent, 0x28);

// Impacts collected during a frame; processed by sound_obj::ProcessType6_CollisionSoundQueue_413760
class CollisionSoundQueue
{
  public:
    enum
    {
        k_max_events = 80
    };

    EXPORT void Reset_40BB90();
    EXPORT void AddCollision_40BBA0(Sprite* pSprite, Fix16 impact_strength);
    EXPORT void AddFloorCollision_40BC40(Sprite* pSprite);
    EXPORT void AddBlockCollision_40BD10(Sprite* pSprite);
    EXPORT void AddSpriteCollision_40BDD0(Sprite* pSprite, Sprite* pHitSprite);
    EXPORT CollisionSoundQueue();
    EXPORT ~CollisionSoundQueue();

    SoundObject* mpSoundObj; // 0x0
    CollisionEvent mEvents[k_max_events]; // 0x4
    s32 mCount; // 0xC84
};
GTA2_ASSERT_SIZEOF_ALWAYS(CollisionSoundQueue, 0xC88);

EXTERN_GLOBAL(CollisionSoundQueue*, gCollisionSoundQueue_66AFE0);
EXTERN_GLOBAL(CollisionTarget, gCollisionTarget_679188);
