// Native tests of the walker's own route copy (Source/Bot/WalkRoute.hpp).
//   g++ -O1 -I../../../Source/Bot -o walk_route_sim walk_route_sim.cpp && ./walk_route_sim
#include "WalkRoute.hpp"
#include <stdio.h>

static int fails = 0;
static void Check(const char* name, bool ok)
{
    printf("%-66s %s\n", name, ok ? "PASS" : "FAIL");
    fails += ok ? 0 : 1;
}

// closed tiles for the corner tests: a fence along y = 243 for x = 100 and 101 (the case seen at (102, 244))
static bool Fence(void*, int x, int y)
{
    return y == 243 && (x == 100 || x == 101);
}
static bool NoneClosed(void*, int, int)
{
    return false;
}
static bool BothClosed(void*, int x, int y)
{
    return (x == 102 && y == 244) || (x == 101 && y == 243);
}

int main()
{
    // a shared path that other code re-plans while the walker walks: first 25 long, then 11
    RouteNode shared[WalkRoute::kMax];
    for (int i = 0; i < 25; i++)
    {
        shared[i].x = static_cast<unsigned char>(100 + i);
        shared[i].y = 50;
        shared[i].z = 2;
    }
    WalkRoute route;
    route.Set(shared, 25, false);
    for (int i = 0; i < 11; i++) // somebody plans another route through the shared buffer
    {
        shared[i].x = 7;
        shared[i].y = 9;
    }
    Check("the shared path is re-planned: the walker's route does not change", route.Length() == 25 && route.At(20).x == 120 && route.At(20).y == 50);
    Check("an index past the end is the last node, not other memory", route.At(40).x == 124 && route.At(1000).x == 124);
    WalkRoute empty;
    Check("an empty route answers node 0, no crash", empty.Length() == 0 && empty.At(5).x == 0);
    RouteNode big[WalkRoute::kMax];
    for (int i = 0; i < WalkRoute::kMax; i++) big[i].x = static_cast<unsigned char>(i & 255);
    WalkRoute capped;
    capped.Set(big, WalkRoute::kMax + 500, true);
    Check("a path longer than the room is cut, and the approximate flag kept", capped.Length() == WalkRoute::kMax && capped.Approximate());
    {
        // from (102,243) the pathfinder steps diagonally to (101,244), past the end of the fence at (101,243)
        RouteNode path[3] = {{103, 242, 2}, {102, 243, 2}, {101, 244, 2}};
        RouteNode out[8];
        const unsigned n = WalkRoute_AvoidCorners(path, 3, out, 8, Fence, 0);
        Check("a diagonal past the end of a fence goes round the corner: one node added", n == 4);
        Check("... through the open side (102,244), then on to (101,244)", n == 4 && out[2].x == 102 && out[2].y == 244 && out[2].z == 2 && out[3].x == 101 && out[3].y == 244);
        Check("... and the nodes either side are kept", out[0].x == 103 && out[1].x == 102 && out[1].y == 243);
    }
    {
        RouteNode path[3] = {{103, 242, 2}, {102, 243, 2}, {101, 244, 2}};
        RouteNode out[8];
        Check("an open diagonal is left alone", WalkRoute_AvoidCorners(path, 3, out, 8, NoneClosed, 0) == 3);
        Check("a diagonal closed on both sides is left alone (it is not ours to fix)", WalkRoute_AvoidCorners(path, 3, out, 8, BothClosed, 0) == 3);
        RouteNode straight[3] = {{100, 50, 2}, {101, 50, 2}, {102, 50, 2}};
        Check("straight steps are left alone", WalkRoute_AvoidCorners(straight, 3, out, 8, Fence, 0) == 3);
        RouteNode longer[3] = {{103, 242, 2}, {102, 243, 2}, {101, 244, 2}};
        RouteNode tiny[3];
        const unsigned t = WalkRoute_AvoidCorners(longer, 3, tiny, 3, Fence, 0);
        Check("never writes past the room it was given", t <= 3);
    }
    printf("%d failed\n", fails);
    return fails ? 1 : 0;
}
