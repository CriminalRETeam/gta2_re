#pragma once

// CarManager::mImpactMode: how Car::ProcessCarToCarImpact treats the cars a crushed car overlaps.
namespace car_impact_mode
{
enum
{
    none_0 = 0,
    crush_damage_1 = 1, // CarPhysics::CheckPendingCollision: damage every area, award score
    crush_max_z_2 = 2, // CarPhysics::UpdateZPhysics: raise CarManager::mZpos to the highest car
};
} // namespace car_impact_mode
