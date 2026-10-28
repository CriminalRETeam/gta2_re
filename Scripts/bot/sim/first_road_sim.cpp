// A mock world for "get a car off the first road": a straight road with cars driving along it, a pedestrian on the pavement, and
// the game's traffic rule (a car brakes for someone standing in its lane ahead, and pulls away again when the way is clear). The
// bot's own decision functions (CarPickRule, CarIntercept) run against it over a table of speeds, distances and directions, so a
// change that stops the bot getting a car off a road full of them fails here, in a second, without the game.
#include "CarPickRule.hpp"
#include "CarIntercept.hpp"
#include <math.h>
#include <stdio.h>

static int fails = 0;

struct MockCar
{
    float x, y;
    float dir_x;      // +1 east, -1 west
    float cruise;     // tiles per frame
    float speed;
    bool has_driver;
};

struct Result
{
    bool got_car;
    int frames;
    bool run_over;
};

// The traffic rule: brakes hard for a pedestrian within 5 tiles ahead in its lane (within 0.8 across), pulls away when clear
static void StepCar(MockCar* c, float ped_x, float ped_y)
{
    const float ahead = (ped_x - c->x) * c->dir_x;
    const float across = fabsf(ped_y - c->y);
    const bool blocked = ahead > 0.0f && ahead < 5.0f && across < 0.8f;
    if (blocked)
    {
        c->speed -= 0.02f;
        if (c->speed < 0.0f)
        {
            c->speed = 0.0f;
        }
    }
    else if (c->speed < c->cruise)
    {
        c->speed += 0.004f;
    }
    c->x += c->dir_x * c->speed;
}

// The bot: every 15 frames it picks the best car (CarPick_Kind / CarPick_WalkOk, straight-line walks standing in for the router),
// walks to the stand spot, stops there facing the car and waits, then walks up to a stopped car and gets in
static Result Play(MockCar* cars, int n, float ped_x, float ped_y, int max_frames)
{
    Result r = {false, 0, false};
    int target = -1;
    float stand_x = 0, stand_y = 0;
    int kind = kCarPickNo;
    const float ped_speed = 0.06f;
    for (int f = 0; f < max_frames; f++)
    {
        r.frames = f;
        for (int i = 0; i < n; i++)
        {
            StepCar(&cars[i], ped_x, ped_y);
            const float dx = cars[i].x - ped_x;
            const float dy = cars[i].y - ped_y;
            if (cars[i].speed > 0.05f && fabsf(dx) < 0.5f && fabsf(dy) < 0.4f)
            {
                r.run_over = true;
            }
        }
        if (f % 15 == 0 || target < 0)
        {
            CarPickPlayer pp = {ped_x, ped_y, 0, false};
            float best = 1e9f;
            int best_i = -1;
            float bx = 0, by = 0;
            int bk = kCarPickNo;
            for (int i = 0; i < n; i++)
            {
                CarPickCar pc;
                pc.x = cars[i].x;
                pc.y = cars[i].y;
                pc.vx = cars[i].dir_x * cars[i].speed;
                pc.vy = 0.0f;
                pc.speed = cars[i].speed;
                pc.dist = sqrtf((pc.x - ped_x) * (pc.x - ped_x) + (pc.y - ped_y) * (pc.y - ped_y));
                pc.damage = 0;
                pc.has_driver = cars[i].has_driver;
                pc.locked = pc.police = pc.mission = pc.bad_model = false;
                float sx, sy;
                const int k = CarPick_Kind(pp, pc, 40.0f, &sx, &sy);
                if (k == kCarPickNo)
                {
                    continue;
                }
                const float wl = sqrtf((sx - ped_x) * (sx - ped_x) + (sy - ped_y) * (sy - ped_y));
                const float s2c = sqrtf((sx - pc.x) * (sx - pc.x) + (sy - pc.y) * (sy - pc.y));
                if (!CarPick_WalkOk(k, pc, wl, s2c))
                {
                    continue;
                }
                if (wl < best)
                {
                    best = wl;
                    best_i = i;
                    bx = sx;
                    by = sy;
                    bk = k;
                }
            }
            if (best_i >= 0 && (target < 0 || kind == kCarPickIntercept))
            {
                target = best_i;
                stand_x = bx;
                stand_y = by;
                kind = bk;
            }
        }
        if (target < 0)
        {
            continue;
        }
        const MockCar& c = cars[target];
        if (kind == kCarPickIntercept && c.speed > 0.03f)
        {
            // walk to the spot, then wait there
            const float dx = stand_x - ped_x;
            const float dy = stand_y - ped_y;
            const float d = sqrtf(dx * dx + dy * dy);
            if (d > 0.7f)
            {
                ped_x += dx / d * ped_speed;
                ped_y += dy / d * ped_speed;
            }
        }
        else
        {
            // stopped (or a parked one): walk up to it
            const float dx = c.x - ped_x;
            const float dy = c.y - ped_y;
            const float d = sqrtf(dx * dx + dy * dy);
            if (d < 1.0f && c.speed < 0.02f)
            {
                r.got_car = true;
                return r;
            }
            if (c.speed > 0.05f && kind != kCarPickIntercept)
            {
                kind = kCarPickIntercept; // it drove off while we walked: get ahead of it
                continue;
            }
            if (d > 0.01f)
            {
                ped_x += dx / d * ped_speed;
                ped_y += dy / d * ped_speed;
            }
        }
    }
    return r;
}

static void Case(const char* name, bool expect_car, float car_x, float dir, float cruise, float ped_y)
{
    MockCar cars[1] = {{car_x, 150.5f, dir, cruise, cruise, true}};
    const Result r = Play(cars, 1, 150.5f, ped_y, 900);
    const bool ok = r.got_car == expect_car && !r.run_over;
    printf("%-78s %s (%s in %d frames%s)\n", name, ok ? "PASS" : "FAIL", r.got_car ? "got the car" : "no car", r.frames, r.run_over ? ", RAN SOMEONE OVER" : "");
    if (!ok)
    {
        fails++;
    }
}

int main()
{
    // the bot on the pavement 3 tiles off the road (ped_y 153.5), the road along y=150.5, the car somewhere along it
    Case("a car coming from the west at 0.12, 20 tiles off", true, 130.0f, 1.0f, 0.12f, 153.5f);
    Case("a car coming from the east at 0.12, 20 tiles off", true, 171.0f, -1.0f, 0.12f, 153.5f);
    Case("a slow car at 0.06, 12 tiles off", true, 138.0f, 1.0f, 0.06f, 153.5f);
    Case("a car at 0.16, 30 tiles off", true, 120.0f, 1.0f, 0.16f, 153.5f);
    Case("a car at 0.12, 14 tiles off", true, 136.0f, 1.0f, 0.12f, 153.5f);
    Case("a car already going by, 2 tiles off at 0.2: nothing to be done (the next one)", false, 148.0f, 1.0f, 0.2f, 153.5f);
    Case("the bot on the other side of the road", true, 130.0f, 1.0f, 0.12f, 147.5f);
    Case("the bot 6 tiles off the road", true, 125.0f, 1.0f, 0.12f, 156.5f);

    // two cars in a line: the nearer is the one that is taken, the other queues behind it
    {
        MockCar cars[2] = {{128.0f, 150.5f, 1.0f, 0.12f, 0.12f, true}, {118.0f, 150.5f, 1.0f, 0.12f, 0.12f, true}};
        const Result r = Play(cars, 2, 150.5f, 153.5f, 900);
        const bool ok = r.got_car && !r.run_over;
        printf("%-78s %s (%d frames)\n", "two cars in a line, 22 and 32 tiles off", ok ? "PASS" : "FAIL", r.frames);
        if (!ok)
        {
            fails++;
        }
    }
    if (fails == 0)
    {
        printf("all passed\n");
    }
    return fails ? 1 : 0;
}
