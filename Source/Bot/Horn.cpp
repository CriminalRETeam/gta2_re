#include "Horn.hpp"

bool Horn_Press(const HornIn& in, int frame)
{
    if (in.running_over || !in.ped_ahead || in.ped_dist < kHornNear || in.ped_dist > kHornFar)
    {
        return false;
    }
    return frame % kHornPeriod < kHornBurst;
}
