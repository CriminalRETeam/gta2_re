// Native tests of when a walker hops over a car (Source/Bot/WalkCarJump.cpp), and a walk along a street through queuing
// traffic with a fake walker that is stopped by a car in front of him until he jumps.
//   g++ -O1 -I../../../Source/Bot -o walk_car_jump_sim walk_car_jump_sim.cpp ../../../Source/Bot/WalkCarJump.cpp && ./walk_car_jump_sim
#include "WalkCarJump.hpp"
#include <math.h>
#include <stdio.h>
#include <vector>

static int fails = 0;
static void Check(const char* name, bool ok)
{
    printf("%-58s %s\n", name, ok ? "PASS" : "FAIL");
    fails += ok ? 0 : 1;
}

struct Sim
{
    float x;
    int frames;
    int jumps;
    bool arrived;
};

// A street to the east, cars stopped or creeping across his path every 12 tiles. A car in front (within 0.9) stops him
// unless he is in the air (the first 20 frames after a jump start)
static Sim Walk(float car_speed)
{
    Sim s = {2.0f, 0, 0, false};
    std::vector<JumpCar> cars;
    for (int i = 0; i < 4; i++)
    {
        JumpCar c = {14.0f + 12.0f * i, 0.0f, car_speed};
        cars.push_back(c);
    }
    int air_until = -1;
    int next = 0;
    for (int f = 0; f < 3000; f++)
    {
        if (s.x >= 60.0f)
        {
            s.arrived = true;
            s.frames = f;
            return s;
        }
        for (size_t i = 0; i < cars.size(); i++)
        {
            cars[i].x += cars[i].speed * 0.0f; // cars creep sideways in the real world; here they stay in the way
        }
        if (f >= next && WalkCarJump_Decide(s.x, 0.0f, 1.0f, 0.0f, &cars[0], static_cast<int>(cars.size())))
        {
            air_until = f + 20;
            next = f + 45;
            s.jumps++;
        }
        bool blocked = false;
        for (size_t i = 0; i < cars.size(); i++)
        {
            if (cars[i].x - s.x > 0.0f && cars[i].x - s.x < 0.9f && f >= air_until)
            {
                blocked = true;
            }
        }
        if (!blocked)
        {
            s.x += 0.07f;
        }
    }
    s.frames = 3000;
    return s;
}

int main()
{
    const JumpCar ahead_stopped[1] = {{11.0f, 0.0f, 0.0f}};
    const JumpCar ahead_creeping[1] = {{11.0f, 0.0f, 0.07f}};
    const JumpCar ahead_fast[1] = {{11.0f, 0.0f, 0.2f}};
    const JumpCar beside[1] = {{11.0f, 3.0f, 0.0f}};
    const JumpCar behind[1] = {{9.0f, 0.0f, 0.0f}};
    const JumpCar far_ahead[1] = {{14.0f, 0.0f, 0.0f}};
    Check("stopped car 1 tile ahead: jump", WalkCarJump_Decide(10.0f, 0.0f, 1.0f, 0.0f, ahead_stopped, 1));
    Check("car creeping in a queue 1 tile ahead: jump", WalkCarJump_Decide(10.0f, 0.0f, 1.0f, 0.0f, ahead_creeping, 1));
    Check("fast car coming: not a hop (a step aside instead)", !WalkCarJump_Decide(10.0f, 0.0f, 1.0f, 0.0f, ahead_fast, 1));
    Check("car beside the path: no jump", !WalkCarJump_Decide(10.0f, 0.0f, 1.0f, 0.0f, beside, 1));
    Check("car behind: no jump", !WalkCarJump_Decide(10.0f, 0.0f, 1.0f, 0.0f, behind, 1));
    Check("car far ahead: not yet", !WalkCarJump_Decide(10.0f, 0.0f, 1.0f, 0.0f, far_ahead, 1));
    Check("on top of a car mid-hop: press enter", WalkCarJump_StealNow(8, 0.4f, false, false));
    Check("not before the hop reaches the car", !WalkCarJump_StealNow(2, 0.4f, false, false));
    Check("not after the landing", !WalkCarJump_StealNow(18, 0.4f, false, false));
    Check("every other frame only (a key held is not pressed again)", !WalkCarJump_StealNow(7, 0.4f, false, false));
    Check("never a police car", !WalkCarJump_StealNow(8, 0.4f, true, false));
    Check("never a locked car", !WalkCarJump_StealNow(8, 0.4f, false, true));
    Check("no car under him: nothing to press for", !WalkCarJump_StealNow(8, 2.5f, false, false));
    {
        const Sim s = Walk(0.0f);
        printf("   stopped cars: %d frames, %d jumps\n", s.frames, s.jumps);
        Check("street of stopped cars: gets across", s.arrived && s.frames < 1100);
    }
    {
        const Sim s = Walk(0.07f);
        printf("   creeping cars: %d frames, %d jumps\n", s.frames, s.jumps);
        Check("street of creeping cars: gets across", s.arrived && s.frames < 1100);
    }
    printf("%d failed\n", fails);
    return fails ? 1 : 0;
}
