#pragma once

// First argument of PlayerScoreTracker_36C::UpdateAccuracyCount_5934F0 (stored in field_194_last_accuracy_event).
// Kept in its own header, like car_despawn_status.hpp, so that the enum does not land in a header many TUs share.
namespace accuracy_event
{
enum
{
    fire_ended_0 = 0,          // Ped::HandleWeaponFireEnd_46FFF0, resets the streak
    weapon_hit_1 = 1,          // Ped::NotifyWeaponHit_46FF00, resets the streak
    hit_threat_ped_2 = 2,      // Ped::HandlePedHitByObject_45D000, ped that was a threat or the target: streak + 1
    hit_other_ped_3 = 3,       // same function, any other ped: resets the streak
    shot_at_car_4 = 4,         // Ped::HandleShootingAtCar_46FC90, no effect on the streak
};
} // namespace accuracy_event
