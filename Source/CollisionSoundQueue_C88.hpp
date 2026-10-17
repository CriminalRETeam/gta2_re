#pragma once

#include "Function.hpp"
#include "fix16.hpp"
#include "sprite.hpp"
#include "collision_target_type.hpp"

class SoundObject_10;
class Car_BC;
class Object_2C;

// What the sprite being moved has just collided with: a wall segment or another sprite. One global instance.
class CollisionTarget_28
{
  public:
    EXPORT bool IsObj2C_477A10();

    void Reset_4637B0()
    {
        this->field_0_type = collision_target_type::none_0;
        this->field_20_pHitSprite = 0;
        this->field_24_pSourceSprite = 0;
    }

    // 9.6f 0x49EF10
    void SetSourceSprite_49EF10(Sprite* pSprite)
    {
        this->field_24_pSourceSprite = pSprite;
    }

    // 9.6f 0x482A70
    void SetType4_482A70()
    {
        this->field_0_type = collision_target_type::floor_below_4;
        this->field_20_pHitSprite = 0;
    }

    // 9.6f 0x482A80
    void SetType5_482A80()
    {
        this->field_0_type = collision_target_type::block_at_z_5;
        this->field_20_pHitSprite = 0;
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
        this->field_20_pHitSprite = 0;
    }

    void SetHorizontalSegment_4BA250(Fix16 x_min, Fix16 x_max, Fix16 y)
    {
        this->field_4_hseg_x_min = x_min;
        this->field_0_type = collision_target_type::horizontal_edge_1;
        this->field_8_hseg_x_max = x_max;
        this->field_18_hseg_y = y;
        this->field_20_pHitSprite = 0;
    }

    bool IsCharB4_49EF20()
    {
        return this->field_0_type == collision_target_type::sprite_3 && field_20_pHitSprite->AsCharB4_40FEA0();
    }

    void SetSprite_40FEE0(Sprite* pSprite)
    {
        field_0_type = collision_target_type::sprite_3;
        field_20_pHitSprite = pSprite;
    }

    s32 field_0_type; // collision_target_type
    Fix16 field_4_hseg_x_min;
    Fix16 field_8_hseg_x_max;
    Fix16 field_C_vseg_y_min;
    Fix16 field_10_vseg_y_max;
    Fix16 field_14_vseg_x;
    Fix16 field_18_hseg_y;
    Fix16 field_1C_z;
    Sprite* field_20_pHitSprite;
    Sprite* field_24_pSourceSprite; // the car sprite that is being moved, set by CarPhysics_B0
};

// One queued impact for the sound system: where it happened, what collided and how hard
class CollisionEvent_28
{
  public:
    EXPORT void SetPosition_40B870(Fix16 x, Fix16 y, Fix16 z);
    EXPORT void SetupForCar_40B890(Car_BC* pCar);
    EXPORT void SetupForPed_40B980();
    EXPORT bool SetupForObject_40BA60(Object_2C* pObj);

    void SetImpactStrength_40FF10(const Fix16& strength)
    {
        this->field_24_impact_strength = strength;
    }

    s32 field_0_type; // collision_event_type
    Fix16 field_4_x;
    Fix16 field_8_y;
    Fix16 field_C_z;
    Car_BC* field_10_car;
    Car_BC* field_14_other_car;
    s32 field_18_object_model; // model of the object involved
    s32 field_1C_other_object_model; // model of the other object collided with
    s32 field_20_map_block_spec;
    Fix16 field_24_impact_strength;
};

// Impacts collected during a frame; processed by sound_obj::ProcessType6_CollisionSoundQueue_413760
class CollisionSoundQueue_C88
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
    EXPORT CollisionSoundQueue_C88();
    EXPORT ~CollisionSoundQueue_C88();

    SoundObject_10* field_0_pSoundObj;
    CollisionEvent_28 field_4_events[k_max_events];
    s32 field_C84_count;
};

EXTERN_GLOBAL(CollisionSoundQueue_C88*, gCollisionSoundQueue_66AFE0);
EXTERN_GLOBAL(CollisionTarget_28, gCollisionTarget_679188);
