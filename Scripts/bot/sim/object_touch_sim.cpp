// Native tests of aiming at an object's collision box (Source/Bot/ObjectTouch.cpp).
//   g++ -O1 -I../../../Source/Bot -o object_touch_sim object_touch_sim.cpp ../../../Source/Bot/ObjectTouch.cpp && ./object_touch_sim
#include "ObjectTouch.hpp"
#include <math.h>
#include <stdio.h>

static int fails = 0;
static void Check(const char* name, bool ok)
{
    printf("%-62s %s\n", name, ok ? "PASS" : "FAIL");
    fails += ok ? 0 : 1;
}

int main()
{
    // a phone: a box 0.5 x 0.4 whose middle is not the object's spot (the sprite origin is off the middle)
    const TouchBox phone = {true, 159.3f, 139.2f, 1.0f, 0.0f, 0.25f, 0.2f};
    const TouchAim a = ObjectTouch_Aim(phone, 159.0f, 139.0f);
    Check("aims at the middle of the box, not at the object's spot", fabsf(a.x - 159.3f) < 0.001f && fabsf(a.y - 139.2f) < 0.001f);
    Check("the reach is inside the box", a.reach >= kTouchMinReach && a.reach <= 0.2f * 0.5f + 0.001f);
    Check("standing at the aim is on it", ObjectTouch_Inside(phone, 159.0f, 139.0f, a.x, a.y));
    Check("just inside the edge of the box is not 'on' it", !ObjectTouch_Inside(phone, 159.0f, 139.0f, 159.3f + 0.24f, 139.2f));
    Check("outside the box is not on it", !ObjectTouch_Inside(phone, 159.0f, 139.0f, 158.5f, 138.7f));
    // a box turned 90 degrees: long side along y
    const TouchBox turned = {true, 10.0f, 20.0f, 0.0f, 1.0f, 0.5f, 0.1f};
    Check("a turned box: inside along its long side", ObjectTouch_Inside(turned, 10, 20, 10.0f, 20.2f));
    Check("a turned box: not inside across its narrow side", !ObjectTouch_Inside(turned, 10, 20, 10.09f, 20.0f));
    // no box: the object's spot, with the fixed reach
    const TouchBox none = {false, 0, 0, 1, 0, 0, 0};
    const TouchAim n = ObjectTouch_Aim(none, 50.5f, 60.5f);
    Check("no box: aims at the object's spot", n.x == 50.5f && n.y == 60.5f && n.reach == kTouchNoBoxReach);
    Check("no box: inside within the reach only", ObjectTouch_Inside(none, 50.5f, 60.5f, 50.6f, 60.5f) && !ObjectTouch_Inside(none, 50.5f, 60.5f, 51.0f, 60.5f));
    // a big box does not make the walk stop far from the middle
    const TouchBox big = {true, 5.0f, 5.0f, 1.0f, 0.0f, 1.5f, 1.5f};
    Check("a big box: the reach is capped", ObjectTouch_Aim(big, 5, 5).reach <= kTouchMaxReach + 0.001f);
    printf("%s\n", fails ? "FAILED" : "all passed");
    return fails ? 1 : 0;
}
