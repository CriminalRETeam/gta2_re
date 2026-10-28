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
    printf("%d failed\n", fails);
    return fails ? 1 : 0;
}
