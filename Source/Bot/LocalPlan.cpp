#include "LocalPlan.hpp"
#include <math.h>
#include <string.h>
#include "BotCompat.hpp"

namespace
{
const int kW = 64; // the search window, tiles
const int kH = 64;
const int kDirs = 8;
const int kStateCount = kW * kH * kDirs;

const int kDx[kDirs] = {0, 1, 1, 1, 0, -1, -1, -1};
const int kDy[kDirs] = {-1, -1, 0, 1, 1, 1, 0, -1};
const float kTurnCost[5] = {0.0f, 0.35f, 1.2f, 3.0f, 6.0f}; // by the size of the turn in 45 degree steps

// The search arrays are static: 64 x 64 x 8 states, no allocation while playing
float sCost[kStateCount];
int sParent[kStateCount];
unsigned char sState[kStateCount]; // 0 untouched, 1 open, 2 closed
unsigned char sBlocked[kW * kH];   // 1 closed (solid or a standing car), 2 closed by the clearance rule
float sExtra[kW * kH];             // cost of the tile itself: beside a wall, on a moving car's track

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

int DirOfHeading(float heading_deg)
{
    float h = heading_deg;
    while (h < 0.0f)
    {
        h += 360.0f;
    }
    return static_cast<int>(h / 45.0f + 0.5f) % kDirs;
}

float Octile(int dx, int dy)
{
    const int ax = dx < 0 ? -dx : dx;
    const int ay = dy < 0 ? -dy : dy;
    return (ax > ay ? ax : ay) + 0.4142f * (ax < ay ? ax : ay);
}

// Does the straight line between two points stay on open tiles with room (not beside a wall, not under a standing car)? The
// tiles half the car's width (with its margin) to each side of the line count too
bool LineOpen(int x0, int y0, float ax, float ay, float bx, float by, float half_width)
{
    const float dx = bx - ax;
    const float dy = by - ay;
    const float len = sqrtf(dx * dx + dy * dy);
    if (len < 0.01f)
    {
        return true;
    }
    const float ux = dx / len;
    const float uy = dy / len;
    for (float d = 0.0f; d <= len; d += 0.4f)
    {
        for (int side = -1; side <= 1; side++)
        {
            const float px = ax + ux * d - uy * half_width * side;
            const float py = ay + uy * d + ux * half_width * side;
            const int cx = static_cast<int>(px) - x0;
            const int cy = static_cast<int>(py) - y0;
            if (cx < 0 || cy < 0 || cx >= kW || cy >= kH)
            {
                return false;
            }
            const int i = cy * kW + cx;
            if (sBlocked[i] || (side == 0 && sExtra[i] >= 4.0f))
            {
                return false;
            }
        }
    }
    return true;
}
} // namespace

bool LocalPlan_Find(const LocalPlanWorld& w, float sx, float sy, float heading_deg, float gx, float gy, LocalPlanOut* out)
{
    out->found = false;
    out->count = 0;
    out->car_closed_count = 0;
    // the goal at most 28 tiles off, along the same line
    float dxg = gx - sx;
    float dyg = gy - sy;
    const float dl = sqrtf(dxg * dxg + dyg * dyg);
    if (dl > 28.0f)
    {
        gx = sx + dxg / dl * 28.0f;
        gy = sy + dyg / dl * 28.0f;
        dxg = gx - sx;
        dyg = gy - sy;
    }
    // the window: round both ends with room to go round things, 64 tiles at most
    int x0 = static_cast<int>((sx < gx ? sx : gx)) - 14;
    int y0 = static_cast<int>((sy < gy ? sy : gy)) - 14;
    if (x0 < 1)
    {
        x0 = 1;
    }
    if (y0 < 1)
    {
        y0 = 1;
    }
    const int start_cx = static_cast<int>(sx) - x0;
    const int start_cy = static_cast<int>(sy) - y0;
    if (start_cx < 0 || start_cy < 0 || start_cx >= kW || start_cy >= kH)
    {
        return false;
    }

    // closed tiles: buildings, water, fences; and the clearance rule (a gap closed on both sides is too narrow for the car)
    unsigned char solid[kW * kH];
    for (int cy = 0; cy < kH; cy++)
    {
        for (int cx = 0; cx < kW; cx++)
        {
            solid[cy * kW + cx] = w.solid(w.user, static_cast<float>(x0 + cx) + 0.5f, static_cast<float>(y0 + cy) + 0.5f) ? 1 : 0;
        }
    }
    memset(sExtra, 0, sizeof(sExtra));
    for (int cy = 0; cy < kH; cy++)
    {
        for (int cx = 0; cx < kW; cx++)
        {
            const int i = cy * kW + cx;
            if (solid[i])
            {
                sBlocked[i] = 1;
                continue;
            }
            sBlocked[i] = 0;
            const bool n = cy > 0 && solid[i - kW];
            const bool s = cy < kH - 1 && solid[i + kW];
            const bool wst = cx > 0 && solid[i - 1];
            const bool e = cx < kW - 1 && solid[i + 1];
            // How far to the nearest closed tile: the car's box (with its margin) is wider than half a tile, so a tile right beside
            // a wall has the box in the wall. Beside a wall costs a lot, a diagonal neighbour less, two tiles off a little
            float extra = 0.0f;
            if (n || s || wst || e)
            {
                extra = 6.0f;
            }
            else
            {
                bool diag = false;
                bool two = false;
                for (int dy = -2; dy <= 2; dy++)
                {
                    for (int dx = -2; dx <= 2; dx++)
                    {
                        const int xx = cx + dx;
                        const int yy = cy + dy;
                        if ((dx == 0 && dy == 0) || xx < 0 || yy < 0 || xx >= kW || yy >= kH || !solid[yy * kW + xx])
                        {
                            continue;
                        }
                        const int ad = (dx < 0 ? -dx : dx) > (dy < 0 ? -dy : dy) ? (dx < 0 ? -dx : dx) : (dy < 0 ? -dy : dy);
                        if (ad == 1)
                        {
                            diag = true;
                        }
                        else
                        {
                            two = true;
                        }
                    }
                }
                extra = diag ? 1.8f : (two ? 0.6f : 0.0f);
            }
            sExtra[i] = extra;
            const int ddx = cx - start_cx;
            const int ddy = cy - start_cy;
            if ((n && s) || (wst && e))
            {
                if (ddx * ddx + ddy * ddy > 4)
                {
                    sBlocked[i] = 2; // a gap narrower than the car (not near the start, where the car already is)
                }
            }
        }
    }
    // cars: the tiles under a standing car and a car's width round it cost a lot; a moving car costs along its track
    const float pad = w.own_hw + 0.1f;
    for (int k = 0; k < w.car_count; k++)
    {
        const SteerCar& c = w.cars[k];
        const bool standing = c.speed < 0.05f;
        for (int step = 0; step < (standing ? 1 : 4); step++)
        {
            SteerBox b = c.box;
            b.cx = c.x + c.vx * 20.0f * step;
            b.cy = c.y + c.vy * 20.0f * step;
            const int bx0 = static_cast<int>(b.cx - b.hl - pad - 1.0f) - x0;
            const int bx1 = static_cast<int>(b.cx + b.hl + pad + 1.0f) - x0;
            const int by0 = static_cast<int>(b.cy - b.hl - pad - 1.0f) - y0;
            const int by1 = static_cast<int>(b.cy + b.hl + pad + 1.0f) - y0;
            for (int cy = by0; cy <= by1; cy++)
            {
                for (int cx = bx0; cx <= bx1; cx++)
                {
                    if (cx < 0 || cy < 0 || cx >= kW || cy >= kH)
                    {
                        continue;
                    }
                    if (!SteerBox_Contains(b, static_cast<float>(x0 + cx) + 0.5f, static_cast<float>(y0 + cy) + 0.5f, pad))
                    {
                        continue;
                    }
                    const int i = cy * kW + cx;
                    // the tile the car itself sits on stays open
                    if (cx == start_cx && cy == start_cy)
                    {
                        continue;
                    }
                    if (standing)
                    {
                        // dear, not closed: round it when there is a way, through the cheapest gap (a bump) when there is
                        // none, which beats spinning about in front of a jam
                        if (sBlocked[i] == 0 && sExtra[i] < 60.0f && out->car_closed_count < 96)
                        {
                            out->car_closed_x[out->car_closed_count] = static_cast<short>(x0 + cx);
                            out->car_closed_y[out->car_closed_count] = static_cast<short>(y0 + cy);
                            out->car_closed_count++;
                        }
                        if (sExtra[i] < 60.0f)
                        {
                            sExtra[i] += 60.0f;
                        }
                    }
                    else
                    {
                        sExtra[i] += 4.0f;
                    }
                }
            }
        }
    }

    // the goal: the nearest open tile to it
    int goal_cx = static_cast<int>(gx) - x0;
    int goal_cy = static_cast<int>(gy) - y0;
    if (goal_cx < 0 || goal_cy < 0 || goal_cx >= kW || goal_cy >= kH)
    {
        return false;
    }
    if (sBlocked[goal_cy * kW + goal_cx])
    {
        bool moved = false;
        for (int r = 1; r <= 4 && !moved; r++)
        {
            for (int dy = -r; dy <= r && !moved; dy++)
            {
                for (int dx = -r; dx <= r && !moved; dx++)
                {
                    const int cx = goal_cx + dx;
                    const int cy = goal_cy + dy;
                    if (cx >= 0 && cy >= 0 && cx < kW && cy < kH && !sBlocked[cy * kW + cx])
                    {
                        goal_cx = cx;
                        goal_cy = cy;
                        moved = true;
                    }
                }
            }
        }
        if (!moved)
        {
            return false;
        }
    }

    // A*: a state is a tile and the direction the car went to get there
    memset(sState, 0, sizeof(sState));
    sHeapCount = 0;
    const int start_dir = DirOfHeading(heading_deg);
    const int start_state = ((start_cy * kW + start_cx) * kDirs) + start_dir;
    sCost[start_state] = 0.0f;
    sParent[start_state] = -1;
    sState[start_state] = 1;
    HeapPush(Octile(goal_cx - start_cx, goal_cy - start_cy), start_state);
    const float lx = dxg;
    const float ly = dyg;
    const float ll = sqrtf(lx * lx + ly * ly);
    int goal_state = -1;
    int expanded = 0;
    while (sHeapCount > 0 && expanded < 24000)
    {
        const HeapItem it = HeapPop();
        const int st = it.state;
        if (sState[st] == 2)
        {
            continue;
        }
        sState[st] = 2;
        expanded++;
        const int tile = st / kDirs;
        const int dir = st % kDirs;
        const int cx = tile % kW;
        const int cy = tile / kW;
        if (cx == goal_cx && cy == goal_cy)
        {
            goal_state = st;
            break;
        }
        for (int nd = 0; nd < kDirs; nd++)
        {
            int turn = nd > dir ? nd - dir : dir - nd;
            turn = turn > 4 ? 8 - turn : turn;
            if (turn > 2)
            {
                continue; // a car does not turn more than 90 degrees from one tile to the next
            }
            const int nx = cx + kDx[nd];
            const int ny = cy + kDy[nd];
            if (nx < 0 || ny < 0 || nx >= kW || ny >= kH)
            {
                continue;
            }
            const int ni = ny * kW + nx;
            if (sBlocked[ni])
            {
                continue;
            }
            const bool diagonal = kDx[nd] != 0 && kDy[nd] != 0;
            if (diagonal && (sBlocked[cy * kW + nx] == 1 || sBlocked[ny * kW + cx] == 1))
            {
                continue; // no cutting a corner of a closed tile
            }
            float step = diagonal ? 1.4142f : 1.0f;
            step += kTurnCost[turn] + sExtra[ni];
            // keep to the line: the further a tile is from the line to the goal, the dearer
            if (ll > 1.0f)
            {
                const float px = static_cast<float>(x0 + nx) + 0.5f - sx;
                const float py = static_cast<float>(y0 + ny) + 0.5f - sy;
                const float off = fabsf(px * ly - py * lx) / ll;
                step += 0.12f * off;
            }
            if (w.step_cost)
            {
                step += w.step_cost(w.user, static_cast<float>(x0 + nx) + 0.5f, static_cast<float>(y0 + ny) + 0.5f, kDx[nd], kDy[nd]);
            }
            const int ns = ni * kDirs + nd;
            if (sState[ns] == 2)
            {
                continue;
            }
            const float nc = sCost[st] + step;
            if (sState[ns] == 0 || nc < sCost[ns])
            {
                sCost[ns] = nc;
                sParent[ns] = st;
                sState[ns] = 1;
                HeapPush(nc + Octile(goal_cx - nx, goal_cy - ny), ns);
            }
        }
    }
    if (goal_state < 0)
    {
        return false;
    }
    // the path, from the goal back, then turned round; the first point is the car itself
    int rev[kLocalPlanMax * 4];
    int rev_count = 0;
    for (int st = goal_state; st >= 0 && rev_count < kLocalPlanMax * 4; st = sParent[st])
    {
        rev[rev_count++] = st / kDirs;
    }
    // the tiles of the path, the car first, then smoothed: from each kept point on to the furthest one the straight line to
    // reaches with room all along (a path of 45 degree steps would swing the wheel from side to side)
    float px[kLocalPlanMax * 4];
    float py[kLocalPlanMax * 4];
    int pn = 0;
    px[pn] = sx;
    py[pn] = sy;
    pn++;
    for (int i = rev_count - 2; i >= 0 && pn < kLocalPlanMax * 4; i--)
    {
        px[pn] = static_cast<float>(x0 + rev[i] % kW) + 0.5f;
        py[pn] = static_cast<float>(y0 + rev[i] / kW) + 0.5f;
        pn++;
    }
    int n = 0;
    out->x[n] = px[0];
    out->y[n] = py[0];
    n++;
    int cur = 0;
    while (cur < pn - 1 && n < kLocalPlanMax)
    {
        int best = cur + 1;
        for (int j = pn - 1; j > cur + 1; j--)
        {
            if (LineOpen(x0, y0, px[cur], py[cur], px[j], py[j], w.own_hw + 0.1f))
            {
                best = j;
                break;
            }
        }
        cur = best;
        out->x[n] = px[cur];
        out->y[n] = py[cur];
        n++;
    }
    out->count = n;
    out->length = 0.0f;
    for (int i = 1; i < n; i++)
    {
        const float lx2 = out->x[i] - out->x[i - 1];
        const float ly2 = out->y[i] - out->y[i - 1];
        out->length += sqrtf(lx2 * lx2 + ly2 * ly2);
    }
    out->found = true;
    return true;
}

void LocalPlan_AimPoint(const LocalPlanOut& p, float x, float y, float look, float* aim_x, float* aim_y)
{
    if (!p.found || p.count == 0)
    {
        return;
    }
    // the car's place on the path: the nearest point of the nearest segment (not the nearest node: a smoothed path has few
    // nodes and the nearest may be far behind the car)
    int seg = 0;
    float best = 1e9f;
    float qx = p.x[0];
    float qy = p.y[0];
    for (int i = 0; i + 1 < p.count; i++)
    {
        const float ex = p.x[i + 1] - p.x[i];
        const float ey = p.y[i + 1] - p.y[i];
        const float el2 = ex * ex + ey * ey;
        float t = el2 > 0.0001f ? ((x - p.x[i]) * ex + (y - p.y[i]) * ey) / el2 : 0.0f;
        t = t < 0.0f ? 0.0f : (t > 1.0f ? 1.0f : t);
        const float cx = p.x[i] + ex * t;
        const float cy = p.y[i] + ey * t;
        const float d2 = (cx - x) * (cx - x) + (cy - y) * (cy - y);
        if (d2 < best)
        {
            best = d2;
            seg = i;
            qx = cx;
            qy = cy;
        }
    }
    *aim_x = p.x[p.count - 1];
    *aim_y = p.y[p.count - 1];
    float rest = look;
    float px = qx;
    float py = qy;
    for (int i = seg + 1; i < p.count; i++)
    {
        const float dx = p.x[i] - px;
        const float dy = p.y[i] - py;
        const float d = sqrtf(dx * dx + dy * dy);
        if (d >= rest && d > 0.001f)
        {
            *aim_x = px + dx / d * rest;
            *aim_y = py + dy / d * rest;
            return;
        }
        rest -= d;
        px = p.x[i];
        py = p.y[i];
    }
}

bool LocalPlan_StillGood(const LocalPlanWorld& w, const LocalPlanOut& p, float x, float y)
{
    if (!p.found || p.count < 2)
    {
        return false;
    }
    // the segment the car is on: the nearest point of the polyline
    int seg = 0;
    float best = 1e9f;
    for (int i = 0; i + 1 < p.count; i++)
    {
        const float ex = p.x[i + 1] - p.x[i];
        const float ey = p.y[i + 1] - p.y[i];
        const float el2 = ex * ex + ey * ey;
        float t = el2 > 0.0001f ? ((x - p.x[i]) * ex + (y - p.y[i]) * ey) / el2 : 0.0f;
        t = t < 0.0f ? 0.0f : (t > 1.0f ? 1.0f : t);
        const float qx = p.x[i] + ex * t - x;
        const float qy = p.y[i] + ey * t - y;
        const float d2 = qx * qx + qy * qy;
        if (d2 < best)
        {
            best = d2;
            seg = i;
        }
    }
    if (best > 2.0f * 2.0f)
    {
        return false;
    }
    // what is left of the path from there, and whether anything stands on the next 14 tiles of it
    float left = 0.0f;
    float walked = 0.0f;
    const float pad = w.own_hw + 0.1f;
    for (int i = seg; i + 1 < p.count; i++)
    {
        const float ex = p.x[i + 1] - p.x[i];
        const float ey = p.y[i + 1] - p.y[i];
        const float el = sqrtf(ex * ex + ey * ey);
        left += el;
        if (walked > 14.0f || el < 0.01f)
        {
            continue;
        }
        for (float d = 0.0f; d < el; d += 0.5f)
        {
            const float px = p.x[i] + ex / el * d;
            const float py = p.y[i] + ey / el * d;
            if (w.solid(w.user, px, py))
            {
                return false;
            }
            for (int k = 0; k < w.car_count; k++)
            {
                if (w.cars[k].speed < 0.05f && SteerBox_Contains(w.cars[k].box, px, py, pad))
                {
                    return false;
                }
            }
        }
        walked += el;
    }
    // used up: under 14 tiles left, or under a third of a short path
    return left >= (14.0f < p.length * 0.35f ? 14.0f : p.length * 0.35f);
}
