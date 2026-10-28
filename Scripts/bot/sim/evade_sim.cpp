// Native test of EvadeRule: which cars are a threat to a pedestrian standing still.
#include "EvadeRule.hpp"
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
    float side = 0, along = 0;
    EvadeCar c[2];

    c[0] = {100.0f, 50.0f, 0.15f, 0.0f};
    Check("a fast car coming at us from 2.5 tiles (17 frames away): a threat", Evade_Threat(102.5f, 50.0f, c, 1, &side, &along) == 0);
    Check("... from 4 tiles (27 frames away): not yet", Evade_Threat(104.0f, 50.0f, c, 1, &side, &along) == -1);

    c[0] = {100.0f, 50.0f, 0.06f, 0.0f};
    Check("A POLICE CAR PUSHING IN AT 0.06, 1.5 tiles away: a threat (was ignored below 0.12)", Evade_Threat(101.5f, 50.0f, c, 1, &side, &along) == 0);

    c[0] = {100.0f, 50.0f, 0.06f, 0.0f};
    Check("... the same car 5 tiles off is not yet", Evade_Threat(105.0f, 50.0f, c, 1, &side, &along) == -1);

    c[0] = {100.0f, 50.0f, 0.02f, 0.0f};
    Check("a car rolling at 0.02: not moving into us", Evade_Threat(101.0f, 50.0f, c, 1, &side, &along) == -1);

    c[0] = {100.0f, 50.0f, 0.15f, 0.0f};
    Check("a car driving away from us: no", Evade_Threat(96.0f, 50.0f, c, 1, &side, &along) == -1);

    c[0] = {100.0f, 50.0f, 0.15f, 0.0f};
    Check("a car going by 2.5 tiles to the side: no", Evade_Threat(104.0f, 52.5f, c, 1, &side, &along) == -1);

    c[0] = {100.0f, 50.0f, 0.15f, 0.0f};
    Evade_Threat(102.5f, 50.4f, c, 1, &side, &along);
    Check("we stand a little to its left: step to that side", side > 0.0f);
    Evade_Threat(102.5f, 49.6f, c, 1, &side, &along);
    Check("... a little to its right: that side", side < 0.0f);

    c[0] = {100.0f, 50.0f, 0.15f, 0.0f};
    c[1] = {101.0f, 50.0f, 0.15f, 0.0f};
    Check("two cars: the nearer one is the threat", Evade_Threat(103.0f, 50.0f, c, 2, &side, &along) == 1);

    if (fails == 0)
    {
        printf("all passed\n");
    }
    return fails ? 1 : 0;
}
