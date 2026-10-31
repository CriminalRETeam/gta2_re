#pragma once

// Hud_Message::ShowMessage second argument (stored in mPriority): a new message replaces the one
// being shown unless the shown one has a higher priority. Kept in its own header, like car_despawn_status.hpp, so that
// the enum does not land in a header many TUs share.
namespace hud_message_priority
{
enum
{
    normal_1 = 1,    // in game events: bonuses, pickups, death text
    important_3 = 3, // mission messages, game over, "you're IT"
};
} // namespace hud_message_priority
