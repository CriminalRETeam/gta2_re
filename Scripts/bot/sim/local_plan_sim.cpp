// Native tests of the detailed route planner (Source/Bot/LocalPlan.cpp): an A* with a node per map tile that keeps to the
// line, goes round closed tiles and standing cars, and prefers the lane that goes the right way.
//   g++ -O1 -I../../../Source/Bot -o local_plan_sim local_plan_sim.cpp ../../../Source/Bot/LocalPlan.cpp ../../../Source/Bot/DriveSteer.cpp && ./local_plan_sim
#include "LocalPlan.hpp"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>

struct Grid
{
    int x0, y0, w, h;
    std::vector<char> cell;
    bool Solid(float x, float y) const
    {
        const int ix = static_cast<int>(floorf(x)) - x0;
        const int iy = static_cast<int>(floorf(y)) - y0;
        if (ix < 0 || iy < 0 || ix >= w || iy >= h)
        {
            return true;
        }
        const char c = cell[iy * w + ix];
        return c == 'B' || c == ' ';
    }
    char At(float x, float y) const
    {
        const int ix = static_cast<int>(floorf(x)) - x0;
        const int iy = static_cast<int>(floorf(y)) - y0;
        return (ix < 0 || iy < 0 || ix >= w || iy >= h) ? ' ' : cell[iy * w + ix];
    }
};

static bool SolidCb(void* user, float x, float y)
{
    return static_cast<const Grid*>(user)->Solid(x, y);
}

// '<' '>' '^' 'v' lanes: stepping against the arrow costs
static float LaneCost(void* user, float x, float y, int dx, int dy)
{
    const char c = static_cast<const Grid*>(user)->At(x, y);
    if ((c == '>' && dx < 0) || (c == '<' && dx > 0) || (c == '^' && dy > 0) || (c == 'v' && dy < 0))
    {
        return 4.0f;
    }
    return 0.0f;
}

static bool LoadGrid(const char* path, Grid* g)
{
    FILE* f = fopen(path, "r");
    if (!f)
    {
        return false;
    }
    char line[512];
    int x1 = 0, y1 = 0;
    if (!fgets(line, sizeof(line), f) || sscanf(strstr(line, "x ") + 2, "%d..%d, y %d..%d", &g->x0, &x1, &g->y0, &y1) != 4)
    {
        return false;
    }
    g->w = x1 - g->x0 + 1;
    g->h = y1 - g->y0 + 1;
    g->cell.assign(g->w * g->h, ' ');
    fgets(line, sizeof(line), f);
    fgets(line, sizeof(line), f);
    while (fgets(line, sizeof(line), f))
    {
        const int y = atoi(line);
        if (y < g->y0 || y > y1)
        {
            continue;
        }
        for (int x = 0; x < g->w && line[4 + x] && line[4 + x] != '\n'; x++)
        {
            g->cell[(y - g->y0) * g->w + x] = line[4 + x];
        }
    }
    fclose(f);
    return true;
}

static SteerCar MakeCar(float x, float y, float vx, float vy)
{
    SteerCar c;
    c.x = x;
    c.y = y;
    c.vx = vx;
    c.vy = vy;
    c.speed = sqrtf(vx * vx + vy * vy);
    c.police = false;
    const SteerBox b = {x, y, 1.0f, 0.0f, 0.56f, 0.33};
    c.box = b;
    return c;
}

static int fails = 0;
static void Check(const char* name, bool ok, const char* why = "")
{
    printf("%-62s %s %s\n", name, ok ? "PASS" : "FAIL", ok ? "" : why);
    fails += ok ? 0 : 1;
}

// an edge wall (a step up with no ramp, or a wall face) between columns 29 and 30 everywhere except at row 40, the ramp
static bool EdgeCb(void*, int x, int y, int nx, int ny)
{
    const bool across = (x == 29 && nx == 30) || (x == 30 && nx == 29);
    return !across || y == 40;
}

// a ramp up the alley: tiles (160,46) and (160,47) are the ramp (axis along y), (160,44), (160,45), (160,48), (160,49) lead to it
static int RampCb(void*, float x, float y)
{
    if (static_cast<int>(x) != 160)
    {
        return 0;
    }
    const int ty = static_cast<int>(y);
    if (ty == 46 || ty == 47)
    {
        return 2 | 4;
    }
    return (ty >= 44 && ty <= 49) ? 2 : 0;
}

static LocalPlanWorld World(const Grid& g, const std::vector<SteerCar>& cars, bool lanes)
{
    LocalPlanWorld w;
    w.edge_ok = NULL;
    w.ramp_zone = NULL;
    w.solid = SolidCb;
    w.step_cost = lanes ? LaneCost : NULL;
    w.user = const_cast<Grid*>(&g);
    w.cars = cars.empty() ? NULL : &cars[0];
    w.car_count = static_cast<int>(cars.size());
    w.ped_count = 0;
    w.prev_x = 0;
    w.prev_y = 0;
    w.prev_n = 0;
    w.own_hl = 0.81f;
    w.own_hw = 0.58f;
    w.own_speed = 0.12f;
    w.route_x = 0;
    w.route_y = 0;
    w.route_n = 0;
    return w;
}

static bool PathTouchesSolid(const Grid& g, const LocalPlanOut& p)
{
    for (int i = 1; i < p.count; i++)
    {
        if (g.Solid(p.x[i], p.y[i]))
        {
            return true;
        }
    }
    return false;
}

static float MaxOffLine(const LocalPlanOut& p, float sx, float sy, float gx, float gy)
{
    const float lx = gx - sx, ly = gy - sy;
    const float ll = sqrtf(lx * lx + ly * ly);
    float worst = 0.0f;
    for (int i = 0; i < p.count; i++)
    {
        const float off = fabsf((p.x[i] - sx) * ly - (p.y[i] - sy) * lx) / ll;
        worst = off > worst ? off : worst;
    }
    return worst;
}

static SteerCar MakeCar(float x, float y)
{
    SteerCar c;
    c.x = x; c.y = y; c.vx = 0; c.vy = 0; c.speed = 0; c.police = false;
    c.box.cx = x; c.box.cy = y; c.box.ax = 0; c.box.ay = -1; c.box.hl = 0.5f; c.box.hw = 0.3f;
    return c;
}

static void WayFreeTests()
{
    SteerCar cars[2] = {MakeCar(73.0f, 49.5f), MakeCar(73.3f, 50.3f)};
    // a swerve from (74.8, 50.6) to a point beside the road: through the cab (car 0) at (73, 49.5) or clear of it
    Check("our own car is found by id", DriveSteer_IsOwnCar(65, 65, true));
    Check("a car 0.5 tiles ahead with another id is not ours", !DriveSteer_IsOwnCar(123, 65, true));
    Check("on foot no car is ours", !DriveSteer_IsOwnCar(65, 65, false));
    Check("a way straight through a car is not free", !DriveSteer_WayFreeOfCars(cars, 2, 76.0f, 52.0f, 70.0f, 47.0f, 0.4f, -1));
    Check("a way beside the cars, a car's width off, is free", DriveSteer_WayFreeOfCars(cars, 2, 76.0f, 52.0f, 76.0f, 46.0f, 0.4f, -1));
    Check("the car being dodged is left out", DriveSteer_WayFreeOfCars(cars, 2, 76.0f, 52.0f, 70.0f, 47.0f, 0.4f, 0) == false);
    Check("... and with both left out of the list it is free", DriveSteer_WayFreeOfCars(cars, 0, 76.0f, 52.0f, 70.0f, 47.0f, 0.4f, -1));
}

static void RouteProgressTests()
{
    WayFreeTests();
    // the route runs east along y=10 to x=20, jogs south to y=12, one tile east, then north along x=21
    const float wx[] = {10.0f, 20.0f, 20.0f, 21.0f, 21.0f};
    const float wy[] = {10.0f, 10.0f, 12.0f, 12.0f, -20.0f};
    Check("on the first leg: head for the corner ahead", LocalPlan_RouteProgress(wx, wy, 5, 1, 15.0f, 10.0f, 1.2f) == 1);
    Check("went straight on past the jog onto the leg north: the jog's corners are dropped", LocalPlan_RouteProgress(wx, wy, 5, 2, 21.2f, 5.0f, 1.2f) == 4);
    Check("short of the jog, 3 tiles from the leg north: not skipped", LocalPlan_RouteProgress(wx, wy, 5, 2, 18.0f, 10.0f, 1.2f) == 2);
    Check("far from every later leg: not skipped", LocalPlan_RouteProgress(wx, wy, 5, 2, 30.0f, 5.0f, 1.2f) == 2);
}

int main()
{
    RouteProgressTests();
    std::vector<SteerCar> none;
    // 1. open ground: a straight line, every step within 45 degrees of the line, one node per tile
    {
        Grid g;
        g.x0 = 0; g.y0 = 0; g.w = 80; g.h = 80;
        g.cell.assign(g.w * g.h, 'F');
        LocalPlanOut p;
        const bool ok = LocalPlan_Find(World(g, none, false), 10.5f, 10.5f, 90.0f, 28.5f, 22.5f, &p);
        Check("open ground: found", ok);
        Check("open ground: stays within a tile and a bit of the line (8 directions)", ok && MaxOffLine(p, 10.5f, 10.5f, 28.5f, 22.5f) < 1.3f, "off the line");
        Check("open ground: smoothed to a few nodes, not a zigzag of tiles", ok && p.count >= 2 && p.count <= 6, "node count");
    }
    // 2. a block of buildings across the line: round it, never through
    {
        Grid g;
        g.x0 = 0; g.y0 = 0; g.w = 80; g.h = 80;
        g.cell.assign(g.w * g.h, 'F');
        for (int y = 14; y < 26; y++) for (int x = 20; x < 24; x++) g.cell[y * g.w + x] = 'B';
        LocalPlanOut p;
        const bool ok = LocalPlan_Find(World(g, none, false), 10.5f, 19.5f, 90.0f, 34.5f, 19.5f, &p);
        Check("a block across the line: a way round is found", ok);
        Check("a block across the line: no node in a building", ok && !PathTouchesSolid(g, p), "node in a building");
    }
    // 3. a road with a queue of standing cars in both lanes and a pavement free beside: through the pavement
    {
        Grid g;
        g.x0 = 0; g.y0 = 0; g.w = 90; g.h = 40;
        g.cell.assign(g.w * g.h, 'B');
        for (int y = 14; y < 26; y++) for (int x = 2; x < 88; x++) g.cell[y * g.w + x] = (y < 17 || y >= 23) ? 'P' : '>';
        std::vector<SteerCar> cars;
        for (int i = 0; i < 3; i++)
        {
            cars.push_back(MakeCar(24.0f + 3.0f * i, 18.5f, 0.0f, 0.0f));
            cars.push_back(MakeCar(24.0f + 3.0f * i, 21.5f, 0.0f, 0.0f));
        }
        LocalPlanOut p;
        const bool ok = LocalPlan_Find(World(g, cars, false), 14.5f, 19.5f, 90.0f, 38.5f, 19.5f, &p);
        Check("queue in both lanes: a way by the pavement is found", ok);
        bool through_car = false;
        for (int i = 1; ok && i < p.count; i++)
            for (size_t k = 0; k < cars.size(); k++)
                if (SteerBox_Contains(cars[k].box, p.x[i], p.y[i], 0.2f)) through_car = true;
        Check("queue in both lanes: no node under a car", ok && !through_car, "node under a car");
        Check("queue in both lanes: the overlay gets the closed tiles", ok && p.car_closed_count > 10, "closed tiles not reported");
    }
    // 4. a car facing east with the goal north-east: the first steps do not turn back
    {
        Grid g;
        g.x0 = 0; g.y0 = 0; g.w = 80; g.h = 80;
        g.cell.assign(g.w * g.h, 'F');
        LocalPlanOut p;
        const bool ok = LocalPlan_Find(World(g, none, false), 20.5f, 40.5f, 90.0f, 32.5f, 30.5f, &p);
        Check("facing east, goal north-east: the first leg goes forward", ok && p.count > 1 && p.x[1] > p.x[0] + 1.0f, "turned back");
    }
    // 5. two lanes side by side going opposite ways: the one that goes our way
    {
        Grid g;
        g.x0 = 0; g.y0 = 0; g.w = 80; g.h = 40;
        g.cell.assign(g.w * g.h, 'B');
        for (int x = 2; x < 78; x++) { g.cell[18 * g.w + x] = '<'; g.cell[19 * g.w + x] = '<'; g.cell[20 * g.w + x] = '>'; g.cell[21 * g.w + x] = '>'; }
        LocalPlanOut p;
        // driving east from the middle of the road, the line along y = 19.5 (a '<' lane)
        const bool ok = LocalPlan_Find(World(g, none, true), 10.5f, 19.5f, 90.0f, 34.5f, 19.5f, &p);
        int against = 0;
        for (int i = 1; ok && i < p.count; i++) if (g.At(p.x[i], p.y[i]) == '<') against++;
        Check("two lanes: keeps to the lane that goes our way", ok && against <= 3, "drove the wrong way");
    }
    // 6. the real junction map: from the stuck start to the way on
    {
        Grid g;
        if (LoadGrid("junction_96_214.map", &g))
        {
            LocalPlanOut p;
            const bool ok = LocalPlan_Find(World(g, none, false), 94.0f, 212.5f, 200.0f, 98.5f, 223.5f, &p);
            Check("real junction: a route from the corner start", ok);
            Check("real junction: no node in a building", ok && !PathTouchesSolid(g, p), "node in a building");
            Check("real junction: a short route", ok && p.count < 14, "too long");
        }
        else
        {
            Check("real junction map present", false);
        }
    }
    // 7. no way: closed in
    {
        Grid g;
        g.x0 = 0; g.y0 = 0; g.w = 60; g.h = 60;
        g.cell.assign(g.w * g.h, 'F');
        for (int x = 8; x < 40; x++) { g.cell[30 * g.w + x] = 'B'; g.cell[10 * g.w + x] = 'B'; }
        for (int y = 10; y <= 30; y++) { g.cell[y * g.w + 8] = 'B'; g.cell[y * g.w + 39] = 'B'; }
        LocalPlanOut p;
        const bool ok = LocalPlan_Find(World(g, none, false), 20.5f, 20.5f, 90.0f, 50.5f, 20.5f, &p);
        Check("walled in: reports no route or a partial one, never a whole one", !ok || p.partial);
    }
    // 7b. a one tile alley between two buildings is the only way: the car fits and the whole-map route goes that way, so the detailed
    // route must too (a whole route, not a partial one that ends in front of the alley)
    {
        Grid g;
        g.x0 = 0; g.y0 = 0; g.w = 60; g.h = 60;
        g.cell.assign(g.w * g.h, 'F');
        for (int y = 0; y < 60; y++) { if (y != 30) { for (int x = 29; x <= 31; x++) g.cell[y * g.w + x] = 'B'; } }
        for (int x = 29; x <= 31; x++) { g.cell[29 * g.w + x] = 'B'; g.cell[31 * g.w + x] = 'B'; }
        LocalPlanOut p;
        const bool ok = LocalPlan_Find(World(g, none, false), 20.5f, 30.5f, 90.0f, 40.5f, 30.5f, &p);
        Check("one tile alley: a whole route through it", ok && !p.partial, "no route or a partial one");
    }
    // 7c. a police car coming along the way in open ground: the route goes well round it (a bump is a wanted level), not straight on
    {
        Grid g;
        g.x0 = 0; g.y0 = 0; g.w = 60; g.h = 60;
        g.cell.assign(g.w * g.h, 'F');
        std::vector<SteerCar> cars;
        SteerCar cop = MakeCar(30.5f, 20.5f, -0.06f, 0.0f);
        cop.police = true;
        cars.push_back(cop);
        LocalPlanOut p;
        const bool ok = LocalPlan_Find(World(g, cars, false), 20.5f, 20.5f, 90.0f, 45.5f, 20.5f, &p);
        float least = 1e9f;
        for (int i = 0; i < p.count; i++)
        {
            for (float t = 0.0f; t <= 1.0f && i + 1 < p.count; t += 0.1f)
            {
                const float qx = p.x[i] + (p.x[i + 1] - p.x[i]) * t;
                const float qy = p.y[i] + (p.y[i + 1] - p.y[i]) * t;
                const float d = sqrtf((qx - 30.5f) * (qx - 30.5f) + (qy - 20.5f) * (qy - 20.5f));
                least = d < least ? d : least;
            }
        }
        Check("police car on the way: a route round it, 1.2 tiles clear", ok && least > 1.2f, "route runs into or beside the police car");
    }
    // 7d. a wall between two columns of open tiles with one way over (the ramp): the route goes over at the ramp, as the whole-map route does
    {
        Grid g;
        g.x0 = 0; g.y0 = 0; g.w = 60; g.h = 60;
        g.cell.assign(g.w * g.h, 'F');
        LocalPlanWorld w = World(g, none, false);
        w.edge_ok = EdgeCb;
        LocalPlanOut p;
        const bool ok = LocalPlan_Find(w, 22.5f, 38.5f, 90.0f, 38.5f, 41.5f, &p);
        bool crosses_at_ramp = ok && !p.partial;
        int prev_x = 0, prev_y = 0;
        bool have = false;
        for (int i = 0; i + 1 < p.count; i++)
        {
            for (float t = 0.0f; t < 1.0f; t += 0.02f)
            {
                const int tx = static_cast<int>(p.x[i] + (p.x[i + 1] - p.x[i]) * t);
                const int ty = static_cast<int>(p.y[i] + (p.y[i + 1] - p.y[i]) * t);
                if (have && tx != prev_x && ((prev_x == 29 && tx == 30) || (prev_x == 30 && tx == 29)) && (prev_y != 40 || ty != 40))
                {
                    crosses_at_ramp = false;
                }
                prev_x = tx;
                prev_y = ty;
                have = true;
            }
        }
        Check("wall between columns: the route crosses at the ramp, not through the wall", crosses_at_ramp, "no route or it goes through the wall");
    }
    // 7c2. a car coming towards us along the way: the route is the square one that goes round its track, not a straight line pulled through it
    {
        Grid g;
        g.x0 = 0; g.y0 = 0; g.w = 60; g.h = 60;
        g.cell.assign(g.w * g.h, 'F');
        std::vector<SteerCar> cars;
        cars.push_back(MakeCar(30.5f, 20.5f, -0.06f, 0.0f));
        LocalPlanOut p;
        const bool ok = LocalPlan_Find(World(g, cars, false), 20.5f, 20.5f, 90.0f, 45.5f, 20.5f, &p);
        float least = 1e9f;
        for (int i = 0; i + 1 < p.count; i++)
        {
            for (float t = 0.0f; t <= 1.0f; t += 0.05f)
            {
                const float qx = p.x[i] + (p.x[i + 1] - p.x[i]) * t;
                const float qy = p.y[i] + (p.y[i + 1] - p.y[i]) * t;
                if (qx > 27.0f && qx < 31.0f && fabsf(qy - 20.5f) < least)
                {
                    least = fabsf(qy - 20.5f);
                }
            }
        }
        Check("a car coming along the way: the route keeps 0.9 tiles off its track", ok && least > 0.9f, "the route runs along or through its track");
    }
    // 7c3. the car is a lane off the global route: it joins within a few tiles and then keeps to it (not one long diagonal across the lanes)
    {
        Grid g;
        g.x0 = 0; g.y0 = 0; g.w = 60; g.h = 80;
        g.cell.assign(g.w * g.h, 'F');
        LocalPlanWorld w = World(g, none, false);
        const float rx[3] = {30.5f, 30.5f, 30.5f};
        const float ry[3] = {60.5f, 45.5f, 36.5f};
        w.route_x = rx;
        w.route_y = ry;
        w.route_n = 3;
        LocalPlanOut p;
        const bool ok = LocalPlan_Find(w, 28.5f, 60.5f, 0.0f, 30.5f, 36.5f, &p);
        float worst = 0.0f;
        for (int i = 0; i + 1 < p.count; i++)
        {
            for (float t = 0.0f; t < 1.0f; t += 0.05f)
            {
                const float qy = p.y[i] + (p.y[i + 1] - p.y[i]) * t;
                const float qx = p.x[i] + (p.x[i + 1] - p.x[i]) * t;
                if (qy < 55.0f && fabsf(qx - 30.5f) > worst)
                {
                    worst = fabsf(qx - 30.5f);
                }
            }
        }
        Check("a lane off the global route: on it within 5 tiles, then keeps to it", ok && worst < 0.9f, "the route wanders off the global one");
    }
    // 7g. a right-angle turn with a building straight on past it: the route cuts across the corner (every combination of directions), so the
    // nose does not swing into the building, and no vertex sits on the corner point itself
    {
        for (int combo = 0; combo < 8; combo++)
        {
            // legs: in along +x, +y, -x, -y (combos 0-3 turn one way, 4-7 the other)
            const int in_dir = combo % 4;
            const int turn = combo < 4 ? 1 : 3;
            const int out_dir = (in_dir + turn) % 4;
            static const int dxs[4] = {1, 0, -1, 0};
            static const int dys[4] = {0, 1, 0, -1};
            Grid g;
            g.x0 = 0; g.y0 = 0; g.w = 60; g.h = 60;
            g.cell.assign(g.w * g.h, 'B');
            const int cx = 30, cy = 30; // the corner tile
            for (int k = -12; k <= 0; k++) g.cell[(cy + dys[in_dir] * k) * g.w + (cx + dxs[in_dir] * k)] = 'F';
            for (int k = 0; k <= 12; k++) g.cell[(cy + dys[out_dir] * k) * g.w + (cx + dxs[out_dir] * k)] = 'F';
            std::vector<SteerCar> cars;
            LocalPlanOut p;
            const float sx = cx + 0.5f - dxs[in_dir] * 8.0f, sy = cy + 0.5f - dys[in_dir] * 8.0f;
            const float gx = cx + 0.5f + dxs[out_dir] * 8.0f, gy = cy + 0.5f + dys[out_dir] * 8.0f;
            const bool ok = LocalPlan_Find(World(g, cars, false), sx, sy, in_dir * 90.0f + 90.0f, gx, gy, &p);
            bool on_corner = false;
            for (int i = 0; i < p.count; i++)
            {
                if (fabsf(p.x[i] - (cx + 0.5f)) < 0.05f && fabsf(p.y[i] - (cy + 0.5f)) < 0.05f)
                {
                    on_corner = true;
                }
            }
            // the inside corner of the turn (the corner of the building on the inside, diagonal from the corner tile): the route keeps its distance
            const float ix = cx + 0.5f + (dxs[out_dir] - dxs[in_dir]) * 0.5f;
            const float iy = cy + 0.5f + (dys[out_dir] - dys[in_dir]) * 0.5f;
            float least = 1e9f;
            for (int i = 0; i + 1 < p.count; i++)
            {
                for (float t = 0.0f; t <= 1.0f; t += 0.05f)
                {
                    const float qx = p.x[i] + (p.x[i + 1] - p.x[i]) * t - ix;
                    const float qy = p.y[i] + (p.y[i + 1] - p.y[i]) * t - iy;
                    least = sqrtf(qx * qx + qy * qy) < least ? sqrtf(qx * qx + qy * qy) : least;
                }
            }
            char name[100];
            snprintf(name, sizeof(name), "corner turn %d: across the corner, not round its point", combo);
            Check(name, ok && !on_corner && p.count >= 4, "a vertex on the corner point or no route");
            snprintf(name, sizeof(name), "corner turn %d: as clear of the building corner on the inside as the corridor itself is", combo);
            Check(name, ok && least >= 0.45f, "the route clips the inside corner");
        }
    }
    // 7h. cars beside the route in the next lane: when the route has been planned round them the wheel follows the aim point (the arcs
    // used to turn off it for every car that would overlap, 3 in 10 of these cases)
    {
        int changed_planned = 0, changed_blind = 0;
        for (int rhc = 0; rhc < 2; rhc++)
        {
            for (float cx = 0.8f; cx <= 2.0f; cx += 0.3f)
            {
                for (float cy = -2.0f; cy >= -8.0f; cy -= 1.0f)
                {
                    for (float cvy = -0.14f; cvy <= 0.0f; cvy += 0.07f)
                    {
                        SteerCar c = MakeCar(170.0f + cx, 140.0f + cy, 0.0f, cvy);
                        c.box.ax = 0.0f;
                        c.box.ay = 1.0f;
                        c.box.hl = 0.5f;
                        c.box.hw = 0.25f;
                        SteerWorld sw;
                        sw.solid = SolidCb;
                        sw.user = NULL;
                        sw.cars = &c;
                        sw.car_count = 1;
                        sw.own_hl = 0.5f;
                        sw.own_hw = 0.3f;
                        sw.route_has_cars = rhc == 1;
                        Grid g;
                        g.x0 = 100; g.y0 = 100; g.w = 150; g.h = 100;
                        g.cell.assign(g.w * g.h, 'F');
                        sw.user = &g;
                        const SteerChoice ch = DriveSteer_Choose(sw, 170.0f, 140.0f, 0.0f, 1.0f, 0.14f, 171.5f, 130.0f, 0.0f, NULL);
                        (rhc == 1 ? changed_planned : changed_blind) += ch.changed ? 1 : 0;
                    }
                }
            }
        }
        Check("cars beside a planned route: the wheel follows the aim point", changed_planned == 0, "the arcs turned off the aim point");
        Check("cars beside an unplanned aim point: the arcs still dodge (as before)", changed_blind > 0);
    }
    // 7i. a chase: the car being chased is where the route ends, so it is left out of the cars to plan round; the others stay
    {
        SteerCar cs[3] = {MakeCar(60.0f, 60.0f, 0.1f, 0.0f), MakeCar(63.5f, 60.0f, 0.0f, 0.0f), MakeCar(80.0f, 60.0f, 0.0f, 0.0f)};
        const int left = DriveSteer_DropCarsNear(cs, 3, 60.5f, 60.0f, 2.0f);
        Check("a chase: the chased car is left out, the others stay", left == 2 && cs[0].x == 63.5f && cs[1].x == 80.0f);
    }
    // 7i1. the car is 0.6 tile off the global route, and the tile route is a line from the car to the end of the route (parallel to it, a little off
    // it): the fine stage joins the route within a few tiles, with a node on it (it was a line that never got there)
    {
        Grid g;
        g.x0 = 0; g.y0 = 0; g.w = 80; g.h = 40;
        g.cell.assign(g.w * g.h, 'B');
        for (int y = 14; y < 26; y++) for (int x = 2; x < 78; x++) g.cell[y * g.w + x] = '>';
        std::vector<SteerCar> cars;
        LocalPlanWorld w = World(g, cars, false);
        const float rx[2] = {20.5f, 60.5f}, ry[2] = {19.5f, 19.5f};
        w.route_x = rx;
        w.route_y = ry;
        w.route_n = 2;
        LocalPlanOut tile;
        memset(&tile, 0, sizeof(tile));
        tile.found = true;
        tile.count = 2;
        tile.x[0] = 20.5f; tile.y[0] = 20.1f; // the car, 0.6 below the route
        tile.x[1] = 60.5f; tile.y[1] = 19.5f; // the end of the route: the line to it is 1.5 degrees off
        LocalPlanOut fine;
        LocalPlan_Fine(w, tile, 20.5f, 20.1f, &fine);
        bool joined = false;
        float at = 0.0f;
        for (int i = 1; i < fine.count; i++)
        {
            if (fabsf(fine.y[i] - 19.5f) < 0.05f && fine.x[i] - 20.5f < 8.0f)
            {
                joined = true;
                at = fine.x[i] - 20.5f;
            }
        }
        char what[120];
        sprintf(what, "a car 0.6 off the route: a node on the route within 8 tiles (%.1f)", at);
        Check(what, joined);
    }
    // 7i2. the straight-line pedestrian dodge: not while a detailed route exists (it goes round pedestrians itself), else a point beside him
    {
        struct S { static bool Solid(float, float) { return false; } };
        const float pxs[1] = {10.0f}, pys[1] = {4.0f}; // a pedestrian 4 tiles ahead (heading -Y from (10, 8)), in the lane
        float ax = 10.0f, ay = 0.0f;
        const bool planned = DriveSteer_PedDodgeAim(true, 10.0f, 8.0f, 0.0f, -1.0f, 0.0f, -1.0f, pxs, pys, 1, S::Solid, &ax, &ay);
        Check("a detailed route exists: the aim is left on the route", !planned && ax == 10.0f && ay == 0.0f);
        const bool dodged = DriveSteer_PedDodgeAim(false, 10.0f, 8.0f, 0.0f, -1.0f, 0.0f, -1.0f, pxs, pys, 1, S::Solid, &ax, &ay);
        Check("no detailed route: the aim moves to the side of the pedestrian", dodged && fabsf(ax - 10.0f) > 1.0f && ay < 4.0f);
    }
    // 7j. a pedestrian standing in the lane of a wide road: the route goes round him with room (a pedestrian's box is small, the car's margin is not)
    {
        Grid g;
        g.x0 = 0; g.y0 = 0; g.w = 80; g.h = 40;
        g.cell.assign(g.w * g.h, 'B');
        for (int y = 14; y < 26; y++) for (int x = 2; x < 78; x++) g.cell[y * g.w + x] = '>';
        std::vector<SteerCar> cars;
        const SteerCar ped = DriveSteer_PedAsCar(40.5f, 19.5f, false, 0.0f, 0.0f, 0.0f, 0.0f);
        Check("a pedestrian with no box from the game gets a small square, standing", ped.speed == 0.0f && ped.box.hl == 0.15f && !ped.police);
        const SteerCar ped2 = DriveSteer_PedAsCar(1.0f, 2.0f, true, 0.0f, 1.0f, 0.12f, 0.1f);
        Check("a pedestrian with a box keeps its own", ped2.box.ay == 1.0f && ped2.box.hl == 0.12f && ped2.box.cx == 1.0f);
        cars.push_back(ped);
        LocalPlanOut p;
        const bool ok = LocalPlan_Find(World(g, cars, false), 20.5f, 19.5f, 90.0f, 60.5f, 19.5f, &p);
        float least = 1e9f;
        for (int i = 0; i + 1 < p.count; i++)
        {
            for (float t = 0.0f; t <= 1.0f; t += 0.05f)
            {
                const float qx = p.x[i] + (p.x[i + 1] - p.x[i]) * t - 40.5f;
                const float qy = p.y[i] + (p.y[i + 1] - p.y[i]) * t - 19.5f;
                least = sqrtf(qx * qx + qy * qy) < least ? sqrtf(qx * qx + qy * qy) : least;
            }
        }
        Check("a pedestrian in the lane: the route passes him with 0.7 tiles clear", ok && least > 0.7f, "the route goes through or beside him");
    }
    // 7k. nothing in the way: the detailed route IS the global route (its own square legs, the car joining it within 4 tiles); a car or a
    // pedestrian on it: only then a way round is searched
    {
        Grid g;
        g.x0 = 0; g.y0 = 0; g.w = 80; g.h = 60;
        g.cell.assign(g.w * g.h, 'F');
        const float rx[4] = {20.5f, 20.5f, 40.5f, 40.5f};
        const float ry[4] = {50.5f, 30.5f, 30.5f, 20.5f};
        LocalPlanWorld w = World(g, none, false);
        w.route_x = rx;
        w.route_y = ry;
        w.route_n = 4;
        LocalPlanOut p;
        bool ok = LocalPlan_Find(w, 20.5f, 50.5f, 0.0f, 40.5f, 20.5f, &p);
        float worst = 0.0f;
        for (int i = 0; i + 1 < p.count; i++)
        {
            for (float t = 0.0f; t <= 1.0f; t += 0.05f)
            {
                const float qx = p.x[i] + (p.x[i + 1] - p.x[i]) * t;
                const float qy = p.y[i] + (p.y[i + 1] - p.y[i]) * t;
                // distance to the route legs
                float d = 1e9f;
                for (int k = 0; k + 1 < 4; k++)
                {
                    const float ex = rx[k + 1] - rx[k], ey = ry[k + 1] - ry[k];
                    float u = ((qx - rx[k]) * ex + (qy - ry[k]) * ey) / (ex * ex + ey * ey);
                    u = u < 0.0f ? 0.0f : (u > 1.0f ? 1.0f : u);
                    const float dx = rx[k] + ex * u - qx, dy = ry[k] + ey * u - qy;
                    d = sqrtf(dx * dx + dy * dy) < d ? sqrtf(dx * dx + dy * dy) : d;
                }
                worst = d > worst ? d : worst;
            }
        }
        Check("nothing in the way: the detailed route runs on the global route", ok && !p.partial && worst < 0.75f, "it leaves the global route (a corner may be cut by a chord)");
        // a standing car on the second leg: a way round, clear of it
        std::vector<SteerCar> cars;
        cars.push_back(MakeCar(20.5f, 40.5f, 0.0f, 0.0f));
        LocalPlanWorld w2 = World(g, cars, false);
        w2.route_x = rx;
        w2.route_y = ry;
        w2.route_n = 4;
        ok = LocalPlan_Find(w2, 20.5f, 50.5f, 0.0f, 40.5f, 20.5f, &p);
        float least = 1e9f;
        for (int i = 0; i + 1 < p.count; i++)
        {
            for (float t = 0.0f; t <= 1.0f; t += 0.05f)
            {
                const float qx = p.x[i] + (p.x[i + 1] - p.x[i]) * t - 20.5f;
                const float qy = p.y[i] + (p.y[i + 1] - p.y[i]) * t - 40.5f;
                least = sqrtf(qx * qx + qy * qy) < least ? sqrtf(qx * qx + qy * qy) : least;
            }
        }
        Check("a car on the global route: a way round it, 1 tile clear", ok && least > 1.0f, "the route goes through or against the car");
    }
    // 7e. the foot of a ramp with the route turning right at its top: the aim point stops at the top of the ramp, not past the corner
    {
        LocalPlanOut p;
        memset(&p, 0, sizeof(p));
        p.found = true;
        p.count = 4;
        p.x[0] = 160.7f; p.y[0] = 48.3f;
        p.x[1] = 160.5f; p.y[1] = 48.5f;
        p.x[2] = 160.5f; p.y[2] = 45.5f;
        p.x[3] = 165.5f; p.y[3] = 45.5f;
        const float l = LocalPlan_RampLook(p, 160.7f, 48.3f, 8.0f, RampCb, NULL);
        float ax = 0.0f, ay = 0.0f;
        LocalPlan_AimPoint(p, 160.7f, 48.3f, l, &ax, &ay);
        Check("foot of a ramp: aims at the top of it, not past the corner", l < 4.5f && ax < 161.0f, "the aim point cuts the corner");
        LocalPlanOut q;
        memset(&q, 0, sizeof(q));
        q.found = true;
        q.count = 2;
        q.x[0] = 140.5f; q.y[0] = 60.5f;
        q.x[1] = 150.5f; q.y[1] = 60.5f;
        const float far_look = LocalPlan_RampLook(q, 141.5f, 60.5f, 2.0f, RampCb, NULL);
        Check("far from a ramp the look distance is untouched", far_look == 2.0f);
    }
    // 7e2. coming at the alley from the side: the aim point holds at the approach (on the middle line of the ramp) until the car is lined up
    {
        LocalPlanOut p;
        memset(&p, 0, sizeof(p));
        p.found = true;
        p.count = 4;
        p.x[0] = 164.0f; p.y[0] = 52.5f;
        p.x[1] = 160.5f; p.y[1] = 52.5f;
        p.x[2] = 160.5f; p.y[2] = 45.5f;
        p.x[3] = 165.5f; p.y[3] = 45.5f;
        const float l = LocalPlan_RampLook(p, 164.0f, 52.5f, 12.0f, RampCb, NULL);
        float ax = 0.0f, ay = 0.0f;
        LocalPlan_AimPoint(p, 164.0f, 52.5f, l, &ax, &ay);
        Check("coming at a ramp from the side: aims at its approach, on its middle line", fabsf(ax - 160.5f) < 0.1f && ay > 49.0f, "the aim point runs on past the approach");
    }
    // 7e3. lines over a ramp and the two tiles before it run along its axis, in the middle: no corner cut into the alley
    {
        Grid g;
        g.x0 = 140; g.y0 = 30; g.w = 40; g.h = 40;
        g.cell.assign(g.w * g.h, 'F');
        LocalPlanWorld w = World(g, none, false);
        w.ramp_zone = RampCb;
        LocalPlanOut p;
        const bool ok = LocalPlan_Find(w, 164.5f, 52.5f, 270.0f, 160.5f, 44.5f, &p);
        bool lined = ok;
        for (int i = 0; i + 1 < p.count; i++)
        {
            const float sx = p.x[i + 1] - p.x[i];
            const float sy = p.y[i + 1] - p.y[i];
            const float sl = sqrtf(sx * sx + sy * sy);
            for (float t = 0.0f; t < 1.0f && sl > 0.01f; t += 0.02f)
            {
                const float qx = p.x[i] + sx * t;
                const float qy = p.y[i] + sy * t;
                if (RampCb(NULL, qx, qy) && (fabsf(sx / sl) > 0.12f || fabsf(qx - 160.5f) > 0.3f))
                {
                    lined = false;
                }
            }
        }
        Check("a route up a ramp is lined up along it for the two tiles before and over it", lined, "the route cuts the corner into the ramp");
    }
    // 7e4. the car is off the global route at the side of a ramp's alley: it does not join the route on the approach by a diagonal
    {
        Grid g;
        g.x0 = 140; g.y0 = 30; g.w = 40; g.h = 40;
        g.cell.assign(g.w * g.h, 'F');
        LocalPlanWorld w = World(g, none, false);
        w.ramp_zone = RampCb;
        const float rx[2] = {160.5f, 160.5f};
        const float ry[2] = {52.5f, 40.5f};
        w.route_x = rx;
        w.route_y = ry;
        w.route_n = 2;
        LocalPlanOut p;
        const bool ok = LocalPlan_Find(w, 163.5f, 52.5f, 270.0f, 160.5f, 40.5f, &p);
        bool lined = ok;
        for (int i = 0; i + 1 < p.count; i++)
        {
            const float sx = p.x[i + 1] - p.x[i];
            const float sy = p.y[i + 1] - p.y[i];
            const float sl = sqrtf(sx * sx + sy * sy);
            for (float t = 0.0f; t < 1.0f && sl > 0.01f; t += 0.02f)
            {
                const float qx = p.x[i] + sx * t;
                const float qy = p.y[i] + sy * t;
                if (RampCb(NULL, qx, qy) && (fabsf(sx / sl) > 0.12f || fabsf(qx - 160.5f) > 0.3f))
                {
                    lined = false;
                }
            }
        }
        Check("off the route beside a ramp: lined up before the ramp, on its middle line", lined, "the route runs diagonally over the ramp's approach");
    }
    // 7f. a car that will be on the route when we get there is in the way; one that has gone by is not
    {
        SteerCar crossing = MakeCar(165.0f, 51.0f, 0.0f, -0.05f); // crossing north to south... moving up the screen
        crossing.vy = 0.05f;                                       // moving down the screen (y grows), 20 frames to the route at y=52
        Check("a car crossing the route in time is in the way", LocalPlan_CarInWayAt(crossing, 165.0f, 52.0f, 20.0f, 0.2f));
        Check("the same car, we are there at once, is not yet", !LocalPlan_CarInWayAt(crossing, 165.0f, 52.0f, 0.0f, 0.2f));
        Check("the same car, we are there much later, has gone by", !LocalPlan_CarInWayAt(crossing, 165.0f, 52.0f, 80.0f, 0.2f));
        SteerCar still = MakeCar(165.0f, 52.0f, 0.0f, 0.0f);
        Check("a standing car on the route is in the way", LocalPlan_CarInWayAt(still, 165.0f, 52.0f, 30.0f, 0.2f));
    }
    // 8. the road shut by a row of standing cars with no way round: a route through the cheapest gap, not "no route"
    {
        Grid g;
        g.x0 = 0; g.y0 = 0; g.w = 80; g.h = 40;
        g.cell.assign(g.w * g.h, 'B');
        for (int y = 16; y < 24; y++) for (int x = 2; x < 78; x++) g.cell[y * g.w + x] = '>';
        std::vector<SteerCar> cars;
        for (int y = 0; y < 8; y++) cars.push_back(MakeCar(30.0f, 16.5f + y * 1.0f, 0.0f, 0.0f));
        LocalPlanOut p;
        const bool ok = LocalPlan_Find(World(g, cars, false), 14.5f, 19.5f, 90.0f, 44.5f, 19.5f, &p);
        Check("road shut by standing cars: still a route (a bump through)", ok);
    }
    printf("%d failed\n", fails);
    return fails ? 1 : 0;
}
