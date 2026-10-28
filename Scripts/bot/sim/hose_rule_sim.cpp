// Native test of HoseRule_Decide: when the fire truck drives and when it fires its hose
#include "HoseRule.hpp"
#include <stdio.h>

static int fails = 0;
static void Check(const char* name, bool ok)
{
    printf("%-70s %s\n", name, ok ? "PASS" : "FAIL");
    fails += ok ? 0 : 1;
}

int main()
{
    HoseDecision d = HoseRule_Decide(20.0f, 3.0f);
    Check("far off, nose on it: drives up, no jet", d.forward && !d.attack && !d.backward);
    d = HoseRule_Decide(6.0f, -5.0f);
    Check("6 tiles, nose on it: the jet fires and the truck closes", d.attack && d.forward);
    d = HoseRule_Decide(6.0f, 40.0f);
    Check("6 tiles, 40 degrees off: no jet (it would miss), drives on to turn", !d.attack && d.forward);
    d = HoseRule_Decide(2.5f, 2.0f);
    Check("2.5 tiles, nose on it: fires, stands off", d.attack && !d.forward && !d.backward);
    d = HoseRule_Decide(1.0f, 60.0f);
    Check("1 tile, far off the nose: backs off to swing round", d.backward && !d.forward && !d.attack);
    d = HoseRule_Decide(9.0f, 0.0f);
    Check("9 tiles, nose on it: out of reach, no jet", !d.attack && d.forward);
    printf("%d failed\n", fails);
    return fails ? 1 : 0;
}
