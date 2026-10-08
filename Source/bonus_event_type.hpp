#pragma once

// First argument of BonusTracker_1C0::ProcessBonusEvent_4320D0 / bonus event type of a BonusRule_2C (stored as s16).
// Kept in its own header, like car_despawn_status.hpp, so that the enum does not land in a header many TUs share.
namespace bonus_event_type
{
enum
{
    ped_killed_0 = 0,    // PlayerScoreTracker_36C::AwardPedKilledScore_592660
    car_destroyed_1 = 1, // AwardCarDestroyedScore_592DD0
    car_hijacked_2 = 2,  // AwardCarHijackedScore_593240
    any_event = -1,      // wildcard in a bonus definition
};
} // namespace bonus_event_type
