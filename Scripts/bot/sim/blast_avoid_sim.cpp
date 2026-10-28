// Native tests of staying clear of burning cars (Source/Bot/BlastAvoid.cpp).
//   g++ -O1 -I../../../Source/Bot -o blast_avoid_sim blast_avoid_sim.cpp ../../../Source/Bot/BlastAvoid.cpp && ./blast_avoid_sim
#include "BlastAvoid.hpp"
#include <math.h>
#include <stdio.h>

static int fails = 0;
static void Check(const char* name, bool ok)
{
    printf("%-60s %s\n", name, ok ? "PASS" : "FAIL");
    fails += ok ? 0 : 1;
}
static bool NoWalls(void*, float, float)
{
    return false;
}
static bool WallEast(void*, float x, float)
{
    return x > 12.0f;
}
static float Dist(float ax, float ay, float bx, float by)
{
    return sqrtf((ax - bx) * (ax - bx) + (ay - by) * (ay - by));
}

int main()
{
    const BlastHazard hot[1] = {{10, 10, 30000}};
    const BlastHazard warm[1] = {{10, 10, 20000}};
    const BlastHazard gone[1] = {{10, 10, 32001}};
    {
        BlastWorld w = {10.5f, 10, hot, 1, NoWalls, 0};
        BlastOut o = BlastAvoid_Decide(w);
        Check("next to a hot car: step out", o.active && Dist(o.aim_x, o.aim_y, 10, 10) >= kBlastStandOff);
    }
    {
        BlastWorld w = {13, 10, hot, 1, NoWalls, 0};
        Check("outside the stand-off: stay and keep shooting", !BlastAvoid_Decide(w).active);
    }
    {
        BlastWorld w = {10.5f, 10, warm, 1, NoWalls, 0};
        Check("a car only burning (20000) is not a hazard yet", !BlastAvoid_Decide(w).active);
    }
    {
        BlastWorld w = {10.5f, 10, gone, 1, NoWalls, 0};
        Check("a wreck (32001) is not a hazard", !BlastAvoid_Decide(w).active);
    }
    {
        BlastWorld w = {11, 10, hot, 1, WallEast, 0};
        BlastOut o = BlastAvoid_Decide(w);
        Check("a wall on one side: the way out is clear of it", o.active && o.aim_x <= 12.0f && Dist(o.aim_x, o.aim_y, 10, 10) >= kBlastStandOff);
    }
    {
        const BlastHazard two[2] = {{10, 10, 30000}, {13, 10, 30000}};
        BlastWorld w = {11.5f, 10, two, 2, NoWalls, 0};
        BlastOut o = BlastAvoid_Decide(w);
        Check("between two hot cars: out of both, not through one",
              o.active && Dist(o.aim_x, o.aim_y, 10, 10) >= kBlastStandOff && Dist(o.aim_x, o.aim_y, 13, 10) >= kBlastStandOff);
    }
    {
        BlastWorld w = {20, 20, hot, 1, NoWalls, 0};
        Check("a car beside a hot one will chain", BlastAvoid_WillChain(w, 10.8f, 10) && !BlastAvoid_WillChain(w, 12.5f, 10));
    }
    printf("%s\n", fails ? "FAILED" : "all passed");
    return fails ? 1 : 0;
}
