#pragma once

// More Char_B4_state values, kept out of enums.hpp: a name added there changes ImGuiDebugDraw (docs/matching_quirks.md)
namespace Char_B4_state
{
enum
{
    Turning_To_Target_25 = 25, // Char::SetTurnTarget_492400, until the rotation is within 12 of field_14_target_rotation
    Hit_From_Front_33 = 33, // knocked down by a projectile coming from ahead, or by a player's punch
    Hit_From_Behind_34 = 34,
};
} // namespace Char_B4_state
