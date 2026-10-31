#pragma once

#include "Function.hpp"
#include "fix16.hpp"

struct object_info;
class Sprite;

// Static table entry (gMapObjectOverrides_6FC5F8): overrides the properties of an existing (map) object definition,
// see ObjectDefinitions::ApplyMapObjectOverrides
class MapObjectOverride
{
  public:
    EXPORT MapObjectOverride(s32 definition_idx,
                  const s32& behavior_type,
                  s32 next_definition_idx,
                  s8 in_pool_list,
                  const s32& touch_behavior,
                  const s32& projectile_hit_type,
                  const s32& collision_bucket_category,
                  Fix16 speed,
                  Fix16 friction,
                  s8 next_definition_timer,
                  const s32& collision_response,
                  const s32& z_collision_response,
                  s8 next_frame_max,
                  const s32& uses_3d_movement,
                  Fix16 mass,
                  s32 draw_layer,
                  s32 sprite_flags,
                  s32 has_sound,
                  s8 has_shadows,
                  s32 instance_limit_kind,
                  s8 makes_collision_sound);

    s32 mDefinitionIdx; // 0x0
    s32 mBehaviorType; // 0x4
    s32 mNextDefinitionIdx; // 0x8
    s8 mInPoolList; // 0xC
    s32 mTouchBehavior; // 0x10
    s32 mProjectileHitType; // 0x14
    s32 mCollisionBucketCategory; // 0x18
    Fix16 mSpeed; // 0x1C
    Fix16 mFriction; // 0x20
    s8 mNextDefinitionTimer; // 0x24
    s32 mCollisionResponse; // 0x28
    s32 mZCollisionResponse; // 0x2C
    s8 mNextFrameMax; // 0x30
    s32 mUses3dMovement; // 0x34
    Fix16 mMass; // 0x38
    s32 mDrawLayer; // 0x3C
    s8 mSpriteFlags; // 0x40
    s32 mHasSound; // 0x44
    s8 mHasShadows; // 0x48
    s32 mInstanceLimitKind; // 0x4C
    s8 mMakesCollisionSound; // 0x50
};
GTA2_ASSERT_SIZEOF_ALWAYS(MapObjectOverride, 0x54);

// Static table entry (gCodeObjectTemplates_6F9038): template of a code object, converted into an ObjectDefinition by
// ObjectDefinitions::CreateCodeObjectDefinitions
class CodeObjectTemplate
{
  public:
    EXPORT CodeObjectTemplate(u32 definition_idx,
                  u8 num_sprites,
                  const u32& behavior_type,
                  u32 impact_definition_idx,
                  u32 next_definition_idx,
                  u8 in_pool_list,
                  const u32& touch_behavior,
                  const u32& projectile_hit_type,
                  const u32& collision_bucket_category,
                  Fix16 speed,
                  Fix16 friction,
                  s8 next_definition_timer,
                  const u32& collision_response,
                  const u32& z_collision_response,
                  u32 sprite_type,
                  u32 next_frame_max,
                  const u32& uses_3d_movement,
                  Fix16 mass,
                  Fix16 width,
                  Fix16 height,
                  Fix16 depth,
                  u32 draw_layer,
                  u32 sprite_flags,
                  u8 destroyed_by_destructor,
                  const u32& instance_limit_kind,
                  u32 uses_bullet_walls,
                  u32 has_sound,
                  u8 has_shadows,
                  u8 makes_collision_sound);

    s32 mDefinitionIdx; // 0x0
    u8 mNumSprites; // 0x4
    s32 mBehaviorType; // 0x8
    s32 mImpactDefinitionIdx; // 0xC
    s32 mNextDefinitionIdx; // 0x10
    s8 mInPoolList; // 0x14
    s32 mTouchBehavior; // 0x18
    s32 mProjectileHitType; // 0x1C
    s32 mCollisionBucketCategory; // 0x20
    Fix16 mSpeed; // 0x24
    Fix16 mFriction; // 0x28
    s8 mNextDefinitionTimer; // 0x2C
    s32 mCollisionResponse; // 0x30
    s32 mZCollisionResponse; // 0x34
    s32 mSpriteType; // 0x38
    s8 mNextFrameMax; // 0x3C
    s32 mUses3dMovement; // 0x40
    Fix16 mMass; // 0x44
    Fix16 mWidth; // 0x48
    Fix16 mHeight; // 0x4C
    Fix16 mDepth; // 0x50
    s32 mDrawLayer; // 0x54
    s8 mSpriteFlags; // 0x58
    s8 mDestroyedByDestructor; // 0x59
    s32 mInstanceLimitKind; // 0x5C
    s32 mUsesBulletWalls; // 0x60
    s32 mHasSound; // 0x64
    s8 mHasShadows; // 0x68
    s8 mMakesCollisionSound; // 0x69
};
GTA2_ASSERT_SIZEOF_ALWAYS(CodeObjectTemplate, 0x6C);

namespace CollisionReaction
{
enum
{
    Always_0 = 0,
    OnlyCars_1 = 1,
    OnlyPeds_2 = 2,
    OnlyObjects_3 = 3,
    Never_4 = 4,
};
} // namespace CollisionReaction

namespace collision_bucket_category
{
enum
{
    sprite_grid_3_single_cell_0 = 0, // gSpriteGrid_3_679210
    sprite_grid_3_single_cell_1 = 1, // gSpriteGrid_3_679210

    none_2 = 2, // no bucket assignment

    sprite_grid_2_region_3 = 3, // gSpriteGrid_2_67920C
    sprite_grid_1_region_4 = 4 // gSpriteGrid_1_679208
};
} // namespace collision_bucket_category

namespace object_behavior_type
{
enum
{
    static_object_0 = 0, // static prop: frame action + effect pool update only
    static_collision_check_1 = 1, // static, also runs CheckCollisionForModel_139_And_141; takes a shop type from scripts
    animated_2 = 2, // animated (UpdateAnimation, has an Object_8)
    physics_3 = 3, // bullets/thrown things: removed from buckets, moves with UpdatePhysicsAndMovement (has an Object_3C)
    physics_animated_4 = 4, // like physics_3 but also animates (UpdatePhysicsMovementAndAnimation)
    explosion_5 = 5, // Explosion explosion / timed effect
    pickup_6 = 6, // static pickup (weapons, health, ...): field_26 is the item parameter, not a ped ref
    pickup_physics_7 = 7, // pickup that moves like physics_3 (dropped items)
    pickup_animated_8 = 8, // animated pickup (like animated_2)
    pickup_physics_animated_9 = 9, // pickup that moves and animates (like physics_animated_4)
    shop_10 = 10, // simple object with special hit logic; takes a shop type from scripts
    light_11 = 11, // runs DispatchFrameAction + UpdateEffectPool only
    effect_only_12 = 12 // runs UpdateEffectPool only
};
} // namespace object_behavior_type

// Properties shared by all objects of one model: size, physics, behaviour, sprite. Looked up by object model id.
class ObjectDefinition
{
  public:
    EXPORT ~ObjectDefinition();
    EXPORT void SetDimensions(Fix16 width, Fix16 height, Fix16 depth);
    EXPORT void SetDimensionsFromSprite();
    EXPORT void SetRemap(s16 remap);
    EXPORT void OffsetBaseSpriteIdxAndSetNumFrames(s16 sprite_idx_offset, s16 num_frames);
    EXPORT Sprite* CreateSpriteFromDefinition() const;
    EXPORT void ApplyDefinitionToSprite(Sprite* pSprite) const;
    EXPORT ObjectDefinition();


    Fix16 mWidth; // 0x0
    Fix16 mHeight; // 0x4
    Fix16 mDepth; // 0x8
    Fix16 mMinSize; // 0xC
    Fix16 mSpeed; // 0x10
    Fix16 mFriction; // 0x14
    Fix16 mMass; // 0x18
    s16 mRemap; // 0x1C
    s16 mBaseSpriteIdx; // 0x1E
    char_type mSpriteFlags; // 0x20
    s32 mObjectIdx; // 0x24
    s32 mSpriteType; // 0x28
    s32 mDrawLayer; // 0x2C
    s32 mPaletteType; // 0x30
    s32 mBehaviorType; // 0x34: One of object_behavior_type
    s32 mImpactDefinitionIdx; // 0x38
    s32 mNextDefinitionIdx; // 0x3C
    s32 mCollisionBucketCategory; // 0x40: One of collision_bucket_category
    s32 mTouchBehavior; // 0x44
    s32 mProjectileHitType; // 0x48
    s32 mCollisionResponse; // 0x4C
    s32 mZCollisionResponse; // 0x50
    s32 mReactToCollisionsWith; // 0x54: One of CollisionReaction
    s32 mUses3dMovement; // 0x58
    s32 mInstanceLimitKind; // 0x5C: One of instance_limit_kind
    char_type mDestroyedByDestructor; // 0x60
    char_type mInPoolList; // 0x61
    char_type mHasShadows; // 0x62
    char_type mMakesCollisionSound; // 0x63
    char_type mNextFrameMax; // 0x64
    char_type mNextDefinitionTimer; // 0x65
    s32 mUsesBulletWalls; // 0x68
    u8 mNumFrames; // 0x6C
    s32 mHasSound; // 0x70
};
GTA2_ASSERT_SIZEOF_ALWAYS(ObjectDefinition, 0x74);

// All object definitions (up to 300), indexed by object model id
class ObjectDefinitions
{
  public:
    enum
    {
        k_max_definitions = 300
    };

    EXPORT ~ObjectDefinitions();
    EXPORT ObjectDefinition* AllocDefinitionWithSprite(s32 idx, s32 sprite_type, s16 base_sprite_idx, u8 num_frames);
    EXPORT void CreateMapObjectDefinitions();
    EXPORT void ApplyMapObjectOverrides();
    EXPORT void CloneAnimatedDefinitions();
    EXPORT void CreateCodeObjectDefinitions();
    EXPORT void CloneRemappedDefinitions();
    EXPORT void ClearColour1PixelsOfDefinitions287To293();
    EXPORT void CacheDef112BaseSpriteIdx();
    EXPORT void ClearColour1PixelsOfDefinitionSprite(s32 idx);
    EXPORT void InitDefinitions();
    EXPORT ObjectDefinition* GetObjectDefinition(s32 idx) const;

    // 9.6f 0x4C6E30
    inline s16 GetObjectBaseSpriteIdx_4C6E30(s32 idx) const
    {
        return GetObjectDefinition(idx)->mBaseSpriteIdx;
    }
    EXPORT ObjectDefinition* CloneDefinition(s32 dst_idx, s32 src_idx);
    EXPORT ObjectDefinition* AllocDefinition(s32 idx);
    EXPORT ObjectDefinitions();

    u16 mNextIdx; // 0x0
    s16 mUnused2; // 0x2
    ObjectDefinition mDefinitions[k_max_definitions]; // 0x4
    ObjectDefinition* mDefinitionByIdx[k_max_definitions]; // 0x87F4
    s16 mDef112BaseSpriteIdx; // 0x8CA4
    s16 mUnused8CA6; // 0x8CA6
};
GTA2_ASSERT_SIZEOF_ALWAYS(ObjectDefinitions, 0x8CA8);

EXTERN_GLOBAL(ObjectDefinitions*, gObjectDefinitions_6FCF00);

EXTERN_GLOBAL(Fix16, kFpQuarter_6F8FAC);
EXTERN_GLOBAL(Fix16, kFpPoint1_6F8FD8);
EXTERN_GLOBAL(Fix16, kFpPoint2_6FC578);
EXTERN_GLOBAL(Fix16, kFpHalf_6FC584);

EXTERN_GLOBAL(s32, gMapObjectOverrides_length_623EEC);
EXTERN_GLOBAL(s32, gCodeObjectTemplates_length_623EF0);

EXTERN_GLOBAL_ARRAY(MapObjectOverride, gMapObjectOverrides_6FC5F8, 24);
EXTERN_GLOBAL_ARRAY(CodeObjectTemplate, gCodeObjectTemplates_6F9038, 126);
