#pragma once

// Car_BC::field_88_despawn_status (stored as s32). Kept in its own header, like ped_graphic_type.hpp, so
// that the enum does not land in a header many TUs share.
namespace car_despawn_status
{
enum
{
    none_0 = 0,
    active_1 = 1,             // normal, in the world
    despawn_pending_2 = 2,    // despawns once it is off screen and far enough (UpdateCarDespawnStatus_4424C0, mostly NPC cars)
    despawn_soon_3 = 3,       // despawns on the next update
    marked_for_despawn_4 = 4, // becomes despawn_pending_2 on the next update
    despawning_5 = 5,
    despawned_6 = 6,
    deactivated_7 = 7,        // unlinked from the active list (Deactivate_43AA60, crane hook)
};
} // namespace car_despawn_status
