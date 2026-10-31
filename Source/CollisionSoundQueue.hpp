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
        this->field_0_type = collision_target_type::none_0;
        this->field_20_pHitSprite = NULL;
        this->field_24_pSourceSprite = NULL;
    }

    // 9.6f 0x49EF10
    void SetSourceSprite_49EF10(Sprite* pSprite)
    {
        this->field_24_pSourceSprite = pSprite;
    }

    // 9.6f 0x482A70
    void SetFloorBelow_482A70()
    {
        this->field_0_type = collision_target_type::floor_below_4;
        this->field_20_pHitSprite = NULL;
    }

    // 9.6f 0x482A80
    void SetBlockAtZ_482A80()
    {
        this->field_0_type = collision_target_type::block_at_z_5;
        this->field_20_pHitSprite = NULL;
    }

    void SetMapZ_4BA2B0(Fix16 z)
    {
        this->field_1C_z = z;
    }

    void SetVerticalSegment_4BA280(Fix16 y_min, Fix16 y_max, Fix16 x)
    {
        this->field_C_vseg_y_min = y_min;
        this->field_0_type = collision_target_type::vertical_edge_2;
        this->field_10_vseg_y_max = y_max;
        this->field_14_vseg_x = x;
        this->field_20_pHitSprite = NULL;
    }

    void SetHorizontalSegment_4BA250(Fix16 x_min, Fix16 x_max, Fix16 y)
    {
        this->field_4_hseg_x_min = x_min;
        this->field_0_type = collision_target_type::horizontal_edge_1;
        this->field_8_hseg_x_max = x_max;
        this->field_18_hseg_y = y;
        this->field_20_pHitSprite = NULL;
    }

    bool IsCharB4_49EF20() const
    {
        return this->field_0_type == collision_target_type::sprite_3 && field_20_pHitSprite->AsCharB4_40FEA0();
    }

    void SetSprite_40FEE0(Sprite* pSprite)
    {
        field_0_type = collision_target_type::sprite_3;
        field_20_pHitSprite = pSprite;
    }

    s32 field_0_type; // 0x0: collision_target_type
    Fix16 field_4_hseg_x_min; // 0x4
    Fix16 field_8_hseg_x_max; // 0x8
    Fix16 field_C_vseg_y_min; // 0xC
    Fix16 field_10_vseg_y_max; // 0x10
    Fix16 field_14_vseg_x; // 0x14
    Fix16 field_18_hseg_y; // 0x18
    Fix16 field_1C_z; // 0x1C
    Sprite* field_20_pHitSprite; // 0x20
    Sprite* field_24_pSourceSprite; // 0x24: the car sprite that is being moved, set by CarPhysics
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
        this->field_24_impact_strength = strength;
    }

    s32 field_0_type; // 0x0: collision_event_type
    Fix16 field_4_x; // 0x4
    Fix16 field_8_y; // 0x8
    Fix16 field_C_z; // 0xC
    Car* field_10_car; // 0x10
    Car* field_14_other_car; // 0x14
    s32 field_18_object_model; // 0x18: model of the object involved
    s32 field_1C_other_object_model; // 0x1C: model of the other object collided with
    s32 field_20_map_block_spec; // 0x20
    Fix16 field_24_impact_strength; // 0x24
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

    SoundObject* field_0_pSoundObj; // 0x0
    CollisionEvent field_4_events[k_max_events]; // 0x4
    s32 field_C84_count; // 0xC84
};
GTA2_ASSERT_SIZEOF_ALWAYS(CollisionSoundQueue, 0xC88);

EXTERN_GLOBAL(CollisionSoundQueue*, gCollisionSoundQueue_66AFE0);
EXTERN_GLOBAL(CollisionTarget, gCollisionTarget_679188);
