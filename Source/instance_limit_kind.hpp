#pragma once

// ObjectDefinition::mInstanceLimitKind
namespace instance_limit_kind
{
enum
{
    unlimited_1 = 1,
    limited_count_2 = 2, // counted in Object_5C::mLimitedObjectCount, at most 360
    sprite_list_3 = 3, // kept in Object_5C::mSpriteList, trimmed by TrimSpriteList_529300
};
} // namespace instance_limit_kind
