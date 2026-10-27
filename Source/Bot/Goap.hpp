#pragma once

#include "types.hpp"

// A small GOAP planner (goal oriented action planning). A world state is the set of entities
// already dealt with (a bit mask) plus where the player stands (the last entity visited); an
// action "Visit(entity i)" has the precondition "i not done", the effect "i done, standing at i"
// and a cost in frames derived from the distance. A goal asks for a number of done entities of
// each kind. The search is Dijkstra over (done mask, location) states, so the plan is the
// cheapest order that satisfies the goal, with ties broken by the caller's rng.
namespace Goap
{
enum
{
    kMaxEntities = 14,
    kMaxKinds = 4
};

struct Entity
{
    s32 id;
    s32 kind;   // 0 .. kMaxKinds - 1
    float x, y;
    float cost; // extra frames to complete the action once there (a kill frenzy takes long)
};

struct Goal
{
    s32 need[kMaxKinds]; // done entities wanted per kind
};

struct Plan
{
    bool found;
    s32 length;
    s32 order[kMaxEntities]; // entity indices in the order to do them
    float total_cost;
    u32 expanded;
    u32 nearest_miss; // when not found: the best number of goal items reachable (for the report)
};

// from_x, from_y: where the player is now. walk_factor scales the straight line distance to frames.
void MakePlan(const Entity* entities, s32 count, const Goal& goal, float from_x, float from_y, float walk_factor, u32 tie_break, Plan* out);
} // namespace Goap
