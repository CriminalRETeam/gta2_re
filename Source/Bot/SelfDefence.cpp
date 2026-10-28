#include "SelfDefence.hpp"

bool SelfDefence_ShouldFight(const SelfDefenceIn& in)
{
    if (!in.mugger)
    {
        return true;
    }
    if (in.cops_near)
    {
        return false; // the police will get the mugger
    }
    return in.frames_on_target < kMuggerBurstFrames;
}
