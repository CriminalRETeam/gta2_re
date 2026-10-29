#include "FinePlan.hpp"
#include "GridRoute.hpp"
#include "BoxSweep.hpp"
#include <math.h>
#include <string.h>
#include "BotCompat.hpp"

namespace
{
const int kLocalCap = 160;         // points of a tile route held
const float kCell = 0.25f;
const int kMaxCells = 240;        // a window of up to 60 tiles
const float kSolidReach = 0.3f;   // a cell is solid when ground this near its centre is (the car is 0.4 wide, 0.8 long)
const float kHug = 0.6f;          // cost of a cell per tile it is off the tile route
const float kCarSoft = 6.0f;      // pass 2: the cost of a cell inside a car's box
const float kStick = 0.5f;        // cost of a cell per tile it is off the route of the frame before
const float kCorridor = 2.5f;     // cells further than this from the tile route are not used: the fine route is a detour round a thing, not a new route
const float kLead = 0.6f;         // a car's box is this much longer, fore and aft, for the plan: the sidestep round it has to start before it, not at it
const float kMaxFrames = 40.0f;   // how far ahead a moving car is taken to carry on

unsigned char sSolid[kMaxCells * kMaxCells]; // 0 not known, 1 open, 2 solid

struct Ctx
{
    const FinePlanWorld* w;
    int x0, y0;  // the tile at the window's corner
    int cw, ch;  // the window, cells
    const float* rx;
    const float* ry;
    int rn;
    const float* qx;
    const float* qy;
    int qn;
    bool peds;
    bool soft;
    float r;     // how far from another box's edge our centre must stay
    float extra; // more of it, for a plan that has to leave room for the car to cut a corner
    float speed;
};

float CellX(const Ctx& c, int cx)
{
    return static_cast<float>(c.x0) + (static_cast<float>(cx) + 0.5f) * kCell;
}

float CellY(const Ctx& c, int cy)
{
    return static_cast<float>(c.y0) + (static_cast<float>(cy) + 0.5f) * kCell;
}

bool SolidCell(const Ctx& c, int cx, int cy)
{
    unsigned char& s = sSolid[cy * kMaxCells + cx];
    if (s == 0)
    {
        const float x = CellX(c, cx);
        const float y = CellY(c, cy);
        const bool solid = c.w->solid(c.w->user, x, y) || c.w->solid(c.w->user, x + kSolidReach, y) || c.w->solid(c.w->user, x - kSolidReach, y) ||
                           c.w->solid(c.w->user, x, y + kSolidReach) || c.w->solid(c.w->user, x, y - kSolidReach);
        s = solid ? 2 : 1;
    }
    return s == 2;
}

// May the car go from the point (ax, ay) to the next (bx, by), a cell or less, as far as the walls between tiles are concerned?
bool EdgeOk(const Ctx& c, float ax, float ay, float bx, float by)
{
    if (!c.w->edge_ok)
    {
        return true;
    }
    const int tx = static_cast<int>(floorf(ax));
    const int ty = static_cast<int>(floorf(ay));
    const int nx = static_cast<int>(floorf(bx));
    const int ny = static_cast<int>(floorf(by));
    if (tx == nx && ty == ny)
    {
        return true;
    }
    if (tx != nx && ty != ny)
    {
        // across a corner: by either of the two tiles beside it
        return (c.w->edge_ok(c.w->user, tx, ty, nx, ty) && c.w->edge_ok(c.w->user, nx, ty, nx, ny)) ||
               (c.w->edge_ok(c.w->user, tx, ty, tx, ny) && c.w->edge_ok(c.w->user, tx, ny, nx, ny));
    }
    return c.w->edge_ok(c.w->user, tx, ty, nx, ny);
}

// Is a car (or a pedestrian, when they count) going to cover the point when we are there, `frames` on?
bool CarAt(const Ctx& c, float x, float y, float frames)
{
    const float t = frames < kMaxFrames ? frames : kMaxFrames;
    const int first_ped = c.w->car_count - c.w->ped_count;
    for (int i = 0; i < c.w->car_count; i++)
    {
        if (i >= first_ped && !c.peds)
        {
            continue;
        }
        const SteerCar& car = c.w->cars[i];
        SteerBox b = car.box;
        // the box at the time, swept over the next few frames of its velocity
        const float ox = b.cx + car.vx * t;
        const float oy = b.cy + car.vy * t;
        BoxSweep(ox, oy, b.ax, b.ay, b.hl, b.hw, car.vx, car.vy, kPredictFrames, &b.cx, &b.cy, &b.hl, &b.hw);
        b.hl += kLead;
        if (SteerBox_Contains(b, x, y, c.r + c.extra + (car.police ? 0.4f : 0.0f)))
        {
            return true;
        }
    }
    return false;
}

float DistToLine(const float* lx, const float* ly, int n, float x, float y)
{
    float best = 1e9f;
    for (int i = 0; i + 1 < n; i++)
    {
        const float ex = lx[i + 1] - lx[i];
        const float ey = ly[i + 1] - ly[i];
        const float el2 = ex * ex + ey * ey;
        float t = el2 > 0.0001f ? ((x - lx[i]) * ex + (y - ly[i]) * ey) / el2 : 0.0f;
        t = t < 0.0f ? 0.0f : (t > 1.0f ? 1.0f : t);
        const float qx = lx[i] + ex * t - x;
        const float qy = ly[i] + ey * t - y;
        const float d = qx * qx + qy * qy;
        best = d < best ? d : best;
    }
    return sqrtf(best);
}

float DistToRoute(const Ctx& c, float x, float y)
{
    return DistToLine(c.rx, c.ry, c.rn, x, y);
}

// How far along the tile route the point nearest to (x, y) is, tiles
float RoutePos(const Ctx& c, float x, float y)
{
    float best = 1e9f;
    float pos = 0.0f;
    float run = 0.0f;
    for (int i = 0; i + 1 < c.rn; i++)
    {
        const float ex = c.rx[i + 1] - c.rx[i];
        const float ey = c.ry[i + 1] - c.ry[i];
        const float el2 = ex * ex + ey * ey;
        const float el = sqrtf(el2);
        float t = el2 > 0.0001f ? ((x - c.rx[i]) * ex + (y - c.ry[i]) * ey) / el2 : 0.0f;
        t = t < 0.0f ? 0.0f : (t > 1.0f ? 1.0f : t);
        const float qx = c.rx[i] + ex * t - x;
        const float qy = c.ry[i] + ey * t - y;
        const float d = qx * qx + qy * qy;
        if (d < best)
        {
            best = d;
            pos = run + el * t;
        }
        run += el;
    }
    return pos;
}

// Does the path go round something and come back, as opposed to cutting off a stretch of the tile route (across a corner)? The place
// nearest on the tile route may not go back along it, and over the whole path it may not gain more than the path travels
// (a corner cut off gains more: the long way round on the route, the short way across)
bool FollowsRoute(const Ctx& c, const float* x, const float* y, int n)
{
    const float first = RoutePos(c, x[0], y[0]);
    float prev = first;
    float travelled = 0.0f;
    for (int i = 0; i + 1 < n; i++)
    {
        const float dx = x[i + 1] - x[i];
        const float dy = y[i + 1] - y[i];
        const float len = sqrtf(dx * dx + dy * dy);
        const int steps = static_cast<int>(len / 0.5f) + 1;
        for (int k = 1; k <= steps; k++)
        {
            const float f = static_cast<float>(k) / static_cast<float>(steps);
            const float pos = RoutePos(c, x[i] + dx * f, y[i] + dy * f);
            if (pos < prev - 1.0f)
            {
                return false;
            }
            prev = pos > prev ? pos : prev;
        }
        travelled += len;
    }
    return prev - first <= travelled * 1.1f + 1.5f;
}

float Frames(const Ctx& c, float tiles)
{
    return tiles / (c.speed > 0.06f ? c.speed : 0.06f);
}

// The step callback of the cell search
float Step(void* user, int x, int y, int dir, float g)
{
    const Ctx& c = *static_cast<const Ctx*>(user);
    static const int dx[4] = {0, 1, 0, -1};
    static const int dy[4] = {-1, 0, 1, 0};
    const int nx = x + dx[dir];
    const int ny = y + dy[dir];
    if (nx < 0 || ny < 0 || nx >= c.cw || ny >= c.ch || SolidCell(c, nx, ny))
    {
        return -1.0f;
    }
    const float px = CellX(c, nx);
    const float py = CellY(c, ny);
    if (!EdgeOk(c, CellX(c, x), CellY(c, y), px, py))
    {
        return -1.0f;
    }
    float extra = 0.0f;
    if (CarAt(c, px, py, Frames(c, g * kCell)))
    {
        if (!c.soft)
        {
            return -1.0f;
        }
        extra += kCarSoft;
    }
    const float off = DistToRoute(c, px, py);
    if (off > kCorridor)
    {
        return -1.0f;
    }
    extra += (off < 4.0f ? off : 4.0f) * kHug;
    if (c.qn >= 2)
    {
        const float stray = DistToLine(c.qx, c.qy, c.qn, px, py);
        extra += (stray < 3.0f ? stray : 3.0f) * kStick;
    }
    return extra;
}

// Is the straight line between two points open for the car, as the cells see it, the first `tiles0` tiles from the car?
bool LineOpen(const Ctx& c, float ax, float ay, float bx, float by, float tiles0)
{
    const float dx = bx - ax;
    const float dy = by - ay;
    const float len = sqrtf(dx * dx + dy * dy);
    const int n = static_cast<int>(len / 0.1f) + 1;
    float lx = ax;
    float ly = ay;
    for (int i = 0; i <= n; i++)
    {
        const float f = static_cast<float>(i) / static_cast<float>(n);
        const float x = ax + dx * f;
        const float y = ay + dy * f;
        if (!EdgeOk(c, lx, ly, x, y))
        {
            return false;
        }
        lx = x;
        ly = y;
        const int cx = static_cast<int>((x - static_cast<float>(c.x0)) / kCell);
        const int cy = static_cast<int>((y - static_cast<float>(c.y0)) / kCell);
        if (cx < 0 || cy < 0 || cx >= c.cw || cy >= c.ch || SolidCell(c, cx, cy))
        {
            return false;
        }
        if (!c.soft && CarAt(c, x, y, Frames(c, tiles0 + len * f)))
        {
            return false;
        }
    }
    return true;
}

// How far the straight line a-b strays from the cells' own path between its cells i0..i1 (the pull may not cut a corner of it)
float Stray(const float* cx, const float* cy, int i0, int i1, float ax, float ay, float bx, float by)
{
    float worst = 0.0f;
    const float dx = bx - ax;
    const float dy = by - ay;
    const int steps = static_cast<int>(sqrtf(dx * dx + dy * dy) / 0.2f) + 1;
    for (int s = 0; s <= steps; s++)
    {
        const float x = ax + dx * static_cast<float>(s) / static_cast<float>(steps);
        const float y = ay + dy * static_cast<float>(s) / static_cast<float>(steps);
        float best = 1e9f;
        for (int i = i0; i < i1; i++)
        {
            const float ex = cx[i + 1] - cx[i];
            const float ey = cy[i + 1] - cy[i];
            const float el2 = ex * ex + ey * ey;
            float t = el2 > 0.0001f ? ((x - cx[i]) * ex + (y - cy[i]) * ey) / el2 : 0.0f;
            t = t < 0.0f ? 0.0f : (t > 1.0f ? 1.0f : t);
            const float qx = cx[i] + ex * t - x;
            const float qy = cy[i] + ey * t - y;
            const float d = qx * qx + qy * qy;
            best = d < best ? d : best;
        }
        worst = best > worst ? best : worst;
    }
    return sqrtf(worst);
}

// Is the tile route itself clear of everything, as the cells see it?
bool RouteClear(const Ctx& c, const float* px, const float* py, int pn)
{
    float dist = 0.0f;
    for (int i = 0; i + 1 < pn; i++)
    {
        if (!LineOpen(c, px[i], py[i], px[i + 1], py[i + 1], dist))
        {
            return false;
        }
        const float dx = px[i + 1] - px[i];
        const float dy = py[i + 1] - py[i];
        dist += sqrtf(dx * dx + dy * dy);
    }
    return true;
}
} // namespace

namespace
{
// Drives a path the way the bot does (pure pursuit on the point 3 + 25 x speed tiles ahead, at most 8; the heading turning at most 1.8 degrees a
// frame) and says whether the car's centre ever gets within the margin of a wall or a car, at the time, and where
bool DriveHits(const Ctx& c, const float* px, const float* py, int pn, float* hit_x, float* hit_y)
{
    if (pn < 2)
    {
        return false;
    }
    const float speed = c.speed > 0.1f ? c.speed : 0.1f;
    float x = px[0];
    float y = py[0];
    float h = atan2f(px[1] - px[0], -(py[1] - py[0])) * 57.29578f;
    const float look = FinePlan_Look(speed);
    for (int f = 0; f < 500; f++)
    {
        int seg = 0;
        float best = 1e9f;
        float qx = x;
        float qy = y;
        for (int i = 0; i + 1 < pn; i++)
        {
            const float ex = px[i + 1] - px[i];
            const float ey = py[i + 1] - py[i];
            const float el2 = ex * ex + ey * ey;
            float t = el2 > 0.0001f ? ((x - px[i]) * ex + (y - py[i]) * ey) / el2 : 0.0f;
            t = t < 0.0f ? 0.0f : (t > 1.0f ? 1.0f : t);
            const float cx = px[i] + ex * t;
            const float cy = py[i] + ey * t;
            if ((cx - x) * (cx - x) + (cy - y) * (cy - y) < best)
            {
                best = (cx - x) * (cx - x) + (cy - y) * (cy - y);
                seg = i;
                qx = cx;
                qy = cy;
            }
        }
        float rest = look;
        float ax = px[pn - 1];
        float ay = py[pn - 1];
        float ux = qx;
        float uy = qy;
        for (int i = seg + 1; i < pn; i++)
        {
            const float dx = px[i] - ux;
            const float dy = py[i] - uy;
            const float d = sqrtf(dx * dx + dy * dy);
            if (d >= rest && d > 0.001f)
            {
                ax = ux + dx / d * rest;
                ay = uy + dy / d * rest;
                break;
            }
            rest -= d;
            ux = px[i];
            uy = py[i];
        }
        if ((px[pn - 1] - x) * (px[pn - 1] - x) + (py[pn - 1] - y) * (py[pn - 1] - y) < 0.36f)
        {
            return false;
        }
        float err = atan2f(ax - x, -(ay - y)) * 57.29578f - h;
        while (err > 180.0f)
        {
            err -= 360.0f;
        }
        while (err < -180.0f)
        {
            err += 360.0f;
        }
        h += err > 1.8f ? 1.8f : (err < -1.8f ? -1.8f : err);
        x += sinf(h / 57.29578f) * speed;
        y += -cosf(h / 57.29578f) * speed;
        const int cx = static_cast<int>((x - static_cast<float>(c.x0)) / kCell);
        const int cy = static_cast<int>((y - static_cast<float>(c.y0)) / kCell);
        if (cx < 0 || cy < 0 || cx >= c.cw || cy >= c.ch)
        {
            continue; // out of the window: not looked at
        }
        if (SolidCell(c, cx, cy) || CarAt(c, x, y, static_cast<float>(f)))
        {
            *hit_x = x;
            *hit_y = y;
            return true;
        }
    }
    return false;
}
} // namespace

namespace
{
// The sharp corners of a path rounded off: each is cut across by up to max_cut tiles (twice over), where the cut stays open (walls, and the cars
// at the time), so the car, which cannot turn a right angle, drives what was planned and does not cut the corner on its own, into whatever is on its inside
int RoundCorners(const Ctx& c, float* ox, float* oy, int n, int max_out, float max_cut)
{
    for (int round = 0; round < 2; round++)
    {
        static float rx_[kGridRouteMax];
        static float ry_[kGridRouteMax];
        int rn2 = 0;
        rx_[rn2] = ox[0];
        ry_[rn2] = oy[0];
        rn2++;
        float done = 0.0f;
        for (int i = 1; i + 1 < n; i++)
        {
            const float ux = ox[i] - ox[i - 1];
            const float uy = oy[i] - oy[i - 1];
            const float vx = ox[i + 1] - ox[i];
            const float vy = oy[i + 1] - oy[i];
            const float ul = sqrtf(ux * ux + uy * uy);
            const float vl = sqrtf(vx * vx + vy * vy);
            bool cut = false;
            if (ul > 0.05f && vl > 0.05f && (ux * vx + uy * vy) / (ul * vl) < 0.8f && rn2 + 2 < max_out)
            {
                const float k = (ul * 0.45f < vl * 0.45f ? ul * 0.45f : vl * 0.45f);
                const float d = k < max_cut ? k : max_cut;
                const float c1x = ox[i] - ux / ul * d;
                const float c1y = oy[i] - uy / ul * d;
                const float c2x = ox[i] + vx / vl * d;
                const float c2y = oy[i] + vy / vl * d;
                if (LineOpen(c, c1x, c1y, ox[i], oy[i], done) && LineOpen(c, ox[i], oy[i], c2x, c2y, done) && LineOpen(c, c1x, c1y, c2x, c2y, done))
                {
                    rx_[rn2] = c1x;
                    ry_[rn2] = c1y;
                    rn2++;
                    rx_[rn2] = c2x;
                    ry_[rn2] = c2y;
                    rn2++;
                    cut = true;
                }
            }
            if (!cut)
            {
                rx_[rn2] = ox[i];
                ry_[rn2] = oy[i];
                rn2++;
            }
            done += ul;
        }
        rx_[rn2] = ox[n - 1];
        ry_[rn2] = oy[n - 1];
        rn2++;
        for (int i = 0; i < rn2 && i < max_out; i++)
        {
            ox[i] = rx_[i];
            oy[i] = ry_[i];
        }
        n = rn2 < max_out ? rn2 : max_out;
    }
    return n;
}
} // namespace

namespace
{
// The point `s` tiles along the polyline
void PointAlong(const float* x, const float* y, int n, float s, float* ox, float* oy)
{
    float run = 0.0f;
    for (int i = 0; i + 1 < n; i++)
    {
        const float dx = x[i + 1] - x[i];
        const float dy = y[i + 1] - y[i];
        const float len = sqrtf(dx * dx + dy * dy);
        if (s <= run + len || i + 2 == n)
        {
            const float f = len > 0.0001f ? (s - run) / len : 0.0f;
            const float g = f < 0.0f ? 0.0f : (f > 1.0f ? 1.0f : f);
            *ox = x[i] + dx * g;
            *oy = y[i] + dy * g;
            return;
        }
        run += len;
    }
    *ox = x[n - 1];
    *oy = y[n - 1];
}
} // namespace

int FinePlan_Refine(const FinePlanWorld& w, const float* px, const float* py, int pn, float sx, float sy, float* ox, float* oy, int* on, int max_out)
{
    if (pn < 2)
    {
        return -1;
    }
    for (int i = 0; i < pn; i++)
    {
        if (w.ramp_zone && w.ramp_zone(w.user, px[i], py[i]) != 0)
        {
            return -1; // a ramp is lined up by the tile route
        }
    }
    float total = 0.0f;
    for (int i = 0; i + 1 < pn; i++)
    {
        total += sqrtf((px[i + 1] - px[i]) * (px[i + 1] - px[i]) + (py[i + 1] - py[i]) * (py[i + 1] - py[i]));
    }
    // where the tile route is blocked, as the cells see it (walls, the cars' boxes at the time we get there): the first place, and the end of
    // the cluster of places (a gap of more than 4 tiles ends it)
    Ctx c;
    c.w = &w;
    c.rx = px;
    c.ry = py;
    c.rn = pn;
    c.qx = w.prev_x;
    c.qy = w.prev_y;
    c.qn = w.prev_n;
    c.peds = true;
    c.soft = false;
    c.r = 0.5f * (w.own_hl + w.own_hw); // a disc for a car that is longer than it is wide (own_hl and own_hw are already with the driver's margin)
    c.speed = w.own_speed;
    c.extra = 0.0f;
    // (the window for the probe: the whole route, three tiles round)
    float minx = sx, maxx = sx, miny = sy, maxy = sy;
    for (int i = 0; i < pn; i++)
    {
        minx = px[i] < minx ? px[i] : minx;
        maxx = px[i] > maxx ? px[i] : maxx;
        miny = py[i] < miny ? py[i] : miny;
        maxy = py[i] > maxy ? py[i] : maxy;
    }
    for (int i = 0; i < w.prev_n; i++)
    {
        minx = w.prev_x[i] < minx ? w.prev_x[i] : minx;
        maxx = w.prev_x[i] > maxx ? w.prev_x[i] : maxx;
        miny = w.prev_y[i] < miny ? w.prev_y[i] : miny;
        maxy = w.prev_y[i] > maxy ? w.prev_y[i] : maxy;
    }
    c.x0 = static_cast<int>(floorf(minx - 3.0f));
    c.y0 = static_cast<int>(floorf(miny - 3.0f));
    c.cw = static_cast<int>((maxx + 3.0f - static_cast<float>(c.x0)) / kCell) + 1;
    c.ch = static_cast<int>((maxy + 3.0f - static_cast<float>(c.y0)) / kCell) + 1;
    if (c.cw > kMaxCells || c.ch > kMaxCells || c.cw < 2 || c.ch < 2)
    {
        return -1;
    }
    memset(sSolid, 0, sizeof(sSolid));
    float s_first = -1.0f;
    float s_last = -1.0f;
    {
        float lx = px[0];
        float ly = py[0];
        for (float s = 0.0f; s <= total; s += 0.25f)
        {
            float x, y;
            PointAlong(px, py, pn, s, &x, &y);
            const int cx = static_cast<int>((x - static_cast<float>(c.x0)) / kCell);
            const int cy = static_cast<int>((y - static_cast<float>(c.y0)) / kCell);
            const bool blocked = cx < 0 || cy < 0 || cx >= c.cw || cy >= c.ch || SolidCell(c, cx, cy) || !EdgeOk(c, lx, ly, x, y) || CarAt(c, x, y, Frames(c, s));
            lx = x;
            ly = y;
            if (blocked)
            {
                if (s_first < 0.0f)
                {
                    s_first = s;
                }
                else if (s - s_last > 4.0f)
                {
                    break;
                }
                s_last = s;
            }
        }
    }
    if (s_first < 0.0f || s_first > 14.0f)
    {
        // nothing on the tile route, or not near yet. The car cannot turn a right angle: it cuts the corners of the route (by a tile or more) and drives
        // something else than the line it is given. So the route has its corners rounded where that is open, and what is driven (the car, with its
        // turn rate, along it) is looked at: when that meets a wall or a car the route is planned again round the place
        float rx[kLocalCap];
        float ry[kLocalCap];
        int n = 0;
        rx[n] = sx;
        ry[n] = sy;
        n++;
        for (int i = 1; i < pn && n < kLocalCap && n < max_out; i++)
        {
            rx[n] = px[i];
            ry[n] = py[i];
            n++;
        }
        const int n0 = n;
        if (n >= 3)
        {
            n = RoundCorners(c, rx, ry, n, kLocalCap < max_out ? kLocalCap : max_out, 1.5f);
        }
        float hx = 0.0f;
        float hy = 0.0f;
        if (!DriveHits(c, rx, ry, n, &hx, &hy))
        {
            if (n == n0)
            {
                return -1; // the tile route as it is: nothing is met, no corner was rounded
            }
            for (int i = 0; i < n; i++)
            {
                ox[i] = rx[i];
                oy[i] = ry[i];
            }
            *on = n;
            return 0;
        }
        // met at (hx, hy): plan round that place of the route
        const float at = RoutePos(c, hx, hy);
        s_first = at > 1.0f ? at - 1.0f : 0.0f;
        s_last = at + 1.0f;
    }
    // the route of the frame before, while it is still clear (of everything, as it is now) and still ends on the tile route, is kept, trimmed to the
    // car: planned afresh every frame the way round a car is any of several equal ones and the wheel follows the choice back and forth
    if (w.prev_n >= 2)
    {
        float vx[kLocalCap];
        float vy[kLocalCap];
        int vn = 0;
        int seg = 0;
        float best = 1e9f;
        for (int i = 0; i + 1 < w.prev_n; i++)
        {
            const float ex = w.prev_x[i + 1] - w.prev_x[i];
            const float ey = w.prev_y[i + 1] - w.prev_y[i];
            const float el2 = ex * ex + ey * ey;
            float t = el2 > 0.0001f ? ((sx - w.prev_x[i]) * ex + (sy - w.prev_y[i]) * ey) / el2 : 0.0f;
            t = t < 0.0f ? 0.0f : (t > 1.0f ? 1.0f : t);
            const float qx = w.prev_x[i] + ex * t - sx;
            const float qy = w.prev_y[i] + ey * t - sy;
            if (qx * qx + qy * qy < best)
            {
                best = qx * qx + qy * qy;
                seg = i;
            }
        }
        vx[vn] = sx;
        vy[vn] = sy;
        vn++;
        for (int i = seg + 1; i < w.prev_n && vn < kLocalCap && vn < max_out; i++)
        {
            vx[vn] = w.prev_x[i];
            vy[vn] = w.prev_y[i];
            vn++;
        }
        if (best < 0.6f * 0.6f && vn >= 2 && DistToLine(px, py, pn, vx[vn - 1], vy[vn - 1]) < 0.6f)
        {
            Ctx k = c;
            k.peds = true;
            k.soft = false;
            k.rx = vx;
            k.ry = vy;
            k.rn = vn;
            float phx = 0.0f;
            float phy = 0.0f;
            if (RouteClear(k, vx, vy, vn) && !DriveHits(k, vx, vy, vn, &phx, &phy))
            {
                for (int i = 0; i < vn; i++)
                {
                    ox[i] = vx[i];
                    oy[i] = vy[i];
                }
                *on = vn;
                return 0;
            }
        }
    }
    // plan from the car to just past the last blocked place, and rejoin the tile route there
    const float s_goal = s_last + 3.0f < total ? s_last + 3.0f : total;
    float gx, gy;
    PointAlong(px, py, pn, s_goal, &gx, &gy);
    // the tile route up to the goal, for the corridor and the checks
    float tx_[kLocalCap];
    float ty_[kLocalCap];
    int tn = 0;
    {
        float run = 0.0f;
        tx_[tn] = px[0];
        ty_[tn] = py[0];
        tn++;
        for (int i = 1; i < pn && tn < kLocalCap - 1; i++)
        {
            const float len = sqrtf((px[i] - px[i - 1]) * (px[i] - px[i - 1]) + (py[i] - py[i - 1]) * (py[i] - py[i - 1]));
            if (run + len >= s_goal)
            {
                break;
            }
            run += len;
            tx_[tn] = px[i];
            ty_[tn] = py[i];
            tn++;
        }
        tx_[tn] = gx;
        ty_[tn] = gy;
        tn++;
    }
    c.rx = tx_;
    c.ry = ty_;
    c.rn = tn;
    // the window of the plan: the car and the stretch to the goal, three tiles round
    minx = sx; maxx = sx; miny = sy; maxy = sy;
    for (int i = 0; i < tn; i++)
    {
        minx = tx_[i] < minx ? tx_[i] : minx;
        maxx = tx_[i] > maxx ? tx_[i] : maxx;
        miny = ty_[i] < miny ? ty_[i] : miny;
        maxy = ty_[i] > maxy ? ty_[i] : maxy;
    }
    c.x0 = static_cast<int>(floorf(minx - 3.0f));
    c.y0 = static_cast<int>(floorf(miny - 3.0f));
    c.cw = static_cast<int>((maxx + 3.0f - static_cast<float>(c.x0)) / kCell) + 1;
    c.ch = static_cast<int>((maxy + 3.0f - static_cast<float>(c.y0)) / kCell) + 1;
    if (c.cw > kMaxCells || c.ch > kMaxCells || c.cw < 2 || c.ch < 2)
    {
        return -1;
    }
    memset(sSolid, 0, sizeof(sSolid));
    static const float kExtras[3] = {0.0f, 0.45f, 0.9f}; // more room left round cars, for a car that cuts the corners of the route
    for (int ei = 0; ei < 3; ei++)
    {
        c.extra = kExtras[ei];
        for (int pass = 0; pass < 3; pass++)
        {
            c.peds = pass == 0;
            c.soft = pass == 2;
            const int scx = static_cast<int>((sx - static_cast<float>(c.x0)) / kCell);
            const int scy = static_cast<int>((sy - static_cast<float>(c.y0)) / kCell);
            int gcx = static_cast<int>((gx - static_cast<float>(c.x0)) / kCell);
            int gcy = static_cast<int>((gy - static_cast<float>(c.y0)) / kCell);
            bool have_goal = false;
            for (int ring = 0; ring <= 8 && !have_goal; ring++)
            {
                for (int ddy = -ring; ddy <= ring && !have_goal; ddy++)
                {
                    for (int ddx = -ring; ddx <= ring && !have_goal; ddx++)
                    {
                        if (abs(ddx) != ring && abs(ddy) != ring)
                        {
                            continue;
                        }
                        const int cx = gcx + ddx;
                        const int cy = gcy + ddy;
                        if (cx >= 0 && cy >= 0 && cx < c.cw && cy < c.ch && !SolidCell(c, cx, cy) && (c.soft || !CarAt(c, CellX(c, cx), CellY(c, cy), Frames(c, s_goal))))
                        {
                            gcx = cx;
                            gcy = cy;
                            have_goal = true;
                        }
                    }
                }
            }
            if (!have_goal || scx < 0 || scy < 0 || scx >= c.cw || scy >= c.ch)
            {
                continue;
            }
            GridRouteWorld gw;
            gw.x0 = 0;
            gw.y0 = 0;
            gw.w = c.cw;
            gw.h = c.ch;
            gw.step = Step;
            gw.user = &c;
            GridRouteParams gp;
            GridRoute_DefaultParams(&gp);
            gp.turn_cost = 0.6f;
            gp.uturn_cost = 8.0f;
            gp.allow_partial = false;
            gp.max_expand = 30000;
            static GridRouteOut gr;
            if (!GridRoute_Find(gw, scx, scy, gcx, gcy, gp, &gr) || gr.count < 2)
            {
                continue;
            }
            static float cx_[kGridRouteMax];
            static float cy_[kGridRouteMax];
            int cn = 0;
            cx_[cn] = sx;
            cy_[cn] = sy;
            cn++;
            for (int i = 1; i < gr.count - 1; i++)
            {
                cx_[cn] = CellX(c, gr.x[i]);
                cy_[cn] = CellY(c, gr.y[i]);
                cn++;
            }
            cx_[cn] = gx; // (the goal itself, not its cell)
            cy_[cn] = gy;
            cn++;
            // the cells' path pulled straight where the line between two is open and does not cut a corner of it
            int n = 0;
            float travelled = 0.0f;
            ox[n] = sx;
            oy[n] = sy;
            n++;
            int at = 0;
            while (at < cn - 1 && n < max_out)
            {
                int far_i = at + 1;
                for (int j = cn - 1; j > at + 1; j--)
                {
                    if (Stray(cx_, cy_, at, j, cx_[at], cy_[at], cx_[j], cy_[j]) <= 0.5f && LineOpen(c, cx_[at], cy_[at], cx_[j], cy_[j], travelled))
                    {
                        far_i = j;
                        break;
                    }
                }
                const float dx = cx_[far_i] - cx_[at];
                const float dy = cy_[far_i] - cy_[at];
                travelled += sqrtf(dx * dx + dy * dy);
                ox[n] = cx_[far_i];
                oy[n] = cy_[far_i];
                n++;
                at = far_i;
            }
            n = RoundCorners(c, ox, oy, n, max_out, 0.8f);
            if (n >= 2 && FollowsRoute(c, ox, oy, n))
            {
                // and on from the goal along the tile route
                float run = 0.0f;
                for (int i = 1; i < pn && n < max_out; i++)
                {
                    run += sqrtf((px[i] - px[i - 1]) * (px[i] - px[i - 1]) + (py[i] - py[i - 1]) * (py[i] - py[i - 1]));
                    if (run > s_goal + 0.01f)
                    {
                        ox[n] = px[i];
                        oy[n] = py[i];
                        n++;
                    }
                }
                // and what is driven along it: when the car, cutting its corners, still meets something, more room is left round it
                float hx2 = 0.0f;
                float hy2 = 0.0f;
                const float used = c.extra;
                c.extra = 0.0f;
                const bool meets = DriveHits(c, ox, oy, n, &hx2, &hy2);
                c.extra = used;
                if (!meets || ei == 2)
                {
                    *on = n;
                    return pass;
                }
                break;
            }
        }
    }
    return -1;
}
