#include "Goap.hpp"
#include "BotHeap.hpp"
#include <math.h>
#include <string.h>
#include "BotCompat.hpp"

namespace Goap
{
namespace
{
const float kInf = 1e30f;

float Dist(float ax, float ay, float bx, float by)
{
    const float dx = ax - bx;
    const float dy = ay - by;
    return sqrtf(dx * dx + dy * dy);
}

bool GoalMet(const Entity* e, s32 count, u32 mask, const Goal& goal)
{
    s32 have[kMaxKinds] = {0, 0, 0, 0};
    for (s32 i = 0; i < count; i++)
    {
        if (mask & (1u << i))
        {
            have[e[i].kind]++;
        }
    }
    for (s32 k = 0; k < kMaxKinds; k++)
    {
        if (have[k] < goal.need[k])
        {
            return false;
        }
    }
    return true;
}
} // namespace

void MakePlan(const Entity* e, s32 count, const Goal& goal, float from_x, float from_y, float walk_factor, u32 tie_break, Plan* out)
{
    memset(out, 0, sizeof(*out));
    if (count > kMaxEntities)
    {
        count = kMaxEntities;
    }
    // state = mask * (count + 1) + loc, loc == count means "still at the start"
    const u32 locs = count + 1;
    const u32 states = (1u << count) * locs;
    float* cost = static_cast<float*>(BotHeap_Alloc(sizeof(float) * states));
    u32* parent = static_cast<u32*>(BotHeap_Alloc(sizeof(u32) * states));
    u8* closed = static_cast<u8*>(BotHeap_Alloc(states));
    u32 st;
    for (st = 0; st < states; st++)
    {
        cost[st] = kInf;
        parent[st] = 0xFFFFFFFFu;
    }

    const u32 start = 0 * locs + count;
    cost[start] = 0.0f;
    u32 best_goal = 0xFFFFFFFFu;
    u32 expanded = 0;
    u32 best_progress = 0;

    // Dijkstra with a linear scan: the state space is small (at most 2^14 * 15)
    while (true)
    {
        u32 cur = 0xFFFFFFFFu;
        float cur_cost = kInf;
        for (st = 0; st < states; st++)
        {
            if (!closed[st] && cost[st] < cur_cost)
            {
                cur_cost = cost[st];
                cur = st;
            }
        }
        if (cur == 0xFFFFFFFFu)
        {
            break;
        }
        closed[cur] = 1;
        expanded++;
        const u32 mask = cur / locs;
        const u32 loc = cur % locs;
        if (GoalMet(e, count, mask, goal))
        {
            best_goal = cur;
            break;
        }
        s32 done = 0;
        s32 i;
        for (i = 0; i < count; i++)
        {
            if (mask & (1u << i))
            {
                done++;
            }
        }
        if (static_cast<u32>(done) > best_progress)
        {
            best_progress = done;
        }
        const float lx = loc == static_cast<u32>(count) ? from_x : e[loc].x;
        const float ly = loc == static_cast<u32>(count) ? from_y : e[loc].y;
        for (i = 0; i < count; i++)
        {
            if (mask & (1u << i))
            {
                continue; // precondition: not done yet
            }
            // effect: i done, standing at i
            const u32 next = (mask | (1u << i)) * locs + i;
            // a tiny tie breaker so that equal plans differ by the caller's seed, not by index order
            const float jitter = static_cast<float>(((tie_break + 2654435761u * (i + 1)) >> 8) & 0xFF) * 1e-4f;
            const float c = cost[cur] + Dist(lx, ly, e[i].x, e[i].y) * walk_factor + e[i].cost + jitter;
            if (c < cost[next] && !closed[next])
            {
                cost[next] = c;
                parent[next] = cur;
            }
        }
    }

    out->expanded = expanded;
    out->nearest_miss = best_progress;
    if (best_goal != 0xFFFFFFFFu)
    {
        out->found = true;
        out->total_cost = cost[best_goal];
        s32 order[kMaxEntities];
        s32 n = 0;
        for (u32 s = best_goal; s != start && s != 0xFFFFFFFFu && n < kMaxEntities; s = parent[s])
        {
            order[n++] = static_cast<s32>(s % locs);
        }
        out->length = n;
        s32 k;
        for (k = 0; k < n; k++)
        {
            out->order[k] = order[n - 1 - k];
        }
    }
    BotHeap_Free(cost);
    BotHeap_Free(parent);
    BotHeap_Free(closed);
}
} // namespace Goap
