#pragma once

// Ped::field_230_jump_over_mode (Char_B4 checks it before jumping over obstacles)
namespace ped_jump_over_mode
{
enum
{
    cannot_jump_1 = 1,
    can_jump_2 = 2, // player, special peds
};
} // namespace ped_jump_over_mode
