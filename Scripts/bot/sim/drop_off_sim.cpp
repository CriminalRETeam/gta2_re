// Native tests of where to leave the car (Source/Bot/DropOff.cpp).
//   g++ -O1 -I../../../Source/Bot -o drop_off_sim drop_off_sim.cpp ../../../Source/Bot/DropOff.cpp && ./drop_off_sim
#include "DropOff.hpp"
#include <stdio.h>

static int fails = 0;
static void Check(const char* name, bool ok)
{
    printf("%-66s %s\n", name, ok ? "PASS" : "FAIL");
    fails += ok ? 0 : 1;
}

int main()
{
    // the phone behind a block (the case that was seen): the road end 19 tiles off is 63 tiles of walking; a road further round is 15
    {
        const DropOffCand c[2] = {{97.5f, 226.5f, 80.0f, 63.0f}, {117.0f, 234.0f, 130.0f, 15.0f}};
        Check("a farther road point with a short walk beats the nearest with a long one", DropOff_Choose(c, 2) == 1);
    }
    {
        const DropOffCand c[2] = {{10.0f, 10.0f, 60.0f, 8.0f}, {20.0f, 20.0f, 75.0f, 6.0f}};
        Check("two near equals: the first (nearest) stays", DropOff_Choose(c, 2) == 0);
    }
    {
        const DropOffCand c[2] = {{10.0f, 10.0f, 60.0f, 40.0f}, {20.0f, 20.0f, -1.0f, 2.0f}};
        Check("a point the car cannot get to is never chosen", DropOff_Choose(c, 2) == 0);
    }
    {
        const DropOffCand c[2] = {{10.0f, 10.0f, 60.0f, -1.0f}, {20.0f, 20.0f, 70.0f, 12.0f}};
        Check("a point with no way on foot from it is never chosen", DropOff_Choose(c, 2) == 1);
    }
    {
        const DropOffCand c[2] = {{10.0f, 10.0f, -1.0f, 5.0f}, {20.0f, 20.0f, -1.0f, 2.0f}};
        Check("nothing usable: -1", DropOff_Choose(c, 2) == -1);
    }
    {
        const DropOffCand c[3] = {{0.0f, 0.0f, 50.0f, 50.0f}, {5.0f, 5.0f, 100.0f, 20.0f}, {9.0f, 9.0f, 110.0f, 6.0f}};
        Check("of several, the cheapest overall", DropOff_Choose(c, 3) == 2);
    }
    {
        const DropOffCand near_goal = {1.0f, 1.0f, 40.0f, 2.0f};
        const DropOffCand far_goal = {1.0f, 1.0f, 40.0f, 10.0f};
        Check("a walk under 3 tiles costs no getting out", DropOff_Cost(near_goal) < DropOff_Cost(far_goal) - kDropExitFrames + 1.0f);
    }
    printf("%s\n", fails ? "FAILED" : "all passed");
    return fails ? 1 : 0;
}
