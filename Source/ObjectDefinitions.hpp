#pragma once

#include "Function.hpp"
#include "fix16.hpp"

struct object_info;
class Sprite;

// Static table entry (gMapObjectOverrides_6FC5F8): overrides the properties of an existing (map) object definition,
// see ObjectDefinitions::ApplyMapObjectOverrides_533360
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
                  const s32& param_12,
                  s8 next_frame_max,
                  const s32& uses_3d_movement,
                  Fix16 mass,
                  s32 draw_layer,
                  s32 sprite_flags,
                  s32 has_sound,
                  s8 has_shadows,
                  s32 instance_limit_kind,
                  s8 makes_collision_sound);

    s32 field_0_definition_idx;
    s32 field_4_behavior_type;
    s32 field_8_next_definition_idx;
    s8 field_C_in_pool_list;
    s32 field_10_touch_behavior;
    s32 field_14_projectile_hit_type;
    s32 field_18_collision_bucket_category;
    Fix16 field_1C_speed;
    Fix16 field_20_friction;
    s8 field_24_next_definition_timer;
    s32 field_28_collision_response;
    s32 field_2C;
    s8 field_30_next_frame_max;
    s32 field_34_uses_3d_movement;
    Fix16 field_38_mass;
    s32 field_3C_draw_layer;
    s8 field_40_sprite_flags;
    s32 field_44_has_sound;
    s8 field_48_has_shadows;
    s32 field_4C_instance_limit_kind;
    s8 field_50_makes_collision_sound;
};
GTA2_ASSERT_SIZEOF_ALWAYS(MapObjectOverride, 0x54);

// Static table entry (gCodeObjectTemplates_6F9038): template of a code object, converted into an ObjectDefinition by
// ObjectDefinitions::CreateCodeObjectDefinitions_533B30
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

    s32 field_0_definition_idx;
    u8 field_4_num_sprites;
    s32 field_8_behavior_type;
    s32 field_C_impact_definition_idx;
    s32 field_10_next_definition_idx;
    s8 field_14_in_pool_list;
    s32 field_18_touch_behavior;
    s32 field_1C_projectile_hit_type;
    s32 field_20_collision_bucket_category;
    Fix16 field_24_speed;
    Fix16 field_28_friction;
    s8 field_2C_next_definition_timer;
    s32 field_30_collision_response;
    s32 field_34_z_collision_response;
    s32 field_38_sprite_type;
    s8 field_3C_next_frame_max;
    s32 field_40_uses_3d_movement;
    Fix16 field_44_mass;
    Fix16 field_48_width;
    Fix16 field_4C_height;
    Fix16 field_50_depth;
    s32 field_54_draw_layer;
    s8 field_58_sprite_flags;
    s8 field_59_destroyed_by_destructor;
    s32 field_5C_instance_limit_kind;
    s32 field_60_uses_bullet_walls;
    s32 field_64_has_sound;
    s8 field_68_has_shadows;
    s8 field_69_makes_collision_sound;
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
    light_11 = 11, // runs DispatchFrameAction_525910 + UpdateEffectPool_525B20 only
    effect_only_12 = 12 // runs UpdateEffectPool_525B20 only
};
} // namespace object_behavior_type

// Properties shared by all objects of one model: size, physics, behaviour, sprite. Looked up by object model id.
class ObjectDefinition
{
  public:
    EXPORT ~ObjectDefinition();
    EXPORT void SetDimensions_533060(Fix16 width, Fix16 height, Fix16 depth);
    EXPORT void SetDimensionsFromSprite_533090();
    EXPORT void SetRemap_533110(s16 remap);
    EXPORT void OffsetBaseSpriteIdxAndSetNumFrames_533150(s16 sprite_idx_offset, s16 anim_speed);
    EXPORT Sprite* CreateSpriteFromDefinition_533170();
    EXPORT void ApplyDefinitionToSprite_5331A0(Sprite* pSprite);
    EXPORT ObjectDefinition();


    Fix16 field_0_width;
    Fix16 field_4_height;
    Fix16 field_8_depth;
    Fix16 field_C_min_size;
    Fix16 field_10_speed;
    Fix16 field_14_friction;
    Fix16 field_18_mass;
    s16 field_1C_remap;
    s16 field_1E_base_sprite_idx;
    char_type field_20_sprite_flags;
    s32 field_24_object_idx;
    s32 field_28_sprite_type;
    s32 field_2C_draw_layer;
    s32 field_30_palette_type;
    s32 field_34_behavior_type; // One of object_behavior_type
    s32 field_38_impact_definition_idx;
    s32 field_3C_next_definition_idx;
    s32 field_40_collision_bucket_category; // One of collision_bucket_category
    s32 field_44_touch_behavior;
    s32 field_48_projectile_hit_type;
    s32 field_4C_collision_response;
    s32 field_50_z_collision_response;
    s32 field_54_react_to_collisions_with; // One of CollisionReaction
    s32 field_58_uses_3d_movement;
    s32 field_5C_instance_limit_kind;
    char_type field_60_destroyed_by_destructor;
    char_type field_61_in_pool_list;
    char_type field_62_has_shadows;
    char_type field_63_makes_collision_sound;
    char_type field_64_next_frame_max;
    char_type field_65_next_definition_timer;
    s32 field_68_uses_bullet_walls;
    u8 field_6C_num_frames;
    s32 field_70_has_sound;
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
    EXPORT ObjectDefinition* AllocDefinitionWithSprite_5332D0(s32 idx, s32 sprite_type, s16 sprite_palette, u8 anim_speed);
    EXPORT void CreateMapObjectDefinitions_533300();
    EXPORT void ApplyMapObjectOverrides_533360();
    EXPORT void CloneAnimatedDefinitions_533420();
    EXPORT void CreateCodeObjectDefinitions_533B30();
    EXPORT void CloneRemappedDefinitions_533C90();
    EXPORT void ClearColour1PixelsOfDefinitions287To293_534270();
    EXPORT void CacheDef112BaseSpriteIdx_5342D0();
    EXPORT void ClearColour1PixelsOfDefinitionSprite_5342F0(s32 idx);
    EXPORT void InitDefinitions_534330();
    EXPORT ObjectDefinition* GetObjectDefinition_534360(s32 idx);

    // 9.6f 0x4C6E30
    inline s16 GetObjectBaseSpriteIdx_4C6E30(s32 idx)
    {
        return GetObjectDefinition_534360(idx)->field_1E_base_sprite_idx;
    }
    EXPORT ObjectDefinition* CloneDefinition_534370(s32 dst_idx, s32 src_idx);
    EXPORT ObjectDefinition* AllocDefinition_5343C0(s32 idx);
    EXPORT ObjectDefinitions();

    u16 field_0_next_idx;
    s16 field_2_unused;
    ObjectDefinition field_4_definitions[k_max_definitions];
    ObjectDefinition* field_87F4_definition_by_idx[k_max_definitions];
    s16 field_8CA4_def112_base_sprite_idx;
    s16 field_8CA6_unused;
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
