#pragma once

// Hud_Brief::SetHudBriefWithCost priority (mBriefPriority), as the script commands DISPLAY_BRIEF,
// DISPLAY_BRIEF_SOON and DISPLAY_BRIEF_NOW pass it: higher queues first, now_3 always replaces the shown brief.
namespace hud_brief_priority
{
enum
{
    normal_1 = 1,
    soon_2 = 2,
    now_3 = 3,
};
} // namespace hud_brief_priority
