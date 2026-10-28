// Native tests of turning a car round (Source/Bot/TurnAround.cpp) with a kinematic car: wheelbase 0.7, wheel 35 degrees, 0.08 tiles a
// frame forward and 0.05 backwards, in corridors of different widths and a dead end.
//   g++ -O1 -I../../../Source/Bot -o turn_around_sim turn_around_sim.cpp ../../../Source/Bot/TurnAround.cpp && ./turn_around_sim
#include "TurnAround.hpp"
#include <math.h>
#include <stdio.h>

static int fails = 0;
static void Check(const char* name, bool ok, const char* extra = "")
{
    printf("%-58s %s %s\n", name, ok ? "PASS" : "FAIL", extra);
    fails += ok ? 0 : 1;
}

struct World
{
    float half_width; // open for |y| < half_width
    float wall_x;     // a wall east of this x (dead end), 1e9 for none
};

static bool Solid(const World& w, float x, float y)
{
    return fabsf(y) >= w.half_width || x >= w.wall_x;
}

static const float kHl = 0.5f, kHw = 0.27f, kPi = 3.14159265f;

static bool BoxHits(const World& w, float x, float y, float h)
{
    const float c = cosf(h), s = sinf(h);
    for (int i = -1; i <= 1; i += 2)
    {
        for (int j = -1; j <= 1; j += 2)
        {
            if (Solid(w, x + c * kHl * i - s * kHw * j, y + s * kHl * i + c * kHw * j))
            {
                return true;
            }
        }
    }
    return false;
}

static float Clear(const World& w, float x, float y, float h, float dir)
{
    float best = 3.0f;
    for (int lat = -1; lat <= 1; lat++)
    {
        for (float d = 0.1f; d <= 3.0f; d += 0.1f)
        {
            const float px = x + cosf(h) * (kHl + d) * dir - sinf(h) * kHw * lat;
            const float py = y + sinf(h) * (kHl + d) * dir + cosf(h) * kHw * lat;
            if (Solid(w, px, py))
            {
                best = d < best ? d : best;
                break;
            }
        }
    }
    return best;
}

// Heads east (0 rad), the way to go is west; returns the frames to face within the release angle, -1 if never or it hit a wall
static int Run(const World& w, float x, float y, float h, float aim_deg, int* hit_frame)
{
    TurnState st;
    TurnAround_Reset(&st);
    float v = 0.0f;
    *hit_frame = -1;
    for (int f = 0; f < 900; f++)
    {
        float err = aim_deg - h * 180.0f / kPi;
        while (err > 180.0f) err -= 360.0f;
        while (err < -180.0f) err += 360.0f;
        const TurnIn in = {err, Clear(w, x, y, h, 1.0f), Clear(w, x, y, h, -1.0f)};
        const TurnOut o = TurnAround_Step(&st, in);
        if (!o.active && fabsf(err) < kTurnReleaseDeg)
        {
            return f;
        }
        float steer = o.right ? 1.0f : -1.0f;
        v = o.gas > 0 ? 0.08f : -0.05f;
        if (!o.active)
        {
            steer = 0.0f;
            v = 0.08f;
        }
        h += v / 0.7f * tanf(35.0f * kPi / 180.0f) * steer;
        x += cosf(h) * v;
        y += sinf(h) * v;
        if (BoxHits(w, x, y, h))
        {
            *hit_frame = f;
            return -1;
        }
    }
    return -1;
}

int main()
{
    int hit;
    {
        World w = {3.0f, 1e9f};
        const int f = Run(w, 0, 0, 0.0f, 180.0f, &hit);
        char b[40];
        snprintf(b, sizeof(b), "(%d frames)", f);
        Check("turn round in a 6 tile wide street", f >= 0, b);
    }
    {
        World w = {2.0f, 1e9f};
        const int f = Run(w, 0, 0, 0.0f, 180.0f, &hit);
        char b[40];
        snprintf(b, sizeof(b), "(%d frames, hit at %d)", f, hit);
        Check("turn round in a 4 tile wide street, no wall touched", f >= 0 && hit < 0, b);
    }
    {
        World w = {2.0f, 1.2f};
        const int f = Run(w, 0, 0, 0.0f, 180.0f, &hit);
        char b[40];
        snprintf(b, sizeof(b), "(%d frames, hit at %d)", f, hit);
        Check("nose to a wall in a 4 tile street: backs up first", f >= 0 && hit < 0, b);
    }
    {
        World w = {2.0f, 1e9f};
        const int f = Run(w, 0, 1.0f, 0.0f, -150.0f, &hit); // up to the north kerb, the way out is south-west
        char b[40];
        snprintf(b, sizeof(b), "(%d frames, hit at %d)", f, hit);
        Check("along the kerb, the way out is behind and to one side", f >= 0 && hit < 0, b);
    }
    {
        TurnState st;
        TurnAround_Reset(&st);
        const TurnIn in = {30.0f, 3.0f, 3.0f};
        Check("facing it well enough: no turning round", !TurnAround_Step(&st, in).active);
    }
    {
        TurnState st;
        TurnAround_Reset(&st);
        const TurnIn in = {170.0f, 0.2f, 0.2f};
        bool spun = false;
        bool let_go = false;
        for (int i = 0; i < 140; i++)
        {
            const TurnOut o = TurnAround_Step(&st, in);
            spun = spun || o.spin;
            let_go = let_go || (i > kTurnSpinAfter + 2 && o.active && !o.spin);
        }
        Check("boxed in both ways: spins on the spot, letting go between", spun && let_go);
    }
    printf("%s\n", fails ? "FAILED" : "all passed");
    return fails ? 1 : 0;
}
