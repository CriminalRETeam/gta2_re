#pragma once

#include "Function.hpp"
#include "fix16.hpp"

struct object_info;
class Sprite;

// Static table entry (gMapObjectOverrides_6FC5F8): overrides the properties of an existing (map) object definition,
// see ObjectDefinitions_8CA8::ApplyMapObjectOverrides_533360
class MapObjectOverride_54
{
  public:
    // Temporary ctor, otherwise it won't be possible to define gMapObjectOverrides_6FC5F8.
    EXPORT MapObjectOverride_54()
    {
    }
    EXPORT MapObjectOverride_54(s32 param_1,
                  s32& param_2,
                  s32 param_3,
                  s8 param_4,
                  s32& param_5,
                  s32& param_6,
                  s32& param_7,
                  Fix16 param_8,
                  Fix16 param_9,
                  s8 param_10,
                  s32& param_11,
                  s32& param_12,
                  s8 param_13,
                  s32& param_14,
                  Fix16 param_15,
                  s32 param_16,
                  s32 param_17,
                  s32 param_18,
                  s8 param_19,
                  s32 param_20,
                  s8 param_21);

    EXPORT MapObjectOverride_54(s32 param_1,
                  s32 param_2,
                  s32 param_3,
                  s8 param_4,
                  s32 param_5,
                  s32 param_6,
                  s32 param_7,
                  Fix16 param_8,
                  Fix16 param_9,
                  s8 param_10,
                  s32 param_11,
                  s32 param_12,
                  s8 param_13,
                  s32 param_14,
                  Fix16 param_15,
                  s32 param_16,
                  s32 param_17,
                  s32 param_18,
                  s8 param_19,
                  s32 param_20,
                  s8 param_21);

    s32 field_0_definition_idx;
    s32 field_4_behavior_type;
    s32 field_8_next_definition_idx;
    s8 field_C;
    s32 field_10;
    s32 field_14;
    s32 field_18_collision_bucket_category;
    Fix16 field_1C_speed;
    Fix16 field_20_friction;
    s8 field_24;
    s32 field_28;
    s32 field_2C;
    s8 field_30_next_frame_max;
    s32 field_34;
    Fix16 field_38_mass;
    s32 field_3C;
    s8 field_40_sprite_flags;
    s32 field_44_has_sound;
    s8 field_48_has_shadows;
    s32 field_4C;
    s8 field_50;
};

// Static table entry (gCodeObjectTemplates_6F9038): template of a code object, converted into an ObjectDefinition_74 by
// ObjectDefinitions_8CA8::CreateCodeObjectDefinitions_533B30
class CodeObjectTemplate_6C
{
  public:
    // Temporary ctor, otherwise it won't be possible to define gCodeObjectTemplates_6F9038.
    EXPORT CodeObjectTemplate_6C()
    {
    }
    EXPORT CodeObjectTemplate_6C(u32 param_1,
                  u8 param_2,
                  u32& param_3,
                  u32 param_4,
                  u32 param_5,
                  u8 param_6,
                  u32& param_7,
                  u32& param_8,
                  u32& param_9,
                  Fix16 param_10,
                  Fix16 param_11,
                  u8 param_12,
                  u32& param_13,
                  u32& param_14,
                  u32 param_15,
                  u32 param_16,
                  u32& param_17,
                  Fix16 param_18,
                  Fix16 param_19,
                  Fix16 param_20,
                  Fix16 param_21,
                  u32 param_22,
                  u32 param_23,
                  u8 param_24,
                  u32& param_25,
                  u32 param_26,
                  u32 param_27,
                  u8 param_28,
                  u8 param_29);

    EXPORT CodeObjectTemplate_6C(u32 param_1,
                  u8 param_2,
                  u32 param_3,
                  u32 param_4,
                  u32 param_5,
                  u8 param_6,
                  u32 param_7,
                  u32 param_8,
                  u32 param_9,
                  Fix16 param_10,
                  Fix16 param_11,
                  u8 param_12,
                  u32 param_13,
                  u32 param_14,
                  u32 param_15,
                  u32 param_16,
                  u32 param_17,
                  Fix16 param_18,
                  Fix16 param_19,
                  Fix16 param_20,
                  Fix16 param_21,
                  u32 param_22,
                  u32 param_23,
                  u8 param_24,
                  u32 param_25,
                  u32 param_26,
                  u32 param_27,
                  u8 param_28,
                  u8 param_29);

    s32 field_0_definition_idx;
    u8 field_4_num_sprites;
    s32 field_8_behavior_type;
    s32 field_C;
    s32 field_10_next_definition_idx;
    s8 field_14;
    s32 field_18;
    s32 field_1C;
    s32 field_20_collision_bucket_category;
    Fix16 field_24_speed;
    Fix16 field_28_friction;
    s8 field_2C;
    s32 field_30;
    s32 field_34;
    s32 field_38_sprite_type;
    s8 field_3C_next_frame_max;
    s32 field_40;
    Fix16 field_44_mass;
    Fix16 field_48_width;
    Fix16 field_4C_height;
    Fix16 field_50_depth;
    s32 field_54;
    s8 field_58_sprite_flags;
    s8 field_59;
    s32 field_5C;
    s32 field_60;
    s32 field_64_has_sound;
    s8 field_68_has_shadows;
    s8 field_69;
};

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
    static_object_0 = 0, // basic object: simple update + collision
    behavior_1 = 1, // basic object + extra collision handling
    behavior_2 = 2, // animated object (UpdateAnimation + collision)
    bullet_type_3 = 3, // removed from buckets, special update routine
    maybe_moving_obj_4 = 4, // removed from buckets, different special routine
    explosion_5 = 5, // Explosion_30 explosion / timed effect
    behavior_6 = 6, // simple object, no special animation
    behavior_7 = 7, // removed from buckets, special routine (like 3)
    self_animated_8 = 8, // animated object (like 2)
    behavior_9 = 9, // removed from buckets, special routine (like 4)
    behavior_10 = 10, // simple object with special hit logic
    light_type_11 = 11, // runs DispatchFrameAction_525910 + UpdateEffectPool_525B20 only
    behavior_12 = 12 // runs UpdateEffectPool_525B20 only
};
} // namespace object_behavior_type

// Properties shared by all objects of one model: size, physics, behaviour, sprite. Looked up by object model id.
class ObjectDefinition_74
{
  public:
    EXPORT ~ObjectDefinition_74();
    EXPORT void SetDimensions_533060(Fix16 width, Fix16 height, Fix16 depth);
    EXPORT void SetDimensionsFromSprite_533090();
    EXPORT void SetRemap_533110(s16 remap);
    EXPORT void AddSpritePaletteAndSetAnimSpeed_533150(s16 palette_offset, s16 anim_speed);
    EXPORT Sprite* CreateSpriteFromDefinition_533170();
    EXPORT void ApplyDefinitionToSprite_5331A0(Sprite* pSprite);
    EXPORT ObjectDefinition_74();


    Fix16 field_0_width;
    Fix16 field_4_height;
    Fix16 field_8_depth;
    Fix16 field_C_min_size;
    Fix16 field_10_speed;
    Fix16 field_14_friction;
    Fix16 field_18_mass;
    s16 field_1C_remap;
    s16 field_1E_sprite_palette;
    char_type field_20_sprite_flags;
    s32 field_24_object_idx;
    s32 field_28_sprite_type;
    s32 field_2C;
    s32 field_30;
    s32 field_34_behavior_type; // One of object_behavior_type
    s32 field_38;
    s32 field_3C_next_definition_idx;
    s32 field_40_collision_bucket_category; // One of collision_bucket_category
    s32 field_44;
    s32 field_48;
    s32 field_4C;
    s32 field_50;
    s32 field_54_react_to_collisions_with; // One of CollisionReaction
    s32 field_58;
    s32 field_5C;
    char_type field_60;
    char_type field_61;
    char_type field_62_has_shadows;
    char_type field_63;
    char_type field_64_next_frame_max;
    char_type field_65;
    s32 field_68;
    u8 field_6C_sprite_anim_speed;
    s32 field_70_has_sound;
};

// All object definitions (up to 300), indexed by object model id
class ObjectDefinitions_8CA8
{
  public:
    enum
    {
        k_max_definitions = 300
    };

    EXPORT ~ObjectDefinitions_8CA8();
    EXPORT ObjectDefinition_74* AllocDefinitionWithSprite_5332D0(s32 idx, s32 sprite_type, s16 sprite_palette, u8 anim_speed);
    EXPORT void CreateMapObjectDefinitions_533300();
    EXPORT void ApplyMapObjectOverrides_533360();
    EXPORT void CloneAnimatedDefinitions_533420();
    EXPORT void CreateCodeObjectDefinitions_533B30();
    EXPORT void CloneRemappedDefinitions_533C90();
    EXPORT void ClearColour1PixelsOfDefinitions287To293_534270();
    EXPORT void CacheDef112SpritePalette_5342D0();
    EXPORT void ClearColour1PixelsOfDefinitionSprite_5342F0(s32 idx);
    EXPORT void InitDefinitions_534330();
    EXPORT ObjectDefinition_74* GetObjectDefinition_534360(s32 idx);

    // 9.6f 0x4C6E30
    inline s16 GetObjectPalette_4C6E30(s32 idx)
    {
        return GetObjectDefinition_534360(idx)->field_1E_sprite_palette;
    }
    EXPORT ObjectDefinition_74* CloneDefinition_534370(s32 dst_idx, s32 src_idx);
    EXPORT ObjectDefinition_74* AllocDefinition_5343C0(s32 idx);
    EXPORT ObjectDefinitions_8CA8();

    u16 field_0_next_idx;
    s16 field_2;
    ObjectDefinition_74 field_4_definitions[k_max_definitions];
    ObjectDefinition_74* field_87F4_definition_by_idx[k_max_definitions];
    s16 field_8CA4_def112_sprite_palette;
    s16 field_8CA6;
};

EXTERN_GLOBAL(ObjectDefinitions_8CA8*, gObjectDefinitions_6FCF00);

EXTERN_GLOBAL(Fix16, kFpQuarter_6F8FAC);
EXTERN_GLOBAL(Fix16, kFpPoint1_6F8FD8);
EXTERN_GLOBAL(Fix16, kFpPoint2_6FC578);
EXTERN_GLOBAL(Fix16, kFpHalf_6FC584);

EXTERN_GLOBAL(s32, gMapObjectOverrides_length_623EEC);
EXTERN_GLOBAL(s32, gCodeObjectTemplates_length_623EF0);

EXTERN_GLOBAL_ARRAY(MapObjectOverride_54, gMapObjectOverrides_6FC5F8, 24);
EXTERN_GLOBAL_ARRAY(CodeObjectTemplate_6C, gCodeObjectTemplates_6F9038, 126);

EXPORT void InitMapObjectOverrides();
EXPORT void InitCodeObjectTemplates();
