#pragma once

// More ped_state_2 values, kept out of enums.hpp: a name added there changes ImGuiDebugDraw (docs/matching_quirks.md)
namespace ped_state_2
{
enum
{
    attacking_standing_11 = 11, // with ped_state_1::standing_still_7 and bAttacking, facing the move target
};
} // namespace ped_state_2
