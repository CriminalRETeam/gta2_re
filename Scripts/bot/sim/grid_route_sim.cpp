// Native test of GridRoute: routes are square (long legs, right angles), go through gaps, keep to the lane's side of a road,
// go round things that will be in the way when we get there, and end at the nearest reachable tile when the goal is shut in.
#include "GridRoute.hpp"
#include <stdio.h>
#include <string.h>
#include <math.h>

static int fails = 0;
#define CHECK(c, ...)                         \
    do                                        \
    {                                         \
        if (!(c))                             \
        {                                     \
            printf("FAIL: " __VA_ARGS__);     \
            printf("\n");                     \
            fails++;                          \
        }                                     \
    } while (0)

struct Grid
{
    unsigned char solid[64 * 64];
    signed char lane[64 * 64]; // 0 none, else the dir+1 cars go on this tile
    // a car that is on tile (cx, cy) from frame t0 to t1
    int car_x, car_y, car_t0, car_t1;
    float speed; // tiles per frame
};

static float Step(void* user, int x, int y, int dir, float g)
{
    const Grid* gr = static_cast<const Grid*>(user);
    static const int dx[4] = {0, 1, 0, -1};
    static const int dy[4] = {-1, 0, 1, 0};
    const int nx = x + dx[dir];
    const int ny = y + dy[dir];
    if (nx < 0 || ny < 0 || nx >= 64 || ny >= 64 || gr->solid[ny * 64 + nx])
    {
        return -1.0f;
    }
    float c = 0.0f;
    const int lane = gr->lane[ny * 64 + nx];
    if (lane)
    {
        c += (lane - 1 == dir) ? -0.2f : ((lane - 1 + 2) % 4 == dir ? 4.0f : 1.0f);
    }
    if (gr->car_t1 > 0 && nx == gr->car_x && ny == gr->car_y)
    {
        const float when = (g + 1.0f) / gr->speed;
        if (when >= gr->car_t0 && when <= gr->car_t1)
        {
            c += 50.0f;
        }
    }
    return c;
}

static void Init(Grid* g)
{
    memset(g, 0, sizeof(*g));
    g->speed = 0.1f;
}

static GridRouteWorld World(Grid* g)
{
    GridRouteWorld w;
    w.x0 = 0;
    w.y0 = 0;
    w.w = 64;
    w.h = 64;
    w.step = Step;
    w.user = g;
    return w;
}

static int Corners(const GridRouteOut& r)
{
    float xs[64], ys[64];
    return GridRoute_Corners(r, xs, ys, 64) - 2;
}

int main()
{
    GridRouteParams p;
    GridRoute_DefaultParams(&p);
    static GridRouteOut out;
    static Grid g;

    // open field: Manhattan length, one corner, no staircase
    Init(&g);
    GridRouteWorld w = World(&g);
    CHECK(GridRoute_Find(w, 5, 5, 30, 20, p, &out), "open field no route");
    CHECK(out.count == 25 + 15 + 1, "open field length %d", out.count);
    CHECK(Corners(out) <= 1, "open field corners %d (a staircase)", Corners(out));

    // a wall across with a gap: through the gap
    Init(&g);
    for (int y = 0; y < 64; y++)
    {
        if (y != 40)
        {
            g.solid[y * 64 + 20] = 1;
        }
    }
    w = World(&g);
    CHECK(GridRoute_Find(w, 5, 10, 35, 10, p, &out), "gap no route");
    bool through = false;
    for (int i = 0; i < out.count; i++)
    {
        if (out.x[i] == 20 && out.y[i] == 40)
        {
            through = true;
        }
    }
    CHECK(through, "did not use the gap");
    CHECK(!out.partial, "gap route partial");

    // a two lane road along x: cars go right on y=10, left on y=11. Starting in the wrong lane, going right: the route
    // gets onto the right lane and stays on it
    Init(&g);
    for (int x = 0; x < 64; x++)
    {
        g.lane[10 * 64 + x] = kGridDirRight + 1;
        g.lane[11 * 64 + x] = kGridDirLeft + 1;
    }
    w = World(&g);
    CHECK(GridRoute_Find(w, 5, 11, 55, 11, p, &out), "lane route");
    int against = 0;
    for (int i = 1; i < out.count; i++)
    {
        if (out.y[i] == 11 && out.x[i] > out.x[i - 1])
        {
            against++;
        }
    }
    CHECK(against <= 2, "%d tiles driven against the lane", against);

    // the same road, a car on the lane's tile when we would pass: round it, not through it
    Init(&g);
    g.car_x = 30;
    g.car_y = 10;
    g.car_t0 = 0;
    g.car_t1 = 1000;
    w = World(&g);
    CHECK(GridRoute_Find(w, 5, 10, 55, 10, p, &out), "car route");
    bool hit = false;
    for (int i = 0; i < out.count; i++)
    {
        if (out.x[i] == 30 && out.y[i] == 10)
        {
            hit = true;
        }
    }
    CHECK(!hit, "drove through the car");

    // a car that has gone by the time we get there does not matter
    g.car_t0 = 0;
    g.car_t1 = 20; // gone after 20 frames, we are there after 25 tiles / 0.1 = 250 frames
    CHECK(GridRoute_Find(w, 5, 10, 55, 10, p, &out), "late car route");
    CHECK(out.count == 51 && Corners(out) == 0, "went round a car that was gone (count %d)", out.count);

    // facing the wrong way: the turn round costs, so a goal behind and just beside takes a route with the u-turn done once
    Init(&g);
    w = World(&g);
    p.start_dir = kGridDirRight;
    CHECK(GridRoute_Find(w, 30, 30, 20, 30, p, &out), "behind route");
    CHECK(out.count == 13, "behind route (a side step, the way back, a side step) length %d", out.count);
    p.start_dir = -1;

    // a goal shut in by walls: partial, ending next to the wall
    Init(&g);
    for (int i = 40; i <= 44; i++)
    {
        g.solid[40 * 64 + i] = 1;
        g.solid[44 * 64 + i] = 1;
        g.solid[i * 64 + 40] = 1;
        g.solid[i * 64 + 44] = 1;
    }
    w = World(&g);
    CHECK(GridRoute_Find(w, 5, 42, 42, 42, p, &out), "shut in no partial route");
    CHECK(out.partial, "shut in not marked partial");
    CHECK(abs(out.x[out.count - 1] - 39) <= 1, "partial route ends at %d,%d", out.x[out.count - 1], out.y[out.count - 1]);

    // walled in start: no route
    Init(&g);
    for (int i = 4; i <= 8; i++)
    {
        g.solid[4 * 64 + i] = 1;
        g.solid[8 * 64 + i] = 1;
        g.solid[i * 64 + 4] = 1;
        g.solid[i * 64 + 8] = 1;
    }
    w = World(&g);
    CHECK(!GridRoute_Find(w, 6, 6, 50, 50, p, &out), "route out of a pocket");

    // the whole map: 250 tiles across with a maze of walls, found within the effort limit
    Init(&g);
    for (int x = 4; x < 60; x += 4)
    {
        for (int y = 0; y < 64; y++)
        {
            g.solid[y * 64 + x] = (y != ((x / 4) % 2 ? 2 : 61)) ? 1 : 0;
        }
    }
    w = World(&g);
    CHECK(GridRoute_Find(w, 1, 2, 62, 2, p, &out), "maze");
    CHECK(out.count > 400, "maze route short %d", out.count);

    if (fails == 0)
    {
        printf("grid_route_sim: all passed\n");
    }
    return fails ? 1 : 0;
}
