#include "PickupReach.hpp"

bool PickupReach_Ok(const PickupReachIn& in)
{
    if (!in.path_found || in.path_approx)
    {
        return false;
    }
    if (in.end_z_gap > kReachMaxZGap)
    {
        return false;
    }
    if (in.needed && in.path_len <= kReachNeededLen)
    {
        return true;
    }
    return in.path_len <= in.flat * kReachDetour + kReachDetourExtra;
}
