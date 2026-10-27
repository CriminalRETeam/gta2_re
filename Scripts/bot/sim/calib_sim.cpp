// Native tests of the steering calibration (Source/Bot/CalibrateCore.cpp) against fake peds and cars.
//   g++ -O1 -I../../../Source/Bot -o calib_sim calib_sim.cpp ../../../Source/Bot/CalibrateCore.cpp && ./calib_sim
#include "CalibrateCore.hpp"
#include <math.h>
#include <stdio.h>
#include <string.h>

// A fake body: heading = sign * rot + offset; Right turns the rotation by right_dir degrees a frame; it only responds
// from frozen_until on (the first frames of a run, a ped held by something)
struct Body
{
    float x, y, rot;
    float sign, offset, right_dir, speed;
    int frozen_until;
    void Step(int frame, unsigned input)
    {
        if (frame < frozen_until)
        {
            return;
        }
        if (input & 0x8)
        {
            rot += right_dir;
        }
        if (input & 0x4)
        {
            rot -= right_dir;
        }
        while (rot > 180.0f) rot -= 360.0f;
        while (rot < -180.0f) rot += 360.0f;
        if (input & 0x1)
        {
            const float h = (sign * rot + offset) * 3.14159265f / 180.0f;
            x += sinf(h) * speed;
            y -= cosf(h) * speed;
        }
    }
};

struct Outcome
{
    CalibrateCore::Result result;
    int frames;
    CalSteering st;
    char fail[160];
};

static Outcome Calibrate(Body b, bool in_car)
{
    CalibrateCore core;
    core.Start(in_car);
    Outcome o;
    memset(&o, 0, sizeof(o));
    for (int f = 0; f < 3000; f++)
    {
        unsigned input = 0;
        o.result = core.Step(f, b.x, b.y, b.rot, &input, &o.st, o.fail, sizeof(o.fail));
        o.frames = f;
        if (o.result != CalibrateCore::Running)
        {
            break;
        }
        b.Step(f, input);
    }
    return o;
}

static int fails = 0;

static void Check(const char* name, bool ok, const Outcome& o)
{
    printf("%-46s %s  (%d frames%s%s)\n", name, ok ? "PASS" : "FAIL", o.frames, o.fail[0] ? ", " : "", o.fail);
    fails += ok ? 0 : 1;
}

// the heading the calibrated model gives for a rotation must match the body's
static bool Model(const Outcome& o, const Body& b)
{
    if (o.result != CalibrateCore::Done || !o.st.valid)
    {
        return false;
    }
    for (float rot = -170.0f; rot <= 170.0f; rot += 40.0f)
    {
        float want = b.sign * rot + b.offset;
        float got = o.st.sign * rot + o.st.offset;
        float d = fmodf(want - got + 540.0f, 360.0f) - 180.0f;
        if (fabsf(d) > 1.0f)
        {
            return false;
        }
    }
    return true;
}

int main()
{
    {
        Body b = {50, 50, 180, -1, 180, -3.0f, 0.07f, 0};
        Outcome o = Calibrate(b, false);
        Check("ped: rotation runs against the heading", Model(o, b) && o.st.turn_sign < 0, o);
    }
    {
        Body b = {50, 50, 30, 1, 0, 3.0f, 0.07f, 0};
        Outcome o = Calibrate(b, false);
        Check("ped: rotation runs with the heading", Model(o, b) && o.st.turn_sign > 0, o);
    }
    {
        Body b = {50, 50, 180, -1, 180, -2.0f, 0.12f, 0};
        Outcome o = Calibrate(b, true);
        Check("car: quicker, slower turning", Model(o, b), o);
    }
    {
        // the case from the watch run: the ped does not answer for the first 200 frames
        Body b = {50, 50, 180, -1, 180, -3.0f, 0.07f, 200};
        Outcome o = Calibrate(b, false);
        Check("ped frozen for 200 frames: waits, then calibrates", Model(o, b), o);
    }
    {
        Body b = {50, 50, 180, -1, 180, -3.0f, 0.07f, 100000};
        Outcome o = Calibrate(b, false);
        Check("ped that never answers: fails after the retries", o.result == CalibrateCore::Failed && o.frames < 800, o);
    }
    {
        Body b = {50, 50, 180, -1, 180, 0.0f, 0.07f, 0};
        Outcome o = Calibrate(b, false);
        Check("ped that cannot turn: fails, does not guess", o.result == CalibrateCore::Failed, o);
    }
    printf("%d failed\n", fails);
    return fails ? 1 : 0;
}
