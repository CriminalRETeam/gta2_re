// The local planner on a real map: the main road at (30..33, 65) runs under a building (a tunnel; blocks at height 3 and 4 above it
// with wall flags on their faces). It was taken for a fence across the road, and the planner found no route through.
//   g++ -O1 -I../../../Source/Bot -o local_plan_real_sim local_plan_real_sim.cpp ../../../Source/Bot/LocalPlan.cpp ../../../Source/Bot/DriveSteer.cpp && ./local_plan_real_sim
#include "LocalPlan.hpp"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
struct Grid { int x0, y0, w, h; std::vector<char> cell; };
static Grid g;
static bool Solid(void*, float x, float y)
{
    const int ix = (int)floorf(x) - g.x0, iy = (int)floorf(y) - g.y0;
    if (ix < 0 || iy < 0 || ix >= g.w || iy >= g.h) return true;
    const char c = g.cell[iy * g.w + ix];
    return c == 'B' || c == 'W' || c == ' ';
}
static int fails = 0;
static void Check(const char* name, bool ok)
{
    printf("%-58s %s\n", name, ok ? "PASS" : "FAIL");
    fails += ok ? 0 : 1;
}

static bool Route(float x0, float y0, float heading, float x1, float y1)
{
    LocalPlanWorld w = {Solid, 0, 0, 0, 0, 0.5f, 0.52f};
    LocalPlanOut out;
    LocalPlan_Find(w, x0, y0, heading, x1, y1, &out);
    return out.found;
}

int main()
{
    FILE* f = fopen("arch_31_65.map", "r");
    if (!f)
    {
        printf("map missing\n");
        return 1;
    }
    char line[512];
    int x1 = 0, y1 = 0;
    fgets(line, sizeof(line), f);
    sscanf(strstr(line, "x ") + 2, "%d..%d, y %d..%d", &g.x0, &x1, &g.y0, &y1);
    g.w = x1 - g.x0 + 1;
    g.h = y1 - g.y0 + 1;
    g.cell.assign(g.w * g.h, ' ');
    fgets(line, sizeof(line), f);
    fgets(line, sizeof(line), f);
    while (fgets(line, sizeof(line), f))
    {
        const int y = atoi(line);
        if (y < g.y0 || y > y1)
        {
            continue;
        }
        for (int x = 0; x < g.w && line[4 + x] && line[4 + x] != '\n'; x++)
        {
            g.cell[(y - g.y0) * g.w + x] = line[4 + x];
        }
    }
    fclose(f);
    Check("south along the main road through the tunnel", Route(32.9f, 43.4f, 180.0f, 30.8f, 67.0f));
    Check("north along the main road through the tunnel", Route(32.5f, 70.0f, 0.0f, 32.5f, 48.0f));
    Check("a goal inside a block of buildings: the route ends as near as it gets", Route(32.5f, 60.0f, 180.0f, 14.0f, 61.0f));
    printf("%s\n", fails ? "FAILED" : "all passed");
    return fails ? 1 : 0;
}
