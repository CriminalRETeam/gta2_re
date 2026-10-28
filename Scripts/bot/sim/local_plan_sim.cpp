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

static LocalPlanWorld World(const Grid& g, const std::vector<SteerCar>& cars, bool lanes)
{
    LocalPlanWorld w;
    w.solid = SolidCb;
    w.step_cost = lanes ? LaneCost : NULL;
    w.user = const_cast<Grid*>(&g);
    w.cars = cars.empty() ? NULL : &cars[0];
    w.car_count = static_cast<int>(cars.size());
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

static void RouteProgressTests()
{
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
