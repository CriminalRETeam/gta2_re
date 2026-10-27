#pragma once

// ObjectDefinition::field_5C_instance_limit_kind
namespace instance_limit_kind
{
enum
{
    unlimited_1 = 1,
    limited_count_2 = 2, // counted in Object_5C::field_10_limited_object_count, at most 360
    sprite_list_3 = 3, // kept in Object_5C::field_1C_sprite_list, trimmed by TrimSpriteList_529300
};
} // namespace instance_limit_kind
