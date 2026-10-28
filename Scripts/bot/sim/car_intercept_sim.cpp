// Native test of CarIntercept: where to stand to take a moving car.
#include "CarIntercept.hpp"
#include <stdio.h>
#include <math.h>

static int fails = 0;
static void Check(const char* name, bool ok)
{
    printf("%-72s %s\n", name, ok ? "PASS" : "FAIL");
    if (!ok)
    {
        fails++;
    }
}

int main()
{
    float x = 0, y = 0;
    InterceptIn in = {100.0f, 100.0f, 0.0f, 0.0f, 104.0f, 100.0f, 0.06f};
    Check("a car that stands: walk up to it", CarIntercept_Plan(in, &x, &y) == kInterceptStopped);

    // the car drives east at 0.1 tiles a frame; the ped is on the pavement beside the road, 3 tiles off the line
    in.car_vx = 0.1f;
    in.car_vy = 0.0f;
    in.ped_x = 110.0f;
    in.ped_y = 103.0f;
    int r = CarIntercept_Plan(in, &x, &y);
    Check("a car driving by: stand on its line ahead of it", r == kInterceptAhead && fabsf(y - 100.0f) < 0.01f && x > 103.0f);
    const float ped_frames = sqrtf((x - in.ped_x) * (x - in.ped_x) + (y - in.ped_y) * (y - in.ped_y)) / in.ped_speed;
    const float car_frames = (x - in.car_x) / in.car_vx;
    Check("... a spot the walker reaches before the car does", ped_frames < car_frames);

    // a car driving north: the spot is north of it
    in.car_vx = 0.0f;
    in.car_vy = -0.1f;
    in.ped_x = 103.0f;
    in.ped_y = 90.0f;
    r = CarIntercept_Plan(in, &x, &y);
    Check("a car driving north: the spot is north of it, on its line", r == kInterceptAhead && y < 97.0f && fabsf(x - 100.0f) < 0.01f);

    // a fast car and a walker far away on the wrong side: nothing to do
    in.car_vx = 0.25f;
    in.car_vy = 0.0f;
    in.ped_x = 100.0f;
    in.ped_y = 140.0f;
    r = CarIntercept_Plan(in, &x, &y);
    Check("a fast car and a walker far off: it would be past before we got there", r == kInterceptNever);

    // a slow queue creeping (just over the stopped speed): close spot is enough
    in.car_vx = 0.04f;
    in.car_vy = 0.0f;
    in.ped_x = 106.0f;
    in.ped_y = 100.0f;
    r = CarIntercept_Plan(in, &x, &y);
    Check("a creeping car: a spot near the front is found", r == kInterceptAhead && x - in.car_x <= 8.0f);

    if (fails == 0)
    {
        printf("all passed\n");
    }
    return fails ? 1 : 0;
}
