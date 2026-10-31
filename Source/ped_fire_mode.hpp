#pragma once

// Ped::mFireMode, read by Ped::ApplyAimJitter
namespace ped_fire_mode
{
enum
{
    random_shots_0 = 0, // pistol / smg shots are randomly skipped
    normal_1 = 1,
    never_fires_2 = 2,
};
} // namespace ped_fire_mode
