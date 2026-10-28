// Native test of FrenzyRange: when a weapon fires at a frenzy target.
#include "FrenzyRange.hpp"
#include <stdio.h>

static int fails = 0;
static void Check(const char* name, bool ok)
{
    printf("%-78s %s\n", name, ok ? "PASS" : "FAIL");
    if (!ok)
    {
        fails++;
    }
}

int main()
{
    const FrenzyRange flame = FrenzyRange_Of(8, false, 0);
    Check("flamethrower: a pedestrian 2 tiles off, in line: fire", FrenzyRange_FireOk(flame, 3.0f, 2.0f, true));
    Check("flamethrower: 4 tiles off: NO (the tail does not reach, he only runs)", !FrenzyRange_FireOk(flame, 3.0f, 4.0f, true));
    Check("flamethrower: 3.4 tiles off: no", !FrenzyRange_FireOk(flame, 3.0f, 3.4f, true));
    Check("flamethrower: 2 tiles off but 25 degrees to the side: no", !FrenzyRange_FireOk(flame, 25.0f, 2.0f, true));
    Check("flamethrower: the walk-up distance is inside the tail", flame.max_d < 2.6f);
    Check("flamethrower: nothing clear to shoot: no", !FrenzyRange_FireOk(flame, 3.0f, 2.0f, false));

    const FrenzyRange smg = FrenzyRange_Of(1, false, 0);
    Check("smg: 8 tiles off, in line: fire (unchanged)", FrenzyRange_FireOk(smg, 10.0f, 8.0f, true));
    Check("smg: 11 tiles off: no", !FrenzyRange_FireOk(smg, 10.0f, 11.0f, true));

    const FrenzyRange rocket = FrenzyRange_Of(2, false, 0);
    Check("rocket: needs room: min 5, max 11 (unchanged)", rocket.min_d == 5.0f && rocket.max_d == 11.0f);
    const FrenzyRange rocket_car = FrenzyRange_Of(2, true, 1);
    Check("rocket at cars, second try: 8 to 14 (unchanged)", rocket_car.min_d == 8.0f && rocket_car.max_d == 14.0f);
    const FrenzyRange grenade = FrenzyRange_Of(5, false, 0);
    Check("grenade: min 3, max 9 (unchanged)", grenade.min_d == 3.0f && grenade.max_d == 9.0f);
    const FrenzyRange shocker = FrenzyRange_Of(3, false, 0);
    Check("shocker: right up to the target", FrenzyRange_FireOk(shocker, 5.0f, 1.2f, true) && !FrenzyRange_FireOk(shocker, 5.0f, 4.0f, true));

    if (fails == 0)
    {
        printf("all passed\n");
    }
    return fails ? 1 : 0;
}
