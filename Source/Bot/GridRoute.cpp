#include "GridRoute.hpp"
#include <string.h>
#include <math.h>

namespace
{
const int kMaxSide = 256;
const int kStateCount = kMaxSide * kMaxSide * 4;
const int kDx[4] = {0, 1, 0, -1};
const int kDy[4] = {-1, 0, 1, 0};

// static: 256 x 256 x 4 states, no allocation while playing
float sCost[kStateCount];
int sParent[kStateCount];
unsigned char sState[kStateCount]; // 0 untouched, 1 open, 2 closed

struct HeapItem
{
    float f;
    int state;
};
HeapItem sHeap[kStateCount * 2];
int sHeapCount = 0;

void HeapPush(float f, int state)
{
    if (sHeapCount >= kStateCount * 2 - 1)
    {
        return;
    }
    int i = sHeapCount++;
    sHeap[i].f = f;
    sHeap[i].state = state;
    while (i > 0)
    {
        const int p = (i - 1) / 2;
        if (sHeap[p].f <= sHeap[i].f)
        {
            break;
        }
        const HeapItem t = sHeap[p];
        sHeap[p] = sHeap[i];
        sHeap[i] = t;
        i = p;
    }
}

HeapItem HeapPop()
{
    const HeapItem top = sHeap[0];
    sHeap[0] = sHeap[--sHeapCount];
    int i = 0;
    for (;;)
    {
        const int l = i * 2 + 1;
        const int r = l + 1;
        int m = i;
        if (l < sHeapCount && sHeap[l].f < sHeap[m].f)
        {
            m = l;
        }
        if (r < sHeapCount && sHeap[r].f < sHeap[m].f)
        {
            m = r;
        }
        if (m == i)
        {
            break;
        }
        const HeapItem t = sHeap[m];
        sHeap[m] = sHeap[i];
        sHeap[i] = t;
        i = m;
    }
    return top;
}

float Manhattan(int dx, int dy)
{
    return static_cast<float>((dx < 0 ? -dx : dx) + (dy < 0 ? -dy : dy));
}
} // namespace

void GridRoute_DefaultParams(GridRouteParams* p)
{
    p->turn_cost = 2.5f;
    p->uturn_cost = 10.0f;
    p->start_dir = -1;
    p->max_expand = 200000;
    p->allow_partial = true;
    p->min_partial_gain = 4.0f;
    p->h_weight = 1.0f;
}

int GridRoute_DirOfHeading(float heading_deg)
{
    float h = heading_deg;
    while (h < 0.0f)
    {
        h += 360.0f;
    }
    return static_cast<int>(h / 90.0f + 0.5f) % 4;
}

bool GridRoute_Find(const GridRouteWorld& w, int sx, int sy, int gx, int gy, const GridRouteParams& p, GridRouteOut* out)
{
    out->found = false;
    out->partial = false;
    out->count = 0;
    out->length = 0.0f;
    out->expanded = 0;
    if (w.w < 1 || w.h < 1 || w.w > kMaxSide || w.h > kMaxSide)
    {
        return false;
    }
    const int scx = sx - w.x0;
    const int scy = sy - w.y0;
    const int gcx = gx - w.x0;
    const int gcy = gy - w.y0;
    if (scx < 0 || scy < 0 || scx >= w.w || scy >= w.h || gcx < 0 || gcy < 0 || gcx >= w.w || gcy >= w.h)
    {
        return false;
    }
    const int tiles = w.w * w.h;
    memset(sState, 0, static_cast<size_t>(tiles) * 4);
    sHeapCount = 0;

    // a start with no direction is any direction: all four are open at no cost
    int goal_state = -1;
    int near_state = -1;
    const float start_h = Manhattan(gcx - scx, gcy - scy);
    float near_h = start_h;
    for (int d = 0; d < 4; d++)
    {
        if (p.start_dir >= 0 && d != p.start_dir)
        {
            continue;
        }
        const int st = (scy * w.w + scx) * 4 + d;
        sCost[st] = 0.0f;
        sParent[st] = -1;
        sState[st] = 1;
        HeapPush(start_h * p.h_weight, st);
        if (near_state < 0)
        {
            near_state = st;
        }
    }
    int expanded = 0;
    while (sHeapCount > 0 && expanded < p.max_expand)
    {
        const HeapItem it = HeapPop();
        const int st = it.state;
        if (sState[st] == 2)
        {
            continue;
        }
        sState[st] = 2;
        expanded++;
        const int tile = st >> 2;
        const int dir = st & 3;
        const int cx = tile % w.w;
        const int cy = tile / w.w;
        if (cx == gcx && cy == gcy)
        {
            goal_state = st;
            break;
        }
        const float h_here = Manhattan(gcx - cx, gcy - cy);
        if (h_here < near_h - 0.01f)
        {
            near_h = h_here;
            near_state = st;
        }
        for (int nd = 0; nd < 4; nd++)
        {
            const int nx = cx + kDx[nd];
            const int ny = cy + kDy[nd];
            if (nx < 0 || ny < 0 || nx >= w.w || ny >= w.h)
            {
                continue;
            }
            const int ns = (ny * w.w + nx) * 4 + nd;
            if (sState[ns] == 2)
            {
                continue;
            }
            const float extra = w.step(w.user, w.x0 + cx, w.y0 + cy, nd, sCost[st]);
            if (extra < 0.0f)
            {
                continue;
            }
            float cost = 1.0f + extra;
            if (nd != dir)
            {
                cost += ((nd + 2) & 3) == dir ? p.uturn_cost : p.turn_cost;
            }
            const float nc = sCost[st] + cost;
            if (sState[ns] == 0 || nc < sCost[ns])
            {
                sCost[ns] = nc;
                sParent[ns] = st;
                sState[ns] = 1;
                HeapPush(nc + Manhattan(gcx - nx, gcy - ny) * p.h_weight, ns);
            }
        }
    }
    out->expanded = expanded;
    if (goal_state < 0 && p.allow_partial && near_state >= 0 && start_h - near_h >= p.min_partial_gain)
    {
        goal_state = near_state;
        out->partial = true;
    }
    if (goal_state < 0)
    {
        return false;
    }
    int n = 0;
    for (int st = goal_state; st >= 0 && n < kGridRouteMax; st = sParent[st])
    {
        const int tile = st >> 2;
        out->x[n] = static_cast<short>(w.x0 + tile % w.w);
        out->y[n] = static_cast<short>(w.y0 + tile / w.w);
        n++;
    }
    if (n >= kGridRouteMax)
    {
        // too long to hold: keep the first stretch, which is what is driven; the caller replans from further along
        out->partial = true;
    }
    for (int i = 0; i < n / 2; i++)
    {
        const short tx = out->x[i];
        const short ty = out->y[i];
        out->x[i] = out->x[n - 1 - i];
        out->y[i] = out->y[n - 1 - i];
        out->x[n - 1 - i] = tx;
        out->y[n - 1 - i] = ty;
    }
    out->count = n;
    out->length = static_cast<float>(n > 0 ? n - 1 : 0);
    out->found = true;
    return true;
}

int GridRoute_Corners(const GridRouteOut& r, float* xs, float* ys, int max)
{
    int n = 0;
    for (int i = 0; i < r.count && n < max; i++)
    {
        const bool end = i == 0 || i == r.count - 1;
        bool corner = end;
        if (!end)
        {
            corner = (r.x[i] - r.x[i - 1]) != (r.x[i + 1] - r.x[i]) || (r.y[i] - r.y[i - 1]) != (r.y[i + 1] - r.y[i]);
        }
        if (corner)
        {
            xs[n] = static_cast<float>(r.x[i]) + 0.5f;
            ys[n] = static_cast<float>(r.y[i]) + 0.5f;
            n++;
        }
    }
    return n;
}
