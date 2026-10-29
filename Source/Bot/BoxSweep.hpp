#pragma once

#include <math.h>

// The box a moving thing covers over the next few frames: its collision box now and where it will be after `frames` more, as one
// oriented box on the same axis. The centre moves half the way and each half size grows by half the travel along that side, so it holds both
// boxes (and a little more when the thing slides sideways). Pure, tested natively in Scripts/bot/sim/box_sweep_sim.cpp.
enum
{
    kPredictFrames = 3
};

inline void BoxSweep(float cx, float cy, float ax, float ay, float hl, float hw, float vx, float vy, int frames, float* out_cx, float* out_cy,
                     float* out_hl, float* out_hw)
{
    const float tx = vx * static_cast<float>(frames);
    const float ty = vy * static_cast<float>(frames);
    const float along = tx * ax + ty * ay;
    const float across = tx * -ay + ty * ax;
    *out_cx = cx + tx * 0.5f;
    *out_cy = cy + ty * 0.5f;
    *out_hl = hl + fabsf(along) * 0.5f;
    *out_hw = hw + fabsf(across) * 0.5f;
}
