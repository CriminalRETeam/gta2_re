#pragma once

// More objects values, kept out of enums.hpp: a name added there changes ImGuiDebugDraw (docs/matching_quirks.md)
namespace objects
{
enum
{
    dead_phone_174 = 174, // SCRCMD_SET_PHONE_DEAD_50C040; a blue_phone_not_ringing_163 that can't be touched
    red_phone_not_ringing_176 = 176,
    yellow_phone_not_ringing_178 = 178,
    green_phone_not_ringing_180 = 180,
    grenade_lying_still_182 = 182, // the last frame of grenade_obj_183, without speed
    blue_phone_answered_185 = 185,
    red_phone_after_answer_186 = 186,
    red_phone_answered_187 = 187,
    yellow_phone_after_answer_188 = 188,
};
} // namespace objects
