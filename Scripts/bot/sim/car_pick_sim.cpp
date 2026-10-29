// Native test of CarPickRule: which cars the bot will walk to and take, and how. Each case is a thing that went wrong once: moving
// traffic refused outright (a road full of cars and a trip on foot), a driver in a car at a light, police about, wrecks, trains.
#include "CarPickRule.hpp"
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

static CarPickCar Car(float x, float y, float vx, float vy, bool driver)
{
    CarPickCar c;
    c.x = x;
    c.y = y;
    c.vx = vx;
    c.vy = vy;
    c.speed = (vx < 0 ? -vx : vx) + (vy < 0 ? -vy : vy);
    c.dist = 0.0f;
    c.damage = 0;
    c.has_driver = driver;
    c.locked = c.police = c.mission = c.bad_model = c.ours = false;
    return c;
}

static int Kind(const CarPickPlayer& p, CarPickCar c, float max_dist = 30.0f)
{
    float sx, sy;
    const float dx = c.x - p.x;
    const float dy = c.y - p.y;
    c.dist = dx * dx + dy * dy > 0 ? __builtin_sqrtf(dx * dx + dy * dy) : 0.0f;
    return CarPick_Kind(p, c, max_dist, &sx, &sy);
}

int main()
{
    const CarPickPlayer ped = {150.0f, 153.0f, 0, false};

    Check("an empty car near: parked", Kind(ped, Car(155.0f, 150.0f, 0, 0, false)) == kCarPickParked);
    Check("... too far for this trip: no", Kind(ped, Car(155.0f, 150.0f, 0, 0, false), 4.0f) == kCarPickNo);
    {
        CarPickCar c = Car(155.0f, 150.0f, 0, 0, false);
        c.locked = true;
        Check("a locked car: no", Kind(ped, c) == kCarPickNo);
        c = Car(155.0f, 150.0f, 0, 0, false);
        c.police = true;
        Check("a police car: no", Kind(ped, c) == kCarPickNo);
        c = Car(155.0f, 150.0f, 0, 0, false);
        c.mission = true;
        Check("a mission car: no", Kind(ped, c) == kCarPickNo);
        c.ours = true;
        Check("... but the mission car we drove here ourselves: yes, it is the way on", Kind(ped, c) == kCarPickParked);
        c = Car(155.0f, 150.0f, 0, 0, false);
        c.damage = 9000;
        Check("a wreck: no", Kind(ped, c) == kCarPickNo);
        c = Car(155.0f, 150.0f, 0, 0, false);
        c.bad_model = true;
        Check("a train, a tank: no", Kind(ped, c) == kCarPickNo);
    }

    // the first road: the bot on the pavement 3 tiles off a road with cars driving along it at 0.12 tiles a frame
    Check("THE FIRST ROAD: a car driving by at 0.12, 14 tiles up the road: taken by getting ahead of it", Kind(ped, Car(138.0f, 150.0f, 0.12f, 0, true)) == kCarPickIntercept);
    Check("... one coming the other way: the same", Kind(ped, Car(164.0f, 150.0f, -0.12f, 0, true)) == kCarPickIntercept);
    Check("... a slow one at 0.06: yes, there is even more time", Kind(ped, Car(145.0f, 150.0f, 0.06f, 0, true)) == kCarPickIntercept);
    Check("a car about to pass us at 0.2, a few tiles off: too fast to get ahead of", Kind(ped, Car(147.0f, 150.0f, 0.2f, 0, true)) == kCarPickNo);

    Check("a driver in a car at a light, right beside us: stopped", Kind(ped, Car(152.0f, 150.0f, 0, 0, true)) == kCarPickStopped);
    Check("... at a light 12 tiles off: it drives off before we are there", Kind(ped, Car(162.0f, 150.0f, 0, 0, true)) == kCarPickNo);

    const CarPickPlayer ped_cops = {150.0f, 153.0f, 0, true};
    Check("police about, no wanted level: no hijack (an arrest)", Kind(ped_cops, Car(152.0f, 150.0f, 0, 0, true)) == kCarPickNo);
    Check("police about, no wanted level: no taking a car off the road either", Kind(ped_cops, Car(138.0f, 150.0f, 0.12f, 0, true)) == kCarPickNo);
    const CarPickPlayer wanted = {150.0f, 153.0f, 2, true};
    Check("wanted, police about: a car is the way out, a hijack is fine", Kind(wanted, Car(152.0f, 150.0f, 0, 0, true)) == kCarPickStopped);

    // the walk, once the route is known
    const CarPickCar parked = Car(155.0f, 150.0f, 0, 0, false);
    Check("a parked car is worth any walk the caller allows", CarPick_WalkOk(kCarPickParked, parked, 40.0f, 0.0f));
    CarPickCar stopped = Car(152.0f, 150.0f, 0, 0, true);
    Check("a stopped car 5 tiles of walk away: yes", CarPick_WalkOk(kCarPickStopped, stopped, 5.0f, 0.0f));
    Check("... 12 tiles of walk round a fence: no", !CarPick_WalkOk(kCarPickStopped, stopped, 12.0f, 0.0f));
    CarPickCar moving = Car(138.0f, 150.0f, 0.12f, 0, true);
    Check("moving car: 5 tiles of walk to the spot, the car 14 tiles from it: there first", CarPick_WalkOk(kCarPickIntercept, moving, 5.0f, 14.0f));
    Check("moving car: 8 tiles of walk (133 frames) to a spot the car is at in 116: it gets there first", !CarPick_WalkOk(kCarPickIntercept, moving, 8.0f, 14.0f));
    Check("moving car: 20 tiles of walk to the spot, the car 6 from it: it gets there first", !CarPick_WalkOk(kCarPickIntercept, moving, 20.0f, 6.0f));
    Check("moving car: the walk round a block is 50 tiles: no", !CarPick_WalkOk(kCarPickIntercept, moving, 50.0f, 200.0f));

    if (fails == 0)
    {
        printf("all passed\n");
    }
    return fails ? 1 : 0;
}
