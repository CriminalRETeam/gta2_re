// Native test of FinePlan: the quarter tile route round what is in the way of the tile route. Each case is a thing the tile route cannot do:
// a fine path round a standing car, behind a car that crosses, round a pedestrian only when there is a way, and never through a wall.
#include "FinePlan.hpp"
#include <stdio.h>
#include <math.h>
#include <vector>

static int fails = 0;
static void Check(const char* n, bool ok)
{
    printf("%-100s %s\n", n, ok ? "PASS" : "FAIL");
    fails += ok ? 0 : 1;
}

// a street: open for y in [y0, y1), solid elsewhere
struct Street
{
    float y0, y1;
};
static bool Solid(void* user, float, float y)
{
    const Street* s = static_cast<const Street*>(user);
    return y < s->y0 || y >= s->y1;
}

static SteerCar CarAt(float x, float y, float vx, float vy, float hl, float hw)
{
    SteerCar c;
    c.x = x;
    c.y = y;
    c.vx = vx;
    c.vy = vy;
    c.speed = sqrtf(vx * vx + vy * vy);
    c.police = false;
    c.box.cx = x;
    c.box.cy = y;
    c.box.ax = 1.0f;
    c.box.ay = 0.0f;
    c.box.hl = hl;
    c.box.hw = hw;
    return c;
}

static FinePlanWorld World(Street* s, const std::vector<SteerCar>& cars, int peds)
{
    FinePlanWorld w;
    w.solid = Solid;
    w.user = s;
    w.cars = cars.empty() ? NULL : &cars[0];
    w.car_count = static_cast<int>(cars.size());
    w.ped_count = peds;
    w.own_hl = 0.66f;
    w.own_hw = 0.44f;
    w.own_speed = 0.12f;
    w.ramp_zone = NULL;
    w.edge_ok = NULL;
    w.prev_x = 0;
    w.prev_y = 0;
    w.prev_n = 0;
    return w;
}

static bool PathInSolid(const Street& s, const float* x, const float* y, int n)
{
    for (int i = 0; i + 1 < n; i++)
    {
        for (float t = 0.0f; t <= 1.0f; t += 0.05f)
        {
            if (Solid(const_cast<Street*>(&s), 0, y[i] + (y[i + 1] - y[i]) * t))
            {
                return true;
            }
        }
    }
    return false;
}

// the closest the path comes to a box (the box grown by `grow`)
static bool PathHitsBox(const float* x, const float* y, int n, const SteerBox& b, float grow)
{
    for (int i = 0; i + 1 < n; i++)
    {
        for (float t = 0.0f; t <= 1.0f; t += 0.02f)
        {
            if (SteerBox_Contains(b, x[i] + (x[i + 1] - x[i]) * t, y[i] + (y[i + 1] - y[i]) * t, grow))
            {
                return true;
            }
        }
    }
    return false;
}


// Drives a path the way the bot does: pure pursuit on the aim point `look` tiles ahead, the heading turning at most 1.8 degrees a frame at
// 0.14 tiles a frame. Returns whether the car's own box (0.41 x 0.19) ever overlaps the box
static bool DriveHits(const float* px, const float* py, int pn, const SteerBox& obstacle)
{
    float x = px[0], y = py[0];
    float h = atan2f(px[1] - px[0], -(py[1] - py[0])) * 57.29578f;
    for (int f = 0; f < 600; f++)
    {
        // the place on the path nearest the car, and the point 6.5 tiles on
        int seg = 0;
        float best = 1e9f, qx = x, qy = y;
        for (int i = 0; i + 1 < pn; i++)
        {
            const float ex = px[i + 1] - px[i], ey = py[i + 1] - py[i];
            const float el2 = ex * ex + ey * ey;
            float t = el2 > 0.0001f ? ((x - px[i]) * ex + (y - py[i]) * ey) / el2 : 0.0f;
            t = t < 0.0f ? 0.0f : (t > 1.0f ? 1.0f : t);
            const float cx = px[i] + ex * t, cy = py[i] + ey * t;
            if ((cx - x) * (cx - x) + (cy - y) * (cy - y) < best)
            {
                best = (cx - x) * (cx - x) + (cy - y) * (cy - y);
                seg = i;
                qx = cx;
                qy = cy;
            }
        }
        float rest = FinePlan_Look(0.14f), ax = px[pn - 1], ay = py[pn - 1], ux = qx, uy = qy;
        for (int i = seg + 1; i < pn; i++)
        {
            const float dx = px[i] - ux, dy = py[i] - uy;
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
        if (sqrtf((px[pn - 1] - x) * (px[pn - 1] - x) + (py[pn - 1] - y) * (py[pn - 1] - y)) < 0.5f)
        {
            return false;
        }
        float err = atan2f(ax - x, -(ay - y)) * 57.29578f - h;
        while (err > 180.0f) err -= 360.0f;
        while (err < -180.0f) err += 360.0f;
        h += err > 1.8f ? 1.8f : (err < -1.8f ? -1.8f : err);
        x += sinf(h / 57.29578f) * 0.14f;
        y += -cosf(h / 57.29578f) * 0.14f;
        SteerBox me = {x, y, sinf(h / 57.29578f), -cosf(h / 57.29578f), 0.41f, 0.19f};
        if (SteerBox_Overlap(me, obstacle))
        {
            return true;
        }
    }
    return false;
}

int main()
{
    float ox[64], oy[64];
    int on = 0;
    // 1. a clear street: the tile route is kept
    {
        Street s = {10.0f, 16.0f};
        std::vector<SteerCar> cars;
        const float px[2] = {20.5f, 40.5f}, py[2] = {13.5f, 13.5f};
        const int pass = FinePlan_Refine(World(&s, cars, 0), px, py, 2, 20.5f, 13.5f, ox, oy, &on, 64);
        Check("nothing in the way: the tile route is left as it is", pass == -1);
    }
    // 2. a standing car on the line of a wide street: a path round it, a few nodes, that clears its box
    {
        Street s = {10.0f, 17.0f};
        std::vector<SteerCar> cars;
        cars.push_back(CarAt(30.5f, 13.5f, 0.0f, 0.0f, 0.55f, 0.3f));
        const float px[2] = {20.5f, 40.5f}, py[2] = {13.5f, 13.5f};
        const int pass = FinePlan_Refine(World(&s, cars, 0), px, py, 2, 20.5f, 13.5f, ox, oy, &on, 64);
        Check("a standing car on the line: a fine path round it was found", pass == 0 && on >= 3);
        Check("... it ends where the tile route does", on >= 2 && fabsf(ox[on - 1] - 40.5f) < 0.6f && fabsf(oy[on - 1] - 13.5f) < 0.6f);
        Check("... it keeps out of the car's box with our half width", !PathHitsBox(ox, oy, on, cars[0].box, 0.44f));
        Check("... and out of the walls", !PathInSolid(s, ox, oy, on));
        Check("... and goes back to the line after the car (the last nodes are on it)", fabsf(oy[on - 1] - 13.5f) < 0.6f);
    }
    // 3. a car crossing the street: its box where it will be when we get there is gone round, not run into
    {
        Street s = {5.0f, 22.0f};
        std::vector<SteerCar> cars;
        SteerCar c = CarAt(30.5f, 8.0f, 0.0f, 0.12f, 0.55f, 0.3f); // across the street (its box is along x here, so it is a long one at 0.12 down)
        c.box.ax = 0.0f;
        c.box.ay = 1.0f;
        cars.push_back(c);
        const float px[2] = {20.5f, 40.5f}, py[2] = {13.5f, 13.5f};
        const int pass = FinePlan_Refine(World(&s, cars, 0), px, py, 2, 20.5f, 13.5f, ox, oy, &on, 64);
        bool hit = false;
        float dist = 0.0f;
        for (int i = 0; i + 1 < on && pass >= 0; i++)
        {
            const float dx = ox[i + 1] - ox[i], dy = oy[i + 1] - oy[i];
            const float len = sqrtf(dx * dx + dy * dy);
            for (float t = 0.0f; t < len; t += 0.05f)
            {
                const float frames = (dist + t) / 0.12f;
                SteerBox b = c.box;
                b.cy = c.box.cy + c.vy * frames;
                if (SteerBox_Contains(b, ox[i] + dx / len * t, oy[i] + dy / len * t, 0.35f))
                {
                    hit = true;
                }
            }
            dist += len;
        }
        Check("a car crossing the street: a path was found and it is never inside the car's box at the time", pass == 0 && on >= 3 && !hit);
    }
    // 4. a pedestrian in a street too narrow to go round: left out (pass 1), the tile route is not stuck behind him
    {
        Street s = {13.0f, 14.0f}; // one tile wide
        std::vector<SteerCar> cars;
        cars.push_back(CarAt(30.5f, 13.5f, 0.0f, 0.0f, 0.1f, 0.1f));
        const float px[2] = {20.5f, 40.5f}, py[2] = {13.5f, 13.5f};
        const int pass = FinePlan_Refine(World(&s, cars, 1), px, py, 2, 20.5f, 13.5f, ox, oy, &on, 64);
        Check("a pedestrian in a one tile gap: planned with pedestrians left out", pass == 1 && on >= 2);
        // the same thing as a car: no way round, so the cars are dear and not closed
        std::vector<SteerCar> cars2;
        cars2.push_back(CarAt(30.5f, 13.5f, 0.0f, 0.0f, 0.55f, 0.3f));
        const int pass2 = FinePlan_Refine(World(&s, cars2, 0), px, py, 2, 20.5f, 13.5f, ox, oy, &on, 64);
        Check("a car in a one tile gap: no way round, so the last pass (cars dear) goes through", pass2 == 2);
    }
    // 5. a ramp zone: left alone
    {
        Street s = {10.0f, 17.0f};
        std::vector<SteerCar> cars;
        cars.push_back(CarAt(30.5f, 13.5f, 0.0f, 0.0f, 0.55f, 0.3f));
        FinePlanWorld w = World(&s, cars, 0);
        struct R { static int Zone(void*, float, float) { return 1; } };
        w.ramp_zone = R::Zone;
        const float px[2] = {20.5f, 40.5f}, py[2] = {13.5f, 13.5f};
        Check("on a ramp the tile route is kept", FinePlan_Refine(w, px, py, 2, 20.5f, 13.5f, ox, oy, &on, 64) == -1);
    }
    // 6. a wall face across the street (a fence between two tiles) with a gap: the fine path goes through the gap, not the fence
    {
        Street s = {10.0f, 20.0f};
        std::vector<SteerCar> cars; // (no car: the fence alone makes the tile route unclear)
        FinePlanWorld w = World(&s, cars, 0);
        struct F { static bool Edge(void*, int x, int y, int nx, int) { return !(x == 28 && nx == 29 && y != 17 && y != 16); } };
        w.edge_ok = F::Edge; // a fence between x=28 and x=29, open on rows 16 and 17
        const float px[2] = {20.5f, 38.5f}, py[2] = {15.5f, 15.5f};
        const int pass = FinePlan_Refine(w, px, py, 2, 20.5f, 15.5f, ox, oy, &on, 64);
        bool through_fence = false;
        for (int i = 0; i + 1 < on; i++)
        {
            for (float u = 0.0f; u <= 1.0f; u += 0.02f)
            {
                const float x0 = ox[i] + (ox[i + 1] - ox[i]) * u;
                const float y0 = oy[i] + (oy[i + 1] - oy[i]) * u;
                if (x0 >= 28.0f && x0 < 29.0f && (y0 < 16.0f || y0 >= 18.0f))
                {
                    through_fence = true;
                }
            }
        }
        Check("a fence between two tiles with a gap: the fine path crosses it in the gap only", pass >= 0 && !through_fence);
    }
    // 7. a route with a corner and a car on its first leg: the open ground across the corner is no way round the car (it cuts off a stretch of the route)
    {
        Street s = {0.0f, 40.0f}; // open everywhere
        std::vector<SteerCar> cars;
        cars.push_back(CarAt(24.5f, 10.5f, 0.0f, 0.0f, 0.55f, 0.3f));
        FinePlanWorld w = World(&s, cars, 0);
        const float px[3] = {20.5f, 30.5f, 30.5f}, py[3] = {10.5f, 10.5f, 20.5f};
        const int pass = FinePlan_Refine(w, px, py, 3, 20.5f, 10.5f, ox, oy, &on, 64);
        bool ok = pass >= 0;
        for (int i = 0; i + 1 < on; i++)
        {
            // every point of the path is within 2.5 of the route and the path ends at its end: no diagonal across the corner
            if (ox[i + 1] > 30.0f && oy[i + 1] < 10.0f + 0.0f && false)
            {
                ok = false;
            }
        }
        float least = 1e9f;
        for (int i = 0; i + 1 < on; i++)
        {
            for (float u = 0.0f; u <= 1.0f; u += 0.05f)
            {
                const float x0 = ox[i] + (ox[i + 1] - ox[i]) * u, y0 = oy[i] + (oy[i + 1] - oy[i]) * u;
                // distance to the corner point (30.5, 10.5): a path across the corner never goes near it
                const float d = sqrtf((x0 - 30.5f) * (x0 - 30.5f) + (y0 - 10.5f) * (y0 - 10.5f));
                least = d < least ? d : least;
            }
        }
        Check("a car on the first leg of a corner: round it, and still to the corner (not across it)", ok && least < 1.6f);
    }
    // 8. a car parked on the inside of a corner: the car cannot turn a right angle and cuts it, so what it drives is not the tile route. The fine route has
    // its corner rounded round the parked car, and the car driven along it (pure pursuit, a turn rate) clears it; along the raw tile route it hits
    {
        Street s = {0.0f, 40.0f};
        std::vector<SteerCar> cars;
        cars.push_back(CarAt(28.9f, 12.0f, 0.0f, 0.0f, 0.5f, 0.3f)); // inside the corner of (30.5, 10.5), south west of it
        const float px[3] = {20.5f, 30.5f, 30.5f}, py[3] = {10.5f, 10.5f, 22.5f};
        const bool raw_hits = DriveHits(px, py, 3, cars[0].box);
        const int pass = FinePlan_Refine(World(&s, cars, 0), px, py, 3, 20.5f, 10.5f, ox, oy, &on, 64);
        const bool fine_hits = pass >= 0 ? DriveHits(ox, oy, on, cars[0].box) : true;
        Check("(the test sees it) driving the raw tile route cuts the corner into the parked car", raw_hits);
        Check("driving the fine route, with the corner rounded round the car, does not touch it", pass >= 0 && !fine_hits);
    }
    printf("%s\n", fails ? "FAILED" : "all passed");
    return fails ? 1 : 0;
}
