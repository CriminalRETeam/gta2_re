// Native test of JamSwitch: when a jammed car is swapped for the one ahead, and when it is not.
#include "JamSwitch.hpp"
#include <stdio.h>

static int fails = 0;
static void Check(const char* name, bool ok)
{
    printf("%-72s %s\n", name, ok ? "PASS" : "FAIL");
    if (!ok)
    {
        fails++;
    }
}

static JamCar Car(float along, float side, int damage, float top, bool driver)
{
    JamCar c;
    c.along = along;
    c.side = side;
    c.damage = damage;
    c.top_speed = top;
    c.has_driver = driver;
    c.locked = false;
    c.mission = false;
    c.police = false;
    return c;
}

int main()
{
    const JamSelf self = {12000, 100.0f, 200};
    JamCar a[4];

    a[0] = Car(2.0f, 0.5f, 3000, 130.0f, true);
    Check("a faster, much less damaged car 2 tiles ahead is taken", JamSwitch_Choose(self, a, 1) == 0);

    JamSelf brief = self;
    brief.still_frames = 40;
    Check("not after only 40 frames of standing", JamSwitch_Choose(brief, a, 1) == -1);

    a[0] = Car(2.0f, 0.5f, 3000, 90.0f, true);
    Check("a slower car is not worth it", JamSwitch_Choose(self, a, 1) == -1);

    a[0] = Car(2.0f, 0.5f, 11000, 130.0f, true);
    Check("a car that is hardly less damaged is not worth it", JamSwitch_Choose(self, a, 1) == -1);

    a[0] = Car(-2.0f, 0.5f, 3000, 130.0f, true);
    Check("a car behind is not the one ahead", JamSwitch_Choose(self, a, 1) == -1);

    a[0] = Car(2.0f, 6.0f, 3000, 130.0f, true);
    Check("a car well to the side is not taken", JamSwitch_Choose(self, a, 1) == -1);

    a[0] = Car(2.0f, 0.5f, 3000, 130.0f, true);
    a[0].police = true;
    Check("a police car is never taken", JamSwitch_Choose(self, a, 1) == -1);

    a[0] = Car(2.0f, 0.5f, 3000, 130.0f, true);
    a[0].locked = true;
    Check("a locked car is never taken", JamSwitch_Choose(self, a, 1) == -1);

    a[0] = Car(2.0f, 0.5f, 3000, 130.0f, true);
    a[0].mission = true;
    Check("a mission car is never taken", JamSwitch_Choose(self, a, 1) == -1);

    a[0] = Car(1.5f, 0.0f, 3000, 130.0f, true);
    a[1] = Car(3.5f, 0.5f, 3000, 130.0f, false);
    Check("an empty car a little further beats one with a driver in it", JamSwitch_Choose(self, a, 2) == 1);

    a[0] = Car(5.0f, 0.0f, 3000, 130.0f, false);
    a[1] = Car(2.0f, 0.0f, 3000, 130.0f, false);
    Check("of two empty cars the nearer", JamSwitch_Choose(self, a, 2) == 1);

    JamSelf wreck = {30000, 100.0f, 200};
    a[0] = Car(2.0f, 0.0f, 20000, 105.0f, true);
    Check("a burning wreck is swapped for any better car that is a bit faster", JamSwitch_Choose(wreck, a, 1) == 0);

    if (fails == 0)
    {
        printf("all passed\n");
    }
    return fails ? 1 : 0;
}
