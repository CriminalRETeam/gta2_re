#pragma once

// CarManager::field_60_impact_mode: how Car::ProcessCarToCarImpact_43ADC0 treats the cars a crushed car overlaps.
namespace car_impact_mode
{
enum
{
    none_0 = 0,
    crush_damage_1 = 1, // CarPhysics::CheckPendingCollision_55F360: damage every area, award score
    crush_max_z_2 = 2, // CarPhysics::UpdateZPhysics_55AD90: raise CarManager::field_64_zpos to the highest car
};
} // namespace car_impact_mode
