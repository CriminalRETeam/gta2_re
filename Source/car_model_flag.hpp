#pragma once

// PlayerScoreTracker_36C::field_8C_car_model_flags bits (one byte per car model). Kept in its own header, like
// car_despawn_status.hpp, so that the enum does not land in a header many TUs share.
namespace car_model_flag
{
enum
{
    stolen_1 = 1,    // the player has hijacked this model (all of them: "stl_all" bonus)
    destroyed_2 = 2, // the player has destroyed this model (all of them: "dst_all" bonus)
    both_3 = stolen_1 | destroyed_2,
};
} // namespace car_model_flag
