// Native tests of which pickups are worth going for (Source/Bot/PickupReach.cpp).
//   g++ -O1 -I../../../Source/Bot -o pickup_reach_sim pickup_reach_sim.cpp ../../../Source/Bot/PickupReach.cpp && ./pickup_reach_sim
#include "PickupReach.hpp"
#include <stdio.h>

static int fails = 0;
static void Check(const char* name, bool ok)
{
    printf("%-62s %s\n", name, ok ? "PASS" : "FAIL");
    fails += ok ? 0 : 1;
}

int main()
{
    //                                flat  found approx len   needed
    Check("a short walk to something near: go", PickupReach_Ok({6.0f, true, false, 8.0f, false}));
    Check("on a roof with no way up: no", !PickupReach_Ok({4.0f, false, false, 0.0f, false}));
    Check("on a roof with no way up, even if needed: no", !PickupReach_Ok({4.0f, false, false, 0.0f, true}));
    Check("the route only reaches near it: no", !PickupReach_Ok({4.0f, true, true, 9.0f, true}));
    Check("up a stair: 12 tiles of walking for 5 tiles off: yes", PickupReach_Ok({5.0f, true, false, 12.0f, false}));
    Check("round the block, 60 tiles for 5 off: not handy enough", !PickupReach_Ok({5.0f, true, false, 60.0f, false}));
    Check("round the block, 60 tiles for 5 off, but needed: yes", PickupReach_Ok({5.0f, true, false, 60.0f, true}));
    Check("across a river, 300 tiles for 20 off, even if needed: no", !PickupReach_Ok({20.0f, true, false, 300.0f, true}));
    Check("a long walk in a straight line (40 for 38): yes", PickupReach_Ok({38.0f, true, false, 40.0f, false}));
    Check("70 tiles for 4 off: no", !PickupReach_Ok({4.0f, true, false, 70.0f, false}));
    printf("%s\n", fails ? "FAILED" : "all passed");
    return fails ? 1 : 0;
}
