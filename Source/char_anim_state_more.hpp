#pragma once

// More Char_Anim_state values, kept out of enums.hpp: a name added there changes ImGuiDebugDraw (docs/matching_quirks.md)
namespace Char_Anim_state
{
enum
{
    Knocked_Down_Hit_Front_15 = 15, // with Char_B4_state::Hit_From_Front_33, ends in Lying_Pose_13 or 19
    Knocked_Down_Hit_Behind_16 = 16, // with Char_B4_state::Hit_From_Behind_34, ends in Lying_Pose_14, 19 or 20
    Electrocuted_17 = 17, // ped_state_2::electrocuted_27
    Lying_Pose_19 = 19,
    Lying_Pose_20 = 20,
    Electrocuted_Dead_21 = 21,
};
} // namespace Char_Anim_state
