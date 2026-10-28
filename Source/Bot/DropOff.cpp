#include "DropOff.hpp"

static const float kDropHuge = 1e9f;
static const float kDropWalkShort = 3.0f; // tiles: a walk this short needs no getting out first, the goal is at the car

float DropOff_Cost(const DropOffCand& c)
{
    if (c.car_len < 0.0f || c.walk_len < 0.0f)
    {
        return kDropHuge;
    }
    float cost = c.car_len * kDropCarFramesPerTile + c.walk_len * kDropWalkFramesPerTile;
    if (c.walk_len > kDropWalkShort)
    {
        cost += kDropExitFrames;
    }
    return cost;
}

int DropOff_Choose(const DropOffCand* c, int n)
{
    int best = -1;
    float best_cost = kDropHuge;
    for (int i = 0; i < n; i++)
    {
        float cost = DropOff_Cost(c[i]);
        if (i > 0 && best == 0)
        {
            cost /= kDropBeatBy; // later candidates have to be clearly cheaper than the first
        }
        if (cost < best_cost)
        {
            best_cost = cost;
            best = i;
        }
    }
    return best;
}
