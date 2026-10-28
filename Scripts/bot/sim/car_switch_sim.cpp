// Native tests of when the goal drops the car it is walking to for a closer one (Source/Bot/CarSwitch.cpp).
//   g++ -O1 -I../../../Source/Bot -o car_switch_sim car_switch_sim.cpp ../../../Source/Bot/CarSwitch.cpp && ./car_switch_sim
#include "CarSwitch.hpp"
#include <stdio.h>

static int fails = 0;
static void Check(const char* name, bool ok)
{
    printf("%-64s %s\n", name, ok ? "PASS" : "FAIL");
    fails += ok ? 0 : 1;
}

int main()
{
    Check("74 tiles to the van, an empty car 2 tiles away: switch", CarSwitch_ShouldSwitch(74.0f, 2.0f, false, 0));
    Check("74 tiles to the van, a car with a driver 2 tiles away: switch (long walk saved)", CarSwitch_ShouldSwitch(74.0f, 2.0f, true, 0));
    Check("30 tiles to the car, one with a driver 2 away: stay (hijack not worth it)", !CarSwitch_ShouldSwitch(30.0f, 2.0f, true, 0));
    Check("30 tiles to the car, an empty one 2 away: switch", CarSwitch_ShouldSwitch(30.0f, 2.0f, false, 0));
    Check("30 tiles, the other is 25 away: stay (barely nearer)", !CarSwitch_ShouldSwitch(30.0f, 25.0f, false, 0));
    Check("10 tiles to the car: nearly there, stay", !CarSwitch_ShouldSwitch(10.0f, 1.0f, false, 0));
    Check("already switched once: never again (no ping-pong)", !CarSwitch_ShouldSwitch(74.0f, 2.0f, false, 1));
    printf("%d failed\n", fails);
    return fails ? 1 : 0;
}
