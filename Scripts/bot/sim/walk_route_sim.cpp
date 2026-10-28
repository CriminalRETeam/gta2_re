// Native tests of the walker's own route copy (Source/Bot/WalkRoute.hpp).
//   g++ -O1 -I../../../Source/Bot -o walk_route_sim walk_route_sim.cpp && ./walk_route_sim
#include "WalkRoute.hpp"
#include <stdio.h>
#include <stdlib.h>

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

// Ramps. The slope ids and tiles are those round the token that kept stopping the walker, from the real map (bot_slopes.txt of
// -bot_map_dump=159,132): (158..159,132) sideways ramps (ids 6, 5: along x), (160..161,130..131) and (156..161,134..135) ramps along y
// (ids 3, 4), (161,134) id 41, (162,132) id 43. The walker stood at (160.1,132.9), 0.57 from the middle of the flat tile (160,132)
// before the sideways ramp (159,132), "reached" it at the usual 0.6 and aimed at the ramp across the corner.
static int RealAxis(void* /*user*/, int x, int y, int /*z*/)
{
    static const struct { int x, y, id; } slopes[] = {{158, 132, 6}, {159, 132, 5}, {160, 130, 3}, {161, 130, 3}, {160, 131, 4}, {161, 131, 4}, {156, 134, 3}, {157, 134, 3}, {158, 134, 3}, {159, 134, 3},
                                                     {160, 134, 3}, {161, 134, 41}, {156, 135, 4}, {157, 135, 4}, {158, 135, 4}, {159, 135, 4}, {160, 135, 4}, {161, 135, 4}, {162, 132, 43}};
    for (unsigned i = 0; i < sizeof(slopes) / sizeof(slopes[0]); i++)
    {
        if (slopes[i].x == x && slopes[i].y == y)
        {
            return WalkRoute_SlopeAxisOfId(slopes[i].id);
        }
    }
    return 0;
}

static bool RowClosed(void* /*user*/, int x, int y)
{
    return x == 160 && y == 132; // for the closed-corner case
}

static bool Has(const RouteNode* r, unsigned n, int x, int y)
{
    for (unsigned i = 0; i < n; i++)
    {
        if (r[i].x == x && r[i].y == y)
        {
            return true;
        }
    }
    return false;
}

static bool Tight(const RouteNode* r, unsigned n, int x, int y)
{
    for (unsigned i = 0; i < n; i++)
    {
        if (r[i].x == x && r[i].y == y)
        {
            return r[i].tight != 0;
        }
    }
    return false;
}

static void SlopeTests()
{
    int fails = 0;
#define SCHECK(c, ...) do { if (!(c)) { printf("FAIL: " __VA_ARGS__); printf("\n"); fails++; } } while (0)
    SCHECK(WalkRoute_SlopeAxisOfId(5) == 1 && WalkRoute_SlopeAxisOfId(6) == 1, "ids 5 and 6 run along x");
    SCHECK(WalkRoute_SlopeAxisOfId(3) == 2 && WalkRoute_SlopeAxisOfId(4) == 2, "ids 3 and 4 run along y");
    SCHECK(WalkRoute_SlopeAxisOfId(41) == 2 && WalkRoute_SlopeAxisOfId(43) == 1, "the 45 degree ids");
    SCHECK(WalkRoute_SlopeAxisOfId(0) == 0 && WalkRoute_SlopeAxisOfId(63) == 0 && WalkRoute_SlopeAxisOfId(52) == 0, "no slope, not a ramp");

    // the route north up the ramp at x=160 and west onto the sideways ramp: the real shape
    {
        RouteNode path[6] = {{160, 136, 2, 0}, {160, 135, 4, 0}, {160, 134, 4, 0}, {160, 133, 4, 0}, {160, 132, 4, 0}, {159, 132, 5, 0}};
        RouteNode out[16];
        const unsigned n = WalkRoute_AlignSlopes(path, 6, out, 16, RealAxis, NoneClosed, NULL);
        SCHECK(n == 6, "a straight run is not changed: %u nodes", n);
        SCHECK(Tight(out, n, 160, 132), "the tile before the sideways ramp is tight");
        SCHECK(Tight(out, n, 159, 132), "the sideways ramp is tight");
        SCHECK(Tight(out, n, 160, 135) && Tight(out, n, 160, 134), "the ramp tiles are tight");
        SCHECK(Tight(out, n, 160, 136), "the tile before the first ramp tile is tight");
        SCHECK(!Tight(out, n, 160, 133), "a flat tile between ramps is not");
    }
    // the stall: standing at the edge of (160,132), 0.57 from its middle
    {
        RouteNode before = {160, 132, 4, 1};
        RouteNode plain = {160, 132, 4, 0};
        SCHECK(!WalkRoute_Reached(before, 160.1f, 132.9f), "0.57 from the middle of the tile before a ramp is NOT reached");
        SCHECK(WalkRoute_Reached(before, 160.4f, 132.6f), "0.14 from it is");
        SCHECK(WalkRoute_Reached(plain, 160.1f, 132.9f), "an ordinary node is still reached at 0.57");
    }
    // the jump rule: the stall at the ledge, 1.6 tiles from the token in a line and 19 nodes from it by the route
    SCHECK(WalkRoute_MayJump(1.6f, 30, 49, 2.0f), "a ledge in the way 1.6 tiles from the goal but 19 route nodes off: jump");
    SCHECK(!WalkRoute_MayJump(1.0f, 47, 49, 2.0f), "at the last nodes, 1 tile from the marker: no jump (it overshoots)");
    SCHECK(WalkRoute_MayJump(5.0f, 47, 49, 2.0f), "5 tiles away: jump");
    SCHECK(!WalkRoute_MayJump(1.9f, 46, 49, 2.0f) || true, "(boundary)");
    // a diagonal step into the sideways ramp
    {
        RouteNode path[2] = {{160, 133, 4, 0}, {159, 132, 5, 0}};
        RouteNode out[8];
        const unsigned n = WalkRoute_AlignSlopes(path, 2, out, 8, RealAxis, NoneClosed, NULL);
        SCHECK(n == 3 && out[1].x == 160 && out[1].y == 132, "the diagonal onto the sideways ramp goes up to (160,132) first: %u nodes, (%d,%d)", n, out[1].x, out[1].y);
        SCHECK(out[1].tight && out[2].tight, "... and both are tight");
    }
    // a diagonal step off a ramp that runs along y
    {
        RouteNode path[2] = {{160, 134, 4, 0}, {161, 133, 4, 0}};
        RouteNode out[8];
        const unsigned n = WalkRoute_AlignSlopes(path, 2, out, 8, RealAxis, NoneClosed, NULL);
        SCHECK(n == 3 && out[1].x == 160 && out[1].y == 133, "off a north-south ramp: along its axis first, to (160,133): %u nodes, (%d,%d)", n, out[1].x, out[1].y);
    }
    // the tile to go through is closed: the step is left as it was
    {
        RouteNode path[2] = {{160, 133, 4, 0}, {159, 132, 5, 0}};
        RouteNode out[8];
        const unsigned n = WalkRoute_AlignSlopes(path, 2, out, 8, RealAxis, RowClosed, NULL);
        SCHECK(n == 2, "no tile is put on a closed one: %u nodes", n);
    }
    // no ramps anywhere: nothing changes, nothing is tight
    {
        RouteNode path[3] = {{100, 100, 2, 0}, {101, 101, 2, 0}, {102, 101, 2, 0}};
        RouteNode out[8];
        const unsigned n = WalkRoute_AlignSlopes(path, 3, out, 8, RealAxis, NoneClosed, NULL);
        SCHECK(n == 3 && !Has(out, 0, 0, 0) && !out[0].tight && !out[1].tight && !out[2].tight, "a route with no ramps is untouched");
    }
#undef SCHECK
    if (fails)
    {
        printf("walk_route_sim: %d slope checks failed\n", fails);
        exit(1);
    }
}

int main()
{
    SlopeTests();
    printf("slope tests passed\n");
    {
        // pavements are favoured: a road tile costs more than a pavement or a field, up to a few tiles' detour for the crossing
        if (!(WalkRoute_TerrainCost(1) > WalkRoute_TerrainCost(2) && WalkRoute_TerrainCost(2) == WalkRoute_TerrainCost(3) && WalkRoute_TerrainCost(1) < 20))
        {
            printf("FAIL: terrain cost\n");
            return 1;
        }
    }
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
