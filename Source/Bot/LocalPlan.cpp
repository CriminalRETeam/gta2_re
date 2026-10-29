#include "LocalPlan.hpp"
#include "FinePlan.hpp"
#include "GridRoute.hpp"
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
unsigned char sDyn[kW * kH];       // a standing car, a pedestrian or a police car is on or beside the tile (cars and pedestrians, not walls)
float sExtra[kW * kH];             // cost of the tile itself: beside a wall, on a moving car's track
unsigned short sOcc[kW * kH];      // bit n: a moving car is over the tile n * 10 frames from now
const int kTimeBuckets = 10;
const float kMovingCarCost = 30.0f; // about a bump: a way round of a few tiles and turns is cheaper (at 12 a head-on car was driven into)

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

// Tiles nearer the goal that a route to the nearest reachable tile must get
static const float kMinPartialGain = 4.0f;

float Octile(int dx, int dy)
{
    const int ax = dx < 0 ? -dx : dx;
    const int ay = dy < 0 ? -dy : dy;
    return (ax > ay ? ax : ay) + 0.4142f * (ax < ay ? ax : ay);
}

// How far (tiles) is the point from the global route ahead (the polyline route_x/route_y)?
float DistToRoute(const LocalPlanWorld& w, float px, float py)
{
    float best = 1e9f;
    for (int i = 0; i + 1 < w.route_n; i++)
    {
        const float ex = w.route_x[i + 1] - w.route_x[i];
        const float ey = w.route_y[i + 1] - w.route_y[i];
        const float el2 = ex * ex + ey * ey;
        float t = el2 > 0.0001f ? ((px - w.route_x[i]) * ex + (py - w.route_y[i]) * ey) / el2 : 0.0f;
        t = t < 0.0f ? 0.0f : (t > 1.0f ? 1.0f : t);
        const float qx = w.route_x[i] + ex * t - px;
        const float qy = w.route_y[i] + ey * t - py;
        const float d = sqrtf(qx * qx + qy * qy);
        best = d < best ? d : best;
    }
    return best;
}

// Does the straight line between two points stay on open tiles with room (not beside a wall, not under a standing car)? The
// tiles half the car's width (with its margin) to each side of the line count too
bool LineOpen(const LocalPlanWorld& w, int x0, int y0, float ax, float ay, float bx, float by, float half_width, float raw_dev)
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
    int prev_tx = static_cast<int>(ax);
    int prev_ty = static_cast<int>(ay);
    // keeping to the global route: a straight line may leave it by 0.8 of a tile, or by as much as the square route it replaces does (that goes round a car), and more only near its own start (the car's place, which
    // may be off the route: it joins within 4 tiles, not by a long diagonal across the lanes)
    const float start_off = w.route_n >= 2 ? DistToRoute(w, ax, ay) : 0.0f;
    for (float d = 0.0f; d <= len; d += 0.4f)
    {
        if (w.route_n >= 2)
        {
            const float allowed = (raw_dev > 0.8f ? raw_dev + 0.3f : 0.8f) + start_off * (d < 4.0f ? 1.0f - d / 4.0f : 0.0f);
            if (DistToRoute(w, ax + ux * d, ay + uy * d) > allowed)
            {
                return false;
            }
        }
        if (w.edge_ok)
        {
            // the line crosses from tile to tile: a wall or a step between them shuts it (one axis at a time on a diagonal)
            const int tx = static_cast<int>(ax + ux * d);
            const int ty = static_cast<int>(ay + uy * d);
            if (tx != prev_tx && !w.edge_ok(w.user, prev_tx, prev_ty, tx, prev_ty))
            {
                return false;
            }
            if (ty != prev_ty && !w.edge_ok(w.user, tx, prev_ty, tx, ty))
            {
                return false;
            }
            prev_tx = tx;
            prev_ty = ty;
        }
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
            // (and no tile a moving car will be over: the square route has gone round those, a straight line over them would go through)
            if (sBlocked[i] || (side == 0 && sExtra[i] >= 4.0f) || sOcc[i])
            {
                return false;
            }
            if (side == 0 && w.ramp_zone)
            {
                // a ramp, or the two tiles before it: along its axis, in the middle of the tile
                const int zone = w.ramp_zone(w.user, px, py) & 3;
                if (zone == 2 && (fabsf(ux) > 0.12f || fabsf(px - floorf(px) - 0.5f) > 0.3f))
                {
                    return false;
                }
                if (zone == 1 && (fabsf(uy) > 0.12f || fabsf(py - floorf(py) - 0.5f) > 0.3f))
                {
                    return false;
                }
            }
        }
    }
    return true;
}

// Is the tile (window coordinates x0, y0 added) closed, or off the window?
bool TileClosed(int x0, int y0, int tx, int ty)
{
    const int cx = tx - x0;
    const int cy = ty - y0;
    return cx < 0 || cy < 0 || cx >= kW || cy >= kH || sBlocked[cy * kW + cx] != 0;
}

// A chord across a corner: its end points and its middle are on tiles that are open (not closed, no moving car over them) and not on a
// ramp or the approach to one. Beside a wall is fine: the corner is where the way turns, and the point of cutting it is the wall straight on
bool ChordOpen(const LocalPlanWorld& w, int x0, int y0, float ax, float ay, float bx, float by)
{
    for (int k = 0; k <= 2; k++)
    {
        const float px = ax + (bx - ax) * 0.5f * k;
        const float py = ay + (by - ay) * 0.5f * k;
        const int cx = static_cast<int>(px) - x0;
        const int cy = static_cast<int>(py) - y0;
        if (cx < 0 || cy < 0 || cx >= kW || cy >= kH || sBlocked[cy * kW + cx] || sOcc[cy * kW + cx] || sExtra[cy * kW + cx] >= 40.0f)
        {
            return false;
        }
        if (w.ramp_zone && w.ramp_zone(w.user, px, py))
        {
            return false;
        }
    }
    return true;
}

// The global route as the detailed route: the corner points from the car's place on its leg on, the car joining the leg within 4 tiles
// when it is off it. Used as it is when nothing in the way (no wall, no standing car or pedestrian, no moving car over a tile when we
// would get there); the planner below only goes round what is in the way. Writes the vertices to ox/oy, returns false when the route
// cannot be driven as it is
bool FollowRoute(const LocalPlanWorld& w, int x0, int y0, float sx, float sy, float speed, float* ox, float* oy, int* on)
{
    if (w.route_n < 2)
    {
        return false;
    }
    float vx[kLocalRouteMax + 4];
    float vy[kLocalRouteMax + 4];
    int vn = 0;
    vx[vn] = sx;
    vy[vn] = sy;
    vn++;
    const float off = DistToRoute(w, sx, sy);
    int i = 1;
    if (off > 0.35f)
    {
        // join the leg: a point 4 tiles along the route
        float cx = w.route_x[0];
        float cy = w.route_y[0];
        float remaining = 4.0f;
        bool merged = false;
        for (; i < w.route_n && !merged; i++)
        {
            const float ex = w.route_x[i] - cx;
            const float ey = w.route_y[i] - cy;
            const float el = sqrtf(ex * ex + ey * ey);
            if (el >= remaining && el > 0.001f)
            {
                vx[vn] = cx + ex / el * remaining;
                vy[vn] = cy + ey / el * remaining;
                vn++;
                merged = true;
                i--; // this leg's far end is still to come
            }
            else
            {
                remaining -= el;
                cx = w.route_x[i];
                cy = w.route_y[i];
            }
        }
        if (!merged)
        {
            i = w.route_n - 1; // the route is shorter than 4 tiles: its end
        }
    }
    for (; i < w.route_n && vn < kLocalRouteMax + 4; i++)
    {
        vx[vn] = w.route_x[i];
        vy[vn] = w.route_y[i];
        vn++;
    }
    if (vn < 2)
    {
        return false;
    }
    // is it clear all along?
    float d = 0.0f;
    int prev_tx = static_cast<int>(vx[0]);
    int prev_ty = static_cast<int>(vy[0]);
    for (int k = 0; k + 1 < vn; k++)
    {
        const float ex = vx[k + 1] - vx[k];
        const float ey = vy[k + 1] - vy[k];
        const float el = sqrtf(ex * ex + ey * ey);
        for (float t = 0.0f; t <= el; t += 0.4f)
        {
            const float px = vx[k] + (el > 0.001f ? ex / el * t : 0.0f);
            const float py = vy[k] + (el > 0.001f ? ey / el * t : 0.0f);
            const int tx = static_cast<int>(px);
            const int ty = static_cast<int>(py);
            const int cx = tx - x0;
            const int cy = ty - y0;
            if (cx < 0 || cy < 0 || cx >= kW || cy >= kH || sBlocked[cy * kW + cx])
            {
                return false;
            }
            if (w.edge_ok && (tx != prev_tx || ty != prev_ty))
            {
                if ((tx != prev_tx && !w.edge_ok(w.user, prev_tx, prev_ty, tx, prev_ty)) || (ty != prev_ty && !w.edge_ok(w.user, tx, prev_ty, tx, ty)))
                {
                    return false;
                }
            }
            prev_tx = tx;
            prev_ty = ty;
            if (w.ramp_zone)
            {
                // a ramp or its approach: only along its axis, in the middle of the tile (the car joins the leg before it, not on it)
                const int zone = w.ramp_zone(w.user, px, py) & 3;
                const float ux = el > 0.001f ? ex / el : 0.0f;
                const float uy = el > 0.001f ? ey / el : 0.0f;
                if ((zone == 2 && (fabsf(ux) > 0.12f || fabsf(px - floorf(px) - 0.5f) > 0.3f)) || (zone == 1 && (fabsf(uy) > 0.12f || fabsf(py - floorf(py) - 0.5f) > 0.3f)))
                {
                    return false;
                }
            }
            // a car, a pedestrian: not on or beside the tile (the tile marks already include our own width); a moving car: not over the
            // tile at the time we would get there
            const int i2 = cy * kW + cx;
            int bucket = static_cast<int>((d + t) / speed / 10.0f);
            bucket = bucket > kTimeBuckets - 1 ? kTimeBuckets - 1 : bucket;
            if (sDyn[i2] || (sOcc[i2] & (1 << bucket)))
            {
                return false;
            }
        }
        d += el;
    }
    for (int k = 0; k < vn; k++)
    {
        ox[k] = vx[k];
        oy[k] = vy[k];
    }
    *on = vn;
    return true;
}

struct PlanCtx
{
    const LocalPlanWorld* w;
    int x0, y0;
    float speed;
    float sx, sy, lx, ly, ll; // the line from the start to the goal
    const float* rx;
    const float* ry;
    int rn;
};

float PlanStep(void* user, int x, int y, int dir, float g)
{
    const PlanCtx* c = static_cast<const PlanCtx*>(user);
    static const int dxs[4] = {0, 1, 0, -1};
    static const int dys[4] = {-1, 0, 1, 0};
    const int nx = x + dxs[dir] - c->x0;
    const int ny = y + dys[dir] - c->y0;
    if (nx < 0 || ny < 0 || nx >= kW || ny >= kH)
    {
        return -1.0f;
    }
    const int ni = ny * kW + nx;
    if (sBlocked[ni])
    {
        return -1.0f;
    }
    if (c->w->edge_ok && !c->w->edge_ok(c->w->user, x, y, x + dxs[dir], y + dys[dir]))
    {
        return -1.0f;
    }
    float cost = sExtra[ni];
    // among routes as long, keep to the global route (or the line to the goal): the car joins it at once and follows
    // it, and the same corner is chosen every time the route is planned
    {
        const float px = static_cast<float>(x + dxs[dir]) + 0.5f;
        const float py = static_cast<float>(y + dys[dir]) + 0.5f;
        float off = 0.0f;
        if (c->rn >= 2)
        {
            off = 1e9f;
            for (int i = 0; i + 1 < c->rn; i++)
            {
                const float ex = c->rx[i + 1] - c->rx[i];
                const float ey = c->ry[i + 1] - c->ry[i];
                const float el2 = ex * ex + ey * ey;
                float t = el2 > 0.0001f ? ((px - c->rx[i]) * ex + (py - c->ry[i]) * ey) / el2 : 0.0f;
                t = t < 0.0f ? 0.0f : (t > 1.0f ? 1.0f : t);
                const float qx = c->rx[i] + ex * t - px;
                const float qy = c->ry[i] + ey * t - py;
                const float d = sqrtf(qx * qx + qy * qy);
                off = d < off ? d : off;
            }
            off = off > 6.0f ? 6.0f : off;
            cost += 0.6f * off; // hugs the global route: joins it within a few tiles and stays in its lane
        }
        else if (c->ll > 1.0f)
        {
            off = fabsf((px - c->sx) * c->ly - (py - c->sy) * c->lx) / c->ll;
            cost += 0.03f * off;
        }
    }
    // the frames until we get there (g tiles at our speed), in tens: which time bucket of the moving cars to look at
    int bucket = static_cast<int>(g / c->speed / 10.0f);
    bucket = bucket > kTimeBuckets - 1 ? kTimeBuckets - 1 : bucket;
    if (sOcc[ni] & (1 << bucket))
    {
        cost += kMovingCarCost;
    }
    if (c->w->step_cost)
    {
        cost += c->w->step_cost(c->w->user, static_cast<float>(x + dxs[dir]) + 0.5f, static_cast<float>(y + dys[dir]) + 0.5f, dxs[dir], dys[dir]);
    }
    return cost;
}

} // namespace

bool LocalPlan_Find(const LocalPlanWorld& w, float sx, float sy, float heading_deg, float gx, float gy, LocalPlanOut* out)
{
    out->found = false;
    out->partial = false;
    out->count = 0;
    out->car_closed_count = 0;
    out->tile_count = 0;
    out->moving_count = 0;
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
    memset(sOcc, 0, sizeof(sOcc));
    memset(sDyn, 0, sizeof(sDyn));
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
                    // a one tile gap between two closed tiles: the car (about half a tile wide) fits, and the whole-map route uses
                    // such alleys, so it is dear to go through but not closed
                    sExtra[i] += 8.0f;
                }
            }
        }
    }
    // cars: the tiles under a standing car and a car's width round it cost a lot; a moving car costs along its track
    const float pad0 = w.own_hw + 0.1f;
    for (int k = 0; k < w.car_count; k++)
    {
        const SteerCar& c = w.cars[k];
        // a police car is kept well clear of whether it moves or not (a bump brings the wanted level): closed, with extra room, where
        // it is and where it will be in the next 30 frames
        const float pad = c.police ? pad0 + 0.5f : pad0;
        const bool standing = c.speed < 0.05f || c.police;
        for (int step = 0; step < (c.police ? 4 : (standing ? 1 : kTimeBuckets)); step++)
        {
            SteerBox b = c.box;
            b.cx = c.x + c.vx * 10.0f * step;
            b.cy = c.y + c.vy * 10.0f * step;
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
                        sDyn[i] = 1;
                        if (sExtra[i] < 60.0f)
                        {
                            sExtra[i] += 60.0f;
                        }
                    }
                    else
                    {
                        // over this tile about now + step * 10 frames: the tile is in the way for whoever gets there then
                        unsigned short mask = static_cast<unsigned short>(1 << step);
                        if (step > 0)
                        {
                            mask = static_cast<unsigned short>(mask | (1 << (step - 1)));
                        }
                        if (step < kTimeBuckets - 1)
                        {
                            mask = static_cast<unsigned short>(mask | (1 << (step + 1)));
                        }
                        sOcc[i] = static_cast<unsigned short>(sOcc[i] | mask);
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

    // First choice: the global route as it is, when nothing is in its way. Only then is there a search for a way round
    int n = 0;
    bool followed = false;
    float px[kLocalPlanMax * 4];
    float py[kLocalPlanMax * 4];
    int pn = 0;
    {
        const float follow_speed = (w.own_speed > 0.02f && w.own_speed < 1.0f) ? w.own_speed : 0.1f;
        float fx[kLocalRouteMax + 4];
        float fy[kLocalRouteMax + 4];
        int fn = 0;
        if (FollowRoute(w, x0, y0, sx, sy, follow_speed, fx, fy, &fn))
        {
            for (int i = 0; i < fn && pn < kLocalPlanMax * 4; i++)
            {
                px[pn] = fx[i];
                py[pn] = fy[i];
                pn++;
            }
            for (int cy = 0; cy < kH && out->moving_count < 96; cy++)
            {
                for (int cx = 0; cx < kW && out->moving_count < 96; cx++)
                {
                    if (sOcc[cy * kW + cx] & 0x0F)
                    {
                        out->moving_x[out->moving_count] = static_cast<short>(x0 + cx);
                        out->moving_y[out->moving_count] = static_cast<short>(y0 + cy);
                        out->moving_count++;
                    }
                }
            }
            out->partial = false;
            followed = true;
        }
    }
    if (!followed)
    {
    // The search is the square grid route (GridRoute): a state is a tile and the way the car went to get there. Tiles cost what
    // the maps above say; a moving car costs on the tiles it will be over when we get there
    PlanCtx cx_;

    cx_.rx = w.route_x;
    cx_.ry = w.route_y;
    cx_.rn = w.route_n;
    cx_.sx = sx;
    cx_.sy = sy;
    cx_.lx = dxg;
    cx_.ly = dyg;
    cx_.ll = sqrtf(dxg * dxg + dyg * dyg);
    cx_.w = &w;
    cx_.x0 = x0;
    cx_.y0 = y0;
    cx_.speed = (w.own_speed > 0.02f && w.own_speed < 1.0f) ? w.own_speed : 0.1f;
    GridRouteWorld gw;
    gw.x0 = x0;
    gw.y0 = y0;
    gw.w = kW;
    gw.h = kH;
    gw.step = PlanStep;
    gw.user = &cx_;
    GridRouteParams gp;
    GridRoute_DefaultParams(&gp);
    gp.start_dir = GridRoute_DirOfHeading(heading_deg);
    gp.max_expand = 24000;
    gp.min_partial_gain = kMinPartialGain;
    static GridRouteOut gr;
    if (!GridRoute_Find(gw, x0 + start_cx, y0 + start_cy, x0 + goal_cx, y0 + goal_cy, gp, &gr))
    {
        return false;
    }
    out->partial = gr.partial;
    for (int i = 0; i < gr.count && out->tile_count < 160; i++)
    {
        out->tile_x[out->tile_count] = gr.x[i];
        out->tile_y[out->tile_count] = gr.y[i];
        out->tile_count++;
    }
    for (int cy = 0; cy < kH && out->moving_count < 96; cy++)
    {
        for (int cx = 0; cx < kW && out->moving_count < 96; cx++)
        {
            if (sOcc[cy * kW + cx] & 0x0F) // over it in the next 40 frames
            {
                out->moving_x[out->moving_count] = static_cast<short>(x0 + cx);
                out->moving_y[out->moving_count] = static_cast<short>(y0 + cy);
                out->moving_count++;
            }
        }
    }
    // The square route is what the car is to follow; it is pulled straight where the way is open with room (a path of 90 degree
    // steps would have the wheel hunting at each jog): from each kept point on to the furthest one the straight line to
    // reaches with room all along
    px[pn] = sx;
    py[pn] = sy;
    pn++;
    for (int i = 1; i < gr.count && pn < kLocalPlanMax * 4; i++)
    {
        px[pn] = static_cast<float>(gr.x[i]) + 0.5f;
        py[pn] = static_cast<float>(gr.y[i]) + 0.5f;
        pn++;
    }
    }
    n = 0;
    out->x[n] = px[0];
    out->y[n] = py[0];
    n++;
    int cur = 0;
    while (cur < pn - 1 && n < kLocalPlanMax)
    {
        int best = cur + 1;
        for (int j = pn - 1; j > cur + 1; j--)
        {
            float raw_dev = 0.0f;
            if (w.route_n >= 2)
            {
                for (int k = cur + 1; k < j; k++)
                {
                    const float dk = DistToRoute(w, px[k], py[k]);
                    raw_dev = dk > raw_dev ? dk : raw_dev;
                }
            }
            if (LineOpen(w, x0, y0, px[cur], py[cur], px[j], py[j], w.own_hw + 0.1f, raw_dev))
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
    // A right-angle turn between two legs is taken across the corner, not round the corner point: from half a tile before it to half a
    // tile after it. The car's nose swings wide in a turn, and the point itself is half a tile from whatever is straight on past it
    // (a building where the way turns, the top of a ramp): the chord keeps the nose off it, for every pair of directions
    {
        float cx_[kLocalPlanMax];
        float cy_[kLocalPlanMax];
        int cn = 0;
        cx_[cn] = out->x[0];
        cy_[cn] = out->y[0];
        cn++;
        for (int i = 1; i < n; i++)
        {
            bool chord = false;
            if (i + 1 < n && cn + 2 < kLocalPlanMax - 1)
            {
                const float ax_ = out->x[i] - out->x[i - 1];
                const float ay_ = out->y[i] - out->y[i - 1];
                const float bx_ = out->x[i + 1] - out->x[i];
                const float by_ = out->y[i + 1] - out->y[i];
                const float la = sqrtf(ax_ * ax_ + ay_ * ay_);
                const float lb = sqrtf(bx_ * bx_ + by_ * by_);
                if (la >= 1.0f && lb >= 1.0f && fabsf(ax_ * bx_ + ay_ * by_) < 0.2f * la * lb)
                {
                    // the leg directions as unit steps (a in, b out)
                    const float ux = ax_ / la;
                    const float uy = ay_ / la;
                    const float vx = bx_ / lb;
                    const float vy = by_ / lb;
                    const int tcx = static_cast<int>(out->x[i]);
                    const int tcy = static_cast<int>(out->y[i]);
                    const int sux = ux > 0.5f ? 1 : (ux < -0.5f ? -1 : 0);
                    const int suy = uy > 0.5f ? 1 : (uy < -0.5f ? -1 : 0);
                    const int svx = vx > 0.5f ? 1 : (vx < -0.5f ? -1 : 0);
                    const int svy = vy > 0.5f ? 1 : (vy < -0.5f ? -1 : 0);
                    // solid straight on past the corner (the way turns into a building's face): the nose of a turn must not swing into
                    // it. The inside of the turn (the diagonal tile on the side it turns to) may be solid too, a building corner the
                    // chord must not clip, so then the cut is shallower: through a point between the corner and the chord's middle
                    const bool solid_ahead = TileClosed(x0, y0, tcx + sux, tcy + suy);
                    const bool solid_inside = TileClosed(x0, y0, tcx + svx - sux, tcy + svy - suy);
                    if (solid_ahead)
                    {
                        const float depth = solid_inside ? 0.1f : 0.25f; // of the corner's step, towards the inside
                        const float c1x = out->x[i] - ux * 0.5f;
                        const float c1y = out->y[i] - uy * 0.5f;
                        const float mx = out->x[i] + (vx - ux) * depth;
                        const float my = out->y[i] + (vy - uy) * depth;
                        const float c2x = out->x[i] + vx * 0.5f;
                        const float c2y = out->y[i] + vy * 0.5f;
                        if (cn + 3 < kLocalPlanMax - 1 && ChordOpen(w, x0, y0, c1x, c1y, c2x, c2y))
                        {
                            cx_[cn] = c1x;
                            cy_[cn] = c1y;
                            cn++;
                            if (solid_inside)
                            {
                                cx_[cn] = mx;
                                cy_[cn] = my;
                                cn++;
                            }
                            cx_[cn] = c2x;
                            cy_[cn] = c2y;
                            cn++;
                            chord = true;
                        }
                    }
                }
            }
            if (!chord && cn < kLocalPlanMax)
            {
                cx_[cn] = out->x[i];
                cy_[cn] = out->y[i];
                cn++;
            }
        }
        for (int i = 0; i < cn; i++)
        {
            out->x[i] = cx_[i];
            out->y[i] = cy_[i];
        }
        n = cn;
    }
    out->fine_pass = 0;
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

void LocalPlan_RouteAhead(const float* wx, const float* wy, int n, int next, float x, float y, float dist, float* route_x, float* route_y, int* route_n)
{
    int rn = 0;
    float px = x;
    float py = y;
    if (next > 0 && next < n)
    {
        // the car's place on its leg, from the corner behind to the one ahead
        const float ex = wx[next] - wx[next - 1];
        const float ey = wy[next] - wy[next - 1];
        const float el2 = ex * ex + ey * ey;
        float t = el2 > 0.0001f ? ((x - wx[next - 1]) * ex + (y - wy[next - 1]) * ey) / el2 : 0.0f;
        t = t < 0.0f ? 0.0f : (t > 1.0f ? 1.0f : t);
        px = wx[next - 1] + ex * t;
        py = wy[next - 1] + ey * t;
    }
    route_x[rn] = px;
    route_y[rn] = py;
    rn++;
    float rest = dist;
    for (int i = next < 0 ? 0 : next; i < n && rn < kLocalRouteMax; i++)
    {
        const float ex = wx[i] - px;
        const float ey = wy[i] - py;
        const float el = sqrtf(ex * ex + ey * ey);
        if (el >= rest && el > 0.001f)
        {
            route_x[rn] = px + ex / el * rest;
            route_y[rn] = py + ey / el * rest;
            rn++;
            *route_n = rn;
            return;
        }
        rest -= el;
        px = wx[i];
        py = wy[i];
        route_x[rn] = px;
        route_y[rn] = py;
        rn++;
    }
    *route_n = rn;
}

int LocalPlan_RouteProgress(const float* wx, const float* wy, int n, int next, float x, float y, float max_off)
{
    int best = next;
    for (int k = next + 1; k <= next + 4 && k < n; k++)
    {
        const float ex = wx[k] - wx[k - 1];
        const float ey = wy[k] - wy[k - 1];
        const float el2 = ex * ex + ey * ey;
        if (el2 < 0.0001f)
        {
            continue;
        }
        const float t = ((x - wx[k - 1]) * ex + (y - wy[k - 1]) * ey) / el2;
        if (t < 0.0f || t > 1.0f)
        {
            continue; // not alongside this leg
        }
        const float qx = wx[k - 1] + ex * t - x;
        const float qy = wy[k - 1] + ey * t - y;
        if (qx * qx + qy * qy <= max_off * max_off)
        {
            best = k;
        }
    }
    return best;
}

float LocalPlan_RampLook(const LocalPlanOut& p, float x, float y, float look, int (*ramp_zone)(void* user, float x, float y), void* user)
{
    if (!p.found || p.count < 2 || !ramp_zone)
    {
        return look;
    }
    float bx = 0.0f;
    float by = 0.0f;
    // the car already in the approach or on the ramp? Then only the end of the ramp limits the aim
    LocalPlan_AimPoint(p, x, y, 0.0f, &bx, &by);
    const bool in_zone = ramp_zone(user, x, y) != 0;
    bool seen_ramp = false;
    for (float d = 0.25f; d <= look; d += 0.25f)
    {
        LocalPlan_AimPoint(p, x, y, d, &bx, &by);
        const int z = ramp_zone(user, bx, by);
        if (!in_zone && z != 0 && !seen_ramp)
        {
            // the path comes to a ramp's approach: aim at where it does (on the middle line of the ramp), not past it
            return d;
        }
        if (z & 4)
        {
            seen_ramp = true;
        }
        else if (seen_ramp)
        {
            return d + 0.3f < look ? d + 0.3f : look;
        }
    }
    return look;
}

bool LocalPlan_CarInWayAt(const SteerCar& car, float px, float py, float frames, float own_hw)
{
    float f = frames < 0.0f ? 0.0f : (frames > 90.0f ? 90.0f : frames);
    SteerBox b = car.box;
    b.cx = car.x + car.vx * f;
    b.cy = car.y + car.vy * f;
    return SteerBox_Contains(b, px, py, own_hw + 0.1f);
}

void LocalPlan_Fine(const LocalPlanWorld& w, const LocalPlanOut& tile, float x, float y, LocalPlanOut* out)
{
    // the tile route from the car on: the car's place on it, then the nodes after
    int seg = 0;
    float best = 1e9f;
    float qx = x;
    float qy = y;
    for (int i = 0; i + 1 < tile.count; i++)
    {
        const float ex = tile.x[i + 1] - tile.x[i];
        const float ey = tile.y[i + 1] - tile.y[i];
        const float el2 = ex * ex + ey * ey;
        float t = el2 > 0.0001f ? ((x - tile.x[i]) * ex + (y - tile.y[i]) * ey) / el2 : 0.0f;
        t = t < 0.0f ? 0.0f : (t > 1.0f ? 1.0f : t);
        const float cx = tile.x[i] + ex * t - x;
        const float cy = tile.y[i] + ey * t - y;
        if (cx * cx + cy * cy < best)
        {
            best = cx * cx + cy * cy;
            seg = i;
        }
    }
    *out = tile;
    out->fine_pass = 0;
    if (!tile.found || tile.count < 2 || w.car_count <= 0)
    {
        return;
    }
    float px[kLocalPlanMax];
    float py[kLocalPlanMax];
    int pn = 0;
    px[pn] = x;
    py[pn] = y;
    pn++;
    for (int i = seg + 1; i < tile.count && pn < kLocalPlanMax; i++)
    {
        px[pn] = tile.x[i];
        py[pn] = tile.y[i];
        pn++;
    }
    (void)qx;
    (void)qy;
    FinePlanWorld fw;
    fw.solid = w.solid;
    fw.user = w.user;
    fw.cars = w.cars;
    fw.car_count = w.car_count;
    fw.ped_count = w.ped_count;
    fw.own_hl = w.own_hl;
    fw.own_hw = w.own_hw;
    fw.own_speed = w.own_speed;
    fw.ramp_zone = w.ramp_zone;
    fw.edge_ok = w.edge_ok;
    fw.prev_x = w.prev_x;
    fw.prev_y = w.prev_y;
    fw.prev_n = w.prev_n;
    float fx[kLocalPlanMax];
    float fy[kLocalPlanMax];
    int fn = 0;
    const int pass = FinePlan_Refine(fw, px, py, pn, x, y, fx, fy, &fn, kLocalPlanMax);
    if (pass >= 0 && fn >= 2)
    {
        for (int i = 0; i < fn; i++)
        {
            out->x[i] = fx[i];
            out->y[i] = fy[i];
        }
        out->count = fn;
        out->length = 0.0f;
        for (int i = 1; i < fn; i++)
        {
            out->length += sqrtf((fx[i] - fx[i - 1]) * (fx[i] - fx[i - 1]) + (fy[i] - fy[i - 1]) * (fy[i] - fy[i - 1]));
        }
        out->fine_pass = pass + 1;
    }
    else
    {
        // nothing in the way (or no way round it): the tile route from the car on
        for (int i = 0; i < pn; i++)
        {
            out->x[i] = px[i];
            out->y[i] = py[i];
        }
        out->count = pn;
    }
}
