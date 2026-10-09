#pragma once

// Ped::field_234_lifetime_timer: counts down every update, the ped is removed when it reaches 0
namespace ped_lifetime
{
enum
{
    remove_0 = 0,
    never_expires_99 = 99,
};
} // namespace ped_lifetime
